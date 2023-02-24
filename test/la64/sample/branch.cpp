#include <asmjit/asmjit.h>
#include <stdio.h>

using namespace asmjit;

typedef int (*Func)(void);

int main(int argc, char* argv[]) {
  JitRuntime rt;
  CodeHolder code;
  code.init(rt.environment());

  la64::Assembler a(&code);

  asmjit::Label lp1 = a.newLabel();
  asmjit::Label end = a.newLabel();

  a.addi_d(la64::a0, la64::zero, 3);
  a.addi_d(la64::a3, la64::zero, 6);

  a.bge(la64::a3, la64::a0, end);

  a.bind(lp1);
  a.addi_d(la64::a0, la64::a0, 1);

  a.bge(la64::a3, la64::a0, lp1);

  a.bind(end);
  a.jirl(la64::zero, la64::ra, 0);     // returns from a function.

  Func fn;
  Error err = rt.add(&fn, &code);
  if (err) {
    printf("AsmJit failed: %s\n", DebugUtils::errorAsString(err));
    return 1;
  }

  int result = fn();
  printf("%d\n", result);

  rt.release(fn);

  return 0;
}
