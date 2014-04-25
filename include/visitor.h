#pragma once

class ASTNode;

// circular references are fun
#define classdef(name) class name ## Node;
classdef(Empty);
classdef(Assignment);
classdef(Block);
classdef(Declaration);
classdef(Declarations);
classdef(Else);
classdef(Expression);
classdef(If);
classdef(Literal);
classdef(Operator);
classdef(Program);
classdef(Return);
classdef(Symbol);
classdef(While);
classdef(For);
classdef(DoWhile);

class Visitor {
public:
	virtual void visit(ASTNode* node);

#define visitFn(name) virtual void visit(name ## Node *node)

	visitFn(Empty);
	visitFn(Assignment);
	visitFn(Block);
	visitFn(Declaration);
	visitFn(Declarations);
	visitFn(Else);
	visitFn(Expression);
	visitFn(If);
	visitFn(Literal);
	visitFn(Operator);
	visitFn(Program);
	visitFn(Return);
	visitFn(Symbol);
	visitFn(While);
	visitFn(For);
	visitFn(DoWhile);
};