#include "symbolVisitor.h"
#include <fstream>

using namespace std;

extern ofstream* cerrFile;

SymbolVisitor::SymbolVisitor() : Visitor() {
	current = global = make_shared<Scope>();
	memOffset = 20 * 1024;  // arbitrary start at 20k
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
	attr.isConst = node->children[0]->isConst;
	for (size_t i = 1; i < node->children.size(); ++i){
		try {
			attr.memLoc = memOffset;
			current->createSymbol(node->children[i]->str, attr);
			memOffset += 4; // ints are size 4 for now
		} catch (exception&) {
			errorFlag = true;
			*cerrFile << "Symbol already exists: " << node->children[i]->str << " at line no: " << node->children[i]->lineno << endl;
		}	
	}
	
	node->nodeScope = current;
	
	for ( auto child : node->children) 
		Visitor::visit(child);
}

void SymbolVisitor::visit(SymbolNode* node){
	auto sym = current->getSymbol(node->str);
	node->nodeScope = current;
	if (sym.get() == nullptr){
		errorFlag = true;
		*cerrFile << "Error symbol does not exist: " << node->str << " at line no: " << node->lineno << endl;
	}
}

void SymbolVisitor::visit(TypeNode* node){
	auto sym = current->getSymbol(node->str);
	node->nodeScope = current;
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


void SymbolVisitor::visit(ExpressionNode* node){
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


