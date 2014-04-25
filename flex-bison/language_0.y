%{
  #include "ast.h"
  #include <iostream>
  #include <string>

  // From flex
  int yylex();

  void yyerror(const char*);
%}

// Debugging flags.
// Some of them need yydebug assigned to 1 before calling yyparse().
//   (They are VERY verbose!)
%debug
%error-verbose
%define parse.lac full

// This adds additonal arguments to yyparse. We can use it to return
//    the AST, but until then it just gets in the way.
//%parse-param {ASTNode& program}.

%token <integer>    integer
%token <real>       real
%token <str>        identifier
%token <str>        unaryPreOperatorKeyword
%token <str>        unaryPostOperatorKeyword
%token <str>        assignmentOperatorKeyword
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

%type <node>        Assignment
%type <node>        Block
%type <node>        Declaration
%type <node>        Declarations
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
%type <node>        Value
%type <node>        WhileStatement
%type <node>        DoWhileStatement
%type <node>        ForStatement
%type <node>        ForExpression
%type <node>        ForAssignment
%type <str>         assignmentOperator
%type <str>         unaryPreOperator

%code requires {
  #include <memory>
  #include "ast.h"
  using namespace std;

  extern ASTNode* root;

  // Structs, unlike unions, allow class members.
  struct YYSTYPE {
    int integer;
    double real;
    string str;
    ASTNode* node;

    // We should probably have a reasonable default constructor.
    YYSTYPE()
    : integer(), real() {}
  };
}

%%

Program:
  Statements {
    root = $$ = $1;
    $$->type = ASTNode::Program;
    $$->print_tree(cout);
  }
;

Statements:
  Statements Statement {
    $$->addChild($2);
  }
| Statement {
    $$ = new BlockNode();
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
| ';' {
    $$ = new ExpressionNode();
  }
;

Declarations:
  Type DeclList {
    $$ = new DeclarationsNode();
	$$->addChild($1);
	for(auto& child: $2->children){
		$$->addChild(child);
	}
  }
;

DeclList:
  Declaration {
    if($$->type != ASTNode::Declaration){
	  $$ = new DeclarationNode();
	} 

    $$->addChild($1);
  }
| DeclList ',' Declaration {
    $$->addChild($3);
  }
;

Declaration:
  Assignment {
    $$ = $1;
  }
| identifier {
    $$ = new SymbolNode($1);
  }
;

Type:
  constQualifier Type {
    $$ = $2;
    $$->makeConst();
  }
| identifier {
    $$ = new SymbolNode($1);
  }
;

Assignment:
  identifier '=' Expression {
    $$ = new AssignmentNode();
	$$->str = $1;  // this is only called for declarations and we need it for symbol table gen 
    $$->addChild(new SymbolNode($1));
    $$->addChild($3);
  }
;

Block:
  '{' Statements '}' {
    $$ = $2;
  }
  | '{' '}' {
    $$ = new BlockNode();
  }
;

// the assignment operators have the lowest precedence so they come first
Expression:

  // this rule is right recursive because it is right associative.
  TermPrecedence9 assignmentOperator Expression {
    $$ = new AssignmentNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence9 {
    $$ = $1;
  }
;

TermPrecedence9:
  TermPrecedence9 binaryOperatorKeyword9 TermPrecedence8 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence8 {
    $$ = $1;
  }
;

TermPrecedence8:
  TermPrecedence8 binaryOperatorKeyword8 TermPrecedence7 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence7 {
    $$ = $1;
  }
;

TermPrecedence7:
  TermPrecedence7 binaryOperatorKeyword7 TermPrecedence6 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence6 {
    $$ = $1;
  }
;

TermPrecedence6:
  TermPrecedence6 binaryOperatorKeyword6 TermPrecedence5 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence5 {
    $$ = $1;
  }
;

TermPrecedence5:
  TermPrecedence5 binaryOperatorKeyword5 TermPrecedence4 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence4 {
    $$ = $1;
  }
;

TermPrecedence4:
  TermPrecedence4 binaryOperatorKeyword4 TermPrecedence3 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence3 {
    $$ = $1;
  }
;

TermPrecedence3:
  TermPrecedence3 binaryOperatorKeyword3 TermPrecedence2 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence2 {
    $$ = $1;
  }
;

TermPrecedence2:
  TermPrecedence2 binaryOperatorKeyword2 TermPrecedence1 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence1 {
    $$ = $1;
  }
;

TermPrecedence1:
  TermPrecedence1 binaryOperatorKeyword1 TermPrecedence0 {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
    $$->addChild($3);
  }
| TermPrecedence0 {
    $$ = $1;
  }
;

TermPrecedence0:
  TermPrecedence0 binaryOperatorKeyword0 TermUnary {
    $$ = new ExpressionNode($2);
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
    $$ = new ExpressionNode($1);
    $$->addChild($2);
  }
;

TermPostUnary:
  PrimaryTerm unaryPostOperatorKeyword {
    $$ = new ExpressionNode($2);
    $$->addChild($1);
  }
;

PrimaryTerm:
  '(' Expression ')' {
    $$ = $2;
  }
| Value {
    $$ = $1;
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
| '*' {
    $$ = string("*");
  }
;

// required because '=' needs special significance in assignment and therefore can't be apart of the assignmentOperatorKeyword regex
assignmentOperator:
  '=' {
    $$ = string("=");
  }
| assignmentOperatorKeyword {
    $$ = $1;
  }

Value:
  identifier {
    $$ = new SymbolNode($1);
  }
| Literal {
    $$ = $1;
  }
;

Literal:
// TODO: const char* and char literals. (Eventually)
  integer {
    $$ = new LiteralNode(to_string($1));
  }
| real {
    $$ = new LiteralNode(to_string($1));
  }
;

IfStatement:
  ifKeyword '(' Expression ')' Block {
    $$ = new IfNode();
    $$->addChild($3);
    $$->addChild($5);
  }
| ifKeyword '(' Expression ')' Block ElseStatement  {
    $$ = new IfNode();
    $$->addChild($3);
    $$->addChild($5);
    $$->addChild($6);
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
    $$ = new WhileNode();
    $$->addChild($3);
    $$->addChild($5);
  }
;

DoWhileStatement:
  doKeyword Block whileKeyword '(' Expression ')' ';' {
    $$ = new DoWhileNode();
    $$->addChild($2);
    $$->addChild($5);
  }
;

ForStatement:
  forKeyword '(' ForAssignment ';' ForExpression ';' ForExpression ')' Block {
    $$ = new ForNode();
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
    $$ = new EmptyNode();
  }
;

ReturnStatement:
  returnKeyword Expression ';' {
    $$ = new ReturnNode();
    $$->addChild($2);
  }
;

%%

void yyerror(const char* msg) {
	cerr << "[Error] " << msg << endl;
  }
