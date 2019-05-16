//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   BCLib\utility\difFile.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_DIFFILE_H__
#define __CLLIB_UTILITY_DIFFILE_H__

namespace BCLib
{
namespace Utility
{
    class CDifFile;
}
}

namespace CLLib
{
namespace Utility
{

public ref class CDifFile
{
public:
    CDifFile();
    virtual ~CDifFile();

    bool genDifFile(const char *oldFileName, const char *newFileName, const char *outFileName, bool bInMem);
    bool genNewFile(const char *oldFileName, const char *difFileName, const char *outFileName, bool bInMem);

protected:
    BCLib::Utility::CDifFile* m_difFile;
};

}//Utility

}//CLLib

#endif//__CLLIB_UTILITY_DIFFILE_H__