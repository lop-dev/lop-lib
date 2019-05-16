//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/ImageProcessor.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_IMAGEPROCESSOR_H__
#define __GELIB_RENDERER_IMAGEPROCESSOR_H__

#include "ImageShader.h"
#include <vector>

namespace GELib
{

class CRenderer;

class CImageProcessor
{
public:
    CImageProcessor();
    virtual ~CImageProcessor();

    void Initialize();
    void Uninitialize();
    void SetRenderer(CRenderer *renderer);

    void FillColor(const CVector4 &color);
    void FillTexture(int tex, bool filter=false);
    void FillTextureWithAlpha(int tex, float alpha, bool filter=false);
    void FillTextureWithMultiplierAndAdder(int tex, const CVector4 &multiplier, const CVector4 &adder, bool filter=false);
    void GammaTexture(int tex, float gamma, bool filter=false);

    void Copy(int src, int dst, bool filter=false);
    void CopyDepth(int src, int dst);
    void DownSample4x4(int src, int dst);
    void DownSample3x3(int src, int dst);
    void DownSample2x2(int src, int dst);
    void DownSampleMax4x4(int src, int dst);
    void DownSampleMax2x2(int src, int dst);
    void DownSampleMin4x4(int src, int dst);
    void DownSampleMin2x2(int src, int dst);
    void NearestDepthUpSample(int src, int depth, int lowDepth, int dst, float threshold, float maxDepth);
    void GaussianFilter3x3(int src, int dst);
    void GaussianFilter5x5(int src, int dst);
    void GaussianFilter15x15(int src, int dst, float angle, float deviation, float multiplier);
    void BrightPass(int src, int dst, float threshold, float scale);
    void DownSample4x4Bilinear(int src, int dst, bool clear=false);
    void GaussianFilterBilinear(int src, int temp, int dst, int radius, float deviation, bool clear=true, float rotate=0);
    void DofFilterBilinear(int src, int temp, int dst, int depth, const CVector4 &dofDistance, int radius, float deviation, float rotate=0);

    void GetSampleOffsetsDownSample4x4Bilinear(int width, int height, CVector2 sampleOffsets[]);
    void GetSampleOffsetsGaussianFilterBilinear(bool horizontal, int size, CVector2 sampleOffsets[], CVector4 sampleWeights[], int radius, float deviation);
    void GetSampleOffsetsGaussianFilterBilinear(int width, int height, CVector2 sampleOffsets[], CVector4 sampleWeights[], int radius, float deviation, float rotate);

    void GetSampleOffsetsDownSample4x4(int width, int height, CVector2 sampleOffsets[]);
    void GetSampleOffsetsDownSample3x3(int width, int height, CVector2 sampleOffsets[]);
    void GetSampleOffsetsDownSample2x2(int width, int height, CVector2 sampleOffsets[]);
    void GetSampleOffsetsGaussianFilter3x3(int width, int height, CVector2 sampleOffsets[], CVector4 sampleWeights[], float multiplier=1.0f);
    void GetSampleOffsetsGaussianFilter5x5(int width, int height, CVector2 sampleOffsets[], CVector4 sampleWeights[], float multiplier=1.0f);
    void GetSampleOffsetsGaussianFilter15x15(int width, int height, float angle, CVector2 sampleOffsets[], CVector4 sampleWeights[], float deviation, float multiplier=1.0f);
    float GaussianDistribution(float x, float y, float deviation);

    // lightmap
    void BlurLightMap(int src, int dst);
    void EdgePadding(int src, int dst);
    void ExtractMonoShadow(int src, int ao, int dst);
    void ExtractColorShadow(int src1, int src2, int dst);
    void EncodeRGBM(int src, int dst, float scale, bool filter=false);
    void MeasureMaxColor(int src, int rt64x64, int rt16x16, int rt4x4, int rt1x1, CVector4 &maxColor);
    void EncodeMaxColor(int src, int dst, const CVector4 &maxColor, bool filter=false);
    void ComputeBasisStrength(int src, int dst);

    void ChangeRenderTarget(int rt, bool clear=false);
    void DrawQuad(int srcRT=0);
    void DrawQuad(const CVector2 offsets[], int numOffsets, int srcRT=0);

    CRenderer *m_pRenderer;

    CFillColorShader *m_pFillColorShader;
    CFillTextureShader *m_pFillTextureShader;
    CFillTextureWithAlphaShader *m_pFillTextureWithAlphaShader;
    CFillTextureWithMultiplierAndAdderShader *m_pFillTextureWithMultiplierAndAdderShader;
    CGammaTextureShader *m_pGammaTextureShader;
    CDownSample4x4Shader *m_pDownSample4x4Shader;
    CDownSample3x3Shader *m_pDownSample3x3Shader;
    CDownSample2x2Shader *m_pDownSample2x2Shader;
    CDownSampleMax4x4Shader *m_pDownSampleMax4x4Shader;
    CDownSampleMax2x2Shader *m_pDownSampleMax2x2Shader;
    CDownSampleMin4x4Shader *m_pDownSampleMin4x4Shader;
    CDownSampleMin2x2Shader *m_pDownSampleMin2x2Shader;
    CNearestDepthUpSampleShader *m_pNearestDepthUpSampleShader;
    CGaussianFilter3x3Shader *m_pGaussianFilter3x3Shader;
    CGaussianFilter5x5Shader *m_pGaussianFilter5x5Shader;
    CGaussianFilter15x15Shader *m_pGaussianFilter15x15Shader;
    CBrightPassShader *m_pBrightPassShader;
    CDownSample4x4BilinearShader *m_pDownSample4x4BilinearShader;
    CGaussianFilterBilinearShader *m_pGaussianFilterBilinearShaders[CGaussianFilterBilinearShader::MAX_RADIUS];
    CDofFilterBilinearShader *m_pDofFilterBilinearShaders[CDofFilterBilinearShader::MAX_RADIUS];
    CBlurLightMapShader *m_pBlurLightMapShader;
    CEdgePaddingShader *m_pEdgePaddingShader;
    CExtractMonoShadowShader *m_pExtractMonoShadowShader;
    CExtractColorShadowShader *m_pExtractColorShadowShader;
    CEncodeRGBMShader *m_pEncodeRGBMShader;
    CComputeBasisIntensityShader *m_pComputeBasisIntensityShader;

private:
    void ClearShaders();
    std::vector<CShader*> m_NotCachedShaders;
};



}//GELib

#endif//__GELIB_RENDERER_IMAGEPROCESSOR_H__
