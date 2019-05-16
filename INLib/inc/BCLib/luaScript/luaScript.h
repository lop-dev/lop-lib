//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/luaScript/luaScript.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_LUASCRIPT_LUASCRIPT_H__
#define __BCLIB_LUASCRIPT_LUASCRIPT_H__

#include <string>
#include <map>
#include <BCLib/luaScript/baseDef.h>

struct lua_State;
typedef int (*lua_CFunction) (lua_State* L);

struct lua_CLib
{
    const char* name;
    lua_CFunction func;
};

namespace BCLib
{
namespace LuaScript
{
// Lua value type
enum ELuaType
{
    LUATYPE_NONE            = -1,
    LUATYPE_NIL             = 0,
    LUATYPE_BOOLEAN         = 1,
    LUATYPE_LIGHTUSERDATA   = 2,
    LUATYPE_NUMBER          = 3,
    LUATYPE_STRING          = 4,
    LUATYPE_TABLE           = 5,
    LUATYPE_FUNCTION        = 6,
    LUATYPE_USERDATA        = 7,
    LUATYPE_THREAD          = 8
};

// Lua error code
const int LUAERR_RUN = 1;
const int LUAERR_FILE = 2;
const int LUAERR_SYNTAX = 3;
const int LUAERR_MEM = 4;
const int LUAERR_ERR = 5;

// pseudo table index
const int LUAINDEX_REGISTRY = -10000;
const int LUAINDEX_ENVIRON = -10001;
const int LUAINDEX_GLOBALS = -10002;


class CLuaValue;
typedef std::map<CLuaValue, CLuaValue> CLuaTable;

class CLuaState;


// global error handler
typedef void (*LuaScriptErrorFunc) (CLuaState* luaState, int err, const char* msg);
void LuaScriptSetErrorFunction(LuaScriptErrorFunc func);

class  BCLIB_LUASCRIPT_API CLuaState
{
public:
    CLuaState();
    CLuaState(lua_State* ls);
    virtual ~CLuaState();

    // set error handler, global handler is used if not set
    void setErrorFunction(LuaScriptErrorFunc func);

    // set global variable, add new one if not exist
    void setGlobal(const char* name, int value);
    void setGlobal(const char* name, bool value);
    void setGlobal(const char* name, float value);
    void setGlobal(const char* name, double value);
    void setGlobal(const char* name, const char* value);
    void setGlobal(const char* name, const wchar_t* value);
    void setGlobal(const char* name, const CLuaTable& value);
    void setGlobalPointer(const char* name, void* value);
    void setGlobalNil(const char* name);
    void setGlobal(const char* name, const CLuaValue& value);

    // get global variable with type checking
    bool getGlobal(const char* name, int& value);
    bool getGlobal(const char* name, bool& value);
    bool getGlobal(const char* name, float& value);
    bool getGlobal(const char* name, double& value);
    bool getGlobal(const char* name, std::string& value);
    bool getGlobal(const char* name, std::wstring& value);
    bool getGlobal(const char* name, CLuaTable& value);
    bool getGlobalPointer(const char* name, void*& value);
    bool isGlobalNil(const char* name);

    // get global variable, cast type automatically
    int getGlobalInt(const char* name);
    bool getGlobalBool(const char* name);
    float getGlobalFloat(const char* name);
    double getGlobalDouble(const char* name);
    const char* getGlobalString(const char* name);
    const wchar_t* getGlobalWString(const char* name);
    CLuaTable getGlobalTable(const char* name);
    void* getGlobalPointer(const char* name);

    // stack operations
    void pushNil();
    void push(int value);
    void push(bool value);
    void push(float value);
    void push(double value);
    void push(const char* value);
    void push(const wchar_t* value);
    void push(const CLuaValue& value);
    void push(const CLuaTable& value);
    void pushPointer(void* value);
    void pushUserData(void* pointer, const char* name);

    void pop();
    void pop(int& value);
    void pop(bool& value);
    void pop(float& value);
    void pop(double& value);
    void pop(std::string& value);
    void pop(std::wstring& value);
    void pop(CLuaValue& value);
    void pop(CLuaTable& value);
    void popPointer(void*& value);
    void popUserData(void*& pointer)
    {
        popPointer(pointer);
    }

    int popInt();
    bool popBool();
    float popFloat();
    double popDouble();
    const char* popString();
    const wchar_t* popWString();
    void* popPointer();
    CLuaValue popValue();
    CLuaTable popTable();

    // low level table functions
    void newTable();
    void setTable(int idx = LUAINDEX_GLOBALS);
    void getTable(int idx = LUAINDEX_GLOBALS);

    // get internal state
    lua_State* getCLuaState();
    int getStackTop();

protected:
    void handleError(int status);
    lua_State* m_pLuaState;
    LuaScriptErrorFunc m_pErrorFunc;
    lua_CFunction m_fnLoader;
};

class BCLIB_LUASCRIPT_API CLuaParamHelper : public CLuaState
{
public:
    CLuaParamHelper();
    CLuaParamHelper(lua_State* ls);

    void param(int value);
    void param(bool value);
    void param(float value);
    void param(double value);
    void param(const char* value);
    void param(const wchar_t* value);
    void param(const CLuaValue& value);
    void param(const CLuaTable& value);
    void paramPointer(void* value);

    // for pushing user datatype
    void param(void* pointer, const char* name);

public: // 专为兼容 引擎 和 华山 而添加的非驼峰命名函数
    inline void Param(int value)
    {
        param(value);
    }
    inline void Param(bool value)
    {
        param(value);
    }
    inline void Param(float value)
    {
        param(value);
    }
    inline void Param(double value)
    {
        param(value);
    }
    inline void Param(const char* value)
    {
        param(value);
    }
    inline void Param(const wchar_t* value)
    {
        param(value);
    }
    inline void Param(const CLuaValue& value)
    {
        param(value);
    }
    inline void Param(const CLuaTable& value)
    {
        param(value);
    }
    inline void ParamPointer(void* value)
    {
        paramPointer(value);
    }
    inline void Param(void* pointer, const char* name)
    {
        param(pointer, name);
    }

protected:
    int m_iNumParams;
};


class CLuaThread;

// 为在Lua文件中提供_loadPackage(*.pkg)功能服务的
struct SToLuaPackage
{
    const char* pcszFuncName;
    lua_CFunction pFunc;
};

class BCLIB_LUASCRIPT_API CLuaScript : public CLuaParamHelper
{
public:
    //CLuaScript();

    /// @brief
    /// @param fnLoader 替换Lua默认加载功能的函数，NULL为不替换
    /// @param
    CLuaScript(lua_CFunction fnLoader);
    virtual ~CLuaScript();

    void reset();

    /// @brief 执行的LUA文件将使用 fnLoader 去加载，该LUA文件可能存在于某种包文件中
    /// @param fname 是Lua文件名
    bool executeFileUseLoader(const char* fname);

    bool executeFile(const char* fname);
    bool executeString(const char* str);
    bool executeBuffer(const void* buf, int bufSize);

    bool isFunction(const char* name);
    bool call(const char* name);        // call script function, without parameter and return value

    void beginCall(const char* name);   // after BeginCall, use param() to set parameters, endCall() to get return value
    bool endCall();
    bool endCall(int& ret);
    bool endCall(bool& ret);
    bool endCall(float& ret);
    bool endCall(double& ret);
    bool endCall(std::string& ret);
    bool endCall(std::wstring& ret);
    bool endCall(CLuaTable& ret);
    bool endCall(void*& ret);
    bool endCall(CLuaValue& ret);//add by 吴财华 20111008

    void registerCFunction(const char* name, lua_CFunction f);
    void registerCLib(const char* name, const lua_CLib* lib);

    CLuaThread* createThread(const char* fnName);
    void deleteThread(CLuaThread* t);

    //static void appendRequirePath(const char* path);
    //static void removeRequirePath(const char* path);

    bool RegisterToLuaPackageList(SToLuaPackage pPackageList[], int count);

protected:
    void _replaceLoader();
    // 辅助读入包中lua文件
    void _getRequirePath(std::string& fname, std::string& requirePath, std::string& requireName);

    static int _include(lua_State* L); // include other script file
    static int _loadPackage(lua_State* L); // load tolua++ package

private:
    struct ltstr
    {
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp(s1, s2) < 0;
        }
    };
    typedef std::map<const char*, lua_CFunction, ltstr> Name2FuncMap;
    Name2FuncMap m_mapName2Func;
};

class BCLIB_LUASCRIPT_API CLuaThread : public CLuaParamHelper
{
public:
    CLuaThread(lua_State* ls);

    bool isDead();
    bool resume();

    void beginResume();     // after beginResume(), use param() to set parameters
    bool endResume();
};

class BCLIB_LUASCRIPT_API CLuaCall : public CLuaState
{
public:
    CLuaCall(lua_State* ls);

    int argCount();
    ELuaType argType(int idx);

    int argInt(int idx);
    bool argBool(int idx);
    float argFloat(int idx);
    double argDouble(int idx);
    const char* argString(int idx);
    const wchar_t* argWString(int idx);
    CLuaTable argTable(int idx);
    void* argPointer(int idx);
    std::string argAsString(int idx);

    int returnValue(int value);
    int returnValue(bool value);
    int returnValue(float value);
    int returnValue(double value);
    int returnValue(const char* value);
    int returnValue(const wchar_t* value);
    int returnValue(const CLuaTable& value);
    int returnPointerValue(void* value);

    int yieldThread();

    // generate a Lua error
    void error(const char* msg = 0);

protected:
    int m_iNumReturnValues;
};

class BCLIB_LUASCRIPT_API CLuaValue
{
public:
    friend class CLuaState;

    CLuaValue();
    ~CLuaValue();
    CLuaValue(int value);
    CLuaValue(bool value);
    CLuaValue(float value);
    CLuaValue(double value);
    CLuaValue(const char* value);
    CLuaValue(const wchar_t* value);
    CLuaValue(const CLuaValue& value);
    CLuaValue(const CLuaTable& value);

    int getInt() const;
    bool getBool() const;
    float getFloat() const;
    double getDouble() const;
    const char* getString() const;
    const wchar_t* getWString() const;
    CLuaTable& getTable() const;
    void* getPointer() const;

    ELuaType getType() const;

    bool operator<(const CLuaValue& r) const;
    CLuaValue& operator=(const CLuaValue& r);

private:

    void setNil();
    void set(int value);
    void set(bool value);
    void set(float value);
    void set(double value);
    void set(const char* value);
    void set(const wchar_t* value);
    void set(const CLuaValue& value);
    void set(const CLuaTable& value);
    void setPointer(void* value);

    void release();

    ELuaType m_eType;
    union
    {
        double m_dNumber;
        char* m_pString;
        CLuaTable* m_pTable;
        void* m_Pointer;
    };
};

inline CLuaState::CLuaState(lua_State* ls)
{
    m_pLuaState = ls;
}

inline int CLuaState::popInt()
{
    int value;
    pop(value);
    return value;
}

inline bool CLuaState::popBool()
{
    bool value;
    pop(value);
    return value;
}

inline float CLuaState::popFloat()
{
    float value;
    pop(value);
    return value;
}

inline double CLuaState::popDouble()
{
    double value;
    pop(value);
    return value;
}

inline void* CLuaState::popPointer()
{
    void* value;
    popPointer(value);
    return value;
}

inline lua_State* CLuaState::getCLuaState()
{
    return m_pLuaState;
}

inline CLuaParamHelper::CLuaParamHelper()
{
    m_iNumParams = 0;
}

inline CLuaParamHelper::CLuaParamHelper(lua_State* ls)
:CLuaState(ls)
{
    m_iNumParams = 0;
}

inline void CLuaParamHelper::param(int value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(bool value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(float value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(double value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(const char* value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(const wchar_t* value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(const CLuaValue& value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(const CLuaTable& value)
{
    push(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::paramPointer(void* value)
{
    pushPointer(value);
    m_iNumParams++;
}

inline void CLuaParamHelper::param(void* pointer, const char* name)
{
    pushUserData(pointer, name);
    ++m_iNumParams;
}

inline CLuaThread::CLuaThread(lua_State* ls)
:CLuaParamHelper(ls)
{
}

inline CLuaCall::CLuaCall(lua_State* ls)
:CLuaState(ls)
{
    m_iNumReturnValues = 0;
}

inline int CLuaCall::returnValue(int value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnValue(bool value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnValue(float value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnValue(double value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnValue(const char* value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnValue(const wchar_t* value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnValue(const CLuaTable& value)
{
    push(value);
    return ++m_iNumReturnValues;
}

inline int CLuaCall::returnPointerValue(void* value)
{
    pushPointer(value);
    return ++m_iNumReturnValues;
}

inline CLuaValue::CLuaValue()
{
    m_eType = LUATYPE_NIL;
    m_dNumber = 0;
}

inline CLuaValue::~CLuaValue()
{
    release();
}

inline CLuaValue::CLuaValue(int value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(bool value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(float value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(double value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(const char* value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(const wchar_t* value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(const CLuaValue& value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline CLuaValue::CLuaValue(const CLuaTable& value)
{
    m_eType = LUATYPE_NIL;
    set(value);
}

inline int CLuaValue::getInt() const
{
    if(m_eType == LUATYPE_NUMBER)
    {
        return (int)m_dNumber;
    }
    else
    {
        return 0;
    }
}

inline bool CLuaValue::getBool() const
{
    if(m_eType == LUATYPE_BOOLEAN)
    {
        return m_dNumber != 0;
    }
    else if(m_eType == LUATYPE_NIL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

inline float CLuaValue::getFloat() const
{
    if(m_eType == LUATYPE_NUMBER)
    {
        return (float)m_dNumber;
    }
    else
    {
        return 0;
    }
}

inline const char* CLuaValue::getString() const
{
    if(m_eType == LUATYPE_STRING)
    {
        return m_pString;
    }
    else
    {
        return "";
    }
}

inline const wchar_t* CLuaValue::getWString() const
{
    if(m_eType == LUATYPE_STRING)
    {
        return (const wchar_t*)m_pString;
    }
    else
    {
        return L"";
    }
}

inline double CLuaValue::getDouble() const
{
    if(m_eType == LUATYPE_NUMBER)
    {
        return m_dNumber;
    }
    else
    {
        return 0;
    }
}

inline void* CLuaValue::getPointer() const
{
    if(m_eType == LUATYPE_LIGHTUSERDATA)
    {
        return m_Pointer;
    }
    else
    {
        return 0;
    }
}

inline void CLuaValue::setNil()
{
    release();
    m_eType = LUATYPE_NIL;
    m_dNumber = 0;
}

inline void CLuaValue::set(int value)
{
    release();
    m_eType = LUATYPE_NUMBER;
    m_dNumber = value;
}

inline void CLuaValue::set(bool value)
{
    release();
    m_eType = LUATYPE_BOOLEAN;
    if(value)
    {
        m_dNumber = 1;
    }
    else
    {
        m_dNumber = 0;
    }
}

inline void CLuaValue::set(float value)
{
    release();
    m_eType = LUATYPE_NUMBER;
    m_dNumber = value;
}

inline void CLuaValue::set(double value)
{
    release();
    m_eType = LUATYPE_NUMBER;
    m_dNumber = value;
}

inline void CLuaValue::setPointer(void* value)
{
    release();
    m_eType = LUATYPE_LIGHTUSERDATA;
    m_Pointer = value;
}

inline ELuaType CLuaValue::getType() const
{
    return m_eType;
}

inline CLuaValue& CLuaValue::operator=(const CLuaValue& r)
{
    set(r);
    return *this;
}
}//LuaScript
}//BCLib

#endif//__BCLIB_LUASCRIPT_LUASCRIPT_H__
