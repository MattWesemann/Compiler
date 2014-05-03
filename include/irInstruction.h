#pragma once
#include "instruction.h"

class IRInstruction : public Instruction {
public:
	IRInstruction(std::string operand1, std::string operand2, std::string comment);
	void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset);
};

#define DEFINSTR(NAME, HAS_OP1, HAS_OP2, SIZE, OP_STR) class NAME ## Instr : public IRInstruction {    \
public:                                                                                                \
	NAME ## Instr(std::string operand1 = "", std::string operand2 = "", std::string comment = "")      \
	: IRInstruction(operand1, operand2, comment){                                                      \
	                                                                                                   \
	    hasOp1 = HAS_OP1;                                                                              \
	    hasOp2 = HAS_OP2;                                                                              \
	    size = SIZE;                                                                                   \
	    op = OP_STR;                                                                                   \
	}                                                                                                  \
	Instruction::InstrType to_type(){                                                                  \
        return Instruction::InstrType::NAME;                                                           \
    }                                                                                                  \
};

// can't use the macro to define this because they require additional arguments
DEFINSTR(Mv, true, true, 0, "mv")
DEFINSTR(Memld, true, true, 0, "memld")
DEFINSTR(Immld, true, true, 0, "immld")
DEFINSTR(Memst, true, true, 0, "memst")
DEFINSTR(Push, true, false, 0, "push")
DEFINSTR(Mempush, true, false, 0, "mempush")
DEFINSTR(Immpush, true, false, 0, "immpush")
DEFINSTR(Pop, true, false, 0, "pop")
DEFINSTR(Mempop, true, false, 0, "mempop")
DEFINSTR(Jump, true, false, 0, "jump")
DEFINSTR(BFalse, true, true, 0, "bfalse")
DEFINSTR(BTrue, true, true, 0, "btrue")
DEFINSTR(Bgt, true, true, 0, "bgt")
DEFINSTR(Bge, true, true, 0, "bge")
DEFINSTR(Beq, true, true, 0, "beq")
DEFINSTR(Bneq, false, true, 0, "bneq")
DEFINSTR(Blt, true, true, 0, "blt")
DEFINSTR(Ble, true, true, 0, "ble")
DEFINSTR(Reljump, true, false, 0, "reljump")
DEFINSTR(Relbfalse, true, true, 0, "relbfalse")
DEFINSTR(Relbtrue, true, true, 0, "relbtrue")
DEFINSTR(Relbgt, true, true, 0, "relbgt")
DEFINSTR(Relbge, true, true, 0, "relbge")
DEFINSTR(Relbeq, true, true, 0, "relbeq")
DEFINSTR(Relbneq, true, true, 0, "relbneq")
DEFINSTR(Relblt, true, true, 0, "relblt")
DEFINSTR(Relble, true, true, 0, "relble")
DEFINSTR(Call, true, false, 0, "call")
DEFINSTR(Return, false, false, 0, "jump +0")   // stupid hack because aparently we can't have real return statements
DEFINSTR(Calc, true, true, 0, "calc")
