#include "visitor.h"
#include "scope.h"
#include "ast.h"
#include <memory>

class SymbolVisitor : public Visitor {
public:
	SymbolVisitor();

	void visit(BlockNode* node);
	void visit(DeclarationsNode* node);
	void visit(SymbolNode* node);

private:

	std::shared_ptr<Scope> global;
	std::shared_ptr<Scope> current;
};