//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiCursor.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICURSOR_H__
#define __GELIB_UI_UICURSOR_H__

#include <vector>
#include <string>

namespace GELib
{

class CursorHandle;
class IPlotter2D;

class CUiTexture;
class CUiResourceManager;

class CUiCursor
{
public:
	CUiCursor(CUiResourceManager *manager);
	virtual ~CUiCursor();

	bool Load(const char *fname);

	void Active();
	void Move();
	void Draw(IPlotter2D &canvas, float animationTime);

	int m_iId;
	int m_iHotspotX;
	int m_iHotspotY;
    float m_fFps;
	std::string m_Name;
	std::vector<CUiTexture*> m_Textures;
	CUiResourceManager *m_pManager;

private:
	int m_iCurTextureId;
	CursorHandle *m_hCursor;
};


}//GELib

#endif//__GELIB_UI_UICURSOR_H__
