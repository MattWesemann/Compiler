#include "IRGenerator.h"
#include "scope.h"
#include <iostream>

using namespace std;

IRGeneratorVisitor::IRGeneratorVisitor(ofstream* _irFile) : irFile(_irFile){
}

void IRGeneratorVisitor::visit(IfNode* node){
	Visitor::visit(node->children[0]);

	bool hasElse = node->children[2]->to_type() != ASTNode::NodeType::Empty;

	ASTNode* jumpTarget = node->children[2];

	*irFile << "bfalse " << jumpTarget->uniqueID << ", R0" << endl;

	// visit block
	Visitor::visit(node->children[1]);

	if(hasElse)
		*irFile << "jump " << node->children[3]->uniqueID << endl;

	// visit else
	if(hasElse)
		Visitor::visit(node->children[2]);
}

void IRGeneratorVisitor::visit(WhileNode* node){
	Visitor::visit(node->children[0]);
	ASTNode* jumpTarget = node->children[2];
	*irFile << "bfalse " << jumpTarget->uniqueID << ", R0" << endl;

	// visit block
	Visitor::visit(node->children[1]);

	*irFile << "jump " << node->uniqueID << endl;
}

void IRGeneratorVisitor::visit(ExpressionNode* node){
	vector<string> regList;
	CalcTree(node, regList);
}

void IRGeneratorVisitor::visit(AssignmentNode* node){
	vector<string> regList;
	CalcTree(node->children[1], regList);
	auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();
	*irFile << "memst " << "R0" << ", " << attr.memLoc << " ; " << node->children[0]->str << endl;
}

void IRGeneratorVisitor::CalcTree(ASTNode* node, vector<string>& regList, int vectStart) {
	if(regList.size() == 0)
			regList.push_back("R0");

	while (node->regCount + vectStart > regList.size()) {
		size_t currentSize = regList.size();
		for(size_t i = regList.size(); i < 2*currentSize; ++i)
			regList.push_back("R" + to_string(i));
	} 

	if (node->to_type() == ASTNode::NodeType::Symbol) {
		auto attr = node->nodeScope->getSymbol(node->str)->getAttributes();
		*irFile << "memld " << regList[vectStart] << ", " << attr.memLoc << " ; " << node->str << endl;
	} else if(node->to_type() == ASTNode::NodeType::Literal) {
		*irFile << "immld " << regList[vectStart] << ", " << node->str << endl;
	} else {
		// check for unary
		if(node->children.size() == 1){
			CalcTree(node->children[0], regList, vectStart);
			*irFile << "calc " << regList[vectStart] << ", " << node->uniqueID << " ; " << node->str << endl;
		} else if(node->children[0]->regCount >= node->children[1]->regCount){
			CalcTree(node->children[0], regList, vectStart);
			CalcTree(node->children[1], regList, vectStart + 1);
			*irFile << "calc " << regList[vectStart] << ", " << node->uniqueID << " ; " << node->str << endl;
		} else {
			CalcTree(node->children[1], regList, vectStart);
			CalcTree(node->children[0], regList, vectStart + 1);
			*irFile << "calc " << regList[vectStart] << ", " << node->uniqueID << " ; " << node->str << endl;
		}

		if(node->to_type() == ASTNode::NodeType::Assignment){
			auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();
			*irFile << "memst " << regList[vectStart] << ", " << attr.memLoc << " ; " << node->children[0]->str << endl;
		}
	}
}
