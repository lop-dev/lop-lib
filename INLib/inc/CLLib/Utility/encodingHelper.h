//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\encodingHelper.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_ENCODINGHELPER_H__
#define __CLLIB_UTILITY_ENCODINGHELPER_H__

#include <string>
#include <CLLib/Utility/managerBufferHelper.h>

namespace CLLib
{
    namespace Utility
    {
        public ref class CEncodingHelper
        {
        public:
            static CEncodingHelper^ singleton()
            {
                return %m_Instance;
            }

            CEncodingHelper()
            {
                CLLib::Utility::CManagerBufferHelper::singleton()->Init(1, 1024);
            }

            System::String^ ConvertUTF8String(const std::string& val)
            {
                const char* nativeBuffer = val.c_str();
                array<System::Byte>^ managedBuffer = nullptr;
                CLLib::Utility::CByteArray^ byteArray = CLLib::Utility::CManagerBufferHelper::singleton()->ConvertNavtiveByteArray(nativeBuffer, val.length(), managedBuffer);
                CLLib::Utility::CManagerBufferHelper::singleton()->FreeMemory(byteArray);
                System::String^ text = System::Text::Encoding::UTF8->GetString(managedBuffer, 0, val.length());
                return text;
            }

        private:
            static CEncodingHelper m_Instance;
        };
    }//Utility
}//CLLib

#endif//__CLLIB_UTILITY_ENCODINGHELPER_H__