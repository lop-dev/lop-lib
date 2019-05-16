//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ProgressCallback.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_PROGRESSCALLBACK_H__
#define __GELIB_UTILITY_PROGRESSCALLBACK_H__

#include <vector>
#include <string>
#include <cassert>

namespace GELib
{


class CProgressCallback
{
public:
    CProgressCallback() {}
    virtual ~CProgressCallback() {}

    class Param
    {
    public:
        Param() { m_fProgress = 0; }
        std::string m_Type;
        std::string m_Name;
		float m_fProgress;		// 0 ~ 1
    };

    virtual void Begin(const Param &param) {}
    virtual void Progress(const Param &param) {}
    virtual void End(const Param &param) {}
};


void SetProgressCallback(CProgressCallback *p);
CProgressCallback* GetProgressCallback();

class CProgressScope
{
public:
    CProgressScope(const CProgressCallback::Param &param) : m_Param(param)
    {
        GetProgressCallback()->Begin(m_Param);
    }

    virtual ~CProgressScope()
    {
        GetProgressCallback()->End(m_Param);
    }

	CProgressScope& operator=(const CProgressScope &r) { assert(false); }	// remove warning C4512

    const CProgressCallback::Param &m_Param;
};


}//GELib

#endif//__GELIB_UTILITY_PROGRESSCALLBACK_H__
