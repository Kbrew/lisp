/**
 * LLVM equivalent of:
 *
 * int sum(int a, int b) {
 *     return a + b;
 * }
 */

#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
	LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");

	LLVMTypeRef param_types[] = {LLVMInt32Type(), LLVMInt32Type()};
	LLVMTypeRef ret_type = LLVMFunctionType(LLVMInt32Type(), param_types, 2, 0);
	LLVMValueRef sum = LLVMAddFunction(mod, "sum", ret_type);

	LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sum, "entry");

	LLVMBuilderRef builder = LLVMCreateBuilder();
	LLVMPositionBuilderAtEnd(builder, entry);
	LLVMValueRef tmp = LLVMBuildAdd(builder, 
	                                LLVMGetParam(sum, 0), 
	                                LLVMGetParam(sum, 1), 
	                                "tmp");
	LLVMBuildRet(builder, tmp);

	char *error = NULL;
	LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
	LLVMDisposeMessage(error);

	// Write out bitcode to file
	if (LLVMWriteBitcodeToFile(mod, "sum.bc") != 0) {
		fprintf(stderr, "error writing bitcode to file, skipping\n");
	}

	LLVMDisposeBuilder(builder);
}
