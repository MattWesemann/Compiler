#pragma once
#include <string>
#include <iostream>

// if you want to run a macro on all instruction types this is how it is done
// simply define a macro expecting a name as a parameter
// if you want to add a new instruction type you only have to add it here
// some  of these macros require more arguments
#define PERFORM_INSTR(INSTR_MACRO)  \
            INSTR_MACRO(Mv)         \
            INSTR_MACRO(Memld)	\
            INSTR_MACRO(Immld)	\
            INSTR_MACRO(Memst)	\
            INSTR_MACRO(Push)		\
            INSTR_MACRO(Mempush)	\
            INSTR_MACRO(Immpush)	\
            INSTR_MACRO(Pop)		\
            INSTR_MACRO(Mempop)	\
            INSTR_MACRO(Jump)		\
            INSTR_MACRO(BFalse)	\
            INSTR_MACRO(BTrue)	\
            INSTR_MACRO(Bgt)		\
            INSTR_MACRO(Bge)		\
            INSTR_MACRO(Beq)		\
            INSTR_MACRO(Bneq)		\
            INSTR_MACRO(Blt)		\
            INSTR_MACRO(Ble)		\
            INSTR_MACRO(Reljump)	\
            INSTR_MACRO(Relbfalse)	\
            INSTR_MACRO(Relbtrue)	\
            INSTR_MACRO(Relbgt)	\
            INSTR_MACRO(Relbge)	\
            INSTR_MACRO(Relbeq)	\
            INSTR_MACRO(Relbneq)	\
            INSTR_MACRO(Relblt)	\
            INSTR_MACRO(Relble)	\
            INSTR_MACRO(Call)		\
            INSTR_MACRO(Return)	\
            INSTR_MACRO(Calc)		\
            INSTR_MACRO(Add)        \
            INSTR_MACRO(Sub)        \
            INSTR_MACRO(Mult)       \
            INSTR_MACRO(Div)        \
            INSTR_MACRO(GT)         \
            INSTR_MACRO(GTEq)       \
            INSTR_MACRO(Equal)      \
            INSTR_MACRO(LT)         \
            INSTR_MACRO(LTEq)       \
            INSTR_MACRO(ShiftL)     \
            INSTR_MACRO(ShiftR)     \


class Instruction {
public:
#define ENUMDEFINE(name) name,
	enum InstrType {
		PERFORM_INSTR(ENUMDEFINE)
	};

	Instruction(std::string operand1, std::string operand2, std::string comment);

	// size of this instruction
	size_t instrSize();

	void print(std::ostream& out);
	virtual InstrType to_type() = 0;
	virtual void emitBinary(char* bytes, size_t& offset, size_t len, size_t dataOffset) = 0;

	bool hasOp1;
	bool hasOp2;
	size_t size;

	std::string op;
	std::string operand1;
	std::string operand2;
	std::string comment;  // an optional comment of the form " ; 'comment'"
};