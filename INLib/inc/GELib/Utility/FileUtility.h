//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/FileUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_FILEUTILITY_H__
#define __GELIB_UTILITY_FILEUTILITY_H__

#include <GELib/GeConfig.h>
#include "DataBuffer.h"

#include <vector>
#include <string>


namespace GELib
{

#ifdef CopyFile
    #undef CopyFile
#endif
#ifdef DeleteFile
    #undef DeleteFile
#endif

GELIB_API void SplitPath(const char *path, std::string *drive, std::string *dir, std::string *name, std::string *ext);
GELIB_API void SplitPath(const char *path, std::string *driveAndDir, std::string *nameAndExt);
GELIB_API std::string MakePath(const char *drive, const char *dir, const char *name, const char *ext);
GELIB_API std::string MakePath(const char *driveAndDir, const char *nameAndExt);
GELIB_API std::string FixPath(const char *path);		// convert to platform dependent path
GELIB_API std::wstring FixPath(const wchar_t *path);	// convert to platform dependent path
GELIB_API bool IsSamePath(const char *path1, const char *path2);
GELIB_API std::string RemoveLastDir(const char *path);
GELIB_API char GetPathSeparator();
GELIB_API wchar_t GetPathSeparatorW();
GELIB_API std::string AppendPathSeparator(const char *path, char separator=0);
GELIB_API std::string ReplacePathSeparator(const char *path, char separator=0);
GELIB_API std::wstring ReplacePathSeparator(const wchar_t *path, wchar_t separator=0);
GELIB_API bool IsRelativePath(const char *path, const char *basePath, std::string &rpath, char separator=0);
GELIB_API std::string ResolveRelativePath(const char *path, char separator=0);		// resolve relative path (../) and replace separator
GELIB_API std::string RemoveExt(const char *path);

class FileHandle;
enum EFSeekOrigin
{
    FSEEK_BEGIN,
    FSEEK_END,
    FSEEK_CURRENT
};

GELIB_API FileHandle* FileOpenRead(const char *fileName);
GELIB_API FileHandle* FileOpenWrite(const char *fileName);
GELIB_API FileHandle* FileOpenRead(const wchar_t *fileName);
GELIB_API FileHandle* FileOpenWrite(const wchar_t *fileName);
GELIB_API void FileClose(FileHandle *file);
GELIB_API int FileRead(FileHandle *file, void *buffer, int size);
GELIB_API int FileWrite(FileHandle *file, const void *buffer, int size);
GELIB_API int FileSeek(FileHandle *file, int offset, EFSeekOrigin origin);
GELIB_API int FileTell(FileHandle *file);
GELIB_API int FileSize(FileHandle *file);

GELIB_API bool MakeDirectory(const char *dirName);
GELIB_API void MakeDirectories(const char *fileName);
GELIB_API void SetProcessCurrentDirectory(const char *dir);
GELIB_API std::string GetProcessCurrentDirectory();

GELIB_API bool IsFileExist(const char *fileName);
GELIB_API int  GetFileSize(const char *fileName);
GELIB_API unsigned long long GetFileLastWriteTime(const char *fileName);
GELIB_API bool CopyFile(const char *srcFileName, const char *destFileName);
GELIB_API void DeleteFile(const char *fileName);

GELIB_API bool LoadFileToBuffer(const char *fileName, CDataBuffer &buf);
GELIB_API bool LoadFileToBuffer(const wchar_t *fileName, CDataBuffer &buf);
GELIB_API bool LoadFileToString(const char *fileName, std::string &str);
GELIB_API bool LoadFileToString(const wchar_t *fileName, std::string &str);
GELIB_API bool WriteTextToFile(const char *fileName, const char *text);
GELIB_API bool WriteDataToFile(const char *fileName, const void *data, int size);

GELIB_API void FindFiles(const char *fileSpec, std::vector<std::string> &files, bool findSubDirectory=false);
GELIB_API void FindDirectories(const char *dirSpec, std::vector<std::string> &dirs, bool findSubDirectory=false);

inline int MAGIC(char a, char b, char c, char d)
{
	return (d << 24) + (c << 16) + (b << 8) + a;
}

inline short MAGIC(char a, char b)
{
	return (b << 8) + a;
}

}//GELib

#endif//__GELIB_UTILITY_FILEUTILITY_H__
