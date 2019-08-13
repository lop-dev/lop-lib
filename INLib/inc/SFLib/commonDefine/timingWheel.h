//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonDefine/timingWheel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONDEFINE_TIMINGWHEEL_H__
#define __SFLIB_COMMONDEFINE_TIMINGWHEEL_H__

#include <BCLib/utility/spointer.h>
#include <BCLib/utility/algorithm/timingWheel.h>
#include <SFLib/commonDefine/baseDef.h>
#include <SFLib/message/message.h>
#include <BCLib/utility/thread/mutex.h>

namespace SFLib
{
    namespace CommonDefine
    {
        class IObjectOwner
        {
        public:
            IObjectOwner() {};
            virtual ~IObjectOwner() {};

            virtual void saveObject(SFLib::PeerID peerID, SFLib::Message::CNetMessage* pNetMessage) = 0;
            virtual void saveObject(SFLib::ServerID serverID, SFLib::Message::CNetMessage* pNetMessage) = 0;
        };

        //
        class CObjectRef : public BCLib::Utility::CRefCnt
        {
        public:
            CObjectRef(SFLib::PeerID peerID, SFLib::ServerID serverID, SFLib::Message::CNetMessage* pNetMessage, IObjectOwner* pObjectOwner);
            virtual ~CObjectRef();

        private:
            SFLib::PeerID m_peerID;
            SFLib::ServerID m_serverID;
            SFLib::Message::CNetMessage* m_pNetMessage;
            IObjectOwner* m_pObjectOwner;
        };
        typedef BCLib::Utility::CSPointer<CObjectRef> CObjectRefPtr;

        //
        class CObjectTimingWheel : public BCLib::Utility::CTimingWheel<CObjectRefPtr>
        {
        public:
            CObjectTimingWheel(IObjectOwner* pObjectOwner);
            virtual ~CObjectTimingWheel();

            void  addObject(SFLib::PeerID netPeerID, SFLib::ServerID serverID, SFLib::Message::CNetMessage* pNetMessage);
            void  delObject(SFLib::PeerID netPeerID);

        private:
            IObjectOwner* m_pObjectOwner;
        };
    }//CommonDefine
}//SFLib

#endif//__SFLIB_COMMONDEFINE_TIMINGWHEEL_H__
