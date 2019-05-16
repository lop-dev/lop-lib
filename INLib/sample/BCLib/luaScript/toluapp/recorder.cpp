//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/luaScript/toluapp/recorder.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include "recorder.h"

void CRecorder::record(const char* content)
{
    m_content = content;
}

void CRecorder::play()
{
    std::cout << m_content << std::endl;
}