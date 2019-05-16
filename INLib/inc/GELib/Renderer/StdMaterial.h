//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/StdMaterial.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_STDMATERIAL_H__
#define __GELIB_RENDERER_STDMATERIAL_H__

#include <string>
#include <vector>

#include <GELib/GeConfig.h>
#include "RenderMaterial.h"
#include <GELib/Math/GeMath.h>
#include <GELib/GraphicDevice/GraphicDeviceTypes.h>
#include <GELib/Utility/VectorMap.h>


namespace GELib
{

const int MATERIALCLASS_STD = 1;

class CStdMaterial : public CRenderMaterial
{
public:
    GELIB_API CStdMaterial();
    GELIB_API virtual ~CStdMaterial();
	GELIB_API virtual int GetClassId() { return MATERIALCLASS_STD; }

    GELIB_API virtual void CheckShaderProperties();
    GELIB_API virtual CMeshShader* GetShader(EShaderFunction func, CRenderMesh *mesh, ELightMapMode lightMapMode);
    GELIB_API virtual void ClearShaders();

    CStdMaterial *m_pBaseMaterial;

    std::string m_DiffuseChannel;
    std::string m_SpecularChannel;
    std::string m_SpecularPowerChannel;
    std::string m_EmissiveChannel;
    std::string m_NormalChannel;
    std::string m_AlphaChannel;
    std::string m_AlphaMaskChannel;
    std::string m_DistortionChannel;
    std::string m_GlowChannel;
    std::string m_LightingChannel;
    std::string m_CustomCode;
    std::string m_RegisterChannel[2];

    float m_fAnimationSpeed;
    float m_fAnimationTimeOffset;
    int   m_iPhysicalProperty;

    class TextureInfo
    {
    public:
        TextureInfo() { m_iTexture=0; m_eAddressMode=TEXADDRESS_WRAP; m_eFilter=TEXFILTER_LINEAR;
                        m_bGammaCorrection=true; m_fMipmapLodBias = 0;}
        int m_iTexture;
        ETextureAddressMode m_eAddressMode;
        ETextureFilter m_eFilter;
        bool m_bGammaCorrection;
        float m_fMipmapLodBias;
    };

	void CheckHasAnimatedTexture();
    void SetAnimatedTextureIndex(float indexf);

    std::vector<TextureInfo> m_Textures;
    std::vector<CVector4> m_Vectors;

    short m_iNumTexCoords;	// computed by CheckShaderProperties(), cleared by ClearShaders()
	bool m_bHasAnimatedTexture;

protected:

	bool IsVertexFog(EShaderFunction func);
	bool IsShaderDepthTest(EShaderFunction func);
	bool IsShaderAlphaTest(EShaderFunction func);
	bool IsShaderAlphaMask(EShaderFunction func);

    std::string  GenerateShaderKey(const char *shaderName, EShaderFunction func, CRenderMesh *mesh, ELightMapMode lightMapMode);
    const char * GenerateShaderCode(const char *shaderName, EShaderFunction func, CRenderMesh *mesh, ELightMapMode lightMapMode);
    const char * GenerateDefaultMaterialShaderCode(const char *shaderName, EShaderFunction func, CRenderMesh *mesh, ELightMapMode lightMapMode);
    CMeshShader* GenerateShader(const char *shaderName, EShaderFunction func, CRenderMesh *mesh, ELightMapMode lightMapMode);
    CMeshShader* GetShaderFromCache(int hash);
    void PutShaderInCache(CMeshShader *shader, int hash);

    int m_iShaderSettingsChangeCount;

	typedef VectorMap<int, CMeshShader*> ShaderCache;
    ShaderCache m_ShaderCache;
};

inline CMeshShader* CStdMaterial::GetShaderFromCache(int hash)
{
	CMeshShader *s = 0;
	m_ShaderCache.Get(hash, s);
	return s;
}

inline void CStdMaterial::PutShaderInCache(CMeshShader *shader, int hash)
{
    if (shader)
		m_ShaderCache.Insert(hash, shader);
}


}//GELib

#endif//__GELIB_RENDERER_STDMATERIAL_H__
