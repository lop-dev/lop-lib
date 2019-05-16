//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiPolygon.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIPOLYGON_H__
#define __GELIB_UI_UIPOLYGON_H__

#include "UiObject.h"

namespace GELib
{


class CUiTexture;

class CUiPolygon : public CUiObject
{
public:

	UICLASS_DECLARE(CUiPolygon);

	CUiPolygon(CUiSystem *system);
	virtual ~CUiPolygon();

	void SetPolygon(const CVector2 *v, int sides);
	int GetNumSides();
	const CVector2* GetVertices();

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
	std::vector<CVector2> m_Polygon;
	std::vector<short> m_TriangleIndices;
};



}//GELib

#endif//__GELIB_UI_UIPOLYGON_H__
