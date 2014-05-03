pragma once
#include "visitor.h"

class RegisterVisitor : public Visitor {
public:
	void visit(ExpressionNode* node);
	void numberNode(ASTNode* node);
};

