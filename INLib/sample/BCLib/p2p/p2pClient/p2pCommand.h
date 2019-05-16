//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/p2pClient/p2pCommand.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_P2P_P2PCLIENT_P2PCOMMAND_H__
#define __BCLIB_SAMPLE_P2P_P2PCLIENT_P2PCOMMAND_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/command.h>

class CP2pClient;

class CP2pCommand : public BCLib::Utility::CCommand
{
public:
    typedef BCLib::uint32(CP2pCommand::*FCommandObj)(BCLib::Utility::CProperties&);
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
    CP2pCommand(CP2pClient* p2sClient);
    virtual ~CP2pCommand();

    virtual void init();

    BCLib::uint32 quit(BCLib::Utility::CProperties& properties);
    BCLib::uint32 login(BCLib::Utility::CProperties& properties);
    BCLib::uint32 connect(BCLib::Utility::CProperties& properties);
    BCLib::uint32 send(BCLib::Utility::CProperties& properties);

private:
    virtual int _execCmd(const std::string& command, BCLib::Utility::CProperties& properties, void* tempPara);

private:
    CP2pClient* m_p2sClient;
    BCLib::Utility::CHashMap<std::string, SCommandItem> m_commands;
};

#endif//__BCLIB_SAMPLE_P2P_P2PCLIENT_P2PCOMMAND_H__
