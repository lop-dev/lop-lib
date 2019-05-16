//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/Null/NullGraphicDevice.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_NULL_NULLGRAPHICDEVICE_H__
#define __GELIB_GRAPHICDEVICE_NULL_NULLGRAPHICDEVICE_H__

#include <GELib/GeConfig.h>
#include <GELib/GraphicDevice/GraphicDevice.h>

namespace GELib
{


class CNullGraphicDevice : public CGraphicDevice
{
public:

    CNullGraphicDevice();
    virtual ~CNullGraphicDevice();

protected:

    virtual void  _GetVideoMode(CVideoMode &videoMode);
    virtual int   _GetNumAdapterModes(EPixelFormat format);
    virtual void  _GetAdapterModes(EPixelFormat format, CVideoMode videoModes[]);

    virtual bool  _CreateDevice();
    virtual void  _DestroyDevice();
    virtual void  _OnLostDevice();
    virtual void  _OnResetDevice();
    virtual bool  _ChangeSettings(const CGraphicDeviceSettings &settings);

    virtual bool  _BeginScene();
    virtual void  _EndScene();
    virtual bool  _IsBeginSceneSucceeded();

    virtual bool  _IsReady();

    virtual void  _Clear(bool color, bool zbuffer, bool stencil);
    virtual void  _PresentBackBuffer(void *hWnd);
    virtual void  _PresentBackBuffer(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, void *hWnd);
    virtual void  _SetViewport(int x, int y, int w, int h);

    virtual void  _SetRenderState(ERenderState state, int value);
    virtual void  _SetPolygonOffset(float factor, float units);
    virtual void  _SetColorWrite(bool r, bool g, bool b, bool a);
    virtual void  _SetPointSize(float s);
    virtual void  _SetClipPlane(int idx, float plane[4]);
    virtual void  _SetScissorRect(int x, int y, int w, int h);
    virtual void  _SetFrontFace(EFrontFace f);
    virtual void  _SetFillMode(EFillMode f);
    virtual void  _SetAlphaFunc(ECmpFunc func, int ref);
    virtual void  _SetBlendFunc(EBlendFactor sfactor, EBlendFactor dfactor);
    virtual void  _SetBlendOp(EBlendOp op);
    virtual void  _SetBlendFuncAlpha(EBlendFactor sfactor, EBlendFactor dfactor);
    virtual void  _SetBlendOpAlpha(EBlendOp op);
    virtual void  _SetDepthFunc(ECmpFunc func);
    virtual void  _SetDepthRange(float znear, float zfar);
    virtual void  _SetStencilFunc(ECmpFunc func, int ref, int mask);
    virtual void  _SetStencilOp(EStencilOP fail, EStencilOP zfail, EStencilOP zpass);
    virtual void  _SetStencilWriteMask(int mask);
    virtual void  _SetCCWStencilFunc(ECmpFunc func, int ref, int mask);
    virtual void  _SetCCWStencilOp(EStencilOP fail, EStencilOP zfail, EStencilOP zpass);
    virtual void  _SetCCWStencilWriteMask(int mask);
    virtual void  _SetHiStencilFunc(ECmpFunc func, int ref);
    virtual void  _FlushHiZStencil(bool asynchronous);

    virtual void  _SetSamplerState(int sampler, ESamplerState state, int value);
    virtual void  _SetTextureAddressMode(int sampler, ETextureAddressMode mode);
    virtual void  _SetTextureFilter(int sampler, ETextureFilter magFilter, ETextureFilter minFilter, ETextureFilter mipFilter);
    virtual void  _SetTextureBorderColor(int sampler, unsigned int color);
    virtual void  _SetMipmapLodBias(int sampler, float bias);

    virtual bool  _CreateTexture(CTextureResource &r, const CTextureSpec &spec);
    virtual void  _DeleteTexture(CTextureResource &r);
    virtual void* _LockTexture(CTextureResource &r, int mipLevel, int *pitch, int lock, const Rect *rect);
    virtual void  _UnlockTexture(CTextureResource &r, int mipLevel);
    virtual void* _LockCubeTexture(CTextureResource &r,  ECubeFace face, int mipLevel, int *pitch, int lock);
    virtual void  _UnlockCubeTexture(CTextureResource &r, ECubeFace face, int mipLevel);
    virtual void* _LockVolumeTexture(CTextureResource &r, int mipLevel, int *rowPitch, int *slicePitch, int lock);
    virtual void  _UnlockVolumeTexture(CTextureResource &r, int mipLevel);
    virtual void  _SetTexture(int sampler, CTextureResource &r);
    virtual void  _CopyBackBufferToTexture(CTextureResource &r, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);

    virtual void  _SaveTextureToFile (CTextureResource &r, char const *path, EImageFileFormat imageFormat, EPixelFormat pixelFormat);
    virtual void  _SaveBackBufferToFile (char const *path, EImageFileFormat imageFormat, EPixelFormat pixelFormat);

    virtual bool  _CreateSurface(CSurfaceResource &r, const CSurfaceSpec &spec);
    virtual void  _DeleteSurface(CSurfaceResource &r);
    virtual void  _SetDepthStencilSurface(CSurfaceResource &r);
    virtual void  _SetDepthStencilTexture(CTextureResource &r);
    virtual void  _SetNullDepthStencilSurface();

    virtual void  _AdjustRenderTargetSpec(CRenderTargetSpec &spec);
    virtual void  _SetRenderTargetSurface(int idx, CSurfaceResource &r);
    virtual void  _SetRenderTargetTexture(int idx, CTextureResource &r);
    virtual void  _SetRenderTargetCubeTexture(int idx, CTextureResource &r, ECubeFace face);

    virtual void  _ResolveCurrentRenderTargetToTexture(int idx, CTextureResource &r);
    virtual void  _ResolveRenderTargetToTexture(CSurfaceResource &rs, CTextureResource &rt);
    virtual void  _GetRenderTargetData(CRenderTargetResource &r, void *data, int &size, int &pitch);

    virtual bool  _CreateVertexShader(CShaderResource &r, const void *code, int size);
    virtual void  _DeleteVertexShader(CShaderResource &r);
    virtual void  _SetVertexShader(CShaderResource &r);
    virtual void  _SetVertexShaderConstantF(int startRegister, const float *value, int vcount);
    virtual void  _SetVertexShaderConstantI(int startRegister, const int *value, int vcount);

    virtual bool  _CreatePixelShader(CShaderResource &r, const void *code, int size);
    virtual void  _DeletePixelShader(CShaderResource &r);
    virtual void  _SetPixelShader(CShaderResource &r);
    virtual void  _SetPixelShaderConstantF(int startRegister, const float *value, int vcount);
    virtual void  _SetPixelShaderConstantI(int startRegister, const int *value, int vcount);

    virtual void  _GetShaderVersion(const void *code, int &versionMajor, int &versionMinor);

    virtual bool  _CreateVertexDeclaration(CVertexDeclarationResource &r, const CVertexSpec &spec);
    virtual void  _DeleteVertexDeclaration(CVertexDeclarationResource &r);
    virtual void  _SetVertexDeclaration(CVertexDeclarationResource &r);

    virtual bool  _CreateVertexBuffer(CBufferResource &r);
    virtual void  _DeleteVertexBuffer(CBufferResource &r);
    virtual void* _LockVertexBuffer(CBufferResource &r, int offset, int size, int lock);
    virtual void  _UnlockVertexBuffer(CBufferResource &r);
    virtual void  _SetVertexBuffer(int stream, CBufferResource &r, int offsetInBytes);

    virtual bool  _CreateIndexBuffer(CBufferResource &r);
    virtual void  _DeleteIndexBuffer(CBufferResource &r);
    virtual void* _LockIndexBuffer(CBufferResource &r, int offset, int size, int lock);
    virtual void  _UnlockIndexBuffer(CBufferResource &r);
    virtual void  _SetIndexBuffer(CBufferResource &r);

    virtual bool  _CreateOcclusionQuery(COcclusionQueryResource &r);
    virtual void  _DeleteOcclusionQuery(COcclusionQueryResource &r);
    virtual void  _BeginOcclusionQuery(COcclusionQueryResource &r);
    virtual void  _EndOcclusionQuery(COcclusionQueryResource &r);
    virtual bool  _GetOcclusionQueryResult(COcclusionQueryResource &r, int &pixels, bool flush);

    virtual void  _DrawPrimitive(EPrimitiveType primType, int startVertex, int primCount);
    virtual void  _DrawPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride);
    virtual void  _DrawIndexedPrimitive(EPrimitiveType primType, int startIndex, int primCount, int minVertex, int numVertices, int baseVertexIndex);
    virtual void  _DrawIndexedPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride, const void *indexData, int indexStride, int minVertex, int numVertices);

    virtual void  _EvictManagedResources();

    virtual void  _ShowCursor(bool show);
    virtual void  _SetCursorTexture(CTextureResource &r, int xHotspot, int yHotspot);
    virtual void  _SetCursorPosition(int x, int y);

    virtual bool  _CheckResourceFormat(EResourceType rt, EPixelFormat format, int usage);
	virtual bool  _CheckMultiSampleType(EMultiSampleType mst, EPixelFormat fmt);

    virtual void* _GetInternalDevice();
    virtual long  _WindowProc(void *hWnd, unsigned int msg, WParam wParam, LParam lParam);
    virtual int   _GetAvailableTextureMemory();

    virtual void  _BeginUseDevice();
    virtual void  _EndUseDevice();
    virtual void  _WaitUntilGpuIdle();
    virtual unsigned long _GetCurrentThreadId();

    virtual void _BeginPerfEvent(const char *name, int color);

    virtual void _EndPerfEvent();

private:
    bool m_bBeginSceneSucceeded;
};

CGraphicDevice* CreateNullGraphicDevice();


}//GELib

#endif//__GELIB_GRAPHICDEVICE_NULL_NULLGRAPHICDEVICE_H__
