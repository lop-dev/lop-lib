//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/network/pipe/pipeStub.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_NETWORK_PIPE_PIPESTUB_H__
#define __BCLIB_NETWORK_PIPE_PIPESTUB_H__

#include <BCLib/network/baseDef.h>
#include <BCLib/utility/spointer.h>


namespace BCLib
{
namespace Utility
{
    class CStream;
}//Utility
namespace Network
{
class CPipeServer;
class CPipe;
typedef BCLib::Utility::CSPointer<CPipe> CPipeSPtr;
class CPipeStubCallback;

class BCLIB_NETWORK_API CPipeStub : public BCLib::Utility::CRefCnt
{
    friend class CPipeServer;
    friend class CPipeStubCallback;
public:
    CPipeStub(const CPipeSPtr& pipePtr);
    virtual ~CPipeStub();

    PipeStubID getStubID() const;
    PipeID getPipeID() const;
    void close();

    CPipeSPtr getPipe() const;

    BCLib::int32 send(BCLib::Utility::CStream& stream);
    BCLib::int32 send(const void* msgBuf, BCLib::uint32 bufSize);

protected:
    void _setStubID(PipeStubID stubID);
    virtual bool _cbParseMsg(const void* msgBuff, BCLib::uint32 msgSize) = 0;
    virtual void _cbTerminate();

private:
    PipeStubID m_stubID;
    CPipeSPtr m_pipePtr;
};
typedef BCLib::Utility::CSPointer<CPipeStub> CPipeStubSPtr;
}//Network
}//BCLib

#endif//__BCLIB_NETWORK_PIPE_PIPESTUB_H__
