//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/binFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_BINFILE_H__
#define __BCLIB_UTILITY_IO_BINFILE_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{

class BCLIB_UTILITY_API CBinData
{
public:
	CBinData();
	~CBinData();

	int8* newData(BCLib::int64 size);
	void freeData();

	int8* getData() { return m_pData; }
	BCLib::int64 getSize() {	return m_iSize;  }
	bool isEmpty() { return m_iSize == 0; }

private:
	int8* m_pData;
	BCLib::int64 m_iSize;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 文件操作，如果要输入其他格式的数据，重新实现operator >> <<
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CBinFile
{
public:
    enum ESharingFlag
    {
        ESF_DENYNO,     // Permits read and write access
        ESF_DENYRD,     // Denies read acess to the file
        ESF_DENYRW,     // Denies read and write access to the file
        ESF_DENYWR      // Denies write access to the file
    };

public:
    CBinFile();
    virtual ~CBinFile();

    // rwMode: r, w, a, r+, w+, a+
    bool create(const std::string& file, std::string rwMode = "w+", ESharingFlag eSharingFlag = ESF_DENYRW);
    bool open(const std::string& file, std::string rwMode = "r+", ESharingFlag eSharingFlag = ESF_DENYRW);
    bool flush();
    void close();

    bool seekCurrent(int64 n);
    bool seekEnd(int64 n);
    bool seekHead(int64 n);
    int64 tellCurrent();

    bool isEnd();

    int read(int8* buff, uint32 buffSize) const;
    int write(const int8* buff, uint32 buffSize);

    int read(std::string& data) const;
    int write(const std::string& data);

    template<typename TYPE>
    int read(TYPE& data) const;

    template<typename TYPE>
    int write(const TYPE& data);

    const CBinFile& operator >>(std::string& n) const;
    CBinFile& operator <<(const std::string& n);

    template<typename TYPE>
    const CBinFile& operator >>(TYPE& n) const;

    template<typename TYPE>
    CBinFile& operator <<(const TYPE& n);

public:
    static bool loadFileToData(const char *fileName, CBinData &binData);

private:
    FILE* m_binFile;
};

template<typename TYPE>
int CBinFile::read(TYPE& data) const
{
    return read((int8 *)&data, sizeof(TYPE));
}

template<typename TYPE>
int CBinFile::write(const TYPE& data)
{
    return write((const int8 *)&data, sizeof(TYPE));
}

template<typename TYPE>
const CBinFile& CBinFile::operator >>(TYPE& data) const
{
    read(data);
    return *this;
}

template<typename TYPE>
CBinFile& CBinFile::operator <<(const TYPE& data)
{
    write(data);
    return *this;
}
}//Utility
}//BCLib


#endif//__BCLIB_UTILITY_IO_BINFILE_H__
