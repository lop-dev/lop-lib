//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/algorithm/variableGear.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_VARIABLEGEAR_H__
#define __BCLIB_UTILITY_IO_VARIABLEGEAR_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/dateTime.h>

namespace BCLib
{
namespace Utility
{

//////////////////////////////////////////////////////////////////////////
/// @brief 检查变速齿轮，所有时间单位是毫秒
//////////////////////////////////////////////////////////////////////////
class CVariableGear
{
public:
	CVariableGear();
	virtual ~CVariableGear();

	/// @brief 用于判断的计算周期
	/// @return void
	/// @param fPercentError 必须是小于60分钟大于10分钟的数值
	void setCycleTime(uint32 uCycleTime);
	uint32 getCycleTime(){return m_uCycleTime;}

	/// @brief 设置用于判断的百分误差
	/// @return void
	/// @param fPercentError 必须是小于1大于0.1的数值
	void setPercentError(float fPercentError);
	float getPercentError(){return m_fPercentError;}

	/// @brief 判断客户端是否使用了变速齿轮
	/// @return bool 为 false 时，表示使用了变速齿轮
	/// @param cltTime 客户端传过来的时间
	bool isValid(uint64 cltTime);

	/// @brief 判断客户端是否使用了变速齿轮
	/// @return bool 为 false 时，表示使用了变速齿轮
	/// @param cltTime 客户端传过来的时间
	/// @param srvTime 服务器自获取的时间（在某些情况下，让上层传入，有助于提高效率）
	bool isValid(uint64 cltTime, uint64 srvTime);

private:
	uint32 m_uCycleTime;   // 用于判断的计算周期
	float m_fPercentError; // 用于判断的百分误差
	CHashMap<uint64, uint64> m_TimeMap; // <客户端时间, 服务器时间>
};

}//Utility
}//BCLib


#endif//__BCLIB_UTILITY_IO_VARIABLEGEAR_H__
