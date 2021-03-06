%{
  #include "ast.h"
  #include "Compiler.h"
  #include <string>
  
  // From flex
  int yylex();
  int yyget_lineno();

  void yyerror(Compiler* compiler, const char*);
%}

// Debugging flags.
// Some of them need yydebug assigned to 1 before calling yyparse().
//   (They are VERY verbose!)
%debug
%error-verbose
%define parse.lac full

// This adds additonal arguments to yyparse.
%parse-param {Compiler* compiler}

%token <integer>    integer
%token <real>       real
%token <str>        identifier
%token <str>        unaryPreOperatorKeyword
%token <str>        unaryPostOperatorKeyword
%token <str>        assignmentOperatorKeyword
%token <str>        starOperatorKeyword
%token <str>        binaryOperatorKeyword0
%token <str>        binaryOperatorKeyword1
%token <str>        binaryOperatorKeyword2
%token <str>        binaryOperatorKeyword3
%token <str>        binaryOperatorKeyword4
%token <str>        binaryOperatorKeyword5
%token <str>        binaryOperatorKeyword6
%token <str>        binaryOperatorKeyword7
%token <str>        binaryOperatorKeyword8
%token <str>        binaryOperatorKeyword9

// Reserved words.
%token <str>        constQualifier
%token <str>        elseKeyword
%token <str>        ifKeyword
%token <str>        returnKeyword
%token <str>        whileKeyword
%token <str>        forKeyword
%token <str>        doKeyword
%token <str>        structKeyword
%token <str>        typedefKeyword

%type <node>        Assignment
%type <node>        Block
%type <node>        Declaration
%type <node>        PointerDeclaration
%type <node>        Declarations
%type <node>        DeclarationsList
%type <node>        DeclList
%type <node>        ElseStatement
%type <node>        Expression
%type <node>        PrimaryTerm 
%type <node>        TermUnary
%type <node>        TermPreUnary
%type <node>        TermPostUnary
%type <node>        TermPrecedence0
%type <node>        TermPrecedence1
%type <node>        TermPrecedence2
%type <node>        TermPrecedence3
%type <node>        TermPrecedence4
%type <node>        TermPrecedence5
%type <node>        TermPrecedence6
%type <node>        TermPrecedence7
%type <node>        TermPrecedence8
%type <node>        TermPrecedence9
%type <node>        IfStatement
%type <node>        Literal
%type <node>        Program
%type <node>        ReturnStatement
%type <node>        Statement
%type <node>        Statements
%type <node>        Type
%type <node>        PointerType
%type <node>        Qualifiers
%type <node>        Value
%type <node>        WhileStatement
%type <node>        DoWhileStatement
%type <node>        ForStatement
%type <node>        StructDefinition
%type <node>        Typedef
%type <node>        Function
%type <node>        FunctionDecl
%type <node>        FunctionImpl
%type <node>        ForExpression
%type <node>        ForAssignment
%type <node>        ArgList
%type <node>        ArgListInternal
%type <str>         assignmentOperator
%type <str>         unaryPreOperator

%code requires {
  #include <memory>
  #include "ast.h"
  #include "Compiler.h"

  // Structs, unlike unions, allow class members.
  struct YYSTYPE {
    int integer;
    double real;
    std::string str;
    std::shared_ptr<ASTNode> node;

    // We should probably have a reasonable default constructor.
    YYSTYPE()
    : integer(), real() {}
  };
}

%%

Program:
  Statements {
    compiler->root = $$ = std::make_shared<ProgramNode>();
    compiler->root->lineno = $1->lineno;
	// slightly hackish but lets steal their children
    compiler->root->children = std::move($1->children);
  }
;

Statements:
  Statements Statement {
    $$->addChild($2);
  }
| Statement {
    $$ = std::make_shared<BlockNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($1);
  }
;

Statement:
  Declarations ';' {
    $$ = $1;
  }
| Expression ';' {
    $$ = $1;
  }
| Block {
    $$ = $1;
  }
| IfStatement {
    $$ = $1;
  }
| WhileStatement {
    $$ = $1;
  }
| DoWhileStatement {
    $$ = $1;
  }
| ForStatement {
    $$ = $1;
  }
| ReturnStatement {
    $$ = $1;
  }
| StructDefinition {
    $$ = $1;
  }
| FunctionDecl {
    $$ = $1;
  }
| FunctionImpl {
    $$ = $1;
  }
| Typedef {
    $$ = $1;
  }
| ';' {
    $$ = std::make_shared<ExpressionNode>();
    $$->lineno = yyget_lineno();
  }
;

Typedef:
  typedefKeyword Declarations ';' {
    $$ = $2;
  }
| typedefKeyword StructDefinition {
    $$ = $2;
  }
;

StructDefinition:
  structKeyword '{' DeclarationsList '}' DeclList ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	$$ = $5;
	$$->addChild($3);
  }
| structKeyword identifier '{' DeclarationsList '}' DeclList ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	$$ = $6;
	$$->addChild($4);
  }
| structKeyword '{' DeclarationsList '}' ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	//$$ = $5;
	//$$->addChild($3);
  }
| structKeyword identifier '{' DeclarationsList '}' ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	//$$ = $6;
	//$$->addChild($4);
  }
| structKeyword '{' '}' DeclList ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	//$$ = $5;
	//$$->addChild($3);
  }
| structKeyword identifier '{' '}' DeclList ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	//$$ = $6;
	//$$->addChild($4);
  }
| structKeyword '{' '}' ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	//$$ = $5;
	//$$->addChild($3);
  }
| structKeyword identifier '{' '}' ';' {
    $$ = std::make_shared<StructNode>();
    $$->lineno = yyget_lineno();
	//$$ = $6;
	//$$->addChild($4);
  }
;

DeclarationsList:
  DeclarationsList Declarations ';' {
    $$ = $1;
	$$->addSibling($2);
  }
| Declarations ';' {
    $$ = $1;
  }
;

Declarations:
  Type DeclList {
    $$ = std::make_shared<DeclarationNode>();
    $$->lineno = yyget_lineno();
	$$->addAsFirstChild($1->clone());
	for(const auto& child : $2->getSiblings()){
		auto temp = std::make_shared<DeclarationNode>();
		temp->lineno = yyget_lineno();
	  	temp->addAsFirstChild($1->clone());
		temp->addChild(child);
		$$->addSibling(temp);
	}
	$2->siblings.clear();
	$$->addChild($2);
  }
;

DeclList:
  PointerDeclaration {
    $$ = $1;
  }
| DeclList ',' PointerDeclaration {
    $$ = $1;
	$$->addSibling($3);
  }
;

PointerDeclaration:
  PointerType Declaration {
    $$ = $2;
	$$->addChild($1);
  }
| Declaration {
    $$ = $1;
  }
;

Declaration:
  Assignment {
    $$ = $1;
  }
| identifier {
    $$ = std::make_shared<SymbolNode>($1);
    $$->lineno = yyget_lineno();
  }
| identifier '[' integer ']' {
    $$ = std::make_shared<SymbolNode>($1);
    $$->lineno = yyget_lineno();
  }
;

Type:
  Qualifiers identifier {
    $$ = std::make_shared<TypeNode>($2);
    $$->lineno = yyget_lineno();
	$$->addChild($1);
  }
| Qualifiers structKeyword identifier {
    $$ = std::make_shared<TypeNode>($3);
    $$->lineno = yyget_lineno();
	$$->addChild($1);
  }
| identifier {
    $$ = std::make_shared<TypeNode>($1);
    $$->lineno = yyget_lineno();
  }
| structKeyword identifier {
    $$ = std::make_shared<TypeNode>($2);
    $$->lineno = yyget_lineno();
  }
;

PointerType:
  starOperatorKeyword {
    $$ = std::make_shared<PointerNode>();
    $$->lineno = yyget_lineno();
  }
| PointerType starOperatorKeyword {
    auto temp = std::make_shared<PointerNode>();
    temp->lineno = yyget_lineno(); 
	$$ = $1;
    $$->addSibling(temp);
  }
| PointerType Qualifiers {
	$$ = $1;
    $$->addSibling($2);
  }
| Qualifiers {
    $$ = $1;
  }
;

Qualifiers:
  constQualifier {
    $$ = std::make_shared<ConstNode>();
    $$->lineno = yyget_lineno();
  }
;

Assignment:
  identifier '=' Expression {
    $$ = std::make_shared<AssignmentNode>();
    $$->lineno = yyget_lineno();
    $$->addChild(std::make_shared<SymbolNode>($1));
    $$->addChild($3);
  }
;

Block:
  '{' Statements '}' {
    $$ = $2;
  }
| '{' '}' {
    $$ = std::make_shared<BlockNode>();
    $$->lineno = yyget_lineno();
  }
;

// the assignment operators have the lowest precedence so they come first
Expression:

  // this rule is right recursive because it is right associative.
  TermPrecedence9 assignmentOperator Expression {
    $$ = std::make_shared<AssignmentNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence9 {
    $$ = $1;
  }
;

TermPrecedence9:
  TermPrecedence9 binaryOperatorKeyword9 TermPrecedence8 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence8 {
    $$ = $1;
  }
;

TermPrecedence8:
  TermPrecedence8 binaryOperatorKeyword8 TermPrecedence7 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence7 {
    $$ = $1;
  }
;

TermPrecedence7:
  TermPrecedence7 binaryOperatorKeyword7 TermPrecedence6 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence6 {
    $$ = $1;
  }
;

TermPrecedence6:
  TermPrecedence6 binaryOperatorKeyword6 TermPrecedence5 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence5 {
    $$ = $1;
  }
;

TermPrecedence5:
  TermPrecedence5 binaryOperatorKeyword5 TermPrecedence4 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence4 {
    $$ = $1;
  }
;

TermPrecedence4:
  TermPrecedence4 binaryOperatorKeyword4 TermPrecedence3 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence3 {
    $$ = $1;
  }
;

TermPrecedence3:
  TermPrecedence3 binaryOperatorKeyword3 TermPrecedence2 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence2 {
    $$ = $1;
  }
;

TermPrecedence2:
  TermPrecedence2 binaryOperatorKeyword2 TermPrecedence1 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence1 {
    $$ = $1;
  }
;

TermPrecedence1:
  TermPrecedence1 binaryOperatorKeyword1 TermPrecedence0 {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence0 {
    $$ = $1;
  }
;

TermPrecedence0:
  TermPrecedence0 binaryOperatorKeyword0 TermUnary {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->addChild($1);
    $$->addChild($3);
  }
| TermUnary {
    $$ = $1;
  }
;

TermUnary:
  TermPreUnary {
    $$ = $1;
  }
| TermPostUnary {
    $$ = $1;
  }
| PrimaryTerm {
    $$ = $1;
  }
;

TermPreUnary:
  unaryPreOperator PrimaryTerm {
    $$ = std::make_shared<ExpressionNode>($1);
    $$->lineno = yyget_lineno();
    $$->isPrefix = true;
    $$->addChild($2);
  }
;

TermPostUnary:
  PrimaryTerm unaryPostOperatorKeyword {
    $$ = std::make_shared<ExpressionNode>($2);
    $$->lineno = yyget_lineno();
    $$->isPrefix = false;
    $$->addChild($1);
  }
| PrimaryTerm '[' Expression ']' {
    $$ = std::make_shared<ExpressionNode>();
    $$->lineno = yyget_lineno();
  }
| PrimaryTerm '-' '>' identifier {
    $$ = std::make_shared<ExpressionNode>();
    $$->lineno = yyget_lineno(); 
  }
| PrimaryTerm '.' identifier {
    $$ = std::make_shared<ExpressionNode>();
    $$->lineno = yyget_lineno();  
  }
;

PrimaryTerm:
  '(' Expression ')' {
    $$ = $2;
  }
| Value {
    $$ = $1;
  }
| Function {
    $$ = $1;
  }
;

ArgList:
  ArgListInternal {
    $$ = $1;
  }
| ArgListInternal ',' '.' '.' '.' {
    $$ = $1;
    auto temp = std::make_shared<SymbolNode>("...");
    temp->lineno = yyget_lineno();  
	$$->addSibling(temp);
  }
| '.' '.' '.' {
    $$ = std::make_shared<SymbolNode>("...");
    $$->lineno = yyget_lineno();  
  }
;

ArgListInternal:
  Type PointerDeclaration {
    $$ = $2;
    $$->addAsFirstChild($1);
  }
| Type {
    $$ = $1;
  }
| Type PointerType {
    $$ = $1;
	$1->addChild($2);
  }
| ArgListInternal ',' Type PointerDeclaration {
    $$ = $1;
    $$->addSibling($3);
	$3->addChild($4);
  }
| ArgListInternal ',' Type {
    $$ = $1;
    $$->addSibling($3);
  }
| ArgListInternal ',' Type PointerType {
    $$ = $1;
    $$->addSibling($3);
	$3->addChild($4);
  }
; 

Function:
  identifier '(' ')' {
    $$ = std::make_shared<FunctionNode>($1);
    $$->lineno = yyget_lineno();
  }
| identifier '(' ArgList ')' {
    $$ = std::make_shared<FunctionNode>($1);
    $$->lineno = yyget_lineno();
	$$->addChild($3);
  }
;

// we can't match all in one regex so we must do it this way
unaryPreOperator: 
  unaryPreOperatorKeyword {
    $$ = $1;
  }
| unaryPostOperatorKeyword {
    $$ = $1;
  }
| binaryOperatorKeyword1 {
    $$ = $1;
  }
| binaryOperatorKeyword5 {
    $$ = $1;
  }
| starOperatorKeyword {
    $$ = $1;
  }
;

// required because '=' needs special significance in assignment and therefore can't be apart of the assignmentOperatorKeyword regex
assignmentOperator:
  '=' {
    $$ = std::string("=");
  }
| assignmentOperatorKeyword {
    $$ = $1;
  }

Value:
  identifier {
    $$ = std::make_shared<SymbolNode>($1);
    $$->lineno = yyget_lineno();
  }
| Literal {
    $$ = $1;
  }
;

Literal:
// TODO: const char* and char literals. (Eventually)
  integer {
    $$ = std::make_shared<LiteralNode>(std::to_string($1));
    $$->lineno = yyget_lineno();
  }
| real {
    $$ = std::make_shared<LiteralNode>(std::to_string($1));
    $$->lineno = yyget_lineno();
  }
;

IfStatement:
  ifKeyword '(' Expression ')' Block {
    $$ = std::make_shared<IfNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($3);
    $$->addChild($5);
    $$->addChild(std::make_shared<EmptyNode>());
  }
| ifKeyword '(' Expression ')' Block ElseStatement  {
    $$ = std::make_shared<IfNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($3);
    $$->addChild($5);
    $$->addChild($6);
    $$->addChild(std::make_shared<EmptyNode>());
  }
;

ElseStatement:
  elseKeyword Block {
    $$ = $2;
  }
| elseKeyword IfStatement {
    $$ = $2;
  }
;

WhileStatement:
  whileKeyword '(' Expression ')' Block {
    $$ = std::make_shared<WhileNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($3);
    $$->addChild($5);
    $$->addChild(std::make_shared<EmptyNode>());
  }
;

DoWhileStatement:
  doKeyword Block whileKeyword '(' Expression ')' ';' {
    $$ = std::make_shared<DoWhileNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($2);
    $$->addChild($5);
  }
;

ForStatement:
  forKeyword '(' ForAssignment ';' ForExpression ';' ForExpression ')' Block {
    $$ = std::make_shared<ForNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($3);
    $$->addChild($5);
    $$->addChild($7);
    $$->addChild($9);
  }
;

ForAssignment:
  Declarations {
    $$ = $1;
  }
| ForExpression {
    $$ = $1;
  }
;

// can be an expression or lambda
ForExpression:
  Expression {
    $$ = $1;
  }
| {
    $$ = std::make_shared<EmptyNode>();
    $$->lineno = yyget_lineno();
  }
;

ReturnStatement:
  returnKeyword Expression ';' {
    $$ = std::make_shared<ReturnNode>();
    $$->lineno = yyget_lineno();
    $$->addChild($2);
  }
| returnKeyword ';' {
    $$ = std::make_shared<ReturnNode>();
    $$->lineno = yyget_lineno();
  }
;

FunctionDecl:
  Type Function ';' {
    $$ = std::make_shared<FunctionDeclNode>();
    $$->lineno = yyget_lineno();
	$$->addChild($1);
	$$->addChild($2);
  }
;

FunctionImpl:
  Type Function '{' Statements '}' {
    $$ = std::make_shared<FunctionNode>();
    $$->lineno = yyget_lineno();
	$$->addChild($1);
	$$->addChild($2);
	$$->addChild($4);
  }
| Type Function '{' '}' {
    $$ = std::make_shared<FunctionNode>();
    $$->lineno = yyget_lineno();
	$$->addChild($1);
	$$->addChild($2);
  }
;

%%

void yyerror(Compiler* compiler, const char* msg) {
    compiler->handleError(msg, yyget_lineno());
}
