#include <iostream>
#include <type_traits>

#include "../../globals.h"
#include "ast.h"

#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Value* FunctionAST::codeGen() {
	std::vector<Type*> argTypes;
	for(auto& e : args)
		argTypes.push_back(e.second);

	FunctionType* FT = FunctionType::get(returnType, argTypes, false);
	Function* func = Function::Create(FT, Function::ExternalLinkage, name, mModule.get());
	currentFunc = func;
	ARCCurrentFunc = name;

	auto i = 0;
	for(auto& a : func->args())
		a.setName(args[i++].first);

	// Create the function
	BasicBlock* bb = BasicBlock::Create(mContext, "entry", func);
	mBuilder.SetInsertPoint(bb);

	i = 0;
	for(auto& a : func->args()) {
		namedArgs[args[i].first] = &a;
		i++;
	}

	// Body of the function
	Value* lastLine;
	for(auto* line : statments->statements) {
		lastLine = line->codeGen();
		delete line;
	}

	// memory management
	statments->statements.clear();

	Value* returnValue = lastLine;
	mBuilder.CreateRet(returnValue);

	return func;
}

Value* CallAST::codeGen() {
	int funcOverloadCount = 0;
	auto baseName = name;

start:
	std::cout << "function name:" << name << std::endl;
	Function* func = mModule->getFunction(name);
	std::cout << "function ptr:" << func << std::endl;
	if (!func) { // also search other places
		auto* funcPtr = namedArgs[name];
		funcPtr->print(errs());
		std::cout << "\n";
		Value* loadedFunc = mBuilder.CreateLoad(funcPtr);
		func = llvm::cast<Function>(loadedFunc);
        func->print(errs());
        std::cout << "\n";
	}
	// TODO: also search namedVariables

	std::vector<Value*> argsV;
	for(auto* e : args)
		argsV.push_back(e->codeGen());

	// search the args to make sure we have the write overload
	auto typeIndex = 0;
	while (true) {
		auto* currentType = func->getFunctionType()->getContainedType(typeIndex);
		if (currentType == nullptr || currentType == nullptr) break;

		auto argTypeId = argsV[typeIndex]->getType()->getTypeID();
		auto funcTypeId = currentType->getTypeID();

		if (argTypeId != funcTypeId) {
			funcOverloadCount++;
			name = baseName + "_" + std::to_string(funcOverloadCount);
			goto start;
		}

		typeIndex++;
	}

	return mBuilder.CreateCall(func, argsV);
}

std::string FunctionAST::out() {
	return std::string("Not Implemented");
}

std::string CallAST::out() {
	return std::string("Not Implemented");
}

Type *FunctionTypeHelper::codeGen() {
    FunctionType* FT = FunctionType::get(returnType, argTypes, false);
    return FT;
}

Value* PrototypeAST::codeGen() {
	FunctionType* FT = FunctionType::get(returnType, argTypes, false);

	auto baseName = name;
	int overloadCount = 1;
	while (mModule->getFunction(name) != nullptr) {
		name = baseName + "_" + std::to_string(overloadCount);
		overloadCount++;
	}

	Function* func = Function::Create(FT, Function::ExternalLinkage, name, mModule.get());
	currentFunc = func;
	return func;
}

std::string PrototypeAST::out() {
	std::string outString;
	llvm::raw_string_ostream rso(outString);
	this->codeGen()->print(rso);

	return outString;
}
