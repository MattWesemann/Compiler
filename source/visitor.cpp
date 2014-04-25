#include "visitor.h"
#include "ast.h"

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
visitFnImpl(Else)
visitFnImpl(Expression)
visitFnImpl(If)
visitFnImpl(Literal)
visitFnImpl(Operator)
visitFnImpl(Program)
visitFnImpl(Return)
visitFnImpl(Symbol)
visitFnImpl(While)
visitFnImpl(For)
visitFnImpl(DoWhile)