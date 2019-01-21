#include <iostream>
#include <type_traits>

#include "../../globals.h"
#include "ast.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Value* VariableAST::codeGen() {
	Function* func = currentFunc; // insert point for alloca

	Value* newValue = value->codeGen();
	auto variableAlloca = namedVariables[name];

	// check if we already have the variable
	if(variableAlloca) {
		mBuilder.CreateStore(newValue, variableAlloca);
		return newValue;
	}

	if(type == nullptr)
		type = newValue->getType();

	AllocaInst* alloca = CreateBlockAlloca(func, name, type);
	mBuilder.CreateStore(newValue, alloca);
	namedVariables[name] = alloca;

	return alloca;
}

Value* VariableSetAST::codeGen() {
	Value* loadAlloca = mBuilder.CreateLoad(namedVariables[name]);
	Value* newValue = newVal->codeGen();

	mBuilder.CreateStore(newValue, loadAlloca);
	return newValue;
}

Value* PointerSetAST::codeGen() {
	return mBuilder.CreateStore(newVal->codeGen(), ptr->codeGen());
}

Value* VariableGetAST::codeGen() {
	// first check if a function exists
	Value* possibleFunction = mModule->getFunction(name);
	if(possibleFunction)
		return possibleFunction;

	Value* alloca = dyn_cast_or_null<Value>(namedVariables[name]);
	if(alloca == nullptr)
		return dyn_cast_or_null<Value>(namedArgs[name]); // TODO: if this is a pointer we still will need to free it

	Value* retValue =
		getPointer ? alloca : mBuilder.CreateLoad(alloca); // TODO: this also needs to be applied to above cases

	return retValue; // TODO: handle arc (we need to free stuff if we are not going to get it again)
}

Value* CastAST::codeGen() {
	Value* V = namedVariables[name];
	if(V == nullptr)
		V = namedArgs[name]; // TODO: this doesnot need to be loaded

	assert(V != nullptr && "Value to cast is null");

	Value* loadAlloca = mBuilder.CreateLoad(V);
	Instruction* castInst = new BitCastInst(loadAlloca, PointerType::getUnqual(to));
	mBuilder.Insert(castInst);

	AllocaInst* alloca = CreateBlockAlloca(currentFunc, name, castInst->getType());
	mBuilder.CreateStore(castInst, alloca);
	namedVariables[name] = alloca;
	return castInst;
}

Value* ArrayAST::codeGen() {
	std::vector<Value*> arrayValues;

	size_t arraySize = 0;
	for(auto* v : values) {
		Value* newValue = v->codeGen();
		arrayValues.push_back(newValue);

		auto type = newValue->getType();
		auto size = type->getPrimitiveSizeInBits() / 8; // get size in bytes

		arraySize += size;
	}

	Function* mallocCall = mModule->getFunction("malloc");
	std::vector<Value*> mallocArgs = {ConstantInt::get(mContext, APInt(32, arraySize))};

	Value* mallocVal = mBuilder.CreateCall(mallocCall, mallocArgs);
	// cast the array to proper type so it can be stored
	auto* arrayType = PointerType::getUnqual(arrayValues[0]->getType());
	auto* array = mBuilder.CreateBitCast(mallocVal, arrayType);

	auto count = 0;
	for(auto* v: arrayValues) {
		auto typeSize = v->getType()->getPrimitiveSizeInBits() / 8;
		if(typeSize == 0)
			typeSize = 4; // if it is a pointer it will be 0

		auto* index =
			ConstantInt::get(mContext, APInt(32, count++)); // /4 because that is the size of an int32 in bytes
		auto* element = mBuilder.CreateGEP(array, index);

		mBuilder.CreateStore(v, element);
	}

	// create a struct and store it there
	std::vector<Type*> structTypes = {i32, arrayType}; // size, array pointer
	auto* arrayStructType = StructType::create(mContext, makeArrayRef(structTypes), "array_type");
	globalTypes.push_back(arrayStructType);

	auto* structPointer = CreateBlockAlloca(currentFunc, "tmp_struct", arrayStructType);
	        // new AllocaInst(arrayStructType, 0, "alloc_list", currentFunc->getEntryBlock());
	        // CreateBlockAlloca(currentFunc, "tmp_struct", arrayStructType);

	auto* foo = cast<PointerType>(structPointer->getType()->getScalarType())->getElementType();

	// set array size
	std::vector<Value*> index = {ConstantInt::get(mContext, APInt(32, 0)), ConstantInt::get(mContext, APInt(32, 0))};
	auto* sizeValue = ConstantInt::get(mContext, APInt(32, arraySize));
	auto* sizeElement = mBuilder.CreateGEP(structPointer, makeArrayRef(index));
	mBuilder.CreateStore(sizeValue, sizeElement);

	// set pointer
	index = {ConstantInt::get(mContext, APInt(32, 0)), ConstantInt::get(mContext, APInt(32, 1))};
	auto* ptrElement = mBuilder.CreateGEP(structPointer, makeArrayRef(index));
	mBuilder.CreateStore(array, ptrElement);

	return structPointer;
}

Value* ArrayGetAST::codeGen() {
	auto a = namedVariables[arrayName];
	auto* arrayHolder = mBuilder.CreateLoad(a);
	arrayHolder = mBuilder.CreateLoad(arrayHolder);

	auto* arrayIndex = index->codeGen();
	assert(arrayIndex->getType()->isIntegerTy() && "Index must be an integer");
	assert(arrayIndex->getType()->getPrimitiveSizeInBits() == 32 && "Warring: non-32bit integer may have unexpected results");

	auto* array = mBuilder.CreateGEP(arrayHolder, ConstantInt::get(mContext, APInt(32, 1))); // extract the pointer from the struct
	auto* element = mBuilder.CreateGEP(array, arrayIndex);

	return mBuilder.CreateLoad(element);
}

Value* ArrayJoinAST::codeGen() {
	auto a = namedVariables[arrayName];
	auto* array = mBuilder.CreateLoad(a);
	assert(array != nullptr && "Array is not defined");

	auto* rightArray = R->codeGen();
	assert(rightArray != nullptr && "Right hand array null");

	return nullptr;

//	Function* reallocCall = mModule->getFunction("realloc");
//	std::vector<Value*> reallocArgs = {ConstantInt::get(mContext, APInt(32, arraySize))};
//	Value* reallocArray = mBuilder.CreateCall(reallocCall, reallocArgs);
}

Value* LoadVariableAST::codeGen() {
	return mBuilder.CreateLoad(ptr->codeGen());
}

std::string CastAST::out() {
	return std::string("Not Implemented");
}

std::string LoadVariableAST::out() {
	return std::string("Not Implemented");
}

std::string VariableAST::out() {
	return std::string("Not Implemented");
}

std::string VariableSetAST::out() {
	return std::string("Not Implemented");
}

std::string VariableGetAST::out() {
	return std::string("Not Implemented");
}

std::string ArrayAST::out() {
	return std::string("Not Implemented");
}

std::string ArrayGetAST::out() {
	return std::string("Not Implemented");
}

std::string ArrayJoinAST::out() {
	return std::string("Not Implemented");
}

std::string PointerSetAST::out() {
	return std::string("Not Implemented");
}
