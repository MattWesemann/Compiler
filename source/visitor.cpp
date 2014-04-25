#include "visitor.h"
#include "ast.h"

void Visitor::visit(ASTNode* node){
	node->accept(this);
}