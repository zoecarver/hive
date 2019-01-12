#pragma once

#include "llvm/IR/Value.h"

#include "../ast.h"

using namespace llvm;

// template <typename T>
class IfAST : public AST {
	AST* condition;
	BlockAST* ifStatments;
	BlockAST* elseStatements;

public:
	IfAST(AST* condition, BlockAST* ifStatments, BlockAST* elseStatements)
		: condition(condition)
		, elseStatements(elseStatements)
		, ifStatments(ifStatments) {}
	Value* codeGen() override;
	std::string out() override;
};