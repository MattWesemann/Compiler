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
		*cerrFile << "Reassigning const variable" << endl;
	}
}

void ConstVisitor::visit(DeclarationsNode* node){
	// do nothing - and keep the warnings away
	(void) node;
}