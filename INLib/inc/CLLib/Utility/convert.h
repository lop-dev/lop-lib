//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\convert.hpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <msclr/marshal.h>          // .NET string to C-style string
#include <msclr/marshal_cppstd.h>   // .NET string to STL string
#include <string>
using namespace msclr::interop;

namespace CLLib
{
    namespace Utility
    {
        public ref class CConvert
        {
        public:
            static std::string DotNetStringToSTLString(System::String^ netString)
            {
                marshal_context^ context = gcnew marshal_context();
                std::string stlString = context->marshal_as<std::string>(netString);

                return stlString;
            }

            static System::String^ STLStringToDotNetString(std::string& stlString)
            {
                System::String^ netString = gcnew System::String(stlString.c_str());
                return netString;
            }

            static std::wstring DotNetStringToSTLWString(System::String^ netString)
            {
                marshal_context^ context = gcnew marshal_context();
                std::wstring stlWString = context->marshal_as<std::wstring>(netString);

                return stlWString;
            }

            static System::String^ STLWStringToDotNetString(std::wstring& stlWString)
            {
                System::String^ netString = gcnew System::String(stlWString.c_str());
                return netString;
            }
        };
    }
}