//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/FileSystem.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_FILESYSTEM_H__
#define __GELIB_UTILITY_FILESYSTEM_H__

#include <list>
#include <vector>
#include <string>
#include "SystemUtility.h"
#include "DataBuffer.h"

namespace GELib
{


class CFile;
class CPackFile;


class CFileSystem
{
public:
    CFileSystem();
    virtual ~CFileSystem();

    bool Initialize();
    void Uninitialize();

    // dir can be a directory or pack file
    bool Mount(const char *dir, const char *mountPoint=0, bool appendEnd=true);
    bool Unmount(const char *dir);

    CFile* OpenFileRead(const char *fname);
    CFile* OpenFileWrite(const char *fname);
    void CloseFile(CFile *f);

    bool LoadFileToBuffer(const char *fname, CDataBuffer &buf);
	bool LoadFileToString(const char *fname, std::string &str);

    std::string MakeFullPath(const char *fname);
    std::string MakeFullPath(const std::string &fname);

	void FindFiles(const char *fileSpec, std::vector<std::string> &files);

    void EnableOpenFileLog(bool enable) { m_bEnableOpenFileLog = enable; }
    void ClearOpenFileLog() { m_OpenFileLog.clear(); }
    const std::vector<std::string> & GetOpenFileLog() { return m_OpenFileLog; }

private:

    class CSearchPath
    {
    public:
        CSearchPath() { m_pPackFile = 0; }
        std::string MakeFullPath(const char *fname);
        std::string m_Directory;
        std::string m_MountPoint;
        CPackFile *m_pPackFile;
    };

    typedef std::list<CSearchPath> SearchPathList;
    SearchPathList m_SearchPathes;

    bool m_bEnableOpenFileLog;
    std::vector<std::string> m_OpenFileLog;

    CCriticalSectionObject m_CS;
};


void SetFileSystem(CFileSystem *fileSystem);
CFileSystem* GetFileSystem();



}//GELib

#endif//__GELIB_UTILITY_FILESYSTEM_H__
