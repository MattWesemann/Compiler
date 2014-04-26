#include "registerVisitor.h"
#include <cmath>
#include "ast.h"

RegisterVisitor::RegisterVisitor(){

}

// sethi ullman numbering
void RegisterVisitor::visit(ExpressionNode* node) {
	//left child
	numberNode(node->children[0]);
	//right child if it exists
	if (node->children.size() > 1) {
		numberNode(node->children[1]); 
	}
}

void RegisterVisitor::numberNode(ASTNode* node) {
	//number leaf nodes with 1
	if(node->children.size() == 0) {
		node->regCount = 1;
	}
	
	else {
		// number all the nodes below this one
		numberNode(node->children[0]);
		if (node->children.size() > 1) {
			numberNode(node->children[1]); 
		}
		
		int left = node->children[0]->regCount;
		int right = node->children[1]->regCount;

		if (left == right) {
			node->regCount = right + 1;
		}
		else {
			node->regCount = fmax(left, right );
		}
	}

}