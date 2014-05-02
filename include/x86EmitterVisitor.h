#pragma once
#include "visitor.h"

class x86EmitterVisitor : public Visitor {
public:
	x86EmitterVisitor(void* buf, size_t size, size_t dataOffset);
	void visit(ASTNode* node);

private:
	char* buf;
	size_t size;
	size_t offset;
	size_t dataOffset;
};