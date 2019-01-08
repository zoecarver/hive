#include "globals.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"

using namespace llvm;

// Global variables for use throughout all ASTs
LLVMContext mContext;
IRBuilder<> mBuilder(mContext);
std::unique_ptr<Module> mModule = llvm::make_unique<Module>("foo", mContext);
std::map<std::string, AllocaInst*> namedVariables;
std::map<std::string, Value*> namedArgs;
std::map<std::string, std::pair<AllocaInst*, AllocaInst*>> namedArrays;
Function* currentFunc;

Type* i32 = IntegerType::get(mContext, 32);
Type* pi32 = PointerType::getUnqual(i32);
Type* i8 = IntegerType::get(mContext, 8);
Type* pi8 = PointerType::getUnqual(i8);
Type* dType = Type::getDoubleTy(mContext);
Type* pdType = PointerType::getUnqual(dType);
StructType* aType = StructType::get(mContext, {i32, pi8});

AllocaInst* CreateBlockAlloca(Function* func, std::string name, Type* type) {
	IRBuilder<> tmpBuilder(&func->getEntryBlock(), func->getEntryBlock().begin());
	return tmpBuilder.CreateAlloca(type, nullptr, name);
}

Type* mapStructToType(Value* i) {
//	mBuilder.CreateICmp()
//	auto* index = dyn_cast<ConstantInt>(i);
//	auto indexInt = index->getSExtValue();
//
//	if (indexInt == 0) return i32;
//	if (indexInt == 1) return i8;
//	if (indexInt == 2) return dType;
}

Value* mapTypeToValue(Type* t) {
	auto tID = t->getTypeID();
	if (tID == i32->getTypeID()) return ConstantInt::get(mContext, APInt(8, 0));
	if (tID == i8->getTypeID()) return ConstantInt::get(mContext, APInt(8, 1));
	if (tID == dType->getTypeID()) return ConstantInt::get(mContext, APInt(8, 2));
}