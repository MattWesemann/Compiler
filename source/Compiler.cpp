#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Compiler.h"
#include "utils.h"
#include "language_0_parser.hpp"
#include <vector>
#include <fstream>

using namespace std;

struct yy_buffer_state;
typedef yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_buffer(char*, size_t);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

ICompiler* COMPILIERAPI GetCompiler(){
    return new Compiler();
}

#define CASEDEFINE(name) case name: return #name;
const char* COMPILIERAPI GetErrorMsg(CompilerErrors error){
    switch (error){
        PERFORM_ERRORS(CASEDEFINE)
    }
    return GetErrorMsg(ErrorUnknown);
}

#define WIDEN(x)  L ## #x
#define CASEDEFINEW(name) case name: return WIDEN(name);
const wchar_t* COMPILIERAPI GetErrorMsgW(CompilerErrors error){
    switch (error){
        PERFORM_ERRORS(CASEDEFINEW)
    }
    return GetErrorMsgW(ErrorUnknown);
}

void COMPILIERAPI Compiler::Release(){
    delete this;
}

CompilerErrors COMPILIERAPI Compiler::Compile(const char* filename){
    CompilerErrors ret = CompilerErrors::ErrorNone;

    ifstream file(filename, ios::binary | ios::ate);

    if (file.fail())
        return ErrorFileDNE;

    std::vector<char> bytes;
    ReadAllBytes(file, bytes);

    file.close();

    ret = Compile(bytes.data(), bytes.size());

    return ret;
}

CompilerErrors COMPILIERAPI Compiler::Compile(char* buffer, unsigned int size)
{
    CompilerErrors ret = CompilerErrors::ErrorNone;
    int result = 0;
    auto bufferstate = yy_scan_buffer(buffer, size);

    result = yyparse(this);

    yy_delete_buffer(bufferstate);

    switch (result) {
    case 0:
        ret = ErrorNone;
        break;
    case 1:
        ret = ErrorSyntax;
        break;
    case 2:
        ret = ErrorMemory;
        break;
    default:
        ret = ErrorUnknown;
        break;
    }

    return ret;
}

const char* COMPILIERAPI Compiler::GetError(){
    return errorMsg.c_str();
}

const wchar_t* COMPILIERAPI Compiler::GetErrorW(){
    int wlen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, errorMsg.c_str(), -1, nullptr, 0);
    if (wlen == 0)
        return nullptr;  // bad string

    errorMsgW.resize(wlen);
    wlen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, errorMsg.c_str(), -1, const_cast<wchar_t*>(errorMsgW.data()), wlen);
    return errorMsgW.c_str();
}

void Compiler::handleError(const char* msg, int lineno){
    errorMsg = "[Error] " + string(msg) + " on line " + to_string(lineno);
}