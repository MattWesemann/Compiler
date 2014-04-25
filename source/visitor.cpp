#include "visitor.h"
#include "ast.h"

Visitor::Visitor(){
	errorFlag = false;
}

#define visitFnImpl(name) void Visitor::visit(name ## Node *node){          \
							for (auto child : node->children)      \
								Visitor::visit(child);             \
						  }

void Visitor::visit(ASTNode* node){
	node->accept(this);
}

visitFnImpl(Empty)
visitFnImpl(Assignment)
visitFnImpl(Block)
visitFnImpl(Declaration)
visitFnImpl(Declarations)
visitFnImpl(If)
visitFnImpl(Else)
visitFnImpl(Expression)
visitFnImpl(Literal)
visitFnImpl(Operator)
visitFnImpl(Program)
visitFnImpl(Return)
visitFnImpl(Symbol)
visitFnImpl(Type)
visitFnImpl(While)
visitFnImpl(For)
visitFnImpl(DoWhile)

bool Visitor::hadError(){
	return errorFlag;
}