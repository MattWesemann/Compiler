#include "language_0_parser.hpp"

#include <iostream>
#include <fstream>
#include "symbolVisitor.h"
#include <string>
#include "constVisitor.h"


using namespace std;

ASTNode* root = nullptr;
ofstream* cerrFile;

// Not declared in the header.
int yyparse();

void handleError(const char* msg, int lineno){
	*cerrFile << "[Error] " << msg << " on line " << lineno << endl;
}

int main(int argc, char* argv[]) {

	if(argc != 2){
		cerr << "You must pass an output filename." << endl;
		return 1;
	}

	string outname(argv[1]);

	cerrFile = new ofstream(outname + ".err");

	//yydebug = 1;
	int ret = 0;
	switch (ret = yyparse()) {
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



	ofstream* out = new ofstream(outname + ".p");

	root->print_tree(*out);

	SymbolVisitor symVisit;

	if (root != nullptr)
		((Visitor*) &symVisit)->visit(root);

	if (symVisit.hadError())
		ret = 1;


	ConstVisitor constvisit;
	if (root != nullptr)
		((Visitor*) &constvisit)->visit(root);

	if (constvisit.hadError())
		ret = 1;

	// now that AST is done print finished tree
	out = new ofstream(outname + ".a");
	root->print_tree(*out);

	return ret;
}
