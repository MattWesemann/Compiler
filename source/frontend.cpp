#include "language_0_parser.hpp"

#include <iostream>
#include <fstream>
#include "symbolVisitor.h"
#include <string>


using namespace std;

ASTNode* root = nullptr;
ofstream cerrFile;

// Not declared in the header.
int yyparse();

void handleError(const char* msg){
	cerrFile << "[Error] " << msg << endl;
}

int main(int argc, char* argv[]) {

	if(argc != 2){
		cerr << "You must pass an output filename." << endl;
		return 1;
	}

	string outname(argv[1]);

	cerrFile = ofstream(outname + ".err");

	//yydebug = 1;
	int ret = 0;
	switch (ret = yyparse()) {
	case 0:
		// Silence is golden.
		break;
	case 1:
		cerrFile << "Syntax error!\n";
		return ret;
	case 2:
		cerrFile << "Memory error!\n";
		return ret;
	default:
		cerrFile << "Unknown error.\n";
		return ret;
	}



	ofstream out(outname + ".p");

	root->print_tree(out);

	SymbolVisitor symVisit;

	if (root != nullptr)
		((Visitor*) &symVisit)->visit(root);

	// now that AST is done print finished tree
	out = ofstream(outname + ".a");
	root->print_tree(out);


}
