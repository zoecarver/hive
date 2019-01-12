#include <type_traits>

#include "../../globals.h"
#include "ast.h"

#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Value* IfAST::codeGen() {
	Value* conditionV = condition->codeGen();
	assert(conditionV != nullptr);

	// compair to 0.0 (comparisons are done in binop which will result in a 1.0 or 0.0)
	conditionV = mBuilder.CreateSIToFP(conditionV, dType);
	conditionV = mBuilder.CreateFCmpONE(conditionV, ConstantFP::get(mContext, APFloat(0.0)), "ifcond");

	auto* func = currentFunc;
	auto* bodyBlock = BasicBlock::Create(mContext, "then", func);
	auto* elseBlock = BasicBlock::Create(mContext, "else");
	auto* endBlock = BasicBlock::Create(mContext, "end");

	mBuilder.CreateCondBr(conditionV, bodyBlock, elseStatements != nullptr ? elseBlock : endBlock);

	// create the then section
	mBuilder.SetInsertPoint(bodyBlock);

	Value* lastLineIf = ConstantInt::get(mContext, APInt(8, 0)); // so this can be used as turnary operator
	for(auto* line : ifStatments->statements) {
		lastLineIf = line->codeGen();
		delete line;
	}

	// memory management
	ifStatments->statements.clear();

	// leave the block
	mBuilder.CreateBr(endBlock);
	bodyBlock = mBuilder.GetInsertBlock();

	if(elseStatements == nullptr) { // create early return
		elseStatements = new BlockAST();
	}

	// add the else block then enter it
	func->getBasicBlockList().push_back(elseBlock);
	mBuilder.SetInsertPoint(elseBlock);

	Value* lastLineElse =
		ConstantInt::get(mContext, APInt(8, 0)); // initialize to `0` so we have a value if the vector is empty
	for(auto* line : elseStatements->statements) {
		lastLineElse = line->codeGen();
		delete line;
	}

	// memory management
	elseStatements->statements.clear();

	mBuilder.CreateBr(endBlock);
	elseBlock = mBuilder.GetInsertBlock(); // block might change in else

	func->getBasicBlockList().push_back(endBlock);
	mBuilder.SetInsertPoint(endBlock);

	assert(lastLineElse->getType() == lastLineIf->getType() && "Both return types must be equal");

	// create ternary expression
	auto* ternaryNode = mBuilder.CreatePHI(lastLineElse->getType(), 2, "iftmp"); // TODO: make this dynamic
	ternaryNode->addIncoming(lastLineIf, bodyBlock);
	ternaryNode->addIncoming(lastLineElse, elseBlock);

	return ternaryNode;
}

std::string IfAST::out() {
	return std::string("Not Implemented");
}