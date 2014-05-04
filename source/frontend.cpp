#include "language_0_parser.hpp"

#include <iostream>
#include <fstream>
#include "symbolVisitor.h"
#include <string>
#include "constVisitor.h"
#include "IRGenerator.h"
#include "irPrinter.h"
#include "registerVisitor.h"
#include "irTox86Visitor.h"
#include "x86EmitterVisitor.h"
#include "jit.h"
#include "offsetVisitor.h"

using namespace std;

shared_ptr<ofstream> cerrFile;

int yyparse(shared_ptr<ASTNode>& root);

void handleError(const char* msg, int lineno){
	*cerrFile << "[Error] " << msg << " on line " << lineno << endl;
}

int main(int argc, char* argv[]) {

	if(argc != 2){
		cerr << "You must pass an output filename." << endl;
		return 1;
	}

	string outname(argv[1]);

	cerrFile = make_shared<ofstream>(outname + ".err");
	ofstream outRaw(outname + ".p");
	ofstream outA(outname + ".a");
	ofstream irOut(outname + ".ir");
	ofstream asmOut(outname + ".asm");

	shared_ptr<ASTNode> root;

	//yydebug = 1;
	int ret = 0;
	switch (ret = yyparse(root)) {
	case 0:
		// Silence is golden.
		break;
	case 1:
		*cerrFile << "Syntax error!\n";
		return ret;
	case 2:
		*cerrFile << "Memory error!\n";
		return ret;
	default:
		*cerrFile << "Unknown error.\n";
		return ret;
	}

	root->print_tree(outRaw);

	SymbolVisitor symVisit;

	if (root != nullptr)
		((Visitor*) &symVisit)->visit(root.get());

	if (symVisit.hadError(cerrFile.get())){
		ret = 1;
		return ret;
	}

	ConstVisitor constvisit;
	if (root != nullptr)
		((Visitor*) &constvisit)->visit(root.get());

	if (constvisit.hadError(cerrFile.get())){
		ret = 1;
		return ret;
	}
		
	// now that AST is done print finished tree
	root->print_tree(outA);

	RegisterVisitor regVisit;
	((Visitor*) &regVisit)->visit(root);

	IRGeneratorVisitor irGenVisit;
	((Visitor*) &irGenVisit)->visit(root);

	OffsetVisitor offvisit(root->instructionSize*4);  // all instructions are 4 bytes
	((Visitor*) &offvisit)->visit(root);

	IRPrinterVisitor irVisit(&irOut);
	((Visitor*) &irVisit)->visit(root);

	IRTox86Visitor asmVisit;
	((Visitor*) &asmVisit)->visit(root);

	IRPrinterVisitor asmPrintVisit(&asmOut);
	((Visitor*) &asmPrintVisit)->visit(root);

	x86Jitter jitter;

	void* buf = jitter.allocateMemory(4 * 1024, 4 * 1024);

	x86EmitterVisitor x86Visit(buf, jitter.getSize(), (size_t) ((char*) buf + jitter.getDataOffset()));
	((Visitor*) &x86Visit)->visit(root);

	//auto fn = jitter.getFunction();

	//int rett = (int) (size_t) fn();

	return ret;
}
