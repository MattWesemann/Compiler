#include "irTox86Visitor.h"
#include "x86Instruction.h"

using namespace std;

IRTox86Visitor::IRTox86Visitor(){
}

void IRTox86Visitor::visit(ASTNode* node){
	translate(node);
	Visitor::visit(node);
}

void IRTox86Visitor::translate(ASTNode* node){
	auto instructions = std::move(node->instructions);

	for (auto instruction : instructions){
		string r;
		switch (instruction->to_type()){
		case Instruction::InstrType::Calc:
			r = "R" + to_string(instruction->operand1[1] - '0' + 1);
			if (node->str == "+")
				node->addInstruction(make_shared<x86Add>(instruction->operand1, r));
			else if (node->str == "-")
				node->addInstruction(make_shared<x86Sub>(instruction->operand1, r));
			break;
		case Instruction::InstrType::Mv:
			node->addInstruction(make_shared<x86Mov>(instruction->operand1, instruction->operand2));
			break;
		case Instruction::InstrType::Immld:
			node->addInstruction(make_shared<x86Mov>(instruction->operand1, instruction->operand2));
			break;
		case Instruction::InstrType::Memst:
			node->addInstruction(make_shared<x86Mov>("[" + instruction->operand2 + "]", instruction->operand1));
			break;
		case Instruction::InstrType::Memld:
			node->addInstruction(make_shared<x86Mov>(instruction->operand1, "[" + instruction->operand2 + "]"));
			break;
		case Instruction::InstrType::Return:
			node->addInstruction(make_shared<x86Ret>());
			break;
		}
	}
}