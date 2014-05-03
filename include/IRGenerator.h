#pragma once
#include <vector>
#include <string>
#include "visitor.h"
#include "ast.h"

class IRGeneratorVisitor : public Visitor {
public:
	IRGeneratorVisitor();
	void visit(ExpressionNode* node);
	void visit(AssignmentNode* node);
	void visit(WhileNode* node);
	void visit(IfNode* node);
	void visit(ReturnNode* node);
	std::string getRW2(std::string rw);


private:
	std::string CalcTree(ASTNode* node, std::string rw, std::vector<std::string>& regList, int vectStart = 0);
	std::string CalcTree(std::shared_ptr<ASTNode> node, std::string rw, std::vector<std::string>& regList, int vectStart = 0);
	void addOPInstruction(ASTNode* node, std::string workingRegister, std::string memLocation);
};