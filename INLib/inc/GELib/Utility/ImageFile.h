//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ImageFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_IMAGEFILE_H__
#define __GELIB_UTILITY_IMAGEFILE_H__

#include <vector>
#include <GELib/GeConfig.h>

namespace GELib
{

class CIOStream;

class CImageFile
{
public:
    GELIB_API CImageFile();
    GELIB_API virtual ~CImageFile();

    class Pixel
    {
    public:
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    GELIB_API bool Load(const char *fname);			// only support tga file
    GELIB_API bool Load(const void *fileInMemory, int fileSize);
    GELIB_API bool Save(const char *fname);
    GELIB_API void Unload();

    GELIB_API Pixel* Allocate(int width, int height);
    GELIB_API void Clear(const Pixel &color);
    GELIB_API bool IsEmpty();

    GELIB_API void Copy(const CImageFile &src, int x, int y);
    GELIB_API void Copy(const CImageFile &src, int sx, int sy, int sw, int sh, int x, int y);
    GELIB_API void CopyChannel(const CImageFile &src, int srcChannel, int dstChannel);		// channel: 0: b, 1: g, 2: r, 3: a
    GELIB_API void FillChannel(int channel, int value);

    GELIB_API void EdgePadding(int n);
    GELIB_API void ComputeAlphaRect(int &x, int &y, int &w, int &h);

    GELIB_API bool Read_Tga(CIOStream &stream);
    GELIB_API void Write_Tga(CIOStream &stream);

    int m_iWidth;
    int m_iHeight;
    bool m_bAllWhite;
    bool m_bAllBlack;
    std::vector<Pixel> m_Pixels;
};

}//GELib

#endif//__GELIB_UTILITY_IMAGEFILE_H__
