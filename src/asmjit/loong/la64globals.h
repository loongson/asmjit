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

#ifndef ASMJIT_LOONG_LA64GLOBALS_H_INCLUDED
#define ASMJIT_LOONG_LA64GLOBALS_H_INCLUDED

#include "../loong/loongglobals.h"

ASMJIT_BEGIN_SUB_NAMESPACE(la64)

// a64 uses everything from arm namespace and adds into it.
using namespace loong;

//! \addtogroup asmjit_la64
//! \{

// ============================================================================
// [asmjit::la64::Inst]
// ============================================================================

//! AArch64 instruction.
//!
//! \note Only used to hold ARM-specific enumerations and static functions.
struct Inst : public BaseInst {
  //! Instruction id.
  enum Id : uint32_t {
    // ${InstId:Begin}
    kIdNone = 0,
    kIdAdd_d,
    kIdAdd_w,
    kIdAddi_d,
    kIdAddi_w,
    kIdAddu16i_d,
    kIdAlsl_d,
    kIdAlsl_w,
    kIdAlsl_wu,
    kIdAmadd_d,
    kIdAmadd_db_d,
    kIdAmadd_db_w,
    kIdAmadd_w,
    kIdAmand_d,
    kIdAmand_db_d,
    kIdAmand_db_w,
    kIdAmand_w,
    kIdAmmax_d,
    kIdAmmax_db_d,
    kIdAmmax_db_du,
    kIdAmmax_db_w,
    kIdAmmax_db_wu,
    kIdAmmax_du,
    kIdAmmax_w,
    kIdAmmax_wu,
    kIdAmmin_d,
    kIdAmmin_db_d,
    kIdAmmin_db_du,
    kIdAmmin_db_w,
    kIdAmmin_db_wu,
    kIdAmmin_du,
    kIdAmmin_w,
    kIdAmmin_wu,
    kIdAmor_d,
    kIdAmor_db_d,
    kIdAmor_db_w,
    kIdAmor_w,
    kIdAmswap_d,
    kIdAmswap_db_d,
    kIdAmswap_db_w,
    kIdAmswap_w,
    kIdAmxor_d,
    kIdAmxor_db_d,
    kIdAmxor_db_w,
    kIdAmxor_w,
    kIdAnd_,
    kIdAndi,
    kIdAndn,
    kIdAsrtgt_d,
    kIdAsrtle_d,
    kIdB,
    kIdBceqz,
    kIdBcnez,
    kIdBeq,
    kIdBge,
    kIdBgeu,
    kIdBitrev_4b,
    kIdBitrev_8b,
    kIdBitrev_d,
    kIdBitrev_w,
    kIdBl,
    kIdBlt,
    kIdBltu,
    kIdBne,
    kIdBreak_,
    kIdBstrins_d,
    kIdBstrins_w,
    kIdBstrpick_d,
    kIdBstrpick_w,
    kIdBytepick_d,
    kIdBytepick_w,
    kIdCacop,
    kIdClo_d,
    kIdClo_w,
    kIdClz_d,
    kIdClz_w,
    kIdCpucfg,
    kIdCrc_w_b_w,
    kIdCrc_w_d_w,
    kIdCrc_w_h_w,
    kIdCrc_w_w_w,
    kIdCrcc_w_b_w,
    kIdCrcc_w_d_w,
    kIdCrcc_w_h_w,
    kIdCrcc_w_w_w,
    kIdCsrrd,
    kIdCsrwr,
    kIdCsrxchg,
    kIdCto_d,
    kIdCto_w,
    kIdCtz_d,
    kIdCtz_w,
    kIdDbar,
    kIdDbcl,
    kIdDiv_d,
    kIdDiv_du,
    kIdDiv_w,
    kIdDiv_wu,
    kIdErtn,
    kIdExt_w_b,
    kIdExt_w_h,
    kIdFabs_d,
    kIdFabs_s,
    kIdFadd_d,
    kIdFadd_s,
    kIdFclass_d,
    kIdFclass_s,
    kIdFcmp_caf_s,
    kIdFcmp_ceq_s,
    kIdFcmp_cle_d,
    kIdFcmp_clt_d,
    kIdFcmp_cne_d,
    kIdFcmp_cor_s,
    kIdFcmp_cueq_s,
    kIdFcmp_cule_s,
    kIdFcmp_cult_s,
    kIdFcmp_cun_d,
    kIdFcmp_cune_d,
    kIdFcmp_saf_s,
    kIdFcmp_seq_s,
    kIdFcmp_sle_s,
    kIdFcmp_slt_s,
    kIdFcmp_sne_s,
    kIdFcmp_sor_d,
    kIdFcmp_sueq_d,
    kIdFcmp_sule_d,
    kIdFcmp_sult_d,
    kIdFcmp_sun_d,
    kIdFcmp_sune_s,
    kIdFcopysign_d,
    kIdFcopysign_s,
    kIdFcvt_d_s,
    kIdFcvt_s_d,
    kIdFdiv_d,
    kIdFdiv_s,
    kIdFfint_d_l,
    kIdFfint_d_w,
    kIdFfint_s_l,
    kIdFfint_s_w,
    kIdFld_d,
    kIdFld_s,
    kIdFldgt_d,
    kIdFldgt_s,
    kIdFldle_d,
    kIdFldle_s,
    kIdFldx_d,
    kIdFldx_s,
    kIdFlogb_d,
    kIdFlogb_s,
    kIdFmadd_d,
    kIdFmadd_s,
    kIdFmax_d,
    kIdFmax_s,
    kIdFmaxa_d,
    kIdFmaxa_s,
    kIdFmin_d,
    kIdFmin_s,
    kIdFmina_d,
    kIdFmina_s,
    kIdFmov_d,
    kIdFmov_s,
    kIdFmul_d,
    kIdFmul_s,
    kIdFneg_d,
    kIdFneg_s,
    kIdFrecip_d,
    kIdFrecip_s,
    kIdFrint_d,
    kIdFrint_s,
    kIdFrsqrt_d,
    kIdFrsqrt_s,
    kIdFscaleb_d,
    kIdFscaleb_s,
    kIdFsel,
    kIdFsqrt_d,
    kIdFsqrt_s,
    kIdFst_d,
    kIdFst_s,
    kIdFstgt_d,
    kIdFstgt_s,
    kIdFstle_d,
    kIdFstle_s,
    kIdFstx_d,
    kIdFstx_s,
    kIdFsub_d,
    kIdFsub_s,
    kIdFtint_l_d,
    kIdFtint_l_s,
    kIdFtint_w_d,
    kIdFtint_w_s,
    kIdFtintrm_l_d,
    kIdFtintrm_l_s,
    kIdFtintrm_w_d,
    kIdFtintrm_w_s,
    kIdFtintrne_l_d,
    kIdFtintrne_l_s,
    kIdFtintrne_w_d,
    kIdFtintrne_w_s,
    kIdFtintrp_l_d,
    kIdFtintrp_l_s,
    kIdFtintrp_w_d,
    kIdFtintrp_w_s,
    kIdFtintrz_l_d,
    kIdFtintrz_l_s,
    kIdFtintrz_w_d,
    kIdFtintrz_w_s,
    kIdIbar,
    kIdIdle,
    kIdInvtlb,
    kIdIocsrrd_b,
    kIdIocsrrd_d,
    kIdIocsrrd_h,
    kIdIocsrrd_w,
    kIdIocsrwr_b,
    kIdIocsrwr_d,
    kIdIocsrwr_h,
    kIdIocsrwr_w,
    kIdJirl,
    kIdLd_b,
    kIdLd_bu,
    kIdLd_d,
    kIdLd_h,
    kIdLd_hu,
    kIdLd_w,
    kIdLd_wu,
    kIdLddir,
    kIdLdgt_b,
    kIdLdgt_d,
    kIdLdgt_h,
    kIdLdgt_w,
    kIdLdle_b,
    kIdLdle_d,
    kIdLdle_h,
    kIdLdle_w,
    kIdLdpte,
    kIdLdptr_d,
    kIdLdptr_w,
    kIdLdx_b,
    kIdLdx_bu,
    kIdLdx_d,
    kIdLdx_h,
    kIdLdx_hu,
    kIdLdx_w,
    kIdLdx_wu,
    kIdLl_d,
    kIdLl_w,
    kIdLu12i_w,
    kIdLu32i_d,
    kIdLu52i_d,
    kIdMaskeqz,
    kIdMasknez,
    kIdMod_d,
    kIdMod_du,
    kIdMod_w,
    kIdMod_wu,
    kIdMovcf2fr,
    kIdMovcf2gr,
    kIdMovfcsr2gr,
    kIdMovfr2cf,
    kIdMovfr2gr_d,
    kIdMovfr2gr_s,
    kIdMovfrh2gr_s,
    kIdMovgr2cf,
    kIdMovgr2fcsr,
    kIdMovgr2fr_d,
    kIdMovgr2fr_w,
    kIdMovgr2frh_w,
    kIdMul_d,
    kIdMul_w,
    kIdMulh_d,
    kIdMulh_du,
    kIdMulh_w,
    kIdMulh_wu,
    kIdMulw_d_w,
    kIdMulw_d_wu,
    kIdNor,
    kIdOr_,
    kIdOri,
    kIdOrn,
    kIdPcaddi,
    kIdPcaddu12i,
    kIdPcaddu18i,
    kIdPcalau12i,
    kIdPreld,
    kIdRdtime_d,
    kIdRdtimeh_w,
    kIdRdtimel_w,
    kIdRevb_2h,
    kIdRevb_2w,
    kIdRevb_4h,
    kIdRevb_d,
    kIdRevh_2w,
    kIdRevh_d,
    kIdRotr_d,
    kIdRotr_w,
    kIdRotri_d,
    kIdRotri_w,
    kIdSc_d,
    kIdSc_w,
    kIdSll_d,
    kIdSll_w,
    kIdSlli_d,
    kIdSlli_w,
    kIdSlti,
    kIdSltui,
    kIdSra_d,
    kIdSra_w,
    kIdSrai_d,
    kIdSrai_w,
    kIdSrl_d,
    kIdSrl_w,
    kIdSrli_d,
    kIdSrli_w,
    kIdSt_b,
    kIdSt_d,
    kIdSt_h,
    kIdSt_w,
    kIdStgt_b,
    kIdStgt_d,
    kIdStgt_h,
    kIdStgt_w,
    kIdStle_b,
    kIdStle_d,
    kIdStle_h,
    kIdStle_w,
    kIdStptr_d,
    kIdStptr_w,
    kIdStx_b,
    kIdStx_d,
    kIdStx_h,
    kIdStx_w,
    kIdSub_d,
    kIdSub_w,
    kIdSyscall,
    kIdTlbclr,
    kIdTlbfill,
    kIdTlbflush,
    kIdTlbrd,
    kIdTlbsrch,
    kIdTlbwr,
    kIdXor_,
    kIdXori,
    kIdVabsd_b,
    kIdVabsd_bu,
    kIdVabsd_d,
    kIdVabsd_du,
    kIdVabsd_h,
    kIdVabsd_hu,
    kIdVabsd_w,
    kIdVabsd_wu,
    kIdVadd_b,
    kIdVadd_d,
    kIdVadd_h,
    kIdVadd_q,
    kIdVadd_w,
    kIdVadda_b,
    kIdVadda_d,
    kIdVadda_h,
    kIdVadda_w,
    kIdVaddi_bu,
    kIdVaddi_du,
    kIdVaddi_hu,
    kIdVaddi_wu,
    kIdVaddwev_d_w,
    kIdVaddwev_d_wu,
    kIdVaddwev_d_wu_w,
    kIdVaddwev_h_b,
    kIdVaddwev_h_bu,
    kIdVaddwev_h_bu_b,
    kIdVaddwev_q_d,
    kIdVaddwev_q_du,
    kIdVaddwev_q_du_d,
    kIdVaddwev_w_h,
    kIdVaddwev_w_hu,
    kIdVaddwev_w_hu_h,
    kIdVaddwod_d_w,
    kIdVaddwod_d_wu,
    kIdVaddwod_d_wu_w,
    kIdVaddwod_h_b,
    kIdVaddwod_h_bu,
    kIdVaddwod_h_bu_b,
    kIdVaddwod_q_d,
    kIdVaddwod_q_du,
    kIdVaddwod_q_du_d,
    kIdVaddwod_w_h,
    kIdVaddwod_w_hu,
    kIdVaddwod_w_hu_h,
    kIdVand_v,
    kIdVandi_b,
    kIdVandn_v,
    kIdVavg_b,
    kIdVavg_bu,
    kIdVavg_d,
    kIdVavg_du,
    kIdVavg_h,
    kIdVavg_hu,
    kIdVavg_w,
    kIdVavg_wu,
    kIdVavgr_b,
    kIdVavgr_bu,
    kIdVavgr_d,
    kIdVavgr_du,
    kIdVavgr_h,
    kIdVavgr_hu,
    kIdVavgr_w,
    kIdVavgr_wu,
    kIdVbitclr_b,
    kIdVbitclr_d,
    kIdVbitclr_h,
    kIdVbitclr_w,
    kIdVbitclri_b,
    kIdVbitclri_d,
    kIdVbitclri_h,
    kIdVbitclri_w,
    kIdVbitrev_b,
    kIdVbitrev_d,
    kIdVbitrev_h,
    kIdVbitrev_w,
    kIdVbitrevi_b,
    kIdVbitrevi_d,
    kIdVbitrevi_h,
    kIdVbitrevi_w,
    kIdVbitsel_v,
    kIdVbitseli_b,
    kIdVbitset_b,
    kIdVbitset_d,
    kIdVbitset_h,
    kIdVbitset_w,
    kIdVbitseti_b,
    kIdVbitseti_d,
    kIdVbitseti_h,
    kIdVbitseti_w,
    kIdVbsll_v,
    kIdVbsrl_v,
    kIdVclo_b,
    kIdVclo_d,
    kIdVclo_h,
    kIdVclo_w,
    kIdVclz_b,
    kIdVclz_d,
    kIdVclz_h,
    kIdVclz_w,
    kIdVdiv_b,
    kIdVdiv_bu,
    kIdVdiv_d,
    kIdVdiv_du,
    kIdVdiv_h,
    kIdVdiv_hu,
    kIdVdiv_w,
    kIdVdiv_wu,
    kIdVext2xv_d_b,
    kIdVext2xv_d_h,
    kIdVext2xv_d_w,
    kIdVext2xv_du_bu,
    kIdVext2xv_du_hu,
    kIdVext2xv_du_wu,
    kIdVext2xv_h_b,
    kIdVext2xv_hu_bu,
    kIdVext2xv_w_b,
    kIdVext2xv_w_h,
    kIdVext2xv_wu_bu,
    kIdVext2xv_wu_hu,
    kIdVexth_d_w,
    kIdVexth_du_wu,
    kIdVexth_h_b,
    kIdVexth_hu_bu,
    kIdVexth_q_d,
    kIdVexth_qu_du,
    kIdVexth_w_h,
    kIdVexth_wu_hu,
    kIdVextl_q_d,
    kIdVextl_qu_du,
    kIdVextrins_b,
    kIdVextrins_d,
    kIdVextrins_h,
    kIdVextrins_w,
    kIdVfadd_d,
    kIdVfadd_s,
    kIdVfclass_d,
    kIdVfclass_s,
    kIdVfcvt_h_s,
    kIdVfcvt_s_d,
    kIdVfcvth_d_s,
    kIdVfcvth_s_h,
    kIdVfcvtl_d_s,
    kIdVfcvtl_s_h,
    kIdVfdiv_d,
    kIdVfdiv_s,
    kIdVffint_d_l,
    kIdVffint_d_lu,
    kIdVffint_s_l,
    kIdVffint_s_w,
    kIdVffint_s_wu,
    kIdVffinth_d_w,
    kIdVffintl_d_w,
    kIdVflogb_d,
    kIdVflogb_s,
    kIdVfmadd_d,
    kIdVfmadd_s,
    kIdVfmax_d,
    kIdVfmax_s,
    kIdVfmaxa_d,
    kIdVfmaxa_s,
    kIdVfmin_d,
    kIdVfmin_s,
    kIdVfmina_d,
    kIdVfmina_s,
    kIdVfmsub_d,
    kIdVfmsub_s,
    kIdVfmul_d,
    kIdVfmul_s,
    kIdVfnmadd_d,
    kIdVfnmadd_s,
    kIdVfnmsub_d,
    kIdVfnmsub_s,
    kIdVfrecip_d,
    kIdVfrecip_s,
    kIdVfrint_d,
    kIdVfrint_s,
    kIdVfrintrm_d,
    kIdVfrintrm_s,
    kIdVfrintrne_d,
    kIdVfrintrne_s,
    kIdVfrintrp_d,
    kIdVfrintrp_s,
    kIdVfrintrz_d,
    kIdVfrintrz_s,
    kIdVfrsqrt_d,
    kIdVfrsqrt_s,
    kIdVfrstp_b,
    kIdVfrstp_h,
    kIdVfrstpi_b,
    kIdVfrstpi_h,
    kIdVfscaleb_d,
    kIdVfscaleb_s,
    kIdVfsqrt_d,
    kIdVfsqrt_s,
    kIdVfsub_d,
    kIdVfsub_s,
    kIdVftint_l_d,
    kIdVftint_lu_d,
    kIdVftint_w_d,
    kIdVftint_w_s,
    kIdVftint_wu_s,
    kIdVftinth_l_s,
    kIdVftintl_l_s,
    kIdVftintrm_l_d,
    kIdVftintrm_w_d,
    kIdVftintrm_w_s,
    kIdVftintrmh_l_s,
    kIdVftintrml_l_s,
    kIdVftintrne_l_d,
    kIdVftintrne_w_d,
    kIdVftintrne_w_s,
    kIdVftintrneh_l_s,
    kIdVftintrnel_l_s,
    kIdVftintrp_l_d,
    kIdVftintrp_w_d,
    kIdVftintrp_w_s,
    kIdVftintrph_l_s,
    kIdVftintrpl_l_s,
    kIdVftintrz_l_d,
    kIdVftintrz_lu_d,
    kIdVftintrz_w_d,
    kIdVftintrz_w_s,
    kIdVftintrz_wu_s,
    kIdVftintrzh_l_s,
    kIdVftintrzl_l_s,
    kIdVhaddw_d_w,
    kIdVhaddw_du_wu,
    kIdVhaddw_h_b,
    kIdVhaddw_hu_bu,
    kIdVhaddw_q_d,
    kIdVhaddw_qu_du,
    kIdVhaddw_w_h,
    kIdVhaddw_wu_hu,
    kIdVhsubw_d_w,
    kIdVhsubw_du_wu,
    kIdVhsubw_h_b,
    kIdVhsubw_hu_bu,
    kIdVhsubw_q_d,
    kIdVhsubw_qu_du,
    kIdVhsubw_w_h,
    kIdVhsubw_wu_hu,
    kIdVilvh_b,
    kIdVilvh_d,
    kIdVilvh_h,
    kIdVilvh_w,
    kIdVilvl_b,
    kIdVilvl_d,
    kIdVilvl_h,
    kIdVilvl_w,
    kIdVinsgr2vr_b,
    kIdVinsgr2vr_d,
    kIdVinsgr2vr_h,
    kIdVinsgr2vr_w,
    kIdVld,
    kIdVldi,
    kIdVldrepl_b,
    kIdVldrepl_d,
    kIdVldrepl_h,
    kIdVldrepl_w,
    kIdVldx,
    kIdVmadd_b,
    kIdVmadd_d,
    kIdVmadd_h,
    kIdVmadd_w,
    kIdVmaddwev_d_w,
    kIdVmaddwev_d_wu,
    kIdVmaddwev_d_wu_w,
    kIdVmaddwev_h_b,
    kIdVmaddwev_h_bu,
    kIdVmaddwev_h_bu_b,
    kIdVmaddwev_q_d,
    kIdVmaddwev_q_du,
    kIdVmaddwev_q_du_d,
    kIdVmaddwev_w_h,
    kIdVmaddwev_w_hu,
    kIdVmaddwev_w_hu_h,
    kIdVmaddwod_d_w,
    kIdVmaddwod_d_wu,
    kIdVmaddwod_d_wu_w,
    kIdVmaddwod_h_b,
    kIdVmaddwod_h_bu,
    kIdVmaddwod_h_bu_b,
    kIdVmaddwod_q_d,
    kIdVmaddwod_q_du,
    kIdVmaddwod_q_du_d,
    kIdVmaddwod_w_h,
    kIdVmaddwod_w_hu,
    kIdVmaddwod_w_hu_h,
    kIdVmax_b,
    kIdVmax_bu,
    kIdVmax_d,
    kIdVmax_du,
    kIdVmax_h,
    kIdVmax_hu,
    kIdVmax_w,
    kIdVmax_wu,
    kIdVmaxi_b,
    kIdVmaxi_bu,
    kIdVmaxi_d,
    kIdVmaxi_du,
    kIdVmaxi_h,
    kIdVmaxi_hu,
    kIdVmaxi_w,
    kIdVmaxi_wu,
    kIdVmepatmsk_v,
    kIdVmin_b,
    kIdVmin_bu,
    kIdVmin_d,
    kIdVmin_du,
    kIdVmin_h,
    kIdVmin_hu,
    kIdVmin_w,
    kIdVmin_wu,
    kIdVmini_b,
    kIdVmini_bu,
    kIdVmini_d,
    kIdVmini_du,
    kIdVmini_h,
    kIdVmini_hu,
    kIdVmini_w,
    kIdVmini_wu,
    kIdVmod_b,
    kIdVmod_bu,
    kIdVmod_d,
    kIdVmod_du,
    kIdVmod_h,
    kIdVmod_hu,
    kIdVmod_w,
    kIdVmod_wu,
    kIdVmskgez_b,
    kIdVmskltz_b,
    kIdVmskltz_d,
    kIdVmskltz_h,
    kIdVmskltz_w,
    kIdVmsknz_b,
    kIdVmsub_b,
    kIdVmsub_d,
    kIdVmsub_h,
    kIdVmsub_w,
    kIdVmuh_b,
    kIdVmuh_bu,
    kIdVmuh_d,
    kIdVmuh_du,
    kIdVmuh_h,
    kIdVmuh_hu,
    kIdVmuh_w,
    kIdVmuh_wu,
    kIdVmul_b,
    kIdVmul_d,
    kIdVmul_h,
    kIdVmul_w,
    kIdVmulwev_d_w,
    kIdVmulwev_d_wu,
    kIdVmulwev_d_wu_w,
    kIdVmulwev_h_b,
    kIdVmulwev_h_bu,
    kIdVmulwev_h_bu_b,
    kIdVmulwev_q_d,
    kIdVmulwev_q_du,
    kIdVmulwev_q_du_d,
    kIdVmulwev_w_h,
    kIdVmulwev_w_hu,
    kIdVmulwev_w_hu_h,
    kIdVmulwod_d_w,
    kIdVmulwod_d_wu,
    kIdVmulwod_d_wu_w,
    kIdVmulwod_h_b,
    kIdVmulwod_h_bu,
    kIdVmulwod_h_bu_b,
    kIdVmulwod_q_d,
    kIdVmulwod_q_du,
    kIdVmulwod_q_du_d,
    kIdVmulwod_w_h,
    kIdVmulwod_w_hu,
    kIdVmulwod_w_hu_h,
    kIdVneg_b,
    kIdVneg_d,
    kIdVneg_h,
    kIdVneg_w,
    kIdVnor_v,
    kIdVnori_b,
    kIdVor_v,
    kIdVori_b,
    kIdVorn_v,
    kIdVpackev_b,
    kIdVpackev_d,
    kIdVpackev_h,
    kIdVpackev_w,
    kIdVpackod_b,
    kIdVpackod_d,
    kIdVpackod_h,
    kIdVpackod_w,
    kIdVpcnt_b,
    kIdVpcnt_d,
    kIdVpcnt_h,
    kIdVpcnt_w,
    kIdVpermi_w,
    kIdVpickev_b,
    kIdVpickev_d,
    kIdVpickev_h,
    kIdVpickev_w,
    kIdVpickod_b,
    kIdVpickod_d,
    kIdVpickod_h,
    kIdVpickod_w,
    kIdVpickve2gr_b,
    kIdVpickve2gr_bu,
    kIdVpickve2gr_d,
    kIdVpickve2gr_du,
    kIdVpickve2gr_h,
    kIdVpickve2gr_hu,
    kIdVpickve2gr_w,
    kIdVpickve2gr_wu,
    kIdVreplgr2vr_b,
    kIdVreplgr2vr_d,
    kIdVreplgr2vr_h,
    kIdVreplgr2vr_w,
    kIdVreplve_b,
    kIdVreplve_d,
    kIdVreplve_h,
    kIdVreplve_w,
    kIdVreplvei_b,
    kIdVreplvei_d,
    kIdVreplvei_h,
    kIdVreplvei_w,
    kIdVrotr_b,
    kIdVrotr_d,
    kIdVrotr_h,
    kIdVrotr_w,
    kIdVrotri_b,
    kIdVrotri_d,
    kIdVrotri_h,
    kIdVrotri_w,
    kIdVsadd_b,
    kIdVsadd_bu,
    kIdVsadd_d,
    kIdVsadd_du,
    kIdVsadd_h,
    kIdVsadd_hu,
    kIdVsadd_w,
    kIdVsadd_wu,
    kIdVsat_b,
    kIdVsat_bu,
    kIdVsat_d,
    kIdVsat_du,
    kIdVsat_h,
    kIdVsat_hu,
    kIdVsat_w,
    kIdVsat_wu,
    kIdVseq_b,
    kIdVseq_d,
    kIdVseq_h,
    kIdVseq_w,
    kIdVseqi_b,
    kIdVseqi_d,
    kIdVseqi_h,
    kIdVseqi_w,
    kIdVsetallnez_b,
    kIdVsetallnez_d,
    kIdVsetallnez_h,
    kIdVsetallnez_w,
    kIdVsetanyeqz_b,
    kIdVsetanyeqz_d,
    kIdVsetanyeqz_h,
    kIdVsetanyeqz_w,
    kIdVseteqz_v,
    kIdVsetnez_v,
    kIdVshuf4i_b,
    kIdVshuf4i_d,
    kIdVshuf4i_h,
    kIdVshuf4i_w,
    kIdVshuf_b,
    kIdVshuf_d,
    kIdVshuf_h,
    kIdVshuf_w,
    kIdVsigncov_b,
    kIdVsigncov_d,
    kIdVsigncov_h,
    kIdVsigncov_w,
    kIdVsle_b,
    kIdVsle_bu,
    kIdVsle_d,
    kIdVsle_du,
    kIdVsle_h,
    kIdVsle_hu,
    kIdVsle_w,
    kIdVsle_wu,
    kIdVslei_b,
    kIdVslei_bu,
    kIdVslei_d,
    kIdVslei_du,
    kIdVslei_h,
    kIdVslei_hu,
    kIdVslei_w,
    kIdVslei_wu,
    kIdVsll_b,
    kIdVsll_d,
    kIdVsll_h,
    kIdVsll_w,
    kIdVslli_b,
    kIdVslli_d,
    kIdVslli_h,
    kIdVslli_w,
    kIdVsllwil_d_w,
    kIdVsllwil_du_wu,
    kIdVsllwil_h_b,
    kIdVsllwil_hu_bu,
    kIdVsllwil_w_h,
    kIdVsllwil_wu_hu,
    kIdVslt_b,
    kIdVslt_bu,
    kIdVslt_d,
    kIdVslt_du,
    kIdVslt_h,
    kIdVslt_hu,
    kIdVslt_w,
    kIdVslt_wu,
    kIdVslti_b,
    kIdVslti_bu,
    kIdVslti_d,
    kIdVslti_du,
    kIdVslti_h,
    kIdVslti_hu,
    kIdVslti_w,
    kIdVslti_wu,
    kIdVsra_b,
    kIdVsra_d,
    kIdVsra_h,
    kIdVsra_w,
    kIdVsrai_b,
    kIdVsrai_d,
    kIdVsrai_h,
    kIdVsrai_w,
    kIdVsran_b_h,
    kIdVsran_h_w,
    kIdVsran_w_d,
    kIdVsrani_b_h,
    kIdVsrani_d_q,
    kIdVsrani_h_w,
    kIdVsrani_w_d,
    kIdVsrar_b,
    kIdVsrar_d,
    kIdVsrar_h,
    kIdVsrar_w,
    kIdVsrari_b,
    kIdVsrari_d,
    kIdVsrari_h,
    kIdVsrari_w,
    kIdVsrarn_b_h,
    kIdVsrarn_h_w,
    kIdVsrarn_w_d,
    kIdVsrarni_b_h,
    kIdVsrarni_d_q,
    kIdVsrarni_h_w,
    kIdVsrarni_w_d,
    kIdVsrl_b,
    kIdVsrl_d,
    kIdVsrl_h,
    kIdVsrl_w,
    kIdVsrli_b,
    kIdVsrli_d,
    kIdVsrli_h,
    kIdVsrli_w,
    kIdVsrln_b_h,
    kIdVsrln_h_w,
    kIdVsrln_w_d,
    kIdVsrlni_b_h,
    kIdVsrlni_d_q,
    kIdVsrlni_h_w,
    kIdVsrlni_w_d,
    kIdVsrlr_b,
    kIdVsrlr_d,
    kIdVsrlr_h,
    kIdVsrlr_w,
    kIdVsrlri_b,
    kIdVsrlri_d,
    kIdVsrlri_h,
    kIdVsrlri_w,
    kIdVsrlrn_b_h,
    kIdVsrlrn_h_w,
    kIdVsrlrn_w_d,
    kIdVssran_b_h,
    kIdVssran_bu_h,
    kIdVssran_h_w,
    kIdVssran_hu_w,
    kIdVssran_w_d,
    kIdVssran_wu_d,
    kIdVssrani_b_h,
    kIdVssrani_bu_h,
    kIdVssrani_d_q,
    kIdVssrani_du_q,
    kIdVssrani_h_w,
    kIdVssrani_hu_w,
    kIdVssrani_w_d,
    kIdVssrani_wu_d,
    kIdVssrarn_b_h,
    kIdVssrarn_bu_h,
    kIdVssrarn_h_w,
    kIdVssrarn_hu_w,
    kIdVssrarn_w_d,
    kIdVssrarn_wu_d,
    kIdVssrarni_b_h,
    kIdVssrarni_bu_h,
    kIdVssrarni_d_q,
    kIdVssrarni_du_q,
    kIdVssrarni_h_w,
    kIdVssrarni_hu_w,
    kIdVssrarni_w_d,
    kIdVssrarni_wu_d,
    kIdVssrln_b_h,
    kIdVssrln_bu_h,
    kIdVssrln_h_w,
    kIdVssrln_hu_w,
    kIdVssrln_w_d,
    kIdVssrln_wu_d,
    kIdVssrlni_b_h,
    kIdVssrlni_bu_h,
    kIdVssrlni_d_q,
    kIdVssrlni_du_q,
    kIdVssrlni_h_w,
    kIdVssrlni_hu_w,
    kIdVssrlni_w_d,
    kIdVssrlni_wu_d,
    kIdVssrlrn_b_h,
    kIdVssrlrn_bu_h,
    kIdVssrlrn_h_w,
    kIdVssrlrn_hu_w,
    kIdVssrlrn_w_d,
    kIdVssrlrn_wu_d,
    kIdVssrlrni_b_h,
    kIdVssrlrni_bu_h,
    kIdVssrlrni_d_q,
    kIdVssrlrni_du_q,
    kIdVssrlrni_h_w,
    kIdVssrlrni_hu_w,
    kIdVssrlrni_w_d,
    kIdVssrlrni_wu_d,
    kIdVssub_b,
    kIdVssub_bu,
    kIdVssub_d,
    kIdVssub_du,
    kIdVssub_h,
    kIdVssub_hu,
    kIdVssub_w,
    kIdVssub_wu,
    kIdVst,
    kIdVstelm_b,
    kIdVstelm_d,
    kIdVstelm_h,
    kIdVstelm_w,
    kIdVstx,
    kIdVsub_b,
    kIdVsub_d,
    kIdVsub_h,
    kIdVsub_q,
    kIdVsub_w,
    kIdVsubi_bu,
    kIdVsubi_du,
    kIdVsubi_hu,
    kIdVsubi_wu,
    kIdVsubwev_d_w,
    kIdVsubwev_d_wu,
    kIdVsubwev_h_b,
    kIdVsubwev_h_bu,
    kIdVsubwev_q_d,
    kIdVsubwev_q_du,
    kIdVsubwev_w_h,
    kIdVsubwev_w_hu,
    kIdVsubwod_d_w,
    kIdVsubwod_d_wu,
    kIdVsubwod_h_b,
    kIdVsubwod_h_bu,
    kIdVsubwod_q_d,
    kIdVsubwod_q_du,
    kIdVsubwod_w_h,
    kIdVsubwod_w_hu,
    kIdVxor_v,
    kIdVxori_b,
    kIdXvabsd_b,
    kIdXvabsd_bu,
    kIdXvabsd_d,
    kIdXvabsd_du,
    kIdXvabsd_h,
    kIdXvabsd_hu,
    kIdXvabsd_w,
    kIdXvabsd_wu,
    kIdXvadd_b,
    kIdXvadd_d,
    kIdXvadd_h,
    kIdXvadd_q,
    kIdXvadd_w,
    kIdXvadda_b,
    kIdXvadda_d,
    kIdXvadda_h,
    kIdXvadda_w,
    kIdXvaddi_bu,
    kIdXvaddi_du,
    kIdXvaddi_hu,
    kIdXvaddi_wu,
    kIdXvaddwev_d_w,
    kIdXvaddwev_d_wu,
    kIdXvaddwev_d_wu_w,
    kIdXvaddwev_h_b,
    kIdXvaddwev_h_bu,
    kIdXvaddwev_h_bu_b,
    kIdXvaddwev_q_d,
    kIdXvaddwev_q_du,
    kIdXvaddwev_q_du_d,
    kIdXvaddwev_w_h,
    kIdXvaddwev_w_hu,
    kIdXvaddwev_w_hu_h,
    kIdXvaddwod_d_w,
    kIdXvaddwod_d_wu,
    kIdXvaddwod_d_wu_w,
    kIdXvaddwod_h_b,
    kIdXvaddwod_h_bu,
    kIdXvaddwod_h_bu_b,
    kIdXvaddwod_q_d,
    kIdXvaddwod_q_du,
    kIdXvaddwod_q_du_d,
    kIdXvaddwod_w_h,
    kIdXvaddwod_w_hu,
    kIdXvaddwod_w_hu_h,
    kIdXvand_v,
    kIdXvandi_b,
    kIdXvandn_v,
    kIdXvavg_b,
    kIdXvavg_bu,
    kIdXvavg_d,
    kIdXvavg_du,
    kIdXvavg_h,
    kIdXvavg_hu,
    kIdXvavg_w,
    kIdXvavg_wu,
    kIdXvavgr_b,
    kIdXvavgr_bu,
    kIdXvavgr_d,
    kIdXvavgr_du,
    kIdXvavgr_h,
    kIdXvavgr_hu,
    kIdXvavgr_w,
    kIdXvavgr_wu,
    kIdXvbitclr_b,
    kIdXvbitclr_d,
    kIdXvbitclr_h,
    kIdXvbitclr_w,
    kIdXvbitclri_b,
    kIdXvbitclri_d,
    kIdXvbitclri_h,
    kIdXvbitclri_w,
    kIdXvbitrev_b,
    kIdXvbitrev_d,
    kIdXvbitrev_h,
    kIdXvbitrev_w,
    kIdXvbitrevi_b,
    kIdXvbitrevi_d,
    kIdXvbitrevi_h,
    kIdXvbitrevi_w,
    kIdXvbitsel_v,
    kIdXvbitseli_b,
    kIdXvbitset_b,
    kIdXvbitset_d,
    kIdXvbitset_h,
    kIdXvbitset_w,
    kIdXvbitseti_b,
    kIdXvbitseti_d,
    kIdXvbitseti_h,
    kIdXvbitseti_w,
    kIdXvbsll_v,
    kIdXvbsrl_v,
    kIdXvclo_b,
    kIdXvclo_d,
    kIdXvclo_h,
    kIdXvclo_w,
    kIdXvclz_b,
    kIdXvclz_d,
    kIdXvclz_h,
    kIdXvclz_w,
    kIdXvdiv_b,
    kIdXvdiv_bu,
    kIdXvdiv_d,
    kIdXvdiv_du,
    kIdXvdiv_h,
    kIdXvdiv_hu,
    kIdXvdiv_w,
    kIdXvdiv_wu,
    kIdXvexth_d_w,
    kIdXvexth_du_wu,
    kIdXvexth_h_b,
    kIdXvexth_hu_bu,
    kIdXvexth_q_d,
    kIdXvexth_qu_du,
    kIdXvexth_w_h,
    kIdXvexth_wu_hu,
    kIdXvextl_q_d,
    kIdXvextl_qu_du,
    kIdXvextrins_b,
    kIdXvextrins_d,
    kIdXvextrins_h,
    kIdXvextrins_w,
    kIdXvfadd_d,
    kIdXvfadd_s,
    kIdXvfclass_d,
    kIdXvfclass_s,
    kIdXvfcvt_h_s,
    kIdXvfcvt_s_d,
    kIdXvfcvth_d_s,
    kIdXvfcvth_s_h,
    kIdXvfcvtl_d_s,
    kIdXvfcvtl_s_h,
    kIdXvfdiv_d,
    kIdXvfdiv_s,
    kIdXvffint_d_l,
    kIdXvffint_d_lu,
    kIdXvffint_s_l,
    kIdXvffint_s_w,
    kIdXvffint_s_wu,
    kIdXvffinth_d_w,
    kIdXvffintl_d_w,
    kIdXvflogb_d,
    kIdXvflogb_s,
    kIdXvfmadd_d,
    kIdXvfmadd_s,
    kIdXvfmax_d,
    kIdXvfmax_s,
    kIdXvfmaxa_d,
    kIdXvfmaxa_s,
    kIdXvfmin_d,
    kIdXvfmin_s,
    kIdXvfmina_d,
    kIdXvfmina_s,
    kIdXvfmsub_d,
    kIdXvfmsub_s,
    kIdXvfmul_d,
    kIdXvfmul_s,
    kIdXvfnmadd_d,
    kIdXvfnmadd_s,
    kIdXvfnmsub_d,
    kIdXvfnmsub_s,
    kIdXvfrecip_d,
    kIdXvfrecip_s,
    kIdXvfrint_d,
    kIdXvfrint_s,
    kIdXvfrintrm_d,
    kIdXvfrintrm_s,
    kIdXvfrintrne_d,
    kIdXvfrintrne_s,
    kIdXvfrintrp_d,
    kIdXvfrintrp_s,
    kIdXvfrintrz_d,
    kIdXvfrintrz_s,
    kIdXvfrsqrt_d,
    kIdXvfrsqrt_s,
    kIdXvfrstp_b,
    kIdXvfrstp_h,
    kIdXvfrstpi_b,
    kIdXvfrstpi_h,
    kIdXvfscaleb_d,
    kIdXvfscaleb_s,
    kIdXvfsqrt_d,
    kIdXvfsqrt_s,
    kIdXvfsub_d,
    kIdXvfsub_s,
    kIdXvftint_l_d,
    kIdXvftint_lu_d,
    kIdXvftint_w_d,
    kIdXvftint_w_s,
    kIdXvftint_wu_s,
    kIdXvftinth_l_s,
    kIdXvftintl_l_s,
    kIdXvftintrm_l_d,
    kIdXvftintrm_w_d,
    kIdXvftintrm_w_s,
    kIdXvftintrmh_l_s,
    kIdXvftintrml_l_s,
    kIdXvftintrne_l_d,
    kIdXvftintrne_w_d,
    kIdXvftintrne_w_s,
    kIdXvftintrneh_l_s,
    kIdXvftintrnel_l_s,
    kIdXvftintrp_l_d,
    kIdXvftintrp_w_d,
    kIdXvftintrp_w_s,
    kIdXvftintrph_l_s,
    kIdXvftintrpl_l_s,
    kIdXvftintrz_l_d,
    kIdXvftintrz_lu_d,
    kIdXvftintrz_w_d,
    kIdXvftintrz_w_s,
    kIdXvftintrz_wu_s,
    kIdXvftintrzh_l_s,
    kIdXvftintrzl_l_s,
    kIdXvhaddw_d_w,
    kIdXvhaddw_du_wu,
    kIdXvhaddw_h_b,
    kIdXvhaddw_hu_bu,
    kIdXvhaddw_q_d,
    kIdXvhaddw_qu_du,
    kIdXvhaddw_w_h,
    kIdXvhaddw_wu_hu,
    kIdXvhseli_d,
    kIdXvhsubw_d_w,
    kIdXvhsubw_du_wu,
    kIdXvhsubw_h_b,
    kIdXvhsubw_hu_bu,
    kIdXvhsubw_q_d,
    kIdXvhsubw_qu_du,
    kIdXvhsubw_w_h,
    kIdXvhsubw_wu_hu,
    kIdXvilvh_b,
    kIdXvilvh_d,
    kIdXvilvh_h,
    kIdXvilvh_w,
    kIdXvilvl_b,
    kIdXvilvl_d,
    kIdXvilvl_h,
    kIdXvilvl_w,
    kIdXvinsgr2vr_d,
    kIdXvinsgr2vr_w,
    kIdXvinsve0_d,
    kIdXvinsve0_w,
    kIdXvld,
    kIdXvldi,
    kIdXvldrepl_b,
    kIdXvldrepl_d,
    kIdXvldrepl_h,
    kIdXvldrepl_w,
    kIdXvldx,
    kIdXvmadd_b,
    kIdXvmadd_d,
    kIdXvmadd_h,
    kIdXvmadd_w,
    kIdXvmaddwev_d_w,
    kIdXvmaddwev_d_wu,
    kIdXvmaddwev_d_wu_w,
    kIdXvmaddwev_h_b,
    kIdXvmaddwev_h_bu,
    kIdXvmaddwev_h_bu_b,
    kIdXvmaddwev_q_d,
    kIdXvmaddwev_q_du,
    kIdXvmaddwev_q_du_d,
    kIdXvmaddwev_w_h,
    kIdXvmaddwev_w_hu,
    kIdXvmaddwev_w_hu_h,
    kIdXvmaddwod_d_w,
    kIdXvmaddwod_d_wu,
    kIdXvmaddwod_d_wu_w,
    kIdXvmaddwod_h_b,
    kIdXvmaddwod_h_bu,
    kIdXvmaddwod_h_bu_b,
    kIdXvmaddwod_q_d,
    kIdXvmaddwod_q_du,
    kIdXvmaddwod_q_du_d,
    kIdXvmaddwod_w_h,
    kIdXvmaddwod_w_hu,
    kIdXvmaddwod_w_hu_h,
    kIdXvmax_b,
    kIdXvmax_bu,
    kIdXvmax_d,
    kIdXvmax_du,
    kIdXvmax_h,
    kIdXvmax_hu,
    kIdXvmax_w,
    kIdXvmax_wu,
    kIdXvmaxi_b,
    kIdXvmaxi_bu,
    kIdXvmaxi_d,
    kIdXvmaxi_du,
    kIdXvmaxi_h,
    kIdXvmaxi_hu,
    kIdXvmaxi_w,
    kIdXvmaxi_wu,
    kIdXvmepatmsk_v,
    kIdXvmin_b,
    kIdXvmin_bu,
    kIdXvmin_d,
    kIdXvmin_du,
    kIdXvmin_h,
    kIdXvmin_hu,
    kIdXvmin_w,
    kIdXvmin_wu,
    kIdXvmini_b,
    kIdXvmini_bu,
    kIdXvmini_d,
    kIdXvmini_du,
    kIdXvmini_h,
    kIdXvmini_hu,
    kIdXvmini_w,
    kIdXvmini_wu,
    kIdXvmod_b,
    kIdXvmod_bu,
    kIdXvmod_d,
    kIdXvmod_du,
    kIdXvmod_h,
    kIdXvmod_hu,
    kIdXvmod_w,
    kIdXvmod_wu,
    kIdXvmskgez_b,
    kIdXvmskltz_b,
    kIdXvmskltz_d,
    kIdXvmskltz_h,
    kIdXvmskltz_w,
    kIdXvmsknz_b,
    kIdXvmsub_b,
    kIdXvmsub_d,
    kIdXvmsub_h,
    kIdXvmsub_w,
    kIdXvmuh_b,
    kIdXvmuh_bu,
    kIdXvmuh_d,
    kIdXvmuh_du,
    kIdXvmuh_h,
    kIdXvmuh_hu,
    kIdXvmuh_w,
    kIdXvmuh_wu,
    kIdXvmul_b,
    kIdXvmul_d,
    kIdXvmul_h,
    kIdXvmul_w,
    kIdXvmulwev_d_w,
    kIdXvmulwev_d_wu,
    kIdXvmulwev_d_wu_w,
    kIdXvmulwev_h_b,
    kIdXvmulwev_h_bu,
    kIdXvmulwev_h_bu_b,
    kIdXvmulwev_q_d,
    kIdXvmulwev_q_du,
    kIdXvmulwev_q_du_d,
    kIdXvmulwev_w_h,
    kIdXvmulwev_w_hu,
    kIdXvmulwev_w_hu_h,
    kIdXvmulwod_d_w,
    kIdXvmulwod_d_wu,
    kIdXvmulwod_d_wu_w,
    kIdXvmulwod_h_b,
    kIdXvmulwod_h_bu,
    kIdXvmulwod_h_bu_b,
    kIdXvmulwod_q_d,
    kIdXvmulwod_q_du,
    kIdXvmulwod_q_du_d,
    kIdXvmulwod_w_h,
    kIdXvmulwod_w_hu,
    kIdXvmulwod_w_hu_h,
    kIdXvneg_b,
    kIdXvneg_d,
    kIdXvneg_h,
    kIdXvneg_w,
    kIdXvnor_v,
    kIdXvnori_b,
    kIdXvor_v,
    kIdXvori_b,
    kIdXvorn_v,
    kIdXvpackev_b,
    kIdXvpackev_d,
    kIdXvpackev_h,
    kIdXvpackev_w,
    kIdXvpackod_b,
    kIdXvpackod_d,
    kIdXvpackod_h,
    kIdXvpackod_w,
    kIdXvpcnt_b,
    kIdXvpcnt_d,
    kIdXvpcnt_h,
    kIdXvpcnt_w,
    kIdXvperm_w,
    kIdXvpermi_d,
    kIdXvpermi_q,
    kIdXvpermi_w,
    kIdXvpickev_b,
    kIdXvpickev_d,
    kIdXvpickev_h,
    kIdXvpickev_w,
    kIdXvpickod_b,
    kIdXvpickod_d,
    kIdXvpickod_h,
    kIdXvpickod_w,
    kIdXvpickve2gr_d,
    kIdXvpickve2gr_du,
    kIdXvpickve2gr_w,
    kIdXvpickve2gr_wu,
    kIdXvpickve_d,
    kIdXvpickve_w,
    kIdXvrepl128vei_b,
    kIdXvrepl128vei_d,
    kIdXvrepl128vei_h,
    kIdXvrepl128vei_w,
    kIdXvreplgr2vr_b,
    kIdXvreplgr2vr_d,
    kIdXvreplgr2vr_h,
    kIdXvreplgr2vr_w,
    kIdXvreplve0_b,
    kIdXvreplve0_d,
    kIdXvreplve0_h,
    kIdXvreplve0_q,
    kIdXvreplve0_w,
    kIdXvreplve_b,
    kIdXvreplve_d,
    kIdXvreplve_h,
    kIdXvreplve_w,
    kIdXvrotr_b,
    kIdXvrotr_d,
    kIdXvrotr_h,
    kIdXvrotr_w,
    kIdXvrotri_b,
    kIdXvrotri_d,
    kIdXvrotri_h,
    kIdXvrotri_w,
    kIdXvsadd_b,
    kIdXvsadd_bu,
    kIdXvsadd_d,
    kIdXvsadd_du,
    kIdXvsadd_h,
    kIdXvsadd_hu,
    kIdXvsadd_w,
    kIdXvsadd_wu,
    kIdXvsat_b,
    kIdXvsat_bu,
    kIdXvsat_d,
    kIdXvsat_du,
    kIdXvsat_h,
    kIdXvsat_hu,
    kIdXvsat_w,
    kIdXvsat_wu,
    kIdXvseq_b,
    kIdXvseq_d,
    kIdXvseq_h,
    kIdXvseq_w,
    kIdXvseqi_b,
    kIdXvseqi_d,
    kIdXvseqi_h,
    kIdXvseqi_w,
    kIdXvsetallnez_b,
    kIdXvsetallnez_d,
    kIdXvsetallnez_h,
    kIdXvsetallnez_w,
    kIdXvsetanyeqz_b,
    kIdXvsetanyeqz_d,
    kIdXvsetanyeqz_h,
    kIdXvsetanyeqz_w,
    kIdXvseteqz_v,
    kIdXvsetnez_v,
    kIdXvshuf4i_b,
    kIdXvshuf4i_d,
    kIdXvshuf4i_h,
    kIdXvshuf4i_w,
    kIdXvshuf_b,
    kIdXvshuf_d,
    kIdXvshuf_h,
    kIdXvshuf_w,
    kIdXvsigncov_b,
    kIdXvsigncov_d,
    kIdXvsigncov_h,
    kIdXvsigncov_w,
    kIdXvsle_b,
    kIdXvsle_bu,
    kIdXvsle_d,
    kIdXvsle_du,
    kIdXvsle_h,
    kIdXvsle_hu,
    kIdXvsle_w,
    kIdXvsle_wu,
    kIdXvslei_b,
    kIdXvslei_bu,
    kIdXvslei_d,
    kIdXvslei_du,
    kIdXvslei_h,
    kIdXvslei_hu,
    kIdXvslei_w,
    kIdXvslei_wu,
    kIdXvsll_b,
    kIdXvsll_d,
    kIdXvsll_h,
    kIdXvsll_w,
    kIdXvslli_b,
    kIdXvslli_d,
    kIdXvslli_h,
    kIdXvslli_w,
    kIdXvsllwil_d_w,
    kIdXvsllwil_du_wu,
    kIdXvsllwil_h_b,
    kIdXvsllwil_hu_bu,
    kIdXvsllwil_w_h,
    kIdXvsllwil_wu_hu,
    kIdXvslt_b,
    kIdXvslt_bu,
    kIdXvslt_d,
    kIdXvslt_du,
    kIdXvslt_h,
    kIdXvslt_hu,
    kIdXvslt_w,
    kIdXvslt_wu,
    kIdXvslti_b,
    kIdXvslti_bu,
    kIdXvslti_d,
    kIdXvslti_du,
    kIdXvslti_h,
    kIdXvslti_hu,
    kIdXvslti_w,
    kIdXvslti_wu,
    kIdXvsra_b,
    kIdXvsra_d,
    kIdXvsra_h,
    kIdXvsra_w,
    kIdXvsrai_b,
    kIdXvsrai_d,
    kIdXvsrai_h,
    kIdXvsrai_w,
    kIdXvsran_b_h,
    kIdXvsran_h_w,
    kIdXvsran_w_d,
    kIdXvsrani_b_h,
    kIdXvsrani_d_q,
    kIdXvsrani_h_w,
    kIdXvsrani_w_d,
    kIdXvsrar_b,
    kIdXvsrar_d,
    kIdXvsrar_h,
    kIdXvsrar_w,
    kIdXvsrari_b,
    kIdXvsrari_d,
    kIdXvsrari_h,
    kIdXvsrari_w,
    kIdXvsrarn_b_h,
    kIdXvsrarn_h_w,
    kIdXvsrarn_w_d,
    kIdXvsrarni_b_h,
    kIdXvsrarni_d_q,
    kIdXvsrarni_h_w,
    kIdXvsrarni_w_d,
    kIdXvsrl_b,
    kIdXvsrl_d,
    kIdXvsrl_h,
    kIdXvsrl_w,
    kIdXvsrli_b,
    kIdXvsrli_d,
    kIdXvsrli_h,
    kIdXvsrli_w,
    kIdXvsrln_b_h,
    kIdXvsrln_h_w,
    kIdXvsrln_w_d,
    kIdXvsrlni_b_h,
    kIdXvsrlni_d_q,
    kIdXvsrlni_h_w,
    kIdXvsrlni_w_d,
    kIdXvsrlr_b,
    kIdXvsrlr_d,
    kIdXvsrlr_h,
    kIdXvsrlr_w,
    kIdXvsrlri_b,
    kIdXvsrlri_d,
    kIdXvsrlri_h,
    kIdXvsrlri_w,
    kIdXvsrlrn_b_h,
    kIdXvsrlrn_h_w,
    kIdXvsrlrn_w_d,
    kIdXvsrlrni_b_h,
    kIdXvsrlrni_d_q,
    kIdXvsrlrni_h_w,
    kIdXvsrlrni_w_d,
    kIdXvssran_b_h,
    kIdXvssran_bu_h,
    kIdXvssran_h_w,
    kIdXvssran_hu_w,
    kIdXvssran_w_d,
    kIdXvssran_wu_d,
    kIdXvssrani_b_h,
    kIdXvssrani_bu_h,
    kIdXvssrani_d_q,
    kIdXvssrani_du_q,
    kIdXvssrani_h_w,
    kIdXvssrani_hu_w,
    kIdXvssrani_w_d,
    kIdXvssrani_wu_d,
    kIdXvssrarn_b_h,
    kIdXvssrarn_bu_h,
    kIdXvssrarn_h_w,
    kIdXvssrarn_hu_w,
    kIdXvssrarn_w_d,
    kIdXvssrarn_wu_d,
    kIdXvssrarni_b_h,
    kIdXvssrarni_bu_h,
    kIdXvssrarni_d_q,
    kIdXvssrarni_du_q,
    kIdXvssrarni_h_w,
    kIdXvssrarni_hu_w,
    kIdXvssrarni_w_d,
    kIdXvssrarni_wu_d,
    kIdXvssrln_b_h,
    kIdXvssrln_bu_h,
    kIdXvssrln_h_w,
    kIdXvssrln_hu_w,
    kIdXvssrln_w_d,
    kIdXvssrln_wu_d,
    kIdXvssrlni_b_h,
    kIdXvssrlni_bu_h,
    kIdXvssrlni_d_q,
    kIdXvssrlni_du_q,
    kIdXvssrlni_h_w,
    kIdXvssrlni_hu_w,
    kIdXvssrlni_w_d,
    kIdXvssrlni_wu_d,
    kIdXvssrlrn_b_h,
    kIdXvssrlrn_bu_h,
    kIdXvssrlrn_h_w,
    kIdXvssrlrn_hu_w,
    kIdXvssrlrn_w_d,
    kIdXvssrlrn_wu_d,
    kIdXvssrlrni_b_h,
    kIdXvssrlrni_bu_h,
    kIdXvssrlrni_d_q,
    kIdXvssrlrni_du_q,
    kIdXvssrlrni_h_w,
    kIdXvssrlrni_hu_w,
    kIdXvssrlrni_w_d,
    kIdXvssrlrni_wu_d,
    kIdXvssub_b,
    kIdXvssub_bu,
    kIdXvssub_d,
    kIdXvssub_du,
    kIdXvssub_h,
    kIdXvssub_hu,
    kIdXvssub_w,
    kIdXvssub_wu,
    kIdXvst,
    kIdXvstelm_b,
    kIdXvstelm_d,
    kIdXvstelm_h,
    kIdXvstelm_w,
    kIdXvstx,
    kIdXvsub_b,
    kIdXvsub_d,
    kIdXvsub_h,
    kIdXvsub_q,
    kIdXvsub_w,
    kIdXvsubi_bu,
    kIdXvsubi_du,
    kIdXvsubi_hu,
    kIdXvsubi_wu,
    kIdXvsubwev_d_w,
    kIdXvsubwev_d_wu,
    kIdXvsubwev_h_b,
    kIdXvsubwev_h_bu,
    kIdXvsubwev_q_d,
    kIdXvsubwev_q_du,
    kIdXvsubwev_w_h,
    kIdXvsubwev_w_hu,
    kIdXvsubwod_d_w,
    kIdXvsubwod_d_wu,
    kIdXvsubwod_h_b,
    kIdXvsubwod_h_bu,
    kIdXvsubwod_q_d,
    kIdXvsubwod_q_du,
    kIdXvsubwod_w_h,
    kIdXvsubwod_w_hu,
    kIdXvxor_v,
    kIdXvxori_b,
    kIdVfcmp_caf_s,
    kIdVfcmp_cun_s,
    kIdVfcmp_ceq_s,
    kIdVfcmp_cueq_s,
    kIdVfcmp_clt_s,
    kIdVfcmp_cult_s,
    kIdVfcmp_cle_s,
    kIdVfcmp_cule_s,
    kIdVfcmp_cne_s,
    kIdVfcmp_cor_s,
    kIdVfcmp_cune_s,
    kIdVfcmp_saf_s,
    kIdVfcmp_sun_s,
    kIdVfcmp_seq_s,
    kIdVfcmp_sueq_s,
    kIdVfcmp_slt_s,
    kIdVfcmp_sult_s,
    kIdVfcmp_sle_s,
    kIdVfcmp_sule_s,
    kIdVfcmp_sne_s,
    kIdVfcmp_sor_s,
    kIdVfcmp_sune_s,
    kIdVfcmp_caf_d,
    kIdVfcmp_cun_d,
    kIdVfcmp_ceq_d,
    kIdVfcmp_cueq_d,
    kIdVfcmp_clt_d,
    kIdVfcmp_cult_d,
    kIdVfcmp_cle_d,
    kIdVfcmp_cule_d,
    kIdVfcmp_cne_d,
    kIdVfcmp_cor_d,
    kIdVfcmp_cune_d,
    kIdVfcmp_saf_d,
    kIdVfcmp_sun_d,
    kIdVfcmp_seq_d,
    kIdVfcmp_sueq_d,
    kIdVfcmp_slt_d,
    kIdVfcmp_sult_d,
    kIdVfcmp_sle_d,
    kIdVfcmp_sule_d,
    kIdVfcmp_sne_d,
    kIdVfcmp_sor_d,
    kIdVfcmp_sune_d,
    kIdXvfcmp_caf_s,
    kIdXvfcmp_cun_s,
    kIdXvfcmp_ceq_s,
    kIdXvfcmp_cueq_s,
    kIdXvfcmp_clt_s,
    kIdXvfcmp_cult_s,
    kIdXvfcmp_cle_s,
    kIdXvfcmp_cule_s,
    kIdXvfcmp_cne_s,
    kIdXvfcmp_cor_s,
    kIdXvfcmp_cune_s,
    kIdXvfcmp_saf_s,
    kIdXvfcmp_sun_s,
    kIdXvfcmp_seq_s,
    kIdXvfcmp_sueq_s,
    kIdXvfcmp_slt_s,
    kIdXvfcmp_sult_s,
    kIdXvfcmp_sle_s,
    kIdXvfcmp_sule_s,
    kIdXvfcmp_sne_s,
    kIdXvfcmp_sor_s,
    kIdXvfcmp_sune_s,
    kIdXvfcmp_caf_d,
    kIdXvfcmp_cun_d,
    kIdXvfcmp_ceq_d,
    kIdXvfcmp_cueq_d,
    kIdXvfcmp_clt_d,
    kIdXvfcmp_cult_d,
    kIdXvfcmp_cle_d,
    kIdXvfcmp_cule_d,
    kIdXvfcmp_cne_d,
    kIdXvfcmp_cor_d,
    kIdXvfcmp_cune_d,
    kIdXvfcmp_saf_d,
    kIdXvfcmp_sun_d,
    kIdXvfcmp_seq_d,
    kIdXvfcmp_sueq_d,
    kIdXvfcmp_slt_d,
    kIdXvfcmp_sult_d,
    kIdXvfcmp_sle_d,
    kIdXvfcmp_sule_d,
    kIdXvfcmp_sne_d,
    kIdXvfcmp_sor_d,
    kIdXvfcmp_sune_d,
    kIdAdc,
    kIdAdcs,
    kIdAdd,
    kIdAddg,
    kIdAdds,
    kIdAdr,
    kIdAdrp,
    kIdAnd,
    kIdAnds,
    kIdAsr,
    kIdAsrv,
    kIdAt,
    kIdAutda,
    kIdAutdza,
    kIdAutdb,
    kIdAutdzb,
    kIdAutia,
    kIdAutia1716,
    kIdAutiasp,
    kIdAutiaz,
    kIdAutib,
    kIdAutib1716,
    kIdAutibsp,
    kIdAutibz,
    kIdAutiza,
    kIdAutizb,
    kIdAxflag,
    //kIdB,
    kIdBfc,
    kIdBfi,
    kIdBfm,
    kIdBfxil,
    kIdBic,
    kIdBics,
    //kIdBl,
    kIdBlr,
    kIdBr,
    kIdBrk,
    kIdCas,
    kIdCasa,
    kIdCasab,
    kIdCasah,
    kIdCasal,
    kIdCasalb,
    kIdCasalh,
    kIdCasb,
    kIdCash,
    kIdCasl,
    kIdCaslb,
    kIdCaslh,
    kIdCasp,
    kIdCaspa,
    kIdCaspal,
    kIdCaspl,
    kIdCbnz,
    kIdCbz,
    kIdCcmn,
    kIdCcmp,
    kIdCfinv,
    kIdCinc,
    kIdCinv,
    kIdClrex,
    kIdCls,
    kIdClz,
    kIdCmn,
    kIdCmp,
    kIdCmpp,
    kIdCneg,
    kIdCrc32b,
    kIdCrc32cb,
    kIdCrc32ch,
    kIdCrc32cw,
    kIdCrc32cx,
    kIdCrc32h,
    kIdCrc32w,
    kIdCrc32x,
    kIdCsdb,
    kIdCsel,
    kIdCset,
    kIdCsetm,
    kIdCsinc,
    kIdCsinv,
    kIdCsneg,
    kIdDc,
    kIdDcps1,
    kIdDcps2,
    kIdDcps3,
    kIdDgh,
    kIdDmb,
    kIdDrps,
    kIdDsb,
    kIdEon,
    kIdEor,
    kIdEsb,
    kIdExtr,
    kIdEret,
    kIdGmi,
    kIdHint,
    kIdHlt,
    kIdHvc,
    kIdIc,
    kIdIsb,
    kIdLdadd,
    kIdLdadda,
    kIdLdaddab,
    kIdLdaddah,
    kIdLdaddal,
    kIdLdaddalb,
    kIdLdaddalh,
    kIdLdaddb,
    kIdLdaddh,
    kIdLdaddl,
    kIdLdaddlb,
    kIdLdaddlh,
    kIdLdar,
    kIdLdarb,
    kIdLdarh,
    kIdLdaxp,
    kIdLdaxr,
    kIdLdaxrb,
    kIdLdaxrh,
    kIdLdclr,
    kIdLdclra,
    kIdLdclrab,
    kIdLdclrah,
    kIdLdclral,
    kIdLdclralb,
    kIdLdclralh,
    kIdLdclrb,
    kIdLdclrh,
    kIdLdclrl,
    kIdLdclrlb,
    kIdLdclrlh,
    kIdLdeor,
    kIdLdeora,
    kIdLdeorab,
    kIdLdeorah,
    kIdLdeoral,
    kIdLdeoralb,
    kIdLdeoralh,
    kIdLdeorb,
    kIdLdeorh,
    kIdLdeorl,
    kIdLdeorlb,
    kIdLdeorlh,
    kIdLdg,
    kIdLdgm,
    kIdLdlar,
    kIdLdlarb,
    kIdLdlarh,
    kIdLdnp,
    kIdLdp,
    kIdLdpsw,
    kIdLdr,
    kIdLdraa,
    kIdLdrab,
    kIdLdrb,
    kIdLdrh,
    kIdLdrsb,
    kIdLdrsh,
    kIdLdrsw,
    kIdLdset,
    kIdLdseta,
    kIdLdsetab,
    kIdLdsetah,
    kIdLdsetal,
    kIdLdsetalb,
    kIdLdsetalh,
    kIdLdsetb,
    kIdLdseth,
    kIdLdsetl,
    kIdLdsetlb,
    kIdLdsetlh,
    kIdLdsmax,
    kIdLdsmaxa,
    kIdLdsmaxab,
    kIdLdsmaxah,
    kIdLdsmaxal,
    kIdLdsmaxalb,
    kIdLdsmaxalh,
    kIdLdsmaxb,
    kIdLdsmaxh,
    kIdLdsmaxl,
    kIdLdsmaxlb,
    kIdLdsmaxlh,
    kIdLdsmin,
    kIdLdsmina,
    kIdLdsminab,
    kIdLdsminah,
    kIdLdsminal,
    kIdLdsminalb,
    kIdLdsminalh,
    kIdLdsminb,
    kIdLdsminh,
    kIdLdsminl,
    kIdLdsminlb,
    kIdLdsminlh,
    kIdLdtr,
    kIdLdtrb,
    kIdLdtrh,
    kIdLdtrsb,
    kIdLdtrsh,
    kIdLdtrsw,
    kIdLdumax,
    kIdLdumaxa,
    kIdLdumaxab,
    kIdLdumaxah,
    kIdLdumaxal,
    kIdLdumaxalb,
    kIdLdumaxalh,
    kIdLdumaxb,
    kIdLdumaxh,
    kIdLdumaxl,
    kIdLdumaxlb,
    kIdLdumaxlh,
    kIdLdumin,
    kIdLdumina,
    kIdLduminab,
    kIdLduminah,
    kIdLduminal,
    kIdLduminalb,
    kIdLduminalh,
    kIdLduminb,
    kIdLduminh,
    kIdLduminl,
    kIdLduminlb,
    kIdLduminlh,
    kIdLdur,
    kIdLdurb,
    kIdLdurh,
    kIdLdursb,
    kIdLdursh,
    kIdLdursw,
    kIdLdxp,
    kIdLdxr,
    kIdLdxrb,
    kIdLdxrh,
    kIdLsl,
    kIdLslv,
    kIdLsr,
    kIdLsrv,
    kIdMadd,
    kIdMneg,
    kIdMov,
    kIdMovk,
    kIdMovn,
    kIdMovz,
    kIdMrs,
    kIdMsr,
    kIdMsub,
    kIdMul,
    kIdMvn,
    kIdNeg,
    kIdNegs,
    kIdNgc,
    kIdNgcs,
    kIdNop,
    kIdOrr,
    kIdPacda,
    kIdPacdb,
    kIdPacdza,
    kIdPacdzb,
    kIdPacga,
    kIdPssbb,
    kIdRbit,
    kIdRet,
    kIdRev,
    kIdRev16,
    kIdRev32,
    kIdRev64,
    kIdRor,
    kIdRorv,
    kIdSbc,
    kIdSbcs,
    kIdSbfiz,
    kIdSbfm,
    kIdSbfx,
    kIdSdiv,
    kIdSetf8,
    kIdSetf16,
    kIdSev,
    kIdSevl,
    kIdSmaddl,
    kIdSmc,
    kIdSmnegl,
    kIdSmsubl,
    kIdSmulh,
    kIdSmull,
    kIdSsbb,
    kIdSt2g,
    kIdStadd,
    kIdStaddl,
    kIdStaddb,
    kIdStaddlb,
    kIdStaddh,
    kIdStaddlh,
    kIdStclr,
    kIdStclrl,
    kIdStclrb,
    kIdStclrlb,
    kIdStclrh,
    kIdStclrlh,
    kIdSteor,
    kIdSteorl,
    kIdSteorb,
    kIdSteorlb,
    kIdSteorh,
    kIdSteorlh,
    kIdStg,
    kIdStgm,
    kIdStgp,
    kIdStllr,
    kIdStllrb,
    kIdStllrh,
    kIdStlr,
    kIdStlrb,
    kIdStlrh,
    kIdStlxp,
    kIdStlxr,
    kIdStlxrb,
    kIdStlxrh,
    kIdStnp,
    kIdStp,
    kIdStr,
    kIdStrb,
    kIdStrh,
    kIdStset,
    kIdStsetl,
    kIdStsetb,
    kIdStsetlb,
    kIdStseth,
    kIdStsetlh,
    kIdStsmax,
    kIdStsmaxl,
    kIdStsmaxb,
    kIdStsmaxlb,
    kIdStsmaxh,
    kIdStsmaxlh,
    kIdStsmin,
    kIdStsminl,
    kIdStsminb,
    kIdStsminlb,
    kIdStsminh,
    kIdStsminlh,
    kIdSttr,
    kIdSttrb,
    kIdSttrh,
    kIdStumax,
    kIdStumaxl,
    kIdStumaxb,
    kIdStumaxlb,
    kIdStumaxh,
    kIdStumaxlh,
    kIdStumin,
    kIdStuminl,
    kIdStuminb,
    kIdStuminlb,
    kIdStuminh,
    kIdStuminlh,
    kIdStur,
    kIdSturb,
    kIdSturh,
    kIdStxp,
    kIdStxr,
    kIdStxrb,
    kIdStxrh,
    kIdStz2g,
    kIdStzg,
    kIdStzgm,
    kIdSub,
    kIdSubg,
    kIdSubp,
    kIdSubps,
    kIdSubs,
    kIdSvc,
    kIdSwp,
    kIdSwpa,
    kIdSwpab,
    kIdSwpah,
    kIdSwpal,
    kIdSwpalb,
    kIdSwpalh,
    kIdSwpb,
    kIdSwph,
    kIdSwpl,
    kIdSwplb,
    kIdSwplh,
    kIdSxtb,
    kIdSxth,
    kIdSxtw,
    kIdSys,
    kIdTlbi,
    kIdTst,
    kIdTbnz,
    kIdTbz,
    kIdUbfiz,
    kIdUbfm,
    kIdUbfx,
    kIdUdf,
    kIdUdiv,
    kIdUmaddl,
    kIdUmnegl,
    kIdUmull,
    kIdUmulh,
    kIdUmsubl,
    kIdUxtb,
    kIdUxth,
    kIdWfe,
    kIdWfi,
    kIdXaflag,
    kIdXpacd,
    kIdXpaci,
    kIdXpaclri,
    kIdYield,
    kIdAbs_v,
    kIdAdd_v,
    kIdAddhn_v,
    kIdAddhn2_v,
    kIdAddp_v,
    kIdAddv_v,
    kIdAesd_v,
    kIdAese_v,
    kIdAesimc_v,
    kIdAesmc_v,
    kIdAnd_v,
    kIdBcax_v,
    kIdBfcvt_v,
    kIdBfcvtn_v,
    kIdBfcvtn2_v,
    kIdBfdot_v,
    kIdBfmlalb_v,
    kIdBfmlalt_v,
    kIdBfmmla_v,
    kIdBic_v,
    kIdBif_v,
    kIdBit_v,
    kIdBsl_v,
    kIdCls_v,
    kIdClz_v,
    kIdCmeq_v,
    kIdCmge_v,
    kIdCmgt_v,
    kIdCmhi_v,
    kIdCmhs_v,
    kIdCmle_v,
    kIdCmlt_v,
    kIdCmtst_v,
    kIdCnt_v,
    kIdDup_v,
    kIdEor_v,
    kIdEor3_v,
    kIdExt_v,
    kIdFabd_v,
    kIdFabs_v,
    kIdFacge_v,
    kIdFacgt_v,
    kIdFadd_v,
    kIdFaddp_v,
    kIdFcadd_v,
    kIdFccmp_v,
    kIdFccmpe_v,
    kIdFcmeq_v,
    kIdFcmge_v,
    kIdFcmgt_v,
    kIdFcmla_v,
    kIdFcmle_v,
    kIdFcmlt_v,
    kIdFcmp_v,
    kIdFcmpe_v,
    kIdFcsel_v,
    kIdFcvt_v,
    kIdFcvtas_v,
    kIdFcvtau_v,
    kIdFcvtl_v,
    kIdFcvtl2_v,
    kIdFcvtms_v,
    kIdFcvtmu_v,
    kIdFcvtn_v,
    kIdFcvtn2_v,
    kIdFcvtns_v,
    kIdFcvtnu_v,
    kIdFcvtps_v,
    kIdFcvtpu_v,
    kIdFcvtxn_v,
    kIdFcvtxn2_v,
    kIdFcvtzs_v,
    kIdFcvtzu_v,
    kIdFdiv_v,
    kIdFjcvtzs_v,
    kIdFmadd_v,
    kIdFmax_v,
    kIdFmaxnm_v,
    kIdFmaxnmp_v,
    kIdFmaxnmv_v,
    kIdFmaxp_v,
    kIdFmaxv_v,
    kIdFmin_v,
    kIdFminnm_v,
    kIdFminnmp_v,
    kIdFminnmv_v,
    kIdFminp_v,
    kIdFminv_v,
    kIdFmla_v,
    kIdFmlal_v,
    kIdFmlal2_v,
    kIdFmls_v,
    kIdFmlsl_v,
    kIdFmlsl2_v,
    kIdFmov_v,
    kIdFmsub_v,
    kIdFmul_v,
    kIdFmulx_v,
    kIdFneg_v,
    kIdFnmadd_v,
    kIdFnmsub_v,
    kIdFnmul_v,
    kIdFrecpe_v,
    kIdFrecps_v,
    kIdFrecpx_v,
    kIdFrint32x_v,
    kIdFrint32z_v,
    kIdFrint64x_v,
    kIdFrint64z_v,
    kIdFrinta_v,
    kIdFrinti_v,
    kIdFrintm_v,
    kIdFrintn_v,
    kIdFrintp_v,
    kIdFrintx_v,
    kIdFrintz_v,
    kIdFrsqrte_v,
    kIdFrsqrts_v,
    kIdFsqrt_v,
    kIdFsub_v,
    kIdIns_v,
    kIdLd1_v,
    kIdLd1r_v,
    kIdLd2_v,
    kIdLd2r_v,
    kIdLd3_v,
    kIdLd3r_v,
    kIdLd4_v,
    kIdLd4r_v,
    kIdLdnp_v,
    kIdLdp_v,
    kIdLdr_v,
    kIdLdur_v,
    kIdMla_v,
    kIdMls_v,
    kIdMov_v,
    kIdMovi_v,
    kIdMul_v,
    kIdMvn_v,
    kIdMvni_v,
    kIdNeg_v,
    kIdNot_v,
    kIdOrn_v,
    kIdOrr_v,
    kIdPmul_v,
    kIdPmull_v,
    kIdPmull2_v,
    kIdRaddhn_v,
    kIdRaddhn2_v,
    kIdRax1_v,
    kIdRbit_v,
    kIdRev16_v,
    kIdRev32_v,
    kIdRev64_v,
    kIdRshrn_v,
    kIdRshrn2_v,
    kIdRsubhn_v,
    kIdRsubhn2_v,
    kIdSaba_v,
    kIdSabal_v,
    kIdSabal2_v,
    kIdSabd_v,
    kIdSabdl_v,
    kIdSabdl2_v,
    kIdSadalp_v,
    kIdSaddl_v,
    kIdSaddl2_v,
    kIdSaddlp_v,
    kIdSaddlv_v,
    kIdSaddw_v,
    kIdSaddw2_v,
    kIdScvtf_v,
    kIdSdot_v,
    kIdSha1c_v,
    kIdSha1h_v,
    kIdSha1m_v,
    kIdSha1p_v,
    kIdSha1su0_v,
    kIdSha1su1_v,
    kIdSha256h_v,
    kIdSha256h2_v,
    kIdSha256su0_v,
    kIdSha256su1_v,
    kIdSha512h_v,
    kIdSha512h2_v,
    kIdSha512su0_v,
    kIdSha512su1_v,
    kIdShadd_v,
    kIdShl_v,
    kIdShll_v,
    kIdShll2_v,
    kIdShrn_v,
    kIdShrn2_v,
    kIdShsub_v,
    kIdSli_v,
    kIdSm3partw1_v,
    kIdSm3partw2_v,
    kIdSm3ss1_v,
    kIdSm3tt1a_v,
    kIdSm3tt1b_v,
    kIdSm3tt2a_v,
    kIdSm3tt2b_v,
    kIdSm4e_v,
    kIdSm4ekey_v,
    kIdSmax_v,
    kIdSmaxp_v,
    kIdSmaxv_v,
    kIdSmin_v,
    kIdSminp_v,
    kIdSminv_v,
    kIdSmlal_v,
    kIdSmlal2_v,
    kIdSmlsl_v,
    kIdSmlsl2_v,
    kIdSmmla_v,
    kIdSmov_v,
    kIdSmull_v,
    kIdSmull2_v,
    kIdSqabs_v,
    kIdSqadd_v,
    kIdSqdmlal_v,
    kIdSqdmlal2_v,
    kIdSqdmlsl_v,
    kIdSqdmlsl2_v,
    kIdSqdmulh_v,
    kIdSqdmull_v,
    kIdSqdmull2_v,
    kIdSqneg_v,
    kIdSqrdmlah_v,
    kIdSqrdmlsh_v,
    kIdSqrdmulh_v,
    kIdSqrshl_v,
    kIdSqrshrn_v,
    kIdSqrshrn2_v,
    kIdSqrshrun_v,
    kIdSqrshrun2_v,
    kIdSqshl_v,
    kIdSqshlu_v,
    kIdSqshrn_v,
    kIdSqshrn2_v,
    kIdSqshrun_v,
    kIdSqshrun2_v,
    kIdSqsub_v,
    kIdSqxtn_v,
    kIdSqxtn2_v,
    kIdSqxtun_v,
    kIdSqxtun2_v,
    kIdSrhadd_v,
    kIdSri_v,
    kIdSrshl_v,
    kIdSrshr_v,
    kIdSrsra_v,
    kIdSshl_v,
    kIdSshll_v,
    kIdSshll2_v,
    kIdSshr_v,
    kIdSsra_v,
    kIdSsubl_v,
    kIdSsubl2_v,
    kIdSsubw_v,
    kIdSsubw2_v,
    kIdSt1_v,
    kIdSt2_v,
    kIdSt3_v,
    kIdSt4_v,
    kIdStnp_v,
    kIdStp_v,
    kIdStr_v,
    kIdStur_v,
    kIdSub_v,
    kIdSubhn_v,
    kIdSubhn2_v,
    kIdSudot_v,
    kIdSuqadd_v,
    kIdSxtl_v,
    kIdSxtl2_v,
    kIdTbl_v,
    kIdTbx_v,
    kIdTrn1_v,
    kIdTrn2_v,
    kIdUaba_v,
    kIdUabal_v,
    kIdUabal2_v,
    kIdUabd_v,
    kIdUabdl_v,
    kIdUabdl2_v,
    kIdUadalp_v,
    kIdUaddl_v,
    kIdUaddl2_v,
    kIdUaddlp_v,
    kIdUaddlv_v,
    kIdUaddw_v,
    kIdUaddw2_v,
    kIdUcvtf_v,
    kIdUdot_v,
    kIdUhadd_v,
    kIdUhsub_v,
    kIdUmax_v,
    kIdUmaxp_v,
    kIdUmaxv_v,
    kIdUmin_v,
    kIdUminp_v,
    kIdUminv_v,
    kIdUmlal_v,
    kIdUmlal2_v,
    kIdUmlsl_v,
    kIdUmlsl2_v,
    kIdUmmla_v,
    kIdUmov_v,
    kIdUmull_v,
    kIdUmull2_v,
    kIdUqadd_v,
    kIdUqrshl_v,
    kIdUqrshrn_v,
    kIdUqrshrn2_v,
    kIdUqshl_v,
    kIdUqshrn_v,
    kIdUqshrn2_v,
    kIdUqsub_v,
    kIdUqxtn_v,
    kIdUqxtn2_v,
    kIdUrecpe_v,
    kIdUrhadd_v,
    kIdUrshl_v,
    kIdUrshr_v,
    kIdUrsqrte_v,
    kIdUrsra_v,
    kIdUsdot_v,
    kIdUshl_v,
    kIdUshll_v,
    kIdUshll2_v,
    kIdUshr_v,
    kIdUsmmla_v,
    kIdUsqadd_v,
    kIdUsra_v,
    kIdUsubl_v,
    kIdUsubl2_v,
    kIdUsubw_v,
    kIdUsubw2_v,
    kIdUxtl_v,
    kIdUxtl2_v,
    kIdUzp1_v,
    kIdUzp2_v,
    kIdXar_v,
    kIdXtn_v,
    kIdXtn2_v,
    kIdZip1_v,
    kIdZip2_v,
    _kIdCount
    // ${InstId:End}
  };

  //! Instruction options
  enum Options : uint32_t {
    //! Condition code flag shift
    kOptionCondFlagShift = 27,
    //! Condition code flag shift
    kOptionCondFlagMask = 1u << kOptionCondFlagShift,

    //! Condition code value shift.
    kOptionCondCodeShift = 28u,
    //! Condition code value mask.
    kOptionCondCodeMask = 0xFu << kOptionCondCodeShift
  };

  // --------------------------------------------------------------------------
  // [Statics]
  // --------------------------------------------------------------------------

  //! Tests whether the `instId` is defined (counts also Inst::kIdNone, which must be zero).
  static inline bool isDefinedId(uint32_t instId) noexcept { return instId < _kIdCount; }
};

// ============================================================================
// [asmjit::a64::Predicate]
// ============================================================================

namespace Predicate {

//! Address translate options (AT).
namespace AT {
  static inline constexpr uint32_t encode(uint32_t op1, uint32_t cRn, uint32_t cRm, uint32_t op2) noexcept {
    return (op1 << 11) | (cRn << 7) | (cRm << 3) | (op2 << 0);
  }

  enum Value : uint32_t {
    kS1E1R  = encode(0b000, 0b0111, 0b1000, 0b000),
    kS1E2R  = encode(0b100, 0b0111, 0b1000, 0b000),
    kS1E3R  = encode(0b110, 0b0111, 0b1000, 0b000),
    kS1E1W  = encode(0b000, 0b0111, 0b1000, 0b001),
    kS1E2W  = encode(0b100, 0b0111, 0b1000, 0b001),
    kS1E3W  = encode(0b110, 0b0111, 0b1000, 0b001),
    kS1E0R  = encode(0b000, 0b0111, 0b1000, 0b010),
    kS1E0W  = encode(0b000, 0b0111, 0b1000, 0b011),
    kS12E1R = encode(0b100, 0b0111, 0b1000, 0b100),
    kS12E1W = encode(0b100, 0b0111, 0b1000, 0b101),
    kS12E0R = encode(0b100, 0b0111, 0b1000, 0b110),
    kS12E0W = encode(0b100, 0b0111, 0b1000, 0b111),
    kS1E1RP = encode(0b000, 0b0111, 0b1001, 0b000),
    kS1E1WP = encode(0b000, 0b0111, 0b1001, 0b001)
  };
}

//! Data barrier options (DMB/DSB).
namespace DB {
  //! Data barrier immediate values.
  enum Value : uint32_t {
    //! Waits only for loads to complete, and only applies to the outer shareable domain.
    kOSHLD = 0x01u,
    //! Waits only for stores to complete, and only applies to the outer shareable domain.
    kOSHST = 0x02u,
    //! Only applies to the outer shareable domain.
    kOSH = 0x03u,

    //! Waits only for loads to complete and only applies out to the point of unification.
    kNSHLD = 0x05u,
    //! Waits only for stores to complete and only applies out to the point of unification.
    kNSHST = 0x06u,
    //! Only applies out to the point of unification.
    kNSH = 0x07u,

    //! Waits only for loads to complete, and only applies to the inner shareable domain.
    kISHLD = 0x09u,
    //! Waits only for stores to complete, and only applies to the inner shareable domain.
    kISHST = 0x0Au,
    //! Only applies to the inner shareable domain.
    kISH = 0x0Bu,

    //! Waits only for loads to complete.
    kLD = 0x0Du,
    //! Waits only for stores to complete.
    kST = 0x0Eu,
    //! Full system memory barrier operation.
    kSY = 0x0Fu
  };
}

//! Data cache maintenance options.
namespace DC {
  static inline constexpr uint32_t encode(uint32_t op1, uint32_t cRn, uint32_t cRm, uint32_t op2) noexcept {
    return (op1 << 11) | (cRn << 7) | (cRm << 3) | (op2 << 0);
  }

  //! Data cache maintenance immediate values.
  enum Value : uint32_t {
    kZVA     = encode(0b011, 0b0111, 0b0100, 0b001),
    kIVAC    = encode(0b000, 0b0111, 0b0110, 0b001),
    kISW     = encode(0b000, 0b0111, 0b0110, 0b010),
    kCVAC    = encode(0b011, 0b0111, 0b1010, 0b001),
    kCSW     = encode(0b000, 0b0111, 0b1010, 0b010),
    kCVAU    = encode(0b011, 0b0111, 0b1011, 0b001),
    kCIVAC   = encode(0b011, 0b0111, 0b1110, 0b001),
    kCISW    = encode(0b000, 0b0111, 0b1110, 0b010),
    kCVAP    = encode(0b011, 0b0111, 0b1100, 0b001),
    kCVADP   = encode(0b011, 0b0111, 0b1101, 0b001),
    kIGVAC   = encode(0b000, 0b0111, 0b0110, 0b011),
    kIGSW    = encode(0b000, 0b0111, 0b0110, 0b100),
    kCGSW    = encode(0b000, 0b0111, 0b1010, 0b100),
    kCIGSW   = encode(0b000, 0b0111, 0b1110, 0b100),
    kCGVAC   = encode(0b011, 0b0111, 0b1010, 0b011),
    kCGVAP   = encode(0b011, 0b0111, 0b1100, 0b011),
    kCGVADP  = encode(0b011, 0b0111, 0b1101, 0b011),
    kCIGVAC  = encode(0b011, 0b0111, 0b1110, 0b011),
    kGVA     = encode(0b011, 0b0111, 0b0100, 0b011),
    kIGDVAC  = encode(0b000, 0b0111, 0b0110, 0b101),
    kIGDSW   = encode(0b000, 0b0111, 0b0110, 0b110),
    kCGDSW   = encode(0b000, 0b0111, 0b1010, 0b110),
    kCIGDSW  = encode(0b000, 0b0111, 0b1110, 0b110),
    kCGDVAC  = encode(0b011, 0b0111, 0b1010, 0b101),
    kCGDVAP  = encode(0b011, 0b0111, 0b1100, 0b101),
    kCGDVADP = encode(0b011, 0b0111, 0b1101, 0b101),
    kCIGDVAC = encode(0b011, 0b0111, 0b1110, 0b101),
    kGZVA    = encode(0b011, 0b0111, 0b0100, 0b100)
  };
}

//! Instruction cache maintenance options.
namespace IC {
  static inline constexpr uint32_t encode(uint32_t op1, uint32_t cRn, uint32_t cRm, uint32_t op2) noexcept {
    return (op1 << 11) | (cRn << 7) | (cRm << 3) | (op2 << 0);
  }

  //! Instruction cache maintenance immediate values.
  enum Value : uint32_t {
    kIALLUIS = encode(0b000, 0b0111, 0b0001, 0b000),
    kIALLU   = encode(0b000, 0b0111, 0b0101, 0b000),
    kIVAU    = encode(0b011, 0b0111, 0b0101, 0b001)
  };
}

//! Instruction-fetch barrier options.
namespace ISB {
  //! Instruction-fetch barrier immediate values.
  enum Value : uint32_t {
    kSY = 0xF
  };
}

//! Prefetch options.
namespace PRFOp {
  //! Prefetch immediate values.
  enum Value : uint32_t {
    kPLDL1KEEP = 0x00,
    kPLDL1STRM = 0x01,
    kPLDL2KEEP = 0x02,
    kPLDL2STRM = 0x03,
    kPLDL3KEEP = 0x04,
    kPLDL3STRM = 0x05,
    kPLIL1KEEP = 0x08,
    kPLIL1STRM = 0x09,
    kPLIL2KEEP = 0x0A,
    kPLIL2STRM = 0x0B,
    kPLIL3KEEP = 0x0C,
    kPLIL3STRM = 0x0D,
    kPSTL1KEEP = 0x10,
    kPSTL1STRM = 0x11,
    kPSTL2KEEP = 0x12,
    kPSTL2STRM = 0x13,
    kPSTL3KEEP = 0x14,
    kPSTL3STRM = 0x15
  };
}

//! PSB instruction options.
namespace PSB {
  //! PSB immediate values.
  enum Value : uint32_t {
    kCSYNC = 0x11u
  };
}

namespace TLBI {
  static inline constexpr uint32_t encode(uint32_t op1, uint32_t cRn, uint32_t cRm, uint32_t op2) noexcept {
    return (op1 << 11) | (cRn << 7) | (cRm << 3) | (op2 << 0);
  }

  enum Value : uint32_t {
    kIPAS2E1IS    = encode(0b100, 0b1000, 0b0000, 0b001),
    kIPAS2LE1IS   = encode(0b100, 0b1000, 0b0000, 0b101),
    kVMALLE1IS    = encode(0b000, 0b1000, 0b0011, 0b000),
    kALLE2IS      = encode(0b100, 0b1000, 0b0011, 0b000),
    kALLE3IS      = encode(0b110, 0b1000, 0b0011, 0b000),
    kVAE1IS       = encode(0b000, 0b1000, 0b0011, 0b001),
    kVAE2IS       = encode(0b100, 0b1000, 0b0011, 0b001),
    kVAE3IS       = encode(0b110, 0b1000, 0b0011, 0b001),
    kASIDE1IS     = encode(0b000, 0b1000, 0b0011, 0b010),
    kVAAE1IS      = encode(0b000, 0b1000, 0b0011, 0b011),
    kALLE1IS      = encode(0b100, 0b1000, 0b0011, 0b100),
    kVALE1IS      = encode(0b000, 0b1000, 0b0011, 0b101),
    kVALE2IS      = encode(0b100, 0b1000, 0b0011, 0b101),
    kVALE3IS      = encode(0b110, 0b1000, 0b0011, 0b101),
    kVMALLS12E1IS = encode(0b100, 0b1000, 0b0011, 0b110),
    kVAALE1IS     = encode(0b000, 0b1000, 0b0011, 0b111),
    kIPAS2E1      = encode(0b100, 0b1000, 0b0100, 0b001),
    kIPAS2LE1     = encode(0b100, 0b1000, 0b0100, 0b101),
    kVMALLE1      = encode(0b000, 0b1000, 0b0111, 0b000),
    kALLE2        = encode(0b100, 0b1000, 0b0111, 0b000),
    kALLE3        = encode(0b110, 0b1000, 0b0111, 0b000),
    kVAE1         = encode(0b000, 0b1000, 0b0111, 0b001),
    kVAE2         = encode(0b100, 0b1000, 0b0111, 0b001),
    kVAE3         = encode(0b110, 0b1000, 0b0111, 0b001),
    kASIDE1       = encode(0b000, 0b1000, 0b0111, 0b010),
    kVAAE1        = encode(0b000, 0b1000, 0b0111, 0b011),
    kALLE1        = encode(0b100, 0b1000, 0b0111, 0b100),
    kVALE1        = encode(0b000, 0b1000, 0b0111, 0b101),
    kVALE2        = encode(0b100, 0b1000, 0b0111, 0b101),
    kVALE3        = encode(0b110, 0b1000, 0b0111, 0b101),
    kVMALLS12E1   = encode(0b100, 0b1000, 0b0111, 0b110),
    kVAALE1       = encode(0b000, 0b1000, 0b0111, 0b111),

    kVMALLE1OS    = encode(0b000, 0b1000, 0b0001, 0b000),
    kVAE1OS       = encode(0b000, 0b1000, 0b0001, 0b001),
    kASIDE1OS     = encode(0b000, 0b1000, 0b0001, 0b010),
    kVAAE1OS      = encode(0b000, 0b1000, 0b0001, 0b011),
    kVALE1OS      = encode(0b000, 0b1000, 0b0001, 0b101),
    kVAALE1OS     = encode(0b000, 0b1000, 0b0001, 0b111),
    kIPAS2E1OS    = encode(0b100, 0b1000, 0b0100, 0b000),
    kIPAS2LE1OS   = encode(0b100, 0b1000, 0b0100, 0b100),
    kVAE2OS       = encode(0b100, 0b1000, 0b0001, 0b001),
    kVALE2OS      = encode(0b100, 0b1000, 0b0001, 0b101),
    kVMALLS12E1OS = encode(0b100, 0b1000, 0b0001, 0b110),
    kVAE3OS       = encode(0b110, 0b1000, 0b0001, 0b001),
    kVALE3OS      = encode(0b110, 0b1000, 0b0001, 0b101),
    kALLE2OS      = encode(0b100, 0b1000, 0b0001, 0b000),
    kALLE1OS      = encode(0b100, 0b1000, 0b0001, 0b100),
    kALLE3OS      = encode(0b110, 0b1000, 0b0001, 0b000),

    kRVAE1        = encode(0b000, 0b1000, 0b0110, 0b001),
    kRVAAE1       = encode(0b000, 0b1000, 0b0110, 0b011),
    kRVALE1       = encode(0b000, 0b1000, 0b0110, 0b101),
    kRVAALE1      = encode(0b000, 0b1000, 0b0110, 0b111),
    kRVAE1IS      = encode(0b000, 0b1000, 0b0010, 0b001),
    kRVAAE1IS     = encode(0b000, 0b1000, 0b0010, 0b011),
    kRVALE1IS     = encode(0b000, 0b1000, 0b0010, 0b101),
    kRVAALE1IS    = encode(0b000, 0b1000, 0b0010, 0b111),
    kRVAE1OS      = encode(0b000, 0b1000, 0b0101, 0b001),
    kRVAAE1OS     = encode(0b000, 0b1000, 0b0101, 0b011),
    kRVALE1OS     = encode(0b000, 0b1000, 0b0101, 0b101),
    kRVAALE1OS    = encode(0b000, 0b1000, 0b0101, 0b111),
    kRIPAS2E1IS   = encode(0b100, 0b1000, 0b0000, 0b010),
    kRIPAS2LE1IS  = encode(0b100, 0b1000, 0b0000, 0b110),
    kRIPAS2E1     = encode(0b100, 0b1000, 0b0100, 0b010),
    kRIPAS2LE1    = encode(0b100, 0b1000, 0b0100, 0b110),
    kRIPAS2E1OS   = encode(0b100, 0b1000, 0b0100, 0b011),
    kRIPAS2LE1OS  = encode(0b100, 0b1000, 0b0100, 0b111),
    kRVAE2        = encode(0b100, 0b1000, 0b0110, 0b001),
    kRVALE2       = encode(0b100, 0b1000, 0b0110, 0b101),
    kRVAE2IS      = encode(0b100, 0b1000, 0b0010, 0b001),
    kRVALE2IS     = encode(0b100, 0b1000, 0b0010, 0b101),
    kRVAE2OS      = encode(0b100, 0b1000, 0b0101, 0b001),
    kRVALE2OS     = encode(0b100, 0b1000, 0b0101, 0b101),
    kRVAE3        = encode(0b110, 0b1000, 0b0110, 0b001),
    kRVALE3       = encode(0b110, 0b1000, 0b0110, 0b101),
    kRVAE3IS      = encode(0b110, 0b1000, 0b0010, 0b001),
    kRVALE3IS     = encode(0b110, 0b1000, 0b0010, 0b101),
    kRVAE3OS      = encode(0b110, 0b1000, 0b0101, 0b001),
    kRVALE3OS     = encode(0b110, 0b1000, 0b0101, 0b101),
  };
}

//! Trace synchronization barrier options.
namespace TSB {
  //! Trace synchronization immediate values.
  enum Value : uint32_t {
    kCSYNC = 0
  };
}

//! Processor state access through MSR.
namespace PState {
  //! Encodes a pstate from `op0` and `op1`.
  static inline constexpr uint32_t encode(uint32_t op0, uint32_t op1) noexcept {
    return (op0 << 3) | (op1 << 0);
  }

  //! Processor state access immediates.
  enum Value : uint32_t {
    kSPSel   = encode(0b000, 0b101),
    kDAIFSet = encode(0b011, 0b110),
    kDAIFClr = encode(0b011, 0b111),
    kPAN     = encode(0b000, 0b100),
    kUAO     = encode(0b000, 0b011),
    kDIT     = encode(0b011, 0b010),
    kSSBS    = encode(0b011, 0b001),
    kTCO     = encode(0b011, 0b100)
  };
};

//! System register identifiers and utilities (MSR/MRS).
namespace SysReg {
  //! System register fields.
  struct Fields {
    uint8_t op0;
    uint8_t op1;
    uint8_t cRn;
    uint8_t cRm;
    uint8_t op2;
  };

  //! Encodes a system register from `op0`, `op1`, `cRn`, `cRm`, and `op2` fields.
  static inline constexpr uint32_t encode(uint32_t op0, uint32_t op1, uint32_t cRn, uint32_t cRm, uint32_t op2) noexcept {
    return (op0 << 14) | (op1 << 11) | (cRn << 7) | (cRm << 3) | (op2 << 0);
  }

  //! Encodes a system register from `fields`.
  static inline constexpr uint32_t encode(const Fields& fields) noexcept {
    return encode(fields.op0, fields.op1, fields.cRn, fields.cRm, fields.op2);
  }

  //! Decodes a system register to \ref Fields.
  static inline constexpr Fields decode(uint32_t id) noexcept {
    return Fields {
      uint8_t((id >> 14) & 0x3u),
      uint8_t((id >> 11) & 0x7u),
      uint8_t((id >>  7) & 0xFu),
      uint8_t((id >>  3) & 0xFu),
      uint8_t((id >>  0) & 0x7u)
    };
  }

  //! System register identifiers.
  enum Id : uint32_t {
    kACTLR_EL1            = encode(0b11, 0b000, 0b0001, 0b0000, 0b001), // RW
    kACTLR_EL2            = encode(0b11, 0b100, 0b0001, 0b0000, 0b001), // RW
    kACTLR_EL3            = encode(0b11, 0b110, 0b0001, 0b0000, 0b001), // RW
    kAFSR0_EL1            = encode(0b11, 0b000, 0b0101, 0b0001, 0b000), // RW
    kAFSR0_EL12           = encode(0b11, 0b101, 0b0101, 0b0001, 0b000), // RW
    kAFSR0_EL2            = encode(0b11, 0b100, 0b0101, 0b0001, 0b000), // RW
    kAFSR0_EL3            = encode(0b11, 0b110, 0b0101, 0b0001, 0b000), // RW
    kAFSR1_EL1            = encode(0b11, 0b000, 0b0101, 0b0001, 0b001), // RW
    kAFSR1_EL12           = encode(0b11, 0b101, 0b0101, 0b0001, 0b001), // RW
    kAFSR1_EL2            = encode(0b11, 0b100, 0b0101, 0b0001, 0b001), // RW
    kAFSR1_EL3            = encode(0b11, 0b110, 0b0101, 0b0001, 0b001), // RW
    kAIDR_EL1             = encode(0b11, 0b001, 0b0000, 0b0000, 0b111), // RO
    kAMAIR_EL1            = encode(0b11, 0b000, 0b1010, 0b0011, 0b000), // RW
    kAMAIR_EL12           = encode(0b11, 0b101, 0b1010, 0b0011, 0b000), // RW
    kAMAIR_EL2            = encode(0b11, 0b100, 0b1010, 0b0011, 0b000), // RW
    kAMAIR_EL3            = encode(0b11, 0b110, 0b1010, 0b0011, 0b000), // RW
    kAMCFGR_EL0           = encode(0b11, 0b011, 0b1101, 0b0010, 0b001), // RO
    kAMCGCR_EL0           = encode(0b11, 0b011, 0b1101, 0b0010, 0b010), // RO
    kAMCNTENCLR0_EL0      = encode(0b11, 0b011, 0b1101, 0b0010, 0b100), // RW
    kAMCNTENCLR1_EL0      = encode(0b11, 0b011, 0b1101, 0b0011, 0b000), // RW
    kAMCNTENSET0_EL0      = encode(0b11, 0b011, 0b1101, 0b0010, 0b101), // RW
    kAMCNTENSET1_EL0      = encode(0b11, 0b011, 0b1101, 0b0011, 0b001), // RW
    kAMCR_EL0             = encode(0b11, 0b011, 0b1101, 0b0010, 0b000), // RW
    kAMEVCNTR00_EL0       = encode(0b11, 0b011, 0b1101, 0b0100, 0b000), // RW
    kAMEVCNTR01_EL0       = encode(0b11, 0b011, 0b1101, 0b0100, 0b001), // RW
    kAMEVCNTR02_EL0       = encode(0b11, 0b011, 0b1101, 0b0100, 0b010), // RW
    kAMEVCNTR03_EL0       = encode(0b11, 0b011, 0b1101, 0b0100, 0b011), // RW
    kAMEVCNTR10_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b000), // RW
    kAMEVCNTR110_EL0      = encode(0b11, 0b011, 0b1101, 0b1101, 0b010), // RW
    kAMEVCNTR111_EL0      = encode(0b11, 0b011, 0b1101, 0b1101, 0b011), // RW
    kAMEVCNTR112_EL0      = encode(0b11, 0b011, 0b1101, 0b1101, 0b100), // RW
    kAMEVCNTR113_EL0      = encode(0b11, 0b011, 0b1101, 0b1101, 0b101), // RW
    kAMEVCNTR114_EL0      = encode(0b11, 0b011, 0b1101, 0b1101, 0b110), // RW
    kAMEVCNTR115_EL0      = encode(0b11, 0b011, 0b1101, 0b1101, 0b111), // RW
    kAMEVCNTR11_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b001), // RW
    kAMEVCNTR12_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b010), // RW
    kAMEVCNTR13_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b011), // RW
    kAMEVCNTR14_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b100), // RW
    kAMEVCNTR15_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b101), // RW
    kAMEVCNTR16_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b110), // RW
    kAMEVCNTR17_EL0       = encode(0b11, 0b011, 0b1101, 0b1100, 0b111), // RW
    kAMEVCNTR18_EL0       = encode(0b11, 0b011, 0b1101, 0b1101, 0b000), // RW
    kAMEVCNTR19_EL0       = encode(0b11, 0b011, 0b1101, 0b1101, 0b001), // RW
    kAMEVTYPER00_EL0      = encode(0b11, 0b011, 0b1101, 0b0110, 0b000), // RO
    kAMEVTYPER01_EL0      = encode(0b11, 0b011, 0b1101, 0b0110, 0b001), // RO
    kAMEVTYPER02_EL0      = encode(0b11, 0b011, 0b1101, 0b0110, 0b010), // RO
    kAMEVTYPER03_EL0      = encode(0b11, 0b011, 0b1101, 0b0110, 0b011), // RO
    kAMEVTYPER10_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b000), // RW
    kAMEVTYPER110_EL0     = encode(0b11, 0b011, 0b1101, 0b1111, 0b010), // RW
    kAMEVTYPER111_EL0     = encode(0b11, 0b011, 0b1101, 0b1111, 0b011), // RW
    kAMEVTYPER112_EL0     = encode(0b11, 0b011, 0b1101, 0b1111, 0b100), // RW
    kAMEVTYPER113_EL0     = encode(0b11, 0b011, 0b1101, 0b1111, 0b101), // RW
    kAMEVTYPER114_EL0     = encode(0b11, 0b011, 0b1101, 0b1111, 0b110), // RW
    kAMEVTYPER115_EL0     = encode(0b11, 0b011, 0b1101, 0b1111, 0b111), // RW
    kAMEVTYPER11_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b001), // RW
    kAMEVTYPER12_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b010), // RW
    kAMEVTYPER13_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b011), // RW
    kAMEVTYPER14_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b100), // RW
    kAMEVTYPER15_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b101), // RW
    kAMEVTYPER16_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b110), // RW
    kAMEVTYPER17_EL0      = encode(0b11, 0b011, 0b1101, 0b1110, 0b111), // RW
    kAMEVTYPER18_EL0      = encode(0b11, 0b011, 0b1101, 0b1111, 0b000), // RW
    kAMEVTYPER19_EL0      = encode(0b11, 0b011, 0b1101, 0b1111, 0b001), // RW
    kAMUSERENR_EL0        = encode(0b11, 0b011, 0b1101, 0b0010, 0b011), // RW
    kAPDAKeyHi_EL1        = encode(0b11, 0b000, 0b0010, 0b0010, 0b001), // RW
    kAPDAKeyLo_EL1        = encode(0b11, 0b000, 0b0010, 0b0010, 0b000), // RW
    kAPDBKeyHi_EL1        = encode(0b11, 0b000, 0b0010, 0b0010, 0b011), // RW
    kAPDBKeyLo_EL1        = encode(0b11, 0b000, 0b0010, 0b0010, 0b010), // RW
    kAPGAKeyHi_EL1        = encode(0b11, 0b000, 0b0010, 0b0011, 0b001), // RW
    kAPGAKeyLo_EL1        = encode(0b11, 0b000, 0b0010, 0b0011, 0b000), // RW
    kAPIAKeyHi_EL1        = encode(0b11, 0b000, 0b0010, 0b0001, 0b001), // RW
    kAPIAKeyLo_EL1        = encode(0b11, 0b000, 0b0010, 0b0001, 0b000), // RW
    kAPIBKeyHi_EL1        = encode(0b11, 0b000, 0b0010, 0b0001, 0b011), // RW
    kAPIBKeyLo_EL1        = encode(0b11, 0b000, 0b0010, 0b0001, 0b010), // RW
    kCCSIDR2_EL1          = encode(0b11, 0b001, 0b0000, 0b0000, 0b010), // RO
    kCCSIDR_EL1           = encode(0b11, 0b001, 0b0000, 0b0000, 0b000), // RO
    kCLIDR_EL1            = encode(0b11, 0b001, 0b0000, 0b0000, 0b001), // RO
    kCNTFRQ_EL0           = encode(0b11, 0b011, 0b1110, 0b0000, 0b000), // RW
    kCNTHCTL_EL2          = encode(0b11, 0b100, 0b1110, 0b0001, 0b000), // RW
    kCNTHPS_CTL_EL2       = encode(0b11, 0b100, 0b1110, 0b0101, 0b001), // RW
    kCNTHPS_CVAL_EL2      = encode(0b11, 0b100, 0b1110, 0b0101, 0b010), // RW
    kCNTHPS_TVAL_EL2      = encode(0b11, 0b100, 0b1110, 0b0101, 0b000), // RW
    kCNTHP_CTL_EL2        = encode(0b11, 0b100, 0b1110, 0b0010, 0b001), // RW
    kCNTHP_CVAL_EL2       = encode(0b11, 0b100, 0b1110, 0b0010, 0b010), // RW
    kCNTHP_TVAL_EL2       = encode(0b11, 0b100, 0b1110, 0b0010, 0b000), // RW
    kCNTHVS_CTL_EL2       = encode(0b11, 0b100, 0b1110, 0b0100, 0b001), // RW
    kCNTHVS_CVAL_EL2      = encode(0b11, 0b100, 0b1110, 0b0100, 0b010), // RW
    kCNTHVS_TVAL_EL2      = encode(0b11, 0b100, 0b1110, 0b0100, 0b000), // RW
    kCNTHV_CTL_EL2        = encode(0b11, 0b100, 0b1110, 0b0011, 0b001), // RW
    kCNTHV_CVAL_EL2       = encode(0b11, 0b100, 0b1110, 0b0011, 0b010), // RW
    kCNTHV_TVAL_EL2       = encode(0b11, 0b100, 0b1110, 0b0011, 0b000), // RW
    kCNTISCALE_EL2        = encode(0b11, 0b100, 0b1110, 0b0000, 0b101), // RW
    kCNTKCTL_EL1          = encode(0b11, 0b000, 0b1110, 0b0001, 0b000), // RW
    kCNTKCTL_EL12         = encode(0b11, 0b101, 0b1110, 0b0001, 0b000), // RW
    kCNTPCTSS_EL0         = encode(0b11, 0b011, 0b1110, 0b0000, 0b101), // RW
    kCNTPCT_EL0           = encode(0b11, 0b011, 0b1110, 0b0000, 0b001), // RO
    kCNTPOFF_EL2          = encode(0b11, 0b100, 0b1110, 0b0000, 0b110), // RW
    kCNTPS_CTL_EL1        = encode(0b11, 0b111, 0b1110, 0b0010, 0b001), // RW
    kCNTPS_CVAL_EL1       = encode(0b11, 0b111, 0b1110, 0b0010, 0b010), // RW
    kCNTPS_TVAL_EL1       = encode(0b11, 0b111, 0b1110, 0b0010, 0b000), // RW
    kCNTP_CTL_EL0         = encode(0b11, 0b011, 0b1110, 0b0010, 0b001), // RW
    kCNTP_CTL_EL02        = encode(0b11, 0b101, 0b1110, 0b0010, 0b001), // RW
    kCNTP_CVAL_EL0        = encode(0b11, 0b011, 0b1110, 0b0010, 0b010), // RW
    kCNTP_CVAL_EL02       = encode(0b11, 0b101, 0b1110, 0b0010, 0b010), // RW
    kCNTP_TVAL_EL0        = encode(0b11, 0b011, 0b1110, 0b0010, 0b000), // RW
    kCNTP_TVAL_EL02       = encode(0b11, 0b101, 0b1110, 0b0010, 0b000), // RW
    kCNTSCALE_EL2         = encode(0b11, 0b100, 0b1110, 0b0000, 0b100), // RW
    kCNTVCTSS_EL0         = encode(0b11, 0b011, 0b1110, 0b0000, 0b110), // RW
    kCNTVCT_EL0           = encode(0b11, 0b011, 0b1110, 0b0000, 0b010), // RO
    kCNTVFRQ_EL2          = encode(0b11, 0b100, 0b1110, 0b0000, 0b111), // RW
    kCNTVOFF_EL2          = encode(0b11, 0b100, 0b1110, 0b0000, 0b011), // RW
    kCNTV_CTL_EL0         = encode(0b11, 0b011, 0b1110, 0b0011, 0b001), // RW
    kCNTV_CTL_EL02        = encode(0b11, 0b101, 0b1110, 0b0011, 0b001), // RW
    kCNTV_CVAL_EL0        = encode(0b11, 0b011, 0b1110, 0b0011, 0b010), // RW
    kCNTV_CVAL_EL02       = encode(0b11, 0b101, 0b1110, 0b0011, 0b010), // RW
    kCNTV_TVAL_EL0        = encode(0b11, 0b011, 0b1110, 0b0011, 0b000), // RW
    kCNTV_TVAL_EL02       = encode(0b11, 0b101, 0b1110, 0b0011, 0b000), // RW
    kCONTEXTIDR_EL1       = encode(0b11, 0b000, 0b1101, 0b0000, 0b001), // RW
    kCONTEXTIDR_EL12      = encode(0b11, 0b101, 0b1101, 0b0000, 0b001), // RW
    kCONTEXTIDR_EL2       = encode(0b11, 0b100, 0b1101, 0b0000, 0b001), // RW
    kCPACR_EL1            = encode(0b11, 0b000, 0b0001, 0b0000, 0b010), // RW
    kCPACR_EL12           = encode(0b11, 0b101, 0b0001, 0b0000, 0b010), // RW
    kCPM_IOACC_CTL_EL3    = encode(0b11, 0b111, 0b1111, 0b0010, 0b000), // RW
    kCPTR_EL2             = encode(0b11, 0b100, 0b0001, 0b0001, 0b010), // RW
    kCPTR_EL3             = encode(0b11, 0b110, 0b0001, 0b0001, 0b010), // RW
    kCSSELR_EL1           = encode(0b11, 0b010, 0b0000, 0b0000, 0b000), // RW
    kCTR_EL0              = encode(0b11, 0b011, 0b0000, 0b0000, 0b001), // RO
    kCurrentEL            = encode(0b11, 0b000, 0b0100, 0b0010, 0b010), // RO
    kDACR32_EL2           = encode(0b11, 0b100, 0b0011, 0b0000, 0b000), // RW
    kDAIF                 = encode(0b11, 0b011, 0b0100, 0b0010, 0b001), // RW
    kDBGAUTHSTATUS_EL1    = encode(0b10, 0b000, 0b0111, 0b1110, 0b110), // RO
    kDBGBCR0_EL1          = encode(0b10, 0b000, 0b0000, 0b0000, 0b101), // RW
    kDBGBCR10_EL1         = encode(0b10, 0b000, 0b0000, 0b1010, 0b101), // RW
    kDBGBCR11_EL1         = encode(0b10, 0b000, 0b0000, 0b1011, 0b101), // RW
    kDBGBCR12_EL1         = encode(0b10, 0b000, 0b0000, 0b1100, 0b101), // RW
    kDBGBCR13_EL1         = encode(0b10, 0b000, 0b0000, 0b1101, 0b101), // RW
    kDBGBCR14_EL1         = encode(0b10, 0b000, 0b0000, 0b1110, 0b101), // RW
    kDBGBCR15_EL1         = encode(0b10, 0b000, 0b0000, 0b1111, 0b101), // RW
    kDBGBCR1_EL1          = encode(0b10, 0b000, 0b0000, 0b0001, 0b101), // RW
    kDBGBCR2_EL1          = encode(0b10, 0b000, 0b0000, 0b0010, 0b101), // RW
    kDBGBCR3_EL1          = encode(0b10, 0b000, 0b0000, 0b0011, 0b101), // RW
    kDBGBCR4_EL1          = encode(0b10, 0b000, 0b0000, 0b0100, 0b101), // RW
    kDBGBCR5_EL1          = encode(0b10, 0b000, 0b0000, 0b0101, 0b101), // RW
    kDBGBCR6_EL1          = encode(0b10, 0b000, 0b0000, 0b0110, 0b101), // RW
    kDBGBCR7_EL1          = encode(0b10, 0b000, 0b0000, 0b0111, 0b101), // RW
    kDBGBCR8_EL1          = encode(0b10, 0b000, 0b0000, 0b1000, 0b101), // RW
    kDBGBCR9_EL1          = encode(0b10, 0b000, 0b0000, 0b1001, 0b101), // RW
    kDBGBVR0_EL1          = encode(0b10, 0b000, 0b0000, 0b0000, 0b100), // RW
    kDBGBVR10_EL1         = encode(0b10, 0b000, 0b0000, 0b1010, 0b100), // RW
    kDBGBVR11_EL1         = encode(0b10, 0b000, 0b0000, 0b1011, 0b100), // RW
    kDBGBVR12_EL1         = encode(0b10, 0b000, 0b0000, 0b1100, 0b100), // RW
    kDBGBVR13_EL1         = encode(0b10, 0b000, 0b0000, 0b1101, 0b100), // RW
    kDBGBVR14_EL1         = encode(0b10, 0b000, 0b0000, 0b1110, 0b100), // RW
    kDBGBVR15_EL1         = encode(0b10, 0b000, 0b0000, 0b1111, 0b100), // RW
    kDBGBVR1_EL1          = encode(0b10, 0b000, 0b0000, 0b0001, 0b100), // RW
    kDBGBVR2_EL1          = encode(0b10, 0b000, 0b0000, 0b0010, 0b100), // RW
    kDBGBVR3_EL1          = encode(0b10, 0b000, 0b0000, 0b0011, 0b100), // RW
    kDBGBVR4_EL1          = encode(0b10, 0b000, 0b0000, 0b0100, 0b100), // RW
    kDBGBVR5_EL1          = encode(0b10, 0b000, 0b0000, 0b0101, 0b100), // RW
    kDBGBVR6_EL1          = encode(0b10, 0b000, 0b0000, 0b0110, 0b100), // RW
    kDBGBVR7_EL1          = encode(0b10, 0b000, 0b0000, 0b0111, 0b100), // RW
    kDBGBVR8_EL1          = encode(0b10, 0b000, 0b0000, 0b1000, 0b100), // RW
    kDBGBVR9_EL1          = encode(0b10, 0b000, 0b0000, 0b1001, 0b100), // RW
    kDBGCLAIMCLR_EL1      = encode(0b10, 0b000, 0b0111, 0b1001, 0b110), // RW
    kDBGCLAIMSET_EL1      = encode(0b10, 0b000, 0b0111, 0b1000, 0b110), // RW
    kDBGDTRRX_EL0         = encode(0b10, 0b011, 0b0000, 0b0101, 0b000), // RO
    kDBGDTRTX_EL0         = encode(0b10, 0b011, 0b0000, 0b0101, 0b000), // WO
    kDBGDTR_EL0           = encode(0b10, 0b011, 0b0000, 0b0100, 0b000), // RW
    kDBGPRCR_EL1          = encode(0b10, 0b000, 0b0001, 0b0100, 0b100), // RW
    kDBGVCR32_EL2         = encode(0b10, 0b100, 0b0000, 0b0111, 0b000), // RW
    kDBGWCR0_EL1          = encode(0b10, 0b000, 0b0000, 0b0000, 0b111), // RW
    kDBGWCR10_EL1         = encode(0b10, 0b000, 0b0000, 0b1010, 0b111), // RW
    kDBGWCR11_EL1         = encode(0b10, 0b000, 0b0000, 0b1011, 0b111), // RW
    kDBGWCR12_EL1         = encode(0b10, 0b000, 0b0000, 0b1100, 0b111), // RW
    kDBGWCR13_EL1         = encode(0b10, 0b000, 0b0000, 0b1101, 0b111), // RW
    kDBGWCR14_EL1         = encode(0b10, 0b000, 0b0000, 0b1110, 0b111), // RW
    kDBGWCR15_EL1         = encode(0b10, 0b000, 0b0000, 0b1111, 0b111), // RW
    kDBGWCR1_EL1          = encode(0b10, 0b000, 0b0000, 0b0001, 0b111), // RW
    kDBGWCR2_EL1          = encode(0b10, 0b000, 0b0000, 0b0010, 0b111), // RW
    kDBGWCR3_EL1          = encode(0b10, 0b000, 0b0000, 0b0011, 0b111), // RW
    kDBGWCR4_EL1          = encode(0b10, 0b000, 0b0000, 0b0100, 0b111), // RW
    kDBGWCR5_EL1          = encode(0b10, 0b000, 0b0000, 0b0101, 0b111), // RW
    kDBGWCR6_EL1          = encode(0b10, 0b000, 0b0000, 0b0110, 0b111), // RW
    kDBGWCR7_EL1          = encode(0b10, 0b000, 0b0000, 0b0111, 0b111), // RW
    kDBGWCR8_EL1          = encode(0b10, 0b000, 0b0000, 0b1000, 0b111), // RW
    kDBGWCR9_EL1          = encode(0b10, 0b000, 0b0000, 0b1001, 0b111), // RW
    kDBGWVR0_EL1          = encode(0b10, 0b000, 0b0000, 0b0000, 0b110), // RW
    kDBGWVR10_EL1         = encode(0b10, 0b000, 0b0000, 0b1010, 0b110), // RW
    kDBGWVR11_EL1         = encode(0b10, 0b000, 0b0000, 0b1011, 0b110), // RW
    kDBGWVR12_EL1         = encode(0b10, 0b000, 0b0000, 0b1100, 0b110), // RW
    kDBGWVR13_EL1         = encode(0b10, 0b000, 0b0000, 0b1101, 0b110), // RW
    kDBGWVR14_EL1         = encode(0b10, 0b000, 0b0000, 0b1110, 0b110), // RW
    kDBGWVR15_EL1         = encode(0b10, 0b000, 0b0000, 0b1111, 0b110), // RW
    kDBGWVR1_EL1          = encode(0b10, 0b000, 0b0000, 0b0001, 0b110), // RW
    kDBGWVR2_EL1          = encode(0b10, 0b000, 0b0000, 0b0010, 0b110), // RW
    kDBGWVR3_EL1          = encode(0b10, 0b000, 0b0000, 0b0011, 0b110), // RW
    kDBGWVR4_EL1          = encode(0b10, 0b000, 0b0000, 0b0100, 0b110), // RW
    kDBGWVR5_EL1          = encode(0b10, 0b000, 0b0000, 0b0101, 0b110), // RW
    kDBGWVR6_EL1          = encode(0b10, 0b000, 0b0000, 0b0110, 0b110), // RW
    kDBGWVR7_EL1          = encode(0b10, 0b000, 0b0000, 0b0111, 0b110), // RW
    kDBGWVR8_EL1          = encode(0b10, 0b000, 0b0000, 0b1000, 0b110), // RW
    kDBGWVR9_EL1          = encode(0b10, 0b000, 0b0000, 0b1001, 0b110), // RW
    kDCZID_EL0            = encode(0b11, 0b011, 0b0000, 0b0000, 0b111), // RO
    kDISR_EL1             = encode(0b11, 0b000, 0b1100, 0b0001, 0b001), // RW
    kDIT                  = encode(0b11, 0b011, 0b0100, 0b0010, 0b101), // RW
    kDLR_EL0              = encode(0b11, 0b011, 0b0100, 0b0101, 0b001), // RW
    kDSPSR_EL0            = encode(0b11, 0b011, 0b0100, 0b0101, 0b000), // RW
    kELR_EL1              = encode(0b11, 0b000, 0b0100, 0b0000, 0b001), // RW
    kELR_EL12             = encode(0b11, 0b101, 0b0100, 0b0000, 0b001), // RW
    kELR_EL2              = encode(0b11, 0b100, 0b0100, 0b0000, 0b001), // RW
    kELR_EL3              = encode(0b11, 0b110, 0b0100, 0b0000, 0b001), // RW
    kERRIDR_EL1           = encode(0b11, 0b000, 0b0101, 0b0011, 0b000), // RO
    kERRSELR_EL1          = encode(0b11, 0b000, 0b0101, 0b0011, 0b001), // RW
    kERXADDR_EL1          = encode(0b11, 0b000, 0b0101, 0b0100, 0b011), // RW
    kERXCTLR_EL1          = encode(0b11, 0b000, 0b0101, 0b0100, 0b001), // RW
    kERXFR_EL1            = encode(0b11, 0b000, 0b0101, 0b0100, 0b000), // RO
    kERXMISC0_EL1         = encode(0b11, 0b000, 0b0101, 0b0101, 0b000), // RW
    kERXMISC1_EL1         = encode(0b11, 0b000, 0b0101, 0b0101, 0b001), // RW
    kERXMISC2_EL1         = encode(0b11, 0b000, 0b0101, 0b0101, 0b010), // RW
    kERXMISC3_EL1         = encode(0b11, 0b000, 0b0101, 0b0101, 0b011), // RW
    kERXPFGCDN_EL1        = encode(0b11, 0b000, 0b0101, 0b0100, 0b110), // RW
    kERXPFGCTL_EL1        = encode(0b11, 0b000, 0b0101, 0b0100, 0b101), // RW
    kERXPFGF_EL1          = encode(0b11, 0b000, 0b0101, 0b0100, 0b100), // RO
    kERXSTATUS_EL1        = encode(0b11, 0b000, 0b0101, 0b0100, 0b010), // RW
    kESR_EL1              = encode(0b11, 0b000, 0b0101, 0b0010, 0b000), // RW
    kESR_EL12             = encode(0b11, 0b101, 0b0101, 0b0010, 0b000), // RW
    kESR_EL2              = encode(0b11, 0b100, 0b0101, 0b0010, 0b000), // RW
    kESR_EL3              = encode(0b11, 0b110, 0b0101, 0b0010, 0b000), // RW
    kFAR_EL1              = encode(0b11, 0b000, 0b0110, 0b0000, 0b000), // RW
    kFAR_EL12             = encode(0b11, 0b101, 0b0110, 0b0000, 0b000), // RW
    kFAR_EL2              = encode(0b11, 0b100, 0b0110, 0b0000, 0b000), // RW
    kFAR_EL3              = encode(0b11, 0b110, 0b0110, 0b0000, 0b000), // RW
    kFPCR                 = encode(0b11, 0b011, 0b0100, 0b0100, 0b000), // RW
    kFPEXC32_EL2          = encode(0b11, 0b100, 0b0101, 0b0011, 0b000), // RW
    kFPSR                 = encode(0b11, 0b011, 0b0100, 0b0100, 0b001), // RW
    kGCR_EL1              = encode(0b11, 0b000, 0b0001, 0b0000, 0b110), // RW
    kGMID_EL1             = encode(0b11, 0b001, 0b0000, 0b0000, 0b100), // RO
    kHACR_EL2             = encode(0b11, 0b100, 0b0001, 0b0001, 0b111), // RW
    kHCR_EL2              = encode(0b11, 0b100, 0b0001, 0b0001, 0b000), // RW
    kHDFGRTR_EL2          = encode(0b11, 0b100, 0b0011, 0b0001, 0b100), // RW
    kHDFGWTR_EL2          = encode(0b11, 0b100, 0b0011, 0b0001, 0b101), // RW
    kHFGITR_EL2           = encode(0b11, 0b100, 0b0001, 0b0001, 0b110), // RW
    kHFGRTR_EL2           = encode(0b11, 0b100, 0b0001, 0b0001, 0b100), // RW
    kHFGWTR_EL2           = encode(0b11, 0b100, 0b0001, 0b0001, 0b101), // RW
    kHPFAR_EL2            = encode(0b11, 0b100, 0b0110, 0b0000, 0b100), // RW
    kHSTR_EL2             = encode(0b11, 0b100, 0b0001, 0b0001, 0b011), // RW
    kICC_AP0R0_EL1        = encode(0b11, 0b000, 0b1100, 0b1000, 0b100), // RW
    kICC_AP0R1_EL1        = encode(0b11, 0b000, 0b1100, 0b1000, 0b101), // RW
    kICC_AP0R2_EL1        = encode(0b11, 0b000, 0b1100, 0b1000, 0b110), // RW
    kICC_AP0R3_EL1        = encode(0b11, 0b000, 0b1100, 0b1000, 0b111), // RW
    kICC_AP1R0_EL1        = encode(0b11, 0b000, 0b1100, 0b1001, 0b000), // RW
    kICC_AP1R1_EL1        = encode(0b11, 0b000, 0b1100, 0b1001, 0b001), // RW
    kICC_AP1R2_EL1        = encode(0b11, 0b000, 0b1100, 0b1001, 0b010), // RW
    kICC_AP1R3_EL1        = encode(0b11, 0b000, 0b1100, 0b1001, 0b011), // RW
    kICC_ASGI1R_EL1       = encode(0b11, 0b000, 0b1100, 0b1011, 0b110), // WO
    kICC_BPR0_EL1         = encode(0b11, 0b000, 0b1100, 0b1000, 0b011), // RW
    kICC_BPR1_EL1         = encode(0b11, 0b000, 0b1100, 0b1100, 0b011), // RW
    kICC_CTLR_EL1         = encode(0b11, 0b000, 0b1100, 0b1100, 0b100), // RW
    kICC_CTLR_EL3         = encode(0b11, 0b110, 0b1100, 0b1100, 0b100), // RW
    kICC_DIR_EL1          = encode(0b11, 0b000, 0b1100, 0b1011, 0b001), // WO
    kICC_EOIR0_EL1        = encode(0b11, 0b000, 0b1100, 0b1000, 0b001), // WO
    kICC_EOIR1_EL1        = encode(0b11, 0b000, 0b1100, 0b1100, 0b001), // WO
    kICC_HPPIR0_EL1       = encode(0b11, 0b000, 0b1100, 0b1000, 0b010), // RO
    kICC_HPPIR1_EL1       = encode(0b11, 0b000, 0b1100, 0b1100, 0b010), // RO
    kICC_IAR0_EL1         = encode(0b11, 0b000, 0b1100, 0b1000, 0b000), // RO
    kICC_IAR1_EL1         = encode(0b11, 0b000, 0b1100, 0b1100, 0b000), // RO
    kICC_IGRPEN0_EL1      = encode(0b11, 0b000, 0b1100, 0b1100, 0b110), // RW
    kICC_IGRPEN1_EL1      = encode(0b11, 0b000, 0b1100, 0b1100, 0b111), // RW
    kICC_IGRPEN1_EL3      = encode(0b11, 0b110, 0b1100, 0b1100, 0b111), // RW
    kICC_PMR_EL1          = encode(0b11, 0b000, 0b0100, 0b0110, 0b000), // RW
    kICC_RPR_EL1          = encode(0b11, 0b000, 0b1100, 0b1011, 0b011), // RO
    kICC_SGI0R_EL1        = encode(0b11, 0b000, 0b1100, 0b1011, 0b111), // WO
    kICC_SGI1R_EL1        = encode(0b11, 0b000, 0b1100, 0b1011, 0b101), // WO
    kICC_SRE_EL1          = encode(0b11, 0b000, 0b1100, 0b1100, 0b101), // RW
    kICC_SRE_EL2          = encode(0b11, 0b100, 0b1100, 0b1001, 0b101), // RW
    kICC_SRE_EL3          = encode(0b11, 0b110, 0b1100, 0b1100, 0b101), // RW
    kICH_AP0R0_EL2        = encode(0b11, 0b100, 0b1100, 0b1000, 0b000), // RW
    kICH_AP0R1_EL2        = encode(0b11, 0b100, 0b1100, 0b1000, 0b001), // RW
    kICH_AP0R2_EL2        = encode(0b11, 0b100, 0b1100, 0b1000, 0b010), // RW
    kICH_AP0R3_EL2        = encode(0b11, 0b100, 0b1100, 0b1000, 0b011), // RW
    kICH_AP1R0_EL2        = encode(0b11, 0b100, 0b1100, 0b1001, 0b000), // RW
    kICH_AP1R1_EL2        = encode(0b11, 0b100, 0b1100, 0b1001, 0b001), // RW
    kICH_AP1R2_EL2        = encode(0b11, 0b100, 0b1100, 0b1001, 0b010), // RW
    kICH_AP1R3_EL2        = encode(0b11, 0b100, 0b1100, 0b1001, 0b011), // RW
    kICH_EISR_EL2         = encode(0b11, 0b100, 0b1100, 0b1011, 0b011), // RO
    kICH_ELRSR_EL2        = encode(0b11, 0b100, 0b1100, 0b1011, 0b101), // RO
    kICH_HCR_EL2          = encode(0b11, 0b100, 0b1100, 0b1011, 0b000), // RW
    kICH_LR0_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b000), // RW
    kICH_LR10_EL2         = encode(0b11, 0b100, 0b1100, 0b1101, 0b010), // RW
    kICH_LR11_EL2         = encode(0b11, 0b100, 0b1100, 0b1101, 0b011), // RW
    kICH_LR12_EL2         = encode(0b11, 0b100, 0b1100, 0b1101, 0b100), // RW
    kICH_LR13_EL2         = encode(0b11, 0b100, 0b1100, 0b1101, 0b101), // RW
    kICH_LR14_EL2         = encode(0b11, 0b100, 0b1100, 0b1101, 0b110), // RW
    kICH_LR15_EL2         = encode(0b11, 0b100, 0b1100, 0b1101, 0b111), // RW
    kICH_LR1_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b001), // RW
    kICH_LR2_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b010), // RW
    kICH_LR3_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b011), // RW
    kICH_LR4_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b100), // RW
    kICH_LR5_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b101), // RW
    kICH_LR6_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b110), // RW
    kICH_LR7_EL2          = encode(0b11, 0b100, 0b1100, 0b1100, 0b111), // RW
    kICH_LR8_EL2          = encode(0b11, 0b100, 0b1100, 0b1101, 0b000), // RW
    kICH_LR9_EL2          = encode(0b11, 0b100, 0b1100, 0b1101, 0b001), // RW
    kICH_MISR_EL2         = encode(0b11, 0b100, 0b1100, 0b1011, 0b010), // RO
    kICH_VMCR_EL2         = encode(0b11, 0b100, 0b1100, 0b1011, 0b111), // RW
    kICH_VTR_EL2          = encode(0b11, 0b100, 0b1100, 0b1011, 0b001), // RO
    kID_AA64AFR0_EL1      = encode(0b11, 0b000, 0b0000, 0b0101, 0b100), // RO
    kID_AA64AFR1_EL1      = encode(0b11, 0b000, 0b0000, 0b0101, 0b101), // RO
    kID_AA64DFR0_EL1      = encode(0b11, 0b000, 0b0000, 0b0101, 0b000), // RO
    kID_AA64DFR1_EL1      = encode(0b11, 0b000, 0b0000, 0b0101, 0b001), // RO
    kID_AA64ISAR0_EL1     = encode(0b11, 0b000, 0b0000, 0b0110, 0b000), // RO
    kID_AA64ISAR1_EL1     = encode(0b11, 0b000, 0b0000, 0b0110, 0b001), // RO
    kID_AA64MMFR0_EL1     = encode(0b11, 0b000, 0b0000, 0b0111, 0b000), // RO
    kID_AA64MMFR1_EL1     = encode(0b11, 0b000, 0b0000, 0b0111, 0b001), // RO
    kID_AA64MMFR2_EL1     = encode(0b11, 0b000, 0b0000, 0b0111, 0b010), // RO
    kID_AA64PFR0_EL1      = encode(0b11, 0b000, 0b0000, 0b0100, 0b000), // RO
    kID_AA64PFR1_EL1      = encode(0b11, 0b000, 0b0000, 0b0100, 0b001), // RO
    kID_AA64ZFR0_EL1      = encode(0b11, 0b000, 0b0000, 0b0100, 0b100), // RO
    kID_AFR0_EL1          = encode(0b11, 0b000, 0b0000, 0b0001, 0b011), // RO
    kID_DFR0_EL1          = encode(0b11, 0b000, 0b0000, 0b0001, 0b010), // RO
    kID_ISAR0_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b000), // RO
    kID_ISAR1_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b001), // RO
    kID_ISAR2_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b010), // RO
    kID_ISAR3_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b011), // RO
    kID_ISAR4_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b100), // RO
    kID_ISAR5_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b101), // RO
    kID_ISAR6_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b111), // RO
    kID_MMFR0_EL1         = encode(0b11, 0b000, 0b0000, 0b0001, 0b100), // RO
    kID_MMFR1_EL1         = encode(0b11, 0b000, 0b0000, 0b0001, 0b101), // RO
    kID_MMFR2_EL1         = encode(0b11, 0b000, 0b0000, 0b0001, 0b110), // RO
    kID_MMFR3_EL1         = encode(0b11, 0b000, 0b0000, 0b0001, 0b111), // RO
    kID_MMFR4_EL1         = encode(0b11, 0b000, 0b0000, 0b0010, 0b110), // RO
    kID_MMFR5_EL1         = encode(0b11, 0b000, 0b0000, 0b0011, 0b110), // RO
    kID_PFR0_EL1          = encode(0b11, 0b000, 0b0000, 0b0001, 0b000), // RO
    kID_PFR1_EL1          = encode(0b11, 0b000, 0b0000, 0b0001, 0b001), // RO
    kID_PFR2_EL1          = encode(0b11, 0b000, 0b0000, 0b0011, 0b100), // RO
    kIFSR32_EL2           = encode(0b11, 0b100, 0b0101, 0b0000, 0b001), // RW
    kISR_EL1              = encode(0b11, 0b000, 0b1100, 0b0001, 0b000), // RO
    kLORC_EL1             = encode(0b11, 0b000, 0b1010, 0b0100, 0b011), // RW
    kLOREA_EL1            = encode(0b11, 0b000, 0b1010, 0b0100, 0b001), // RW
    kLORID_EL1            = encode(0b11, 0b000, 0b1010, 0b0100, 0b111), // RO
    kLORN_EL1             = encode(0b11, 0b000, 0b1010, 0b0100, 0b010), // RW
    kLORSA_EL1            = encode(0b11, 0b000, 0b1010, 0b0100, 0b000), // RW
    kMAIR_EL1             = encode(0b11, 0b000, 0b1010, 0b0010, 0b000), // RW
    kMAIR_EL12            = encode(0b11, 0b101, 0b1010, 0b0010, 0b000), // RW
    kMAIR_EL2             = encode(0b11, 0b100, 0b1010, 0b0010, 0b000), // RW
    kMAIR_EL3             = encode(0b11, 0b110, 0b1010, 0b0010, 0b000), // RW
    kMDCCINT_EL1          = encode(0b10, 0b000, 0b0000, 0b0010, 0b000), // RW
    kMDCCSR_EL0           = encode(0b10, 0b011, 0b0000, 0b0001, 0b000), // RO
    kMDCR_EL2             = encode(0b11, 0b100, 0b0001, 0b0001, 0b001), // RW
    kMDCR_EL3             = encode(0b11, 0b110, 0b0001, 0b0011, 0b001), // RW
    kMDRAR_EL1            = encode(0b10, 0b000, 0b0001, 0b0000, 0b000), // RO
    kMDSCR_EL1            = encode(0b10, 0b000, 0b0000, 0b0010, 0b010), // RW
    kMIDR_EL1             = encode(0b11, 0b000, 0b0000, 0b0000, 0b000), // RO
    kMPAM0_EL1            = encode(0b11, 0b000, 0b1010, 0b0101, 0b001), // RW
    kMPAM1_EL1            = encode(0b11, 0b000, 0b1010, 0b0101, 0b000), // RW
    kMPAM1_EL12           = encode(0b11, 0b101, 0b1010, 0b0101, 0b000), // RW
    kMPAM2_EL2            = encode(0b11, 0b100, 0b1010, 0b0101, 0b000), // RW
    kMPAM3_EL3            = encode(0b11, 0b110, 0b1010, 0b0101, 0b000), // RW
    kMPAMHCR_EL2          = encode(0b11, 0b100, 0b1010, 0b0100, 0b000), // RW
    kMPAMIDR_EL1          = encode(0b11, 0b000, 0b1010, 0b0100, 0b100), // RO
    kMPAMVPM0_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b000), // RW
    kMPAMVPM1_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b001), // RW
    kMPAMVPM2_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b010), // RW
    kMPAMVPM3_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b011), // RW
    kMPAMVPM4_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b100), // RW
    kMPAMVPM5_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b101), // RW
    kMPAMVPM6_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b110), // RW
    kMPAMVPM7_EL2         = encode(0b11, 0b100, 0b1010, 0b0110, 0b111), // RW
    kMPAMVPMV_EL2         = encode(0b11, 0b100, 0b1010, 0b0100, 0b001), // RW
    kMPIDR_EL1            = encode(0b11, 0b000, 0b0000, 0b0000, 0b101), // RO
    kMVFR0_EL1            = encode(0b11, 0b000, 0b0000, 0b0011, 0b000), // RO
    kMVFR1_EL1            = encode(0b11, 0b000, 0b0000, 0b0011, 0b001), // RO
    kMVFR2_EL1            = encode(0b11, 0b000, 0b0000, 0b0011, 0b010), // RO
    kNZCV                 = encode(0b11, 0b011, 0b0100, 0b0010, 0b000), // RW
    kOSDLR_EL1            = encode(0b10, 0b000, 0b0001, 0b0011, 0b100), // RW
    kOSDTRRX_EL1          = encode(0b10, 0b000, 0b0000, 0b0000, 0b010), // RW
    kOSDTRTX_EL1          = encode(0b10, 0b000, 0b0000, 0b0011, 0b010), // RW
    kOSECCR_EL1           = encode(0b10, 0b000, 0b0000, 0b0110, 0b010), // RW
    kOSLAR_EL1            = encode(0b10, 0b000, 0b0001, 0b0000, 0b100), // WO
    kOSLSR_EL1            = encode(0b10, 0b000, 0b0001, 0b0001, 0b100), // RO
    kPAN                  = encode(0b11, 0b000, 0b0100, 0b0010, 0b011), // RW
    kPAR_EL1              = encode(0b11, 0b000, 0b0111, 0b0100, 0b000), // RW
    kPMBIDR_EL1           = encode(0b11, 0b000, 0b1001, 0b1010, 0b111), // RO
    kPMBLIMITR_EL1        = encode(0b11, 0b000, 0b1001, 0b1010, 0b000), // RW
    kPMBPTR_EL1           = encode(0b11, 0b000, 0b1001, 0b1010, 0b001), // RW
    kPMBSR_EL1            = encode(0b11, 0b000, 0b1001, 0b1010, 0b011), // RW
    kPMCCFILTR_EL0        = encode(0b11, 0b011, 0b1110, 0b1111, 0b111), // RW
    kPMCCNTR_EL0          = encode(0b11, 0b011, 0b1001, 0b1101, 0b000), // RW
    kPMCEID0_EL0          = encode(0b11, 0b011, 0b1001, 0b1100, 0b110), // RO
    kPMCEID1_EL0          = encode(0b11, 0b011, 0b1001, 0b1100, 0b111), // RO
    kPMCNTENCLR_EL0       = encode(0b11, 0b011, 0b1001, 0b1100, 0b010), // RW
    kPMCNTENSET_EL0       = encode(0b11, 0b011, 0b1001, 0b1100, 0b001), // RW
    kPMCR_EL0             = encode(0b11, 0b011, 0b1001, 0b1100, 0b000), // RW
    kPMEVCNTR0_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b000), // RW
    kPMEVCNTR10_EL0       = encode(0b11, 0b011, 0b1110, 0b1001, 0b010), // RW
    kPMEVCNTR11_EL0       = encode(0b11, 0b011, 0b1110, 0b1001, 0b011), // RW
    kPMEVCNTR12_EL0       = encode(0b11, 0b011, 0b1110, 0b1001, 0b100), // RW
    kPMEVCNTR13_EL0       = encode(0b11, 0b011, 0b1110, 0b1001, 0b101), // RW
    kPMEVCNTR14_EL0       = encode(0b11, 0b011, 0b1110, 0b1001, 0b110), // RW
    kPMEVCNTR15_EL0       = encode(0b11, 0b011, 0b1110, 0b1001, 0b111), // RW
    kPMEVCNTR16_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b000), // RW
    kPMEVCNTR17_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b001), // RW
    kPMEVCNTR18_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b010), // RW
    kPMEVCNTR19_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b011), // RW
    kPMEVCNTR1_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b001), // RW
    kPMEVCNTR20_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b100), // RW
    kPMEVCNTR21_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b101), // RW
    kPMEVCNTR22_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b110), // RW
    kPMEVCNTR23_EL0       = encode(0b11, 0b011, 0b1110, 0b1010, 0b111), // RW
    kPMEVCNTR24_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b000), // RW
    kPMEVCNTR25_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b001), // RW
    kPMEVCNTR26_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b010), // RW
    kPMEVCNTR27_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b011), // RW
    kPMEVCNTR28_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b100), // RW
    kPMEVCNTR29_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b101), // RW
    kPMEVCNTR2_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b010), // RW
    kPMEVCNTR30_EL0       = encode(0b11, 0b011, 0b1110, 0b1011, 0b110), // RW
    kPMEVCNTR3_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b011), // RW
    kPMEVCNTR4_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b100), // RW
    kPMEVCNTR5_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b101), // RW
    kPMEVCNTR6_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b110), // RW
    kPMEVCNTR7_EL0        = encode(0b11, 0b011, 0b1110, 0b1000, 0b111), // RW
    kPMEVCNTR8_EL0        = encode(0b11, 0b011, 0b1110, 0b1001, 0b000), // RW
    kPMEVCNTR9_EL0        = encode(0b11, 0b011, 0b1110, 0b1001, 0b001), // RW
    kPMEVTYPER0_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b000), // RW
    kPMEVTYPER10_EL0      = encode(0b11, 0b011, 0b1110, 0b1101, 0b010), // RW
    kPMEVTYPER11_EL0      = encode(0b11, 0b011, 0b1110, 0b1101, 0b011), // RW
    kPMEVTYPER12_EL0      = encode(0b11, 0b011, 0b1110, 0b1101, 0b100), // RW
    kPMEVTYPER13_EL0      = encode(0b11, 0b011, 0b1110, 0b1101, 0b101), // RW
    kPMEVTYPER14_EL0      = encode(0b11, 0b011, 0b1110, 0b1101, 0b110), // RW
    kPMEVTYPER15_EL0      = encode(0b11, 0b011, 0b1110, 0b1101, 0b111), // RW
    kPMEVTYPER16_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b000), // RW
    kPMEVTYPER17_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b001), // RW
    kPMEVTYPER18_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b010), // RW
    kPMEVTYPER19_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b011), // RW
    kPMEVTYPER1_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b001), // RW
    kPMEVTYPER20_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b100), // RW
    kPMEVTYPER21_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b101), // RW
    kPMEVTYPER22_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b110), // RW
    kPMEVTYPER23_EL0      = encode(0b11, 0b011, 0b1110, 0b1110, 0b111), // RW
    kPMEVTYPER24_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b000), // RW
    kPMEVTYPER25_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b001), // RW
    kPMEVTYPER26_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b010), // RW
    kPMEVTYPER27_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b011), // RW
    kPMEVTYPER28_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b100), // RW
    kPMEVTYPER29_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b101), // RW
    kPMEVTYPER2_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b010), // RW
    kPMEVTYPER30_EL0      = encode(0b11, 0b011, 0b1110, 0b1111, 0b110), // RW
    kPMEVTYPER3_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b011), // RW
    kPMEVTYPER4_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b100), // RW
    kPMEVTYPER5_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b101), // RW
    kPMEVTYPER6_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b110), // RW
    kPMEVTYPER7_EL0       = encode(0b11, 0b011, 0b1110, 0b1100, 0b111), // RW
    kPMEVTYPER8_EL0       = encode(0b11, 0b011, 0b1110, 0b1101, 0b000), // RW
    kPMEVTYPER9_EL0       = encode(0b11, 0b011, 0b1110, 0b1101, 0b001), // RW
    kPMINTENCLR_EL1       = encode(0b11, 0b000, 0b1001, 0b1110, 0b010), // RW
    kPMINTENSET_EL1       = encode(0b11, 0b000, 0b1001, 0b1110, 0b001), // RW
    kPMMIR_EL1            = encode(0b11, 0b000, 0b1001, 0b1110, 0b110), // RW
    kPMOVSCLR_EL0         = encode(0b11, 0b011, 0b1001, 0b1100, 0b011), // RW
    kPMOVSSET_EL0         = encode(0b11, 0b011, 0b1001, 0b1110, 0b011), // RW
    kPMSCR_EL1            = encode(0b11, 0b000, 0b1001, 0b1001, 0b000), // RW
    kPMSCR_EL12           = encode(0b11, 0b101, 0b1001, 0b1001, 0b000), // RW
    kPMSCR_EL2            = encode(0b11, 0b100, 0b1001, 0b1001, 0b000), // RW
    kPMSELR_EL0           = encode(0b11, 0b011, 0b1001, 0b1100, 0b101), // RW
    kPMSEVFR_EL1          = encode(0b11, 0b000, 0b1001, 0b1001, 0b101), // RW
    kPMSFCR_EL1           = encode(0b11, 0b000, 0b1001, 0b1001, 0b100), // RW
    kPMSICR_EL1           = encode(0b11, 0b000, 0b1001, 0b1001, 0b010), // RW
    kPMSIDR_EL1           = encode(0b11, 0b000, 0b1001, 0b1001, 0b111), // RO
    kPMSIRR_EL1           = encode(0b11, 0b000, 0b1001, 0b1001, 0b011), // RW
    kPMSLATFR_EL1         = encode(0b11, 0b000, 0b1001, 0b1001, 0b110), // RW
    kPMSWINC_EL0          = encode(0b11, 0b011, 0b1001, 0b1100, 0b100), // WO
    kPMUSERENR_EL0        = encode(0b11, 0b011, 0b1001, 0b1110, 0b000), // RW
    kPMXEVCNTR_EL0        = encode(0b11, 0b011, 0b1001, 0b1101, 0b010), // RW
    kPMXEVTYPER_EL0       = encode(0b11, 0b011, 0b1001, 0b1101, 0b001), // RW
    kREVIDR_EL1           = encode(0b11, 0b000, 0b0000, 0b0000, 0b110), // RO
    kRGSR_EL1             = encode(0b11, 0b000, 0b0001, 0b0000, 0b101), // RW
    kRMR_EL1              = encode(0b11, 0b000, 0b1100, 0b0000, 0b010), // RW
    kRMR_EL2              = encode(0b11, 0b100, 0b1100, 0b0000, 0b010), // RW
    kRMR_EL3              = encode(0b11, 0b110, 0b1100, 0b0000, 0b010), // RW
    kRNDR                 = encode(0b11, 0b011, 0b0010, 0b0100, 0b000), // RO
    kRNDRRS               = encode(0b11, 0b011, 0b0010, 0b0100, 0b001), // RO
    kRVBAR_EL1            = encode(0b11, 0b000, 0b1100, 0b0000, 0b001), // RO
    kRVBAR_EL2            = encode(0b11, 0b100, 0b1100, 0b0000, 0b001), // RO
    kRVBAR_EL3            = encode(0b11, 0b110, 0b1100, 0b0000, 0b001), // RO
    kSCR_EL3              = encode(0b11, 0b110, 0b0001, 0b0001, 0b000), // RW
    kSCTLR_EL1            = encode(0b11, 0b000, 0b0001, 0b0000, 0b000), // RW
    kSCTLR_EL12           = encode(0b11, 0b101, 0b0001, 0b0000, 0b000), // RW
    kSCTLR_EL2            = encode(0b11, 0b100, 0b0001, 0b0000, 0b000), // RW
    kSCTLR_EL3            = encode(0b11, 0b110, 0b0001, 0b0000, 0b000), // RW
    kSCXTNUM_EL0          = encode(0b11, 0b011, 0b1101, 0b0000, 0b111), // RW
    kSCXTNUM_EL1          = encode(0b11, 0b000, 0b1101, 0b0000, 0b111), // RW
    kSCXTNUM_EL12         = encode(0b11, 0b101, 0b1101, 0b0000, 0b111), // RW
    kSCXTNUM_EL2          = encode(0b11, 0b100, 0b1101, 0b0000, 0b111), // RW
    kSCXTNUM_EL3          = encode(0b11, 0b110, 0b1101, 0b0000, 0b111), // RW
    kSDER32_EL2           = encode(0b11, 0b100, 0b0001, 0b0011, 0b001), // RW
    kSDER32_EL3           = encode(0b11, 0b110, 0b0001, 0b0001, 0b001), // RW
    kSPSR_EL1             = encode(0b11, 0b000, 0b0100, 0b0000, 0b000), // RW
    kSPSR_EL12            = encode(0b11, 0b101, 0b0100, 0b0000, 0b000), // RW
    kSPSR_EL2             = encode(0b11, 0b100, 0b0100, 0b0000, 0b000), // RW
    kSPSR_EL3             = encode(0b11, 0b110, 0b0100, 0b0000, 0b000), // RW
    kSPSR_abt             = encode(0b11, 0b100, 0b0100, 0b0011, 0b001), // RW
    kSPSR_fiq             = encode(0b11, 0b100, 0b0100, 0b0011, 0b011), // RW
    kSPSR_irq             = encode(0b11, 0b100, 0b0100, 0b0011, 0b000), // RW
    kSPSR_und             = encode(0b11, 0b100, 0b0100, 0b0011, 0b010), // RW
    kSPSel                = encode(0b11, 0b000, 0b0100, 0b0010, 0b000), // RW
    kSP_EL0               = encode(0b11, 0b000, 0b0100, 0b0001, 0b000), // RW
    kSP_EL1               = encode(0b11, 0b100, 0b0100, 0b0001, 0b000), // RW
    kSP_EL2               = encode(0b11, 0b110, 0b0100, 0b0001, 0b000), // RW
    kSSBS                 = encode(0b11, 0b011, 0b0100, 0b0010, 0b110), // RW
    kTCO                  = encode(0b11, 0b011, 0b0100, 0b0010, 0b111), // RW
    kTCR_EL1              = encode(0b11, 0b000, 0b0010, 0b0000, 0b010), // RW
    kTCR_EL12             = encode(0b11, 0b101, 0b0010, 0b0000, 0b010), // RW
    kTCR_EL2              = encode(0b11, 0b100, 0b0010, 0b0000, 0b010), // RW
    kTCR_EL3              = encode(0b11, 0b110, 0b0010, 0b0000, 0b010), // RW
    kTEECR32_EL1          = encode(0b10, 0b010, 0b0000, 0b0000, 0b000), // RW
    kTEEHBR32_EL1         = encode(0b10, 0b010, 0b0001, 0b0000, 0b000), // RW
    kTFSRE0_EL1           = encode(0b11, 0b000, 0b0101, 0b0110, 0b001), // RW
    kTFSR_EL1             = encode(0b11, 0b000, 0b0101, 0b0110, 0b000), // RW
    kTFSR_EL12            = encode(0b11, 0b101, 0b0101, 0b0110, 0b000), // RW
    kTFSR_EL2             = encode(0b11, 0b100, 0b0101, 0b0110, 0b000), // RW
    kTFSR_EL3             = encode(0b11, 0b110, 0b0101, 0b0110, 0b000), // RW
    kTPIDRRO_EL0          = encode(0b11, 0b011, 0b1101, 0b0000, 0b011), // RW
    kTPIDR_EL0            = encode(0b11, 0b011, 0b1101, 0b0000, 0b010), // RW
    kTPIDR_EL1            = encode(0b11, 0b000, 0b1101, 0b0000, 0b100), // RW
    kTPIDR_EL2            = encode(0b11, 0b100, 0b1101, 0b0000, 0b010), // RW
    kTPIDR_EL3            = encode(0b11, 0b110, 0b1101, 0b0000, 0b010), // RW
    kTRBBASER_EL1         = encode(0b11, 0b000, 0b1001, 0b1011, 0b010), // RW
    kTRBIDR_EL1           = encode(0b11, 0b000, 0b1001, 0b1011, 0b111), // RO
    kTRBLIMITR_EL1        = encode(0b11, 0b000, 0b1001, 0b1011, 0b000), // RW
    kTRBMAR_EL1           = encode(0b11, 0b000, 0b1001, 0b1011, 0b100), // RW
    kTRBPTR_EL1           = encode(0b11, 0b000, 0b1001, 0b1011, 0b001), // RW
    kTRBSR_EL1            = encode(0b11, 0b000, 0b1001, 0b1011, 0b011), // RW
    kTRBTRG_EL1           = encode(0b11, 0b000, 0b1001, 0b1011, 0b110), // RW
    kTRCACATR0            = encode(0b10, 0b001, 0b0010, 0b0000, 0b010), // RW
    kTRCACATR1            = encode(0b10, 0b001, 0b0010, 0b0010, 0b010), // RW
    kTRCACATR10           = encode(0b10, 0b001, 0b0010, 0b0100, 0b011), // RW
    kTRCACATR11           = encode(0b10, 0b001, 0b0010, 0b0110, 0b011), // RW
    kTRCACATR12           = encode(0b10, 0b001, 0b0010, 0b1000, 0b011), // RW
    kTRCACATR13           = encode(0b10, 0b001, 0b0010, 0b1010, 0b011), // RW
    kTRCACATR14           = encode(0b10, 0b001, 0b0010, 0b1100, 0b011), // RW
    kTRCACATR15           = encode(0b10, 0b001, 0b0010, 0b1110, 0b011), // RW
    kTRCACATR2            = encode(0b10, 0b001, 0b0010, 0b0100, 0b010), // RW
    kTRCACATR3            = encode(0b10, 0b001, 0b0010, 0b0110, 0b010), // RW
    kTRCACATR4            = encode(0b10, 0b001, 0b0010, 0b1000, 0b010), // RW
    kTRCACATR5            = encode(0b10, 0b001, 0b0010, 0b1010, 0b010), // RW
    kTRCACATR6            = encode(0b10, 0b001, 0b0010, 0b1100, 0b010), // RW
    kTRCACATR7            = encode(0b10, 0b001, 0b0010, 0b1110, 0b010), // RW
    kTRCACATR8            = encode(0b10, 0b001, 0b0010, 0b0000, 0b011), // RW
    kTRCACATR9            = encode(0b10, 0b001, 0b0010, 0b0010, 0b011), // RW
    kTRCACVR0             = encode(0b10, 0b001, 0b0010, 0b0000, 0b000), // RW
    kTRCACVR1             = encode(0b10, 0b001, 0b0010, 0b0010, 0b000), // RW
    kTRCACVR10            = encode(0b10, 0b001, 0b0010, 0b0100, 0b001), // RW
    kTRCACVR11            = encode(0b10, 0b001, 0b0010, 0b0110, 0b001), // RW
    kTRCACVR12            = encode(0b10, 0b001, 0b0010, 0b1000, 0b001), // RW
    kTRCACVR13            = encode(0b10, 0b001, 0b0010, 0b1010, 0b001), // RW
    kTRCACVR14            = encode(0b10, 0b001, 0b0010, 0b1100, 0b001), // RW
    kTRCACVR15            = encode(0b10, 0b001, 0b0010, 0b1110, 0b001), // RW
    kTRCACVR2             = encode(0b10, 0b001, 0b0010, 0b0100, 0b000), // RW
    kTRCACVR3             = encode(0b10, 0b001, 0b0010, 0b0110, 0b000), // RW
    kTRCACVR4             = encode(0b10, 0b001, 0b0010, 0b1000, 0b000), // RW
    kTRCACVR5             = encode(0b10, 0b001, 0b0010, 0b1010, 0b000), // RW
    kTRCACVR6             = encode(0b10, 0b001, 0b0010, 0b1100, 0b000), // RW
    kTRCACVR7             = encode(0b10, 0b001, 0b0010, 0b1110, 0b000), // RW
    kTRCACVR8             = encode(0b10, 0b001, 0b0010, 0b0000, 0b001), // RW
    kTRCACVR9             = encode(0b10, 0b001, 0b0010, 0b0010, 0b001), // RW
    kTRCAUTHSTATUS        = encode(0b10, 0b001, 0b0111, 0b1110, 0b110), // RO
    kTRCAUXCTLR           = encode(0b10, 0b001, 0b0000, 0b0110, 0b000), // RW
    kTRCBBCTLR            = encode(0b10, 0b001, 0b0000, 0b1111, 0b000), // RW
    kTRCCCCTLR            = encode(0b10, 0b001, 0b0000, 0b1110, 0b000), // RW
    kTRCCIDCCTLR0         = encode(0b10, 0b001, 0b0011, 0b0000, 0b010), // RW
    kTRCCIDCCTLR1         = encode(0b10, 0b001, 0b0011, 0b0001, 0b010), // RW
    kTRCCIDCVR0           = encode(0b10, 0b001, 0b0011, 0b0000, 0b000), // RW
    kTRCCIDCVR1           = encode(0b10, 0b001, 0b0011, 0b0010, 0b000), // RW
    kTRCCIDCVR2           = encode(0b10, 0b001, 0b0011, 0b0100, 0b000), // RW
    kTRCCIDCVR3           = encode(0b10, 0b001, 0b0011, 0b0110, 0b000), // RW
    kTRCCIDCVR4           = encode(0b10, 0b001, 0b0011, 0b1000, 0b000), // RW
    kTRCCIDCVR5           = encode(0b10, 0b001, 0b0011, 0b1010, 0b000), // RW
    kTRCCIDCVR6           = encode(0b10, 0b001, 0b0011, 0b1100, 0b000), // RW
    kTRCCIDCVR7           = encode(0b10, 0b001, 0b0011, 0b1110, 0b000), // RW
    kTRCCIDR0             = encode(0b10, 0b001, 0b0111, 0b1100, 0b111), // RO
    kTRCCIDR1             = encode(0b10, 0b001, 0b0111, 0b1101, 0b111), // RO
    kTRCCIDR2             = encode(0b10, 0b001, 0b0111, 0b1110, 0b111), // RO
    kTRCCIDR3             = encode(0b10, 0b001, 0b0111, 0b1111, 0b111), // RO
    kTRCCLAIMCLR          = encode(0b10, 0b001, 0b0111, 0b1001, 0b110), // RW
    kTRCCLAIMSET          = encode(0b10, 0b001, 0b0111, 0b1000, 0b110), // RW
    kTRCCNTCTLR0          = encode(0b10, 0b001, 0b0000, 0b0100, 0b101), // RW
    kTRCCNTCTLR1          = encode(0b10, 0b001, 0b0000, 0b0101, 0b101), // RW
    kTRCCNTCTLR2          = encode(0b10, 0b001, 0b0000, 0b0110, 0b101), // RW
    kTRCCNTCTLR3          = encode(0b10, 0b001, 0b0000, 0b0111, 0b101), // RW
    kTRCCNTRLDVR0         = encode(0b10, 0b001, 0b0000, 0b0000, 0b101), // RW
    kTRCCNTRLDVR1         = encode(0b10, 0b001, 0b0000, 0b0001, 0b101), // RW
    kTRCCNTRLDVR2         = encode(0b10, 0b001, 0b0000, 0b0010, 0b101), // RW
    kTRCCNTRLDVR3         = encode(0b10, 0b001, 0b0000, 0b0011, 0b101), // RW
    kTRCCNTVR0            = encode(0b10, 0b001, 0b0000, 0b1000, 0b101), // RW
    kTRCCNTVR1            = encode(0b10, 0b001, 0b0000, 0b1001, 0b101), // RW
    kTRCCNTVR2            = encode(0b10, 0b001, 0b0000, 0b1010, 0b101), // RW
    kTRCCNTVR3            = encode(0b10, 0b001, 0b0000, 0b1011, 0b101), // RW
    kTRCCONFIGR           = encode(0b10, 0b001, 0b0000, 0b0100, 0b000), // RW
    kTRCDEVAFF0           = encode(0b10, 0b001, 0b0111, 0b1010, 0b110), // RO
    kTRCDEVAFF1           = encode(0b10, 0b001, 0b0111, 0b1011, 0b110), // RO
    kTRCDEVARCH           = encode(0b10, 0b001, 0b0111, 0b1111, 0b110), // RO
    kTRCDEVID             = encode(0b10, 0b001, 0b0111, 0b0010, 0b111), // RO
    kTRCDEVTYPE           = encode(0b10, 0b001, 0b0111, 0b0011, 0b111), // RO
    kTRCDVCMR0            = encode(0b10, 0b001, 0b0010, 0b0000, 0b110), // RW
    kTRCDVCMR1            = encode(0b10, 0b001, 0b0010, 0b0100, 0b110), // RW
    kTRCDVCMR2            = encode(0b10, 0b001, 0b0010, 0b1000, 0b110), // RW
    kTRCDVCMR3            = encode(0b10, 0b001, 0b0010, 0b1100, 0b110), // RW
    kTRCDVCMR4            = encode(0b10, 0b001, 0b0010, 0b0000, 0b111), // RW
    kTRCDVCMR5            = encode(0b10, 0b001, 0b0010, 0b0100, 0b111), // RW
    kTRCDVCMR6            = encode(0b10, 0b001, 0b0010, 0b1000, 0b111), // RW
    kTRCDVCMR7            = encode(0b10, 0b001, 0b0010, 0b1100, 0b111), // RW
    kTRCDVCVR0            = encode(0b10, 0b001, 0b0010, 0b0000, 0b100), // RW
    kTRCDVCVR1            = encode(0b10, 0b001, 0b0010, 0b0100, 0b100), // RW
    kTRCDVCVR2            = encode(0b10, 0b001, 0b0010, 0b1000, 0b100), // RW
    kTRCDVCVR3            = encode(0b10, 0b001, 0b0010, 0b1100, 0b100), // RW
    kTRCDVCVR4            = encode(0b10, 0b001, 0b0010, 0b0000, 0b101), // RW
    kTRCDVCVR5            = encode(0b10, 0b001, 0b0010, 0b0100, 0b101), // RW
    kTRCDVCVR6            = encode(0b10, 0b001, 0b0010, 0b1000, 0b101), // RW
    kTRCDVCVR7            = encode(0b10, 0b001, 0b0010, 0b1100, 0b101), // RW
    kTRCEVENTCTL0R        = encode(0b10, 0b001, 0b0000, 0b1000, 0b000), // RW
    kTRCEVENTCTL1R        = encode(0b10, 0b001, 0b0000, 0b1001, 0b000), // RW
    kTRCEXTINSELR         = encode(0b10, 0b001, 0b0000, 0b1000, 0b100), // RW
    kTRCEXTINSELR0        = encode(0b10, 0b001, 0b0000, 0b1000, 0b100), // RW
    kTRCEXTINSELR1        = encode(0b10, 0b001, 0b0000, 0b1001, 0b100), // RW
    kTRCEXTINSELR2        = encode(0b10, 0b001, 0b0000, 0b1010, 0b100), // RW
    kTRCEXTINSELR3        = encode(0b10, 0b001, 0b0000, 0b1011, 0b100), // RW
    kTRCIDR0              = encode(0b10, 0b001, 0b0000, 0b1000, 0b111), // RO
    kTRCIDR1              = encode(0b10, 0b001, 0b0000, 0b1001, 0b111), // RO
    kTRCIDR10             = encode(0b10, 0b001, 0b0000, 0b0010, 0b110), // RO
    kTRCIDR11             = encode(0b10, 0b001, 0b0000, 0b0011, 0b110), // RO
    kTRCIDR12             = encode(0b10, 0b001, 0b0000, 0b0100, 0b110), // RO
    kTRCIDR13             = encode(0b10, 0b001, 0b0000, 0b0101, 0b110), // RO
    kTRCIDR2              = encode(0b10, 0b001, 0b0000, 0b1010, 0b111), // RO
    kTRCIDR3              = encode(0b10, 0b001, 0b0000, 0b1011, 0b111), // RO
    kTRCIDR4              = encode(0b10, 0b001, 0b0000, 0b1100, 0b111), // RO
    kTRCIDR5              = encode(0b10, 0b001, 0b0000, 0b1101, 0b111), // RO
    kTRCIDR6              = encode(0b10, 0b001, 0b0000, 0b1110, 0b111), // RO
    kTRCIDR7              = encode(0b10, 0b001, 0b0000, 0b1111, 0b111), // RO
    kTRCIDR8              = encode(0b10, 0b001, 0b0000, 0b0000, 0b110), // RO
    kTRCIDR9              = encode(0b10, 0b001, 0b0000, 0b0001, 0b110), // RO
    kTRCIMSPEC0           = encode(0b10, 0b001, 0b0000, 0b0000, 0b111), // RW
    kTRCIMSPEC1           = encode(0b10, 0b001, 0b0000, 0b0001, 0b111), // RW
    kTRCIMSPEC2           = encode(0b10, 0b001, 0b0000, 0b0010, 0b111), // RW
    kTRCIMSPEC3           = encode(0b10, 0b001, 0b0000, 0b0011, 0b111), // RW
    kTRCIMSPEC4           = encode(0b10, 0b001, 0b0000, 0b0100, 0b111), // RW
    kTRCIMSPEC5           = encode(0b10, 0b001, 0b0000, 0b0101, 0b111), // RW
    kTRCIMSPEC6           = encode(0b10, 0b001, 0b0000, 0b0110, 0b111), // RW
    kTRCIMSPEC7           = encode(0b10, 0b001, 0b0000, 0b0111, 0b111), // RW
    kTRCITCTRL            = encode(0b10, 0b001, 0b0111, 0b0000, 0b100), // RW
    kTRCLAR               = encode(0b10, 0b001, 0b0111, 0b1100, 0b110), // WO
    kTRCLSR               = encode(0b10, 0b001, 0b0111, 0b1101, 0b110), // RO
    kTRCOSLAR             = encode(0b10, 0b001, 0b0001, 0b0000, 0b100), // WO
    kTRCOSLSR             = encode(0b10, 0b001, 0b0001, 0b0001, 0b100), // RO
    kTRCPDCR              = encode(0b10, 0b001, 0b0001, 0b0100, 0b100), // RW
    kTRCPDSR              = encode(0b10, 0b001, 0b0001, 0b0101, 0b100), // RO
    kTRCPIDR0             = encode(0b10, 0b001, 0b0111, 0b1000, 0b111), // RO
    kTRCPIDR1             = encode(0b10, 0b001, 0b0111, 0b1001, 0b111), // RO
    kTRCPIDR2             = encode(0b10, 0b001, 0b0111, 0b1010, 0b111), // RO
    kTRCPIDR3             = encode(0b10, 0b001, 0b0111, 0b1011, 0b111), // RO
    kTRCPIDR4             = encode(0b10, 0b001, 0b0111, 0b0100, 0b111), // RO
    kTRCPIDR5             = encode(0b10, 0b001, 0b0111, 0b0101, 0b111), // RO
    kTRCPIDR6             = encode(0b10, 0b001, 0b0111, 0b0110, 0b111), // RO
    kTRCPIDR7             = encode(0b10, 0b001, 0b0111, 0b0111, 0b111), // RO
    kTRCPRGCTLR           = encode(0b10, 0b001, 0b0000, 0b0001, 0b000), // RW
    kTRCPROCSELR          = encode(0b10, 0b001, 0b0000, 0b0010, 0b000), // RW
    kTRCQCTLR             = encode(0b10, 0b001, 0b0000, 0b0001, 0b001), // RW
    kTRCRSCTLR10          = encode(0b10, 0b001, 0b0001, 0b1010, 0b000), // RW
    kTRCRSCTLR11          = encode(0b10, 0b001, 0b0001, 0b1011, 0b000), // RW
    kTRCRSCTLR12          = encode(0b10, 0b001, 0b0001, 0b1100, 0b000), // RW
    kTRCRSCTLR13          = encode(0b10, 0b001, 0b0001, 0b1101, 0b000), // RW
    kTRCRSCTLR14          = encode(0b10, 0b001, 0b0001, 0b1110, 0b000), // RW
    kTRCRSCTLR15          = encode(0b10, 0b001, 0b0001, 0b1111, 0b000), // RW
    kTRCRSCTLR16          = encode(0b10, 0b001, 0b0001, 0b0000, 0b001), // RW
    kTRCRSCTLR17          = encode(0b10, 0b001, 0b0001, 0b0001, 0b001), // RW
    kTRCRSCTLR18          = encode(0b10, 0b001, 0b0001, 0b0010, 0b001), // RW
    kTRCRSCTLR19          = encode(0b10, 0b001, 0b0001, 0b0011, 0b001), // RW
    kTRCRSCTLR2           = encode(0b10, 0b001, 0b0001, 0b0010, 0b000), // RW
    kTRCRSCTLR20          = encode(0b10, 0b001, 0b0001, 0b0100, 0b001), // RW
    kTRCRSCTLR21          = encode(0b10, 0b001, 0b0001, 0b0101, 0b001), // RW
    kTRCRSCTLR22          = encode(0b10, 0b001, 0b0001, 0b0110, 0b001), // RW
    kTRCRSCTLR23          = encode(0b10, 0b001, 0b0001, 0b0111, 0b001), // RW
    kTRCRSCTLR24          = encode(0b10, 0b001, 0b0001, 0b1000, 0b001), // RW
    kTRCRSCTLR25          = encode(0b10, 0b001, 0b0001, 0b1001, 0b001), // RW
    kTRCRSCTLR26          = encode(0b10, 0b001, 0b0001, 0b1010, 0b001), // RW
    kTRCRSCTLR27          = encode(0b10, 0b001, 0b0001, 0b1011, 0b001), // RW
    kTRCRSCTLR28          = encode(0b10, 0b001, 0b0001, 0b1100, 0b001), // RW
    kTRCRSCTLR29          = encode(0b10, 0b001, 0b0001, 0b1101, 0b001), // RW
    kTRCRSCTLR3           = encode(0b10, 0b001, 0b0001, 0b0011, 0b000), // RW
    kTRCRSCTLR30          = encode(0b10, 0b001, 0b0001, 0b1110, 0b001), // RW
    kTRCRSCTLR31          = encode(0b10, 0b001, 0b0001, 0b1111, 0b001), // RW
    kTRCRSCTLR4           = encode(0b10, 0b001, 0b0001, 0b0100, 0b000), // RW
    kTRCRSCTLR5           = encode(0b10, 0b001, 0b0001, 0b0101, 0b000), // RW
    kTRCRSCTLR6           = encode(0b10, 0b001, 0b0001, 0b0110, 0b000), // RW
    kTRCRSCTLR7           = encode(0b10, 0b001, 0b0001, 0b0111, 0b000), // RW
    kTRCRSCTLR8           = encode(0b10, 0b001, 0b0001, 0b1000, 0b000), // RW
    kTRCRSCTLR9           = encode(0b10, 0b001, 0b0001, 0b1001, 0b000), // RW
    kTRCRSR               = encode(0b10, 0b001, 0b0000, 0b1010, 0b000), // RW
    kTRCSEQEVR0           = encode(0b10, 0b001, 0b0000, 0b0000, 0b100), // RW
    kTRCSEQEVR1           = encode(0b10, 0b001, 0b0000, 0b0001, 0b100), // RW
    kTRCSEQEVR2           = encode(0b10, 0b001, 0b0000, 0b0010, 0b100), // RW
    kTRCSEQRSTEVR         = encode(0b10, 0b001, 0b0000, 0b0110, 0b100), // RW
    kTRCSEQSTR            = encode(0b10, 0b001, 0b0000, 0b0111, 0b100), // RW
    kTRCSSCCR0            = encode(0b10, 0b001, 0b0001, 0b0000, 0b010), // RW
    kTRCSSCCR1            = encode(0b10, 0b001, 0b0001, 0b0001, 0b010), // RW
    kTRCSSCCR2            = encode(0b10, 0b001, 0b0001, 0b0010, 0b010), // RW
    kTRCSSCCR3            = encode(0b10, 0b001, 0b0001, 0b0011, 0b010), // RW
    kTRCSSCCR4            = encode(0b10, 0b001, 0b0001, 0b0100, 0b010), // RW
    kTRCSSCCR5            = encode(0b10, 0b001, 0b0001, 0b0101, 0b010), // RW
    kTRCSSCCR6            = encode(0b10, 0b001, 0b0001, 0b0110, 0b010), // RW
    kTRCSSCCR7            = encode(0b10, 0b001, 0b0001, 0b0111, 0b010), // RW
    kTRCSSCSR0            = encode(0b10, 0b001, 0b0001, 0b1000, 0b010), // RW
    kTRCSSCSR1            = encode(0b10, 0b001, 0b0001, 0b1001, 0b010), // RW
    kTRCSSCSR2            = encode(0b10, 0b001, 0b0001, 0b1010, 0b010), // RW
    kTRCSSCSR3            = encode(0b10, 0b001, 0b0001, 0b1011, 0b010), // RW
    kTRCSSCSR4            = encode(0b10, 0b001, 0b0001, 0b1100, 0b010), // RW
    kTRCSSCSR5            = encode(0b10, 0b001, 0b0001, 0b1101, 0b010), // RW
    kTRCSSCSR6            = encode(0b10, 0b001, 0b0001, 0b1110, 0b010), // RW
    kTRCSSCSR7            = encode(0b10, 0b001, 0b0001, 0b1111, 0b010), // RW
    kTRCSSPCICR0          = encode(0b10, 0b001, 0b0001, 0b0000, 0b011), // RW
    kTRCSSPCICR1          = encode(0b10, 0b001, 0b0001, 0b0001, 0b011), // RW
    kTRCSSPCICR2          = encode(0b10, 0b001, 0b0001, 0b0010, 0b011), // RW
    kTRCSSPCICR3          = encode(0b10, 0b001, 0b0001, 0b0011, 0b011), // RW
    kTRCSSPCICR4          = encode(0b10, 0b001, 0b0001, 0b0100, 0b011), // RW
    kTRCSSPCICR5          = encode(0b10, 0b001, 0b0001, 0b0101, 0b011), // RW
    kTRCSSPCICR6          = encode(0b10, 0b001, 0b0001, 0b0110, 0b011), // RW
    kTRCSSPCICR7          = encode(0b10, 0b001, 0b0001, 0b0111, 0b011), // RW
    kTRCSTALLCTLR         = encode(0b10, 0b001, 0b0000, 0b1011, 0b000), // RW
    kTRCSTATR             = encode(0b10, 0b001, 0b0000, 0b0011, 0b000), // RO
    kTRCSYNCPR            = encode(0b10, 0b001, 0b0000, 0b1101, 0b000), // RW
    kTRCTRACEIDR          = encode(0b10, 0b001, 0b0000, 0b0000, 0b001), // RW
    kTRCTSCTLR            = encode(0b10, 0b001, 0b0000, 0b1100, 0b000), // RW
    kTRCVDARCCTLR         = encode(0b10, 0b001, 0b0000, 0b1010, 0b010), // RW
    kTRCVDCTLR            = encode(0b10, 0b001, 0b0000, 0b1000, 0b010), // RW
    kTRCVDSACCTLR         = encode(0b10, 0b001, 0b0000, 0b1001, 0b010), // RW
    kTRCVICTLR            = encode(0b10, 0b001, 0b0000, 0b0000, 0b010), // RW
    kTRCVIIECTLR          = encode(0b10, 0b001, 0b0000, 0b0001, 0b010), // RW
    kTRCVIPCSSCTLR        = encode(0b10, 0b001, 0b0000, 0b0011, 0b010), // RW
    kTRCVISSCTLR          = encode(0b10, 0b001, 0b0000, 0b0010, 0b010), // RW
    kTRCVMIDCCTLR0        = encode(0b10, 0b001, 0b0011, 0b0010, 0b010), // RW
    kTRCVMIDCCTLR1        = encode(0b10, 0b001, 0b0011, 0b0011, 0b010), // RW
    kTRCVMIDCVR0          = encode(0b10, 0b001, 0b0011, 0b0000, 0b001), // RW
    kTRCVMIDCVR1          = encode(0b10, 0b001, 0b0011, 0b0010, 0b001), // RW
    kTRCVMIDCVR2          = encode(0b10, 0b001, 0b0011, 0b0100, 0b001), // RW
    kTRCVMIDCVR3          = encode(0b10, 0b001, 0b0011, 0b0110, 0b001), // RW
    kTRCVMIDCVR4          = encode(0b10, 0b001, 0b0011, 0b1000, 0b001), // RW
    kTRCVMIDCVR5          = encode(0b10, 0b001, 0b0011, 0b1010, 0b001), // RW
    kTRCVMIDCVR6          = encode(0b10, 0b001, 0b0011, 0b1100, 0b001), // RW
    kTRCVMIDCVR7          = encode(0b10, 0b001, 0b0011, 0b1110, 0b001), // RW
    kTRFCR_EL1            = encode(0b11, 0b000, 0b0001, 0b0010, 0b001), // RW
    kTRFCR_EL12           = encode(0b11, 0b101, 0b0001, 0b0010, 0b001), // RW
    kTRFCR_EL2            = encode(0b11, 0b100, 0b0001, 0b0010, 0b001), // RW
    kTTBR0_EL1            = encode(0b11, 0b000, 0b0010, 0b0000, 0b000), // RW
    kTTBR0_EL12           = encode(0b11, 0b101, 0b0010, 0b0000, 0b000), // RW
    kTTBR0_EL2            = encode(0b11, 0b100, 0b0010, 0b0000, 0b000), // RW
    kTTBR0_EL3            = encode(0b11, 0b110, 0b0010, 0b0000, 0b000), // RW
    kTTBR1_EL1            = encode(0b11, 0b000, 0b0010, 0b0000, 0b001), // RW
    kTTBR1_EL12           = encode(0b11, 0b101, 0b0010, 0b0000, 0b001), // RW
    kTTBR1_EL2            = encode(0b11, 0b100, 0b0010, 0b0000, 0b001), // RW
    kUAO                  = encode(0b11, 0b000, 0b0100, 0b0010, 0b100), // RW
    kVBAR_EL1             = encode(0b11, 0b000, 0b1100, 0b0000, 0b000), // RW
    kVBAR_EL12            = encode(0b11, 0b101, 0b1100, 0b0000, 0b000), // RW
    kVBAR_EL2             = encode(0b11, 0b100, 0b1100, 0b0000, 0b000), // RW
    kVBAR_EL3             = encode(0b11, 0b110, 0b1100, 0b0000, 0b000), // RW
    kVDISR_EL2            = encode(0b11, 0b100, 0b1100, 0b0001, 0b001), // RW
    kVMPIDR_EL2           = encode(0b11, 0b100, 0b0000, 0b0000, 0b101), // RW
    kVNCR_EL2             = encode(0b11, 0b100, 0b0010, 0b0010, 0b000), // RW
    kVPIDR_EL2            = encode(0b11, 0b100, 0b0000, 0b0000, 0b000), // RW
    kVSESR_EL2            = encode(0b11, 0b100, 0b0101, 0b0010, 0b011), // RW
    kVSTCR_EL2            = encode(0b11, 0b100, 0b0010, 0b0110, 0b010), // RW
    kVSTTBR_EL2           = encode(0b11, 0b100, 0b0010, 0b0110, 0b000), // RW
    kVTCR_EL2             = encode(0b11, 0b100, 0b0010, 0b0001, 0b010), // RW
    kVTTBR_EL2            = encode(0b11, 0b100, 0b0010, 0b0001, 0b000), // RW
    kZCR_EL1              = encode(0b11, 0b000, 0b0001, 0b0010, 0b000), // RW
    kZCR_EL12             = encode(0b11, 0b101, 0b0001, 0b0010, 0b000), // RW
    kZCR_EL2              = encode(0b11, 0b100, 0b0001, 0b0010, 0b000), // RW
    kZCR_EL3              = encode(0b11, 0b110, 0b0001, 0b0010, 0b000)  // RW
  };
};

} // {Predicate}

//! \}

ASMJIT_END_SUB_NAMESPACE

#endif // ASMJIT_LOONG_LA64GLOBALS_H_INCLUDED
