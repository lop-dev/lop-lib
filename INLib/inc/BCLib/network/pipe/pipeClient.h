//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/pipe/pipeClient.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_PIPE_PIPECLIENT_H__
#define __BCLIB_NETWORK_PIPE_PIPECLIENT_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/spointer.h>
#include <BCLib/utility/stream.h>


namespace BCLib
{
namespace Network
{
class CPipe;
typedef BCLib::Utility::CSPointer<CPipe> CPipeSPtr;
class CPipeMgr;
class CPipeClientCallback;

class BCLIB_NETWORK_API CPipeClient : public BCLib::Utility::CRefCnt
{
    friend class CPipeClientCallback;
public:
    CPipeClient();
    virtual ~CPipeClient();

    bool start();
    void terminate();
    bool isRunning() const;
    void join();

    bool connect(const char* serverName);
    void close();

    BCLib::int32 send(BCLib::Utility::CStream& stream);
    BCLib::int32 send(const void* msgBuf, BCLib::uint32 bufSize);

protected:
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize) = 0;
    virtual void _cbTerminate();

    bool _addPipe2Mgr();

private:
    BCLib::uint8 m_pipeState;
    CPipeSPtr m_pipePtr;
    CPipeMgr* m_pipeMgr;
};
typedef BCLib::Utility::CSPointer<CPipeClient> CPipeClientSPtr;
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_PIPE_PIPECLIENT_H__
