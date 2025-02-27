/*******************************************************************************
 * Copyright 2019-2020 LOONGSON LIMITED
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
// clang-format off
#include <asmjit/asmjit.h>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#define NUM_OF_ARRAY(x) (sizeof(x) / sizeof(x[0]))

using namespace asmjit;

// const int bitEnd = 64;

enum AddressingType {
  TP_PreImm,  /** pre-indexded immediate */
  TP_PostImm, /** post-indexed immediate */
  TP_PostReg, /** Post-indexed register */
  TP_None
};

uint64_t flagBit = 0;
const uint64_t WREG = flagBit++;  /** Test vector is {w0, w1, ..., w30 } */
const uint64_t XREG = flagBit++;  /** Test vector is {x0, x1, ..., x30 } */
const uint64_t XREG2 = flagBit++; /** Test vector is {x0, x1, ..., x30 } */
const uint64_t WSP = flagBit++;   /** Test vector is {wsp} */
const uint64_t XSP = flagBit++;   /** Test vector is {sp} */
const uint64_t XNSP = flagBit++;
const uint64_t XNSP2 = flagBit++;
const uint64_t XNSP3 = flagBit++;

const uint64_t WNZR = flagBit++; /** Test vector is {w0, w1, ..., w30, wzr } */
const uint64_t XNZR = flagBit++; /** Test vector is {x0, x1, ..., x30, xzr } */

const uint64_t IMM1BIT = flagBit++; /** Test vector is {0, 1 } */
const uint64_t IMM2BIT = flagBit++; /** Test vector is {0, 1, ..., 3 } */
const uint64_t IMM3BIT = flagBit++; /** Test vector is {0, 1, ..., 7 } */
const uint64_t IMM3BIT_FCC = flagBit++; /** Test vector is {0, 1, 2, 4, 7} */
const uint64_t IMM4BIT = flagBit++; /** Test vector is {0, 1, ..., 8, 15 } */
const uint64_t IMM5BIT = flagBit++; /** Test vector is {0, 1, ..., 31 } */
const uint64_t IMM6BIT = flagBit++; /** Test vector is {0, 1, ..., 63 } */
const uint64_t IMM7BIT = flagBit++; /** Test vector is {0, 1, ..., 127 } */
const uint64_t IMM16BIT =
    flagBit++; /** Test vector is {0, 1, ..., 2048, 4095 } */
const uint64_t UIMM16BIT = flagBit++; /** Test vector is {0, 1, ..., 4096, 1<<13,
                                        1<<14, 1<<15, 1<<16-1 } */
const uint64_t IMM9BIT_PM =
    flagBit++; /** Test vector is {-256, -255, ..., 255 } */
const uint64_t IMM7BIT_MUL4 =
    flagBit++; /** Test vector is { 0, 4, 8, ..., 127*4 } */
const uint64_t IMM7BIT_MUL8 =
    flagBit++; /** Test vector is { 0, 8, 16, ..., 127*8 } */
const uint64_t IMM7BIT_MUL16 =
    flagBit++; /** Test vector is { -256, -252, ..., 252 } */
const uint64_t IMM7BIT_PM_MUL4 = flagBit++;
const uint64_t IMM7BIT_PM_MUL8 = flagBit++;
const uint64_t IMM7BIT_PM_MUL16 =
    flagBit++; /** Test vector is { 0, 16, 32, ..., 127*16 } */
const uint64_t IMM10BIT_PM_MUL8 = flagBit++;
const uint64_t IMM12BIT_MUL2 =
    flagBit++; /** Test vector is {0, 4, 8, ..., 8190 } */
const uint64_t IMM12BIT_MUL4 =
    flagBit++; /** Test vector is {0, 4, 8, ..., 16380 } */
const uint64_t IMM12BIT_MUL8 =
    flagBit++; /** Test vector is {0, 8, 16, ..., 32760 } */
const uint64_t IMM12BIT_MUL16 =
    flagBit++; /** Test vector is {0, 16, 32, ..., 65320 } */
const uint64_t IMM14BIT_MUL4 = flagBit++;
const uint64_t IMM16BIT_MUL4 = flagBit++;
const uint64_t IMM20BIT_MUL4 = flagBit++;
const uint64_t IMM21BIT_MUL4 =
    flagBit++; /** Test vector is {-2^19*4, ...., (2^19-1)*4 } */
const uint64_t IMM26BIT_MUL4 = flagBit++;

const uint64_t BREG = flagBit++; /** Test vector is { b0, b1, ..., b31 } */
const uint64_t HREG = flagBit++; /** Test vector is { h0, h1, ..., h31 } */
const uint64_t SREG = flagBit++; /** Test vector is { s0, s1, ..., s31 } */
const uint64_t DREG = flagBit++; /** Test vector is { d0, d1, ..., d31 } */
const uint64_t QREG = flagBit++; /** Test vector is { q0, q1, ..., q31 } */

const uint64_t SPECIFIC32 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC64 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC32_1 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC64_1 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC32_2 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC64_2 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC32_3 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC64_3 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC0 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC1 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC2 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC3 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC4 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC5 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC6 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC7 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC8 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC9 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC10 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC11 =
    flagBit++; /** Test vector is generated on the fly. */
const uint64_t SPECIFIC12 = flagBit++;
const uint64_t SPECIFIC13 = flagBit++;
const uint64_t SPECIFIC14 = flagBit++;
const uint64_t SPECIFIC15 = flagBit++;
const uint64_t SPECIFIC16 = flagBit++;
const uint64_t SPECIFIC17 = flagBit++;

const uint64_t PTR_O = flagBit++;
const uint64_t PTR_C = flagBit++;

const uint64_t BRA_O = flagBit++; /** Test vector is { "{" } */
const uint64_t BRA_C = flagBit++; /** Test vector is { "}" } */

const uint64_t PAR_O = flagBit++; /** Test vector is { "(" } */
const uint64_t PAR_C = flagBit++; /** Test vector is { ">" } */

const uint64_t PRE_PTR_O =
    flagBit++; /** Test vector is { "[" } or { "pre_ptr(" } */
const uint64_t PRE_PTR_C = flagBit++; /** Test vector is { "]!" } or { ")" } */

// const uint64_t POST_PTR  = flagBit++;

const uint64_t T_LSL = flagBit++;  /** Test vector is { "LSL" } */
const uint64_t T_UXTW = flagBit++; /** Test vector is { "UXTW", "UXT" } */
const uint64_t T_SXTW = flagBit++; /** Test vector is { "SXTW", "SXT" } */
const uint64_t T_SXTX = flagBit++; /** Test vector is { "SXTX", "SXT" } */

const uint64_t IMM_0 = flagBit++;      /** Test vector is { "0" } */
const uint64_t IMM_0_OR_1 = flagBit++; /** Test vector is { "0", "1" } */
const uint64_t IMM_0_OR_2 = flagBit++; /** Test vector is { "0", "2" } */
const uint64_t IMM_0_OR_3 = flagBit++; /** Test vector is { "0", "3" } */
const uint64_t IMM_0_OR_4 = flagBit++; /** Test vector is { "0", "4" } */
const uint64_t IMM_2 = flagBit++;
const uint64_t IMM_4 = flagBit++;
const uint64_t IMM_8 = flagBit++;
const uint64_t IMM_16 = flagBit++;
const uint64_t IMM_8_OR_16 = flagBit++; /** Test vector is { "8", "16" } */
const uint64_t IMM_16_OR_32 = flagBit++;
const uint64_t IMM_24_OR_48 = flagBit++;
const uint64_t IMM_32_OR_64 = flagBit++;

const uint64_t VREGB_1D = flagBit++;
const uint64_t VREGH_1D = flagBit++;
const uint64_t VREGS_1D = flagBit++;
const uint64_t VREGD_1D = flagBit++;
const uint64_t VREGB_2D = flagBit++;
const uint64_t VREGH_2D = flagBit++;
const uint64_t VREGS_2D = flagBit++;
const uint64_t VREGD_2D = flagBit++;
const uint64_t VREGB_3D = flagBit++;
const uint64_t VREGH_3D = flagBit++;
const uint64_t VREGS_3D = flagBit++;
const uint64_t VREGD_3D = flagBit++;
const uint64_t VREGB_4D = flagBit++;
const uint64_t VREGH_4D = flagBit++;
const uint64_t VREGS_4D = flagBit++;
const uint64_t VREGD_4D = flagBit++;

const uint64_t VREG16B_1D = flagBit++;
const uint64_t VREG8B_1D = flagBit++;
const uint64_t VREG8H_1D = flagBit++;
const uint64_t VREG4H_1D = flagBit++;
const uint64_t VREG4S_1D = flagBit++;
const uint64_t VREG2S_1D = flagBit++;
const uint64_t VREG2D_1D = flagBit++;
const uint64_t VREG1D_1D = flagBit++;
const uint64_t VREG16B_2D = flagBit++;
const uint64_t VREG8B_2D = flagBit++;
const uint64_t VREG8H_2D = flagBit++;
const uint64_t VREG4H_2D = flagBit++;
const uint64_t VREG4S_2D = flagBit++;
const uint64_t VREG2S_2D = flagBit++;
const uint64_t VREG2D_2D = flagBit++;
const uint64_t VREG1D_2D = flagBit++;
const uint64_t VREG16B_3D = flagBit++;
const uint64_t VREG8B_3D = flagBit++;
const uint64_t VREG8H_3D = flagBit++;
const uint64_t VREG4H_3D = flagBit++;
const uint64_t VREG4S_3D = flagBit++;
const uint64_t VREG2S_3D = flagBit++;
const uint64_t VREG2D_3D = flagBit++;
const uint64_t VREG1D_3D = flagBit++;
const uint64_t VREG16B_4D = flagBit++;
const uint64_t VREG8B_4D = flagBit++;
const uint64_t VREG8H_4D = flagBit++;
const uint64_t VREG4H_4D = flagBit++;
const uint64_t VREG4S_4D = flagBit++;
const uint64_t VREG2S_4D = flagBit++;
const uint64_t VREG2D_4D = flagBit++;
const uint64_t VREG1D_4D = flagBit++;

const uint64_t VREGB_1D_ELEM = flagBit++;
const uint64_t VREGH_1D_ELEM = flagBit++;
const uint64_t VREGS_1D_ELEM = flagBit++;
const uint64_t VREGD_1D_ELEM = flagBit++;
const uint64_t VREGB_2D_ELEM = flagBit++;
const uint64_t VREGH_2D_ELEM = flagBit++;
const uint64_t VREGS_2D_ELEM = flagBit++;
const uint64_t VREGD_2D_ELEM = flagBit++;
const uint64_t VREGB_3D_ELEM = flagBit++;
const uint64_t VREGH_3D_ELEM = flagBit++;
const uint64_t VREGS_3D_ELEM = flagBit++;
const uint64_t VREGD_3D_ELEM = flagBit++;
const uint64_t VREGB_4D_ELEM = flagBit++;
const uint64_t VREGH_4D_ELEM = flagBit++;
const uint64_t VREGS_4D_ELEM = flagBit++;
const uint64_t VREGD_4D_ELEM = flagBit++;

const uint64_t VREG16B_1D_ELEM = flagBit++;
const uint64_t VREG8B_1D_ELEM = flagBit++;
const uint64_t VREG8H_1D_ELEM = flagBit++;
const uint64_t VREG4H_1D_ELEM = flagBit++;
const uint64_t VREG4S_1D_ELEM = flagBit++;
const uint64_t VREG2S_1D_ELEM = flagBit++;
const uint64_t VREG2D_1D_ELEM = flagBit++;
const uint64_t VREG1D_1D_ELEM = flagBit++;
const uint64_t VREG16B_2D_ELEM = flagBit++;
const uint64_t VREG8B_2D_ELEM = flagBit++;
const uint64_t VREG8H_2D_ELEM = flagBit++;
const uint64_t VREG4H_2D_ELEM = flagBit++;
const uint64_t VREG4S_2D_ELEM = flagBit++;
const uint64_t VREG2S_2D_ELEM = flagBit++;
const uint64_t VREG2D_2D_ELEM = flagBit++;
const uint64_t VREG1D_2D_ELEM = flagBit++;
const uint64_t VREG16B_3D_ELEM = flagBit++;
const uint64_t VREG8B_3D_ELEM = flagBit++;
const uint64_t VREG8H_3D_ELEM = flagBit++;
const uint64_t VREG4H_3D_ELEM = flagBit++;
const uint64_t VREG4S_3D_ELEM = flagBit++;
const uint64_t VREG2S_3D_ELEM = flagBit++;
const uint64_t VREG2D_3D_ELEM = flagBit++;
const uint64_t VREG1D_3D_ELEM = flagBit++;
const uint64_t VREG16B_4D_ELEM = flagBit++;
const uint64_t VREG8B_4D_ELEM = flagBit++;
const uint64_t VREG8H_4D_ELEM = flagBit++;
const uint64_t VREG4H_4D_ELEM = flagBit++;
const uint64_t VREG4S_4D_ELEM = flagBit++;
const uint64_t VREG2S_4D_ELEM = flagBit++;
const uint64_t VREG2D_4D_ELEM = flagBit++;
const uint64_t VREG1D_4D_ELEM = flagBit++;

const uint64_t PRFOP = flagBit++;
const uint64_t BARRIER = flagBit++;

const uint64_t NOPARA = 100000;

#define PUT0(name, nm)                                                         \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    put(nemonic, #name, 0);                                                    \
  }

#define PUT1(name, nm, op_1)                                                   \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    put(nemonic, op1, #name, 0);                                               \
  }

#define PUT2(name, nm, op_1, op_2)                                             \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
  }

#define PUT3(name, nm, op_1, op_2, op_3)                                       \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
  }

#define PUT4(name, nm, op_1, op_2, op_3, op_4)                                 \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
  }

#define PUT5(name, nm, op_1, op_2, op_3, op_4, op_5)                           \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
  }

#define PUT6(name, nm, op_1, op_2, op_3, op_4, op_5, op_6)                     \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    std::vector<uint64_t> op6(op_6);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
    put(nemonic, op6, #name, 5);                                               \
  }

#define PUT7(name, nm, op_1, op_2, op_3, op_4, op_5, op_6, op_7)               \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    std::vector<uint64_t> op6(op_6);                                           \
    std::vector<uint64_t> op7(op_7);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
    put(nemonic, op6, #name, 5);                                               \
    put(nemonic, op7, #name, 6);                                               \
  }

#define PUT8(name, nm, op_1, op_2, op_3, op_4, op_5, op_6, op_7, op_8)         \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    std::vector<uint64_t> op6(op_6);                                           \
    std::vector<uint64_t> op7(op_7);                                           \
    std::vector<uint64_t> op8(op_8);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
    put(nemonic, op6, #name, 5);                                               \
    put(nemonic, op7, #name, 6);                                               \
    put(nemonic, op8, #name, 7);                                               \
  }

#define PUT9(name, nm, op_1, op_2, op_3, op_4, op_5, op_6, op_7, op_8, op_9)   \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    std::vector<uint64_t> op6(op_6);                                           \
    std::vector<uint64_t> op7(op_7);                                           \
    std::vector<uint64_t> op8(op_8);                                           \
    std::vector<uint64_t> op9(op_9);                                           \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
    put(nemonic, op6, #name, 5);                                               \
    put(nemonic, op7, #name, 6);                                               \
    put(nemonic, op8, #name, 7);                                               \
    put(nemonic, op9, #name, 8);                                               \
  }

#define PUT10(name, nm, op_1, op_2, op_3, op_4, op_5, op_6, op_7, op_8, op_9,  \
              op_10)                                                           \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    std::vector<uint64_t> op6(op_6);                                           \
    std::vector<uint64_t> op7(op_7);                                           \
    std::vector<uint64_t> op8(op_8);                                           \
    std::vector<uint64_t> op9(op_9);                                           \
    std::vector<uint64_t> op10(op_10);                                         \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
    put(nemonic, op6, #name, 5);                                               \
    put(nemonic, op7, #name, 6);                                               \
    put(nemonic, op8, #name, 7);                                               \
    put(nemonic, op9, #name, 8);                                               \
    put(nemonic, op10, #name, 9);                                              \
  }

#define PUT16(name, nm, op_1, op_2, op_3, op_4, op_5, op_6, op_7, op_8, op_9,  \
              op_10, op_11, op_12, op_13, op_14, op_15, op_16)                 \
  void put##name() const {                                                     \
    std::vector<std::string> nemonic(nm);                                      \
    std::vector<uint64_t> op1(op_1);                                           \
    std::vector<uint64_t> op2(op_2);                                           \
    std::vector<uint64_t> op3(op_3);                                           \
    std::vector<uint64_t> op4(op_4);                                           \
    std::vector<uint64_t> op5(op_5);                                           \
    std::vector<uint64_t> op6(op_6);                                           \
    std::vector<uint64_t> op7(op_7);                                           \
    std::vector<uint64_t> op8(op_8);                                           \
    std::vector<uint64_t> op9(op_9);                                           \
    std::vector<uint64_t> op10(op_10);                                         \
    std::vector<uint64_t> op11(op_11);                                         \
    std::vector<uint64_t> op12(op_12);                                         \
    std::vector<uint64_t> op13(op_13);                                         \
    std::vector<uint64_t> op14(op_14);                                         \
    std::vector<uint64_t> op15(op_15);                                         \
    std::vector<uint64_t> op16(op_16);                                         \
    put(nemonic, op1, #name, 0);                                               \
    put(nemonic, op2, #name, 1);                                               \
    put(nemonic, op3, #name, 2);                                               \
    put(nemonic, op4, #name, 3);                                               \
    put(nemonic, op5, #name, 4);                                               \
    put(nemonic, op6, #name, 5);                                               \
    put(nemonic, op7, #name, 6);                                               \
    put(nemonic, op8, #name, 7);                                               \
    put(nemonic, op9, #name, 8);                                               \
    put(nemonic, op10, #name, 9);                                              \
    put(nemonic, op11, #name, 10);                                             \
    put(nemonic, op12, #name, 11);                                             \
    put(nemonic, op13, #name, 12);                                             \
    put(nemonic, op14, #name, 13);                                             \
    put(nemonic, op15, #name, 14);                                             \
    put(nemonic, op16, #name, 15);                                             \
  }

#define OPS(...)                                                               \
  { __VA_ARGS__ }
#define NM(...)                                                                \
  { __VA_ARGS__ }

class Test {
  Test(const Test &);
  void operator=(const Test &);
  const bool isAsmjit_;
  int funcNum_;

  /** String used as test vector */

  //std::vector<std::string> tv_WREG = {"$zero", "$t1", "$v0", "$a0", "$a4", "$s0", "$ra", "$a7", "$s3", "$t6"};
  std::vector<std::string> tv_WREG = {"$zero", "$t1", "$a0", "$a0", "$a4", "$s0", "$ra", "$a7", "$s3", "$t6"};
  std::vector<std::string> tv_XREG = {"$f1", "$f2", "$f5", "$f7", "$f8", "$f9", "$f20", "$f31"};
  std::vector<std::string> tv_XREG2 = {"$f4", "$f12", "$f14", "$f15", "$f19", "$f22", "$f25", "$f26"};
  std::vector<std::string> tv_WSP = {"$sp"};
  std::vector<std::string> tv_XSP = {"$sp"};
  std::vector<std::string> tv_XNSP = {"x7", "x0",  "x1",  "x2", "x4",
                                      "x8", "x16", "x30", "sp"};
  std::vector<std::string> tv_XNSP2 = {"x3", "x0",  "x1",  "x2", "x4",
                                       "x8", "x16", "x30", "sp"};
  std::vector<std::string> tv_XNSP3 = {"x5", "x15", "x29"};
  std::vector<std::string> tv_WNZR = {"w7", "w0",  "w1",  "w2", "w4",
                                      "w8", "w16", "w30", "wzr"};
  std::vector<std::string> tv_XNZR = {"x3", "x0",  "x1",  "x2", "x4",
                                      "x8", "x16", "x30", "xzr"};
  std::vector<std::string> tv_IMM1BIT = {"1", "0"};
  std::vector<std::string> tv_IMM2BIT = {"1", "0", "1", "2"};
  std::vector<std::string> tv_IMM3BIT = {"3", "0", "1", "2", "4", "7"};
  std::vector<std::string> jtv_IMM3BIT_FCC = {"3", "0", "1", "2", "4", "7"};
  std::vector<std::string> tv_IMM3BIT_FCC = {"$fcc3", "$fcc0", "$fcc1", "$fcc2", "$fcc4", "$fcc7"};
  std::vector<std::string> tv_IMM4BIT = {"7", "0", "1", "2", "4", "8", "15"};
  std::vector<std::string> tv_IMM5BIT = {"7", "0", "1",  "2",
                                         "4", "8", "16", "31"};
  std::vector<std::string> tv_IMM6BIT = {"7", "0",  "1",  "2", "4",
                                         "8", "16", "32", "63"};
  std::vector<std::string> tv_IMM7BIT = {"63", "0",  "1",  "2",  "4",
                                         "8",  "16", "32", "64", "127"};
  std::vector<std::string> tv_IMM16BIT = {
      "0x2aa", "0",   "1",   "2",   "4",    "-8",    "16",  "32",
      "64",    "128", "256", "-512", "1024", "2048", "-4095"};
  std::vector<std::string> tv_UIMM16BIT = {
      "0xe38e", "0",    "1",     "2",     "4",        "8",    "16",
      "32",     "64",   "128",   "256",   "512",      "1024", "2048",
      "4096",   "8191", "1<<14", "1<<15", "(1<<16)-1"};
  std::vector<std::string> tv_IMM9BIT_PM = {
      "127", "-256", "-128", "-64", "-32", "-16", "-8", "-4", "-2",  "-1",
      "0",   "1",    "2",    "4",   "8",   "16",  "32", "64", "128", "255"};
  std::vector<std::string> tv_IMM7BIT_MUL4, tv_IMM7BIT_MUL8, tv_IMM7BIT_MUL16;
  std::vector<std::string> tv_IMM7BIT_PM_MUL4, tv_IMM7BIT_PM_MUL8,
      tv_IMM7BIT_PM_MUL16;
  std::vector<std::string> tv_IMM10BIT_PM_MUL8;
  std::vector<std::string> tv_IMM12BIT_MUL2, tv_IMM12BIT_MUL4, tv_IMM12BIT_MUL8,
      tv_IMM12BIT_MUL16;
  std::vector<std::string> tv_IMM14BIT_MUL4;
  std::vector<std::string> tv_IMM16BIT_MUL4;
  std::vector<std::string> tv_IMM20BIT_MUL4;
  std::vector<std::string> tv_IMM21BIT_MUL4;
  std::vector<std::string> tv_IMM26BIT_MUL4;

  std::vector<std::string> tv_BREG = {"b7", "b0", "b1",  "b2",
                                      "b4", "b8", "b16", "b31"};
  std::vector<std::string> tv_HREG = {"h7", "h0", "h1",  "h2",
                                      "h4", "h8", "h16", "h31"};
  std::vector<std::string> tv_SREG = {"s7", "s0", "s1",  "s2",
                                      "s4", "s8", "s16", "s31"};
  std::vector<std::string> tv_DREG = {"d7", "d0", "d1",  "d2",
                                      "d4", "d8", "d16", "d31"};
  std::vector<std::string> tv_QREG = {"q7", "q0", "q1",  "q2",
                                      "q4", "q8", "q16", "q31"};

  std::vector<std::string> tv_SPECIFIC32, tv_SPECIFIC64, tv_SPECIFIC32_1,
      tv_SPECIFIC64_1, tv_SPECIFIC32_2, tv_SPECIFIC64_2, tv_SPECIFIC32_3,
      tv_SPECIFIC64_3;
  std::vector<std::string> jtv_SPECIFIC32, jtv_SPECIFIC64, jtv_SPECIFIC32_1,
      jtv_SPECIFIC64_1, jtv_SPECIFIC32_2, jtv_SPECIFIC64_2, jtv_SPECIFIC32_3,
      jtv_SPECIFIC64_3;

  std::vector<std::string> tv_SPECIFIC0, tv_SPECIFIC1, tv_SPECIFIC2,
      tv_SPECIFIC3, tv_SPECIFIC4, tv_SPECIFIC5, tv_SPECIFIC6, tv_SPECIFIC7,
      tv_SPECIFIC8, tv_SPECIFIC9, tv_SPECIFIC10, tv_SPECIFIC11, tv_SPECIFIC12,
      tv_SPECIFIC13, tv_SPECIFIC14, tv_SPECIFIC15, tv_SPECIFIC16, tv_SPECIFIC17;
  std::vector<std::string> jtv_SPECIFIC0, jtv_SPECIFIC1, jtv_SPECIFIC2,
      jtv_SPECIFIC3, jtv_SPECIFIC4, jtv_SPECIFIC5, jtv_SPECIFIC6, jtv_SPECIFIC7,
      jtv_SPECIFIC8, jtv_SPECIFIC9, jtv_SPECIFIC10, jtv_SPECIFIC11,
      jtv_SPECIFIC12, jtv_SPECIFIC13, jtv_SPECIFIC14, jtv_SPECIFIC15,
      jtv_SPECIFIC16, jtv_SPECIFIC17;

  std::vector<std::string> tv_PTR_O = {"["};
  std::vector<std::string> tv_PTR_C = {"]"};
  std::vector<std::string> jtv_PTR_O = {"ptr("};
  std::vector<std::string> jtv_PTR_C = {")"};

  std::vector<std::string> tv_BRA_O = {"{"};
  std::vector<std::string> tv_BRA_C = {"}"};
  std::vector<std::string> jtv_BRA_O = {""};
  std::vector<std::string> jtv_BRA_C = {""};

  std::vector<std::string> tv_PAR_O = {"("};
  std::vector<std::string> tv_PAR_C = {")"};
  std::vector<std::string> jtv_PAR_O = {""};
  std::vector<std::string> jtv_PAR_C = {""};

  std::vector<std::string> tv_PRE_PTR_O = {"["};
  std::vector<std::string> tv_PRE_PTR_C = {"]!"};
  std::vector<std::string> jtv_PRE_PTR_O = {"pre_ptr("};
  std::vector<std::string> jtv_PRE_PTR_C = {")"};

  std::vector<std::string> tv_LSL = {"LSL"};

  std::vector<std::string> tv_UXTW = {"UXTW", "UXTW"};
  std::vector<std::string> tv_SXTW = {"SXTW", "SXTW"};
  std::vector<std::string> tv_SXTX = {"SXTX", "SXTX"};
  std::vector<std::string> jtv_UXTW = {"UXT", "UXTW"};
  std::vector<std::string> jtv_SXTW = {"SXT", "SXTW"};
  std::vector<std::string> jtv_SXTX = {"SXT", "SXTX"};

  std::vector<std::string> tv_IMM_0 = {"0"};
  std::vector<std::string> tv_IMM_0_OR_1 = {"1", "0"};
  std::vector<std::string> tv_IMM_0_OR_2 = {"2", "0"};
  std::vector<std::string> tv_IMM_0_OR_3 = {"3", "0"};
  std::vector<std::string> tv_IMM_0_OR_4 = {"4", "0"};

  std::vector<std::string> tv_IMM_2 = {"2"};
  std::vector<std::string> tv_IMM_4 = {"4"};
  std::vector<std::string> tv_IMM_8 = {"8"};
  std::vector<std::string> tv_IMM_16 = {"16"};
  std::vector<std::string> tv_IMM_8_OR_16 = {"16", "8"};
  std::vector<std::string> tv_IMM_16_OR_32 = {"32", "16"};
  std::vector<std::string> tv_IMM_24_OR_48 = {"48", "24"};
  std::vector<std::string> tv_IMM_32_OR_64 = {"64", "32"};

  std::vector<std::string> tv_VREGB_1D, tv_VREGH_1D, tv_VREGS_1D, tv_VREGD_1D,
      tv_VREGB_2D, tv_VREGH_2D, tv_VREGS_2D, tv_VREGD_2D, tv_VREGB_3D,
      tv_VREGH_3D, tv_VREGS_3D, tv_VREGD_3D, tv_VREGB_4D, tv_VREGH_4D,
      tv_VREGS_4D, tv_VREGD_4D;
  std::vector<std::string> tv_VREG16B_1D, tv_VREG8B_1D, tv_VREG8H_1D,
      tv_VREG4H_1D, tv_VREG4S_1D, tv_VREG2S_1D, tv_VREG2D_1D, tv_VREG1D_1D,
      tv_VREG16B_2D, tv_VREG8B_2D, tv_VREG8H_2D, tv_VREG4H_2D, tv_VREG4S_2D,
      tv_VREG2S_2D, tv_VREG2D_2D, tv_VREG1D_2D, tv_VREG16B_3D, tv_VREG8B_3D,
      tv_VREG8H_3D, tv_VREG4H_3D, tv_VREG4S_3D, tv_VREG2S_3D, tv_VREG2D_3D,
      tv_VREG1D_3D, tv_VREG16B_4D, tv_VREG8B_4D, tv_VREG8H_4D, tv_VREG4H_4D,
      tv_VREG4S_4D, tv_VREG2S_4D, tv_VREG2D_4D, tv_VREG1D_4D;
  std::vector<std::string> tv_VREGB_1D_ELEM, tv_VREGH_1D_ELEM, tv_VREGS_1D_ELEM,
      tv_VREGD_1D_ELEM, tv_VREGB_2D_ELEM, tv_VREGH_2D_ELEM, tv_VREGS_2D_ELEM,
      tv_VREGD_2D_ELEM, tv_VREGB_3D_ELEM, tv_VREGH_3D_ELEM, tv_VREGS_3D_ELEM,
      tv_VREGD_3D_ELEM, tv_VREGB_4D_ELEM, tv_VREGH_4D_ELEM, tv_VREGS_4D_ELEM,
      tv_VREGD_4D_ELEM;
  std::vector<std::string> tv_VREG16B_1D_ELEM, tv_VREG8B_1D_ELEM,
      tv_VREG8H_1D_ELEM, tv_VREG4H_1D_ELEM, tv_VREG4S_1D_ELEM,
      tv_VREG2S_1D_ELEM, tv_VREG2D_1D_ELEM, tv_VREG1D_1D_ELEM,
      tv_VREG16B_2D_ELEM, tv_VREG8B_2D_ELEM, tv_VREG8H_2D_ELEM,
      tv_VREG4H_2D_ELEM, tv_VREG4S_2D_ELEM, tv_VREG2S_2D_ELEM,
      tv_VREG2D_2D_ELEM, tv_VREG1D_2D_ELEM, tv_VREG16B_3D_ELEM,
      tv_VREG8B_3D_ELEM, tv_VREG8H_3D_ELEM, tv_VREG4H_3D_ELEM,
      tv_VREG4S_3D_ELEM, tv_VREG2S_3D_ELEM, tv_VREG2D_3D_ELEM,
      tv_VREG1D_3D_ELEM, tv_VREG16B_4D_ELEM, tv_VREG8B_4D_ELEM,
      tv_VREG8H_4D_ELEM, tv_VREG4H_4D_ELEM, tv_VREG4S_4D_ELEM,
      tv_VREG2S_4D_ELEM, tv_VREG2D_4D_ELEM, tv_VREG1D_4D_ELEM;
  std::vector<std::string> jtv_VREGB_1D, jtv_VREGH_1D, jtv_VREGS_1D,
      jtv_VREGD_1D, jtv_VREGB_2D, jtv_VREGH_2D, jtv_VREGS_2D, jtv_VREGD_2D,
      jtv_VREGB_3D, jtv_VREGH_3D, jtv_VREGS_3D, jtv_VREGD_3D, jtv_VREGB_4D,
      jtv_VREGH_4D, jtv_VREGS_4D, jtv_VREGD_4D;
  std::vector<std::string> jtv_VREG16B_1D, jtv_VREG8B_1D, jtv_VREG8H_1D,
      jtv_VREG4H_1D, jtv_VREG4S_1D, jtv_VREG2S_1D, jtv_VREG2D_1D, jtv_VREG1D_1D,
      jtv_VREG16B_2D, jtv_VREG8B_2D, jtv_VREG8H_2D, jtv_VREG4H_2D,
      jtv_VREG4S_2D, jtv_VREG2S_2D, jtv_VREG2D_2D, jtv_VREG1D_2D,
      jtv_VREG16B_3D, jtv_VREG8B_3D, jtv_VREG8H_3D, jtv_VREG4H_3D,
      jtv_VREG4S_3D, jtv_VREG2S_3D, jtv_VREG2D_3D, jtv_VREG1D_3D,
      jtv_VREG16B_4D, jtv_VREG8B_4D, jtv_VREG8H_4D, jtv_VREG4H_4D,
      jtv_VREG4S_4D, jtv_VREG2S_4D, jtv_VREG2D_4D, jtv_VREG1D_4D;
  std::vector<std::string> jtv_VREGB_1D_ELEM, jtv_VREGH_1D_ELEM,
      jtv_VREGS_1D_ELEM, jtv_VREGD_1D_ELEM, jtv_VREGB_2D_ELEM,
      jtv_VREGH_2D_ELEM, jtv_VREGS_2D_ELEM, jtv_VREGD_2D_ELEM,
      jtv_VREGB_3D_ELEM, jtv_VREGH_3D_ELEM, jtv_VREGS_3D_ELEM,
      jtv_VREGD_3D_ELEM, jtv_VREGB_4D_ELEM, jtv_VREGH_4D_ELEM,
      jtv_VREGS_4D_ELEM, jtv_VREGD_4D_ELEM;
  std::vector<std::string> jtv_VREG16B_1D_ELEM, jtv_VREG8B_1D_ELEM,
      jtv_VREG8H_1D_ELEM, jtv_VREG4H_1D_ELEM, jtv_VREG4S_1D_ELEM,
      jtv_VREG2S_1D_ELEM, jtv_VREG2D_1D_ELEM, jtv_VREG1D_1D_ELEM,
      jtv_VREG16B_2D_ELEM, jtv_VREG8B_2D_ELEM, jtv_VREG8H_2D_ELEM,
      jtv_VREG4H_2D_ELEM, jtv_VREG4S_2D_ELEM, jtv_VREG2S_2D_ELEM,
      jtv_VREG2D_2D_ELEM, jtv_VREG1D_2D_ELEM, jtv_VREG16B_3D_ELEM,
      jtv_VREG8B_3D_ELEM, jtv_VREG8H_3D_ELEM, jtv_VREG4H_3D_ELEM,
      jtv_VREG4S_3D_ELEM, jtv_VREG2S_3D_ELEM, jtv_VREG2D_3D_ELEM,
      jtv_VREG1D_3D_ELEM, jtv_VREG16B_4D_ELEM, jtv_VREG8B_4D_ELEM,
      jtv_VREG8H_4D_ELEM, jtv_VREG4H_4D_ELEM, jtv_VREG4S_4D_ELEM,
      jtv_VREG2S_4D_ELEM, jtv_VREG2D_4D_ELEM, jtv_VREG1D_4D_ELEM;

  std::vector<std::string> tv_PRFOP = {
      "PLDL1KEEP", "PLDL1STRM", "PLDL2KEEP", "PLDL2STRM", "PLDL3KEEP",
      "PLDL3STRM", "PSTL1KEEP", "PSTL1STRM", "PSTL2KEEP", "PSTL2STRM",
      "PSTL3KEEP", "PSTL3STRM", "PLIL1KEEP", "PLIL1STRM", "PLIL2KEEP",
      "PLIL2STRM", "PLIL3KEEP", "PLIL3STRM"};
  std::vector<std::string> tv_BARRIER = {"SY",    "ST",    "LD",    "ISH",
                                         "ISHST", "ISHLD", "NSH",   "NSHST",
                                         "NSHLD", "OSH",   "OSHST", "OSHLD"};

  std::vector<std::vector<std::string> *> tv_VectorsAs = {&tv_WREG,
                                                          &tv_XREG,
                                                          &tv_XREG2,
                                                          &tv_WSP,
                                                          &tv_XSP,
                                                          &tv_XNSP,
                                                          &tv_XNSP2,
                                                          &tv_XNSP3,
                                                          &tv_WNZR,
                                                          &tv_XNZR,
                                                          &tv_IMM1BIT,
                                                          &tv_IMM2BIT,
                                                          &tv_IMM3BIT,
                                                          &tv_IMM3BIT_FCC,
                                                          &tv_IMM4BIT,
                                                          &tv_IMM5BIT,
                                                          &tv_IMM6BIT,
                                                          &tv_IMM7BIT,
                                                          &tv_IMM16BIT,
                                                          &tv_UIMM16BIT,
                                                          &tv_IMM9BIT_PM,
                                                          &tv_IMM7BIT_MUL4,
                                                          &tv_IMM7BIT_MUL8,
                                                          &tv_IMM7BIT_MUL16,
                                                          &tv_IMM7BIT_PM_MUL4,
                                                          &tv_IMM7BIT_PM_MUL8,
                                                          &tv_IMM7BIT_PM_MUL16,
                                                          &tv_IMM10BIT_PM_MUL8,
                                                          &tv_IMM12BIT_MUL2,
                                                          &tv_IMM12BIT_MUL4,
                                                          &tv_IMM12BIT_MUL8,
                                                          &tv_IMM12BIT_MUL16,
                                                          &tv_IMM14BIT_MUL4,
                                                          &tv_IMM16BIT_MUL4,
                                                          &tv_IMM20BIT_MUL4,
                                                          &tv_IMM21BIT_MUL4,
                                                          &tv_IMM26BIT_MUL4,
                                                          &tv_BREG,
                                                          &tv_HREG,
                                                          &tv_SREG,
                                                          &tv_DREG,
                                                          &tv_QREG,
                                                          &tv_SPECIFIC32,
                                                          &tv_SPECIFIC64,
                                                          &tv_SPECIFIC32_1,
                                                          &tv_SPECIFIC64_1,
                                                          &tv_SPECIFIC32_2,
                                                          &tv_SPECIFIC64_2,
                                                          &tv_SPECIFIC32_3,
                                                          &tv_SPECIFIC64_3,
                                                          &tv_SPECIFIC0,
                                                          &tv_SPECIFIC1,
                                                          &tv_SPECIFIC2,
                                                          &tv_SPECIFIC3,
                                                          &tv_SPECIFIC4,
                                                          &tv_SPECIFIC5,
                                                          &tv_SPECIFIC6,
                                                          &tv_SPECIFIC7,
                                                          &tv_SPECIFIC8,
                                                          &tv_SPECIFIC9,
                                                          &tv_SPECIFIC10,
                                                          &tv_SPECIFIC11,
                                                          &tv_SPECIFIC12,
                                                          &tv_SPECIFIC13,
                                                          &tv_SPECIFIC14,
                                                          &tv_SPECIFIC15,
                                                          &tv_SPECIFIC16,
                                                          &tv_SPECIFIC17,
                                                          &tv_PTR_O,
                                                          &tv_PTR_C,
                                                          &tv_BRA_O,
                                                          &tv_BRA_C,
                                                          &tv_PAR_O,
                                                          &tv_PAR_C,
                                                          &tv_PRE_PTR_O,
                                                          &tv_PRE_PTR_C,
                                                          &tv_LSL,
                                                          &tv_UXTW,
                                                          &tv_SXTW,
                                                          &tv_SXTX,
                                                          &tv_IMM_0,
                                                          &tv_IMM_0_OR_1,
                                                          &tv_IMM_0_OR_2,
                                                          &tv_IMM_0_OR_3,
                                                          &tv_IMM_0_OR_4,
                                                          &tv_IMM_2,
                                                          &tv_IMM_4,
                                                          &tv_IMM_8,
                                                          &tv_IMM_16,
                                                          &tv_IMM_8_OR_16,
                                                          &tv_IMM_16_OR_32,
                                                          &tv_IMM_24_OR_48,
                                                          &tv_IMM_32_OR_64,
                                                          &tv_VREGB_1D,
                                                          &tv_VREGH_1D,
                                                          &tv_VREGS_1D,
                                                          &tv_VREGD_1D,
                                                          &tv_VREGB_2D,
                                                          &tv_VREGH_2D,
                                                          &tv_VREGS_2D,
                                                          &tv_VREGD_2D,
                                                          &tv_VREGB_3D,
                                                          &tv_VREGH_3D,
                                                          &tv_VREGS_3D,
                                                          &tv_VREGD_3D,
                                                          &tv_VREGB_4D,
                                                          &tv_VREGH_4D,
                                                          &tv_VREGS_4D,
                                                          &tv_VREGD_4D,
                                                          &tv_VREG16B_1D,
                                                          &tv_VREG8B_1D,
                                                          &tv_VREG8H_1D,
                                                          &tv_VREG4H_1D,
                                                          &tv_VREG4S_1D,
                                                          &tv_VREG2S_1D,
                                                          &tv_VREG2D_1D,
                                                          &tv_VREG1D_1D,
                                                          &tv_VREG16B_2D,
                                                          &tv_VREG8B_2D,
                                                          &tv_VREG8H_2D,
                                                          &tv_VREG4H_2D,
                                                          &tv_VREG4S_2D,
                                                          &tv_VREG2S_2D,
                                                          &tv_VREG2D_2D,
                                                          &tv_VREG1D_2D,
                                                          &tv_VREG16B_3D,
                                                          &tv_VREG8B_3D,
                                                          &tv_VREG8H_3D,
                                                          &tv_VREG4H_3D,
                                                          &tv_VREG4S_3D,
                                                          &tv_VREG2S_3D,
                                                          &tv_VREG2D_3D,
                                                          &tv_VREG1D_3D,
                                                          &tv_VREG16B_4D,
                                                          &tv_VREG8B_4D,
                                                          &tv_VREG8H_4D,
                                                          &tv_VREG4H_4D,
                                                          &tv_VREG4S_4D,
                                                          &tv_VREG2S_4D,
                                                          &tv_VREG2D_4D,
                                                          &tv_VREG1D_4D,
                                                          &tv_VREGB_1D_ELEM,
                                                          &tv_VREGH_1D_ELEM,
                                                          &tv_VREGS_1D_ELEM,
                                                          &tv_VREGD_1D_ELEM,
                                                          &tv_VREGB_2D_ELEM,
                                                          &tv_VREGH_2D_ELEM,
                                                          &tv_VREGS_2D_ELEM,
                                                          &tv_VREGD_2D_ELEM,
                                                          &tv_VREGB_3D_ELEM,
                                                          &tv_VREGH_3D_ELEM,
                                                          &tv_VREGS_3D_ELEM,
                                                          &tv_VREGD_3D_ELEM,
                                                          &tv_VREGB_4D_ELEM,
                                                          &tv_VREGH_4D_ELEM,
                                                          &tv_VREGS_4D_ELEM,
                                                          &tv_VREGD_4D_ELEM,
                                                          &tv_VREG16B_1D_ELEM,
                                                          &tv_VREG8B_1D_ELEM,
                                                          &tv_VREG8H_1D_ELEM,
                                                          &tv_VREG4H_1D_ELEM,
                                                          &tv_VREG4S_1D_ELEM,
                                                          &tv_VREG2S_1D_ELEM,
                                                          &tv_VREG2D_1D_ELEM,
                                                          &tv_VREG1D_1D_ELEM,
                                                          &tv_VREG16B_2D_ELEM,
                                                          &tv_VREG8B_2D_ELEM,
                                                          &tv_VREG8H_2D_ELEM,
                                                          &tv_VREG4H_2D_ELEM,
                                                          &tv_VREG4S_2D_ELEM,
                                                          &tv_VREG2S_2D_ELEM,
                                                          &tv_VREG2D_2D_ELEM,
                                                          &tv_VREG1D_2D_ELEM,
                                                          &tv_VREG16B_3D_ELEM,
                                                          &tv_VREG8B_3D_ELEM,
                                                          &tv_VREG8H_3D_ELEM,
                                                          &tv_VREG4H_3D_ELEM,
                                                          &tv_VREG4S_3D_ELEM,
                                                          &tv_VREG2S_3D_ELEM,
                                                          &tv_VREG2D_3D_ELEM,
                                                          &tv_VREG1D_3D_ELEM,
                                                          &tv_VREG16B_4D_ELEM,
                                                          &tv_VREG8B_4D_ELEM,
                                                          &tv_VREG8H_4D_ELEM,
                                                          &tv_VREG4H_4D_ELEM,
                                                          &tv_VREG4S_4D_ELEM,
                                                          &tv_VREG2S_4D_ELEM,
                                                          &tv_VREG2D_4D_ELEM,
                                                          &tv_VREG1D_4D_ELEM,
                                                          &tv_PRFOP,
                                                          &tv_BARRIER};
  std::vector<std::vector<std::string> *> tv_VectorsJit = {&tv_WREG,
                                                           &tv_XREG,
                                                           &tv_XREG2,
                                                           &tv_WSP,
                                                           &tv_XSP,
                                                           &tv_XNSP,
                                                           &tv_XNSP2,
                                                           &tv_XNSP3,
                                                           &tv_WNZR,
                                                           &tv_XNZR,
                                                           &tv_IMM1BIT,
                                                           &tv_IMM2BIT,
                                                           &tv_IMM3BIT,
                                                           &jtv_IMM3BIT_FCC,
                                                           &tv_IMM4BIT,
                                                           &tv_IMM5BIT,
                                                           &tv_IMM6BIT,
                                                           &tv_IMM7BIT,
                                                           &tv_IMM16BIT,
                                                           &tv_UIMM16BIT,
                                                           &tv_IMM9BIT_PM,
                                                           &tv_IMM7BIT_MUL4,
                                                           &tv_IMM7BIT_MUL8,
                                                           &tv_IMM7BIT_MUL16,
                                                           &tv_IMM7BIT_PM_MUL4,
                                                           &tv_IMM7BIT_PM_MUL8,
                                                           &tv_IMM7BIT_PM_MUL16,
                                                           &tv_IMM10BIT_PM_MUL8,
                                                           &tv_IMM12BIT_MUL2,
                                                           &tv_IMM12BIT_MUL4,
                                                           &tv_IMM12BIT_MUL8,
                                                           &tv_IMM12BIT_MUL16,
                                                           &tv_IMM14BIT_MUL4,
                                                           &tv_IMM16BIT_MUL4,
                                                           &tv_IMM20BIT_MUL4,
                                                           &tv_IMM21BIT_MUL4,
                                                           &tv_IMM26BIT_MUL4,
                                                           &tv_BREG,
                                                           &tv_HREG,
                                                           &tv_SREG,
                                                           &tv_DREG,
                                                           &tv_QREG,
                                                           &jtv_SPECIFIC32,
                                                           &jtv_SPECIFIC64,
                                                           &jtv_SPECIFIC32_1,
                                                           &jtv_SPECIFIC64_1,
                                                           &jtv_SPECIFIC32_2,
                                                           &jtv_SPECIFIC64_2,
                                                           &jtv_SPECIFIC32_3,
                                                           &jtv_SPECIFIC64_3,
                                                           &jtv_SPECIFIC0,
                                                           &jtv_SPECIFIC1,
                                                           &jtv_SPECIFIC2,
                                                           &jtv_SPECIFIC3,
                                                           &jtv_SPECIFIC4,
                                                           &jtv_SPECIFIC5,
                                                           &jtv_SPECIFIC6,
                                                           &jtv_SPECIFIC7,
                                                           &jtv_SPECIFIC8,
                                                           &jtv_SPECIFIC9,
                                                           &jtv_SPECIFIC10,
                                                           &jtv_SPECIFIC11,
                                                           &jtv_SPECIFIC12,
                                                           &jtv_SPECIFIC13,
                                                           &jtv_SPECIFIC14,
                                                           &jtv_SPECIFIC15,
                                                           &jtv_SPECIFIC16,
                                                           &jtv_SPECIFIC17,
                                                           &jtv_PTR_O,
                                                           &jtv_PTR_C,
                                                           &jtv_BRA_O,
                                                           &jtv_BRA_C,
                                                           &jtv_PAR_O,
                                                           &jtv_PAR_C,
                                                           &jtv_PRE_PTR_O,
                                                           &jtv_PRE_PTR_C,
                                                           &tv_LSL,
                                                           &jtv_UXTW,
                                                           &jtv_SXTW,
                                                           &jtv_SXTX,
                                                           &tv_IMM_0,
                                                           &tv_IMM_0_OR_1,
                                                           &tv_IMM_0_OR_2,
                                                           &tv_IMM_0_OR_3,
                                                           &tv_IMM_0_OR_4,
                                                           &tv_IMM_2,
                                                           &tv_IMM_4,
                                                           &tv_IMM_8,
                                                           &tv_IMM_16,
                                                           &tv_IMM_8_OR_16,
                                                           &tv_IMM_16_OR_32,
                                                           &tv_IMM_24_OR_48,
                                                           &tv_IMM_32_OR_64,
                                                           &jtv_VREGB_1D,
                                                           &jtv_VREGH_1D,
                                                           &jtv_VREGS_1D,
                                                           &jtv_VREGD_1D,
                                                           &jtv_VREGB_2D,
                                                           &jtv_VREGH_2D,
                                                           &jtv_VREGS_2D,
                                                           &jtv_VREGD_2D,
                                                           &jtv_VREGB_3D,
                                                           &jtv_VREGH_3D,
                                                           &jtv_VREGS_3D,
                                                           &jtv_VREGD_3D,
                                                           &jtv_VREGB_4D,
                                                           &jtv_VREGH_4D,
                                                           &jtv_VREGS_4D,
                                                           &jtv_VREGD_4D,
                                                           &jtv_VREG16B_1D,
                                                           &jtv_VREG8B_1D,
                                                           &jtv_VREG8H_1D,
                                                           &jtv_VREG4H_1D,
                                                           &jtv_VREG4S_1D,
                                                           &jtv_VREG2S_1D,
                                                           &jtv_VREG2D_1D,
                                                           &jtv_VREG1D_1D,
                                                           &jtv_VREG16B_2D,
                                                           &jtv_VREG8B_2D,
                                                           &jtv_VREG8H_2D,
                                                           &jtv_VREG4H_2D,
                                                           &jtv_VREG4S_2D,
                                                           &jtv_VREG2S_2D,
                                                           &jtv_VREG2D_2D,
                                                           &jtv_VREG1D_2D,
                                                           &jtv_VREG16B_3D,
                                                           &jtv_VREG8B_3D,
                                                           &jtv_VREG8H_3D,
                                                           &jtv_VREG4H_3D,
                                                           &jtv_VREG4S_3D,
                                                           &jtv_VREG2S_3D,
                                                           &jtv_VREG2D_3D,
                                                           &jtv_VREG1D_3D,
                                                           &jtv_VREG16B_4D,
                                                           &jtv_VREG8B_4D,
                                                           &jtv_VREG8H_4D,
                                                           &jtv_VREG4H_4D,
                                                           &jtv_VREG4S_4D,
                                                           &jtv_VREG2S_4D,
                                                           &jtv_VREG2D_4D,
                                                           &jtv_VREG1D_4D,
                                                           &jtv_VREGB_1D_ELEM,
                                                           &jtv_VREGH_1D_ELEM,
                                                           &jtv_VREGS_1D_ELEM,
                                                           &jtv_VREGD_1D_ELEM,
                                                           &jtv_VREGB_2D_ELEM,
                                                           &jtv_VREGH_2D_ELEM,
                                                           &jtv_VREGS_2D_ELEM,
                                                           &jtv_VREGD_2D_ELEM,
                                                           &jtv_VREGB_3D_ELEM,
                                                           &jtv_VREGH_3D_ELEM,
                                                           &jtv_VREGS_3D_ELEM,
                                                           &jtv_VREGD_3D_ELEM,
                                                           &jtv_VREGB_4D_ELEM,
                                                           &jtv_VREGH_4D_ELEM,
                                                           &jtv_VREGS_4D_ELEM,
                                                           &jtv_VREGD_4D_ELEM,
                                                           &jtv_VREG16B_1D_ELEM,
                                                           &jtv_VREG8B_1D_ELEM,
                                                           &jtv_VREG8H_1D_ELEM,
                                                           &jtv_VREG4H_1D_ELEM,
                                                           &jtv_VREG4S_1D_ELEM,
                                                           &jtv_VREG2S_1D_ELEM,
                                                           &jtv_VREG2D_1D_ELEM,
                                                           &jtv_VREG1D_1D_ELEM,
                                                           &jtv_VREG16B_2D_ELEM,
                                                           &jtv_VREG8B_2D_ELEM,
                                                           &jtv_VREG8H_2D_ELEM,
                                                           &jtv_VREG4H_2D_ELEM,
                                                           &jtv_VREG4S_2D_ELEM,
                                                           &jtv_VREG2S_2D_ELEM,
                                                           &jtv_VREG2D_2D_ELEM,
                                                           &jtv_VREG1D_2D_ELEM,
                                                           &jtv_VREG16B_3D_ELEM,
                                                           &jtv_VREG8B_3D_ELEM,
                                                           &jtv_VREG8H_3D_ELEM,
                                                           &jtv_VREG4H_3D_ELEM,
                                                           &jtv_VREG4S_3D_ELEM,
                                                           &jtv_VREG2S_3D_ELEM,
                                                           &jtv_VREG2D_3D_ELEM,
                                                           &jtv_VREG1D_3D_ELEM,
                                                           &jtv_VREG16B_4D_ELEM,
                                                           &jtv_VREG8B_4D_ELEM,
                                                           &jtv_VREG8H_4D_ELEM,
                                                           &jtv_VREG4H_4D_ELEM,
                                                           &jtv_VREG4S_4D_ELEM,
                                                           &jtv_VREG2S_4D_ELEM,
                                                           &jtv_VREG2D_4D_ELEM,
                                                           &jtv_VREG1D_4D_ELEM,
                                                           &tv_PRFOP,
                                                           &tv_BARRIER};

  std::vector<std::vector<std::string> *> &tv_Vectors = tv_VectorsAs;

  bool isRequireComma(int num_ops, int idx, std::string curr,
                      std::string next) const {
    if (idx == num_ops - 1) { /** Last operand isn't followed by ",". */
      return false;
    }

    if (curr == "" || curr == "[" || curr == "ptr(" || curr == "{" ||
        curr == "(" || curr == "pre_ptr(" || curr == "post_ptr(") {
      return false;
    }

    if (next == "]" || next == ")" || next == "}" || next == "]!") {
      return false;
    }

    if (curr == "LSL" || curr == "SXTW" || curr == "SXTX" || curr == "UXTW" ||
        curr == "UXTX") {
      if (!isAsmjit_) {
        return false;
      }
    }

    return true;
  }

  /*
    and_, or_, xor_, not_ => and, or, xor, not
  */
  std::string removeUnderScore(std::string s) const {
    if (!isAsmjit_ && s[s.size() - 1] == '_')
      s.resize(s.size() - 1);
    return s;
  }

  std::string replaceDotWithUnderScore(std::string s) const {
    if(!isAsmjit_) return s;

    for(uint32_t i=0; i<s.size(); i++)
    {
      if (s[i] == '.')
        s[i] = '_';
    }

    return s;
  }

  void put(std::vector<std::string> &n, std::string name,
           int serial = 0) const {
    if( 0 ) {
      std::cout << "#" << name << ":" << serial << std::endl; /** For easy debug */
    }
    for (std::string i : n) {
      const std::string nm = removeUnderScore(i);
      if (isAsmjit_) {
        std::cout << nm << "();";
      } else {
        std::cout << nm << std::endl;
      }
    }
  }

  void put(std::vector<std::string> &n, std::vector<uint64_t> &opSet,
           std::string name, int serial = 0) const {
    if( 0 ) {
      std::cout << "#" << name << ":" << serial << std::endl; /** For easy debug */
    }
    for (size_t i = 0; i < n.size(); i++) {
      const std::string nm = replaceDotWithUnderScore(n[i]);
      put(nm.c_str(), opSet);
    }
  }

  //  char* getBaseStr(uint64_t op1)
  const char *getBaseStr(uint64_t op1) const { return get(op1, (uint64_t)0); }

  /** check all op1, op2, op3, op4, op5, op6, op7, op8 */
  void put(const char *nm, std::vector<uint64_t> &ops) const {
    std::vector<std::string> strBase;
    std::string hoge;
    int i, k;
    uint jj;
    int num_ops = ops.size();

    for (i = 0; i < num_ops; i++) {
      strBase.push_back(getBaseStr(ops[i]));
    }

    /** No operand exists. */
    if (num_ops == 0) {
      if (isAsmjit_) {
        std::cout << "a.";
      }
      std::cout << nm << " ";

      if (isAsmjit_) {
        std::cout << "();";
      }

      std::cout << std::endl;

      return;
    }

    /** Some operands exist.
        Example:nm = add, num_ops = 2, op1 = Xn, op2 = Xn

        The following 64 patterns are printed.
        Expand for op1
        add(X0, X0); dump();
        add(x1, x0); dump();
        ...
        add(x31, x0); dump();
        Expand for op2
        add(x0, x0); dump();
        add(x0, x1); dump();
        ...
        add(x0, x31); dump();
    */
    for (i = 0; i < num_ops; i++) {
      for (jj = 0; jj < getNum(ops[i]); jj++) {
        /** print nemonic */
        if (isAsmjit_) {
          std::cout << "a.";
        }
        std::cout << nm;
        if (isAsmjit_) {
          std::cout << "(";
        } else {
          std::cout << " ";
        }

        /** Print 0 - (i-1)th operand */
        for (k = 0; k < i; k++) {
          std::string tmp = removeDollar(strBase[k]);
          std::string nextStr = "";
          std::cout << tmp;

          if (k + 1 < num_ops) {
            nextStr = removeDollar(strBase[k + 1]);
          }
          if (isRequireComma(num_ops, k, tmp, nextStr)) {
            std::cout << ", ";
          } else {
            std::cout << " ";
          }
        }

        /** Print i-th operand, which is rotated. */
        {
          std::string tmp = removeDollar(get(ops[i], jj));
          std::string nextStr = "";
          std::cout << removeDollar(get(ops[i], jj));

          if (i + 1 < num_ops) {
            nextStr = removeDollar(strBase[i + 1]);
          }
          if (isRequireComma(num_ops, i, tmp, nextStr)) {
            std::cout << ", ";
          } else {
            std::cout << " ";
          }
        }

        /** Print (i+1) to (num_ops-1)-th operand */
        for (k = i + 1; k < num_ops; k++) {
          std::string tmp = removeDollar(strBase[k]);
          std::string nextStr = "";
          std::cout << tmp;

          if (k + 1 < num_ops) {
            nextStr = removeDollar(strBase[k + 1]);
          }

          if (isRequireComma(num_ops, k, tmp, nextStr)) {
            std::cout << ", ";
          } else {
            std::cout << " ";
          }
        }

        if (isAsmjit_) {
          std::cout << ");";
        }

        std::cout << std::endl;
      }
    }
  }

  uint64_t getNum(uint64_t type) const {
    if (type == NOPARA) {
      return 0;
    }

    return tv_Vectors[type]->size();
  }

  const char *get(uint64_t type, uint64_t index) const {
    if (type == NOPARA) {
      std::cerr << std::endl
                << __FILE__ << ":" << __LINE__
                << ", Something wrong. type=" << type << " index=" << index
                << std::endl;
      assert(0);
      return NULL;
    }

    return tv_Vectors[type]->at(index).c_str();
  }

  void initTv() {
    initTvIMM12BIT();
    initTvIMM7BIT();
    initTvIMM7BIT_PM();
    initTvIMM14BIT();
    initTvIMM10BIT();
    initTvIMM16BIT();
    initTvIMM20BIT();
    initTvIMM21BIT();
    initTvIMM26BIT();
    initTvVregSimplePluralDim();
    initTvVregPluralDim();
    initTvVregSimplePluralDimElem();
    initTvVregPluralDimElem();
  }

  void initTvVregSimplePluralDim() {
    /**
       { <Vt>.b }, - { <Vt>.b, <Vt4>.b }
       { <Vt>.h }, - { <Vt>.h, <Vt4>.h }
       { <Vt>.s }, - { <Vt>.s, <Vt4>.s }
       { <Vt>.d }, - { <Vt>.d, <Vt4>.d }
    */
    std::vector<int> vregIdx = {15, 0, 1, 2, 4, 8, 16, 31};

    tv_VREGB_1D.clear();
    tv_VREGH_1D.clear();
    tv_VREGS_1D.clear();
    tv_VREGD_1D.clear();
    tv_VREGB_2D.clear();
    tv_VREGH_2D.clear();
    tv_VREGS_2D.clear();
    tv_VREGD_2D.clear();
    tv_VREGB_3D.clear();
    tv_VREGH_3D.clear();
    tv_VREGS_3D.clear();
    tv_VREGD_3D.clear();
    tv_VREGB_4D.clear();
    tv_VREGH_4D.clear();
    tv_VREGS_4D.clear();
    tv_VREGD_4D.clear();

    jtv_VREGB_1D.clear();
    jtv_VREGH_1D.clear();
    jtv_VREGS_1D.clear();
    jtv_VREGD_1D.clear();
    jtv_VREGB_2D.clear();
    jtv_VREGH_2D.clear();
    jtv_VREGS_2D.clear();
    jtv_VREGD_2D.clear();
    jtv_VREGB_3D.clear();
    jtv_VREGH_3D.clear();
    jtv_VREGS_3D.clear();
    jtv_VREGD_3D.clear();
    jtv_VREGB_4D.clear();
    jtv_VREGH_4D.clear();
    jtv_VREGS_4D.clear();
    jtv_VREGD_4D.clear();

    setPluralReg(tv_VREGB_1D, jtv_VREGB_1D, vregIdx, "b", "b", 1);
    setPluralReg(tv_VREGH_1D, jtv_VREGH_1D, vregIdx, "h", "h", 1);
    setPluralReg(tv_VREGS_1D, jtv_VREGS_1D, vregIdx, "s", "s", 1);
    setPluralReg(tv_VREGD_1D, jtv_VREGD_1D, vregIdx, "d", "d", 1);

    setPluralReg(tv_VREGB_2D, jtv_VREGB_2D, vregIdx, "b", "b", 2);
    setPluralReg(tv_VREGH_2D, jtv_VREGH_2D, vregIdx, "h", "h", 2);
    setPluralReg(tv_VREGS_2D, jtv_VREGS_2D, vregIdx, "s", "s", 2);
    setPluralReg(tv_VREGD_2D, jtv_VREGD_2D, vregIdx, "d", "d", 2);

    setPluralReg(tv_VREGB_3D, jtv_VREGB_3D, vregIdx, "b", "b", 3);
    setPluralReg(tv_VREGH_3D, jtv_VREGH_3D, vregIdx, "h", "h", 3);
    setPluralReg(tv_VREGS_3D, jtv_VREGS_3D, vregIdx, "s", "s", 3);
    setPluralReg(tv_VREGD_3D, jtv_VREGD_3D, vregIdx, "d", "d", 3);

    setPluralReg(tv_VREGB_4D, jtv_VREGB_4D, vregIdx, "b", "b", 4);
    setPluralReg(tv_VREGH_4D, jtv_VREGH_4D, vregIdx, "h", "h", 4);
    setPluralReg(tv_VREGS_4D, jtv_VREGS_4D, vregIdx, "s", "s", 4);
    setPluralReg(tv_VREGD_4D, jtv_VREGD_4D, vregIdx, "d", "d", 4);
  }

  void initTvVregPluralDim() {
    /**
       { <Vt>.16b }, - { <Vt>.16B, <Vt4>.16b }
       { <Vt>.8b }, - { <Vt>.8B, <Vt4>.8b }
       { <Vt>.8h }, - { <Vt>.8H, <Vt4>.8h }
       { <Vt>.4h }, - { <Vt>.4H, <Vt4>.4h }
       { <Vt>.2s }, - { <Vt>.2s, <Vt4>.2s }
       { <Vt>.1s }, - { <Vt>.S, <Vt4>.1s }
       { <Vt>.2d }, - { <Vt>.D, <Vt4>.2d }
    */

    std::vector<int> regIdx = {15, 0, 1, 2, 4, 8, 16, 31};
    std::vector<std::string> type = {"8b", "16b", "4h", "8h",
                                     "2s", "4s",  "2d", "1d"};
    std::vector<std::string> jtype = {"b8", "b16", "h4", "h8",
                                      "s2", "s4",  "d2", "d1"};
    std::vector<std::vector<std::string> *> reg1D = {
        &tv_VREG8B_1D, &tv_VREG16B_1D, &tv_VREG4H_1D, &tv_VREG8H_1D,
        &tv_VREG2S_1D, &tv_VREG4S_1D,  &tv_VREG2D_1D, &tv_VREG1D_1D};
    std::vector<std::vector<std::string> *> reg2D = {
        &tv_VREG8B_2D, &tv_VREG16B_2D, &tv_VREG4H_2D, &tv_VREG8H_2D,
        &tv_VREG2S_2D, &tv_VREG4S_2D,  &tv_VREG2D_2D, &tv_VREG1D_2D};
    std::vector<std::vector<std::string> *> reg3D = {
        &tv_VREG8B_3D, &tv_VREG16B_3D, &tv_VREG4H_3D, &tv_VREG8H_3D,
        &tv_VREG2S_3D, &tv_VREG4S_3D,  &tv_VREG2D_3D, &tv_VREG1D_3D};
    std::vector<std::vector<std::string> *> reg4D = {
        &tv_VREG8B_4D, &tv_VREG16B_4D, &tv_VREG4H_4D, &tv_VREG8H_4D,
        &tv_VREG2S_4D, &tv_VREG4S_4D,  &tv_VREG2D_4D, &tv_VREG1D_4D};
    std::vector<std::vector<std::string> *> jreg1D = {
        &jtv_VREG8B_1D, &jtv_VREG16B_1D, &jtv_VREG4H_1D, &jtv_VREG8H_1D,
        &jtv_VREG2S_1D, &jtv_VREG4S_1D,  &jtv_VREG2D_1D, &jtv_VREG1D_1D};
    std::vector<std::vector<std::string> *> jreg2D = {
        &jtv_VREG8B_2D, &jtv_VREG16B_2D, &jtv_VREG4H_2D, &jtv_VREG8H_2D,
        &jtv_VREG2S_2D, &jtv_VREG4S_2D,  &jtv_VREG2D_2D, &jtv_VREG1D_2D};
    std::vector<std::vector<std::string> *> jreg3D = {
        &jtv_VREG8B_3D, &jtv_VREG16B_3D, &jtv_VREG4H_3D, &jtv_VREG8H_3D,
        &jtv_VREG2S_3D, &jtv_VREG4S_3D,  &jtv_VREG2D_3D, &jtv_VREG1D_3D};
    std::vector<std::vector<std::string> *> jreg4D = {
        &jtv_VREG8B_4D, &jtv_VREG16B_4D, &jtv_VREG4H_4D, &jtv_VREG8H_4D,
        &jtv_VREG2S_4D, &jtv_VREG4S_4D,  &jtv_VREG2D_4D, &jtv_VREG1D_4D};

    for (size_t i = 0; i < reg1D.size(); i++) {
      reg1D[i]->clear();
      reg2D[i]->clear();
      reg3D[i]->clear();
      reg4D[i]->clear();
      jreg1D[i]->clear();
      jreg2D[i]->clear();
      jreg3D[i]->clear();
      jreg4D[i]->clear();
    }

    /** Register index rotation */
    for (int i : regIdx) {
      std::string tmp0 = std::to_string(i);
      std::string tmp1 = std::to_string((i + 1) % 32);
      std::string tmp2 = std::to_string((i + 2) % 32);
      std::string tmp3 = std::to_string((i + 3) % 32);

      for (size_t j = 0; j < type.size(); j++) {
        reg1D[j]->push_back("{ v" + tmp0 + "." + type[j] + " }");
        jreg1D[j]->push_back("v" + tmp0 + "." + jtype[j] + ", 1");

        reg2D[j]->push_back("{ v" + tmp0 + "." + type[j] + ", v" + tmp1 + "." +
                            type[j] + " }");
        jreg2D[j]->push_back("v" + tmp0 + "." + jtype[j] + ", 2");

        reg3D[j]->push_back("{ v" + tmp0 + "." + type[j] + ", v" + tmp1 + "." +
                            type[j] + ", v" + tmp2 + "." + type[j] + " }");
        jreg3D[j]->push_back("v" + tmp0 + "." + jtype[j] + ", 3");

        reg4D[j]->push_back("{ v" + tmp0 + "." + type[j] + ", v" + tmp1 + "." +
                            type[j] + ", v" + tmp2 + "." + type[j] + ", v" +
                            tmp3 + "." + type[j] + " }");
        jreg4D[j]->push_back("v" + tmp0 + "." + jtype[j] + ", 4");
      }
    }

    /** Debug
    for(std::string i : tv_VREG1D_1D) {
      std::cout << i << std::endl;
    }
    for(std::string i : jtv_VREG1D_1D) {
      std::cout << i << std::endl;
    } */
  }

  void initTvVregSimplePluralDimElem() {
    /**
       { <Vt>.b }[i], - { <Vt>.b, <Vt4>.b }[i] i = 0 - 15
       { <Vt>.h }[i], - { <Vt>.h, <Vt4>.h }[i] i = 0 - 7
       { <Vt>.s }[i], - { <Vt>.s, <Vt4>.s }[i] i = 0 - 3
       { <Vt>.d }[i], - { <Vt>.d, <Vt4>.d }[i] i = 0 - 1
    */
    std::vector<int> vregIdx = {15, 0, 1, 2, 4, 8, 16, 31};
    std::vector<int> tmp16 = {7, 0, 1, 2, 4, 8, 15};
    std::vector<int> tmp8 = {7, 0, 1, 2, 4};
    std::vector<int> tmp4 = {3, 0, 1, 2};
    std::vector<int> tmp2 = {1, 0};

    tv_VREGB_1D_ELEM.clear();
    tv_VREGH_1D_ELEM.clear();
    tv_VREGS_1D_ELEM.clear();
    tv_VREGD_1D_ELEM.clear();
    tv_VREGB_2D_ELEM.clear();
    tv_VREGH_2D_ELEM.clear();
    tv_VREGS_2D_ELEM.clear();
    tv_VREGD_2D_ELEM.clear();
    tv_VREGB_3D_ELEM.clear();
    tv_VREGH_3D_ELEM.clear();
    tv_VREGS_3D_ELEM.clear();
    tv_VREGD_3D_ELEM.clear();
    tv_VREGB_4D_ELEM.clear();
    tv_VREGH_4D_ELEM.clear();
    tv_VREGS_4D_ELEM.clear();
    tv_VREGD_4D_ELEM.clear();

    jtv_VREGB_1D_ELEM.clear();
    jtv_VREGH_1D_ELEM.clear();
    jtv_VREGS_1D_ELEM.clear();
    jtv_VREGD_1D_ELEM.clear();
    jtv_VREGB_2D_ELEM.clear();
    jtv_VREGH_2D_ELEM.clear();
    jtv_VREGS_2D_ELEM.clear();
    jtv_VREGD_2D_ELEM.clear();
    jtv_VREGB_3D_ELEM.clear();
    jtv_VREGH_3D_ELEM.clear();
    jtv_VREGS_3D_ELEM.clear();
    jtv_VREGD_3D_ELEM.clear();
    jtv_VREGB_4D_ELEM.clear();
    jtv_VREGH_4D_ELEM.clear();
    jtv_VREGS_4D_ELEM.clear();
    jtv_VREGD_4D_ELEM.clear();

    setPluralRegWithIndex(tv_VREGB_1D_ELEM, jtv_VREGB_1D_ELEM, vregIdx, "b",
                          "b", tmp16, 1);
    setPluralRegWithIndex(tv_VREGH_1D_ELEM, jtv_VREGH_1D_ELEM, vregIdx, "h",
                          "h", tmp8, 1);
    setPluralRegWithIndex(tv_VREGS_1D_ELEM, jtv_VREGS_1D_ELEM, vregIdx, "s",
                          "s", tmp4, 1);
    setPluralRegWithIndex(tv_VREGD_1D_ELEM, jtv_VREGD_1D_ELEM, vregIdx, "d",
                          "d", tmp2, 1);

    setPluralRegWithIndex(tv_VREGB_2D_ELEM, jtv_VREGB_2D_ELEM, vregIdx, "b",
                          "b", tmp16, 2);
    setPluralRegWithIndex(tv_VREGH_2D_ELEM, jtv_VREGH_2D_ELEM, vregIdx, "h",
                          "h", tmp8, 2);
    setPluralRegWithIndex(tv_VREGS_2D_ELEM, jtv_VREGS_2D_ELEM, vregIdx, "s",
                          "s", tmp4, 2);
    setPluralRegWithIndex(tv_VREGD_2D_ELEM, jtv_VREGD_2D_ELEM, vregIdx, "d",
                          "d", tmp2, 2);

    setPluralRegWithIndex(tv_VREGB_3D_ELEM, jtv_VREGB_3D_ELEM, vregIdx, "b",
                          "b", tmp16, 3);
    setPluralRegWithIndex(tv_VREGH_3D_ELEM, jtv_VREGH_3D_ELEM, vregIdx, "h",
                          "h", tmp8, 3);
    setPluralRegWithIndex(tv_VREGS_3D_ELEM, jtv_VREGS_3D_ELEM, vregIdx, "s",
                          "s", tmp4, 3);
    setPluralRegWithIndex(tv_VREGD_3D_ELEM, jtv_VREGD_3D_ELEM, vregIdx, "d",
                          "d", tmp2, 3);

    setPluralRegWithIndex(tv_VREGB_4D_ELEM, jtv_VREGB_4D_ELEM, vregIdx, "b",
                          "b", tmp16, 4);
    setPluralRegWithIndex(tv_VREGH_4D_ELEM, jtv_VREGH_4D_ELEM, vregIdx, "h",
                          "h", tmp8, 4);
    setPluralRegWithIndex(tv_VREGS_4D_ELEM, jtv_VREGS_4D_ELEM, vregIdx, "s",
                          "s", tmp4, 4);
    setPluralRegWithIndex(tv_VREGD_4D_ELEM, jtv_VREGD_4D_ELEM, vregIdx, "d",
                          "d", tmp2, 4);

    /** Debug
    for(std::string i : tv_VREGD_1D_ELEM) {
      std::cout << i << std::endl;
    }
    for(std::string i : jtv_VREGD_1D_ELEM) {
      std::cout << i << std::endl;
    } */
  }

  void initTvVregPluralDimElem() {
    /**
       { <Vt>.16b }[i], - { <Vt>.16b, <Vt4>.16b }[i] i = 0 - 15
       { <Vt>.8b }[i], - { <Vt>.8b, <Vt4>.8b }[i] i = 0 - 15
       { <Vt>.8h }[i], - { <Vt>.8h, <Vt4>.8h }[i] i = 0 - 7
       { <Vt>.4h }[i], - { <Vt>.4h, <Vt4>.4h }[i] i = 0 - 7
       { <Vt>.4s }[i], - { <Vt>.4s, <Vt4>.4s }[i] i = 0 - 3
       { <Vt>.2s }[i], - { <Vt>.2s, <Vt4>.2s }[i] i = 0 - 3
       { <Vt>.2d }[i], - { <Vt>.2d, <Vt4>.2d }[i] i = 0 - 1
    */
    std::vector<int> vregIdx = {15, 0, 1, 2, 4, 8, 16, 31};
    std::vector<int> tmp16 = {7, 0, 1, 2, 4, 8, 15};
    std::vector<int> tmp8 = {7, 0, 1, 2, 4};
    std::vector<int> tmp4 = {3, 0, 1, 2};
    std::vector<int> tmp2 = {1, 0};

    tv_VREG16B_1D_ELEM.clear();
    tv_VREG8B_1D_ELEM.clear();
    tv_VREG8H_1D_ELEM.clear();
    tv_VREG4H_1D_ELEM.clear();
    tv_VREG4S_1D_ELEM.clear();
    tv_VREG2S_1D_ELEM.clear();
    tv_VREG2D_1D_ELEM.clear();
    tv_VREG16B_2D_ELEM.clear();
    tv_VREG8B_2D_ELEM.clear();
    tv_VREG8H_2D_ELEM.clear();
    tv_VREG4H_2D_ELEM.clear();
    tv_VREG4S_2D_ELEM.clear();
    tv_VREG2S_2D_ELEM.clear();
    tv_VREG2D_2D_ELEM.clear();
    tv_VREG16B_3D_ELEM.clear();
    tv_VREG8B_3D_ELEM.clear();
    tv_VREG8H_3D_ELEM.clear();
    tv_VREG4H_3D_ELEM.clear();
    tv_VREG4S_3D_ELEM.clear();
    tv_VREG2S_3D_ELEM.clear();
    tv_VREG2D_3D_ELEM.clear();
    tv_VREG16B_4D_ELEM.clear();
    tv_VREG8B_4D_ELEM.clear();
    tv_VREG8H_4D_ELEM.clear();
    tv_VREG4H_4D_ELEM.clear();
    tv_VREG4S_4D_ELEM.clear();
    tv_VREG2S_4D_ELEM.clear();
    tv_VREG2D_4D_ELEM.clear();

    jtv_VREG16B_1D_ELEM.clear();
    jtv_VREG8B_1D_ELEM.clear();
    jtv_VREG8H_1D_ELEM.clear();
    jtv_VREG4H_1D_ELEM.clear();
    jtv_VREG4S_1D_ELEM.clear();
    jtv_VREG2S_1D_ELEM.clear();
    jtv_VREG2D_1D_ELEM.clear();
    jtv_VREG16B_2D_ELEM.clear();
    jtv_VREG8B_2D_ELEM.clear();
    jtv_VREG8H_2D_ELEM.clear();
    jtv_VREG4H_2D_ELEM.clear();
    jtv_VREG4S_2D_ELEM.clear();
    jtv_VREG2S_2D_ELEM.clear();
    jtv_VREG2D_2D_ELEM.clear();
    jtv_VREG16B_3D_ELEM.clear();
    jtv_VREG8B_3D_ELEM.clear();
    jtv_VREG8H_3D_ELEM.clear();
    jtv_VREG4H_3D_ELEM.clear();
    jtv_VREG4S_3D_ELEM.clear();
    jtv_VREG2S_3D_ELEM.clear();
    jtv_VREG2D_3D_ELEM.clear();
    jtv_VREG16B_4D_ELEM.clear();
    jtv_VREG8B_4D_ELEM.clear();
    jtv_VREG8H_4D_ELEM.clear();
    jtv_VREG4H_4D_ELEM.clear();
    jtv_VREG4S_4D_ELEM.clear();
    jtv_VREG2S_4D_ELEM.clear();
    jtv_VREG2D_4D_ELEM.clear();

    setPluralRegWithIndex(tv_VREG16B_1D_ELEM, jtv_VREG16B_1D_ELEM, vregIdx,
                          "16b", "b16", tmp16, 1);
    setPluralRegWithIndex(tv_VREG8B_1D_ELEM, jtv_VREG8B_1D_ELEM, vregIdx, "8b",
                          "b8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG8H_1D_ELEM, jtv_VREG8H_1D_ELEM, vregIdx, "8h",
                          "h8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG4H_1D_ELEM, jtv_VREG4H_1D_ELEM, vregIdx, "4h",
                          "h4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG4S_1D_ELEM, jtv_VREG4S_1D_ELEM, vregIdx, "4s",
                          "s4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG2S_1D_ELEM, jtv_VREG2S_1D_ELEM, vregIdx, "2s",
                          "s2", tmp2, 1);
    setPluralRegWithIndex(tv_VREG2D_1D_ELEM, jtv_VREG2D_1D_ELEM, vregIdx, "2d",
                          " d2", tmp2, 1);

    setPluralRegWithIndex(tv_VREG16B_2D_ELEM, jtv_VREG16B_2D_ELEM, vregIdx,
                          "16b", "b16", tmp16, 1);
    setPluralRegWithIndex(tv_VREG8B_2D_ELEM, jtv_VREG8B_2D_ELEM, vregIdx, "8b",
                          "b8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG8H_2D_ELEM, jtv_VREG8H_2D_ELEM, vregIdx, "8h",
                          "h8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG4H_2D_ELEM, jtv_VREG4H_2D_ELEM, vregIdx, "4h",
                          "h4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG4S_2D_ELEM, jtv_VREG4S_2D_ELEM, vregIdx, "4s",
                          "s4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG2S_2D_ELEM, jtv_VREG2S_2D_ELEM, vregIdx, "2s",
                          "s2", tmp2, 1);
    setPluralRegWithIndex(tv_VREG2D_2D_ELEM, jtv_VREG2D_2D_ELEM, vregIdx, "2d",
                          " d2", tmp2, 1);

    setPluralRegWithIndex(tv_VREG16B_3D_ELEM, jtv_VREG16B_3D_ELEM, vregIdx,
                          "16b", "b16", tmp16, 1);
    setPluralRegWithIndex(tv_VREG8B_3D_ELEM, jtv_VREG8B_3D_ELEM, vregIdx, "8b",
                          "b8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG8H_3D_ELEM, jtv_VREG8H_3D_ELEM, vregIdx, "8h",
                          "h8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG4H_3D_ELEM, jtv_VREG4H_3D_ELEM, vregIdx, "4h",
                          "h4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG4S_3D_ELEM, jtv_VREG4S_3D_ELEM, vregIdx, "4s",
                          "s4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG2S_3D_ELEM, jtv_VREG2S_3D_ELEM, vregIdx, "2s",
                          "s2", tmp2, 1);
    setPluralRegWithIndex(tv_VREG2D_3D_ELEM, jtv_VREG2D_3D_ELEM, vregIdx, "2d",
                          " d2", tmp2, 1);

    setPluralRegWithIndex(tv_VREG16B_4D_ELEM, jtv_VREG16B_4D_ELEM, vregIdx,
                          "16b", "b16", tmp16, 1);
    setPluralRegWithIndex(tv_VREG8B_4D_ELEM, jtv_VREG8B_4D_ELEM, vregIdx, "8b",
                          "b8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG8H_4D_ELEM, jtv_VREG8H_4D_ELEM, vregIdx, "8h",
                          "h8", tmp8, 1);
    setPluralRegWithIndex(tv_VREG4H_4D_ELEM, jtv_VREG4H_4D_ELEM, vregIdx, "4h",
                          "h4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG4S_4D_ELEM, jtv_VREG4S_4D_ELEM, vregIdx, "4s",
                          "s4", tmp4, 1);
    setPluralRegWithIndex(tv_VREG2S_4D_ELEM, jtv_VREG2S_4D_ELEM, vregIdx, "2s",
                          "s2", tmp2, 1);
    setPluralRegWithIndex(tv_VREG2D_4D_ELEM, jtv_VREG2D_4D_ELEM, vregIdx, "2d",
                          " d2", tmp2, 1);
  }

  void initTvIMM12BIT() {
    std::vector<int> tmp = {1023, 0,   1,   2,   4,    8,    16,  32,
                            64,   128, 256, 512, 1024, 2048, 4095};
    tv_IMM12BIT_MUL2.clear();
    tv_IMM12BIT_MUL4.clear();
    tv_IMM12BIT_MUL8.clear();
    tv_IMM12BIT_MUL16.clear();

    for (int i : tmp) {
      tv_IMM12BIT_MUL2.push_back(std::to_string(i * 2));
      tv_IMM12BIT_MUL4.push_back(std::to_string(i * 4));
      tv_IMM12BIT_MUL8.push_back(std::to_string(i * 8));
      tv_IMM12BIT_MUL16.push_back(std::to_string(i * 16));
    }
  }
#if 0
  void initTvIMM12BIT() {
    std::vector<int> tmp= { 1023, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4095 };
    tv_IMM12BIT_MUL2.clear();
    tv_IMM12BIT_MUL4.clear();
    tv_IMM12BIT_MUL8.clear();
    tv_IMM12BIT_MUL16.clear();

    for(int i : tmp) {
      tv_IMM12BIT_MUL2.push_back(std::to_string(i*2));
      tv_IMM12BIT_MUL4.push_back(std::to_string(i*4));
      tv_IMM12BIT_MUL8.push_back(std::to_string(i*8));
      tv_IMM12BIT_MUL16.push_back(std::to_string(i*16));
    }
  }
#endif

  void initTvIMM7BIT() {
    std::vector<int> tmp = {127, 0, 1, 2, 4, 8, 16, 32, 64, 128, 255};
    tv_IMM7BIT_MUL4.clear();
    tv_IMM7BIT_MUL8.clear();
    tv_IMM7BIT_MUL16.clear();

    for (int i : tmp) {
      tv_IMM7BIT_MUL4.push_back(std::to_string(i * 4));
      tv_IMM7BIT_MUL8.push_back(std::to_string(i * 8));
      tv_IMM7BIT_MUL16.push_back(std::to_string(i * 16));
    }
  }

  void initTvIMM7BIT_PM() {
    std::vector<int> tmp = {63, -64, -32, -16, -8, -4, -2, -1,
                            0,  1,   2,   4,   8,  16, 32};
    tv_IMM7BIT_PM_MUL4.clear();
    tv_IMM7BIT_PM_MUL8.clear();
    tv_IMM7BIT_PM_MUL16.clear();

    for (int i : tmp) {
      tv_IMM7BIT_PM_MUL4.push_back(std::to_string(i * 4));
      tv_IMM7BIT_PM_MUL8.push_back(std::to_string(i * 8));
      tv_IMM7BIT_PM_MUL16.push_back(std::to_string(i * 16));
    }
  }

  void initTvIMM10BIT() {
    tv_IMM10BIT_PM_MUL8.clear();

    tv_IMM10BIT_PM_MUL8.push_back("8*((1<<6)-1)");

    for (int i = 9; i >= 0; i--) {
      tv_IMM10BIT_PM_MUL8.push_back("-8*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM10BIT_PM_MUL8.push_back("0");

    for (int i = 0; i < 9; i++) {
      tv_IMM10BIT_PM_MUL8.push_back("8*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM10BIT_PM_MUL8.push_back("8*((1<<9)-1)");
  }

  void initTvIMM14BIT() {
    tv_IMM14BIT_MUL4.clear();

    tv_IMM14BIT_MUL4.push_back("4*((1<<10)-1)");

    for (int i = 13; i >= 0; i--) {
      tv_IMM14BIT_MUL4.push_back("-4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM14BIT_MUL4.push_back("0");

    for (int i = 0; i < 13; i++) {
      tv_IMM14BIT_MUL4.push_back("4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM14BIT_MUL4.push_back("4*((1<<13)-1)");

    /** Debug
        for(std::string i : tv_IMM14BIT_MUL4) {
        std::cout << "IMM14BIT_MUL4=" << i << std::endl;
        }
    */
  }

  void initTvIMM16BIT() {
    tv_IMM16BIT_MUL4.clear();

    tv_IMM16BIT_MUL4.push_back("4*((1<<13)-1)");

    for (int i = 15; i >= 0; i--) {
      tv_IMM16BIT_MUL4.push_back("-4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM16BIT_MUL4.push_back("0");

    for (int i = 0; i < 15; i++) {
      tv_IMM16BIT_MUL4.push_back("4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM16BIT_MUL4.push_back("4*((1<<15)-1)");

    /** Debug
        for(std::string i : tv_IMM16BIT_MUL4) {
        std::cout << "IMM16BIT_MUL4=" << i << std::endl;
        }
    */
  }

  void initTvIMM20BIT() {
    tv_IMM20BIT_MUL4.clear();

    tv_IMM20BIT_MUL4.push_back("4*((1<<14)-1)");

    for (int i = 16; i >= 0; i--) {
      tv_IMM20BIT_MUL4.push_back("-4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM20BIT_MUL4.push_back("0");

    for (int i = 0; i < 16; i++) {
      tv_IMM20BIT_MUL4.push_back("4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM20BIT_MUL4.push_back("4*((1<<15)-1)");

    /** Debug
        for(std::string i : tv_IMM20BIT_MUL4) {
        std::cout << "IMM20BIT_MUL4=" << i << std::endl;
        }
    */
  }

  void initTvIMM21BIT() {
    tv_IMM21BIT_MUL4.clear();

    tv_IMM21BIT_MUL4.push_back("4*((1<<16)-1)");

    for (int i = 20; i >= 0; i--) {
      tv_IMM21BIT_MUL4.push_back("-4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM21BIT_MUL4.push_back("0");

    for (int i = 0; i < 20; i++) {
      tv_IMM21BIT_MUL4.push_back("4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM21BIT_MUL4.push_back("4*((1<<20)-1)");

    /** Debug
        for(std::string i : tv_IMM21BIT_MUL4) {
        std::cout << "IMM21BIT_MUL4=" << i << std::endl;
        }
    */
  }

  void initTvIMM26BIT() {
    tv_IMM26BIT_MUL4.clear();

    tv_IMM26BIT_MUL4.push_back("4*((1<<16)-1)");

    for (int i = 25; i >= 0; i--) {
      tv_IMM26BIT_MUL4.push_back("-4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM26BIT_MUL4.push_back("0");

    for (int i = 0; i < 25; i++) {
      tv_IMM26BIT_MUL4.push_back("4*(1<<" + std::to_string(i) + ")");
    }

    tv_IMM26BIT_MUL4.push_back("4*((1<<25)-1)");
  }

  void setPluralRegWithIndex(std::vector<std::string> &tv,
                             std::vector<std::string> &jtv,
                             std::vector<int> &reg_idx, std::string type,
                             std::string jtype, std::vector<int> &elem_idx,
                             int num) {
    std::string strNum = std::to_string(num);
    std::string tmp = "";

    if (num == 1) {
      /** Single register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tmp = std::to_string(elem_idx[0]);
        tv.push_back("{ v" + std::to_string(i) + "." + type + " }[" + tmp +
                     "]");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + "[" + tmp +
                      "], " + strNum);
      }
      /*** Element index rotation */
      for (int i : elem_idx) {
        tmp = std::to_string(i);
        tv.push_back("{ v" + std::to_string(reg_idx[0]) + "." + type + " }[" +
                     tmp + "]");
        jtv.push_back("v" + std::to_string(reg_idx[0]) + "." + jtype + "[" +
                      tmp + "], " + strNum);
      }
    } else if (num == 2) {
      /** Two register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tmp = std::to_string(elem_idx[0]);
        tv.push_back("{ v" + std::to_string(i) + "." + type + ", v" +
                     std::to_string((i + 1) % 32) + "." + type + " }[" + tmp +
                     "]");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + "[" + tmp +
                      "], " + strNum);
      }
      /*** Element index rotation */
      for (int i : elem_idx) {
        tmp = std::to_string(i);
        tv.push_back("{ v" + std::to_string(reg_idx[0]) + "." + type + ", v" +
                     std::to_string((reg_idx[0] + 1) % 32) + "." + type +
                     " }[" + tmp + "]");
        jtv.push_back("v" + std::to_string(reg_idx[0]) + "." + jtype + "[" +
                      tmp + "], " + strNum);
      }
    } else if (num == 3) {
      /** Three register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tmp = std::to_string(elem_idx[0]);
        tv.push_back("{ v" + std::to_string(i) + "." + type + ", v" +
                     std::to_string((i + 1) % 32) + "." + type + ", v" +
                     std::to_string((i + 2) % 32) + "." + type + " }[" + tmp +
                     "]");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + "[" + tmp +
                      "], " + strNum);
      }
      /*** Element index rotation */
      for (int i : elem_idx) {
        tmp = std::to_string(i);
        tv.push_back("{ v" + std::to_string(reg_idx[0]) + "." + type + ", v" +
                     std::to_string((reg_idx[0] + 1) % 32) + "." + type +
                     ", v" + std::to_string((reg_idx[0] + 2) % 32) + "." +
                     type + " }[" + tmp + "]");
        jtv.push_back("v" + std::to_string(reg_idx[0]) + "." + jtype + "[" +
                      tmp + "], " + strNum);
      }
    } else if (num == 4) {
      /** Four register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tmp = std::to_string(elem_idx[0]);
        tv.push_back("{ v" + std::to_string(reg_idx[0]) + "." + type + ", v" +
                     std::to_string((reg_idx[0] + 1) % 32) + "." + type +
                     ", v" + std::to_string((reg_idx[0] + 2) % 32) + "." +
                     type + ", v" + std::to_string((reg_idx[0] + 3) % 32) +
                     "." + type + " }[" + tmp + "]");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + "[" + tmp +
                      "], " + strNum);
      }
      /*** Element index rotation */
      for (int i : elem_idx) {
        tmp = std::to_string(i);
        tv.push_back("{ v" + std::to_string(reg_idx[0]) + "." + type + ", v" +
                     std::to_string((reg_idx[0] + 1) % 32) + "." + type +
                     ", v" + std::to_string((reg_idx[0] + 2) % 32) + "." +
                     type + ", v" + std::to_string((reg_idx[0] + 3) % 32) +
                     "." + type + " }[" + tmp + "]");
        jtv.push_back("v" + std::to_string(reg_idx[0]) + "." + jtype + "[" +
                      tmp + "], " + strNum);
      }
    } else {
      std::cerr << __FILE__ << ":" << __LINE__ << ":"
                << "Invalid num=" << num << std::endl;
      assert(NULL);
    }
  }

  void setPluralReg(std::vector<std::string> &tv, std::vector<std::string> &jtv,
                    std::vector<int> &reg_idx, std::string type,
                    std::string jtype, int num) {
    std::string strNum = std::to_string(num);
    std::string tmp = "";

    if (num == 1) {
      /** Single register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tv.push_back("{ v" + std::to_string(i) + "." + type + " }");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + ", " + strNum);
      }
    } else if (num == 2) {
      /** Two register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tv.push_back("{ v" + std::to_string(i) + "." + type + ", v" +
                     std::to_string((i + 1) % 32) + "." + type + " }");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + ", " + strNum);
      }
    } else if (num == 3) {
      /** Two register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tv.push_back("{ v" + std::to_string(i) + "." + type + ", v" +
                     std::to_string((i + 1) % 32) + "." + ", v" +
                     std::to_string((i + 2) % 32) + "." + type + " }");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + ", " + strNum);
      }
    } else if (num == 4) {
      /** Two register */
      /*** Register index rotation */
      for (int i : reg_idx) {
        tv.push_back("{ v" + std::to_string(i) + "." + type + ", v" +
                     std::to_string((i + 1) % 32) + "." + ", v" +
                     std::to_string((i + 2) % 32) + "." + ", v" +
                     std::to_string((i + 3) % 32) + "." + type + " }");
        jtv.push_back("v" + std::to_string(i) + "." + jtype + ", " + strNum);
      }
    } else {
      std::cerr << __FILE__ << ":" << __LINE__ << ":"
                << "Invalid num=" << num << std::endl;
      assert(NULL);
    }
  }

  void setTvAddressing(std::vector<std::string> &tv,
                       std::vector<std::string> &jtv,
                       std::vector<std::string> &base,
                       std::vector<std::string> &imm, AddressingType type) {
    std::string prefix = "[";
    std::string jprefix = "ptr(";
    std::string suffix = "]";
    std::string jsuffix = ")";

    tv.clear();
    jtv.clear();

    switch (type) {
    case TP_PreImm: /** pre-indexded immediate */
      prefix = "[";
      suffix = "]!";
      jprefix = "pre_ptr(";
      jsuffix = ")";
      break;

    case TP_PostImm: /** post-indexed immediate */
      prefix = "[";
      suffix = "]";
      jprefix = "post_ptr(";
      jsuffix = ")";
      break;

    case TP_PostReg: /** Post-indexed register */
      prefix = "[";
      suffix = "]";
      jprefix = "post_ptr(";
      jsuffix = ")";
      break;

    case TP_None:
      prefix = "[";
      suffix = "]";
      jprefix = "ptr(";
      jsuffix = ")";
      break;

    default:
      std::cerr << __FILE__ << ":" << __LINE__
                << "Invalid addresing type=" << type << std::endl;
      assert(NULL);
      break;
    }

    /** base rotation */
    for (std::string i : base) {
      if (type == TP_PreImm || type == TP_None) {
        tv.push_back(prefix + i + ", " + imm[0] + suffix);
      } else {
        tv.push_back(prefix + i + suffix + ", " + imm[0]);
      }

      jtv.push_back(jprefix + i + ", " + imm[0] + jsuffix);
    }

    /** imm rotation */
    for (std::string i : imm) {
      if (type == TP_PreImm || type == TP_None) {
        tv.push_back(prefix + base[0] + ", " + i + suffix);
      } else {
        tv.push_back(prefix + base[0] + suffix + ", " + i);
      }

      jtv.push_back(jprefix + base[0] + ", " + i + jsuffix);
    }
  }

  void putDataProcImm_PcRelAddressing() const {
    /*    static const char tbl[][16] = {
          "adr",
          "adrp",
          };
          for (size_t i = 0; i < NUM_OF_ARRAY(tbl); i++) {
          const char *p = tbl[i];
          put(p, */
    printf("Under construction!\n");
  }

  void clearTvAndJtv() {
    tv_SPECIFIC32.clear();
    tv_SPECIFIC32_1.clear();
    tv_SPECIFIC32_2.clear();
    tv_SPECIFIC32_3.clear();
    //    tv_SPECIFIC32_4.clear();
    //    tv_SPECIFIC32_5.clear();
    tv_SPECIFIC64.clear();
    tv_SPECIFIC64.clear();
    tv_SPECIFIC64_1.clear();
    tv_SPECIFIC64_2.clear();
    tv_SPECIFIC64_3.clear();
    //    tv_SPECIFIC64_4.clear();
    //    tv_SPECIFIC64_5.clear();

    jtv_SPECIFIC32.clear();
    jtv_SPECIFIC32_1.clear();
    jtv_SPECIFIC32_2.clear();
    jtv_SPECIFIC32_3.clear();
    //    jtv_SPECIFIC32_4.clear();
    //    jtv_SPECIFIC32_5.clear();
    jtv_SPECIFIC64.clear();
    jtv_SPECIFIC64.clear();
    jtv_SPECIFIC64_1.clear();
    jtv_SPECIFIC64_2.clear();
    jtv_SPECIFIC64_3.clear();
    //    jtv_SPECIFIC64_4.clear();
    //    jtv_SPECIFIC64_5.clear();

    tv_SPECIFIC0.clear();
    tv_SPECIFIC1.clear();
    tv_SPECIFIC2.clear();
    tv_SPECIFIC3.clear();
    tv_SPECIFIC4.clear();
    tv_SPECIFIC5.clear();
    tv_SPECIFIC6.clear();
    tv_SPECIFIC7.clear();
    tv_SPECIFIC8.clear();
    tv_SPECIFIC9.clear();
    tv_SPECIFIC10.clear();
    tv_SPECIFIC11.clear();
    tv_SPECIFIC12.clear();
    tv_SPECIFIC13.clear();

    jtv_SPECIFIC0.clear();
    jtv_SPECIFIC1.clear();
    jtv_SPECIFIC2.clear();
    jtv_SPECIFIC3.clear();
    jtv_SPECIFIC4.clear();
    jtv_SPECIFIC5.clear();
    jtv_SPECIFIC6.clear();
    jtv_SPECIFIC7.clear();
    jtv_SPECIFIC8.clear();
    jtv_SPECIFIC9.clear();
    jtv_SPECIFIC10.clear();
    jtv_SPECIFIC11.clear();
    jtv_SPECIFIC12.clear();
    jtv_SPECIFIC13.clear();
  }

public:
  Test(bool isAsmjit) : isAsmjit_(isAsmjit), funcNum_(1) {

    initTv();

    if (!isAsmjit_) {
      tv_Vectors = tv_VectorsAs;
      return;
    } else {
      tv_Vectors = tv_VectorsJit;
    }
  }
  /*
    gcc and vc give up to compile this source,
    so I split functions.
  */
  void separateFunc() {
    if (!isAsmjit_)
      return;
  }
  ~Test() {
    if (!isAsmjit_)
      return;
  }

  std::string removeDollar(std::string s) const {
    if (isAsmjit_ && s[0] == '$')
      return s.substr(1);

    return s;
  }

  /** MACRO is expanded in order from the outside. */
  /*** C3.1.1 Conditional branch */
  /**** B.cond Branch conditionally ��1MB B.cond on page C6-721 */
  void putConditional_branch0() {
    std::vector<std::string> cond = {"EQ", "NE", "CS", "CC", "MI",
                                     "PL", "VS", "VC", "HI", "LS",
                                     "GE", "LT", "GT", "LE", "AL"};

    /** Cond rotation */
    for (size_t i = 0; i < cond.size(); i++) {
      if (isAsmjit_) {
        std::cout << "b(" << cond[i] << ", " << tv_IMM21BIT_MUL4[0] << ");"
                  << std::endl;
      } else {
        std::cout << "b." << cond[i] << " " << tv_IMM21BIT_MUL4[0] << std::endl;
      }
    }

    /** Label rotation */
    for (std::string i : tv_IMM21BIT_MUL4) {
      if (isAsmjit_) {
        std::cout << "b(" << cond[0] << ", " << i << ");" << std::endl;
      } else {
        std::cout << "b." << cond[0] << " " << i << std::endl;
      }
    }
  }

  /*** C3.1.2 Unconditional branch (immediate) */
  PUT1(Undinditional_branch0, NM(/*"beqz", "bnez"*/), OPS(WREG, IMM21BIT_MUL4));  //TODO
  PUT1(Undinditional_branch1, NM("bceqz", "bcnez"), OPS(IMM3BIT_FCC, IMM21BIT_MUL4));
  PUT1(Undinditional_branch2, NM(/*"b", "bl"*/), OPS(IMM26BIT_MUL4));  //TODO
  PUT1(Undinditional_branch3, NM("jirl"/*, "beq", "bne", "blt", "bge", "bltu", "bgeu"*/), OPS(WREG, WREG, IMM16BIT_MUL4));  //TODO
  PUT1(Undinditional_branch4, NM("ldptr.w", "ll.w", "sc.w", "ll.d", "sc.d", "stptr.w", "ldptr.d", "stptr.d"), OPS(WREG, WREG, IMM14BIT_MUL4));

  void putUndinditional_branch_core() {
    putUndinditional_branch0();
    putUndinditional_branch1();
    putUndinditional_branch2();
    putUndinditional_branch3();
    putUndinditional_branch4();
  }

  void putUndinditional_branch() { putUndinditional_branch_core(); }

  void put() {
    putUndinditional_branch();

    //    Ops hoge();
    //    hoge.pushNm({"add", "sub"});
  }
};

int main(int argc, char *[]) {
  Test test(argc > 1);
  test.put();
}
