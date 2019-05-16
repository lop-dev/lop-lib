//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/dateTime.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_DATETIME_H__
#define __BCLIB_UTILITY_DATETIME_H__

#include <BCLib/utility/baseDef.h>
#include <time.h>
#include <chrono>
#ifndef _WIN32
#include <sys/time.h>
#else
#include <Winbase.h>
#include <time.h>
#define localtime_r(t, tm) localtime_s(tm, t)
#endif

namespace BCLib
{
namespace Utility
{
/// @brief 得到当前时间的毫秒值
extern int64 getMilliseconds();

//////////////////////////////////////////////////////////////////////////
/// @brief 封装基本的时间类，精确到秒，最常用的时间类
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CDateTime
{
public:
    enum EWeekday
    {
        ESUNDAY,
        EMONDAY,
        ETUESDAY,
        EWEDNESDAY,
        ETHURSDAY,
        EFRIDAY,
        ESATURDAY,
    };

    CDateTime();
    CDateTime(time_t t);
    CDateTime(BCLib::uint32 time);
    CDateTime(const CDateTime& t);
    CDateTime(int year, int month, int day, int hour, int minute, int second);
    CDateTime(std::string strDateTime); // 参数类似这样的格式：“2012-08-01 11:11:11”
    ~CDateTime();

    static CDateTime now();
    bool setSystemTime();

    void setTime(time_t timSec);
    void setTime(BCLib::uint32 time);
    void setTime(int year, int month, int day, int hour, int minute, int second);
    void setTime(std::string strDateTime); // 参数类似这样的格式：“2012-08-01 11:11:11”

    time_t getTime() const;  // the seconds elapsed from 1970
    void getTime(int& year, int& month, int& day, int& hour, int& minute, int& second) const;
    //BCLib::uint32 getNumber() const;

    uint32 getYear() const;
    uint32 getMonth() const;
    uint32 getDay() const;
    uint32 getHour() const;
    uint32 getMinute() const;
    uint32 getSecond() const;

    EWeekday getWeekday() const;

    std::string toString() const;
    std::string toString(const char* pszFormat) const;

    CDateTime& operator=(const CDateTime& t);
    bool operator ==(const CDateTime& rhs) const;
    bool operator !=(const CDateTime& rhs) const;
    bool operator <(const CDateTime& rhs) const;
    bool operator <=(const CDateTime& rhs) const;
    bool operator >(const CDateTime& rhs) const;
    bool operator >=(const CDateTime& rhs) const;

    /// @brief 当前时间增加nYear年后的时间，nYear可以是负数
    CDateTime addYear(BCLib::int16 nYear);
    /// @brief 当前时间增加nMonth月后的时间，nMonth可以是负数
    CDateTime addMonth(BCLib::int16 nMonth);
    /// @brief 当前时间增加nDay天后的时间，nDay可以是负数
    CDateTime addDay(BCLib::int16 nDay);
    /// @brief 当前时间增加nHour时后的时间，nHour可以是负数
    CDateTime addHour(BCLib::int16 nHour);
    /// @brief 当前时间增加nSecond分后的时间，nSecond可以是负数
    CDateTime addMinute(BCLib::int16 nMinute);
    /// @brief 当前时间增加nSecond秒后的时间，nSecond可以是负数
    CDateTime addSecond(BCLib::int16 nSecond);

    /// @brief 计算两个时间相差的天、时、分、秒，time 可以比当前时间大，也可以比当前时间小，总是返回正数
    void subTime(const CDateTime& time, BCLib::int32& day, BCLib::int32& hour, BCLib::int32& minute, BCLib::int32& second) const;
    /// @brief 计算两个时间相差的秒，time 可以比当前时间大，也可以比当前时间小，总是返回正数
    void subTime(const CDateTime& time, BCLib::int32& second) const;

public:
    //static uint32 getMilliseconds(); // the milliseconds elapsed from compute started

    static bool isExpired(int year, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0);
    static bool isExpired(CDateTime dtExpired);
    static bool setSystemTime(int year, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0);

    /// @brief 计算start之后num分钟后的时间
    /// @return uint32 时间戳
    /// @param num
    /// @param start
    static uint32 sharpMinute(int num, uint32 start = now().getTime());

    /// @brief 计算start之后num小时后的时间
    /// @return uint32 时间戳
    /// @param num
    /// @param start
    static uint32 sharpHour(int num, uint32 start = now().getTime());

    /// @brief 计算start之后num天后的时间
    /// @return uint32 时间戳
    /// @param num
    /// @param start
    static uint32 sharpDay(int num = 0, uint32 start = now().getTime());

    /// @brief 计算start之后num周后的时间
    /// @return uint32 时间戳
    /// @param num
    /// @param start
    static uint32 sharpWeek(int num = 0, uint32 start = now().getTime());

    /// @brief 计算start之后num月后的时间
    /// @return uint32 时间戳
    /// @param num
    /// @param start
    static uint32 sharpMonth(int num = 0, uint32 start = now().getTime());

    /// @brief 计算start之后num年后的时间
    /// @return uint32 时间戳
    /// @param num
    /// @param start
    static uint32 sharpYear(int num = 0, uint32 start = now().getTime());

    /// @brief 判断两个时间戳是否为同一天
    /// @return bool
    /// @param time1
    /// @param time2
    static bool isSameDay(uint32 time1, uint32 time2);

private:
    time_t m_iSeconds;

    const static BCLib::uint32 ms_hour = 60;
    const static BCLib::uint32 ms_day = 24 * ms_hour;
    const static BCLib::uint32 ms_month = 31 * ms_day;
    const static BCLib::uint32 ms_year = 12 * ms_month;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 简单的日期类(4 bytes) 精度到分钟，很少用的时间类
/// @brief 能够表示[0000/00/00 00:00 - 8017/12/31 23:59]
/// @brief 能够计算两个日期之间相差的天数/小时/分钟
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CSimpleTime
{
public:
    CSimpleTime();
    CSimpleTime(time_t t);
    CSimpleTime(BCLib::uint32 timeMin);
    CSimpleTime(const CSimpleTime& date);
    CSimpleTime(BCLib::uint32 year, BCLib::uint32 month, BCLib::uint32 day, BCLib::uint32 hour, BCLib::uint32 minute);
    CSimpleTime(std::string strDateTime); // 参数类似这样的格式：“2012-08-01 11:11:11”
    CSimpleTime(const CDateTime& date);
    ~CSimpleTime();

    static CSimpleTime now();

    void setTime(time_t t);
    bool setTime(BCLib::uint32 timeMin);
    bool setTime(BCLib::uint32 year, BCLib::uint32 month, BCLib::uint32 day, BCLib::uint32 hour, BCLib::uint32 minute);
    void setTime(std::string strDateTime); // 参数类似这样的格式：“2012-08-01 11:11:11”

    BCLib::uint32 getTime() const;
    void getTime(BCLib::uint32& year, BCLib::uint32& month, BCLib::uint32& day, BCLib::uint32& hour, BCLib::uint32& minute) const;

    BCLib::uint32 getYear() const;
    BCLib::uint32 getMonth() const;
    BCLib::uint32 getDay() const;
    BCLib::uint32 getHour() const;
    BCLib::uint32 getMinute() const;

    std::string toString() const;
    std::string toString(const char* pszFormat) const;

    CSimpleTime& operator=(const CSimpleTime& date);
    CSimpleTime& operator=(const CDateTime& date);
    bool operator ==(const CSimpleTime& date) const;
    bool operator !=(const CSimpleTime& date) const;
    bool operator <(const CSimpleTime& date) const;
    bool operator <=(const CSimpleTime& date) const;
    bool operator >(const CSimpleTime& date) const;
    bool operator >=(const CSimpleTime& date) const;

    /// @brief 计算两个时间相差的天、时、分，time 可以比当前时间大，也可以比当前时间小，总是返回正数
    void subTime(const CSimpleTime& date, BCLib::int32& day, BCLib::int32& hour, BCLib::int32& minute) const;
    /// @brief 计算两个时间相差的秒，time 可以比当前时间大，也可以比当前时间小，总是返回正数
    void subTime(const CSimpleTime& date, BCLib::int32& minute) const;

private:
    BCLib::uint32 m_iMinutes;

    const static BCLib::uint32 ms_hour = 60;
    const static BCLib::uint32 ms_day = 24 * ms_hour;
    const static BCLib::uint32 ms_month = 31 * ms_day;
    const static BCLib::uint32 ms_year = 12 * ms_month;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 本地真实时间，精确到纳秒
//////////////////////////////////////////////////////////////////////////
template<class CLOCK_TYPE>
class CTemplateTime
{
public:
	typedef std::chrono::time_point<CLOCK_TYPE> TimePoint;
	typedef std::chrono::microseconds Microseconds;
	typedef std::chrono::milliseconds Milliseconds;
	typedef std::chrono::seconds Seconds;
	typedef std::chrono::minutes Minutes;
	typedef std::chrono::hours Hours;

	CTemplateTime(int delay = 0)
	{
		m_timePoint = CLOCK_TYPE::now();
        delayMilliseconds(delay);
	}

	void now()
	{
		m_timePoint = CLOCK_TYPE::now();
	}
	
    //纳秒
    int64 getMicroseconds() const
    {
        return durationCast<Microseconds>();
    }
	//毫秒
	int64 getMilliseconds() const
	{
		return durationCast<Milliseconds>();
	}
	//秒
	int64 getSeconds() const
	{
		return durationCast<Seconds>();
	}
    //分
	int64 getMinutes() const
	{
		return durationCast<Minutes>();
	}
    //时
	int64 getHours() const
	{
		return durationCast<Hours>();
	}

    //延长多少毫秒
	void delayMilliseconds(int delay)
	{
        if (delay > 0)
        {
            m_timePoint += Milliseconds(delay);
        }
	}

	CTemplateTime& operator =(const CTemplateTime& rt)
	{
		m_timePoint = rt.m_timePoint; return *this;
	}
	CTemplateTime& operator +(const CTemplateTime& rt)
	{
		m_timePoint += rt.m_timePoint.time_since_epoch();
		return *this;
	}
	CTemplateTime& operator -(const CTemplateTime& rt)
	{
		m_timePoint -= rt.m_timePoint.time_since_epoch();
		return *this;
	}

	bool operator >(const CTemplateTime& rt)
	{
		return m_timePoint > rt.m_timePoint;
	}
	bool operator <(const CTemplateTime& rt)
	{
		return m_timePoint < rt.m_timePoint;
	}
	bool operator >=(const CTemplateTime& rt)
	{
		return m_timePoint >= rt.m_timePoint;
	}
	bool operator <=(const CTemplateTime& rt)
	{
		return m_timePoint <= rt.m_timePoint;
	}
	bool operator ==(const CTemplateTime& rt)
	{
		return m_timePoint == rt.m_timePoint;
	}

private:
	template<typename DurationType>
	int64 durationCast() const
	{
		return std::chrono::duration_cast<DurationType>(m_timePoint.time_since_epoch()).count();
	}

private:
	TimePoint m_timePoint;
};

// 系统时间类
typedef CTemplateTime<std::chrono::system_clock> CSystemTime;
// 上次重启开始的时间 不可以被修改本地时间所影响
typedef CTemplateTime<std::chrono::steady_clock> CSteadyTime;
typedef CSteadyTime CRealTime; // 兼容之前的类名，等所有项目都替换后，再彻底去掉

//////////////////////////////////////////////////////////////////////////
/// @brief 本地真实时间，精确到毫秒
//////////////////////////////////////////////////////////////////////////
//class BCLIB_UTILITY_API CRealTime
//{
//public:
//    CRealTime(int delay = 0);
//
//    CRealTime(const CRealTime& rt)
//    {
//        m_milliseconds = rt.m_milliseconds;
//    }
//
//    /// @brief 设置当前的毫秒
//    /// @return void
//    void now();
//
//    //void setMilliSeconds(int64 ms)
//    //{
//    //    m_milliseconds = ms;
//    //}
//
//    //毫秒
//    int64 getMilliseconds() 
//    {
//        return m_milliseconds;
//    }
//
//    //秒
//    int64 getSeconds() 
//    {
//        return m_milliseconds / 1000;
//    }
//
//    void delayMilliseconds(int delay)
//    {
//        m_milliseconds += delay;
//    }
//
//    CRealTime& operator =(const CRealTime& rt)
//    {
//        m_milliseconds = rt.m_milliseconds; return *this;
//    }
//    CRealTime& operator +(const CRealTime& rt)
//    {
//        m_milliseconds += rt.m_milliseconds; return *this;
//    }
//    CRealTime& operator -(const CRealTime& rt)
//    {
//        m_milliseconds -= rt.m_milliseconds; return *this;
//    }
//
//    bool operator >(const CRealTime& rt)
//    {
//        return m_milliseconds > rt.m_milliseconds;
//    }
//    bool operator <(const CRealTime& rt)
//    {
//        return m_milliseconds < rt.m_milliseconds;
//    }
//    bool operator >=(const CRealTime& rt)
//    {
//        return m_milliseconds >= rt.m_milliseconds;
//    }
//    bool operator <=(const CRealTime& rt)
//    {
//        return m_milliseconds <= rt.m_milliseconds;
//    }
//    bool operator ==(const CRealTime& rt)
//    {
//        return m_milliseconds == rt.m_milliseconds;
//    }
//
//private:
//#if defined(_WIN32)
//    static int64 m_frequency;
//#endif
//    int64 m_milliseconds;
//};

//////////////////////////////////////////////////////////////////////////
/// @brief 延时定时器，轻量级的定时器，主动去判断是否超时
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CDelayTimer
{
public:
    CDelayTimer(int delay)
    :m_delay(delay)
    ,m_steadyTime(delay)
    {
    }

    CDelayTimer(int delay, const CSteadyTime& rt)
    :m_delay(delay)
    ,m_steadyTime(rt)
    {
        m_steadyTime.delayMilliseconds(delay);
    }

    /// @brief 重置定时器
    /// @return void
    /// @param delay 延迟时间
    /// @param ct 下一次超时的时间
    void reset(int delay, const CSteadyTime& ct)
    {
        m_delay = delay;
        m_steadyTime = ct;
        m_steadyTime.delayMilliseconds(delay);
    }

    /// @brief 更新下次超时时间
    /// @return void
    /// @param ct ct + m_delay为下次超时时间
    void next(const CSteadyTime& ct)
    {
        m_steadyTime = ct;
        m_steadyTime.delayMilliseconds(m_delay);
    }

    /// @brief 获取下次超时时间(millisecond)
    /// @return int64
    int64 getExpireTimeInMS()
    {
        return m_steadyTime.getMilliseconds();
    }

    /// @brief 判断是否超时
    /// @return bool
    /// @param ct 当前时间
    bool operator ()(const CSteadyTime& ct)
    {
        if(m_steadyTime <= ct)
        {
            m_steadyTime = ct;
            m_steadyTime.delayMilliseconds(m_delay);
            return true;
        }
        return false;
    }

private:
    int m_delay;
    //下次超时时间
    CSteadyTime m_steadyTime;
};

//void Sample()
//{
//    CSteadyTime steadyTime;
//    CDelayTimer delayTime(300);
//    while(true)
//    {
//        if(dalayTime(steadyTime)) {
//            //TODO
//        }
//        steadyTime.now();
//    };
//}

//////////////////////////////////////////////////////////////////////////
/// @brief 性能测试，计时器
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CSurveyTimer
{
public:
    CSurveyTimer();

    /// @brief 重置计时器
    /// @return void
    void reset();

    /// @brief 返回上次重置到现在经过的秒数
    /// @return BCLib::int64
    int64 seconds();

    /// @brief 返回上次重置到现在经过的毫秒数
    /// @return BCLib::int64
    int64 milliseconds();

    /// @brief 返回上次重置到现在经过的微妙数
    /// @return BCLib::int64
    int64 microseconds();

private:
#if defined(_WIN32)
    LARGE_INTEGER m_lastTime;
    static int64 m_frequency;
#elif defined(_LINUX)
    timespec m_lastTime;
#endif
};

//void Sample()
//{
//    BCLib::Utility::CSurveyTimer surveyTimer;
//    surveyTimer.reset();
//    //TODO
//    BCLib::int64 tempMs = surveyTimer.milliseconds();
//    BCLIB_LOG_TRACE(BCLib::ELOGMODULE_DEFAULT, "TODO花费的时间[%d]", tempMs);
//}

}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_DATETIME_H__
