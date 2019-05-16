//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/GraphicDevice.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_GRAPHICDEVICE_H__
#define __GELIB_GRAPHICDEVICE_GRAPHICDEVICE_H__

#if defined(_WIN32)
	#pragma warning ( disable: 4251 )
#endif

#include <string>
#include "GraphicDeviceTypes.h"
#include <GELib/GeConfig.h>


namespace GELib
{

class CGraphicDevice;
GELIB_API CGraphicDevice* CreateGraphicDevice(EGraphicDeviceType deviceType);
GELIB_API void DeleteGraphicDevice(CGraphicDevice *device);
GELIB_API void SetGraphicDevice(CGraphicDevice *device);

#ifdef GE_DLL
    GELIB_API CGraphicDevice* GetGraphicDevice();
#else
    inline CGraphicDevice* GetGraphicDevice()
    {
        extern CGraphicDevice *g_pGraphicDevice;
        return g_pGraphicDevice;
    }
#endif


class CGraphicDeviceResource;
class CTextureResource;
class CSurfaceResource;
class CRenderTargetResource;
class CShaderResource;
class CBufferResource;
class CVertexDeclarationResource;
class COcclusionQueryResource;

class CTextureResourceManager;
class CSurfaceResourceManager;
class CRenderTargetResourceManager;
class CShaderResourceManager;
class CBufferResourceManager;
class CVertexDeclarationResourceManager;
class COcclusionQueryResourceManager;
class CAnimatedTextureResourceManager;


class GELIB_API CGraphicDevice
{
public:

    class Rect
    {
    public:
        int x;
        int y;
        int width;
        int height;
    };

    CGraphicDevice();
    virtual ~CGraphicDevice();

    // GetVideoMode() can be used before Initialize()
    void GetVideoMode(CVideoMode &videoMode);

    int  GetNumAdapterModes(EPixelFormat format);
    void GetAdapterModes(EPixelFormat format, CVideoMode videoModes[]);

    bool Initialize(const CGraphicDeviceSettings &settings);
    void Uninitialize();

    // only m_iWidth, m_iHeight, m_bFullScreen and m_eVSync are changeable
    bool ChangeSettings(const CGraphicDeviceSettings &settings);

    // if device is used in multi-threads, each thread must call BeginUseDevice()/EndUseDevice() when using any CGraphicDevice function
    void BeginUseDevice();
    void EndUseDevice();
    void WaitUntilGpuIdle();
	void EnableSyncFrame(bool enable);	// flush gpu command buffer every frame, reduce mouse lag

    bool BeginScene();
    void EndScene();
    bool IsBeginSceneSucceeded();  // may return false if BeginScene is called, but device lost later

    bool IsReady();
    unsigned int GetResetCount(void);

    void Clear(bool color, bool zbuffer, bool stencil);
    void PresentBackBuffer(void *hWnd=0);
    void PresentBackBuffer(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, void *hWnd=0);
    void SetViewport(int x, int y, int w, int h);

    void SetClearColor(int r, int g, int b, int a);
    void SetClearColor(float r, float g, float b, float a);
    void SetClearDepth(float z);
    void SetClearStencil(unsigned int s);

    void SetRenderState(ERenderState state, int value);
    void SetPolygonOffset(float factor, float units);
    void SetColorWrite(bool r, bool g, bool b, bool a);
    void SetPointSize(float s);
    void SetClipPlane(int idx, float plane[4]);
    void SetScissorRect(int x, int y, int w, int h);
    void SetFrontFace(EFrontFace f);
    void SetFillMode(EFillMode f);

    void SetAlphaFunc(ECmpFunc func, int ref);
    void SetBlendFunc(EBlendFactor sfactor, EBlendFactor dfactor);
    void SetBlendOp(EBlendOp op);
    void SetBlendFuncAlpha(EBlendFactor sfactor, EBlendFactor dfactor);
    void SetBlendOpAlpha(EBlendOp op);
    void SetDepthFunc(ECmpFunc func);
    void SetDepthRange(float znear, float zfar);
    void SetStencilFunc(ECmpFunc func, int ref, int mask);
    void SetStencilOp(EStencilOP fail, EStencilOP zfail, EStencilOP zpass);
    void SetStencilWriteMask(int mask);
    void SetCCWStencilFunc(ECmpFunc func, int ref, int mask);
    void SetCCWStencilOp(EStencilOP fail, EStencilOP zfail, EStencilOP zpass);
    void SetCCWStencilWriteMask(int mask);
    void SetHiStencilFunc(ECmpFunc func, int ref);
    void FlushHiZStencil(bool asynchronous=true);

    void SetSamplerState(int sampler, ESamplerState state, int value);
    void SetTextureAddressMode(int sampler, ETextureAddressMode mode);
    void SetTextureFilter(int sampler, ETextureFilter magFilter, ETextureFilter minFilter, ETextureFilter mipFilter);
    void SetTextureBorderColor(int sampler, int r, int g, int b, int a);
    void SetTextureBorderColor(int sampler, unsigned int packedColor);
    void SetMipmapLodBias(int sampler, float bias);

    void IgnoreGammaCorrection(bool ignore);		// disable D3DSAMP_SRGBTEXTURE and D3DRS_SRGBWRITEENABLE
	bool IsGammaCorrectionIgnored();

    int CreateTexture(const CTextureSpec &spec, const char *name=0);
    int CreateAnimatedTexture(int *textures, int numTextures, float fps, const char *name=0);
    void DeleteTexture(int id);   // id can be texture or animated texture
    int ReplaceTexture(int id, const CTextureSpec &spec);
    int ReplaceAnimatedTexture(int id, int *textures, int numTextures, float fps);
    void* LockTexture(int id, int mipLevel=0, int *pitch=0, int lock=RES_LOCK_WRITE, const Rect *rect=0);
    void UnlockTexture(int id, int mipLevel=0);
    void* LockCubeTexture(int id, ECubeFace face, int mipLevel=0, int *pitch=0, int lock=RES_LOCK_WRITE);
    void UnlockCubeTexture(int id, ECubeFace face, int mipLevel=0);
    void* LockVolumeTexture(int id, int mipLevel=0, int *rowPitch=0, int *slicePitch=0, int lock=RES_LOCK_WRITE);
    void UnlockVolumeTexture(int id, int mipLevel=0);
    void SetTexture(int sampler, int id);    // id can be texture, animated texture or render target

    void GetTextureSize(int id, int &width, int &height);  // id can be texture, animated texture or render target
    void GetTextureFormat(int id, EPixelFormat &format);   // id can be texture, animated texture or render target

    void CopyBackBufferToTexture(int id, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);
    void SaveTextureToFile(int id, char const *path, EImageFileFormat imageFormat=IFF_DDS, EPixelFormat pixelFormat=FMT_UNKNOWN);
    void SaveRenderTargetToFile(int id, char const *path, EImageFileFormat imageFormat=IFF_DDS, EPixelFormat pixelFormat=FMT_UNKNOWN);

    int CreateRenderTarget(const CRenderTargetSpec &spec, const char *name=0);
    int CreateRenderTarget(int colorRT, int depthRT, const char *name=0);
    void DeleteRenderTarget(int id);
    void SetRenderTarget(int idx, int id);
    void SetCubeRenderTarget(int idx, int id, ECubeFace face);

    void ResolveCurrentRenderTarget(int idx, int id);            // xbox resolve from EDRAM to texture
    void ResolveRenderTarget(int id);                              // pc directx resolve from AA-surface to non-AA texture
    void GetRenderTargetData(int id, void *data, int &size, int &pitch);       // data == 0 to return size and pitch only

    int CreateVertexBuffer(int vertexCount, int vertexStride, int usage=RES_USAGE_WRITEONLY, const char *name=0);
    void DeleteVertexBuffer(int id);
    int ReplaceVertexBuffer(int id, int vertexCount, int vertexStride, int usage=RES_USAGE_WRITEONLY);
    void* LockVertexBuffer(int id, int firstVertex=0, int numVertices=0, int lock=RES_LOCK_WRITE);
    void UnlockVertexBuffer(int id);
    void SetVertexBuffer(int stream, int id, int offsetInBytes=0);

    int CreateIndexBuffer(int indexCount, int indexStride, int usage=RES_USAGE_WRITEONLY, const char *name=0);
    void DeleteIndexBuffer(int id);
    int ReplaceIndexBuffer(int id, int indexCount, int indexStride, int usage=RES_USAGE_WRITEONLY);
    void* LockIndexBuffer(int id, int firstIndex=0, int numIndices=0, int lock=RES_LOCK_WRITE);
    void UnlockIndexBuffer(int id);
    void SetIndexBuffer(int id);

    int CreateVertexShader(const void *code, int size, const char *name=0);
    void DeleteVertexShader(int id);
    void SetVertexShader(int id);
    void SetVertexShaderConstant(int startRegister, const float *value, int vcount);
    void SetVertexShaderConstant(int startRegister, const int *value, int vcount);

    int CreatePixelShader(const void *code, int size, const char *name=0);
    void DeletePixelShader(int id);
    void SetPixelShader(int id);
    void SetPixelShaderConstant(int startRegister, const float *value, int vcount);
    void SetPixelShaderConstant(int startRegister, const int *value, int vcount);

    void GetShaderVersion(const void *code, int &versionMajor, int &versionMinor);

    int CreateVertexDeclaration(const CVertexSpec &spec, const char *name=0);
    void DeleteVertexDeclaration(int id);
    void SetVertexDeclaration(int id);

    int CreateOcclusionQuery();
    void DeleteOcclusionQuery(int id);
    void BeginOcclusionQuery(int id);
    void EndOcclusionQuery(int id);
    bool GetOcclusionQueryResult(int id, int &pixels, bool flush);

    void DrawPrimitive(EPrimitiveType primType, int startVertex, int primCount);
    void DrawPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride);
    void DrawIndexedPrimitive(EPrimitiveType primType, int startIndex, int primCount, int minVertex, int numVertices, int baseVertexIndex=0);
    void DrawIndexedPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride, const void *indexData, int indexStride, int minVertex, int numVertices);

    bool CheckResourceFormat(EResourceType rt, EPixelFormat format, int usage);
	bool CheckMultiSampleType(EMultiSampleType mst, EPixelFormat fmt);
    void EvictManagedResources();

    CGraphicDeviceResource* GetResource(int id);
    CGraphicDeviceResource* GetResourceByName(EResourceType rt, const char *name);

	// gl
	int gl_CreateProgram(int vs, int ps, const char *name=0);
    void gl_DeleteProgram(int id);
    void gl_SetProgram(int id);
    void gl_GetProgramConstantInfo(int id, const char *name, int &index, int &vcount);
    void gl_SetProgramConstant(int index, const float *value, int vcount);
    void gl_SetProgramConstant(int index, const int *value, int vcount);

    void ShowCursor(bool show);
    void SetCursorTexture(int id, int xHotspot, int yHotspot);
    void SetCursorPosition(int x, int y);

    // pix named event
    void BeginPerfEvent(const char *name, int color);
    void EndPerfEvent();

    enum
    {
        CLIP_PLANES_MAX = 6,
        RENDERTARGETS_MAX = 4,
        VERTEX_STREAMS_MAX = 16,
        TEXTURE_SAMPLERS_MAX = 16
    };

    class GELIB_API RenderStates
    {
    public:
        bool m_bAlphaTest;
        bool m_bBlend;
        bool m_bSeparateAlphaBlend;
        bool m_bCullFace;
        bool m_bColorWrite[4];
        bool m_bDepthTest;
        bool m_bDepthWrite;
        bool m_bStencilTest;
        bool m_bStencilTwoSided;
        bool m_bHiStencilTest;
        bool m_bHiStencilWrite;
        bool m_bScissorTest;
        bool m_bPointSprite;
        bool m_bSRGBWrite;
        unsigned int m_iClearColor;
        unsigned int m_iClearColorR;
        unsigned int m_iClearColorG;
        unsigned int m_iClearColorB;
        unsigned int m_iClearColorA;
        unsigned int m_iClearStencil;
        unsigned int m_iClipPlanes;
        unsigned int m_iStencilWriteMask;
        unsigned int m_iCCWStencilWriteMask;
        unsigned int m_iAlphaRef;
        unsigned int m_iStencilRef;
        unsigned int m_iStencilMask;
        unsigned int m_iCCWStencilRef;
        unsigned int m_iCCWStencilMask;
        unsigned int m_iHiStencilRef;
        float m_fClearDepth;
        float m_fPointSize;
        float m_fPolygonOffsetFactor;
        float m_fPolygonOffsetUnits;
        float m_fZNear;
        float m_fZFar;
        float m_ClipPlaneEquation[CLIP_PLANES_MAX][4];
        EFrontFace m_eFrontFace;
        EFillMode m_eFillMode;
        ECmpFunc m_eAlphaFunc;
        ECmpFunc m_eDepthFunc;
        ECmpFunc m_eStencilFunc;
        ECmpFunc m_eCCWStencilFunc;
        ECmpFunc m_eHiStencilFunc;
        EBlendFactor m_eSrcBlendFactor;
        EBlendFactor m_eDestBlendFactor;
        EBlendOp m_eBlendOp;
        EBlendFactor m_eSrcBlendFactorAlpha;
        EBlendFactor m_eDestBlendFactorAlpha;
        EBlendOp m_eBlendOpAlpha;
        EStencilOP m_eStencilFail;
        EStencilOP m_eStencilZFail;
        EStencilOP m_eStencilZPass;
        EStencilOP m_eCCWStencilFail;
        EStencilOP m_eCCWStencilZFail;
        EStencilOP m_eCCWStencilZPass;

        Rect m_Viewport;
        Rect m_Scissor;
    };

    class GELIB_API SamplerStates
    {
    public:
        ETextureAddressMode m_ETextureAddressMode;
        ETextureFilter m_eMagFilter;
        ETextureFilter m_eMinFilter;
        ETextureFilter m_eMipFilter;
        float m_fMipmapLodBias;
        unsigned int m_iBorderColor;
        unsigned int m_iMaxMipLevel;
        unsigned int m_iMaxAnisotropy;
    };

    class GELIB_API ActiveResources
    {
    public:
        int m_iTextureID[TEXTURE_SAMPLERS_MAX];
        int m_iRenderTargetID[RENDERTARGETS_MAX];
        int m_iVertexBufferID[VERTEX_STREAMS_MAX];
        int m_iIndexBufferID;
        int m_iVertexDeclarationID;
        int m_iVertexShaderID;
        int m_iPixelShaderID;
        int m_iGlProgramID;

        CTextureResource *m_pTexture[TEXTURE_SAMPLERS_MAX];
        CRenderTargetResource *m_pRenderTarget[RENDERTARGETS_MAX];
        CBufferResource *m_pVertexBuffer[VERTEX_STREAMS_MAX];
        CBufferResource *m_pIndexBuffer;
        CVertexDeclarationResource *m_pVertexDeclaration;
        CShaderResource *m_pVertexShader;
        CShaderResource *m_pPixelShader;
        CShaderResource *m_pGlProgram;
    };

    class GELIB_API Caps
    {
    public:
		Caps();

        std::string m_AdapterDescription;
        std::string m_AdapterDriver;
		int m_iAdapterVendorId;
		int m_iAdapterDeviceId;
        bool m_bDynamicTextures;
        bool m_bScissorTest;
        bool m_bStencilTwoSided;
        bool m_b32BitsIndexBuffer;
        bool m_bOcclusionQuery;
        bool m_bAutoGenMipMap;
		bool m_bAlphaTest;
        int m_iMaxTextureWidth;
        int m_iMaxTextureHeight;
        int m_iMaxUserClipPlanes;
        int m_iMaxStreams;
        int m_iMaxSimultaneousRenderTargets;
        int m_iVertexShaderVersionMajor;
        int m_iVertexShaderVersionMinor;
        int m_iPixelShaderVersionMajor;
        int m_iPixelShaderVersionMinor;
        int m_iVideoMemory;
        float m_fMaxPointSize;
    };

    class GELIB_API Statistic
    {
    public:
        Statistic();
        void Reset();

        int m_iNumTextures;
        int m_iNumSurfaces;
        int m_iNumRenderTargets;
        int m_iNumVertexShaders;
        int m_iNumPixelShaders;
        int m_iNumVertexBuffers;
        int m_iNumIndexBuffers;
        int m_iNumVertexDeclarations;

        int m_iTexturesSize;
        int m_iSurfaceSize;
        int m_iRenderTargetsSize;
        int m_iVertexShadersSize;
        int m_iPixelShadersSize;
        int m_iVertexBuffersSize;
        int m_iIndexBuffersSize;
        int m_iVertexDeclarationsSize;
    };

    const RenderStates& GetRenderStates();
    const SamplerStates& GetSamplerStates(int sampler);
    const ActiveResources& GetActiveResources();
    const Statistic& GetStatistic();
    int GetAvailableTextureMemory();

    EGraphicDeviceType GetDeviceType();
    void* GetInternalDevice();
    const Caps& GetCaps();
    void GetBackBufferSize(int &w, int &h);
    void* GetWindow();
    long  WindowProc(void *hWnd, unsigned int msg, WParam wParam, LParam lParam);
    void SetAnimationTime(float time);

	void SetMaxDrawPrimitiveCount(int n);
	void SetMaxViewportSize(int w, int h);

protected:

    void OnLostDevice();
    void OnResetDevice();
	void OnDeviceError(const char *message);
    void RestoreRenderState();

    void UnbindResource(void);

    int CreateSurface(const CSurfaceSpec &spec, const char *name=0);
    void DeleteSurface(int id);
    void SetDepthStencilSurface(int id);
    void SetDepthStencilTexture(int id);
    void SetRenderTargetSurface(int idx, int id);
    void SetRenderTargetTexture(int idx, int id);
    void SetRenderTargetCubeTexture(int idx, int id, ECubeFace face);
    int GetRealTextureID(int id);

    friend class CTextureResource;
    friend class CSurfaceResource;
    friend class CRenderTargetResource;
    friend class CBufferResource;
    friend class CShaderResource;
    friend class CVertexDeclarationResource;
    friend class COcclusionQueryResource;

    virtual void _GetVideoMode(CVideoMode &videoMode)=0;
    virtual int  _GetNumAdapterModes(EPixelFormat format)=0;
    virtual void _GetAdapterModes(EPixelFormat format, CVideoMode videoModes[])=0;

    virtual bool _CreateDevice()=0;
    virtual void _DestroyDevice()=0;
    virtual void _OnLostDevice()=0;
    virtual void _OnResetDevice()=0;
    virtual bool _ChangeSettings(const CGraphicDeviceSettings &settings)=0;

    virtual bool _BeginScene()=0;
    virtual void _EndScene()=0;
    virtual bool _IsBeginSceneSucceeded()=0;

    virtual bool _IsReady()=0;

    virtual void _Clear(bool color, bool zbuffer, bool stencil)=0;
    virtual void _PresentBackBuffer(void *hWnd)=0;
    virtual void _PresentBackBuffer(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, void *hWnd)=0;
    virtual void _SetViewport(int x, int y, int w, int h)=0;

    virtual void _SetRenderState(ERenderState state, int value)=0;
    virtual void _SetPolygonOffset(float factor, float unit)=0;
    virtual void _SetColorWrite(bool r, bool g, bool b, bool a)=0;
    virtual void _SetPointSize(float s)=0;
    virtual void _SetClipPlane(int idx, float plane[4])=0;
    virtual void _SetScissorRect(int x, int y, int w, int h)=0;
    virtual void _SetFrontFace(EFrontFace f)=0;
    virtual void _SetFillMode(EFillMode f)=0;
    virtual void _SetAlphaFunc(ECmpFunc func, int ref)=0;
    virtual void _SetBlendFunc(EBlendFactor sfactor, EBlendFactor dfactor)=0;
    virtual void _SetBlendOp(EBlendOp op)=0;
    virtual void _SetBlendFuncAlpha(EBlendFactor sfactor, EBlendFactor dfactor)=0;
    virtual void _SetBlendOpAlpha(EBlendOp op)=0;
    virtual void _SetDepthRange(float znear, float zfar)=0;
    virtual void _SetDepthFunc(ECmpFunc func)=0;
    virtual void _SetStencilFunc(ECmpFunc func, int ref, int mask)=0;
    virtual void _SetStencilOp(EStencilOP fail, EStencilOP zfail, EStencilOP zpass)=0;
    virtual void _SetStencilWriteMask(int mask)=0;
    virtual void _SetCCWStencilFunc(ECmpFunc func, int ref, int mask)=0;
    virtual void _SetCCWStencilOp(EStencilOP fail, EStencilOP zfail, EStencilOP zpass)=0;
    virtual void _SetCCWStencilWriteMask(int mask)=0;
    virtual void _SetHiStencilFunc(ECmpFunc func, int ref)=0;
    virtual void _FlushHiZStencil(bool asynchronous)=0;

    virtual void _SetSamplerState(int sampler, ESamplerState state, int value)=0;
    virtual void _SetTextureAddressMode(int sampler, ETextureAddressMode mode)=0;
    virtual void _SetTextureFilter(int sampler, ETextureFilter magFilter, ETextureFilter minFilter, ETextureFilter mipFilter)=0;
    virtual void _SetTextureBorderColor(int sampler, unsigned int color)=0;
    virtual void _SetMipmapLodBias(int sampler, float bias)=0;

    virtual bool _CreateTexture(CTextureResource &r, const CTextureSpec &spec)=0;
    virtual void _DeleteTexture(CTextureResource &r)=0;
    virtual void* _LockTexture(CTextureResource &r, int mipLevel, int *pitch, int lock, const Rect *rect)=0;
    virtual void _UnlockTexture(CTextureResource &r, int mipLevel)=0;
    virtual void* _LockCubeTexture(CTextureResource &r,  ECubeFace face, int mipLevel, int *pitch, int lock)=0;
    virtual void _UnlockCubeTexture(CTextureResource &r, ECubeFace face, int mipLevel)=0;
    virtual void* _LockVolumeTexture(CTextureResource &r, int mipLevel, int *rowPitch, int *slicePitch, int lock)=0;
    virtual void _UnlockVolumeTexture(CTextureResource &r, int mipLevel)=0;
    virtual void _SetTexture(int sampler, CTextureResource &r)=0;
    virtual void _CopyBackBufferToTexture(CTextureResource &r, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH)=0;

    virtual void _SaveTextureToFile(CTextureResource &r, char const *path, EImageFileFormat imageFormat, EPixelFormat pixelFormat)=0;
    virtual void _SaveBackBufferToFile(char const *path, EImageFileFormat imageFormat, EPixelFormat pixelFormat)=0;

    virtual bool _CreateSurface(CSurfaceResource &r, const CSurfaceSpec &spec)=0;
    virtual void _DeleteSurface(CSurfaceResource &r)=0;
    virtual void _SetDepthStencilSurface(CSurfaceResource &r)=0;
    virtual void _SetDepthStencilTexture(CTextureResource &r)=0;
    virtual void _SetNullDepthStencilSurface()=0;

    virtual void _AdjustRenderTargetSpec(CRenderTargetSpec &spec)=0;
    virtual void _SetRenderTargetSurface(int idx, CSurfaceResource &r)=0;
    virtual void _SetRenderTargetTexture(int idx, CTextureResource &r)=0;
    virtual void _SetRenderTargetCubeTexture(int idx, CTextureResource &r, ECubeFace face)=0;
    virtual void _ResolveCurrentRenderTargetToTexture(int idx, CTextureResource &r)=0;
    virtual void _ResolveRenderTargetToTexture(CSurfaceResource &rs, CTextureResource &rt)=0;
    virtual void _GetRenderTargetData(CRenderTargetResource &r, void *data, int &size, int &pitch)=0;

    virtual bool _CreateVertexShader(CShaderResource &r, const void *code, int size)=0;
    virtual void _DeleteVertexShader(CShaderResource &r)=0;
    virtual void _SetVertexShader(CShaderResource &r)=0;
    virtual void _SetVertexShaderConstantF(int startRegister, const float *value, int vcount)=0;
    virtual void _SetVertexShaderConstantI(int startRegister, const int *value, int vcount)=0;

    virtual bool _CreatePixelShader(CShaderResource &r, const void *code, int size)=0;
    virtual void _DeletePixelShader(CShaderResource &r)=0;
    virtual void _SetPixelShader(CShaderResource &r)=0;
    virtual void _SetPixelShaderConstantF(int startRegister, const float *value, int vcount)=0;
    virtual void _SetPixelShaderConstantI(int startRegister, const int *value, int vcount)=0;

    virtual void _GetShaderVersion(const void *code, int &versionMajor, int &versionMinor)=0;

    virtual bool _CreateVertexDeclaration(CVertexDeclarationResource &r, const CVertexSpec &spec)=0;
    virtual void _DeleteVertexDeclaration(CVertexDeclarationResource &r)=0;
    virtual void _SetVertexDeclaration(CVertexDeclarationResource &r)=0;

    virtual bool _CreateVertexBuffer(CBufferResource &r)=0;
    virtual void _DeleteVertexBuffer(CBufferResource &r)=0;
    virtual void* _LockVertexBuffer(CBufferResource &r, int offset, int size, int lock)=0;
    virtual void _UnlockVertexBuffer(CBufferResource &r)=0;
    virtual void _SetVertexBuffer(int stream, CBufferResource &r, int offsetInBytes)=0;

    virtual bool _CreateIndexBuffer(CBufferResource &r)=0;
    virtual void _DeleteIndexBuffer(CBufferResource &r)=0;
    virtual void* _LockIndexBuffer(CBufferResource &r, int offset, int size, int lock)=0;
    virtual void _UnlockIndexBuffer(CBufferResource &r)=0;
    virtual void _SetIndexBuffer(CBufferResource &r)=0;

    virtual bool _CreateOcclusionQuery(COcclusionQueryResource &r)=0;
    virtual void _DeleteOcclusionQuery(COcclusionQueryResource &r)=0;
    virtual void _BeginOcclusionQuery(COcclusionQueryResource &r)=0;
    virtual void _EndOcclusionQuery(COcclusionQueryResource &r)=0;
    virtual bool _GetOcclusionQueryResult(COcclusionQueryResource &r, int &pixels, bool flush)=0;

    virtual void _DrawPrimitive(EPrimitiveType primType, int startVertex, int primCount)=0;
    virtual void _DrawPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride)=0;
    virtual void _DrawIndexedPrimitive(EPrimitiveType primType, int startIndex, int primCount, int minVertex, int numVertices, int baseVertexIndex)=0;
    virtual void _DrawIndexedPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride, const void *indexData, int indexStride, int minVertex, int numVertices)=0;

	// gl
	virtual bool _gl_CreateProgram(CShaderResource &r, CShaderResource &vsr, CShaderResource &psr) { return 0; }
	virtual void _gl_DeleteProgram(CShaderResource &r) {}
	virtual void _gl_GetProgramConstantInfo(CShaderResource &r, const char *name, int &index, int &vcount) {}
	virtual void _gl_SetProgram(CShaderResource &r) {}
	virtual void _gl_SetProgramConstantF(int index, const float *value, int vcount) {}
	virtual void _gl_SetProgramConstantI(int index, const int *value, int vcount) {}

    virtual void _EvictManagedResources()=0;

    virtual void _ShowCursor(bool show)=0;
    virtual void _SetCursorTexture(CTextureResource &r, int xHotspot, int yHotspot)=0;
    virtual void _SetCursorPosition(int x, int y)=0;

    virtual bool _CheckResourceFormat(EResourceType rt, EPixelFormat format, int usage)=0;
	virtual bool _CheckMultiSampleType(EMultiSampleType mst, EPixelFormat fmt) = 0;

    virtual void* _GetInternalDevice()=0;
    virtual long  _WindowProc(void *hWnd, unsigned int msg, WParam wParam, LParam lParam)=0;
    virtual int _GetAvailableTextureMemory()=0;

    virtual void _BeginUseDevice()=0;
    virtual void _EndUseDevice()=0;
    virtual void _WaitUntilGpuIdle()=0;
    virtual unsigned long _GetCurrentThreadId()=0;

    virtual void _BeginPerfEvent(const char *name, int color=0xffff)=0;
    virtual void _EndPerfEvent()=0;

    RenderStates m_RenderStates;
    SamplerStates m_SamplerStates[TEXTURE_SAMPLERS_MAX];
    ActiveResources m_ActiveResources;
    Caps m_Caps;
    Statistic m_Statistic;

    bool m_bInitialized;
    bool m_bCreatingDevice;
    bool m_bDestroyingDevice;
	bool m_bSyncFrame;
	bool m_bIgnoreGammaCorrection;
    EGraphicDeviceType m_eDeviceType;
    CGraphicDeviceSettings m_Settings;
    int m_iBackBufferWidth;
    int m_iBackBufferHeight;
	int m_iMaxDrawPrimitiveCount;
	int m_iMaxViewportWidth;
	int m_iMaxViewportHeight;
    float m_fAnimationTime;
    void *m_hWnd;

    CTextureResourceManager *m_pTextureManager;
    CSurfaceResourceManager *m_pSurfaceManager;
    CRenderTargetResourceManager *m_pRenderTargetManager;
    CBufferResourceManager *m_pVertexBufferManager;
    CBufferResourceManager *m_pIndexBufferManager;
    CShaderResourceManager *m_pVertexShaderManager;
    CShaderResourceManager *m_pPixelShaderManager;
    CShaderResourceManager *m_pGlProgramManager;
    CVertexDeclarationResourceManager *m_pVertexDeclarationManager;
    COcclusionQueryResourceManager *m_pOcclusionQueryManager;
    CAnimatedTextureResourceManager *m_pAnimatedTextureManager;

    int m_iDummyTexture;
    CTextureResource *m_pDummyTextureRes;

    int m_iCursorTextureId;
    int m_iCursorHotspotX;
    int m_iCursorHotspotY;

    unsigned int m_iResetCount;
    unsigned long m_iActiveThreadId;
    unsigned long m_iUseDeviceCount;
};


inline const CGraphicDevice::RenderStates& CGraphicDevice::GetRenderStates()
{
    return m_RenderStates;
}

inline const CGraphicDevice::SamplerStates& CGraphicDevice::GetSamplerStates(int sampler)
{
    return m_SamplerStates[sampler];
}

inline const CGraphicDevice::ActiveResources& CGraphicDevice::GetActiveResources()
{
    return m_ActiveResources;
}

inline void CGraphicDevice::GetBackBufferSize(int &w, int &h)
{
    w = m_iBackBufferWidth;
    h = m_iBackBufferHeight;
}

inline const CGraphicDevice::Caps& CGraphicDevice::GetCaps()
{
    return m_Caps;
}

inline EGraphicDeviceType CGraphicDevice::GetDeviceType()
{
    return m_eDeviceType;
}

inline void* CGraphicDevice::GetWindow()
{
    return m_hWnd;
}

inline void CGraphicDevice::DrawPrimitive(EPrimitiveType primType, int startVertex, int primCount)
{
	if (primCount > m_iMaxDrawPrimitiveCount)
		primCount = m_iMaxDrawPrimitiveCount;
    if (primCount > 0)
        _DrawPrimitive(primType, startVertex, primCount);
}

inline void CGraphicDevice::DrawPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride)
{
	if (primCount > m_iMaxDrawPrimitiveCount)
		primCount = m_iMaxDrawPrimitiveCount;
    if (primCount > 0)
        _DrawPrimitiveUP(primType, primCount, vertexData, vertexStride);
}

inline void CGraphicDevice::DrawIndexedPrimitive(EPrimitiveType primType, int startIndex, int primCount, int minVertex, int numVertices, int baseVertexIndex)
{
	if (primCount > m_iMaxDrawPrimitiveCount)
		primCount = m_iMaxDrawPrimitiveCount;
    if (primCount > 0)
        _DrawIndexedPrimitive(primType, startIndex, primCount, minVertex, numVertices, baseVertexIndex);
}

inline void CGraphicDevice::DrawIndexedPrimitiveUP(EPrimitiveType primType, int primCount, const void *vertexData, int vertexStride, const void *indexData, int indexStride, int minVertex, int numVertices)
{
	if (primCount > m_iMaxDrawPrimitiveCount)
		primCount = m_iMaxDrawPrimitiveCount;
    if (primCount > 0)
        _DrawIndexedPrimitiveUP(primType, primCount, vertexData, vertexStride, indexData, indexStride, minVertex, numVertices);
}

inline void CGraphicDevice::SetVertexShaderConstant(int startRegister, const float *value, int vcount)
{
    _SetVertexShaderConstantF(startRegister, value, vcount);
}

inline void CGraphicDevice::SetVertexShaderConstant(int startRegister, const int *value, int vcount)
{
    _SetVertexShaderConstantI(startRegister, value, vcount);
}

inline void CGraphicDevice::SetPixelShaderConstant(int startRegister, const float *value, int vcount)
{
    _SetPixelShaderConstantF(startRegister, value, vcount);
}

inline void CGraphicDevice::SetPixelShaderConstant(int startRegister, const int *value, int vcount)
{
    _SetPixelShaderConstantI(startRegister, value, vcount);
}

inline void CGraphicDevice::gl_SetProgramConstant(int location, const float *value, int vcount)
{
	_gl_SetProgramConstantF(location, value, vcount);
}

inline void CGraphicDevice::gl_SetProgramConstant(int location, const int *value, int vcount)
{
	_gl_SetProgramConstantI(location, value, vcount);
}

inline void CGraphicDevice::BeginPerfEvent(const char *name, int color)
{
    _BeginPerfEvent(name, color);
}

inline void CGraphicDevice::EndPerfEvent()
{
    _EndPerfEvent();
}

class CGraphicDeviceLock
{
public:
    GELIB_API CGraphicDeviceLock();
    GELIB_API CGraphicDeviceLock(CGraphicDevice *device);
    GELIB_API ~CGraphicDeviceLock();
    GELIB_API void Lock(CGraphicDevice *device);
private:
    CGraphicDevice *m_pDevice;
};

class CPixEvent
{
public:
    CPixEvent(const char *name, int color=0xffff);
    ~CPixEvent();
};

#define PIXEVENT(a) CPixEvent _pixEvent(a);
#define PIXEVENTC(a, c) CPixEvent _pixEvent(a, c);


}//GELib

#endif//__GELIB_GRAPHICDEVICE_GRAPHICDEVICE_H__
