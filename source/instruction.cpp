#include "instruction.h"

using namespace std;

Instruction::Instruction(string operand1, string operand2, string operand3, string comment){
	this->operand1 = operand1;
	this->operand2 = operand2;
	this->operand3 = operand3;
	this->comment = comment;
}

size_t Instruction::instrSize(){
	return size;
}

void Instruction::print(std::ostream& out){
	out << op;

	if (hasOp1)
		out << " " << operand1;

	if (hasOp2)
		out << ", " << operand2;

	if (hasOp3)
		out << ", " << operand3;

	if (comment.length())
		out << " ; " << comment;

	out << endl;
}