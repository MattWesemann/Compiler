#include "x86EmitterVisitor.h"

x86EmitterVisitor::x86EmitterVisitor(void* buf, size_t size, size_t dataOffset){
	this->buf = (char*) buf;
	this->size = size;
	this->dataOffset = dataOffset;
	offset = 0;
}

void x86EmitterVisitor::visit(ASTNode* node){
	Visitor::visit(node);
	for (auto instruction : node->instructions)
		instruction->emitBinary(buf, offset, size, dataOffset);
}