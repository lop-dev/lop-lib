//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/ShaderCache.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_SHADERCACHE_H__
#define __GELIB_RENDERER_SHADERCACHE_H__

#include <string>
#include <vector>
#include <map>
#include <GELib/GeConfig.h>
#include <GELib/Utility/DataBuffer.h>

namespace GELib
{


class CShader;

class CShaderCache
{
public:
    GELIB_API CShaderCache();
    GELIB_API virtual ~CShaderCache();

    GELIB_API void SetShaderPath(const char *path);                           // shader code path
    GELIB_API void SetTexturePath(const char *path);                          // texture path
    GELIB_API bool Save(const char *fileName, int version=0, int group=0);    // save cache, version=0: all versions, group=0: all groups
    GELIB_API bool Load(const char *fileName);                                // load cache
    GELIB_API void Clear();                                                   // clear all, must be called before destroy GraphicDevice
	GELIB_API void Drop();													  // remove but not delete cache, used by tool

	GELIB_API void WarmupShader();

    // called by renderer
	GELIB_API const std::string& GetShaderCode(const char *fileName);
    GELIB_API bool GetTexture(const char *fileName, CDataBuffer &textureData);
    GELIB_API CShader* GetShader(const std::string &key);
    GELIB_API void CacheShader(const std::string &key, CShader *shader, bool save=true);

	GELIB_API void SetGroup(int g);
	GELIB_API int GetGroup();

private:

    class CShaderCacheEntry
    {
    public:
        CShaderCacheEntry() {}
        CShaderCacheEntry(CShader *shader, bool save, short group) { m_pShader = shader; m_bSave = save; m_iGroup = group; }

        CShader *m_pShader;
        bool m_bSave;
		short m_iGroup;
    };

    typedef std::map<std::string, std::string> ShaderCodeCache;
    ShaderCodeCache m_ShaderCodeCache;

    typedef std::map<std::string, CShaderCacheEntry> ShaderBinaryCache;
    ShaderBinaryCache m_ShaderBinaryCache;

	std::vector<CShaderCacheEntry> m_DroppedCache;

    std::string m_ShaderPath;
    std::string m_TexturePath;
	short m_iGroup;
};

GELIB_API void SetShaderCache(CShaderCache *c);
GELIB_API CShaderCache* GetShaderCache();

}//GELib

#endif//__GELIB_RENDERER_SHADERCACHE_H__
