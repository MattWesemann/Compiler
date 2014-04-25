#include "visitor.h"
#include "scope.h"
#include "ast.h"
#include <memory>

class SymbolVisitor : public Visitor {
public:
	SymbolVisitor();

	void visit(BlockNode* node) override;
	void visit(DeclarationsNode* node) override;
	void visit(SymbolNode* node) override;

private:

	std::shared_ptr<Scope> global;
	std::shared_ptr<Scope> current;
};