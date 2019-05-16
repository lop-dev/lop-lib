//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UITYPES_H__
#define __GELIB_UI_UITYPES_H__

namespace GELib
{

enum EUiBlendMode
{
	UI_BLEND_NONE,
	UI_BLEND_ALPHA,
	UI_BLEND_ADDITIVE
};

enum EOrientation
{
	ORIENTATION_HORIZONTAL,
	ORIENTATION_VERTICAL
};

enum ESelectionMode
{
	SELECTION_MODE_NONE,
	SELECTION_MODE_SINGLE,
	SELECTION_MODE_MULTIPLE
};

enum EScrollBarVisible
{
	SCROLLBAR_VIS_NONE,
	SCROLLBAR_VIS_HIDDEN,
	SCROLLBAR_VIS_VISIBLE,
	SCROLLBAR_VIS_AUTO,
};

enum ETextLineMode
{
	TEXT_SINGLE_LINE,
	TEXT_MULTI_LINE,
	TEXT_MULTI_LINE_NO_WORDWRAP,
};

enum EImageGridMode
{
	IMAGE_GRID_STRETCH,
	IMAGE_GRID_TILE,
	IMAGE_GRID_NINE_GRID,	// 9 grid
	IMAGE_GRID_BORDER		// 9 grid without center
};


}//GELib

#endif//__GELIB_UI_UITYPES_H__
