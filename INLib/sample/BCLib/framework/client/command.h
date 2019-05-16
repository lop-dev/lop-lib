//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/netfw/client/command.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_COMMAND_H__
#define __BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_COMMAND_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/command.h>
#include "mainThread.h"
#include "client.h"

class CCommand : public BCLib::Utility::CCommand
{
public:
    typedef BCLib::uint32(CCommand::*FCommandObj)(BCLib::Utility::CProperties&);

    struct SCommandItem
    {
        std::string m_comandName;
        FCommandObj m_commandObjFun;
        std::string m_context;

    public:
        SCommandItem()
        :m_commandObjFun(NULL)
        {
        }

        SCommandItem(const SCommandItem& other)
        {
            m_comandName = other.m_comandName;
            m_commandObjFun = other.m_commandObjFun;
            m_context = other.m_context;
        }
        SCommandItem(const std::string& comandName, FCommandObj commandObjFun, const std::string& context)
        {
            m_comandName = comandName;
            m_commandObjFun = commandObjFun;
            m_context = context;
        }
    };

public:
    CCommand();
    virtual ~CCommand();

    void init(CMainThread* mainThread);
    void close();

public:
    BCLib::uint32 quit(BCLib::Utility::CProperties& properties);
    BCLib::uint32 connect(BCLib::Utility::CProperties& properties);
    BCLib::uint32 send(BCLib::Utility::CProperties& properties);
    BCLib::uint32 sendc(BCLib::Utility::CProperties& properties);

private:
    virtual int _execCmd(const std::string& command, BCLib::Utility::CProperties& properties, void* tempPara);

private:
    CMainThread* m_mainThread;
    BCLib::Utility::CHashMap<std::string, SCommandItem> m_commands;

    CClient m_netClient;
};

#endif//__BCLIB_SAMPLE_FRAMEWORK_NETFW_CLIENT_COMMAND_H__
