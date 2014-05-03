#pragma once
#include <vector>
#include <string>
#include "visitor.h"
#include "ast.h"

class IRGeneratorVisitor : public Visitor {
public:
	IRGeneratorVisitor();
	void visit(ProgramNode* node);
	void visit(ExpressionNode* node);
	void visit(AssignmentNode* node);
	void visit(WhileNode* node);
	void visit(IfNode* node);
	void visit(ReturnNode* node);
	void visit(BlockNode* node);
	void visit(DeclarationNode* node);

private:
	std::string CalcTree(ASTNode* node, std::string rw, std::vector<std::string>& regList, int vectStart = 0);
	std::string CalcTree(std::shared_ptr<ASTNode> node, std::string rw, std::vector<std::string>& regList, int vectStart = 0);
	void addOPInstruction(ASTNode* node, std::string workingRegister, std::string memLocation);
	std::string getRW2(std::string rw);

};

