#pragma once
#include "visitor.h"

class RegisterVisitor : public Visitor {
public:
	RegisterVisitor();
	void visit(ExpressionNode* node);
	void numberNode(ASTNode* node);
};

