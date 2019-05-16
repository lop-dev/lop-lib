//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/TerrainSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_TERRAINSCENEENTITY_H__
#define __GELIB_SCENE_TERRAINSCENEENTITY_H__

#include <string>
#include <map>
#include <GELib/Renderer/RendererTypes.h>
#include "SceneEntity.h"


namespace GELib
{

class CIOStream;
class CAssetHelper;
class CTerrain;
class CTerrainRenderObject;

class CScene;

class CTerrainSceneEntity : public CSceneEntity
{
public:
    CTerrainSceneEntity(CScene *scene);
    virtual ~CTerrainSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_TERRAIN; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

	virtual int GetNumLightMapLods();
    virtual void SetLightMapIndex(int index, int lod);
    virtual int GetLightMapIndex(int lod) const;
    virtual void SetRenderLightMapMode(int mode) { m_iRenderLightMapMode = mode; }
    virtual int GetRenderLightMapMode() const { return m_iRenderLightMapMode; }

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);

    virtual void Update(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

	void Create(int size, float scale, float maxHeight);
	void Resize(int size, float scale, float maxHeight);

    void SetCastDynamicShadow(bool b);
    bool GetCastDynamicShadow() const { return m_bCastDynamicShadow; }
    void SetCastStaticShadow(bool b);
    bool GetCastStaticShadow() const { return m_bCastStaticShadow; }
    void SetShadowResolutionFactor(float r);
    float GetShadowResolutionFactor() const { return m_fShadowResolutionFactor; }
    void SetShadowDepthBias(float b);
    float GetShadowDepthBias() const { return m_fShadowDepthBias; }
    void SetLightGroups(int g);
    int  GetLightGroups() const { return m_iLightGroups; }
	void ShowGrass(bool show);
	bool IsShowGrass() const { return m_bShowGrass; }
    void UpdateRenderObjects();

	void SetCollisionMask(int mask);
	int GetCollisionMask() const;
	void SetPhysicsMaterial(int index, int material);
	int GetPhysicsMaterial(int index);
	void BuildPhysicsMaterialMap();

	int GetNumSections() const;
	CTerrainRenderObject* GetRenderObject(int sectionIndex);

	CTerrain* GetTerrain() { return m_pTerrain; }

protected:

	CTerrain *m_pTerrain;
    int m_iRenderLightMapMode;
    float m_fShadowResolutionFactor;
    float m_fShadowDepthBias;
    int m_iLightGroups;
    bool m_bCastDynamicShadow;
    bool m_bCastStaticShadow;
	bool m_bShowGrass;
	int m_iCollisionMask;
	unsigned short m_PhysicsMaterials[4];

	void CreateSections();

	class CTerrainSectionInfo
	{
	public:
		CTerrainSectionInfo();
		int m_iLightMapIndex;
	};
	std::vector<CTerrainSectionInfo> m_Sections;
};



}//GELib

#endif//__GELIB_SCENE_TERRAINSCENEENTITY_H__
