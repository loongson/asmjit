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

#ifndef ASMJIT_LOONG_LA64INSTDB_H_P_INCLUDED
#define ASMJIT_LOONG_LA64INSTDB_H_P_INCLUDED

#include "../loong/la64instdb.h"
#include "../loong/la64operand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(la64)

//! \cond INTERNAL
//! \addtogroup asmjit_la64
//! \{

namespace InstDB {

// ============================================================================
// [asmjit::la64::InstDB - Constants Used by Instructions]
// ============================================================================

// GP register types supported by base instructions.
static constexpr uint32_t kW = 0x1;
static constexpr uint32_t kX = 0x2;
static constexpr uint32_t kWX = 0x3;
//static constexpr uint32_t kB = 0x1;
// GP high register IDs supported by the instruction.
static constexpr uint32_t kZR = Gp::kIdZr;
static constexpr uint32_t kSP = Gp::kIdSp;

// ============================================================================
// [asmjit::a64::InstDB - RWInfo]
// ============================================================================

enum RWInfoType : uint32_t {
  kRWI_R,
  kRWI_RW,
  kRWI_RX,
  kRWI_RRW,
  kRWI_RWX,
  kRWI_W,
  kRWI_WRW,
  kRWI_WRX,
  kRWI_WRRW,
  kRWI_WRRX,
  kRWI_WW,
  kRWI_X,
  kRWI_XRX,
  kRWI_XXRRX,

  kRWI_LDn,
  kRWI_STn,

  kRWI_SpecialStart = kRWI_LDn
};

// ============================================================================
// [asmjit::a64::InstDB - ElementType]
// ============================================================================

enum ElementType : uint8_t {
  kET_None = Vec::kElementTypeNone,
  kET_B    = Vec::kElementTypeB,
  kET_H    = Vec::kElementTypeH,
  kET_S    = Vec::kElementTypeS,
  kET_D    = Vec::kElementTypeD,
  kET_2H   = Vec::kElementTypeH2,
  kET_4B   = Vec::kElementTypeB4
};
// ============================================================================
// [asmjit::a64::InstDB - GpType]
// ============================================================================

enum GpType : uint8_t {
  kGp_W,
  kGp_X,
  kGp_X_SP
};

// ============================================================================
// [asmjit::a64::InstDB - OPSig]
// ============================================================================

enum kOpSignature : uint32_t {
  kOp_GpW = GpW::kSignature,
  kOp_GpX = GpX::kSignature,

  kOp_B = VecB::kSignature,
  kOp_H = VecH::kSignature,
  kOp_S = VecS::kSignature,
  kOp_D = VecD::kSignature,
  kOp_Q = VecV::kSignature,

  kOp_V8B = VecD::kSignature | Vec::kSignatureElementB,
  kOp_V4H = VecD::kSignature | Vec::kSignatureElementH,
  kOp_V2S = VecD::kSignature | Vec::kSignatureElementS,

  kOp_V16B = VecV::kSignature | Vec::kSignatureElementB,
  kOp_V8H = VecV::kSignature | Vec::kSignatureElementH,
  kOp_V4S = VecV::kSignature | Vec::kSignatureElementS,
  kOp_V2D = VecV::kSignature | Vec::kSignatureElementD
};

// ============================================================================
// [asmjit::a64::InstDB - HFConv]
// ============================================================================

enum kHFConv : uint32_t {
  //! FP16 version of the instruction is not available.
  kHF_N,

  //! Doesn't do any change to the opcode.
  kHF_0,

  kHF_A,
  kHF_B,
  kHF_C,
  kHF_D,

  kHF_Count
};

// ============================================================================
// [asmjit::a64::InstDB - VOType]
// ============================================================================

//! Vector operand type combinations used by FP&SIMD instructions.
enum VOType : uint32_t {
  kVO_V_B,
  kVO_V_BH,
  kVO_V_BH_4S,
  kVO_V_BHS,
  kVO_V_BHS_D2,
  kVO_V_HS,
  kVO_V_S,

  kVO_V_B8H4,
  kVO_V_B8H4S2,
  kVO_V_B8D1,
  kVO_V_H4S2,

  kVO_V_B16,
  kVO_V_B16H8,
  kVO_V_B16H8S4,
  kVO_V_B16D2,
  kVO_V_H8S4,
  kVO_V_S4,
  kVO_V_D2,

  kVO_SV_BHS,
  kVO_SV_B8H4S2,
  kVO_SV_HS,
  kVO_V_Any,
  kVO_SV_Any,

  kVO_Count
};

// ============================================================================
// [asmjit::a64::InstDB - EncodingId]
// ============================================================================

// ${EncodingId:Begin}
// ------------------- Automatically generated, do not edit -------------------
enum EncodingId : uint32_t {
  kEncodingNone = 0,
  kEncodingBaseOp,
  kEncodingBaseOpImm,
  kEncodingBaseLRR,
  kEncodingBaseLRRR,
  kEncodingFpLVVVV,
  kEncodingBTLRRI,
  kEncodingJBTLRRI,
  kEncodingBaseBLI,
  kEncodingBaseBranchRel,
  kEncodingBaseLRRI,
  kEncodingBaseLldst,
  kEncodingBaseLdSt,
  kEncodingSimdLdst,
  kEncodingLFldst,
  kEncodingLPldst,
  kEncodingLCldst,
  kEncodingLFPldst,
  kEncodingBaseLRRIL,
  kEncodingBaseLRI,
  kEncodingBaseLRRL,
  kEncodingBaseLRRRI,
  kEncodingBaseLIC,
  kEncodingBaseLRRII,
  kEncodingBaseLIR,
  kEncodingBaseLIV,
  kEncodingBaseLVI,
  kEncodingBaseLIRR,
  kEncodingBaseLFIVV,
  kEncodingLfVVVI,
  kEncodingBaseLII,
  kEncodingBaseLVV,
  kEncodingBaseLRV,
  kEncodingBaseLVR,
  kEncodingBaseLVVV,
  kEncodingBaseLVRR,
  kEncodingBaseLRRRT,
  kEncodinglsxVVVV,
  kEncodinglasxXXXX,
  kEncodinglsxVVV,
  kEncodinglasxXXX,
  kEncodinglsxVRI,
  kEncodinglasxXRI,
  kEncodinglsxVRII,
  kEncodinglasxXRII,
  kEncodinglsxVRR,
  kEncodinglasxXRR,
  kEncodinglsxVVR,
  kEncodinglasxXXR,
  kEncodinglsxVVI,
  kEncodinglasxXXI,
  kEncodinglsxVII,
  kEncodinglasxXII,
  kEncodinglsxVV,
  kEncodinglasxXX,
  kEncodinglsxIV,
  kEncodinglasxIX,
  kEncodinglsxVR,
  kEncodinglasxXR,
  kEncodinglsxRVI,
  kEncodinglasxRXI,
  kEncodinglsxVI,
  kEncodinglasxXI
};
// ----------------------------------------------------------------------------
// ${EncodingId:End}

// ============================================================================
// [asmjit::a64::InstDB::EncodingData]
// ============================================================================

namespace EncodingData {

#define M_OPCODE(field, bits) \
  uint32_t _##field : bits; \
  inline constexpr uint32_t field() const noexcept { return uint32_t(_##field) << (32 - bits); }

struct BaseOp {
  uint32_t opcode;
};

struct BaseOpImm {
  //uint32_t opcode;
  M_OPCODE(opcode, 17)
  uint16_t immBits;
  uint16_t immOffset;
};

struct BaseLIC {
  M_OPCODE(opcode, 17)
  uint32_t immOffset : 5;
};

struct BaseLRI {
  //uint32_t opcode;
  M_OPCODE(opcode, 22)
  uint32_t rType : 8;
  //uint32_t rHiId : 8;
  uint32_t rShift : 8;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 3;
};

struct BaseLIR {
  //uint32_t opcode;
  M_OPCODE(opcode, 22)
  uint32_t aType : 2;
  uint32_t aShift : 5;
  //uint32_t aImmSize : 3;
  uint32_t aImmOffset : 5;
};

struct BaseLIV {
  M_OPCODE(opcode, 22)
  uint32_t aShift : 5;
  uint32_t aImmOffset : 5;
};

typedef BaseLIV BaseLVI;

struct BaseLRR {
  //uint32_t opcode;
  M_OPCODE(opcode, 22)
  uint32_t aType : 2;
  //uint32_t aHiId : 6;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  //uint32_t bHiId : 6;
  uint32_t bShift : 5;
  uint32_t uniform : 1;
};

struct BaseLVV {
  M_OPCODE(opcode, 22)
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t uniform : 1;
};

struct BaseLRV {
  M_OPCODE(opcode, 22)
  uint32_t aType : 2;
  //uint32_t aHiId : 6;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t uniform : 1;
};

struct BaseLVR {
  M_OPCODE(opcode, 22)
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  uint32_t uniform : 1;
};

struct BaseLRRL {
  //uint32_t opcode;
  M_OPCODE(opcode, 17)
  uint32_t aType : 2;
  //uint32_t aHiId : 6;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  //uint32_t bHiId : 6;
  uint32_t bShift : 5;
  //uint32_t uniform : 1;
};

struct BaseLRRRI {
  M_OPCODE(opcode, 15)
  uint32_t aType : 2;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  uint32_t cType : 2;
  uint32_t cShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 2;
};

//typedef BaseLRRR BaseLRRRT;

struct BaseLRRR {
  M_OPCODE(opcode, 17)
  //uint32_t opcode;
  uint32_t aType : 2;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  uint32_t cType : 2;
  uint32_t cShift : 5;
  uint32_t uniform : 1;
};

struct BaseLVRR {
  M_OPCODE(opcode, 17)
  //uint32_t opcode;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  uint32_t cType : 2;
  uint32_t cShift : 5;
  uint32_t uniform : 1;
};

struct BaseLVVV {
  M_OPCODE(opcode, 17)
  //uint32_t opcode;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t cShift : 5;
  uint32_t uniform : 1;
};

struct BaseLII {
  M_OPCODE(opcode, 6)
  uint32_t aImmSize : 5;
  uint32_t aImmOffset : 5;
  uint32_t bImmSize : 3;
  uint32_t bImmOffset : 5;
  uint32_t cImmSize : 16;
  uint32_t cImmOffset : 5;
  uint32_t uniform : 1;
};

struct BaseLRRII {
  M_OPCODE(opcode, 11)
  uint32_t aType : 2;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  //uint32_t aImmSize : 6;
  //uint32_t aImmDiscardLsb : 5;
  uint32_t aImmOffset : 5;
  //uint32_t bImmSize : 6;
  //uint32_t bImmDiscardLsb : 5;
  uint32_t bImmOffset : 5;
  uint32_t uniform : 2;
};

struct BaseLRRI {
  M_OPCODE(opcode, 17)
  uint32_t aType : 2;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 3;
};

struct BaseLFIVV {
  M_OPCODE(opcode, 17)
  uint32_t aImmOffset : 5;
  uint32_t bShift : 5;
  uint32_t cShift : 5;
  uint32_t uniform : 2;
};

typedef BaseLRRI BaseLIRR;

struct BaseLRRIL {
  M_OPCODE(opcode, 6)
  uint32_t aType : 2;
  uint32_t aHiId : 6;
  uint32_t aImmOffset : 5;
};

struct BaseLldst {
  uint32_t offsetOp : 10;
  uint32_t prePostOp : 10;
  uint32_t rType : 2;
  //uint32_t xOffset : 5;
  uint32_t offsetShift : 5;
};

struct BaseLdSt {
  uint32_t _opcode;
  uint32_t opShift : 5;
  constexpr uint32_t opcode() const noexcept { return _opcode << opShift; }
};

typedef BaseLRRR BaseLRRRT;
//typedef BaseOp BTLRRI;

struct FpLVVVV {
  uint32_t _opcode;
  constexpr uint32_t opcode() const noexcept { return _opcode << 20; }
};

struct lsxVVVV {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t cShift : 5;
  uint32_t dShift : 5;
  uint32_t uniform : 2;
  constexpr uint32_t opcode() const noexcept { return _opcode << 20; }
};

struct lsxVVV {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t cShift : 5;
  uint32_t uniform : 2;
  constexpr uint32_t opcode() const noexcept { return _opcode << 15; }
};

struct lsxVRI {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType: 2;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 4;
  constexpr uint32_t opcode() const noexcept { return _opcode << 11; }
};

struct lasxXRI {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType: 2;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 4;
  constexpr uint32_t opcode() const noexcept { return _opcode << 12; }
};

struct SimdLdst {
  uint32_t _opcode;
  uint32_t opShift: 5;
  uint32_t offsetLen: 5;
  constexpr uint32_t opcode() const noexcept { return _opcode << opShift; }
};

struct lsxVRII {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType: 2;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t bImmOffset : 5;
  uint32_t uniform : 3;
  constexpr uint32_t opcode() const noexcept { return _opcode << 19; }
};

struct lasxXRII {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType: 2;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t bImmOffset : 5;
  uint32_t uniform : 3;
  constexpr uint32_t opcode() const noexcept { return _opcode << 20; }
};

struct lsxVRR {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType: 2;
  uint32_t bShift : 5;
  uint32_t cType : 2;
  uint32_t cShift : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 15; }
};

struct lasxXRR {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType: 2;
  uint32_t bShift : 5;
  uint32_t cType : 2;
  uint32_t cShift : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 15; }
};

struct lsxVVR {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t cType : 2;
  uint32_t cShift : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 15; }
};

struct lsxVVI {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 4;
  constexpr uint32_t opcode() const noexcept { return _opcode << 11; }
};

struct lsxVII {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t bImmOffset : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 15; }
};

struct lsxVV {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 10; }
};

struct lsxVR {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t bType : 2;
  uint32_t bShift : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 10; }
};

struct lsxIV {
  uint32_t _opcode;
  uint32_t aImmOffset : 5;
  uint32_t aShift : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 10; }
};

struct lsxRVI {
  uint32_t _opcode;
  uint32_t aType : 2;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 2;
  constexpr uint32_t opcode() const noexcept { return _opcode << 11; }
};

struct lasxRXI {
  uint32_t _opcode;
  uint32_t aType : 2;
  uint32_t aShift : 5;
  uint32_t bShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 12; }
};

struct lsxVI {
  uint32_t _opcode;
  uint32_t aShift : 5;
  uint32_t aImmOffset : 5;
  uint32_t uniform : 1;
  constexpr uint32_t opcode() const noexcept { return _opcode << 18; }
};

typedef lsxVVVV lasxXXXX;
typedef lsxVVV lasxXXX;
typedef lsxVVR lasxXXR;
typedef lsxVVI lasxXXI;
typedef lsxVII lasxXII;
typedef lsxVV lasxXX;
typedef lsxIV lasxIX;
typedef lsxVR lasxXR;
typedef lsxVI lasxXI;
//typedef lsxVRII lasxXRII;
//typedef lsxVRI lasxXRI;
struct BTLRRI {
  uint32_t _opcode;
  constexpr uint32_t opcode() const noexcept { return _opcode << 26; }
};

struct BaseBLI {
  uint32_t _opcode;
  //uint32_t btypel : 2;
  constexpr uint32_t opcode() const noexcept { return _opcode << 26; }
};

struct BaseBranchRel {
  uint32_t _opcode;
  uint32_t offsetType : 10;
  constexpr uint32_t opcode() const noexcept { return _opcode << 26; }
};

struct JBTLRRI {
  uint32_t _opcode;
  constexpr uint32_t opcode() const noexcept { return _opcode << 26; }
};

struct LfVVVI {
  M_OPCODE(opcode, 14)
  uint32_t ashift : 5;
  uint32_t bshift : 5;
  uint32_t cshift : 5;
  uint32_t immSize : 4;
  uint32_t immShift : 4;
  uint32_t uniform : 2;
};

struct LFldst {
  uint32_t offsetOp : 10;
};

typedef LFldst LPldst;
typedef LFldst LCldst;

struct LFPldst {
  uint32_t offsetOp : 8;
};

#undef M_OPCODE

// ${EncodingDataForward:Begin}
// ------------------- Automatically generated, do not edit -------------------
extern const BaseLldst baseLldst[11];
extern const BaseLdSt baseLdst[30];
extern const BaseOp baseOp[7];
extern const BaseOpImm baseOpImm[3];
extern const BaseLRR baseLRR[34];
extern const BaseLRRI baseLRRI[18];
extern const BaseLRRIL baseLRRIL[1];
extern const BaseLRRR baseLRRR[73];
extern const FpLVVVV fpLVVVV[2];
extern const BTLRRI btLRRI[6];
extern const JBTLRRI jbtLRRI[1];
extern const BaseBLI basebLI[2];
extern const BaseBranchRel baseBranchRel[8];
extern const LFldst lfldst[4];
extern const LPldst lpldst[1];
extern const LCldst lcldst[1];
extern const LFPldst lfpldst[8];
extern const BaseLRI baseLRI[10];
extern const BaseLRRL baseLRRL[2];
extern const BaseLRRRI baseLRRRI[5];
extern const BaseLIC baseLIC[3];
extern const BaseLRRII baseLRRII[4];
extern const BaseLIR baseLIR[1];
extern const BaseLIV baseLIV[1];
extern const BaseLVI baseLVI[1];
extern const BaseLIRR baseLIRR[1];
extern const BaseLFIVV baseLFIVV[22];
extern const LfVVVI lfVVVI[1];
extern const BaseLII baseLII[2];
extern const BaseLVV baseLVV[44];
extern const BaseLRV baseLRV[3];
extern const BaseLVR baseLVR[3];
extern const BaseLVVV baseLVVV[20];
extern const BaseLVRR baseLVRR[12];
extern const BaseLRRRT baseLRRRT[36];
extern const lsxVVVV lsxvvvv[10];
extern const lasxXXXX lasxxxxx[10];
extern const lsxVVV lsxvvv[411];
extern const lasxXXX lasxxxx[412];
extern const lsxVRI lsxvri[4];
extern const lasxXRI lasxxri[2];
extern const SimdLdst simdLdst[16];
extern const lsxVRII lsxvrii[4];
extern const lasxXRII lasxxrii[4];
extern const lsxVRR lsxvrr[2];
extern const lasxXRR lasxxrr[2];
extern const lsxVVR lsxvvr[4];
extern const lasxXXR lasxxxr[4];
extern const lsxVVI lsxvvi[160];
extern const lasxXXI lasxxxi[171];
extern const lsxVII lsxvii[1];
extern const lasxXII lasxxii[1];
extern const lsxVV lsxvv[86];
extern const lasxXX lasxxx[103];
extern const lsxIV lsxiv[10];
extern const lasxIX lasxix[10];
extern const lsxVR lsxvr[4];
extern const lasxXR lasxxr[4];
extern const lsxRVI lsxrvi[8];
extern const lasxRXI lasxrxi[4];
extern const lsxVI lsxvi[1];
extern const lasxXI lasxxi[1];
// ----------------------------------------------------------------------------
// ${EncodingDataForward:End}

} // {EncodingData}

// ============================================================================
// [asmjit::a64::InstDB - InstNameIndex]
// ============================================================================

// ${NameLimits:Begin}
// ------------------- Automatically generated, do not edit -------------------
enum : uint32_t { kMaxNameSize = 9 };
// ----------------------------------------------------------------------------
// ${NameLimits:End}

struct InstNameIndex {
  uint16_t start;
  uint16_t end;
};

// ============================================================================
// [asmjit::a64::InstDB - Tables]
// ============================================================================

#ifndef ASMJIT_NO_TEXT
extern const char _nameData[];
extern const InstNameIndex instNameIndex[26];
#endif // !ASMJIT_NO_TEXT

} // {InstDB}

//! \}
//! \endcond

ASMJIT_END_SUB_NAMESPACE

#endif // ASMJIT_LA64_LOONGINSTDB_H_P_INCLUDED
