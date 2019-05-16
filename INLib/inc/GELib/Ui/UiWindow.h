//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiWindow.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIWINDOW_H__
#define __GELIB_UI_UIWINDOW_H__

#include "UiControl.h"
#include "UiThumb.h"

namespace GELib
{


class CUiWindow : public CUiControl
{
public:

	UICLASS_DECLARE(CUiWindow);

	CUiWindow(CUiSystem *system);
	virtual ~CUiWindow();

	void SetPositionLimit(float xmin, float ymin, float xmax, float ymax);
	void GetPositionLimit(float &xmin, float &ymin, float &xmax, float &ymax);

	void SetSizeLimit(float wmin, float hmin, float wmax, float hmax);
	void GetSizeLimit(float &wmin, float &hmin, float &wmax, float &hmax);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);

	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	void StartDrag();
	void StopDrag();
	void Dragging();
	CVector2 ConstrainPosition(const CVector2 &p);
	CVector2 ConstrainSize(const CVector2 &s);

	virtual void OnEvent(CUiEvent &e);

	bool m_bDragging;
	float m_fXMin, m_fYMin, m_fXMax, m_fYMax;
	float m_fWMin, m_fHMin, m_fWMax, m_fHMax;
	CVector2 m_DragPosition;
	EThumbFunction m_eDragFunction;
};



}//GELib

#endif//__GELIB_UI_UIWINDOW_H__
