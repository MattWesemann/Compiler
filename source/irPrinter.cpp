#include "irPrinter.h"

using namespace std;

IRPrinterVisitor::IRPrinterVisitor(ofstream* _irFile) : irFile(_irFile){
}

void IRPrinterVisitor::visit(ASTNode* node) {
	// post order visit
	Visitor::visit(node); 
	node->printInstructions(*irFile); 
}  