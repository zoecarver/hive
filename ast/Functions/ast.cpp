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
	for(auto& a : func->args())
		namedArgs[args[i++].first] = &a;

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
	Function* func = mModule->getFunction(name);

	if(!func) { // also search other places
		// TODO fix this
		auto* funcPtr = namedArgs[name];
		Value* loadedFunc = mBuilder.CreateLoad(funcPtr);
		func = llvm::cast<Function>(loadedFunc);
	}
	// TODO: also search namedVariables

	std::vector<Value*> argsV;
	for(auto* e : args)
		argsV.push_back(e->codeGen());

	return mBuilder.CreateCall(func, argsV);
}

std::string FunctionAST::out() {
	return std::string("Not Implemented");
}

std::string CallAST::out() {
	return std::string("Not Implemented");
}

Type* FunctionTypeHelper::codeGen() {
	FunctionType* FT = FunctionType::get(returnType, argTypes, false);
	return FT;
}

Value* PrototypeAST::codeGen() {
	FunctionType* FT = FunctionType::get(returnType, argTypes, false);
	Function* func = Function::Create(FT, Function::ExternalLinkage, name, mModule.get());
	currentFunc = func;
	return func;
}

std::string PrototypeAST::out() { // so we can `extern` functions
	std::string outString;
	llvm::raw_string_ostream rso(outString);
	this->codeGen()->print(rso);

	return outString;
}
