//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/difFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_DIFFILE_H__
#define __BCLIB_UTILITY_IO_DIFFILE_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{

class BCLIB_UTILITY_API CDifFile
{
public:
    CDifFile();
    virtual ~CDifFile();

    bool genDifData(const void *oldData, int oldDataSize, const void *newData, int newDataSize, std::string &outData);
    bool genNewData(const void *oldData, int oldDataSize, const void *difData, int difDataSize, std::string &outData);

    bool genDifFile(const char *oldFileName, const char *newFileName, const char *outFileName, bool bInMem = false);
    bool genNewFile(const char *oldFileName, const char *difFileName, const char *outFileName, bool bInMem = false);

private:
    bool _genDifFile1(const char *oldFileName, const char *newFileName, const char *outFileName);
    bool _genNewFile1(const char *oldFileName, const char *difFileName, const char *outFileName);

    bool _genDifFile2(const char *oldFileName, const char *newFileName, const char *outFileName);
    bool _genNewFile2(const char *oldFileName, const char *difFileName, const char *outFileName);
};

}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_DIFFILE_H__
