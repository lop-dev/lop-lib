//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/ShaderSettings.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_SHADERSETTINGS_H__
#define __GELIB_RENDERER_SHADERSETTINGS_H__

#include <GELib/GeConfig.h>
#include <string>

namespace GELib
{


class CShaderSettings
{
public:

    GELIB_API CShaderSettings();

    GELIB_API const char * GetVertexShaderProfile() const;
    GELIB_API const char * GetPixelShaderProfile() const;

    GELIB_API char const * const * GetDefines() const;

    GELIB_API int GetChangeCount() const;

    GELIB_API void SetVersion(int version);
    GELIB_API int GetVersion() const;

    GELIB_API void SetFullPrecision(bool fp);
    GELIB_API bool IsFullPrecision() const;

private:

    void ProcessDefine();

    int m_iShaderVersion;
    bool m_bFullPrecision;

    enum DefineName
    {
        DEFINE_SHADER_VERSION,
        DEFINE_FULL_PRECISION,
        DEFINE_PLATFORM,
        DEFINE_NUMBER
    };
    std::string m_DefineName[DEFINE_NUMBER];
    std::string m_DefineValue[DEFINE_NUMBER];
    char const *m_DefineOutput[DEFINE_NUMBER * 2 + 2];
    int m_iChangeCounter;
};


GELIB_API CShaderSettings & GetShaderSettings();


}//GELib

#endif//__GELIB_RENDERER_SHADERSETTINGS_H__
