#include "constVisitor.h"
#include "ast.h"
#include <fstream>

using namespace std;

extern ofstream* cerrFile;

ConstVisitor::ConstVisitor() : Visitor() {

}

void ConstVisitor::visit(AssignmentNode* node){
	if (node->nodeScope->getSymbol(node->children[0]->str)->getAttributes().isConst){
		errorFlag = true;
		*cerrFile << "Reassigning const variable: " << node->children[0]->str << " at linge no: " << node->lineno << endl;
	}
}

void ConstVisitor::visit(DeclarationsNode* node){
	//This should check to see if it is an assignment node(as opposed to a identifier) and will check if you tryied to chain assign to a const variable.
	for (auto child : node->children){
		if (!(child->children.empty())){
			Visitor::visit(child->children[1]);
		}

	}	
}
