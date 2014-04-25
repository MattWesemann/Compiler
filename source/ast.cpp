#include "ast.h"

#include <functional>
#include <sstream>
#include <unordered_map>

using namespace std;

int ASTNode::nodeCount = 0;

void ASTNode::addChild(ASTNode* node) {
	children.push_back(node);
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
	// TODO: Stop copying over 200 nodes before any of the final ones,
	//   which appear in the graph, are created.
	// str += "#" + to_string(node.uniqueID) + " ";
	if (node->to_string() != "Symbol") {
	//if (node->type != ASTNode::Symbol) {
		str += node->to_string();
	}
	if (node->str.length() >= 1) {
		if (node->isConst) {
			str += " const";
		}
		str += " '" + node->str + "'";
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