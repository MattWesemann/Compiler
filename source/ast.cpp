#include "ast.h"
#include "visitor.h"

#include <functional>
#include <sstream>
#include <unordered_map>

using namespace std;

int ASTNode::nodeCount = 0;

void ASTNode::addChild(std::shared_ptr<ASTNode> node) {
	children.push_back(node);
}

void ASTNode::addChildFront(shared_ptr<ASTNode> node){
	children.insert(children.begin(), node);
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

#define NODEIMPL(name)                                  \
	void name ## Node ::accept(Visitor* visitor) {      \
		visitor->visit(this);                           \
	}                                                   \

PERFORM_NODES(NODEIMPL)