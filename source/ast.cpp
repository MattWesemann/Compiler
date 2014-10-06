#include "ast.h"
#include "visitor.h"

#include <functional>
#include <sstream>
#include <unordered_map>

using namespace std;

int ASTNode::nodeCount = 0;

void ASTNode::addChild(shared_ptr<ASTNode> node) {
    node->parent = this;
	children.push_back(node);
    for (auto child : node->siblings)
        children.push_back(child);
    node->siblings.clear();
}

void ASTNode::addAsFirstChild(shared_ptr<ASTNode> node){
    node->parent = this;
    // add in reverse order since we want to preserve it at the beginning
    for (size_t i = node->siblings.size() - 1; i < node->siblings.size(); --i){
        children.insert(children.begin(), node->siblings[i]);
    }
    children.insert(children.begin(), node);
    node->siblings.clear();
}

void ASTNode::addSibling(shared_ptr<ASTNode> node) {
    if (parent){
        parent->addChild(node);
    }
    else {
        siblings.push_back(node);
    }
}

void ASTNode::addInstruction(shared_ptr<Instruction> instr){
	instructions.push_back(instr);
	instructionSize+=1;
}

vector<shared_ptr<ASTNode>>& ASTNode::getSiblings(){
    if (parent)
        return parent->children;
    return siblings;
}

void ASTNode::printInstructions(ostream& out){
	for (auto instruction : instructions)
		instruction->print(out);
}

void ASTNode::find_all_children(vector<ASTNode*>& nodes) const {
	nodes.push_back((ASTNode*) this);
	for (auto child : children) {
		child->find_all_children(nodes);
	}
}

void ASTNode::print_tree(ostream& os) {
	vector<ASTNode*> allChildren;
	find_all_children(allChildren);

	// We print the uniqueID of each node and its name first and foremost.
	for (auto node : allChildren) {
		os << node->uniqueID << " " << ::to_string(node) << endl;
	}
	// Separate the 'declarations' and 'definitions'.
	os << "\n";

	// Print the uniqueID of each node and all of its children.
	for (auto node : allChildren) {
		if (node->children.size() < 1) {
			// Skip over leaves, as they have no children.
			continue;
		}
		os << node->uniqueID << " ";
		for (auto child : node->children) {
			os << child->uniqueID << " ";
		}
		os << endl;
	}

}

void ASTNode::makeConst() {
	isConst = true;
}

string to_string(const ASTNode* node) {
	string str;
	if (node->str.length()) {
		if (node->isConst) {
			str += " const";
		}
		str += " '" + node->str + "'";
	} else {
		str += node->to_string();
	}

	if (str.length() == 0) {
		static int errorCount = 1;
		str = "Error #" + to_string(errorCount);
		errorCount += 1;
	}
	return str;
}

bool operator==(const ASTNode& a, const ASTNode& b) {
	return a.uniqueID == b.uniqueID;
}

#define NODEIMPL(name)                             \
	void name ## Node ::accept(Visitor* visitor) { \
		visitor->visit(this);                      \
	}                                              \

PERFORM_NODES(NODEIMPL)
