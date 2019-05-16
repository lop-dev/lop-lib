//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/PerlinNoiseMaker.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_PERLINNOISEMAKER_H__
#define __GELIB_EXT_PERLINNOISEMAKER_H__

#include <string>
#include <GELib/Math/GeMath.h>

namespace GELib
{
class CRenderer;
class CStdMaterial;
class CRendererContext;
class CPerlinNoiseMakerShader;
class CPerlinOctaveMakerShader;
class CTextureAsset;
class CShader;

static const int MAX_OCTAVES = 4;

struct PerlinNoiseDesc
{
	PerlinNoiseDesc()
	{
		m_iWidth = 256;
		m_iHeight= 256;
		m_iOctaveCount = 4;
		m_fFrequence = 2.0f;
		m_fPersistence = 0.68f;
		m_fScale = 1.0f;
		m_fTimeScale = 1.0f;
	}
	int m_iWidth;
	int m_iHeight;
	int m_iOctaveCount;
	float m_fFrequence;
	float m_fPersistence;
	float m_fScale;
	float m_fTimeScale;
};

class CPerlinNoiseMaker
{
public:
	CPerlinNoiseMaker(void);
	~CPerlinNoiseMaker(void);
	bool Create(const PerlinNoiseDesc& _desc);
	void ResetParam(const PerlinNoiseDesc& _desc);
	bool RenderNoise(int _iRenderTarget,CRendererContext& _rc,CRenderer* _pRender);

private:
	bool _RenderOctaves(CRendererContext& _rc,CRenderer* _pRender);
	bool _CreateResource();
	bool _FreeResource();
	CVector2 _GetFrameOffset(int _iFrameIndex);

private:
    CPerlinNoiseMakerShader* m_pNoiseMakerShader;
    CPerlinOctaveMakerShader* m_pOctaveMakeShader;
    std::vector<CShader*> m_NotCachedShaders;

	PerlinNoiseDesc m_desc;

	int m_pOctaveRTs[MAX_OCTAVES];
	float m_pOctaveMultis[MAX_OCTAVES];

	//raw noise texture desc
	CTextureAsset* m_pFrameTexture;

	int m_iFrames;
	int m_iFrameSize;
	int m_iFrameGridWidth;
	int m_iFrameGridHeight;
};
}//GELib
#endif//__GELIB_EXT_PERLINNOISEMAKER_H__
