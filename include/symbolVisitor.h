#include "visitor.h"
#include "scope.h"
#include "ast.h"
#include <memory>

class SymbolVisitor : public Visitor {
public:
	SymbolVisitor();
	void visit(EmptyNode* node);
	void visit(AssignmentNode* node);
	void visit(BlockNode* node);
	void visit(DeclarationsNode* node);
	void visit(IfNode* node);
	void visit(ElseNode* node);
	void visit(ExpressionNode* node);
	void visit(LiteralNode* node);
	void visit(OperatorNode* node);
	void visit(ProgramNode* node);
	void visit(ReturnNode* node);
	void visit(SymbolNode* node);
	void visit(TypeNode* node);
	void visit(WhileNode* node);
	void visit(ForNode* node);
	void visit(DoWhileNode* node);

private:
	std::shared_ptr<Scope> global;
	std::shared_ptr<Scope> current;
};
