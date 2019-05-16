//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/LuaScript.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_LUASCRIPT_H__
#define __GELIB_UTILITY_LUASCRIPT_H__

#include <string>
#include <map>

struct lua_State;
typedef int (*lua_CFunction) (lua_State *L);

struct lua_CLib
{
	const char *name;
	lua_CFunction func;
};

namespace GELib
{


// Lua value type
enum ELuaType
{
	LUATYPE_NONE = -1,
	LUATYPE_NIL = 0,
	LUATYPE_BOOLEAN = 1,
	LUATYPE_LIGHTUSERDATA = 2,
	LUATYPE_NUMBER = 3,
	LUATYPE_STRING = 4,
	LUATYPE_TABLE = 5,
	LUATYPE_FUNCTION = 6,
	LUATYPE_USERDATA = 7,
	LUATYPE_THREAD = 8
};

// Lua error code
const int LUAERR_RUN = 1;
const int LUAERR_FILE = 2;
const int LUAERR_SYNTAX	= 3;
const int LUAERR_MEM = 4;
const int LUAERR_ERR = 5;

// pseudo table index
const int LUAINDEX_REGISTRY = -10000;
const int LUAINDEX_ENVIRON  = -10001;
const int LUAINDEX_GLOBALS  = -10002;


class CLuaValue;
typedef std::map<CLuaValue, CLuaValue> CLuaTable;

class CLuaState;


// global error handler
typedef void (*LuaScriptErrorFunc) (CLuaState *luaState, int err, const char *msg);
void LuaScriptSetErrorFunction(LuaScriptErrorFunc func);

class CLuaState
{
public:
	CLuaState();
	CLuaState(lua_State *ls);
	virtual ~CLuaState();

    // set error handler, global handler is used if not set
    void SetErrorFunction(LuaScriptErrorFunc func);

	// set global variable, add new one if not exist
	void SetGlobal(const char *name, int value);
	void SetGlobal(const char *name, bool value);
	void SetGlobal(const char *name, float value);
	void SetGlobal(const char *name, double value);
	void SetGlobal(const char *name, const char *value);
	void SetGlobal(const char *name, const wchar_t *value);
	void SetGlobal(const char *name, const CLuaTable &value);
	void SetGlobalPointer(const char *name, void *value);
	void SetGlobalNil(const char *name);

	// get global variable with type checking
	bool GetGlobal(const char *name, int &value);
	bool GetGlobal(const char *name, bool &value);
	bool GetGlobal(const char *name, float &value);
	bool GetGlobal(const char *name, double &value);
	bool GetGlobal(const char *name, std::string &value);
	bool GetGlobal(const char *name, std::wstring &value);
	bool GetGlobal(const char *name, CLuaTable &value);
	bool GetGlobalPointer(const char *name, void *&value);
	bool IsGlobalNil(const char *name);

	// stack operations
	void PushNil();
	void Push(int value);
	void Push(bool value);
	void Push(float value);
	void Push(double value);
	void Push(const char *value);
	void Push(const wchar_t *value);
	void Push(const CLuaValue &value);
	void Push(const CLuaTable &value);
	void PushPointer(void *value);

	void Pop();
	void Pop(int &value);
	void Pop(bool &value);
	void Pop(float &value);
	void Pop(double &value);
	void Pop(std::string &value);
	void Pop(std::wstring &value);
	void Pop(CLuaValue &value);
	void Pop(CLuaTable &value);
	void PopPointer(void *&value);

	int PopInt();
	bool PopBool();
	float PopFloat();
	double PopDouble();
	const char* PopString();
	const wchar_t* PopWString();
	void* PopPointer();
	CLuaValue PopValue();
	CLuaTable PopTable();

	// low level table functions
	void NewTable();
	void SetTable(int idx=LUAINDEX_GLOBALS);
	void GetTable(int idx=LUAINDEX_GLOBALS);

	// get internal state
	lua_State* GetCLuaState();
	int GetStackTop();

protected:
	void HandleError(int status);
	lua_State *m_pLuaState;
    LuaScriptErrorFunc m_pErrorFunc;
};

class CLuaParamHelper : public CLuaState
{
public:
	CLuaParamHelper();
	CLuaParamHelper(lua_State *ls);

	void Param(int value);
	void Param(bool value);
	void Param(float value);
	void Param(double value);
	void Param(const char *value);
	void Param(const wchar_t *value);
	void Param(const CLuaValue &value);
	void Param(const CLuaTable &value);
	void ParamPointer(void *value);

protected:
	int m_iNumParams;
};


class CLuaThread;

class CLuaScript : public CLuaParamHelper
{
public:
	CLuaScript();
	virtual ~CLuaScript();

	void Reset();

	bool ExecuteFile(const char *fname);
	bool ExecuteString(const char *str);
	bool ExecuteBuffer(const void *buf, int bufSize);

	bool IsFunction(const char *name);
	bool Call(const char *name);        // call script function, without parameter and return value

	void BeginCall(const char *name);   // after BeginCall, use Param() to set parameters, EndCall() to get return value
	bool EndCall();
	bool EndCall(int &ret);
	bool EndCall(bool &ret);
	bool EndCall(float &ret);
	bool EndCall(double &ret);
	bool EndCall(std::string &ret);
	bool EndCall(std::wstring &ret);
	bool EndCall(CLuaTable &ret);
	bool EndCall(void *&ret);

	void RegisterCFunction(const char *name, lua_CFunction f);
	void RegisterCLib(const char *name, const lua_CLib *lib);

	CLuaThread* CreateThread(const char *fnName);
	void DeleteThread(CLuaThread *t);

private:
};

class CLuaThread : public CLuaParamHelper
{
public:
	CLuaThread(lua_State *ls);

	bool IsDead();
	bool Resume();

	void BeginResume();     // after BeginResume(), use Param() to set parameters
	bool EndResume();
};

class CLuaCall : public CLuaState
{
public:
	CLuaCall(lua_State *ls);

	int ArgCount();
	ELuaType ArgType(int idx);

	int ArgInt(int idx);
	bool ArgBool(int idx);
	float ArgFloat(int idx);
	double ArgDouble(int idx);
	const char * ArgString(int idx);
	const wchar_t * ArgWString(int idx);
	CLuaTable ArgTable(int idx);
	void* ArgPointer(int idx);
	std::string ArgAsString(int idx);

	int ReturnValue(int value);
	int ReturnValue(bool value);
	int ReturnValue(float value);
	int ReturnValue(double value);
	int ReturnValue(const char *value);
	int ReturnValue(const wchar_t *value);
	int ReturnValue(const CLuaTable &value);
	int ReturnPointerValue(void *value);

	int YieldThread();

	// generate a Lua error
	void Error(const char *msg=0);

protected:
	int m_iNumReturnValues;
};

class CLuaValue
{
public:
	friend class CLuaState;

	CLuaValue();
	~CLuaValue();
	CLuaValue(int value);
	CLuaValue(bool value);
	CLuaValue(float value);
	CLuaValue(double value);
	CLuaValue(const char *value);
	CLuaValue(const wchar_t *value);
	CLuaValue(const CLuaValue &value);
	CLuaValue(const CLuaTable &value);

	int GetInt() const;
	bool GetBool() const;
	float GetFloat() const;
	double GetDouble() const;
	const char * GetString() const;
	const wchar_t * GetWString() const;
	CLuaTable& GetTable() const;
	void * GetPointer() const;

	ELuaType GetType() const;

	bool operator<(const CLuaValue &r) const;
	CLuaValue& operator=(const CLuaValue &r);

private:

	void SetNil();
	void Set(int value);
	void Set(bool value);
	void Set(float value);
	void Set(double value);
	void Set(const char *value);
	void Set(const wchar_t *value);
	void Set(const CLuaValue &value);
	void Set(const CLuaTable &value);
	void SetPointer(void *value);

	void Release();

	ELuaType m_eType;
	union
	{
		double m_dNumber;
		char *m_pString;
		CLuaTable *m_pTable;
		void *m_Pointer;
	};
};

inline CLuaState::CLuaState(lua_State *ls)
{
	m_pLuaState = ls;
}

inline int CLuaState::PopInt()
{
	int value;
	Pop(value);
	return value;
}

inline bool CLuaState::PopBool()
{
	bool value;
	Pop(value);
	return value;
}

inline float CLuaState::PopFloat()
{
	float value;
	Pop(value);
	return value;
}

inline double CLuaState::PopDouble()
{
	double value;
	Pop(value);
	return value;
}

inline void* CLuaState::PopPointer()
{
	void *value;
	PopPointer(value);
	return value;
}

inline lua_State* CLuaState::GetCLuaState()
{
	return m_pLuaState;
}

inline CLuaParamHelper::CLuaParamHelper()
{
	m_iNumParams = 0;
}

inline CLuaParamHelper::CLuaParamHelper(lua_State *ls) : CLuaState(ls)
{
	m_iNumParams = 0;
}

inline void CLuaParamHelper::Param(int value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(bool value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(float value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(double value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(const char *value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(const wchar_t *value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(const CLuaValue &value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::Param(const CLuaTable &value)
{
	Push(value);
	m_iNumParams++;
}

inline void CLuaParamHelper::ParamPointer(void *value)
{
	PushPointer(value);
	m_iNumParams++;
}


inline CLuaThread::CLuaThread(lua_State *ls) : CLuaParamHelper(ls)
{
}

inline CLuaCall::CLuaCall(lua_State *ls) : CLuaState(ls)
{
	m_iNumReturnValues = 0;
}

inline int CLuaCall::ReturnValue(int value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnValue(bool value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnValue(float value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnValue(double value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnValue(const char *value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnValue(const wchar_t *value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnValue(const CLuaTable &value)
{
	Push(value);
	return ++m_iNumReturnValues;
}

inline int CLuaCall::ReturnPointerValue(void *value)
{
	PushPointer(value);
	return ++m_iNumReturnValues;
}

inline CLuaValue::CLuaValue()
{
	m_eType = LUATYPE_NIL;
	m_dNumber = 0;
}

inline CLuaValue::~CLuaValue()
{
	Release();
}

inline CLuaValue::CLuaValue(int value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(bool value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(float value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(double value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(const char *value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(const wchar_t *value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(const CLuaValue &value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline CLuaValue::CLuaValue(const CLuaTable &value)
{
	m_eType = LUATYPE_NIL;
	Set(value);
}

inline int CLuaValue::GetInt() const
{
	if (m_eType == LUATYPE_NUMBER)
		return (int)m_dNumber;
	else
		return 0;
}

inline bool CLuaValue::GetBool() const
{
	if (m_eType == LUATYPE_BOOLEAN)
		return m_dNumber != 0;
	else if (m_eType == LUATYPE_NIL)
		return false;
	else
		return true;
}

inline float CLuaValue::GetFloat() const
{
	if (m_eType == LUATYPE_NUMBER)
		return (float)m_dNumber;
	else
		return 0;
}

inline const char * CLuaValue::GetString() const
{
	if (m_eType == LUATYPE_STRING)
		return m_pString;
	else
		return "";
}

inline const wchar_t * CLuaValue::GetWString() const
{
	if (m_eType == LUATYPE_STRING)
		return (const wchar_t *)m_pString;
	else
		return L"";
}

inline double CLuaValue::GetDouble() const
{
	if (m_eType == LUATYPE_NUMBER)
		return m_dNumber;
	else
		return 0;
}

inline void * CLuaValue::GetPointer() const
{
	if (m_eType == LUATYPE_LIGHTUSERDATA)
		return m_Pointer;
	else
		return 0;
}

inline void CLuaValue::SetNil()
{
	Release();
	m_eType = LUATYPE_NIL;
	m_dNumber = 0;
}

inline void CLuaValue::Set(int value)
{
	Release();
	m_eType = LUATYPE_NUMBER;
	m_dNumber = value;
}

inline void CLuaValue::Set(bool value)
{
	Release();
	m_eType = LUATYPE_BOOLEAN;
	if (value)
		m_dNumber = 1;
	else
		m_dNumber = 0;
}

inline void CLuaValue::Set(float value)
{
	Release();
	m_eType = LUATYPE_NUMBER;
	m_dNumber = value;
}

inline void CLuaValue::Set(double value)
{
	Release();
	m_eType = LUATYPE_NUMBER;
	m_dNumber = value;
}

inline void CLuaValue::SetPointer(void *value)
{
	Release();
	m_eType = LUATYPE_LIGHTUSERDATA;
	m_Pointer = value;
}

inline ELuaType CLuaValue::GetType() const
{
	return m_eType;
}

inline CLuaValue& CLuaValue::operator=(const CLuaValue &r)
{
	Set(r);
	return *this;
}



}//GELib

#endif//__GELIB_UTILITY_LUASCRIPT_H__
