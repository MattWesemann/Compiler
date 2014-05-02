#include "IRGenerator.h"
#include "irInstruction.h"
#include "scope.h"
#include <iostream>

using namespace std;

IRGeneratorVisitor::IRGeneratorVisitor() {
}

void IRGeneratorVisitor::visit(IfNode* node){
	Visitor::visit(node->children[0]);

	bool hasElse = node->children[2]->to_type() != ASTNode::NodeType::Empty;

	auto jumpTarget = node->children[2];

	node->children[0]->addInstruction(make_shared<BFalseInstr>(to_string(jumpTarget->uniqueID), ", R0"));

	// visit block
	Visitor::visit(node->children[1]);

	if (hasElse){
		node->children[1]->addInstruction(make_shared<JumpInstr>(to_string(node->children[3]->uniqueID)));
	}

	// visit else
	if(hasElse)
		Visitor::visit(node->children[2]);
}

void IRGeneratorVisitor::visit(WhileNode* node){
	Visitor::visit(node->children[0]);
	auto jumpTarget = node->children[2];
	node->children[0]->addInstruction(make_shared<BFalseInstr>(to_string(jumpTarget->uniqueID), "R0"));

	// visit block
	Visitor::visit(node->children[1]);

	node->children[1]->addInstruction(make_shared<JumpInstr>(to_string(node->uniqueID)));
}

void IRGeneratorVisitor::visit(ExpressionNode* node){
	vector<string> regList;
	CalcTree(node, regList);
}

void IRGeneratorVisitor::visit(AssignmentNode* node){
	vector<string> regList;
	CalcTree(node->children[1], regList);
	auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();

	node->addInstruction(make_shared<MemstInstr>("R0", to_string(attr.memLoc), node->children[0]->str));
}

void IRGeneratorVisitor::visit(ReturnNode* node){
	vector<string> regList;
	CalcTree(node->children[0], regList);
	node->addInstruction(make_shared<ReturnInstr>());
}

void IRGeneratorVisitor::CalcTree(std::shared_ptr<ASTNode> node, vector<string>& regList, int vectStart){
	CalcTree(node.get(), regList, vectStart);
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

		node->addInstruction(make_shared<MemldInstr>(regList[vectStart], to_string(attr.memLoc), node->str));
	} else if(node->to_type() == ASTNode::NodeType::Literal) {

		node->addInstruction(make_shared<ImmldInstr>(regList[vectStart], node->str));
	} else {
		// check for unary
		if(node->children.size() == 1){
			CalcTree(node->children[0], regList, vectStart);

			node->addInstruction(make_shared<CalcInstr>(regList[vectStart], to_string(node->uniqueID), node->str));
		} else if(node->children[0]->regCount >= node->children[1]->regCount){
			CalcTree(node->children[0], regList, vectStart);
			CalcTree(node->children[1], regList, vectStart + 1);

			node->addInstruction(make_shared<CalcInstr>(regList[vectStart], to_string(node->uniqueID), node->str));
		} else {
			CalcTree(node->children[1], regList, vectStart);
			CalcTree(node->children[0], regList, vectStart + 1);

			node->addInstruction(make_shared<CalcInstr>(regList[vectStart], to_string(node->uniqueID), node->str));
		}

		if (node->to_type() == ASTNode::NodeType::Assignment){
			auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();

			node->addInstruction(make_shared<MemstInstr>(regList[vectStart], to_string(attr.memLoc), node->children[0]->str));
		}
	}
}
