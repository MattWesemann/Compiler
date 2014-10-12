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
#include "Compiler.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {

	if(argc != 2){
		cerr << "You must pass an output filename." << endl;
		return 1;
	}

	string outname(argv[1]);

    ofstream cerrFile(outname + ".err");
	ofstream outRaw(outname + ".p");
	ofstream outA(outname + ".a");
	ofstream irOut(outname + ".ir");
	ofstream asmOut(outname + ".asm");

    std::vector<char> bytes;
    ReadAllBytes(cin, bytes);
    
    //yydebug = 1;

    Compiler compiler;
    auto error = compiler.Compile(bytes.data(), bytes.size());

    cerrFile << compiler.GetError() << endl;

    int ret = error;
    switch (error) {
    case ErrorNone:
		// Silence is golden.
		break;
    case ErrorSyntax:
		cerrFile << "Syntax error!\n";
		return ret;
    case ErrorMemory:
		cerrFile << "Memory error!\n";
		return ret;
	default:
		cerrFile << "Unknown error.\n";
		return ret;
	}

    compiler.root->print_tree(outRaw);

	SymbolVisitor symVisit;

    //((Visitor*) &symVisit)->visit(compiler.root);

	if (symVisit.hadError(&cerrFile)){
		ret = 1;
		return ret;
	}

	ConstVisitor constvisit;
	//((Visitor*) &constvisit)->visit(compiler.root);

	if (constvisit.hadError(&cerrFile)){
		ret = 1;
		return ret;
	}
		
	// now that AST is done print finished tree
    compiler.root->print_tree(outA);

	RegisterVisitor regVisit;
	//((Visitor*) &regVisit)->visit(compiler.root);

	IRGeneratorVisitor irGenVisit;
	//((Visitor*) &irGenVisit)->visit(compiler.root);

    OffsetVisitor offvisit(compiler.root->instructionSize * 4);  // all instructions are 4 bytes
	//((Visitor*) &offvisit)->visit(compiler.root);

	IRPrinterVisitor irVisit(&irOut);
	//((Visitor*) &irVisit)->visit(compiler.root);

	IRTox86Visitor asmVisit;
	//((Visitor*) &asmVisit)->visit(compiler.root);

	IRPrinterVisitor asmPrintVisit(&asmOut);
	//((Visitor*) &asmPrintVisit)->visit(compiler.root);

	x86Jitter jitter;

	void* buf = jitter.allocateMemory(4 * 1024, 4 * 1024);

	x86EmitterVisitor x86Visit(buf, jitter.getSize(), (size_t) ((char*) buf + jitter.getDataOffset()));
    ((Visitor*) &x86Visit)->visit(compiler.root);

	//auto fn = jitter.getFunction();

	//int rett = (int) (size_t) fn();

	return ret;
}
