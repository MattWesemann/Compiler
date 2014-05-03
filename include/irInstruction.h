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
DEFINSTR(Mv, true, true, 4, "mv")
DEFINSTR(Memld, true, true, 4, "memld")
DEFINSTR(Immld, true, true, 4, "immld")
DEFINSTR(Memst, true, true, 4, "memst")
DEFINSTR(Push, true, false, 4, "push")
DEFINSTR(Mempush, true, false, 4, "mempush")
DEFINSTR(Immpush, true, false, 4, "immpush")
DEFINSTR(Pop, true, false, 4, "pop")
DEFINSTR(Mempop, true, false, 4, "mempop")
DEFINSTR(Jump, true, false, 4, "jump")
DEFINSTR(BFalse, true, true, 4, "bfalse")
DEFINSTR(BTrue, true, true, 4, "btrue")
DEFINSTR(Bgt, true, true, 4, "bgt")
DEFINSTR(Bge, true, true, 4, "bge")
DEFINSTR(Beq, true, true, 4, "beq")
DEFINSTR(Bneq, false, true, 4, "bneq")
DEFINSTR(Blt, true, true, 4, "blt")
DEFINSTR(Ble, true, true, 4, "ble")
DEFINSTR(Reljump, true, false, 4, "reljump")
DEFINSTR(Relbfalse, true, true, 4, "relbfalse")
DEFINSTR(Relbtrue, true, true, 4, "relbtrue")
DEFINSTR(Relbgt, true, true, 4, "relbgt")
DEFINSTR(Relbge, true, true, 4, "relbge")
DEFINSTR(Relbeq, true, true, 4, "relbeq")
DEFINSTR(Relbneq, true, true, 4, "relbneq")
DEFINSTR(Relblt, true, true, 4, "relblt")
DEFINSTR(Relble, true, true, 4, "relble")
DEFINSTR(Call, true, false, 4, "call")
DEFINSTR(Return, false, false, 4, "return")
DEFINSTR(Calc, true, true, 4, "calc")
DEFINSTR(Add, true, true, 4, "+")
DEFINSTR(Sub, true, true, 4, "-")
DEFINSTR(Mult, true, true, 4, "*")
DEFINSTR(Div, true, true, 4, "/")
DEFINSTR(GT, true, true, 4, ">")
DEFINSTR(GTEq, true, true, 4, ">=")
DEFINSTR(Equal, true, true, 4, "==")
DEFINSTR(LT, true, true, 4, "<")
DEFINSTR(LTEq, true, true, 4, "<=")
DEFINSTR(ShiftL, true, true, 4, "<<")
DEFINSTR(ShiftR, true, true, 4, ">>")