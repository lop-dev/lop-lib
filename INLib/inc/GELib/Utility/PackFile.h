//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/PackFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_PACKFILE_H__
#define __GELIB_UTILITY_PACKFILE_H__

#include <string>
#include <vector>
#include "FileUtility.h"
#include "StringMap.h"

namespace GELib
{

class CPackFile
{
public:
    CPackFile();
    virtual ~CPackFile();

    bool Open(const char *packFile);
    void Close();

    // return -1 if not found
    int GetFileIndexByName(const char *fname);

	class CFileInfo
	{
	public:
		const char *m_Name;
        unsigned int m_iSize;
        unsigned int m_iCompressedSize;
	};
    void GetFileInfo(int index, CFileInfo &info);
	int GetNumFiles();

    int ReadFile(int index, void *buffer, int size);
    void CloseFile(int index);

	static bool Pack(const std::vector<std::string> &files, const char *baseDir, const char *outFile, int compressType=1);	// compressType 0: none, 1: lz4, 2: lzma
	static bool Unpack(const char *packFile, const char *outDir);

private:

    FileHandle *m_hFile;
	unsigned int m_iReadPosition;

    class CPackFileEntry
    {
    public:
        CPackFileEntry() { m_pData = 0; m_iReadPosition = 0; }
        unsigned short m_iCompressType;
        unsigned short m_iVersion;
        unsigned int m_iOffset;
        unsigned int m_iCompressedSize;
        unsigned int m_iUncompressedSize;
        unsigned int m_iFlag;
        std::string m_Name;
        std::string m_LowerName;
        CPackFile *m_pPackFile;

        char *m_pData;
        unsigned int m_iReadPosition;
    };
    std::vector<CPackFileEntry> m_Entries;
    StringVectorMap<int> m_EntryNameMap;

    CPackFileEntry* GetEntry(int index);
    bool ReadWholeFile(CPackFileEntry *e, void *data);
};


}//GELib

#endif//__GELIB_UTILITY_PACKFILE_H__
