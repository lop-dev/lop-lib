//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/DDSTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_DDSTYPES_H__
#define __GELIB_GRAPHICDEVICE_DDSTYPES_H__


//
// For compilers that don't support nameless unions, do a
// #define NONAMELESSUNION
#ifndef DUMMYUNIONNAMEN
#if defined(__cplusplus) || !defined(NONAMELESSUNION)
#define DUMMYUNIONNAMEN(n)
#else
#define DUMMYUNIONNAMEN(n)      u##n
#endif
#endif


/*
 * DDCOLORKEY
 */
typedef struct _DDCOLORKEY
{
    unsigned long       dwColorSpaceLowValue;   // low boundary of color space that is to
                                        // be treated as Color Key, inclusive
    unsigned long       dwColorSpaceHighValue;  // high boundary of color space that is
                                        // to be treated as Color Key, inclusive
} DDCOLORKEY;

/*
 * DDPIXELFORMAT
 */
typedef struct _DDPIXELFORMAT
{
    unsigned long       dwSize;                 // size of structure
    unsigned long       dwFlags;                // pixel format flags
    unsigned long       dwFourCC;               // (FOURCC code)
    union
    {
        unsigned long   dwRGBBitCount;          // how many bits per pixel
        unsigned long   dwYUVBitCount;          // how many bits per pixel
        unsigned long   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
        unsigned long   dwAlphaBitDepth;        // how many bits for alpha channels
        unsigned long   dwLuminanceBitCount;    // how many bits per pixel
        unsigned long   dwBumpBitCount;         // how many bits per "buxel", total
        unsigned long   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
                                        // format list and if DDPF_D3DFORMAT is set
    } DUMMYUNIONNAMEN(1);
    union
    {
        unsigned long   dwRBitMask;             // mask for red bit
        unsigned long   dwYBitMask;             // mask for Y bits
        unsigned long   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
        unsigned long   dwLuminanceBitMask;     // mask for luminance bits
        unsigned long   dwBumpDuBitMask;        // mask for bump map U delta bits
        unsigned long   dwOperations;           // DDPF_D3DFORMAT Operations
    } DUMMYUNIONNAMEN(2);
    union
    {
        unsigned long   dwGBitMask;             // mask for green bits
        unsigned long   dwUBitMask;             // mask for U bits
        unsigned long   dwZBitMask;             // mask for Z bits
        unsigned long   dwBumpDvBitMask;        // mask for bump map V delta bits
        struct
        {
            unsigned short    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
            unsigned short    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
        } MultiSampleCaps;

    } DUMMYUNIONNAMEN(3);
    union
    {
        unsigned long   dwBBitMask;             // mask for blue bits
        unsigned long   dwVBitMask;             // mask for V bits
        unsigned long   dwStencilBitMask;       // mask for stencil bits
        unsigned long   dwBumpLuminanceBitMask; // mask for luminance in bump map
    } DUMMYUNIONNAMEN(4);
    union
    {
        unsigned long   dwRGBAlphaBitMask;      // mask for alpha channel
        unsigned long   dwYUVAlphaBitMask;      // mask for alpha channel
        unsigned long   dwLuminanceAlphaBitMask;// mask for alpha channel
        unsigned long   dwRGBZBitMask;          // mask for Z channel
        unsigned long   dwYUVZBitMask;          // mask for Z channel
    } DUMMYUNIONNAMEN(5);
} DDPIXELFORMAT;


/*
 * DDSCAPS2
 */
typedef struct _DDSCAPS2
{
    unsigned long       dwCaps;         // capabilities of surface wanted
    unsigned long       dwCaps2;
    unsigned long       dwCaps3;
    union
    {
        unsigned long       dwCaps4;
        unsigned long       dwVolumeDepth;
    } DUMMYUNIONNAMEN(1);
} DDSCAPS2;


typedef struct _DDSURFACEDESC2_32BIT
{
    unsigned long               dwSize;                 // size of the DDSURFACEDESC structure
    unsigned long               dwFlags;                // determines what fields are valid
    unsigned long               dwHeight;               // height of surface to be created
    unsigned long               dwWidth;                // width of input surface
    union
    {
        long            lPitch;                 // distance to start of next line (return value only)
        unsigned long           dwLinearSize;           // Formless late-allocated optimized surface size
    } DUMMYUNIONNAMEN(1);
    union
    {
        unsigned long           dwBackBufferCount;      // number of back buffers requested
        unsigned long           dwDepth;                // the depth if this is a volume texture
    } DUMMYUNIONNAMEN(5);
    union
    {
        unsigned long           dwMipMapCount;          // number of mip-map levels requestde
                                                // dwZBufferBitDepth removed, use ddpfPixelFormat one instead
        unsigned long           dwRefreshRate;          // refresh rate (used when display mode is described)
        unsigned long           dwSrcVBHandle;          // The source used in VB::Optimize
    } DUMMYUNIONNAMEN(2);
    unsigned long               dwAlphaBitDepth;        // depth of alpha buffer requested
    unsigned long               dwReserved;             // reserved
    unsigned long               lpSurface32;            // this ptr isn't 64bit safe in the ddraw.h header
    union
    {
        DDCOLORKEY      ddckCKDestOverlay;      // color key for destination overlay use
        unsigned long           dwEmptyFaceColor;       // Physical color for empty cubemap faces
    } DUMMYUNIONNAMEN(3);
    DDCOLORKEY          ddckCKDestBlt;          // color key for destination blt use
    DDCOLORKEY          ddckCKSrcOverlay;       // color key for source overlay use
    DDCOLORKEY          ddckCKSrcBlt;           // color key for source blt use
    union
    {
        DDPIXELFORMAT   ddpfPixelFormat;        // pixel format description of the surface
        unsigned long           dwFVF;                  // vertex format description of vertex buffers
    } DUMMYUNIONNAMEN(4);
    DDSCAPS2            ddsCaps;                // direct draw surface capabilities
    unsigned long               dwTextureStage;         // stage in multitexture cascade
} DDSURFACEDESC2_32BIT;

#endif//__GELIB_GRAPHICDEVICE_DDSTYPES_H__
