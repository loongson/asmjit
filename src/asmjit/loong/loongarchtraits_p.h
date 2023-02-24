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

#ifndef ASMJIT_LOONG_LOONGARCHTRAITS_P_H_INCLUDED
#define ASMJIT_LOONG_LOONGARCHTRAITS_P_H_INCLUDED

#include "../core/archtraits.h"
#include "../core/misc_p.h"
#include "../core/type.h"
#include "../loong/loongoperand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(loong)

//! \cond INTERNAL
//! \addtogroup asmjit_loong
//! \{

// ============================================================================
// [asmjit::loong::a64ArchTraits
// ============================================================================

static const constexpr ArchTraits la64ArchTraits = {
  // SP/FP/LR/PC.
  //Gp::kIdSp, Gp::kIdFp, Gp::kIdLr, 0xFF,
  Gp::kIdSp, Gp::kIdFp, Gp::kIdRa, 0xFF,

  // Reserved.
  { 0, 0, 0 },

  // HW stack alignment (AArch64 requires stack aligned to 64 bytes).
  //16,
  1,

  // Min/max stack offset - byte addressing is the worst, VecQ addressing the best.
  4095, 65520,

  // ISA features [Gp, Vec, Other0, Other1].
  //{ ArchTraits::kIsaFeaturePushPop, ArchTraits::kIsaFeaturePushPop, 0, 0 },
  { 0, 0, 0, 0 },

  // RegInfo.
  #define V(type) { loong::RegTraits<type>::kSignature }
  { ASMJIT_LOOKUP_TABLE_32(V, 0) },
  #undef V

  // RegTypeToTypeId.
  #define V(type) loong::RegTraits<type>::kTypeId
  { ASMJIT_LOOKUP_TABLE_32(V, 0) },
  #undef V

  // TypeIdToRegType.
  #define V(index) (index + Type::_kIdBaseStart == Type::kIdI8      ? Reg::kTypeGpW   : \
                    index + Type::_kIdBaseStart == Type::kIdU8      ? Reg::kTypeGpW   : \
                    index + Type::_kIdBaseStart == Type::kIdI16     ? Reg::kTypeGpW   : \
                    index + Type::_kIdBaseStart == Type::kIdU16     ? Reg::kTypeGpW   : \
                    index + Type::_kIdBaseStart == Type::kIdI32     ? Reg::kTypeGpW   : \
                    index + Type::_kIdBaseStart == Type::kIdU32     ? Reg::kTypeGpW   : \
                    index + Type::_kIdBaseStart == Type::kIdI64     ? Reg::kTypeGpX   : \
                    index + Type::_kIdBaseStart == Type::kIdU64     ? Reg::kTypeGpX   : \
                    index + Type::_kIdBaseStart == Type::kIdIntPtr  ? Reg::kTypeGpX   : \
                    index + Type::_kIdBaseStart == Type::kIdUIntPtr ? Reg::kTypeGpX   : \
                    index + Type::_kIdBaseStart == Type::kIdF32     ? Reg::kTypeVecS  : \
                    index + Type::_kIdBaseStart == Type::kIdF64     ? Reg::kTypeVecD  : Reg::kTypeNone)
  { ASMJIT_LOOKUP_TABLE_32(V, 0) },
  #undef V

  // Word names of 8-bit, 16-bit, 32-bit, and 64-bit quantities.
  {
    ISAWordNameId::kByte,
    ISAWordNameId::kHWord,
    ISAWordNameId::kWord,
    ISAWordNameId::kXWord
  }

};

//! \}
//! \endcond

ASMJIT_END_SUB_NAMESPACE

#endif // ASMJIT_LOONG_LOONGARCHTRAITS_P_H_INCLUDED
