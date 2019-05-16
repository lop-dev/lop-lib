//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiMask.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIMASK_H__
#define __GELIB_UI_UIMASK_H__

#include "UiObject.h"

namespace GELib
{


class CUiTexture;

class CUiMask : public CUiObject
{
public:

	UICLASS_DECLARE(CUiMask);

	CUiMask(CUiSystem *system);
	virtual ~CUiMask();

	bool SetTexture(const char *name);
	CUiTexture* GetTexture();
	void DeleteTexture();

	// external texture or render target managed by other system
	void SetExternalTexture(int t) { m_iExternalTextureId = t; }
	int GetExternalTexture() { return  m_iExternalTextureId; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void Draw(IPlotter2D &canvas);
	virtual void PostDraw(IPlotter2D &canvas);

protected:

	CUiTexture *m_pTexture;
	int m_iExternalTextureId;
};



}//GELib

#endif//__GELIB_UI_UIMASK_H__
