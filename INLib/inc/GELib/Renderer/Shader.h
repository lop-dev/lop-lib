//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/Shader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_SHADER_H__
#define __GELIB_RENDERER_SHADER_H__

#include <string>
#include <vector>
#include <GELib/GeConfig.h>
#include <GELib/GraphicDevice/GraphicDevice.h>
#include <GELib/Math/GeMath.h>
#include "ShaderFunction.h"

namespace GELib
{

class CShaderCompiler;
class CShader;


class CShaderParameter
{
public:

    GELIB_API CShaderParameter();

    GELIB_API void SetFloats(const float *f);
    GELIB_API void SetFloatsArray(const float *f, int arrayIndex, int registerCount);
    GELIB_API void SetFloat(float f);
    GELIB_API void SetVector(const CVector2 &v);
    GELIB_API void SetVector(const CVector3 &v);
    GELIB_API void SetVector(const CVector4 &v);
    GELIB_API void SetMatrix(const CMatrix &m);
    GELIB_API void SetTMatrix(const CMatrix &m);
    GELIB_API void SetTexture(int texture,
                    ETextureAddressMode addrMode=TEXADDRESS_WRAP,
                    ETextureFilter filter=TEXFILTER_LINEAR,
                    float mipmapLodBias=0,
                    int arrayIndex=0);

    GELIB_API bool IsActive() const { return m_iPixelRegisterIndex>=0 || m_iVertexRegisterIndex>=0; }

    short m_iVertexRegisterIndex;
    short m_iVertexRegisterCount;
    short m_iPixelRegisterIndex;
    short m_iPixelRegisterCount;
    const char *m_pName;
    CShaderParameter *m_pNext;
};

class CUnmappedShaderParameter
{
public:
    short m_iVertexRegisterIndex;
    short m_iVertexRegisterCount;
    short m_iPixelRegisterIndex;
    short m_iPixelRegisterCount;
};

class CShader
{
public:

    GELIB_API CShader();
    GELIB_API virtual ~CShader();

    GELIB_API virtual bool Initialize();

    GELIB_API void AddParameter(CShaderParameter &p, const char *name);

    GELIB_API bool CompileFile(const char *vsFileName, const char *vsFuncName, const char *vsProfile,
                const char *psFileName, const char *psFuncName, const char *psProfile,
                const char *extraCode="", int compilerOptions=0, std::string *errorMsg=0);
    GELIB_API bool CompileText(const char *vsSrcCode, const char *vsFuncName, const char *vsProfile,
                const char *psSrcCode, const char *psFuncName, const char *psProfile,
                const char *extraCode="", int compilerOptions=0, std::string *errorMsg=0);
    bool CreateFromExistShader(int vertexShader, int pixelShader, const CUnmappedShaderParameter *params, int numParams);
    bool MapParameters();

    void SetShader();

    // used by shader cache
    int  GetVertexShader();
    int  GetPixelShader();
    int  GetNumParameters();
    int  GetNumUnmappedParameters();
    const CShaderParameter* GetParameterList();
    const CUnmappedShaderParameter* GetUnmappedParameters();

	EShaderFunction GetFunction() { return m_eFunction; }

protected:

    bool gl_CompileText(const char *vsSrcCode, const char *vsFuncName, const char *psSrcCode, const char *psFuncName, const char *extraCode="");

    void UninitializeShader();
    void InitializeParameters(const CShaderCompiler &sc, bool vertexParam);
    void gl_InitializeParameters();

	EShaderFunction m_eFunction;
    int m_iVertexShader;
    int m_iPixelShader;
    int m_iGlProgram;
    CShaderParameter *m_pParamList;
    std::vector<CUnmappedShaderParameter> m_UnmappedParameters;
};

inline void CShaderParameter::SetFloats(const float *f)
{
    if (m_iVertexRegisterIndex >= 0)
    {
        //assert(m_iVertexRegisterCount > 0);
        GetGraphicDevice()->SetVertexShaderConstant(m_iVertexRegisterIndex, f, m_iVertexRegisterCount);
    }
    if (m_iPixelRegisterIndex >= 0)
    {
        //assert(m_iPixelRegisterCount > 0);
        GetGraphicDevice()->SetPixelShaderConstant(m_iPixelRegisterIndex, f, m_iPixelRegisterCount);
        GetGraphicDevice()->gl_SetProgramConstant(m_iPixelRegisterIndex, f, m_iPixelRegisterCount);
    }
}

inline void CShaderParameter::SetFloat(float f)
{
    float fa[4] = { f, 0, 0, 0 };
    SetFloats(fa);
}

inline void CShaderParameter::SetVector(const CVector2 &v)
{
    float fa[4] = { v.x, v.y, 0, 1 };
    SetFloats(fa);
}

inline void CShaderParameter::SetVector(const CVector3 &v)
{
    float fa[4] = { v.x, v.y, v.z, 1 };
    SetFloats(fa);
}

inline void CShaderParameter::SetVector(const CVector4 &v)
{
    SetFloats(v.m_v);
}

inline void CShaderParameter::SetMatrix(const CMatrix &m)
{
	CMatrix tm = m.Transpose();
    SetFloats(tm.m_m16);
}

inline void CShaderParameter::SetTMatrix(const CMatrix &m)
{
    SetFloats(m.m_m16);
}

inline int CShader::GetVertexShader()
{
    return m_iVertexShader;
}

inline int CShader::GetPixelShader()
{
    return m_iPixelShader;
}



}//GELib

#endif//__GELIB_RENDERER_SHADER_H__
