//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pClient/p2pCommand.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/framework/sysConfig.h>
#include <BCLib/utility/logFile.h>

#include "p2pCommand.h"
#include "p2pClient.h"
#include "message.h"

CP2pCommand::SCommandItem g_gameCommands[] =
{
    CP2pCommand::SCommandItem("login", &CP2pCommand::login, "登录P2P服务器"),
    CP2pCommand::SCommandItem("connect", &CP2pCommand::connect, "连接Peer客户端"),
    CP2pCommand::SCommandItem("send", &CP2pCommand::send, "发送测试消息"),
    CP2pCommand::SCommandItem("quit", &CP2pCommand::quit, "退出客户端"),
};

CP2pCommand::CP2pCommand(CP2pClient* p2sClient)
:m_p2sClient(p2sClient)
{
    ;
}

CP2pCommand::~CP2pCommand()
{
    ;
}

void CP2pCommand::init()
{
    int count = sizeof(g_gameCommands) / sizeof(CP2pCommand::SCommandItem);
    for(int i = 0; i < count; ++i)
    {
        m_commands.setValue(g_gameCommands[i].m_comandName, g_gameCommands[i]);
    }
}

BCLib::uint32 CP2pCommand::quit(BCLib::Utility::CProperties& properties)
{
    m_p2sClient->terminate();
    return 1;
}

BCLib::uint32 CP2pCommand::login(BCLib::Utility::CProperties& properties)
{
    //std::string serverIP = properties.getValueString("ip");
    //BCLib::uint16 serverPort = 9000;

    //std::string name = properties.getValueString("name");
    //std::string password = properties.getValueString("password");
    //if(serverIP.empty() || serverIP == "")
    //{
    //    serverIP = properties.getValueString("1");
    //    if(serverIP.empty() || serverIP == "")
    //    {
    //        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "ip 不能为空");
    //        return 0;
    //    }
    //}

    //if(name.empty() || name == "")
    //{
    //    name = properties.getValueString("2");
    //    if(name.empty() || name == "")
    //    {
    //        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "name 不能为空");
    //        return 0;
    //    }
    //}
    //if(password.empty() || password == "")
    //{
    //    password = properties.getValueString("3");
    //}

    std::string serverIP = "192.168.1.120";
    BCLib::uint16 serverPort = 9000;
    std::string name = "abc";
    std::string password = "123";

    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "连接服务器[%s:%d]......", serverIP.c_str(), serverPort);

    if(!m_p2sClient->connectServer(serverIP.c_str(), serverPort))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "连接服务器[%s:%d]失败", serverIP.c_str(), serverPort);
        return 0;
    }
    SMsgPc2PsReqLogin loginMsg;
    loginMsg.setName(name.c_str());
    loginMsg.setPassword(password.c_str());
    m_p2sClient->sendMsgToServer(&loginMsg, sizeof(loginMsg));
    return 0;
}

BCLib::uint32 CP2pCommand::connect(BCLib::Utility::CProperties& properties)
{
    BCLib::P2p::PeerID peerID = properties.getValueInt32("peer");

    if(peerID == 0)
    {
        peerID = properties.getValueInt32("1");
        if(peerID == 0)
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "peer 不能为0");
            return 0;
        }
    }

    if(!m_p2sClient->connectPeer(peerID))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "连接Peer[%d]失败", peerID);
        return 0;
    }
    return 0;
}

BCLib::uint32 CP2pCommand::send(BCLib::Utility::CProperties& properties)
{
    std::string toTarget = properties.getValueString("to");
    std::string text = properties.getValueString("text");
    if(toTarget == "server")
    {
        SMsgPc2PsNtfSay sayMsg;
        sayMsg.setText(text);
        m_p2sClient->sendMsgToServer(&sayMsg, sizeof(sayMsg));
    }
    else if(toTarget == "peer")
    {
        BCLib::P2p::PeerID peerID = properties.getValueInt32("peer");
        if(peerID == 0)
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "peer 不能为0");
            return 0;
        }
        SMsgPc2PcNtfSay sayMsg;
        sayMsg.setText(text);
        sayMsg.m_time = m_p2sClient->getNowTime();
        m_p2sClient->sendMsgToPeer(peerID, &sayMsg, sizeof(sayMsg));
    }
    return 0;
}

int CP2pCommand::_execCmd(const std::string& command, BCLib::Utility::CProperties& properties, void* tempPara)
{
    SCommandItem item = m_commands.getValue(command);
    if(item.m_commandObjFun != NULL)
    {
        return (this->*item.m_commandObjFun)(properties);
    }
    return -1;
}