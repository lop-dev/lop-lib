//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/command.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/convert.h>
#include "command.h"
#include "message.h"

CCommand::SCommandItem g_gameCommands[] =
{
    CCommand::SCommandItem("quit", &CCommand::quit, "退出客户端"),
    CCommand::SCommandItem("connect", &CCommand::connect, "连接服务器"),
    CCommand::SCommandItem("send", &CCommand::send, "连接服务器"),
    CCommand::SCommandItem("sendc", &CCommand::sendc, "连接服务器"),
};

CCommand::CCommand()
{
    ;
}

CCommand::~CCommand()
{
    ;
}

void CCommand::init(CMainThread* mainThread)
{
    m_mainThread = mainThread;
    int count = sizeof(g_gameCommands) / sizeof(CCommand::SCommandItem);
    for(int i = 0; i < count; ++i)
    {
        m_commands.setValue(g_gameCommands[i].m_comandName, g_gameCommands[i]);
    }
}

void CCommand::close()
{
    m_netClient.terminate();
}

BCLib::uint32 CCommand::quit(BCLib::Utility::CProperties& properties)
{
    m_mainThread->terminate();
    return 1;
}

BCLib::uint32 CCommand::connect(BCLib::Utility::CProperties& properties)
{
    std::string ip = properties.getValueString("ip");
    BCLib::uint32 port = properties.getValueInt32("port");
    if(ip.empty())
    {
        ip = properties.getValueString("1");
        if(ip.empty())
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "ip不能为空");
            return 0;
        }
    }
    if(port == 0)
    {
        port = properties.getValueInt32("2");
        if(port == 0)
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "port无效");
        }
    }

    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "连接服务器[%s:%d]......", ip.c_str(), port);

    int count = 1;


    while(!m_netClient.connect(ip.c_str(), port))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "连接服务器[%s:%d]失败", ip.c_str(), port);
        if(count < 3)
        {
            BCLib::Utility::CThread::sleep(1);
            count ++;
        }
        else
        {
            return 0;
        }
    }
    m_netClient.start();
    SMsgSampleNtfOk ntfOK;
    if(!m_netClient.sendMsg(&ntfOK, sizeof(ntfOK)))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");
    }
    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "连接服务器[%s:%d]成功", ip.c_str(), port);
    return 0;
}

BCLib::uint32 CCommand::send(BCLib::Utility::CProperties& properties)
{
    std::string text = properties.getValueString("text");
    if(text.empty())
    {
        text = properties.getValueString("1");
        if(text.empty())
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "text不能为空");
            return 0;
        }
    }
    CMsgSampleReqSay reqSay;
    reqSay.m_int = BCLib::Utility::CConvert::toInt32(text);

    if(!m_netClient.sendMsg(reqSay))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");
    }
    return 0;
}

BCLib::uint32 CCommand::sendc(BCLib::Utility::CProperties& properties)
{
    std::string text = properties.getValueString("text");
    if(text.empty())
    {
        text = properties.getValueString("1");
        if(text.empty())
        {
            BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "text不能为空");
            return 0;
        }
    }
    CMsgSampleReqSayC reqSay;
    reqSay.m_strSay = text;

    if(!m_netClient.sendMsg(reqSay))
    {
        BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "发送消息失败");
    }
    return 0;
}

int CCommand::_execCmd(const std::string& command, BCLib::Utility::CProperties& properties, void* tempPara)
{
    SCommandItem item = m_commands.getValue(command);
    if(item.m_commandObjFun != NULL)
    {
        return (this->*item.m_commandObjFun)(properties);
    }
    return -1;
}