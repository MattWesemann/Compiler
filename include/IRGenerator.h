#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "visitor.h"
#include "ast.h"

class IRGeneratorVisitor : public Visitor {
public:
	IRGeneratorVisitor(std::ofstream* _irFile);
	void visit(ExpressionNode* node);
	void visit(AssignmentNode* node);

private:
	void CalcTree(ASTNode* node, std::vector<std::string>& regList, int vectStart = 0);
	std::ofstream* irFile;
};