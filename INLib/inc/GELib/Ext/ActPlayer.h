//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/ActPlayer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ACTPLAYER_H__
#define __GELIB_EXT_ACTPLAYER_H__

#include <GELib/Ext/Act.h>
#include <map>
#include <vector>

namespace GELib
{

class CRenderWorld;
class CPhysicsWorld;
class CFxPlayer;
class CLoadParameter;
class CMemoryStream;
class CCharacter;
class CFx;
class CActAsset;

class CActPlayer
{
public:

    CActPlayer();
    virtual ~CActPlayer();

    void Initialize(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, CFxPlayer *fxPlayer);

    bool Play(CActAsset *a);
    bool Play(const char *fname, const CLoadParameter &param);
	void Stop();
	bool IsStopped();

	void Update(float t);

	float GetTime() { return m_fTime; }

	// editor use
	void SetFastUpdate(bool enable) { m_bFastUpdate = enable; }
	bool IsFastUpdate() { return m_bFastUpdate; }

	// act use
	class CActorCharacter
	{
	public:
		CActorCharacter() { m_pCharacter = 0; }
		CActorCharacter(CCharacter *c) { m_pCharacter = c; }
		void AppendFx(unsigned short id);
		void RemoveFx(unsigned short id);
		void ReattachFx(CActPlayer *player);
		void DetachFx(CActPlayer *player);

		CCharacter *m_pCharacter;
		std::vector<unsigned short> m_Fxs;
	};

	class CActorFx
	{
	public:
		CActorFx() { m_pFx = 0; m_iAttachId[0] = m_iAttachId[1] = 0; }
		CActorFx(CFx *fx) { m_pFx = fx; m_iAttachId[0] = m_iAttachId[1] = 0; }

		CFx *m_pFx;
		unsigned short m_iAttachId[2];
	};

	CActorCharacter* GetCharacter(unsigned short id);
	CActorFx* GetFx(unsigned short id);
	void UpdateCharacter(CCharacter *c, float t);
	const std::string& GetString(unsigned short id);

	void SetCamera(const CMatrix &matrix, float fov, float znear, float zfar);
	bool GetCamera(CMatrix &matrix, float &fov, float &znear, float &zfar);

private:

	typedef std::map<unsigned short, CActorCharacter> CharacterMap;
	CharacterMap m_Characters;
	typedef std::map<unsigned short, CActorFx> FxMap;
	FxMap m_Fxs;

	CMatrix m_CameraMatrix;
	float m_fCameraFov;
	float m_fCameraNear;
	float m_fCameraFar;

	CMemoryStream *m_pStream;
	CActAsset *m_pAsset;
	float m_fTime;
	float m_fFrameTime;
	bool m_bFastUpdate;

    CRenderWorld *m_pRenderWorld;
    CPhysicsWorld *m_pPhysicsWorld;
    CFxPlayer *m_pFxPlayer;
};


}//GELib

#endif//__GELIB_EXT_ACTPLAYER_H__
