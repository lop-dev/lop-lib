//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/DDSFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_DDSFILE_H__
#define __GELIB_GRAPHICDEVICE_DDSFILE_H__

#include <GELib/GraphicDevice/GraphicDeviceTypes.h>

namespace DDSFile
{

class CDDSInfo
{
public:
    CDDSInfo();
    int m_iWidth;
    int m_iHeight;
    int m_iDepth;
    int m_iMipLevels;
    const unsigned char *m_pBitData;
    int m_iBitSize;
    GELib::EPixelFormat m_eFormat;
};

bool ParseDDSFile(const void *fileData, int fileSize, CDDSInfo &info);
void GetDDSSurfaceInfo(GELib::EPixelFormat format, int width, int height, int *pNumBytes, int *pNumRows, int *pRowBytes);


}//DDSFile
#endif//__GELIB_GRAPHICDEVICE_DDSFILE_H__
