//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiResourceManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIRESOURCEMANAGER_H__
#define __GELIB_UI_UIRESOURCEMANAGER_H__

#include <string>
#include <vector>
#include <map>
#include <GELib/Utility/StringMap.h>
#include <GELib/Utility/DataBuffer.h>

namespace GELib
{


class CUiTexture;
class CUiCursor;
class CUiFont;
class CUiSubTexture;
class CUiTextureSet;
class CDataBuffer;

class CUiFileData
{
public:
	std::string m_Name;
	CDataBuffer m_Data;
};

class CUiResourceStatistic
{
public:
	CUiResourceStatistic();
	void Reset();

	int m_iNumTextures;
	int m_iTextureSize;
};

class CUiResourceManager
{
public:
	CUiResourceManager();
	virtual ~CUiResourceManager();

	void SetBasePath(const char *path);
	const std::string& GetBasePath() { return m_BasePath; }
	void SetFontPath(const char *path);
	const std::string& GetFontPath() { return m_FontPath; }
	void SetCursorPath(const char *path);
	const std::string& GetCursorPath() { return m_CursorPath; }

	bool LoadFile(const char *fileName, CDataBuffer &data);
	bool SaveFile(const char *fileName, const void *data, int dataSize);

	bool LoadFreeTypeFont(int id, const char *fontName, const char *fileName, int faceIndex, int style, int fontSize, int cacheSize, int textureSize, bool preload);
	bool LoadWindowsFont(int id, const char *fontName, const char *fileName, const wchar_t *faceName, int style, int fontSize, int cacheSize, int textureSize, bool preload);
    bool LoadTextureFont(int id, const char *fontName, const char *ddsFileName, const char *abcFileName);
	bool LoadDirectXCursor(int id, const char *cursorName, const char **textureNames, int numTextures, float fps, int hotspotx, int hotspoty);
	bool LoadWindowsCursor(int id, const char *cursorName, const char *fileName);

	void SetResolution(int windowWidth, int windowHeight, int plotterWidth, int plotterHeight);
	void GetResolution(int &windowWidth, int &windowHeight, int &plotterWidth, int &plotterHeight);
	bool LoadResources(const char *resName);

	CUiTexture* LoadTexture(const char *fileName, bool loadResouce=true);
	void DeleteTexture(CUiTexture *tex);

	CUiTextureSet* CreateEmptyTextureSet(const char *name);
	bool SaveTextureSet(const char *fileName, CUiTextureSet *ts);
	CUiTextureSet* LoadTextureSet(const char *fileName, bool loadTexture=true);
	void DeleteTextureSet(CUiTextureSet *ts);
	CUiTextureSet* GetTextureSet(const char *name);

	CUiFont* GetFont(int id);
	CUiCursor* GetCursor(int id);

	// tool support
	void GetFontIds(std::vector<int> &ids);
	void GetCursorIds(std::vector<int> &ids);
	void GetTextureSets(std::vector<CUiTextureSet*> &ts);
	void ForceLoadResource();		// ignore loadTexture, loadResouce flag

	// statistic
	const CUiResourceStatistic* GetStatistic();

private:

	bool LoadFreeTypeFont(CUiFont *font, const char *fileName, int faceIndex, int style, int fontSize, int cacheSize, int textureSize, bool preload);
	bool LoadWindowsFont(CUiFont *font, const char *fileName, const wchar_t *faceName, int style, int fontSize, int cacheSize, int textureSize, bool preload);
    bool LoadTextureFont(CUiFont *font, const char *ddsFileName, const char *abcFileName);

	CUiFileData* LoadFile(const char *fileName, bool cache=false);	// if cache == false, user is responsible to delete file data

	std::string m_BasePath;
	std::string m_FontPath;
	std::string m_CursorPath;

    typedef StringMap<CUiFileData*> FileCache;
	FileCache m_FileCache;

    typedef StringMap<CUiTexture*> TextureCache;
    TextureCache m_TextureCache;

	typedef std::map<int, CUiFont*> FontCache;
	FontCache m_FontCache;

    typedef std::map<int, CUiCursor*> CursorCache;
	CursorCache m_CursorCache;

	typedef StringMap<CUiTextureSet*> TextureSetCache;
	TextureSetCache m_TextureSetCache;

	bool m_bForceLoadResource;
	int m_Resolutions[4];
	float m_fResolutionScale;
	float m_fLoadResolutionScale;
	CUiResourceStatistic m_Statistic;
};


}//GELib

#endif//__GELIB_UI_UIRESOURCEMANAGER_H__
