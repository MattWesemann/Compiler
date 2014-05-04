#pragma once
#include "instruction.h"

class x86Instruction : public Instruction {
public:
	x86Instruction(std::string operand1, std::string operand2, std::string operand3, std::string comment);
	static std::string convertRegsiterTox86(std::string regName);
	static char convertOpsToHex(std::string op1, std::string op2);
	static char regToInt(std::string regName);
	static std::string intToReg(int reg);
	void emitByte(char* bytes, size_t& offset, size_t len, unsigned char byte);
	void emitBytes(char* bytes, size_t& offset, size_t len, char* src, int srcLen);
};

class x86Add : public x86Instruction {
public:
	x86Add(std::string operand1, std::string operand2, std::string operand3 = "", std::string comment = "") : x86Instruction(operand1, operand2, operand3, comment) {
		hasOp1 = true;
		hasOp2 = true;
		hasOp3 = false;
		size = 0;
		op = "add";
	}

	Instruction::InstrType to_type(){
		return Instruction::InstrType::Calc;  // hack for now because nothing better exists
	}

	void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset){
		(void) dataOffset;

		char reg2 = regToInt(operand2);
		if (reg2 != -1){
			emitByte(bytes, offset, len, 0x03);
			emitByte(bytes, offset, len, convertOpsToHex(operand1, operand2));
		}
		else {
			emitByte(bytes, offset, len, 0x05);
			int val = atoi(operand2.c_str());
			emitBytes(bytes, offset, len, (char*) &val, sizeof(val));
		}
	}
};

class x86Mul : public x86Instruction {
public:
	x86Mul(std::string operand1, std::string operand2 = "", std::string operand3 = "", std::string comment = "") : x86Instruction(operand1, operand2, operand3, comment) {
		hasOp1 = true;
		hasOp2 = true;
		hasOp3 = false;
		size = 0;
		op = "mul";
	}

	Instruction::InstrType to_type(){
		return Instruction::InstrType::Calc;  // hack for now because nothing better exists
	}

	void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset);
};

class x86Sub : public x86Instruction {
public:
	x86Sub(std::string operand1, std::string operand2, std::string operand3 = "", std::string comment = "") : x86Instruction(operand1, operand2, operand3, comment) {
		hasOp1 = true;
		hasOp2 = true;
		hasOp3 = false;
		size = 0;
		op = "sub";
	}

	Instruction::InstrType to_type(){
		return Instruction::InstrType::Calc;  // hack for now because nothing better exists
	}

	void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset){
		(void) dataOffset;

		char reg2 = regToInt(operand2);
		if (reg2 != -1){
			emitByte(bytes, offset, len, 0x2B);
			emitByte(bytes, offset, len, convertOpsToHex(operand1, operand2));
		}
		else {
			emitByte(bytes, offset, len, 0x2D);
			int val = atoi(operand2.c_str());
			emitBytes(bytes, offset, len, (char*) &val, sizeof(val));
		}
	}
};

class x86Mov : public x86Instruction {
public:
	x86Mov(std::string operand1, std::string operand2, std::string operand3 = "", std::string comment = "") : x86Instruction(operand1, operand2, operand3, comment) {
		hasOp1 = true;
		hasOp2 = true;
		hasOp3 = false;
		size = 0;
		op = "mov";
	}

	Instruction::InstrType to_type(){
		return Instruction::InstrType::Mv;
	}

	void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset);
};

class x86Ret : public x86Instruction {
public:
	x86Ret(std::string operand1 = "", std::string operand2 = "", std::string operand3 = "", std::string comment = "") : x86Instruction(operand1, operand2, operand3, comment) {
		hasOp1 = false;
		hasOp2 = false;
		hasOp3 = false;
		size = 0;
		op = "ret";
	}

	Instruction::InstrType to_type(){
		return Instruction::InstrType::Return;
	}

	void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset){
		(void) dataOffset;
		emitByte(bytes, offset, len, 0xC3);
	}
};