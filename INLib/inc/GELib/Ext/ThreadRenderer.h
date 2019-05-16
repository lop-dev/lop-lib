//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/ThreadRenderer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_THREADRENDERER_H__
#define __GELIB_EXT_THREADRENDERER_H__

#include <GELib/Utility/SystemUtility.h>


namespace GELib
{


class CRenderThread;


class CThreadRenderer
{
public:
    friend class CRenderThread;

    CThreadRenderer();
    virtual ~CThreadRenderer();

    void Initialize();
    void Uninitialize();
    void SetRenderThreadProcessor(int processNo);

    void EnableRenderThread(bool enable);   // default is true

    void Render();
    void Present();

protected:

    // work in main thread
    virtual void BuildRenderBuffer();

    // swap renderer buffer
    virtual void SwapRenderBuffer();

    // work in render thread
    virtual void DrawRenderBuffer();

    // work in main thread, before DrawRenderBuffer() start
    virtual void PreDraw();

    // work in main thread, after DrawRenderBuffer() finished
    virtual void PostDraw();

    // present graphic device back buffer
    virtual void PresentBackBuffer();

private:

    void RunRenderThread();

    CEventObject m_BeginDrawEvent;
    CEventObject m_EndDrawEvent;
    CEventObject m_RenderThreadUseDeviceEvent;
    CRenderThread *m_pRenderThread;
    bool m_bEnableRenderThread;
    bool m_bFirstFrame;
    volatile int m_iStopRenderThread;
};


}//GELib

#endif//__GELIB_EXT_THREADRENDERER_H__
