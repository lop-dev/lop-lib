//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/BaseRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_BASERENDEROBJECT_H__
#define __GELIB_EXT_BASERENDEROBJECT_H__

#include <GELib/Renderer/RenderObject.h>
#include "MaterialAsset.h"

namespace GELib
{

class CConvexVolume;
class CMeshShader;
class CRendererContext;
class CRenderLight;
class CRenderWorld;
class CMaterialAsset;

const int LIGHTMAP_LOD_MAX = 3;

class CRenderWorldRef
{
public:
    CRenderWorldRef() { m_pRenderWorld = 0; m_iVisibilityId = 0; }
    CRenderWorld *m_pRenderWorld;
    int m_iVisibilityId;
};

class CBaseRenderObject : public CRenderObject
{
public:
    CBaseRenderObject();
    virtual ~CBaseRenderObject();

    virtual float ComputeLightOcclusion(GELib::CRenderLight *light);
    void SetIgnoreStaticLight(bool ignore);
    bool GetIgnoreStaticLight();

    void AddToRenderWorld(CRenderWorld *rw);
    void RemoveFromRenderWorld();
    void UpdateInRenderWorld();
    void ForceUpdateInRenderWorld();
	bool IsInRenderWorld();

	void SetLodParent(CBaseRenderObject *parent);	// both object and parent must be in render world

    CRenderWorldRef m_WorldRef;

private:
    bool m_bIgnoreStaticLight;
	unsigned char m_iLightOcclusion;
	unsigned char m_iSmoothLightOcclusion;
	float m_fPrevTime;
};

class CShadowDepthMapInfo
{
public:
	CShadowDepthMapInfo();

	int m_iWidth;
	int m_iHeight;
	CMatrix m_ShadowMatrix;
	const unsigned short *m_pDepth;
};

class CBaseRenderLight : public CRenderLight
{
public:
    CBaseRenderLight();
    virtual ~CBaseRenderLight();

    void SetMatrix(const CMatrix &matrix);

    void SetMaterial(CMaterialAsset *material) { m_pMaterial = material; }
    virtual CRenderMaterial* GetMaterial() { return m_pMaterial ? m_pMaterial->GetRenderMaterial() : 0; }

	void SetMaterialParameter(int index, const CVector4 &m) { m_MaterialParameters[index] = m; }
    virtual void GetMaterialParameter(int index, CVector4 &m) { m = m_MaterialParameters[index]; }

	void SetShadowDepthMap(CShadowDepthMapInfo *m) { m_pShadowDepthMap = m; }
	CShadowDepthMapInfo* GetShadowDepthMap() { return m_pShadowDepthMap; }
	bool IsInShadow(const CVector3 &p);

    void AddToRenderWorld(CRenderWorld *rw);
    void RemoveFromRenderWorld();
    void UpdateInRenderWorld();
    void ForceUpdateInRenderWorld();
	bool IsInRenderWorld();

    CRenderWorldRef m_WorldRef;

private:
	CVector4 m_MaterialParameters[2];
    CMaterialAsset *m_pMaterial;
	CShadowDepthMapInfo *m_pShadowDepthMap;
};

class CBaseRenderOccluder : public CRenderOccluder
{
public:
    CBaseRenderOccluder();
    virtual ~CBaseRenderOccluder();

    void SetMatrix(const CMatrix &matrix);

    bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    bool IntersectConvexVolume(const CConvexVolume &c);

    void AddToRenderWorld(CRenderWorld *rw);
    void RemoveFromRenderWorld();
    void UpdateInRenderWorld();
	bool IsInRenderWorld();

    CRenderWorldRef m_WorldRef;
};

class CBaseRenderPortal : public CRenderPortal
{
public:
    CBaseRenderPortal();
    virtual ~CBaseRenderPortal();

    void SetMatrix(const CMatrix &matrix);

    bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    bool IntersectConvexVolume(const CConvexVolume &c);

    void AddToRenderWorld(CRenderWorld *rw);
    void RemoveFromRenderWorld();
    void UpdateInRenderWorld();
	bool IsInRenderWorld();

    CRenderWorldRef m_WorldRef;
};

class CBaseRenderZone : public CRenderZone
{
public:
    CBaseRenderZone();
    virtual ~CBaseRenderZone();

    void SetMatrix(const CMatrix &matrix);

    bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    bool IntersectConvexVolume(const CConvexVolume &c);

    void AddToRenderWorld(CRenderWorld *rw);
    void RemoveFromRenderWorld();
    void UpdateInRenderWorld();
	bool IsInRenderWorld();

    CRenderWorldRef m_WorldRef;
};


void DeleteRenderObject(CBaseRenderObject *obj);
void DeleteRenderObject(CBaseRenderLight *obj);
void DeleteRenderObject(CBaseRenderOccluder *obj);
void DeleteRenderObject(CBaseRenderPortal *obj);
void DeleteRenderObject(CBaseRenderZone *obj);


}//GELib

#endif//__GELIB_EXT_BASERENDEROBJECT_H__
