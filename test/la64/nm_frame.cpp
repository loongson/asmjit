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
#include <asmjit/core.h>
#include <stdio.h>
using namespace asmjit;
using namespace la64;

typedef int (*Func)(void);

int main(int argc, char* argv[]) {
  JitRuntime rt;
  CodeHolder code;
  code.init(rt.environment());
  la64::Assembler a(&code);

  #include "nm.cpp"

  asmjit::String  encodedOpcode;
  asmjit::Section* text = code.textSection();
  encodedOpcode.appendHex(text->data(), text->bufferSize());
  printf("%s", encodedOpcode.data());  //will redirect to file

  return 0;
}
