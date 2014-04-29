#include "symbolVisitor.h"
#include <fstream>

using namespace std;

SymbolVisitor::SymbolVisitor() : Visitor() {
	current = global = make_shared<Scope>();
	memOffset = 20 * 1024;  // arbitrary start at 20k
}

void SymbolVisitor::visit(BlockNode* node){
	auto old = current;
	current = make_shared<Scope>(current.get());	
	node->nodeScope = current;
	Visitor::visit(node);
	current = old;
}

string SymbolVisitor::getSymName(ASTNode* node){
	if (node->to_type() == ASTNode::Assignment)
		return node->children[0]->str;
	else
		return node->str;
}

void SymbolVisitor::visit(DeclarationNode* node){
	Attributes attr;
	attr.type = node->children[0]->str;
	attr.isConst = node->children[0]->isConst;
	for (size_t i = 1; i < node->children.size(); ++i){
		try {
			attr.memLoc = memOffset;
			string name = getSymName(node->children[i].get());
			current->createSymbol(name, attr);
			memOffset += 4; // ints are size 4 for now
		} catch (exception&) {
			errors.push_back("Symbol already exists: " + node->children[i]->str + " at line no: " + to_string(node->children[i]->lineno));
		}	
	}
	
	node->nodeScope = current;
	
	Visitor::visit(node);
}

void SymbolVisitor::visit(SymbolNode* node){
	auto sym = current->getSymbol(node->str);
	node->nodeScope = current;
	if (sym.get() == nullptr){
		errors.push_back("Error symbol does not exist: " + node->str + " at line no: " + to_string(node->lineno));
	}
}

void SymbolVisitor::visit(TypeNode* node){
	auto sym = current->getSymbol(node->str);
	node->nodeScope = current;
}

void SymbolVisitor::visit(EmptyNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(AssignmentNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(ElseNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(ExpressionNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(IfNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(LiteralNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(ProgramNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(ReturnNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(WhileNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(ForNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}

void SymbolVisitor::visit(DoWhileNode* node){
	node->nodeScope = current;
	Visitor::visit(node);
}