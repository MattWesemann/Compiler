#include "IRGenerator.h"
#include "irInstruction.h"
#include "scope.h"
#include <iostream>

using namespace std;

IRGeneratorVisitor::IRGeneratorVisitor() {
}

void IRGeneratorVisitor::visit(ProgramNode* node){
	Visitor::visit(node);
	
	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}

void IRGeneratorVisitor::visit(IfNode* node){
	Visitor::visit(node->children[0]);

	bool hasElse = node->children[2]->to_type() != ASTNode::NodeType::Empty;

	auto jumpTarget = node->children[2];

	// visit block
	Visitor::visit(node->children[1]);

	if (hasElse){
		// visit else
		Visitor::visit(node->children[2]);
		node->children[1]->addInstruction(make_shared<JumpInstr>("+" + to_string(node->children[2]->instructionSize + 1))); //PC is not incremented for some reason. 
	}
	node->children[0]->addInstruction(make_shared<BFalseInstr>("+" + to_string(node->children[1]->instructionSize + 1), "R0")); //PC is not incremented for some reason. 
	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}

void IRGeneratorVisitor::visit(BlockNode* node){
	Visitor::visit(node);
	
	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}

void IRGeneratorVisitor::visit(DeclarationNode* node){
	Visitor::visit(node);
	
	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}
void IRGeneratorVisitor::visit(WhileNode* node){
	Visitor::visit(node->children[0]);
	auto jumpTarget = node->children[2];

	// visit block
	Visitor::visit(node->children[1]);
	
	JumpInstr instruction;
	
	node->children[0]->addInstruction(make_shared<BFalseInstr>("+" + to_string(node->children[1]->instructionSize +2), "R0")); //1 for the instruction we add in children[1], 1 because pc is not incremented.

	node->children[1]->addInstruction(make_shared<JumpInstr>("-" + to_string( node->children[1]->instructionSize + node->children[0]->instructionSize )));

	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}

void IRGeneratorVisitor::visit(ExpressionNode* node){
	vector<string> regList;
	CalcTree(node, "R9", regList);
}

void IRGeneratorVisitor::visit(AssignmentNode* node){
	vector<string> regList;
	CalcTree(node->children[1], "R9", regList);
	auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();

	node->addInstruction(make_shared<MemstInstr>("R0", to_string(attr.memLoc), node->children[0]->str));
	
	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}

void IRGeneratorVisitor::visit(ReturnNode* node){
	vector<string> regList;

	if(node->children.size())
		CalcTree(node->children[0], "R9", regList);

	node->addInstruction(make_shared<ReturnInstr>());

	for( auto child : node->children){
		node->instructionSize += child->instructionSize;
	}
}

string IRGeneratorVisitor::CalcTree(std::shared_ptr<ASTNode> node, string rw, vector<string>& regList, int vectStart){
	return CalcTree(node.get(), rw, regList, vectStart);
}

string IRGeneratorVisitor::getRW2(string rw) {
	return "R" + to_string(rw[1] - '0' + 1);
}

string IRGeneratorVisitor::CalcTree(ASTNode* node, string rw, vector<string>& regList, int vectStart) {
	
	int x = regList.size();
	while (x < 9) {
		regList.push_back("R" + to_string(x++));
	}
	

	while (node->regCount + vectStart > regList.size()) {
		size_t currentSize = regList.size();
		
		regList.push_back("V" + to_string(x++-9));
	} 

	string u = regList[vectStart];

	if (node->to_type() == ASTNode::NodeType::Symbol) {
		auto attr = node->nodeScope->getSymbol(node->str)->getAttributes();
		node->addInstruction(make_shared<MemldInstr>(rw, to_string(attr.memLoc), node->str));
		return rw;
	} else if(node->to_type() == ASTNode::NodeType::Literal) {
		node->addInstruction(make_shared<ImmldInstr>(rw, node->str));
		return rw;
	} 

	else {
		// check for unary -- this is "fine"
		if(node->children.size() == 1) {
			string s = CalcTree(node->children[0], rw, regList, vectStart);
			if (s[0] == 'V') {
				node->addInstruction(make_shared<PopInstr>(rw, node->str));
				s = rw;
			} 
				unaryInstruction(node, rw, s);
		} 

		else if(node->children[0]->regCount >= node->children[1]->regCount) {
			string s = CalcTree(node->children[0], rw, regList, vectStart);
			string t = CalcTree(node->children[1], getRW2(rw), regList, vectStart + 1);
			
			if (t[0] == 'V') {
				node->addInstruction(make_shared<PopInstr>(rw, node->str));
				t = rw;
			}
			if(s[0] == 'V') {
				node->addInstruction(make_shared<PopInstr>(getRW2(rw), node->str));
				s = getRW2(rw);
			}
			if (u[0] == 'V') {
				addOPInstruction(node, rw, s, t);
				node->addInstruction(make_shared<PushInstr>(u, node->str));

			} else {
				addOPInstruction(node, u, s, t);
			}
			return u;
		}

		// comment out...
		else {
			string s = CalcTree(node->children[1], rw, regList, vectStart);
			string t = CalcTree(node->children[0], getRW2(rw), regList, vectStart + 1);
			auto attr = node->nodeScope->getSymbol(node->str)->getAttributes();

			if (t[0] == 'V') {
				node->addInstruction(make_shared<PopInstr>(rw, node->str));
				t = rw;
			}
			if(s[0] == 'V') {
				node->addInstruction(make_shared<PopInstr>(getRW2(rw), node->str));
				s = getRW2(rw);
			}
			if (u[0] == 'V') {
				addOPInstruction(node, rw, t, s);
				node->addInstruction(make_shared<PushInstr>(u, node->str));		

			} else {
				addOPInstruction(node, u, t, s);
			}
			return u;
		}

		if (node->to_type() == ASTNode::NodeType::Assignment){
			auto attr = node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes();

			node->addInstruction(make_shared<MemstInstr>(regList[vectStart], to_string(attr.memLoc), node->children[0]->str));
		}
	}
	
	return u;
}

void IRGeneratorVisitor::addOPInstruction(ASTNode* node, string target, string lReg, string rReg) {

	if (node->str == "+") {
		node->addInstruction(make_shared<AddInstr>(target, lReg, rReg));
	}
	else if (node->str == "-") {
		node->addInstruction(make_shared<SubInstr>(target, lReg, rReg));
	}
	else if (node->str == "*") {
		node->addInstruction(make_shared<MultInstr>(target, lReg, rReg));
	}
	else if (node->str == "/") {
		node->addInstruction(make_shared<DivInstr>(target, lReg, rReg));
	}
	else if (node->str == ">") {
		node->addInstruction(make_shared<GTInstr>(target, lReg, rReg));
	}
	else if (node->str == ">=") {
		node->addInstruction(make_shared<GTEqInstr>(target, lReg, rReg));
	}
	else if (node->str == "==") {
		node->addInstruction(make_shared<EqualInstr>(target, lReg, rReg));
	}
	else if (node->str == "<") {
		node->addInstruction(make_shared<LTInstr>(target, lReg, rReg));
	}
	else if (node->str == "<=") {
		node->addInstruction(make_shared<LTEqInstr>(target, lReg, rReg));
	}
	else if (node->str == "<<") {
		node->addInstruction(make_shared<ShiftLInstr>(target, lReg, rReg));
	}
	else if (node->str == ">>") {
		node->addInstruction(make_shared<ShiftRInstr>(target, lReg, rReg));
	} else {
		node->addInstruction(make_shared<CalcInstr>(target, "", node->str));
	}

}

void IRGeneratorVisitor::unaryInstruction(ASTNode* node, string target, string lReg) {

	/*if (node->str == "+") {
		node->addInstruction(make_shared<AddInstr>(target, lReg, rReg));
	}
	else if (node->str == "-") {
		node->addInstruction(make_shared<SubInstr>(target, lReg, rReg));
	}
	else if (node->str == "*") {
		node->addInstruction(make_shared<MultInstr>(target, lReg, rReg));
	}
	else if (node->str == "/") {
		node->addInstruction(make_shared<DivInstr>(target, lReg, rReg));
	}
	else if (node->str == ">") {
		node->addInstruction(make_shared<GTInstr>(target, lReg, rReg));
	}
	else if (node->str == ">=") {
		node->addInstruction(make_shared<GTEqInstr>(target, lReg, rReg));
	}
	else if (node->str == "==") {
		node->addInstruction(make_shared<EqualInstr>(target, lReg, rReg));
	}
	else if (node->str == "<") {
		node->addInstruction(make_shared<LTInstr>(target, lReg, rReg));
	}
	else if (node->str == "<=") {
		node->addInstruction(make_shared<LTEqInstr>(target, lReg, rReg));
	}
	else if (node->str == "<<") {
		node->addInstruction(make_shared<ShiftLInstr>(target, lReg, rReg));
	}
	else if (node->str == ">>") {
		node->addInstruction(make_shared<ShiftRInstr>(target, lReg, rReg));
	} else {
		cout << "Does this happen?\n";
		node->addInstruction(make_shared<CalcInstr>(target, "", node->str));
	}*/
}
