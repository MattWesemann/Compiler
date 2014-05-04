#pragma once
#include "instruction.h"

class IRInstruction : public Instruction {
public:
    IRInstruction(std::string operand1, std::string operand2, std::string operand3, std::string comment);
    void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset);
};

#define DEFINSTR(NAME, HAS_OP1, HAS_OP2, HAS_OP3, SIZE, OP_STR) class NAME ## Instr : public IRInstruction {    \
public:                                                                                                \
    NAME ## Instr(std::string operand1 = "", std::string operand2 = "", std::string operand3 = "", std::string comment = "")      \
    : IRInstruction(operand1, operand2, operand3, comment){                                                      \
                                                                                                       \
        hasOp1 = HAS_OP1;                                                                              \
        hasOp2 = HAS_OP2;                                                                              \
        hasOp3 = HAS_OP3;                                                                              \
        size = SIZE;                                                                                   \
        op = OP_STR;                                                                                   \
    }                                                                                                  \
    Instruction::InstrType to_type(){                                                                  \
        return Instruction::InstrType::NAME;                                                           \
    }                                                                                                  \
};

// can't use the macro to define this because they require additional arguments
DEFINSTR(Mv, true, true, false, 4, "mv")
DEFINSTR(Memld, true, true, false, 4, "memld")
DEFINSTR(Immld, true, true, false, 4, "immld")
DEFINSTR(Memst, true, true, false, 4, "memst")
DEFINSTR(Push, true, false, false, 4, "push")
DEFINSTR(Mempush, true, false, false, 4, "mempush")
DEFINSTR(Immpush, true, false, false, 4, "immpush")
DEFINSTR(Pop, true, false, false, 4, "pop")
DEFINSTR(Mempop, true, false, false, 4, "mempop")
DEFINSTR(Jump, true, false, false, 4, "jump")
DEFINSTR(BFalse, true, true, false, 4, "bfalse")
DEFINSTR(BTrue, true, true, false, 4, "btrue")
DEFINSTR(Bgt, true, true, false, 4, "bgt")
DEFINSTR(Bge, true, true, false, 4, "bge")
DEFINSTR(Beq, true, true, false, 4, "beq")
DEFINSTR(Bneq, false, true, false, 4, "bneq")
DEFINSTR(Blt, true, true, false, 4, "blt")
DEFINSTR(Ble, true, true, false, 4, "ble")
DEFINSTR(Reljump, true, false, false, 4, "reljump")
DEFINSTR(Relbfalse, true, true, false, 4, "relbfalse")
DEFINSTR(Relbtrue, true, true, false, 4, "relbtrue")
DEFINSTR(Relbgt, true, true, false, 4, "relbgt")
DEFINSTR(Relbge, true, true, false, 4, "relbge")
DEFINSTR(Relbeq, true, true, false, 4, "relbeq")
DEFINSTR(Relbneq, true, true, false, 4, "relbneq")
DEFINSTR(Relblt, true, true, false, 4, "relblt")
DEFINSTR(Relble, true, true, false, 4, "relble")
DEFINSTR(Call, true, false, false, 4, "call")
DEFINSTR(Return, false, false, false, 4, "jump +0")   // stupid hack because aparently we can't have real return statements
DEFINSTR(Calc, true, true, false, 4, "calc")
DEFINSTR(Add, true, true, true, 4, "+")
DEFINSTR(Sub, true, true, true, 4, "-")
DEFINSTR(Mult, true, true, true, 4, "*")
DEFINSTR(Div, true, true, true, 4, "/")
DEFINSTR(GT, true, true, true, 4, ">")
DEFINSTR(GTEq, true, true, true, 4, ">=")
DEFINSTR(Equal, true, true, true, 4, "==")
DEFINSTR(LT, true, true, true, 4, "<")
DEFINSTR(LTEq, true, true, true, 4, "<=")
DEFINSTR(ShiftL, true, true, true, 4, "<<")
DEFINSTR(ShiftR, true, true, true, 4, ">>")
