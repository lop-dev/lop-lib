//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/msgFreq.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_MSGFREQ_H__
#define __BCLIB_FRAMEWORK_MSGFREQ_H__

#include <BCLib/framework/baseDef.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/framework/message.h>

namespace BCLib
{
namespace Framework
{

class CRecord;

//////////////////////////////////////////////////////////////////////////
/// @brief 检查消息接收频率，所有时间单位是毫秒
//////////////////////////////////////////////////////////////////////////
class BCLIB_FRAMEWORK_API CMsgFreq
{
    BCLIB_SINGLETON_DECLARE(CMsgFreq);

protected:
    CMsgFreq();
    virtual ~CMsgFreq();

public:
	/// @brief 设置消息接收的最高频率，即每秒最多收到几次
	/// @return void
	/// @param uniqueID 消息的编号
	/// @param uFreq 接收频率
    void setFreq(uint32 uniqueID, uint16 uFreq);
    uint16 getFreq(uint32 uniqueID);

	/// @brief 清空之前的记录
	/// @return void
	/// @param peerID 连接表示（可以是Socket数值、逻辑上的Peer等）
	void clearStat(uint64 peerID);

	/// @brief 判断并记录接收到的消息
	/// @return bool 为true表示有效
	/// @param uniqueID 消息的编号（程序内部会使用系统的当前时间）
	bool isValid(uint64 peerID, uint32 uniqueID);

	/// @brief 判断并记录接收到的消息
	/// @return bool 为true表示有效
	/// @param uniqueID 消息的编号
	/// @param curTime 当前时间（在某些情况下，让上层传入，有助于提高效率）
	bool isValid(uint64 peerID, uint32 uniqueID, uint64 curTime);

private:
    BCLib::Utility::CHashMap<uint32, uint16> m_FreqMap;
	BCLib::Utility::CHashMap<uint64, CRecord*> m_StatMap;
};

}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_MSGFREQ_H__
