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
#include <BCLib/database/baseDef.h>
#include <SFLib/commonServer/netPeer.h>

namespace SFLib
{
    namespace CommonDefine
    {
		typedef bool(*FSendMsgToDB1)(BCLib::uint32 classNameKey, BCLib::Database::EDBActID actID, void* pBuf, SFLib::ServerID serverID);
		typedef bool(*FSendMsgToDB2)(BCLib::uint32 classNameKey, BCLib::Database::EDBActID actID, void* pBuf, SFLib::CommonServer::CNetPeerPtr& netPeer);

        class CObjectOwner
        {
        public:
            CObjectOwner() {};
            virtual ~CObjectOwner() {};

            virtual void saveObject(SFLib::PeerID peerID, SFLib::Message::CNetMessage* pNetMessage) = 0;
            virtual void saveObject(SFLib::ServerID serverID, SFLib::Message::CNetMessage* pNetMessage) = 0;

			bool sendMsgToDB(BCLib::uint32 classNameKey, BCLib::Database::EDBActID actID, void* pBuf, SFLib::ServerID serverID);
			bool sendMsgToDB(BCLib::uint32 classNameKey, BCLib::Database::EDBActID actID, void* pBuf, SFLib::CommonServer::CNetPeerPtr& netPeer);

		public:
			static FSendMsgToDB1 m_FSendMsgToDB1;
			static FSendMsgToDB2 m_FSendMsgToDB2;
        };

        //
        class CObjectRef : public BCLib::Utility::CRefCnt
        {
        public:
            CObjectRef(SFLib::PeerID peerID, SFLib::ServerID serverID, SFLib::Message::CNetMessage* pNetMessage, CObjectOwner* pObjectOwner);
            virtual ~CObjectRef();

        private:
            SFLib::PeerID m_peerID;
            SFLib::ServerID m_serverID;
            SFLib::Message::CNetMessage* m_pNetMessage;
            CObjectOwner* m_pObjectOwner;
        };
        typedef BCLib::Utility::CSPointer<CObjectRef> CObjectRefPtr;

        //
        class CObjectTimingWheel : public BCLib::Utility::CTimingWheel<CObjectRefPtr>
        {
        public:
            CObjectTimingWheel(CObjectOwner* pObjectOwner);
            virtual ~CObjectTimingWheel();

            void  addObject(SFLib::PeerID netPeerID, SFLib::ServerID serverID, SFLib::Message::CNetMessage* pNetMessage);
            void  delObject(SFLib::PeerID netPeerID);

        private:
            CObjectOwner* m_pObjectOwner;
        };
    }//CommonDefine
}//SFLib

#endif//__SFLIB_COMMONDEFINE_TIMINGWHEEL_H__
