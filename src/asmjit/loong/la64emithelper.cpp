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

#include "../core/api-build_p.h"
#if !defined(ASMJIT_NO_LOONG)

#include "../core/formatter.h"
#include "../core/funcargscontext_p.h"
#include "../core/string.h"
#include "../core/support.h"
#include "../core/type.h"
#include "../loong/la64emithelper_p.h"
#include "../loong/la64operand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(la64)

// ============================================================================
// [asmjit::la64::EmitHelper - Emit Operations]
// ============================================================================
#define EMIT_LD(df, dst, src) \
{ \
    if(BaseReg::kTypeNone == src.indexType()) { \
        return emitter->ld_##df(dst.as<Gp>(), src); \
    } else { \
        return emitter->ldx_##df(dst.as<Gp>(), src); \
    } \
}

#define EMIT_ST(df, dst, src) \
{ \
    if(BaseReg::kTypeNone == dst.indexType()) { \
        return emitter->st_##df(src.as<Gp>(), dst); \
    } else { \
        return emitter->stx_##df(src.as<Gp>(), dst); \
    } \
}

#define EMIT_FLD(df, dst, src) \
{ \
    if(BaseReg::kTypeNone == src.indexType()) { \
        return emitter->fld_##df(dst.as<Vec>(), src); \
    } else { \
        return emitter->fldx_##df(dst.as<Vec>(), src); \
    } \
}

#define EMIT_FST(df, dst, src) \
{ \
    if(BaseReg::kTypeNone == dst.indexType()) { \
        return emitter->fst_##df(src.as<Vec>(), dst); \
    } else { \
        return emitter->fstx_##df(src.as<Vec>(), dst); \
    } \
}

#define EMIT_SIMD_LD(act, dst, src) \
{ \
    if(BaseReg::kTypeNone == src.indexType()) { \
        return emitter->act(dst.as<Vec>(), src); \
    } else { \
        return emitter->act##x(dst.as<Vec>(), src); \
    } \
}

#define EMIT_SIMD_ST(act, dst, src) \
{ \
    if(BaseReg::kTypeNone == dst.indexType()) { \
        return emitter->act(src.as<Vec>(), dst); \
    } else { \
        return emitter->act##x(src.as<Vec>(), dst); \
    } \
}

ASMJIT_FAVOR_SIZE Error EmitHelper::emitRegMove(
  const Operand_& dst_,
  const Operand_& src_, uint32_t typeId, const char* comment) {

  Emitter* emitter = _emitter->as<Emitter>();

  // Invalid or abstract TypeIds are not allowed.
  ASMJIT_ASSERT(Type::isValid(typeId) && !Type::isAbstract(typeId));

  emitter->setInlineComment(comment);

  if (dst_.isReg() && src_.isMem()) {
    Reg dst(dst_.as<Reg>());
    Mem src(src_.as<Mem>());

    switch (typeId) {
      case Type::kIdI8:
      case Type::kIdU8:
        //return emitter->ld_b(dst.as<Gp>(), r0, src);
        EMIT_LD(b, dst, src);

      case Type::kIdI16:
      case Type::kIdU16:
        //return emitter->ld_h(dst.as<Gp>(), r0, src);
        EMIT_LD(h, dst, src);

      case Type::kIdI32:
      case Type::kIdU32:
        //return emitter->ld_w(dst.as<Gp>().w(), r0, src);
        EMIT_LD(w, dst, src);

      case Type::kIdI64:
      case Type::kIdU64:
        //return emitter->ld_d(dst.as<Gp>().w(), r0, src);
        EMIT_LD(d, dst, src);

      default: {
        if (Type::isVec32(typeId))
          //return emitter->ldr(dst.as<Vec>().s(), src);
          EMIT_FLD(s, dst, src);

        if (Type::isVec64(typeId))
          //return emitter->ldr(dst.as<Vec>().d(), src);
          EMIT_FLD(d, dst, src);

        if (Type::isVec128(typeId))
          //return emitter->ldr(dst.as<Vec>().q(), src);
          EMIT_SIMD_LD(vld, dst, src);

	    if (Type::isVec256(typeId))
	      ////return emitter->xvld(dst.as<Vec>().lx(), r0, src_.as<Imm>());
          EMIT_SIMD_LD(xvld, dst, src);

        break;
      }
    }
  }

  if (dst_.isMem() && src_.isReg()) {
    Mem dst(dst_.as<Mem>());
    Reg src(src_.as<Reg>());

    switch (typeId) {
      case Type::kIdI8:
      case Type::kIdU8:
        //return emitter->st_b(src.as<Gp>(), r0, dst);
        EMIT_ST(b, dst, src);

      case Type::kIdI16:
      case Type::kIdU16:
        //return emitter->st_h(src.as<Gp>(), r0, dst);
        EMIT_ST(h, dst, src);

      case Type::kIdI32:
      case Type::kIdU32:
        //return emitter->st_w(src.as<Gp>().w(), r0, dst);
        EMIT_ST(w, dst, src);

      case Type::kIdI64:
      case Type::kIdU64:
        //return emitter->st_d(src.as<Gp>().w(), r0, dst);
        EMIT_ST(d, dst, src);

      default: {
        if (Type::isVec32(typeId))
          //return emitter->str(src.as<Vec>().s(), dst);
          EMIT_FST(s, dst, src);

        if (Type::isVec64(typeId))
          //return emitter->str(src.as<Vec>().d(), dst);
          EMIT_FST(d, dst, src);

        if (Type::isVec128(typeId))
          //return emitter->vst(src.as<Vec>().q(), r0, Imm(dst));
          EMIT_SIMD_ST(vst, dst, src);

	    if (Type::isVec256(typeId))
	      //return emitter->xvst(src.as<Vec>().lx(), r0, Imm(dst));
          EMIT_SIMD_ST(xvst, dst, src);

        break;
      }
    }
  }

  if (dst_.isReg() && src_.isReg()) {
    Reg dst(dst_.as<Reg>());
    Reg src(src_.as<Reg>());

    switch (typeId) {
      case Type::kIdI8:
      case Type::kIdU8:
      case Type::kIdI16:
      case Type::kIdU16:
      case Type::kIdI32:
      case Type::kIdU32:
      case Type::kIdI64:
      case Type::kIdU64:
        //return emitter->mov(src.as<Gp>().x(), dst.as<Gp>().x());  //ARM have a bug  ?
        ////return emitter->add_d(src.as<Gp>().x(), dst.as<Gp>().x(), r0);
        return emitter->add_d(dst.as<Gp>().x(), src.as<Gp>().x(), r0);

      default: {
        if (Type::isVec32(typeId))
          //return emitter->fmov(dst.as<Vec>().s(), src.as<Vec>().s());
          return emitter->fmov_s(dst.as<Vec>(), src.as<Vec>());

        if (Type::isVec64(typeId))
          //return emitter->mov(dst.as<Vec>().b8(), src.as<Vec>().b8());
          return emitter->fmov_d(dst.as<Vec>(), src.as<Vec>());

        if (Type::isVec128(typeId))
          //return emitter->mov(dst.as<Vec>().b16(), src.as<Vec>().b16());
          return emitter->vor_v(dst.as<Vec>(), src.as<Vec>(), src.as<Vec>());

	    if (Type::isVec256(typeId))
	      ////return emitter->xvpermi_q(dst.as<Vec>().b32(), src.as<Vec>().b32(), 16);
          return emitter->xvor_v(dst.as<Vec>(), src.as<Vec>(), src.as<Vec>());

        break;
      }
    }
  }

  emitter->setInlineComment(nullptr);
  return DebugUtils::errored(kErrorInvalidState);
}

Error EmitHelper::emitRegSwap(
  const BaseReg& a,
  const BaseReg& b, const char* comment) {

  DebugUtils::unused(a, b, comment);
  return DebugUtils::errored(kErrorInvalidState);
}

Error EmitHelper::emitArgMove(
  const BaseReg& dst_, uint32_t dstTypeId,
  const Operand_& src_, uint32_t srcTypeId, const char* comment) {

  // Deduce optional `dstTypeId`, which may be `Type::kIdVoid` in some cases.
  if (!dstTypeId) {
    const ArchTraits& archTraits = ArchTraits::byArch(_emitter->arch());
    dstTypeId = archTraits.regTypeToTypeId(dst_.type());
  }

  // Invalid or abstract TypeIds are not allowed.
  ASMJIT_ASSERT(Type::isValid(dstTypeId) && !Type::isAbstract(dstTypeId));
  ASMJIT_ASSERT(Type::isValid(srcTypeId) && !Type::isAbstract(srcTypeId));

  Reg dst(dst_.as<Reg>());
  Operand src(src_);

  uint32_t dstSize = Type::sizeOf(dstTypeId);
  // uint32_t srcSize = Type::sizeOf(srcTypeId);

  // TODO: [ARM] EmitArgMove is unfinished.

  if (Type::isInt(dstTypeId)) {
    if (Type::isInt(srcTypeId)) {
      uint32_t x = dstSize == 8;
      uint32_t instId = Inst::kIdNone;

      switch (srcTypeId) {
        case Type::kIdI8 : instId = Inst::kIdLd_b; break;
        case Type::kIdU8 : instId = Inst::kIdLd_bu; break;

        case Type::kIdI16: instId = Inst::kIdLd_h; break;
        case Type::kIdU16: instId = Inst::kIdLd_hu; break;

        case Type::kIdI32: instId = Inst::kIdLd_w; break;
        case Type::kIdU32: instId = Inst::kIdLd_wu; break;

        case Type::kIdI64: instId = Inst::kIdLd_d; break;
        case Type::kIdU64: instId = Inst::kIdLd_d; break;
      }

      if (!instId)
        return DebugUtils::errored(kErrorInvalidState);

      dst.setSignature(x ? GpX::kSignature : GpW::kSignature);
      _emitter->setInlineComment(comment);
      return _emitter->emit(instId, dst, src);
    }
  }

  return DebugUtils::errored(kErrorInvalidState);
}

// ============================================================================
// [asmjit::a64::EmitHelper - Emit Prolog & Epilog]
// ============================================================================

struct PrologEpilogInfo {
  struct RegPair {
    uint8_t ids[2];
    uint16_t offset;
  };

  struct GroupData {
    RegPair pairs[16];
    uint32_t pairCount;
  };

  GroupData groups[2];
  uint32_t sizeTotal;

  Error init(const FuncFrame& frame) noexcept {
    uint32_t offset = 0;

    for (uint32_t group = 0; group < 2; group++) {
      GroupData& data = groups[group];
      uint32_t n = 0;
      uint32_t pairCount = 0;
      RegPair* pairs = data.pairs;
      uint32_t slotSize = frame.saveRestoreRegSize(group);

      uint32_t savedRegs = frame.savedRegs(group);
      if (group == 0 && frame.hasPreservedFP()) {
        // Must be at the beginning of the push/pop sequence.
        ASMJIT_ASSERT(pairCount == 0);

        pairs[0].offset = uint16_t(offset);
        pairs[0].ids[0] = Gp::kIdFp;
        //pairs[0].ids[1] = Gp::kIdLr;
        pairs[0].ids[1] = Gp::kIdRa;
        offset += slotSize * 2;
        pairCount++;

        //savedRegs &= ~Support::bitMask(Gp::kIdFp, Gp::kIdLr);
        savedRegs &= ~Support::bitMask(Gp::kIdFp, Gp::kIdRa);
      }

      Support::BitWordIterator<uint32_t> it(savedRegs);
      while (it.hasNext()) {
        pairs[pairCount].ids[n] = uint8_t(it.next());

        if (++n == 2) {
          pairs[pairCount].offset = uint16_t(offset);
          offset += slotSize * 2;

          n = 0;
          pairCount++;
        }
      }

      if (n == 1) {
        pairs[pairCount].ids[1] = uint8_t(BaseReg::kIdBad);
        pairs[pairCount].offset = uint16_t(offset);
        offset += slotSize * 2;
        pairCount++;
      }

      data.pairCount = pairCount;
    }

    sizeTotal = offset;
    return kErrorOk;
  }
};

// TODO: [ARM] Emit prolog.
ASMJIT_FAVOR_SIZE Error EmitHelper::emitProlog(const FuncFrame& frame) {
  Emitter* emitter = _emitter->as<Emitter>();

  PrologEpilogInfo pei;
  ASMJIT_PROPAGATE(pei.init(frame));

  ////static const Reg groupRegs[2] = { r4, d0 };
  static const Reg groupRegs[2] = { r0, f0 };
  static const uint32_t groupInsts[2][2] = {
    { Inst::kIdSt_d , Inst::kIdSt_d },
    { Inst::kIdFst_d, Inst::kIdFst_d }
  };

  static const uint32_t groupSlotSize[2] = {8, 8};

  uint32_t adjustInitialOffset = pei.sizeTotal;

  for (uint32_t group = 0; group < 2; group++) {
    const PrologEpilogInfo::GroupData& data = pei.groups[group];
    uint32_t pairCount = data.pairCount;

    uint32_t slotSize = groupSlotSize[group];

    Reg regs[2] = { groupRegs[group], groupRegs[group] };
    Mem mem = ptr(sp);

    const uint32_t* insts = groupInsts[group];

    for (uint32_t i = 0; i < pairCount; i++) {
      const PrologEpilogInfo::RegPair& pair = data.pairs[i];

      regs[0].setId(pair.ids[0]);
      regs[1].setId(pair.ids[1]);
      //mem.setOffsetLo32(pair.offset);
      mem.setOffsetLo32(-int(adjustInitialOffset) + int(pair.offset));

      /*if (pair.offset == 0 && adjustInitialOffset) {
        mem.setOffset(-int(adjustInitialOffset));
        //mem.makePreIndex();
      } */

      if (pair.ids[1] == BaseReg::kIdBad)
        ASMJIT_PROPAGATE(emitter->emit(insts[0], regs[0], mem));
      else {
        //ASMJIT_PROPAGATE(emitter->emit(insts[1], regs[0], regs[1], mem));
        ASMJIT_PROPAGATE(emitter->emit(insts[1], regs[0], mem));
        mem.setOffsetLo32( mem.offsetLo32() + slotSize );
        ASMJIT_PROPAGATE(emitter->emit(insts[1], regs[1], mem));
      }

      //mem.resetToFixedOffset();

      if (i == 0 && frame.hasPreservedFP()) {
        ////ASMJIT_PROPAGATE(emitter->add_d(r22, r3, r0));
        ASMJIT_PROPAGATE(emitter->add_d(fp, sp, r0));
      }
    }
  }

  if (frame.hasStackAdjustment()) {
    uint32_t adj = frame.stackAdjustment();
    if (adj <= 0x7FFu) {
      //ASMJIT_PROPAGATE(emitter->sub(r3, r3, adj));  //adj: u12
      ASMJIT_PROPAGATE(emitter->addi_d(sp, sp, -int(adj)));
    }
    else if (adj <= 0xFFFFFFu)  {
      // TODO: [ARM] Prolog - we must touch the pages otherwise it's undefined.
      //ASMJIT_PROPAGATE(emitter->sub(r3, r3, adj & 0x000FFFu));
      //ASMJIT_PROPAGATE(emitter->sub(r3, r3, adj & 0xFFF000u));
      /*ASMJIT_PROPAGATE(emitter->addi_d(r12, r0, adj & 0x000FFFu));
      ASMJIT_PROPAGATE(emitter->sub_d(r5, r5, r12));
      ASMJIT_PROPAGATE(emitter->addi_d(r12, r0, adj & 0xFFF000u));
      ASMJIT_PROPAGATE(emitter->sub_d(r5, r5, r12)); */
      ASMJIT_PROPAGATE(emitter->lu12i_w(t0, adj >> 12));
      ASMJIT_PROPAGATE(emitter->ori(t0, t0, adj & 0xFFF));
      ASMJIT_PROPAGATE(emitter->sub_d(sp, sp, t0));
    }
    else {
      return DebugUtils::errored(kErrorInvalidState);
    }
  }

  return kErrorOk;
}

// TODO: [LOONG] Emit epilog.
ASMJIT_FAVOR_SIZE Error EmitHelper::emitEpilog(const FuncFrame& frame) {
  Emitter* emitter = _emitter->as<Emitter>();
  PrologEpilogInfo pei;
  ASMJIT_PROPAGATE(pei.init(frame));
  static const Reg groupRegs[2] = { r0, f0 };
  static const uint32_t groupInsts[2][2] = {
    { Inst::kIdLd_d , Inst::kIdLd_d  },
    { Inst::kIdFld_d , Inst::kIdFld_d  }
  };

  static const uint32_t groupSlotSize[2] = {8, 8};

  uint32_t adjustInitialOffset = pei.sizeTotal;
  if (frame.hasStackAdjustment()) {
    uint32_t adj = frame.stackAdjustment();
    //if (adj <= 0xFFFu) {
    if (adj <= 0x7FFu) {
      //ASMJIT_PROPAGATE(emitter->add(r3, r3, adj));
      ASMJIT_PROPAGATE(emitter->addi_d(sp, sp, adj));
    }
    else if (adj <= 0xFFFFFFu)  {
      /*ASMJIT_PROPAGATE(emitter->addi_d(r5, r5, adj & 0x000FFFu));
      ASMJIT_PROPAGATE(emitter->addi_d(r5, r5, adj & 0xFFF000u)); */
      ASMJIT_PROPAGATE(emitter->lu12i_w(t0, adj >> 12));
      ASMJIT_PROPAGATE(emitter->ori(t0, t0, adj & 0xFFF));
      ASMJIT_PROPAGATE(emitter->add_d(sp, sp, t0));
    }
    else {
      return DebugUtils::errored(kErrorInvalidState);
    }
  }
  for (int group = 1; group >= 0; group--) {
    const PrologEpilogInfo::GroupData& data = pei.groups[group];
    uint32_t pairCount = data.pairCount;
    uint32_t slotSize = groupSlotSize[group];

    Reg regs[2] = { groupRegs[group], groupRegs[group] };
    Mem mem = ptr(sp);

    const uint32_t* insts = groupInsts[group];

    //for (int i = int(pairCount) - 1; i >= 0; i--) {
    for (uint32_t i = 0; i < pairCount; i++) {
      const PrologEpilogInfo::RegPair& pair = data.pairs[i];

      regs[0].setId(pair.ids[0]);
      regs[1].setId(pair.ids[1]);
      //mem.setOffsetLo32(pair.offset);
      mem.setOffsetLo32(-int(adjustInitialOffset) + int(pair.offset));

      /*if (pair.offset == 0 && adjustInitialOffset) {
        mem.setOffset(int(adjustInitialOffset));
        //mem.makePostIndex();
      } */

      if (pair.ids[1] == BaseReg::kIdBad)
        ASMJIT_PROPAGATE(emitter->emit(insts[0], regs[0], mem));
      else {
        //ASMJIT_PROPAGATE(emitter->emit(insts[1], regs[0], regs[1], mem));
        ASMJIT_PROPAGATE(emitter->emit(insts[1], regs[0], mem));
        mem.setOffsetLo32( mem.offsetLo32() + slotSize);
        ASMJIT_PROPAGATE(emitter->emit(insts[1], regs[1], mem));
      }

      //mem.resetToFixedOffset();
    }
  }
  ASMJIT_PROPAGATE(emitter->jirl(zero, ra, 0));

  return kErrorOk;
}

ASMJIT_END_SUB_NAMESPACE

#endif // !ASMJIT_NO_LOONG
