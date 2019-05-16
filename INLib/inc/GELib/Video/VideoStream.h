//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Video/VideoStream.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_VIDEO_VIDEOSTREAM_H__
#define __GELIB_VIDEO_VIDEOSTREAM_H__

namespace GELib
{


class CVideoStream
{
public:

    CVideoStream() {}
    virtual ~CVideoStream() {}

	virtual bool Open(const char *fname) = 0;
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual void Pause(bool pause=true) = 0;

    virtual void GetVideoSize(int &width, int &height) = 0;
    virtual void GetVideoSample(void *buf, int pitch, int height, int flag=0) = 0;
};

enum EVideoStreamType
{
    VIDEO_STREAM_NULL,
    VIDEO_STREAM_WMF
};

CVideoStream* CreateVideoStream(EVideoStreamType streamType);
void DeleteVideoStream(CVideoStream *v);


}//GELib

#endif//__GELIB_VIDEO_VIDEOSTREAM_H__
