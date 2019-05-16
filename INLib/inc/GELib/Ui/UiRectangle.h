//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiRectangle.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIRECTANGLE_H__
#define __GELIB_UI_UIRECTANGLE_H__

#include "UiObject.h"

namespace GELib
{

class CUiTexture;

class CUiRectangle : public CUiObject
{
public:

	UICLASS_DECLARE(CUiRectangle);

	CUiRectangle(CUiSystem *system);
	virtual ~CUiRectangle();

	void SetBlendMode(EUiBlendMode b);
	EUiBlendMode GetBlendMode();

	void UseControlColor(bool use) { m_bUseControlColor = use; }
	bool IsUseControlColor() { return m_bUseControlColor; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void Draw(IPlotter2D &canvas);

protected:

	bool m_bUseControlColor;
	EUiBlendMode m_eBlendMode;
};



}//GELib

#endif//__GELIB_UI_UIRECTANGLE_H__
