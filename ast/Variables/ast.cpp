#include <iostream>
#include <type_traits>

#include "../../globals.h"
#include "ast.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// create value from malloc
//    Function* mallocCall = mModule->getFunction("malloc");
//    std::vector<Value*> mallocArgs = {ConstantInt::get(mContext, APInt(32, size))};
//
//    AllocaInst* alloca = CreateBlockAlloca(func, name, pi8);
//    Value* mallocVal = mBuilder.CreateCall(mallocCall, mallocArgs);

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
	auto size = type->getPrimitiveSizeInBits();

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
	if (possibleFunction) return possibleFunction;

	Value* alloca = dyn_cast_or_null<Value>(namedVariables[name]);
	if(alloca == nullptr)
		return dyn_cast_or_null<Value>(namedArgs[name]); // TODO: if this is a pointer we still will need to free it

	Value* retValue = getPointer ? alloca : mBuilder.CreateLoad(alloca); // TODO: this also needs to be applied to above cases

	return retValue; // TODO: fix below

	//    if(ARC[ARCCurrentFunc][name] < 0) { // if we arent going to use the variable again, free it
	//        Instruction* castInst = new BitCastInst(loadAlloca, pi8);
	//        mBuilder.Insert(castInst);
	//
	//        Function* freeCall = mModule->getFunction("free");
	//        std::vector<Value*> freeArgs = {castInst};
	//        mBuilder.CreateCall(freeCall, freeArgs);
	//    } else
	//        ARC[ARCCurrentFunc][name]--;
	//
	//    return retValue;
}

Value* CastAST::codeGen() {
    Value* V = namedVariables[name];
    if (V == nullptr) V = namedArgs[name];

	Value* loadAlloca = mBuilder.CreateLoad(V);
	Instruction* castInst = new BitCastInst(loadAlloca, PointerType::getUnqual(to));
	mBuilder.Insert(castInst);

	AllocaInst* alloca = CreateBlockAlloca(currentFunc, name, castInst->getType());
	mBuilder.CreateStore(castInst, alloca);
	namedVariables[name] = alloca;
	return castInst;
}

Value* ArrayAST::codeGen() {
	auto* func = currentFunc;

	std::vector<Value*> arrayValues;

	size_t arraySize = 4;
	for(auto* v : values) {
		Value* newValue = v->codeGen();
		arrayValues.push_back(newValue);

		auto type = newValue->getType();
		auto size = type->getPrimitiveSizeInBits() / 8; // get size in bytes

		arraySize += size;
	}

	auto* arrType = arrayValues[0]->getType();

	Function* mallocCall = mModule->getFunction("malloc");
	std::vector<Value*> mallocArgs = {ConstantInt::get(mContext, APInt(32, arraySize))};

	Value* mallocVal = mBuilder.CreateCall(mallocCall, mallocArgs);
	// cast the array to proper type so it can be stored
	Instruction* castInst = new BitCastInst(mallocVal, PointerType::getUnqual(arrayValues[0]->getType()));
	mBuilder.Insert(castInst); // TODO is this nessisary

	// add type as first value
//	auto* index = ConstantInt::get(mContext, APInt(32, 0));
//	auto* element = mBuilder.CreateGEP(mallocVal, index);
//	mBuilder.CreateStore(mapTypeToValue(arrType), element);

	size_t currentSize = 0; // 4;
	int count = 0; // 1;
	for(auto* v : arrayValues) {
		auto typeSize = v->getType()->getPrimitiveSizeInBits() / 8;
		if (typeSize == 0) typeSize = 4; // if it is a pointer it will be 0
		auto* index = ConstantInt::get(mContext, APInt(32, currentSize / 4)); // /4 because that is the size of an int32 in bytes
		auto* element = mBuilder.CreateGEP(castInst, index);

		mBuilder.CreateStore(v, element);

		currentSize += typeSize;
		count++;
	}

//	// indexs for struct below
//	auto* zero = ConstantInt::get(mContext, APInt(32, 0));
//	auto* one = ConstantInt::get(mContext, APInt(32, 1));
//
//	auto* arrayHolderAlloca = CreateBlockAlloca(func, "array_holder", aType);
//	auto* arrayHolder = mBuilder.CreateLoad(arrayHolderAlloca);
//	auto* typePtr = mBuilder.CreateGEP(arrayHolder, zero);
//	auto* arrPtr = mBuilder.CreateGEP(arrayHolder, one);
//
//	// store info and array in struct
//	mBuilder.CreateStore(mallocVal, arrPtr);
//	mBuilder.CreateStore(mapTypeToValue(arrType), typePtr);

	return castInst;
}

Value* ArrayGetAST::codeGen() {
	auto a = namedVariables[arrayName];
	auto* array = mBuilder.CreateLoad(a);

//	auto* typeElement = mBuilder.CreateGEP(array, ConstantInt::get(mContext, APInt(32, 0)));
//	auto* arrayType = mapStructToType(mBuilder.CreateLoad(typeElement));

	// bitcast array to type
//	Instruction* arrayCastInst = new BitCastInst(array, arrayType);
//	mBuilder.Insert(arrayCastInst); // TODO is this nessisary

	auto* arrayIndex = index->codeGen();

	auto* element = mBuilder.CreateGEP(array, arrayIndex);
	return mBuilder.CreateLoad(element);
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

std::string PointerSetAST::out() {
	return std::string("Not Implemented");
}
