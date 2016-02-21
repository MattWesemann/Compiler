#pragma once

#ifdef WIN32
#ifdef EXPORTING
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif
#define COMPILIERAPI __stdcall 
#else
#define DLLIMPORT
#define COMPILIERAPI
#endif

#define PERFORM_ERRORS(ERROR_MACRO)   \
    ERROR_MACRO(ErrorNone)            \
    ERROR_MACRO(ErrorUnknown)  		  \
    ERROR_MACRO(ErrorSyntax)          \
    ERROR_MACRO(ErrorMemory) 		  \
    ERROR_MACRO(ErrorFileDNE)         \


#define ENUMDEFINE(name) name,
enum CompilerErrors {
    PERFORM_ERRORS(ENUMDEFINE)
};

class ICompiler {
public:
    virtual void COMPILIERAPI Release() = 0;
    virtual CompilerErrors COMPILIERAPI Compile(const char* filename) = 0;
    virtual CompilerErrors COMPILIERAPI Compile(char* buffer, unsigned int size) = 0;
    virtual const char* COMPILIERAPI GetError() = 0;
    virtual const wchar_t* COMPILIERAPI GetErrorW() = 0;

    void operator delete(void* p) {
        if (p) {
            ICompiler* ptr = static_cast<ICompiler*>(p);
            ptr->Release();
        }
    }

protected:
    virtual ~ICompiler(){}
};

// This is the public interface into the compiler
extern "C" ICompiler* COMPILIERAPI GetCompiler();
extern "C" const char* COMPILIERAPI GetErrorMsg(CompilerErrors error);
extern "C" const wchar_t* COMPILIERAPI GetErrorMsgW(CompilerErrors error);