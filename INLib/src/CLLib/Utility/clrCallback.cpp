//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib/Utility/clrCallback.cpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#include <CLLib/Utility/clrCallback.h>
#include <CLLib/Utility/managerBufferHelper.h>

using namespace System::Runtime::InteropServices;

namespace CLLib
{
    namespace Utility
    {
#ifndef NULL
#define NULL 0
#endif
        CNativeCallback::CNativeCallback() :
            m_nativeDataReceiveCb(NULL),
            m_nativeReleaseCb(NULL),
            m_nativeNewObjCb(NULL),
            m_msgBuff(NULL),
            m_obj(NULL)

        {

        }

        CNativeCallback::~CNativeCallback()
        {
            if (m_nativeReleaseCb != NULL)
            {
                m_nativeReleaseCb(this);
            }
        }

        void CNativeCallback::setNativeDataReceiveCallback(NativeDataReceiveCallback nativeDataReceiveCb)
        {
            m_nativeDataReceiveCb = nativeDataReceiveCb;
        }

        void CNativeCallback::setNativeReleaseCallback(NativeReleaseCallback nativeReleaseCb)
        {
            m_nativeReleaseCb = nativeReleaseCb;
        }

        void CNativeCallback::setNativeNewObjCallback(NativeNewObjCallback nativeNewObjCb)
        {
            m_nativeNewObjCb = nativeNewObjCb;
        }

        void CNativeCallback::_cbReceiveMsg(const void* msgBuff, unsigned int msgSize)
        {
            if (m_nativeDataReceiveCb != NULL)
            {
                m_msgBuff = msgBuff;
                m_nativeDataReceiveCb(msgSize, this);
                m_msgBuff = NULL;
            }
        }

        void CNativeCallback::_cbNewObj(void* obj)
        {
            if (m_nativeNewObjCb != NULL)
            {
                m_obj = obj;
                m_nativeNewObjCb(this);
                m_obj = NULL;
            }
        }

        //////////////////////////////////////////////////////////////////////////

        CClrCallback::CClrCallback(DataReceiveCallback^ dataReceiveCallback, NewObjCallback^ newObjCallback, ReleaseCallback^ releaseCallback) :
            m_dataReceiveCb(dataReceiveCallback),
            m_newObjCb(newObjCallback),
            m_releaseCb(releaseCallback),
            m_nativeDataReceiveCb(NULL),
            m_nativeNewObjCb(NULL),
            m_nativeReleaseCb(NULL),
            m_dataReceiveCbWrapper(nullptr),
            m_newObjCbWrapper(nullptr),
            m_releaseCbWrapper(nullptr)
        {
        }

        CClrCallback::~CClrCallback()
        {

        }

        void CClrCallback::_initDataReceiveCb(CNativeCallback* nativeCb)
        {
            if (nativeCb != NULL)
            {
                //receive cb
                if (m_nativeDataReceiveCb == NULL)
                {
                    m_dataReceiveCbWrapper = gcnew DataReceiveCallbackWrapper(this, &CLLib::Utility::CClrCallback::_nativeDataReceiveCbWrapper);
                    System::IntPtr ptr = Marshal::GetFunctionPointerForDelegate(m_dataReceiveCbWrapper);
                    m_nativeDataReceiveCb = static_cast<NativeDataReceiveCallback>(ptr.ToPointer());
                }
                nativeCb->setNativeDataReceiveCallback(m_nativeDataReceiveCb);
            }
        }

        void CClrCallback::_initNewObjCb(CNativeCallback* nativeCb)
        {
            if (nativeCb != NULL)
            {
                //new obj cb
                if (m_nativeNewObjCb == NULL)
                {
                    m_newObjCbWrapper = gcnew NewObjCallbackWrapper(this, &CLLib::Utility::CClrCallback::_nativeNewObjCbWrapper);
                    System::IntPtr ptr = Marshal::GetFunctionPointerForDelegate(m_newObjCbWrapper);
                    m_nativeNewObjCb = static_cast<NativeNewObjCallback>(ptr.ToPointer());
                }
                nativeCb->setNativeNewObjCallback(m_nativeNewObjCb);
            }
        }

        void CClrCallback::_initReleaseCb(CNativeCallback* nativeCb)
        {
            if (nativeCb != NULL)
            {
                //release cb
                if (m_nativeReleaseCb == NULL)
                {
                    m_releaseCbWrapper = gcnew ReleaseCallbackWrapper(this, &CLLib::Utility::CClrCallback::_nativeReleaseCbWrapper);
                    System::IntPtr ptr = Marshal::GetFunctionPointerForDelegate(m_releaseCbWrapper);
                    m_nativeReleaseCb = static_cast<NativeReleaseCallback>(ptr.ToPointer());
                }
                nativeCb->setNativeReleaseCallback(m_nativeReleaseCb);
            }
        }

        void CClrCallback::_initAll(CNativeCallback* nativeCb)
        {
            _initDataReceiveCb(nativeCb);
            _initNewObjCb(nativeCb);
            _initReleaseCb(nativeCb);
        }

        void CClrCallback::_nativeDataReceiveCbWrapper(System::UInt32 msgSize, CNativeCallback* nativeCb)
        {
            if (nativeCb != NULL&&m_dataReceiveCb != nullptr)
            {
                char* msgBuffer = (char*)nativeCb->MsgBuff();
                array<System::Byte>^ managedBuffer = nullptr;
                CLLib::Utility::CByteArray^ byteArray = CLLib::Utility::CManagerBufferHelper::singleton()->ConvertNavtiveByteArray(msgBuffer, msgSize, managedBuffer);
                if (managedBuffer != nullptr)
                {
                    m_dataReceiveCb(managedBuffer, msgSize);
                }
                CLLib::Utility::CManagerBufferHelper::singleton()->FreeMemory(byteArray);
            }
        }

        void CClrCallback::_nativeNewObjCbWrapper(CNativeCallback* nativeCb)
        {

        }

        void CClrCallback::_nativeReleaseCbWrapper(CNativeCallback* nativeCb)
        {
            if (m_releaseCb != nullptr)
            {
                m_releaseCb();
            }
        }
    }//Utility
}//CLLib