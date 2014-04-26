#include "IRGenerator.h"
#include "scope.h"

using namespace std;

IRGeneratorVisitor::IRGeneratorVisitor(ofstream* _irFile) : irFile(_irFile){
}

void IRGeneratorVisitor::visit(ExpressionNode* node){
	vector<string> regList;
	CalcTree(node, regList, 0);
}

void IRGeneratorVisitor::visit(AssignmentNode* node){
	(void) node;
}

void IRGeneratorVisitor::CalcTree(ASTNode* node, vector<string>& regList, int vectStart) {

	if (node->regCount > regList.size()) {
		if(regList.size() == 0)
			regList.push_back("R" + 0);

		size_t currentSize = regList.size();
		for(size_t i = regList.size(); i < 2*currentSize; ++i)
			regList.push_back("R" + i);
	} 

	// TODO add unary

	if (node->to_type() == ASTNode::NodeType::Symbol) {
		auto attr = node->nodeScope->getSymbol(node->str)->getAttributes();
		*irFile << "memld " << regList[vectStart] << ", " << attr.memLoc;
	} else if(node->to_type() == ASTNode::NodeType::Literal) {
		*irFile << "immld " << regList[vectStart] << ", " << node->str;
	} else {
		if(node->children[0]->regCount >= node->children[1]->regCount){
			CalcTree(node->children[0], regList);
			CalcTree(node->children[1], regList, 1);
			*irFile << "calc " << regList[vectStart+1] << ", " << node;
		} else {
			CalcTree(node->children[1], regList);
			CalcTree(node->children[0], regList, 1);
			*irFile << "calc " << regList[vectStart+1] << ", " << node;
		}
	}
}
