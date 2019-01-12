#include <iostream>

#include "ast.h"

#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Value* BlockAST::codeGen() {
	Value* _;
	for(auto e : statements)
		_ = e->codeGen();

	return _; // its okay if this is null
}

std::string BlockAST::out() {
	std::string outString;
	llvm::raw_string_ostream rso(outString);

	for(auto e : statements) {
		e->codeGen()->print(rso);
		outString += "\n";
	}

	return outString;
}