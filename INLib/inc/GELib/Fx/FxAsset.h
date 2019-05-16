//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXASSET_H__
#define __GELIB_FX_FXASSET_H__

#include <vector>
#include <string>

#include <GELib/Ext/BaseAsset.h>
#include <GELib/Math/GeMath.h>

namespace GELib
{

class CIOStream;
class CMaterialAsset;
class CMeshAsset;
class CAssetHelper;
class CFxManager;
class CFxComponentResource;

const int ASSETCLASS_FX = 10;

class CFxAsset : public CBaseAsset
{
public:
    CFxAsset(CFxManager *manager, const char *cacheKey=0);
    virtual ~CFxAsset();

	virtual int GetClassId() { return ASSETCLASS_FX; }
    CFxManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
	bool Load(CIOStream &stream, const CLoadParameter &param);
    void Unload();
	virtual bool CreateLoadedResource();

    CFxComponentResource* CreateComponent(int classId);
    void DeleteComponent(CFxComponentResource *c);

	int GetNumComponents() { return (int)m_Components.size(); }
    CFxComponentResource* GetComponent(int index);
    CFxComponentResource* GetComponentByName(const char *name);

    int GetComponentIndex(CFxComponentResource *c);
    void SetComponentIndex(CFxComponentResource *c, int index);
    short GetComponentSubDrawOrder(CFxComponentResource *c);

    void GetAssets(CAssetHelper &helper);

    int IncreaseChangeCounter() { return ++m_iChangeCounter; }
    int GetChangeCounter() { return m_iChangeCounter; }

	void SetLoop(bool loop) { m_bLoop = loop; }
	bool GetLoop() { return m_bLoop; }
	void SetUseLoopInterval(bool use) { m_bUseLoopInterval = use; }
	bool GetUseLoopInterval() { return m_bUseLoopInterval; }
	void SetLoopInterval(float loopBegin, float loopEnd) { m_fLoopIntervalBegin = loopBegin; m_fLoopIntervalEnd = loopEnd; }
	void GetLoopInterval(float &loopBegin, float &loopEnd) { loopBegin = m_fLoopIntervalBegin; loopEnd = m_fLoopIntervalEnd; }
	void SetLoopCount(int count) { m_iLoopCount = count; }
	int GetLoopCount() { return m_iLoopCount; }
	void  SetMaxVisibleDistanceRatio(float s) { m_fMaxVisibleDistanceRatio = s; }
    float GetMaxVisibleDistanceRatio() { return m_fMaxVisibleDistanceRatio; }

	void SetBoundingBox(const CAABox &box) { m_Box = box; }
	const CAABox& GetBoundingBox() { return m_Box; }

    void ComputeTimeRange();
	void GetTimeRange(float &beginTime, float &endTime) { beginTime = m_fBeginTime; endTime = m_fEndTime; }

    bool IsAttack() { return m_bAttack; }
	bool HasComponent(int classId) { return (m_iClassIds & (1 << (classId - 1))) != 0; }

private:

    bool m_bLoop;
    bool m_bUseLoopInterval;
    bool m_bAttack;
	int m_iLoopCount;
    float m_fBeginTime;
    float m_fEndTime;
    float m_fLoopIntervalBegin;
    float m_fLoopIntervalEnd;
    float m_fMaxVisibleDistanceRatio;
	int m_iChangeCounter;
	int m_iClassIds;
    CAABox m_Box;
    std::vector<CFxComponentResource*> m_Components;

    CFxManager *m_pManager;
	bool m_bNeedCreateLoadedResource;

};



}//GELib

#endif//__GELIB_FX_FXASSET_H__
