#include "constVisitor.h"
#include "ast.h"
#include <fstream>

using namespace std;

ConstVisitor::ConstVisitor() : Visitor() {

}

void ConstVisitor::visit(AssignmentNode* node){
	if (node->nodeScope->getSymbol(node->children[0]->str)->getAttributes().isConst){
		errors.push_back("Reassigning const variable: " + node->children[0]->str + " at line no: " + to_string(node->lineno));
	}
}

void ConstVisitor::visit(DeclarationNode* node){
	//This should check to see if it is an assignment node(as opposed to a identifier) and will check if you tried to chain assign to a const variable.
	for (auto child : node->children){
		if (!(child->children.empty())){
			Visitor::visit(child->children[1]);
		}
	}	
}

void ConstVisitor::visit(ExpressionNode* node){
	if (node->str == "++" || node->str == "--"){
		if (node->children[0]->nodeScope->getSymbol(node->children[0]->str)->getAttributes().isConst){
			errors.push_back("Reassigning const variable: " + node->children[0]->str + " at line no: " + to_string(node->lineno));
		}
	}
}
