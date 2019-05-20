//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\Utility\delegateDef.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_DELEGATEDEF_H__
#define __CLLIB_UTILITY_DELEGATEDEF_H__

namespace CLLib
{
    namespace Utility
    {
        class CNativeCallback;

        public delegate void DataReceiveCallback(array<System::Byte>^ msgBuffer, System::UInt32 msgSize);
        public delegate void DataReceiveCallbackWrapper(System::UInt32 msgSize, CNativeCallback* nativeCb);
        typedef void(__stdcall *NativeDataReceiveCallback)(unsigned int size, CNativeCallback* nativeCb);

        public delegate void NewObjCallback(System::Object^ obj);
        public delegate void NewObjCallbackWrapper(CNativeCallback* nativeCb);
        typedef void(__stdcall *NativeNewObjCallback)(CNativeCallback* nativeCb);

        public delegate void ReleaseCallback();
        public delegate void ReleaseCallbackWrapper(CNativeCallback* nativeCb);
        typedef void(__stdcall *NativeReleaseCallback)(CNativeCallback* nativeCb);
    }//Utility
}//CLLib

#endif//__CLLIB_NETWORK_DELEGATEDEF_H__