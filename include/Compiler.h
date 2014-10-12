#pragma once
#include "ICompiler.h"
#include "ast.h"
#include <memory>
#include <iostream>
#include <string>

class Compiler : public ICompiler {
public:
    virtual void COMPILIERAPI Release() override;
    virtual CompilerErrors COMPILIERAPI Compile(const char* filename) override;
    virtual CompilerErrors COMPILIERAPI Compile(char* buffer, unsigned int size) override;
    virtual const char* COMPILIERAPI GetError() override;
    virtual const wchar_t* COMPILIERAPI GetErrorW() override;

    void handleError(const char* msg, int lineno);

    void operator delete(void* p) {
        ::operator delete (p);
    }

//private:
    std::shared_ptr<ASTNode> root;
    std::string errorMsg;
    std::wstring errorMsgW;
};