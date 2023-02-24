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

#ifndef ASMJIT_LOONG_LA64OPERAND_H_INCLUDED
#define ASMJIT_LOONG_LA64OPERAND_H_INCLUDED

#include "../loong/loongoperand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(la64)

//! \addtogroup asmjit_la64
//! \{

using loong::Reg;
using loong::Mem;
using loong::Gp;
using loong::GpW;
using loong::GpX;
using loong::Vec;
using loong::VecB;
using loong::VecH;
using loong::VecS;
using loong::VecD;
using loong::VecV;
using loong::VecX;

#ifndef _DOXYGEN
namespace regs {
#endif

using namespace ::asmjit::loong::regs;

static constexpr GpW w0 = GpW(Gp::kIdZr);
static constexpr GpW w1 = GpW(1);
static constexpr GpW w2 = GpW(2);
static constexpr GpW w3 = GpW(Gp::kIdSp);
static constexpr GpW w4 = GpW(4);
static constexpr GpW w5 = GpW(5);
static constexpr GpW w6 = GpW(6);
static constexpr GpW w7 = GpW(7);
static constexpr GpW w8 = GpW(8);
static constexpr GpW w9 = GpW(9);
static constexpr GpW w10 = GpW(10);
static constexpr GpW w11 = GpW(11);
static constexpr GpW w12 = GpW(12);
static constexpr GpW w13 = GpW(13);
static constexpr GpW w14 = GpW(14);
static constexpr GpW w15 = GpW(15);
static constexpr GpW w16 = GpW(16);
static constexpr GpW w17 = GpW(17);
static constexpr GpW w18 = GpW(18);
static constexpr GpW w19 = GpW(19);
static constexpr GpW w20 = GpW(20);
static constexpr GpW w21 = GpW(21);
static constexpr GpW w22 = GpW(22);
static constexpr GpW w23 = GpW(23);
static constexpr GpW w24 = GpW(24);
static constexpr GpW w25 = GpW(25);
static constexpr GpW w26 = GpW(26);
static constexpr GpW w27 = GpW(27);
static constexpr GpW w28 = GpW(28);
static constexpr GpW w29 = GpW(29);
static constexpr GpW w30 = GpW(30);
static constexpr GpW w31 = GpW(31);
//static constexpr GpW wzr = GpW(Gp::kIdZr);
//static constexpr GpW wsp = GpW(Gp::kIdSp);

static constexpr GpX r0 = GpX(Gp::kIdZr);
static constexpr GpX r1 = GpX(1);
static constexpr GpX r2 = GpX(2);
static constexpr GpX r3 = GpX(Gp::kIdSp);
static constexpr GpX r4 = GpX(4);
static constexpr GpX r5 = GpX(5);
static constexpr GpX r6 = GpX(6);
static constexpr GpX r7 = GpX(7);
static constexpr GpX r8 = GpX(8);
static constexpr GpX r9 = GpX(9);
static constexpr GpX r10 = GpX(10);
static constexpr GpX r11 = GpX(11);
static constexpr GpX r12 = GpX(12);
static constexpr GpX r13 = GpX(13);
static constexpr GpX r14 = GpX(14);
static constexpr GpX r15 = GpX(15);
static constexpr GpX r16 = GpX(16);
static constexpr GpX r17 = GpX(17);
static constexpr GpX r18 = GpX(18);
static constexpr GpX r19 = GpX(19);
static constexpr GpX r20 = GpX(20);
static constexpr GpX r21 = GpX(21);
static constexpr GpX r22 = GpX(22);
static constexpr GpX r23 = GpX(23);
static constexpr GpX r24 = GpX(24);
static constexpr GpX r25 = GpX(25);
static constexpr GpX r26 = GpX(26);
static constexpr GpX r27 = GpX(27);
static constexpr GpX r28 = GpX(28);
static constexpr GpX r29 = GpX(29);
static constexpr GpX r30 = GpX(30);
static constexpr GpX r31 = GpX(31);
//static constexpr GpX xzr = GpX(Gp::kIdZr);
//static constexpr GpX sp = GpX(Gp::kIdSp);

static constexpr GpX fcsr0 = GpX(0);
static constexpr GpX fcsr1 = GpX(1);
static constexpr GpX fcsr2 = GpX(2);
static constexpr GpX fcsr3 = GpX(3);

static constexpr GpX zero = GpX(Gp::kIdZr);
static constexpr GpX ra = GpX(1);
static constexpr GpX tp = GpX(2);
static constexpr GpX sp = GpX(Gp::kIdSp);
//static constexpr GpX v0 = GpX(4);
//static constexpr GpX v1 = GpX(5);
static constexpr GpX a0 = GpX(4);
static constexpr GpX a1 = GpX(5);
static constexpr GpX a2 = GpX(6);
static constexpr GpX a3 = GpX(7);
static constexpr GpX a4 = GpX(8);
static constexpr GpX a5 = GpX(9);
static constexpr GpX a6 = GpX(10);
static constexpr GpX a7 = GpX(11);
static constexpr GpX t0 = GpX(12);
static constexpr GpX t1 = GpX(13);
static constexpr GpX t2 = GpX(14);
static constexpr GpX t3 = GpX(15);
static constexpr GpX t4 = GpX(16);
static constexpr GpX t5 = GpX(17);
static constexpr GpX t6 = GpX(18);
static constexpr GpX t7 = GpX(19);
static constexpr GpX t8 = GpX(20);
//static constexpr GpX x = GpX(21);  //build issue: confilict
static constexpr GpX fp = GpX(22);
static constexpr GpX s0 = GpX(23);
static constexpr GpX s1 = GpX(24);
static constexpr GpX s2 = GpX(25);
static constexpr GpX s3 = GpX(26);
static constexpr GpX s4 = GpX(27);
static constexpr GpX s5 = GpX(28);
static constexpr GpX s6 = GpX(29);
static constexpr GpX s7 = GpX(30);
static constexpr GpX s8 = GpX(31);

static constexpr VecB b0 = VecB(0);
static constexpr VecB b1 = VecB(1);
static constexpr VecB b2 = VecB(2);
static constexpr VecB b3 = VecB(3);
static constexpr VecB b4 = VecB(4);
static constexpr VecB b5 = VecB(5);
static constexpr VecB b6 = VecB(6);
static constexpr VecB b7 = VecB(7);
static constexpr VecB b8 = VecB(8);
static constexpr VecB b9 = VecB(9);
static constexpr VecB b10 = VecB(10);
static constexpr VecB b11 = VecB(11);
static constexpr VecB b12 = VecB(12);
static constexpr VecB b13 = VecB(13);
static constexpr VecB b14 = VecB(14);
static constexpr VecB b15 = VecB(15);
static constexpr VecB b16 = VecB(16);
static constexpr VecB b17 = VecB(17);
static constexpr VecB b18 = VecB(18);
static constexpr VecB b19 = VecB(19);
static constexpr VecB b20 = VecB(20);
static constexpr VecB b21 = VecB(21);
static constexpr VecB b22 = VecB(22);
static constexpr VecB b23 = VecB(23);
static constexpr VecB b24 = VecB(24);
static constexpr VecB b25 = VecB(25);
static constexpr VecB b26 = VecB(26);
static constexpr VecB b27 = VecB(27);
static constexpr VecB b28 = VecB(28);
static constexpr VecB b29 = VecB(29);
static constexpr VecB b30 = VecB(30);
static constexpr VecB b31 = VecB(31);

static constexpr VecH h0 = VecH(0);
static constexpr VecH h1 = VecH(1);
static constexpr VecH h2 = VecH(2);
static constexpr VecH h3 = VecH(3);
static constexpr VecH h4 = VecH(4);
static constexpr VecH h5 = VecH(5);
static constexpr VecH h6 = VecH(6);
static constexpr VecH h7 = VecH(7);
static constexpr VecH h8 = VecH(8);
static constexpr VecH h9 = VecH(9);
static constexpr VecH h10 = VecH(10);
static constexpr VecH h11 = VecH(11);
static constexpr VecH h12 = VecH(12);
static constexpr VecH h13 = VecH(13);
static constexpr VecH h14 = VecH(14);
static constexpr VecH h15 = VecH(15);
static constexpr VecH h16 = VecH(16);
static constexpr VecH h17 = VecH(17);
static constexpr VecH h18 = VecH(18);
static constexpr VecH h19 = VecH(19);
static constexpr VecH h20 = VecH(20);
static constexpr VecH h21 = VecH(21);
static constexpr VecH h22 = VecH(22);
static constexpr VecH h23 = VecH(23);
static constexpr VecH h24 = VecH(24);
static constexpr VecH h25 = VecH(25);
static constexpr VecH h26 = VecH(26);
static constexpr VecH h27 = VecH(27);
static constexpr VecH h28 = VecH(28);
static constexpr VecH h29 = VecH(29);
static constexpr VecH h30 = VecH(30);
static constexpr VecH h31 = VecH(31);
/*
static constexpr VecS s0 = VecS(0);
static constexpr VecS s1 = VecS(1);
static constexpr VecS s2 = VecS(2);
static constexpr VecS s3 = VecS(3);
static constexpr VecS s4 = VecS(4);
static constexpr VecS s5 = VecS(5);
static constexpr VecS s6 = VecS(6);
static constexpr VecS s7 = VecS(7);
static constexpr VecS s8 = VecS(8);
static constexpr VecS s9 = VecS(9);
static constexpr VecS s10 = VecS(10);
static constexpr VecS s11 = VecS(11);
static constexpr VecS s12 = VecS(12);
static constexpr VecS s13 = VecS(13);
static constexpr VecS s14 = VecS(14);
static constexpr VecS s15 = VecS(15);
static constexpr VecS s16 = VecS(16);
static constexpr VecS s17 = VecS(17);
static constexpr VecS s18 = VecS(18);
static constexpr VecS s19 = VecS(19);
static constexpr VecS s20 = VecS(20);
static constexpr VecS s21 = VecS(21);
static constexpr VecS s22 = VecS(22);
static constexpr VecS s23 = VecS(23);
static constexpr VecS s24 = VecS(24);
static constexpr VecS s25 = VecS(25);
static constexpr VecS s26 = VecS(26);
static constexpr VecS s27 = VecS(27);
static constexpr VecS s28 = VecS(28);
static constexpr VecS s29 = VecS(29);
static constexpr VecS s30 = VecS(30);
static constexpr VecS s31 = VecS(31);
*/
static constexpr VecD d0 = VecD(0);
static constexpr VecD d1 = VecD(1);
static constexpr VecD d2 = VecD(2);
static constexpr VecD d3 = VecD(3);
static constexpr VecD d4 = VecD(4);
static constexpr VecD d5 = VecD(5);
static constexpr VecD d6 = VecD(6);
static constexpr VecD d7 = VecD(7);
static constexpr VecD d8 = VecD(8);
static constexpr VecD d9 = VecD(9);
static constexpr VecD d10 = VecD(10);
static constexpr VecD d11 = VecD(11);
static constexpr VecD d12 = VecD(12);
static constexpr VecD d13 = VecD(13);
static constexpr VecD d14 = VecD(14);
static constexpr VecD d15 = VecD(15);
static constexpr VecD d16 = VecD(16);
static constexpr VecD d17 = VecD(17);
static constexpr VecD d18 = VecD(18);
static constexpr VecD d19 = VecD(19);
static constexpr VecD d20 = VecD(20);
static constexpr VecD d21 = VecD(21);
static constexpr VecD d22 = VecD(22);
static constexpr VecD d23 = VecD(23);
static constexpr VecD d24 = VecD(24);
static constexpr VecD d25 = VecD(25);
static constexpr VecD d26 = VecD(26);
static constexpr VecD d27 = VecD(27);
static constexpr VecD d28 = VecD(28);
static constexpr VecD d29 = VecD(29);
static constexpr VecD d30 = VecD(30);
static constexpr VecD d31 = VecD(31);

static constexpr VecD f0 = VecD(0);
static constexpr VecD f1 = VecD(1);
static constexpr VecD f2 = VecD(2);
static constexpr VecD f3 = VecD(3);
static constexpr VecD f4 = VecD(4);
static constexpr VecD f5 = VecD(5);
static constexpr VecD f6 = VecD(6);
static constexpr VecD f7 = VecD(7);
static constexpr VecD f8 = VecD(8);
static constexpr VecD f9 = VecD(9);
static constexpr VecD f10 = VecD(10);
static constexpr VecD f11 = VecD(11);
static constexpr VecD f12 = VecD(12);
static constexpr VecD f13 = VecD(13);
static constexpr VecD f14 = VecD(14);
static constexpr VecD f15 = VecD(15);
static constexpr VecD f16 = VecD(16);
static constexpr VecD f17 = VecD(17);
static constexpr VecD f18 = VecD(18);
static constexpr VecD f19 = VecD(19);
static constexpr VecD f20 = VecD(20);
static constexpr VecD f21 = VecD(21);
static constexpr VecD f22 = VecD(22);
static constexpr VecD f23 = VecD(23);
static constexpr VecD f24 = VecD(24);
static constexpr VecD f25 = VecD(25);
static constexpr VecD f26 = VecD(26);
static constexpr VecD f27 = VecD(27);
static constexpr VecD f28 = VecD(28);
static constexpr VecD f29 = VecD(29);
static constexpr VecD f30 = VecD(30);
static constexpr VecD f31 = VecD(31);

static constexpr VecV q0 = VecV(0);
static constexpr VecV q1 = VecV(1);
static constexpr VecV q2 = VecV(2);
static constexpr VecV q3 = VecV(3);
static constexpr VecV q4 = VecV(4);
static constexpr VecV q5 = VecV(5);
static constexpr VecV q6 = VecV(6);
static constexpr VecV q7 = VecV(7);
static constexpr VecV q8 = VecV(8);
static constexpr VecV q9 = VecV(9);
static constexpr VecV q10 = VecV(10);
static constexpr VecV q11 = VecV(11);
static constexpr VecV q12 = VecV(12);
static constexpr VecV q13 = VecV(13);
static constexpr VecV q14 = VecV(14);
static constexpr VecV q15 = VecV(15);
static constexpr VecV q16 = VecV(16);
static constexpr VecV q17 = VecV(17);
static constexpr VecV q18 = VecV(18);
static constexpr VecV q19 = VecV(19);
static constexpr VecV q20 = VecV(20);
static constexpr VecV q21 = VecV(21);
static constexpr VecV q22 = VecV(22);
static constexpr VecV q23 = VecV(23);
static constexpr VecV q24 = VecV(24);
static constexpr VecV q25 = VecV(25);
static constexpr VecV q26 = VecV(26);
static constexpr VecV q27 = VecV(27);
static constexpr VecV q28 = VecV(28);
static constexpr VecV q29 = VecV(29);
static constexpr VecV q30 = VecV(30);
static constexpr VecV q31 = VecV(31);

static constexpr VecV v0 = VecV(0);
static constexpr VecV v1 = VecV(1);
static constexpr VecV v2 = VecV(2);
static constexpr VecV v3 = VecV(3);
static constexpr VecV v4 = VecV(4);
static constexpr VecV v5 = VecV(5);
static constexpr VecV v6 = VecV(6);
static constexpr VecV v7 = VecV(7);
static constexpr VecV v8 = VecV(8);
static constexpr VecV v9 = VecV(9);
static constexpr VecV v10 = VecV(10);
static constexpr VecV v11 = VecV(11);
static constexpr VecV v12 = VecV(12);
static constexpr VecV v13 = VecV(13);
static constexpr VecV v14 = VecV(14);
static constexpr VecV v15 = VecV(15);
static constexpr VecV v16 = VecV(16);
static constexpr VecV v17 = VecV(17);
static constexpr VecV v18 = VecV(18);
static constexpr VecV v19 = VecV(19);
static constexpr VecV v20 = VecV(20);
static constexpr VecV v21 = VecV(21);
static constexpr VecV v22 = VecV(22);
static constexpr VecV v23 = VecV(23);
static constexpr VecV v24 = VecV(24);
static constexpr VecV v25 = VecV(25);
static constexpr VecV v26 = VecV(26);
static constexpr VecV v27 = VecV(27);
static constexpr VecV v28 = VecV(28);
static constexpr VecV v29 = VecV(29);
static constexpr VecV v30 = VecV(30);
static constexpr VecV v31 = VecV(31);

static constexpr VecX xr0 = VecX(0);
static constexpr VecX xr1 = VecX(1);
static constexpr VecX xr2 = VecX(2);
static constexpr VecX xr3 = VecX(3);
static constexpr VecX xr4 = VecX(4);
static constexpr VecX xr5 = VecX(5);
static constexpr VecX xr6 = VecX(6);
static constexpr VecX xr7 = VecX(7);
static constexpr VecX xr8 = VecX(8);
static constexpr VecX xr9 = VecX(9);
static constexpr VecX xr10 = VecX(10);
static constexpr VecX xr11 = VecX(11);
static constexpr VecX xr12 = VecX(12);
static constexpr VecX xr13 = VecX(13);
static constexpr VecX xr14 = VecX(14);
static constexpr VecX xr15 = VecX(15);
static constexpr VecX xr16 = VecX(16);
static constexpr VecX xr17 = VecX(17);
static constexpr VecX xr18 = VecX(18);
static constexpr VecX xr19 = VecX(19);
static constexpr VecX xr20 = VecX(20);
static constexpr VecX xr21 = VecX(21);
static constexpr VecX xr22 = VecX(22);
static constexpr VecX xr23 = VecX(23);
static constexpr VecX xr24 = VecX(24);
static constexpr VecX xr25 = VecX(25);
static constexpr VecX xr26 = VecX(26);
static constexpr VecX xr27 = VecX(27);
static constexpr VecX xr28 = VecX(28);
static constexpr VecX xr29 = VecX(29);
static constexpr VecX xr30 = VecX(30);
static constexpr VecX xr31 = VecX(31);

#ifndef _DOXYGEN
} // {regs}

// Make `loong::regs` accessible through `loong` namespace as well.
using namespace regs;
#endif

//! \}

ASMJIT_END_SUB_NAMESPACE

#endif // ASMJIT_LOONG_LA64OPERAND_H_INCLUDED
