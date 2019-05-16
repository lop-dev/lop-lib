//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/systemInfo.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_SYSTEMINFO_H__
#define __BCLIB_UTILITY_SYSTEMINFO_H__

#include <BCLib/utility/singleton.h>

namespace BCLib
{
namespace Utility
{

class BCLIB_UTILITY_API CSystemInfo
{
    BCLIB_SINGLETON_DECLARE(CSystemInfo);
    CSystemInfo();
    ~CSystemInfo();
public:
    struct SVideoCardInfo
    {
        SVideoCardInfo() : m_fVideoMemSizeInGiga(0) {}
        std::string     m_driverDesc;
        std::string     m_driverDate;
        std::string     m_driverVersion;
        float           m_fVideoMemSizeInGiga;
    };
    typedef std::vector <SVideoCardInfo> TVideoCardInfos;

public:
    // get all system info. call it before get any system info
    void retrieveSystemInfo();

    // all systme infos
    const std::string& getCpuBriefInfo() { return m_cpuBriefInfo; }
    BCLib::uint8 getNumOfProcessor() { return m_uNumberOfProcessor; }
    float getPhyMemTotalSizeInGiga() { return m_fPhyMemSizeInG; }
    const std::string& getOSBriefInfo() { return m_osBriefInfo; }
    const TVideoCardInfos& getVideoCardInfos() { return m_videoCardInfos; }

private:
    // get cpu info
    void _determineCpuInfo();
    void _determineCpuNameString(void);

    // get memory info
    void _determineMemoryInfo();

    // get operating system brief info
    void _determineOSInfo();

    // get video card info
    void _determineVideoCardInfo();

    // cpu
    std::string     m_cpuBriefInfo;         // vendor, etc.
    BCLib::uint8    m_uNumberOfProcessor;
    // memory
    float           m_fPhyMemSizeInG;       // in G
    // os
    std::string     m_osBriefInfo;

    // display
    TVideoCardInfos m_videoCardInfos;
};

}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_SYSTEMINFO_H__
