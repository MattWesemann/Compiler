#pragma once

#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "scope.h"
#include <memory>
#include "instruction.h"

// if you want to run a macro on all nodes this is how it is done
// simply define a macro expecting a name as a parameter
// if you want to add a new node type you only have to add it here
#define PERFORM_NODES(NODE_MACRO)    \
    NODE_MACRO(Empty)       		 \
    NODE_MACRO(Assignment)  		 \
    NODE_MACRO(Block)       		 \
    NODE_MACRO(Declaration) 		 \
    NODE_MACRO(Else)        		 \
    NODE_MACRO(Expression)			 \
    NODE_MACRO(If)					 \
    NODE_MACRO(Literal)				 \
    NODE_MACRO(Program)				 \
    NODE_MACRO(Return)				 \
    NODE_MACRO(Symbol)				 \
    NODE_MACRO(Type)				 \
    NODE_MACRO(While)				 \
    NODE_MACRO(For)					 \
    NODE_MACRO(DoWhile)				 \
    NODE_MACRO(Struct)				 \
    NODE_MACRO(Qualifiers)		     \
    NODE_MACRO(Function)		     \
    NODE_MACRO(FunctionDecl)	     \
    NODE_MACRO(Pointer)	             \
    NODE_MACRO(Const)	             \
    NODE_MACRO(Typedef)	             \

class Visitor;

class ASTNode {

public:

#define ENUMDEFINE(name) name,
	enum NodeType {
		PERFORM_NODES(ENUMDEFINE)
	};

	// This guarantees a unique number for every node created,
	static int nodeCount;

    void addChild(std::shared_ptr<ASTNode> node);
    void addAsFirstChild(std::shared_ptr<ASTNode> node);
    void addSibling(std::shared_ptr<ASTNode> node);
	void addInstruction(std::shared_ptr<Instruction> instr);

    std::vector<std::shared_ptr<ASTNode>>& getSiblings();

	void printInstructions(std::ostream& out);

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

    std::vector<std::shared_ptr<ASTNode>> children;
    std::vector<std::shared_ptr<ASTNode>> siblings;
	std::vector<std::shared_ptr<Instruction>> instructions;
	size_t instructionSize;

    // non-owning
    ASTNode* parent;

	ASTNode(std::string str = "")
		: isConst(false), str(str) {
		nodeCount += 1;
		uniqueID = nodeCount;
		regCount = 0;
		instructionSize = 0;
        parent = nullptr;
        isPrefix = false;
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
    virtual std::shared_ptr<ASTNode> clone() = 0;
};

std::string to_string(const ASTNode* node);

bool operator==(const ASTNode& a, const ASTNode& b);

#define NODEDEFINE(name)                                    \
	class name ## Node : public ASTNode {                   \
		public:                                             \
		name ## Node(std::string str = "") : ASTNode(str){} \
		void accept(Visitor* visitor) override;             \
		std::string to_string() const override { 			\
			return #name;								    \
		}													\
		ASTNode::NodeType to_type() override {              \
			return ASTNode::NodeType::name;                 \
		}                                                   \
        std::shared_ptr<ASTNode> clone() override {         \
            auto node = std::make_shared<name ## Node>(str);\
            node->lineno = lineno;                          \
            node->isPrefix = isPrefix;                      \
            for(const auto& child : children){              \
                node->addChild(child->clone());             \
            }                                               \
            return node;                                    \
        }                                                   \
	};

PERFORM_NODES(NODEDEFINE)
