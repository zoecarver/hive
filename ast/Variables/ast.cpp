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
    if (variableAlloca) {
        mBuilder.CreateStore(newValue, variableAlloca);
        return newValue;
    }

    if (type == nullptr)
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
    Value *alloca = dyn_cast_or_null<Value>(namedVariables[name]);
    if (alloca == nullptr)
        return dyn_cast_or_null<Value>(namedArgs[name]); // TODO: if this is a pointer we still will need to free it

    Value* retValue = getPointer ? alloca : mBuilder.CreateLoad(alloca);

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
	Value* loadAlloca = mBuilder.CreateLoad(namedVariables[name]);
	Instruction* castInst = new BitCastInst(loadAlloca, PointerType::getUnqual(to));
	mBuilder.Insert(castInst);

	AllocaInst* alloca = CreateBlockAlloca(currentFunc, name, castInst->getType());
	mBuilder.CreateStore(castInst, alloca);
	namedVariables[name] = alloca;
	return castInst;
}

Value* ArrayAST::codeGen() {
    Function* func = currentFunc; // insert point for alloca

    std::vector<Value*> arrayValues;

    size_t arraySize = 0;
    for (auto* v: values) {
        Value* newValue = v->codeGen();
        arrayValues.push_back(newValue);

        auto type = newValue->getType();
        auto size = type->getPrimitiveSizeInBits() / 8; // get size in bytes

        arraySize += size;
    }

    Function* mallocCall = mModule->getFunction("malloc");
    std::vector<Value*> mallocArgs = {ConstantInt::get(mContext, APInt(32, arraySize))};

    AllocaInst* alloca = CreateBlockAlloca(func, name, pi8);
    Value* mallocVal = mBuilder.CreateCall(mallocCall, mallocArgs);

    // make an array to store the sizes we need to access to get indexs of our main array
    Function* sizesMallocCall = mModule->getFunction("malloc");
    std::vector<Value*> sizesMallocArgs = {ConstantInt::get(mContext, APInt(32, arrayValues.size() * 4))};

    AllocaInst* sizesAlloca = CreateBlockAlloca(func, "sizes_arr", pi32);
    Value* sizesArray = mBuilder.CreateCall(sizesMallocCall, sizesMallocArgs);
    // cast the sizes array so we dont have to do ugly things later
    Instruction* sizesArrayCastInst = new BitCastInst(sizesArray, pi32);
    mBuilder.Insert(sizesArrayCastInst); // TODO is this nessisary
    sizesArray = sizesArrayCastInst;

    size_t currentSize = 0;
    int count = 0;
    for (auto* v: arrayValues) {
        // cast the array to proper type so it can be stored
        Instruction* castInst = new BitCastInst(mallocVal, PointerType::getUnqual(v->getType()));
        mBuilder.Insert(castInst); // TODO is this nessisary

        auto typeSize = v->getType()->getPrimitiveSizeInBits() / 8;
        auto* index = ConstantInt::get(mContext, APInt(32, currentSize));
        auto* element = mBuilder.CreateGEP(castInst, index);

        mBuilder.CreateStore(v, element);

        // update the sizes array accordingly
        auto* sizesIndex = ConstantInt::get(mContext, APInt(32, count * 4)); // *4 because 4 is the number of bytes in 32 bit array
        auto* sizesElement = mBuilder.CreateGEP(sizesArray, sizesIndex);

        auto* sizeValue = ConstantInt::get(mContext, APInt(32, currentSize)); // * count or current index
        mBuilder.CreateStore(sizeValue, sizesElement);

        currentSize += typeSize;
        count++;
    }

    mBuilder.CreateStore(mallocVal, alloca);
    mBuilder.CreateStore(sizesArray, sizesAlloca);
    namedArrays[name] = std::make_pair(alloca, sizesAlloca);

    return alloca;
}

Value* ArrayGetAST::codeGen() {
    auto a = namedArrays[arrayName];
    auto* array = mBuilder.CreateLoad(a.first);
    auto* sizes = mBuilder.CreateLoad(a.second);

    // bitcast array to type
    Instruction* arrayCastInst = new BitCastInst(array, pi32); // TODO: this type needs to be dynamic
    mBuilder.Insert(arrayCastInst); // TODO is this nessisary

    auto* sizeElement = mBuilder.CreateGEP(sizes, index->codeGen());
    auto* arrayIndex = mBuilder.CreateLoad(sizeElement);

    auto* element = mBuilder.CreateGEP(arrayCastInst, arrayIndex);
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
