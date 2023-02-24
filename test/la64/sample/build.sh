INCLUDE_PATH=/home/loongson/gitloongson/asmjit/src
ASMJIT_LIB_PATH=/home/loongson/gitloongson/asmjit/build

#gcc demo.cpp -I${INCLUDE_PATH} -lasmjit -lstdc++ -L${ASMJIT_LIB_PATH} -o demo
gcc -g branch.cpp -I${INCLUDE_PATH} -lasmjit -lstdc++ -L${ASMJIT_LIB_PATH} -o branch
