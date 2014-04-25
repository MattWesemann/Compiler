#include "symbolVisitor.h"

using namespace std;

SymbolVisitor::SymbolVisitor(){
	current = global = make_shared<Scope>();
}

void SymbolVisitor::visit(BlockNode* node){
	auto old = current;
	current = make_shared<Scope>(current.get());
	((Visitor*) this)->visit((EmptyNode*) node);
	current = old;
}

void SymbolVisitor::visit(DeclarationsNode* node){
	Attributes attr;
	attr.type = node->children[0]->str;
	
	for (size_t i = 1; i < node->children.size(); ++i){
		try {
			current->createSymbol(node->children[i]->str, attr);
		} catch (exception&) {
			cerr << "Symbol already exists: " << node->children[i]->str << endl;
		}	
	}
}

void SymbolVisitor::visit(SymbolNode* node){
	auto sym = current->getSymbol(node->str);
	if (sym.get() == nullptr)
		cerr << "Error symbol does not exist: " << sym->getName() << endl;
}