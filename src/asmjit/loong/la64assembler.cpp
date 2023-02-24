// AsmJit - Machine code generation for C++
//
//  * Official AsmJit Home Page: https://asmjit.com
//  * Official Github Repository: https://github.com/asmjit/asmjit
//
// Copyright (c) 2008-2020 The AsmJit Authors
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include <assert.h>
#include "../core/api-build_p.h"
#if !defined(ASMJIT_NO_LOONG)

#include "../core/codewriter_p.h"
#include "../core/cpuinfo.h"
#include "../core/emitterutils_p.h"
#include "../core/formatter.h"
#include "../core/logger.h"
#include "../core/misc_p.h"
#include "../core/support.h"
#include "../loong/loongformatter_p.h"
#include "../loong/la64assembler.h"
#include "../loong/la64instdb_p.h"
#include "../loong/la64utils.h"

ASMJIT_BEGIN_SUB_NAMESPACE(la64)
// ============================================================================
// [asmjit::la64::Bits]
// ============================================================================

static constexpr inline uint32_t B(uint32_t index) noexcept { return 1u << index; }
template<typename T>
static inline uint32_t shiftImm(T value, uint32_t rbitIndex, uint32_t lbitIndex) noexcept {
	uint32_t lvalue = (uint32_t(value) & 0xFFFF);
	return ((uint32_t(value) >> rbitIndex) | (uint32_t(lvalue) << lbitIndex));
}

static constexpr uint32_t kSP = Gp::kIdSp;
static constexpr uint32_t kZR = Gp::kIdZr;
static constexpr uint32_t kWX = InstDB::kWX;

// ============================================================================
// [asmjit::a64::Assembler - ShiftOpToLdStOptMap]
// ============================================================================

// Table that maps ShiftOp to OPT part in LD/ST (register) opcode.
#define VALUE(x) x == loongShift::kOpUXTW ? 2u : \
                 x == loongShift::kOpLSL  ? 3u : \
                 x == loongShift::kOpSXTW ? 6u : \
                 x == loongShift::kOpSXTX ? 7u : 0xFF
static const uint8_t loongShiftOpToLdStOptMap[] = { ASMJIT_LOOKUP_TABLE_16(VALUE, 0) };
#undef VALUE

// ============================================================================
// [asmjit::a64::Assembler - SizeOp]
// ============================================================================

//! Struct that contains Size (2 bits), Q flag, and S (scalar) flag. These values
//! are used to encode Q, Size, and Scalar fields in an opcode.
struct SizeOp {
  enum : uint8_t {
    k128BitShift = 0,
    kScalarShift = 1,
    kSizeShift = 2,

    kQ = uint8_t(1u << k128BitShift),
    kS = uint8_t(1u << kScalarShift),

    k00 = uint8_t(0 << kSizeShift),
    k01 = uint8_t(1 << kSizeShift),
    k10 = uint8_t(2 << kSizeShift),
    k11 = uint8_t(3 << kSizeShift),

    k00Q = k00 | kQ,
    k01Q = k01 | kQ,
    k10Q = k10 | kQ,
    k11Q = k11 | kQ,

    k00S = k00 | kS,
    k01S = k01 | kS,
    k10S = k10 | kS,
    k11S = k11 | kS,

    kInvalid = 0xFFu,

    // Masks used by SizeOpMap.
    kSzQ = (0x3u << kSizeShift) | kQ,
    kSzS = (0x3u << kSizeShift) | kS,
    kSzQS = (0x3u << kSizeShift) | kQ | kS
  };

  uint8_t value;

  inline bool isValid() const noexcept { return value != kInvalid; }
  inline void makeInvalid() noexcept { value = kInvalid; }

  inline uint32_t q() const noexcept { return (value >> k128BitShift) & 0x1u; }
  inline uint32_t qs() const noexcept { return ((value >> k128BitShift) | (value >> kScalarShift)) & 0x1u; }
  inline uint32_t scalar() const noexcept { return (value >> kScalarShift) & 0x1u; }
  inline uint32_t size() const noexcept { return (value >> kSizeShift) & 0x3u; }

  inline void decrementSize() noexcept {
    ASMJIT_ASSERT(size() > 0);
    value = uint8_t(value - (1u << kSizeShift));
  }
};

struct SizeOpTable {
  enum TableId : uint8_t {
    kTableBin = 0,
    kTableAny,
    kCount
  };

  // 40 elements for each combination.
  SizeOp array[(Reg::kTypeVecV - Reg::kTypeVecB + 1) * 8];
};

#define VALUE_BIN(x) { \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeNone)) ? SizeOp::k00  : \
  x == ((uint32_t(Reg::kTypeVecV - Reg::kTypeVecB) << 3) | (Vec::kElementTypeNone)) ? SizeOp::k00Q : \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeB   )) ? SizeOp::k00  : \
  x == ((uint32_t(Reg::kTypeVecV - Reg::kTypeVecB) << 3) | (Vec::kElementTypeB   )) ? SizeOp::k00Q : SizeOp::kInvalid \
}

#define VALUE_ANY(x) { \
  x == ((uint32_t(Reg::kTypeVecB - Reg::kTypeVecB) << 3) | (Vec::kElementTypeNone)) ? SizeOp::k00S : \
  x == ((uint32_t(Reg::kTypeVecH - Reg::kTypeVecB) << 3) | (Vec::kElementTypeNone)) ? SizeOp::k01S : \
  x == ((uint32_t(Reg::kTypeVecS - Reg::kTypeVecB) << 3) | (Vec::kElementTypeNone)) ? SizeOp::k10S : \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeNone)) ? SizeOp::k11S : \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeB   )) ? SizeOp::k00  : \
  x == ((uint32_t(Reg::kTypeVecV - Reg::kTypeVecB) << 3) | (Vec::kElementTypeB   )) ? SizeOp::k00Q : \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeH   )) ? SizeOp::k01  : \
  x == ((uint32_t(Reg::kTypeVecV - Reg::kTypeVecB) << 3) | (Vec::kElementTypeH   )) ? SizeOp::k01Q : \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeS   )) ? SizeOp::k10  : \
  x == ((uint32_t(Reg::kTypeVecV - Reg::kTypeVecB) << 3) | (Vec::kElementTypeS   )) ? SizeOp::k10Q : \
  x == ((uint32_t(Reg::kTypeVecD - Reg::kTypeVecB) << 3) | (Vec::kElementTypeD   )) ? SizeOp::k11S : \
  x == ((uint32_t(Reg::kTypeVecV - Reg::kTypeVecB) << 3) | (Vec::kElementTypeD   )) ? SizeOp::k11Q : SizeOp::kInvalid \
}

static const SizeOpTable sizeOpTable[SizeOpTable::kCount] = {
  {{ ASMJIT_LOOKUP_TABLE_40(VALUE_BIN, 0) }},
  {{ ASMJIT_LOOKUP_TABLE_40(VALUE_ANY, 0) }}
};

#undef VALUE_ANY
#undef VALUE_BIN

struct SizeOpMap {
  uint8_t tableId;
  uint8_t sizeOpMask;
  uint16_t acceptMask;
};

static const constexpr SizeOpMap sizeOpMap[InstDB::kVO_Count] = {
  { // kVO_V_B:
    SizeOpTable::kTableBin, SizeOp::kQ   , uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q))
  },

  { // kVO_V_BH:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k01) | B(SizeOp::k01Q))
  },

  { // kVO_V_BH_4S:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k10Q))
  },

  { // kVO_V_BHS:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k10) | B(SizeOp::k10Q))
  },

  { // kVO_V_BHS_D2:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k10) | B(SizeOp::k10Q) | B(SizeOp::k11Q))
  },

  { // kVO_V_HS:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k10) | B(SizeOp::k10Q))
  },

  { // kVO_V_S:
    SizeOpTable::kTableAny, SizeOp::kQ   , uint16_t(B(SizeOp::k10) | B(SizeOp::k10Q))
  },

  { // kVO_V_B8H4:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k01))
  },

  { // kVO_V_B8H4S2:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k01) | B(SizeOp::k10))
  },

  { // kVO_V_B8D1:
    SizeOpTable::kTableAny, SizeOp::kSzQ , uint16_t(B(SizeOp::k00) | B(SizeOp::k11S))
  },

  { // kVO_V_H4S2:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k01) | B(SizeOp::k10))
  },

  { // kVO_V_B16:
    SizeOpTable::kTableBin, SizeOp::kQ   , uint16_t(B(SizeOp::k00Q))
  },

  { // kVO_V_B16H8:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00Q) | B(SizeOp::k01Q))
  },

  { // kVO_V_B16H8S4:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00Q) | B(SizeOp::k01Q) | B(SizeOp::k10Q))
  },

  { // kVO_V_B16D2:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00Q) | B(SizeOp::k11Q))
  },

  { // kVO_V_H8S4:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k01Q) | B(SizeOp::k10Q))
  },

  { // kVO_V_S4:
    SizeOpTable::kTableAny, 0            , uint16_t(B(SizeOp::k10Q))
  },

  { // kVO_V_D2:
    SizeOpTable::kTableAny, 0            , uint16_t(B(SizeOp::k11Q))
  },

  { // kVO_SV_BHS:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k00S) | B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k01S) | B(SizeOp::k10) | B(SizeOp::k10Q) | B(SizeOp::k10S))
  },

  { // kVO_SV_B8H4S2:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00S) | B(SizeOp::k01) | B(SizeOp::k01S) | B(SizeOp::k10) | B(SizeOp::k10S))
  },

  { // kVO_SV_HS:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k01S) | B(SizeOp::k10) | B(SizeOp::k10Q) | B(SizeOp::k10S))
  },

  { // kVO_V_Any:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k10) | B(SizeOp::k10Q) | B(SizeOp::k11S) | B(SizeOp::k11Q))
  },

  { // kVO_SV_Any:
    SizeOpTable::kTableAny, SizeOp::kSzQS, uint16_t(B(SizeOp::k00) | B(SizeOp::k00Q) | B(SizeOp::k00S) |
                                                    B(SizeOp::k01) | B(SizeOp::k01Q) | B(SizeOp::k01S) |
                                                    B(SizeOp::k10) | B(SizeOp::k10Q) | B(SizeOp::k10S) |
                                                    B(SizeOp::k11) | B(SizeOp::k11Q) | B(SizeOp::k11S))
  }
};
/*
static const Operand_& significantSimdOp(const Operand_& o0, const Operand_& o1, uint32_t instFlags) noexcept {
  return !(instFlags & InstDB::kInstFlagLong) ? o0 : o1;
}
*/
static ASMJIT_INLINE SizeOp armElementTypeToSizeOp(uint32_t vecOpType, uint32_t regType, uint32_t elementType) noexcept {
  // Instruction data or Assembler is wrong if this triggers an assertion failure.
  ASMJIT_ASSERT(vecOpType < InstDB::kVO_Count);
  // ElementType uses 3 bits in the operand signature, it should never overflow.
  ASMJIT_ASSERT(elementType <= 0x7u);

  const SizeOpMap& map = sizeOpMap[vecOpType];
  const SizeOpTable& table = sizeOpTable[map.tableId];

  size_t index = (Support::min<uint32_t>(regType - Vec::kTypeVecB, Vec::kTypeVecV - Vec::kTypeVecB + 1) << 3) | elementType;
  SizeOp op = table.array[index];
  SizeOp modifiedOp { uint8_t(op.value & map.sizeOpMask) };

  if (!Support::bitTest(map.acceptMask, op.value))
    modifiedOp.makeInvalid();

  return modifiedOp;
}

// ============================================================================
// [asmjit::a64::Assembler - Immediate Encoding Utilities (Integral)]
// ============================================================================

using Utils::LogicalImm;

struct HalfWordImm {
  uint32_t hw;
  uint32_t inv;
  uint32_t imm;
};

struct LMHImm {
  uint32_t lm;
  uint32_t h;
  uint32_t maxRmId;
};

static inline uint32_t countZeroHalfWords64(uint64_t imm) noexcept {
  return uint32_t((imm & 0x000000000000FFFFu) == 0) +
         uint32_t((imm & 0x00000000FFFF0000u) == 0) +
         uint32_t((imm & 0x0000FFFF00000000u) == 0) +
         uint32_t((imm & 0xFFFF000000000000u) == 0) ;
}

/*
static uint32_t encodeMovSequence32(uint32_t out[2], uint32_t imm, uint32_t rd, uint32_t x) noexcept {
  ASMJIT_ASSERT(rd <= 31);

  uint32_t kMovZ = 0b01010010100000000000000000000000 | (x << 31);
  uint32_t kMovN = 0b00010010100000000000000000000000;
  uint32_t kMovK = 0b01110010100000000000000000000000;

  if ((imm & 0xFFFF0000u) == 0x00000000u) {
    out[0] = kMovZ | (0 << 21) | ((imm & 0xFFFFu) << 5) | rd;
    return 1;
  }

  if ((imm & 0xFFFF0000u) == 0xFFFF0000u) {
    out[0] = kMovN | (0 << 21) | ((~imm & 0xFFFFu) << 5) | rd;
    return 1;
  }

  if ((imm & 0x0000FFFFu) == 0x00000000u) {
    out[0] = kMovZ | (1 << 21) | ((imm >> 16) << 5) | rd;
    return 1;
  }

  if ((imm & 0x0000FFFFu) == 0x0000FFFFu) {
    out[0] = kMovN | (1 << 21) | ((~imm >> 16) << 5) | rd;
    return 1;
  }

  out[0] = kMovZ | (0 << 21) | ((imm & 0xFFFFu) << 5) | rd;
  out[1] = kMovK | (1 << 21) | ((imm     >> 16) << 5) | rd;
  return 2;
}

static uint32_t encodeMovSequence64(uint32_t out[4], uint64_t imm, uint32_t rd, uint32_t x) noexcept {
  ASMJIT_ASSERT(rd <= 31);

  uint32_t kMovZ = 0b11010010100000000000000000000000;
  uint32_t kMovN = 0b10010010100000000000000000000000;
  uint32_t kMovK = 0b11110010100000000000000000000000;

  if (imm <= 0xFFFFFFFFu)
    return encodeMovSequence32(out, uint32_t(imm), rd, x);

  uint32_t zhw = countZeroHalfWords64( imm);
  uint32_t ohw = countZeroHalfWords64(~imm);

  if (zhw >= ohw) {
    uint32_t op = kMovZ;
    uint32_t count = 0;

    for (uint32_t hwIndex = 0; hwIndex < 4; hwIndex++, imm >>= 16) {
      uint32_t hwImm = uint32_t(imm & 0xFFFFu);
      if (hwImm == 0)
        continue;

      out[count++] = op | (hwIndex << 21) | (hwImm << 5) | rd;
      op = kMovK;
    }

    // This should not happen - zero should be handled by encodeMovSequence32().
    ASMJIT_ASSERT(count > 0);

    return count;
  }
  else {
    uint32_t op = kMovN;
    uint32_t count = 0;
    uint32_t negMask = 0xFFFFu;

    for (uint32_t hwIndex = 0; hwIndex < 4; hwIndex++, imm >>= 16) {
      uint32_t hwImm = uint32_t(imm & 0xFFFFu);
      if (hwImm == 0xFFFFu)
        continue;

      out[count++] = op | (hwIndex << 21) | ((hwImm ^ negMask) << 5) | rd;
      op = kMovK;
      negMask = 0;
    }

    if (count == 0) {
      out[count++] = kMovN | ((0xFFFF ^ negMask) << 5) | rd;
    }

    return count;
  }
}
*/
static ASMJIT_INLINE bool encodeLMH(uint32_t sizeField, uint32_t elementIndex, LMHImm* out) noexcept {
  if (sizeField != 1 && sizeField != 2)
    return false;

  uint32_t hShift = 3u - sizeField;
  uint32_t lmShift = sizeField - 1u;
  uint32_t maxElementIndex = 15u >> sizeField;

  out->h = elementIndex >> hShift;
  out->lm = (elementIndex << lmShift) & 0x3u;
  out->maxRmId = (8u << sizeField) - 1;

  return elementIndex <= maxElementIndex;
}

// [.......A|B.......|.......C|D.......|.......E|F.......|.......G|H.......]
static ASMJIT_INLINE uint32_t encodeImm64ByteMaskToImm8(uint64_t imm) noexcept {
  return uint32_t(((imm >> (7  - 0)) & 0b00000011) | // [.......G|H.......]
                  ((imm >> (23 - 2)) & 0b00001100) | // [.......E|F.......]
                  ((imm >> (39 - 4)) & 0b00110000) | // [.......C|D.......]
                  ((imm >> (55 - 6)) & 0b11000000)); // [.......A|B.......]
}

// ============================================================================
// [asmjit::a64::Opcode]
// ============================================================================

//! Helper class to store and manipulate ARM opcode.
struct Opcode {
  uint32_t v;

  enum Bits : uint32_t {
    kN = (1u << 22),
    kQ = (1u << 30),
    kX = (1u << 31)
  };

  // --------------------------------------------------------------------------
  // [Opcode Builder]
  // --------------------------------------------------------------------------

  ASMJIT_INLINE uint32_t get() const noexcept { return v; }
  ASMJIT_INLINE void reset(uint32_t value) noexcept { v = value; }

  ASMJIT_INLINE bool hasQ() const noexcept { return (v & kQ) != 0; }
  ASMJIT_INLINE bool hasX() const noexcept { return (v & kX) != 0; }

  template<typename T>
  ASMJIT_INLINE Opcode& addImm(T value, uint32_t bitIndex) noexcept { return operator|=(uint32_t(value) << bitIndex); }

  template<typename T>
  ASMJIT_INLINE Opcode& xorImm(T value, uint32_t bitIndex) noexcept { return operator^=(uint32_t(value) << bitIndex); }
  //template<typename T>
  ASMJIT_INLINE Opcode& shiftopR(uint32_t bitIndex) noexcept { return operator>>=(bitIndex); }
  ASMJIT_INLINE Opcode& shiftopL(uint32_t bitIndex) noexcept { return operator<<=(bitIndex); }
  template<typename T, typename Condition>
  ASMJIT_INLINE Opcode& addIf(T value, const Condition& condition) noexcept { return operator|=(condition ? uint32_t(value) : uint32_t(0)); }

  ASMJIT_INLINE Opcode& addLogicalImm(const LogicalImm& logicalImm) noexcept {
    addImm(logicalImm.n, 22);
    addImm(logicalImm.r, 16);
    addImm(logicalImm.s, 10);
    return *this;
  }

  //template<typename T>
  //ASMJIT_INLINE Opcode& addshift(uint32_t bitIndex) noexcept { return operator=(v << bitIndex); }

  ASMJIT_INLINE Opcode& addReg(uint32_t id, uint32_t bitIndex) noexcept { return operator|=((id & 31u) << bitIndex); }
  ASMJIT_INLINE Opcode& addReg(const Operand_& op, uint32_t bitIndex) noexcept { return addReg(op.id(), bitIndex); }

  ASMJIT_INLINE Opcode& operator=(uint32_t x) noexcept { v = x; return *this; }
  ASMJIT_INLINE Opcode& operator&=(uint32_t x) noexcept { v &= x; return *this; }
  ASMJIT_INLINE Opcode& operator>>=(uint32_t x) noexcept { v >>= x; return *this; }
  ASMJIT_INLINE Opcode& operator<<=(uint32_t x) noexcept { v <<= x; return *this; }
  ASMJIT_INLINE Opcode& operator|=(uint32_t x) noexcept { v |= x; return *this; }
  ASMJIT_INLINE Opcode& operator^=(uint32_t x) noexcept { v ^= x; return *this; }

  ASMJIT_INLINE uint32_t operator&(uint32_t x) const noexcept { return v & x; }
  ASMJIT_INLINE uint32_t operator|(uint32_t x) const noexcept { return v | x; }
  ASMJIT_INLINE uint32_t operator^(uint32_t x) const noexcept { return v ^ x; }
};

// ============================================================================
// [asmjit::a64::Assembler - Signature Utilities]
// ============================================================================

// TODO: [ARM] Deprecate matchSignature.
static inline bool matchSignature(const Operand_& o0, const Operand_& o1, uint32_t instFlags) noexcept {
  if (!(instFlags & (InstDB::kInstFlagLong | InstDB::kInstFlagNarrow)))
    return o0.signature() == o1.signature();

  // TODO: [ARM] Something smart to validate this.
  return true;
}

static inline bool matchSignature(const Operand_& o0, const Operand_& o1, const Operand_& o2, uint32_t instFlags) noexcept {
  return matchSignature(o0, o1, instFlags) && o1.signature() == o2.signature();
}

static inline bool matchSignature(const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_& o3, uint32_t instFlags) noexcept {
  return matchSignature(o0, o1, instFlags) && o1.signature() == o2.signature() && o2.signature() == o3.signature();;
}

// Memory must be either:
// 1. Absolute address, which will be converted to relative.
// 2. Relative displacement (Label).
// 3. Base register + either offset or index.
static ASMJIT_INLINE bool armCheckMemBaseIndexRel(const Mem& mem) noexcept {
  // Allowed base types (Nothing, Label, and GpX).
  constexpr uint32_t kBaseMask  = B(0) |
                                  B(Operand::kLabelTag) |
                                  B(Reg::kTypeGpX);

  // Allowed index types (Nothing, GpW, and GpX).
  constexpr uint32_t kIndexMask = B(0) |
                                  B(Reg::kTypeGpW) |
                                  B(Reg::kTypeGpX) ;

  uint32_t baseType = mem.baseType();
  uint32_t indexType = mem.indexType();

  if (!((1u << baseType) & kBaseMask))
    return false;

  if (baseType > Operand::kLabelTag) {
    // Index allows either GpW or GpX.
    if (!((1u << indexType) & kIndexMask))
      return false;

    if (indexType == 0)
      return true;
    else
      return !mem.hasOffset();
  }
  else {
    // No index register allowed if this is a PC relative address (literal).
    return indexType == 0;
  }
}

struct EncodeFpOpcodeBits {
  uint32_t sizeMask;
  uint32_t mask[3];
};

static ASMJIT_INLINE bool pickFpOpcode(const Vec& reg, uint32_t sOp, uint32_t sHf, uint32_t vOp, uint32_t vHf, Opcode* opcode, uint32_t* szOut) noexcept {
  static constexpr uint32_t kQBitIndex = 30;

  static const EncodeFpOpcodeBits szBits[InstDB::kHF_Count] = {
    { B(2) | B(1)       , { 0u                           , 0u, B(22) } },
    { B(2) | B(1) | B(0), { 0u                           , 0u, 0u    } },
    { B(2) | B(1) | B(0), { B(23) | B(22)                , 0u, B(22) } },
    { B(2) | B(1) | B(0), { B(22) | B(20) | B(19)        , 0u, B(22) } },
    { B(2) | B(1) | B(0), { B(22) | B(21) | B(15) | B(14), 0u, B(22) } },
    { B(2) | B(1) | B(0), { B(23)                        , 0u, B(22) } }
  };

  if (!reg.hasElementType()) {
    // Scalar operation [HSD].
    uint32_t sz = reg.type() - Reg::kTypeVecH;
    if (sz > 2u || !Support::bitTest(szBits[sHf].sizeMask, sz))
      return false;

    opcode->reset(szBits[sHf].mask[sz] ^ sOp);
    *szOut = sz;
    return sOp != 0;
  }
  else {
    // Vector operation [HSD].
    uint32_t q = reg.type() - Reg::kTypeVecD;
    uint32_t sz = reg.elementType() - Vec::kElementTypeH;

    if (q > 1u || sz > 2u || !Support::bitTest(szBits[vHf].sizeMask, sz))
      return false;

    opcode->reset(szBits[vHf].mask[sz] ^ (vOp | (q << kQBitIndex)));
    *szOut = sz;
    return vOp != 0;
  }
}

static ASMJIT_INLINE bool pickFpOpcode(const Vec& reg, uint32_t sOp, uint32_t sHf, uint32_t vOp, uint32_t vHf, Opcode* opcode) noexcept {
  uint32_t sz;
  return pickFpOpcode(reg, sOp, sHf, vOp, vHf, opcode, &sz);
}

// ============================================================================
// [asmjit::a64::Assembler - Operand Checks]
// ============================================================================

// Checks whether all operands have the same signature.
static ASMJIT_INLINE bool checkSignature(const Operand_& o0, const Operand_& o1) noexcept {
  return o0.signature() == o1.signature();
}

static ASMJIT_INLINE bool checkSignature(const Operand_& o0, const Operand_& o1, const Operand_& o2) noexcept {
  return o0.signature() == o1.signature() &&
         o1.signature() == o2.signature();
}

static ASMJIT_INLINE bool checkSignature(const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_& o3) noexcept {
  return o0.signature() == o1.signature() &&
         o1.signature() == o2.signature() &&
         o2.signature() == o3.signature();
}

// Checks whether the register is GP register of the allowed types.
//
// Allowed is a 2-bit mask, where the first bits allows GpW and the second bit
// allows GpX. These bits are usually stored within the instruction, but could
// be also hardcoded in the assembler for instructions where GP types are not
// selectable.
static ASMJIT_INLINE bool checkGpType(const Operand_& op, uint32_t allowed) noexcept {
  uint32_t type = op.as<Reg>().type();
  return Support::bitTest(allowed << Reg::kTypeGpW, type);
}

static ASMJIT_INLINE bool checkGpType(const Operand_& op, uint32_t allowed, uint32_t* x) noexcept {
  // NOTE: We set 'x' to one only when GpW is allowed, otherwise the X is part
  // of the opcode and we cannot set it. This is why this works without requiring
  // additional logic.
  uint32_t type = op.as<Reg>().type();
  *x = (type - Reg::kTypeGpW) & allowed;
  return Support::bitTest(allowed << Reg::kTypeGpW, type);
}

static ASMJIT_INLINE bool checkGpType(const Operand_& o0, const Operand_& o1, uint32_t allowed, uint32_t* x) noexcept {
  return checkGpType(o0, allowed, x) && checkSignature(o0, o1);
}

static ASMJIT_INLINE bool checkGpType(const Operand_& o0, const Operand_& o1, const Operand_& o2, uint32_t allowed, uint32_t* x) noexcept {
  return checkGpType(o0, allowed, x) && checkSignature(o0, o1, o2);
}

static ASMJIT_INLINE bool checkGpId(const Operand_& op, uint32_t hiId = kZR) noexcept {
  uint32_t id = op.as<Reg>().id();
  return id <= 31u || id == hiId;
}

static ASMJIT_INLINE bool checkGpId(const Operand_& o0, const Operand_& o1, uint32_t hiId = kZR) noexcept {
  uint32_t id0 = o0.as<Reg>().id();
  uint32_t id1 = o1.as<Reg>().id();

  return (id0 <= 31u || id0 == hiId) && (id1 <= 31u || id1 == hiId);
}

static ASMJIT_INLINE bool checkGpId(const Operand_& o0, const Operand_& o1, const Operand_& o2, uint32_t hiId = kZR) noexcept {
  uint32_t id0 = o0.as<Reg>().id();
  uint32_t id1 = o1.as<Reg>().id();
  uint32_t id2 = o2.as<Reg>().id();

  return (id0 <= 31u || id0 == hiId) && (id1 <= 31u || id1 == hiId) && (id2 <= 31u || id2 == hiId);
}

static ASMJIT_INLINE bool checkVecId(const Operand_& op) noexcept {
  uint32_t id = op.as<Reg>().id();
  return id <= 31u;
}

static ASMJIT_INLINE bool checkVecId(const Operand_& o0, const Operand_& o1) noexcept {
  uint32_t id0 = o0.as<Reg>().id();
  uint32_t id1 = o1.as<Reg>().id();

  return (id0 | id1) <= 31u;
}

static ASMJIT_INLINE bool checkVecId(const Operand_& o0, const Operand_& o1, const Operand_& o2) noexcept {
  uint32_t id0 = o0.as<Reg>().id();
  uint32_t id1 = o1.as<Reg>().id();
  uint32_t id2 = o2.as<Reg>().id();

  return (id0 | id1 | id2) <= 31u;
}

static ASMJIT_INLINE bool checkVecId(const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_& o3) noexcept {
  uint32_t id0 = o0.as<Reg>().id();
  uint32_t id1 = o1.as<Reg>().id();
  uint32_t id2 = o2.as<Reg>().id();
  uint32_t id3 = o3.as<Reg>().id();

  return (id0 | id1 | id2 | id3) <= 31u;
}

static ASMJIT_INLINE bool checkMemBase(const Mem& mem) noexcept {
  return mem.baseType() == Reg::kTypeGpX && mem.baseId() <= 31;
}

static ASMJIT_INLINE bool checkEven(const Operand_& o0, const Operand_& o1) noexcept {
  return ((o0.id() | o1.id()) & 1) == 0;
}

static ASMJIT_INLINE bool checkConsecutive(const Operand_& o0, const Operand_& o1) noexcept {
  return ((o0.id() + 1u) & 0x1Fu) == o1.id();
}

static ASMJIT_INLINE bool checkConsecutive(const Operand_& o0, const Operand_& o1, const Operand_& o2) noexcept {
  return ((o0.id() + 1u) & 0x1Fu) == o1.id() &&
         ((o0.id() + 2u) & 0x1Fu) == o2.id();
}

static ASMJIT_INLINE bool checkConsecutive(const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_& o3) noexcept {
  return ((o0.id() + 1u) & 0x1Fu) == o1.id() &&
         ((o0.id() + 2u) & 0x1Fu) == o2.id() &&
         ((o0.id() + 3u) & 0x1Fu) == o3.id();
}

// ============================================================================
// [asmjit::Assembler - CheckReg]
// ============================================================================

#define V(type) (type == Reg::kTypeGpW  ? Gp::kIdZr :  \
                 type == Reg::kTypeGpX  ? Gp::kIdZr :  \
                 type == Reg::kTypeVecB ? 31u       :  \
                 type == Reg::kTypeVecH ? 31u       :  \
                 type == Reg::kTypeVecS ? 31u       :  \
                 type == Reg::kTypeVecD ? 31u       :  \
                 type == Reg::kTypeVecV ? 31u       : 0)
static const uint8_t commonHiRegIdOfType[32] = {
  ASMJIT_LOOKUP_TABLE_32(V, 0)
};
#undef V

static inline bool checkValidRegs(const Operand_& o0) noexcept {
  return ((o0.id() <= 31) | (o0.id() == commonHiRegIdOfType[o0.as<Reg>().type()]));
}

static inline bool checkValidRegs(const Operand_& o0, const Operand_& o1) noexcept {
  return ((o0.id() <= 31) | (o0.id() == commonHiRegIdOfType[o0.as<Reg>().type()])) &
         ((o1.id() <= 31) | (o1.id() == commonHiRegIdOfType[o1.as<Reg>().type()])) ;
}

static inline bool checkValidRegs(const Operand_& o0, const Operand_& o1, const Operand_& o2) noexcept {
  return ((o0.id() <= 31) | (o0.id() == commonHiRegIdOfType[o0.as<Reg>().type()])) &
         ((o1.id() <= 31) | (o1.id() == commonHiRegIdOfType[o1.as<Reg>().type()])) &
         ((o2.id() <= 31) | (o2.id() == commonHiRegIdOfType[o2.as<Reg>().type()])) ;
}

static inline bool checkValidRegs(const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_& o3) noexcept {
  return ((o0.id() <= 31) | (o0.id() == commonHiRegIdOfType[o0.as<Reg>().type()])) &
         ((o1.id() <= 31) | (o1.id() == commonHiRegIdOfType[o1.as<Reg>().type()])) &
         ((o2.id() <= 31) | (o2.id() == commonHiRegIdOfType[o2.as<Reg>().type()])) &
         ((o3.id() <= 31) | (o3.id() == commonHiRegIdOfType[o3.as<Reg>().type()])) ;
}

// ============================================================================
// [asmjit::Assembler - Construction / Destruction]
// ============================================================================

Assembler::Assembler(CodeHolder* code) noexcept : BaseAssembler() {
  if (code)
    code->attach(this);
}

Assembler::~Assembler() noexcept {}

// ============================================================================
// [asmjit::Assembler - Emit]
// ============================================================================

#define ENC_OPS1(OP0)                ((Operand::kOp##OP0))
#define ENC_OPS2(OP0, OP1)           ((Operand::kOp##OP0) + ((Operand::kOp##OP1) << 3))
#define ENC_OPS3(OP0, OP1, OP2)      ((Operand::kOp##OP0) + ((Operand::kOp##OP1) << 3) + ((Operand::kOp##OP2) << 6))
#define ENC_OPS4(OP0, OP1, OP2, OP3) ((Operand::kOp##OP0) + ((Operand::kOp##OP1) << 3) + ((Operand::kOp##OP2) << 6) + ((Operand::kOp##OP3) << 9))

Error Assembler::_emit(uint32_t instId, const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_* opExt) {
  constexpr uint32_t kRequiresSpecialHandling =
    uint32_t(Inst::kOptionReserved) ; // Logging/Validation/Error.

  Error err;
  uint32_t options;
  CodeWriter writer(this);

  if (instId >= Inst::_kIdCount)
    instId = 0;

  const InstDB::InstInfo* instInfo = &InstDB::_instInfoLTable[instId];
  uint32_t encodingIndex = instInfo->_encodingDataIndex;

  Opcode opcode;
  uint32_t isign4;
  uint32_t instFlags;

  const Operand_& o3 = opExt[EmitterUtils::kOp3];
  const Operand_* rmRel = nullptr;

  // uint32_t multipleOpData[4];
  // uint32_t multipleOpCount;

  // These are only used when instruction uses a relative displacement.
  OffsetFormat offsetFormat;     // Offset format.
  uint64_t offsetValue;          // Offset value (if known).

  // Combine all instruction options and also check whether the instruction
  // is valid. All options that require special handling (including invalid
  // instruction) are handled by the next branch.
  options  = uint32_t(instId == 0);
  options |= uint32_t((size_t)(_bufferEnd - writer.cursor()) < 4);
  options |= uint32_t(instOptions() | forcedInstOptions());

  if (ASMJIT_UNLIKELY(options & kRequiresSpecialHandling)) {
    if (ASMJIT_UNLIKELY(!_code))
      return reportError(DebugUtils::errored(kErrorNotInitialized));

    // Unknown instruction.
    if (ASMJIT_UNLIKELY(instId == 0))
      goto InvalidInstruction;

    // Grow request, happens rarely.
    err = writer.ensureSpace(this, 4);
    if (ASMJIT_UNLIKELY(err))
      goto Failed;

#ifndef ASMJIT_NO_VALIDATION
    // Strict validation.
    if (hasValidationOption(kValidationOptionAssembler)) {
      Operand_ opArray[Globals::kMaxOpCount];
      EmitterUtils::opArrayFromEmitArgs(opArray, o0, o1, o2, opExt);

      err = InstAPI::validate(arch(), BaseInst(instId, options, _extraReg), opArray, Globals::kMaxOpCount);
      if (ASMJIT_UNLIKELY(err))
        goto Failed;
    }
#endif
  }

  // Signature of the first 4 operands.
  isign4 = o0.opType() + (o1.opType() << 3) + (o2.opType() << 6) + (o3.opType() << 9);
  instFlags = instInfo->flags();

  switch (instInfo->_encoding) {
    // ------------------------------------------------------------------------
    // [Base - Universal]
    // ------------------------------------------------------------------------

    case InstDB::kEncodingBaseOp: {
      const InstDB::EncodingData::BaseOp& opData = InstDB::EncodingData::baseOp[encodingIndex];

      if (isign4 == 0) {
        opcode.reset(opData.opcode);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseOpImm: {
      const InstDB::EncodingData::BaseOpImm& opData = InstDB::EncodingData::baseOpImm[encodingIndex];

      if (isign4 == ENC_OPS1(Imm)) {
        uint64_t imm = o0.as<Imm>().valueAs<uint64_t>();
        if (imm >= uint64_t(1u << opData.immBits))
          goto InvalidImmediate;

        opcode.reset(opData.opcode());
        opcode.addImm(imm, opData.immOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLIC: {
      const InstDB::EncodingData::BaseLIC& opData = InstDB::EncodingData::baseLIC[encodingIndex];

      if (isign4 == ENC_OPS1(Imm)) {
        uint64_t imm = o0.as<Imm>().valueAs<uint64_t>();
        if ((imm >> 15) > 0)
          goto InvalidImmediate;

        opcode.reset(opData.opcode());
        opcode.addImm(imm, opData.immOffset);
        goto EmitOp;
      }

      break;
    }


   case InstDB::kEncodingBaseLRI: {
      const InstDB::EncodingData::BaseLRI& opData = InstDB::EncodingData::baseLRI[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Imm)) {
        if (!checkGpType(o0, opData.rType))
          goto InvalidInstruction;

	uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();

    if ((aImm > 7) && opData.uniform == 1 )
	{
          goto InvalidImmediate;
	}
	if ((opData.uniform == 2 || opData.uniform == 3) && ((aImm >> 14) > 0 ))
	{
	  goto InvalidImmediate;
	}
        if (opData.uniform == 4 && ((aImm >> 8) > 0 ))
	{
	  goto InvalidImmediate;
	}
        if (((aImm >> 20) > 0) && opData.uniform == 0)
        {
          aImm &= 0xFFFFF;
        }
	opcode.reset(opData.opcode());
	if (opData.uniform == 0)
	{
          opcode.shiftopL(15);
	}
	if (opData.uniform == 2 || opData.uniform == 3)
	{
	  opcode.shiftopL(14);
	}
	if (opData.uniform == 4)
	{
	  opcode.shiftopL(8);
	}
        opcode.addImm(aImm, opData.aImmOffset);
	if (opData.uniform == 3)
	{
	  opcode.addImm(0b100000,0);
	}
        opcode.addReg(o0, opData.rShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLIR: {
      const InstDB::EncodingData::BaseLIR& opData = InstDB::EncodingData::baseLIR[encodingIndex];

      if (isign4 == ENC_OPS2(Imm, Reg)) {
        if (!checkGpType(o1, opData.aType))
          goto InvalidInstruction;

        uint32_t aImm = o0.as<Imm>().valueAs<uint32_t>();

        if (aImm > 7)
          goto InvalidImmediate;
        opcode.reset(opData.opcode());
        //opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o1, opData.aShift);
	opcode.addImm(aImm, opData.aImmOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLIV: {
      const InstDB::EncodingData::BaseLIV& opData = InstDB::EncodingData::baseLIV[encodingIndex];

      if (isign4 == ENC_OPS2(Imm, Reg)) {
        uint32_t aImm = o0.as<Imm>().valueAs<uint32_t>();
	    if (aImm > 7)
          goto InvalidImmediate;

        opcode.reset(opData.opcode());
	    opcode.addReg(o1, opData.aShift);
	    opcode.addImm(aImm, opData.aImmOffset);
	    goto EmitOp;
      }
      break;
    }

    case InstDB::kEncodingBaseLVI: {
      const InstDB::EncodingData::BaseLVI& opData = InstDB::EncodingData::baseLVI[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Imm)) {
        uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();
        if (aImm > 7)
          goto InvalidImmediate;

        opcode.reset(opData.opcode());
        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o0, opData.aShift);
        //opcode.addImm(aImm, opData.aImmOffset);
        goto EmitOp;
      }
      break;
    }

    case InstDB::kEncodingBaseLRR: {
      const InstDB::EncodingData::BaseLRR& opData = InstDB::EncodingData::baseLRR[encodingIndex];
      if (isign4 == ENC_OPS2(Reg, Reg)) {
	if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;
	if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;
        if (opData.uniform == 0 && !checkSignature(o0, o1))
          goto InvalidInstruction;
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
       }

       break;
     }

    case InstDB::kEncodingBaseLVV: {
      const InstDB::EncodingData::BaseLVV& opData = InstDB::EncodingData::baseLVV[encodingIndex];
      if (isign4 == ENC_OPS2(Reg, Reg)) {
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
       }

       break;
     }

    case InstDB::kEncodingBaseLVR: {
      const InstDB::EncodingData::BaseLVR& opData = InstDB::EncodingData::baseLVR[encodingIndex];
      if (isign4 == ENC_OPS2(Reg, Reg)) {
	if (!checkGpType(o1, opData.bType))
        {
          goto InvalidInstruction;
	}
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
       }

       break;
     }

    case InstDB::kEncodingBaseLRV: {
      const InstDB::EncodingData::BaseLRV& opData = InstDB::EncodingData::baseLRV[encodingIndex];
      if (isign4 == ENC_OPS2(Reg, Reg)) {
        if (!checkGpType(o0, opData.aType))
        {
          goto InvalidInstruction;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
       }

       break;
     }

    case InstDB::kEncodingBaseLRRL: {
      const InstDB::EncodingData::BaseLRRL& opData = InstDB::EncodingData::baseLRRL[encodingIndex];
      if (isign4 == ENC_OPS2(Reg, Reg)) {
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;
        //if (opData.uniform && !checkSignature(o0, o1))
          //goto InvalidInstruction;
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
       }

       break;
     }

    case InstDB::kEncodingBaseLRRR: {
      const InstDB::EncodingData::BaseLRRR& opData = InstDB::EncodingData::baseLRRR[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        //uint32_t x;
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (opData.uniform == 0 && !checkSignature(o0, o1, o2))
          goto InvalidInstruction;

        opcode.reset(opData.opcode());
        //opcode.addImm(x, 31);
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
	goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLRRRT: {
      const InstDB::EncodingData::BaseLRRRT& opData = InstDB::EncodingData::baseLRRRT[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        //uint32_t x;
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (opData.uniform == 0 && !checkSignature(o0, o1, o2))
          goto InvalidInstruction;

        opcode.reset(opData.opcode());
        //opcode.addImm(x, 31);
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLVRR: {
      const InstDB::EncodingData::BaseLVRR& opData = InstDB::EncodingData::baseLVRR[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {

        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (opData.uniform == 0 && !checkSignature(o1, o2))
          goto InvalidInstruction;

        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLVVV: {
      const InstDB::EncodingData::BaseLVVV& opData = InstDB::EncodingData::baseLVVV[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {

        if (opData.uniform == 0 && !checkSignature(o0, o1, o2))
          goto InvalidInstruction;

        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLRRRI: {
      const InstDB::EncodingData::BaseLRRRI& opData = InstDB::EncodingData::baseLRRRI[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Reg, Imm)) {
        //uint32_t x;
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        uint32_t aImm = o3.as<Imm>().valueAs<uint32_t>();

        if ((aImm) != o3.as<Imm>().valueAs<uint32_t>())
          goto InvalidImmediate;

        if ((aImm < 1 || aImm > 4) && opData.uniform != 2)
        {
          goto InvalidInstruction;
        }

        if(( aImm > 7 ) && opData.uniform == 2)
	    {
          goto InvalidInstruction;
	    }

	    if (opData.uniform == 0)
	    {
          aImm--;
	    }
        //opcode.reset(opData.opcode);
        opcode.reset(opData.opcode());
	    uint32_t ik = 1;
	    if (opData.uniform == 2)
	    {
          opcode.shiftopL(ik);
        }
        opcode.addImm(aImm, opData.aImmOffset);

        opcode.addReg(o2, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      }

      break;
    }
    case InstDB::kEncodingBaseLRRII: {
      const InstDB::EncodingData::BaseLRRII& opData = InstDB::EncodingData::baseLRRII[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Imm, Imm)) {
        //uint32_t x;
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        uint32_t bImm = o2.as<Imm>().valueAs<uint32_t>();
        uint32_t aImm = o3.as<Imm>().valueAs<uint32_t>();

        if ((aImm > 31 || bImm > 31 || aImm > bImm) && (opData.uniform == 0 || opData.uniform ==1))
        {
          goto InvalidImmediate;
        }
        if((aImm > 63 || bImm > 63 || aImm > bImm) && opData.uniform == 2)
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 1)
        {
          aImm |= 0x20;
        }
        //opcode.reset(opData.opcode);
        opcode.reset(opData.opcode());
        uint32_t ik = 1;
        if (opData.uniform == 2)
        {
          opcode.shiftopL(ik);
        }

	opcode.addImm(bImm, opData.bImmOffset);
        opcode.addImm(aImm, opData.aImmOffset);
        //opcode.addReg(o2, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLRRI: {
      const InstDB::EncodingData::BaseLRRI& opData = InstDB::EncodingData::baseLRRI[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();

	int32_t amk = o2.as<Imm>().valueAs<int32_t>();
        if (((aImm >> 12) > 0) && opData.uniform == 0)
        {
          aImm &= 0xFFF;
        }

	if ( opData.uniform == 0 && (amk > 2047 || amk < -2048))
	{
          goto InvalidImmediate;
	}
	if ( opData.uniform == 1 && ( aImm > 31))
	{
	  goto InvalidImmediate;
	}
	if ( opData.uniform == 2 && ( aImm > 63 ))
	{
	  goto InvalidImmediate;
	}
	if ( opData.uniform == 3 && ((aImm >> 14) > 0) )
	{
	  goto InvalidImmediate;
	}
	if ( opData.uniform == 4 && ((aImm >> 8) > 0) )
	{
	  goto InvalidImmediate;
	}
	if ( opData.uniform == 5 && ((aImm >> 12) > 0) )  //ui12
	{
	  goto InvalidImmediate;
	}
        opcode.reset(opData.opcode());
	if ( (opData.uniform == 0) || (opData.uniform == 5) )  //si12 or ui12
	{
          opcode.shiftopL(7);
	}
	if ( opData.uniform == 2)
	{
          opcode.shiftopL(1);
	}
	if (opData.uniform == 3)
	{
	  opcode.shiftopL(9);
	}
	if ( opData.uniform == 4 )
	{
	  opcode.shiftopL(3);
	}
	if (opData.uniform == 3 && (o1._baseId == 0 || o1._baseId == 1))
	{
	  goto InvalidInstruction;
	}
        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLIRR: {
      const InstDB::EncodingData::BaseLIRR& opData = InstDB::EncodingData::baseLIRR[encodingIndex];

      if (isign4 == ENC_OPS3(Imm, Reg, Reg)) {
        if (!checkGpType(o1, opData.aType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.bType))
          goto InvalidInstruction;

        uint32_t aImm = o0.as<Imm>().valueAs<uint32_t>();

        if ( opData.uniform == 0 && ( aImm > 31))
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.bShift);
        opcode.addReg(o1, opData.aShift);
        opcode.addImm(aImm, opData.aImmOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLFIVV: {
      const InstDB::EncodingData::BaseLFIVV& opData = InstDB::EncodingData::baseLFIVV[encodingIndex];

      if (isign4 == ENC_OPS3(Imm, Reg, Reg)) {

        uint32_t aImm = o0.as<Imm>().valueAs<uint32_t>();

        if ( opData.uniform == 0 && ( aImm > 7))
        {
          goto InvalidInstruction;
        }

        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addImm(aImm, opData.aImmOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLRRIL: {
      const InstDB::EncodingData::BaseLRRIL& opData = InstDB::EncodingData::baseLRRIL[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkGpId(o0, opData.aHiId))
          goto InvalidPhysId;
        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();

        if ((aImm) != o2.as<Imm>().valueAs<uint32_t>())
          goto InvalidImmediate;
        if ((aImm >> 16) > 0)
        {
          aImm &= 0xFFFF;
        }
        opcode.reset(opData.opcode());
        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLII: {
      const InstDB::EncodingData::BaseLII& opData = InstDB::EncodingData::baseLII[encodingIndex];

      if (isign4 == ENC_OPS2(Imm, Imm)) {

        uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();
        uint32_t bImm = o0.as<Imm>().valueAs<uint32_t>();
	uint32_t cImm;
	int64_t ka = o1.as<Imm>().valueAs<int64_t>();

	if ( (aImm & 0x3u) != 0 )
	{
	  printf("The lowest double bit must be zero\n");
	  goto InvalidImmediate;
	}

        if ( (ka < -1*(1<<22)) || (ka > 0x3fffff) )
	{
	  goto InvalidImmediate;
	}

        if (bImm >= uint64_t(1u << opData.bImmSize))
        {
          goto InvalidImmediate;
        }

	if ( opData.uniform == 1)
	{
	  bImm ^= 0x8;
	}
	//if (o1.as<Imm>().valueAs<int64_t>() > 0)
        //{
	cImm = ((aImm >> 2) & 0xFFFF);
	aImm >>= 18;
	aImm &= 0x1F;
	//}
        opcode.reset(opData.opcode());
        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addImm(bImm, opData.bImmOffset);
        opcode.addImm(cImm, opData.cImmOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseBLI: {
      const InstDB::EncodingData::BaseBLI& opData = InstDB::EncodingData::basebLI[encodingIndex];

      if (isign4 == ENC_OPS1(Imm)) {
        opcode.reset(opData.opcode());
        rmRel = &o0;

        /*if (options & Inst::kOptionCondFlagMask) {
          uint32_t cond = (options >> Inst::kOptionCondCodeShift) & 0xFu;
          if (cond != Cond::kAL) {
            opcode |= B(30);
            opcode.addImm(cond, 0);
            offsetFormat.resetToImmValue(OffsetFormat::kTypeCommon, 4, 5, 19, 2);
            goto EmitOp_Rel;
          }
        }*/

        offsetFormat.resetToImmValue(OffsetFormat::kTypeLa64_BBL, 4, 0, 26, 2);
        goto EmitOp_Rel;
      }

      break;
    }

    case InstDB::kEncodingBaseBranchRel: {
      const InstDB::EncodingData::BaseBranchRel& opData = InstDB::EncodingData::baseBranchRel[encodingIndex];

      if (isign4 == ENC_OPS1(Label) || isign4 == ENC_OPS1(Imm)) {
        opcode.reset(opData.opcode());
        rmRel = &o0;

        offsetFormat.resetToImmValue(OffsetFormat::kTypeLa64_BBL, 4, 0, 26, 2);  //B and BL
        goto EmitOp_Rel;
      }
      else if (isign4 == ENC_OPS3(Reg, Reg, Imm) || isign4 == ENC_OPS3(Reg, Reg, Label)) {
        opcode.reset(opData.opcode());
        rmRel = &o2;

        opcode.addReg(o0, 5);
        opcode.addReg(o1, 0);

        offsetFormat.resetToImmValue(OffsetFormat::kTypeLa64_BEQ, 4, 10, 16, 2);  //BEQ ...
        goto EmitOp_Rel;
      }

      break;
    }

    case InstDB::kEncodingBTLRRI: {
      const InstDB::EncodingData::BTLRRI& opData = InstDB::EncodingData::btLRRI[encodingIndex];
     /* if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        uint32_t x;
        if (!checkGpType(o0, o1, InstDB::kWX, &x))
          goto InvalidInstruction;

        if (!checkGpId(o0, o1, kZR))
          goto InvalidPhysId;

        uint64_t cond = o2.as<Imm>().valueAs<uint64_t>();*/
      if (isign4 == ENC_OPS3(Reg, Reg, Imm) || isign4 == ENC_OPS3(Reg, Reg, Label)) {
        //uint32_t x;
        //if (!checkGpType(o0, o1, InstDB::kWX))
          //goto InvalidInstruction;

        if (!checkGpId(o0, o1,kZR))
          goto InvalidPhysId;

        uint64_t imm = o2.as<Imm>().valueAs<uint64_t>();

        opcode.reset(opData.opcode());
        if (imm >= 65535) {
          //if (!x)
            //goto InvalidImmediate;
          //opcode.addImm(x, 31);
          imm &= 0x1F;
        }

        opcode.addReg(o0, 5);
        opcode.addReg(o1, 0);
        offsetFormat.resetToImmValue(OffsetFormat::kTypeCommon, 4, 10, 14, 2);

        rmRel = &o2;
        goto EmitOp_Rel;
      }

      break;
    }

    case InstDB::kEncodingJBTLRRI: {
      const InstDB::EncodingData::JBTLRRI& opData = InstDB::EncodingData::jbtLRRI[encodingIndex];
      if (isign4 == ENC_OPS3(Reg, Reg, Imm) || isign4 == ENC_OPS3(Reg, Reg, Label)) {
        //uint32_t x;
        //if (!checkGpType(o0, o1, InstDB::kWX))
          //goto InvalidInstruction;

        if (!checkGpId(o0, o1,kZR))
          goto InvalidPhysId;

        int32_t imm = o2.as<Imm>().valueAs<int32_t>();

        if ( (imm > 0x1FFFF) || imm < -1*(1<<17)) {
            goto InvalidImmediate;
	}

        opcode.reset(opData.opcode());
        opcode.addReg(o0, 0);
        opcode.addReg(o1, 5);
	//offsetFormat.resetToImmValue(OffsetFormat::kTypeCommon, 4, 10, 14, 2);
	imm &= 0x3FFFF;
	imm >>= 2;
        opcode.addImm(imm, 10);
        //rmRel = &o2;
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingBaseLldst: {
      const InstDB::EncodingData::BaseLldst& opData = InstDB::EncodingData::baseLldst[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Mem)) {
	//const Operand_& o5 = o1 + o2;
        //uint64_t o5 = (uint64_t)((o1._baseId) + (o2._data[1]));
        //const Operand_& o5 = o5;
        //const Mem& m1 = (o1).as<Mem>();
        const Mem& m = (o1).as<Mem>();
        //const Mem& m = (m1 + m2);
        rmRel = &m;

        //uint32_t x;
        //if (!checkGpType(o0, o1, opData.rType))
          //goto InvalidInstruction;

        if (!checkGpId(o0, o1, kZR))
          goto InvalidPhysId;

        if (m.baseType() != Reg::kTypeGpX || m.hasIndex())
          goto InvalidAddress;

        if (m.isOffset64Bit())
          goto InvalidDisplacement;

        //uint32_t offsetShift = opData.offsetShift + x;
	uint32_t offsetShift = opData.offsetShift;
        int32_t offset32 = m.offsetLo32() >> offsetShift;

        // Make sure we didn't lose bits by applying the mandatory offset shift.
        if (uint32_t(offset32) << offsetShift != uint32_t(m.offsetLo32()))
          goto InvalidDisplacement;

        // Offset is encoded as 7-bit immediate.
        if (!Support::isInt7(offset32))
          goto InvalidDisplacement;

        if (m.isPreOrPost() && offset32 != 0) {
	  if (!opData.prePostOp)
            goto InvalidAddress;

          opcode.reset(uint32_t(opData.prePostOp) << 22);
          opcode.addImm(m.isPreIndex(), 24);
        }
        else {
          opcode.reset(uint32_t(opData.offsetOp) << 22);
        }
	uint32_t kk;
	if (o2._data[1] > 2047)
	  {
            //kk = ((~(o2._data[1])) + (uint32_t(1)));
	    kk = o2._data[1] & 0xFFF;
	    //kk ^= 0x200000;
          }
	else {
          kk = o2._data[1];
	  }
        //opcode.reset(uint32_t(opData.offsetOp) << 22);
        //opcode.addImm(x, opData.xOffset);
        opcode.addImm(kk, 10);
        //opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp_MemBase_Rn5;
      }

      break;
    }

    case InstDB::kEncodingBaseLdSt: {
      const InstDB::EncodingData::BaseLdSt& opData = InstDB::EncodingData::baseLdst[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Mem)) {
        const Mem& m = o1.as<Mem>();
        rmRel = &m;

        /*uint32_t x;
        if (!checkGpType(o0, opData.rType, &x))
          goto InvalidInstruction; */

        if (!checkGpId(o0, kZR))
          goto InvalidPhysId;

        // Instructions that work with either word or dword have the unsigned
        // offset shift set to 2 (word), so we set it to 3 (dword) if this is
        // X version of the instruction.
        //uint32_t xShiftMask = uint32_t(opData.uOffsetShift == 2);
        //uint32_t immShift = uint32_t(opData.uOffsetShift) + (x & xShiftMask);

        if (!armCheckMemBaseIndexRel(m))
          goto InvalidAddress;

        int64_t offset = m.offset();
        if (m.hasBaseReg()) {
          // [Base {Offset | Index}]
          if (m.hasIndex()) {
            /*uint32_t opt = armShiftOpToLdStOptMap[m.predicate()];
            if (opt == 0xFF)
              goto InvalidAddress;

            uint32_t shift = m.shift();
            uint32_t s = shift != 0;

            if (s && shift != immShift)
              goto InvalidAddressScale; */

            /*opcode.reset(uint32_t(opData.registerOp) << 21);
            opcode.xorImm(x, opData.xOffset);
            opcode.addImm(opt, 13);
            opcode.addImm(s, 12);
            opcode |= B(11);
            opcode.addReg(o0, 0);
            goto EmitOp_MemBaseIndex_Rn5_Rm16; */

            opcode.reset(opData.opcode());
            opcode.addReg(o0, 0);
            goto EmitOp_MemBaseIndex_Rj5_Rk10;
          }

          // Makes it easier to work with the offset especially on 32-bit arch.
          if (!Support::isInt32(offset))
            goto InvalidDisplacement;
          int32_t offset32 = int32_t(offset);

          //if (m.isPreOrPost()) {
            //if (!Support::isInt9(offset32))
            if (!Support::isInt12(offset32))
              goto InvalidDisplacement;

            /*opcode.reset(uint32_t(opData.prePostOp) << 21);
            opcode.xorImm(x, opData.xOffset);
            opcode.addImm(offset32 & 0x1FF, 12);
            opcode.addImm(m.isPreIndex(), 11);
            opcode |= B(10);
            opcode.addReg(o0, 0);
            goto EmitOp_MemBase_Rn5; */

            opcode.reset(opData.opcode());
            opcode.addImm(offset32 & 0xFFF, 10);
            opcode.addReg(o0, 0);
            goto EmitOp_MemBase_Rj5;
          /*}
          else {
            uint32_t imm12 = uint32_t(offset32) >> immShift;

            // Alternative form of LDUR/STUR and related instructions as described by AArch64 reference manual:
            //
            // If this instruction is not encodable with scaled unsigned offset, try unscaled signed offset.
            if (!Support::isUInt12(imm12) || (imm12 << immShift) != uint32_t(offset32)) {
              instId = opData.uAltInstId;
              instInfo = &InstDB::_instInfoTable[instId];
              encodingIndex = instInfo->_encodingDataIndex;
              goto Case_BaseLdurStur;
            }

            opcode.reset(uint32_t(opData.uOffsetOp) << 22);
            opcode.xorImm(x, opData.xOffset);
            opcode.addImm(imm12, 10);
            opcode.addReg(o0, 0);
            goto EmitOp_MemBase_Rn5;
          } */
        }
        else {
          /*if (!opData.literalOp)
            goto InvalidAddress;

          opcode.reset(uint32_t(opData.literalOp) << 24);
          opcode.xorImm(x, opData.xOffset);
          opcode.addReg(o0, 0);
          offsetFormat.resetToImmValue(OffsetFormat::kTypeCommon, 4, 5, 19, 2);
          goto EmitOp_Rel; */

          goto InvalidAddress;
        }
      } else if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        //TODO: add reg type check
        int64_t immVal = o2.as<Imm>().valueAs<int64_t>();

        // Makes it easier to work with the offset especially on 32-bit arch.
        if (!Support::isInt32(immVal))
          goto InvalidDisplacement;

        int32_t offset32 = int32_t(immVal);
        if (!Support::isInt12(offset32))
          goto InvalidDisplacement;

        uint32_t offset = offset32 & 0xFFF;
        opcode.reset(opData.opcode());
        opcode.addImm(offset, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      } else if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        //TODO: add instr check
        opcode.reset(opData.opcode());
        opcode.addReg(o2, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingSimdLdst: {
      const InstDB::EncodingData::SimdLdst& opData = InstDB::EncodingData::simdLdst[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Mem)) {
        const Mem& m = o1.as<Mem>();
        rmRel = &m;

        if (!armCheckMemBaseIndexRel(m))
          goto InvalidAddress;

        int64_t offset = m.offset();
        if (m.hasBaseReg()) {
          // [Base {Offset | Index}]
          if (m.hasIndex()) {
            opcode.reset(opData.opcode());
            opcode.addReg(o0, 0);
            goto EmitOp_MemBaseIndex_Rj5_Rk10;
          }

          // Makes it easier to work with the offset especially on 32-bit arch.
          if (!Support::isInt32(offset))
            goto InvalidDisplacement;
          int32_t offset32 = int32_t(offset);

          if (!Support::isInt12(offset32))
            goto InvalidDisplacement;

          uint32_t offsetLen = opData.offsetLen;
          uint32_t offset = 0;
          switch(offsetLen){
              case 9:
                if ( (offset32 & 0x7) != 0)
                  goto InvalidDisplacement;
                offset = (offset32 >> 3) & 0x1FF;
                break;
              case 10:
                if ( (offset32 & 0x3) != 0)
                  goto InvalidDisplacement;
                offset = (offset32 >> 2) & 0x3FF;
                break;
              case 11:
                if ( (offset32 & 0x1) != 0)
                  goto InvalidDisplacement;
                offset = (offset32 >> 1) & 0x7FF;
                break;
              case 12:
                offset = offset32 & 0xFFF;
                break;
              default:
                goto InvalidDisplacement;

          }

          opcode.reset(opData.opcode());
          opcode.addImm(offset, 10);
          opcode.addReg(o0, 0);
          goto EmitOp_MemBase_Rj5;
        }
        else {
          goto InvalidAddress;
        }
      } else if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        //TODO: add reg type check
        int64_t immVal = o2.as<Imm>().valueAs<int64_t>();

        // Makes it easier to work with the offset especially on 32-bit arch.
        if (!Support::isInt32(immVal))
          goto InvalidDisplacement;

        int32_t offset32 = int32_t(immVal);
        if (!Support::isInt12(offset32))
          goto InvalidDisplacement;

        uint32_t offsetLen = opData.offsetLen;
        uint32_t offset = 0;
        switch(offsetLen){
            case 9:
              if ( (offset32 & 0x7) != 0)
                  goto InvalidDisplacement;
              offset = (offset32 >> 3) & 0x1FF;
              break;
            case 10:
              if ( (offset32 & 0x3) != 0)
                  goto InvalidDisplacement;
              offset = (offset32 >> 2) & 0x3FF;
              break;
            case 11:
              if ( (offset32 & 0x1) != 0)
                  goto InvalidDisplacement;
              offset = (offset32 >> 1) & 0x7FF;
              break;
            case 12:
              offset = offset32 & 0xFFF;
              break;
            default:
              goto InvalidDisplacement;
        }

        opcode.reset(opData.opcode());
        opcode.addImm(offset, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      } else if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        uint32_t offsetLen = opData.offsetLen;
        if (0 != offsetLen)
          goto InvalidDisplacement;

        opcode.reset(opData.opcode());
        opcode.addReg(o2, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingFpLVVVV: {
      const InstDB::EncodingData::FpLVVVV& opData = InstDB::EncodingData::fpLVVVV[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Reg, Reg)) {
        if (!matchSignature(o0, o1, o2, o3, instFlags))
          goto InvalidInstruction;

        //if (!pickFpOpcode(o0.as<Vec>(), opData.scalarOp(), opData.scalarHf(), opData.vectorOp(), opData.vectorHf(), &opcode))
          //goto InvalidInstruction;

        //goto EmitOp_Rd0_Rn5_Rm16_Ra10;
	if (!checkValidRegs(o0, o1, o2, o3))
          goto InvalidPhysId;
        opcode.reset(opData.opcode());
        opcode.addReg(o0, 0);
        opcode.addReg(o1, 5);
        opcode.addReg(o2, 10);
        opcode.addReg(o3, 15);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVVVV: {
      const InstDB::EncodingData::lsxVVVV& opData = InstDB::EncodingData::lsxvvvv[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Reg, Reg)) {
        if (!matchSignature(o0, o1, o2, o3, instFlags))
          goto InvalidInstruction;

        //if (!pickFpOpcode(o0.as<Vec>(), opData.scalarOp(), opData.scalarHf(), opData.vectorOp(), opData.vectorHf(), &opcode))
          //goto InvalidInstruction;

        //goto EmitOp_Rd0_Rn5_Rm16_Ra10;
        if (!checkValidRegs(o0, o1, o2, o3))
          goto InvalidPhysId;
        opcode.reset(opData.opcode());
        opcode.addReg(o0, opData.aShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o3, opData.dShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVVV: {
      const InstDB::EncodingData::lsxVVV& opData = InstDB::EncodingData::lsxvvv[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        if (!matchSignature(o0, o1, o2, instFlags))
	{
          goto InvalidInstruction;
	}

        //if (!pickFpOpcode(o0.as<Vec>(), opData.scalarOp(), opData.scalarHf(), opData.vectorOp(), opData.vectorHf(), &opcode))
          //goto InvalidInstruction;

        //goto EmitOp_Rd0_Rn5_Rm16_Ra10;
        if (!checkValidRegs(o0, o1, o2))
	{
          goto InvalidPhysId;
	}
        opcode.reset(opData.opcode());
        opcode.addReg(o0, opData.aShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o2, opData.cShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXXX: {
      const InstDB::EncodingData::lasxXXX& opData = InstDB::EncodingData::lasxxxx[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        if (!matchSignature(o0, o1, o2, instFlags))
        {
          goto InvalidInstruction;
        }

        //if (!pickFpOpcode(o0.as<Vec>(), opData.scalarOp(), opData.scalarHf(), opData.vectorOp(), opData.vectorHf(), &opcode))
          //goto InvalidInstruction;

        //goto EmitOp_Rd0_Rn5_Rm16_Ra10;
        if (!checkValidRegs(o0, o1, o2))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o0, opData.aShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o2, opData.cShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXXXX: {
      const InstDB::EncodingData::lasxXXXX& opData = InstDB::EncodingData::lasxxxxx[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Reg, Reg)) {
        if (!matchSignature(o0, o1, o2, o3, instFlags))
          goto InvalidInstruction;

        //if (!pickFpOpcode(o0.as<Vec>(), opData.scalarOp(), opData.scalarHf(), opData.vectorOp(), opData.vectorHf(), &opcode))
          //goto InvalidInstruction;

        //goto EmitOp_Rd0_Rn5_Rm16_Ra10;
        if (!checkValidRegs(o0, o1, o2, o3))
	{
          goto InvalidPhysId;
	}
        opcode.reset(opData.opcode());
        opcode.addReg(o0, opData.aShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o3, opData.dShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVRI: {
      const InstDB::EncodingData::lsxVRI& opData = InstDB::EncodingData::lsxvri[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t immValue = o2.as<Imm>().valueAs<int64_t>();

        if ((immValue > 2047 || immValue < (-2048)) && (opData.uniform == 0 || opData.uniform == 4))
	{
          goto InvalidImmediate;
        }

	if (opData.uniform == 1 && ( ((immValue + 0x800) & 0xFFFFFFFFFFFFF007) != 0) )
	{
	  goto InvalidImmediate;
	}

    if (opData.uniform == 2 && ( ((immValue + 0x800) & 0xFFFFFFFFFFFFF003) != 0) )
	{
	  goto InvalidImmediate;
	}

    if (opData.uniform == 3 && ( ((immValue + 0x800) & 0xFFFFFFFFFFFFF001) != 0))
    {
      goto InvalidImmediate;
    }

        if (opData.uniform == 5 && (immValue > 15 || immValue < 0))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 6 && (immValue > 7 || immValue < 0))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 7 && (immValue > 3 || immValue < 0))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 8 && (immValue > 1 || immValue < 0))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 1)
        {
          aImm >>= 3;
          immValue >>= 3;
        }

        if (opData.uniform == 2)
        {
          aImm >>= 2;
          immValue >>= 2;
        }

	if (opData.uniform == 3)
        {
          aImm >>= 1;
          immValue >>= 1;
        }

        opcode.reset(opData.opcode());
	if (opData.uniform == 0 || opData.uniform == 4)
        {
          opcode.shiftopL(11);
        }
	if (opData.uniform == 1)
	{
	  opcode.shiftopL(8);
	}

	if (opData.uniform == 2)
        {
          opcode.shiftopL(9);
        }

        if (opData.uniform == 3)
        {
          opcode.shiftopL(10);
        }

        if (opData.uniform == 5)
        {
          opcode.shiftopL(3);
        }

        if (opData.uniform == 6)
        {
          opcode.shiftopL(2);
        }

        if (opData.uniform == 7)
        {
          opcode.shiftopL(1);
        }

	if (immValue >= 0)
	{
          opcode.addImm(aImm, opData.aImmOffset);
	}
	if ((immValue < 0 && opData.uniform == 0) || opData.uniform == 4)
	{
	  aImm &= 0xFFF;
          opcode.addImm(aImm, opData.aImmOffset);
	}

	if (immValue < 0 && opData.uniform == 1)
	{
	  aImm &= 0x1FF;
	  opcode.addImm(aImm, opData.aImmOffset);
	}

        if (immValue < 0 && opData.uniform == 2)
        {
          aImm &= 0x3FF;
          opcode.addImm(aImm, opData.aImmOffset);
        }

        if (immValue < 0 && opData.uniform == 3)
        {
          aImm &= 0x7FF;
          opcode.addImm(aImm, opData.aImmOffset);
        }

        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXRI: {
      const InstDB::EncodingData::lasxXRI& opData = InstDB::EncodingData::lasxxri[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t immValue = o2.as<Imm>().valueAs<int64_t>();

        if ((immValue > 2047 || immValue < (-2048)) && (opData.uniform == 0 || opData.uniform == 4))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 1 && ( ((immValue + 0x800) & 0xFFFFFFFFFFFFF007) != 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 2 && ( ((immValue + 0x800) & 0xFFFFFFFFFFFFF003) != 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && ( ((immValue + 0x800) & 0xFFFFFFFFFFFFF001) != 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 5 && (immValue > 7 || immValue < 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 6 && (immValue > 3 || immValue < 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 1)
        {
          aImm >>= 3;
          immValue >>= 3;
        }

        if (opData.uniform == 2)
        {
          aImm >>= 2;
          immValue >>= 2;
        }

        if (opData.uniform == 3)
        {
          aImm >>= 1;
          immValue >>= 1;
        }

        opcode.reset(opData.opcode());
        if (opData.uniform == 0 || opData.uniform == 4)
        {
          opcode.shiftopL(10);
        }
        if (opData.uniform == 1)
        {
          opcode.shiftopL(7);
        }

        if (opData.uniform == 2)
        {
          opcode.shiftopL(8);
        }

        if (opData.uniform == 3)
        {
          opcode.shiftopL(9);
        }

        if (opData.uniform == 5)
        {
          opcode.shiftopL(1);
        }

        if (immValue >= 0)
        {
          opcode.addImm(aImm, opData.aImmOffset);
        }
        if ((immValue < 0 && opData.uniform == 0) || opData.uniform == 4)
        {
          aImm &= 0xFFF;
          opcode.addImm(aImm, opData.aImmOffset);
        }

        if (immValue < 0 && opData.uniform == 1)
        {
          aImm &= 0x1FF;
          opcode.addImm(aImm, opData.aImmOffset);
        }

        if (immValue < 0 && opData.uniform == 2)
        {
          aImm &= 0x3FF;
          opcode.addImm(aImm, opData.aImmOffset);
        }

        if (immValue < 0 && opData.uniform == 3)
        {
          aImm &= 0x7FF;
          opcode.addImm(aImm, opData.aImmOffset);
        }

        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVRII: {
      const InstDB::EncodingData::lsxVRII& opData = InstDB::EncodingData::lsxvrii[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Imm, Imm)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        uint32_t bImm = o3.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o2.as<Imm>().valueAs<int64_t>();
	int64_t bimmValue = o3.as<Imm>().valueAs<int64_t>();

        if (((aimmValue >> 3) > 127 || (aimmValue >> 3) < (-128)) && opData.uniform == 0)
	{
          goto InvalidImmediate;
        }

        if ((bimmValue > 1 || bimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 1 && ((aimmValue >> 2) > 127 || (aimmValue >> 2) < (-128)))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 1 && (bimmValue > 3 || bimmValue < 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 2 && ((aimmValue >> 1) > 127 || (aimmValue >> 1) < (-128)))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 2 && (bimmValue > 7 || bimmValue < 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && (aimmValue > 127 || aimmValue < (-128)))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && (bimmValue > 15 || bimmValue < 0))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 0)
        {
          aImm >>= 3;
          aimmValue >>= 3;
        }

        if (opData.uniform == 1)
        {
          aImm >>= 2;
          aimmValue >>= 2;
        }

        if (opData.uniform == 2)
        {
          aImm >>= 1;
          aimmValue >>= 1;
        }

        opcode.reset(opData.opcode());

	if (opData.uniform == 1)
	{
	  opcode.shiftopL(1);
	}

	if (opData.uniform == 2)
        {
          opcode.shiftopL(2);
        }

        if (opData.uniform == 3)
        {
          opcode.shiftopL(3);
        }

        opcode.addImm(bImm, opData.bImmOffset);

	if (aimmValue >= 0)
	{
          opcode.addImm(aImm, opData.aImmOffset);
	}

	if (aimmValue < 0)
	{
	  aImm &= 0xFF;
          opcode.addImm(aImm, opData.aImmOffset);
	}

        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXRII: {
      const InstDB::EncodingData::lasxXRII& opData = InstDB::EncodingData::lasxxrii[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Imm, Imm)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        uint32_t bImm = o3.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o2.as<Imm>().valueAs<int64_t>();
	int64_t bimmValue = o3.as<Imm>().valueAs<int64_t>();

        if (((aimmValue >> 3) > 127 || (aimmValue >> 3) < (-128)) && opData.uniform == 0)
	{
          goto InvalidImmediate;
        }

        if ((bimmValue > 3 || bimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 1 && ((aimmValue >> 2) > 127 || (aimmValue >> 2) < (-128)))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 1 && (bimmValue > 7 || bimmValue < 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 2 && ((aimmValue >> 1) > 127 || (aimmValue >> 1) < (-128)))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 2 && (bimmValue > 15 || bimmValue < 0))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && (aimmValue > 127 || aimmValue < (-128)))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && (bimmValue > 31 || bimmValue < 0))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 0)
        {
          aImm >>= 3;
          aimmValue >>= 3;
        }

        if (opData.uniform == 1)
        {
          aImm >>= 2;
          aimmValue >>= 2;
        }

        if (opData.uniform == 2)
        {
          aImm >>= 1;
          aimmValue >>= 1;
        }

        opcode.reset(opData.opcode());

	if (opData.uniform == 1)
	{
	  opcode.shiftopL(1);
	}

	if (opData.uniform == 2)
        {
          opcode.shiftopL(2);
        }

        if (opData.uniform == 3)
        {
          opcode.shiftopL(3);
        }

        opcode.addImm(bImm, opData.bImmOffset);

	if (aimmValue >= 0)
	{
          opcode.addImm(aImm, opData.aImmOffset);
	}

	if (aimmValue < 0)
	{
	  aImm &= 0xFF;
          opcode.addImm(aImm, opData.aImmOffset);
	}

        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVRR: {
      const InstDB::EncodingData::lsxVRR& opData = InstDB::EncodingData::lsxvrr[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
	if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o0, opData.aShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o2, opData.cShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXRR: {
      const InstDB::EncodingData::lasxXRR& opData = InstDB::EncodingData::lasxxrr[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.cShift);
	opcode.addReg(o1, opData.bShift);
	opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVVR: {
      const InstDB::EncodingData::lsxVVR& opData = InstDB::EncodingData::lsxvvr[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (!checkValidRegs(o0, o1))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.cShift);
	opcode.addReg(o1, opData.bShift);
	opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXXR: {
      const InstDB::EncodingData::lasxXXR& opData = InstDB::EncodingData::lasxxxr[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Reg)) {
        if (!checkGpType(o2, opData.cType))
          goto InvalidInstruction;

        if (!checkValidRegs(o0, o1))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o2, opData.cShift);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVVI: {
      const InstDB::EncodingData::lsxVVI& opData = InstDB::EncodingData::lsxvvi[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkValidRegs(o0, o1))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t immValue = o2.as<Imm>().valueAs<int64_t>();

        if (opData.uniform == 0 && (immValue < (-16) || immValue > 15))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 9 && (immValue < 0 || immValue > 31))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 8 && (immValue < 0 || immValue > 255))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 7 && (immValue < 0 || immValue > 127))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 1 && (immValue < 0 || immValue > 7))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 2 && (immValue < 0 || immValue > 15))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && (immValue < 0 || immValue > 31))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 4 && (immValue < 0 || immValue > 63))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 5 && (immValue < 0 || immValue > 3))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 6 && (immValue < 0 || immValue > 1))
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());

	if (opData.uniform == 0 || opData.uniform == 3 || opData.uniform ==9)
	{
	  opcode.shiftopL(4);
	}

        if (opData.uniform == 8)
        {
          opcode.shiftopL(7);
        }

        if (opData.uniform == 7)
        {
	  opcode.shiftopL(6);
        }

        if (opData.uniform == 5)
        {
          opcode.shiftopL(1);
        }

        if (opData.uniform == 1)
        {
          opcode.shiftopL(2);
        }

        if (opData.uniform == 2)
        {
          opcode.shiftopL(3);
        }

        if (opData.uniform == 4)
        {
          opcode.shiftopL(5);
        }

        if (immValue < 0 && opData.uniform == 0)
	{
	  //immValue &= 0x1F;
	  aImm &= 0x1F;
	  opcode.addImm(aImm, opData.aImmOffset);
	}

        if (immValue >= 0)
	{
	  opcode.addImm(aImm, opData.aImmOffset);
	}

        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXXI: {
      const InstDB::EncodingData::lasxXXI& opData = InstDB::EncodingData::lasxxxi[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkValidRegs(o0, o1))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t immValue = o2.as<Imm>().valueAs<int64_t>();

        if (opData.uniform == 0 && (immValue < (-16) || immValue > 15))
	{
	  goto InvalidImmediate;
	}

        if (opData.uniform == 9 && (immValue < 0 || immValue > 31))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 8 && (immValue < 0 || immValue > 255))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 7 && (immValue < 0 || immValue > 127))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 1 && (immValue < 0 || immValue > 7))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 2 && (immValue < 0 || immValue > 15))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 3 && (immValue < 0 || immValue > 31))
        {
          goto InvalidImmediate;
        }

	if (opData.uniform == 4 && (immValue < 0 || immValue > 63))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 5 && (immValue < 0 || immValue > 3))
        {
          goto InvalidImmediate;
        }

        if (opData.uniform == 6 && (immValue < 0 || immValue > 1))
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());

	if (opData.uniform == 0 || opData.uniform == 3 || opData.uniform ==9)
	{
	  opcode.shiftopL(4);
	}

        if (opData.uniform == 8)
        {
          opcode.shiftopL(7);
        }

        if (opData.uniform == 7)
        {
	  opcode.shiftopL(6);
        }

        if (opData.uniform == 5)
        {
          opcode.shiftopL(1);
        }

        if (opData.uniform == 1)
        {
          opcode.shiftopL(2);
        }

        if (opData.uniform == 2)
        {
          opcode.shiftopL(3);
        }

        if (opData.uniform == 4)
        {
          opcode.shiftopL(5);
        }

        if (immValue < 0 && opData.uniform == 0)
	{
	  aImm &= 0x1F;
	  opcode.addImm(aImm, opData.aImmOffset);
	}

        if (immValue >= 0)
	{
	  opcode.addImm(aImm, opData.aImmOffset);
	}

        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVII: {
      const InstDB::EncodingData::lsxVII& opData = InstDB::EncodingData::lsxvii[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Imm, Imm)) {
	if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();
        uint32_t bImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o1.as<Imm>().valueAs<int64_t>();
	int64_t bimmValue = o2.as<Imm>().valueAs<int64_t>();
        if ((aimmValue > 31 || aimmValue < 0) && opData.uniform == 0)
	{
          goto InvalidImmediate;
        }

        if ((bimmValue > 31 || bimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
        opcode.addImm(bImm, opData.bImmOffset);
        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXII: {
      const InstDB::EncodingData::lasxXII& opData = InstDB::EncodingData::lasxxii[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Imm, Imm)) {
        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();
        uint32_t bImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o1.as<Imm>().valueAs<int64_t>();
        int64_t bimmValue = o2.as<Imm>().valueAs<int64_t>();
        if ((aimmValue > 31 || aimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

        if ((bimmValue > 31 || bimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
        opcode.addImm(bImm, opData.bImmOffset);
        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVV: {
      const InstDB::EncodingData::lsxVV& opData = InstDB::EncodingData::lsxvv[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Reg)) {
        if (!checkValidRegs(o0, o1))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXX: {
      const InstDB::EncodingData::lasxXX& opData = InstDB::EncodingData::lasxxx[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Reg)) {
        if (!checkValidRegs(o0, o1))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxIV: {
      const InstDB::EncodingData::lsxIV& opData = InstDB::EncodingData::lsxiv[encodingIndex];

      if (isign4 == ENC_OPS2(Imm, Reg)) {
        if (!checkValidRegs(o1))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o0.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o0.as<Imm>().valueAs<int64_t>();
        if ((aimmValue > 7 || aimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
	opcode.addReg(o1, opData.aShift);
        opcode.addImm(aImm, opData.aImmOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxIX: {
      const InstDB::EncodingData::lasxIX& opData = InstDB::EncodingData::lasxix[encodingIndex];

      if (isign4 == ENC_OPS2(Imm, Reg)) {
        if (!checkValidRegs(o1))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o0.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o0.as<Imm>().valueAs<int64_t>();
        if ((aimmValue > 7 || aimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
	opcode.addReg(o1, opData.aShift);
        opcode.addImm(aImm, opData.aImmOffset);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVR: {
      const InstDB::EncodingData::lsxVR& opData = InstDB::EncodingData::lsxvr[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Reg)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXR: {
      const InstDB::EncodingData::lasxXR& opData = InstDB::EncodingData::lasxxr[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Reg)) {
        if (!checkGpType(o1, opData.bType))
          goto InvalidInstruction;

        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }
        opcode.reset(opData.opcode());
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxRVI: {
      const InstDB::EncodingData::lsxRVI& opData = InstDB::EncodingData::lsxrvi[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkValidRegs(o1))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o2.as<Imm>().valueAs<int64_t>();

        if ((aimmValue > 15 || aimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

	if ((aimmValue > 7 || aimmValue < 0) && opData.uniform == 1)
        {
          goto InvalidImmediate;
        }

        if ((aimmValue > 3 || aimmValue < 0) && opData.uniform == 2)
        {
          goto InvalidImmediate;
        }

        if ((aimmValue > 1 || aimmValue < 0) && opData.uniform == 3)
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
	if (opData.uniform == 2)
	{
	  opcode.shiftopL(1);
	}

	if (opData.uniform == 1)
        {
          opcode.shiftopL(2);
        }

        if (opData.uniform == 0)
        {
          opcode.shiftopL(3);
        }

        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxRXI: {
      const InstDB::EncodingData::lasxRXI& opData = InstDB::EncodingData::lasxrxi[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Imm)) {
        if (!checkGpType(o0, opData.aType))
          goto InvalidInstruction;

        if (!checkValidRegs(o1))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o2.as<Imm>().valueAs<uint32_t>();
        int64_t aimmValue = o2.as<Imm>().valueAs<int64_t>();

        if ((aimmValue > 7 || aimmValue < 0) && opData.uniform == 0)
        {
          goto InvalidImmediate;
        }

        if ((aimmValue > 3 || aimmValue < 0) && opData.uniform == 1)
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
        if (opData.uniform == 0)
        {
          opcode.shiftopL(1);
        }

        opcode.addImm(aImm, opData.aImmOffset);
        opcode.addReg(o1, opData.bShift);
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglsxVI: {
      const InstDB::EncodingData::lsxVI& opData = InstDB::EncodingData::lsxvi[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Imm)) {

        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();
        int64_t immValue = o1.as<Imm>().valueAs<int64_t>();

        if (immValue > 4095 || immValue < (-4096))
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
        if (immValue >= 0)
	{
	  opcode.addImm(aImm, opData.aImmOffset);
	}
        if (immValue < 0)
	{
	  immValue &= 0x1FFF;
	  opcode.addImm(immValue, opData.aImmOffset);
	}
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodinglasxXI: {
      const InstDB::EncodingData::lasxXI& opData = InstDB::EncodingData::lasxxi[encodingIndex];

      if (isign4 == ENC_OPS2(Reg, Imm)) {

        if (!checkValidRegs(o0))
        {
          goto InvalidPhysId;
        }

        uint32_t aImm = o1.as<Imm>().valueAs<uint32_t>();
        int64_t immValue = o1.as<Imm>().valueAs<int64_t>();

        if (immValue > 4095 || immValue < (-4096))
        {
          goto InvalidImmediate;
        }

        opcode.reset(opData.opcode());
        if (immValue >= 0)
        {
          opcode.addImm(aImm, opData.aImmOffset);
        }
        if (immValue < 0)
        {
          immValue &= 0x1FFF;
          opcode.addImm(immValue, opData.aImmOffset);
        }
        opcode.addReg(o0, opData.aShift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingLfVVVI: {
      const InstDB::EncodingData::LfVVVI& opData = InstDB::EncodingData::lfVVVI[encodingIndex];

      if (isign4 == ENC_OPS4(Reg, Reg, Reg, Imm)) {
        if (!matchSignature(o0, o1, o2, instFlags))
          goto InvalidInstruction;

        uint64_t immValue = o3.as<Imm>().valueAs<uint64_t>();
        uint32_t immSize = opData.immSize;

        if (immValue >= (1u << immSize))
          goto InvalidImmediate;

        opcode.reset(opData.opcode());
        opcode.addImm(immValue, opData.immShift);
	opcode.addReg(o2, opData.cshift);
	opcode.addReg(o1, opData.bshift);
	opcode.addReg(o0, opData.ashift);
        goto EmitOp;
      }

      break;
    }

    case InstDB::kEncodingLFldst: {
      const InstDB::EncodingData::LFldst& opData = InstDB::EncodingData::lfldst[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Mem)) {
        const Mem& m = o1.as<Mem>();
        rmRel = &m;

        uint32_t opc = o0.as<Reg>().type() - Reg::kTypeVecS;
        if (opc > 2u || o0.as<Vec>().hasElementTypeOrIndex())
          goto InvalidInstruction;

        //if (!checkSignature(o0, o1))
          //goto InvalidInstruction;

        if (!checkVecId(o0, o1))
          goto InvalidPhysId;

        if (m.baseType() != Reg::kTypeGpX || m.hasIndex())
          goto InvalidAddress;

        if (m.isOffset64Bit())
          goto InvalidDisplacement;

        uint32_t offsetShift = 2u + opc;
        int32_t offset32 = m.offsetLo32() >> offsetShift;

        // Make sure we didn't lose bits by applying the mandatory offset shift.
        if (Support::shl(offset32, offsetShift) != m.offsetLo32())
          goto InvalidDisplacement;
        // Offset is encoded as a 7-bit immediate.
        if (!Support::isInt7(offset32))
          goto InvalidDisplacement;

        //if (m.isPreOrPost() && offset32 != 0) {
          //if (!opData.prePostOp)
            //goto InvalidAddress;

          //opcode.reset(uint32_t(opData.prePostOp) << 22);
          //opcode.addImm(m.isPreIndex(), 24);
        //}
        //else {
          opcode.reset(uint32_t(opData.offsetOp) << 22);
        //}
        uint32_t kk;
        if (o2._data[1] > 2047)
          {
            //kk = ((~(o2._data[1])) + (uint32_t(1)));
            kk = o2._data[1] & 0xFFF;
            //kk ^= 0x200000;
          }
        else {
          kk = o2._data[1];
          }
        //opcode.reset(uint32_t(opData.offsetOp) << 22);
        //opcode.addImm(x, opData.xOffset);

        opcode.addImm(kk, 10);
        //opcode.addImm(opc, 30);
        //opcode.addImm(, 10);
        opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp_MemBase_Rn5;
      }

      break;
    }

    case InstDB::kEncodingLPldst: {
      const InstDB::EncodingData::LPldst& opData = InstDB::EncodingData::lpldst[encodingIndex];

      if (isign4 == ENC_OPS3(Imm, Reg, Mem)) {
        const Mem& m = o1.as<Mem>();
        rmRel = &m;

	//if ( o0._data[0] != 0 && o0._data[0] != 8)  //Not same as assembler
          //goto EmitOp;
        if (!checkVecId(o0, o1))
          goto InvalidPhysId;

        if (m.baseType() != Reg::kTypeGpX || m.hasIndex())
          goto InvalidAddress;

        if (m.isOffset64Bit())
          goto InvalidDisplacement;
        opcode.reset(uint32_t(opData.offsetOp) << 22);
        uint32_t kk;
        if (o2._data[1] > 2047)
          {
            //kk = ((~(o2._data[1])) + (uint32_t(1)));
            kk = o2._data[1] & 0xFFF;
            //kk ^= 0x200000;
          }
        else {
          kk = o2._data[1];
          }
        //opcode.reset(uint32_t(opData.offsetOp) << 22);
        //opcode.addImm(x, opData.xOffset);

        opcode.addImm(kk, 10);
        //opcode.addImm(opc, 30);
        //opcode.addImm(, 10);
        //opcode.addReg(o1, 5);
        opcode.addImm(o0._data[0], 0);
        goto EmitOp_MemBase_Rn5;
      }

      break;
    }

    case InstDB::kEncodingLCldst: {
      const InstDB::EncodingData::LCldst& opData = InstDB::EncodingData::lcldst[encodingIndex];

      if (isign4 == ENC_OPS3(Imm, Reg, Mem)) {
        const Mem& m = o1.as<Mem>();
        rmRel = &m;

        //if ( o0._data[0] != 0 && o0._data[0] != 8)
          //goto EmitOp;
        if (!checkVecId(o0, o1))
          goto InvalidPhysId;

        if (m.baseType() != Reg::kTypeGpX || m.hasIndex())
          goto InvalidAddress;

        if (m.isOffset64Bit())
          goto InvalidDisplacement;
        opcode.reset(uint32_t(opData.offsetOp) << 22);
        uint32_t kk;
        if (o2._data[1] > 2047)
          {
            //kk = ((~(o2._data[1])) + (uint32_t(1)));
            kk = o2._data[1] & 0xFFF;
            //kk ^= 0x200000;
          }
        else {
          kk = o2._data[1];
          }
        //opcode.reset(uint32_t(opData.offsetOp) << 22);
        //opcode.addImm(x, opData.xOffset);

	opcode.addImm(kk, 10);
        //opcode.addImm(opc, 30);
        //opcode.addImm(, 10);
        //opcode.addReg(o1, 5);
        opcode.addImm(o0._data[0], 0);
        goto EmitOp_MemBase_Rn5;
      }

      break;
    }

    case InstDB::kEncodingLFPldst: {
      const InstDB::EncodingData::LFPldst& opData = InstDB::EncodingData::lfpldst[encodingIndex];

      if (isign4 == ENC_OPS3(Reg, Reg, Mem)) {
        const Mem& m = o1.as<Mem>();
        rmRel = &m;

        //if ( o0._data[0] != 0 && o0._data[0] != 8)
          //goto EmitOp;
        if (!checkVecId(o0, o1))
          goto InvalidPhysId;

        if (m.baseType() != Reg::kTypeGpX || m.hasIndex())
          goto InvalidAddress;

        if (m.isOffset64Bit())
          goto InvalidDisplacement;
        opcode.reset(uint32_t(opData.offsetOp) << 24);
	if ((o2._data[1] & 0x3) != 0)
	  {
             printf("require imm low 2 bit is 0.");
	     goto EmitOp;
          }
        uint32_t kk;
        if (o2._data[1] > 32764)
          {
            //kk = ((~(o2._data[1])) + (uint32_t(1)));
            kk = o2._data[1] & 0xFFFF;
            //kk ^= 0x200000;
          }
        else {
          kk = o2._data[1];
          }
	kk >>= 2;
        //opcode.reset(uint32_t(opData.offsetOp) << 22);
        //opcode.addImm(x, opData.xOffset);

	opcode.addImm(kk, 10);
        //opcode.addImm(opc, 30);
        //opcode.addImm(, 10);
        //opcode.addReg(o1, 5);
        opcode.addReg(o0, 0);
        goto EmitOp_MemBase_Rn5;
      }

      break;
   }

   default:
      break;
 }

  goto InvalidInstruction;

/*
EmitOp_Rd0:
  if (!checkValidRegs(o0))
    goto InvalidPhysId;

  opcode.addReg(o0, 0);
  goto EmitOp;

EmitOp_Rn5:
  if (!checkValidRegs(o0))
    goto InvalidPhysId;

  opcode.addReg(o0, 5);
  goto EmitOp;

EmitOp_Rn5_Rm16:
  if (!checkValidRegs(o0, o1))
    goto InvalidPhysId;

  opcode.addReg(o0, 5);
  opcode.addReg(o1, 16);
  goto EmitOp;

EmitOp_Rd0_Rn5:
  if (!checkValidRegs(o0, o1))
    goto InvalidPhysId;

  opcode.addReg(o0, 0);
  opcode.addReg(o1, 5);
  goto EmitOp;

EmitOp_Rd0_Rn5_Rm16_Ra10:
  if (!checkValidRegs(o0, o1, o2, o3))
    goto InvalidPhysId;

  opcode.addReg(o0, 0);
  opcode.addReg(o1, 5);
  opcode.addReg(o2, 10);
  opcode.addReg(o3, 15);
  goto EmitOp;

EmitOp_Rd0_Rn5_Rm16:
  if (!checkValidRegs(o0, o1, o3))
    goto InvalidPhysId;

  opcode.addReg(o0, 0);
  opcode.addReg(o1, 5);
  opcode.addReg(o2, 16);
  goto EmitOp;

  // --------------------------------------------------------------------------
  // [EmitGp - Multiple]
  // --------------------------------------------------------------------------

EmitOp_Multiple:
  {
    ASMJIT_ASSERT(multipleOpCount > 0);
    err = writer.ensureSpace(this, multipleOpCount * 4u);
    if (ASMJIT_UNLIKELY(err))
      goto Failed;

    for (uint32_t i = 0; i < multipleOpCount; i++)
      writer.emit32uLE(multipleOpData[i]);

    goto EmitDone;
  }
*/
  // --------------------------------------------------------------------------
  // [EmitGp - Memory]
  // --------------------------------------------------------------------------

EmitOp_MemBase_Rn5:
  if (!checkMemBase(rmRel->as<Mem>()))
    goto InvalidAddress;

  opcode.addReg(rmRel->as<Mem>().baseId(), 5);
  goto EmitOp;

EmitOp_MemBase_Rj5:
  if (!checkMemBase(rmRel->as<Mem>()))
    goto InvalidAddress;

  opcode.addReg(rmRel->as<Mem>().baseId(), 5);
  goto EmitOp;

/*
EmitOp_MemBaseNoImm_Rn5:
  if (!checkMemBase(rmRel->as<Mem>()) || rmRel->as<Mem>().hasIndex())
    goto InvalidAddress;

  if (rmRel->as<Mem>().hasOffset())
    goto InvalidDisplacement;

  opcode.addReg(rmRel->as<Mem>().baseId(), 5);
  goto EmitOp;

EmitOp_MemBaseIndex_Rn5_Rm16:
  if (!rmRel->as<Mem>().hasBaseReg())
    goto InvalidAddress;

  if (rmRel->as<Mem>().indexId() > 30 && rmRel->as<Mem>().indexId() != Gp::kIdZr)
    goto InvalidPhysId;

  opcode.addReg(rmRel->as<Mem>().indexId(), 16);
  opcode.addReg(rmRel->as<Mem>().baseId(), 5);
  goto EmitOp;
*/

EmitOp_MemBaseIndex_Rj5_Rk10:
  if (!rmRel->as<Mem>().hasBaseReg())
    goto InvalidAddress;

  //if (rmRel->as<Mem>().indexId() > 30 && rmRel->as<Mem>().indexId() != Gp::kIdZr)
    //goto InvalidPhysId;

  opcode.addReg(rmRel->as<Mem>().indexId(), 10);
  opcode.addReg(rmRel->as<Mem>().baseId(), 5);
  goto EmitOp;

  // --------------------------------------------------------------------------
  // [EmitOp - PC Relative]
  // --------------------------------------------------------------------------

EmitOp_Rel:
  {
    if (rmRel->isLabel() || rmRel->isMem()) {
      uint32_t labelId;
      int64_t labelOffset = 0;

      if (rmRel->isLabel()) {
        labelId = rmRel->as<Label>().id();
      }
      else {
        labelId = rmRel->as<Mem>().baseId();
        labelOffset = rmRel->as<Mem>().offset();
      }

      LabelEntry* label = _code->labelEntry(labelId);
      if (ASMJIT_UNLIKELY(!label))
        goto InvalidLabel;

      if (offsetFormat.type() == OffsetFormat::kTypeAArch64_ADRP) {
        // TODO: [ARM] Always create relocation entry.
      }

      if (label->isBoundTo(_section)) {
        // Label bound to the current section.
        offsetValue = label->offset() - uint64_t(offset()) + uint64_t(labelOffset);
        goto EmitOp_DispImm;
      }
      else {
        // Record non-bound label.
        size_t codeOffset = writer.offsetFrom(_bufferData);
        LabelLink* link = _code->newLabelLink(label, _section->id(), codeOffset, intptr_t(labelOffset), offsetFormat);

        if (ASMJIT_UNLIKELY(!link))
          goto OutOfMemory;

        goto EmitOp;
      }
    }
  }

  if (rmRel->isImm()) {
    uint64_t baseAddress = _code->baseAddress();
    uint64_t targetOffset = rmRel->as<Imm>().valueAs<uint64_t>();

    size_t codeOffset = writer.offsetFrom(_bufferData);

    if (baseAddress == Globals::kNoBaseAddress || _section->id() != 0) {
      // Create a new RelocEntry as we cannot calculate the offset right now.
      RelocEntry* re;
      err = _code->newRelocEntry(&re, RelocEntry::kTypeAbsToRel);
      if (err)
        goto Failed;

      re->_sourceSectionId = _section->id();
      re->_sourceOffset = codeOffset;
      re->_format = offsetFormat;
      re->_payload = 0;
      goto EmitOp;
    }
    else {
      uint64_t pc = baseAddress + codeOffset;

      if (offsetFormat.type() == OffsetFormat::kTypeAArch64_ADRP)
        pc &= ~uint64_t(16384 - 1);

      offsetValue = targetOffset - pc;
      goto EmitOp_DispImm;
    }
  }

  goto InvalidInstruction;

EmitOp_DispImm:
  {
    if ((offsetValue & Support::lsbMask<uint32_t>(offsetFormat.immDiscardLsb())) != 0)
      goto InvalidDisplacement;

    int64_t dispImm64 = int64_t(offsetValue) >> offsetFormat.immDiscardLsb();
    if (!Support::isEncodableOffset64(dispImm64, offsetFormat.immBitCount()))
      goto InvalidDisplacement;

    uint32_t dispImm32 = uint32_t(dispImm64 & Support::lsbMask<uint32_t>(offsetFormat.immBitCount()));
    switch (offsetFormat.type()) {
      case OffsetFormat::kTypeCommon: {
        opcode.addImm(dispImm32, offsetFormat.immBitShift());
        goto EmitOp;
      }

      case OffsetFormat::kTypeAArch64_ADR:  {
        opcode.addImm(dispImm32, offsetFormat.immBitShift());
        goto EmitOp;
     }
      case OffsetFormat::kTypeAArch64_ADRP: {
        /*uint32_t immLo = dispImm32 & 0x3u;
        uint32_t immHi = dispImm32 >> 2;
        opcode.addImm(immLo, 29);
        opcode.addImm(immHi*/
        opcode.addImm(dispImm32, offsetFormat.immBitShift());
        goto EmitOp;
      }
      case OffsetFormat::kTypeLa64_BBL: {
        uint32_t imm25_16 = dispImm32 >> 16;
        uint32_t imm15_0  = dispImm32 & 0xFFFFu;
        opcode.addImm(imm15_0, 10);
        opcode.addImm(imm25_16, 0);
        goto EmitOp;
      }
      case OffsetFormat::kTypeLa64_BEQ: {
        uint32_t imm15_0  = dispImm32 & 0xFFFFu;
        opcode.addImm(imm15_0, offsetFormat.immBitShift());
        goto EmitOp;
      }

      default:
        goto InvalidDisplacement;
    }
  }

  // --------------------------------------------------------------------------
  // [EmitOp - Opcode]
  // --------------------------------------------------------------------------

EmitOp:
  writer.emit32uLE(opcode.get());
  goto EmitDone;

  // --------------------------------------------------------------------------
  // [Done]
  // --------------------------------------------------------------------------

EmitDone:
  if (ASMJIT_UNLIKELY(options & Inst::kOptionReserved)) {
#ifndef ASMJIT_NO_LOGGING
    //printf("logInstructionEmitted loongarch !!!\n");
    if (_logger)
      EmitterUtils::logInstructionEmitted(this, instId, options, o0, o1, o2, opExt, 0, 0, writer.cursor());
#endif
  }

  resetExtraReg();
  resetInstOptions();
  resetInlineComment();

  writer.done(this);
  return kErrorOk;

  // --------------------------------------------------------------------------
  // [Error Handler]
  // --------------------------------------------------------------------------

#define ERROR_HANDLER(ERR) ERR: \
  err = DebugUtils::errored(kError##ERR); \
  printf("Instruction Failed [Err: %s]: in %s(...) at %s:%d \n\n", #ERR, __FUNCTION__, __FILE__, __LINE__); \
  assert(0); \
  goto Failed;

  ERROR_HANDLER(OutOfMemory)
  ERROR_HANDLER(InvalidAddress)
  // ERROR_HANDLER(InvalidAddressScale)
  ERROR_HANDLER(InvalidDisplacement)
  // ERROR_HANDLER(InvalidElementIndex)
  ERROR_HANDLER(InvalidLabel)
  ERROR_HANDLER(InvalidImmediate)
  ERROR_HANDLER(InvalidInstruction)
  ERROR_HANDLER(InvalidPhysId)
  // ERROR_HANDLER(InvalidRegType)
#undef ERROR_HANDLER

Failed:
  printf("Instruction Failed: %s[%d], Err: kError##ERR \n", __FUNCTION__, __LINE__);
#ifndef ASMJIT_NO_LOGGING
  //printf("logInstructionFailed loongarch !!!\n");
  return EmitterUtils::logInstructionFailed(this, err, instId, options, o0, o1, o2, opExt);
#else
  resetExtraReg();
  resetInstOptions();
  resetInlineComment();
  return reportError(err);
#endif
}

#undef ENC_OPS1
#undef ENC_OPS2
#undef ENC_OPS3
#undef ENC_OPS4

// ============================================================================
// [asmjit::Assembler - Align]
// ============================================================================

Error Assembler::align(uint32_t alignMode, uint32_t alignment) {
  if (ASMJIT_UNLIKELY(!_code))
    return reportError(DebugUtils::errored(kErrorNotInitialized));

  if (ASMJIT_UNLIKELY(alignMode >= kAlignCount))
    return reportError(DebugUtils::errored(kErrorInvalidArgument));

  if (alignment <= 1)
    return kErrorOk;

  if (ASMJIT_UNLIKELY(alignment > Globals::kMaxAlignment || !Support::isPowerOf2(alignment)))
    return reportError(DebugUtils::errored(kErrorInvalidArgument));

  uint32_t i = uint32_t(Support::alignUpDiff<size_t>(offset(), alignment));

  if (i == 0)
    return kErrorOk;

  CodeWriter writer(this);
  ASMJIT_PROPAGATE(writer.ensureSpace(this, i));

  constexpr uint32_t kNopT16 = 0x0000BF00u; // [10111111|00000000].
  constexpr uint32_t kNopT32 = 0xF3AF8000u; // [11110011|10101111|10000000|00000000].
  constexpr uint32_t kNopA32 = 0xE3AF8000u; // [Cond0011|00100000|11110000|00000000].
  constexpr uint32_t kNopA64 = 0xD503201Fu; // [11010101|00000011|00100000|00011111].

  switch (alignMode) {
    case kAlignCode: {
      uint32_t pattern = 0;
      if (is64Bit()) {
        pattern = kNopA64;
      }
      else if (isInThumbMode()) {
        if (ASMJIT_UNLIKELY(offset() & 0x1u))
          return DebugUtils::errored(kErrorInvalidState);

        if (i & 0x2) {
          writer.emit16uLE(kNopT16);
          i -= 2;
        }
        pattern = kNopT32;
      }
      else {
        pattern = kNopA32;
      }

      if (ASMJIT_UNLIKELY(offset() & 0x3u))
        return DebugUtils::errored(kErrorInvalidState);

      while (i >= 4) {
        writer.emit32uLE(pattern);
        i -= 4;
      }

      ASMJIT_ASSERT(i == 0);
      break;
    }

    case kAlignData:
    case kAlignZero:
      writer.emitZeros(i);
      break;
  }

  writer.done(this);

#ifndef ASMJIT_NO_LOGGING
  if (_logger) {
    StringTmp<128> sb;
    sb.appendChars(' ', _logger->indentation(FormatOptions::kIndentationCode));
    sb.appendFormat("align %u\n", alignment);
    _logger->log(sb);
  }
#endif

  return kErrorOk;
}

// ============================================================================
// [asmjit::Assembler - Events]
// ============================================================================

Error Assembler::onAttach(CodeHolder* code) noexcept {
  uint32_t arch = code->arch();
  if (!Environment::isFamilyLOONGARCH(arch))
    return DebugUtils::errored(kErrorInvalidArch);

  return Base::onAttach(code);
}

Error Assembler::onDetach(CodeHolder* code) noexcept {
  return Base::onDetach(code);
}

ASMJIT_END_SUB_NAMESPACE

#endif // !ASMJIT_NO_LOONG
