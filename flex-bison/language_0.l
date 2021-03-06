%option noyywrap
%option yylineno
%{
	#include "language_0_parser.hpp"
%}

StarOp          "*"
PreUnaryOp      "!"|"~"
PostUnaryOp     "++"|"--"
BinaryOp0       "/"|"%"
BinaryOp1       "+"
BinaryOp2       ">>"|"<<"
BinaryOp3       "<"|"<="|">="
BinaryOp4       "=="|"!="
BinaryOp5       "&"
BinaryOp6       "^"
BinaryOp7       "|"
BinaryOp8       "&&"
BinaryOp9       "||"

AssignmentOp    "+="|"-="|"*="|"/="|"%="|"|="|"&="|"^="|"<<="|">>="

%%

\/\/[^\n]* {
  //eol comments
  continue;
}

[ \n\t\r]+ {
	// Eat whitespace.
	continue;
}

"if" {
	return ifKeyword;
}
"else" {
	return elseKeyword;
}
"while" {
	return whileKeyword;
}
"return" {
	return returnKeyword;
}
"const" {
	return constQualifier;
}
"for" {
	return forKeyword;
}
"do" {
	return doKeyword;
}
"struct" {
	return structKeyword;
}
"typedef" {
	return typedefKeyword;
}

[0-9]+ {
	yylval.integer = atoi(yytext);
	return integer;
}

[0-9]+\.?[0-9]*([eE][+-]?[0-9]+)? {
	yylval.real = atof(yytext);
	return real;
}

[a-zA-Z_]+[a-zA-Z_0-9]* {
	yylval.str = yytext;
	return identifier;
}

{StarOp} {
	yylval.str = yytext;
	return starOperatorKeyword;
}

{PreUnaryOp} {
	yylval.str = yytext;
	return unaryPreOperatorKeyword;
}

{PostUnaryOp} {
	yylval.str = yytext;
	return unaryPostOperatorKeyword;
}

{BinaryOp0}|{StarOp} {
	yylval.str = yytext;
	return binaryOperatorKeyword0;
}

{BinaryOp1}|'-' {
	yylval.str = yytext;
	return binaryOperatorKeyword1;
}

{BinaryOp2} {
	yylval.str = yytext;
	return binaryOperatorKeyword2;
}

{BinaryOp3}|'>' {
	yylval.str = yytext;
	return binaryOperatorKeyword3;
}

{BinaryOp4} {
	yylval.str = yytext;
	return binaryOperatorKeyword4;
}

{BinaryOp5} {
	yylval.str = yytext;
	return binaryOperatorKeyword5;
}

{BinaryOp6} {
	yylval.str = yytext;
	return binaryOperatorKeyword6;
}

{BinaryOp7} {
	yylval.str = yytext;
	return binaryOperatorKeyword7;
}

{BinaryOp8} {
	yylval.str = yytext;
	return binaryOperatorKeyword8;
}

{BinaryOp9} {
	yylval.str = yytext;
	return binaryOperatorKeyword9;
}

{AssignmentOp} {
	yylval.str = yytext;
	return assignmentOperatorKeyword;
}

. {
	// Pass unknown characters through to Bison, they might be important.
	return yytext[0];
}

%%
