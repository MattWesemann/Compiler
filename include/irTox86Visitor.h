#pragma once
#include "visitor.h"

class IRTox86Visitor : public Visitor {
public:
	IRTox86Visitor();

	void visit(ASTNode* node);

private:
	void translate(ASTNode* node);
};