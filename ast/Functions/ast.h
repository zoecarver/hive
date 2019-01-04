#pragma once

#include "llvm/IR/Value.h"

#include "../ast.h"

using namespace llvm;

class FunctionAST: public AST {
  std::string name;
  BlockAST *statments;
  std::vector<std::pair<std::string, Type *>> args;

  public:
    FunctionAST(
      std::string name, 
      std::vector<std::pair<std::string, Type *>> args,
      BlockAST *statments
    ): name(name), statments(statments), args(args) { }
    Value *codeGen() override;
    std::string out() override;
};

class PrototypeAST: public AST {
  Type *returnType;
  std::vector<Type *> argTypes;
  std::string name;
  public:
    PrototypeAST (
      std::string name,
      std::vector<Type *>  argTypes,
      Type *returnType
    ): name(name), argTypes(argTypes), returnType(returnType) { }
    Value *codeGen() override;
    std::string out() override;
};

class CallAST: public AST {
  std::string name;
  std::vector<AST *> args;

  public:
    CallAST(std::string name, std::vector<AST *> args): name(name), args(args) { }
    Value *codeGen() override;
    std::string out() override;
};
