//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SFLib/logicServer/variableGear.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SFLIB_LOGICSERVER_VARIABLEGEAR_H__
#define __SFLIB_LOGICSERVER_VARIABLEGEAR_H__

#include <SFLib/commonDefine/baseDef.h>

namespace SFLib
{
namespace Logic
{
class SFLIB_LOGIC_API CVariableGear
{
public:
	CVariableGear();
	virtual ~CVariableGear();

    /// @brief 设置出错阈值，默认是30%
    /// @return void
    /// @param fPercentError 0.1~1.0
    void setPercentError(float fPercentError);
    float getPercentError() { return m_fPercentError; }

    /// @brief 设置周期时间，默认是10分钟，可设置范围是5~60分钟
    /// @return void
    /// @param uCycleTime 毫秒
	void setCycleTime(BCLib::uint32 uCycleTime);
    BCLib::uint32 getCycleTime(){return m_uCycleTime;}

	bool isValid(BCLib::uint64 cltTime);
	bool isValid(BCLib::uint64 cltTime, BCLib::uint64 srvTime);

private:
    float m_fPercentError;
    BCLib::uint32 m_uCycleTime;
	std::map<BCLib::uint64, BCLib::uint64> m_TimeMap;
};
}//Logic
}//SFLib

#endif//__SFLIB_LOGICSERVER_VARIABLEGEAR_H__
