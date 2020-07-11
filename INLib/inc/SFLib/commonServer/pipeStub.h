//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/commonServer/pipeStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_COMMONSERVER_PIPESTUB_H__
#define __SFLIB_COMMONSERVER_PIPESTUB_H__

#include <BCLib/utility/pointer.h>
#include <BCLib/utility/task.h>
#include <BCLib/utility/dateTime.h>
#include <BCLib/network/pipe/pipeStub.h>
#include <BCLib/framework/message.h>
#include <BCLib/framework/msgType.h>
#include <BCLib/framework/msgExecMgr.h>
#include <SFLib/message/gameFrame.h>
#include <SFLib/commonServer/netPeer.h>
#include <SFLib/commonServer/msgLabel.h>

namespace SFLib
{
namespace CommonServer
{
class SFLIB_COMMON_API CPipeStub : public BCLib::Framework::CMsgExecMgr, public BCLib::Network::CPipeStub
{
public:
    CPipeStub(const BCLib::Network::CPipeSPtr& pipePtr);

    ServerID getServerID() const
    {
        return m_serverID;
    }

    EServerType getServerType() const
    {
        return m_serverType;
    }

    std::string getDebugPrompt();

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual bool _cbParseMsg2(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _cbTerminate();

    virtual void _unhandledMsg(SSFMsgLabel& msgLabel, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);
    virtual bool _transformMsg(PeerID peerID, SFLib::Message::SNetMessage* msg, BCLib::uint32 msgSize);

    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::uint16 id, BCLib::Framework::CMsgExecPtr& msgExecPtr);
    virtual bool _createMsgExecPtr(BCLib::uint16 type, BCLib::Framework::CMsgExecPtr& msgExecPtr);

    virtual void _onXX2XSNtfServerType(BCLib::Framework::SThdMsgLabel* msgLabel, BCLib::Framework::SMessage* msg);

    void _setServerType(EServerType serverType)
    {
        m_serverType = serverType;
    }
    void _setServerID(ServerID serverID)
    {
        m_serverID = serverID;
    }

private:
    EServerType m_serverType;
    ServerID m_serverID;
};
typedef BCLib::Utility::CSPointer<CPipeStub> CPipeStubPtr;
}//CommonServer
}//SFLib

#endif//__SFLIB_COMMONSERVER_PIPESTUB_H__
