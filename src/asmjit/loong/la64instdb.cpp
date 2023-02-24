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

#include "../core/codeholder.h"
#include "../core/support.h"
#include "../loong/la64instdb_p.h"
#include "../loong/la64operand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(la64)

namespace InstDB {

// ============================================================================
// [asmjit::a64::InstDB - InstInfoTable]
// ============================================================================

// Don't store `_nameDataIndex` if instruction names are disabled. Since some
// APIs can use `_nameDataIndex` it's much safer if it's zero if it's not used.
#if defined(ASMJIT_NO_TEXT)
  #define NAME_DATA_INDEX(x) 0
#else
  #define NAME_DATA_INDEX(x) x
#endif

// Defines an loongarch64 instruction.
#define INSTL(id, opcodeEncoding, opcodeData, rwInfoIndex, flags, opcodeDataIndex, nameDataIndex) { \
  uint32_t(kEncoding##opcodeEncoding),      \
  uint32_t(opcodeDataIndex),                \
  0,                                        \
  uint32_t(NAME_DATA_INDEX(nameDataIndex)), \
  uint16_t(rwInfoIndex),                    \
  uint16_t(flags)                           \
}

#define F(flag) kInstFlag##flag

// TODO: [ARM] Missing Instructions:
/*
BLRAA, BLRAAZ, BLRAB, BLRABZ: Branch with Link to Register, with pointer authentication.
BRAA, BRAAZ, BRAB, BRABZ: Branch to Register, with pointer authentication.

CFP: Control Flow Prediction Restriction by Context: an alias of SYS.
CPP: Cache Prefetch Prediction Restriction by Context: an alias of SYS.
DVP: Data Value Prediction Restriction by Context: an alias of SYS.
PSB CSYNC: Profiling Synchronization Barrier.

ERETAA, ERETAB: Exception Return, with pointer authentication.
LDAPxxx
PACIA, PACIA1716, PACIASP, PACIAZ, PACIZA: Pointer Authentication Code for Instruction address, using key A.
PACIB, PACIB1716, PACIBSP, PACIBZ, PACIZB: Pointer Authentication Code for Instruction address, using key B.
PRFM (immediate): Prefetch Memory (immediate).
PRFM (literal): Prefetch Memory (literal).
PRFM (register): Prefetch Memory (register).
PRFUM: Prefetch Memory (unscaled offset).
RETAA, RETAB: Return from subroutine, with pointer authentication.
RMIF: Rotate, Mask Insert Flags.
SYSL
IRG: Insert Random Tag.
INST_(Irg              , BaseRRR            , (0b1001101011000000000100, kX , kSP, kX , kSP, kX , kZR, true)                        , kRWI_W    , 0                         , 0  , 1   ), // #1
*/

const InstInfo _instInfoLTable[] = {
  INSTL(None,            None,     (_),    0,   0,   0,   0),//#0
  INSTL(add_d, BaseLRRR, (0b00000000000100001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 1, 1), //add_d
  INSTL(add_w, BaseLRRR, (0b00000000000100000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 0, 7), //add_w
  INSTL(addi_d, BaseLRRI, (0b0000001011, kX, kSP, 10), 0, 0, 3, 13), //addi_d
  INSTL(addi_w, BaseLRRI, (0b0000001010, kX, kSP, 10), 0, 0, 2, 20), //addi_w
  INSTL(addu16i_d, BaseLRRIL, (0b000100, kX, kSP, 10), 0, 0, 0, 27), //addu16i_d
  INSTL(alsl_d, BaseLRRRI, (0b000000000010110, kWX, 0, kWX, 5, kWX, 10, 15, 0), 0, 0, 4, 37), //alsl_d
  INSTL(alsl_w, BaseLRRRI, (0b000000000000010, kWX, 0, kWX, 5, kWX, 10, 15, 0), 0, 0, 0, 44), //alsl_w
  INSTL(alsl_wu, BaseLRRRI, (0b000000000000011, kWX, 0, kWX, 5, kWX, 10, 15, 0), 0, 0, 1, 51), //alsl_wu
  INSTL(amadd_d, BaseLRRRT, (0b00111000011000011, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 3, 59), //amadd_d
  INSTL(amadd_db_d, BaseLRRRT, (0b00111000011010101, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 21, 67), //amadd_db_d
  INSTL(amadd_db_w, BaseLRRRT, (0b00111000011010100, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 20, 78), //amadd_db_w
  INSTL(amadd_w, BaseLRRRT, (0b00111000011000010, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 2, 89), //amadd_w
  INSTL(amand_d, BaseLRRRT, (0b00111000011000101, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 5, 97), //amand_d
  INSTL(amand_db_d, BaseLRRRT, (0b00111000011010111, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 23, 105), //amand_db_d
  INSTL(amand_db_w, BaseLRRRT, (0b00111000011010110, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 22, 116), //amand_db_w
  INSTL(amand_w, BaseLRRRT, (0b00111000011000100, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 4, 127), //amand_w
  INSTL(ammax_d, BaseLRRRT, (0b00111000011001011, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 11, 135), //ammax_d
  INSTL(ammax_db_d, BaseLRRRT, (0b00111000011011101, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 29, 143), //ammax_db_d
  INSTL(ammax_db_du, BaseLRRRT, (0b00111000011100001, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 33, 154), //ammax_db_du
  INSTL(ammax_db_w, BaseLRRRT, (0b00111000011011100, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 28, 166), //ammax_db_w
  INSTL(ammax_db_wu, BaseLRRRT, (0b00111000011100000, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 32, 177), //ammax_db_wu
  INSTL(ammax_du, BaseLRRRT, (0b00111000011001111, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 15, 189), //ammax_du
  INSTL(ammax_w, BaseLRRRT, (0b00111000011001010, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 10, 198), //ammax_w
  INSTL(ammax_wu, BaseLRRRT, (0b00111000011001110, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 14, 206), //ammax_wu
  INSTL(ammin_d, BaseLRRRT, (0b00111000011001101, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 13, 215), //ammin_d
  INSTL(ammin_db_d, BaseLRRRT, (0b00111000011011111, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 31, 223), //ammin_db_d
  INSTL(ammin_db_du, BaseLRRRT, (0b00111000011100011, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 35, 234), //ammin_db_du
  INSTL(ammin_db_w, BaseLRRRT, (0b00111000011011110, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 30, 246), //ammin_db_w
  INSTL(ammin_db_wu, BaseLRRRT, (0b00111000011100010, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 34, 257), //ammin_db_wu
  INSTL(ammin_du, BaseLRRRT, (0b00111000011010001, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 17, 269), //ammin_du
  INSTL(ammin_w, BaseLRRRT, (0b00111000011001100, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 12, 278), //ammin_w
  INSTL(ammin_wu, BaseLRRRT, (0b00111000011010000, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 16, 286), //ammin_wu
  INSTL(amor_d, BaseLRRRT, (0b00111000011000111, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 7, 295), //amor_d
  INSTL(amor_db_d, BaseLRRRT, (0b00111000011011001, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 25, 302), //amor_db_d
  INSTL(amor_db_w, BaseLRRRT, (0b00111000011011000, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 24, 312), //amor_db_w
  INSTL(amor_w, BaseLRRRT, (0b00111000011000110, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 6, 322), //amor_w
  INSTL(amswap_d, BaseLRRRT, (0b00111000011000001, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 1, 329), //amswap_d
  INSTL(amswap_db_d, BaseLRRRT, (0b00111000011010011, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 19, 338), //amswap_db_d
  INSTL(amswap_db_w, BaseLRRRT, (0b00111000011010010, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 18, 350), //amswap_db_w
  INSTL(amswap_w, BaseLRRRT, (0b00111000011000000, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 0, 362), //amswap_w
  INSTL(amxor_d, BaseLRRRT, (0b00111000011001001, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 9, 371), //amxor_d
  INSTL(amxor_db_d, BaseLRRRT, (0b00111000011011011, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 27, 379), //amxor_db_d
  INSTL(amxor_db_w, BaseLRRRT, (0b00111000011011010, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 26, 390), //amxor_db_w
  INSTL(amxor_w, BaseLRRRT, (0b00111000011001000, kWX, 0, kWX, 10, kWX, 5, 0), 0, 0, 8, 401), //amxor_w
  INSTL(and_, BaseLRRR, (0b00000000000101001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 9, 409), //and_
  INSTL(andi, BaseLRRI, (0b0000001101, kX, kSP, 10), 0, 0, 5, 414), //andi
  INSTL(andn, BaseLRRR, (0b00000000000101101, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 13, 419), //andn
  INSTL(asrtgt_d, BaseLRRL, (0b00000000000000011, kWX, 5, kWX, 10), 0, 0, 1, 424), //asrtgt_d
  INSTL(asrtle_d, BaseLRRL, (0b00000000000000010, kWX, 5, kWX, 10), 0, 0, 0, 433), //asrtle_d
  INSTL(b,  BaseBranchRel, (0b010100, OffsetFormat::kTypeLa64_BBL), 0, 0, 0, 442), //b
  INSTL(bceqz, BaseLII, (0b010010, 5, 0, 3, 5, 16, 10, 0), 0, 0, 0, 444), //bceqz
  INSTL(bcnez, BaseLII, (0b010010, 5, 0, 3, 5, 16, 10, 1), 0, 0, 1, 450), //bcnez
  ////INSTL(beq, BTLRRI, (0b010110), 0, 0, 0, 456), //beq
  ////INSTL(bge, BTLRRI, (0b011001), 0, 0, 3, 460), //bge
  ////INSTL(bgeu, BTLRRI, (0b011011), 0, 0, 5, 464), //bgeu
  INSTL(beq,  BaseBranchRel, (0b010110, OffsetFormat::kTypeLa64_BEQ), 0, 0, 2, 456), //beq
  INSTL(bge,  BaseBranchRel, (0b011001, OffsetFormat::kTypeLa64_BEQ), 0, 0, 5, 460), //bge
  INSTL(bgeu, BaseBranchRel, (0b011011, OffsetFormat::kTypeLa64_BEQ), 0, 0, 7, 464), //bgeu
  INSTL(bitrev_4b, BaseLRR, (0b0000000000000000010010, kWX, 0, kWX, 5, 0), 0, 0, 15, 469), //bitrev_4b
  INSTL(bitrev_8b, BaseLRR, (0b0000000000000000010011, kWX, 0, kWX, 5, 0), 0, 0, 16, 479), //bitrev_8b
  INSTL(bitrev_d, BaseLRR, (0b0000000000000000010101, kWX, 0, kWX, 5, 0), 0, 0, 18, 489), //bitrev_d
  INSTL(bitrev_w, BaseLRR, (0b0000000000000000010100, kWX, 0, kWX, 5, 0), 0, 0, 17, 498), //bitrev_w
  ////INSTL(bl, BaseBLI, (0b010101), 0, 0, 1, 507), //bl
  INSTL(bl, BaseBranchRel, (0b010101, OffsetFormat::kTypeLa64_BBL), 0, 0, 1, 507), //bl
  ////INSTL(blt, BTLRRI, (0b011000), 0, 0, 2, 510), //blt
  ////INSTL(bltu, BTLRRI, (0b011010), 0, 0, 4, 514), //bltu
  ////INSTL(bne, BTLRRI, (0b010111), 0, 0, 1, 519), //bne
  INSTL(blt,  BaseBranchRel, (0b011000, OffsetFormat::kTypeLa64_BEQ), 0, 0, 4, 510), //blt
  INSTL(bltu, BaseBranchRel, (0b011010, OffsetFormat::kTypeLa64_BEQ), 0, 0, 6, 514), //bltu
  INSTL(bne,  BaseBranchRel, (0b010111, OffsetFormat::kTypeLa64_BEQ), 0, 0, 3, 519), //bne
  INSTL(break_, BaseLIC, (0b00000000001010100, 0), 0, 0, 0, 523), //break_
  INSTL(bstrins_d, BaseLRRII, (0b0000000010, kWX, 0, kWX, 5, 10, 16, 2), 0, 0, 2, 530), //bstrins_d
  INSTL(bstrins_w, BaseLRRII, (0b00000000011, kWX, 0, kWX, 5, 10, 16, 0), 0, 0, 0, 540), //bstrins_w
  INSTL(bstrpick_d, BaseLRRII, (0b0000000011, kWX, 0, kWX, 5, 10, 16, 2), 0, 0, 3, 550), //bstrpick_d
  INSTL(bstrpick_w, BaseLRRII, (0b00000000011, kWX, 0, kWX, 5, 10, 16, 1), 0, 0, 1, 561), //bstrpick_w
  INSTL(bytepick_d, BaseLRRRI, (0b00000000000011, kWX, 0, kWX, 5, kWX, 10, 15, 2), 0, 0, 3, 572), //bytepick_d
  INSTL(bytepick_w, BaseLRRRI, (0b000000000000100, kWX, 0, kWX, 5, kWX, 10, 15, 1), 0, 0, 2, 583), //bytepick_w
  INSTL(cacop, LCldst, (0b0000011000), 0, 0, 0, 594), //cacop
  INSTL(clo_d, BaseLRR, (0b0000000000000000001000, kWX, 0, kWX, 5, 0), 0, 0, 5, 600), //clo_d
  INSTL(clo_w, BaseLRR, (0b0000000000000000000100, kWX, 0, kWX, 5, 0), 0, 0, 0, 606), //clo_w
  INSTL(clz_d, BaseLRR, (0b0000000000000000001001, kWX, 0, kWX, 5, 0), 0, 0, 6, 612), //clz_d
  INSTL(clz_w, BaseLRR, (0b0000000000000000000101, kWX, 0, kWX, 5, 0), 0, 0, 1, 618), //clz_w
  INSTL(cpucfg, BaseLRR, (0b0000000000000000011011, kWX, 0, kWX, 5, 0), 0, 0, 4, 624), //cpucfg
  INSTL(crc_w_b_w, BaseLRRR, (0b00000000001001000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 38, 631), //crc_w_b_w
  INSTL(crc_w_d_w, BaseLRRR, (0b00000000001001011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 41, 641), //crc_w_d_w
  INSTL(crc_w_h_w, BaseLRRR, (0b00000000001001001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 39, 651), //crc_w_h_w
  INSTL(crc_w_w_w, BaseLRRR, (0b00000000001001010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 40, 661), //crc_w_w_w
  INSTL(crcc_w_b_w, BaseLRRR, (0b00000000001001100, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 42, 671), //crcc_w_b_w
  INSTL(crcc_w_d_w, BaseLRRR, (0b00000000001001111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 45, 682), //crcc_w_d_w
  INSTL(crcc_w_h_w, BaseLRRR, (0b00000000001001101, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 43, 693), //crcc_w_h_w
  INSTL(crcc_w_w_w, BaseLRRR, (0b00000000001001110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 44, 704), //crcc_w_w_w
  INSTL(csrrd, BaseLRI, (0b00000100, kWX, 0, 10, 2), 0, 0, 7, 715), //csrrd
  INSTL(csrwr, BaseLRI, (0b00000100, kWX, 0, 10, 3), 0, 0, 8, 721), //csrwr
  INSTL(csrxchg, BaseLRRI, (0b00000100, kWX, 0, kWX, 5, 10, 3), 0, 0, 16, 727), //csrxchg
  INSTL(cto_d, BaseLRR, (0b0000000000000000001010, kWX, 0, kWX, 5, 0), 0, 0, 7, 735), //cto_d
  INSTL(cto_w, BaseLRR, (0b0000000000000000000110, kWX, 0, kWX, 5, 0), 0, 0, 2, 741), //cto_w
  INSTL(ctz_d, BaseLRR, (0b0000000000000000001011, kWX, 0, kWX, 5, 0), 0, 0, 8, 747), //ctz_d
  INSTL(ctz_w, BaseLRR, (0b0000000000000000000111, kWX, 0, kWX, 5, 0), 0, 0, 3, 753), //ctz_w
  INSTL(dbar, BaseOpImm, (0b00111000011100100, 15, 0), 0, 0, 1, 759), //dbar
  INSTL(dbcl, BaseLIC, (0b00000000001010101, 0), 0, 0, 1, 764), //dbcl
  INSTL(div_d, BaseLRRR, (0b00000000001000100, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 34, 769), //div_d
  INSTL(div_du, BaseLRRR, (0b00000000001000110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 36, 775), //div_du
  INSTL(div_w, BaseLRRR, (0b00000000001000000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 30, 782), //div_w
  INSTL(div_wu, BaseLRRR, (0b00000000001000010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 32, 788), //div_wu
  INSTL(ertn, BaseOp, (0b00000110010010000011100000000000), 0, 0, 6, 795), //ertn
  INSTL(ext_w_b, BaseLRR, (0b0000000000000000010111, kWX, 0, kWX, 5, 0), 0, 0, 20, 800), //ext_w_b
  INSTL(ext_w_h, BaseLRR, (0b0000000000000000010110, kWX, 0, kWX, 5, 0), 0, 0, 19, 808), //ext_w_h
  INSTL(fabs_d, BaseLVV, (0b0000000100010100000010, 0, 5, 0), 0, 0, 1, 816), //fabs_d
  INSTL(fabs_s, BaseLVV, (0b0000000100010100000001, 0, 5, 0), 0, 0, 0, 823), //fabs_s
  INSTL(fadd_d, BaseLVVV, (0b00000001000000010, 0, 5, 10, 0), 0, 0, 1, 830), //fadd_d
  INSTL(fadd_s, BaseLVVV, (0b00000001000000001, 0, 5, 10, 0), 0, 0, 0, 837), //fadd_s
  INSTL(fclass_d, BaseLVV, (0b0000000100010100001110, 0, 5, 0), 0, 0, 7, 844), //fclass_d
  INSTL(fclass_s, BaseLVV, (0b0000000100010100001101, 0, 5, 0), 0, 0, 6, 853), //fclass_s
  INSTL(fcmp_caf_s, BaseLFIVV, (0b00001100000100000, 0, 5, 10, 0), 0, 0, 0, 862), //fcmp_caf_s
  INSTL(fcmp_ceq_s, BaseLFIVV, (0b00001100000100100, 0, 5, 10, 0), 0, 0, 2, 873), //fcmp_ceq_s
  INSTL(fcmp_cle_d, BaseLFIVV, (0b00001100001000110, 0, 5, 10, 0), 0, 0, 6, 884), //fcmp_cle_d
  INSTL(fcmp_clt_d, BaseLFIVV, (0b00001100001000010, 0, 5, 10, 0), 0, 0, 4, 895), //fcmp_clt_d
  INSTL(fcmp_cne_d, BaseLFIVV, (0b00001100001010000, 0, 5, 10, 0), 0, 0, 8, 906), //fcmp_cne_d
  INSTL(fcmp_cor_s, BaseLFIVV, (0b00001100000110100, 0, 5, 10, 0), 0, 0, 9, 917), //fcmp_cor_s
  INSTL(fcmp_cueq_s, BaseLFIVV, (0b00001100000101100, 0, 5, 10, 0), 0, 0, 3, 928), //fcmp_cueq_s
  INSTL(fcmp_cule_s, BaseLFIVV, (0b00001100000101110, 0, 5, 10, 0), 0, 0, 7, 940), //fcmp_cule_s
  INSTL(fcmp_cult_s, BaseLFIVV, (0b00001100000101010, 0, 5, 10, 0), 0, 0, 5, 952), //fcmp_cult_s
  INSTL(fcmp_cun_d, BaseLFIVV, (0b00001100001001000, 0, 5, 10, 0), 0, 0, 1, 964), //fcmp_cun_d
  INSTL(fcmp_cune_d, BaseLFIVV, (0b00001100001011000, 0, 5, 10, 0), 0, 0, 10, 975), //fcmp_cune_d
  INSTL(fcmp_saf_s, BaseLFIVV, (0b00001100000100001, 0, 5, 10, 0), 0, 0, 11, 987), //fcmp_saf_s
  INSTL(fcmp_seq_s, BaseLFIVV, (0b00001100000100101, 0, 5, 10, 0), 0, 0, 13, 998), //fcmp_seq_s
  INSTL(fcmp_sle_s, BaseLFIVV, (0b00001100000100111, 0, 5, 10, 0), 0, 0, 17, 1009), //fcmp_sle_s
  INSTL(fcmp_slt_s, BaseLFIVV, (0b00001100000100011, 0, 5, 10, 0), 0, 0, 15, 1020), //fcmp_slt_s
  INSTL(fcmp_sne_s, BaseLFIVV, (0b00001100000110001, 0, 5, 10, 0), 0, 0, 19, 1031), //fcmp_sne_s
  INSTL(fcmp_sor_d, BaseLFIVV, (0b00001100001010101, 0, 5, 10, 0), 0, 0, 20, 1042), //fcmp_sor_d
  INSTL(fcmp_sueq_d, BaseLFIVV, (0b00001100001001101, 0, 5, 10, 0), 0, 0, 14, 1053), //fcmp_sueq_d
  INSTL(fcmp_sule_d, BaseLFIVV, (0b00001100001001111, 0, 5, 10, 0), 0, 0, 18, 1065), //fcmp_sule_d
  INSTL(fcmp_sult_d, BaseLFIVV, (0b00001100001001011, 0, 5, 10, 0), 0, 0, 16, 1077), //fcmp_sult_d
  INSTL(fcmp_sun_d, BaseLFIVV, (0b00001100001001001, 0, 5, 10, 0), 0, 0, 12, 1089), //fcmp_sun_d
  INSTL(fcmp_sune_s, BaseLFIVV, (0b00001100000111001, 0, 5, 10, 0), 0, 0, 21, 1100), //fcmp_sune_s
  INSTL(fcopysign_d, BaseLVVV, (0b00000001000100110, 0, 5, 10, 0), 0, 0, 19, 1112), //fcopysign_d
  INSTL(fcopysign_s, BaseLVVV, (0b00000001000100101, 0, 5, 10, 0), 0, 0, 18, 1124), //fcopysign_s
  INSTL(fcvt_d_s, BaseLVV, (0b0000000100011001001001, 0, 5, 0), 0, 0, 17, 1136), //fcvt_d_s
  INSTL(fcvt_s_d, BaseLVV, (0b0000000100011001000110, 0, 5, 0), 0, 0, 16, 1145), //fcvt_s_d
  INSTL(fdiv_d, BaseLVVV, (0b00000001000001110, 0, 5, 10, 0), 0, 0, 7, 1154), //fdiv_d
  INSTL(fdiv_s, BaseLVVV, (0b00000001000001101, 0, 5, 10, 0), 0, 0, 6, 1161), //fdiv_s
  INSTL(ffint_d_l, BaseLVV, (0b0000000100011101001010, 0, 5, 0), 0, 0, 41, 1168), //ffint_d_l
  INSTL(ffint_d_w, BaseLVV, (0b0000000100011101001000, 0, 5, 0), 0, 0, 40, 1178), //ffint_d_w
  INSTL(ffint_s_l, BaseLVV, (0b0000000100011101000110, 0, 5, 0), 0, 0, 39, 1188), //ffint_s_l
  INSTL(ffint_s_w, BaseLVV, (0b0000000100011101000100, 0, 5, 0), 0, 0, 38, 1198), //ffint_s_w
  ////INSTL(fld_d, LFldst, (0b0010101110), 0, 0, 2, 1208), //fld_d
  INSTL(fld_d, BaseLdSt, (0b0010101110, 22),         0, 0, 24, 1208), //fld_d
  ////INSTL(fld_s, LFldst, (0b0010101100), 0, 0, 0, 1214), //fld_s
  INSTL(fld_s, BaseLdSt, (0b0010101100, 22),         0, 0, 22, 1214), //fld_s
  INSTL(fldgt_d, BaseLVRR, (0b00111000011101001, 0, kWX, 5, kWX, 10, 0), 0, 0, 5, 1220), //fldgt_d
  INSTL(fldgt_s, BaseLVRR, (0b00111000011101000, 0, kWX, 5, kWX, 10, 0), 0, 0, 4, 1228), //fldgt_s
  INSTL(fldle_d, BaseLVRR, (0b00111000011101011, 0, kWX, 5, kWX, 10, 0), 0, 0, 7, 1236), //fldle_d
  INSTL(fldle_s, BaseLVRR, (0b00111000011101010, 0, kWX, 5, kWX, 10, 0), 0, 0, 6, 1244), //fldle_s
  ////INSTL(fldx_d, BaseLVRR, (0b00111000001101000, 0, kWX, 5, kWX, 10, 0), 0, 0, 1, 1252), //fldx_d
  INSTL(fldx_d, BaseLdSt, (0b00111000001101000, 15), 0, 0, 27, 1252), //fldx_d
  ////INSTL(fldx_s, BaseLVRR, (0b00111000001100000, 0, kWX, 5, kWX, 10, 0), 0, 0, 0, 1259), //fldx_s
  INSTL(fldx_s, BaseLdSt, (0b00111000001100000, 15), 0, 0, 26, 1259), //fldx_s
  INSTL(flogb_d, BaseLVV, (0b0000000100010100001010, 0, 5, 0), 0, 0, 5, 1266), //flogb_d
  INSTL(flogb_s, BaseLVV, (0b0000000100010100001001, 0, 5, 0), 0, 0, 4, 1274), //flogb_s
  INSTL(fmadd_d, FpLVVVV, (0b000010000010), 0, 0, 1, 1282), //fmadd_d
  INSTL(fmadd_s, FpLVVVV, (0b000010000001), 0, 0, 0, 1290), //fmadd_s
  INSTL(fmax_d, BaseLVVV, (0b00000001000010010, 0, 5, 10, 0), 0, 0, 9, 1298), //fmax_d
  INSTL(fmax_s, BaseLVVV, (0b00000001000010001, 0, 5, 10, 0), 0, 0, 8, 1305), //fmax_s
  INSTL(fmaxa_d, BaseLVVV, (0b00000001000011010, 0, 5, 10, 0), 0, 0, 13, 1312), //fmaxa_d
  INSTL(fmaxa_s, BaseLVVV, (0b00000001000011001, 0, 5, 10, 0), 0, 0, 12, 1320), //fmaxa_s
  INSTL(fmin_d, BaseLVVV, (0b00000001000010110, 0, 5, 10, 0), 0, 0, 11, 1328), //fmin_d
  INSTL(fmin_s, BaseLVVV, (0b00000001000010101, 0, 5, 10, 0), 0, 0, 10, 1335), //fmin_s
  INSTL(fmina_d, BaseLVVV, (0b00000001000011110, 0, 5, 10, 0), 0, 0, 15, 1342), //fmina_d
  INSTL(fmina_s, BaseLVVV, (0b00000001000011101, 0, 5, 10, 0), 0, 0, 14, 1350), //fmina_s
  INSTL(fmov_d, BaseLVV, (0b0000000100010100100110, 0, 5, 0), 0, 0, 15, 1358), //fmov_d
  INSTL(fmov_s, BaseLVV, (0b0000000100010100100101, 0, 5, 0), 0, 0, 14, 1365), //fmov_s
  INSTL(fmul_d, BaseLVVV, (0b00000001000001010, 0, 5, 10, 0), 0, 0, 5, 1372), //fmul_d
  INSTL(fmul_s, BaseLVVV, (0b00000001000001001, 0, 5, 10, 0), 0, 0, 4, 1379), //fmul_s
  INSTL(fneg_d, BaseLVV, (0b0000000100010100000110, 0, 5, 0), 0, 0, 3, 1386), //fneg_d
  INSTL(fneg_s, BaseLVV, (0b0000000100010100000101, 0, 5, 0), 0, 0, 2, 1393), //fneg_s
  INSTL(frecip_d, BaseLVV, (0b0000000100010100010110, 0, 5, 0), 0, 0, 11, 1400), //frecip_d
  INSTL(frecip_s, BaseLVV, (0b0000000100010100010101, 0, 5, 0), 0, 0, 10, 1409), //frecip_s
  INSTL(frint_d, BaseLVV, (0b0000000100011110010010, 0, 5, 0), 0, 0, 43, 1418), //frint_d
  INSTL(frint_s, BaseLVV, (0b0000000100011110010001, 0, 5, 0), 0, 0, 42, 1426), //frint_s
  INSTL(frsqrt_d, BaseLVV, (0b0000000100010100011010, 0, 5, 0), 0, 0, 13, 1434), //frsqrt_d
  INSTL(frsqrt_s, BaseLVV, (0b0000000100010100011001, 0, 5, 0), 0, 0, 12, 1443), //frsqrt_s
  INSTL(fscaleb_d, BaseLVVV, (0b00000001000100010, 0, 5, 10, 0), 0, 0, 17, 1452), //fscaleb_d
  INSTL(fscaleb_s, BaseLVVV, (0b00000001000100001, 0, 5, 10, 0), 0, 0, 16, 1462), //fscaleb_s
  INSTL(fsel, LfVVVI, (0b00001101000000, 0, 5, 10, 3, 15, 0), 0, 0, 0, 1472), //fsel
  INSTL(fsqrt_d, BaseLVV, (0b0000000100010100010010, 0, 5, 0), 0, 0, 9, 1477), //fsqrt_d
  INSTL(fsqrt_s, BaseLVV, (0b0000000100010100010001, 0, 5, 0), 0, 0, 8, 1485), //fsqrt_s
  ////INSTL(fst_d, LFldst, (0b0010101111), 0, 0, 3, 1493), //fst_d
  INSTL(fst_d, BaseLdSt, (0b0010101111, 22),         0, 0, 25, 1493), //fst_d
  ////INSTL(fst_s, LFldst, (0b0010101101), 0, 0, 1, 1499), //fst_s
  INSTL(fst_s, BaseLdSt, (0b0010101101, 22),         0, 0, 23, 1499), //fst_s
  INSTL(fstgt_d, BaseLVRR, (0b00111000011101101, 0, kWX, 5, kWX, 10, 0), 0, 0, 9, 1505), //fstgt_d
  INSTL(fstgt_s, BaseLVRR, (0b00111000011101100, 0, kWX, 5, kWX, 10, 0), 0, 0, 8, 1513), //fstgt_s
  INSTL(fstle_d, BaseLVRR, (0b00111000011101111, 0, kWX, 5, kWX, 10, 0), 0, 0, 11, 1521), //fstle_d
  INSTL(fstle_s, BaseLVRR, (0b00111000011101110, 0, kWX, 5, kWX, 10, 0), 0, 0, 10, 1529), //fstle_s
  ////INSTL(fstx_d, BaseLVRR, (0b00111000001111000, 0, kWX, 5, kWX, 10, 0), 0, 0, 3, 1537), //fstx_d
  INSTL(fstx_d, BaseLdSt, (0b00111000001111000, 15), 0, 0, 29, 1537), //fstx_d
  ////INSTL(fstx_s, BaseLVRR, (0b00111000001110000, 0, kWX, 5, kWX, 10, 0), 0, 0, 2, 1544), //fstx_s
  INSTL(fstx_s, BaseLdSt, (0b00111000001110000, 15), 0, 0, 28, 1544), //fstx_s
  INSTL(fsub_d, BaseLVVV, (0b00000001000000110, 0, 5, 10, 0), 0, 0, 3, 1551), //fsub_d
  INSTL(fsub_s, BaseLVVV, (0b00000001000000101, 0, 5, 10, 0), 0, 0, 2, 1558), //fsub_s
  INSTL(ftint_l_d, BaseLVV, (0b0000000100011011001010, 0, 5, 0), 0, 0, 37, 1565), //ftint_l_d
  INSTL(ftint_l_s, BaseLVV, (0b0000000100011011001001, 0, 5, 0), 0, 0, 36, 1575), //ftint_l_s
  INSTL(ftint_w_d, BaseLVV, (0b0000000100011011000010, 0, 5, 0), 0, 0, 35, 1585), //ftint_w_d
  INSTL(ftint_w_s, BaseLVV, (0b0000000100011011000001, 0, 5, 0), 0, 0, 34, 1595), //ftint_w_s
  INSTL(ftintrm_l_d, BaseLVV, (0b0000000100011010001010, 0, 5, 0), 0, 0, 21, 1605), //ftintrm_l_d
  INSTL(ftintrm_l_s, BaseLVV, (0b0000000100011010001001, 0, 5, 0), 0, 0, 20, 1617), //ftintrm_l_s
  INSTL(ftintrm_w_d, BaseLVV, (0b0000000100011010000010, 0, 5, 0), 0, 0, 19, 1629), //ftintrm_w_d
  INSTL(ftintrm_w_s, BaseLVV, (0b0000000100011010000001, 0, 5, 0), 0, 0, 18, 1641), //ftintrm_w_s
  INSTL(ftintrne_l_d, BaseLVV, (0b0000000100011010111010, 0, 5, 0), 0, 0, 33, 1653), //ftintrne_l_d
  INSTL(ftintrne_l_s, BaseLVV, (0b0000000100011010111001, 0, 5, 0), 0, 0, 32, 1666), //ftintrne_l_s
  INSTL(ftintrne_w_d, BaseLVV, (0b0000000100011010110010, 0, 5, 0), 0, 0, 31, 1679), //ftintrne_w_d
  INSTL(ftintrne_w_s, BaseLVV, (0b0000000100011010110001, 0, 5, 0), 0, 0, 30, 1692), //ftintrne_w_s
  INSTL(ftintrp_l_d, BaseLVV, (0b0000000100011010011010, 0, 5, 0), 0, 0, 25, 1705), //ftintrp_l_d
  INSTL(ftintrp_l_s, BaseLVV, (0b0000000100011010011001, 0, 5, 0), 0, 0, 24, 1717), //ftintrp_l_s
  INSTL(ftintrp_w_d, BaseLVV, (0b0000000100011010010010, 0, 5, 0), 0, 0, 23, 1729), //ftintrp_w_d
  INSTL(ftintrp_w_s, BaseLVV, (0b0000000100011010010001, 0, 5, 0), 0, 0, 22, 1741), //ftintrp_w_s
  INSTL(ftintrz_l_d, BaseLVV, (0b0000000100011010101010, 0, 5, 0), 0, 0, 29, 1753), //ftintrz_l_d
  INSTL(ftintrz_l_s, BaseLVV, (0b0000000100011010101001, 0, 5, 0), 0, 0, 28, 1765), //ftintrz_l_s
  INSTL(ftintrz_w_d, BaseLVV, (0b0000000100011010100010, 0, 5, 0), 0, 0, 27, 1777), //ftintrz_w_d
  INSTL(ftintrz_w_s, BaseLVV, (0b0000000100011010100001, 0, 5, 0), 0, 0, 26, 1789), //ftintrz_w_s
  INSTL(ibar, BaseOpImm, (0b00111000011100101, 15, 0), 0, 0, 2, 1801), //ibar
  INSTL(idle, BaseOpImm, (0b00000110010010001, 15, 0), 0, 0, 0, 1806), //idle
  INSTL(invtlb, BaseLIRR, (0b00000110010010011, kWX, 5, kWX, 10, 0, 0), 0, 0, 0, 1811), //invtlb
  INSTL(iocsrrd_b, BaseLRR, (0b0000011001001000000000, kWX, 0, kWX, 5, 0), 0, 0, 26, 1818), //iocsrrd_b
  INSTL(iocsrrd_d, BaseLRR, (0b0000011001001000000011, kWX, 0, kWX, 5, 0), 0, 0, 29, 1828), //iocsrrd_d
  INSTL(iocsrrd_h, BaseLRR, (0b0000011001001000000001, kWX, 0, kWX, 5, 0), 0, 0, 27, 1838), //iocsrrd_h
  INSTL(iocsrrd_w, BaseLRR, (0b0000011001001000000010, kWX, 0, kWX, 5, 0), 0, 0, 28, 1848), //iocsrrd_w
  INSTL(iocsrwr_b, BaseLRR, (0b0000011001001000000100, kWX, 0, kWX, 5, 0), 0, 0, 30, 1858), //iocsrwr_b
  INSTL(iocsrwr_d, BaseLRR, (0b0000011001001000000111, kWX, 0, kWX, 5, 0), 0, 0, 33, 1868), //iocsrwr_d
  INSTL(iocsrwr_h, BaseLRR, (0b0000011001001000000101, kWX, 0, kWX, 5, 0), 0, 0, 31, 1878), //iocsrwr_h
  INSTL(iocsrwr_w, BaseLRR, (0b0000011001001000000110, kWX, 0, kWX, 5, 0), 0, 0, 32, 1888), //iocsrwr_w
  INSTL(jirl, JBTLRRI, (0b010011), 0, 0, 0, 1898), //jirl

  INSTL(ld_b,  BaseLdSt, (0b0010100000, 22), 0, 0, 0, 1903), //ld_b
  INSTL(ld_bu, BaseLdSt, (0b0010101000, 22), 0, 0, 8, 1908), //ld_bu
  INSTL(ld_d,  BaseLdSt, (0b0010100011, 22), 0, 0, 3, 1914), //ld_d
  INSTL(ld_h,  BaseLdSt, (0b0010100001, 22), 0, 0, 1, 1919), //ld_h
  INSTL(ld_hu, BaseLdSt, (0b0010101001, 22), 0, 0, 9, 1924), //ld_hu
  INSTL(ld_w,  BaseLdSt, (0b0010100010, 22), 0, 0, 2, 1930), //ld_w
  INSTL(ld_wu, BaseLdSt, (0b0010101010, 22), 0, 0, 10, 1935), //ld_wu

  INSTL(lddir, BaseLRRI, (0b00000110010000, kWX, 0, kWX, 5, 10, 4), 0, 0, 17, 1941), //lddir
  INSTL(ldgt_b, BaseLRRR, (0b00111000011110000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 57, 1947), //ldgt_b
  INSTL(ldgt_d, BaseLRRR, (0b00111000011110011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 60, 1954), //ldgt_d
  INSTL(ldgt_h, BaseLRRR, (0b00111000011110001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 58, 1961), //ldgt_h
  INSTL(ldgt_w, BaseLRRR, (0b00111000011110010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 59, 1968), //ldgt_w
  INSTL(ldle_b, BaseLRRR, (0b00111000011110100, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 61, 1975), //ldle_b
  INSTL(ldle_d, BaseLRRR, (0b00111000011110111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 64, 1982), //ldle_d
  INSTL(ldle_h, BaseLRRR, (0b00111000011110101, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 62, 1989), //ldle_h
  INSTL(ldle_w, BaseLRRR, (0b00111000011110110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 63, 1996), //ldle_w
  INSTL(ldpte, BaseLRI, (0b00000110010001, kWX, 5, 10, 4), 0, 0, 9, 2003), //ldpte
  INSTL(ldptr_d, LFPldst, (0b00100110), 0, 0, 1, 2009), //ldptr_d
  INSTL(ldptr_w, LFPldst, (0b00100100), 0, 0, 0, 2017), //ldptr_w

  INSTL(ldx_b,  BaseLdSt, (0b00111000000000000, 15), 0, 0, 11, 2025), //ldx_b
  INSTL(ldx_bu, BaseLdSt, (0b00111000001000000, 15), 0, 0, 19, 2031), //ldx_bu
  INSTL(ldx_d,  BaseLdSt, (0b00111000000011000, 15), 0, 0, 14, 2038), //ldx_d
  INSTL(ldx_h,  BaseLdSt, (0b00111000000001000, 15), 0, 0, 12, 2044), //ldx_h
  INSTL(ldx_hu, BaseLdSt, (0b00111000001001000, 15), 0, 0, 20, 2050), //ldx_hu
  INSTL(ldx_w,  BaseLdSt, (0b00111000000010000, 15), 0, 0, 13, 2057), //ldx_w
  INSTL(ldx_wu, BaseLdSt, (0b00111000001010000, 15), 0, 0, 21, 2063), //ldx_wu

  INSTL(ll_d, LFPldst, (0b00100010), 0, 0, 5, 2070), //ll_d
  INSTL(ll_w, LFPldst, (0b00100000), 0, 0, 4, 2075), //ll_w
  INSTL(lu12i_w, BaseLRI, (0b0001010, kWX, 0, 5, 0), 0, 0, 0, 2080), //lu12i_w
  INSTL(lu32i_d, BaseLRI, (0b0001011, kWX, 0, 5, 0), 0, 0, 1, 2088), //lu32i_d
  INSTL(lu52i_d, BaseLRRI, (0b0000001100, kX, kSP, 10), 0, 0, 4, 2096), //lu52i_d
  INSTL(maskeqz, BaseLRRR, (0b00000000000100110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 6, 2104), //maskeqz
  INSTL(masknez, BaseLRRR, (0b00000000000100111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 7, 2112), //masknez
  INSTL(mod_d, BaseLRRR, (0b00000000001000101, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 35, 2120), //mod_d
  INSTL(mod_du, BaseLRRR, (0b00000000001000111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 37, 2126), //mod_du
  INSTL(mod_w, BaseLRRR, (0b00000000001000001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 31, 2133), //mod_w
  INSTL(mod_wu, BaseLRRR, (0b00000000001000011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 33, 2139), //mod_wu
  INSTL(movcf2fr, BaseLVI, (0b0000000100010100110101, 0, 5), 0, 0, 0, 2146), //movcf2fr
  INSTL(movcf2gr, BaseLRI, (0b0000000100010100110111, kWX, 0, 5, 1), 0, 0, 6, 2155), //movcf2gr
  INSTL(movfcsr2gr, BaseLRR, (0b0000000100010100110010, kWX, 0, kWX, 5, 0), 0, 0, 25, 2164), //movfcsr2gr
  INSTL(movfr2cf, BaseLIV, (0b0000000100010100110100, 5, 0), 0, 0, 0, 2175), //movfr2cf
  INSTL(movfr2gr_d, BaseLRV, (0b0000000100010100101110, kWX, 0, 5, 0), 0, 0, 1, 2184), //movfr2gr_d
  INSTL(movfr2gr_s, BaseLRV, (0b0000000100010100101101, kWX, 0, 5, 0), 0, 0, 0, 2195), //movfr2gr_s
  INSTL(movfrh2gr_s, BaseLRV, (0b0000000100010100101111, kWX, 0, 5, 0), 0, 0, 2, 2206), //movfrh2gr_s
  INSTL(movgr2cf, BaseLIR, (0b0000000100010100110110, kWX, 5, 0), 0, 0, 0, 2218), //movgr2cf
  INSTL(movgr2fcsr, BaseLRR, (0b0000000100010100110000, kWX, 0, kWX, 5, 0), 0, 0, 24, 2227), //movgr2fcsr
  INSTL(movgr2fr_d, BaseLVR, (0b0000000100010100101010, 0, kWX, 5, 0), 0, 0, 1, 2238), //movgr2fr_d
  INSTL(movgr2fr_w, BaseLVR, (0b0000000100010100101001, 0, kWX, 5, 0), 0, 0, 0, 2249), //movgr2fr_w
  INSTL(movgr2frh_w, BaseLVR, (0b0000000100010100101011, 0, kWX, 5, 0), 0, 0, 2, 2260), //movgr2frh_w
  INSTL(mul_d, BaseLRRR, (0b00000000000111011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 25, 2272), //mul_d
  INSTL(mul_w, BaseLRRR, (0b00000000000111000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 22, 2278), //mul_w
  INSTL(mulh_d, BaseLRRR, (0b00000000000111100, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 26, 2284), //mulh_d
  INSTL(mulh_du, BaseLRRR, (0b00000000000111101, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 27, 2291), //mulh_du
  INSTL(mulh_w, BaseLRRR, (0b00000000000111001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 23, 2299), //mulh_w
  INSTL(mulh_wu, BaseLRRR, (0b00000000000111010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 24, 2306), //mulh_wu
  INSTL(mulw_d_w, BaseLRRR, (0b00000000000111110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 28, 2314), //mulw_d_w
  INSTL(mulw_d_wu, BaseLRRR, (0b00000000000111111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 29, 2323), //mulw_d_wu
  INSTL(nor, BaseLRRR, (0b00000000000101000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 8, 2333), //nor
  INSTL(or_, BaseLRRR, (0b00000000000101010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 10, 2337), //or_
  INSTL(ori, BaseLRRI, (0b0000001110, kX, kSP, 10), 0, 0, 6, 2341), //ori
  INSTL(orn, BaseLRRR, (0b00000000000101100, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 12, 2345), //orn
  INSTL(pcaddi, BaseLRI, (0b0001100, kWX, 0, 5, 0), 0, 0, 2, 2349), //pcaddi
  INSTL(pcaddu12i, BaseLRI, (0b0001110, kWX, 0, 5, 0), 0, 0, 4, 2356), //pcaddu12i
  INSTL(pcaddu18i, BaseLRI, (0b0001111, kWX, 0, 5, 0), 0, 0, 5, 2366), //pcaddu18i
  INSTL(pcalau12i, BaseLRI, (0b0001101, kWX, 0, 5, 0), 0, 0, 3, 2376), //pcalau12i
  INSTL(preld, LPldst, (0b0010101011), 0, 0, 0, 2386), //preld
  INSTL(rdtime_d, BaseLRR, (0b0000000000000000011010, kWX, 0, kWX, 5, 0), 0, 0, 23, 2392), //rdtime_d
  INSTL(rdtimeh_w, BaseLRR, (0b0000000000000000011001, kWX, 0, kWX, 5, 0), 0, 0, 22, 2401), //rdtimeh_w
  INSTL(rdtimel_w, BaseLRR, (0b0000000000000000011000, kWX, 0, kWX, 5, 0), 0, 0, 21, 2411), //rdtimel_w
  INSTL(revb_2h, BaseLRR, (0b0000000000000000001100, kWX, 0, kWX, 5, 0), 0, 0, 9, 2421), //revb_2h
  INSTL(revb_2w, BaseLRR, (0b0000000000000000001110, kWX, 0, kWX, 5, 0), 0, 0, 11, 2429), //revb_2w
  INSTL(revb_4h, BaseLRR, (0b0000000000000000001101, kWX, 0, kWX, 5, 0), 0, 0, 10, 2437), //revb_4h
  INSTL(revb_d, BaseLRR, (0b0000000000000000001111, kWX, 0, kWX, 5, 0), 0, 0, 12, 2445), //revb_d
  INSTL(revh_2w, BaseLRR, (0b0000000000000000010000, kWX, 0, kWX, 5, 0), 0, 0, 13, 2452), //revh_2w
  INSTL(revh_d, BaseLRR, (0b0000000000000000010001, kWX, 0, kWX, 5, 0), 0, 0, 14, 2460), //revh_d
  INSTL(rotr_d, BaseLRRR, (0b00000000000110111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 21, 2467), //rotr_d
  INSTL(rotr_w, BaseLRRR, (0b00000000000110110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 20, 2474), //rotr_w
  INSTL(rotri_d, BaseLRRI, (0b0000000001001101, kWX, 0, kWX, 5, 10, 2), 0, 0, 15, 2481), //rotri_d
  INSTL(rotri_w, BaseLRRI, (0b00000000010011001, kWX, 0, kWX, 5, 10, 1), 0, 0, 14, 2489), //rotri_w
  INSTL(sc_d, LFPldst, (0b00100011), 0, 0, 7, 2497), //sc_d
  INSTL(sc_w, LFPldst, (0b00100001), 0, 0, 6, 2502), //sc_w
  INSTL(sll_d, BaseLRRR, (0b00000000000110001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 17, 2507), //sll_d
  INSTL(sll_w, BaseLRRR, (0b00000000000101110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 14, 2513), //sll_w
  INSTL(slli_d, BaseLRRI, (0b0000000001000001, kWX, 0, kWX, 5, 10, 2), 0, 0, 9, 2519), //slli_d
  INSTL(slli_w, BaseLRRI, (0b00000000010000001, kWX, 0, kWX, 5, 10, 1), 0, 0, 8, 2526), //slli_w
  INSTL(slti, BaseLRRI, (0b0000001000, kX, kSP, 10), 0, 0, 0, 2533), //slti
  INSTL(sltui, BaseLRRI, (0b0000001001, kX, kSP, 10), 0, 0, 1, 2538), //sltui
  INSTL(sra_d, BaseLRRR, (0b00000000000110011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 19, 2544), //sra_d
  INSTL(sra_w, BaseLRRR, (0b00000000000110000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 16, 2550), //sra_w
  INSTL(srai_d, BaseLRRI, (0b0000000001001001, kWX, 0, kWX, 5, 10, 2), 0, 0, 13, 2556), //srai_d
  INSTL(srai_w, BaseLRRI, (0b00000000010010001, kWX, 0, kWX, 5, 10, 1), 0, 0, 12, 2563), //srai_w
  INSTL(srl_d, BaseLRRR, (0b00000000000110010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 18, 2570), //srl_d
  INSTL(srl_w, BaseLRRR, (0b00000000000101111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 15, 2576), //srl_w
  INSTL(srli_d, BaseLRRI, (0b0000000001000101, kWX, 0, kWX, 5, 10, 2), 0, 0, 11, 2582), //srli_d
  INSTL(srli_w, BaseLRRI, (0b00000000010001001, kWX, 0, kWX, 5, 10, 1), 0, 0, 10, 2589), //srli_w

  INSTL(st_b, BaseLdSt, (0b0010100100, 22), 0, 0, 4, 2596), //st_b
  INSTL(st_d, BaseLdSt, (0b0010100111, 22), 0, 0, 7, 2601), //st_d
  INSTL(st_h, BaseLdSt, (0b0010100101, 22), 0, 0, 5, 2606), //st_h
  INSTL(st_w, BaseLdSt, (0b0010100110, 22), 0, 0, 6, 2611), //st_w

  INSTL(stgt_b, BaseLRRR, (0b00111000011111000, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 65, 2616), //stgt_b
  INSTL(stgt_d, BaseLRRR, (0b00111000011111011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 68, 2623), //stgt_d
  INSTL(stgt_h, BaseLRRR, (0b00111000011111001, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 66, 2630), //stgt_h
  INSTL(stgt_w, BaseLRRR, (0b00111000011111010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 67, 2637), //stgt_w
  INSTL(stle_b, BaseLRRR, (0b00111000011111100, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 69, 2644), //stle_b
  INSTL(stle_d, BaseLRRR, (0b00111000011111111, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 72, 2651), //stle_d
  INSTL(stle_h, BaseLRRR, (0b00111000011111101, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 70, 2658), //stle_h
  INSTL(stle_w, BaseLRRR, (0b00111000011111110, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 71, 2665), //stle_w
  INSTL(stptr_d, LFPldst, (0b00100111), 0, 0, 3, 2672), //stptr_d
  INSTL(stptr_w, LFPldst, (0b00100101), 0, 0, 2, 2680), //stptr_w

  INSTL(stx_b, BaseLdSt, (0b00111000000100000, 15), 0, 0, 15, 2688), //stx_b
  INSTL(stx_d, BaseLdSt, (0b00111000000111000, 15), 0, 0, 18, 2694), //stx_d
  INSTL(stx_h, BaseLdSt, (0b00111000000101000, 15), 0, 0, 16, 2700), //stx_h
  INSTL(stx_w, BaseLdSt, (0b00111000000110000, 15), 0, 0, 17, 2706), //stx_w

  INSTL(sub_d, BaseLRRR, (0b00000000000100011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 3, 18412),
  INSTL(sub_w, BaseLRRR, (0b00000000000100010, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 2, 18418),
  INSTL(syscall, BaseLIC, (0b00000000001010110, 0), 0, 0, 2, 2712), //syscall
  INSTL(tlbclr, BaseOp, (0b00000110010010000010000000000000), 0, 0, 0, 2720), //tlbclr
  INSTL(tlbfill, BaseOp, (0b00000110010010000011010000000000), 0, 0, 5, 2727), //tlbfill
  INSTL(tlbflush, BaseOp, (0b00000110010010000010010000000000), 0, 0, 1, 2735), //tlbflush
  INSTL(tlbrd, BaseOp, (0b00000110010010000010110000000000), 0, 0, 3, 2744), //tlbrd
  INSTL(tlbsrch, BaseOp, (0b00000110010010000010100000000000), 0, 0, 2, 2750), //tlbsrch
  INSTL(tlbwr, BaseOp, (0b00000110010010000011000000000000), 0, 0, 4, 2758), //tlbwr
  INSTL(xor_, BaseLRRR, (0b00000000000101011, kWX, 0, kWX, 5, kWX, 10, 0), 0, 0, 11, 2764), //xor_
  INSTL(xori, BaseLRRI, (0b0000001111, kX, kSP, 10), 0, 0, 7, 2769), //xori
  INSTL(vabsd_b, lsxVVV, (0b01110000011000000, 0, 5, 10, 0), 0, 0, 104, 2774), //vabsd_b
  INSTL(vabsd_bu, lsxVVV, (0b01110000011000100, 0, 5, 10, 0), 0, 0, 108, 2782), //vabsd_bu
  INSTL(vabsd_d, lsxVVV, (0b01110000011000011, 0, 5, 10, 0), 0, 0, 107, 2791), //vabsd_d
  INSTL(vabsd_du, lsxVVV, (0b01110000011000111, 0, 5, 10, 0), 0, 0, 111, 2799), //vabsd_du
  INSTL(vabsd_h, lsxVVV, (0b01110000011000001, 0, 5, 10, 0), 0, 0, 105, 2808), //vabsd_h
  INSTL(vabsd_hu, lsxVVV, (0b01110000011000101, 0, 5, 10, 0), 0, 0, 109, 2816), //vabsd_hu
  INSTL(vabsd_w, lsxVVV, (0b01110000011000010, 0, 5, 10, 0), 0, 0, 106, 2825), //vabsd_w
  INSTL(vabsd_wu, lsxVVV, (0b01110000011000110, 0, 5, 10, 0), 0, 0, 110, 2833), //vabsd_wu
  INSTL(vadd_b, lsxVVV, (0b01110000000010100, 0, 5, 10, 0), 0, 0, 20, 2842), //vadd_b
  INSTL(vadd_d, lsxVVV, (0b01110000000010111, 0, 5, 10, 0), 0, 0, 23, 2849), //vadd_d
  INSTL(vadd_h, lsxVVV, (0b01110000000010101, 0, 5, 10, 0), 0, 0, 21, 2856), //vadd_h
  INSTL(vadd_q, lsxVVV, (0b01110001001011010, 0, 5, 10, 0), 0, 0, 332, 2863), //vadd_q
  INSTL(vadd_w, lsxVVV, (0b01110000000010110, 0, 5, 10, 0), 0, 0, 22, 2870), //vadd_w
  INSTL(vadda_b, lsxVVV, (0b01110000010111000, 0, 5, 10, 0), 0, 0, 100, 2877), //vadda_b
  INSTL(vadda_d, lsxVVV, (0b01110000010111011, 0, 5, 10, 0), 0, 0, 103, 2885), //vadda_d
  INSTL(vadda_h, lsxVVV, (0b01110000010111001, 0, 5, 10, 0), 0, 0, 101, 2893), //vadda_h
  INSTL(vadda_w, lsxVVV, (0b01110000010111010, 0, 5, 10, 0), 0, 0, 102, 2901), //vadda_w
  INSTL(vaddi_bu, lsxVVI, (0b01110010100010100, 0, 5, 10, 0), 0, 0, 20, 2909), //vaddi_bu
  INSTL(vaddi_du, lsxVVI, (0b01110010100010111, 0, 5, 10, 0), 0, 0, 23, 2918), //vaddi_du
  INSTL(vaddi_hu, lsxVVI, (0b01110010100010101, 0, 5, 10, 0), 0, 0, 21, 2927), //vaddi_hu
  INSTL(vaddi_wu, lsxVVI, (0b01110010100010110, 0, 5, 10, 0), 0, 0, 22, 2936), //vaddi_wu
  INSTL(vaddwev_d_w, lsxVVV, (0b01110000000111110, 0, 5, 10, 0), 0, 0, 30, 2945), //vaddwev_d_w
  INSTL(vaddwev_d_wu, lsxVVV, (0b01110000001011110, 0, 5, 10, 0), 0, 0, 46, 2957), //vaddwev_d_wu
  INSTL(vaddwev_d_wu_w, lsxVVV, (0b01110000001111110, 0, 5, 10, 0), 0, 0, 62, 2970), //vaddwev_d_wu_w
  INSTL(vaddwev_h_b, lsxVVV, (0b01110000000111100, 0, 5, 10, 0), 0, 0, 28, 2985), //vaddwev_h_b
  INSTL(vaddwev_h_bu, lsxVVV, (0b01110000001011100, 0, 5, 10, 0), 0, 0, 44, 2997), //vaddwev_h_bu
  INSTL(vaddwev_h_bu_b, lsxVVV, (0b01110000001111100, 0, 5, 10, 0), 0, 0, 60, 3010), //vaddwev_h_bu_b
  INSTL(vaddwev_q_d, lsxVVV, (0b01110000000111111, 0, 5, 10, 0), 0, 0, 31, 3025), //vaddwev_q_d
  INSTL(vaddwev_q_du, lsxVVV, (0b01110000001011111, 0, 5, 10, 0), 0, 0, 47, 3037), //vaddwev_q_du
  INSTL(vaddwev_q_du_d, lsxVVV, (0b01110000001111111, 0, 5, 10, 0), 0, 0, 63, 3050), //vaddwev_q_du_d
  INSTL(vaddwev_w_h, lsxVVV, (0b01110000000111101, 0, 5, 10, 0), 0, 0, 29, 3065), //vaddwev_w_h
  INSTL(vaddwev_w_hu, lsxVVV, (0b01110000001011101, 0, 5, 10, 0), 0, 0, 45, 3077), //vaddwev_w_hu
  INSTL(vaddwev_w_hu_h, lsxVVV, (0b01110000001111101, 0, 5, 10, 0), 0, 0, 61, 3090), //vaddwev_w_hu_h
  INSTL(vaddwod_d_w, lsxVVV, (0b01110000001000110, 0, 5, 10, 0), 0, 0, 38, 3105), //vaddwod_d_w
  INSTL(vaddwod_d_wu, lsxVVV, (0b01110000001100110, 0, 5, 10, 0), 0, 0, 54, 3117), //vaddwod_d_wu
  INSTL(vaddwod_d_wu_w, lsxVVV, (0b01110000010000010, 0, 5, 10, 0), 0, 0, 66, 3130), //vaddwod_d_wu_w
  INSTL(vaddwod_h_b, lsxVVV, (0b01110000001000100, 0, 5, 10, 0), 0, 0, 36, 3145), //vaddwod_h_b
  INSTL(vaddwod_h_bu, lsxVVV, (0b01110000001100100, 0, 5, 10, 0), 0, 0, 52, 3157), //vaddwod_h_bu
  INSTL(vaddwod_h_bu_b, lsxVVV, (0b01110000010000000, 0, 5, 10, 0), 0, 0, 64, 3170), //vaddwod_h_bu_b
  INSTL(vaddwod_q_d, lsxVVV, (0b01110000001000111, 0, 5, 10, 0), 0, 0, 39, 3185), //vaddwod_q_d
  INSTL(vaddwod_q_du, lsxVVV, (0b01110000001100111, 0, 5, 10, 0), 0, 0, 55, 3197), //vaddwod_q_du
  INSTL(vaddwod_q_du_d, lsxVVV, (0b01110000010000011, 0, 5, 10, 0), 0, 0, 67, 3210), //vaddwod_q_du_d
  INSTL(vaddwod_w_h, lsxVVV, (0b01110000001000101, 0, 5, 10, 0), 0, 0, 37, 3225), //vaddwod_w_h
  INSTL(vaddwod_w_hu, lsxVVV, (0b01110000001100101, 0, 5, 10, 0), 0, 0, 53, 3237), //vaddwod_w_hu
  INSTL(vaddwod_w_hu_h, lsxVVV, (0b01110000010000001, 0, 5, 10, 0), 0, 0, 65, 3250), //vaddwod_w_hu_h
  INSTL(vand_v, lsxVVV, (0b01110001001001100, 0, 5, 10, 0), 0, 0, 324, 3265), //vand_v
  INSTL(vandi_b, lsxVVI, (0b01110011110100, 0, 5, 10, 0), 0, 0, 155, 3272), //vandi_b
  INSTL(vandn_v, lsxVVV, (0b01110001001010000, 0, 5, 10, 0), 0, 0, 328, 3280), //vandn_v
  INSTL(vavg_b, lsxVVV, (0b01110000011001000, 0, 5, 10, 0), 0, 0, 112, 3288), //vavg_b
  INSTL(vavg_bu, lsxVVV, (0b01110000011001100, 0, 5, 10, 0), 0, 0, 116, 3295), //vavg_bu
  INSTL(vavg_d, lsxVVV, (0b01110000011001011, 0, 5, 10, 0), 0, 0, 115, 3303), //vavg_d
  INSTL(vavg_du, lsxVVV, (0b01110000011001111, 0, 5, 10, 0), 0, 0, 119, 3310), //vavg_du
  INSTL(vavg_h, lsxVVV, (0b01110000011001001, 0, 5, 10, 0), 0, 0, 113, 3318), //vavg_h
  INSTL(vavg_hu, lsxVVV, (0b01110000011001101, 0, 5, 10, 0), 0, 0, 117, 3325), //vavg_hu
  INSTL(vavg_w, lsxVVV, (0b01110000011001010, 0, 5, 10, 0), 0, 0, 114, 3333), //vavg_w
  INSTL(vavg_wu, lsxVVV, (0b01110000011001110, 0, 5, 10, 0), 0, 0, 118, 3340), //vavg_wu
  INSTL(vavgr_b, lsxVVV, (0b01110000011010000, 0, 5, 10, 0), 0, 0, 120, 3348), //vavgr_b
  INSTL(vavgr_bu, lsxVVV, (0b01110000011010100, 0, 5, 10, 0), 0, 0, 124, 3356), //vavgr_bu
  INSTL(vavgr_d, lsxVVV, (0b01110000011010011, 0, 5, 10, 0), 0, 0, 123, 3365), //vavgr_d
  INSTL(vavgr_du, lsxVVV, (0b01110000011010111, 0, 5, 10, 0), 0, 0, 127, 3373), //vavgr_du
  INSTL(vavgr_h, lsxVVV, (0b01110000011010001, 0, 5, 10, 0), 0, 0, 121, 3382), //vavgr_h
  INSTL(vavgr_hu, lsxVVV, (0b01110000011010101, 0, 5, 10, 0), 0, 0, 125, 3390), //vavgr_hu
  INSTL(vavgr_w, lsxVVV, (0b01110000011010010, 0, 5, 10, 0), 0, 0, 122, 3399), //vavgr_w
  INSTL(vavgr_wu, lsxVVV, (0b01110000011010110, 0, 5, 10, 0), 0, 0, 126, 3407), //vavgr_wu
  INSTL(vbitclr_b, lsxVVV, (0b01110001000011000, 0, 5, 10, 0), 0, 0, 288, 3416), //vbitclr_b
  INSTL(vbitclr_d, lsxVVV, (0b01110001000011011, 0, 5, 10, 0), 0, 0, 291, 3426), //vbitclr_d
  INSTL(vbitclr_h, lsxVVV, (0b01110001000011001, 0, 5, 10, 0), 0, 0, 289, 3436), //vbitclr_h
  INSTL(vbitclr_w, lsxVVV, (0b01110001000011010, 0, 5, 10, 0), 0, 0, 290, 3446), //vbitclr_w
  INSTL(vbitclri_b, lsxVVI, (0b0111001100010000001, 0, 5, 10, 0), 0, 0, 70, 3456), //vbitclri_b
  INSTL(vbitclri_d, lsxVVI, (0b0111001100010001, 0, 5, 10, 0), 0, 0, 73, 3467), //vbitclri_d
  INSTL(vbitclri_h, lsxVVI, (0b011100110001000001, 0, 5, 10, 0), 0, 0, 71, 3478), //vbitclri_h
  INSTL(vbitclri_w, lsxVVI, (0b01110011000100001, 0, 5, 10, 0), 0, 0, 72, 3489), //vbitclri_w
  INSTL(vbitrev_b, lsxVVV, (0b01110001000100000, 0, 5, 10, 0), 0, 0, 296, 3500), //vbitrev_b
  INSTL(vbitrev_d, lsxVVV, (0b01110001000100011, 0, 5, 10, 0), 0, 0, 299, 3510), //vbitrev_d
  INSTL(vbitrev_h, lsxVVV, (0b01110001000100001, 0, 5, 10, 0), 0, 0, 297, 3520), //vbitrev_h
  INSTL(vbitrev_w, lsxVVV, (0b01110001000100010, 0, 5, 10, 0), 0, 0, 298, 3530), //vbitrev_w
  INSTL(vbitrevi_b, lsxVVI, (0b0111001100011000001, 0, 5, 10, 0), 0, 0, 78, 3540), //vbitrevi_b
  INSTL(vbitrevi_d, lsxVVI, (0b0111001100011001, 0, 5, 10, 0), 0, 0, 81, 3551), //vbitrevi_d
  INSTL(vbitrevi_h, lsxVVI, (0b011100110001100001, 0, 5, 10, 0), 0, 0, 79, 3562), //vbitrevi_h
  INSTL(vbitrevi_w, lsxVVI, (0b01110011000110001, 0, 5, 10, 0), 0, 0, 80, 3573), //vbitrevi_w
  INSTL(vbitsel_v, lsxVVVV, (0b000011010001, 0, 5, 10, 15, 0), 0, 0, 8, 3584), //vbitsel_v
  INSTL(vbitseli_b, lsxVVI, (0b01110011110001, 0, 5, 10, 0), 0, 0, 154, 3594), //vbitseli_b
  INSTL(vbitset_b, lsxVVV, (0b01110001000011100, 0, 5, 10, 0), 0, 0, 292, 3605), //vbitset_b
  INSTL(vbitset_d, lsxVVV, (0b01110001000011111, 0, 5, 10, 0), 0, 0, 295, 3615), //vbitset_d
  INSTL(vbitset_h, lsxVVV, (0b01110001000011101, 0, 5, 10, 0), 0, 0, 293, 3625), //vbitset_h
  INSTL(vbitset_w, lsxVVV, (0b01110001000011110, 0, 5, 10, 0), 0, 0, 294, 3635), //vbitset_w
  INSTL(vbitseti_b, lsxVVI, (0b0111001100010100001, 0, 5, 10, 0), 0, 0, 74, 3645), //vbitseti_b
  INSTL(vbitseti_d, lsxVVI, (0b0111001100010101, 0, 5, 10, 0), 0, 0, 77, 3656), //vbitseti_d
  INSTL(vbitseti_h, lsxVVI, (0b011100110001010001, 0, 5, 10, 0), 0, 0, 75, 3667), //vbitseti_h
  INSTL(vbitseti_w, lsxVVI, (0b01110011000101001, 0, 5, 10, 0), 0, 0, 76, 3678), //vbitseti_w
  INSTL(vbsll_v, lsxVVI, (0b01110010100011100, 0, 5, 10, 0), 0, 0, 28, 3689), //vbsll_v
  INSTL(vbsrl_v, lsxVVI, (0b01110010100011101, 0, 5, 10, 0), 0, 0, 29, 3697), //vbsrl_v
  INSTL(vclo_b, lsxVV, (0b0111001010011100000000, 0, 5, 0), 0, 0, 0, 3705), //vclo_b
  INSTL(vclo_d, lsxVV, (0b0111001010011100000011, 0, 5, 0), 0, 0, 3, 3712), //vclo_d
  INSTL(vclo_h, lsxVV, (0b0111001010011100000001, 0, 5, 0), 0, 0, 1, 3719), //vclo_h
  INSTL(vclo_w, lsxVV, (0b0111001010011100000010, 0, 5, 0), 0, 0, 2, 3726), //vclo_w
  INSTL(vclz_b, lsxVV, (0b0111001010011100000100, 0, 5, 0), 0, 0, 4, 3733), //vclz_b
  INSTL(vclz_d, lsxVV, (0b0111001010011100000111, 0, 5, 0), 0, 0, 7, 3740), //vclz_d
  INSTL(vclz_h, lsxVV, (0b0111001010011100000101, 0, 5, 0), 0, 0, 5, 3747), //vclz_h
  INSTL(vclz_w, lsxVV, (0b0111001010011100000110, 0, 5, 0), 0, 0, 6, 3754), //vclz_w
  INSTL(vdiv_b, lsxVVV, (0b01110000111000000, 0, 5, 10, 0), 0, 0, 212, 3761), //vdiv_b
  INSTL(vdiv_bu, lsxVVV, (0b01110000111001000, 0, 5, 10, 0), 0, 0, 220, 3768), //vdiv_bu
  INSTL(vdiv_d, lsxVVV, (0b01110000111000011, 0, 5, 10, 0), 0, 0, 215, 3776), //vdiv_d
  INSTL(vdiv_du, lsxVVV, (0b01110000111001011, 0, 5, 10, 0), 0, 0, 223, 3783), //vdiv_du
  INSTL(vdiv_h, lsxVVV, (0b01110000111000001, 0, 5, 10, 0), 0, 0, 213, 3791), //vdiv_h
  INSTL(vdiv_hu, lsxVVV, (0b01110000111001001, 0, 5, 10, 0), 0, 0, 221, 3798), //vdiv_hu
  INSTL(vdiv_w, lsxVVV, (0b01110000111000010, 0, 5, 10, 0), 0, 0, 214, 3806), //vdiv_w
  INSTL(vdiv_wu, lsxVVV, (0b01110000111001010, 0, 5, 10, 0), 0, 0, 222, 3813), //vdiv_wu
  INSTL(vext2xv_d_b, lasxXX, (0b0111011010011111000110, 0, 5, 0), 0, 0, 86, 3821), //vext2xv_d_b
  INSTL(vext2xv_d_h, lasxXX, (0b0111011010011111001000, 0, 5, 0), 0, 0, 88, 3833), //vext2xv_d_h
  INSTL(vext2xv_d_w, lasxXX, (0b0111011010011111001001, 0, 5, 0), 0, 0, 89, 3845), //vext2xv_d_w
  INSTL(vext2xv_du_bu, lasxXX, (0b0111011010011111001100, 0, 5, 0), 0, 0, 92, 3857), //vext2xv_du_bu
  INSTL(vext2xv_du_hu, lasxXX, (0b0111011010011111001110, 0, 5, 0), 0, 0, 94, 3871), //vext2xv_du_hu
  INSTL(vext2xv_du_wu, lasxXX, (0b0111011010011111001111, 0, 5, 0), 0, 0, 95, 3885), //vext2xv_du_wu
  INSTL(vext2xv_h_b, lasxXX, (0b0111011010011111000100, 0, 5, 0), 0, 0, 84, 3899), //vext2xv_h_b
  INSTL(vext2xv_hu_bu, lasxXX, (0b0111011010011111001010, 0, 5, 0), 0, 0, 90, 3911), //vext2xv_hu_bu
  INSTL(vext2xv_w_b, lasxXX, (0b0111011010011111000101, 0, 5, 0), 0, 0, 85, 3925), //vext2xv_w_b
  INSTL(vext2xv_w_h, lasxXX, (0b0111011010011111000111, 0, 5, 0), 0, 0, 87, 3937), //vext2xv_w_h
  INSTL(vext2xv_wu_bu, lasxXX, (0b0111011010011111001011, 0, 5, 0), 0, 0, 91, 3949), //vext2xv_wu_bu
  INSTL(vext2xv_wu_hu, lasxXX, (0b0111011010011111001101, 0, 5, 0), 0, 0, 93, 3963), //vext2xv_wu_hu
  INSTL(vexth_d_w, lsxVV, (0b0111001010011110111010, 0, 5, 0), 0, 0, 78, 3977), //vexth_d_w
  INSTL(vexth_du_wu, lsxVV, (0b0111001010011110111110, 0, 5, 0), 0, 0, 82, 3987), //vexth_du_wu
  INSTL(vexth_h_b, lsxVV, (0b0111001010011110111000, 0, 5, 0), 0, 0, 76, 3999), //vexth_h_b
  INSTL(vexth_hu_bu, lsxVV, (0b0111001010011110111100, 0, 5, 0), 0, 0, 80, 4009), //vexth_hu_bu
  INSTL(vexth_q_d, lsxVV, (0b0111001010011110111011, 0, 5, 0), 0, 0, 79, 4021), //vexth_q_d
  INSTL(vexth_qu_du, lsxVV, (0b0111001010011110111111, 0, 5, 0), 0, 0, 83, 4031), //vexth_qu_du
  INSTL(vexth_w_h, lsxVV, (0b0111001010011110111001, 0, 5, 0), 0, 0, 77, 4043), //vexth_w_h
  INSTL(vexth_wu_hu, lsxVV, (0b0111001010011110111101, 0, 5, 0), 0, 0, 81, 4053), //vexth_wu_hu
  INSTL(vextl_q_d, lsxVV, (0b0111001100001001000000, 0, 5, 0), 0, 0, 84, 4065), //vextl_q_d
  INSTL(vextl_qu_du, lsxVV, (0b0111001100001101000000, 0, 5, 0), 0, 0, 85, 4075), //vextl_qu_du
  INSTL(vextrins_b, lsxVVI, (0b01110011100011, 0, 5, 10, 0), 0, 0, 149, 4087), //vextrins_b
  INSTL(vextrins_d, lsxVVI, (0b01110011100000, 0, 5, 10, 0), 0, 0, 146, 4098), //vextrins_d
  INSTL(vextrins_h, lsxVVI, (0b01110011100010, 0, 5, 10, 0), 0, 0, 148, 4109), //vextrins_h
  INSTL(vextrins_w, lsxVVI, (0b01110011100001, 0, 5, 10, 0), 0, 0, 147, 4120), //vextrins_w
  INSTL(vfadd_d, lsxVVV, (0b01110001001100010, 0, 5, 10, 0), 0, 0, 339, 4131), //vfadd_d
  INSTL(vfadd_s, lsxVVV, (0b01110001001100001, 0, 5, 10, 0), 0, 0, 338, 4139), //vfadd_s
  INSTL(vfclass_d, lsxVV, (0b0111001010011100110110, 0, 5, 0), 0, 0, 25, 4147), //vfclass_d
  INSTL(vfclass_s, lsxVV, (0b0111001010011100110101, 0, 5, 0), 0, 0, 24, 4157), //vfclass_s
  INSTL(vfcvt_h_s, lsxVVV, (0b01110001010001100, 0, 5, 10, 0), 0, 0, 356, 4167), //vfcvt_h_s
  INSTL(vfcvt_s_d, lsxVVV, (0b01110001010001101, 0, 5, 10, 0), 0, 0, 357, 4177), //vfcvt_s_d
  INSTL(vfcvth_d_s, lsxVV, (0b0111001010011101111101, 0, 5, 0), 0, 0, 45, 4187), //vfcvth_d_s
  INSTL(vfcvth_s_h, lsxVV, (0b0111001010011101111011, 0, 5, 0), 0, 0, 43, 4198), //vfcvth_s_h
  INSTL(vfcvtl_d_s, lsxVV, (0b0111001010011101111100, 0, 5, 0), 0, 0, 44, 4209), //vfcvtl_d_s
  INSTL(vfcvtl_s_h, lsxVV, (0b0111001010011101111010, 0, 5, 0), 0, 0, 42, 4220), //vfcvtl_s_h
  INSTL(vfdiv_d, lsxVVV, (0b01110001001110110, 0, 5, 10, 0), 0, 0, 345, 4231), //vfdiv_d
  INSTL(vfdiv_s, lsxVVV, (0b01110001001110101, 0, 5, 10, 0), 0, 0, 344, 4239), //vfdiv_s
  INSTL(vffint_d_l, lsxVV, (0b0111001010011110000010, 0, 5, 0), 0, 0, 48, 4247), //vffint_d_l
  INSTL(vffint_d_lu, lsxVV, (0b0111001010011110000011, 0, 5, 0), 0, 0, 49, 4258), //vffint_d_lu
  INSTL(vffint_s_l, lsxVVV, (0b01110001010010000, 0, 5, 10, 0), 0, 0, 358, 4270), //vffint_s_l
  INSTL(vffint_s_w, lsxVV, (0b0111001010011110000000, 0, 5, 0), 0, 0, 46, 4281), //vffint_s_w
  INSTL(vffint_s_wu, lsxVV, (0b0111001010011110000001, 0, 5, 0), 0, 0, 47, 4292), //vffint_s_wu
  INSTL(vffinth_d_w, lsxVV, (0b0111001010011110000101, 0, 5, 0), 0, 0, 51, 4304), //vffinth_d_w
  INSTL(vffintl_d_w, lsxVV, (0b0111001010011110000100, 0, 5, 0), 0, 0, 50, 4316), //vffintl_d_w
  INSTL(vflogb_d, lsxVV, (0b0111001010011100110010, 0, 5, 0), 0, 0, 23, 4328), //vflogb_d
  INSTL(vflogb_s, lsxVV, (0b0111001010011100110001, 0, 5, 0), 0, 0, 22, 4337), //vflogb_s
  INSTL(vfmadd_d, lsxVVVV, (0b000010010010, 0, 5, 10, 15, 0), 0, 0, 1, 4346), //vfmadd_d
  INSTL(vfmadd_s, lsxVVVV, (0b000010010001, 0, 5, 10, 15, 0), 0, 0, 0, 4355), //vfmadd_s
  INSTL(vfmax_d, lsxVVV, (0b01110001001111010, 0, 5, 10, 0), 0, 0, 347, 4364), //vfmax_d
  INSTL(vfmax_s, lsxVVV, (0b01110001001111001, 0, 5, 10, 0), 0, 0, 346, 4372), //vfmax_s
  INSTL(vfmaxa_d, lsxVVV, (0b01110001010000010, 0, 5, 10, 0), 0, 0, 351, 4380), //vfmaxa_d
  INSTL(vfmaxa_s, lsxVVV, (0b01110001010000001, 0, 5, 10, 0), 0, 0, 350, 4389), //vfmaxa_s
  INSTL(vfmin_d, lsxVVV, (0b01110001001111110, 0, 5, 10, 0), 0, 0, 349, 4398), //vfmin_d
  INSTL(vfmin_s, lsxVVV, (0b01110001001111101, 0, 5, 10, 0), 0, 0, 348, 4406), //vfmin_s
  INSTL(vfmina_d, lsxVVV, (0b01110001010000110, 0, 5, 10, 0), 0, 0, 353, 4414), //vfmina_d
  INSTL(vfmina_s, lsxVVV, (0b01110001010000101, 0, 5, 10, 0), 0, 0, 352, 4423), //vfmina_s
  INSTL(vfmsub_d, lsxVVVV, (0b000010010110, 0, 5, 10, 15, 0), 0, 0, 3, 4432), //vfmsub_d
  INSTL(vfmsub_s, lsxVVVV, (0b000010010101, 0, 5, 10, 15, 0), 0, 0, 2, 4441), //vfmsub_s
  INSTL(vfmul_d, lsxVVV, (0b01110001001110010, 0, 5, 10, 0), 0, 0, 343, 4450), //vfmul_d
  INSTL(vfmul_s, lsxVVV, (0b01110001001110001, 0, 5, 10, 0), 0, 0, 342, 4458), //vfmul_s
  INSTL(vfnmadd_d, lsxVVVV, (0b000010011010, 0, 5, 10, 15, 0), 0, 0, 5, 4466), //vfnmadd_d
  INSTL(vfnmadd_s, lsxVVVV, (0b000010011001, 0, 5, 10, 15, 0), 0, 0, 4, 4476), //vfnmadd_s
  INSTL(vfnmsub_d, lsxVVVV, (0b000010011110, 0, 5, 10, 15, 0), 0, 0, 7, 4486), //vfnmsub_d
  INSTL(vfnmsub_s, lsxVVVV, (0b000010011101, 0, 5, 10, 15, 0), 0, 0, 6, 4496), //vfnmsub_s
  INSTL(vfrecip_d, lsxVV, (0b0111001010011100111110, 0, 5, 0), 0, 0, 29, 4506), //vfrecip_d
  INSTL(vfrecip_s, lsxVV, (0b0111001010011100111101, 0, 5, 0), 0, 0, 28, 4516), //vfrecip_s
  INSTL(vfrint_d, lsxVV, (0b0111001010011101001110, 0, 5, 0), 0, 0, 33, 4526), //vfrint_d
  INSTL(vfrint_s, lsxVV, (0b0111001010011101001101, 0, 5, 0), 0, 0, 32, 4535), //vfrint_s
  INSTL(vfrintrm_d, lsxVV, (0b0111001010011101010010, 0, 5, 0), 0, 0, 35, 4544), //vfrintrm_d
  INSTL(vfrintrm_s, lsxVV, (0b0111001010011101010001, 0, 5, 0), 0, 0, 34, 4555), //vfrintrm_s
  INSTL(vfrintrne_d, lsxVV, (0b0111001010011101011110, 0, 5, 0), 0, 0, 41, 4566), //vfrintrne_d
  INSTL(vfrintrne_s, lsxVV, (0b0111001010011101011101, 0, 5, 0), 0, 0, 40, 4578), //vfrintrne_s
  INSTL(vfrintrp_d, lsxVV, (0b0111001010011101010110, 0, 5, 0), 0, 0, 37, 4590), //vfrintrp_d
  INSTL(vfrintrp_s, lsxVV, (0b0111001010011101010101, 0, 5, 0), 0, 0, 36, 4601), //vfrintrp_s
  INSTL(vfrintrz_d, lsxVV, (0b0111001010011101011010, 0, 5, 0), 0, 0, 39, 4612), //vfrintrz_d
  INSTL(vfrintrz_s, lsxVV, (0b0111001010011101011001, 0, 5, 0), 0, 0, 38, 4623), //vfrintrz_s
  INSTL(vfrsqrt_d, lsxVV, (0b0111001010011101000010, 0, 5, 0), 0, 0, 31, 4634), //vfrsqrt_d
  INSTL(vfrsqrt_s, lsxVV, (0b0111001010011101000001, 0, 5, 0), 0, 0, 30, 4644), //vfrsqrt_s
  INSTL(vfrstp_b, lsxVVV, (0b01110001001010110, 0, 5, 10, 0), 0, 0, 330, 4654), //vfrstp_b
  INSTL(vfrstp_h, lsxVVV, (0b01110001001010111, 0, 5, 10, 0), 0, 0, 331, 4663), //vfrstp_h
  INSTL(vfrstpi_b, lsxVVI, (0b01110010100110100, 0, 5, 10, 0), 0, 0, 46, 4672), //vfrstpi_b
  INSTL(vfrstpi_h, lsxVVI, (0b01110010100110101, 0, 5, 10, 0), 0, 0, 47, 4682), //vfrstpi_h
  INSTL(vfscaleb_d, lsxVVV, (0b01110001010001010, 0, 5, 10, 0), 0, 0, 355, 4692), //vfscaleb_d
  INSTL(vfscaleb_s, lsxVVV, (0b01110001010001001, 0, 5, 10, 0), 0, 0, 354, 4703), //vfscaleb_s
  INSTL(vfsqrt_d, lsxVV, (0b0111001010011100111010, 0, 5, 0), 0, 0, 27, 4714), //vfsqrt_d
  INSTL(vfsqrt_s, lsxVV, (0b0111001010011100111001, 0, 5, 0), 0, 0, 26, 4723), //vfsqrt_s
  INSTL(vfsub_d, lsxVVV, (0b01110001001100110, 0, 5, 10, 0), 0, 0, 341, 4732), //vfsub_d
  INSTL(vfsub_s, lsxVVV, (0b01110001001100101, 0, 5, 10, 0), 0, 0, 340, 4740), //vfsub_s
  INSTL(vftint_l_d, lsxVV, (0b0111001010011110001101, 0, 5, 0), 0, 0, 53, 4748), //vftint_l_d
  INSTL(vftint_lu_d, lsxVV, (0b0111001010011110010111, 0, 5, 0), 0, 0, 63, 4759), //vftint_lu_d
  INSTL(vftint_w_d, lsxVVV, (0b01110001010010011, 0, 5, 10, 0), 0, 0, 359, 4771), //vftint_w_d
  INSTL(vftint_w_s, lsxVV, (0b0111001010011110001100, 0, 5, 0), 0, 0, 52, 4782), //vftint_w_s
  INSTL(vftint_wu_s, lsxVV, (0b0111001010011110010110, 0, 5, 0), 0, 0, 62, 4793), //vftint_wu_s
  INSTL(vftinth_l_s, lsxVV, (0b0111001010011110100001, 0, 5, 0), 0, 0, 67, 4805), //vftinth_l_s
  INSTL(vftintl_l_s, lsxVV, (0b0111001010011110100000, 0, 5, 0), 0, 0, 66, 4817), //vftintl_l_s
  INSTL(vftintrm_l_d, lsxVV, (0b0111001010011110001111, 0, 5, 0), 0, 0, 55, 4829), //vftintrm_l_d
  INSTL(vftintrm_w_d, lsxVVV, (0b01110001010010100, 0, 5, 10, 0), 0, 0, 360, 4842), //vftintrm_w_d
  INSTL(vftintrm_w_s, lsxVV, (0b0111001010011110001110, 0, 5, 0), 0, 0, 54, 4855), //vftintrm_w_s
  INSTL(vftintrmh_l_s, lsxVV, (0b0111001010011110100011, 0, 5, 0), 0, 0, 69, 4868), //vftintrmh_l_s
  INSTL(vftintrml_l_s, lsxVV, (0b0111001010011110100010, 0, 5, 0), 0, 0, 68, 4882), //vftintrml_l_s
  INSTL(vftintrne_l_d, lsxVV, (0b0111001010011110010101, 0, 5, 0), 0, 0, 61, 4896), //vftintrne_l_d
  INSTL(vftintrne_w_d, lsxVVV, (0b01110001010010111, 0, 5, 10, 0), 0, 0, 363, 4910), //vftintrne_w_d
  INSTL(vftintrne_w_s, lsxVV, (0b0111001010011110010100, 0, 5, 0), 0, 0, 60, 4924), //vftintrne_w_s
  INSTL(vftintrneh_l_s, lsxVV, (0b0111001010011110101001, 0, 5, 0), 0, 0, 75, 4938), //vftintrneh_l_s
  INSTL(vftintrnel_l_s, lsxVV, (0b0111001010011110101000, 0, 5, 0), 0, 0, 74, 4953), //vftintrnel_l_s
  INSTL(vftintrp_l_d, lsxVV, (0b0111001010011110010001, 0, 5, 0), 0, 0, 57, 4968), //vftintrp_l_d
  INSTL(vftintrp_w_d, lsxVVV, (0b01110001010010101, 0, 5, 10, 0), 0, 0, 361, 4981), //vftintrp_w_d
  INSTL(vftintrp_w_s, lsxVV, (0b0111001010011110010000, 0, 5, 0), 0, 0, 56, 4994), //vftintrp_w_s
  INSTL(vftintrph_l_s, lsxVV, (0b0111001010011110100101, 0, 5, 0), 0, 0, 71, 5007), //vftintrph_l_s
  INSTL(vftintrpl_l_s, lsxVV, (0b0111001010011110100100, 0, 5, 0), 0, 0, 70, 5021), //vftintrpl_l_s
  INSTL(vftintrz_l_d, lsxVV, (0b0111001010011110010011, 0, 5, 0), 0, 0, 59, 5035), //vftintrz_l_d
  INSTL(vftintrz_lu_d, lsxVV, (0b0111001010011110011101, 0, 5, 0), 0, 0, 65, 5048), //vftintrz_lu_d
  INSTL(vftintrz_w_d, lsxVVV, (0b01110001010010110, 0, 5, 10, 0), 0, 0, 362, 5062), //vftintrz_w_d
  INSTL(vftintrz_w_s, lsxVV, (0b0111001010011110010010, 0, 5, 0), 0, 0, 58, 5075), //vftintrz_w_s
  INSTL(vftintrz_wu_s, lsxVV, (0b0111001010011110011100, 0, 5, 0), 0, 0, 64, 5088), //vftintrz_wu_s
  INSTL(vftintrzh_l_s, lsxVV, (0b0111001010011110100111, 0, 5, 0), 0, 0, 73, 5102), //vftintrzh_l_s
  INSTL(vftintrzl_l_s, lsxVV, (0b0111001010011110100110, 0, 5, 0), 0, 0, 72, 5116), //vftintrzl_l_s
  INSTL(vhaddw_d_w, lsxVVV, (0b01110000010101010, 0, 5, 10, 0), 0, 0, 86, 5130), //vhaddw_d_w
  INSTL(vhaddw_du_wu, lsxVVV, (0b01110000010110010, 0, 5, 10, 0), 0, 0, 94, 5141), //vhaddw_du_wu
  INSTL(vhaddw_h_b, lsxVVV, (0b01110000010101000, 0, 5, 10, 0), 0, 0, 84, 5154), //vhaddw_h_b
  INSTL(vhaddw_hu_bu, lsxVVV, (0b01110000010110000, 0, 5, 10, 0), 0, 0, 92, 5165), //vhaddw_hu_bu
  INSTL(vhaddw_q_d, lsxVVV, (0b01110000010101011, 0, 5, 10, 0), 0, 0, 87, 5178), //vhaddw_q_d
  INSTL(vhaddw_qu_du, lsxVVV, (0b01110000010110011, 0, 5, 10, 0), 0, 0, 95, 5189), //vhaddw_qu_du
  INSTL(vhaddw_w_h, lsxVVV, (0b01110000010101001, 0, 5, 10, 0), 0, 0, 85, 5202), //vhaddw_w_h
  INSTL(vhaddw_wu_hu, lsxVVV, (0b01110000010110001, 0, 5, 10, 0), 0, 0, 93, 5213), //vhaddw_wu_hu
  INSTL(vhsubw_d_w, lsxVVV, (0b01110000010101110, 0, 5, 10, 0), 0, 0, 90, 5226), //vhsubw_d_w
  INSTL(vhsubw_du_wu, lsxVVV, (0b01110000010110110, 0, 5, 10, 0), 0, 0, 98, 5237), //vhsubw_du_wu
  INSTL(vhsubw_h_b, lsxVVV, (0b01110000010101100, 0, 5, 10, 0), 0, 0, 88, 5250), //vhsubw_h_b
  INSTL(vhsubw_hu_bu, lsxVVV, (0b01110000010110100, 0, 5, 10, 0), 0, 0, 96, 5261), //vhsubw_hu_bu
  INSTL(vhsubw_q_d, lsxVVV, (0b01110000010101111, 0, 5, 10, 0), 0, 0, 91, 5274), //vhsubw_q_d
  INSTL(vhsubw_qu_du, lsxVVV, (0b01110000010110111, 0, 5, 10, 0), 0, 0, 99, 5285), //vhsubw_qu_du
  INSTL(vhsubw_w_h, lsxVVV, (0b01110000010101101, 0, 5, 10, 0), 0, 0, 89, 5298), //vhsubw_w_h
  INSTL(vhsubw_wu_hu, lsxVVV, (0b01110000010110101, 0, 5, 10, 0), 0, 0, 97, 5309), //vhsubw_wu_hu
  INSTL(vilvh_b, lsxVVV, (0b01110001000111000, 0, 5, 10, 0), 0, 0, 312, 5322), //vilvh_b
  INSTL(vilvh_d, lsxVVV, (0b01110001000111011, 0, 5, 10, 0), 0, 0, 315, 5330), //vilvh_d
  INSTL(vilvh_h, lsxVVV, (0b01110001000111001, 0, 5, 10, 0), 0, 0, 313, 5338), //vilvh_h
  INSTL(vilvh_w, lsxVVV, (0b01110001000111010, 0, 5, 10, 0), 0, 0, 314, 5346), //vilvh_w
  INSTL(vilvl_b, lsxVVV, (0b01110001000110100, 0, 5, 10, 0), 0, 0, 308, 5354), //vilvl_b
  INSTL(vilvl_d, lsxVVV, (0b01110001000110111, 0, 5, 10, 0), 0, 0, 311, 5362), //vilvl_d
  INSTL(vilvl_h, lsxVVV, (0b01110001000110101, 0, 5, 10, 0), 0, 0, 309, 5370), //vilvl_h
  INSTL(vilvl_w, lsxVVV, (0b01110001000110110, 0, 5, 10, 0), 0, 0, 310, 5378), //vilvl_w
  INSTL(vinsgr2vr_b, lsxVRI, (0b011100101110101110, 0, kWX, 5, 10, 0), 0, 0, 0, 5386), //vinsgr2vr_b
  INSTL(vinsgr2vr_d, lsxVRI, (0b011100101110101111110, 0, kWX, 5, 10, 0), 0, 0, 3, 5398), //vinsgr2vr_d
  INSTL(vinsgr2vr_h, lsxVRI, (0b0111001011101011110, 0, kWX, 5, 10, 0), 0, 0, 1, 5410), //vinsgr2vr_h
  INSTL(vinsgr2vr_w, lsxVRI, (0b01110010111010111110, 0, kWX, 5, 10, 0), 0, 0, 2, 5422), //vinsgr2vr_w
  INSTL(vld,       SimdLdst, (0b0010110000,        22, 12), 0, 0, 6, 5434), //vld
  INSTL(vldi, lsxVI, (0b01110011111000, 0, 5, 0), 0, 0, 0, 5438), //vldi
  INSTL(vldrepl_b, SimdLdst, (0b0011000010,        22, 12), 0, 0, 8, 5443), //vldrepl_b
  INSTL(vldrepl_d, SimdLdst, (0b0011000000010,     19, 9 ), 0, 0, 11, 5453), //vldrepl_d
  INSTL(vldrepl_h, SimdLdst, (0b00110000010,       21, 11), 0, 0, 9, 5463), //vldrepl_h
  INSTL(vldrepl_w, SimdLdst, (0b001100000010,      20, 10), 0, 0, 10, 5473), //vldrepl_w
  INSTL(vldx,      SimdLdst, (0b00111000010000000, 15, 0 ), 0, 0, 7, 5483), //vldx
  INSTL(vmadd_b, lsxVVV, (0b01110000101010000, 0, 5, 10, 0), 0, 0, 180, 5488), //vmadd_b
  INSTL(vmadd_d, lsxVVV, (0b01110000101010011, 0, 5, 10, 0), 0, 0, 183, 5496), //vmadd_d
  INSTL(vmadd_h, lsxVVV, (0b01110000101010001, 0, 5, 10, 0), 0, 0, 181, 5504), //vmadd_h
  INSTL(vmadd_w, lsxVVV, (0b01110000101010010, 0, 5, 10, 0), 0, 0, 182, 5512), //vmadd_w
  INSTL(vmaddwev_d_w, lsxVVV, (0b01110000101011010, 0, 5, 10, 0), 0, 0, 190, 5520), //vmaddwev_d_w
  INSTL(vmaddwev_d_wu, lsxVVV, (0b01110000101101010, 0, 5, 10, 0), 0, 0, 198, 5533), //vmaddwev_d_wu
  INSTL(vmaddwev_d_wu_w, lsxVVV, (0b01110000101111010, 0, 5, 10, 0), 0, 0, 206, 5547), //vmaddwev_d_wu_w
  INSTL(vmaddwev_h_b, lsxVVV, (0b01110000101011000, 0, 5, 10, 0), 0, 0, 188, 5563), //vmaddwev_h_b
  INSTL(vmaddwev_h_bu, lsxVVV, (0b01110000101101000, 0, 5, 10, 0), 0, 0, 196, 5576), //vmaddwev_h_bu
  INSTL(vmaddwev_h_bu_b, lsxVVV, (0b01110000101111000, 0, 5, 10, 0), 0, 0, 204, 5590), //vmaddwev_h_bu_b
  INSTL(vmaddwev_q_d, lsxVVV, (0b01110000101011011, 0, 5, 10, 0), 0, 0, 191, 5606), //vmaddwev_q_d
  INSTL(vmaddwev_q_du, lsxVVV, (0b01110000101101011, 0, 5, 10, 0), 0, 0, 199, 5619), //vmaddwev_q_du
  INSTL(vmaddwev_q_du_d, lsxVVV, (0b01110000101111011, 0, 5, 10, 0), 0, 0, 207, 5633), //vmaddwev_q_du_d
  INSTL(vmaddwev_w_h, lsxVVV, (0b01110000101011001, 0, 5, 10, 0), 0, 0, 189, 5649), //vmaddwev_w_h
  INSTL(vmaddwev_w_hu, lsxVVV, (0b01110000101101001, 0, 5, 10, 0), 0, 0, 197, 5662), //vmaddwev_w_hu
  INSTL(vmaddwev_w_hu_h, lsxVVV, (0b01110000101111001, 0, 5, 10, 0), 0, 0, 205, 5676), //vmaddwev_w_hu_h
  INSTL(vmaddwod_d_w, lsxVVV, (0b01110000101011110, 0, 5, 10, 0), 0, 0, 194, 5692), //vmaddwod_d_w
  INSTL(vmaddwod_d_wu, lsxVVV, (0b01110000101101110, 0, 5, 10, 0), 0, 0, 202, 5705), //vmaddwod_d_wu
  INSTL(vmaddwod_d_wu_w, lsxVVV, (0b01110000101111110, 0, 5, 10, 0), 0, 0, 210, 5719), //vmaddwod_d_wu_w
  INSTL(vmaddwod_h_b, lsxVVV, (0b01110000101011100, 0, 5, 10, 0), 0, 0, 192, 5735), //vmaddwod_h_b
  INSTL(vmaddwod_h_bu, lsxVVV, (0b01110000101101100, 0, 5, 10, 0), 0, 0, 200, 5748), //vmaddwod_h_bu
  INSTL(vmaddwod_h_bu_b, lsxVVV, (0b01110000101111100, 0, 5, 10, 0), 0, 0, 208, 5762), //vmaddwod_h_bu_b
  INSTL(vmaddwod_q_d, lsxVVV, (0b01110000101011111, 0, 5, 10, 0), 0, 0, 195, 5778), //vmaddwod_q_d
  INSTL(vmaddwod_q_du, lsxVVV, (0b01110000101101111, 0, 5, 10, 0), 0, 0, 203, 5791), //vmaddwod_q_du
  INSTL(vmaddwod_q_du_d, lsxVVV, (0b01110000101111111, 0, 5, 10, 0), 0, 0, 211, 5805), //vmaddwod_q_du_d
  INSTL(vmaddwod_w_h, lsxVVV, (0b01110000101011101, 0, 5, 10, 0), 0, 0, 193, 5821), //vmaddwod_w_h
  INSTL(vmaddwod_w_hu, lsxVVV, (0b01110000101101101, 0, 5, 10, 0), 0, 0, 201, 5834), //vmaddwod_w_hu
  INSTL(vmaddwod_w_hu_h, lsxVVV, (0b01110000101111101, 0, 5, 10, 0), 0, 0, 209, 5848), //vmaddwod_w_hu_h
  INSTL(vmax_b, lsxVVV, (0b01110000011100000, 0, 5, 10, 0), 0, 0, 128, 5864), //vmax_b
  INSTL(vmax_bu, lsxVVV, (0b01110000011101000, 0, 5, 10, 0), 0, 0, 136, 5871), //vmax_bu
  INSTL(vmax_d, lsxVVV, (0b01110000011100011, 0, 5, 10, 0), 0, 0, 131, 5879), //vmax_d
  INSTL(vmax_du, lsxVVV, (0b01110000011101011, 0, 5, 10, 0), 0, 0, 139, 5886), //vmax_du
  INSTL(vmax_h, lsxVVV, (0b01110000011100001, 0, 5, 10, 0), 0, 0, 129, 5894), //vmax_h
  INSTL(vmax_hu, lsxVVV, (0b01110000011101001, 0, 5, 10, 0), 0, 0, 137, 5901), //vmax_hu
  INSTL(vmax_w, lsxVVV, (0b01110000011100010, 0, 5, 10, 0), 0, 0, 130, 5909), //vmax_w
  INSTL(vmax_wu, lsxVVV, (0b01110000011101010, 0, 5, 10, 0), 0, 0, 138, 5916), //vmax_wu
  INSTL(vmaxi_b, lsxVVI, (0b01110010100100000, 0, 5, 10, 0), 0, 0, 30, 5924), //vmaxi_b
  INSTL(vmaxi_bu, lsxVVI, (0b01110010100101000, 0, 5, 10, 0), 0, 0, 38, 5932), //vmaxi_bu
  INSTL(vmaxi_d, lsxVVI, (0b01110010100100011, 0, 5, 10, 0), 0, 0, 33, 5941), //vmaxi_d
  INSTL(vmaxi_du, lsxVVI, (0b01110010100101011, 0, 5, 10, 0), 0, 0, 41, 5949), //vmaxi_du
  INSTL(vmaxi_h, lsxVVI, (0b01110010100100001, 0, 5, 10, 0), 0, 0, 31, 5958), //vmaxi_h
  INSTL(vmaxi_hu, lsxVVI, (0b01110010100101001, 0, 5, 10, 0), 0, 0, 39, 5966), //vmaxi_hu
  INSTL(vmaxi_w, lsxVVI, (0b01110010100100010, 0, 5, 10, 0), 0, 0, 32, 5975), //vmaxi_w
  INSTL(vmaxi_wu, lsxVVI, (0b01110010100101010, 0, 5, 10, 0), 0, 0, 40, 5983), //vmaxi_wu
  INSTL(vmepatmsk_v, lsxVII, (0b01110010100110111, 0, 5, 10, 0), 0, 0, 0, 5992), //vmepatmsk_v
  INSTL(vmin_b, lsxVVV, (0b01110000011100100, 0, 5, 10, 0), 0, 0, 132, 6004), //vmin_b
  INSTL(vmin_bu, lsxVVV, (0b01110000011101100, 0, 5, 10, 0), 0, 0, 140, 6011), //vmin_bu
  INSTL(vmin_d, lsxVVV, (0b01110000011100111, 0, 5, 10, 0), 0, 0, 135, 6019), //vmin_d
  INSTL(vmin_du, lsxVVV, (0b01110000011101111, 0, 5, 10, 0), 0, 0, 143, 6026), //vmin_du
  INSTL(vmin_h, lsxVVV, (0b01110000011100101, 0, 5, 10, 0), 0, 0, 133, 6034), //vmin_h
  INSTL(vmin_hu, lsxVVV, (0b01110000011101101, 0, 5, 10, 0), 0, 0, 141, 6041), //vmin_hu
  INSTL(vmin_w, lsxVVV, (0b01110000011100110, 0, 5, 10, 0), 0, 0, 134, 6049), //vmin_w
  INSTL(vmin_wu, lsxVVV, (0b01110000011101110, 0, 5, 10, 0), 0, 0, 142, 6056), //vmin_wu
  INSTL(vmini_b, lsxVVI, (0b01110010100100100, 0, 5, 10, 0), 0, 0, 34, 6064), //vmini_b
  INSTL(vmini_bu, lsxVVI, (0b01110010100101100, 0, 5, 10, 0), 0, 0, 42, 6072), //vmini_bu
  INSTL(vmini_d, lsxVVI, (0b01110010100100111, 0, 5, 10, 0), 0, 0, 37, 6081), //vmini_d
  INSTL(vmini_du, lsxVVI, (0b01110010100101111, 0, 5, 10, 0), 0, 0, 45, 6089), //vmini_du
  INSTL(vmini_h, lsxVVI, (0b01110010100100101, 0, 5, 10, 0), 0, 0, 35, 6098), //vmini_h
  INSTL(vmini_hu, lsxVVI, (0b01110010100101101, 0, 5, 10, 0), 0, 0, 43, 6106), //vmini_hu
  INSTL(vmini_w, lsxVVI, (0b01110010100100110, 0, 5, 10, 0), 0, 0, 36, 6115), //vmini_w
  INSTL(vmini_wu, lsxVVI, (0b01110010100101110, 0, 5, 10, 0), 0, 0, 44, 6123), //vmini_wu
  INSTL(vmod_b, lsxVVV, (0b01110000111000100, 0, 5, 10, 0), 0, 0, 216, 6132), //vmod_b
  INSTL(vmod_bu, lsxVVV, (0b01110000111001100, 0, 5, 10, 0), 0, 0, 224, 6139), //vmod_bu
  INSTL(vmod_d, lsxVVV, (0b01110000111000111, 0, 5, 10, 0), 0, 0, 219, 6147), //vmod_d
  INSTL(vmod_du, lsxVVV, (0b01110000111001111, 0, 5, 10, 0), 0, 0, 227, 6154), //vmod_du
  INSTL(vmod_h, lsxVVV, (0b01110000111000101, 0, 5, 10, 0), 0, 0, 217, 6162), //vmod_h
  INSTL(vmod_hu, lsxVVV, (0b01110000111001101, 0, 5, 10, 0), 0, 0, 225, 6169), //vmod_hu
  INSTL(vmod_w, lsxVVV, (0b01110000111000110, 0, 5, 10, 0), 0, 0, 218, 6177), //vmod_w
  INSTL(vmod_wu, lsxVVV, (0b01110000111001110, 0, 5, 10, 0), 0, 0, 226, 6184), //vmod_wu
  INSTL(vmskgez_b, lsxVV, (0b0111001010011100010100, 0, 5, 0), 0, 0, 20, 6192), //vmskgez_b
  INSTL(vmskltz_b, lsxVV, (0b0111001010011100010000, 0, 5, 0), 0, 0, 16, 6202), //vmskltz_b
  INSTL(vmskltz_d, lsxVV, (0b0111001010011100010011, 0, 5, 0), 0, 0, 19, 6212), //vmskltz_d
  INSTL(vmskltz_h, lsxVV, (0b0111001010011100010001, 0, 5, 0), 0, 0, 17, 6222), //vmskltz_h
  INSTL(vmskltz_w, lsxVV, (0b0111001010011100010010, 0, 5, 0), 0, 0, 18, 6232), //vmskltz_w
  INSTL(vmsknz_b, lsxVV, (0b0111001010011100011000, 0, 5, 0), 0, 0, 21, 6242), //vmsknz_b
  INSTL(vmsub_b, lsxVVV, (0b01110000101010100, 0, 5, 10, 0), 0, 0, 184, 6251), //vmsub_b
  INSTL(vmsub_d, lsxVVV, (0b01110000101010111, 0, 5, 10, 0), 0, 0, 187, 6259), //vmsub_d
  INSTL(vmsub_h, lsxVVV, (0b01110000101010101, 0, 5, 10, 0), 0, 0, 185, 6267), //vmsub_h
  INSTL(vmsub_w, lsxVVV, (0b01110000101010110, 0, 5, 10, 0), 0, 0, 186, 6275), //vmsub_w
  INSTL(vmuh_b, lsxVVV, (0b01110000100001100, 0, 5, 10, 0), 0, 0, 148, 6283), //vmuh_b
  INSTL(vmuh_bu, lsxVVV, (0b01110000100010000, 0, 5, 10, 0), 0, 0, 152, 6290), //vmuh_bu
  INSTL(vmuh_d, lsxVVV, (0b01110000100001111, 0, 5, 10, 0), 0, 0, 151, 6298), //vmuh_d
  INSTL(vmuh_du, lsxVVV, (0b01110000100010011, 0, 5, 10, 0), 0, 0, 155, 6305), //vmuh_du
  INSTL(vmuh_h, lsxVVV, (0b01110000100001101, 0, 5, 10, 0), 0, 0, 149, 6313), //vmuh_h
  INSTL(vmuh_hu, lsxVVV, (0b01110000100010001, 0, 5, 10, 0), 0, 0, 153, 6320), //vmuh_hu
  INSTL(vmuh_w, lsxVVV, (0b01110000100001110, 0, 5, 10, 0), 0, 0, 150, 6328), //vmuh_w
  INSTL(vmuh_wu, lsxVVV, (0b01110000100010010, 0, 5, 10, 0), 0, 0, 154, 6335), //vmuh_wu
  INSTL(vmul_b, lsxVVV, (0b01110000100001000, 0, 5, 10, 0), 0, 0, 144, 6343), //vmul_b
  INSTL(vmul_d, lsxVVV, (0b01110000100001011, 0, 5, 10, 0), 0, 0, 147, 6350), //vmul_d
  INSTL(vmul_h, lsxVVV, (0b01110000100001001, 0, 5, 10, 0), 0, 0, 145, 6357), //vmul_h
  INSTL(vmul_w, lsxVVV, (0b01110000100001010, 0, 5, 10, 0), 0, 0, 146, 6364), //vmul_w
  INSTL(vmulwev_d_w, lsxVVV, (0b01110000100100010, 0, 5, 10, 0), 0, 0, 158, 6371), //vmulwev_d_w
  INSTL(vmulwev_d_wu, lsxVVV, (0b01110000100110010, 0, 5, 10, 0), 0, 0, 166, 6383), //vmulwev_d_wu
  INSTL(vmulwev_d_wu_w, lsxVVV, (0b01110000101000010, 0, 5, 10, 0), 0, 0, 174, 6396), //vmulwev_d_wu_w
  INSTL(vmulwev_h_b, lsxVVV, (0b01110000100100000, 0, 5, 10, 0), 0, 0, 156, 6411), //vmulwev_h_b
  INSTL(vmulwev_h_bu, lsxVVV, (0b01110000100110000, 0, 5, 10, 0), 0, 0, 164, 6423), //vmulwev_h_bu
  INSTL(vmulwev_h_bu_b, lsxVVV, (0b01110000101000000, 0, 5, 10, 0), 0, 0, 172, 6436), //vmulwev_h_bu_b
  INSTL(vmulwev_q_d, lsxVVV, (0b01110000100100011, 0, 5, 10, 0), 0, 0, 159, 6451), //vmulwev_q_d
  INSTL(vmulwev_q_du, lsxVVV, (0b01110000100110011, 0, 5, 10, 0), 0, 0, 167, 6463), //vmulwev_q_du
  INSTL(vmulwev_q_du_d, lsxVVV, (0b01110000101000011, 0, 5, 10, 0), 0, 0, 175, 6476), //vmulwev_q_du_d
  INSTL(vmulwev_w_h, lsxVVV, (0b01110000100100001, 0, 5, 10, 0), 0, 0, 157, 6491), //vmulwev_w_h
  INSTL(vmulwev_w_hu, lsxVVV, (0b01110000100110001, 0, 5, 10, 0), 0, 0, 165, 6503), //vmulwev_w_hu
  INSTL(vmulwev_w_hu_h, lsxVVV, (0b01110000101000001, 0, 5, 10, 0), 0, 0, 173, 6516), //vmulwev_w_hu_h
  INSTL(vmulwod_d_w, lsxVVV, (0b01110000100100110, 0, 5, 10, 0), 0, 0, 162, 6531), //vmulwod_d_w
  INSTL(vmulwod_d_wu, lsxVVV, (0b01110000100110110, 0, 5, 10, 0), 0, 0, 170, 6543), //vmulwod_d_wu
  INSTL(vmulwod_d_wu_w, lsxVVV, (0b01110000101000110, 0, 5, 10, 0), 0, 0, 178, 6556), //vmulwod_d_wu_w
  INSTL(vmulwod_h_b, lsxVVV, (0b01110000100100100, 0, 5, 10, 0), 0, 0, 160, 6571), //vmulwod_h_b
  INSTL(vmulwod_h_bu, lsxVVV, (0b01110000100110100, 0, 5, 10, 0), 0, 0, 168, 6583), //vmulwod_h_bu
  INSTL(vmulwod_h_bu_b, lsxVVV, (0b01110000101000100, 0, 5, 10, 0), 0, 0, 176, 6596), //vmulwod_h_bu_b
  INSTL(vmulwod_q_d, lsxVVV, (0b01110000100100111, 0, 5, 10, 0), 0, 0, 163, 6611), //vmulwod_q_d
  INSTL(vmulwod_q_du, lsxVVV, (0b01110000100110111, 0, 5, 10, 0), 0, 0, 171, 6623), //vmulwod_q_du
  INSTL(vmulwod_q_du_d, lsxVVV, (0b01110000101000111, 0, 5, 10, 0), 0, 0, 179, 6636), //vmulwod_q_du_d
  INSTL(vmulwod_w_h, lsxVVV, (0b01110000100100101, 0, 5, 10, 0), 0, 0, 161, 6651), //vmulwod_w_h
  INSTL(vmulwod_w_hu, lsxVVV, (0b01110000100110101, 0, 5, 10, 0), 0, 0, 169, 6663), //vmulwod_w_hu
  INSTL(vmulwod_w_hu_h, lsxVVV, (0b01110000101000101, 0, 5, 10, 0), 0, 0, 177, 6676), //vmulwod_w_hu_h
  INSTL(vneg_b, lsxVV, (0b0111001010011100001100, 0, 5, 0), 0, 0, 12, 6691), //vneg_b
  INSTL(vneg_d, lsxVV, (0b0111001010011100001111, 0, 5, 0), 0, 0, 15, 6698), //vneg_d
  INSTL(vneg_h, lsxVV, (0b0111001010011100001101, 0, 5, 0), 0, 0, 13, 6705), //vneg_h
  INSTL(vneg_w, lsxVV, (0b0111001010011100001110, 0, 5, 0), 0, 0, 14, 6712), //vneg_w
  INSTL(vnor_v, lsxVVV, (0b01110001001001111, 0, 5, 10, 0), 0, 0, 327, 6719), //vnor_v
  INSTL(vnori_b, lsxVVI, (0b01110011110111, 0, 5, 10, 0), 0, 0, 158, 6726), //vnori_b
  INSTL(vor_v, lsxVVV, (0b01110001001001101, 0, 5, 10, 0), 0, 0, 325, 6734), //vor_v
  INSTL(vori_b, lsxVVI, (0b01110011110101, 0, 5, 10, 0), 0, 0, 156, 6740), //vori_b
  INSTL(vorn_v, lsxVVV, (0b01110001001010001, 0, 5, 10, 0), 0, 0, 329, 6747), //vorn_v
  INSTL(vpackev_b, lsxVVV, (0b01110001000101100, 0, 5, 10, 0), 0, 0, 300, 6754), //vpackev_b
  INSTL(vpackev_d, lsxVVV, (0b01110001000101111, 0, 5, 10, 0), 0, 0, 303, 6764), //vpackev_d
  INSTL(vpackev_h, lsxVVV, (0b01110001000101101, 0, 5, 10, 0), 0, 0, 301, 6774), //vpackev_h
  INSTL(vpackev_w, lsxVVV, (0b01110001000101110, 0, 5, 10, 0), 0, 0, 302, 6784), //vpackev_w
  INSTL(vpackod_b, lsxVVV, (0b01110001000110000, 0, 5, 10, 0), 0, 0, 304, 6794), //vpackod_b
  INSTL(vpackod_d, lsxVVV, (0b01110001000110011, 0, 5, 10, 0), 0, 0, 307, 6804), //vpackod_d
  INSTL(vpackod_h, lsxVVV, (0b01110001000110001, 0, 5, 10, 0), 0, 0, 305, 6814), //vpackod_h
  INSTL(vpackod_w, lsxVVV, (0b01110001000110010, 0, 5, 10, 0), 0, 0, 306, 6824), //vpackod_w
  INSTL(vpcnt_b, lsxVV, (0b0111001010011100001000, 0, 5, 0), 0, 0, 8, 6834), //vpcnt_b
  INSTL(vpcnt_d, lsxVV, (0b0111001010011100001011, 0, 5, 0), 0, 0, 11, 6842), //vpcnt_d
  INSTL(vpcnt_h, lsxVV, (0b0111001010011100001001, 0, 5, 0), 0, 0, 9, 6850), //vpcnt_h
  INSTL(vpcnt_w, lsxVV, (0b0111001010011100001010, 0, 5, 0), 0, 0, 10, 6858), //vpcnt_w
  INSTL(vpermi_w, lsxVVI, (0b01110011111001, 0, 5, 10, 0), 0, 0, 159, 6866), //vpermi_w
  INSTL(vpickev_b, lsxVVV, (0b01110001000111100, 0, 5, 10, 0), 0, 0, 316, 6875), //vpickev_b
  INSTL(vpickev_d, lsxVVV, (0b01110001000111111, 0, 5, 10, 0), 0, 0, 319, 6885), //vpickev_d
  INSTL(vpickev_h, lsxVVV, (0b01110001000111101, 0, 5, 10, 0), 0, 0, 317, 6895), //vpickev_h
  INSTL(vpickev_w, lsxVVV, (0b01110001000111110, 0, 5, 10, 0), 0, 0, 318, 6905), //vpickev_w
  INSTL(vpickod_b, lsxVVV, (0b01110001001000000, 0, 5, 10, 0), 0, 0, 320, 6915), //vpickod_b
  INSTL(vpickod_d, lsxVVV, (0b01110001001000011, 0, 5, 10, 0), 0, 0, 323, 6925), //vpickod_d
  INSTL(vpickod_h, lsxVVV, (0b01110001001000001, 0, 5, 10, 0), 0, 0, 321, 6935), //vpickod_h
  INSTL(vpickod_w, lsxVVV, (0b01110001001000010, 0, 5, 10, 0), 0, 0, 322, 6945), //vpickod_w
  INSTL(vpickve2gr_b, lsxRVI, (0b011100101110111110, kWX, 0, 5, 10, 0), 0, 0, 0, 6955), //vpickve2gr_b
  INSTL(vpickve2gr_bu, lsxRVI, (0b011100101111001110, kWX, 0, 5, 10, 0), 0, 0, 4, 6968), //vpickve2gr_bu
  INSTL(vpickve2gr_d, lsxRVI, (0b011100101110111111110, kWX, 0, 5, 10, 0), 0, 0, 3, 6982), //vpickve2gr_d
  INSTL(vpickve2gr_du, lsxRVI, (0b011100101111001111110, kWX, 0, 5, 10, 0), 0, 0, 7, 6995), //vpickve2gr_du
  INSTL(vpickve2gr_h, lsxRVI, (0b0111001011101111110, kWX, 0, 5, 10, 0), 0, 0, 1, 7009), //vpickve2gr_h
  INSTL(vpickve2gr_hu, lsxRVI, (0b0111001011110011110, kWX, 0, 5, 10, 0), 0, 0, 5, 7022), //vpickve2gr_hu
  INSTL(vpickve2gr_w, lsxRVI, (0b01110010111011111110, kWX, 0, 5, 10, 0), 0, 0, 2, 7036), //vpickve2gr_w
  INSTL(vpickve2gr_wu, lsxRVI, (0b01110010111100111110, kWX, 0, 5, 10, 0), 0, 0, 6, 7049), //vpickve2gr_wu
  INSTL(vreplgr2vr_b, lsxVR, (0b0111001010011111000000, 0, 5, 0), 0, 0, 0, 7063), //vreplgr2vr_b
  INSTL(vreplgr2vr_d, lsxVR, (0b0111001010011111000011, 0, 5, 0), 0, 0, 3, 7076), //vreplgr2vr_d
  INSTL(vreplgr2vr_h, lsxVR, (0b0111001010011111000001, 0, 5, 0), 0, 0, 1, 7089), //vreplgr2vr_h
  INSTL(vreplgr2vr_w, lsxVR, (0b0111001010011111000010, 0, 5, 0), 0, 0, 2, 7102), //vreplgr2vr_w
  INSTL(vreplve_b, lsxVVR, (0b01110001001000100, 0, 5, kWX, 10, 0), 0, 0, 0, 7115), //vreplve_b
  INSTL(vreplve_d, lsxVVR, (0b01110001001000111, 0, 5, kWX, 10, 0), 0, 0, 3, 7125), //vreplve_d
  INSTL(vreplve_h, lsxVVR, (0b01110001001000101, 0, 5, kWX, 10, 0), 0, 0, 1, 7135), //vreplve_h
  INSTL(vreplve_w, lsxVVR, (0b01110001001000110, 0, 5, kWX, 10, 0), 0, 0, 2, 7145), //vreplve_w
  INSTL(vreplvei_b, lsxVVI, (0b011100101111011110, 0, 5, 10, 0), 0, 0, 60, 7155), //vreplvei_b
  INSTL(vreplvei_d, lsxVVI, (0b011100101111011111110, 0, 5, 10, 0), 0, 0, 63, 7166), //vreplvei_d
  INSTL(vreplvei_h, lsxVVI, (0b0111001011110111110, 0, 5, 10, 0), 0, 0, 61, 7177), //vreplvei_h
  INSTL(vreplvei_w, lsxVVI, (0b01110010111101111110, 0, 5, 10, 0), 0, 0, 62, 7188), //vreplvei_w
  INSTL(vrotr_b, lsxVVV, (0b01110000111011100, 0, 5, 10, 0), 0, 0, 240, 7199), //vrotr_b
  INSTL(vrotr_d, lsxVVV, (0b01110000111011111, 0, 5, 10, 0), 0, 0, 243, 7207), //vrotr_d
  INSTL(vrotr_h, lsxVVV, (0b01110000111011101, 0, 5, 10, 0), 0, 0, 241, 7215), //vrotr_h
  INSTL(vrotr_w, lsxVVV, (0b01110000111011110, 0, 5, 10, 0), 0, 0, 242, 7223), //vrotr_w
  INSTL(vrotri_b, lsxVVI, (0b0111001010100000001, 0, 5, 10, 0), 0, 0, 48, 7231), //vrotri_b
  INSTL(vrotri_d, lsxVVI, (0b0111001010100001, 0, 5, 10, 0), 0, 0, 51, 7240), //vrotri_d
  INSTL(vrotri_h, lsxVVI, (0b011100101010000001, 0, 5, 10, 0), 0, 0, 49, 7249), //vrotri_h
  INSTL(vrotri_w, lsxVVI, (0b01110010101000001, 0, 5, 10, 0), 0, 0, 50, 7258), //vrotri_w
  INSTL(vsadd_b, lsxVVV, (0b01110000010001100, 0, 5, 10, 0), 0, 0, 68, 7267), //vsadd_b
  INSTL(vsadd_bu, lsxVVV, (0b01110000010010100, 0, 5, 10, 0), 0, 0, 76, 7275), //vsadd_bu
  INSTL(vsadd_d, lsxVVV, (0b01110000010001111, 0, 5, 10, 0), 0, 0, 71, 7284), //vsadd_d
  INSTL(vsadd_du, lsxVVV, (0b01110000010010111, 0, 5, 10, 0), 0, 0, 79, 7292), //vsadd_du
  INSTL(vsadd_h, lsxVVV, (0b01110000010001101, 0, 5, 10, 0), 0, 0, 69, 7301), //vsadd_h
  INSTL(vsadd_hu, lsxVVV, (0b01110000010010101, 0, 5, 10, 0), 0, 0, 77, 7309), //vsadd_hu
  INSTL(vsadd_w, lsxVVV, (0b01110000010001110, 0, 5, 10, 0), 0, 0, 70, 7318), //vsadd_w
  INSTL(vsadd_wu, lsxVVV, (0b01110000010010110, 0, 5, 10, 0), 0, 0, 78, 7326), //vsadd_wu
  INSTL(vsat_b, lsxVVI, (0b0111001100100100001, 0, 5, 10, 0), 0, 0, 82, 7335), //vsat_b
  INSTL(vsat_bu, lsxVVI, (0b0111001100101000001, 0, 5, 10, 0), 0, 0, 86, 7342), //vsat_bu
  INSTL(vsat_d, lsxVVI, (0b0111001100100101, 0, 5, 10, 0), 0, 0, 85, 7350), //vsat_d
  INSTL(vsat_du, lsxVVI, (0b0111001100101001, 0, 5, 10, 0), 0, 0, 89, 7357), //vsat_du
  INSTL(vsat_h, lsxVVI, (0b011100110010010001, 0, 5, 10, 0), 0, 0, 83, 7365), //vsat_h
  INSTL(vsat_hu, lsxVVI, (0b011100110010100001, 0, 5, 10, 0), 0, 0, 87, 7372), //vsat_hu
  INSTL(vsat_w, lsxVVI, (0b01110011001001001, 0, 5, 10, 0), 0, 0, 84, 7380), //vsat_w
  INSTL(vsat_wu, lsxVVI, (0b01110011001010001, 0, 5, 10, 0), 0, 0, 88, 7387), //vsat_wu
  INSTL(vseq_b, lsxVVV, (0b01110000000000000, 0, 5, 10, 0), 0, 0, 0, 7395), //vseq_b
  INSTL(vseq_d, lsxVVV, (0b01110000000000011, 0, 5, 10, 0), 0, 0, 3, 7402), //vseq_d
  INSTL(vseq_h, lsxVVV, (0b01110000000000001, 0, 5, 10, 0), 0, 0, 1, 7409), //vseq_h
  INSTL(vseq_w, lsxVVV, (0b01110000000000010, 0, 5, 10, 0), 0, 0, 2, 7416), //vseq_w
  INSTL(vseqi_b, lsxVVI, (0b01110010100000000, 0, 5, 10, 0), 0, 0, 0, 7423), //vseqi_b
  INSTL(vseqi_d, lsxVVI, (0b01110010100000011, 0, 5, 10, 0), 0, 0, 3, 7431), //vseqi_d
  INSTL(vseqi_h, lsxVVI, (0b01110010100000001, 0, 5, 10, 0), 0, 0, 1, 7439), //vseqi_h
  INSTL(vseqi_w, lsxVVI, (0b01110010100000010, 0, 5, 10, 0), 0, 0, 2, 7447), //vseqi_w
  INSTL(vsetallnez_b, lsxIV, (0b0111001010011100101100, 0, 5, 0), 0, 0, 6, 7455), //vsetallnez_b
  INSTL(vsetallnez_d, lsxIV, (0b0111001010011100101111, 0, 5, 0), 0, 0, 9, 7468), //vsetallnez_d
  INSTL(vsetallnez_h, lsxIV, (0b0111001010011100101101, 0, 5, 0), 0, 0, 7, 7481), //vsetallnez_h
  INSTL(vsetallnez_w, lsxIV, (0b0111001010011100101110, 0, 5, 0), 0, 0, 8, 7494), //vsetallnez_w
  INSTL(vsetanyeqz_b, lsxIV, (0b0111001010011100101000, 0, 5, 0), 0, 0, 2, 7507), //vsetanyeqz_b
  INSTL(vsetanyeqz_d, lsxIV, (0b0111001010011100101011, 0, 5, 0), 0, 0, 5, 7520), //vsetanyeqz_d
  INSTL(vsetanyeqz_h, lsxIV, (0b0111001010011100101001, 0, 5, 0), 0, 0, 3, 7533), //vsetanyeqz_h
  INSTL(vsetanyeqz_w, lsxIV, (0b0111001010011100101010, 0, 5, 0), 0, 0, 4, 7546), //vsetanyeqz_w
  INSTL(vseteqz_v, lsxIV, (0b0111001010011100100110, 0, 5, 0), 0, 0, 0, 7559), //vseteqz_v
  INSTL(vsetnez_v, lsxIV, (0b0111001010011100100111, 0, 5, 0), 0, 0, 1, 7569), //vsetnez_v
  INSTL(vshuf4i_b, lsxVVI, (0b01110011100100, 0, 5, 10, 0), 0, 0, 150, 7579), //vshuf4i_b
  INSTL(vshuf4i_d, lsxVVI, (0b01110011100111, 0, 5, 10, 0), 0, 0, 153, 7589), //vshuf4i_d
  INSTL(vshuf4i_h, lsxVVI, (0b01110011100101, 0, 5, 10, 0), 0, 0, 151, 7599), //vshuf4i_h
  INSTL(vshuf4i_w, lsxVVI, (0b01110011100110, 0, 5, 10, 0), 0, 0, 152, 7609), //vshuf4i_w
  INSTL(vshuf_b, lsxVVVV, (0b000011010101, 0, 5, 10, 15, 0), 0, 0, 9, 7619), //vshuf_b
  INSTL(vshuf_d, lsxVVV, (0b01110001011110111, 0, 5, 10, 0), 0, 0, 366, 7627), //vshuf_d
  INSTL(vshuf_h, lsxVVV, (0b01110001011110101, 0, 5, 10, 0), 0, 0, 364, 7635), //vshuf_h
  INSTL(vshuf_w, lsxVVV, (0b01110001011110110, 0, 5, 10, 0), 0, 0, 365, 7643), //vshuf_w
  INSTL(vsigncov_b, lsxVVV, (0b01110001001011100, 0, 5, 10, 0), 0, 0, 334, 7651), //vsigncov_b
  INSTL(vsigncov_d, lsxVVV, (0b01110001001011111, 0, 5, 10, 0), 0, 0, 337, 7662), //vsigncov_d
  INSTL(vsigncov_h, lsxVVV, (0b01110001001011101, 0, 5, 10, 0), 0, 0, 335, 7673), //vsigncov_h
  INSTL(vsigncov_w, lsxVVV, (0b01110001001011110, 0, 5, 10, 0), 0, 0, 336, 7684), //vsigncov_w
  INSTL(vsle_b, lsxVVV, (0b01110000000000100, 0, 5, 10, 0), 0, 0, 4, 7695), //vsle_b
  INSTL(vsle_bu, lsxVVV, (0b01110000000001000, 0, 5, 10, 0), 0, 0, 8, 7702), //vsle_bu
  INSTL(vsle_d, lsxVVV, (0b01110000000000111, 0, 5, 10, 0), 0, 0, 7, 7710), //vsle_d
  INSTL(vsle_du, lsxVVV, (0b01110000000001011, 0, 5, 10, 0), 0, 0, 11, 7717), //vsle_du
  INSTL(vsle_h, lsxVVV, (0b01110000000000101, 0, 5, 10, 0), 0, 0, 5, 7725), //vsle_h
  INSTL(vsle_hu, lsxVVV, (0b01110000000001001, 0, 5, 10, 0), 0, 0, 9, 7732), //vsle_hu
  INSTL(vsle_w, lsxVVV, (0b01110000000000110, 0, 5, 10, 0), 0, 0, 6, 7740), //vsle_w
  INSTL(vsle_wu, lsxVVV, (0b01110000000001010, 0, 5, 10, 0), 0, 0, 10, 7747), //vsle_wu
  INSTL(vslei_b, lsxVVI, (0b01110010100000100, 0, 5, 10, 0), 0, 0, 4, 7755), //vslei_b
  INSTL(vslei_bu, lsxVVI, (0b01110010100001000, 0, 5, 10, 0), 0, 0, 8, 7763), //vslei_bu
  INSTL(vslei_d, lsxVVI, (0b01110010100000111, 0, 5, 10, 0), 0, 0, 7, 7772), //vslei_d
  INSTL(vslei_du, lsxVVI, (0b01110010100001011, 0, 5, 10, 0), 0, 0, 11, 7780), //vslei_du
  INSTL(vslei_h, lsxVVI, (0b01110010100000101, 0, 5, 10, 0), 0, 0, 5, 7789), //vslei_h
  INSTL(vslei_hu, lsxVVI, (0b01110010100001001, 0, 5, 10, 0), 0, 0, 9, 7797), //vslei_hu
  INSTL(vslei_w, lsxVVI, (0b01110010100000110, 0, 5, 10, 0), 0, 0, 6, 7806), //vslei_w
  INSTL(vslei_wu, lsxVVI, (0b01110010100001010, 0, 5, 10, 0), 0, 0, 10, 7814), //vslei_wu
  INSTL(vsll_b, lsxVVV, (0b01110000111010000, 0, 5, 10, 0), 0, 0, 228, 7823), //vsll_b
  INSTL(vsll_d, lsxVVV, (0b01110000111010011, 0, 5, 10, 0), 0, 0, 231, 7830), //vsll_d
  INSTL(vsll_h, lsxVVV, (0b01110000111010001, 0, 5, 10, 0), 0, 0, 229, 7837), //vsll_h
  INSTL(vsll_w, lsxVVV, (0b01110000111010010, 0, 5, 10, 0), 0, 0, 230, 7844), //vsll_w
  INSTL(vslli_b, lsxVVI, (0b0111001100101100001, 0, 5, 10, 0), 0, 0, 90, 7851), //vslli_b
  INSTL(vslli_d, lsxVVI, (0b0111001100101101, 0, 5, 10, 0), 0, 0, 93, 7859), //vslli_d
  INSTL(vslli_h, lsxVVI, (0b011100110010110001, 0, 5, 10, 0), 0, 0, 91, 7867), //vslli_h
  INSTL(vslli_w, lsxVVI, (0b01110011001011001, 0, 5, 10, 0), 0, 0, 92, 7875), //vslli_w
  INSTL(vsllwil_d_w, lsxVVI, (0b01110011000010001, 0, 5, 10, 0), 0, 0, 66, 7883), //vsllwil_d_w
  INSTL(vsllwil_du_wu, lsxVVI, (0b01110011000011001, 0, 5, 10, 0), 0, 0, 69, 7895), //vsllwil_du_wu
  INSTL(vsllwil_h_b, lsxVVI, (0b0111001100001000001, 0, 5, 10, 0), 0, 0, 64, 7909), //vsllwil_h_b
  INSTL(vsllwil_hu_bu, lsxVVI, (0b0111001100001100001, 0, 5, 10, 0), 0, 0, 67, 7921), //vsllwil_hu_bu
  INSTL(vsllwil_w_h, lsxVVI, (0b011100110000100001, 0, 5, 10, 0), 0, 0, 65, 7935), //vsllwil_w_h
  INSTL(vsllwil_wu_hu, lsxVVI, (0b011100110000110001, 0, 5, 10, 0), 0, 0, 68, 7947), //vsllwil_wu_hu
  INSTL(vslt_b, lsxVVV, (0b01110000000001100, 0, 5, 10, 0), 0, 0, 12, 7961), //vslt_b
  INSTL(vslt_bu, lsxVVV, (0b01110000000010000, 0, 5, 10, 0), 0, 0, 16, 7968), //vslt_bu
  INSTL(vslt_d, lsxVVV, (0b01110000000001111, 0, 5, 10, 0), 0, 0, 15, 7976), //vslt_d
  INSTL(vslt_du, lsxVVV, (0b01110000000010011, 0, 5, 10, 0), 0, 0, 19, 7983), //vslt_du
  INSTL(vslt_h, lsxVVV, (0b01110000000001101, 0, 5, 10, 0), 0, 0, 13, 7991), //vslt_h
  INSTL(vslt_hu, lsxVVV, (0b01110000000010001, 0, 5, 10, 0), 0, 0, 17, 7998), //vslt_hu
  INSTL(vslt_w, lsxVVV, (0b01110000000001110, 0, 5, 10, 0), 0, 0, 14, 8006), //vslt_w
  INSTL(vslt_wu, lsxVVV, (0b01110000000010010, 0, 5, 10, 0), 0, 0, 18, 8013), //vslt_wu
  INSTL(vslti_b, lsxVVI, (0b01110010100001100, 0, 5, 10, 0), 0, 0, 12, 8021), //vslti_b
  INSTL(vslti_bu, lsxVVI, (0b01110010100010000, 0, 5, 10, 0), 0, 0, 16, 8029), //vslti_bu
  INSTL(vslti_d, lsxVVI, (0b01110010100001111, 0, 5, 10, 0), 0, 0, 15, 8038), //vslti_d
  INSTL(vslti_du, lsxVVI, (0b01110010100010011, 0, 5, 10, 0), 0, 0, 19, 8046), //vslti_du
  INSTL(vslti_h, lsxVVI, (0b01110010100001101, 0, 5, 10, 0), 0, 0, 13, 8055), //vslti_h
  INSTL(vslti_hu, lsxVVI, (0b01110010100010001, 0, 5, 10, 0), 0, 0, 17, 8063), //vslti_hu
  INSTL(vslti_w, lsxVVI, (0b01110010100001110, 0, 5, 10, 0), 0, 0, 14, 8072), //vslti_w
  INSTL(vslti_wu, lsxVVI, (0b01110010100010010, 0, 5, 10, 0), 0, 0, 18, 8080), //vslti_wu
  INSTL(vsra_b, lsxVVV, (0b01110000111011000, 0, 5, 10, 0), 0, 0, 236, 8089), //vsra_b
  INSTL(vsra_d, lsxVVV, (0b01110000111011011, 0, 5, 10, 0), 0, 0, 239, 8096), //vsra_d
  INSTL(vsra_h, lsxVVV, (0b01110000111011001, 0, 5, 10, 0), 0, 0, 237, 8103), //vsra_h
  INSTL(vsra_w, lsxVVV, (0b01110000111011010, 0, 5, 10, 0), 0, 0, 238, 8110), //vsra_w
  INSTL(vsrai_b, lsxVVI, (0b0111001100110100001, 0, 5, 10, 0), 0, 0, 98, 8117), //vsrai_b
  INSTL(vsrai_d, lsxVVI, (0b0111001100110101, 0, 5, 10, 0), 0, 0, 101, 8125), //vsrai_d
  INSTL(vsrai_h, lsxVVI, (0b011100110011010001, 0, 5, 10, 0), 0, 0, 99, 8133), //vsrai_h
  INSTL(vsrai_w, lsxVVI, (0b01110011001101001, 0, 5, 10, 0), 0, 0, 100, 8141), //vsrai_w
  INSTL(vsran_b_h, lsxVVV, (0b01110000111101101, 0, 5, 10, 0), 0, 0, 255, 8149), //vsran_b_h
  INSTL(vsran_h_w, lsxVVV, (0b01110000111101110, 0, 5, 10, 0), 0, 0, 256, 8159), //vsran_h_w
  INSTL(vsran_w_d, lsxVVV, (0b01110000111101111, 0, 5, 10, 0), 0, 0, 257, 8169), //vsran_w_d
  INSTL(vsrani_b_h, lsxVVI, (0b011100110101100001, 0, 5, 10, 0), 0, 0, 122, 8179), //vsrani_b_h
  INSTL(vsrani_d_q, lsxVVI, (0b011100110101101, 0, 5, 10, 0), 0, 0, 125, 8190), //vsrani_d_q
  INSTL(vsrani_h_w, lsxVVI, (0b01110011010110001, 0, 5, 10, 0), 0, 0, 123, 8201), //vsrani_h_w
  INSTL(vsrani_w_d, lsxVVI, (0b0111001101011001, 0, 5, 10, 0), 0, 0, 124, 8212), //vsrani_w_d
  INSTL(vsrar_b, lsxVVV, (0b01110000111100100, 0, 5, 10, 0), 0, 0, 248, 8223), //vsrar_b
  INSTL(vsrar_d, lsxVVV, (0b01110000111100111, 0, 5, 10, 0), 0, 0, 251, 8231), //vsrar_d
  INSTL(vsrar_h, lsxVVV, (0b01110000111100101, 0, 5, 10, 0), 0, 0, 249, 8239), //vsrar_h
  INSTL(vsrar_w, lsxVVV, (0b01110000111100110, 0, 5, 10, 0), 0, 0, 250, 8247), //vsrar_w
  INSTL(vsrari_b, lsxVVI, (0b0111001010101000001, 0, 5, 10, 0), 0, 0, 56, 8255), //vsrari_b
  INSTL(vsrari_d, lsxVVI, (0b0111001010101001, 0, 5, 10, 0), 0, 0, 59, 8264), //vsrari_d
  INSTL(vsrari_h, lsxVVI, (0b011100101010100001, 0, 5, 10, 0), 0, 0, 57, 8273), //vsrari_h
  INSTL(vsrari_w, lsxVVI, (0b01110010101010001, 0, 5, 10, 0), 0, 0, 58, 8282), //vsrari_w
  INSTL(vsrarn_b_h, lsxVVV, (0b01110000111110101, 0, 5, 10, 0), 0, 0, 261, 8291), //vsrarn_b_h
  INSTL(vsrarn_h_w, lsxVVV, (0b01110000111110110, 0, 5, 10, 0), 0, 0, 262, 8302), //vsrarn_h_w
  INSTL(vsrarn_w_d, lsxVVV, (0b01110000111110111, 0, 5, 10, 0), 0, 0, 263, 8313), //vsrarn_w_d
  INSTL(vsrarni_b_h, lsxVVI, (0b011100110101110001, 0, 5, 10, 0), 0, 0, 126, 8324), //vsrarni_b_h
  INSTL(vsrarni_d_q, lsxVVI, (0b011100110101111, 0, 5, 10, 0), 0, 0, 129, 8336), //vsrarni_d_q
  INSTL(vsrarni_h_w, lsxVVI, (0b01110011010111001, 0, 5, 10, 0), 0, 0, 127, 8348), //vsrarni_h_w
  INSTL(vsrarni_w_d, lsxVVI, (0b0111001101011101, 0, 5, 10, 0), 0, 0, 128, 8360), //vsrarni_w_d
  INSTL(vsrl_b, lsxVVV, (0b01110000111010100, 0, 5, 10, 0), 0, 0, 232, 8372), //vsrl_b
  INSTL(vsrl_d, lsxVVV, (0b01110000111010111, 0, 5, 10, 0), 0, 0, 235, 8379), //vsrl_d
  INSTL(vsrl_h, lsxVVV, (0b01110000111010101, 0, 5, 10, 0), 0, 0, 233, 8386), //vsrl_h
  INSTL(vsrl_w, lsxVVV, (0b01110000111010110, 0, 5, 10, 0), 0, 0, 234, 8393), //vsrl_w
  INSTL(vsrli_b, lsxVVI, (0b0111001100110000001, 0, 5, 10, 0), 0, 0, 94, 8400), //vsrli_b
  INSTL(vsrli_d, lsxVVI, (0b0111001100110001, 0, 5, 10, 0), 0, 0, 97, 8408), //vsrli_d
  INSTL(vsrli_h, lsxVVI, (0b011100110011000001, 0, 5, 10, 0), 0, 0, 95, 8416), //vsrli_h
  INSTL(vsrli_w, lsxVVI, (0b01110011001100001, 0, 5, 10, 0), 0, 0, 96, 8424), //vsrli_w
  INSTL(vsrln_b_h, lsxVVV, (0b01110000111101001, 0, 5, 10, 0), 0, 0, 252, 8432), //vsrln_b_h
  INSTL(vsrln_h_w, lsxVVV, (0b01110000111101010, 0, 5, 10, 0), 0, 0, 253, 8442), //vsrln_h_w
  INSTL(vsrln_w_d, lsxVVV, (0b01110000111101011, 0, 5, 10, 0), 0, 0, 254, 8452), //vsrln_w_d
  INSTL(vsrlni_b_h, lsxVVI, (0b011100110100000001, 0, 5, 10, 0), 0, 0, 102, 8462), //vsrlni_b_h
  INSTL(vsrlni_d_q, lsxVVI, (0b011100110100001, 0, 5, 10, 0), 0, 0, 105, 8473), //vsrlni_d_q
  INSTL(vsrlni_h_w, lsxVVI, (0b01110011010000001, 0, 5, 10, 0), 0, 0, 103, 8484), //vsrlni_h_w
  INSTL(vsrlni_w_d, lsxVVI, (0b0111001101000001, 0, 5, 10, 0), 0, 0, 104, 8495), //vsrlni_w_d
  INSTL(vsrlr_b, lsxVVV, (0b01110000111100000, 0, 5, 10, 0), 0, 0, 244, 8506), //vsrlr_b
  INSTL(vsrlr_d, lsxVVV, (0b01110000111100011, 0, 5, 10, 0), 0, 0, 247, 8514), //vsrlr_d
  INSTL(vsrlr_h, lsxVVV, (0b01110000111100001, 0, 5, 10, 0), 0, 0, 245, 8522), //vsrlr_h
  INSTL(vsrlr_w, lsxVVV, (0b01110000111100010, 0, 5, 10, 0), 0, 0, 246, 8530), //vsrlr_w
  INSTL(vsrlri_b, lsxVVI, (0b0111001010100100001, 0, 5, 10, 0), 0, 0, 52, 8538), //vsrlri_b
  INSTL(vsrlri_d, lsxVVI, (0b0111001010100101, 0, 5, 10, 0), 0, 0, 55, 8547), //vsrlri_d
  INSTL(vsrlri_h, lsxVVI, (0b011100101010010001, 0, 5, 10, 0), 0, 0, 53, 8556), //vsrlri_h
  INSTL(vsrlri_w, lsxVVI, (0b01110010101001001, 0, 5, 10, 0), 0, 0, 54, 8565), //vsrlri_w
  INSTL(vsrlrn_b_h, lsxVVV, (0b01110000111110001, 0, 5, 10, 0), 0, 0, 258, 8574), //vsrlrn_b_h
  INSTL(vsrlrn_h_w, lsxVVV, (0b01110000111110010, 0, 5, 10, 0), 0, 0, 259, 8585), //vsrlrn_h_w
  INSTL(vsrlrn_w_d, lsxVVV, (0b01110000111110011, 0, 5, 10, 0), 0, 0, 260, 8596), //vsrlrn_w_d
  INSTL(vssran_b_h, lsxVVV, (0b01110000111111101, 0, 5, 10, 0), 0, 0, 267, 8607), //vssran_b_h
  INSTL(vssran_bu_h, lsxVVV, (0b01110001000001101, 0, 5, 10, 0), 0, 0, 279, 8618), //vssran_bu_h
  INSTL(vssran_h_w, lsxVVV, (0b01110000111111110, 0, 5, 10, 0), 0, 0, 268, 8630), //vssran_h_w
  INSTL(vssran_hu_w, lsxVVV, (0b01110001000001110, 0, 5, 10, 0), 0, 0, 280, 8641), //vssran_hu_w
  INSTL(vssran_w_d, lsxVVV, (0b01110000111111111, 0, 5, 10, 0), 0, 0, 269, 8653), //vssran_w_d
  INSTL(vssran_wu_d, lsxVVV, (0b01110001000001111, 0, 5, 10, 0), 0, 0, 281, 8664), //vssran_wu_d
  INSTL(vssrani_b_h, lsxVVI, (0b011100110110000001, 0, 5, 10, 0), 0, 0, 130, 8676), //vssrani_b_h
  INSTL(vssrani_bu_h, lsxVVI, (0b011100110110010001, 0, 5, 10, 0), 0, 0, 134, 8688), //vssrani_bu_h
  INSTL(vssrani_d_q, lsxVVI, (0b011100110110001, 0, 5, 10, 0), 0, 0, 133, 8701), //vssrani_d_q
  INSTL(vssrani_du_q, lsxVVI, (0b011100110110011, 0, 5, 10, 0), 0, 0, 137, 8713), //vssrani_du_q
  INSTL(vssrani_h_w, lsxVVI, (0b01110011011000001, 0, 5, 10, 0), 0, 0, 131, 8726), //vssrani_h_w
  INSTL(vssrani_hu_w, lsxVVI, (0b01110011011001001, 0, 5, 10, 0), 0, 0, 135, 8738), //vssrani_hu_w
  INSTL(vssrani_w_d, lsxVVI, (0b0111001101100001, 0, 5, 10, 0), 0, 0, 132, 8751), //vssrani_w_d
  INSTL(vssrani_wu_d, lsxVVI, (0b0111001101100101, 0, 5, 10, 0), 0, 0, 136, 8763), //vssrani_wu_d
  INSTL(vssrarn_b_h, lsxVVV, (0b01110001000000101, 0, 5, 10, 0), 0, 0, 273, 8776), //vssrarn_b_h
  INSTL(vssrarn_bu_h, lsxVVV, (0b01110001000010101, 0, 5, 10, 0), 0, 0, 285, 8788), //vssrarn_bu_h
  INSTL(vssrarn_h_w, lsxVVV, (0b01110001000000110, 0, 5, 10, 0), 0, 0, 274, 8801), //vssrarn_h_w
  INSTL(vssrarn_hu_w, lsxVVV, (0b01110001000010110, 0, 5, 10, 0), 0, 0, 286, 8813), //vssrarn_hu_w
  INSTL(vssrarn_w_d, lsxVVV, (0b01110001000000111, 0, 5, 10, 0), 0, 0, 275, 8826), //vssrarn_w_d
  INSTL(vssrarn_wu_d, lsxVVV, (0b01110001000010111, 0, 5, 10, 0), 0, 0, 287, 8838), //vssrarn_wu_d
  INSTL(vssrarni_b_h, lsxVVI, (0b011100110110100001, 0, 5, 10, 0), 0, 0, 138, 8851), //vssrarni_b_h
  INSTL(vssrarni_bu_h, lsxVVI, (0b011100110110110001, 0, 5, 10, 0), 0, 0, 142, 8864), //vssrarni_bu_h
  INSTL(vssrarni_d_q, lsxVVI, (0b011100110110101, 0, 5, 10, 0), 0, 0, 141, 8878), //vssrarni_d_q
  INSTL(vssrarni_du_q, lsxVVI, (0b011100110110111, 0, 5, 10, 0), 0, 0, 145, 8891), //vssrarni_du_q
  INSTL(vssrarni_h_w, lsxVVI, (0b01110011011010001, 0, 5, 10, 0), 0, 0, 139, 8905), //vssrarni_h_w
  INSTL(vssrarni_hu_w, lsxVVI, (0b01110011011011001, 0, 5, 10, 0), 0, 0, 143, 8918), //vssrarni_hu_w
  INSTL(vssrarni_w_d, lsxVVI, (0b0111001101101001, 0, 5, 10, 0), 0, 0, 140, 8932), //vssrarni_w_d
  INSTL(vssrarni_wu_d, lsxVVI, (0b0111001101101101, 0, 5, 10, 0), 0, 0, 144, 8945), //vssrarni_wu_d
  INSTL(vssrln_b_h, lsxVVV, (0b01110000111111001, 0, 5, 10, 0), 0, 0, 264, 8959), //vssrln_b_h
  INSTL(vssrln_bu_h, lsxVVV, (0b01110001000001001, 0, 5, 10, 0), 0, 0, 276, 8970), //vssrln_bu_h
  INSTL(vssrln_h_w, lsxVVV, (0b01110000111111010, 0, 5, 10, 0), 0, 0, 265, 8982), //vssrln_h_w
  INSTL(vssrln_hu_w, lsxVVV, (0b01110001000001010, 0, 5, 10, 0), 0, 0, 277, 8993), //vssrln_hu_w
  INSTL(vssrln_w_d, lsxVVV, (0b01110000111111011, 0, 5, 10, 0), 0, 0, 266, 9005), //vssrln_w_d
  INSTL(vssrln_wu_d, lsxVVV, (0b01110001000001011, 0, 5, 10, 0), 0, 0, 278, 9016), //vssrln_wu_d
  INSTL(vssrlni_b_h, lsxVVI, (0b011100110100100001, 0, 5, 10, 0), 0, 0, 106, 9028), //vssrlni_b_h
  INSTL(vssrlni_bu_h, lsxVVI, (0b011100110100110001, 0, 5, 10, 0), 0, 0, 110, 9040), //vssrlni_bu_h
  INSTL(vssrlni_d_q, lsxVVI, (0b011100110100101, 0, 5, 10, 0), 0, 0, 109, 9053), //vssrlni_d_q
  INSTL(vssrlni_du_q, lsxVVI, (0b011100110100111, 0, 5, 10, 0), 0, 0, 113, 9065), //vssrlni_du_q
  INSTL(vssrlni_h_w, lsxVVI, (0b01110011010010001, 0, 5, 10, 0), 0, 0, 107, 9078), //vssrlni_h_w
  INSTL(vssrlni_hu_w, lsxVVI, (0b01110011010011001, 0, 5, 10, 0), 0, 0, 111, 9090), //vssrlni_hu_w
  INSTL(vssrlni_w_d, lsxVVI, (0b0111001101001001, 0, 5, 10, 0), 0, 0, 108, 9103), //vssrlni_w_d
  INSTL(vssrlni_wu_d, lsxVVI, (0b0111001101001101, 0, 5, 10, 0), 0, 0, 112, 9115), //vssrlni_wu_d
  INSTL(vssrlrn_b_h, lsxVVV, (0b01110001000000001, 0, 5, 10, 0), 0, 0, 270, 9128), //vssrlrn_b_h
  INSTL(vssrlrn_bu_h, lsxVVV, (0b01110001000010001, 0, 5, 10, 0), 0, 0, 282, 9140), //vssrlrn_bu_h
  INSTL(vssrlrn_h_w, lsxVVV, (0b01110001000000010, 0, 5, 10, 0), 0, 0, 271, 9153), //vssrlrn_h_w
  INSTL(vssrlrn_hu_w, lsxVVV, (0b01110001000010010, 0, 5, 10, 0), 0, 0, 283, 9165), //vssrlrn_hu_w
  INSTL(vssrlrn_w_d, lsxVVV, (0b01110001000000011, 0, 5, 10, 0), 0, 0, 272, 9178), //vssrlrn_w_d
  INSTL(vssrlrn_wu_d, lsxVVV, (0b01110001000010011, 0, 5, 10, 0), 0, 0, 284, 9190), //vssrlrn_wu_d
  INSTL(vssrlrni_b_h, lsxVVI, (0b011100110101000001, 0, 5, 10, 0), 0, 0, 114, 9203), //vssrlrni_b_h
  INSTL(vssrlrni_bu_h, lsxVVI, (0b011100110101010001, 0, 5, 10, 0), 0, 0, 118, 9216), //vssrlrni_bu_h
  INSTL(vssrlrni_d_q, lsxVVI, (0b011100110101001, 0, 5, 10, 0), 0, 0, 117, 9230), //vssrlrni_d_q
  INSTL(vssrlrni_du_q, lsxVVI, (0b011100110101011, 0, 5, 10, 0), 0, 0, 121, 9243), //vssrlrni_du_q
  INSTL(vssrlrni_h_w, lsxVVI, (0b01110011010100001, 0, 5, 10, 0), 0, 0, 115, 9257), //vssrlrni_h_w
  INSTL(vssrlrni_hu_w, lsxVVI, (0b01110011010101001, 0, 5, 10, 0), 0, 0, 119, 9270), //vssrlrni_hu_w
  INSTL(vssrlrni_w_d, lsxVVI, (0b0111001101010001, 0, 5, 10, 0), 0, 0, 116, 9284), //vssrlrni_w_d
  INSTL(vssrlrni_wu_d, lsxVVI, (0b0111001101010101, 0, 5, 10, 0), 0, 0, 120, 9297), //vssrlrni_wu_d
  INSTL(vssub_b, lsxVVV, (0b01110000010010000, 0, 5, 10, 0), 0, 0, 72, 9311), //vssub_b
  INSTL(vssub_bu, lsxVVV, (0b01110000010011000, 0, 5, 10, 0), 0, 0, 80, 9319), //vssub_bu
  INSTL(vssub_d, lsxVVV, (0b01110000010010011, 0, 5, 10, 0), 0, 0, 75, 9328), //vssub_d
  INSTL(vssub_du, lsxVVV, (0b01110000010011011, 0, 5, 10, 0), 0, 0, 83, 9336), //vssub_du
  INSTL(vssub_h, lsxVVV, (0b01110000010010001, 0, 5, 10, 0), 0, 0, 73, 9345), //vssub_h
  INSTL(vssub_hu, lsxVVV, (0b01110000010011001, 0, 5, 10, 0), 0, 0, 81, 9353), //vssub_hu
  INSTL(vssub_w, lsxVVV, (0b01110000010010010, 0, 5, 10, 0), 0, 0, 74, 9362), //vssub_w
  INSTL(vssub_wu, lsxVVV, (0b01110000010011010, 0, 5, 10, 0), 0, 0, 82, 9370), //vssub_wu
  INSTL(vst,  SimdLdst, (0b0010110001,        22, 12), 0, 0, 12, 9379), //vst
  INSTL(vstelm_b, lsxVRII, (0b0011000110, 0, kWX, 5, 10, 18, 0), 0, 0, 3, 9383), //vstelm_b
  INSTL(vstelm_d, lsxVRII, (0b0011000100010, 0, kWX, 5, 10, 18, 0), 0, 0, 0, 9392), //vstelm_d
  INSTL(vstelm_h, lsxVRII, (0b00110001010, 0, kWX, 5, 10, 18, 0), 0, 0, 2, 9401), //vstelm_h
  INSTL(vstelm_w, lsxVRII, (0b001100010010, 0, kWX, 5, 10, 18, 0), 0, 0, 1, 9410), //vstelm_w
  INSTL(vstx, SimdLdst, (0b00111000010001000, 15, 0 ), 0, 0, 13, 9419), //vstx
  INSTL(vsub_b, lsxVVV, (0b01110000000011000, 0, 5, 10, 0), 0, 0, 24, 9424), //vsub_b
  INSTL(vsub_d, lsxVVV, (0b01110000000011011, 0, 5, 10, 0), 0, 0, 27, 9431), //vsub_d
  INSTL(vsub_h, lsxVVV, (0b01110000000011001, 0, 5, 10, 0), 0, 0, 25, 9438), //vsub_h
  INSTL(vsub_q, lsxVVV, (0b01110001001011011, 0, 5, 10, 0), 0, 0, 333, 9445), //vsub_q
  INSTL(vsub_w, lsxVVV, (0b01110000000011010, 0, 5, 10, 0), 0, 0, 26, 9452), //vsub_w
  INSTL(vsubi_bu, lsxVVI, (0b01110010100011000, 0, 5, 10, 0), 0, 0, 24, 9459), //vsubi_bu
  INSTL(vsubi_du, lsxVVI, (0b01110010100011011, 0, 5, 10, 0), 0, 0, 27, 9468), //vsubi_du
  INSTL(vsubi_hu, lsxVVI, (0b01110010100011001, 0, 5, 10, 0), 0, 0, 25, 9477), //vsubi_hu
  INSTL(vsubi_wu, lsxVVI, (0b01110010100011010, 0, 5, 10, 0), 0, 0, 26, 9486), //vsubi_wu
  INSTL(vsubwev_d_w, lsxVVV, (0b01110000001000010, 0, 5, 10, 0), 0, 0, 34, 9495), //vsubwev_d_w
  INSTL(vsubwev_d_wu, lsxVVV, (0b01110000001100010, 0, 5, 10, 0), 0, 0, 50, 9507), //vsubwev_d_wu
  INSTL(vsubwev_h_b, lsxVVV, (0b01110000001000000, 0, 5, 10, 0), 0, 0, 32, 9520), //vsubwev_h_b
  INSTL(vsubwev_h_bu, lsxVVV, (0b01110000001100000, 0, 5, 10, 0), 0, 0, 48, 9532), //vsubwev_h_bu
  INSTL(vsubwev_q_d, lsxVVV, (0b01110000001000011, 0, 5, 10, 0), 0, 0, 35, 9545), //vsubwev_q_d
  INSTL(vsubwev_q_du, lsxVVV, (0b01110000001100011, 0, 5, 10, 0), 0, 0, 51, 9557), //vsubwev_q_du
  INSTL(vsubwev_w_h, lsxVVV, (0b01110000001000001, 0, 5, 10, 0), 0, 0, 33, 9570), //vsubwev_w_h
  INSTL(vsubwev_w_hu, lsxVVV, (0b01110000001100001, 0, 5, 10, 0), 0, 0, 49, 9582), //vsubwev_w_hu
  INSTL(vsubwod_d_w, lsxVVV, (0b01110000001001010, 0, 5, 10, 0), 0, 0, 42, 9595), //vsubwod_d_w
  INSTL(vsubwod_d_wu, lsxVVV, (0b01110000001101010, 0, 5, 10, 0), 0, 0, 58, 9607), //vsubwod_d_wu
  INSTL(vsubwod_h_b, lsxVVV, (0b01110000001001000, 0, 5, 10, 0), 0, 0, 40, 9620), //vsubwod_h_b
  INSTL(vsubwod_h_bu, lsxVVV, (0b01110000001101000, 0, 5, 10, 0), 0, 0, 56, 9632), //vsubwod_h_bu
  INSTL(vsubwod_q_d, lsxVVV, (0b01110000001001011, 0, 5, 10, 0), 0, 0, 43, 9645), //vsubwod_q_d
  INSTL(vsubwod_q_du, lsxVVV, (0b01110000001101011, 0, 5, 10, 0), 0, 0, 59, 9657), //vsubwod_q_du
  INSTL(vsubwod_w_h, lsxVVV, (0b01110000001001001, 0, 5, 10, 0), 0, 0, 41, 9670), //vsubwod_w_h
  INSTL(vsubwod_w_hu, lsxVVV, (0b01110000001101001, 0, 5, 10, 0), 0, 0, 57, 9682), //vsubwod_w_hu
  INSTL(vxor_v, lsxVVV, (0b01110001001001110, 0, 5, 10, 0), 0, 0, 326, 9695), //vxor_v
  INSTL(vxori_b, lsxVVI, (0b01110011110110, 0, 5, 10, 0), 0, 0, 157, 9702), //vxori_b
  INSTL(xvabsd_b, lasxXXX, (0b01110100011000000, 0, 5, 10, 0), 0, 0, 104, 9710), //xvabsd_b
  INSTL(xvabsd_bu, lasxXXX, (0b01110100011000100, 0, 5, 10, 0), 0, 0, 108, 9719), //xvabsd_bu
  INSTL(xvabsd_d, lasxXXX, (0b01110100011000011, 0, 5, 10, 0), 0, 0, 107, 9729), //xvabsd_d
  INSTL(xvabsd_du, lasxXXX, (0b01110100011000111, 0, 5, 10, 0), 0, 0, 111, 9738), //xvabsd_du
  INSTL(xvabsd_h, lasxXXX, (0b01110100011000001, 0, 5, 10, 0), 0, 0, 105, 9748), //xvabsd_h
  INSTL(xvabsd_hu, lasxXXX, (0b01110100011000101, 0, 5, 10, 0), 0, 0, 109, 9757), //xvabsd_hu
  INSTL(xvabsd_w, lasxXXX, (0b01110100011000010, 0, 5, 10, 0), 0, 0, 106, 9767), //xvabsd_w
  INSTL(xvabsd_wu, lasxXXX, (0b01110100011000110, 0, 5, 10, 0), 0, 0, 110, 9776), //xvabsd_wu
  INSTL(xvadd_b, lasxXXX, (0b01110100000010100, 0, 5, 10, 0), 0, 0, 20, 9786), //xvadd_b
  INSTL(xvadd_d, lasxXXX, (0b01110100000010111, 0, 5, 10, 0), 0, 0, 23, 9794), //xvadd_d
  INSTL(xvadd_h, lasxXXX, (0b01110100000010101, 0, 5, 10, 0), 0, 0, 21, 9802), //xvadd_h
  INSTL(xvadd_q, lasxXXX, (0b01110101001011010, 0, 5, 10, 0), 0, 0, 332, 9810), //xvadd_q
  INSTL(xvadd_w, lasxXXX, (0b01110100000010110, 0, 5, 10, 0), 0, 0, 22, 9818), //xvadd_w
  INSTL(xvadda_b, lasxXXX, (0b01110100010111000, 0, 5, 10, 0), 0, 0, 100, 9826), //xvadda_b
  INSTL(xvadda_d, lasxXXX, (0b01110100010111011, 0, 5, 10, 0), 0, 0, 103, 9835), //xvadda_d
  INSTL(xvadda_h, lasxXXX, (0b01110100010111001, 0, 5, 10, 0), 0, 0, 101, 9844), //xvadda_h
  INSTL(xvadda_w, lasxXXX, (0b01110100010111010, 0, 5, 10, 0), 0, 0, 102, 9853), //xvadda_w
  INSTL(xvaddi_bu, lasxXXI, (0b01110110100010100, 0, 5, 10, 0), 0, 0, 20, 9862), //xvaddi_bu
  INSTL(xvaddi_du, lasxXXI, (0b01110110100010111, 0, 5, 10, 0), 0, 0, 23, 9872), //xvaddi_du
  INSTL(xvaddi_hu, lasxXXI, (0b01110110100010101, 0, 5, 10, 0), 0, 0, 21, 9882), //xvaddi_hu
  INSTL(xvaddi_wu, lasxXXI, (0b01110110100010110, 0, 5, 10, 0), 0, 0, 22, 9892), //xvaddi_wu
  INSTL(xvaddwev_d_w, lasxXXX, (0b01110100000111110, 0, 5, 10, 0), 0, 0, 30, 9902), //xvaddwev_d_w
  INSTL(xvaddwev_d_wu, lasxXXX, (0b01110100001011110, 0, 5, 10, 0), 0, 0, 46, 9915), //xvaddwev_d_wu
  INSTL(xvaddwev_d_wu_w, lasxXXX, (0b01110100001111110, 0, 5, 10, 0), 0, 0, 62, 9929), //xvaddwev_d_wu_w
  INSTL(xvaddwev_h_b, lasxXXX, (0b01110100000111100, 0, 5, 10, 0), 0, 0, 28, 9945), //xvaddwev_h_b
  INSTL(xvaddwev_h_bu, lasxXXX, (0b01110100001011100, 0, 5, 10, 0), 0, 0, 44, 9958), //xvaddwev_h_bu
  INSTL(xvaddwev_h_bu_b, lasxXXX, (0b01110100001111100, 0, 5, 10, 0), 0, 0, 60, 9972), //xvaddwev_h_bu_b
  INSTL(xvaddwev_q_d, lasxXXX, (0b01110100000111111, 0, 5, 10, 0), 0, 0, 31, 9988), //xvaddwev_q_d
  INSTL(xvaddwev_q_du, lasxXXX, (0b01110100001011111, 0, 5, 10, 0), 0, 0, 47, 10001), //xvaddwev_q_du
  INSTL(xvaddwev_q_du_d, lasxXXX, (0b01110100001111111, 0, 5, 10, 0), 0, 0, 63, 10015), //xvaddwev_q_du_d
  INSTL(xvaddwev_w_h, lasxXXX, (0b01110100000111101, 0, 5, 10, 0), 0, 0, 29, 10031), //xvaddwev_w_h
  INSTL(xvaddwev_w_hu, lasxXXX, (0b01110100001011101, 0, 5, 10, 0), 0, 0, 45, 10044), //xvaddwev_w_hu
  INSTL(xvaddwev_w_hu_h, lasxXXX, (0b01110100001111101, 0, 5, 10, 0), 0, 0, 61, 10058), //xvaddwev_w_hu_h
  INSTL(xvaddwod_d_w, lasxXXX, (0b01110100001000110, 0, 5, 10, 0), 0, 0, 38, 10074), //xvaddwod_d_w
  INSTL(xvaddwod_d_wu, lasxXXX, (0b01110100001100110, 0, 5, 10, 0), 0, 0, 54, 10087), //xvaddwod_d_wu
  INSTL(xvaddwod_d_wu_w, lasxXXX, (0b01110100010000010, 0, 5, 10, 0), 0, 0, 66, 10101), //xvaddwod_d_wu_w
  INSTL(xvaddwod_h_b, lasxXXX, (0b01110100001000100, 0, 5, 10, 0), 0, 0, 36, 10117), //xvaddwod_h_b
  INSTL(xvaddwod_h_bu, lasxXXX, (0b01110100001100100, 0, 5, 10, 0), 0, 0, 52, 10130), //xvaddwod_h_bu
  INSTL(xvaddwod_h_bu_b, lasxXXX, (0b01110100010000000, 0, 5, 10, 0), 0, 0, 64, 10144), //xvaddwod_h_bu_b
  INSTL(xvaddwod_q_d, lasxXXX, (0b01110100001000111, 0, 5, 10, 0), 0, 0, 39, 10160), //xvaddwod_q_d
  INSTL(xvaddwod_q_du, lasxXXX, (0b01110100001100111, 0, 5, 10, 0), 0, 0, 55, 10173), //xvaddwod_q_du
  INSTL(xvaddwod_q_du_d, lasxXXX, (0b01110100010000011, 0, 5, 10, 0), 0, 0, 67, 10187), //xvaddwod_q_du_d
  INSTL(xvaddwod_w_h, lasxXXX, (0b01110100001000101, 0, 5, 10, 0), 0, 0, 37, 10203), //xvaddwod_w_h
  INSTL(xvaddwod_w_hu, lasxXXX, (0b01110100001100101, 0, 5, 10, 0), 0, 0, 53, 10216), //xvaddwod_w_hu
  INSTL(xvaddwod_w_hu_h, lasxXXX, (0b01110100010000001, 0, 5, 10, 0), 0, 0, 65, 10230), //xvaddwod_w_hu_h
  INSTL(xvand_v, lasxXXX, (0b01110101001001100, 0, 5, 10, 0), 0, 0, 324, 10246), //xvand_v
  INSTL(xvandi_b, lasxXXI, (0b01110111110100, 0, 5, 10, 0), 0, 0, 164, 10254), //xvandi_b
  INSTL(xvandn_v, lasxXXX, (0b01110101001010000, 0, 5, 10, 0), 0, 0, 328, 10263), //xvandn_v
  INSTL(xvavg_b, lasxXXX, (0b01110100011001000, 0, 5, 10, 0), 0, 0, 112, 10272), //xvavg_b
  INSTL(xvavg_bu, lasxXXX, (0b01110100011001100, 0, 5, 10, 0), 0, 0, 116, 10280), //xvavg_bu
  INSTL(xvavg_d, lasxXXX, (0b01110100011001011, 0, 5, 10, 0), 0, 0, 115, 10289), //xvavg_d
  INSTL(xvavg_du, lasxXXX, (0b01110100011001111, 0, 5, 10, 0), 0, 0, 119, 10297), //xvavg_du
  INSTL(xvavg_h, lasxXXX, (0b01110100011001001, 0, 5, 10, 0), 0, 0, 113, 10306), //xvavg_h
  INSTL(xvavg_hu, lasxXXX, (0b01110100011001101, 0, 5, 10, 0), 0, 0, 117, 10314), //xvavg_hu
  INSTL(xvavg_w, lasxXXX, (0b01110100011001010, 0, 5, 10, 0), 0, 0, 114, 10323), //xvavg_w
  INSTL(xvavg_wu, lasxXXX, (0b01110100011001110, 0, 5, 10, 0), 0, 0, 118, 10331), //xvavg_wu
  INSTL(xvavgr_b, lasxXXX, (0b01110100011010000, 0, 5, 10, 0), 0, 0, 120, 10340), //xvavgr_b
  INSTL(xvavgr_bu, lasxXXX, (0b01110100011010100, 0, 5, 10, 0), 0, 0, 124, 10349), //xvavgr_bu
  INSTL(xvavgr_d, lasxXXX, (0b01110100011010011, 0, 5, 10, 0), 0, 0, 123, 10359), //xvavgr_d
  INSTL(xvavgr_du, lasxXXX, (0b01110100011010111, 0, 5, 10, 0), 0, 0, 127, 10368), //xvavgr_du
  INSTL(xvavgr_h, lasxXXX, (0b01110100011010001, 0, 5, 10, 0), 0, 0, 121, 10378), //xvavgr_h
  INSTL(xvavgr_hu, lasxXXX, (0b01110100011010101, 0, 5, 10, 0), 0, 0, 125, 10387), //xvavgr_hu
  INSTL(xvavgr_w, lasxXXX, (0b01110100011010010, 0, 5, 10, 0), 0, 0, 122, 10397), //xvavgr_w
  INSTL(xvavgr_wu, lasxXXX, (0b01110100011010110, 0, 5, 10, 0), 0, 0, 126, 10406), //xvavgr_wu
  INSTL(xvbitclr_b, lasxXXX, (0b01110101000011000, 0, 5, 10, 0), 0, 0, 288, 10416), //xvbitclr_b
  INSTL(xvbitclr_d, lasxXXX, (0b01110101000011011, 0, 5, 10, 0), 0, 0, 291, 10427), //xvbitclr_d
  INSTL(xvbitclr_h, lasxXXX, (0b01110101000011001, 0, 5, 10, 0), 0, 0, 289, 10438), //xvbitclr_h
  INSTL(xvbitclr_w, lasxXXX, (0b01110101000011010, 0, 5, 10, 0), 0, 0, 290, 10449), //xvbitclr_w
  INSTL(xvbitclri_b, lasxXXI, (0b0111011100010000001, 0, 5, 10, 0), 0, 0, 75, 10460), //xvbitclri_b
  INSTL(xvbitclri_d, lasxXXI, (0b0111011100010001, 0, 5, 10, 0), 0, 0, 78, 10472), //xvbitclri_d
  INSTL(xvbitclri_h, lasxXXI, (0b011101110001000001, 0, 5, 10, 0), 0, 0, 76, 10484), //xvbitclri_h
  INSTL(xvbitclri_w, lasxXXI, (0b01110111000100001, 0, 5, 10, 0), 0, 0, 77, 10496), //xvbitclri_w
  INSTL(xvbitrev_b, lasxXXX, (0b01110101000100000, 0, 5, 10, 0), 0, 0, 296, 10508), //xvbitrev_b
  INSTL(xvbitrev_d, lasxXXX, (0b01110101000100011, 0, 5, 10, 0), 0, 0, 299, 10519), //xvbitrev_d
  INSTL(xvbitrev_h, lasxXXX, (0b01110101000100001, 0, 5, 10, 0), 0, 0, 297, 10530), //xvbitrev_h
  INSTL(xvbitrev_w, lasxXXX, (0b01110101000100010, 0, 5, 10, 0), 0, 0, 298, 10541), //xvbitrev_w
  INSTL(xvbitrevi_b, lasxXXI, (0b0111011100011000001, 0, 5, 10, 0), 0, 0, 83, 10552), //xvbitrevi_b
  INSTL(xvbitrevi_d, lasxXXI, (0b0111011100011001, 0, 5, 10, 0), 0, 0, 86, 10564), //xvbitrevi_d
  INSTL(xvbitrevi_h, lasxXXI, (0b011101110001100001, 0, 5, 10, 0), 0, 0, 84, 10576), //xvbitrevi_h
  INSTL(xvbitrevi_w, lasxXXI, (0b01110111000110001, 0, 5, 10, 0), 0, 0, 85, 10588), //xvbitrevi_w
  INSTL(xvbitsel_v, lasxXXXX, (0b000011010010, 0, 5, 10, 15, 0), 0, 0, 8, 10600), //xvbitsel_v
  INSTL(xvbitseli_b, lasxXXI, (0b01110111110001, 0, 5, 10, 0), 0, 0, 163, 10611), //xvbitseli_b
  INSTL(xvbitset_b, lasxXXX, (0b01110101000011100, 0, 5, 10, 0), 0, 0, 292, 10623), //xvbitset_b
  INSTL(xvbitset_d, lasxXXX, (0b01110101000011111, 0, 5, 10, 0), 0, 0, 295, 10634), //xvbitset_d
  INSTL(xvbitset_h, lasxXXX, (0b01110101000011101, 0, 5, 10, 0), 0, 0, 293, 10645), //xvbitset_h
  INSTL(xvbitset_w, lasxXXX, (0b01110101000011110, 0, 5, 10, 0), 0, 0, 294, 10656), //xvbitset_w
  INSTL(xvbitseti_b, lasxXXI, (0b0111011100010100001, 0, 5, 10, 0), 0, 0, 79, 10667), //xvbitseti_b
  INSTL(xvbitseti_d, lasxXXI, (0b0111011100010101, 0, 5, 10, 0), 0, 0, 82, 10679), //xvbitseti_d
  INSTL(xvbitseti_h, lasxXXI, (0b011101110001010001, 0, 5, 10, 0), 0, 0, 80, 10691), //xvbitseti_h
  INSTL(xvbitseti_w, lasxXXI, (0b01110111000101001, 0, 5, 10, 0), 0, 0, 81, 10703), //xvbitseti_w
  INSTL(xvbsll_v, lasxXXI, (0b01110110100011100, 0, 5, 10, 0), 0, 0, 28, 10715), //xvbsll_v
  INSTL(xvbsrl_v, lasxXXI, (0b01110110100011101, 0, 5, 10, 0), 0, 0, 29, 10724), //xvbsrl_v
  INSTL(xvclo_b, lasxXX, (0b0111011010011100000000, 0, 5, 0), 0, 0, 0, 10733), //xvclo_b
  INSTL(xvclo_d, lasxXX, (0b0111011010011100000011, 0, 5, 0), 0, 0, 3, 10741), //xvclo_d
  INSTL(xvclo_h, lasxXX, (0b0111011010011100000001, 0, 5, 0), 0, 0, 1, 10749), //xvclo_h
  INSTL(xvclo_w, lasxXX, (0b0111011010011100000010, 0, 5, 0), 0, 0, 2, 10757), //xvclo_w
  INSTL(xvclz_b, lasxXX, (0b0111011010011100000100, 0, 5, 0), 0, 0, 4, 10765), //xvclz_b
  INSTL(xvclz_d, lasxXX, (0b0111011010011100000111, 0, 5, 0), 0, 0, 7, 10773), //xvclz_d
  INSTL(xvclz_h, lasxXX, (0b0111011010011100000101, 0, 5, 0), 0, 0, 5, 10781), //xvclz_h
  INSTL(xvclz_w, lasxXX, (0b0111011010011100000110, 0, 5, 0), 0, 0, 6, 10789), //xvclz_w
  INSTL(xvdiv_b, lasxXXX, (0b01110100111000000, 0, 5, 10, 0), 0, 0, 212, 10797), //xvdiv_b
  INSTL(xvdiv_bu, lasxXXX, (0b01110100111001000, 0, 5, 10, 0), 0, 0, 220, 10805), //xvdiv_bu
  INSTL(xvdiv_d, lasxXXX, (0b01110100111000011, 0, 5, 10, 0), 0, 0, 215, 10814), //xvdiv_d
  INSTL(xvdiv_du, lasxXXX, (0b01110100111001011, 0, 5, 10, 0), 0, 0, 223, 10822), //xvdiv_du
  INSTL(xvdiv_h, lasxXXX, (0b01110100111000001, 0, 5, 10, 0), 0, 0, 213, 10831), //xvdiv_h
  INSTL(xvdiv_hu, lasxXXX, (0b01110100111001001, 0, 5, 10, 0), 0, 0, 221, 10839), //xvdiv_hu
  INSTL(xvdiv_w, lasxXXX, (0b01110100111000010, 0, 5, 10, 0), 0, 0, 214, 10848), //xvdiv_w
  INSTL(xvdiv_wu, lasxXXX, (0b01110100111001010, 0, 5, 10, 0), 0, 0, 222, 10856), //xvdiv_wu
  INSTL(xvexth_d_w, lasxXX, (0b0111011010011110111010, 0, 5, 0), 0, 0, 78, 10865), //xvexth_d_w
  INSTL(xvexth_du_wu, lasxXX, (0b0111011010011110111110, 0, 5, 0), 0, 0, 82, 10876), //xvexth_du_wu
  INSTL(xvexth_h_b, lasxXX, (0b0111011010011110111000, 0, 5, 0), 0, 0, 76, 10889), //xvexth_h_b
  INSTL(xvexth_hu_bu, lasxXX, (0b0111011010011110111100, 0, 5, 0), 0, 0, 80, 10900), //xvexth_hu_bu
  INSTL(xvexth_q_d, lasxXX, (0b0111011010011110111011, 0, 5, 0), 0, 0, 79, 10913), //xvexth_q_d
  INSTL(xvexth_qu_du, lasxXX, (0b0111011010011110111111, 0, 5, 0), 0, 0, 83, 10924), //xvexth_qu_du
  INSTL(xvexth_w_h, lasxXX, (0b0111011010011110111001, 0, 5, 0), 0, 0, 77, 10937), //xvexth_w_h
  INSTL(xvexth_wu_hu, lasxXX, (0b0111011010011110111101, 0, 5, 0), 0, 0, 81, 10948), //xvexth_wu_hu
  INSTL(xvextl_q_d, lasxXX, (0b0111011100001001000000, 0, 5, 0), 0, 0, 101, 10961), //xvextl_q_d
  INSTL(xvextl_qu_du, lasxXX, (0b0111011100001101000000, 0, 5, 0), 0, 0, 102, 10972), //xvextl_qu_du
  INSTL(xvextrins_b, lasxXXI, (0b01110111100011, 0, 5, 10, 0), 0, 0, 158, 10985), //xvextrins_b
  INSTL(xvextrins_d, lasxXXI, (0b01110111100000, 0, 5, 10, 0), 0, 0, 155, 10997), //xvextrins_d
  INSTL(xvextrins_h, lasxXXI, (0b01110111100010, 0, 5, 10, 0), 0, 0, 157, 11009), //xvextrins_h
  INSTL(xvextrins_w, lasxXXI, (0b01110111100001, 0, 5, 10, 0), 0, 0, 156, 11021), //xvextrins_w
  INSTL(xvfadd_d, lasxXXX, (0b01110101001100010, 0, 5, 10, 0), 0, 0, 339, 11033), //xvfadd_d
  INSTL(xvfadd_s, lasxXXX, (0b01110101001100001, 0, 5, 10, 0), 0, 0, 338, 11042), //xvfadd_s
  INSTL(xvfclass_d, lasxXX, (0b0111011010011100110110, 0, 5, 0), 0, 0, 25, 11051), //xvfclass_d
  INSTL(xvfclass_s, lasxXX, (0b0111011010011100110101, 0, 5, 0), 0, 0, 24, 11062), //xvfclass_s
  INSTL(xvfcvt_h_s, lasxXXX, (0b01110101010001100, 0, 5, 10, 0), 0, 0, 356, 11073), //xvfcvt_h_s
  INSTL(xvfcvt_s_d, lasxXXX, (0b01110101010001101, 0, 5, 10, 0), 0, 0, 357, 11084), //xvfcvt_s_d
  INSTL(xvfcvth_d_s, lasxXX, (0b0111011010011101111101, 0, 5, 0), 0, 0, 45, 11095), //xvfcvth_d_s
  INSTL(xvfcvth_s_h, lasxXX, (0b0111011010011101111011, 0, 5, 0), 0, 0, 43, 11107), //xvfcvth_s_h
  INSTL(xvfcvtl_d_s, lasxXX, (0b0111011010011101111100, 0, 5, 0), 0, 0, 44, 11119), //xvfcvtl_d_s
  INSTL(xvfcvtl_s_h, lasxXX, (0b0111011010011101111010, 0, 5, 0), 0, 0, 42, 11131), //xvfcvtl_s_h
  INSTL(xvfdiv_d, lasxXXX, (0b01110101001110110, 0, 5, 10, 0), 0, 0, 345, 11143), //xvfdiv_d
  INSTL(xvfdiv_s, lasxXXX, (0b01110101001110101, 0, 5, 10, 0), 0, 0, 344, 11152), //xvfdiv_s
  INSTL(xvffint_d_l, lasxXX, (0b0111011010011110000010, 0, 5, 0), 0, 0, 48, 11161), //xvffint_d_l
  INSTL(xvffint_d_lu, lasxXX, (0b0111011010011110000011, 0, 5, 0), 0, 0, 49, 11173), //xvffint_d_lu
  INSTL(xvffint_s_l, lasxXXX, (0b01110101010010000, 0, 5, 10, 0), 0, 0, 358, 11186), //xvffint_s_l
  INSTL(xvffint_s_w, lasxXX, (0b0111011010011110000000, 0, 5, 0), 0, 0, 46, 11198), //xvffint_s_w
  INSTL(xvffint_s_wu, lasxXX, (0b0111011010011110000001, 0, 5, 0), 0, 0, 47, 11210), //xvffint_s_wu
  INSTL(xvffinth_d_w, lasxXX, (0b0111011010011110000101, 0, 5, 0), 0, 0, 51, 11223), //xvffinth_d_w
  INSTL(xvffintl_d_w, lasxXX, (0b0111011010011110000100, 0, 5, 0), 0, 0, 50, 11236), //xvffintl_d_w
  INSTL(xvflogb_d, lasxXX, (0b0111011010011100110010, 0, 5, 0), 0, 0, 23, 11249), //xvflogb_d
  INSTL(xvflogb_s, lasxXX, (0b0111011010011100110001, 0, 5, 0), 0, 0, 22, 11259), //xvflogb_s
  INSTL(xvfmadd_d, lasxXXXX, (0b000010100010, 0, 5, 10, 15, 0), 0, 0, 1, 11269), //xvfmadd_d
  INSTL(xvfmadd_s, lasxXXXX, (0b000010100001, 0, 5, 10, 15, 0), 0, 0, 0, 11279), //xvfmadd_s
  INSTL(xvfmax_d, lasxXXX, (0b01110101001111010, 0, 5, 10, 0), 0, 0, 347, 11289), //xvfmax_d
  INSTL(xvfmax_s, lasxXXX, (0b01110101001111001, 0, 5, 10, 0), 0, 0, 346, 11298), //xvfmax_s
  INSTL(xvfmaxa_d, lasxXXX, (0b01110101010000010, 0, 5, 10, 0), 0, 0, 351, 11307), //xvfmaxa_d
  INSTL(xvfmaxa_s, lasxXXX, (0b01110101010000001, 0, 5, 10, 0), 0, 0, 350, 11317), //xvfmaxa_s
  INSTL(xvfmin_d, lasxXXX, (0b01110101001111110, 0, 5, 10, 0), 0, 0, 349, 11327), //xvfmin_d
  INSTL(xvfmin_s, lasxXXX, (0b01110101001111101, 0, 5, 10, 0), 0, 0, 348, 11336), //xvfmin_s
  INSTL(xvfmina_d, lasxXXX, (0b01110101010000110, 0, 5, 10, 0), 0, 0, 353, 11345), //xvfmina_d
  INSTL(xvfmina_s, lasxXXX, (0b01110101010000101, 0, 5, 10, 0), 0, 0, 352, 11355), //xvfmina_s
  INSTL(xvfmsub_d, lasxXXXX, (0b000010100110, 0, 5, 10, 15, 0), 0, 0, 3, 11365), //xvfmsub_d
  INSTL(xvfmsub_s, lasxXXXX, (0b000010100101, 0, 5, 10, 15, 0), 0, 0, 2, 11375), //xvfmsub_s
  INSTL(xvfmul_d, lasxXXX, (0b01110101001110010, 0, 5, 10, 0), 0, 0, 343, 11385), //xvfmul_d
  INSTL(xvfmul_s, lasxXXX, (0b01110101001110001, 0, 5, 10, 0), 0, 0, 342, 11394), //xvfmul_s
  INSTL(xvfnmadd_d, lasxXXXX, (0b000010101010, 0, 5, 10, 15, 0), 0, 0, 5, 11403), //xvfnmadd_d
  INSTL(xvfnmadd_s, lasxXXXX, (0b000010101001, 0, 5, 10, 15, 0), 0, 0, 4, 11414), //xvfnmadd_s
  INSTL(xvfnmsub_d, lasxXXXX, (0b000010101110, 0, 5, 10, 15, 0), 0, 0, 7, 11425), //xvfnmsub_d
  INSTL(xvfnmsub_s, lasxXXXX, (0b000010101101, 0, 5, 10, 15, 0), 0, 0, 6, 11436), //xvfnmsub_s
  INSTL(xvfrecip_d, lasxXX, (0b0111011010011100111110, 0, 5, 0), 0, 0, 29, 11447), //xvfrecip_d
  INSTL(xvfrecip_s, lasxXX, (0b0111011010011100111101, 0, 5, 0), 0, 0, 28, 11458), //xvfrecip_s
  INSTL(xvfrint_d, lasxXX, (0b0111011010011101001110, 0, 5, 0), 0, 0, 33, 11469), //xvfrint_d
  INSTL(xvfrint_s, lasxXX, (0b0111011010011101001101, 0, 5, 0), 0, 0, 32, 11479), //xvfrint_s
  INSTL(xvfrintrm_d, lasxXX, (0b0111011010011101010010, 0, 5, 0), 0, 0, 35, 11489), //xvfrintrm_d
  INSTL(xvfrintrm_s, lasxXX, (0b0111011010011101010001, 0, 5, 0), 0, 0, 34, 11501), //xvfrintrm_s
  INSTL(xvfrintrne_d, lasxXX, (0b0111011010011101011110, 0, 5, 0), 0, 0, 41, 11513), //xvfrintrne_d
  INSTL(xvfrintrne_s, lasxXX, (0b0111011010011101011101, 0, 5, 0), 0, 0, 40, 11526), //xvfrintrne_s
  INSTL(xvfrintrp_d, lasxXX, (0b0111011010011101010110, 0, 5, 0), 0, 0, 37, 11539), //xvfrintrp_d
  INSTL(xvfrintrp_s, lasxXX, (0b0111011010011101010101, 0, 5, 0), 0, 0, 36, 11551), //xvfrintrp_s
  INSTL(xvfrintrz_d, lasxXX, (0b0111011010011101011010, 0, 5, 0), 0, 0, 39, 11563), //xvfrintrz_d
  INSTL(xvfrintrz_s, lasxXX, (0b0111011010011101011001, 0, 5, 0), 0, 0, 38, 11575), //xvfrintrz_s
  INSTL(xvfrsqrt_d, lasxXX, (0b0111011010011101000010, 0, 5, 0), 0, 0, 31, 11587), //xvfrsqrt_d
  INSTL(xvfrsqrt_s, lasxXX, (0b0111011010011101000001, 0, 5, 0), 0, 0, 30, 11598), //xvfrsqrt_s
  INSTL(xvfrstp_b, lasxXXX, (0b01110101001010110, 0, 5, 10, 0), 0, 0, 330, 11609), //xvfrstp_b
  INSTL(xvfrstp_h, lasxXXX, (0b01110101001010111, 0, 5, 10, 0), 0, 0, 331, 11619), //xvfrstp_h
  INSTL(xvfrstpi_b, lasxXXI, (0b01110110100110100, 0, 5, 10, 0), 0, 0, 46, 11629), //xvfrstpi_b
  INSTL(xvfrstpi_h, lasxXXI, (0b01110110100110101, 0, 5, 10, 0), 0, 0, 47, 11640), //xvfrstpi_h
  INSTL(xvfscaleb_d, lasxXXX, (0b01110101010001010, 0, 5, 10, 0), 0, 0, 355, 11651), //xvfscaleb_d
  INSTL(xvfscaleb_s, lasxXXX, (0b01110101010001001, 0, 5, 10, 0), 0, 0, 354, 11663), //xvfscaleb_s
  INSTL(xvfsqrt_d, lasxXX, (0b0111011010011100111010, 0, 5, 0), 0, 0, 27, 11675), //xvfsqrt_d
  INSTL(xvfsqrt_s, lasxXX, (0b0111011010011100111001, 0, 5, 0), 0, 0, 26, 11685), //xvfsqrt_s
  INSTL(xvfsub_d, lasxXXX, (0b01110101001100110, 0, 5, 10, 0), 0, 0, 341, 11695), //xvfsub_d
  INSTL(xvfsub_s, lasxXXX, (0b01110101001100101, 0, 5, 10, 0), 0, 0, 340, 11704), //xvfsub_s
  INSTL(xvftint_l_d, lasxXX, (0b0111011010011110001101, 0, 5, 0), 0, 0, 53, 11713), //xvftint_l_d
  INSTL(xvftint_lu_d, lasxXX, (0b0111011010011110010111, 0, 5, 0), 0, 0, 63, 11725), //xvftint_lu_d
  INSTL(xvftint_w_d, lasxXXX, (0b01110101010010011, 0, 5, 10, 0), 0, 0, 359, 11738), //xvftint_w_d
  INSTL(xvftint_w_s, lasxXX, (0b0111011010011110001100, 0, 5, 0), 0, 0, 52, 11750), //xvftint_w_s
  INSTL(xvftint_wu_s, lasxXX, (0b0111011010011110010110, 0, 5, 0), 0, 0, 62, 11762), //xvftint_wu_s
  INSTL(xvftinth_l_s, lasxXX, (0b0111011010011110100001, 0, 5, 0), 0, 0, 67, 11775), //xvftinth_l_s
  INSTL(xvftintl_l_s, lasxXX, (0b0111011010011110100000, 0, 5, 0), 0, 0, 66, 11788), //xvftintl_l_s
  INSTL(xvftintrm_l_d, lasxXX, (0b0111011010011110001111, 0, 5, 0), 0, 0, 55, 11801), //xvftintrm_l_d
  INSTL(xvftintrm_w_d, lasxXXX, (0b01110101010010100, 0, 5, 10, 0), 0, 0, 360, 11815), //xvftintrm_w_d
  INSTL(xvftintrm_w_s, lasxXX, (0b0111011010011110001110, 0, 5, 0), 0, 0, 54, 11829), //xvftintrm_w_s
  INSTL(xvftintrmh_l_s, lasxXX, (0b0111011010011110100011, 0, 5, 0), 0, 0, 69, 11843), //xvftintrmh_l_s
  INSTL(xvftintrml_l_s, lasxXX, (0b0111011010011110100010, 0, 5, 0), 0, 0, 68, 11858), //xvftintrml_l_s
  INSTL(xvftintrne_l_d, lasxXX, (0b0111011010011110010101, 0, 5, 0), 0, 0, 61, 11873), //xvftintrne_l_d
  INSTL(xvftintrne_w_d, lasxXXX, (0b01110101010010111, 0, 5, 10, 0), 0, 0, 363, 11888), //xvftintrne_w_d
  INSTL(xvftintrne_w_s, lasxXX, (0b0111011010011110010100, 0, 5, 0), 0, 0, 60, 11903), //xvftintrne_w_s
  INSTL(xvftintrneh_l_s, lasxXX, (0b0111011010011110101001, 0, 5, 0), 0, 0, 75, 11918), //xvftintrneh_l_s
  INSTL(xvftintrnel_l_s, lasxXX, (0b0111011010011110101000, 0, 5, 0), 0, 0, 74, 11934), //xvftintrnel_l_s
  INSTL(xvftintrp_l_d, lasxXX, (0b0111011010011110010001, 0, 5, 0), 0, 0, 57, 11950), //xvftintrp_l_d
  INSTL(xvftintrp_w_d, lasxXXX, (0b01110101010010101, 0, 5, 10, 0), 0, 0, 361, 11964), //xvftintrp_w_d
  INSTL(xvftintrp_w_s, lasxXX, (0b0111011010011110010000, 0, 5, 0), 0, 0, 56, 11978), //xvftintrp_w_s
  INSTL(xvftintrph_l_s, lasxXX, (0b0111011010011110100101, 0, 5, 0), 0, 0, 71, 11992), //xvftintrph_l_s
  INSTL(xvftintrpl_l_s, lasxXX, (0b0111011010011110100100, 0, 5, 0), 0, 0, 70, 12007), //xvftintrpl_l_s
  INSTL(xvftintrz_l_d, lasxXX, (0b0111011010011110010011, 0, 5, 0), 0, 0, 59, 12022), //xvftintrz_l_d
  INSTL(xvftintrz_lu_d, lasxXX, (0b0111011010011110011101, 0, 5, 0), 0, 0, 65, 12036), //xvftintrz_lu_d
  INSTL(xvftintrz_w_d, lasxXXX, (0b01110101010010110, 0, 5, 10, 0), 0, 0, 362, 12051), //xvftintrz_w_d
  INSTL(xvftintrz_w_s, lasxXX, (0b0111011010011110010010, 0, 5, 0), 0, 0, 58, 12065), //xvftintrz_w_s
  INSTL(xvftintrz_wu_s, lasxXX, (0b0111011010011110011100, 0, 5, 0), 0, 0, 64, 12079), //xvftintrz_wu_s
  INSTL(xvftintrzh_l_s, lasxXX, (0b0111011010011110100111, 0, 5, 0), 0, 0, 73, 12094), //xvftintrzh_l_s
  INSTL(xvftintrzl_l_s, lasxXX, (0b0111011010011110100110, 0, 5, 0), 0, 0, 72, 12109), //xvftintrzl_l_s
  INSTL(xvhaddw_d_w, lasxXXX, (0b01110100010101010, 0, 5, 10, 0), 0, 0, 86, 12124), //xvhaddw_d_w
  INSTL(xvhaddw_du_wu, lasxXXX, (0b01110100010110010, 0, 5, 10, 0), 0, 0, 94, 12136), //xvhaddw_du_wu
  INSTL(xvhaddw_h_b, lasxXXX, (0b01110100010101000, 0, 5, 10, 0), 0, 0, 84, 12150), //xvhaddw_h_b
  INSTL(xvhaddw_hu_bu, lasxXXX, (0b01110100010110000, 0, 5, 10, 0), 0, 0, 92, 12162), //xvhaddw_hu_bu
  INSTL(xvhaddw_q_d, lasxXXX, (0b01110100010101011, 0, 5, 10, 0), 0, 0, 87, 12176), //xvhaddw_q_d
  INSTL(xvhaddw_qu_du, lasxXXX, (0b01110100010110011, 0, 5, 10, 0), 0, 0, 95, 12188), //xvhaddw_qu_du
  INSTL(xvhaddw_w_h, lasxXXX, (0b01110100010101001, 0, 5, 10, 0), 0, 0, 85, 12202), //xvhaddw_w_h
  INSTL(xvhaddw_wu_hu, lasxXXX, (0b01110100010110001, 0, 5, 10, 0), 0, 0, 93, 12214), //xvhaddw_wu_hu
  INSTL(xvhseli_d, lasxXXI, (0b01110110100111111, 0, 5, 10, 0), 0, 0, 48, 12228), //xvhseli_d
  INSTL(xvhsubw_d_w, lasxXXX, (0b01110100010101110, 0, 5, 10, 0), 0, 0, 90, 12238), //xvhsubw_d_w
  INSTL(xvhsubw_du_wu, lasxXXX, (0b01110100010110110, 0, 5, 10, 0), 0, 0, 98, 12250), //xvhsubw_du_wu
  INSTL(xvhsubw_h_b, lasxXXX, (0b01110100010101100, 0, 5, 10, 0), 0, 0, 88, 12264), //xvhsubw_h_b
  INSTL(xvhsubw_hu_bu, lasxXXX, (0b01110100010110100, 0, 5, 10, 0), 0, 0, 96, 12276), //xvhsubw_hu_bu
  INSTL(xvhsubw_q_d, lasxXXX, (0b01110100010101111, 0, 5, 10, 0), 0, 0, 91, 12290), //xvhsubw_q_d
  INSTL(xvhsubw_qu_du, lasxXXX, (0b01110100010110111, 0, 5, 10, 0), 0, 0, 99, 12302), //xvhsubw_qu_du
  INSTL(xvhsubw_w_h, lasxXXX, (0b01110100010101101, 0, 5, 10, 0), 0, 0, 89, 12316), //xvhsubw_w_h
  INSTL(xvhsubw_wu_hu, lasxXXX, (0b01110100010110101, 0, 5, 10, 0), 0, 0, 97, 12328), //xvhsubw_wu_hu
  INSTL(xvilvh_b, lasxXXX, (0b01110101000111000, 0, 5, 10, 0), 0, 0, 312, 12342), //xvilvh_b
  INSTL(xvilvh_d, lasxXXX, (0b01110101000111011, 0, 5, 10, 0), 0, 0, 315, 12351), //xvilvh_d
  INSTL(xvilvh_h, lasxXXX, (0b01110101000111001, 0, 5, 10, 0), 0, 0, 313, 12360), //xvilvh_h
  INSTL(xvilvh_w, lasxXXX, (0b01110101000111010, 0, 5, 10, 0), 0, 0, 314, 12369), //xvilvh_w
  INSTL(xvilvl_b, lasxXXX, (0b01110101000110100, 0, 5, 10, 0), 0, 0, 308, 12378), //xvilvl_b
  INSTL(xvilvl_d, lasxXXX, (0b01110101000110111, 0, 5, 10, 0), 0, 0, 311, 12387), //xvilvl_d
  INSTL(xvilvl_h, lasxXXX, (0b01110101000110101, 0, 5, 10, 0), 0, 0, 309, 12396), //xvilvl_h
  INSTL(xvilvl_w, lasxXXX, (0b01110101000110110, 0, 5, 10, 0), 0, 0, 310, 12405), //xvilvl_w
  INSTL(xvinsgr2vr_d, lasxXRI, (0b01110110111010111110, 0, kWX, 5, 10, 0), 0, 0, 1, 12414), //xvinsgr2vr_d
  INSTL(xvinsgr2vr_w, lasxXRI, (0b0111011011101011110, 0, kWX, 5, 10, 0), 0, 0, 0, 12427), //xvinsgr2vr_w
  INSTL(xvinsve0_d, lasxXXI, (0b01110110111111111110, 0, 5, 10, 0), 0, 0, 66, 12440), //xvinsve0_d
  INSTL(xvinsve0_w, lasxXXI, (0b0111011011111111110, 0, 5, 10, 0), 0, 0, 65, 12451), //xvinsve0_w
  INSTL(xvld,       SimdLdst, (0b0010110010,       22, 12), 0, 0, 0, 12462), //xvld
  INSTL(xvldi, lasxXI, (0b01110111111000, 0, 5, 0), 0, 0, 0, 12467), //xvldi
  INSTL(xvldrepl_b, SimdLdst, (0b0011001010,       22, 12), 0, 0, 5, 12473), //xvldrepl_b
  INSTL(xvldrepl_d, SimdLdst, (0b0011001000010,    19, 9 ), 0, 0, 2, 12484), //xvldrepl_d
  INSTL(xvldrepl_h, SimdLdst, (0b00110010010,      21, 11), 0, 0, 4, 12495), //xvldrepl_h
  INSTL(xvldrepl_w, SimdLdst, (0b001100100010,     20, 10), 0, 0, 3, 12506), //xvldrepl_w
  INSTL(xvldx,      SimdLdst, (0b00111000010010000,15, 0 ), 0, 0, 1, 12517), //xvldx
  INSTL(xvmadd_b, lasxXXX, (0b01110100101010000, 0, 5, 10, 0), 0, 0, 180, 12523), //xvmadd_b
  INSTL(xvmadd_d, lasxXXX, (0b01110100101010011, 0, 5, 10, 0), 0, 0, 183, 12532), //xvmadd_d
  INSTL(xvmadd_h, lasxXXX, (0b01110100101010001, 0, 5, 10, 0), 0, 0, 181, 12541), //xvmadd_h
  INSTL(xvmadd_w, lasxXXX, (0b01110100101010010, 0, 5, 10, 0), 0, 0, 182, 12550), //xvmadd_w
  INSTL(xvmaddwev_d_w, lasxXXX, (0b01110100101011010, 0, 5, 10, 0), 0, 0, 190, 12559), //xvmaddwev_d_w
  INSTL(xvmaddwev_d_wu, lasxXXX, (0b01110100101101010, 0, 5, 10, 0), 0, 0, 198, 12573), //xvmaddwev_d_wu
  INSTL(xvmaddwev_d_wu_w, lasxXXX, (0b01110100101111010, 0, 5, 10, 0), 0, 0, 206, 12588), //xvmaddwev_d_wu_w
  INSTL(xvmaddwev_h_b, lasxXXX, (0b01110100101011000, 0, 5, 10, 0), 0, 0, 188, 12605), //xvmaddwev_h_b
  INSTL(xvmaddwev_h_bu, lasxXXX, (0b01110100101101000, 0, 5, 10, 0), 0, 0, 196, 12619), //xvmaddwev_h_bu
  INSTL(xvmaddwev_h_bu_b, lasxXXX, (0b01110100101111000, 0, 5, 10, 0), 0, 0, 204, 12634), //xvmaddwev_h_bu_b
  INSTL(xvmaddwev_q_d, lasxXXX, (0b01110100101011011, 0, 5, 10, 0), 0, 0, 191, 12651), //xvmaddwev_q_d
  INSTL(xvmaddwev_q_du, lasxXXX, (0b01110100101101011, 0, 5, 10, 0), 0, 0, 199, 12665), //xvmaddwev_q_du
  INSTL(xvmaddwev_q_du_d, lasxXXX, (0b01110100101111011, 0, 5, 10, 0), 0, 0, 207, 12680), //xvmaddwev_q_du_d
  INSTL(xvmaddwev_w_h, lasxXXX, (0b01110100101011001, 0, 5, 10, 0), 0, 0, 189, 12697), //xvmaddwev_w_h
  INSTL(xvmaddwev_w_hu, lasxXXX, (0b01110100101101001, 0, 5, 10, 0), 0, 0, 197, 12711), //xvmaddwev_w_hu
  INSTL(xvmaddwev_w_hu_h, lasxXXX, (0b01110100101111001, 0, 5, 10, 0), 0, 0, 205, 12726), //xvmaddwev_w_hu_h
  INSTL(xvmaddwod_d_w, lasxXXX, (0b01110100101011110, 0, 5, 10, 0), 0, 0, 194, 12743), //xvmaddwod_d_w
  INSTL(xvmaddwod_d_wu, lasxXXX, (0b01110100101101110, 0, 5, 10, 0), 0, 0, 202, 12757), //xvmaddwod_d_wu
  INSTL(xvmaddwod_d_wu_w, lasxXXX, (0b01110100101111110, 0, 5, 10, 0), 0, 0, 210, 12772), //xvmaddwod_d_wu_w
  INSTL(xvmaddwod_h_b, lasxXXX, (0b01110100101011100, 0, 5, 10, 0), 0, 0, 192, 12789), //xvmaddwod_h_b
  INSTL(xvmaddwod_h_bu, lasxXXX, (0b01110100101101100, 0, 5, 10, 0), 0, 0, 200, 12803), //xvmaddwod_h_bu
  INSTL(xvmaddwod_h_bu_b, lasxXXX, (0b01110100101111100, 0, 5, 10, 0), 0, 0, 208, 12818), //xvmaddwod_h_bu_b
  INSTL(xvmaddwod_q_d, lasxXXX, (0b01110100101011111, 0, 5, 10, 0), 0, 0, 195, 12835), //xvmaddwod_q_d
  INSTL(xvmaddwod_q_du, lasxXXX, (0b01110100101101111, 0, 5, 10, 0), 0, 0, 203, 12849), //xvmaddwod_q_du
  INSTL(xvmaddwod_q_du_d, lasxXXX, (0b01110100101111111, 0, 5, 10, 0), 0, 0, 211, 12864), //xvmaddwod_q_du_d
  INSTL(xvmaddwod_w_h, lasxXXX, (0b01110100101011101, 0, 5, 10, 0), 0, 0, 193, 12881), //xvmaddwod_w_h
  INSTL(xvmaddwod_w_hu, lasxXXX, (0b01110100101101101, 0, 5, 10, 0), 0, 0, 201, 12895), //xvmaddwod_w_hu
  INSTL(xvmaddwod_w_hu_h, lasxXXX, (0b01110100101111101, 0, 5, 10, 0), 0, 0, 209, 12910), //xvmaddwod_w_hu_h
  INSTL(xvmax_b, lasxXXX, (0b01110100011100000, 0, 5, 10, 0), 0, 0, 128, 12927), //xvmax_b
  INSTL(xvmax_bu, lasxXXX, (0b01110100011101000, 0, 5, 10, 0), 0, 0, 136, 12935), //xvmax_bu
  INSTL(xvmax_d, lasxXXX, (0b01110100011100011, 0, 5, 10, 0), 0, 0, 131, 12944), //xvmax_d
  INSTL(xvmax_du, lasxXXX, (0b01110100011101011, 0, 5, 10, 0), 0, 0, 139, 12952), //xvmax_du
  INSTL(xvmax_h, lasxXXX, (0b01110100011100001, 0, 5, 10, 0), 0, 0, 129, 12961), //xvmax_h
  INSTL(xvmax_hu, lasxXXX, (0b01110100011101001, 0, 5, 10, 0), 0, 0, 137, 12969), //xvmax_hu
  INSTL(xvmax_w, lasxXXX, (0b01110100011100010, 0, 5, 10, 0), 0, 0, 130, 12978), //xvmax_w
  INSTL(xvmax_wu, lasxXXX, (0b01110100011101010, 0, 5, 10, 0), 0, 0, 138, 12986), //xvmax_wu
  INSTL(xvmaxi_b, lasxXXI, (0b01110110100100000, 0, 5, 10, 0), 0, 0, 30, 12995), //xvmaxi_b
  INSTL(xvmaxi_bu, lasxXXI, (0b01110110100101000, 0, 5, 10, 0), 0, 0, 38, 13004), //xvmaxi_bu
  INSTL(xvmaxi_d, lasxXXI, (0b01110110100100011, 0, 5, 10, 0), 0, 0, 33, 13014), //xvmaxi_d
  INSTL(xvmaxi_du, lasxXXI, (0b01110110100101011, 0, 5, 10, 0), 0, 0, 41, 13023), //xvmaxi_du
  INSTL(xvmaxi_h, lasxXXI, (0b01110110100100001, 0, 5, 10, 0), 0, 0, 31, 13033), //xvmaxi_h
  INSTL(xvmaxi_hu, lasxXXI, (0b01110110100101001, 0, 5, 10, 0), 0, 0, 39, 13042), //xvmaxi_hu
  INSTL(xvmaxi_w, lasxXXI, (0b01110110100100010, 0, 5, 10, 0), 0, 0, 32, 13052), //xvmaxi_w
  INSTL(xvmaxi_wu, lasxXXI, (0b01110110100101010, 0, 5, 10, 0), 0, 0, 40, 13061), //xvmaxi_wu
  INSTL(xvmepatmsk_v, lasxXII, (0b01110110100110111, 0, 5, 10, 0), 0, 0, 0, 13071), //xvmepatmsk_v
  INSTL(xvmin_b, lasxXXX, (0b01110100011100100, 0, 5, 10, 0), 0, 0, 132, 13084), //xvmin_b
  INSTL(xvmin_bu, lasxXXX, (0b01110100011101100, 0, 5, 10, 0), 0, 0, 140, 13092), //xvmin_bu
  INSTL(xvmin_d, lasxXXX, (0b01110100011100111, 0, 5, 10, 0), 0, 0, 135, 13101), //xvmin_d
  INSTL(xvmin_du, lasxXXX, (0b01110100011101111, 0, 5, 10, 0), 0, 0, 143, 13109), //xvmin_du
  INSTL(xvmin_h, lasxXXX, (0b01110100011100101, 0, 5, 10, 0), 0, 0, 133, 13118), //xvmin_h
  INSTL(xvmin_hu, lasxXXX, (0b01110100011101101, 0, 5, 10, 0), 0, 0, 141, 13126), //xvmin_hu
  INSTL(xvmin_w, lasxXXX, (0b01110100011100110, 0, 5, 10, 0), 0, 0, 134, 13135), //xvmin_w
  INSTL(xvmin_wu, lasxXXX, (0b01110100011101110, 0, 5, 10, 0), 0, 0, 142, 13143), //xvmin_wu
  INSTL(xvmini_b, lasxXXI, (0b01110110100100100, 0, 5, 10, 0), 0, 0, 34, 13152), //xvmini_b
  INSTL(xvmini_bu, lasxXXI, (0b01110110100101100, 0, 5, 10, 0), 0, 0, 42, 13161), //xvmini_bu
  INSTL(xvmini_d, lasxXXI, (0b01110110100100111, 0, 5, 10, 0), 0, 0, 37, 13171), //xvmini_d
  INSTL(xvmini_du, lasxXXI, (0b01110110100101111, 0, 5, 10, 0), 0, 0, 45, 13180), //xvmini_du
  INSTL(xvmini_h, lasxXXI, (0b01110110100100101, 0, 5, 10, 0), 0, 0, 35, 13190), //xvmini_h
  INSTL(xvmini_hu, lasxXXI, (0b01110110100101101, 0, 5, 10, 0), 0, 0, 43, 13199), //xvmini_hu
  INSTL(xvmini_w, lasxXXI, (0b01110110100100110, 0, 5, 10, 0), 0, 0, 36, 13209), //xvmini_w
  INSTL(xvmini_wu, lasxXXI, (0b01110110100101110, 0, 5, 10, 0), 0, 0, 44, 13218), //xvmini_wu
  INSTL(xvmod_b, lasxXXX, (0b01110100111000100, 0, 5, 10, 0), 0, 0, 216, 13228), //xvmod_b
  INSTL(xvmod_bu, lasxXXX, (0b01110100111001100, 0, 5, 10, 0), 0, 0, 224, 13236), //xvmod_bu
  INSTL(xvmod_d, lasxXXX, (0b01110100111000111, 0, 5, 10, 0), 0, 0, 219, 13245), //xvmod_d
  INSTL(xvmod_du, lasxXXX, (0b01110100111001111, 0, 5, 10, 0), 0, 0, 227, 13253), //xvmod_du
  INSTL(xvmod_h, lasxXXX, (0b01110100111000101, 0, 5, 10, 0), 0, 0, 217, 13262), //xvmod_h
  INSTL(xvmod_hu, lasxXXX, (0b01110100111001101, 0, 5, 10, 0), 0, 0, 225, 13270), //xvmod_hu
  INSTL(xvmod_w, lasxXXX, (0b01110100111000110, 0, 5, 10, 0), 0, 0, 218, 13279), //xvmod_w
  INSTL(xvmod_wu, lasxXXX, (0b01110100111001110, 0, 5, 10, 0), 0, 0, 226, 13287), //xvmod_wu
  INSTL(xvmskgez_b, lasxXX, (0b0111011010011100010100, 0, 5, 0), 0, 0, 20, 13296), //xvmskgez_b
  INSTL(xvmskltz_b, lasxXX, (0b0111011010011100010000, 0, 5, 0), 0, 0, 16, 13307), //xvmskltz_b
  INSTL(xvmskltz_d, lasxXX, (0b0111011010011100010011, 0, 5, 0), 0, 0, 19, 13318), //xvmskltz_d
  INSTL(xvmskltz_h, lasxXX, (0b0111011010011100010001, 0, 5, 0), 0, 0, 17, 13329), //xvmskltz_h
  INSTL(xvmskltz_w, lasxXX, (0b0111011010011100010010, 0, 5, 0), 0, 0, 18, 13340), //xvmskltz_w
  INSTL(xvmsknz_b, lasxXX, (0b0111011010011100011000, 0, 5, 0), 0, 0, 21, 13351), //xvmsknz_b
  INSTL(xvmsub_b, lasxXXX, (0b01110100101010100, 0, 5, 10, 0), 0, 0, 184, 13361), //xvmsub_b
  INSTL(xvmsub_d, lasxXXX, (0b01110100101010111, 0, 5, 10, 0), 0, 0, 187, 13370), //xvmsub_d
  INSTL(xvmsub_h, lasxXXX, (0b01110100101010101, 0, 5, 10, 0), 0, 0, 185, 13379), //xvmsub_h
  INSTL(xvmsub_w, lasxXXX, (0b01110100101010110, 0, 5, 10, 0), 0, 0, 186, 13388), //xvmsub_w
  INSTL(xvmuh_b, lasxXXX, (0b01110100100001100, 0, 5, 10, 0), 0, 0, 148, 13397), //xvmuh_b
  INSTL(xvmuh_bu, lasxXXX, (0b01110100100010000, 0, 5, 10, 0), 0, 0, 152, 13405), //xvmuh_bu
  INSTL(xvmuh_d, lasxXXX, (0b01110100100001111, 0, 5, 10, 0), 0, 0, 151, 13414), //xvmuh_d
  INSTL(xvmuh_du, lasxXXX, (0b01110100100010011, 0, 5, 10, 0), 0, 0, 155, 13422), //xvmuh_du
  INSTL(xvmuh_h, lasxXXX, (0b01110100100001101, 0, 5, 10, 0), 0, 0, 149, 13431), //xvmuh_h
  INSTL(xvmuh_hu, lasxXXX, (0b01110100100010001, 0, 5, 10, 0), 0, 0, 153, 13439), //xvmuh_hu
  INSTL(xvmuh_w, lasxXXX, (0b01110100100001110, 0, 5, 10, 0), 0, 0, 150, 13448), //xvmuh_w
  INSTL(xvmuh_wu, lasxXXX, (0b01110100100010010, 0, 5, 10, 0), 0, 0, 154, 13456), //xvmuh_wu
  INSTL(xvmul_b, lasxXXX, (0b01110100100001000, 0, 5, 10, 0), 0, 0, 144, 13465), //xvmul_b
  INSTL(xvmul_d, lasxXXX, (0b01110100100001011, 0, 5, 10, 0), 0, 0, 147, 13473), //xvmul_d
  INSTL(xvmul_h, lasxXXX, (0b01110100100001001, 0, 5, 10, 0), 0, 0, 145, 13481), //xvmul_h
  INSTL(xvmul_w, lasxXXX, (0b01110100100001010, 0, 5, 10, 0), 0, 0, 146, 13489), //xvmul_w
  INSTL(xvmulwev_d_w, lasxXXX, (0b01110100100100010, 0, 5, 10, 0), 0, 0, 158, 13497), //xvmulwev_d_w
  INSTL(xvmulwev_d_wu, lasxXXX, (0b01110100100110010, 0, 5, 10, 0), 0, 0, 166, 13510), //xvmulwev_d_wu
  INSTL(xvmulwev_d_wu_w, lasxXXX, (0b01110100101000010, 0, 5, 10, 0), 0, 0, 174, 13524), //xvmulwev_d_wu_w
  INSTL(xvmulwev_h_b, lasxXXX, (0b01110100100100000, 0, 5, 10, 0), 0, 0, 156, 13540), //xvmulwev_h_b
  INSTL(xvmulwev_h_bu, lasxXXX, (0b01110100100110000, 0, 5, 10, 0), 0, 0, 164, 13553), //xvmulwev_h_bu
  INSTL(xvmulwev_h_bu_b, lasxXXX, (0b01110100101000000, 0, 5, 10, 0), 0, 0, 172, 13567), //xvmulwev_h_bu_b
  INSTL(xvmulwev_q_d, lasxXXX, (0b01110100100100011, 0, 5, 10, 0), 0, 0, 159, 13583), //xvmulwev_q_d
  INSTL(xvmulwev_q_du, lasxXXX, (0b01110100100110011, 0, 5, 10, 0), 0, 0, 167, 13596), //xvmulwev_q_du
  INSTL(xvmulwev_q_du_d, lasxXXX, (0b01110100101000011, 0, 5, 10, 0), 0, 0, 175, 13610), //xvmulwev_q_du_d
  INSTL(xvmulwev_w_h, lasxXXX, (0b01110100100100001, 0, 5, 10, 0), 0, 0, 157, 13626), //xvmulwev_w_h
  INSTL(xvmulwev_w_hu, lasxXXX, (0b01110100100110001, 0, 5, 10, 0), 0, 0, 165, 13639), //xvmulwev_w_hu
  INSTL(xvmulwev_w_hu_h, lasxXXX, (0b01110100101000001, 0, 5, 10, 0), 0, 0, 173, 13653), //xvmulwev_w_hu_h
  INSTL(xvmulwod_d_w, lasxXXX, (0b01110100100100110, 0, 5, 10, 0), 0, 0, 162, 13669), //xvmulwod_d_w
  INSTL(xvmulwod_d_wu, lasxXXX, (0b01110100100110110, 0, 5, 10, 0), 0, 0, 170, 13682), //xvmulwod_d_wu
  INSTL(xvmulwod_d_wu_w, lasxXXX, (0b01110100101000110, 0, 5, 10, 0), 0, 0, 178, 13696), //xvmulwod_d_wu_w
  INSTL(xvmulwod_h_b, lasxXXX, (0b01110100100100100, 0, 5, 10, 0), 0, 0, 160, 13712), //xvmulwod_h_b
  INSTL(xvmulwod_h_bu, lasxXXX, (0b01110100100110100, 0, 5, 10, 0), 0, 0, 168, 13725), //xvmulwod_h_bu
  INSTL(xvmulwod_h_bu_b, lasxXXX, (0b01110100101000100, 0, 5, 10, 0), 0, 0, 176, 13739), //xvmulwod_h_bu_b
  INSTL(xvmulwod_q_d, lasxXXX, (0b01110100100100111, 0, 5, 10, 0), 0, 0, 163, 13755), //xvmulwod_q_d
  INSTL(xvmulwod_q_du, lasxXXX, (0b01110100100110111, 0, 5, 10, 0), 0, 0, 171, 13768), //xvmulwod_q_du
  INSTL(xvmulwod_q_du_d, lasxXXX, (0b01110100101000111, 0, 5, 10, 0), 0, 0, 179, 13782), //xvmulwod_q_du_d
  INSTL(xvmulwod_w_h, lasxXXX, (0b01110100100100101, 0, 5, 10, 0), 0, 0, 161, 13798), //xvmulwod_w_h
  INSTL(xvmulwod_w_hu, lasxXXX, (0b01110100100110101, 0, 5, 10, 0), 0, 0, 169, 13811), //xvmulwod_w_hu
  INSTL(xvmulwod_w_hu_h, lasxXXX, (0b01110100101000101, 0, 5, 10, 0), 0, 0, 177, 13825), //xvmulwod_w_hu_h
  INSTL(xvneg_b, lasxXX, (0b0111011010011100001100, 0, 5, 0), 0, 0, 12, 13841), //xvneg_b
  INSTL(xvneg_d, lasxXX, (0b0111011010011100001111, 0, 5, 0), 0, 0, 15, 13849), //xvneg_d
  INSTL(xvneg_h, lasxXX, (0b0111011010011100001101, 0, 5, 0), 0, 0, 13, 13857), //xvneg_h
  INSTL(xvneg_w, lasxXX, (0b0111011010011100001110, 0, 5, 0), 0, 0, 14, 13865), //xvneg_w
  INSTL(xvnor_v, lasxXXX, (0b01110101001001111, 0, 5, 10, 0), 0, 0, 327, 13873), //xvnor_v
  INSTL(xvnori_b, lasxXXI, (0b01110111110111, 0, 5, 10, 0), 0, 0, 167, 13881), //xvnori_b
  INSTL(xvor_v, lasxXXX, (0b01110101001001101, 0, 5, 10, 0), 0, 0, 325, 13890), //xvor_v
  INSTL(xvori_b, lasxXXI, (0b01110111110101, 0, 5, 10, 0), 0, 0, 165, 13897), //xvori_b
  INSTL(xvorn_v, lasxXXX, (0b01110101001010001, 0, 5, 10, 0), 0, 0, 329, 13905), //xvorn_v
  INSTL(xvpackev_b, lasxXXX, (0b01110101000101100, 0, 5, 10, 0), 0, 0, 300, 13913), //xvpackev_b
  INSTL(xvpackev_d, lasxXXX, (0b01110101000101111, 0, 5, 10, 0), 0, 0, 303, 13924), //xvpackev_d
  INSTL(xvpackev_h, lasxXXX, (0b01110101000101101, 0, 5, 10, 0), 0, 0, 301, 13935), //xvpackev_h
  INSTL(xvpackev_w, lasxXXX, (0b01110101000101110, 0, 5, 10, 0), 0, 0, 302, 13946), //xvpackev_w
  INSTL(xvpackod_b, lasxXXX, (0b01110101000110000, 0, 5, 10, 0), 0, 0, 304, 13957), //xvpackod_b
  INSTL(xvpackod_d, lasxXXX, (0b01110101000110011, 0, 5, 10, 0), 0, 0, 307, 13968), //xvpackod_d
  INSTL(xvpackod_h, lasxXXX, (0b01110101000110001, 0, 5, 10, 0), 0, 0, 305, 13979), //xvpackod_h
  INSTL(xvpackod_w, lasxXXX, (0b01110101000110010, 0, 5, 10, 0), 0, 0, 306, 13990), //xvpackod_w
  INSTL(xvpcnt_b, lasxXX, (0b0111011010011100001000, 0, 5, 0), 0, 0, 8, 14001), //xvpcnt_b
  INSTL(xvpcnt_d, lasxXX, (0b0111011010011100001011, 0, 5, 0), 0, 0, 11, 14010), //xvpcnt_d
  INSTL(xvpcnt_h, lasxXX, (0b0111011010011100001001, 0, 5, 0), 0, 0, 9, 14019), //xvpcnt_h
  INSTL(xvpcnt_w, lasxXX, (0b0111011010011100001010, 0, 5, 0), 0, 0, 10, 14028), //xvpcnt_w
  INSTL(xvperm_w, lasxXXX, (0b01110101011111010, 0, 5, 10, 0), 0, 0, 367, 14037), //xvperm_w
  INSTL(xvpermi_d, lasxXXI, (0b01110111111010, 0, 5, 10, 0), 0, 0, 169, 14046), //xvpermi_d
  INSTL(xvpermi_q, lasxXXI, (0b01110111111011, 0, 5, 10, 0), 0, 0, 170, 14056), //xvpermi_q
  INSTL(xvpermi_w, lasxXXI, (0b01110111111001, 0, 5, 10, 0), 0, 0, 168, 14066), //xvpermi_w
  INSTL(xvpickev_b, lasxXXX, (0b01110101000111100, 0, 5, 10, 0), 0, 0, 316, 14076), //xvpickev_b
  INSTL(xvpickev_d, lasxXXX, (0b01110101000111111, 0, 5, 10, 0), 0, 0, 319, 14087), //xvpickev_d
  INSTL(xvpickev_h, lasxXXX, (0b01110101000111101, 0, 5, 10, 0), 0, 0, 317, 14098), //xvpickev_h
  INSTL(xvpickev_w, lasxXXX, (0b01110101000111110, 0, 5, 10, 0), 0, 0, 318, 14109), //xvpickev_w
  INSTL(xvpickod_b, lasxXXX, (0b01110101001000000, 0, 5, 10, 0), 0, 0, 320, 14120), //xvpickod_b
  INSTL(xvpickod_d, lasxXXX, (0b01110101001000011, 0, 5, 10, 0), 0, 0, 323, 14131), //xvpickod_d
  INSTL(xvpickod_h, lasxXXX, (0b01110101001000001, 0, 5, 10, 0), 0, 0, 321, 14142), //xvpickod_h
  INSTL(xvpickod_w, lasxXXX, (0b01110101001000010, 0, 5, 10, 0), 0, 0, 322, 14153), //xvpickod_w
  INSTL(xvpickve2gr_d, lasxRXI, (0b01110110111011111110, kWX, 0, 5, 10, 0), 0, 0, 1, 14164), //xvpickve2gr_d
  INSTL(xvpickve2gr_du, lasxRXI, (0b01110110111100111110, kWX, 0, 5, 10, 0), 0, 0, 3, 14178), //xvpickve2gr_du
  INSTL(xvpickve2gr_w, lasxRXI, (0b0111011011101111110, kWX, 0, 5, 10, 0), 0, 0, 0, 14193), //xvpickve2gr_w
  INSTL(xvpickve2gr_wu, lasxRXI, (0b0111011011110011110, kWX, 0, 5, 10, 0), 0, 0, 2, 14207), //xvpickve2gr_wu
  INSTL(xvpickve_d, lasxXXI, (0b01110111000000111110, 0, 5, 10, 0), 0, 0, 68, 14222), //xvpickve_d
  INSTL(xvpickve_w, lasxXXI, (0b0111011100000011110, 0, 5, 10, 0), 0, 0, 67, 14233), //xvpickve_w
  INSTL(xvrepl128vei_b, lasxXXI, (0b011101101111011110, 0, 5, 10, 0), 0, 0, 61, 14244), //xvrepl128vei_b
  INSTL(xvrepl128vei_d, lasxXXI, (0b011101101111011111110, 0, 5, 10, 0), 0, 0, 64, 14259), //xvrepl128vei_d
  INSTL(xvrepl128vei_h, lasxXXI, (0b0111011011110111110, 0, 5, 10, 0), 0, 0, 62, 14274), //xvrepl128vei_h
  INSTL(xvrepl128vei_w, lasxXXI, (0b01110110111101111110, 0, 5, 10, 0), 0, 0, 63, 14289), //xvrepl128vei_w
  INSTL(xvreplgr2vr_b, lasxXR, (0b0111011010011111000000, 0, 5, 0), 0, 0, 0, 14304), //xvreplgr2vr_b
  INSTL(xvreplgr2vr_d, lasxXR, (0b0111011010011111000011, 0, 5, 0), 0, 0, 3, 14318), //xvreplgr2vr_d
  INSTL(xvreplgr2vr_h, lasxXR, (0b0111011010011111000001, 0, 5, 0), 0, 0, 1, 14332), //xvreplgr2vr_h
  INSTL(xvreplgr2vr_w, lasxXR, (0b0111011010011111000010, 0, 5, 0), 0, 0, 2, 14346), //xvreplgr2vr_w
  INSTL(xvreplve0_b, lasxXX, (0b0111011100000111000000, 0, 5, 0), 0, 0, 96, 14360), //xvreplve0_b
  INSTL(xvreplve0_d, lasxXX, (0b0111011100000111111000, 0, 5, 0), 0, 0, 99, 14372), //xvreplve0_d
  INSTL(xvreplve0_h, lasxXX, (0b0111011100000111100000, 0, 5, 0), 0, 0, 97, 14384), //xvreplve0_h
  INSTL(xvreplve0_q, lasxXX, (0b0111011100000111111100, 0, 5, 0), 0, 0, 100, 14396), //xvreplve0_q
  INSTL(xvreplve0_w, lasxXX, (0b0111011100000111110000, 0, 5, 0), 0, 0, 98, 14408), //xvreplve0_w
  INSTL(xvreplve_b, lasxXXR, (0b01110101001000100, 0, 5, kWX, 10, 0), 0, 0, 0, 14420), //xvreplve_b
  INSTL(xvreplve_d, lasxXXR, (0b01110101001000111, 0, 5, kWX, 10, 0), 0, 0, 3, 14431), //xvreplve_d
  INSTL(xvreplve_h, lasxXXR, (0b01110101001000101, 0, 5, kWX, 10, 0), 0, 0, 1, 14442), //xvreplve_h
  INSTL(xvreplve_w, lasxXXR, (0b01110101001000110, 0, 5, kWX, 10, 0), 0, 0, 2, 14453), //xvreplve_w
  INSTL(xvrotr_b, lasxXXX, (0b01110100111011100, 0, 5, 10, 0), 0, 0, 240, 14464), //xvrotr_b
  INSTL(xvrotr_d, lasxXXX, (0b01110100111011111, 0, 5, 10, 0), 0, 0, 243, 14473), //xvrotr_d
  INSTL(xvrotr_h, lasxXXX, (0b01110100111011101, 0, 5, 10, 0), 0, 0, 241, 14482), //xvrotr_h
  INSTL(xvrotr_w, lasxXXX, (0b01110100111011110, 0, 5, 10, 0), 0, 0, 242, 14491), //xvrotr_w
  INSTL(xvrotri_b, lasxXXI, (0b0111011010100000001, 0, 5, 10, 0), 0, 0, 49, 14500), //xvrotri_b
  INSTL(xvrotri_d, lasxXXI, (0b0111011010100001, 0, 5, 10, 0), 0, 0, 52, 14510), //xvrotri_d
  INSTL(xvrotri_h, lasxXXI, (0b011101101010000001, 0, 5, 10, 0), 0, 0, 50, 14520), //xvrotri_h
  INSTL(xvrotri_w, lasxXXI, (0b01110110101000001, 0, 5, 10, 0), 0, 0, 51, 14530), //xvrotri_w
  INSTL(xvsadd_b, lasxXXX, (0b01110100010001100, 0, 5, 10, 0), 0, 0, 68, 14540), //xvsadd_b
  INSTL(xvsadd_bu, lasxXXX, (0b01110100010010100, 0, 5, 10, 0), 0, 0, 76, 14549), //xvsadd_bu
  INSTL(xvsadd_d, lasxXXX, (0b01110100010001111, 0, 5, 10, 0), 0, 0, 71, 14559), //xvsadd_d
  INSTL(xvsadd_du, lasxXXX, (0b01110100010010111, 0, 5, 10, 0), 0, 0, 79, 14568), //xvsadd_du
  INSTL(xvsadd_h, lasxXXX, (0b01110100010001101, 0, 5, 10, 0), 0, 0, 69, 14578), //xvsadd_h
  INSTL(xvsadd_hu, lasxXXX, (0b01110100010010101, 0, 5, 10, 0), 0, 0, 77, 14587), //xvsadd_hu
  INSTL(xvsadd_w, lasxXXX, (0b01110100010001110, 0, 5, 10, 0), 0, 0, 70, 14597), //xvsadd_w
  INSTL(xvsadd_wu, lasxXXX, (0b01110100010010110, 0, 5, 10, 0), 0, 0, 78, 14606), //xvsadd_wu
  INSTL(xvsat_b, lasxXXI, (0b0111011100100100001, 0, 5, 10, 0), 0, 0, 87, 14616), //xvsat_b
  INSTL(xvsat_bu, lasxXXI, (0b0111011100101000001, 0, 5, 10, 0), 0, 0, 91, 14624), //xvsat_bu
  INSTL(xvsat_d, lasxXXI, (0b0111011100100101, 0, 5, 10, 0), 0, 0, 90, 14633), //xvsat_d
  INSTL(xvsat_du, lasxXXI, (0b0111011100101001, 0, 5, 10, 0), 0, 0, 94, 14641), //xvsat_du
  INSTL(xvsat_h, lasxXXI, (0b011101110010010001, 0, 5, 10, 0), 0, 0, 88, 14650), //xvsat_h
  INSTL(xvsat_hu, lasxXXI, (0b011101110010100001, 0, 5, 10, 0), 0, 0, 92, 14658), //xvsat_hu
  INSTL(xvsat_w, lasxXXI, (0b01110111001001001, 0, 5, 10, 0), 0, 0, 89, 14667), //xvsat_w
  INSTL(xvsat_wu, lasxXXI, (0b01110111001010001, 0, 5, 10, 0), 0, 0, 93, 14675), //xvsat_wu
  INSTL(xvseq_b, lasxXXX, (0b01110100000000000, 0, 5, 10, 0), 0, 0, 0, 14684), //xvseq_b
  INSTL(xvseq_d, lasxXXX, (0b01110100000000011, 0, 5, 10, 0), 0, 0, 3, 14692), //xvseq_d
  INSTL(xvseq_h, lasxXXX, (0b01110100000000001, 0, 5, 10, 0), 0, 0, 1, 14700), //xvseq_h
  INSTL(xvseq_w, lasxXXX, (0b01110100000000010, 0, 5, 10, 0), 0, 0, 2, 14708), //xvseq_w
  INSTL(xvseqi_b, lasxXXI, (0b01110110100000000, 0, 5, 10, 0), 0, 0, 0, 14716), //xvseqi_b
  INSTL(xvseqi_d, lasxXXI, (0b01110110100000011, 0, 5, 10, 0), 0, 0, 3, 14725), //xvseqi_d
  INSTL(xvseqi_h, lasxXXI, (0b01110110100000001, 0, 5, 10, 0), 0, 0, 1, 14734), //xvseqi_h
  INSTL(xvseqi_w, lasxXXI, (0b01110110100000010, 0, 5, 10, 0), 0, 0, 2, 14743), //xvseqi_w
  INSTL(xvsetallnez_b, lasxIX, (0b0111011010011100101100, 0, 5, 0), 0, 0, 6, 14752), //xvsetallnez_b
  INSTL(xvsetallnez_d, lasxIX, (0b0111011010011100101111, 0, 5, 0), 0, 0, 9, 14766), //xvsetallnez_d
  INSTL(xvsetallnez_h, lasxIX, (0b0111011010011100101101, 0, 5, 0), 0, 0, 7, 14780), //xvsetallnez_h
  INSTL(xvsetallnez_w, lasxIX, (0b0111011010011100101110, 0, 5, 0), 0, 0, 8, 14794), //xvsetallnez_w
  INSTL(xvsetanyeqz_b, lasxIX, (0b0111011010011100101000, 0, 5, 0), 0, 0, 2, 14808), //xvsetanyeqz_b
  INSTL(xvsetanyeqz_d, lasxIX, (0b0111011010011100101011, 0, 5, 0), 0, 0, 5, 14822), //xvsetanyeqz_d
  INSTL(xvsetanyeqz_h, lasxIX, (0b0111011010011100101001, 0, 5, 0), 0, 0, 3, 14836), //xvsetanyeqz_h
  INSTL(xvsetanyeqz_w, lasxIX, (0b0111011010011100101010, 0, 5, 0), 0, 0, 4, 14850), //xvsetanyeqz_w
  INSTL(xvseteqz_v, lasxIX, (0b0111011010011100100110, 0, 5, 0), 0, 0, 0, 14864), //xvseteqz_v
  INSTL(xvsetnez_v, lasxIX, (0b0111011010011100100111, 0, 5, 0), 0, 0, 1, 14875), //xvsetnez_v
  INSTL(xvshuf4i_b, lasxXXI, (0b01110111100100, 0, 5, 10, 0), 0, 0, 159, 14886), //xvshuf4i_b
  INSTL(xvshuf4i_d, lasxXXI, (0b01110111100111, 0, 5, 10, 0), 0, 0, 162, 14897), //xvshuf4i_d
  INSTL(xvshuf4i_h, lasxXXI, (0b01110111100101, 0, 5, 10, 0), 0, 0, 160, 14908), //xvshuf4i_h
  INSTL(xvshuf4i_w, lasxXXI, (0b01110111100110, 0, 5, 10, 0), 0, 0, 161, 14919), //xvshuf4i_w
  INSTL(xvshuf_b, lasxXXXX, (0b000011010110, 0, 5, 10, 15, 0), 0, 0, 9, 14930), //xvshuf_b
  INSTL(xvshuf_d, lasxXXX, (0b01110101011110111, 0, 5, 10, 0), 0, 0, 366, 14939), //xvshuf_d
  INSTL(xvshuf_h, lasxXXX, (0b01110101011110101, 0, 5, 10, 0), 0, 0, 364, 14948), //xvshuf_h
  INSTL(xvshuf_w, lasxXXX, (0b01110101011110110, 0, 5, 10, 0), 0, 0, 365, 14957), //xvshuf_w
  INSTL(xvsigncov_b, lasxXXX, (0b01110101001011100, 0, 5, 10, 0), 0, 0, 334, 14966), //xvsigncov_b
  INSTL(xvsigncov_d, lasxXXX, (0b01110101001011111, 0, 5, 10, 0), 0, 0, 337, 14978), //xvsigncov_d
  INSTL(xvsigncov_h, lasxXXX, (0b01110101001011101, 0, 5, 10, 0), 0, 0, 335, 14990), //xvsigncov_h
  INSTL(xvsigncov_w, lasxXXX, (0b01110101001011110, 0, 5, 10, 0), 0, 0, 336, 15002), //xvsigncov_w
  INSTL(xvsle_b, lasxXXX, (0b01110100000000100, 0, 5, 10, 0), 0, 0, 4, 15014), //xvsle_b
  INSTL(xvsle_bu, lasxXXX, (0b01110100000001000, 0, 5, 10, 0), 0, 0, 8, 15022), //xvsle_bu
  INSTL(xvsle_d, lasxXXX, (0b01110100000000111, 0, 5, 10, 0), 0, 0, 7, 15031), //xvsle_d
  INSTL(xvsle_du, lasxXXX, (0b01110100000001011, 0, 5, 10, 0), 0, 0, 11, 15039), //xvsle_du
  INSTL(xvsle_h, lasxXXX, (0b01110100000000101, 0, 5, 10, 0), 0, 0, 5, 15048), //xvsle_h
  INSTL(xvsle_hu, lasxXXX, (0b01110100000001001, 0, 5, 10, 0), 0, 0, 9, 15056), //xvsle_hu
  INSTL(xvsle_w, lasxXXX, (0b01110100000000110, 0, 5, 10, 0), 0, 0, 6, 15065), //xvsle_w
  INSTL(xvsle_wu, lasxXXX, (0b01110100000001010, 0, 5, 10, 0), 0, 0, 10, 15073), //xvsle_wu
  INSTL(xvslei_b, lasxXXI, (0b01110110100000100, 0, 5, 10, 0), 0, 0, 4, 15082), //xvslei_b
  INSTL(xvslei_bu, lasxXXI, (0b01110110100001000, 0, 5, 10, 0), 0, 0, 8, 15091), //xvslei_bu
  INSTL(xvslei_d, lasxXXI, (0b01110110100000111, 0, 5, 10, 0), 0, 0, 7, 15101), //xvslei_d
  INSTL(xvslei_du, lasxXXI, (0b01110110100001011, 0, 5, 10, 0), 0, 0, 11, 15110), //xvslei_du
  INSTL(xvslei_h, lasxXXI, (0b01110110100000101, 0, 5, 10, 0), 0, 0, 5, 15120), //xvslei_h
  INSTL(xvslei_hu, lasxXXI, (0b01110110100001001, 0, 5, 10, 0), 0, 0, 9, 15129), //xvslei_hu
  INSTL(xvslei_w, lasxXXI, (0b01110110100000110, 0, 5, 10, 0), 0, 0, 6, 15139), //xvslei_w
  INSTL(xvslei_wu, lasxXXI, (0b01110110100001010, 0, 5, 10, 0), 0, 0, 10, 15148), //xvslei_wu
  INSTL(xvsll_b, lasxXXX, (0b01110100111010000, 0, 5, 10, 0), 0, 0, 228, 15158), //xvsll_b
  INSTL(xvsll_d, lasxXXX, (0b01110100111010011, 0, 5, 10, 0), 0, 0, 231, 15166), //xvsll_d
  INSTL(xvsll_h, lasxXXX, (0b01110100111010001, 0, 5, 10, 0), 0, 0, 229, 15174), //xvsll_h
  INSTL(xvsll_w, lasxXXX, (0b01110100111010010, 0, 5, 10, 0), 0, 0, 230, 15182), //xvsll_w
  INSTL(xvslli_b, lasxXXI, (0b0111011100101100001, 0, 5, 10, 0), 0, 0, 95, 15190), //xvslli_b
  INSTL(xvslli_d, lasxXXI, (0b0111011100101101, 0, 5, 10, 0), 0, 0, 98, 15199), //xvslli_d
  INSTL(xvslli_h, lasxXXI, (0b011101110010110001, 0, 5, 10, 0), 0, 0, 96, 15208), //xvslli_h
  INSTL(xvslli_w, lasxXXI, (0b01110111001011001, 0, 5, 10, 0), 0, 0, 97, 15217), //xvslli_w
  INSTL(xvsllwil_d_w, lasxXXI, (0b01110111000010001, 0, 5, 10, 0), 0, 0, 71, 15226), //xvsllwil_d_w
  INSTL(xvsllwil_du_wu, lasxXXI, (0b01110111000011001, 0, 5, 10, 0), 0, 0, 74, 15239), //xvsllwil_du_wu
  INSTL(xvsllwil_h_b, lasxXXI, (0b0111011100001000001, 0, 5, 10, 0), 0, 0, 69, 15254), //xvsllwil_h_b
  INSTL(xvsllwil_hu_bu, lasxXXI, (0b0111011100001100001, 0, 5, 10, 0), 0, 0, 72, 15267), //xvsllwil_hu_bu
  INSTL(xvsllwil_w_h, lasxXXI, (0b011101110000100001, 0, 5, 10, 0), 0, 0, 70, 15282), //xvsllwil_w_h
  INSTL(xvsllwil_wu_hu, lasxXXI, (0b011101110000110001, 0, 5, 10, 0), 0, 0, 73, 15295), //xvsllwil_wu_hu
  INSTL(xvslt_b, lasxXXX, (0b01110100000001100, 0, 5, 10, 0), 0, 0, 12, 15310), //xvslt_b
  INSTL(xvslt_bu, lasxXXX, (0b01110100000010000, 0, 5, 10, 0), 0, 0, 16, 15318), //xvslt_bu
  INSTL(xvslt_d, lasxXXX, (0b01110100000001111, 0, 5, 10, 0), 0, 0, 15, 15327), //xvslt_d
  INSTL(xvslt_du, lasxXXX, (0b01110100000010011, 0, 5, 10, 0), 0, 0, 19, 15335), //xvslt_du
  INSTL(xvslt_h, lasxXXX, (0b01110100000001101, 0, 5, 10, 0), 0, 0, 13, 15344), //xvslt_h
  INSTL(xvslt_hu, lasxXXX, (0b01110100000010001, 0, 5, 10, 0), 0, 0, 17, 15352), //xvslt_hu
  INSTL(xvslt_w, lasxXXX, (0b01110100000001110, 0, 5, 10, 0), 0, 0, 14, 15361), //xvslt_w
  INSTL(xvslt_wu, lasxXXX, (0b01110100000010010, 0, 5, 10, 0), 0, 0, 18, 15369), //xvslt_wu
  INSTL(xvslti_b, lasxXXI, (0b01110110100001100, 0, 5, 10, 0), 0, 0, 12, 15378), //xvslti_b
  INSTL(xvslti_bu, lasxXXI, (0b01110110100010000, 0, 5, 10, 0), 0, 0, 16, 15387), //xvslti_bu
  INSTL(xvslti_d, lasxXXI, (0b01110110100001111, 0, 5, 10, 0), 0, 0, 15, 15397), //xvslti_d
  INSTL(xvslti_du, lasxXXI, (0b01110110100010011, 0, 5, 10, 0), 0, 0, 19, 15406), //xvslti_du
  INSTL(xvslti_h, lasxXXI, (0b01110110100001101, 0, 5, 10, 0), 0, 0, 13, 15416), //xvslti_h
  INSTL(xvslti_hu, lasxXXI, (0b01110110100010001, 0, 5, 10, 0), 0, 0, 17, 15425), //xvslti_hu
  INSTL(xvslti_w, lasxXXI, (0b01110110100001110, 0, 5, 10, 0), 0, 0, 14, 15435), //xvslti_w
  INSTL(xvslti_wu, lasxXXI, (0b01110110100010010, 0, 5, 10, 0), 0, 0, 18, 15444), //xvslti_wu
  INSTL(xvsra_b, lasxXXX, (0b01110100111011000, 0, 5, 10, 0), 0, 0, 236, 15454), //xvsra_b
  INSTL(xvsra_d, lasxXXX, (0b01110100111011011, 0, 5, 10, 0), 0, 0, 239, 15462), //xvsra_d
  INSTL(xvsra_h, lasxXXX, (0b01110100111011001, 0, 5, 10, 0), 0, 0, 237, 15470), //xvsra_h
  INSTL(xvsra_w, lasxXXX, (0b01110100111011010, 0, 5, 10, 0), 0, 0, 238, 15478), //xvsra_w
  INSTL(xvsrai_b, lasxXXI, (0b0111011100110100001, 0, 5, 10, 0), 0, 0, 103, 15486), //xvsrai_b
  INSTL(xvsrai_d, lasxXXI, (0b0111011100110101, 0, 5, 10, 0), 0, 0, 106, 15495), //xvsrai_d
  INSTL(xvsrai_h, lasxXXI, (0b011101110011010001, 0, 5, 10, 0), 0, 0, 104, 15504), //xvsrai_h
  INSTL(xvsrai_w, lasxXXI, (0b01110111001101001, 0, 5, 10, 0), 0, 0, 105, 15513), //xvsrai_w
  INSTL(xvsran_b_h, lasxXXX, (0b01110100111101101, 0, 5, 10, 0), 0, 0, 255, 15522), //xvsran_b_h
  INSTL(xvsran_h_w, lasxXXX, (0b01110100111101110, 0, 5, 10, 0), 0, 0, 256, 15533), //xvsran_h_w
  INSTL(xvsran_w_d, lasxXXX, (0b01110100111101111, 0, 5, 10, 0), 0, 0, 257, 15544), //xvsran_w_d
  INSTL(xvsrani_b_h, lasxXXI, (0b011101110101100001, 0, 5, 10, 0), 0, 0, 131, 15555), //xvsrani_b_h
  INSTL(xvsrani_d_q, lasxXXI, (0b011101110101101, 0, 5, 10, 0), 0, 0, 134, 15567), //xvsrani_d_q
  INSTL(xvsrani_h_w, lasxXXI, (0b01110111010110001, 0, 5, 10, 0), 0, 0, 132, 15579), //xvsrani_h_w
  INSTL(xvsrani_w_d, lasxXXI, (0b0111011101011001, 0, 5, 10, 0), 0, 0, 133, 15591), //xvsrani_w_d
  INSTL(xvsrar_b, lasxXXX, (0b01110100111100100, 0, 5, 10, 0), 0, 0, 248, 15603), //xvsrar_b
  INSTL(xvsrar_d, lasxXXX, (0b01110100111100111, 0, 5, 10, 0), 0, 0, 251, 15612), //xvsrar_d
  INSTL(xvsrar_h, lasxXXX, (0b01110100111100101, 0, 5, 10, 0), 0, 0, 249, 15621), //xvsrar_h
  INSTL(xvsrar_w, lasxXXX, (0b01110100111100110, 0, 5, 10, 0), 0, 0, 250, 15630), //xvsrar_w
  INSTL(xvsrari_b, lasxXXI, (0b0111011010101000001, 0, 5, 10, 0), 0, 0, 57, 15639), //xvsrari_b
  INSTL(xvsrari_d, lasxXXI, (0b0111011010101001, 0, 5, 10, 0), 0, 0, 60, 15649), //xvsrari_d
  INSTL(xvsrari_h, lasxXXI, (0b011101101010100001, 0, 5, 10, 0), 0, 0, 58, 15659), //xvsrari_h
  INSTL(xvsrari_w, lasxXXI, (0b01110110101010001, 0, 5, 10, 0), 0, 0, 59, 15669), //xvsrari_w
  INSTL(xvsrarn_b_h, lasxXXX, (0b01110100111110101, 0, 5, 10, 0), 0, 0, 261, 15679), //xvsrarn_b_h
  INSTL(xvsrarn_h_w, lasxXXX, (0b01110100111110110, 0, 5, 10, 0), 0, 0, 262, 15691), //xvsrarn_h_w
  INSTL(xvsrarn_w_d, lasxXXX, (0b01110100111110111, 0, 5, 10, 0), 0, 0, 263, 15703), //xvsrarn_w_d
  INSTL(xvsrarni_b_h, lasxXXI, (0b011101110101110001, 0, 5, 10, 0), 0, 0, 135, 15715), //xvsrarni_b_h
  INSTL(xvsrarni_d_q, lasxXXI, (0b011101110101111, 0, 5, 10, 0), 0, 0, 138, 15728), //xvsrarni_d_q
  INSTL(xvsrarni_h_w, lasxXXI, (0b01110111010111001, 0, 5, 10, 0), 0, 0, 136, 15741), //xvsrarni_h_w
  INSTL(xvsrarni_w_d, lasxXXI, (0b0111011101011101, 0, 5, 10, 0), 0, 0, 137, 15754), //xvsrarni_w_d
  INSTL(xvsrl_b, lasxXXX, (0b01110100111010100, 0, 5, 10, 0), 0, 0, 232, 15767), //xvsrl_b
  INSTL(xvsrl_d, lasxXXX, (0b01110100111010111, 0, 5, 10, 0), 0, 0, 235, 15775), //xvsrl_d
  INSTL(xvsrl_h, lasxXXX, (0b01110100111010101, 0, 5, 10, 0), 0, 0, 233, 15783), //xvsrl_h
  INSTL(xvsrl_w, lasxXXX, (0b01110100111010110, 0, 5, 10, 0), 0, 0, 234, 15791), //xvsrl_w
  INSTL(xvsrli_b, lasxXXI, (0b0111011100110000001, 0, 5, 10, 0), 0, 0, 99, 15799), //xvsrli_b
  INSTL(xvsrli_d, lasxXXI, (0b0111011100110001, 0, 5, 10, 0), 0, 0, 102, 15808), //xvsrli_d
  INSTL(xvsrli_h, lasxXXI, (0b011101110011000001, 0, 5, 10, 0), 0, 0, 100, 15817), //xvsrli_h
  INSTL(xvsrli_w, lasxXXI, (0b01110111001100001, 0, 5, 10, 0), 0, 0, 101, 15826), //xvsrli_w
  INSTL(xvsrln_b_h, lasxXXX, (0b01110100111101001, 0, 5, 10, 0), 0, 0, 252, 15835), //xvsrln_b_h
  INSTL(xvsrln_h_w, lasxXXX, (0b01110100111101010, 0, 5, 10, 0), 0, 0, 253, 15846), //xvsrln_h_w
  INSTL(xvsrln_w_d, lasxXXX, (0b01110100111101011, 0, 5, 10, 0), 0, 0, 254, 15857), //xvsrln_w_d
  INSTL(xvsrlni_b_h, lasxXXI, (0b011101110100000001, 0, 5, 10, 0), 0, 0, 107, 15868), //xvsrlni_b_h
  INSTL(xvsrlni_d_q, lasxXXI, (0b011101110100001, 0, 5, 10, 0), 0, 0, 110, 15880), //xvsrlni_d_q
  INSTL(xvsrlni_h_w, lasxXXI, (0b01110111010000001, 0, 5, 10, 0), 0, 0, 108, 15892), //xvsrlni_h_w
  INSTL(xvsrlni_w_d, lasxXXI, (0b0111011101000001, 0, 5, 10, 0), 0, 0, 109, 15904), //xvsrlni_w_d
  INSTL(xvsrlr_b, lasxXXX, (0b01110100111100000, 0, 5, 10, 0), 0, 0, 244, 15916), //xvsrlr_b
  INSTL(xvsrlr_d, lasxXXX, (0b01110100111100011, 0, 5, 10, 0), 0, 0, 247, 15925), //xvsrlr_d
  INSTL(xvsrlr_h, lasxXXX, (0b01110100111100001, 0, 5, 10, 0), 0, 0, 245, 15934), //xvsrlr_h
  INSTL(xvsrlr_w, lasxXXX, (0b01110100111100010, 0, 5, 10, 0), 0, 0, 246, 15943), //xvsrlr_w
  INSTL(xvsrlri_b, lasxXXI, (0b0111011010100100001, 0, 5, 10, 0), 0, 0, 53, 15952), //xvsrlri_b
  INSTL(xvsrlri_d, lasxXXI, (0b0111011010100101, 0, 5, 10, 0), 0, 0, 56, 15962), //xvsrlri_d
  INSTL(xvsrlri_h, lasxXXI, (0b011101101010010001, 0, 5, 10, 0), 0, 0, 54, 15972), //xvsrlri_h
  INSTL(xvsrlri_w, lasxXXI, (0b01110110101001001, 0, 5, 10, 0), 0, 0, 55, 15982), //xvsrlri_w
  INSTL(xvsrlrn_b_h, lasxXXX, (0b01110100111110001, 0, 5, 10, 0), 0, 0, 258, 15992), //xvsrlrn_b_h
  INSTL(xvsrlrn_h_w, lasxXXX, (0b01110100111110010, 0, 5, 10, 0), 0, 0, 259, 16004), //xvsrlrn_h_w
  INSTL(xvsrlrn_w_d, lasxXXX, (0b01110100111110011, 0, 5, 10, 0), 0, 0, 260, 16016), //xvsrlrn_w_d
  INSTL(xvsrlrni_b_h, lasxXXI, (0b011101110100010001, 0, 5, 10, 0), 0, 0, 111, 16028), //xvsrlrni_b_h
  INSTL(xvsrlrni_d_q, lasxXXI, (0b011101110100011, 0, 5, 10, 0), 0, 0, 114, 16041), //xvsrlrni_d_q
  INSTL(xvsrlrni_h_w, lasxXXI, (0b01110111010001001, 0, 5, 10, 0), 0, 0, 112, 16054), //xvsrlrni_h_w
  INSTL(xvsrlrni_w_d, lasxXXI, (0b0111011101000101, 0, 5, 10, 0), 0, 0, 113, 16067), //xvsrlrni_w_d
  INSTL(xvssran_b_h, lasxXXX, (0b01110100111111101, 0, 5, 10, 0), 0, 0, 267, 16080), //xvssran_b_h
  INSTL(xvssran_bu_h, lasxXXX, (0b01110101000001101, 0, 5, 10, 0), 0, 0, 279, 16092), //xvssran_bu_h
  INSTL(xvssran_h_w, lasxXXX, (0b01110100111111110, 0, 5, 10, 0), 0, 0, 268, 16105), //xvssran_h_w
  INSTL(xvssran_hu_w, lasxXXX, (0b01110101000001110, 0, 5, 10, 0), 0, 0, 280, 16117), //xvssran_hu_w
  INSTL(xvssran_w_d, lasxXXX, (0b01110100111111111, 0, 5, 10, 0), 0, 0, 269, 16130), //xvssran_w_d
  INSTL(xvssran_wu_d, lasxXXX, (0b01110101000001111, 0, 5, 10, 0), 0, 0, 281, 16142), //xvssran_wu_d
  INSTL(xvssrani_b_h, lasxXXI, (0b011101110110000001, 0, 5, 10, 0), 0, 0, 139, 16155), //xvssrani_b_h
  INSTL(xvssrani_bu_h, lasxXXI, (0b011101110110010001, 0, 5, 10, 0), 0, 0, 143, 16168), //xvssrani_bu_h
  INSTL(xvssrani_d_q, lasxXXI, (0b011101110110001, 0, 5, 10, 0), 0, 0, 142, 16182), //xvssrani_d_q
  INSTL(xvssrani_du_q, lasxXXI, (0b011101110110011, 0, 5, 10, 0), 0, 0, 146, 16195), //xvssrani_du_q
  INSTL(xvssrani_h_w, lasxXXI, (0b01110111011000001, 0, 5, 10, 0), 0, 0, 140, 16209), //xvssrani_h_w
  INSTL(xvssrani_hu_w, lasxXXI, (0b01110111011001001, 0, 5, 10, 0), 0, 0, 144, 16222), //xvssrani_hu_w
  INSTL(xvssrani_w_d, lasxXXI, (0b0111011101100001, 0, 5, 10, 0), 0, 0, 141, 16236), //xvssrani_w_d
  INSTL(xvssrani_wu_d, lasxXXI, (0b0111011101100101, 0, 5, 10, 0), 0, 0, 145, 16249), //xvssrani_wu_d
  INSTL(xvssrarn_b_h, lasxXXX, (0b01110101000000101, 0, 5, 10, 0), 0, 0, 273, 16263), //xvssrarn_b_h
  INSTL(xvssrarn_bu_h, lasxXXX, (0b01110101000010101, 0, 5, 10, 0), 0, 0, 285, 16276), //xvssrarn_bu_h
  INSTL(xvssrarn_h_w, lasxXXX, (0b01110101000000110, 0, 5, 10, 0), 0, 0, 274, 16290), //xvssrarn_h_w
  INSTL(xvssrarn_hu_w, lasxXXX, (0b01110101000010110, 0, 5, 10, 0), 0, 0, 286, 16303), //xvssrarn_hu_w
  INSTL(xvssrarn_w_d, lasxXXX, (0b01110101000000111, 0, 5, 10, 0), 0, 0, 275, 16317), //xvssrarn_w_d
  INSTL(xvssrarn_wu_d, lasxXXX, (0b01110101000010111, 0, 5, 10, 0), 0, 0, 287, 16330), //xvssrarn_wu_d
  INSTL(xvssrarni_b_h, lasxXXI, (0b011101110110100001, 0, 5, 10, 0), 0, 0, 147, 16344), //xvssrarni_b_h
  INSTL(xvssrarni_bu_h, lasxXXI, (0b011101110110110001, 0, 5, 10, 0), 0, 0, 151, 16358), //xvssrarni_bu_h
  INSTL(xvssrarni_d_q, lasxXXI, (0b011101110110101, 0, 5, 10, 0), 0, 0, 150, 16373), //xvssrarni_d_q
  INSTL(xvssrarni_du_q, lasxXXI, (0b011101110110111, 0, 5, 10, 0), 0, 0, 154, 16387), //xvssrarni_du_q
  INSTL(xvssrarni_h_w, lasxXXI, (0b01110111011010001, 0, 5, 10, 0), 0, 0, 148, 16402), //xvssrarni_h_w
  INSTL(xvssrarni_hu_w, lasxXXI, (0b01110111011011001, 0, 5, 10, 0), 0, 0, 152, 16416), //xvssrarni_hu_w
  INSTL(xvssrarni_w_d, lasxXXI, (0b0111011101101001, 0, 5, 10, 0), 0, 0, 149, 16431), //xvssrarni_w_d
  INSTL(xvssrarni_wu_d, lasxXXI, (0b0111011101101101, 0, 5, 10, 0), 0, 0, 153, 16445), //xvssrarni_wu_d
  INSTL(xvssrln_b_h, lasxXXX, (0b01110100111111001, 0, 5, 10, 0), 0, 0, 264, 16460), //xvssrln_b_h
  INSTL(xvssrln_bu_h, lasxXXX, (0b01110101000001001, 0, 5, 10, 0), 0, 0, 276, 16472), //xvssrln_bu_h
  INSTL(xvssrln_h_w, lasxXXX, (0b01110100111111010, 0, 5, 10, 0), 0, 0, 265, 16485), //xvssrln_h_w
  INSTL(xvssrln_hu_w, lasxXXX, (0b01110101000001010, 0, 5, 10, 0), 0, 0, 277, 16497), //xvssrln_hu_w
  INSTL(xvssrln_w_d, lasxXXX, (0b01110100111111011, 0, 5, 10, 0), 0, 0, 266, 16510), //xvssrln_w_d
  INSTL(xvssrln_wu_d, lasxXXX, (0b01110101000001011, 0, 5, 10, 0), 0, 0, 278, 16522), //xvssrln_wu_d
  INSTL(xvssrlni_b_h, lasxXXI, (0b011101110100100001, 0, 5, 10, 0), 0, 0, 115, 16535), //xvssrlni_b_h
  INSTL(xvssrlni_bu_h, lasxXXI, (0b011101110100110001, 0, 5, 10, 0), 0, 0, 119, 16548), //xvssrlni_bu_h
  INSTL(xvssrlni_d_q, lasxXXI, (0b011101110100101, 0, 5, 10, 0), 0, 0, 118, 16562), //xvssrlni_d_q
  INSTL(xvssrlni_du_q, lasxXXI, (0b011101110100111, 0, 5, 10, 0), 0, 0, 122, 16575), //xvssrlni_du_q
  INSTL(xvssrlni_h_w, lasxXXI, (0b01110111010010001, 0, 5, 10, 0), 0, 0, 116, 16589), //xvssrlni_h_w
  INSTL(xvssrlni_hu_w, lasxXXI, (0b01110111010011001, 0, 5, 10, 0), 0, 0, 120, 16602), //xvssrlni_hu_w
  INSTL(xvssrlni_w_d, lasxXXI, (0b0111011101001001, 0, 5, 10, 0), 0, 0, 117, 16616), //xvssrlni_w_d
  INSTL(xvssrlni_wu_d, lasxXXI, (0b0111011101001101, 0, 5, 10, 0), 0, 0, 121, 16629), //xvssrlni_wu_d
  INSTL(xvssrlrn_b_h, lasxXXX, (0b01110101000000001, 0, 5, 10, 0), 0, 0, 270, 16643), //xvssrlrn_b_h
  INSTL(xvssrlrn_bu_h, lasxXXX, (0b01110101000010001, 0, 5, 10, 0), 0, 0, 282, 16656), //xvssrlrn_bu_h
  INSTL(xvssrlrn_h_w, lasxXXX, (0b01110101000000010, 0, 5, 10, 0), 0, 0, 271, 16670), //xvssrlrn_h_w
  INSTL(xvssrlrn_hu_w, lasxXXX, (0b01110101000010010, 0, 5, 10, 0), 0, 0, 283, 16683), //xvssrlrn_hu_w
  INSTL(xvssrlrn_w_d, lasxXXX, (0b01110101000000011, 0, 5, 10, 0), 0, 0, 272, 16697), //xvssrlrn_w_d
  INSTL(xvssrlrn_wu_d, lasxXXX, (0b01110101000010011, 0, 5, 10, 0), 0, 0, 284, 16710), //xvssrlrn_wu_d
  INSTL(xvssrlrni_b_h, lasxXXI, (0b011101110101000001, 0, 5, 10, 0), 0, 0, 123, 16724), //xvssrlrni_b_h
  INSTL(xvssrlrni_bu_h, lasxXXI, (0b011101110101010001, 0, 5, 10, 0), 0, 0, 127, 16738), //xvssrlrni_bu_h
  INSTL(xvssrlrni_d_q, lasxXXI, (0b011101110101001, 0, 5, 10, 0), 0, 0, 126, 16753), //xvssrlrni_d_q
  INSTL(xvssrlrni_du_q, lasxXXI, (0b011101110101011, 0, 5, 10, 0), 0, 0, 130, 16767), //xvssrlrni_du_q
  INSTL(xvssrlrni_h_w, lasxXXI, (0b01110111010100001, 0, 5, 10, 0), 0, 0, 124, 16782), //xvssrlrni_h_w
  INSTL(xvssrlrni_hu_w, lasxXXI, (0b01110111010101001, 0, 5, 10, 0), 0, 0, 128, 16796), //xvssrlrni_hu_w
  INSTL(xvssrlrni_w_d, lasxXXI, (0b0111011101010001, 0, 5, 10, 0), 0, 0, 125, 16811), //xvssrlrni_w_d
  INSTL(xvssrlrni_wu_d, lasxXXI, (0b0111011101010101, 0, 5, 10, 0), 0, 0, 129, 16825), //xvssrlrni_wu_d
  INSTL(xvssub_b, lasxXXX, (0b01110100010010000, 0, 5, 10, 0), 0, 0, 72, 16840), //xvssub_b
  INSTL(xvssub_bu, lasxXXX, (0b01110100010011000, 0, 5, 10, 0), 0, 0, 80, 16849), //xvssub_bu
  INSTL(xvssub_d, lasxXXX, (0b01110100010010011, 0, 5, 10, 0), 0, 0, 75, 16859), //xvssub_d
  INSTL(xvssub_du, lasxXXX, (0b01110100010011011, 0, 5, 10, 0), 0, 0, 83, 16868), //xvssub_du
  INSTL(xvssub_h, lasxXXX, (0b01110100010010001, 0, 5, 10, 0), 0, 0, 73, 16878), //xvssub_h
  INSTL(xvssub_hu, lasxXXX, (0b01110100010011001, 0, 5, 10, 0), 0, 0, 81, 16887), //xvssub_hu
  INSTL(xvssub_w, lasxXXX, (0b01110100010010010, 0, 5, 10, 0), 0, 0, 74, 16897), //xvssub_w
  INSTL(xvssub_wu, lasxXXX, (0b01110100010011010, 0, 5, 10, 0), 0, 0, 82, 16906), //xvssub_wu
  INSTL(xvst,      SimdLdst, (0b0010110011,        22, 12), 0, 0, 14, 16916), //xvst
  INSTL(xvstelm_b, lasxXRII, (0b001100111, 0, kWX, 5, 10, 18, 0), 0, 0, 3, 16921), //xvstelm_b
  INSTL(xvstelm_d, lasxXRII, (0b001100110001, 0, kWX, 5, 10, 18, 0), 0, 0, 0, 16931), //xvstelm_d
  INSTL(xvstelm_h, lasxXRII, (0b0011001101, 0, kWX, 5, 10, 18, 0), 0, 0, 2, 16941), //xvstelm_h
  INSTL(xvstelm_w, lasxXRII, (0b00110011001, 0, kWX, 5, 10, 18, 0), 0, 0, 1, 16951), //xvstelm_w
  INSTL(xvstx,     SimdLdst, (0b00111000010011000, 15, 0 ), 0, 0, 15, 16961), //xvstx
  INSTL(xvsub_b, lasxXXX, (0b01110100000011000, 0, 5, 10, 0), 0, 0, 24, 16967), //xvsub_b
  INSTL(xvsub_d, lasxXXX, (0b01110100000011011, 0, 5, 10, 0), 0, 0, 27, 16975), //xvsub_d
  INSTL(xvsub_h, lasxXXX, (0b01110100000011001, 0, 5, 10, 0), 0, 0, 25, 16983), //xvsub_h
  INSTL(xvsub_q, lasxXXX, (0b01110101001011011, 0, 5, 10, 0), 0, 0, 333, 16991), //xvsub_q
  INSTL(xvsub_w, lasxXXX, (0b01110100000011010, 0, 5, 10, 0), 0, 0, 26, 16999), //xvsub_w
  INSTL(xvsubi_bu, lasxXXI, (0b01110110100011000, 0, 5, 10, 0), 0, 0, 24, 17007), //xvsubi_bu
  INSTL(xvsubi_du, lasxXXI, (0b01110110100011011, 0, 5, 10, 0), 0, 0, 27, 17017), //xvsubi_du
  INSTL(xvsubi_hu, lasxXXI, (0b01110110100011001, 0, 5, 10, 0), 0, 0, 25, 17027), //xvsubi_hu
  INSTL(xvsubi_wu, lasxXXI, (0b01110110100011010, 0, 5, 10, 0), 0, 0, 26, 17037), //xvsubi_wu
  INSTL(xvsubwev_d_w, lasxXXX, (0b01110100001000010, 0, 5, 10, 0), 0, 0, 34, 17047), //xvsubwev_d_w
  INSTL(xvsubwev_d_wu, lasxXXX, (0b01110100001100010, 0, 5, 10, 0), 0, 0, 50, 17060), //xvsubwev_d_wu
  INSTL(xvsubwev_h_b, lasxXXX, (0b01110100001000000, 0, 5, 10, 0), 0, 0, 32, 17074), //xvsubwev_h_b
  INSTL(xvsubwev_h_bu, lasxXXX, (0b01110100001100000, 0, 5, 10, 0), 0, 0, 48, 17087), //xvsubwev_h_bu
  INSTL(xvsubwev_q_d, lasxXXX, (0b01110100001000011, 0, 5, 10, 0), 0, 0, 35, 17101), //xvsubwev_q_d
  INSTL(xvsubwev_q_du, lasxXXX, (0b01110100001100011, 0, 5, 10, 0), 0, 0, 51, 17114), //xvsubwev_q_du
  INSTL(xvsubwev_w_h, lasxXXX, (0b01110100001000001, 0, 5, 10, 0), 0, 0, 33, 17128), //xvsubwev_w_h
  INSTL(xvsubwev_w_hu, lasxXXX, (0b01110100001100001, 0, 5, 10, 0), 0, 0, 49, 17141), //xvsubwev_w_hu
  INSTL(xvsubwod_d_w, lasxXXX, (0b01110100001001010, 0, 5, 10, 0), 0, 0, 42, 17155), //xvsubwod_d_w
  INSTL(xvsubwod_d_wu, lasxXXX, (0b01110100001101010, 0, 5, 10, 0), 0, 0, 58, 17168), //xvsubwod_d_wu
  INSTL(xvsubwod_h_b, lasxXXX, (0b01110100001001000, 0, 5, 10, 0), 0, 0, 40, 17182), //xvsubwod_h_b
  INSTL(xvsubwod_h_bu, lasxXXX, (0b01110100001101000, 0, 5, 10, 0), 0, 0, 56, 17195), //xvsubwod_h_bu
  INSTL(xvsubwod_q_d, lasxXXX, (0b01110100001001011, 0, 5, 10, 0), 0, 0, 43, 17209), //xvsubwod_q_d
  INSTL(xvsubwod_q_du, lasxXXX, (0b01110100001101011, 0, 5, 10, 0), 0, 0, 59, 17222), //xvsubwod_q_du
  INSTL(xvsubwod_w_h, lasxXXX, (0b01110100001001001, 0, 5, 10, 0), 0, 0, 41, 17236), //xvsubwod_w_h
  INSTL(xvsubwod_w_hu, lasxXXX, (0b01110100001101001, 0, 5, 10, 0), 0, 0, 57, 17249), //xvsubwod_w_hu
  INSTL(xvxor_v, lasxXXX, (0b01110101001001110, 0, 5, 10, 0), 0, 0, 326, 17263), //xvxor_v
  INSTL(xvxori_b, lasxXXI, (0b01110111110110, 0, 5, 10, 0), 0, 0, 166, 17271), //xvxori_b
  INSTL(vfcmp_caf_s,  lsxVVV, (0b0000110001010, 0, 5, 10, 0), 0, 0, 367, 17280), //vfcmp.caf.s
  INSTL(vfcmp_cun_s,  lsxVVV, (0b0000110001011000, 0, 5, 10, 0), 0, 0, 368, 17292), //vfcmp.cun.s
  INSTL(vfcmp_ceq_s,  lsxVVV, (0b000011000101100, 0, 5, 10, 0), 0, 0, 369, 17304), //vfcmp.ceq.s
  INSTL(vfcmp_cueq_s,  lsxVVV, (0b0000110001011100, 0, 5, 10, 0), 0, 0, 370, 17316), //vfcmp.cueq.s
  INSTL(vfcmp_clt_s,  lsxVVV, (0b00001100010110, 0, 5, 10, 0), 0, 0, 371, 17329), //vfcmp.clt.s
  INSTL(vfcmp_cult_s,  lsxVVV, (0b0000110001011010, 0, 5, 10, 0), 0, 0, 372, 17341), //vfcmp.cult.s
  INSTL(vfcmp_cle_s,  lsxVVV, (0b000011000101110, 0, 5, 10, 0), 0, 0, 373, 17354), //vfcmp.cle.s
  INSTL(vfcmp_cule_s,  lsxVVV, (0b0000110001011110, 0, 5, 10, 0), 0, 0, 374, 17366), //vfcmp.cule.s
  INSTL(vfcmp_cne_s,  lsxVVV, (0b00001100010110000, 0, 5, 10, 0), 0, 0, 375, 17379), //vfcmp.cne.s
  INSTL(vfcmp_cor_s,  lsxVVV, (0b00001100010110100, 0, 5, 10, 0), 0, 0, 376, 17391), //vfcmp.cor.s
  INSTL(vfcmp_cune_s,  lsxVVV, (0b00001100010111000, 0, 5, 10, 0), 0, 0, 377, 17403), //vfcmp.cune.s
  INSTL(vfcmp_saf_s,  lsxVVV, (0b0000110001011, 0, 5, 10, 0), 0, 0, 378, 17416), //vfcmp.saf.s
  INSTL(vfcmp_sun_s,  lsxVVV, (0b0000110001011001, 0, 5, 10, 0), 0, 0, 379, 17428), //vfcmp.sun.s
  INSTL(vfcmp_seq_s,  lsxVVV, (0b000011000101101, 0, 5, 10, 0), 0, 0, 380, 17440), //vfcmp.seq.s
  INSTL(vfcmp_sueq_s,  lsxVVV, (0b0000110001011101, 0, 5, 10, 0), 0, 0, 381, 17452), //vfcmp.sueq.s
  INSTL(vfcmp_slt_s,  lsxVVV, (0b00001100010111, 0, 5, 10, 0), 0, 0, 382, 17465), //vfcmp.slt.s
  INSTL(vfcmp_sult_s,  lsxVVV, (0b0000110001011011, 0, 5, 10, 0), 0, 0, 383, 17477), //vfcmp.sult.s
  INSTL(vfcmp_sle_s,  lsxVVV, (0b000011000101111, 0, 5, 10, 0), 0, 0, 384, 17490), //vfcmp.sle.s
  INSTL(vfcmp_sule_s,  lsxVVV, (0b0000110001011111, 0, 5, 10, 0), 0, 0, 385, 17502), //vfcmp.sule.s
  INSTL(vfcmp_sne_s,  lsxVVV, (0b00001100010110001, 0, 5, 10, 0), 0, 0, 386, 17515), //vfcmp.sne.s
  INSTL(vfcmp_sor_s,  lsxVVV, (0b00001100010110101, 0, 5, 10, 0), 0, 0, 387, 17527), //vfcmp.sor.s
  INSTL(vfcmp_sune_s,  lsxVVV, (0b00001100010111001, 0, 5, 10, 0), 0, 0, 388, 17539), //vfcmp.sune.s
  INSTL(vfcmp_caf_d,  lsxVVV, (0b0000110001100, 0, 5, 10, 0), 0, 0, 389, 17552), //vfcmp.caf.d
  INSTL(vfcmp_cun_d,  lsxVVV, (0b0000110001101000, 0, 5, 10, 0), 0, 0, 390, 17564), //vfcmp.cun.d
  INSTL(vfcmp_ceq_d,  lsxVVV, (0b000011000110100, 0, 5, 10, 0), 0, 0, 391, 17576), //vfcmp.ceq.d
  INSTL(vfcmp_cueq_d,  lsxVVV, (0b0000110001101100, 0, 5, 10, 0), 0, 0, 392, 17588), //vfcmp.cueq.d
  INSTL(vfcmp_clt_d,  lsxVVV, (0b00001100011010, 0, 5, 10, 0), 0, 0, 393, 17601), //vfcmp.clt.d
  INSTL(vfcmp_cult_d,  lsxVVV, (0b0000110001101010, 0, 5, 10, 0), 0, 0, 394, 17613), //vfcmp.cult.d
  INSTL(vfcmp_cle_d,  lsxVVV, (0b000011000110110, 0, 5, 10, 0), 0, 0, 395, 17626), //vfcmp.cle.d
  INSTL(vfcmp_cule_d,  lsxVVV, (0b0000110001101110, 0, 5, 10, 0), 0, 0, 396, 17638), //vfcmp.cule.d
  INSTL(vfcmp_cne_d,  lsxVVV, (0b00001100011010000, 0, 5, 10, 0), 0, 0, 397, 17651), //vfcmp.cne.d
  INSTL(vfcmp_cor_d,  lsxVVV, (0b00001100011010100, 0, 5, 10, 0), 0, 0, 398, 17663), //vfcmp.cor.d
  INSTL(vfcmp_cune_d,  lsxVVV, (0b00001100011011000, 0, 5, 10, 0), 0, 0, 399, 17675), //vfcmp.cune.d
  INSTL(vfcmp_saf_d,  lsxVVV, (0b0000110001101, 0, 5, 10, 0), 0, 0, 400, 17688), //vfcmp.saf.d
  INSTL(vfcmp_sun_d,  lsxVVV, (0b0000110001101001, 0, 5, 10, 0), 0, 0, 401, 17700), //vfcmp.sun.d
  INSTL(vfcmp_seq_d,  lsxVVV, (0b000011000110101, 0, 5, 10, 0), 0, 0, 402, 17712), //vfcmp.seq.d
  INSTL(vfcmp_sueq_d,  lsxVVV, (0b0000110001101101, 0, 5, 10, 0), 0, 0, 403, 17724), //vfcmp.sueq.d
  INSTL(vfcmp_slt_d,  lsxVVV, (0b00001100011011, 0, 5, 10, 0), 0, 0, 404, 17737), //vfcmp.slt.d
  INSTL(vfcmp_sult_d,  lsxVVV, (0b0000110001101011, 0, 5, 10, 0), 0, 0, 405, 17749), //vfcmp.sult.d
  INSTL(vfcmp_sle_d,  lsxVVV, (0b000011000110111, 0, 5, 10, 0), 0, 0, 406, 17762), //vfcmp.sle.d
  INSTL(vfcmp_sule_d,  lsxVVV, (0b0000110001101111, 0, 5, 10, 0), 0, 0, 407, 17774), //vfcmp.sule.d
  INSTL(vfcmp_sne_d,  lsxVVV, (0b00001100011010001, 0, 5, 10, 0), 0, 0, 408, 17787), //vfcmp.sne.d
  INSTL(vfcmp_sor_d,  lsxVVV, (0b00001100011010101, 0, 5, 10, 0), 0, 0, 409, 17799), //vfcmp.sor.d
  INSTL(vfcmp_sune_d,  lsxVVV, (0b00001100011011001, 0, 5, 10, 0), 0, 0, 410, 17811), //vfcmp.sune.d
  INSTL(xvfcmp_caf_s,  lasxXXX, (0b0000110010010, 0, 5, 10, 0), 0, 0, 368, 17824), //xvfcmp.caf.s
  INSTL(xvfcmp_cun_s,  lasxXXX, (0b0000110010011000, 0, 5, 10, 0), 0, 0, 369, 17837), //xvfcmp.cun.s
  INSTL(xvfcmp_ceq_s,  lasxXXX, (0b000011001001100, 0, 5, 10, 0), 0, 0, 370, 17850), //xvfcmp.ceq.s
  INSTL(xvfcmp_cueq_s,  lasxXXX, (0b0000110010011100, 0, 5, 10, 0), 0, 0, 371, 17863), //xvfcmp.cueq.s
  INSTL(xvfcmp_clt_s,  lasxXXX, (0b00001100100110, 0, 5, 10, 0), 0, 0, 372, 17877), //xvfcmp.clt.s
  INSTL(xvfcmp_cult_s,  lasxXXX, (0b0000110010011010, 0, 5, 10, 0), 0, 0, 373, 17890), //xvfcmp.cult.s
  INSTL(xvfcmp_cle_s,  lasxXXX, (0b000011001001110, 0, 5, 10, 0), 0, 0, 374, 17904), //xvfcmp.cle.s
  INSTL(xvfcmp_cule_s,  lasxXXX, (0b0000110010011110, 0, 5, 10, 0), 0, 0, 375, 17917), //xvfcmp.cule.s
  INSTL(xvfcmp_cne_s,  lasxXXX, (0b00001100100110000, 0, 5, 10, 0), 0, 0, 376, 17931), //xvfcmp.cne.s
  INSTL(xvfcmp_cor_s,  lasxXXX, (0b00001100100110100, 0, 5, 10, 0), 0, 0, 377, 17944), //xvfcmp.cor.s
  INSTL(xvfcmp_cune_s,  lasxXXX, (0b00001100100111000, 0, 5, 10, 0), 0, 0, 378, 17957), //xvfcmp.cune.s
  INSTL(xvfcmp_saf_s,  lasxXXX, (0b0000110010011, 0, 5, 10, 0), 0, 0, 379, 17971), //xvfcmp.saf.s
  INSTL(xvfcmp_sun_s,  lasxXXX, (0b0000110010011001, 0, 5, 10, 0), 0, 0, 380, 17984), //xvfcmp.sun.s
  INSTL(xvfcmp_seq_s,  lasxXXX, (0b000011001001101, 0, 5, 10, 0), 0, 0, 381, 17997), //xvfcmp.seq.s
  INSTL(xvfcmp_sueq_s,  lasxXXX, (0b0000110010011101, 0, 5, 10, 0), 0, 0, 382, 18010), //xvfcmp.sueq.s
  INSTL(xvfcmp_slt_s,  lasxXXX, (0b00001100100111, 0, 5, 10, 0), 0, 0, 383, 18024), //xvfcmp.slt.s
  INSTL(xvfcmp_sult_s,  lasxXXX, (0b0000110010011011, 0, 5, 10, 0), 0, 0, 384, 18037), //xvfcmp.sult.s
  INSTL(xvfcmp_sle_s,  lasxXXX, (0b000011001001111, 0, 5, 10, 0), 0, 0, 385, 18051), //xvfcmp.sle.s
  INSTL(xvfcmp_sule_s,  lasxXXX, (0b0000110010011111, 0, 5, 10, 0), 0, 0, 386, 18064), //xvfcmp.sule.s
  INSTL(xvfcmp_sne_s,  lasxXXX, (0b00001100100110001, 0, 5, 10, 0), 0, 0, 387, 18078), //xvfcmp.sne.s
  INSTL(xvfcmp_sor_s,  lasxXXX, (0b00001100100110101, 0, 5, 10, 0), 0, 0, 388, 18091), //xvfcmp.sor.s
  INSTL(xvfcmp_sune_s,  lasxXXX, (0b00001100100111001, 0, 5, 10, 0), 0, 0, 389, 18104), //xvfcmp.sune.s
  INSTL(xvfcmp_caf_d,  lasxXXX, (0b0000110010100, 0, 5, 10, 0), 0, 0, 390, 18118), //xvfcmp.caf.d
  INSTL(xvfcmp_cun_d,  lasxXXX, (0b0000110010101000, 0, 5, 10, 0), 0, 0, 391, 18131), //xvfcmp.cun.d
  INSTL(xvfcmp_ceq_d,  lasxXXX, (0b000011001010100, 0, 5, 10, 0), 0, 0, 392, 18144), //xvfcmp.ceq.d
  INSTL(xvfcmp_cueq_d,  lasxXXX, (0b0000110010101100, 0, 5, 10, 0), 0, 0, 393, 18157), //xvfcmp.cueq.d
  INSTL(xvfcmp_clt_d,  lasxXXX, (0b00001100101010, 0, 5, 10, 0), 0, 0, 394, 18171), //xvfcmp.clt.d
  INSTL(xvfcmp_cult_d,  lasxXXX, (0b0000110010101010, 0, 5, 10, 0), 0, 0, 395, 18184), //xvfcmp.cult.d
  INSTL(xvfcmp_cle_d,  lasxXXX, (0b000011001010110, 0, 5, 10, 0), 0, 0, 396, 18198), //xvfcmp.cle.d
  INSTL(xvfcmp_cule_d,  lasxXXX, (0b0000110010101110, 0, 5, 10, 0), 0, 0, 397, 18211), //xvfcmp.cule.d
  INSTL(xvfcmp_cne_d,  lasxXXX, (0b00001100101010000, 0, 5, 10, 0), 0, 0, 398, 18225), //xvfcmp.cne.d
  INSTL(xvfcmp_cor_d,  lasxXXX, (0b00001100101010100, 0, 5, 10, 0), 0, 0, 399, 18238), //xvfcmp.cor.d
  INSTL(xvfcmp_cune_d,  lasxXXX, (0b00001100101011000, 0, 5, 10, 0), 0, 0, 400, 18251), //xvfcmp.cune.d
  INSTL(xvfcmp_saf_d,  lasxXXX, (0b0000110010101, 0, 5, 10, 0), 0, 0, 401, 18265), //xvfcmp.saf.d
  INSTL(xvfcmp_sun_d,  lasxXXX, (0b0000110010101001, 0, 5, 10, 0), 0, 0, 402, 18278), //xvfcmp.sun.d
  INSTL(xvfcmp_seq_d,  lasxXXX, (0b000011001010101, 0, 5, 10, 0), 0, 0, 403, 18291), //xvfcmp.seq.d
  INSTL(xvfcmp_sueq_d,  lasxXXX, (0b0000110010101101, 0, 5, 10, 0), 0, 0, 404, 18304), //xvfcmp.sueq.d
  INSTL(xvfcmp_slt_d,  lasxXXX, (0b00001100101011, 0, 5, 10, 0), 0, 0, 405, 18318), //xvfcmp.slt.d
  INSTL(xvfcmp_sult_d,  lasxXXX, (0b0000110010101011, 0, 5, 10, 0), 0, 0, 406, 18331), //xvfcmp.sult.d
  INSTL(xvfcmp_sle_d,  lasxXXX, (0b000011001010111, 0, 5, 10, 0), 0, 0, 407, 18345), //xvfcmp.sle.d
  INSTL(xvfcmp_sule_d,  lasxXXX, (0b0000110010101111, 0, 5, 10, 0), 0, 0, 408, 18358), //xvfcmp.sule.d
  INSTL(xvfcmp_sne_d,  lasxXXX, (0b00001100101010001, 0, 5, 10, 0), 0, 0, 409, 18372), //xvfcmp.sne.d
  INSTL(xvfcmp_sor_d,  lasxXXX, (0b00001100101010101, 0, 5, 10, 0), 0, 0, 410, 18385), //xvfcmp.sor.d
  INSTL(xvfcmp_sune_d,  lasxXXX, (0b00001100101011001, 0, 5, 10, 0), 0, 0, 411, 18398), //xvfcmp.sune.d
};


#undef F
#undef INST
#undef INSTL
#undef NAME_DATA_INDEX

namespace EncodingData {

// ${EncodingData:Begin}
// ------------------- Automatically generated, do not edit -------------------

const BaseLldst baseLldst[11] = {
  { 0b0010100000, 0, kWX, 2 },
  { 0b0010100001, 0, kWX, 2 },
  { 0b0010100010, 0, kWX, 2 },
  { 0b0010100011, 0, kWX, 2 },
  { 0b0010100100, 0, kWX, 2 },
  { 0b0010100101, 0, kWX, 2 },
  { 0b0010100110, 0, kWX, 2 },
  { 0b0010100111, 0, kWX, 2 },
  { 0b0010101000, 0, kWX, 2 },
  { 0b0010101001, 0, kWX, 2 },
  { 0b0010101010, 0, kWX, 2 },
};

const BaseLdSt baseLdst[30] = {
  { 0b0010100000, 22 },
  { 0b0010100001, 22 },
  { 0b0010100010, 22 },
  { 0b0010100011, 22 },
  { 0b0010100100, 22 },
  { 0b0010100101, 22 },
  { 0b0010100110, 22 },
  { 0b0010100111, 22 },
  { 0b0010101000, 22 },
  { 0b0010101001, 22 },
  { 0b0010101010, 22 },
  { 0b00111000000000000, 15 }, //ldx.b  //index 11
  { 0b00111000000001000, 15 }, //ldx.h
  { 0b00111000000010000, 15 }, //ldx.w
  { 0b00111000000011000, 15 }, //ldx.d
  { 0b00111000000100000, 15 }, //stx.b
  { 0b00111000000101000, 15 }, //stx.h
  { 0b00111000000110000, 15 }, //stx.w
  { 0b00111000000111000, 15 }, //stx.d
  { 0b00111000001000000, 15 }, //ldx.bu
  { 0b00111000001001000, 15 }, //ldx.hu
  { 0b00111000001010000, 15 }, //ldx.wu
  { 0b0010101100, 22 }, //fld.s  //index 22
  { 0b0010101101, 22 }, //fst.s
  { 0b0010101110, 22 }, //fld.d
  { 0b0010101111, 22 }, //fst.d
  { 0b00111000001100000, 15}, //fldx.s
  { 0b00111000001101000, 15}, //fldx.d
  { 0b00111000001110000, 15}, //fstx.s
  { 0b00111000001111000, 15}, //fstx.d
};

const BaseOp baseOp[7] = {
  { 0b00000110010010000010000000000000 },
  { 0b00000110010010000010010000000000 },
  { 0b00000110010010000010100000000000 },
  { 0b00000110010010000010110000000000 },
  { 0b00000110010010000011000000000000 },
  { 0b00000110010010000011010000000000 },
  { 0b00000110010010000011100000000000 },
};

const BaseOpImm baseOpImm[3] = {
  { 0b00000110010010001, 15, 0 },
  { 0b00111000011100100, 15, 0 },
  { 0b00111000011100101, 15, 0 },
};

const BaseLIC baseLIC[3] = {
  { 0b00000000001010100, 0 },
  { 0b00000000001010101, 0 },
  { 0b00000000001010110, 0 },
};

const BaseLRI baseLRI[10] = {
  { 0b0001010, kWX, 0, 5, 0},
  { 0b0001011, kWX, 0, 5, 0 },
  { 0b0001100, kWX, 0, 5, 0 },
  { 0b0001101, kWX, 0, 5, 0 },
  { 0b0001110, kWX, 0, 5, 0 },
  { 0b0001111, kWX, 0, 5, 0 },
  { 0b0000000100010100110111, kWX, 0, 5, 1 },
  { 0b00000100, kWX, 0, 10, 2 },
  { 0b00000100, kWX, 0, 10, 3 },
  { 0b00000110010001, kWX, 5, 10, 4 },
};

const BaseLRRL baseLRRL[2] = {
  { 0b00000000000000010, kWX, 5, kWX, 10 },
  { 0b00000000000000011, kWX, 5, kWX, 10 },
};

const BaseLRR baseLRR[34] = {
  { 0b0000000000000000000100, kWX, 0, kWX, 5, 0 }, // CLO.W
  { 0b0000000000000000000101, kWX, 0, kWX, 5, 0 }, // CLZ.W
  { 0b0000000000000000000110, kWX, 0, kWX, 5, 0 }, // CTO.W
  { 0b0000000000000000000111, kWX, 0, kWX, 5, 0 }, // CTZ.W
  { 0b0000000000000000011011, kWX, 0, kWX, 5, 0 }, // CPUCFG
  { 0b0000000000000000001000, kWX, 0, kWX, 5, 0 }, // CLO.D
  { 0b0000000000000000001001, kWX, 0, kWX, 5, 0 }, // CLZ.D
  { 0b0000000000000000001010, kWX, 0, kWX, 5, 0 }, // CTO.D
  { 0b0000000000000000001011, kWX, 0, kWX, 5, 0 }, // CTZ.D
  { 0b0000000000000000001100, kWX, 0, kWX, 5, 0 }, // REVB.2H
  { 0b0000000000000000001101, kWX, 0, kWX, 5, 0 }, // REVB.4H
  { 0b0000000000000000001110, kWX, 0, kWX, 5, 0 }, // REVB.2W
  { 0b0000000000000000001111, kWX, 0, kWX, 5, 0 }, // REVB.D
  { 0b0000000000000000010000, kWX, 0, kWX, 5, 0 }, // REVH.2W
  { 0b0000000000000000010001, kWX, 0, kWX, 5, 0 }, // REVH.D
  { 0b0000000000000000010010, kWX, 0, kWX, 5, 0 }, // BITREV.4B
  { 0b0000000000000000010011, kWX, 0, kWX, 5, 0 }, // BITREV.8B
  { 0b0000000000000000010100, kWX, 0, kWX, 5, 0 }, // BITREV.W
  { 0b0000000000000000010101, kWX, 0, kWX, 5, 0 }, // BITREV.D
  { 0b0000000000000000010110, kWX, 0, kWX, 5, 0 }, // EXT.W.H
  { 0b0000000000000000010111, kWX, 0, kWX, 5, 0 }, // EXT.W.B
  { 0b0000000000000000011000, kWX, 0, kWX, 5, 0 }, // RDTIMEL.W
  { 0b0000000000000000011001, kWX, 0, kWX, 5, 0 }, // RDTIMEH.W
  { 0b0000000000000000011010, kWX, 0, kWX, 5, 0 }, // RDTIME.D
  { 0b0000000100010100110000, kWX, 0, kWX, 5, 0 }, // MOVGR2FCSR
  { 0b0000000100010100110010, kWX, 0, kWX, 5, 0 }, // MOVFCSR2GR
  { 0b0000011001001000000000, kWX, 0, kWX, 5, 0 }, // IOCSRRD.B
  { 0b0000011001001000000001, kWX, 0, kWX, 5, 0 }, // IOCSRRD.H
  { 0b0000011001001000000010, kWX, 0, kWX, 5, 0 }, // IOCSRRD.W
  { 0b0000011001001000000011, kWX, 0, kWX, 5, 0 }, // IOCSRRD.D
  { 0b0000011001001000000100, kWX, 0, kWX, 5, 0 }, // IOCSRWR.B
  { 0b0000011001001000000101, kWX, 0, kWX, 5, 0 }, // IOCSRWR.H
  { 0b0000011001001000000110, kWX, 0, kWX, 5, 0 }, // IOCSRWR.W
  { 0b0000011001001000000111, kWX, 0, kWX, 5, 0 }, // IOCSRWR.D
};

const BaseLVV baseLVV[44] = {
  { 0b0000000100010100000001, 0, 5, 0 }, // FABS.S
  { 0b0000000100010100000010, 0, 5, 0 }, // FABS.D
  { 0b0000000100010100000101, 0, 5, 0 }, // FNEG.S
  { 0b0000000100010100000110, 0, 5, 0 }, // FNEG.D
  { 0b0000000100010100001001, 0, 5, 0 }, // FLOGB.S
  { 0b0000000100010100001010, 0, 5, 0 }, // FLOGB.D
  { 0b0000000100010100001101, 0, 5, 0 }, // FCLASS.S
  { 0b0000000100010100001110, 0, 5, 0 }, // FCLASS.D
  { 0b0000000100010100010001, 0, 5, 0 }, // FSQRT.S
  { 0b0000000100010100010010, 0, 5, 0 }, // FSQRT.D
  { 0b0000000100010100010101, 0, 5, 0 }, // FRECIP.S
  { 0b0000000100010100010110, 0, 5, 0 }, // FRECIP.D
  { 0b0000000100010100011001, 0, 5, 0 }, // FRSQRT.S
  { 0b0000000100010100011010, 0, 5, 0 }, // FRSQRT.D
  { 0b0000000100010100100101, 0, 5, 0 }, // FMOV.S
  { 0b0000000100010100100110, 0, 5, 0 }, // FMOV.D
  { 0b0000000100011001000110, 0, 5, 0 }, // FCVT.S.D
  { 0b0000000100011001001001, 0, 5, 0 }, // FCVT.D.S
  { 0b0000000100011010000001, 0, 5, 0 }, // FTINTRM.W.S
  { 0b0000000100011010000010, 0, 5, 0 }, // FTINTRM.W.D
  { 0b0000000100011010001001, 0, 5, 0 }, // FTINTRM.L.S
  { 0b0000000100011010001010, 0, 5, 0 }, // FTINTRM.L.D
  { 0b0000000100011010010001, 0, 5, 0 }, // FTINTRP.W.S
  { 0b0000000100011010010010, 0, 5, 0 }, // FTINTRP.W.D
  { 0b0000000100011010011001, 0, 5, 0 }, // FTINTRP.L.S
  { 0b0000000100011010011010, 0, 5, 0 }, // FTINTRP.L.D
  { 0b0000000100011010100001, 0, 5, 0 }, // FTINTRZ.W.S
  { 0b0000000100011010100010, 0, 5, 0 }, // FTINTRZ.W.D
  { 0b0000000100011010101001, 0, 5, 0 }, // FTINTRZ.L.S
  { 0b0000000100011010101010, 0, 5, 0 }, // FTINTRZ.L.D
  { 0b0000000100011010110001, 0, 5, 0 }, // FTINTRNE.W.S
  { 0b0000000100011010110010, 0, 5, 0 }, // FTINTRNE.W.D
  { 0b0000000100011010111001, 0, 5, 0 }, // FTINTRNE.L.S
  { 0b0000000100011010111010, 0, 5, 0 }, // FTINTRNE.L.D
  { 0b0000000100011011000001, 0, 5, 0 }, // FTINT.W.S
  { 0b0000000100011011000010, 0, 5, 0 }, // FTINT.W.D
  { 0b0000000100011011001001, 0, 5, 0 }, // FTINT.L.S
  { 0b0000000100011011001010, 0, 5, 0 }, // FTINT.L.D
  { 0b0000000100011101000100, 0, 5, 0 }, // FFINT.S.W
  { 0b0000000100011101000110, 0, 5, 0 }, // FFINT.S.L
  { 0b0000000100011101001000, 0, 5, 0 }, // FFINT.D.W
  { 0b0000000100011101001010, 0, 5, 0 }, // FFINT.D.L
  { 0b0000000100011110010001, 0, 5, 0 }, // FRINT.S
  { 0b0000000100011110010010, 0, 5, 0 }, // FRINT.D
};

const BaseLVR baseLVR[3] = {
  { 0b0000000100010100101001, 0, kWX, 5, 0 }, // MOVGR2FR.W
  { 0b0000000100010100101010, 0, kWX, 5, 0 }, // MOVGR2FR.D
  { 0b0000000100010100101011, 0, kWX, 5, 0 }, // MOVGR2FRH.W
};

const BaseLRV baseLRV[3] = {
  { 0b0000000100010100101101, kWX, 0, 5, 0 }, // MOVFR2GR.S
  { 0b0000000100010100101110, kWX, 0, 5, 0 }, // MOVFR2GR.D
  { 0b0000000100010100101111, kWX, 0, 5, 0 }, // MOVFRH2GR.S
};

const BaseLII baseLII[2] = {
  { 0b010010, 5, 0, 3, 5, 16, 10, 0 },
  { 0b010010, 5, 0, 3, 5, 16, 10, 1 },
};

const BaseLIR baseLIR[1] = {
  //{ 0b0000000100010100110100, kWX, 5, 3, 0 },
  { 0b0000000100010100110110, kWX, 5, 0 },
};

const BaseLIV baseLIV[1] = {
  { 0b0000000100010100110100, 5, 0 },
};

const BaseLVI baseLVI[1] = {
  { 0b0000000100010100110101, 0, 5 },
};

const BaseLRRRI baseLRRRI[5] = {
  { 0b000000000000010, kWX, 0, kWX, 5, kWX, 10, 15, 0 },
  { 0b000000000000011, kWX, 0, kWX, 5, kWX, 10, 15, 0 },
  { 0b000000000000100, kWX, 0, kWX, 5, kWX, 10, 15, 1 },
  { 0b00000000000011,  kWX, 0, kWX, 5, kWX, 10, 15, 2 },
  { 0b000000000010110, kWX, 0, kWX, 5, kWX, 10, 15, 0 },
};

const BaseLRRII baseLRRII[4] = {
  { 0b00000000011, kWX, 0, kWX, 5, 10, 16, 0 },
  { 0b00000000011, kWX, 0, kWX, 5, 10, 16, 1 },
  { 0b0000000010 , kWX, 0, kWX, 5, 10, 16, 2 },
  { 0b0000000011 , kWX, 0, kWX, 5, 10, 16, 2 },
};

const BaseLIRR baseLIRR[1] = {
  { 0b00000110010010011, kWX, 5, kWX, 10, 0, 0 },
};

const BaseLRRI baseLRRI[18] = {
  { 0b0000001000, kWX, 0, kWX, 5, 10, 0 },
  { 0b0000001001, kWX, 0, kWX, 5, 10, 0 },
  { 0b0000001010, kWX, 0, kWX, 5, 10, 0 },
  { 0b0000001011, kWX, 0, kWX, 5, 10, 0 },
  { 0b0000001100, kWX, 0, kWX, 5, 10, 0 },
  { 0b0000001101, kWX, 0, kWX, 5, 10, 5 },
  { 0b0000001110, kWX, 0, kWX, 5, 10, 5 },
  { 0b0000001111, kWX, 0, kWX ,5, 10, 5 },
  { 0b00000000010000001, kWX, 0, kWX, 5, 10, 1 },
  { 0b0000000001000001, kWX, 0, kWX, 5, 10, 2 },
  { 0b00000000010001001, kWX, 0, kWX, 5, 10, 1 },
  { 0b0000000001000101, kWX, 0, kWX, 5, 10, 2 },
  { 0b00000000010010001, kWX, 0, kWX, 5, 10, 1 },
  { 0b0000000001001001, kWX, 0, kWX, 5, 10 ,2 },
  { 0b00000000010011001, kWX, 0, kWX, 5, 10, 1 },
  { 0b0000000001001101, kWX, 0, kWX, 5, 10, 2 },
  { 0b00000100, kWX, 0, kWX, 5, 10, 3 },
  { 0b00000110010000, kWX, 0, kWX, 5, 10, 4 },
};

const BaseLRRIL baseLRRIL[1] = {
  { 0b000100, kX, kSP, 10 },
};

const BaseLVVV baseLVVV[20] = {
  { 0b00000001000000001, 0, 5, 10, 0 }, //fadd.s
  { 0b00000001000000010, 0, 5, 10, 0 }, //fadd.d
  { 0b00000001000000101, 0, 5, 10, 0 }, //fsub.s
  { 0b00000001000000110, 0, 5, 10, 0 }, //fsub.d
  { 0b00000001000001001, 0, 5, 10, 0 }, //fmul.s
  { 0b00000001000001010, 0, 5, 10, 0 }, //fmul.d
  { 0b00000001000001101, 0, 5, 10, 0 }, //fdiv.s
  { 0b00000001000001110, 0, 5, 10, 0 }, //fdiv.d
  { 0b00000001000010001, 0, 5, 10, 0 }, //fmax.s
  { 0b00000001000010010, 0, 5, 10, 0 }, //fmax.d
  { 0b00000001000010101, 0, 5, 10, 0 }, //fmin.s
  { 0b00000001000010110, 0, 5, 10, 0 }, //fmin.d
  { 0b00000001000011001, 0, 5, 10, 0 }, //fmaxa.s
  { 0b00000001000011010, 0, 5, 10, 0 }, //fmaxa.d
  { 0b00000001000011101, 0, 5, 10, 0 }, //fmina.s
  { 0b00000001000011110, 0, 5, 10, 0 }, //fmina.d
  { 0b00000001000100001, 0, 5, 10, 0 }, //fscaleb.s
  { 0b00000001000100010, 0, 5, 10, 0 }, //fscaleb.d
  { 0b00000001000100101, 0, 5, 10, 0 }, //fcopysign.s
  { 0b00000001000100110, 0, 5, 10, 0 }, //fcopysign.d
};

const BaseLVRR baseLVRR[12] = {
  { 0b00111000001100000, 0, kWX, 5, kWX, 10, 0 }, //fldx.s
  { 0b00111000001101000, 0, kWX, 5, kWX, 10, 0 }, //fldx.d
  { 0b00111000001110000, 0, kWX, 5, kWX, 10, 0 }, //fstx.s
  { 0b00111000001111000, 0, kWX, 5, kWX, 10, 0 }, //fstx.d
  { 0b00111000011101000, 0, kWX, 5, kWX, 10, 0 }, //fldgt.s
  { 0b00111000011101001, 0, kWX, 5, kWX, 10, 0 }, //fldgt.d
  { 0b00111000011101010, 0, kWX, 5, kWX, 10, 0 }, //fldle.s
  { 0b00111000011101011, 0, kWX, 5, kWX, 10, 0 }, //fldle.d
  { 0b00111000011101100, 0, kWX, 5, kWX, 10, 0 }, //fstgt.s
  { 0b00111000011101101, 0, kWX, 5, kWX, 10, 0 }, //fstgt.d
  { 0b00111000011101110, 0, kWX, 5, kWX, 10, 0 }, //fstle.s
  { 0b00111000011101111, 0, kWX, 5, kWX, 10, 0 }, //fstle.d
};

const BaseLRRRT baseLRRRT[36] = {
  { 0b00111000011000000, kWX, 0, kWX, 10, kWX, 5, 0 }, //amswap.w
  { 0b00111000011000001, kWX, 0, kWX, 10, kWX, 5, 0 }, //amswap.d
  { 0b00111000011000010, kWX, 0, kWX, 10, kWX, 5, 0 }, //amadd.w
  { 0b00111000011000011, kWX, 0, kWX, 10, kWX, 5, 0 }, //amadd.d
  { 0b00111000011000100, kWX, 0, kWX, 10, kWX, 5, 0 }, //amand.w
  { 0b00111000011000101, kWX, 0, kWX, 10, kWX, 5, 0 }, //amand.d
  { 0b00111000011000110, kWX, 0, kWX, 10, kWX, 5, 0 }, //amor.w
  { 0b00111000011000111, kWX, 0, kWX, 10, kWX, 5, 0 }, //amor.d
  { 0b00111000011001000, kWX, 0, kWX, 10, kWX, 5, 0 }, //amxor.w
  { 0b00111000011001001, kWX, 0, kWX, 10, kWX, 5, 0 }, //amxor.d
  { 0b00111000011001010, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax.w
  { 0b00111000011001011, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax.d
  { 0b00111000011001100, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin.w
  { 0b00111000011001101, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin.d
  { 0b00111000011001110, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax.wu
  { 0b00111000011001111, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax.du
  { 0b00111000011010000, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin.wu
  { 0b00111000011010001, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin.du
  { 0b00111000011010010, kWX, 0, kWX, 10, kWX, 5, 0 }, //amswap_db.w
  { 0b00111000011010011, kWX, 0, kWX, 10, kWX, 5, 0 }, //amswap_db.d
  { 0b00111000011010100, kWX, 0, kWX, 10, kWX, 5, 0 }, //amadd_db.w
  { 0b00111000011010101, kWX, 0, kWX, 10, kWX, 5, 0 }, //amadd_db.d
  { 0b00111000011010110, kWX, 0, kWX, 10, kWX, 5, 0 }, //amand_db.w
  { 0b00111000011010111, kWX, 0, kWX, 10, kWX, 5, 0 }, //amand_db.d
  { 0b00111000011011000, kWX, 0, kWX, 10, kWX, 5, 0 }, //amor_db.w
  { 0b00111000011011001, kWX, 0, kWX, 10, kWX, 5, 0 }, //amor_db.d
  { 0b00111000011011010, kWX, 0, kWX, 10, kWX, 5, 0 }, //amxor_db.w
  { 0b00111000011011011, kWX, 0, kWX, 10, kWX, 5, 0 }, //amxor_db.d
  { 0b00111000011011100, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax_db.w
  { 0b00111000011011101, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax_db.d
  { 0b00111000011011110, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin_db.w
  { 0b00111000011011111, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin_db.d
  { 0b00111000011100000, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax_db.wu
  { 0b00111000011100001, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammax_db.du
  { 0b00111000011100010, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin_db.wu
  { 0b00111000011100011, kWX, 0, kWX, 10, kWX, 5, 0 }, //ammin_db.du
};

const BaseLRRR baseLRRR[73] = {
  { 0b00000000000100000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ADD.W
  { 0b00000000000100001, kWX, 0, kWX, 5, kWX, 10, 0 }, //ADD.D
  { 0b00000000000100010, kWX, 0, kWX, 5, kWX, 10, 0 }, //SUB.W
  { 0b00000000000100011, kWX, 0, kWX, 5, kWX, 10, 0 }, //SUB.D
  { 0b00000000000100100, kWX, 0, kWX, 5, kWX, 10, 0 }, //SLT
  { 0b00000000000100101, kWX, 0, kWX, 5, kWX, 10, 0 }, //SLTU
  { 0b00000000000100110, kWX, 0, kWX, 5, kWX, 10, 0 }, //maskeqz
  { 0b00000000000100111, kWX, 0, kWX, 5, kWX, 10, 0 }, //masknez
  { 0b00000000000101000, kWX, 0, kWX, 5, kWX, 10, 0 }, //nor
  { 0b00000000000101001, kWX, 0, kWX, 5, kWX, 10, 0 }, //and
  { 0b00000000000101010, kWX, 0, kWX, 5, kWX, 10, 0 }, //or
  { 0b00000000000101011, kWX, 0, kWX, 5, kWX, 10, 0 }, //xor
  { 0b00000000000101100, kWX, 0, kWX, 5, kWX, 10, 0 }, //orn
  { 0b00000000000101101, kWX, 0, kWX, 5, kWX, 10, 0 }, //andn
  { 0b00000000000101110, kWX, 0, kWX, 5, kWX, 10, 0 }, //sll.w
  { 0b00000000000101111, kWX, 0, kWX, 5, kWX, 10, 0 }, //srl.w
  { 0b00000000000110000, kWX, 0, kWX, 5, kWX, 10, 0 }, //sra.w
  { 0b00000000000110001, kWX, 0, kWX, 5, kWX, 10, 0 }, //sll.d
  { 0b00000000000110010, kWX, 0, kWX, 5, kWX, 10, 0 }, //srl.d
  { 0b00000000000110011, kWX, 0, kWX, 5, kWX, 10, 0 }, //sra.d
  { 0b00000000000110110, kWX, 0, kWX, 5, kWX, 10, 0 }, //rotr.w
  { 0b00000000000110111, kWX, 0, kWX, 5, kWX, 10, 0 }, //rotr.d
  { 0b00000000000111000, kWX, 0, kWX, 5, kWX, 10, 0 }, //mul.w
  { 0b00000000000111001, kWX, 0, kWX, 5, kWX, 10, 0 }, //mulh.w
  { 0b00000000000111010, kWX, 0, kWX, 5, kWX, 10, 0 }, //mulh.wu
  { 0b00000000000111011, kWX, 0, kWX, 5, kWX, 10, 0 }, //mul.d
  { 0b00000000000111100, kWX, 0, kWX, 5, kWX, 10, 0 }, //mulh.d
  { 0b00000000000111101, kWX, 0, kWX, 5, kWX, 10, 0 }, //mulh.du
  { 0b00000000000111110, kWX, 0, kWX, 5, kWX, 10, 0 }, //mulw.d.w
  { 0b00000000000111111, kWX, 0, kWX, 5, kWX, 10, 0 }, //mulw.d.wu
  { 0b00000000001000000, kWX, 0, kWX, 5, kWX, 10, 0 }, //div.w
  { 0b00000000001000001, kWX, 0, kWX, 5, kWX, 10, 0 }, //mod.w
  { 0b00000000001000010, kWX, 0, kWX, 5, kWX, 10, 0 }, //div.wu
  { 0b00000000001000011, kWX, 0, kWX, 5, kWX, 10, 0 }, //mod.wu
  { 0b00000000001000100, kWX, 0, kWX, 5, kWX, 10, 0 }, //div.d
  { 0b00000000001000101, kWX, 0, kWX, 5, kWX, 10, 0 }, //mod.d
  { 0b00000000001000110, kWX, 0, kWX, 5, kWX, 10, 0 }, //div.du
  { 0b00000000001000111, kWX, 0, kWX, 5, kWX, 10, 0 }, //mod.du
  { 0b00000000001001000, kWX, 0, kWX, 5, kWX, 10, 0 }, //crc.w.b.w
  { 0b00000000001001001, kWX, 0, kWX, 5, kWX, 10, 0 }, //crc.w.h.w
  { 0b00000000001001010, kWX, 0, kWX, 5, kWX, 10, 0 }, //crc.w.w.w
  { 0b00000000001001011, kWX, 0, kWX, 5, kWX, 10, 0 }, //crc.w.d.w
  { 0b00000000001001100, kWX, 0, kWX, 5, kWX, 10, 0 }, //crcc.w.b.w
  { 0b00000000001001101, kWX, 0, kWX, 5, kWX, 10, 0 }, //crcc.w.h.w
  { 0b00000000001001110, kWX, 0, kWX, 5, kWX, 10, 0 }, //crcc.w.w.w
  { 0b00000000001001111, kWX, 0, kWX, 5, kWX, 10, 0 }, //crcc.w.d.w
  { 0b00111000000000000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.b
  { 0b00111000000001000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.h
  { 0b00111000000010000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.w
  { 0b00111000000011000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.d
  { 0b00111000000100000, kWX, 0, kWX, 5, kWX, 10, 0 }, //stx.b
  { 0b00111000000101000, kWX, 0, kWX, 5, kWX, 10, 0 }, //stx.h
  { 0b00111000000110000, kWX, 0, kWX, 5, kWX, 10, 0 }, //stx.w
  { 0b00111000000111000, kWX, 0, kWX, 5, kWX, 10, 0 }, //stx.d
  { 0b00111000001000000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.bu
  { 0b00111000001001000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.hu
  { 0b00111000001010000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldx.wu
  { 0b00111000011110000, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldgt.b
  { 0b00111000011110001, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldgt.h
  { 0b00111000011110010, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldgt.w
  { 0b00111000011110011, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldgt.d
  { 0b00111000011110100, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldle.b
  { 0b00111000011110101, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldle.h
  { 0b00111000011110110, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldle.w
  { 0b00111000011110111, kWX, 0, kWX, 5, kWX, 10, 0 }, //ldle.d
  { 0b00111000011111000, kWX, 0, kWX, 5, kWX, 10, 0 }, //stgt.b
  { 0b00111000011111001, kWX, 0, kWX, 5, kWX, 10, 0 }, //stgt.h
  { 0b00111000011111010, kWX, 0, kWX, 5, kWX, 10, 0 }, //stgt.w
  { 0b00111000011111011, kWX, 0, kWX, 5, kWX, 10, 0 }, //stgt.d
  { 0b00111000011111100, kWX, 0, kWX, 5, kWX, 10, 0 }, //stle.b
  { 0b00111000011111101, kWX, 0, kWX, 5, kWX, 10, 0 }, //stle.h
  { 0b00111000011111110, kWX, 0, kWX, 5, kWX, 10, 0 }, //stle.w
  { 0b00111000011111111, kWX, 0, kWX, 5, kWX, 10, 0 }, //stle.d
};

const FpLVVVV fpLVVVV[2] = {
  { 0b000010000001 }, //FMADD.S
  { 0b000010000010 }, //FMADD.D
};

const lsxVVVV lsxvvvv[10] = {
  { 0b000010010001, 0, 5, 10, 15, 0 }, //vfmadd.s
  { 0b000010010010, 0, 5, 10, 15, 0 }, //vfmadd.d
  { 0b000010010101, 0, 5, 10, 15, 0 }, //vfmsub.s
  { 0b000010010110, 0, 5, 10, 15, 0 }, //vfmsub.d
  { 0b000010011001, 0, 5, 10, 15, 0 }, //vfnmadd.s
  { 0b000010011010, 0, 5, 10, 15, 0 }, //vfnmadd.d
  { 0b000010011101, 0, 5, 10, 15, 0 }, //vfnmsub.s
  { 0b000010011110, 0, 5, 10, 15, 0 }, //vfnmsub.d
  { 0b000011010001, 0, 5, 10, 15, 0 }, //vbitsel.v
  { 0b000011010101, 0, 5, 10, 15, 0 }, //vshuf.b
};

const lasxXXXX lasxxxxx[10] = {
  { 0b000010100001, 0, 5, 10, 15, 0 }, //xvfmadd.s
  { 0b000010100010, 0, 5, 10, 15, 0 }, //xvfmadd.d
  { 0b000010100101, 0, 5, 10, 15, 0 }, //xvfmsub.s
  { 0b000010100110, 0, 5, 10, 15, 0 }, //xvfmsub.d
  { 0b000010101001, 0, 5, 10, 15, 0 }, //xvfnmadd.s
  { 0b000010101010, 0, 5, 10, 15, 0 }, //xvfnmadd.d
  { 0b000010101101, 0, 5, 10, 15, 0 }, //xvfnmsub.s
  { 0b000010101110, 0, 5, 10, 15, 0 }, //xvfnmsub.d
  { 0b000011010010, 0, 5, 10, 15, 0 }, //xvbitsel.v
  { 0b000011010110, 0, 5, 10, 15, 0 }, //xvshuf.b
};

const lsxVVV lsxvvv[411] = {
  { 0b01110000000000000, 0, 5, 10, 0 }, //vseq.b 0
  { 0b01110000000000001, 0, 5, 10, 0 }, //vseq.h 1
  { 0b01110000000000010, 0, 5, 10, 0 }, //vseq.w 2
  { 0b01110000000000011, 0, 5, 10, 0 }, //vseq.d 3
  { 0b01110000000000100, 0, 5, 10, 0 }, //vsle.b 4
  { 0b01110000000000101, 0, 5, 10, 0 }, //vsle.h 5
  { 0b01110000000000110, 0, 5, 10, 0 }, //vsle.w 6
  { 0b01110000000000111, 0, 5, 10, 0 }, //vsle.d 7
  { 0b01110000000001000, 0, 5, 10, 0 }, //vsle.bu 8
  { 0b01110000000001001, 0, 5, 10, 0 }, //vsle.hu 9
  { 0b01110000000001010, 0, 5, 10, 0 }, //vsle.wu 10
  { 0b01110000000001011, 0, 5, 10, 0 }, //vsle.du 11
  { 0b01110000000001100, 0, 5, 10, 0 }, //vslt.b 12
  { 0b01110000000001101, 0, 5, 10, 0 }, //vslt.h 13
  { 0b01110000000001110, 0, 5, 10, 0 }, //vslt.w 14
  { 0b01110000000001111, 0, 5, 10, 0 }, //vslt.d 15
  { 0b01110000000010000, 0, 5, 10, 0 }, //vslt.bu 16
  { 0b01110000000010001, 0, 5, 10, 0 }, //vslt.hu 17
  { 0b01110000000010010, 0, 5, 10, 0 }, //vslt.wu 18
  { 0b01110000000010011, 0, 5, 10, 0 }, //vslt.du 19
  { 0b01110000000010100, 0, 5, 10, 0 }, //vadd.b 20
  { 0b01110000000010101, 0, 5, 10, 0 }, //vadd.h 21
  { 0b01110000000010110, 0, 5, 10, 0 }, //vadd.w 22
  { 0b01110000000010111, 0, 5, 10, 0 }, //vadd.d 23
  { 0b01110000000011000, 0, 5, 10, 0 }, //vsub.b 24
  { 0b01110000000011001, 0, 5, 10, 0 }, //vsub.h 25
  { 0b01110000000011010, 0, 5, 10, 0 }, //vsub.w 26
  { 0b01110000000011011, 0, 5, 10, 0 }, //vsub.d 27
  { 0b01110000000111100, 0, 5, 10, 0 }, //vaddwev.h.b 28
  { 0b01110000000111101, 0, 5, 10, 0 }, //vaddwev.w.h 29
  { 0b01110000000111110, 0, 5, 10, 0 }, //vaddwev.d.w 30
  { 0b01110000000111111, 0, 5, 10, 0 }, //vaddwev.q.d 31
  { 0b01110000001000000, 0, 5, 10, 0 }, //vsubwev.h.b 32
  { 0b01110000001000001, 0, 5, 10, 0 }, //vsubwev.w.h 33
  { 0b01110000001000010, 0, 5, 10, 0 }, //vsubwev.d.w 34
  { 0b01110000001000011, 0, 5, 10, 0 }, //vsubwev.q.d 35
  { 0b01110000001000100, 0, 5, 10, 0 }, //vaddwod.h.b 36
  { 0b01110000001000101, 0, 5, 10, 0 }, //vaddwod.w.h 37
  { 0b01110000001000110, 0, 5, 10, 0 }, //vaddwod.d.w 38
  { 0b01110000001000111, 0, 5, 10, 0 }, //vaddwod.q.d 39
  { 0b01110000001001000, 0, 5, 10, 0 }, //vsubwod.h.b 40
  { 0b01110000001001001, 0, 5, 10, 0 }, //vsubwod.w.h 41
  { 0b01110000001001010, 0, 5, 10, 0 }, //vsubwod.d.w 42
  { 0b01110000001001011, 0, 5, 10, 0 }, //vsubwod.q.d 43
  { 0b01110000001011100, 0, 5, 10, 0 }, //vaddwev.h.bu 44
  { 0b01110000001011101, 0, 5, 10, 0 }, //vaddwev.w.hu 45
  { 0b01110000001011110, 0, 5, 10, 0 }, //vaddwev.d.wu 46
  { 0b01110000001011111, 0, 5, 10, 0 }, //vaddwev.q.du 47
  { 0b01110000001100000, 0, 5, 10, 0 }, //vsubwev.h.bu 48
  { 0b01110000001100001, 0, 5, 10, 0 }, //vsubwev.w.hu 49
  { 0b01110000001100010, 0, 5, 10, 0 }, //vsubwev.d.wu 50
  { 0b01110000001100011, 0, 5, 10, 0 }, //vsubwev.q.du 51
  { 0b01110000001100100, 0, 5, 10, 0 }, //vaddwod.h.bu 52
  { 0b01110000001100101, 0, 5, 10, 0 }, //vaddwod.w.hu 53
  { 0b01110000001100110, 0, 5, 10, 0 }, //vaddwod.d.wu 54
  { 0b01110000001100111, 0, 5, 10, 0 }, //vaddwod.q.du 55
  { 0b01110000001101000, 0, 5, 10, 0 }, //vsubwod.h.bu 56
  { 0b01110000001101001, 0, 5, 10, 0 }, //vsubwod.w.hu 57
  { 0b01110000001101010, 0, 5, 10, 0 }, //vsubwod.d.wu 58
  { 0b01110000001101011, 0, 5, 10, 0 }, //vsubwod.q.du 59
  { 0b01110000001111100, 0, 5, 10, 0 }, //vaddwev.h.bu.b 60
  { 0b01110000001111101, 0, 5, 10, 0 }, //vaddwev.w.hu.h 61
  { 0b01110000001111110, 0, 5, 10, 0 }, //vaddwev.d.wu.w 62
  { 0b01110000001111111, 0, 5, 10, 0 }, //vaddwev.q.du.d 63
  { 0b01110000010000000, 0, 5, 10, 0 }, //vaddwod.h.bu.b 64
  { 0b01110000010000001, 0, 5, 10, 0 }, //vaddwod.w.hu.h 65
  { 0b01110000010000010, 0, 5, 10, 0 }, //vaddwod.d.wu.w 66
  { 0b01110000010000011, 0, 5, 10, 0 }, //vaddwod.q.du.d 67
  { 0b01110000010001100, 0, 5, 10, 0 }, //vsadd.b 68
  { 0b01110000010001101, 0, 5, 10, 0 }, //vsadd.h 69
  { 0b01110000010001110, 0, 5, 10, 0 }, //vsadd.w 70
  { 0b01110000010001111, 0, 5, 10, 0 }, //vsadd.d 71
  { 0b01110000010010000, 0, 5, 10, 0 }, //vssub.b 72
  { 0b01110000010010001, 0, 5, 10, 0 }, //vssub.h 73
  { 0b01110000010010010, 0, 5, 10, 0 }, //vssub.w 74
  { 0b01110000010010011, 0, 5, 10, 0 }, //vssub.d 75
  { 0b01110000010010100, 0, 5, 10, 0 }, //vsadd.bu 76
  { 0b01110000010010101, 0, 5, 10, 0 }, //vsadd.hu 77
  { 0b01110000010010110, 0, 5, 10, 0 }, //vsadd.wu 78
  { 0b01110000010010111, 0, 5, 10, 0 }, //vsadd.du 79
  { 0b01110000010011000, 0, 5, 10, 0 }, //vssub.bu 80
  { 0b01110000010011001, 0, 5, 10, 0 }, //vssub.hu 81
  { 0b01110000010011010, 0, 5, 10, 0 }, //vssub.wu 82
  { 0b01110000010011011, 0, 5, 10, 0 }, //vssub.du 83
  { 0b01110000010101000, 0, 5, 10, 0 }, //vhaddw.h.b 84
  { 0b01110000010101001, 0, 5, 10, 0 }, //vhaddw.w.h 85
  { 0b01110000010101010, 0, 5, 10, 0 }, //vhaddw.d.w 86
  { 0b01110000010101011, 0, 5, 10, 0 }, //vhaddw.q.d 87
  { 0b01110000010101100, 0, 5, 10, 0 }, //vhsubw.h.b 88
  { 0b01110000010101101, 0, 5, 10, 0 }, //vhsubw.w.h 89
  { 0b01110000010101110, 0, 5, 10, 0 }, //vhsubw.d.w 90
  { 0b01110000010101111, 0, 5, 10, 0 }, //vhsubw.q.d 91
  { 0b01110000010110000, 0, 5, 10, 0 }, //vhaddw.hu.bu 92
  { 0b01110000010110001, 0, 5, 10, 0 }, //vhaddw.wu.hu 93
  { 0b01110000010110010, 0, 5, 10, 0 }, //vhaddw.du.wu 94
  { 0b01110000010110011, 0, 5, 10, 0 }, //vhaddw.qu.du 95
  { 0b01110000010110100, 0, 5, 10, 0 }, //vhsubw.hu.bu 96
  { 0b01110000010110101, 0, 5, 10, 0 }, //vhsubw.wu.hu 97
  { 0b01110000010110110, 0, 5, 10, 0 }, //vhsubw.du.wu 98
  { 0b01110000010110111, 0, 5, 10, 0 }, //vhsubw.qu.du 99
  { 0b01110000010111000, 0, 5, 10, 0 }, //vadda.b 100
  { 0b01110000010111001, 0, 5, 10, 0 }, //vadda.h 101
  { 0b01110000010111010, 0, 5, 10, 0 }, //vadda.w 102
  { 0b01110000010111011, 0, 5, 10, 0 }, //vadda.d 103
  { 0b01110000011000000, 0, 5, 10, 0 }, //vabsd.b 104
  { 0b01110000011000001, 0, 5, 10, 0 }, //vabsd.h 105
  { 0b01110000011000010, 0, 5, 10, 0 }, //vabsd.w 106
  { 0b01110000011000011, 0, 5, 10, 0 }, //vabsd.d 107
  { 0b01110000011000100, 0, 5, 10, 0 }, //vabsd.bu 108
  { 0b01110000011000101, 0, 5, 10, 0 }, //vabsd.hu 109
  { 0b01110000011000110, 0, 5, 10, 0 }, //vabsd.wu 110
  { 0b01110000011000111, 0, 5, 10, 0 }, //vabsd.du 111
  { 0b01110000011001000, 0, 5, 10, 0 }, //vavg.b 112
  { 0b01110000011001001, 0, 5, 10, 0 }, //vavg.h 113
  { 0b01110000011001010, 0, 5, 10, 0 }, //vavg.w 114
  { 0b01110000011001011, 0, 5, 10, 0 }, //vavg.d 115
  { 0b01110000011001100, 0, 5, 10, 0 }, //vavg.bu 116
  { 0b01110000011001101, 0, 5, 10, 0 }, //vavg.hu 117
  { 0b01110000011001110, 0, 5, 10, 0 }, //vavg.wu 118
  { 0b01110000011001111, 0, 5, 10, 0 }, //vavg.du 119
  { 0b01110000011010000, 0, 5, 10, 0 }, //vavgr.b 120
  { 0b01110000011010001, 0, 5, 10, 0 }, //vavgr.h 121
  { 0b01110000011010010, 0, 5, 10, 0 }, //vavgr.w 122
  { 0b01110000011010011, 0, 5, 10, 0 }, //vavgr.d 123
  { 0b01110000011010100, 0, 5, 10, 0 }, //vavgr.bu 124
  { 0b01110000011010101, 0, 5, 10, 0 }, //vavgr.hu 125
  { 0b01110000011010110, 0, 5, 10, 0 }, //vavgr.wu 126
  { 0b01110000011010111, 0, 5, 10, 0 }, //vavgr.du 127
  { 0b01110000011100000, 0, 5, 10, 0 }, //vmax.b 128
  { 0b01110000011100001, 0, 5, 10, 0 }, //vmax.h 129
  { 0b01110000011100010, 0, 5, 10, 0 }, //vmax.w 130
  { 0b01110000011100011, 0, 5, 10, 0 }, //vmax.d 131
  { 0b01110000011100100, 0, 5, 10, 0 }, //vmin.b 132
  { 0b01110000011100101, 0, 5, 10, 0 }, //vmin.h 133
  { 0b01110000011100110, 0, 5, 10, 0 }, //vmin.w 134
  { 0b01110000011100111, 0, 5, 10, 0 }, //vmin.d 135
  { 0b01110000011101000, 0, 5, 10, 0 }, //vmax.bu 136
  { 0b01110000011101001, 0, 5, 10, 0 }, //vmax.hu 137
  { 0b01110000011101010, 0, 5, 10, 0 }, //vmax.wu 138
  { 0b01110000011101011, 0, 5, 10, 0 }, //vmax.du 139
  { 0b01110000011101100, 0, 5, 10, 0 }, //vmin.bu 140
  { 0b01110000011101101, 0, 5, 10, 0 }, //vmin.hu 141
  { 0b01110000011101110, 0, 5, 10, 0 }, //vmin.wu 142
  { 0b01110000011101111, 0, 5, 10, 0 }, //vmin.du 143
  { 0b01110000100001000, 0, 5, 10, 0 }, //vmul.b 144
  { 0b01110000100001001, 0, 5, 10, 0 }, //vmul.h 145
  { 0b01110000100001010, 0, 5, 10, 0 }, //vmul.w 146
  { 0b01110000100001011, 0, 5, 10, 0 }, //vmul.d 147
  { 0b01110000100001100, 0, 5, 10, 0 }, //vmuh.b 148
  { 0b01110000100001101, 0, 5, 10, 0 }, //vmuh.h 149
  { 0b01110000100001110, 0, 5, 10, 0 }, //vmuh.w 150
  { 0b01110000100001111, 0, 5, 10, 0 }, //vmuh.d 151
  { 0b01110000100010000, 0, 5, 10, 0 }, //vmuh.bu 152
  { 0b01110000100010001, 0, 5, 10, 0 }, //vmuh.hu 153
  { 0b01110000100010010, 0, 5, 10, 0 }, //vmuh.wu 154
  { 0b01110000100010011, 0, 5, 10, 0 }, //vmuh.du 155
  { 0b01110000100100000, 0, 5, 10, 0 }, //vmulwev.h.b 156
  { 0b01110000100100001, 0, 5, 10, 0 }, //vmulwev.w.h 157
  { 0b01110000100100010, 0, 5, 10, 0 }, //vmulwev.d.w 158
  { 0b01110000100100011, 0, 5, 10, 0 }, //vmulwev.q.d 159
  { 0b01110000100100100, 0, 5, 10, 0 }, //vmulwod.h.b 160
  { 0b01110000100100101, 0, 5, 10, 0 }, //vmulwod.w.h 161
  { 0b01110000100100110, 0, 5, 10, 0 }, //vmulwod.d.w 162
  { 0b01110000100100111, 0, 5, 10, 0 }, //vmulwod.q.d 163
  { 0b01110000100110000, 0, 5, 10, 0 }, //vmulwev.h.bu 164
  { 0b01110000100110001, 0, 5, 10, 0 }, //vmulwev.w.hu 165
  { 0b01110000100110010, 0, 5, 10, 0 }, //vmulwev.d.wu 166
  { 0b01110000100110011, 0, 5, 10, 0 }, //vmulwev.q.du 167
  { 0b01110000100110100, 0, 5, 10, 0 }, //vmulwod.h.bu 168
  { 0b01110000100110101, 0, 5, 10, 0 }, //vmulwod.w.hu 169
  { 0b01110000100110110, 0, 5, 10, 0 }, //vmulwod.d.wu 170
  { 0b01110000100110111, 0, 5, 10, 0 }, //vmulwod.q.du 171
  { 0b01110000101000000, 0, 5, 10, 0 }, //vmulwev.h.bu.b 172
  { 0b01110000101000001, 0, 5, 10, 0 }, //vmulwev.w.hu.h 173
  { 0b01110000101000010, 0, 5, 10, 0 }, //vmulwev.d.wu.w 174
  { 0b01110000101000011, 0, 5, 10, 0 }, //vmulwev.q.du.d 175
  { 0b01110000101000100, 0, 5, 10, 0 }, //vmulwod.h.bu.b 176
  { 0b01110000101000101, 0, 5, 10, 0 }, //vmulwod.w.hu.h 177
  { 0b01110000101000110, 0, 5, 10, 0 }, //vmulwod.d.wu.w 178
  { 0b01110000101000111, 0, 5, 10, 0 }, //vmulwod.q.du.d 179
  { 0b01110000101010000, 0, 5, 10, 0 }, //vmadd.b 180
  { 0b01110000101010001, 0, 5, 10, 0 }, //vmadd.h 181
  { 0b01110000101010010, 0, 5, 10, 0 }, //vmadd.w 182
  { 0b01110000101010011, 0, 5, 10, 0 }, //vmadd.d 183
  { 0b01110000101010100, 0, 5, 10, 0 }, //vmsub.b 184
  { 0b01110000101010101, 0, 5, 10, 0 }, //vmsub.h 185
  { 0b01110000101010110, 0, 5, 10, 0 }, //vmsub.w 186
  { 0b01110000101010111, 0, 5, 10, 0 }, //vmsub.d 187
  { 0b01110000101011000, 0, 5, 10, 0 }, //vmaddwev.h.b 188
  { 0b01110000101011001, 0, 5, 10, 0 }, //vmaddwev.w.h 189
  { 0b01110000101011010, 0, 5, 10, 0 }, //vmaddwev.d.w 190
  { 0b01110000101011011, 0, 5, 10, 0 }, //vmaddwev.q.d 191
  { 0b01110000101011100, 0, 5, 10, 0 }, //vmaddwod.h.b 192
  { 0b01110000101011101, 0, 5, 10, 0 }, //vmaddwod.w.h 193
  { 0b01110000101011110, 0, 5, 10, 0 }, //vmaddwod.d.w 194
  { 0b01110000101011111, 0, 5, 10, 0 }, //vmaddwod.q.d 195
  { 0b01110000101101000, 0, 5, 10, 0 }, //vmaddwev.h.bu 196
  { 0b01110000101101001, 0, 5, 10, 0 }, //vmaddwev.w.hu 197
  { 0b01110000101101010, 0, 5, 10, 0 }, //vmaddwev.d.wu 198
  { 0b01110000101101011, 0, 5, 10, 0 }, //vmaddwev.q.du 199
  { 0b01110000101101100, 0, 5, 10, 0 }, //vmaddwod.h.bu 200
  { 0b01110000101101101, 0, 5, 10, 0 }, //vmaddwod.w.hu 201
  { 0b01110000101101110, 0, 5, 10, 0 }, //vmaddwod.d.wu 202
  { 0b01110000101101111, 0, 5, 10, 0 }, //vmaddwod.q.du 203
  { 0b01110000101111000, 0, 5, 10, 0 }, //vmaddwev.h.bu.b 204
  { 0b01110000101111001, 0, 5, 10, 0 }, //vmaddwev.w.hu.h 205
  { 0b01110000101111010, 0, 5, 10, 0 }, //vmaddwev.d.wu.w 206
  { 0b01110000101111011, 0, 5, 10, 0 }, //vmaddwev.q.du.d 207
  { 0b01110000101111100, 0, 5, 10, 0 }, //vmaddwod.h.bu.b 208
  { 0b01110000101111101, 0, 5, 10, 0 }, //vmaddwod.w.hu.h 209
  { 0b01110000101111110, 0, 5, 10, 0 }, //vmaddwod.d.wu.w 210
  { 0b01110000101111111, 0, 5, 10, 0 }, //vmaddwod.q.du.d 211
  { 0b01110000111000000, 0, 5, 10, 0 }, //vdiv.b 212
  { 0b01110000111000001, 0, 5, 10, 0 }, //vdiv.h 213
  { 0b01110000111000010, 0, 5, 10, 0 }, //vdiv.w 214
  { 0b01110000111000011, 0, 5, 10, 0 }, //vdiv.d 215
  { 0b01110000111000100, 0, 5, 10, 0 }, //vmod.b 216
  { 0b01110000111000101, 0, 5, 10, 0 }, //vmod.h 217
  { 0b01110000111000110, 0, 5, 10, 0 }, //vmod.w 218
  { 0b01110000111000111, 0, 5, 10, 0 }, //vmod.d 219
  { 0b01110000111001000, 0, 5, 10, 0 }, //vdiv.bu 220
  { 0b01110000111001001, 0, 5, 10, 0 }, //vdiv.hu 221
  { 0b01110000111001010, 0, 5, 10, 0 }, //vdiv.wu 222
  { 0b01110000111001011, 0, 5, 10, 0 }, //vdiv.du 223
  { 0b01110000111001100, 0, 5, 10, 0 }, //vmod.bu 224
  { 0b01110000111001101, 0, 5, 10, 0 }, //vmod.hu 225
  { 0b01110000111001110, 0, 5, 10, 0 }, //vmod.wu 226
  { 0b01110000111001111, 0, 5, 10, 0 }, //vmod.du 227
  { 0b01110000111010000, 0, 5, 10, 0 }, //vsll.b 228
  { 0b01110000111010001, 0, 5, 10, 0 }, //vsll.h 229
  { 0b01110000111010010, 0, 5, 10, 0 }, //vsll.w 230
  { 0b01110000111010011, 0, 5, 10, 0 }, //vsll.d 231
  { 0b01110000111010100, 0, 5, 10, 0 }, //vsrl.b 232
  { 0b01110000111010101, 0, 5, 10, 0 }, //vsrl.h 233
  { 0b01110000111010110, 0, 5, 10, 0 }, //vsrl.w 234
  { 0b01110000111010111, 0, 5, 10, 0 }, //vsrl.d 235
  { 0b01110000111011000, 0, 5, 10, 0 }, //vsra.b 236
  { 0b01110000111011001, 0, 5, 10, 0 }, //vsra.h 237
  { 0b01110000111011010, 0, 5, 10, 0 }, //vsra.w 238
  { 0b01110000111011011, 0, 5, 10, 0 }, //vsra.d 239
  { 0b01110000111011100, 0, 5, 10, 0 }, //vrotr.b 240
  { 0b01110000111011101, 0, 5, 10, 0 }, //vrotr.h 241
  { 0b01110000111011110, 0, 5, 10, 0 }, //vrotr.w 242
  { 0b01110000111011111, 0, 5, 10, 0 }, //vrotr.d 243
  { 0b01110000111100000, 0, 5, 10, 0 }, //vsrlr.b 244
  { 0b01110000111100001, 0, 5, 10, 0 }, //vsrlr.h 245
  { 0b01110000111100010, 0, 5, 10, 0 }, //vsrlr.w 246
  { 0b01110000111100011, 0, 5, 10, 0 }, //vsrlr.d 247
  { 0b01110000111100100, 0, 5, 10, 0 }, //vsrar.b 248
  { 0b01110000111100101, 0, 5, 10, 0 }, //vsrar.h 249
  { 0b01110000111100110, 0, 5, 10, 0 }, //vsrar.w 250
  { 0b01110000111100111, 0, 5, 10, 0 }, //vsrar.d 251
  { 0b01110000111101001, 0, 5, 10, 0 }, //vsrln.b.h 252
  { 0b01110000111101010, 0, 5, 10, 0 }, //vsrln.h.w 253
  { 0b01110000111101011, 0, 5, 10, 0 }, //vsrln.w.d 254
  { 0b01110000111101101, 0, 5, 10, 0 }, //vsran.b.h 255
  { 0b01110000111101110, 0, 5, 10, 0 }, //vsran.h.w 256
  { 0b01110000111101111, 0, 5, 10, 0 }, //vsran.w.d 257
  { 0b01110000111110001, 0, 5, 10, 0 }, //vsrlrn.b.h 258
  { 0b01110000111110010, 0, 5, 10, 0 }, //vsrlrn.h.w 259
  { 0b01110000111110011, 0, 5, 10, 0 }, //vsrlrn.w.d 260
  { 0b01110000111110101, 0, 5, 10, 0 }, //vsrarn.b.h 261
  { 0b01110000111110110, 0, 5, 10, 0 }, //vsrarn.h.w 262
  { 0b01110000111110111, 0, 5, 10, 0 }, //vsrarn.w.d 263
  { 0b01110000111111001, 0, 5, 10, 0 }, //vssrln.b.h 264
  { 0b01110000111111010, 0, 5, 10, 0 }, //vssrln.h.w 265
  { 0b01110000111111011, 0, 5, 10, 0 }, //vssrln.w.d 266
  { 0b01110000111111101, 0, 5, 10, 0 }, //vssran.b.h 267
  { 0b01110000111111110, 0, 5, 10, 0 }, //vssran.h.w 268
  { 0b01110000111111111, 0, 5, 10, 0 }, //vssran.w.d 269
  { 0b01110001000000001, 0, 5, 10, 0 }, //vssrlrn.b.h 270
  { 0b01110001000000010, 0, 5, 10, 0 }, //vssrlrn.h.w 271
  { 0b01110001000000011, 0, 5, 10, 0 }, //vssrlrn.w.d 272
  { 0b01110001000000101, 0, 5, 10, 0 }, //vssrarn.b.h 273
  { 0b01110001000000110, 0, 5, 10, 0 }, //vssrarn.h.w 274
  { 0b01110001000000111, 0, 5, 10, 0 }, //vssrarn.w.d 275
  { 0b01110001000001001, 0, 5, 10, 0 }, //vssrln.bu.h 276
  { 0b01110001000001010, 0, 5, 10, 0 }, //vssrln.hu.w 277
  { 0b01110001000001011, 0, 5, 10, 0 }, //vssrln.wu.d 278
  { 0b01110001000001101, 0, 5, 10, 0 }, //vssran.bu.h 279
  { 0b01110001000001110, 0, 5, 10, 0 }, //vssran.hu.w 280
  { 0b01110001000001111, 0, 5, 10, 0 }, //vssran.wu.d 281
  { 0b01110001000010001, 0, 5, 10, 0 }, //vssrlrn.bu.h 282
  { 0b01110001000010010, 0, 5, 10, 0 }, //vssrlrn.hu.w 283
  { 0b01110001000010011, 0, 5, 10, 0 }, //vssrlrn.wu.d 284
  { 0b01110001000010101, 0, 5, 10, 0 }, //vssrarn.bu.h 285
  { 0b01110001000010110, 0, 5, 10, 0 }, //vssrarn.hu.w 286
  { 0b01110001000010111, 0, 5, 10, 0 }, //vssrarn.wu.d 287
  { 0b01110001000011000, 0, 5, 10, 0 }, //vbitclr.b 288
  { 0b01110001000011001, 0, 5, 10, 0 }, //vbitclr.h 289
  { 0b01110001000011010, 0, 5, 10, 0 }, //vbitclr.w 290
  { 0b01110001000011011, 0, 5, 10, 0 }, //vbitclr.d 291
  { 0b01110001000011100, 0, 5, 10, 0 }, //vbitset.b 292
  { 0b01110001000011101, 0, 5, 10, 0 }, //vbitset.h 293
  { 0b01110001000011110, 0, 5, 10, 0 }, //vbitset.w 294
  { 0b01110001000011111, 0, 5, 10, 0 }, //vbitset.d 295
  { 0b01110001000100000, 0, 5, 10, 0 }, //vbitrev.b 296
  { 0b01110001000100001, 0, 5, 10, 0 }, //vbitrev.h 297
  { 0b01110001000100010, 0, 5, 10, 0 }, //vbitrev.w 298
  { 0b01110001000100011, 0, 5, 10, 0 }, //vbitrev.d 299
  { 0b01110001000101100, 0, 5, 10, 0 }, //vpackev.b 300
  { 0b01110001000101101, 0, 5, 10, 0 }, //vpackev.h 301
  { 0b01110001000101110, 0, 5, 10, 0 }, //vpackev.w 302
  { 0b01110001000101111, 0, 5, 10, 0 }, //vpackev.d 303
  { 0b01110001000110000, 0, 5, 10, 0 }, //vpackod.b 304
  { 0b01110001000110001, 0, 5, 10, 0 }, //vpackod.h 305
  { 0b01110001000110010, 0, 5, 10, 0 }, //vpackod.w 306
  { 0b01110001000110011, 0, 5, 10, 0 }, //vpackod.d 307
  { 0b01110001000110100, 0, 5, 10, 0 }, //vilvl.b 308
  { 0b01110001000110101, 0, 5, 10, 0 }, //vilvl.h 309
  { 0b01110001000110110, 0, 5, 10, 0 }, //vilvl.w 310
  { 0b01110001000110111, 0, 5, 10, 0 }, //vilvl.d 311
  { 0b01110001000111000, 0, 5, 10, 0 }, //vilvh.b 312
  { 0b01110001000111001, 0, 5, 10, 0 }, //vilvh.h 313
  { 0b01110001000111010, 0, 5, 10, 0 }, //vilvh.w 314
  { 0b01110001000111011, 0, 5, 10, 0 }, //vilvh.d 315
  { 0b01110001000111100, 0, 5, 10, 0 }, //vpickev.b 316
  { 0b01110001000111101, 0, 5, 10, 0 }, //vpickev.h 317
  { 0b01110001000111110, 0, 5, 10, 0 }, //vpickev.w 318
  { 0b01110001000111111, 0, 5, 10, 0 }, //vpickev.d 319
  { 0b01110001001000000, 0, 5, 10, 0 }, //vpickod.b 320
  { 0b01110001001000001, 0, 5, 10, 0 }, //vpickod.h 321
  { 0b01110001001000010, 0, 5, 10, 0 }, //vpickod.w 322
  { 0b01110001001000011, 0, 5, 10, 0 }, //vpickod.d 323
  { 0b01110001001001100, 0, 5, 10, 0 }, //vand.v 324
  { 0b01110001001001101, 0, 5, 10, 0 }, //vor.v 325
  { 0b01110001001001110, 0, 5, 10, 0 }, //vxor.v 326
  { 0b01110001001001111, 0, 5, 10, 0 }, //vnor.v 327
  { 0b01110001001010000, 0, 5, 10, 0 }, //vandn.v 328
  { 0b01110001001010001, 0, 5, 10, 0 }, //vorn.v 329
  { 0b01110001001010110, 0, 5, 10, 0 }, //vfrstp.b 330
  { 0b01110001001010111, 0, 5, 10, 0 }, //vfrstp.h 331
  { 0b01110001001011010, 0, 5, 10, 0 }, //vadd.q 332
  { 0b01110001001011011, 0, 5, 10, 0 }, //vsub.q 333
  { 0b01110001001011100, 0, 5, 10, 0 }, //vsigncov.b 334
  { 0b01110001001011101, 0, 5, 10, 0 }, //vsigncov.h 335
  { 0b01110001001011110, 0, 5, 10, 0 }, //vsigncov.w 336
  { 0b01110001001011111, 0, 5, 10, 0 }, //vsigncov.d 337
  { 0b01110001001100001, 0, 5, 10, 0 }, //vfadd.s 338
  { 0b01110001001100010, 0, 5, 10, 0 }, //vfadd.d 339
  { 0b01110001001100101, 0, 5, 10, 0 }, //vfsub.s 340
  { 0b01110001001100110, 0, 5, 10, 0 }, //vfsub.d 341
  { 0b01110001001110001, 0, 5, 10, 0 }, //vfmul.s 342
  { 0b01110001001110010, 0, 5, 10, 0 }, //vfmul.d 343
  { 0b01110001001110101, 0, 5, 10, 0 }, //vfdiv.s 344
  { 0b01110001001110110, 0, 5, 10, 0 }, //vfdiv.d 345
  { 0b01110001001111001, 0, 5, 10, 0 }, //vfmax.s 346
  { 0b01110001001111010, 0, 5, 10, 0 }, //vfmax.d 347
  { 0b01110001001111101, 0, 5, 10, 0 }, //vfmin.s 348
  { 0b01110001001111110, 0, 5, 10, 0 }, //vfmin.d 349
  { 0b01110001010000001, 0, 5, 10, 0 }, //vfmaxa.s 350
  { 0b01110001010000010, 0, 5, 10, 0 }, //vfmaxa.d 351
  { 0b01110001010000101, 0, 5, 10, 0 }, //vfmina.s 352
  { 0b01110001010000110, 0, 5, 10, 0 }, //vfmina.d 353
  { 0b01110001010001001, 0, 5, 10, 0 }, //vfscaleb.s 354
  { 0b01110001010001010, 0, 5, 10, 0 }, //vfscaleb.d 355
  { 0b01110001010001100, 0, 5, 10, 0 }, //vfcvt.h.s 356
  { 0b01110001010001101, 0, 5, 10, 0 }, //vfcvt.s.d 357
  { 0b01110001010010000, 0, 5, 10, 0 }, //vffint.s.l 358
  { 0b01110001010010011, 0, 5, 10, 0 }, //vftint.w.d 359
  { 0b01110001010010100, 0, 5, 10, 0 }, //vftintrm.w.d 360
  { 0b01110001010010101, 0, 5, 10, 0 }, //vftintrp.w.d 361
  { 0b01110001010010110, 0, 5, 10, 0 }, //vftintrz.w.d 362
  { 0b01110001010010111, 0, 5, 10, 0 }, //vftintrne.w.d 363
  { 0b01110001011110101, 0, 5, 10, 0 }, //vshuf.h 364
  { 0b01110001011110110, 0, 5, 10, 0 }, //vshuf.w 365
  { 0b01110001011110111, 0, 5, 10, 0 }, //vshuf.d 366
  { 0b00001100010100000, 0, 5, 10, 0 }, //vfcmp.caf.s 367
  { 0b00001100010101000, 0, 5, 10, 0 }, //vfcmp.cun.s 368
  { 0b00001100010100100, 0, 5, 10, 0 }, //vfcmp.ceq.s 369
  { 0b00001100010101100, 0, 5, 10, 0 }, //vfcmp.cueq.s 370
  { 0b00001100010100010, 0, 5, 10, 0 }, //vfcmp.clt.s 371
  { 0b00001100010101010, 0, 5, 10, 0 }, //vfcmp.cult.s 372
  { 0b00001100010100110, 0, 5, 10, 0 }, //vfcmp.cle.s 373
  { 0b00001100010101110, 0, 5, 10, 0 }, //vfcmp.cule.s 374
  { 0b00001100010110000, 0, 5, 10, 0 }, //vfcmp.cne.s 375
  { 0b00001100010110100, 0, 5, 10, 0 }, //vfcmp.cor.s 376
  { 0b00001100010111000, 0, 5, 10, 0 }, //vfcmp.cune.s 377
  { 0b00001100010100001, 0, 5, 10, 0 }, //vfcmp.saf.s 378
  { 0b00001100010101001, 0, 5, 10, 0 }, //vfcmp.sun.s 379
  { 0b00001100010100101, 0, 5, 10, 0 }, //vfcmp.seq.s 380
  { 0b00001100010101101, 0, 5, 10, 0 }, //vfcmp.sueq.s 381
  { 0b00001100010100011, 0, 5, 10, 0 }, //vfcmp.slt.s 382
  { 0b00001100010101011, 0, 5, 10, 0 }, //vfcmp.sult.s 383
  { 0b00001100010100111, 0, 5, 10, 0 }, //vfcmp.sle.s 384
  { 0b00001100010101111, 0, 5, 10, 0 }, //vfcmp.sule.s 385
  { 0b00001100010110001, 0, 5, 10, 0 }, //vfcmp.sne.s 386
  { 0b00001100010110101, 0, 5, 10, 0 }, //vfcmp.sor.s 387
  { 0b00001100010111001, 0, 5, 10, 0 }, //vfcmp.sune.s 388
  { 0b00001100011000000, 0, 5, 10, 0 }, //vfcmp.caf.d 389
  { 0b00001100011001000, 0, 5, 10, 0 }, //vfcmp.cun.d 390
  { 0b00001100011000100, 0, 5, 10, 0 }, //vfcmp.ceq.d 391
  { 0b00001100011001100, 0, 5, 10, 0 }, //vfcmp.cueq.d 392
  { 0b00001100011000010, 0, 5, 10, 0 }, //vfcmp.clt.d 393
  { 0b00001100011001010, 0, 5, 10, 0 }, //vfcmp.cult.d 394
  { 0b00001100011000110, 0, 5, 10, 0 }, //vfcmp.cle.d 395
  { 0b00001100011001110, 0, 5, 10, 0 }, //vfcmp.cule.d 396
  { 0b00001100011010000, 0, 5, 10, 0 }, //vfcmp.cne.d 397
  { 0b00001100011010100, 0, 5, 10, 0 }, //vfcmp.cor.d 398
  { 0b00001100011011000, 0, 5, 10, 0 }, //vfcmp.cune.d 399
  { 0b00001100011000001, 0, 5, 10, 0 }, //vfcmp.saf.d 400
  { 0b00001100011001001, 0, 5, 10, 0 }, //vfcmp.sun.d 401
  { 0b00001100011000101, 0, 5, 10, 0 }, //vfcmp.seq.d 402
  { 0b00001100011001101, 0, 5, 10, 0 }, //vfcmp.sueq.d 403
  { 0b00001100011000011, 0, 5, 10, 0 }, //vfcmp.slt.d 404
  { 0b00001100011001011, 0, 5, 10, 0 }, //vfcmp.sult.d 405
  { 0b00001100011000111, 0, 5, 10, 0 }, //vfcmp.sle.d 406
  { 0b00001100011001111, 0, 5, 10, 0 }, //vfcmp.sule.d 407
  { 0b00001100011010001, 0, 5, 10, 0 }, //vfcmp.sne.d 408
  { 0b00001100011010101, 0, 5, 10, 0 }, //vfcmp.sor.d 409
  { 0b00001100011011001, 0, 5, 10, 0 }, //vfcmp.sune.d 410
};

const lasxXXX lasxxxx[412] = {
  { 0b01110100000000000, 0, 5, 10, 0 }, //xvseq.b
  { 0b01110100000000001, 0, 5, 10, 0 }, //xvseq.h
  { 0b01110100000000010, 0, 5, 10, 0 }, //xvseq.w
  { 0b01110100000000011, 0, 5, 10, 0 }, //xvseq.d
  { 0b01110100000000100, 0, 5, 10, 0 }, //xvsle.b
  { 0b01110100000000101, 0, 5, 10, 0 }, //xvsle.h
  { 0b01110100000000110, 0, 5, 10, 0 }, //xvsle.w
  { 0b01110100000000111, 0, 5, 10, 0 }, //xvsle.d
  { 0b01110100000001000, 0, 5, 10, 0 }, //xvsle.bu
  { 0b01110100000001001, 0, 5, 10, 0 }, //xvsle.hu
  { 0b01110100000001010, 0, 5, 10, 0 }, //xvsle.wu
  { 0b01110100000001011, 0, 5, 10, 0 }, //xvsle.du
  { 0b01110100000001100, 0, 5, 10, 0 }, //xvslt.b
  { 0b01110100000001101, 0, 5, 10, 0 }, //xvslt.h
  { 0b01110100000001110, 0, 5, 10, 0 }, //xvslt.w
  { 0b01110100000001111, 0, 5, 10, 0 }, //xvslt.d
  { 0b01110100000010000, 0, 5, 10, 0 }, //xvslt.bu
  { 0b01110100000010001, 0, 5, 10, 0 }, //xvslt.hu
  { 0b01110100000010010, 0, 5, 10, 0 }, //xvslt.wu
  { 0b01110100000010011, 0, 5, 10, 0 }, //xvslt.du
  { 0b01110100000010100, 0, 5, 10, 0 }, //xvadd.b
  { 0b01110100000010101, 0, 5, 10, 0 }, //xvadd.h
  { 0b01110100000010110, 0, 5, 10, 0 }, //xvadd.w
  { 0b01110100000010111, 0, 5, 10, 0 }, //xvadd.d
  { 0b01110100000011000, 0, 5, 10, 0 }, //xvsub.b
  { 0b01110100000011001, 0, 5, 10, 0 }, //xvsub.h
  { 0b01110100000011010, 0, 5, 10, 0 }, //xvsub.w
  { 0b01110100000011011, 0, 5, 10, 0 }, //xvsub.d
  { 0b01110100000111100, 0, 5, 10, 0 }, //xvaddwev.h.b
  { 0b01110100000111101, 0, 5, 10, 0 }, //xvaddwev.w.h
  { 0b01110100000111110, 0, 5, 10, 0 }, //xvaddwev.d.w
  { 0b01110100000111111, 0, 5, 10, 0 }, //xvaddwev.q.d
  { 0b01110100001000000, 0, 5, 10, 0 }, //xvsubwev.h.b
  { 0b01110100001000001, 0, 5, 10, 0 }, //xvsubwev.w.h
  { 0b01110100001000010, 0, 5, 10, 0 }, //xvsubwev.d.w
  { 0b01110100001000011, 0, 5, 10, 0 }, //xvsubwev.q.d
  { 0b01110100001000100, 0, 5, 10, 0 }, //xvaddwod.h.b
  { 0b01110100001000101, 0, 5, 10, 0 }, //xvaddwod.w.h
  { 0b01110100001000110, 0, 5, 10, 0 }, //xvaddwod.d.w
  { 0b01110100001000111, 0, 5, 10, 0 }, //xvaddwod.q.d
  { 0b01110100001001000, 0, 5, 10, 0 }, //xvsubwod.h.b
  { 0b01110100001001001, 0, 5, 10, 0 }, //xvsubwod.w.h
  { 0b01110100001001010, 0, 5, 10, 0 }, //xvsubwod.d.w
  { 0b01110100001001011, 0, 5, 10, 0 }, //xvsubwod.q.d
  { 0b01110100001011100, 0, 5, 10, 0 }, //xvaddwev.h.bu
  { 0b01110100001011101, 0, 5, 10, 0 }, //xvaddwev.w.hu
  { 0b01110100001011110, 0, 5, 10, 0 }, //xvaddwev.d.wu
  { 0b01110100001011111, 0, 5, 10, 0 }, //xvaddwev.q.du
  { 0b01110100001100000, 0, 5, 10, 0 }, //xvsubwev.h.bu
  { 0b01110100001100001, 0, 5, 10, 0 }, //xvsubwev.w.hu
  { 0b01110100001100010, 0, 5, 10, 0 }, //xvsubwev.d.wu
  { 0b01110100001100011, 0, 5, 10, 0 }, //xvsubwev.q.du
  { 0b01110100001100100, 0, 5, 10, 0 }, //xvaddwod.h.bu
  { 0b01110100001100101, 0, 5, 10, 0 }, //xvaddwod.w.hu
  { 0b01110100001100110, 0, 5, 10, 0 }, //xvaddwod.d.wu
  { 0b01110100001100111, 0, 5, 10, 0 }, //xvaddwod.q.du
  { 0b01110100001101000, 0, 5, 10, 0 }, //xvsubwod.h.bu
  { 0b01110100001101001, 0, 5, 10, 0 }, //xvsubwod.w.hu
  { 0b01110100001101010, 0, 5, 10, 0 }, //xvsubwod.d.wu
  { 0b01110100001101011, 0, 5, 10, 0 }, //xvsubwod.q.du
  { 0b01110100001111100, 0, 5, 10, 0 }, //xvaddwev.h.bu.b
  { 0b01110100001111101, 0, 5, 10, 0 }, //xvaddwev.w.hu.h
  { 0b01110100001111110, 0, 5, 10, 0 }, //xvaddwev.d.wu.w
  { 0b01110100001111111, 0, 5, 10, 0 }, //xvaddwev.q.du.d
  { 0b01110100010000000, 0, 5, 10, 0 }, //xvaddwod.h.bu.b
  { 0b01110100010000001, 0, 5, 10, 0 }, //xvaddwod.w.hu.h
  { 0b01110100010000010, 0, 5, 10, 0 }, //xvaddwod.d.wu.w
  { 0b01110100010000011, 0, 5, 10, 0 }, //xvaddwod.q.du.d
  { 0b01110100010001100, 0, 5, 10, 0 }, //xvsadd.b
  { 0b01110100010001101, 0, 5, 10, 0 }, //xvsadd.h
  { 0b01110100010001110, 0, 5, 10, 0 }, //xvsadd.w
  { 0b01110100010001111, 0, 5, 10, 0 }, //xvsadd.d
  { 0b01110100010010000, 0, 5, 10, 0 }, //xvssub.b
  { 0b01110100010010001, 0, 5, 10, 0 }, //xvssub.h
  { 0b01110100010010010, 0, 5, 10, 0 }, //xvssub.w
  { 0b01110100010010011, 0, 5, 10, 0 }, //xvssub.d
  { 0b01110100010010100, 0, 5, 10, 0 }, //xvsadd.bu
  { 0b01110100010010101, 0, 5, 10, 0 }, //xvsadd.hu
  { 0b01110100010010110, 0, 5, 10, 0 }, //xvsadd.wu
  { 0b01110100010010111, 0, 5, 10, 0 }, //xvsadd.du
  { 0b01110100010011000, 0, 5, 10, 0 }, //xvssub.bu
  { 0b01110100010011001, 0, 5, 10, 0 }, //xvssub.hu
  { 0b01110100010011010, 0, 5, 10, 0 }, //xvssub.wu
  { 0b01110100010011011, 0, 5, 10, 0 }, //xvssub.du
  { 0b01110100010101000, 0, 5, 10, 0 }, //xvhaddw.h.b
  { 0b01110100010101001, 0, 5, 10, 0 }, //xvhaddw.w.h
  { 0b01110100010101010, 0, 5, 10, 0 }, //xvhaddw.d.w
  { 0b01110100010101011, 0, 5, 10, 0 }, //xvhaddw.q.d
  { 0b01110100010101100, 0, 5, 10, 0 }, //xvhsubw.h.b
  { 0b01110100010101101, 0, 5, 10, 0 }, //xvhsubw.w.h
  { 0b01110100010101110, 0, 5, 10, 0 }, //xvhsubw.d.w
  { 0b01110100010101111, 0, 5, 10, 0 }, //xvhsubw.q.d
  { 0b01110100010110000, 0, 5, 10, 0 }, //xvhaddw.hu.bu
  { 0b01110100010110001, 0, 5, 10, 0 }, //xvhaddw.wu.hu
  { 0b01110100010110010, 0, 5, 10, 0 }, //xvhaddw.du.wu
  { 0b01110100010110011, 0, 5, 10, 0 }, //xvhaddw.qu.du
  { 0b01110100010110100, 0, 5, 10, 0 }, //xvhsubw.hu.bu
  { 0b01110100010110101, 0, 5, 10, 0 }, //xvhsubw.wu.hu
  { 0b01110100010110110, 0, 5, 10, 0 }, //xvhsubw.du.wu
  { 0b01110100010110111, 0, 5, 10, 0 }, //xvhsubw.qu.du
  { 0b01110100010111000, 0, 5, 10, 0 }, //xvadda.b
  { 0b01110100010111001, 0, 5, 10, 0 }, //xvadda.h
  { 0b01110100010111010, 0, 5, 10, 0 }, //xvadda.w
  { 0b01110100010111011, 0, 5, 10, 0 }, //xvadda.d
  { 0b01110100011000000, 0, 5, 10, 0 }, //xvabsd.b
  { 0b01110100011000001, 0, 5, 10, 0 }, //xvabsd.h
  { 0b01110100011000010, 0, 5, 10, 0 }, //xvabsd.w
  { 0b01110100011000011, 0, 5, 10, 0 }, //xvabsd.d
  { 0b01110100011000100, 0, 5, 10, 0 }, //xvabsd.bu
  { 0b01110100011000101, 0, 5, 10, 0 }, //xvabsd.hu
  { 0b01110100011000110, 0, 5, 10, 0 }, //xvabsd.wu
  { 0b01110100011000111, 0, 5, 10, 0 }, //xvabsd.du
  { 0b01110100011001000, 0, 5, 10, 0 }, //xvavg.b
  { 0b01110100011001001, 0, 5, 10, 0 }, //xvavg.h
  { 0b01110100011001010, 0, 5, 10, 0 }, //xvavg.w
  { 0b01110100011001011, 0, 5, 10, 0 }, //xvavg.d
  { 0b01110100011001100, 0, 5, 10, 0 }, //xvavg.bu
  { 0b01110100011001101, 0, 5, 10, 0 }, //xvavg.hu
  { 0b01110100011001110, 0, 5, 10, 0 }, //xvavg.wu
  { 0b01110100011001111, 0, 5, 10, 0 }, //xvavg.du
  { 0b01110100011010000, 0, 5, 10, 0 }, //xvavgr.b
  { 0b01110100011010001, 0, 5, 10, 0 }, //xvavgr.h
  { 0b01110100011010010, 0, 5, 10, 0 }, //xvavgr.w
  { 0b01110100011010011, 0, 5, 10, 0 }, //xvavgr.d
  { 0b01110100011010100, 0, 5, 10, 0 }, //xvavgr.bu
  { 0b01110100011010101, 0, 5, 10, 0 }, //xvavgr.hu
  { 0b01110100011010110, 0, 5, 10, 0 }, //xvavgr.wu
  { 0b01110100011010111, 0, 5, 10, 0 }, //xvavgr.du
  { 0b01110100011100000, 0, 5, 10, 0 }, //xvmax.b
  { 0b01110100011100001, 0, 5, 10, 0 }, //xvmax.h
  { 0b01110100011100010, 0, 5, 10, 0 }, //xvmax.w
  { 0b01110100011100011, 0, 5, 10, 0 }, //xvmax.d
  { 0b01110100011100100, 0, 5, 10, 0 }, //xvmin.b
  { 0b01110100011100101, 0, 5, 10, 0 }, //xvmin.h
  { 0b01110100011100110, 0, 5, 10, 0 }, //xvmin.w
  { 0b01110100011100111, 0, 5, 10, 0 }, //xvmin.d
  { 0b01110100011101000, 0, 5, 10, 0 }, //xvmax.bu
  { 0b01110100011101001, 0, 5, 10, 0 }, //xvmax.hu
  { 0b01110100011101010, 0, 5, 10, 0 }, //xvmax.wu
  { 0b01110100011101011, 0, 5, 10, 0 }, //xvmax.du
  { 0b01110100011101100, 0, 5, 10, 0 }, //xvmin.bu
  { 0b01110100011101101, 0, 5, 10, 0 }, //xvmin.hu
  { 0b01110100011101110, 0, 5, 10, 0 }, //xvmin.wu
  { 0b01110100011101111, 0, 5, 10, 0 }, //xvmin.du
  { 0b01110100100001000, 0, 5, 10, 0 }, //xvmul.b
  { 0b01110100100001001, 0, 5, 10, 0 }, //xvmul.h
  { 0b01110100100001010, 0, 5, 10, 0 }, //xvmul.w
  { 0b01110100100001011, 0, 5, 10, 0 }, //xvmul.d
  { 0b01110100100001100, 0, 5, 10, 0 }, //xvmuh.b
  { 0b01110100100001101, 0, 5, 10, 0 }, //xvmuh.h
  { 0b01110100100001110, 0, 5, 10, 0 }, //xvmuh.w
  { 0b01110100100001111, 0, 5, 10, 0 }, //xvmuh.d
  { 0b01110100100010000, 0, 5, 10, 0 }, //xvmuh.bu
  { 0b01110100100010001, 0, 5, 10, 0 }, //xvmuh.hu
  { 0b01110100100010010, 0, 5, 10, 0 }, //xvmuh.wu
  { 0b01110100100010011, 0, 5, 10, 0 }, //xvmuh.du
  { 0b01110100100100000, 0, 5, 10, 0 }, //xvmulwev.h.b
  { 0b01110100100100001, 0, 5, 10, 0 }, //xvmulwev.w.h
  { 0b01110100100100010, 0, 5, 10, 0 }, //xvmulwev.d.w
  { 0b01110100100100011, 0, 5, 10, 0 }, //xvmulwev.q.d
  { 0b01110100100100100, 0, 5, 10, 0 }, //xvmulwod.h.b
  { 0b01110100100100101, 0, 5, 10, 0 }, //xvmulwod.w.h
  { 0b01110100100100110, 0, 5, 10, 0 }, //xvmulwod.d.w
  { 0b01110100100100111, 0, 5, 10, 0 }, //xvmulwod.q.d
  { 0b01110100100110000, 0, 5, 10, 0 }, //xvmulwev.h.bu
  { 0b01110100100110001, 0, 5, 10, 0 }, //xvmulwev.w.hu
  { 0b01110100100110010, 0, 5, 10, 0 }, //xvmulwev.d.wu
  { 0b01110100100110011, 0, 5, 10, 0 }, //xvmulwev.q.du
  { 0b01110100100110100, 0, 5, 10, 0 }, //xvmulwod.h.bu
  { 0b01110100100110101, 0, 5, 10, 0 }, //xvmulwod.w.hu
  { 0b01110100100110110, 0, 5, 10, 0 }, //xvmulwod.d.wu
  { 0b01110100100110111, 0, 5, 10, 0 }, //xvmulwod.q.du
  { 0b01110100101000000, 0, 5, 10, 0 }, //xvmulwev.h.bu.b
  { 0b01110100101000001, 0, 5, 10, 0 }, //xvmulwev.w.hu.h
  { 0b01110100101000010, 0, 5, 10, 0 }, //xvmulwev.d.wu.w
  { 0b01110100101000011, 0, 5, 10, 0 }, //xvmulwev.q.du.d
  { 0b01110100101000100, 0, 5, 10, 0 }, //xvmulwod.h.bu.b
  { 0b01110100101000101, 0, 5, 10, 0 }, //xvmulwod.w.hu.h
  { 0b01110100101000110, 0, 5, 10, 0 }, //xvmulwod.d.wu.w
  { 0b01110100101000111, 0, 5, 10, 0 }, //xvmulwod.q.du.d
  { 0b01110100101010000, 0, 5, 10, 0 }, //xvmadd.b
  { 0b01110100101010001, 0, 5, 10, 0 }, //xvmadd.h
  { 0b01110100101010010, 0, 5, 10, 0 }, //xvmadd.w
  { 0b01110100101010011, 0, 5, 10, 0 }, //xvmadd.d
  { 0b01110100101010100, 0, 5, 10, 0 }, //xvmsub.b
  { 0b01110100101010101, 0, 5, 10, 0 }, //xvmsub.h
  { 0b01110100101010110, 0, 5, 10, 0 }, //xvmsub.w
  { 0b01110100101010111, 0, 5, 10, 0 }, //xvmsub.d
  { 0b01110100101011000, 0, 5, 10, 0 }, //xvmaddwev.h.b
  { 0b01110100101011001, 0, 5, 10, 0 }, //xvmaddwev.w.h
  { 0b01110100101011010, 0, 5, 10, 0 }, //xvmaddwev.d.w
  { 0b01110100101011011, 0, 5, 10, 0 }, //xvmaddwev.q.d
  { 0b01110100101011100, 0, 5, 10, 0 }, //xvmaddwod.h.b
  { 0b01110100101011101, 0, 5, 10, 0 }, //xvmaddwod.w.h
  { 0b01110100101011110, 0, 5, 10, 0 }, //xvmaddwod.d.w
  { 0b01110100101011111, 0, 5, 10, 0 }, //xvmaddwod.q.d
  { 0b01110100101101000, 0, 5, 10, 0 }, //xvmaddwev.h.bu
  { 0b01110100101101001, 0, 5, 10, 0 }, //xvmaddwev.w.hu
  { 0b01110100101101010, 0, 5, 10, 0 }, //xvmaddwev.d.wu
  { 0b01110100101101011, 0, 5, 10, 0 }, //xvmaddwev.q.du
  { 0b01110100101101100, 0, 5, 10, 0 }, //xvmaddwod.h.bu
  { 0b01110100101101101, 0, 5, 10, 0 }, //xvmaddwod.w.hu
  { 0b01110100101101110, 0, 5, 10, 0 }, //xvmaddwod.d.wu
  { 0b01110100101101111, 0, 5, 10, 0 }, //xvmaddwod.q.du
  { 0b01110100101111000, 0, 5, 10, 0 }, //xvmaddwev.h.bu.b
  { 0b01110100101111001, 0, 5, 10, 0 }, //xvmaddwev.w.hu.h
  { 0b01110100101111010, 0, 5, 10, 0 }, //xvmaddwev.d.wu.w
  { 0b01110100101111011, 0, 5, 10, 0 }, //xvmaddwev.q.du.d
  { 0b01110100101111100, 0, 5, 10, 0 }, //xvmaddwod.h.bu.b
  { 0b01110100101111101, 0, 5, 10, 0 }, //xvmaddwod.w.hu.h
  { 0b01110100101111110, 0, 5, 10, 0 }, //xvmaddwod.d.wu.w
  { 0b01110100101111111, 0, 5, 10, 0 }, //xvmaddwod.q.du.d
  { 0b01110100111000000, 0, 5, 10, 0 }, //xvdiv.b
  { 0b01110100111000001, 0, 5, 10, 0 }, //xvdiv.h
  { 0b01110100111000010, 0, 5, 10, 0 }, //xvdiv.w
  { 0b01110100111000011, 0, 5, 10, 0 }, //xvdiv.d
  { 0b01110100111000100, 0, 5, 10, 0 }, //xvmod.b
  { 0b01110100111000101, 0, 5, 10, 0 }, //xvmod.h
  { 0b01110100111000110, 0, 5, 10, 0 }, //xvmod.w
  { 0b01110100111000111, 0, 5, 10, 0 }, //xvmod.d
  { 0b01110100111001000, 0, 5, 10, 0 }, //xvdiv.bu
  { 0b01110100111001001, 0, 5, 10, 0 }, //xvdiv.hu
  { 0b01110100111001010, 0, 5, 10, 0 }, //xvdiv.wu
  { 0b01110100111001011, 0, 5, 10, 0 }, //xvdiv.du
  { 0b01110100111001100, 0, 5, 10, 0 }, //xvmod.bu
  { 0b01110100111001101, 0, 5, 10, 0 }, //xvmod.hu
  { 0b01110100111001110, 0, 5, 10, 0 }, //xvmod.wu
  { 0b01110100111001111, 0, 5, 10, 0 }, //xvmod.du
  { 0b01110100111010000, 0, 5, 10, 0 }, //xvsll.b
  { 0b01110100111010001, 0, 5, 10, 0 }, //xvsll.h
  { 0b01110100111010010, 0, 5, 10, 0 }, //xvsll.w
  { 0b01110100111010011, 0, 5, 10, 0 }, //xvsll.d
  { 0b01110100111010100, 0, 5, 10, 0 }, //xvsrl.b
  { 0b01110100111010101, 0, 5, 10, 0 }, //xvsrl.h
  { 0b01110100111010110, 0, 5, 10, 0 }, //xvsrl.w
  { 0b01110100111010111, 0, 5, 10, 0 }, //xvsrl.d
  { 0b01110100111011000, 0, 5, 10, 0 }, //xvsra.b
  { 0b01110100111011001, 0, 5, 10, 0 }, //xvsra.h
  { 0b01110100111011010, 0, 5, 10, 0 }, //xvsra.w
  { 0b01110100111011011, 0, 5, 10, 0 }, //xvsra.d
  { 0b01110100111011100, 0, 5, 10, 0 }, //xvrotr.b
  { 0b01110100111011101, 0, 5, 10, 0 }, //xvrotr.h
  { 0b01110100111011110, 0, 5, 10, 0 }, //xvrotr.w
  { 0b01110100111011111, 0, 5, 10, 0 }, //xvrotr.d
  { 0b01110100111100000, 0, 5, 10, 0 }, //xvsrlr.b
  { 0b01110100111100001, 0, 5, 10, 0 }, //xvsrlr.h
  { 0b01110100111100010, 0, 5, 10, 0 }, //xvsrlr.w
  { 0b01110100111100011, 0, 5, 10, 0 }, //xvsrlr.d
  { 0b01110100111100100, 0, 5, 10, 0 }, //xvsrar.b
  { 0b01110100111100101, 0, 5, 10, 0 }, //xvsrar.h
  { 0b01110100111100110, 0, 5, 10, 0 }, //xvsrar.w
  { 0b01110100111100111, 0, 5, 10, 0 }, //xvsrar.d
  { 0b01110100111101001, 0, 5, 10, 0 }, //xvsrln.b.h
  { 0b01110100111101010, 0, 5, 10, 0 }, //xvsrln.h.w
  { 0b01110100111101011, 0, 5, 10, 0 }, //xvsrln.w.d
  { 0b01110100111101101, 0, 5, 10, 0 }, //xvsran.b.h
  { 0b01110100111101110, 0, 5, 10, 0 }, //xvsran.h.w
  { 0b01110100111101111, 0, 5, 10, 0 }, //xvsran.w.d
  { 0b01110100111110001, 0, 5, 10, 0 }, //xvsrlrn.b.h
  { 0b01110100111110010, 0, 5, 10, 0 }, //xvsrlrn.h.w
  { 0b01110100111110011, 0, 5, 10, 0 }, //xvsrlrn.w.d
  { 0b01110100111110101, 0, 5, 10, 0 }, //xvsrarn.b.h
  { 0b01110100111110110, 0, 5, 10, 0 }, //xvsrarn.h.w
  { 0b01110100111110111, 0, 5, 10, 0 }, //xvsrarn.w.d
  { 0b01110100111111001, 0, 5, 10, 0 }, //xvssrln.b.h
  { 0b01110100111111010, 0, 5, 10, 0 }, //xvssrln.h.w
  { 0b01110100111111011, 0, 5, 10, 0 }, //xvssrln.w.d
  { 0b01110100111111101, 0, 5, 10, 0 }, //xvssran.b.h
  { 0b01110100111111110, 0, 5, 10, 0 }, //xvssran.h.w
  { 0b01110100111111111, 0, 5, 10, 0 }, //xvssran.w.d
  { 0b01110101000000001, 0, 5, 10, 0 }, //xvssrlrn.b.h
  { 0b01110101000000010, 0, 5, 10, 0 }, //xvssrlrn.h.w
  { 0b01110101000000011, 0, 5, 10, 0 }, //xvssrlrn.w.d
  { 0b01110101000000101, 0, 5, 10, 0 }, //xvssrarn.b.h
  { 0b01110101000000110, 0, 5, 10, 0 }, //xvssrarn.h.w
  { 0b01110101000000111, 0, 5, 10, 0 }, //xvssrarn.w.d
  { 0b01110101000001001, 0, 5, 10, 0 }, //xvssrln.bu.h
  { 0b01110101000001010, 0, 5, 10, 0 }, //xvssrln.hu.w
  { 0b01110101000001011, 0, 5, 10, 0 }, //xvssrln.wu.d
  { 0b01110101000001101, 0, 5, 10, 0 }, //xvssran.bu.h
  { 0b01110101000001110, 0, 5, 10, 0 }, //xvssran.hu.w
  { 0b01110101000001111, 0, 5, 10, 0 }, //xvssran.wu.d
  { 0b01110101000010001, 0, 5, 10, 0 }, //xvssrlrn.bu.h
  { 0b01110101000010010, 0, 5, 10, 0 }, //xvssrlrn.hu.w
  { 0b01110101000010011, 0, 5, 10, 0 }, //xvssrlrn.wu.d
  { 0b01110101000010101, 0, 5, 10, 0 }, //xvssrarn.bu.h
  { 0b01110101000010110, 0, 5, 10, 0 }, //xvssrarn.hu.w
  { 0b01110101000010111, 0, 5, 10, 0 }, //xvssrarn.wu.d
  { 0b01110101000011000, 0, 5, 10, 0 }, //xvbitclr.b
  { 0b01110101000011001, 0, 5, 10, 0 }, //xvbitclr.h
  { 0b01110101000011010, 0, 5, 10, 0 }, //xvbitclr.w
  { 0b01110101000011011, 0, 5, 10, 0 }, //xvbitclr.d
  { 0b01110101000011100, 0, 5, 10, 0 }, //xvbitset.b
  { 0b01110101000011101, 0, 5, 10, 0 }, //xvbitset.h
  { 0b01110101000011110, 0, 5, 10, 0 }, //xvbitset.w
  { 0b01110101000011111, 0, 5, 10, 0 }, //xvbitset.d
  { 0b01110101000100000, 0, 5, 10, 0 }, //xvbitrev.b
  { 0b01110101000100001, 0, 5, 10, 0 }, //xvbitrev.h
  { 0b01110101000100010, 0, 5, 10, 0 }, //xvbitrev.w
  { 0b01110101000100011, 0, 5, 10, 0 }, //xvbitrev.d
  { 0b01110101000101100, 0, 5, 10, 0 }, //xvpackev.b
  { 0b01110101000101101, 0, 5, 10, 0 }, //xvpackev.h
  { 0b01110101000101110, 0, 5, 10, 0 }, //xvpackev.w
  { 0b01110101000101111, 0, 5, 10, 0 }, //xvpackev.d
  { 0b01110101000110000, 0, 5, 10, 0 }, //xvpackod.b
  { 0b01110101000110001, 0, 5, 10, 0 }, //xvpackod.h
  { 0b01110101000110010, 0, 5, 10, 0 }, //xvpackod.w
  { 0b01110101000110011, 0, 5, 10, 0 }, //xvpackod.d
  { 0b01110101000110100, 0, 5, 10, 0 }, //xvilvl.b
  { 0b01110101000110101, 0, 5, 10, 0 }, //xvilvl.h
  { 0b01110101000110110, 0, 5, 10, 0 }, //xvilvl.w
  { 0b01110101000110111, 0, 5, 10, 0 }, //xvilvl.d
  { 0b01110101000111000, 0, 5, 10, 0 }, //xvilvh.b
  { 0b01110101000111001, 0, 5, 10, 0 }, //xvilvh.h
  { 0b01110101000111010, 0, 5, 10, 0 }, //xvilvh.w
  { 0b01110101000111011, 0, 5, 10, 0 }, //xvilvh.d
  { 0b01110101000111100, 0, 5, 10, 0 }, //xvpickev.b
  { 0b01110101000111101, 0, 5, 10, 0 }, //xvpickev.h
  { 0b01110101000111110, 0, 5, 10, 0 }, //xvpickev.w
  { 0b01110101000111111, 0, 5, 10, 0 }, //xvpickev.d
  { 0b01110101001000000, 0, 5, 10, 0 }, //xvpickod.b
  { 0b01110101001000001, 0, 5, 10, 0 }, //xvpickod.h
  { 0b01110101001000010, 0, 5, 10, 0 }, //xvpickod.w
  { 0b01110101001000011, 0, 5, 10, 0 }, //xvpickod.d
  { 0b01110101001001100, 0, 5, 10, 0 }, //xvand.v
  { 0b01110101001001101, 0, 5, 10, 0 }, //xvor.v
  { 0b01110101001001110, 0, 5, 10, 0 }, //xvxor.v
  { 0b01110101001001111, 0, 5, 10, 0 }, //xvnor.v
  { 0b01110101001010000, 0, 5, 10, 0 }, //xvandn.v
  { 0b01110101001010001, 0, 5, 10, 0 }, //xvorn.v
  { 0b01110101001010110, 0, 5, 10, 0 }, //xvfrstp.b
  { 0b01110101001010111, 0, 5, 10, 0 }, //xvfrstp.h
  { 0b01110101001011010, 0, 5, 10, 0 }, //xvadd.q
  { 0b01110101001011011, 0, 5, 10, 0 }, //xvsub.q
  { 0b01110101001011100, 0, 5, 10, 0 }, //xvsigncov.b
  { 0b01110101001011101, 0, 5, 10, 0 }, //xvsigncov.h
  { 0b01110101001011110, 0, 5, 10, 0 }, //xvsigncov.w
  { 0b01110101001011111, 0, 5, 10, 0 }, //xvsigncov.d
  { 0b01110101001100001, 0, 5, 10, 0 }, //xvfadd.s
  { 0b01110101001100010, 0, 5, 10, 0 }, //xvfadd.d
  { 0b01110101001100101, 0, 5, 10, 0 }, //xvfsub.s
  { 0b01110101001100110, 0, 5, 10, 0 }, //xvfsub.d
  { 0b01110101001110001, 0, 5, 10, 0 }, //xvfmul.s
  { 0b01110101001110010, 0, 5, 10, 0 }, //xvfmul.d
  { 0b01110101001110101, 0, 5, 10, 0 }, //xvfdiv.s
  { 0b01110101001110110, 0, 5, 10, 0 }, //xvfdiv.d
  { 0b01110101001111001, 0, 5, 10, 0 }, //xvfmax.s
  { 0b01110101001111010, 0, 5, 10, 0 }, //xvfmax.d
  { 0b01110101001111101, 0, 5, 10, 0 }, //xvfmin.s
  { 0b01110101001111110, 0, 5, 10, 0 }, //xvfmin.d
  { 0b01110101010000001, 0, 5, 10, 0 }, //xvfmaxa.s
  { 0b01110101010000010, 0, 5, 10, 0 }, //xvfmaxa.d
  { 0b01110101010000101, 0, 5, 10, 0 }, //xvfmina.s
  { 0b01110101010000110, 0, 5, 10, 0 }, //xvfmina.d
  { 0b01110101010001001, 0, 5, 10, 0 }, //xvfscaleb.s
  { 0b01110101010001010, 0, 5, 10, 0 }, //xvfscaleb.d
  { 0b01110101010001100, 0, 5, 10, 0 }, //xvfcvt.h.s
  { 0b01110101010001101, 0, 5, 10, 0 }, //xvfcvt.s.d
  { 0b01110101010010000, 0, 5, 10, 0 }, //xvffint.s.l
  { 0b01110101010010011, 0, 5, 10, 0 }, //xvftint.w.d
  { 0b01110101010010100, 0, 5, 10, 0 }, //xvftintrm.w.d
  { 0b01110101010010101, 0, 5, 10, 0 }, //xvftintrp.w.d
  { 0b01110101010010110, 0, 5, 10, 0 }, //xvftintrz.w.d
  { 0b01110101010010111, 0, 5, 10, 0 }, //xvftintrne.w.d
  { 0b01110101011110101, 0, 5, 10, 0 }, //xvshuf.h
  { 0b01110101011110110, 0, 5, 10, 0 }, //xvshuf.w
  { 0b01110101011110111, 0, 5, 10, 0 }, //xvshuf.d
  { 0b01110101011111010, 0, 5, 10, 0 }, //xvperm.w
  { 0b00001100100100000, 0, 5, 10, 0 }, //xvfcmp.caf.s 368
  { 0b00001100100101000, 0, 5, 10, 0 }, //xvfcmp.cun.s 369
  { 0b00001100100100100, 0, 5, 10, 0 }, //xvfcmp.ceq.s 370
  { 0b00001100100101100, 0, 5, 10, 0 }, //xvfcmp.cueq.s 371
  { 0b00001100100100010, 0, 5, 10, 0 }, //xvfcmp.clt.s 372
  { 0b00001100100101010, 0, 5, 10, 0 }, //xvfcmp.cult.s 373
  { 0b00001100100100110, 0, 5, 10, 0 }, //xvfcmp.cle.s 374
  { 0b00001100100101110, 0, 5, 10, 0 }, //xvfcmp.cule.s 375
  { 0b00001100100110000, 0, 5, 10, 0 }, //xvfcmp.cne.s 376
  { 0b00001100100110100, 0, 5, 10, 0 }, //xvfcmp.cor.s 377
  { 0b00001100100111000, 0, 5, 10, 0 }, //xvfcmp.cune.s 378
  { 0b00001100100100001, 0, 5, 10, 0 }, //xvfcmp.saf.s 379
  { 0b00001100100101001, 0, 5, 10, 0 }, //xvfcmp.sun.s 380
  { 0b00001100100100101, 0, 5, 10, 0 }, //xvfcmp.seq.s 381
  { 0b00001100100101101, 0, 5, 10, 0 }, //xvfcmp.sueq.s 382
  { 0b00001100100100011, 0, 5, 10, 0 }, //xvfcmp.slt.s 383
  { 0b00001100100101011, 0, 5, 10, 0 }, //xvfcmp.sult.s 384
  { 0b00001100100100111, 0, 5, 10, 0 }, //xvfcmp.sle.s 385
  { 0b00001100100101111, 0, 5, 10, 0 }, //xvfcmp.sule.s 386
  { 0b00001100100110001, 0, 5, 10, 0 }, //xvfcmp.sne.s 387
  { 0b00001100100110101, 0, 5, 10, 0 }, //xvfcmp.sor.s 388
  { 0b00001100100111001, 0, 5, 10, 0 }, //xvfcmp.sune.s 389
  { 0b00001100101000000, 0, 5, 10, 0 }, //xvfcmp.caf.d 390
  { 0b00001100101001000, 0, 5, 10, 0 }, //xvfcmp.cun.d 391
  { 0b00001100101000100, 0, 5, 10, 0 }, //xvfcmp.ceq.d 392
  { 0b00001100101001100, 0, 5, 10, 0 }, //xvfcmp.cueq.d 393
  { 0b00001100101000010, 0, 5, 10, 0 }, //xvfcmp.clt.d 394
  { 0b00001100101001010, 0, 5, 10, 0 }, //xvfcmp.cult.d 395
  { 0b00001100101000110, 0, 5, 10, 0 }, //xvfcmp.cle.d 396
  { 0b00001100101001110, 0, 5, 10, 0 }, //xvfcmp.cule.d 397
  { 0b00001100101010000, 0, 5, 10, 0 }, //xvfcmp.cne.d 398
  { 0b00001100101010100, 0, 5, 10, 0 }, //xvfcmp.cor.d 399
  { 0b00001100101011000, 0, 5, 10, 0 }, //xvfcmp.cune.d 400
  { 0b00001100101000001, 0, 5, 10, 0 }, //xvfcmp.saf.d 401
  { 0b00001100101001001, 0, 5, 10, 0 }, //xvfcmp.sun.d 402
  { 0b00001100101000101, 0, 5, 10, 0 }, //xvfcmp.seq.d 403
  { 0b00001100101001101, 0, 5, 10, 0 }, //xvfcmp.sueq.d 404
  { 0b00001100101000011, 0, 5, 10, 0 }, //xvfcmp.slt.d 405
  { 0b00001100101001011, 0, 5, 10, 0 }, //xvfcmp.sult.d 406
  { 0b00001100101000111, 0, 5, 10, 0 }, //xvfcmp.sle.d 407
  { 0b00001100101001111, 0, 5, 10, 0 }, //xvfcmp.sule.d 408 xvfcmp_sule_d
  { 0b00001100101010001, 0, 5, 10, 0 }, //xvfcmp.sne.d 409
  { 0b00001100101010101, 0, 5, 10, 0 }, //xvfcmp.sor.d 410
  { 0b00001100101011001, 0, 5, 10, 0 }, //xvfcmp.sune.d 411
};

const lsxVRI lsxvri[4] = {
  { 0b011100101110101110, 0, kWX, 5, 10, 5 }, //vinsgr2vr.b 6
  { 0b0111001011101011110, 0, kWX, 5, 10, 6 }, //vinsgr2vr.h 7
  { 0b01110010111010111110, 0, kWX, 5, 10, 7 }, //vinsgr2vr.w 8
  { 0b011100101110101111110, 0, kWX, 5, 10, 8 }, //vinsgr2vr.d 9
};

const lasxXRI lasxxri[2] = {
  { 0b0111011011101011110, 0, kWX, 5, 10, 5 }, //xvinsgr2vr.w 6
  { 0b01110110111010111110, 0, kWX, 5, 10, 6 }, //xvinsgr2vr.d 7
};

const SimdLdst simdLdst[16] = {
  { 0b0010110010,        22, 12}, //xvld 0
  { 0b00111000010010000, 15, 0 }, //xvldx 1
  { 0b0011001000010,     19, 9 }, //xvldrepl.d 2
  { 0b001100100010,      20, 10}, //xvldrepl.w 3
  { 0b00110010010,       21, 11}, //xvldrepl.h 4
  { 0b0011001010,        22, 12}, //xvldrepl.b 5
  { 0b0010110000,        22, 12}, //vld
  { 0b00111000010000000, 15, 0 }, //vldx
  { 0b0011000010,        22, 12}, //vldrepl_b
  { 0b00110000010,       21, 11}, //vldrepl_h
  { 0b001100000010,      20, 10}, //vldrepl_w
  { 0b0011000000010,     19, 9 }, //vldrepl_d
  { 0b0010110001,        22, 12}, //vst
  { 0b00111000010001000, 15, 0 }, //vstx
  { 0b0010110011,        22, 12}, //xvst
  { 0b00111000010011000, 15, 0 }, //xvstx

};

const lsxVRII lsxvrii[4] = {
  { 0b0011000100010, 0, kWX, 5, 10, 18, 0 }, //vstelm.d 0
  { 0b001100010010, 0, kWX, 5, 10, 18, 1 }, //vstelm.w 1
  { 0b00110001010, 0, kWX, 5, 10, 18, 2 }, //vstelm.h 2
  { 0b0011000110, 0, kWX, 5, 10, 18, 3 }, //vstelm.b 3
};

const lasxXRII lasxxrii[4] = {
  { 0b001100110001, 0, kWX, 5, 10, 18, 0 }, //xvstelm.d 0
  { 0b00110011001, 0, kWX, 5, 10, 18, 1 }, //xvstelm.w 1
  { 0b0011001101, 0, kWX, 5, 10, 18, 2 }, //xvstelm.h 2
  { 0b001100111, 0, kWX, 5, 10, 18, 3 }, //xvstelm.b 3
};

const lsxVRR lsxvrr[2] = {
  { 0b00111000010000000, 0, kWX, 5, kWX, 10, 0 }, //vldx 0
  { 0b00111000010001000, 0, kWX, 5, kWX, 10, 0 }, //vstx 1
};

const lasxXRR lasxxrr[2] = {
  { 0b00111000010010000, 0, kWX, 5, kWX, 10, 0 }, //xvldx 0
  { 0b00111000010011000, 0, kWX, 5, kWX, 10, 0 }, //xvstx 1
};

const lsxVVR lsxvvr[4] = {
  { 0b01110001001000100, 0, 5, kWX, 10, 0 }, //vreplve.b 0
  { 0b01110001001000101, 0, 5, kWX, 10, 0 }, //vreplve.h 1
  { 0b01110001001000110, 0, 5, kWX, 10, 0 }, //vreplve.w 2
  { 0b01110001001000111, 0, 5, kWX, 10, 0 }, //vreplve.d 3
};

const lasxXXR lasxxxr[4] = {
  { 0b01110101001000100, 0, 5, kWX, 10, 0 }, //xvreplve.b 0
  { 0b01110101001000101, 0, 5, kWX, 10, 0 }, //xvreplve.h 1
  { 0b01110101001000110, 0, 5, kWX, 10, 0 }, //xvreplve.w 2
  { 0b01110101001000111, 0, 5, kWX, 10, 0 }, //xvreplve.d 3
};

const lsxVVI lsxvvi[160] = {
  { 0b01110010100000000, 0, 5, 10, 0 }, //vseqi.b 0
  { 0b01110010100000001, 0, 5, 10, 0 }, //vseqi.h 1
  { 0b01110010100000010, 0, 5, 10, 0 }, //vseqi.w 2
  { 0b01110010100000011, 0, 5, 10, 0 }, //vseqi.d 3
  { 0b01110010100000100, 0, 5, 10, 0 }, //vslei.b 4
  { 0b01110010100000101, 0, 5, 10, 0 }, //vslei.h 5
  { 0b01110010100000110, 0, 5, 10, 0 }, //vslei.w 6
  { 0b01110010100000111, 0, 5, 10, 0 }, //vslei.d 7
  { 0b01110010100001000, 0, 5, 10, 9 }, //vslei.bu 8
  { 0b01110010100001001, 0, 5, 10, 9 }, //vslei.hu 9
  { 0b01110010100001010, 0, 5, 10, 9 }, //vslei.wu 10
  { 0b01110010100001011, 0, 5, 10, 9 }, //vslei.du 11
  { 0b01110010100001100, 0, 5, 10, 0 }, //vslti.b 12
  { 0b01110010100001101, 0, 5, 10, 0 }, //vslti.h 13
  { 0b01110010100001110, 0, 5, 10, 0 }, //vslti.w 14
  { 0b01110010100001111, 0, 5, 10, 0 }, //vslti.d 15
  { 0b01110010100010000, 0, 5, 10, 9 }, //vslti.bu 16
  { 0b01110010100010001, 0, 5, 10, 9 }, //vslti.hu 17
  { 0b01110010100010010, 0, 5, 10, 9 }, //vslti.wu 18
  { 0b01110010100010011, 0, 5, 10, 9 }, //vslti.du 19
  { 0b01110010100010100, 0, 5, 10, 9 }, //vaddi.bu 20
  { 0b01110010100010101, 0, 5, 10, 9 }, //vaddi.hu 21
  { 0b01110010100010110, 0, 5, 10, 9 }, //vaddi.wu 22
  { 0b01110010100010111, 0, 5, 10, 9 }, //vaddi.du 23
  { 0b01110010100011000, 0, 5, 10, 9 }, //vsubi.bu 24
  { 0b01110010100011001, 0, 5, 10, 9 }, //vsubi.hu 25
  { 0b01110010100011010, 0, 5, 10, 9 }, //vsubi.wu 26
  { 0b01110010100011011, 0, 5, 10, 9 }, //vsubi.du 27
  { 0b01110010100011100, 0, 5, 10, 9 }, //vbsll.v 28
  { 0b01110010100011101, 0, 5, 10, 9 }, //vbsrl.v 29
  { 0b01110010100100000, 0, 5, 10, 0 }, //vmaxi.b 30
  { 0b01110010100100001, 0, 5, 10, 0 }, //vmaxi.h 31
  { 0b01110010100100010, 0, 5, 10, 0 }, //vmaxi.w 32
  { 0b01110010100100011, 0, 5, 10, 0 }, //vmaxi.d 33
  { 0b01110010100100100, 0, 5, 10, 0 }, //vmini.b 34
  { 0b01110010100100101, 0, 5, 10, 0 }, //vmini.h 35
  { 0b01110010100100110, 0, 5, 10, 0 }, //vmini.w 36
  { 0b01110010100100111, 0, 5, 10, 0 }, //vmini.d 37
  { 0b01110010100101000, 0, 5, 10, 9 }, //vmaxi.bu 38
  { 0b01110010100101001, 0, 5, 10, 9 }, //vmaxi.hu 39
  { 0b01110010100101010, 0, 5, 10, 9 }, //vmaxi.wu 40
  { 0b01110010100101011, 0, 5, 10, 9 }, //vmaxi.du 41
  { 0b01110010100101100, 0, 5, 10, 9 }, //vmini.bu 42
  { 0b01110010100101101, 0, 5, 10, 9 }, //vmini.hu 43
  { 0b01110010100101110, 0, 5, 10, 9 }, //vmini.wu 44
  { 0b01110010100101111, 0, 5, 10, 9 }, //vmini.du 45
  { 0b01110010100110100, 0, 5, 10, 9 }, //vfrstpi.b 46
  { 0b01110010100110101, 0, 5, 10, 9 }, //vfrstpi.h 47
  { 0b0111001010100000001, 0, 5, 10, 1 }, //vrotri.b 48
  { 0b011100101010000001, 0, 5, 10, 2 }, //vrotri.h 49
  { 0b01110010101000001, 0, 5, 10, 3 }, //vrotri.w 50
  { 0b0111001010100001, 0, 5, 10, 4 }, //vrotri.d 51
  { 0b0111001010100100001, 0, 5, 10, 1 }, //vsrlri.b 52
  { 0b011100101010010001, 0, 5, 10, 2 }, //vsrlri.h 53
  { 0b01110010101001001, 0, 5, 10, 3 }, //vsrlri.w 54
  { 0b0111001010100101, 0, 5, 10, 4 }, //vsrlri.d 55
  { 0b0111001010101000001, 0, 5, 10, 1 }, //vsrari.b 56
  { 0b011100101010100001, 0, 5, 10, 2 }, //vsrari.h 57
  { 0b01110010101010001, 0, 5, 10, 3 }, //vsrari.w 58
  { 0b0111001010101001, 0, 5, 10, 4 }, //vsrari.d 59
  { 0b011100101111011110, 0, 5, 10, 2 }, //vreplvei.b 60
  { 0b0111001011110111110, 0, 5, 10, 1 }, //vreplvei.h 61
  { 0b01110010111101111110, 0, 5, 10, 5 }, //vreplvei.w 62
  { 0b011100101111011111110, 0, 5, 10, 6 }, //vreplvei.d 63
  { 0b0111001100001000001, 0, 5, 10, 1 }, //vsllwil.h.b 64
  { 0b011100110000100001, 0, 5, 10, 2 }, //vsllwil.w.h 65
  { 0b01110011000010001, 0, 5, 10, 3 }, //vsllwil.d.w 66
  { 0b0111001100001100001, 0, 5, 10, 1 }, //vsllwil.hu.bu 67
  { 0b011100110000110001, 0, 5, 10, 2 }, //vsllwil.wu.hu 68
  { 0b01110011000011001, 0, 5, 10, 3 }, //vsllwil.du.wu 69
  { 0b0111001100010000001, 0, 5, 10, 1 }, //vbitclri.b 70
  { 0b011100110001000001, 0, 5, 10, 2 }, //vbitclri.h 71
  { 0b01110011000100001, 0, 5, 10, 3 }, //vbitclri.w 72
  { 0b0111001100010001, 0, 5, 10, 4 }, //vbitclri.d 73
  { 0b0111001100010100001, 0, 5, 10, 1 }, //vbitseti.b 74
  { 0b011100110001010001, 0, 5, 10, 2 }, //vbitseti.h 75
  { 0b01110011000101001, 0, 5, 10, 3 }, //vbitseti.w 76
  { 0b0111001100010101, 0, 5, 10, 4 }, //vbitseti.d 77
  { 0b0111001100011000001, 0, 5, 10, 1 }, //vbitrevi.b 78
  { 0b011100110001100001, 0, 5, 10, 2 }, //vbitrevi.h 79
  { 0b01110011000110001, 0, 5, 10, 3 }, //vbitrevi.w 80
  { 0b0111001100011001, 0, 5, 10, 4 }, //vbitrevi.d 81
  { 0b0111001100100100001, 0, 5, 10, 1 }, //vsat.b 82
  { 0b011100110010010001, 0, 5, 10, 2 }, //vsat.h 83
  { 0b01110011001001001, 0, 5, 10, 3 }, //vsat.w 84
  { 0b0111001100100101, 0, 5, 10, 4 }, //vsat.d 85
  { 0b0111001100101000001, 0, 5, 10, 1 }, //vsat.bu 86
  { 0b011100110010100001, 0, 5, 10, 2 }, //vsat.hu 87
  { 0b01110011001010001, 0, 5, 10, 3 }, //vsat.wu 88
  { 0b0111001100101001, 0, 5, 10, 4 }, //vsat.du 89
  { 0b0111001100101100001, 0, 5, 10, 1 }, //vslli.b 90
  { 0b011100110010110001, 0, 5, 10, 2 }, //vslli.h 91
  { 0b01110011001011001, 0, 5, 10, 3 }, //vslli.w 92
  { 0b0111001100101101, 0, 5, 10, 4 }, //vslli.d 93
  { 0b0111001100110000001, 0, 5, 10, 1 }, //vsrli.b 94
  { 0b011100110011000001, 0, 5, 10, 2 }, //vsrli.h 95
  { 0b01110011001100001, 0, 5, 10, 3 }, //vsrli.w 96
  { 0b0111001100110001, 0, 5, 10, 4 }, //vsrli.d 97
  { 0b0111001100110100001, 0, 5, 10, 1 }, //vsrai.b 98
  { 0b011100110011010001, 0, 5, 10, 2 }, //vsrai.h 99
  { 0b01110011001101001, 0, 5, 10, 3 }, //vsrai.w 100
  { 0b0111001100110101, 0, 5, 10, 4 }, //vsrai.d 101
  { 0b011100110100000001, 0, 5, 10, 2 }, //vsrlni.b.h 102
  { 0b01110011010000001, 0, 5, 10, 3 }, //vsrlni.h.w 103
  { 0b0111001101000001, 0, 5, 10, 4 }, //vsrlni.w.d 104
  { 0b011100110100001, 0, 5, 10, 7 }, //vsrlni.d.q 105
  { 0b011100110100100001, 0, 5, 10, 2 }, //vssrlni.b.h 106
  { 0b01110011010010001, 0, 5, 10, 3 }, //vssrlni.h.w 107
  { 0b0111001101001001, 0, 5, 10, 4 }, //vssrlni.w.d 108
  { 0b011100110100101, 0, 5, 10, 7 }, //vssrlni.d.q 109
  { 0b011100110100110001, 0, 5, 10, 2 }, //vssrlni.bu.h 110
  { 0b01110011010011001, 0, 5, 10, 3 }, //vssrlni.hu.w 111
  { 0b0111001101001101, 0, 5, 10, 4 }, //vssrlni.wu.d 112
  { 0b011100110100111, 0, 5, 10, 7 }, //vssrlni.du.q 113
  { 0b011100110101000001, 0, 5, 10, 2 }, //vssrlrni.b.h 114
  { 0b01110011010100001, 0, 5, 10, 3 }, //vssrlrni.h.w 115
  { 0b0111001101010001, 0, 5, 10, 4 }, //vssrlrni.w.d 116
  { 0b011100110101001, 0, 5, 10, 7 }, //vssrlrni.d.q 117
  { 0b011100110101010001, 0, 5, 10, 2 }, //vssrlrni.bu.h 118
  { 0b01110011010101001, 0, 5, 10, 3 }, //vssrlrni.hu.w 119
  { 0b0111001101010101, 0, 5, 10, 4 }, //vssrlrni.wu.d 120
  { 0b011100110101011, 0, 5, 10, 7 }, //vssrlrni.du.q 121
  { 0b011100110101100001, 0, 5, 10, 2 }, //vsrani.b.h 122
  { 0b01110011010110001, 0, 5, 10, 3 }, //vsrani.h.w 123
  { 0b0111001101011001, 0, 5, 10, 4 }, //vsrani.w.d 124
  { 0b011100110101101, 0, 5, 10, 7 }, //vsrani.d.q 125
  { 0b011100110101110001, 0, 5, 10, 2 }, //vsrarni.b.h 126
  { 0b01110011010111001, 0, 5, 10, 3 }, //vsrarni.h.w 127
  { 0b0111001101011101, 0, 5, 10, 4 }, //vsrarni.w.d 128
  { 0b011100110101111, 0, 5, 10, 7 }, //vsrarni.d.q 129
  { 0b011100110110000001, 0, 5, 10, 2 }, //vssrani.b.h 130
  { 0b01110011011000001, 0, 5, 10, 3 }, //vssrani.h.w 131
  { 0b0111001101100001, 0, 5, 10, 4 }, //vssrani.w.d 132
  { 0b011100110110001, 0, 5, 10, 7 }, //vssrani.d.q 133
  { 0b011100110110010001, 0, 5, 10, 2 }, //vssrani.bu.h 134
  { 0b01110011011001001, 0, 5, 10, 3 }, //vssrani.hu.w 135
  { 0b0111001101100101, 0, 5, 10, 4 }, //vssrani.wu.d 136
  { 0b011100110110011, 0, 5, 10, 7 }, //vssrani.du.q 137
  { 0b011100110110100001, 0, 5, 10, 2 }, //vssrarni.b.h 138
  { 0b01110011011010001, 0, 5, 10, 3 }, //vssrarni.h.w 139
  { 0b0111001101101001, 0, 5, 10, 4 }, //vssrarni.w.d 140
  { 0b011100110110101, 0, 5, 10, 7 }, //vssrarni.d.q 141
  { 0b011100110110110001, 0, 5, 10, 2 }, //vssrarni.bu.h 142
  { 0b01110011011011001, 0, 5, 10, 3 }, //vssrarni.hu.w 143
  { 0b0111001101101101, 0, 5, 10, 4 }, //vssrarni.wu.d 144
  { 0b011100110110111, 0, 5, 10, 7 }, //vssrarni.du.q 145
  { 0b01110011100000, 0, 5, 10, 8 }, //vextrins.d 146
  { 0b01110011100001, 0, 5, 10, 8 }, //vextrins.w 147
  { 0b01110011100010, 0, 5, 10, 8 }, //vextrins.h 148
  { 0b01110011100011, 0, 5, 10, 8 }, //vextrins.b 149
  { 0b01110011100100, 0, 5, 10, 8 }, //vshuf4i.b 150
  { 0b01110011100101, 0, 5, 10, 8 }, //vshuf4i.h 151
  { 0b01110011100110, 0, 5, 10, 8 }, //vshuf4i.w 152
  { 0b01110011100111, 0, 5, 10, 8 }, //vshuf4i.d 153
  { 0b01110011110001, 0, 5, 10, 8 }, //vbitseli.b 154
  { 0b01110011110100, 0, 5, 10, 8 }, //vandi.b 155
  { 0b01110011110101, 0, 5, 10, 8 }, //vori.b 156
  { 0b01110011110110, 0, 5, 10, 8 }, //vxori.b 157
  { 0b01110011110111, 0, 5, 10, 8 }, //vnori.b 158
  { 0b01110011111001, 0, 5, 10, 8 }, //vpermi.w 159
};

const lasxXXI lasxxxi[171] = {
  { 0b01110110100000000, 0, 5, 10, 0 }, //xvseqi.b 0
  { 0b01110110100000001, 0, 5, 10, 0 }, //xvseqi.h 1
  { 0b01110110100000010, 0, 5, 10, 0 }, //xvseqi.w 2
  { 0b01110110100000011, 0, 5, 10, 0 }, //xvseqi.d 3
  { 0b01110110100000100, 0, 5, 10, 0 }, //xvslei.b 4
  { 0b01110110100000101, 0, 5, 10, 0 }, //xvslei.h 5
  { 0b01110110100000110, 0, 5, 10, 0 }, //xvslei.w 6
  { 0b01110110100000111, 0, 5, 10, 0 }, //xvslei.d 7
  { 0b01110110100001000, 0, 5, 10, 9 }, //xvslei.bu 8
  { 0b01110110100001001, 0, 5, 10, 9 }, //xvslei.hu 9
  { 0b01110110100001010, 0, 5, 10, 9 }, //xvslei.wu 10
  { 0b01110110100001011, 0, 5, 10, 9 }, //xvslei.du 11
  { 0b01110110100001100, 0, 5, 10, 0 }, //xvslti.b 12
  { 0b01110110100001101, 0, 5, 10, 0 }, //xvslti.h 13
  { 0b01110110100001110, 0, 5, 10, 0 }, //xvslti.w 14
  { 0b01110110100001111, 0, 5, 10, 0 }, //xvslti.d 15
  { 0b01110110100010000, 0, 5, 10, 9 }, //xvslti.bu 16
  { 0b01110110100010001, 0, 5, 10, 9 }, //xvslti.hu 17
  { 0b01110110100010010, 0, 5, 10, 9 }, //xvslti.wu 18
  { 0b01110110100010011, 0, 5, 10, 9 }, //xvslti.du 19
  { 0b01110110100010100, 0, 5, 10, 9 }, //xvaddi.bu 20
  { 0b01110110100010101, 0, 5, 10, 9 }, //xvaddi.hu 21
  { 0b01110110100010110, 0, 5, 10, 9 }, //xvaddi.wu 22
  { 0b01110110100010111, 0, 5, 10, 9 }, //xvaddi.du 23
  { 0b01110110100011000, 0, 5, 10, 9 }, //xvsubi.bu 24
  { 0b01110110100011001, 0, 5, 10, 9 }, //xvsubi.hu 25
  { 0b01110110100011010, 0, 5, 10, 9 }, //xvsubi.wu 26
  { 0b01110110100011011, 0, 5, 10, 9 }, //xvsubi.du 27
  { 0b01110110100011100, 0, 5, 10, 9 }, //xvbsll.v 28
  { 0b01110110100011101, 0, 5, 10, 9 }, //xvbsrl.v 29
  { 0b01110110100100000, 0, 5, 10, 0 }, //xvmaxi.b 30
  { 0b01110110100100001, 0, 5, 10, 0 }, //xvmaxi.h 31
  { 0b01110110100100010, 0, 5, 10, 0 }, //xvmaxi.w 32
  { 0b01110110100100011, 0, 5, 10, 0 }, //xvmaxi.d 33
  { 0b01110110100100100, 0, 5, 10, 0 }, //xvmini.b 34
  { 0b01110110100100101, 0, 5, 10, 0 }, //xvmini.h 35
  { 0b01110110100100110, 0, 5, 10, 0 }, //xvmini.w 36
  { 0b01110110100100111, 0, 5, 10, 0 }, //xvmini.d 37
  { 0b01110110100101000, 0, 5, 10, 9 }, //xvmaxi.bu 38
  { 0b01110110100101001, 0, 5, 10, 9 }, //xvmaxi.hu 39
  { 0b01110110100101010, 0, 5, 10, 9 }, //xvmaxi.wu 40
  { 0b01110110100101011, 0, 5, 10, 9 }, //xvmaxi.du 41
  { 0b01110110100101100, 0, 5, 10, 9 }, //xvmini.bu 42
  { 0b01110110100101101, 0, 5, 10, 9 }, //xvmini.hu 43
  { 0b01110110100101110, 0, 5, 10, 9 }, //xvmini.wu 44
  { 0b01110110100101111, 0, 5, 10, 9 }, //xvmini.du 45
  { 0b01110110100110100, 0, 5, 10, 9 }, //xvfrstpi.b 46
  { 0b01110110100110101, 0, 5, 10, 9 }, //xvfrstpi.h 47
  { 0b01110110100111111, 0, 5, 10, 9 }, //xvhseli.d 48
  { 0b0111011010100000001, 0, 5, 10, 1 }, //xvrotri.b 49
  { 0b011101101010000001, 0, 5, 10, 2 }, //xvrotri.h 50
  { 0b01110110101000001, 0, 5, 10, 3 }, //xvrotri.w 51
  { 0b0111011010100001, 0, 5, 10, 4 }, //xvrotri.d 52
  { 0b0111011010100100001, 0, 5, 10, 1 }, //xvsrlri.b 53
  { 0b011101101010010001, 0, 5, 10, 2 }, //xvsrlri.h 54
  { 0b01110110101001001, 0, 5, 10, 3 }, //xvsrlri.w 55
  { 0b0111011010100101, 0, 5, 10, 4 }, //xvsrlri.d 56
  { 0b0111011010101000001, 0, 5, 10, 1 }, //xvsrari.b 57
  { 0b011101101010100001, 0, 5, 10, 2 }, //xvsrari.h 58
  { 0b01110110101010001, 0, 5, 10, 3 }, //xvsrari.w 59
  { 0b0111011010101001, 0, 5, 10, 4 }, //xvsrari.d 60
  { 0b011101101111011110, 0, 5, 10, 2 }, //xvrepl128vei.b 61
  { 0b0111011011110111110, 0, 5, 10, 1 }, //xvrepl128vei.h 62
  { 0b01110110111101111110, 0, 5, 10, 5 }, //xvrepl128vei.w 63
  { 0b011101101111011111110, 0, 5, 10, 6 }, //xvrepl128vei.d 64
  { 0b0111011011111111110, 0, 5, 10, 1 }, //xvinsve0.w 65
  { 0b01110110111111111110, 0, 5, 10, 5 }, //xvinsve0.d 66
  { 0b0111011100000011110, 0, 5, 10, 1 }, //xvpickve.w 67
  { 0b01110111000000111110, 0, 5, 10, 5 }, //xvpickve.d 68
  { 0b0111011100001000001, 0, 5, 10, 1 }, //xvsllwil.h.b 69
  { 0b011101110000100001, 0, 5, 10, 2 }, //xvsllwil.w.h 70
  { 0b01110111000010001, 0, 5, 10, 3 }, //xvsllwil.d.w 71
  { 0b0111011100001100001, 0, 5, 10, 1 }, //xvsllwil.hu.bu 72
  { 0b011101110000110001, 0, 5, 10, 2 }, //xvsllwil.wu.hu 73
  { 0b01110111000011001, 0, 5, 10, 3 }, //xvsllwil.du.wu 74
  { 0b0111011100010000001, 0, 5, 10, 1 }, //xvbitclri.b 75
  { 0b011101110001000001, 0, 5, 10, 2 }, //xvbitclri.h 76
  { 0b01110111000100001, 0, 5, 10, 3 }, //xvbitclri.w 77
  { 0b0111011100010001, 0, 5, 10, 4 }, //xvbitclri.d 78
  { 0b0111011100010100001, 0, 5, 10, 1 }, //xvbitseti.b 79
  { 0b011101110001010001, 0, 5, 10, 2 }, //xvbitseti.h 80
  { 0b01110111000101001, 0, 5, 10, 3 }, //xvbitseti.w 81
  { 0b0111011100010101, 0, 5, 10, 4 }, //xvbitseti.d 82
  { 0b0111011100011000001, 0, 5, 10, 1 }, //xvbitrevi.b 83
  { 0b011101110001100001, 0, 5, 10, 2 }, //xvbitrevi.h 84
  { 0b01110111000110001, 0, 5, 10, 3 }, //xvbitrevi.w 85
  { 0b0111011100011001, 0, 5, 10, 4 }, //xvbitrevi.d 86
  { 0b0111011100100100001, 0, 5, 10, 1 }, //xvsat.b 87
  { 0b011101110010010001, 0, 5, 10, 2 }, //xvsat.h 88
  { 0b01110111001001001, 0, 5, 10, 3 }, //xvsat.w 89
  { 0b0111011100100101, 0, 5, 10, 4 }, //xvsat.d 90
  { 0b0111011100101000001, 0, 5, 10, 1 }, //xvsat.bu 91
  { 0b011101110010100001, 0, 5, 10, 2 }, //xvsat.hu 92
  { 0b01110111001010001, 0, 5, 10, 3 }, //xvsat.wu 93
  { 0b0111011100101001, 0, 5, 10, 4 }, //xvsat.du 94
  { 0b0111011100101100001, 0, 5, 10, 1 }, //xvslli.b 95
  { 0b011101110010110001, 0, 5, 10, 2 }, //xvslli.h 96
  { 0b01110111001011001, 0, 5, 10, 3 }, //xvslli.w 97
  { 0b0111011100101101, 0, 5, 10, 4 }, //xvslli.d 98
  { 0b0111011100110000001, 0, 5, 10, 1 }, //xvsrli.b 99
  { 0b011101110011000001, 0, 5, 10, 2 }, //xvsrli.h 100
  { 0b01110111001100001, 0, 5, 10, 3 }, //xvsrli.w 101
  { 0b0111011100110001, 0, 5, 10, 4 }, //xvsrli.d 102
  { 0b0111011100110100001, 0, 5, 10, 1 }, //xvsrai.b 103
  { 0b011101110011010001, 0, 5, 10, 2 }, //xvsrai.h 104
  { 0b01110111001101001, 0, 5, 10, 3 }, //xvsrai.w 105
  { 0b0111011100110101, 0, 5, 10, 4 }, //xvsrai.d 106
  { 0b011101110100000001, 0, 5, 10, 2 }, //xvsrlni.b.h 107
  { 0b01110111010000001, 0, 5, 10, 3 }, //xvsrlni.h.w 108
  { 0b0111011101000001, 0, 5, 10, 4 }, //xvsrlni.w.d 109
  { 0b011101110100001, 0, 5, 10, 7 }, //xvsrlni.d.q 110
  { 0b011101110100010001, 0, 5, 10, 2 }, //xvsrlrni.b.h 111
  { 0b01110111010001001, 0, 5, 10, 3 }, //xvsrlrni.h.w 112
  { 0b0111011101000101, 0, 5, 10, 4 }, //xvsrlrni.w.d 113
  { 0b011101110100011, 0, 5, 10, 7 }, //xvsrlrni.d.q 114
  { 0b011101110100100001, 0, 5, 10, 2 }, //xvssrlni.b.h 115
  { 0b01110111010010001, 0, 5, 10, 3 }, //xvssrlni.h.w 116
  { 0b0111011101001001, 0, 5, 10, 4 }, //xvssrlni.w.d 117
  { 0b011101110100101, 0, 5, 10, 7 }, //xvssrlni.d.q 118
  { 0b011101110100110001, 0, 5, 10, 2 }, //xvssrlni.bu.h 119
  { 0b01110111010011001, 0, 5, 10, 3 }, //xvssrlni.hu.w 120
  { 0b0111011101001101, 0, 5, 10, 4 }, //xvssrlni.wu.d 121
  { 0b011101110100111, 0, 5, 10, 7 }, //xvssrlni.du.q 122
  { 0b011101110101000001, 0, 5, 10, 2 }, //xvssrlrni.b.h 123
  { 0b01110111010100001, 0, 5, 10, 3 }, //xvssrlrni.h.w 124
  { 0b0111011101010001, 0, 5, 10, 4 }, //xvssrlrni.w.d 125
  { 0b011101110101001, 0, 5, 10, 7 }, //xvssrlrni.d.q 126
  { 0b011101110101010001, 0, 5, 10, 2 }, //xvssrlrni.bu.h 127
  { 0b01110111010101001, 0, 5, 10, 3 }, //xvssrlrni.hu.w 128
  { 0b0111011101010101, 0, 5, 10, 4 }, //xvssrlrni.wu.d 129
  { 0b011101110101011, 0, 5, 10, 7 }, //xvssrlrni.du.q 130
  { 0b011101110101100001, 0, 5, 10, 2 }, //xvsrani.b.h 131
  { 0b01110111010110001, 0, 5, 10, 3 }, //xvsrani.h.w 132
  { 0b0111011101011001, 0, 5, 10, 4 }, //xvsrani.w.d 133
  { 0b011101110101101, 0, 5, 10, 7 }, //xvsrani.d.q 134
  { 0b011101110101110001, 0, 5, 10, 2 }, //xvsrarni.b.h 135
  { 0b01110111010111001, 0, 5, 10, 3 }, //xvsrarni.h.w 136
  { 0b0111011101011101, 0, 5, 10, 4 }, //xvsrarni.w.d 137
  { 0b011101110101111, 0, 5, 10, 7 }, //xvsrarni.d.q 138
  { 0b011101110110000001, 0, 5, 10, 2 }, //xvssrani.b.h 139
  { 0b01110111011000001, 0, 5, 10, 3 }, //xvssrani.h.w 140
  { 0b0111011101100001, 0, 5, 10, 4 }, //xvssrani.w.d 141
  { 0b011101110110001, 0, 5, 10, 7 }, //xvssrani.d.q 142
  { 0b011101110110010001, 0, 5, 10, 2 }, //xvssrani.bu.h 143
  { 0b01110111011001001, 0, 5, 10, 3 }, //xvssrani.hu.w 144
  { 0b0111011101100101, 0, 5, 10, 4 }, //xvssrani.wu.d 145
  { 0b011101110110011, 0, 5, 10, 7 }, //xvssrani.du.q 146
  { 0b011101110110100001, 0, 5, 10, 2 }, //xvssrarni.b.h 147
  { 0b01110111011010001, 0, 5, 10, 3 }, //xvssrarni.h.w 148
  { 0b0111011101101001, 0, 5, 10, 4 }, //xvssrarni.w.d 149
  { 0b011101110110101, 0, 5, 10, 7 }, //xvssrarni.d.q 150
  { 0b011101110110110001, 0, 5, 10, 2 }, //xvssrarni.bu.h 151
  { 0b01110111011011001, 0, 5, 10, 3 }, //xvssrarni.hu.w 152
  { 0b0111011101101101, 0, 5, 10, 4 }, //xvssrarni.wu.d 153
  { 0b011101110110111, 0, 5, 10, 7 }, //xvssrarni.du.q 154
  { 0b01110111100000, 0, 5, 10, 8 }, //xvextrins.d 155
  { 0b01110111100001, 0, 5, 10, 8 }, //xvextrins.w 156
  { 0b01110111100010, 0, 5, 10, 8 }, //xvextrins.h 157
  { 0b01110111100011, 0, 5, 10, 8 }, //xvextrins.b 158
  { 0b01110111100100, 0, 5, 10, 8 }, //xvshuf4i.b 159
  { 0b01110111100101, 0, 5, 10, 8 }, //xvshuf4i.h 160
  { 0b01110111100110, 0, 5, 10, 8 }, //xvshuf4i.w 161
  { 0b01110111100111, 0, 5, 10, 8 }, //xvshuf4i.d 162
  { 0b01110111110001, 0, 5, 10, 8 }, //xvbitseli.b 163
  { 0b01110111110100, 0, 5, 10, 8 }, //xvandi.b 164
  { 0b01110111110101, 0, 5, 10, 8 }, //xvori.b 165
  { 0b01110111110110, 0, 5, 10, 8 }, //xvxori.b 166
  { 0b01110111110111, 0, 5, 10, 8 }, //xvnori.b 167
  { 0b01110111111001, 0, 5, 10, 8 }, //xvpermi.w 168
  { 0b01110111111010, 0, 5, 10, 8 }, //xvpermi.d 169
  { 0b01110111111011, 0, 5, 10, 8 }, //xvpermi.q 170
};

const lsxVII lsxvii[1] = {
  { 0b01110010100110111, 0, 5, 10, 0 }, //vmepatmsk.v 0
};

const lasxXII lasxxii[1] = {
  { 0b01110110100110111, 0, 5, 10, 0 }, //xvmepatmsk.v 0
};

const lsxVV lsxvv[86] = {
  { 0b0111001010011100000000, 0, 5, 0 }, //vclo.b 0
  { 0b0111001010011100000001, 0, 5, 0 }, //vclo.h 1
  { 0b0111001010011100000010, 0, 5, 0 }, //vclo.w 2
  { 0b0111001010011100000011, 0, 5, 0 }, //vclo.d 3
  { 0b0111001010011100000100, 0, 5, 0 }, //vclz.b 4
  { 0b0111001010011100000101, 0, 5, 0 }, //vclz.h 5
  { 0b0111001010011100000110, 0, 5, 0 }, //vclz.w 6
  { 0b0111001010011100000111, 0, 5, 0 }, //vclz.d 7
  { 0b0111001010011100001000, 0, 5, 0 }, //vpcnt.b 8
  { 0b0111001010011100001001, 0, 5, 0 }, //vpcnt.h 9
  { 0b0111001010011100001010, 0, 5, 0 }, //vpcnt.w 10
  { 0b0111001010011100001011, 0, 5, 0 }, //vpcnt.d 11
  { 0b0111001010011100001100, 0, 5, 0 }, //vneg.b 12
  { 0b0111001010011100001101, 0, 5, 0 }, //vneg.h 13
  { 0b0111001010011100001110, 0, 5, 0 }, //vneg.w 14
  { 0b0111001010011100001111, 0, 5, 0 }, //vneg.d 15
  { 0b0111001010011100010000, 0, 5, 0 }, //vmskltz.b 16
  { 0b0111001010011100010001, 0, 5, 0 }, //vmskltz.h 17
  { 0b0111001010011100010010, 0, 5, 0 }, //vmskltz.w 18
  { 0b0111001010011100010011, 0, 5, 0 }, //vmskltz.d 19
  { 0b0111001010011100010100, 0, 5, 0 }, //vmskgez.b 20
  { 0b0111001010011100011000, 0, 5, 0 }, //vmsknz.b 21
  { 0b0111001010011100110001, 0, 5, 0 }, //vflogb.s 22
  { 0b0111001010011100110010, 0, 5, 0 }, //vflogb.d 23
  { 0b0111001010011100110101, 0, 5, 0 }, //vfclass.s 24
  { 0b0111001010011100110110, 0, 5, 0 }, //vfclass.d 25
  { 0b0111001010011100111001, 0, 5, 0 }, //vfsqrt.s 26
  { 0b0111001010011100111010, 0, 5, 0 }, //vfsqrt.d 27
  { 0b0111001010011100111101, 0, 5, 0 }, //vfrecip.s 28
  { 0b0111001010011100111110, 0, 5, 0 }, //vfrecip.d 29
  { 0b0111001010011101000001, 0, 5, 0 }, //vfrsqrt.s 30
  { 0b0111001010011101000010, 0, 5, 0 }, //vfrsqrt.d 31
  { 0b0111001010011101001101, 0, 5, 0 }, //vfrint.s 32
  { 0b0111001010011101001110, 0, 5, 0 }, //vfrint.d 33
  { 0b0111001010011101010001, 0, 5, 0 }, //vfrintrm.s 34
  { 0b0111001010011101010010, 0, 5, 0 }, //vfrintrm.d 35
  { 0b0111001010011101010101, 0, 5, 0 }, //vfrintrp.s 36
  { 0b0111001010011101010110, 0, 5, 0 }, //vfrintrp.d 37
  { 0b0111001010011101011001, 0, 5, 0 }, //vfrintrz.s 38
  { 0b0111001010011101011010, 0, 5, 0 }, //vfrintrz.d 39
  { 0b0111001010011101011101, 0, 5, 0 }, //vfrintrne.s 40
  { 0b0111001010011101011110, 0, 5, 0 }, //vfrintrne.d 41
  { 0b0111001010011101111010, 0, 5, 0 }, //vfcvtl.s.h 42
  { 0b0111001010011101111011, 0, 5, 0 }, //vfcvth.s.h 43
  { 0b0111001010011101111100, 0, 5, 0 }, //vfcvtl.d.s 44
  { 0b0111001010011101111101, 0, 5, 0 }, //vfcvth.d.s 45
  { 0b0111001010011110000000, 0, 5, 0 }, //vffint.s.w 46
  { 0b0111001010011110000001, 0, 5, 0 }, //vffint.s.wu 47
  { 0b0111001010011110000010, 0, 5, 0 }, //vffint.d.l 48
  { 0b0111001010011110000011, 0, 5, 0 }, //vffint.d.lu 49
  { 0b0111001010011110000100, 0, 5, 0 }, //vffintl.d.w 50
  { 0b0111001010011110000101, 0, 5, 0 }, //vffinth.d.w 51
  { 0b0111001010011110001100, 0, 5, 0 }, //vftint.w.s 52
  { 0b0111001010011110001101, 0, 5, 0 }, //vftint.l.d 53
  { 0b0111001010011110001110, 0, 5, 0 }, //vftintrm.w.s 54
  { 0b0111001010011110001111, 0, 5, 0 }, //vftintrm.l.d 55
  { 0b0111001010011110010000, 0, 5, 0 }, //vftintrp.w.s 56
  { 0b0111001010011110010001, 0, 5, 0 }, //vftintrp.l.d 57
  { 0b0111001010011110010010, 0, 5, 0 }, //vftintrz.w.s 58
  { 0b0111001010011110010011, 0, 5, 0 }, //vftintrz.l.d 59
  { 0b0111001010011110010100, 0, 5, 0 }, //vftintrne.w.s 60
  { 0b0111001010011110010101, 0, 5, 0 }, //vftintrne.l.d 61
  { 0b0111001010011110010110, 0, 5, 0 }, //vftint.wu.s 62
  { 0b0111001010011110010111, 0, 5, 0 }, //vftint.lu.d 63
  { 0b0111001010011110011100, 0, 5, 0 }, //vftintrz.wu.s 64
  { 0b0111001010011110011101, 0, 5, 0 }, //vftintrz.lu.d 65
  { 0b0111001010011110100000, 0, 5, 0 }, //vftintl.l.s 66
  { 0b0111001010011110100001, 0, 5, 0 }, //vftinth.l.s 67
  { 0b0111001010011110100010, 0, 5, 0 }, //vftintrml.l.s 68
  { 0b0111001010011110100011, 0, 5, 0 }, //vftintrmh.l.s 69
  { 0b0111001010011110100100, 0, 5, 0 }, //vftintrpl.l.s 70
  { 0b0111001010011110100101, 0, 5, 0 }, //vftintrph.l.s 71
  { 0b0111001010011110100110, 0, 5, 0 }, //vftintrzl.l.s 72
  { 0b0111001010011110100111, 0, 5, 0 }, //vftintrzh.l.s 73
  { 0b0111001010011110101000, 0, 5, 0 }, //vftintrnel.l.s 74
  { 0b0111001010011110101001, 0, 5, 0 }, //vftintrneh.l.s 75
  { 0b0111001010011110111000, 0, 5, 0 }, //vexth.h.b 76
  { 0b0111001010011110111001, 0, 5, 0 }, //vexth.w.h 77
  { 0b0111001010011110111010, 0, 5, 0 }, //vexth.d.w 78
  { 0b0111001010011110111011, 0, 5, 0 }, //vexth.q.d 79
  { 0b0111001010011110111100, 0, 5, 0 }, //vexth.hu.bu 80
  { 0b0111001010011110111101, 0, 5, 0 }, //vexth.wu.hu 81
  { 0b0111001010011110111110, 0, 5, 0 }, //vexth.du.wu 82
  { 0b0111001010011110111111, 0, 5, 0 }, //vexth.qu.du 83
  { 0b0111001100001001000000, 0, 5, 0 }, //vextl.q.d 84
  { 0b0111001100001101000000, 0, 5, 0 }, //vextl.qu.du 85
};

const lasxXX lasxxx[103] = {
  { 0b0111011010011100000000, 0, 5, 0 }, //xvclo.b 0
  { 0b0111011010011100000001, 0, 5, 0 }, //xvclo.h 1
  { 0b0111011010011100000010, 0, 5, 0 }, //xvclo.w 2
  { 0b0111011010011100000011, 0, 5, 0 }, //xvclo.d 3
  { 0b0111011010011100000100, 0, 5, 0 }, //xvclz.b 4
  { 0b0111011010011100000101, 0, 5, 0 }, //xvclz.h 5
  { 0b0111011010011100000110, 0, 5, 0 }, //xvclz.w 6
  { 0b0111011010011100000111, 0, 5, 0 }, //xvclz.d 7
  { 0b0111011010011100001000, 0, 5, 0 }, //xvpcnt.b 8
  { 0b0111011010011100001001, 0, 5, 0 }, //xvpcnt.h 9
  { 0b0111011010011100001010, 0, 5, 0 }, //xvpcnt.w 10
  { 0b0111011010011100001011, 0, 5, 0 }, //xvpcnt.d 11
  { 0b0111011010011100001100, 0, 5, 0 }, //xvneg.b 12
  { 0b0111011010011100001101, 0, 5, 0 }, //xvneg.h 13
  { 0b0111011010011100001110, 0, 5, 0 }, //xvneg.w 14
  { 0b0111011010011100001111, 0, 5, 0 }, //xvneg.d 15
  { 0b0111011010011100010000, 0, 5, 0 }, //xvmskltz.b 16
  { 0b0111011010011100010001, 0, 5, 0 }, //xvmskltz.h 17
  { 0b0111011010011100010010, 0, 5, 0 }, //xvmskltz.w 18
  { 0b0111011010011100010011, 0, 5, 0 }, //xvmskltz.d 19
  { 0b0111011010011100010100, 0, 5, 0 }, //xvmskgez.b 20
  { 0b0111011010011100011000, 0, 5, 0 }, //xvmsknz.b 21
  { 0b0111011010011100110001, 0, 5, 0 }, //xvflogb.s 22
  { 0b0111011010011100110010, 0, 5, 0 }, //xvflogb.d 23
  { 0b0111011010011100110101, 0, 5, 0 }, //xvfclass.s 24
  { 0b0111011010011100110110, 0, 5, 0 }, //xvfclass.d 25
  { 0b0111011010011100111001, 0, 5, 0 }, //xvfsqrt.s 26
  { 0b0111011010011100111010, 0, 5, 0 }, //xvfsqrt.d 27
  { 0b0111011010011100111101, 0, 5, 0 }, //xvfrecip.s 28
  { 0b0111011010011100111110, 0, 5, 0 }, //xvfrecip.d 29
  { 0b0111011010011101000001, 0, 5, 0 }, //xvfrsqrt.s 30
  { 0b0111011010011101000010, 0, 5, 0 }, //xvfrsqrt.d 31
  { 0b0111011010011101001101, 0, 5, 0 }, //xvfrint.s 32
  { 0b0111011010011101001110, 0, 5, 0 }, //xvfrint.d 33
  { 0b0111011010011101010001, 0, 5, 0 }, //xvfrintrm.s 34
  { 0b0111011010011101010010, 0, 5, 0 }, //xvfrintrm.d 35
  { 0b0111011010011101010101, 0, 5, 0 }, //xvfrintrp.s 36
  { 0b0111011010011101010110, 0, 5, 0 }, //xvfrintrp.d 37
  { 0b0111011010011101011001, 0, 5, 0 }, //xvfrintrz.s 38
  { 0b0111011010011101011010, 0, 5, 0 }, //xvfrintrz.d 39
  { 0b0111011010011101011101, 0, 5, 0 }, //xvfrintrne.s 40
  { 0b0111011010011101011110, 0, 5, 0 }, //xvfrintrne.d 41
  { 0b0111011010011101111010, 0, 5, 0 }, //xvfcvtl.s.h 42
  { 0b0111011010011101111011, 0, 5, 0 }, //xvfcvth.s.h 43
  { 0b0111011010011101111100, 0, 5, 0 }, //xvfcvtl.d.s 44
  { 0b0111011010011101111101, 0, 5, 0 }, //xvfcvth.d.s 45
  { 0b0111011010011110000000, 0, 5, 0 }, //xvffint.s.w 46
  { 0b0111011010011110000001, 0, 5, 0 }, //xvffint.s.wu 47
  { 0b0111011010011110000010, 0, 5, 0 }, //xvffint.d.l 48
  { 0b0111011010011110000011, 0, 5, 0 }, //xvffint.d.lu 49
  { 0b0111011010011110000100, 0, 5, 0 }, //xvffintl.d.w 50
  { 0b0111011010011110000101, 0, 5, 0 }, //xvffinth.d.w 51
  { 0b0111011010011110001100, 0, 5, 0 }, //xvftint.w.s 52
  { 0b0111011010011110001101, 0, 5, 0 }, //xvftint.l.d 53
  { 0b0111011010011110001110, 0, 5, 0 }, //xvftintrm.w.s 54
  { 0b0111011010011110001111, 0, 5, 0 }, //xvftintrm.l.d 55
  { 0b0111011010011110010000, 0, 5, 0 }, //xvftintrp.w.s 56
  { 0b0111011010011110010001, 0, 5, 0 }, //xvftintrp.l.d 57
  { 0b0111011010011110010010, 0, 5, 0 }, //xvftintrz.w.s 58
  { 0b0111011010011110010011, 0, 5, 0 }, //xvftintrz.l.d 59
  { 0b0111011010011110010100, 0, 5, 0 }, //xvftintrne.w.s 60
  { 0b0111011010011110010101, 0, 5, 0 }, //xvftintrne.l.d 61
  { 0b0111011010011110010110, 0, 5, 0 }, //xvftint.wu.s 62
  { 0b0111011010011110010111, 0, 5, 0 }, //xvftint.lu.d 63
  { 0b0111011010011110011100, 0, 5, 0 }, //xvftintrz.wu.s 64
  { 0b0111011010011110011101, 0, 5, 0 }, //xvftintrz.lu.d 65
  { 0b0111011010011110100000, 0, 5, 0 }, //xvftintl.l.s 66
  { 0b0111011010011110100001, 0, 5, 0 }, //xvftinth.l.s 67
  { 0b0111011010011110100010, 0, 5, 0 }, //xvftintrml.l.s 68
  { 0b0111011010011110100011, 0, 5, 0 }, //xvftintrmh.l.s 69
  { 0b0111011010011110100100, 0, 5, 0 }, //xvftintrpl.l.s 70
  { 0b0111011010011110100101, 0, 5, 0 }, //xvftintrph.l.s 71
  { 0b0111011010011110100110, 0, 5, 0 }, //xvftintrzl.l.s 72
  { 0b0111011010011110100111, 0, 5, 0 }, //xvftintrzh.l.s 73
  { 0b0111011010011110101000, 0, 5, 0 }, //xvftintrnel.l.s 74
  { 0b0111011010011110101001, 0, 5, 0 }, //xvftintrneh.l.s 75
  { 0b0111011010011110111000, 0, 5, 0 }, //xvexth.h.b 76
  { 0b0111011010011110111001, 0, 5, 0 }, //xvexth.w.h 77
  { 0b0111011010011110111010, 0, 5, 0 }, //xvexth.d.w 78
  { 0b0111011010011110111011, 0, 5, 0 }, //xvexth.q.d 79
  { 0b0111011010011110111100, 0, 5, 0 }, //xvexth.hu.bu 80
  { 0b0111011010011110111101, 0, 5, 0 }, //xvexth.wu.hu 81
  { 0b0111011010011110111110, 0, 5, 0 }, //xvexth.du.wu 82
  { 0b0111011010011110111111, 0, 5, 0 }, //xvexth.qu.du 83
  { 0b0111011010011111000100, 0, 5, 0 }, //vext2xv.h.b 84
  { 0b0111011010011111000101, 0, 5, 0 }, //vext2xv.w.b 85
  { 0b0111011010011111000110, 0, 5, 0 }, //vext2xv.d.b 86
  { 0b0111011010011111000111, 0, 5, 0 }, //vext2xv.w.h 87
  { 0b0111011010011111001000, 0, 5, 0 }, //vext2xv.d.h 88
  { 0b0111011010011111001001, 0, 5, 0 }, //vext2xv.d.w 89
  { 0b0111011010011111001010, 0, 5, 0 }, //vext2xv.hu.bu 90
  { 0b0111011010011111001011, 0, 5, 0 }, //vext2xv.wu.bu 91
  { 0b0111011010011111001100, 0, 5, 0 }, //vext2xv.du.bu 92
  { 0b0111011010011111001101, 0, 5, 0 }, //vext2xv.wu.hu 93
  { 0b0111011010011111001110, 0, 5, 0 }, //vext2xv.du.hu 94
  { 0b0111011010011111001111, 0, 5, 0 }, //vext2xv.du.wu 95
  { 0b0111011100000111000000, 0, 5, 0 }, //xvreplve0.b 96
  { 0b0111011100000111100000, 0, 5, 0 }, //xvreplve0.h 97
  { 0b0111011100000111110000, 0, 5, 0 }, //xvreplve0.w 98
  { 0b0111011100000111111000, 0, 5, 0 }, //xvreplve0.d 99
  { 0b0111011100000111111100, 0, 5, 0 }, //xvreplve0.q 100
  { 0b0111011100001001000000, 0, 5, 0 }, //xvextl.q.d 101
  { 0b0111011100001101000000, 0, 5, 0 }, //xvextl.qu.du 102
};

const lsxIV lsxiv[10] = {
  { 0b0111001010011100100110, 0, 5, 0 }, //vseteqz.v 0
  { 0b0111001010011100100111, 0, 5, 0 }, //vsetnez.v 1
  { 0b0111001010011100101000, 0, 5, 0 }, //vsetanyeqz.b 2
  { 0b0111001010011100101001, 0, 5, 0 }, //vsetanyeqz.h 3
  { 0b0111001010011100101010, 0, 5, 0 }, //vsetanyeqz.w 4
  { 0b0111001010011100101011, 0, 5, 0 }, //vsetanyeqz.d 5
  { 0b0111001010011100101100, 0, 5, 0 }, //vsetallnez.b 6
  { 0b0111001010011100101101, 0, 5, 0 }, //vsetallnez.h 7
  { 0b0111001010011100101110, 0, 5, 0 }, //vsetallnez.w 8
  { 0b0111001010011100101111, 0, 5, 0 }, //vsetallnez.d 9
};

const lasxIX lasxix[10] = {
  { 0b0111011010011100100110, 0, 5, 0 }, //xvseteqz.v 0
  { 0b0111011010011100100111, 0, 5, 0 }, //xvsetnez.v 1
  { 0b0111011010011100101000, 0, 5, 0 }, //xvsetanyeqz.b 2
  { 0b0111011010011100101001, 0, 5, 0 }, //xvsetanyeqz.h 3
  { 0b0111011010011100101010, 0, 5, 0 }, //xvsetanyeqz.w 4
  { 0b0111011010011100101011, 0, 5, 0 }, //xvsetanyeqz.d 5
  { 0b0111011010011100101100, 0, 5, 0 }, //xvsetallnez.b 6
  { 0b0111011010011100101101, 0, 5, 0 }, //xvsetallnez.h 7
  { 0b0111011010011100101110, 0, 5, 0 }, //xvsetallnez.w 8
  { 0b0111011010011100101111, 0, 5, 0 }, //xvsetallnez.d 9
};

const lsxVR lsxvr[4] = {
  { 0b0111001010011111000000, 0, kWX, 5, 0 }, //vreplgr2vr.b 0
  { 0b0111001010011111000001, 0, kWX, 5, 0 }, //vreplgr2vr.h 1
  { 0b0111001010011111000010, 0, kWX, 5, 0 }, //vreplgr2vr.w 2
  { 0b0111001010011111000011, 0, kWX, 5, 0 }, //vreplgr2vr.d 3
};

const lasxXR lasxxr[4] = {
  { 0b0111011010011111000000, 0, kWX, 5, 0 }, //xvreplgr2vr.b 0
  { 0b0111011010011111000001, 0, kWX, 5, 0 }, //xvreplgr2vr.h 1
  { 0b0111011010011111000010, 0, kWX, 5, 0 }, //xvreplgr2vr.w 2
  { 0b0111011010011111000011, 0, kWX, 5, 0 }, //xvreplgr2vr.d 3
};

const lsxRVI lsxrvi[8] = {
  { 0b011100101110111110, kWX, 0, 5, 10, 0 }, //vpickve2gr.b 0
  { 0b0111001011101111110, kWX, 0, 5, 10, 1 }, //vpickve2gr.h 1
  { 0b01110010111011111110, kWX, 0, 5, 10, 2 }, //vpickve2gr.w 2
  { 0b011100101110111111110, kWX, 0, 5, 10, 3 }, //vpickve2gr.d 3
  { 0b011100101111001110, kWX, 0, 5, 10, 0 }, //vpickve2gr.bu 4
  { 0b0111001011110011110, kWX, 0, 5, 10, 1 }, //vpickve2gr.hu 5
  { 0b01110010111100111110, kWX, 0, 5, 10, 2 }, //vpickve2gr.wu 6
  { 0b011100101111001111110, kWX, 0, 5, 10, 3 }, //vpickve2gr.du 7
};

const lasxRXI lasxrxi[4] = {
  { 0b0111011011101111110, kWX, 0, 5, 10, 0 }, //xvpickve2gr.w 0
  { 0b01110110111011111110, kWX, 0, 5, 10, 1 }, //xvpickve2gr.d 1
  { 0b0111011011110011110, kWX, 0, 5, 10, 0 }, //xvpickve2gr.wu 2
  { 0b01110110111100111110, kWX, 0, 5, 10, 1 }, //xvpickve2gr.du 3
};

const lsxVI lsxvi[1] = {
  { 0b01110011111000, 0, 5, 0 }, //vldi 0
};

const lasxXI lasxxi[1] = {
  { 0b01110111111000, 0, 5, 0 }, //xvldi 0
};

const BTLRRI btLRRI[6] = {
  { 0b010110 },
  { 0b010111 },
  //{ 0b010011 },
  { 0b011000 },
  { 0b011001 },
  { 0b011010 },
  { 0b011011 },
};

const JBTLRRI jbtLRRI[1] = {
  { 0b010011 },
};

const BaseBLI basebLI[2] = {
  { 0b010100 },
  { 0b010101 },
};

const BaseBranchRel baseBranchRel[8] = {
  { 0b010100, OffsetFormat::kTypeLa64_BBL },
  { 0b010101, OffsetFormat::kTypeLa64_BBL },
  { 0b010110, OffsetFormat::kTypeLa64_BEQ },
  { 0b010111, OffsetFormat::kTypeLa64_BEQ },
  { 0b011000, OffsetFormat::kTypeLa64_BEQ },
  { 0b011001, OffsetFormat::kTypeLa64_BEQ },
  { 0b011010, OffsetFormat::kTypeLa64_BEQ },
  { 0b011011, OffsetFormat::kTypeLa64_BEQ },
};

const BaseLFIVV baseLFIVV[22] = {
  { 0b00001100000100000, 0, 5, 10, 0 }, // #0
  { 0b00001100001001000, 0, 5, 10, 0 }, // #1
  { 0b00001100000100100, 0, 5, 10, 0 }, // #2
  { 0b00001100000101100, 0, 5, 10, 0 }, // #3
  { 0b00001100001000010, 0, 5, 10, 0 }, // #4
  { 0b00001100000101010, 0, 5, 10, 0 }, // #5
  { 0b00001100001000110, 0, 5, 10, 0 }, // #6
  { 0b00001100000101110, 0, 5, 10, 0 }, // #7
  { 0b00001100001010000, 0, 5, 10, 0 }, // #8
  { 0b00001100000110100, 0, 5, 10, 0 }, // #9
  { 0b00001100001011000, 0, 5, 10, 0 }, // #10
  { 0b00001100000100001, 0, 5, 10, 0 }, // #11
  { 0b00001100001001001, 0, 5, 10, 0 }, // #12
  { 0b00001100000100101, 0, 5, 10, 0 }, // #13
  { 0b00001100001001101, 0, 5, 10, 0 }, // #14
  { 0b00001100000100011, 0, 5, 10, 0 }, // #15
  { 0b00001100001001011, 0, 5, 10, 0 }, // #16
  { 0b00001100000100111, 0, 5, 10, 0 }, // #17
  { 0b00001100001001111, 0, 5, 10, 0 }, // #18
  { 0b00001100000110001, 0, 5, 10, 0 }, // #19
  { 0b00001100001010101, 0, 5, 10, 0 }, // #20
  { 0b00001100000111001, 0, 5, 10, 0 }, // #21
};

const LfVVVI lfVVVI[1] = {
  { 0b00001101000000, 0, 5, 10, 3, 15, 0 }, // fsel
};

const LFldst lfldst[4] = {
  { 0b0010101100 },
  { 0b0010101101 },
  { 0b0010101110 },
  { 0b0010101111 },
};

const LPldst lpldst[1] = {
  { 0b0010101011 },
  //{ 0b0000011000 },
};

const LCldst lcldst[1] = {
  { 0b0000011000 },
};

const LFPldst lfpldst[8] = {
  { 0b00100100 },
  { 0b00100110 },
  { 0b00100101 },
  { 0b00100111 },
  { 0b00100000 },
  { 0b00100010 },
  { 0b00100001 },
  { 0b00100011 },
};
// ----------------------------------------------------------------------------
// ${EncodingData:End}

} // {EncodingData}
} // {InstDB}

/*
// ${CommonData:Begin}
// ------------------- Automatically generated, do not edit -------------------
const InstDB::CommonInfo InstDB::commonData[] = {
  { 0}  // #0 [ref=440x]
};
// ----------------------------------------------------------------------------
// ${CommonData:End}
*/

// ============================================================================
// [asmjit::ArmUtil - Id <-> Name]
// ============================================================================

#ifndef ASMJIT_DISABLE_TEXT
// ${NameData:Begin}
// ------------------- Automatically generated, do not edit -------------------
const char InstDB::_nameData[] =
  "\0" "add_d\0" "add_w\0" "addi_d\0" "addi_w\0" "addu16i_d\0" "alsl_d\0" "alsl_w\0" "alsl_wu\0" "amadd_d\0" "amadd_db_d\0" "amadd_db_w\0" "amadd_w\0" "amand_d\0" "amand_db_d\0" "amand_db_w\0" "amand_w\0" "ammax_d\0" "ammax_db_d\0" "ammax_db_du\0" "ammax_db_w\0" "ammax_db_wu\0" "ammax_du\0" "ammax_w\0" "ammax_wu\0" "ammin_d\0" "ammin_db_d\0" "ammin_db_du\0" "ammin_db_w\0" "ammin_db_wu\0" "ammin_du\0" "ammin_w\0" "ammin_wu\0" "amor_d\0" "amor_db_d\0" "amor_db_w\0" "amor_w\0" "amswap_d\0" "amswap_db_d\0" "amswap_db_w\0" "amswap_w\0" "amxor_d\0" "amxor_db_d\0" "amxor_db_w\0" "amxor_w\0" "and_\0" "andi\0" "andn\0" "asrtgt_d\0" "asrtle_d\0"
  "b\0" "bceqz\0" "bcnez\0" "beq\0" "bge\0" "bgeu\0" "bitrev_4b\0" "bitrev_8b\0" "bitrev_d\0" "bitrev_w\0" "bl\0" "blt\0" "bltu\0" "bne\0" "break_\0" "bstrins_d\0" "bstrins_w\0" "bstrpick_d\0" "bstrpick_w\0" "bytepick_d\0" "bytepick_w\0"
  "cacop\0" "clo_d\0" "clo_w\0" "clz_d\0" "clz_w\0" "cpucfg\0" "crc_w_b_w\0" "crc_w_d_w\0" "crc_w_h_w\0" "crc_w_w_w\0" "crcc_w_b_w\0" "crcc_w_d_w\0" "crcc_w_h_w\0" "crcc_w_w_w\0" "csrrd\0" "csrwr\0" "csrxchg\0" "cto_d\0" "cto_w\0" "ctz_d\0" "ctz_w\0"
  "dbar\0" "dbcl\0" "div_d\0" "div_du\0" "div_w\0" "div_wu\0"
  "ertn\0" "ext_w_b\0" "ext_w_h\0"
  "fabs_d\0" "fabs_s\0" "fadd_d\0" "fadd_s\0" "fclass_d\0" "fclass_s\0" "fcmp_caf_s\0" "fcmp_ceq_s\0" "fcmp_cle_d\0" "fcmp_clt_d\0" "fcmp_cne_d\0" "fcmp_cor_s\0" "fcmp_cueq_s\0" "fcmp_cule_s\0" "fcmp_cult_s\0" "fcmp_cun_d\0" "fcmp_cune_d\0" "fcmp_saf_s\0" "fcmp_seq_s\0" "fcmp_sle_s\0" "fcmp_slt_s\0" "fcmp_sne_s\0" "fcmp_sor_d\0" "fcmp_sueq_d\0" "fcmp_sule_d\0" "fcmp_sult_d\0" "fcmp_sun_d\0" "fcmp_sune_s\0" "fcopysign_d\0" "fcopysign_s\0" "fcvt_d_s\0" "fcvt_s_d\0" "fdiv_d\0" "fdiv_s\0" "ffint_d_l\0" "ffint_d_w\0" "ffint_s_l\0" "ffint_s_w\0" "fld_d\0" "fld_s\0" "fldgt_d\0" "fldgt_s\0" "fldle_d\0" "fldle_s\0" "fldx_d\0" "fldx_s\0" "flogb_d\0" "flogb_s\0" "fmadd_d\0" "fmadd_s\0" "fmax_d\0" "fmax_s\0" "fmaxa_d\0" "fmaxa_s\0" "fmin_d\0" "fmin_s\0" "fmina_d\0" "fmina_s\0" "fmov_d\0" "fmov_s\0" "fmul_d\0" "fmul_s\0" "fneg_d\0" "fneg_s\0" "frecip_d\0" "frecip_s\0" "frint_d\0" "frint_s\0" "frsqrt_d\0" "frsqrt_s\0" "fscaleb_d\0" "fscaleb_s\0" "fsel\0" "fsqrt_d\0" "fsqrt_s\0" "fst_d\0" "fst_s\0" "fstgt_d\0" "fstgt_s\0" "fstle_d\0" "fstle_s\0" "fstx_d\0" "fstx_s\0" "fsub_d\0" "fsub_s\0" "ftint_l_d\0" "ftint_l_s\0" "ftint_w_d\0" "ftint_w_s\0" "ftintrm_l_d\0" "ftintrm_l_s\0" "ftintrm_w_d\0" "ftintrm_w_s\0" "ftintrne_l_d\0" "ftintrne_l_s\0" "ftintrne_w_d\0" "ftintrne_w_s\0" "ftintrp_l_d\0" "ftintrp_l_s\0" "ftintrp_w_d\0" "ftintrp_w_s\0" "ftintrz_l_d\0" "ftintrz_l_s\0" "ftintrz_w_d\0" "ftintrz_w_s\0"
  "ibar\0" "idle\0" "invtlb\0" "iocsrrd_b\0" "iocsrrd_d\0" "iocsrrd_h\0" "iocsrrd_w\0" "iocsrwr_b\0" "iocsrwr_d\0" "iocsrwr_h\0" "iocsrwr_w\0"
  "jirl\0"
  "ld_b\0" "ld_bu\0" "ld_d\0" "ld_h\0" "ld_hu\0" "ld_w\0" "ld_wu\0" "lddir\0" "ldgt_b\0" "ldgt_d\0" "ldgt_h\0" "ldgt_w\0" "ldle_b\0" "ldle_d\0" "ldle_h\0" "ldle_w\0" "ldpte\0" "ldptr_d\0" "ldptr_w\0" "ldx_b\0" "ldx_bu\0" "ldx_d\0" "ldx_h\0" "ldx_hu\0" "ldx_w\0" "ldx_wu\0" "ll_d\0" "ll_w\0" "lu12i_w\0" "lu32i_d\0" "lu52i_d\0"
  "maskeqz\0" "masknez\0" "mod_d\0" "mod_du\0" "mod_w\0" "mod_wu\0" "movcf2fr\0" "movcf2gr\0" "movfcsr2gr\0" "movfr2cf\0" "movfr2gr_d\0" "movfr2gr_s\0" "movfrh2gr_s\0" "movgr2cf\0" "movgr2fcsr\0" "movgr2fr_d\0" "movgr2fr_w\0" "movgr2frh_w\0" "mul_d\0" "mul_w\0" "mulh_d\0" "mulh_du\0" "mulh_w\0" "mulh_wu\0" "mulw_d_w\0" "mulw_d_wu\0"
  "nor\0"
  "or_\0" "ori\0" "orn\0"
  "pcaddi\0" "pcaddu12i\0" "pcaddu18i\0" "pcalau12i\0" "preld\0"
  "rdtime_d\0" "rdtimeh_w\0" "rdtimel_w\0" "revb_2h\0" "revb_2w\0" "revb_4h\0" "revb_d\0" "revh_2w\0" "revh_d\0" "rotr_d\0" "rotr_w\0" "rotri_d\0" "rotri_w\0"
  "sc_d\0" "sc_w\0" "sll_d\0" "sll_w\0" "slli_d\0" "slli_w\0" "slti\0" "sltui\0" "sra_d\0" "sra_w\0" "srai_d\0" "srai_w\0" "srl_d\0" "srl_w\0" "srli_d\0" "srli_w\0" "st_b\0" "st_d\0" "st_h\0" "st_w\0" "stgt_b\0" "stgt_d\0" "stgt_h\0" "stgt_w\0" "stle_b\0" "stle_d\0" "stle_h\0" "stle_w\0" "stptr_d\0" "stptr_w\0" "stx_b\0" "stx_d\0" "stx_h\0" "stx_w\0" "syscall\0"
  "tlbclr\0" "tlbfill\0" "tlbflush\0" "tlbrd\0" "tlbsrch\0" "tlbwr\0"
  "xor_\0" "xori\0"
  "vabsd_b\0" "vabsd_bu\0" "vabsd_d\0" "vabsd_du\0" "vabsd_h\0" "vabsd_hu\0" "vabsd_w\0" "vabsd_wu\0" "vadd_b\0" "vadd_d\0" "vadd_h\0" "vadd_q\0" "vadd_w\0" "vadda_b\0" "vadda_d\0" "vadda_h\0" "vadda_w\0" "vaddi_bu\0" "vaddi_du\0" "vaddi_hu\0" "vaddi_wu\0" "vaddwev_d_w\0" "vaddwev_d_wu\0" "vaddwev_d_wu_w\0" "vaddwev_h_b\0" "vaddwev_h_bu\0" "vaddwev_h_bu_b\0" "vaddwev_q_d\0" "vaddwev_q_du\0" "vaddwev_q_du_d\0" "vaddwev_w_h\0" "vaddwev_w_hu\0" "vaddwev_w_hu_h\0" "vaddwod_d_w\0" "vaddwod_d_wu\0" "vaddwod_d_wu_w\0" "vaddwod_h_b\0" "vaddwod_h_bu\0" "vaddwod_h_bu_b\0" "vaddwod_q_d\0" "vaddwod_q_du\0" "vaddwod_q_du_d\0" "vaddwod_w_h\0" "vaddwod_w_hu\0" "vaddwod_w_hu_h\0" "vand_v\0" "vandi_b\0" "vandn_v\0" "vavg_b\0" "vavg_bu\0" "vavg_d\0" "vavg_du\0" "vavg_h\0" "vavg_hu\0" "vavg_w\0" "vavg_wu\0" "vavgr_b\0" "vavgr_bu\0" "vavgr_d\0" "vavgr_du\0" "vavgr_h\0" "vavgr_hu\0" "vavgr_w\0" "vavgr_wu\0" "vbitclr_b\0" "vbitclr_d\0" "vbitclr_h\0" "vbitclr_w\0" "vbitclri_b\0" "vbitclri_d\0" "vbitclri_h\0" "vbitclri_w\0" "vbitrev_b\0" "vbitrev_d\0" "vbitrev_h\0" "vbitrev_w\0" "vbitrevi_b\0" "vbitrevi_d\0" "vbitrevi_h\0" "vbitrevi_w\0" "vbitsel_v\0" "vbitseli_b\0" "vbitset_b\0" "vbitset_d\0" "vbitset_h\0" "vbitset_w\0" "vbitseti_b\0" "vbitseti_d\0" "vbitseti_h\0" "vbitseti_w\0" "vbsll_v\0" "vbsrl_v\0" "vclo_b\0" "vclo_d\0" "vclo_h\0" "vclo_w\0" "vclz_b\0" "vclz_d\0" "vclz_h\0" "vclz_w\0" "vdiv_b\0" "vdiv_bu\0" "vdiv_d\0" "vdiv_du\0" "vdiv_h\0" "vdiv_hu\0" "vdiv_w\0" "vdiv_wu\0" "vext2xv_d_b\0" "vext2xv_d_h\0" "vext2xv_d_w\0" "vext2xv_du_bu\0" "vext2xv_du_hu\0" "vext2xv_du_wu\0" "vext2xv_h_b\0" "vext2xv_hu_bu\0" "vext2xv_w_b\0" "vext2xv_w_h\0" "vext2xv_wu_bu\0" "vext2xv_wu_hu\0" "vexth_d_w\0" "vexth_du_wu\0" "vexth_h_b\0" "vexth_hu_bu\0" "vexth_q_d\0" "vexth_qu_du\0" "vexth_w_h\0" "vexth_wu_hu\0" "vextl_q_d\0" "vextl_qu_du\0" "vextrins_b\0" "vextrins_d\0" "vextrins_h\0" "vextrins_w\0" "vfadd_d\0" "vfadd_s\0" "vfclass_d\0" "vfclass_s\0" "vfcvt_h_s\0" "vfcvt_s_d\0" "vfcvth_d_s\0" "vfcvth_s_h\0" "vfcvtl_d_s\0" "vfcvtl_s_h\0" "vfdiv_d\0" "vfdiv_s\0" "vffint_d_l\0" "vffint_d_lu\0" "vffint_s_l\0" "vffint_s_w\0" "vffint_s_wu\0" "vffinth_d_w\0" "vffintl_d_w\0" "vflogb_d\0" "vflogb_s\0" "vfmadd_d\0" "vfmadd_s\0" "vfmax_d\0" "vfmax_s\0" "vfmaxa_d\0" "vfmaxa_s\0" "vfmin_d\0" "vfmin_s\0" "vfmina_d\0" "vfmina_s\0" "vfmsub_d\0" "vfmsub_s\0" "vfmul_d\0" "vfmul_s\0" "vfnmadd_d\0" "vfnmadd_s\0" "vfnmsub_d\0" "vfnmsub_s\0" "vfrecip_d\0" "vfrecip_s\0" "vfrint_d\0" "vfrint_s\0" "vfrintrm_d\0" "vfrintrm_s\0" "vfrintrne_d\0" "vfrintrne_s\0" "vfrintrp_d\0" "vfrintrp_s\0" "vfrintrz_d\0" "vfrintrz_s\0" "vfrsqrt_d\0" "vfrsqrt_s\0" "vfrstp_b\0" "vfrstp_h\0" "vfrstpi_b\0" "vfrstpi_h\0" "vfscaleb_d\0" "vfscaleb_s\0" "vfsqrt_d\0" "vfsqrt_s\0" "vfsub_d\0" "vfsub_s\0" "vftint_l_d\0" "vftint_lu_d\0" "vftint_w_d\0" "vftint_w_s\0" "vftint_wu_s\0" "vftinth_l_s\0" "vftintl_l_s\0" "vftintrm_l_d\0" "vftintrm_w_d\0" "vftintrm_w_s\0" "vftintrmh_l_s\0" "vftintrml_l_s\0" "vftintrne_l_d\0" "vftintrne_w_d\0" "vftintrne_w_s\0" "vftintrneh_l_s\0" "vftintrnel_l_s\0" "vftintrp_l_d\0" "vftintrp_w_d\0" "vftintrp_w_s\0" "vftintrph_l_s\0" "vftintrpl_l_s\0" "vftintrz_l_d\0" "vftintrz_lu_d\0" "vftintrz_w_d\0" "vftintrz_w_s\0" "vftintrz_wu_s\0" "vftintrzh_l_s\0" "vftintrzl_l_s\0" "vhaddw_d_w\0" "vhaddw_du_wu\0" "vhaddw_h_b\0" "vhaddw_hu_bu\0" "vhaddw_q_d\0" "vhaddw_qu_du\0" "vhaddw_w_h\0" "vhaddw_wu_hu\0" "vhsubw_d_w\0" "vhsubw_du_wu\0" "vhsubw_h_b\0" "vhsubw_hu_bu\0" "vhsubw_q_d\0" "vhsubw_qu_du\0" "vhsubw_w_h\0" "vhsubw_wu_hu\0" "vilvh_b\0" "vilvh_d\0" "vilvh_h\0" "vilvh_w\0" "vilvl_b\0" "vilvl_d\0" "vilvl_h\0" "vilvl_w\0" "vinsgr2vr_b\0" "vinsgr2vr_d\0" "vinsgr2vr_h\0" "vinsgr2vr_w\0" "vld\0" "vldi\0" "vldrepl_b\0" "vldrepl_d\0" "vldrepl_h\0" "vldrepl_w\0" "vldx\0" "vmadd_b\0" "vmadd_d\0" "vmadd_h\0" "vmadd_w\0" "vmaddwev_d_w\0" "vmaddwev_d_wu\0" "vmaddwev_d_wu_w\0" "vmaddwev_h_b\0" "vmaddwev_h_bu\0" "vmaddwev_h_bu_b\0" "vmaddwev_q_d\0" "vmaddwev_q_du\0" "vmaddwev_q_du_d\0" "vmaddwev_w_h\0" "vmaddwev_w_hu\0" "vmaddwev_w_hu_h\0" "vmaddwod_d_w\0" "vmaddwod_d_wu\0" "vmaddwod_d_wu_w\0" "vmaddwod_h_b\0" "vmaddwod_h_bu\0" "vmaddwod_h_bu_b\0" "vmaddwod_q_d\0" "vmaddwod_q_du\0" "vmaddwod_q_du_d\0" "vmaddwod_w_h\0" "vmaddwod_w_hu\0" "vmaddwod_w_hu_h\0" "vmax_b\0" "vmax_bu\0" "vmax_d\0" "vmax_du\0" "vmax_h\0" "vmax_hu\0" "vmax_w\0" "vmax_wu\0" "vmaxi_b\0" "vmaxi_bu\0" "vmaxi_d\0" "vmaxi_du\0" "vmaxi_h\0" "vmaxi_hu\0" "vmaxi_w\0" "vmaxi_wu\0" "vmepatmsk_v\0" "vmin_b\0" "vmin_bu\0" "vmin_d\0" "vmin_du\0" "vmin_h\0" "vmin_hu\0" "vmin_w\0" "vmin_wu\0" "vmini_b\0" "vmini_bu\0" "vmini_d\0" "vmini_du\0" "vmini_h\0" "vmini_hu\0" "vmini_w\0" "vmini_wu\0" "vmod_b\0" "vmod_bu\0" "vmod_d\0" "vmod_du\0" "vmod_h\0" "vmod_hu\0" "vmod_w\0" "vmod_wu\0" "vmskgez_b\0" "vmskltz_b\0" "vmskltz_d\0" "vmskltz_h\0" "vmskltz_w\0" "vmsknz_b\0" "vmsub_b\0" "vmsub_d\0" "vmsub_h\0" "vmsub_w\0" "vmuh_b\0" "vmuh_bu\0" "vmuh_d\0" "vmuh_du\0" "vmuh_h\0" "vmuh_hu\0" "vmuh_w\0" "vmuh_wu\0" "vmul_b\0" "vmul_d\0" "vmul_h\0" "vmul_w\0" "vmulwev_d_w\0" "vmulwev_d_wu\0" "vmulwev_d_wu_w\0" "vmulwev_h_b\0" "vmulwev_h_bu\0" "vmulwev_h_bu_b\0" "vmulwev_q_d\0" "vmulwev_q_du\0" "vmulwev_q_du_d\0" "vmulwev_w_h\0" "vmulwev_w_hu\0" "vmulwev_w_hu_h\0" "vmulwod_d_w\0" "vmulwod_d_wu\0" "vmulwod_d_wu_w\0" "vmulwod_h_b\0" "vmulwod_h_bu\0" "vmulwod_h_bu_b\0" "vmulwod_q_d\0" "vmulwod_q_du\0" "vmulwod_q_du_d\0" "vmulwod_w_h\0" "vmulwod_w_hu\0" "vmulwod_w_hu_h\0" "vneg_b\0" "vneg_d\0" "vneg_h\0" "vneg_w\0" "vnor_v\0" "vnori_b\0" "vor_v\0" "vori_b\0" "vorn_v\0" "vpackev_b\0" "vpackev_d\0" "vpackev_h\0" "vpackev_w\0" "vpackod_b\0" "vpackod_d\0" "vpackod_h\0" "vpackod_w\0" "vpcnt_b\0" "vpcnt_d\0" "vpcnt_h\0" "vpcnt_w\0" "vpermi_w\0" "vpickev_b\0" "vpickev_d\0" "vpickev_h\0" "vpickev_w\0" "vpickod_b\0" "vpickod_d\0" "vpickod_h\0" "vpickod_w\0" "vpickve2gr_b\0" "vpickve2gr_bu\0" "vpickve2gr_d\0" "vpickve2gr_du\0" "vpickve2gr_h\0" "vpickve2gr_hu\0" "vpickve2gr_w\0" "vpickve2gr_wu\0" "vreplgr2vr_b\0" "vreplgr2vr_d\0" "vreplgr2vr_h\0" "vreplgr2vr_w\0" "vreplve_b\0" "vreplve_d\0" "vreplve_h\0" "vreplve_w\0" "vreplvei_b\0" "vreplvei_d\0" "vreplvei_h\0" "vreplvei_w\0" "vrotr_b\0" "vrotr_d\0" "vrotr_h\0" "vrotr_w\0" "vrotri_b\0" "vrotri_d\0" "vrotri_h\0" "vrotri_w\0" "vsadd_b\0" "vsadd_bu\0" "vsadd_d\0" "vsadd_du\0" "vsadd_h\0" "vsadd_hu\0" "vsadd_w\0" "vsadd_wu\0" "vsat_b\0" "vsat_bu\0" "vsat_d\0" "vsat_du\0" "vsat_h\0" "vsat_hu\0" "vsat_w\0" "vsat_wu\0" "vseq_b\0" "vseq_d\0" "vseq_h\0" "vseq_w\0" "vseqi_b\0" "vseqi_d\0" "vseqi_h\0" "vseqi_w\0" "vsetallnez_b\0" "vsetallnez_d\0" "vsetallnez_h\0" "vsetallnez_w\0" "vsetanyeqz_b\0" "vsetanyeqz_d\0" "vsetanyeqz_h\0" "vsetanyeqz_w\0" "vseteqz_v\0" "vsetnez_v\0" "vshuf4i_b\0" "vshuf4i_d\0" "vshuf4i_h\0" "vshuf4i_w\0" "vshuf_b\0" "vshuf_d\0" "vshuf_h\0" "vshuf_w\0" "vsigncov_b\0" "vsigncov_d\0" "vsigncov_h\0" "vsigncov_w\0" "vsle_b\0" "vsle_bu\0" "vsle_d\0" "vsle_du\0" "vsle_h\0" "vsle_hu\0" "vsle_w\0" "vsle_wu\0" "vslei_b\0" "vslei_bu\0" "vslei_d\0" "vslei_du\0" "vslei_h\0" "vslei_hu\0" "vslei_w\0" "vslei_wu\0" "vsll_b\0" "vsll_d\0" "vsll_h\0" "vsll_w\0" "vslli_b\0" "vslli_d\0" "vslli_h\0" "vslli_w\0" "vsllwil_d_w\0" "vsllwil_du_wu\0" "vsllwil_h_b\0" "vsllwil_hu_bu\0" "vsllwil_w_h\0" "vsllwil_wu_hu\0" "vslt_b\0" "vslt_bu\0" "vslt_d\0" "vslt_du\0" "vslt_h\0" "vslt_hu\0" "vslt_w\0" "vslt_wu\0" "vslti_b\0" "vslti_bu\0" "vslti_d\0" "vslti_du\0" "vslti_h\0" "vslti_hu\0" "vslti_w\0" "vslti_wu\0" "vsra_b\0" "vsra_d\0" "vsra_h\0" "vsra_w\0" "vsrai_b\0" "vsrai_d\0" "vsrai_h\0" "vsrai_w\0" "vsran_b_h\0" "vsran_h_w\0" "vsran_w_d\0" "vsrani_b_h\0" "vsrani_d_q\0" "vsrani_h_w\0" "vsrani_w_d\0" "vsrar_b\0" "vsrar_d\0" "vsrar_h\0" "vsrar_w\0" "vsrari_b\0" "vsrari_d\0" "vsrari_h\0" "vsrari_w\0" "vsrarn_b_h\0" "vsrarn_h_w\0" "vsrarn_w_d\0" "vsrarni_b_h\0" "vsrarni_d_q\0" "vsrarni_h_w\0" "vsrarni_w_d\0" "vsrl_b\0" "vsrl_d\0" "vsrl_h\0" "vsrl_w\0" "vsrli_b\0" "vsrli_d\0" "vsrli_h\0" "vsrli_w\0" "vsrln_b_h\0" "vsrln_h_w\0" "vsrln_w_d\0" "vsrlni_b_h\0" "vsrlni_d_q\0" "vsrlni_h_w\0" "vsrlni_w_d\0" "vsrlr_b\0" "vsrlr_d\0" "vsrlr_h\0" "vsrlr_w\0" "vsrlri_b\0" "vsrlri_d\0" "vsrlri_h\0" "vsrlri_w\0" "vsrlrn_b_h\0" "vsrlrn_h_w\0" "vsrlrn_w_d\0" "vssran_b_h\0" "vssran_bu_h\0" "vssran_h_w\0" "vssran_hu_w\0" "vssran_w_d\0" "vssran_wu_d\0" "vssrani_b_h\0" "vssrani_bu_h\0" "vssrani_d_q\0" "vssrani_du_q\0" "vssrani_h_w\0" "vssrani_hu_w\0" "vssrani_w_d\0" "vssrani_wu_d\0" "vssrarn_b_h\0" "vssrarn_bu_h\0" "vssrarn_h_w\0" "vssrarn_hu_w\0" "vssrarn_w_d\0" "vssrarn_wu_d\0" "vssrarni_b_h\0" "vssrarni_bu_h\0" "vssrarni_d_q\0" "vssrarni_du_q\0" "vssrarni_h_w\0" "vssrarni_hu_w\0" "vssrarni_w_d\0" "vssrarni_wu_d\0" "vssrln_b_h\0" "vssrln_bu_h\0" "vssrln_h_w\0" "vssrln_hu_w\0" "vssrln_w_d\0" "vssrln_wu_d\0" "vssrlni_b_h\0" "vssrlni_bu_h\0" "vssrlni_d_q\0" "vssrlni_du_q\0" "vssrlni_h_w\0" "vssrlni_hu_w\0" "vssrlni_w_d\0" "vssrlni_wu_d\0" "vssrlrn_b_h\0" "vssrlrn_bu_h\0" "vssrlrn_h_w\0" "vssrlrn_hu_w\0" "vssrlrn_w_d\0" "vssrlrn_wu_d\0" "vssrlrni_b_h\0" "vssrlrni_bu_h\0" "vssrlrni_d_q\0" "vssrlrni_du_q\0" "vssrlrni_h_w\0" "vssrlrni_hu_w\0" "vssrlrni_w_d\0" "vssrlrni_wu_d\0" "vssub_b\0" "vssub_bu\0" "vssub_d\0" "vssub_du\0" "vssub_h\0" "vssub_hu\0" "vssub_w\0" "vssub_wu\0" "vst\0" "vstelm_b\0" "vstelm_d\0" "vstelm_h\0" "vstelm_w\0" "vstx\0" "vsub_b\0" "vsub_d\0" "vsub_h\0" "vsub_q\0" "vsub_w\0" "vsubi_bu\0" "vsubi_du\0" "vsubi_hu\0" "vsubi_wu\0" "vsubwev_d_w\0" "vsubwev_d_wu\0" "vsubwev_h_b\0" "vsubwev_h_bu\0" "vsubwev_q_d\0" "vsubwev_q_du\0" "vsubwev_w_h\0" "vsubwev_w_hu\0" "vsubwod_d_w\0" "vsubwod_d_wu\0" "vsubwod_h_b\0" "vsubwod_h_bu\0" "vsubwod_q_d\0" "vsubwod_q_du\0" "vsubwod_w_h\0" "vsubwod_w_hu\0" "vxor_v\0" "vxori_b\0"
  "xvabsd_b\0" "xvabsd_bu\0" "xvabsd_d\0" "xvabsd_du\0" "xvabsd_h\0" "xvabsd_hu\0" "xvabsd_w\0" "xvabsd_wu\0" "xvadd_b\0" "xvadd_d\0" "xvadd_h\0" "xvadd_q\0" "xvadd_w\0" "xvadda_b\0" "xvadda_d\0" "xvadda_h\0" "xvadda_w\0" "xvaddi_bu\0" "xvaddi_du\0" "xvaddi_hu\0" "xvaddi_wu\0" "xvaddwev_d_w\0" "xvaddwev_d_wu\0" "xvaddwev_d_wu_w\0" "xvaddwev_h_b\0" "xvaddwev_h_bu\0" "xvaddwev_h_bu_b\0" "xvaddwev_q_d\0" "xvaddwev_q_du\0" "xvaddwev_q_du_d\0" "xvaddwev_w_h\0" "xvaddwev_w_hu\0" "xvaddwev_w_hu_h\0" "xvaddwod_d_w\0" "xvaddwod_d_wu\0" "xvaddwod_d_wu_w\0" "xvaddwod_h_b\0" "xvaddwod_h_bu\0" "xvaddwod_h_bu_b\0" "xvaddwod_q_d\0" "xvaddwod_q_du\0" "xvaddwod_q_du_d\0" "xvaddwod_w_h\0" "xvaddwod_w_hu\0" "xvaddwod_w_hu_h\0" "xvand_v\0" "xvandi_b\0" "xvandn_v\0" "xvavg_b\0" "xvavg_bu\0" "xvavg_d\0" "xvavg_du\0" "xvavg_h\0" "xvavg_hu\0" "xvavg_w\0" "xvavg_wu\0" "xvavgr_b\0" "xvavgr_bu\0" "xvavgr_d\0" "xvavgr_du\0" "xvavgr_h\0" "xvavgr_hu\0" "xvavgr_w\0" "xvavgr_wu\0" "xvbitclr_b\0" "xvbitclr_d\0" "xvbitclr_h\0" "xvbitclr_w\0" "xvbitclri_b\0" "xvbitclri_d\0" "xvbitclri_h\0" "xvbitclri_w\0" "xvbitrev_b\0" "xvbitrev_d\0" "xvbitrev_h\0" "xvbitrev_w\0" "xvbitrevi_b\0" "xvbitrevi_d\0" "xvbitrevi_h\0" "xvbitrevi_w\0" "xvbitsel_v\0" "xvbitseli_b\0" "xvbitset_b\0" "xvbitset_d\0" "xvbitset_h\0" "xvbitset_w\0" "xvbitseti_b\0" "xvbitseti_d\0" "xvbitseti_h\0" "xvbitseti_w\0" "xvbsll_v\0" "xvbsrl_v\0" "xvclo_b\0" "xvclo_d\0" "xvclo_h\0" "xvclo_w\0" "xvclz_b\0" "xvclz_d\0" "xvclz_h\0" "xvclz_w\0" "xvdiv_b\0" "xvdiv_bu\0" "xvdiv_d\0" "xvdiv_du\0" "xvdiv_h\0" "xvdiv_hu\0" "xvdiv_w\0" "xvdiv_wu\0" "xvexth_d_w\0" "xvexth_du_wu\0" "xvexth_h_b\0" "xvexth_hu_bu\0" "xvexth_q_d\0" "xvexth_qu_du\0" "xvexth_w_h\0" "xvexth_wu_hu\0" "xvextl_q_d\0" "xvextl_qu_du\0" "xvextrins_b\0" "xvextrins_d\0" "xvextrins_h\0" "xvextrins_w\0" "xvfadd_d\0" "xvfadd_s\0" "xvfclass_d\0" "xvfclass_s\0" "xvfcvt_h_s\0" "xvfcvt_s_d\0" "xvfcvth_d_s\0" "xvfcvth_s_h\0" "xvfcvtl_d_s\0" "xvfcvtl_s_h\0" "xvfdiv_d\0" "xvfdiv_s\0" "xvffint_d_l\0" "xvffint_d_lu\0" "xvffint_s_l\0" "xvffint_s_w\0" "xvffint_s_wu\0" "xvffinth_d_w\0" "xvffintl_d_w\0" "xvflogb_d\0" "xvflogb_s\0" "xvfmadd_d\0" "xvfmadd_s\0" "xvfmax_d\0" "xvfmax_s\0" "xvfmaxa_d\0" "xvfmaxa_s\0" "xvfmin_d\0" "xvfmin_s\0" "xvfmina_d\0" "xvfmina_s\0" "xvfmsub_d\0" "xvfmsub_s\0" "xvfmul_d\0" "xvfmul_s\0" "xvfnmadd_d\0" "xvfnmadd_s\0" "xvfnmsub_d\0" "xvfnmsub_s\0" "xvfrecip_d\0" "xvfrecip_s\0" "xvfrint_d\0" "xvfrint_s\0" "xvfrintrm_d\0" "xvfrintrm_s\0" "xvfrintrne_d\0" "xvfrintrne_s\0" "xvfrintrp_d\0" "xvfrintrp_s\0" "xvfrintrz_d\0" "xvfrintrz_s\0" "xvfrsqrt_d\0" "xvfrsqrt_s\0" "xvfrstp_b\0" "xvfrstp_h\0" "xvfrstpi_b\0" "xvfrstpi_h\0" "xvfscaleb_d\0" "xvfscaleb_s\0" "xvfsqrt_d\0" "xvfsqrt_s\0" "xvfsub_d\0" "xvfsub_s\0" "xvftint_l_d\0" "xvftint_lu_d\0" "xvftint_w_d\0" "xvftint_w_s\0" "xvftint_wu_s\0" "xvftinth_l_s\0" "xvftintl_l_s\0" "xvftintrm_l_d\0" "xvftintrm_w_d\0" "xvftintrm_w_s\0" "xvftintrmh_l_s\0" "xvftintrml_l_s\0" "xvftintrne_l_d\0" "xvftintrne_w_d\0" "xvftintrne_w_s\0" "xvftintrneh_l_s\0" "xvftintrnel_l_s\0" "xvftintrp_l_d\0" "xvftintrp_w_d\0" "xvftintrp_w_s\0" "xvftintrph_l_s\0" "xvftintrpl_l_s\0" "xvftintrz_l_d\0" "xvftintrz_lu_d\0" "xvftintrz_w_d\0" "xvftintrz_w_s\0" "xvftintrz_wu_s\0" "xvftintrzh_l_s\0" "xvftintrzl_l_s\0" "xvhaddw_d_w\0" "xvhaddw_du_wu\0" "xvhaddw_h_b\0" "xvhaddw_hu_bu\0" "xvhaddw_q_d\0" "xvhaddw_qu_du\0" "xvhaddw_w_h\0" "xvhaddw_wu_hu\0" "xvhseli_d\0" "xvhsubw_d_w\0" "xvhsubw_du_wu\0" "xvhsubw_h_b\0" "xvhsubw_hu_bu\0" "xvhsubw_q_d\0" "xvhsubw_qu_du\0" "xvhsubw_w_h\0" "xvhsubw_wu_hu\0" "xvilvh_b\0" "xvilvh_d\0" "xvilvh_h\0" "xvilvh_w\0" "xvilvl_b\0" "xvilvl_d\0" "xvilvl_h\0" "xvilvl_w\0" "xvinsgr2vr_d\0" "xvinsgr2vr_w\0" "xvinsve0_d\0" "xvinsve0_w\0" "xvld\0" "xvldi\0" "xvldrepl_b\0" "xvldrepl_d\0" "xvldrepl_h\0" "xvldrepl_w\0" "xvldx\0" "xvmadd_b\0" "xvmadd_d\0" "xvmadd_h\0" "xvmadd_w\0" "xvmaddwev_d_w\0" "xvmaddwev_d_wu\0" "xvmaddwev_d_wu_w\0" "xvmaddwev_h_b\0" "xvmaddwev_h_bu\0" "xvmaddwev_h_bu_b\0" "xvmaddwev_q_d\0" "xvmaddwev_q_du\0" "xvmaddwev_q_du_d\0" "xvmaddwev_w_h\0" "xvmaddwev_w_hu\0" "xvmaddwev_w_hu_h\0" "xvmaddwod_d_w\0" "xvmaddwod_d_wu\0" "xvmaddwod_d_wu_w\0" "xvmaddwod_h_b\0" "xvmaddwod_h_bu\0" "xvmaddwod_h_bu_b\0" "xvmaddwod_q_d\0" "xvmaddwod_q_du\0" "xvmaddwod_q_du_d\0" "xvmaddwod_w_h\0" "xvmaddwod_w_hu\0" "xvmaddwod_w_hu_h\0" "xvmax_b\0" "xvmax_bu\0" "xvmax_d\0" "xvmax_du\0" "xvmax_h\0" "xvmax_hu\0" "xvmax_w\0" "xvmax_wu\0" "xvmaxi_b\0" "xvmaxi_bu\0" "xvmaxi_d\0" "xvmaxi_du\0" "xvmaxi_h\0" "xvmaxi_hu\0" "xvmaxi_w\0" "xvmaxi_wu\0" "xvmepatmsk_v\0" "xvmin_b\0" "xvmin_bu\0" "xvmin_d\0" "xvmin_du\0" "xvmin_h\0" "xvmin_hu\0" "xvmin_w\0" "xvmin_wu\0" "xvmini_b\0" "xvmini_bu\0" "xvmini_d\0" "xvmini_du\0" "xvmini_h\0" "xvmini_hu\0" "xvmini_w\0" "xvmini_wu\0" "xvmod_b\0" "xvmod_bu\0" "xvmod_d\0" "xvmod_du\0" "xvmod_h\0" "xvmod_hu\0" "xvmod_w\0" "xvmod_wu\0" "xvmskgez_b\0" "xvmskltz_b\0" "xvmskltz_d\0" "xvmskltz_h\0" "xvmskltz_w\0" "xvmsknz_b\0" "xvmsub_b\0" "xvmsub_d\0" "xvmsub_h\0" "xvmsub_w\0" "xvmuh_b\0" "xvmuh_bu\0" "xvmuh_d\0" "xvmuh_du\0" "xvmuh_h\0" "xvmuh_hu\0" "xvmuh_w\0" "xvmuh_wu\0" "xvmul_b\0" "xvmul_d\0" "xvmul_h\0" "xvmul_w\0" "xvmulwev_d_w\0" "xvmulwev_d_wu\0" "xvmulwev_d_wu_w\0" "xvmulwev_h_b\0" "xvmulwev_h_bu\0" "xvmulwev_h_bu_b\0" "xvmulwev_q_d\0" "xvmulwev_q_du\0" "xvmulwev_q_du_d\0" "xvmulwev_w_h\0" "xvmulwev_w_hu\0" "xvmulwev_w_hu_h\0" "xvmulwod_d_w\0" "xvmulwod_d_wu\0" "xvmulwod_d_wu_w\0" "xvmulwod_h_b\0" "xvmulwod_h_bu\0" "xvmulwod_h_bu_b\0" "xvmulwod_q_d\0" "xvmulwod_q_du\0" "xvmulwod_q_du_d\0" "xvmulwod_w_h\0" "xvmulwod_w_hu\0" "xvmulwod_w_hu_h\0" "xvneg_b\0" "xvneg_d\0" "xvneg_h\0" "xvneg_w\0" "xvnor_v\0" "xvnori_b\0" "xvor_v\0" "xvori_b\0" "xvorn_v\0" "xvpackev_b\0" "xvpackev_d\0" "xvpackev_h\0" "xvpackev_w\0" "xvpackod_b\0" "xvpackod_d\0" "xvpackod_h\0" "xvpackod_w\0" "xvpcnt_b\0" "xvpcnt_d\0" "xvpcnt_h\0" "xvpcnt_w\0" "xvperm_w\0" "xvpermi_d\0" "xvpermi_q\0" "xvpermi_w\0" "xvpickev_b\0" "xvpickev_d\0" "xvpickev_h\0" "xvpickev_w\0" "xvpickod_b\0" "xvpickod_d\0" "xvpickod_h\0" "xvpickod_w\0" "xvpickve2gr_d\0" "xvpickve2gr_du\0" "xvpickve2gr_w\0" "xvpickve2gr_wu\0" "xvpickve_d\0" "xvpickve_w\0" "xvrepl128vei_b\0" "xvrepl128vei_d\0" "xvrepl128vei_h\0" "xvrepl128vei_w\0" "xvreplgr2vr_b\0" "xvreplgr2vr_d\0" "xvreplgr2vr_h\0" "xvreplgr2vr_w\0" "xvreplve0_b\0" "xvreplve0_d\0" "xvreplve0_h\0" "xvreplve0_q\0" "xvreplve0_w\0" "xvreplve_b\0" "xvreplve_d\0" "xvreplve_h\0" "xvreplve_w\0" "xvrotr_b\0" "xvrotr_d\0" "xvrotr_h\0" "xvrotr_w\0" "xvrotri_b\0" "xvrotri_d\0" "xvrotri_h\0" "xvrotri_w\0" "xvsadd_b\0" "xvsadd_bu\0" "xvsadd_d\0" "xvsadd_du\0" "xvsadd_h\0" "xvsadd_hu\0" "xvsadd_w\0" "xvsadd_wu\0" "xvsat_b\0" "xvsat_bu\0" "xvsat_d\0" "xvsat_du\0" "xvsat_h\0" "xvsat_hu\0" "xvsat_w\0" "xvsat_wu\0" "xvseq_b\0" "xvseq_d\0" "xvseq_h\0" "xvseq_w\0" "xvseqi_b\0" "xvseqi_d\0" "xvseqi_h\0" "xvseqi_w\0" "xvsetallnez_b\0" "xvsetallnez_d\0" "xvsetallnez_h\0" "xvsetallnez_w\0" "xvsetanyeqz_b\0" "xvsetanyeqz_d\0" "xvsetanyeqz_h\0" "xvsetanyeqz_w\0" "xvseteqz_v\0" "xvsetnez_v\0" "xvshuf4i_b\0" "xvshuf4i_d\0" "xvshuf4i_h\0" "xvshuf4i_w\0" "xvshuf_b\0" "xvshuf_d\0" "xvshuf_h\0" "xvshuf_w\0" "xvsigncov_b\0" "xvsigncov_d\0" "xvsigncov_h\0" "xvsigncov_w\0" "xvsle_b\0" "xvsle_bu\0" "xvsle_d\0" "xvsle_du\0" "xvsle_h\0" "xvsle_hu\0" "xvsle_w\0" "xvsle_wu\0" "xvslei_b\0" "xvslei_bu\0" "xvslei_d\0" "xvslei_du\0" "xvslei_h\0" "xvslei_hu\0" "xvslei_w\0" "xvslei_wu\0" "xvsll_b\0" "xvsll_d\0" "xvsll_h\0" "xvsll_w\0" "xvslli_b\0" "xvslli_d\0" "xvslli_h\0" "xvslli_w\0" "xvsllwil_d_w\0" "xvsllwil_du_wu\0" "xvsllwil_h_b\0" "xvsllwil_hu_bu\0" "xvsllwil_w_h\0" "xvsllwil_wu_hu\0" "xvslt_b\0" "xvslt_bu\0" "xvslt_d\0" "xvslt_du\0" "xvslt_h\0" "xvslt_hu\0" "xvslt_w\0" "xvslt_wu\0" "xvslti_b\0" "xvslti_bu\0" "xvslti_d\0" "xvslti_du\0" "xvslti_h\0" "xvslti_hu\0" "xvslti_w\0" "xvslti_wu\0" "xvsra_b\0" "xvsra_d\0" "xvsra_h\0" "xvsra_w\0" "xvsrai_b\0" "xvsrai_d\0" "xvsrai_h\0" "xvsrai_w\0" "xvsran_b_h\0" "xvsran_h_w\0" "xvsran_w_d\0" "xvsrani_b_h\0" "xvsrani_d_q\0" "xvsrani_h_w\0" "xvsrani_w_d\0" "xvsrar_b\0" "xvsrar_d\0" "xvsrar_h\0" "xvsrar_w\0" "xvsrari_b\0" "xvsrari_d\0" "xvsrari_h\0" "xvsrari_w\0" "xvsrarn_b_h\0" "xvsrarn_h_w\0" "xvsrarn_w_d\0" "xvsrarni_b_h\0" "xvsrarni_d_q\0" "xvsrarni_h_w\0" "xvsrarni_w_d\0" "xvsrl_b\0" "xvsrl_d\0" "xvsrl_h\0" "xvsrl_w\0" "xvsrli_b\0" "xvsrli_d\0" "xvsrli_h\0" "xvsrli_w\0" "xvsrln_b_h\0" "xvsrln_h_w\0" "xvsrln_w_d\0" "xvsrlni_b_h\0" "xvsrlni_d_q\0" "xvsrlni_h_w\0" "xvsrlni_w_d\0" "xvsrlr_b\0" "xvsrlr_d\0" "xvsrlr_h\0" "xvsrlr_w\0" "xvsrlri_b\0" "xvsrlri_d\0" "xvsrlri_h\0" "xvsrlri_w\0" "xvsrlrn_b_h\0" "xvsrlrn_h_w\0" "xvsrlrn_w_d\0" "xvsrlrni_b_h\0" "xvsrlrni_d_q\0" "xvsrlrni_h_w\0" "xvsrlrni_w_d\0" "xvssran_b_h\0" "xvssran_bu_h\0" "xvssran_h_w\0" "xvssran_hu_w\0" "xvssran_w_d\0" "xvssran_wu_d\0" "xvssrani_b_h\0" "xvssrani_bu_h\0" "xvssrani_d_q\0" "xvssrani_du_q\0" "xvssrani_h_w\0" "xvssrani_hu_w\0" "xvssrani_w_d\0" "xvssrani_wu_d\0" "xvssrarn_b_h\0" "xvssrarn_bu_h\0" "xvssrarn_h_w\0" "xvssrarn_hu_w\0" "xvssrarn_w_d\0" "xvssrarn_wu_d\0" "xvssrarni_b_h\0" "xvssrarni_bu_h\0" "xvssrarni_d_q\0" "xvssrarni_du_q\0" "xvssrarni_h_w\0" "xvssrarni_hu_w\0" "xvssrarni_w_d\0" "xvssrarni_wu_d\0" "xvssrln_b_h\0" "xvssrln_bu_h\0" "xvssrln_h_w\0" "xvssrln_hu_w\0" "xvssrln_w_d\0" "xvssrln_wu_d\0" "xvssrlni_b_h\0" "xvssrlni_bu_h\0" "xvssrlni_d_q\0" "xvssrlni_du_q\0" "xvssrlni_h_w\0" "xvssrlni_hu_w\0" "xvssrlni_w_d\0" "xvssrlni_wu_d\0" "xvssrlrn_b_h\0" "xvssrlrn_bu_h\0" "xvssrlrn_h_w\0" "xvssrlrn_hu_w\0" "xvssrlrn_w_d\0" "xvssrlrn_wu_d\0" "xvssrlrni_b_h\0" "xvssrlrni_bu_h\0" "xvssrlrni_d_q\0" "xvssrlrni_du_q\0" "xvssrlrni_h_w\0" "xvssrlrni_hu_w\0" "xvssrlrni_w_d\0" "xvssrlrni_wu_d\0" "xvssub_b\0" "xvssub_bu\0" "xvssub_d\0" "xvssub_du\0" "xvssub_h\0" "xvssub_hu\0" "xvssub_w\0" "xvssub_wu\0" "xvst\0" "xvstelm_b\0" "xvstelm_d\0" "xvstelm_h\0" "xvstelm_w\0" "xvstx\0" "xvsub_b\0" "xvsub_d\0" "xvsub_h\0" "xvsub_q\0" "xvsub_w\0" "xvsubi_bu\0" "xvsubi_du\0" "xvsubi_hu\0" "xvsubi_wu\0" "xvsubwev_d_w\0" "xvsubwev_d_wu\0" "xvsubwev_h_b\0" "xvsubwev_h_bu\0" "xvsubwev_q_d\0" "xvsubwev_q_du\0" "xvsubwev_w_h\0" "xvsubwev_w_hu\0" "xvsubwod_d_w\0" "xvsubwod_d_wu\0" "xvsubwod_h_b\0" "xvsubwod_h_bu\0" "xvsubwod_q_d\0" "xvsubwod_q_du\0" "xvsubwod_w_h\0" "xvsubwod_w_hu\0" "xvxor_v\0" "xvxori_b\0"
  "vfcmp_caf_s\0" "vfcmp_cun_s\0" "vfcmp_ceq_s\0" "vfcmp_cueq_s\0" "vfcmp_clt_s\0" "vfcmp_cult_s\0" "vfcmp_cle_s\0" "vfcmp_cule_s\0" "vfcmp_cne_s\0" "vfcmp_cor_s\0" "vfcmp_cune_s\0" "vfcmp_saf_s\0" "vfcmp_sun_s\0" "vfcmp_seq_s\0" "vfcmp_sueq_s\0" "vfcmp_slt_s\0" "vfcmp_sult_s\0" "vfcmp_sle_s\0" "vfcmp_sule_s\0" "vfcmp_sne_s\0" "vfcmp_sor_s\0" "vfcmp_sune_s\0" "vfcmp_caf_d\0" "vfcmp_cun_d\0" "vfcmp_ceq_d\0" "vfcmp_cueq_d\0" "vfcmp_clt_d\0" "vfcmp_cult_d\0" "vfcmp_cle_d\0" "vfcmp_cule_d\0" "vfcmp_cne_d\0" "vfcmp_cor_d\0" "vfcmp_cune_d\0" "vfcmp_saf_d\0" "vfcmp_sun_d\0" "vfcmp_seq_d\0" "vfcmp_sueq_d\0" "vfcmp_slt_d\0" "vfcmp_sult_d\0" "vfcmp_sle_d\0" "vfcmp_sule_d\0" "vfcmp_sne_d\0" "vfcmp_sor_d\0" "vfcmp_sune_d\0" "xvfcmp_caf_s\0" "xvfcmp_cun_s\0" "xvfcmp_ceq_s\0" "xvfcmp_cueq_s\0" "xvfcmp_clt_s\0" "xvfcmp_cult_s\0" "xvfcmp_cle_s\0" "xvfcmp_cule_s\0" "xvfcmp_cne_s\0" "xvfcmp_cor_s\0" "xvfcmp_cune_s\0" "xvfcmp_saf_s\0" "xvfcmp_sun_s\0" "xvfcmp_seq_s\0" "xvfcmp_sueq_s\0" "xvfcmp_slt_s\0" "xvfcmp_sult_s\0" "xvfcmp_sle_s\0" "xvfcmp_sule_s\0" "xvfcmp_sne_s\0" "xvfcmp_sor_s\0" "xvfcmp_sune_s\0" "xvfcmp_caf_d\0" "xvfcmp_cun_d\0" "xvfcmp_ceq_d\0" "xvfcmp_cueq_d\0" "xvfcmp_clt_d\0" "xvfcmp_cult_d\0" "xvfcmp_cle_d\0" "xvfcmp_cule_d\0" "xvfcmp_cne_d\0" "xvfcmp_cor_d\0" "xvfcmp_cune_d\0" "xvfcmp_saf_d\0" "xvfcmp_sun_d\0" "xvfcmp_seq_d\0" "xvfcmp_sueq_d\0" "xvfcmp_slt_d\0" "xvfcmp_sult_d\0" "xvfcmp_sle_d\0" "xvfcmp_sule_d\0" "xvfcmp_sne_d\0" "xvfcmp_sor_d\0" "xvfcmp_sune_d\0" "sub_d\0" "sub_w\0";

const InstDB::InstNameIndex InstDB::instNameIndex[26] = {
  { Inst::kIdAdd_d        , Inst::kIdAsrtle_d      + 1 },
  { Inst::kIdB            , Inst::kIdBytepick_w    + 1 },
  { Inst::kIdCacop        , Inst::kIdCtz_w         + 1 },
  { Inst::kIdDbar         , Inst::kIdDiv_wu        + 1 },
  { Inst::kIdErtn         , Inst::kIdExt_w_h       + 1 },
  { Inst::kIdFabs_d       , Inst::kIdFtintrz_w_s   + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdIbar         , Inst::kIdIocsrwr_w     + 1 },
  { Inst::kIdJirl         , Inst::kIdJirl          + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdLd_b         , Inst::kIdLu52i_d       + 1 },
  { Inst::kIdMaskeqz      , Inst::kIdMulw_d_wu     + 1 },
  { Inst::kIdNor          , Inst::kIdNor           + 1 },
  { Inst::kIdOr_          , Inst::kIdOrn           + 1 },
  { Inst::kIdPcaddi       , Inst::kIdPreld         + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdRdtime_d     , Inst::kIdRotri_w       + 1 },
  { Inst::kIdSc_d         , Inst::kIdSub_w         + 1 },
  { Inst::kIdTlbclr       , Inst::kIdTlbwr         + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdVabsd_b      , Inst::kIdVfcmp_sune_d  + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdXor_         , Inst::kIdXvfcmp_sune_d + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 },
  { Inst::kIdNone         , Inst::kIdNone          + 1 }
};
// ----------------------------------------------------------------------------
// ${NameData:End}
#endif

ASMJIT_END_SUB_NAMESPACE

#endif // !ASMJIT_NO_LOONG
