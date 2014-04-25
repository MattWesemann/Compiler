#include "language_0_parser.hpp"

#include <iostream>
#include "symbolVisitor.h"

using namespace std;

ASTNode* root = nullptr;

// Not declared in the header.
int yyparse();

int main() {
	//yydebug = 1;
	switch (yyparse()) {
	case 0:
		// Silence is golden.
		break;
	case 1:
		cout << "Syntax error!\n";
		break;
	case 2:
		cout << "Memory error!\n";
		break;
	default:
		cout << "Unknown error.\n";
	}

	SymbolVisitor symVisit;

	if (root != nullptr)
		((Visitor*) &symVisit)->visit(root);

}
