#include "offsetVisitor.h"
#include <cstdlib>
#include <string>
#include "instruction.h"

using namespace std;

OffsetVisitor::OffsetVisitor(size_t dataOffset){
	this->dataOffset = dataOffset;
}

void OffsetVisitor::visit(AssignmentNode* node){
	for(auto instruction : node->instructions)
		if(instruction->to_type() == Instruction::InstrType::Memst){
			instruction->operand2 = to_string(atoi(instruction->operand2.c_str()) + dataOffset);
			break;
		}
	Visitor::visit(node->children[1]);		
}

void OffsetVisitor::visit(SymbolNode* node){
	if(node->instructions.size())
		node->instructions[0]->operand2 = to_string(atoi(node->instructions[0]->operand2.c_str()) + dataOffset);
}
