//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib/Utility/clrCallback.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_CLRCALLBACK_H__
#define __CLLIB_UTILITY_CLRCALLBACK_H__

#include <CLLib/Utility/delegateDef.h>

namespace CLLib
{
    namespace Utility
    {
        class CNativeCallback
        {
        public:
            CNativeCallback();
            virtual ~CNativeCallback();

            void setNativeDataReceiveCallback(NativeDataReceiveCallback nativeDataReceiveCb);
            void setNativeReleaseCallback(NativeReleaseCallback nativeReleaseCb);
            void setNativeNewObjCallback(NativeNewObjCallback nativeNewObjCb);

            const void* MsgBuff() const { return m_msgBuff; }
            void* Obj() const { return m_obj; }

        protected:
            virtual void _cbReceiveMsg(const void* msgBuff, unsigned int msgSize);
            virtual void _cbNewObj(void* obj);

        protected:
            NativeDataReceiveCallback m_nativeDataReceiveCb;
            NativeReleaseCallback m_nativeReleaseCb;
            NativeNewObjCallback m_nativeNewObjCb;
            const void* m_msgBuff;
            void* m_obj;
        };

        public ref class CClrCallback
        {
        public:
            CClrCallback(DataReceiveCallback^ dataReceiveCallback, NewObjCallback^ newObjCallback, ReleaseCallback^ releaseCallback);
            virtual ~CClrCallback();

        protected:
            void    _initAll(CNativeCallback* nativeCb);
            void    _initDataReceiveCb(CNativeCallback* nativeCb);
            void    _initNewObjCb(CNativeCallback* nativeCb);
            void    _initReleaseCb(CNativeCallback* nativeCb);

            virtual void    _nativeDataReceiveCbWrapper(System::UInt32 msgSize, CNativeCallback* nativeCb);
            virtual void    _nativeNewObjCbWrapper(CNativeCallback* nativeCb);
            virtual void    _nativeReleaseCbWrapper(CNativeCallback* nativeCb);

        protected:
            DataReceiveCallback^ m_dataReceiveCb; // C# 设置过来的函数
            DataReceiveCallbackWrapper^ m_dataReceiveCbWrapper;
            NativeDataReceiveCallback m_nativeDataReceiveCb;

            NewObjCallback^ m_newObjCb;
            NewObjCallbackWrapper^ m_newObjCbWrapper;
            NativeNewObjCallback m_nativeNewObjCb;

            ReleaseCallback^    m_releaseCb;
            ReleaseCallbackWrapper^ m_releaseCbWrapper;
            NativeReleaseCallback m_nativeReleaseCb;
        };
    }//Utility
}//CLLib

#endif//__CLLIB_UTILITY_CLRCALLBACK_H__