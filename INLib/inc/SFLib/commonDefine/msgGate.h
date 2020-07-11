//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonDefine/msgGate.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONDEFINE_MSGGATE_H__
#define __SFLIB_COMMONDEFINE_MSGGATE_H__

#include <BCLib/framework/msgExecMgr.h>
#include <SFLib/commonServer/msgLabel.h>
#include <SFLib/message/message.h>

namespace SFLib
{
    class SFLIB_COMMON_API CMsgGate
    {
    public:
        static CMsgGate& getSingleton()
        {
            return *m_pMsgGate;
        }

        bool addMsgExecMgr(BCLib::int32 funcType, BCLib::Framework::CMsgExecMgr* msgExecMgr);
        bool delMsgExecMgr(BCLib::int32 funcType);
        BCLib::Framework::CMsgExecMgr* findMsgExecMgr(BCLib::int32 funcType);

        virtual bool parseMsg(const SFLib::CommonServer::SSFMsgLabel& msgLabel, const SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
        virtual bool parseMsg(SFLib::PeerID peerID, const SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    public:
        // 服务器专用
        virtual bool sendMsgToGC(SFLib::PeerID peerID, SFLib::Message::CNetMessage* msg);
        virtual bool sendMsgToGC(SFLib::PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

        // 客户端专用
        virtual bool sendMsgToSV(SFLib::Message::CNetMessage* msg);
        virtual bool sendMsgToSV(SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

        const SFLib::Message::SNetMessage* getCurNetMessage() const { return m_curNetMessage; }

    protected:
        CMsgGate();
        virtual ~CMsgGate();

    private:
        static CMsgGate* m_pMsgGate;

        typedef BCLib::Utility::CHashMap<BCLib::int32, BCLib::Framework::CMsgExecMgr*> CMsgExecMgrHashMap;
        CMsgExecMgrHashMap m_msgExecMgrHashMap;

        // 当前正在处理的消息
        const SFLib::Message::SNetMessage* m_curNetMessage;
    };
}//SFLib

#endif//__SFLIB_COMMONDEFINE_MSGGATE_H__
