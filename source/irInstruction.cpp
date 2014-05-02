#include "irInstruction.h"

using namespace std;

IRInstruction::IRInstruction(string operand1, string operand2, string comment) : Instruction(operand1, operand2, comment) {}

void IRInstruction::emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset){
	(void) bytes;
	(void) offset;
	(void) len;
	(void) dataOffset;
}