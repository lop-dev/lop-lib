//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiAnimation.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIANIMATION_H__
#define __GELIB_UI_UIANIMATION_H__

#include <string>
#include <vector>
#include <GELib/Math/InterpolationValue.h>

namespace GELib
{

static const short UIFILE_VERSION_ANIMATION = 105;

class CIOStream;
class CUiObject;

class CUiAnimationClip
{
public:
	CUiAnimationClip();

    bool Save(CIOStream &stream);
    bool Load(short version, CIOStream &stream);

	float AdvanceTime(float t, float dt);

	std::string m_Name;
	float m_fBeginTime;
	float m_fEndTime;
    float m_fLoopTime;
	bool m_bLoop;
	bool m_bAutoPlay;
};


class CUiAnimationTrack
{
public:
	CUiAnimationTrack() { m_pTarget = 0; }
	virtual ~CUiAnimationTrack() {}

    virtual bool Save(CIOStream &stream);
    virtual bool Load(short version, CIOStream &stream);

	bool IsEmpty();

	std::string m_Name;
	CUiObject *m_pTarget;

	CInterpolationBool m_VisibleKeys;
	CInterpolationVector2 m_PositionKeys;
	CInterpolationVector2 m_SizeKeys;
	CInterpolationColor m_ColorKeys;
	CInterpolationFloat m_OpacityKeys;
	CInterpolationVector2 m_TextureCoordKeys;	// for CUiImage
	CInterpolationInt m_IndexKeys;			// for CUiImage, CUiImagePresenter
	CInterpolationBool m_GrayscaleKeys;		// for CUiImage, CUiImagePresenter
	CInterpolationString m_CommandKeys;		// for CUiCommander
	CInterpolationColor m_Color2Keys;		// for CImagePresenter SetOutlineColor
	CInterpolationColor m_Color3Keys;		// for CImagePresenter SetGradientColor
	CInterpolationColor m_Color4Keys;		// for CImagePresenter SetShadowColor
};


class CUiAnimation
{
public:

	CUiAnimation();
	virtual ~CUiAnimation();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream);

	// convert target from name to pointer
	void BindTrack(CUiObject *u);
	// convert target from pointer to name
	void UpdateBindedTrackName();

	// apply animation to target
	void Apply(float t);

	bool IsEmpty();
	void SortClip();
	void SortTrack();

	CUiAnimationClip* FindClip(const char *name);
	CUiAnimationClip* FindClipByTime(float t);
	CUiAnimationClip* FindAutoPlayClip();
	void DeleteClip(CUiAnimationClip *c);

	CUiAnimationTrack* FindTrack(const char *name);
	CUiAnimationTrack* FindTrack(CUiObject *u);
	void DeleteTrack(CUiAnimationTrack *t);

	bool m_bHasCommand;
	std::vector<CUiAnimationClip> m_Clips;
	std::vector<CUiAnimationTrack> m_Tracks;
};

class CUiAnimationPlayState
{
public:
	CUiAnimationPlayState();
	void Reset();
	void ResetTime();
	void AdvanceTime(float t);
	void ExecuteCommand(float t0, float t1);

	CUiAnimation *m_pAnimation;
	CUiAnimationClip *m_pClip;
	float m_fTime;
	int m_iFrameCounter;
};


}//GELib

#endif//__GELIB_UI_UIANIMATION_H__
