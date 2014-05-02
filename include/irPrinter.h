#pragma once
#include "visitor.h"
#include "instruction.h"
#include <fstream>
#include <vector>

class IRPrinterVisitor : public Visitor {
public:
	IRPrinterVisitor(std::ofstream* _irFile);
	void visit(ASTNode* node);

private:
	std::ofstream* irFile;
};