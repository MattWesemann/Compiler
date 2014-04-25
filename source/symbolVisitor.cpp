#include "symbolVisitor.h"

using namespace std;

SymbolVisitor::SymbolVisitor(){
	current = global = make_shared<Scope>();
}

void SymbolVisitor::visit(BlockNode* node){
	auto old = current;
	current = make_shared<Scope>(current.get());	
	node->nodeScope = current;
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
	
	node->nodeScope = current;
}

void SymbolVisitor::visit(SymbolNode* node){
	auto sym = current->getSymbol(node->str);
	node->nodeScope = current;
	if (sym.get() == nullptr)
		cerr << "Error symbol does not exist: " << sym->getName() << endl;
}




void SymbolVisitor::visit(EmptyNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(AssignmentNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(ElseNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(ExpressionNode node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(IfNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(LiteralNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(OperatorNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(ProgramNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(ReturnNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}

void SymbolVisitor::visit(SymbolNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}

void SymbolVisitor::visit(WhileNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}



void SymbolVisitor::visit(ForNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


void SymbolVisitor::visit(DoWhileNode* node){
	node->nodeScope = current;
	for ( auto child : node->children) {
		Visitor::visit(child);
	}
}


