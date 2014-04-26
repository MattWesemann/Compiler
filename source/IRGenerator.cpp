#include "IRGenerator.h"
#include "scope.h"
#include <iostream>

using namespace std;

IRGeneratorVisitor::IRGeneratorVisitor(ofstream* _irFile) : irFile(_irFile){
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

	while (node->regCount + vectStart > regList.size()) {
		if(regList.size() == 0)
			regList.push_back("R0");

		size_t currentSize = regList.size();
		for(size_t i = regList.size(); i < 2*currentSize; ++i)
			regList.push_back("R" + to_string(i));
	} 

	// TODO add unary

	if (node->to_type() == ASTNode::NodeType::Symbol) {
		auto attr = node->nodeScope->getSymbol(node->str)->getAttributes();
		*irFile << "memld " << regList[vectStart] << ", " << attr.memLoc << " ; " << node->str << endl;
	} else if(node->to_type() == ASTNode::NodeType::Literal) {
		*irFile << "immld " << regList[vectStart] << ", " << node->str << endl;
	} else {

		cout << "Left " << node->str << ": " << node->children[0]->regCount << ", Right " << node->str << ": " << node->children[1]->regCount << endl;

		if(node->children[0]->regCount >= node->children[1]->regCount){
			CalcTree(node->children[0], regList, vectStart);
			CalcTree(node->children[1], regList, vectStart + 1);
			*irFile << "calc " << regList[vectStart] << ", " << node->str << endl;
		} else {
			CalcTree(node->children[1], regList, vectStart);
			CalcTree(node->children[0], regList, vectStart + 1);
			*irFile << "calc " << regList[vectStart] << ", " << node->str << endl;
		}

		if(node->to_type() == ASTNode::NodeType::Assignment){
			auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();
			*irFile << "memst " << regList[vectStart] << ", " << attr.memLoc << " ; " << node->children[0]->str << endl;
		}
	}
}
