//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/luaScript/toluapp/recorder.h
//  author:     League of Perfect
/// @brief      为了演示用tolua++转换到c++而写的一个简单的录音机类
///             record() 记录string
///             play()   播放string
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_LUASCRIPT_TOLUAPP_RECORDER_H__
#define __BCLIB_SAMPLE_LUASCRIPT_TOLUAPP_RECORDER_H__

#include <string>

class CRecorder
{
public:
    CRecorder()
    {
    }
    ~CRecorder()
    {
    }

    void play();
    void record(const char* content);

private:
    std::string m_content;
};


#endif//__BCLIB_SAMPLE_LUASCRIPT_TOLUAPP_RECORDER_H__
