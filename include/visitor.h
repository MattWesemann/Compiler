#pragma once

class ASTNode;

class Visitor {
public:
	void visit(ASTNode* node);
};