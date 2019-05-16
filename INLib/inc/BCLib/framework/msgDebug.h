//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgDebug.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGDEBUG_H__
#define __BCLIB_FRAMEWORK_MSGDEBUG_H__

#include <BCLib/framework/baseDef.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/framework/message.h>

namespace BCLib
{
namespace Framework
{
class BCLIB_FRAMEWORK_API CMsgDebug
{
    BCLIB_SINGLETON_DECLARE(CMsgDebug);

protected:
    CMsgDebug();
    virtual ~CMsgDebug();

public:
    void setPrompt(uint16 type, uint16 id, const std::string& strPrompt);
    std::string getPrompt(uint16 type, uint16 id);

    std::string getPrompt(SMessage* message);
    std::string getPrompt(CMessage* message);

private:
    BCLib::Utility::CHashMap<uint32, std::string> m_hashMap;
};

#ifdef _DEBUG


#define BCLIB_MSGDEBUG(type, id) \
    class CMsgDebug_ ## id\
    {\
    public:\
        CMsgDebug_ ## id()\
        {\
            BCLib::Framework::CMsgDebug::singleton().setPrompt(type, id, #id);\
        }\
    };\
    static CMsgDebug_ ## id g_msgDebug_ ## id;

#else

//#define BCLIB_MSGDEBUG(type, id);

// TODO 临时在Release版本中，打开消息调试
#define BCLIB_MSGDEBUG(type, id) \
    class CMsgDebug_ ## id\
    {\
    public:\
        CMsgDebug_ ## id()\
        {\
            BCLib::Framework::CMsgDebug::singleton().setPrompt(type, id, #id);\
        }\
    };\
    static CMsgDebug_ ## id g_msgDebug_ ## id;

#endif
}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_MSGDEBUG_H__
