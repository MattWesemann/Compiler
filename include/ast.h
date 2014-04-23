#pragma once

#include <array>
#include <iostream>
#include <string>
#include <vector>

class ASTNode {

public:
	// When adding to this, remember to adjust std::to_string in ast.cpp!
	enum Type {
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

	void addChild(const ASTNode& node);

	Type type;
	size_t uniqueID;

	// This is only used by Symbol.
	bool isConst;

	// Some nodes (e.g. int and float literals) need to save data, but not
	//   as strings.
	// TODO: More flexibility in type stored.
	std::string str;

	// TODO: Children should know about their parent.
	std::vector<ASTNode> children;

	ASTNode(Type type = Empty, std::string str = "")
	: type(type), isConst(false), str(str) {
		nodeCount += 1;
		uniqueID = nodeCount;
	}

	// Modifies a vector to add copies of all children, grand children, etc.
	//   The order is the result of a depth-first search.
	void find_all_children(std::vector<ASTNode>& nodes) const;

	// Prints the tree in accordance with Hellman's specification.
	void print_tree(std::ostream& os);

	void makeConst();
};

std::string to_string(const ASTNode& node);
std::string to_string(ASTNode::Type type);

bool operator==(const ASTNode& a, const ASTNode& b);

