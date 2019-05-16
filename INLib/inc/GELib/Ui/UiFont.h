//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiFont.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIFONT_H__
#define __GELIB_UI_UIFONT_H__

#include <GELib/GeConfig.h>
#include <string>

namespace GELib
{

class ITextureFont;

class CUiResourceManager;

class CUiFont
{
public:
	CUiFont(CUiResourceManager *manager=0);
	virtual ~CUiFont();

    bool CreateFreeTypeFont(const char *fileName, int faceIndex, int style, float fontSize, int internalFontSize, int cacheSize, int textureSize);
    bool CreateFreeTypeFont(const void *fileInMemory, int fileSize, int faceIndex, int style, float fontSize, int internalFontSize, int cacheSize, int textureSize);
	bool CreateWindowsFont(const wchar_t *faceName, int style, float fontSize, int internalFontSize, int cacheSize, int textureSize);
	bool CreateWindowsFont(const char *fileName, const wchar_t *faceName, int style, float fontSize, int internalFontSize, int cacheSize, int textureSize);
	bool CreateWindowsFont(const void *fileInMemory, int fileSize, const wchar_t *faceName, int style, float fontSize, int internalFontSize, int cacheSize, int textureSize);
    bool CreateTextureFont(const void *textureFileInMemory, int textureFileSize, const void *abcFileInMemory, int abcFileSize, const char *name=0);
	void DeleteFont();

	float GetSize();
	float GetHeight();
    float GetCharWidth(int c);
	float GetTextWidth(const wchar_t *text, int textLen=-1);
	int GetNearestCharIndex(float x, const wchar_t *text, int textLen=-1);
	int GetNumCharsInWidth(float width, float &textWidth, const wchar_t *text, int textLen=-1);

	float GetEllipsisWidth() { return m_fEllipsisWidth; }
	const wchar_t* GetEllipsisText();

	int m_iId;
	std::string m_Name;
	ITextureFont *m_pFont;
	CUiResourceManager *m_pManager;

private:
	void PrepareFont(ITextureFont *font, float fontSize);
	float m_fSize;
	float m_fHeight;
	float m_fSizeRatio;
	float m_fEllipsisWidth;
};


}//GELib

#endif//__GELIB_UI_UIFONT_H__
