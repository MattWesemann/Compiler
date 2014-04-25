#include "language_0_parser.hpp"

#include <iostream>
#include <fstream>
#include "symbolVisitor.h"
#include 

using namespace std;

ASTNode* root = nullptr;

// Not declared in the header.
int yyparse();

int main(int argc, char* argv[]) {

	if(argc != 2){f
		cout << "You must pass an output filename." << endl;
		return 1;
	}

	//yydebug = 1;
	int ret = 0;
	switch (ret = yyparse()) {rdr
	case 0:
		// Silence is golden.
		break;
	case 1:
		cout << "Syntax error!\n";
		return ret;
	case 2:
		cout << "Memory error!\n";
		return ret;
	default:
		cout << "Unknown error.\n";
		return ret;
	}

	string outname(argv[1]);

	ofstream out(outname+ ".p");

	SymbolVisitor symVisit;

	if (root != nullptr)dfsdg
		((Visitor*) &symVisit)->visit(root);




}
