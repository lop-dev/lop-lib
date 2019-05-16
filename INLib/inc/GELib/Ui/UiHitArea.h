//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiHitArea.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIHITAREA_H__
#define __GELIB_UI_UIHITAREA_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include "UiAlignment.h"

namespace GELib
{

class IPlotter2D;
class CIOStream;


class CUiHitArea
{
public:

	CUiHitArea() {}
	~CUiHitArea() {}

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream);

	bool IsHit(const CVector2 &p, const CVector2 &baseSize, int *hitCode=0);
	void Draw(const CVector2 &absPos, const CVector2 &baseSize, IPlotter2D &canvas);

	class Shape
	{
	public:
		Shape();
		~Shape();

		bool Save(CIOStream &stream);
		bool Load(CIOStream &stream);

		bool IsHit(const CVector2 &p, const CVector2 &baseSize, int *hitCode=0);
		void Draw(int index, const CVector2 &absPos, const CVector2 &baseSize, IPlotter2D &canvas);

		void SetPolygon(const CVector2 *v, int sides);

		int m_iCode;
		CVector2 m_Position;
		CVector2 m_Size;
		std::vector<CVector2> m_Polygon;
		CUiAlignment m_Alignment;
	};

	std::vector<Shape> m_Shapes;
};


}//GELib

#endif//__GELIB_UI_UIHITAREA_H__
