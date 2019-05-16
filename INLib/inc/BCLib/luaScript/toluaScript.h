//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/luaScript/toluaScript.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_LUASCRIPT_TOLUASCRIPT_H__
#define __BCLIB_LUASCRIPT_TOLUASCRIPT_H__

#include <BCLib/luaScript/luaScript.h>

namespace BCLib
{
namespace LuaScript
{
class BCLIB_LUASCRIPT_API CToLuaScript : public CLuaScript
{
public:
    //CToLuaScript();

    /// @brief
    /// @param fnLoader 替换Lua默认加载功能的函数，NULL为不替换
    /// @param
    CToLuaScript(lua_CFunction fnLoader);
    virtual ~CToLuaScript();

    const wchar_t* getGlobalWString(const char* name);

    void setGlobalPointer(const char* sName, const char* sType, void* pValue);
    void getGlobalPointer(const char* sName, const char* sType, void*& pValue);//To get this point of the "sType".
    void getType(const char* sName, std::string& sType);
};

class BCLIB_LUASCRIPT_API CToLuaCall : public CLuaCall
{
public:
    CToLuaCall(lua_State* ls);

    std::wstring argAsWString(int idx);//UTF-8 to UTF-16
    std::wstring argWString(int idx);//UTF-8 to UTF-16

    //int ReturnValue(const wchar_t *value);
    int returnPointerValue(void* value, const char* type);
    int returnPointerValue(const char* sName);
};
}//LuaScript
}//BCLib

#endif//__BCLIB_LUASCRIPT_TOLUASCRIPT_H__
