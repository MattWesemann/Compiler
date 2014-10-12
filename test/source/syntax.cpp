#include "CppUnitTest.h"
#include "Compiler.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<CompilerErrors>(const CompilerErrors& t) { return GetErrorMsgW(t); }

namespace CompilerTests
{		
    TEST_CLASS(SyntaxTests)
	{
	public:
		TEST_METHOD(DeclarationTest)
		{
            Compiler compiler;
            auto ret = compiler.Compile("..\\..\\test\\resources\\syntax\\declarations.txt");
            Assert::AreEqual(CompilerErrors::ErrorNone, ret, compiler.GetErrorW());
		}
        TEST_METHOD(DeclarationBadTest)
        {
            Compiler compiler;
            auto ret = compiler.Compile("..\\..\\test\\resources\\syntax\\declarations-bad.txt");
            Assert::AreEqual(CompilerErrors::ErrorSyntax, ret, compiler.GetErrorW());
        }
        TEST_METHOD(AssignmentTest)
        {
            Compiler compiler;
            auto ret = compiler.Compile("..\\..\\test\\resources\\syntax\\assignment.txt");
            Assert::AreEqual(CompilerErrors::ErrorNone, ret, compiler.GetErrorW());
        }
        TEST_METHOD(AssignmentBadTest)
        {
            Compiler compiler;
            auto ret = compiler.Compile("..\\..\\test\\resources\\syntax\\assignment-bad.txt");
            Assert::AreEqual(CompilerErrors::ErrorSyntax, ret, compiler.GetErrorW());
        }
        TEST_METHOD(SructsTest)
        {
            Compiler compiler;
            auto ret = compiler.Compile("..\\..\\test\\resources\\syntax\\structs.txt");
            Assert::AreEqual(CompilerErrors::ErrorNone, ret, compiler.GetErrorW());
        }
	};
}