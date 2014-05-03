#include "x86Instruction.h"

using namespace std;

x86Instruction::x86Instruction(string operand1, string operand2, string comment) 
			: Instruction(convertRegsiterTox86(operand1), convertRegsiterTox86(operand2), comment) {
}

string x86Instruction::convertRegsiterTox86(string regName){
	if (regName == "R0")
		return "eax";
	if (regName == "R1")
		return "ebx";
	if (regName == "R2")
		return "ecx";
	if (regName == "R3")
		return "edx";
	return regName;
}

char x86Instruction::regToInt(string regName){
	if (regName == "eax")
		return 0;
	if (regName == "ebx")
		return 3;
	if (regName == "ecx")
		return 1;
	if (regName == "edx")
		return 2;
	return -1;
}

string intToReg(int reg){
	switch (reg){
	case 0:
		return "eax";
	case 1:
		return "ecx";
	case 2:
		return "edx";
	case 3:
		return "ebx";
	}
	return "";
}

char x86Instruction::convertOpsToHex(string op1, string op2){

	static unsigned char bytes[8][8] = {{ 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7 },
	                           { 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF },
	                           { 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7 },
	                           { 0xD8, 0xD9, 0xDA, 0xDB, 0xDD, 0xDD, 0xDE, 0xDF },
	                           { 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7 },
	                           { 0xE8, 0xE9, 0xEA, 0xEB, 0xEE, 0xED, 0xEE, 0xEF },
	                           { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7 },
	                           { 0xF8, 0xF9, 0xFA, 0xFB, 0xFF, 0xFD, 0xFE, 0xFF }};

	int reg1 = regToInt(op1);
	int reg2 = regToInt(op2);

	if (reg1 == -1 || reg2 == -1)
		return 0;

	return bytes[reg1][reg2];
}

void x86Instruction::emitByte(char* bytes, size_t& offset, size_t len, unsigned char byte){
	if (1+offset >= len)
		return;

	bytes[offset++] = byte;
}

void x86Instruction::emitBytes(char* bytes, size_t& offset, size_t len, char* src, int srcLen){
	for (int i = 0; i < srcLen; ++i)
		emitByte(bytes, offset, len, src[i]);
}

void x86Mov::emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset){

	// mov [reg1], reg2  ; [reg2][reg1]
	// mov reg1, [reg2]  ;  [reg1][reg2]
	// second op code of instruction
	static unsigned char derefOps[4][4] = { { 0x00, 0x01, 0x02, 0x03 },
	                               { 0x08, 0x09, 0x0A, 0x0B },
	                               { 0x10, 0x11, 0x12, 0x13 },
	                               { 0x18, 0x19, 0x1A, 0x1B } };


	bool deref1 = operand1[0] == '[';

	if (deref1 && operand1[operand1.length() - 1] != ']')
		return;

	string trueOp1 = deref1 ? operand1.substr(1, operand1.length() - 2) : operand1;

	bool deref2 = operand2[0] == '[';

	if (deref2 && operand2[operand2.length() - 1] != ']')
		return;

	string trueOp2 = deref2 ? operand2.substr(1, operand2.length() - 2) : operand2;

	if (deref1 && deref2)
		return;

	int reg1 = regToInt(trueOp1);
	int reg2 = regToInt(trueOp2);

	if (reg1 == -1 && reg2 == -1)
		return;

	// the pushes and pops are here because I don't want to mess with EDI which is the general way of 
	// dealing loading addresses. Because of that it takes 2 instructions to load at a memory - including
	// using a register that may already be holding something useful. So we push it to the stack.
	if (deref1){
		int tempReg = (reg2 + 1) % 4;
		if (reg1 == -1){
			// pop reg
			emitByte(bytes, offset, len, 0x50 + tempReg);

			// mov reg, addr
			emitByte(bytes, offset, len, 0xB8 + tempReg);
			size_t val = atoi(trueOp1.c_str()) + dataOffset;
			emitBytes(bytes, offset, len, (char*) &val, sizeof(val));

			// mov [reg], reg
			emitByte(bytes, offset, len, 0x89);
			emitByte(bytes, offset, len, derefOps[reg2][tempReg]);

			if (reg1 == -1){
				// pop reg
				emitByte(bytes, offset, len, 0x58 + tempReg);
			}
		}
		else {
			// mov [reg], reg
			emitByte(bytes, offset, len, 0x89);
			emitByte(bytes, offset, len, derefOps[reg2][reg2]);
		}
	}
	else if (deref2){
		int tempReg = (reg1 + 1) % 4;
		if (reg2 == -1){
			// pop reg
			emitByte(bytes, offset, len, 0x50 + tempReg);

			// mov reg, addr
			emitByte(bytes, offset, len, 0xB8 + tempReg);
			size_t val = atoi(trueOp2.c_str()) + dataOffset;
			emitBytes(bytes, offset, len, (char*) &val, sizeof(val));

			// mov reg, [reg]
			emitByte(bytes, offset, len, 0x8B);
			emitByte(bytes, offset, len, derefOps[reg1][tempReg]);

			if (reg2 == -1){
				// pop reg
				emitByte(bytes, offset, len, 0x58 + tempReg);
			}
		}
		else {
			// mov reg, [reg]
			emitByte(bytes, offset, len, 0x8B);
			emitByte(bytes, offset, len, derefOps[reg1][reg1]);
		}
	}
	else if (reg2 == -1){
		emitByte(bytes, offset, len, 0xB8 + reg1);
		size_t val = atoi(trueOp2.c_str());
		emitBytes(bytes, offset, len, (char*) &val, sizeof(val));
	}
	else {
		// bugbug finish this
	}
}
