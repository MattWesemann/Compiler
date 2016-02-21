#include "visitor.h"
#include "ast.h"

using namespace std;

Visitor::Visitor(){
}

void Visitor::visit(Visitor* pVisitor, shared_ptr<ASTNode> node){
    pVisitor->visit(node);
}

void Visitor::visit(shared_ptr<ASTNode> node){
	visit(node.get());
}

void Visitor::visit(ASTNode* node){
	node->accept(this);
}

#define visitFnImpl(name) void Visitor::visit(name ## Node *node){  \
    for (auto child : node->children)                               \
        Visitor::visit(child);                                      \
    }

PERFORM_NODES(visitFnImpl)

bool Visitor::hadError(ostream* cerrFile){
	if (cerrFile)
		for (auto error : errors)
			*cerrFile << error << endl;
	
	return errors.size() != 0;
}