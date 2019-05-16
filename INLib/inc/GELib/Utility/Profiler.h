//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/Profiler.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_PROFILER_H__
#define __GELIB_UTILITY_PROFILER_H__

#include <vector>

namespace GELib
{


#define ENABLE_GELIB_PROFILE
#ifdef  ENABLE_GELIB_PROFILE
    #define GELIB_PROFILE(name)  static short _CategoryIndex_##name = GELib::_ProfileCategory(#name); \
                           GELib::CProfileSampler _ProfileSampler_##name(_CategoryIndex_##name);

    #define GELIB_PROFILE_BEGIN(name)  static short _CategoryIndex_##name = GELib::_ProfileCategory(#name); \
                                 int _SampleIndex_##name = GELib::_ProfileSampleBegin(_CategoryIndex_##name);

    #define GELIB_PROFILE_END(name)    GELib::_ProfileSampleEnd(_SampleIndex_##name);
#else
    #define GELIB_PROFILE(name)
    #define GELIB_PROFILE_BEGIN(name)
    #define GELIB_PROFILE_END(name)
#endif


class CProfileCategory
{
public:
    const char *m_pName;
    double m_fTime;
    int m_iCount;
    int m_iFirstSampleIndex;
    int m_iLastSampleIndex;
    int m_iUserData;
};

class CProfileSample
{
public:
    double m_fBeginTime;
    double m_fEndTime;
    unsigned int m_iThreadId;
    short m_iDepth;
    short m_iCategoryIndex;
};

typedef std::vector<CProfileCategory> ProfileCategories;
typedef std::vector<CProfileSample> ProfileSamples;

ProfileCategories& GetProfileCategories();
ProfileSamples& GetProfileSamples();
void ClearProfileSamples();

short _ProfileCategory(const char *name);
int  _ProfileSampleBegin(short categoryIndex);
void _ProfileSampleEnd(int sampleIndex);

class CProfileSampler
{
public:
	CProfileSampler(short categoryIndex) { m_iSampleIndex = _ProfileSampleBegin(categoryIndex); }
	~CProfileSampler() { _ProfileSampleEnd(m_iSampleIndex); }
    int m_iSampleIndex;
};



}//GELib


#endif//__GELIB_UTILITY_PROFILER_H__
