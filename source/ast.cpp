#include "ast.h"

#include <functional>
#include <sstream>
#include <unordered_map>

using namespace std;

int ASTNode::nodeCount = 0;

void ASTNode::addChild(const ASTNode& node) {
	children.push_back(node);
}

void ASTNode::find_all_children(vector<ASTNode>& nodes) const {
	nodes.push_back(*this);
	for (const ASTNode& child : children) {
		child.find_all_children(nodes);
	}
}

void ASTNode::print_tree(ostream& os) {
	vector<ASTNode> allChildren;
	find_all_children(allChildren);

	// We print the uniqueID of each node and its name first and foremost.
	for (const ASTNode& node : allChildren) {
		os << node.uniqueID << " " << to_string(node) << endl;
	}
	// Separate the 'declarations' and 'definitions'.
	os << "\n";

	// Print the uniqueID of each node and all of its children.
	for (const ASTNode& node : allChildren) {
		if (node.children.size() < 1) {
			// Skip over leaves, as they have no children.
			continue;
		}
		os << node.uniqueID << " ";
		for (const ASTNode& child : node.children) {
			os << child.uniqueID << " ";
		}
		os << endl;
	}

}

void ASTNode::makeConst() {
	isConst = true;
}

string to_string(const ASTNode& node) {
	string str;
	// TODO: Stop copying over 200 nodes before any of the final ones,
	//   which appear in the graph, are created.
	// str += "#" + to_string(node.uniqueID) + " ";
	if (node.type != ASTNode::Symbol) {
		str += to_string(node.type);
	}
	if (node.str.length() >= 1) {
		if (node.isConst) {
			str += " const";
		}
		str += " '" + node.str + "'";
	}
	if (str.length() == 0) {
		static int errorCount = 1;
		str = "Error #" + to_string(errorCount);
		errorCount += 1;
	}
	return str;
}

string to_string(ASTNode::Type type) {
	switch (type) {
	// Make sure every case is handled, when changing the enum declaration
	//   in ast.h.
	case ASTNode::Empty:        return "Empty";
	case ASTNode::Assignment:   return "Assignment";
	case ASTNode::Block:        return "Block";
	case ASTNode::Declaration:  return "Declaration";
	case ASTNode::Declarations: return "Declarations";
	case ASTNode::Else:         return "Else";
	case ASTNode::Expression:   return "Expression";
	case ASTNode::If:           return "If";
	case ASTNode::Literal:      return "Literal";
	case ASTNode::Operator:     return "Operator";
	case ASTNode::Program:      return "Program";
	case ASTNode::Return:       return "Return";
	case ASTNode::Symbol:       return "Symbol";
	case ASTNode::While:        return "While";
	case ASTNode::For:          return "For";
	case ASTNode::DoWhile:      return "DoWhile";
	default:
		return "? (" + to_string(type) + ")";
	}
}

bool operator==(const ASTNode& a, const ASTNode& b) {
	return a.uniqueID == b.uniqueID;
}
