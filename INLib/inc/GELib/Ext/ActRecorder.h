//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/ActRecorder.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ACTRECORDER_H__
#define __GELIB_EXT_ACTRECORDER_H__

#include <set>
#include <map>
#include <GELib/Utility/SystemUtility.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Ext/Act.h>

namespace GELib
{

class CFileStream;
class CCharacter;
class CFx;
class CActAsset;
class CMemoryWriteStream;
class CBaseAsset;

class CActRecorder
{
public:

    CActRecorder();
    virtual ~CActRecorder();

	void Enable(bool enable) { 	m_bEnable = enable; }
	bool IsEnabled() { return m_bEnable; }

	void Start();
	bool Stop(const char *saveName=0);

	void SetFpsLimit(float f) { m_fFpsLimit = f; }		// 0: no limit
	float GetFpsLimit() { return m_fFpsLimit; }

	void CreateCharacter(CCharacter *c);
	void DeleteCharacter(CCharacter *c);

	void CreateFx(CFx *fx);
	void DeleteFx(CFx *fx);

	void SetCamera(const CMatrix &matrix, float fov, float znear, float zfar);
	void GetCamera(CMatrix &matrix, float &fov, float &znear, float &zfar);

	void Update(float t);

	unsigned short AppendString(const std::string &s);
	unsigned short AppendString(CBaseAsset *a);
	unsigned short GetCharacterId(CCharacter *c);

private:

	typedef std::set<CCharacter*> CharacterSet;
	CharacterSet m_Characters;
	typedef std::set<CFx*> FxSet;
	FxSet m_Fxs;

	typedef std::map<CCharacter*, CActCharacter> CharacterActionMap;
	CharacterActionMap m_CharacterActs;
	typedef std::map<CFx*, CActFx> FxActionMap;
	FxActionMap m_FxActs;

	CMemoryWriteStream *m_pStream;
	CActAsset *m_pAsset;

	CActCamera m_ActCamera;
	CMatrix m_CameraMatrix;
	float m_fCameraFov;
	float m_fCameraNear;
	float m_fCameraFar;

	CCriticalSectionObject m_CS;
	bool m_bEnable;
	unsigned short m_iIdCounter[2];
	float m_fFpsLimit;
	float m_fFrameTime;
	int m_iFrameCounter;
};

class CDisableActRecorder
{
public:
	CDisableActRecorder();
	~CDisableActRecorder();

private:

	CActRecorder *m_pRecorder;
	bool m_bEnable;
};


void SetActRecorder(CActRecorder *a);
CActRecorder* GetActRecorder();

}//GELib

#endif//__GELIB_EXT_ACTRECORDER_H__
