#pragma once
#include "visitor.h"

class ConstVisitor : public Visitor {
public:
	ConstVisitor();

	void visit(AssignmentNode* node);
	void visit(DeclarationsNode* node);
	void visit(ExpressionNode* node);


};
