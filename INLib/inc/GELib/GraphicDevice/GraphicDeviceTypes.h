//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/GraphicDeviceTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_GRAPHICDEVICETYPES_H__
#define __GELIB_GRAPHICDEVICE_GRAPHICDEVICETYPES_H__

#include <GELib/GeConfig.h>

namespace GELib
{


enum EGraphicDeviceType
{
    GRAPHIC_DEVICE_NULL,
    GRAPHIC_DEVICE_D3D9,
    GRAPHIC_DEVICE_XENON,
    GRAPHIC_DEVICE_PS3,
    GRAPHIC_DEVICE_GLES2,
};

enum ERenderState
{
    RS_ALPHA_TEST,
    RS_BLEND,
    RS_SEPARATE_ALPHA_BLEND,
    RS_CULL_FACE,
    RS_CLIP_PLANE,
    RS_COLOR_WRITE,
    RS_DEPTH_TEST,
    RS_DEPTH_WRITE,
    RS_STENCIL_TEST,
    RS_STENCIL_TWO_SIDED,
    RS_HISTENCIL_TEST,
    RS_HISTENCIL_WRITE,
    RS_POINT_SPRITE,
    RS_SCISSOR_TEST,
    RS_SRGB_WRITE,
};

enum ESamplerState
{
    SAMP_MAXMIPLEVEL,
    SAMP_MAXANISOTROPY,
	SAMP_SRGBTEXTURE
};

enum ETextureAddressMode
{
    TEXADDRESS_WRAP,
    TEXADDRESS_CLAMP,
    TEXADDRESS_BORDER,
    TEXADDRESS_MIRROR
};

enum ETextureFilter
{
    TEXFILTER_NEAREST,
    TEXFILTER_LINEAR,
    TEXFILTER_ANISOTROPIC,
    TEXFILTER_NO_MIPMAP,

	// only useful in CShaderParameter::SetTexture()
	TEXFILTER_BILINEAR  = 100,
	TEXFILTER_BILINEAR_NO_MIPMAP  = 101
};

enum ECmpFunc
{
    CMP_NEVER,
    CMP_LESS,
    CMP_EQUAL,
    CMP_LESS_EQUAL,
    CMP_GREATER,
    CMP_NOT_EQUAL,
    CMP_GREATER_EQUAL,
    CMP_ALWAYS
};

enum EBlendFactor
{
    BLEND_ZERO,
    BLEND_ONE,
    BLEND_SRC_COLOR,
    BLEND_DEST_COLOR,
    BLEND_ONE_MINUS_SRC_COLOR,
    BLEND_ONE_MINUS_DEST_COLOR,
    BLEND_SRC_ALPHA,
    BLEND_DEST_ALPHA,
    BLEND_ONE_MINUS_SRC_ALPHA,
    BLEND_ONE_MINUS_DEST_ALPHA
};

enum EBlendOp
{
    BLEND_ADD,
    BLEND_SUB,
    BLEND_REV_SUB,
    BLEND_MIN,
    BLEND_MAX
};

enum EStencilOP
{
    STENCIL_KEEP,
    STENCIL_ZERO,
    STENCIL_REPLACE,
    STENCIL_INCR,
    STENCIL_DECR,
    STENCIL_INVERT
};

enum EPrimitiveType
{
    PT_TRIANGLE_LIST,
    PT_TRIANGLE_FAN,
    PT_TRIANGLE_STRIP,
    PT_LINE_LIST,
    PT_LINE_STRIP,
    PT_POINT_LIST
};

enum EFrontFace
{
    FACE_CW,
    FACE_CCW
};

enum EFillMode
{
    FILL_POINT,
    FILL_WIREFRAME,
    FILL_SOLID
};

enum EDeclType
{
    DECL_FLOAT1,
    DECL_FLOAT2,
    DECL_FLOAT3,
    DECL_FLOAT4,
    DECL_D3DCOLOR,
    DECL_UBYTE4,
    DECL_UBYTE4N,
    DECL_SHORT2,
    DECL_SHORT4,
    DECL_SHORT2N,
    DECL_SHORT4N,
    DECL_USHORT2N,
    DECL_USHORT4N,
    DECL_UDEC3,
    DECL_DEC3N,
    DECL_FLOAT16_2,
    DECL_FLOAT16_4
};

enum EDeclUsage
{
    DECL_POSITION,
    DECL_BLENDWEIGHT,
    DECL_BLENDINDICES,
    DECL_NORMAL,
    DECL_PSIZE,
    DECL_TEXCOORD,
    DECL_TANGENT,
    DECL_BINORMAL,
    DECL_TESSFACTOR,
    DECL_COLOR,
    DECL_FOG,
    DECL_DEPTH,
    DECL_SAMPLE,
};

enum EDeclMethod
{
    DECL_DEFAULT,
    DECL_PARTIALU,
    DECL_PARTIALV,
    DECL_CROSSUV,
    DECL_UV,
    DECL_LOOKUP,
    DECL_LOOKUP_PRESAMPLED
};

enum EImageFileFormat
{
    IFF_BMP,
    IFF_JPG,
    IFF_TGA,
    IFF_PNG,
    IFF_DDS,
    IFF_PPM,
    IFF_DIB,
    IFF_HDR,
    IFF_PFM
};

enum EPixelFormat
{
    FMT_UNKNOWN,

    FMT_A8R8G8B8,
    FMT_X8R8G8B8,

    FMT_A2R10G10B10,
    FMT_A2B10G10R10,

    FMT_A1R5G5B5,
    FMT_X1R5G5B5,
    FMT_R5G6B5,
    FMT_UYVY,

    FMT_DXT1,
    FMT_DXT3,
    FMT_DXT5,

    FMT_A8,
    FMT_L8,

    FMT_D16,
    FMT_D24S8,
    FMT_D24X8,

    FMT_R16F,
    FMT_R32F,
    FMT_G16R16F,
    FMT_A16B16G16R16F,
    FMT_A32B32G32R32F,

    FMT_G16R16,

    FMT_D24FS8,
};

enum EMultiSampleType
{
    MULTISAMPLE_NONE,
    MULTISAMPLE_2_SAMPLES,
    MULTISAMPLE_4_SAMPLES,
    MULTISAMPLE_8_SAMPLES,
    MULTISAMPLE_16_SAMPLES
};

enum EVSync
{
    VSYNC_NONE,
    VSYNC_1,
    VSYNC_2,
    VSYNC_3,
};

enum ECubeFace
{
    CUBE_X,
    CUBE_NX,
    CUBE_Y,
    CUBE_NY,
    CUBE_Z,
    CUBE_NZ,
    CUBE_MAX
};

enum EResourceType
{
    RES_UNKNOWN,
    RES_TEXTURE,
    RES_CUBE_TEXTURE,
    RES_VOLUME_TEXTURE,
    RES_SURFACE,
    RES_VERTEX_BUFFER,
    RES_INDEX_BUFFER,
    RES_VERTEX_SHADER,
    RES_PIXEL_SHADER,
    RES_GL_PROGRAM,
    RES_VERTEX_DECLARATION,
    RES_RENDER_TARGET,
    RES_ANIMATED_TEXTURE,
    RES_OCCLUSION_QUERY
};

const int RES_LOCK_DEFAULT =  0;
const int RES_LOCK_READ =    (1 << 0);
const int RES_LOCK_WRITE =   (1 << 1);

const int RES_USAGE_DEFAULT =      0;
const int RES_USAGE_DYNAMIC =      (1 << 0);
const int RES_USAGE_WRITEONLY =    (1 << 1);
const int RES_USAGE_RENDERTARGET = (1 << 2);
const int RES_USAGE_DEPTHSTENCIL = (1 << 3);
const int RES_USAGE_AUTOGENMIPMAP = (1 << 4);

// only useful for CheckResourceFormat()
const int RES_USAGE_QUERY_FILTER = (1 << 16);
const int RES_USAGE_QUERY_BLENDING = (1 << 17);
const int RES_USAGE_QUERY_SRGBREAD = (1 << 18);
const int RES_USAGE_QUERY_SRGBWRITE = (1 << 19);
const int RES_USAGE_QUERY_VERTEXTEXTURE = (1 << 20);

const int CLIP_PLANE0 = (1 << 0);
const int CLIP_PLANE1 = (1 << 1);
const int CLIP_PLANE2 = (1 << 2);
const int CLIP_PLANE3 = (1 << 3);
const int CLIP_PLANE4 = (1 << 4);
const int CLIP_PLANE5 = (1 << 5);


class GELIB_API CGraphicDeviceSettings
{
public:
    CGraphicDeviceSettings();
    int m_iWidth;
    int m_iHeight;
    bool m_bFullScreen;
    bool m_bStencilBuffer;
    bool m_bSoftwareRender;
    bool m_bNVPerfHUD;
	bool m_bAllowD3DPerf;
    bool m_bCopySwap;
    bool m_bFpuPreserve;
	bool m_bThreadSafe;
    EMultiSampleType m_eMultiSampleType;
    EVSync m_eVSync;
    void *m_hWnd;
    long (*m_MsgProcCallback)(void *hWnd, unsigned int msg, WParam wParam, LParam lParam, bool &processed);
    void (*m_LostDeviceCallback)();
    void (*m_ResetDeviceCallback)();
    void (*m_DeviceErrorCallback)(const char *message);
};

class GELIB_API CVertexSpec
{
public:
    CVertexSpec();
    void Reset();

    void AddPosition(EDeclType type=DECL_FLOAT3, int stream=0, int offset=-1);
    void AddNormal(EDeclType type=DECL_FLOAT3, int stream=0, int offset=-1);
    void AddTangent(EDeclType type=DECL_FLOAT3, int stream=0, int offset=-1);
    void AddBinormal(EDeclType type=DECL_FLOAT3, int stream=0, int offset=-1);
    void AddColor(EDeclType type=DECL_D3DCOLOR, int stream=0, int offset=-1);
    void AddTexCoord(EDeclType type=DECL_FLOAT2, int stream=0, int offset=-1);
    void AddBlendWeight(EDeclType type=DECL_FLOAT4, int stream=0, int offset=-1);
    void AddBlendIndices(EDeclType type=DECL_UBYTE4, int stream=0, int offset=-1);
    void AddPointSize(EDeclType type=DECL_FLOAT1, int stream=0, int offset=-1);
    void AddElement(EDeclType type, int stream, int offset, EDeclUsage usage, int usageIndex, EDeclMethod method=DECL_DEFAULT);

    class Element
    {
    public:
        EDeclType m_eType;
        EDeclUsage m_eUsage;
        EDeclMethod m_eMethod;
        unsigned short m_iStream;
        unsigned short m_iOffset;
        unsigned char m_iUsageIndex;
    };
    enum
    {
        ELEMENTS_MAX = 16,
        VERTEX_STREAMS_MAX = 16
    };


	int m_iNumPos;
	int m_iNumNormal;
	int m_iNumTangent;
	int m_iNumBinormal;
	int m_iNumColor;
    int m_iNumTexCoords;
	int m_iNumBlendWeight;
	int m_iNumBlendIndices;
	int m_iNumPointSize;
    int m_iNumElements;
    unsigned short m_VertexSize[VERTEX_STREAMS_MAX];
    Element m_Elements[ELEMENTS_MAX];
};

class GELIB_API CTextureSpec
{
public:
    CTextureSpec();
    void Reset();

    void SetSize(int width, int height);
    void SetCube(int size);
    void SetVolume(int width, int height, int depth);
    void SetFileData(const void *fileData, int fileSize);

    int m_iWidth;
    int m_iHeight;
    int m_iDepth;
    int m_iMipLevels;
    EPixelFormat m_eFormat;
    EResourceType m_eType;
    int m_iUsage;
    bool m_bLinear;
    bool m_bGammaCorrection;

    const void *m_pFileInMemory;
    int m_iFileSize;
    int m_iSkipFileMipLevels;
};

class CSurfaceSpec
{
public:
    CSurfaceSpec();
    void Reset();

    int m_iWidth;
    int m_iHeight;
    int m_iUsage;
    bool m_bAllocateEDRAM;
    EPixelFormat m_eFormat;
    EMultiSampleType m_eMultiSampleType;
};

class GELIB_API CRenderTargetSpec
{
public:
    CRenderTargetSpec();
    void Reset();

    int m_iWidth;
    int m_iHeight;
    bool m_bCreateColorTexture;
    bool m_bCreateColorSurface;
    bool m_bCreateDepthTexture;
    bool m_bCreateDepthSurface;
    bool m_bAllocateColorEDRAM;
    bool m_bAllocateDepthEDRAM;
    bool m_bGammaCorrectionRead;
    bool m_bGammaCorrectionWrite;
    bool m_bUseDefaultDepthSurface;
    bool m_bAutoGenMipMap;
    EPixelFormat m_eColorFormat;
    EPixelFormat m_eDepthFormat;
    EMultiSampleType m_eMultiSampleType;

};

class GELIB_API CVideoMode
{
public:
    int m_iWidth;
    int m_iHeight;
    int m_iRefreshRate; // 0 means default refersh rate
};


}//GELib

#endif//__GELIB_GRAPHICDEVICE_GRAPHICDEVICETYPES_H__
