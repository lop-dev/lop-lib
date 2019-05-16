//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/luaScript/toluapp/main.cpp
//  author:     League of Perfect
/// @brief      演示tolua++使用的主程序，通过导入recorder类进入脚本，可以在脚本中
///             使用recorder类，进行记录和播放。
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/luaScript/luaScript.h>
#include "recorder.h"

#include "tolua++.h"
#include "recorder.tolua.h"

BCLib::LuaScript::SToLuaPackage gsLuaPackageList[] =
{
    {"recorder.pkg", tolua_recorder_open},
};

int main(int argc, char argv[])
{
    BCLib::LuaScript::CLuaScript luaScript(NULL);

    bool ret = luaScript.RegisterToLuaPackageList(gsLuaPackageList, sizeof(gsLuaPackageList) / sizeof(gsLuaPackageList[0]));
    if(!ret)
    {
        printf("failed to register To-Lua-Package\n");
        return -1;
    }

    ret = luaScript.executeFile("main.lua");
    if(!ret)
    {
        printf("failed to execute lua file\n");
        return -1;
    }

    CRecorder recorder;

    luaScript.beginCall("test");
    luaScript.param(&recorder, "CRecorder");
    luaScript.endCall();

    return 0;
}