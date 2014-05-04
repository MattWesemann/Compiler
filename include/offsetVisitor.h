#pragma once
#include "visitor.h"

class OffsetVisitor : public Visitor {
public:
	OffsetVisitor(size_t dataOffset);
	void visit(AssignmentNode* node);
	void visit(SymbolNode* node);

private:
	size_t dataOffset;
};
