#include "irTox86Visitor.h"
#include "x86Instruction.h"

using namespace std;

IRTox86Visitor::IRTox86Visitor(){
}

void IRTox86Visitor::visit(ASTNode* node){
	translate(node);
	Visitor::visit(node);
}

void IRTox86Visitor::checkOp1IsDest(ASTNode* node, string dest, string op1){
	if (dest != op1)
		node->addInstruction(make_shared<x86Mov>(dest, op1));
}

void IRTox86Visitor::translate(ASTNode* node){
	auto instructions = std::move(node->instructions);

	for (auto instruction : instructions){
		string r;
		switch (instruction->to_type()){
		case Instruction::InstrType::Add:
			node->addInstruction(make_shared<x86Add>(instruction->operand2, instruction->operand3));
			checkOp1IsDest(node, instruction->operand1, instruction->operand2);
			break;
		case Instruction::InstrType::Sub:
			node->addInstruction(make_shared<x86Sub>(instruction->operand2, instruction->operand3));
			checkOp1IsDest(node, instruction->operand1, instruction->operand2);
			break;
		case Instruction::InstrType::Mult:
			node->addInstruction(make_shared<x86Mul>(instruction->operand2, instruction->operand3));
			checkOp1IsDest(node, instruction->operand1, instruction->operand2);
			break;
		case Instruction::InstrType::Calc:
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