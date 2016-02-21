#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "ast.h"

class Visitor {
public:
	Visitor();

    static void visit(Visitor* pVisitor, std::shared_ptr<ASTNode> node);

	void visit(std::shared_ptr<ASTNode> node);
	virtual void visit(ASTNode* node);

	// detect errors and optionally print them out
	virtual bool hadError(std::ostream* cerrFile);

#define visitFn(name) virtual void visit(name ## Node *node);
	PERFORM_NODES(visitFn)

protected:
	std::vector<std::string> errors;
};