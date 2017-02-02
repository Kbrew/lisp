#include <stdio.h>

#include <llvm-c/Core.h>

int main(){
  LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");

  printf("%llx\n", (unsigned long long) mod);
  return 0;
}
