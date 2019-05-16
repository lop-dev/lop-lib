//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/singleton.h>
#include <BCLib/network/pipe/pipeClient.h>
#include <BCLib/framework/mainThread.h>


class CMyPipeClient : public BCLib::Network::CPipeClient
{
public:
    CMyPipeClient();
    virtual ~CMyPipeClient();

    bool sendMsg();

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize);
    virtual void _cbTerminate();
};

class CPipeClient : BCLib::Framework::CMainThread
{
    BCLIB_SINGLETON_DECLARE(CPipeClient);
public:
    static int main();

protected:
    CPipeClient();
    virtual ~CPipeClient();

    virtual bool _init();
    virtual bool _callback();
    virtual int _final();

protected:
    CMyPipeClient* m_pipe;
};
