#pragma once

#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "scope.h"
#include "visitor.h"
#include <memory>

class ASTNode {

public:
	// When adding to this, remember to adjust std::to_string in ast.cpp!
	enum NodeType {
		Empty,
		Assignment,
		Block,
		Declaration,
		Declarations,
		Else,
		Expression,
		If,
		Literal,
		Operator,
		Program,
		Return,
		Symbol,
		Type,
		While,
		For,
		DoWhile
	};

	// This guarentees a unique number for every node created,
	//   but causes problems with copy constructors. There are
	//   over 200 nodes created before any of the final ones
	//   which we see in the tree are created!
	// TODO: Work around copy construtors creating false nodes.
	static int nodeCount;

	void addChild(ASTNode* node);

	NodeType type;
	size_t uniqueID;

	// This is only used by Symbol.
	bool isConst;

	// This is used for Sethi-Ullman
	size_t regCount;
	
	//this is for prefix-postfix operators
	bool isPrefix;

	// This allows the ast to know what scope it's in. 
	std::shared_ptr<Scope> nodeScope;

	// Some nodes (e.g. int and float literals) need to save data, but not
	//   as strings.
	// TODO: More flexibility in type stored.
	std::string str;

	// TODO: Children should know about their parent.
	std::vector<ASTNode*> children;

	ASTNode(std::string str = "")
		: isConst(false), str(str) {
		nodeCount += 1;
		uniqueID = nodeCount;
		type = NodeType::Empty;
		regCount = 0;
	}

	// Modifies a vector to add copies of all children, grand children, etc.
	//   The order is the result of a depth-first search.
	void find_all_children(std::vector<ASTNode*>& nodes) const;

	// Prints the tree in accordance with Hellman's specification.
	void print_tree(std::ostream& os);

	void makeConst();
	int lineno;

	virtual void accept(Visitor* visitor) = 0;
	virtual std::string to_string() const = 0;
	virtual ASTNode::NodeType to_type() = 0;
};

std::string to_string(const ASTNode* node);

bool operator==(const ASTNode& a, const ASTNode& b);

#define NODEDEFINE(name)                                    \
	class name ## Node : public ASTNode {                   \
		public:                                             \
		name ## Node(std::string str = "") : ASTNode(str){} \
		void accept(Visitor* visitor) {                     \
			visitor->visit(this);                           \
		}                                                   \
		std::string to_string() const { 					\
			if(type == NodeType::Program)                   \
				return "Program";                           \
			return #name;								    \
		}													\
		ASTNode::NodeType to_type() {                       \
			return ASTNode::NodeType::name;                 \
		}                                                   \
	}

NODEDEFINE(Empty);
NODEDEFINE(Assignment);
NODEDEFINE(Block);
NODEDEFINE(Declaration);
NODEDEFINE(Declarations);
NODEDEFINE(Else);
NODEDEFINE(Expression);
NODEDEFINE(If);
NODEDEFINE(Literal);
NODEDEFINE(Operator);
NODEDEFINE(Program);
NODEDEFINE(Return);
NODEDEFINE(Symbol);
NODEDEFINE(Type);
NODEDEFINE(While);
NODEDEFINE(For);
NODEDEFINE(DoWhile); 
