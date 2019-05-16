//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiAlignment.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIALIGNMENT_H__
#define __GELIB_UI_UIALIGNMENT_H__

#include <GELib/Math/GeMath.h>

namespace GELib
{

class CIOStream;


enum EAlignment
{
	ALIGNMENT_NONE,
	ALIGNMENT_LEFT,
	ALIGNMENT_RIGHT,
	ALIGNMENT_TOP,
	ALIGNMENT_BOTTOM,
	ALIGNMENT_CENTER,
	ALIGNMENT_STRETCH,
};

class CUiObject;

class CUiAlignment
{
public:
	CUiAlignment();
	~CUiAlignment() {}

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream);

	void Apply(CUiObject *u);
	void Apply(const CVector2 &parentSize, CVector2 &pos, CVector2 &size);

	void AlignLeft(float d=0);
	void AlignRight(float d=0);
	void AlignTop(float d=0);
	void AlignBottom(float d=0);
	void AlignHorizontalCenter(float d=0);
	void AlignVerticalCenter(float d=0);
	void AlignCenter(float hd=0, float vd=0);
	void StretchHorizontal(float left=0, float right=0);
	void StretchVertical(float top=0, float bottom=0);
	void Stretch(float left=0, float right=0, float top=0, float bottom=0);
	void DisableHorizontal();
	void DisableVertical();
	void Disable();

	EAlignment m_eHorizontalAlign;
	EAlignment m_eVerticalAlign;
	float m_fLeft, m_fTop, m_fRight, m_fBottom;
	bool m_bHorizontalPercent;
	bool m_bVerticalPercent;
};


}//GELib

#endif//__GELIB_UI_UIALIGNMENT_H__
