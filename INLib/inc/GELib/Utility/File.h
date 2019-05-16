//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/File.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_FILE_H__
#define __GELIB_UTILITY_FILE_H__

#include "IOStream.h"

namespace GELib
{

class CFileSystem;
class CPackFile;

class CFile : public CIOStream
{
public:

    friend class CFileSystem;

    CFile(CFileSystem *fileSystem);
    virtual ~CFile();

    virtual int Read(void *buffer, int size);
    virtual int Write(const void *buffer, int size);

    int GetSize();

    CFileSystem* GetFileSystem() { return m_pFileSystem; }

private:

    CPackFile *m_pPackFile;
    int m_iPackFileEntryIndex;
    FileHandle *m_hFile;
    CFileSystem *m_pFileSystem;
};


}//GELib

#endif//__GELIB_UTILITY_FILE_H__
