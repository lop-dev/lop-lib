//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/IOStream.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_IOSTREAM_H__
#define __GELIB_UTILITY_IOSTREAM_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include "EndianSwap.h"
#include "FileUtility.h"

#include <vector>
#include <string>
#include <cassert>
#include <stdio.h>


namespace GELib
{


class CIOStream
{
public:
    GELIB_API CIOStream() { m_bSwapEndian = false; }
    GELIB_API virtual ~CIOStream() {}

    GELIB_API virtual int Read(void *buffer, int size)=0;
    GELIB_API virtual int Write(const void *buffer, int size)=0;

    GELIB_API void SetSwapEndian(bool swap) { m_bSwapEndian = swap; }
    GELIB_API bool GetSwapEndian() { return m_bSwapEndian; }

protected:
    bool m_bSwapEndian;
};


class CFileStream : public CIOStream
{
public:
    GELIB_API CFileStream();
    GELIB_API virtual ~CFileStream();

    GELIB_API bool OpenRead(const char *fileName);
    GELIB_API bool OpenWrite(const char *fileName);
    GELIB_API void AttachHandle(FileHandle *file);         // file will not be closed
	GELIB_API void Close();
    GELIB_API int  Seek(int offset, EFSeekOrigin origin);
    GELIB_API int  Tell();

    GELIB_API virtual int Read(void *buffer, int size);
    GELIB_API virtual int Write(const void *buffer, int size);

private:
    bool m_bCloseFile;
    FileHandle *m_hFile;
};


class CMemoryStream : public CIOStream
{
public:
    GELIB_API CMemoryStream();
    GELIB_API virtual ~CMemoryStream();

    GELIB_API void SetReadBuffer(const void *buffer, int bufferSize);
    GELIB_API void SetWriteBuffer(void *buffer, int bufferSize);

    GELIB_API virtual int Read(void *buffer, int size);
    GELIB_API virtual int Write(const void *buffer, int size);

private:
    unsigned char *m_pReadBuffer;
    int m_iReadBufferSize;
    int m_iReadPosition;
    unsigned char *m_pWriteBuffer;
    int m_iWriteBufferSize;
    int m_iWritePosition;
};


class CMemoryWriteStream : public CIOStream
{
public:
    GELIB_API CMemoryWriteStream();
    GELIB_API CMemoryWriteStream(std::vector<unsigned char> &writeBuffer);
    GELIB_API virtual ~CMemoryWriteStream();

	GELIB_API CMemoryWriteStream& operator=(const CMemoryWriteStream &r) { assert(false); }	// remove warning C4512

    GELIB_API int   GetWriteSize();
    GELIB_API void* GetWriteData();

    GELIB_API void Clear();

    GELIB_API virtual int Read(void *buffer, int size);
    GELIB_API virtual int Write(const void *buffer, int size);

private:
    std::vector<unsigned char> &m_WriteBuffer;
    std::vector<unsigned char> m_Buffer;
};


inline void ReadStream(CIOStream &stream, bool   &v) { stream.Read(&v, sizeof(v)); }
inline void ReadStream(CIOStream &stream, char   &v) { stream.Read(&v, sizeof(v)); }
inline void ReadStream(CIOStream &stream, short  &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, long   &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, int    &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, float  &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, double &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, long long &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, unsigned char  &v) { stream.Read(&v, sizeof(v)); }
inline void ReadStream(CIOStream &stream, unsigned short &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, unsigned long  &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, unsigned int   &v) { stream.Read(&v, sizeof(v)); if (stream.GetSwapEndian()) SwapEndian(v); }
inline void ReadStream(CIOStream &stream, CRGBAColor &v) { stream.Read(&v, sizeof(v)); }
GELIB_API void ReadStream(CIOStream &stream, float *v, int n);
inline void ReadStream(CIOStream &stream, CVector2 &v) { ReadStream(stream, v.m_v, 2); }
inline void ReadStream(CIOStream &stream, CVector3 &v) { ReadStream(stream, v.m_v, 3); }
inline void ReadStream(CIOStream &stream, CVector4 &v) { ReadStream(stream, v.m_v, 4); }
inline void ReadStream(CIOStream &stream, CMatrix  &v) { ReadStream(stream, v.m_m16, 16); }
inline void ReadStream(CIOStream &stream, CQuaternion &v) { ReadStream(stream, v.m_v, 4); }
GELIB_API void ReadStream(CIOStream &stream, std::string &v);
GELIB_API void ReadStreamShortString(CIOStream &stream, std::string &v);
GELIB_API void ReadStream(CIOStream &stream, std::wstring &v);
GELIB_API void ReadStreamShortString(CIOStream &stream, std::wstring &v);


inline void WriteStream(CIOStream &stream, bool   v) { stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, char   v) { stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, short  v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, long   v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, int    v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, float  v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, double v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, long long v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, unsigned char  v) { stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, unsigned short v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, unsigned long  v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, unsigned int   v) { if (stream.GetSwapEndian()) SwapEndian(v); stream.Write(&v, sizeof(v)); }
inline void WriteStream(CIOStream &stream, CRGBAColor v) { stream.Write(&v, sizeof(v)); }
GELIB_API void WriteStream(CIOStream &stream, const float *v, int n);
inline void WriteStream(CIOStream &stream, const CVector2 &v) { WriteStream(stream, v.m_v, 2); }
inline void WriteStream(CIOStream &stream, const CVector3 &v) { WriteStream(stream, v.m_v, 3); }
inline void WriteStream(CIOStream &stream, const CVector4 &v) { WriteStream(stream, v.m_v, 4); }
inline void WriteStream(CIOStream &stream, const CMatrix  &v) { WriteStream(stream, v.m_m16, 16); }
inline void WriteStream(CIOStream &stream, const CQuaternion &v) { WriteStream(stream, v.m_v, 4); }
GELIB_API void WriteStream(CIOStream &stream, const std::string &v);
GELIB_API void WriteStream(CIOStream &stream, const char *v);
GELIB_API void WriteStreamShortString(CIOStream &stream, const std::string &v);
GELIB_API void WriteStreamShortString(CIOStream &stream, const char *v);
GELIB_API void WriteStream(CIOStream &stream, const std::wstring &v);
GELIB_API void WriteStream(CIOStream &stream, const wchar_t *v);
GELIB_API void WriteStreamShortString(CIOStream &stream, const std::wstring &v);
GELIB_API void WriteStreamShortString(CIOStream &stream, const wchar_t *v);



}//GELib

#endif//__GELIB_UTILITY_IOSTREAM_H__
