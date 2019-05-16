//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/Terrain.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAIN_H__
#define __GELIB_EXT_TERRAIN_H__

#include <vector>
#include <map>
#include <GELib/Math/GeMath.h>
#include <GELib/GraphicDevice/GraphicDeviceTypes.h>
#include "TerrainGrass.h"

namespace GELib
{

class CIOStream;
class IPlotter3D;
class CLightMapInfo;
class CPhysicsObject;
class CPhysicsWorld;

class CMeshUtility;
class CBaseAsset;
class CTextureAsset;
class CMaterialAsset;
class CAssetHelper;
class CRenderWorld;
class CTerrainRenderObject;
class CTerrainGrassRenderObject;
class CTerrainGrassMeshCell;
class CLoadParameter;

const int TERRAIN_PATCH_SIZE = 32;
const int TERRAIN_LODS = 4;
const int TERRAIN_EXLODS = 3;
const int TERRAIN_SECTION_LOD = 3;
const int TERRAIN_SECTION_SIZE = 256;

const int TERRAIN_HOLE_MATERIAL_ATTRIB = 15;

class CTerrainAsset;

class CTerrain
{
public:
	CTerrain();
	virtual ~CTerrain();

	void Initialize(CRenderWorld *renderWorld, CPhysicsWorld *physicsWorld, bool editorMode=false);

	void Create(int size, float scale, float maxHeight);
	void Resize(int size, float scale, float maxHeight);

    bool Save(CBaseAsset *container, CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
	void Unload();
	bool CreateLoadedResource();
    bool IsLoading();

    void GetAssets(CAssetHelper &helper);
	bool SaveModifiedAssets(const char *path, const char *name);
	void SetModified(bool m) { m_bModified = m; }
	bool IsModified() { return m_bModified; }

	void ConvertToMeshUtility(CMeshUtility &meshUtil, int lod=0);

	void AddToWorld();		// add to render world, physics world
	void SetCollisionProperty(int group, int mask);

    bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    bool IntersectBox(const CAABox &box);
    bool IntersectBox(const COBox &box);

	void GetTrianglesInBox(const CAABox &box, std::vector<CTriangle> &triangles);

	void Update(float t);
    void Draw(IPlotter3D &plotter);

	CTerrainRenderObject* GetRenderObject(int lod, int patchX, int patchY);
	CTerrainGrassRenderObject* GetGrassRenderObject(int sectionX, int sectionY);

	CTerrainAsset* GetAsset() { return m_pMap; }
	CMaterialAsset* GetMaterial() { return m_pMaterial; }
	CMaterialAsset* GetMaterialAlphaTest() { return m_pMaterialAlphaTest; }
	CMaterialAsset* GetGrassMaterial() { return m_pGrassMaterial; }
	CTextureAsset* GetNormalTexture() { return m_pNormalTexture; }
	CTextureAsset* GetColorTexture() { return m_pColorTexture; }
	CTextureAsset* GetBlendTexture() { return m_pBlendTexture; }
	void ForceEditorTextures();
	void GenerateMaterialAlphaTest();

	void Show(bool show);
    void SetCastDynamicShadow(bool b);
    void SetCastStaticShadow(bool b);
    void SetShadowResolutionFactor(float r);
    void SetShadowDepthBias(float b);
    void SetLightGroups(int g);
    void SetLodGroup(int g);
	void SetNestedLodDistance(int lod, float d);
	float GetNestedLodDistance(int lod) const;
	void SetLightMap(int sectionIndex, CLightMapInfo *lm);
	void UpdateRenderObjects();

	void ShowGrass(bool show);
    void SetGrassLightGroups(int g);
    void SetGrassLodGroup(int g);
	void SetGrassMaxVisibleDistance(float d);
    float GetGrassMaxVisibleDistance() const { return m_fGrassMaxVisibleDistance; }
	void SetGrassFadeDistance(float d) { m_fGrassFadeDistance = d; }
    float GetGrassFadeDistance() const { return m_fGrassFadeDistance; }
	void SetGrassCollider(int index, const CVector3 &position, float radius);

    void SetPosition(const CVector3 &p);
	void GetPosition(CVector3 &p) const { p = m_Position; }
	const CVector3& GetPosition() const { return m_Position; }
    void GetBoundingBox(CAABox &box);

	int GetSize() const { return m_iSize; }
	float GetScale() const { return m_fScale; }
	float GetMaxHeight() const { return m_fMaxHeight; }

	float GetHeightMap(int x, int y);
	float SampleHeightMap(float x, float y);		// x, y is world position, return world z
	float SampleHeightMapRough(float x, float y);	// no interpolation, x, y is world position, return world z
	float SampleTriangleHeight(float x, float y);	// x, y is world position, return world z

	// editor
	void BeginModifyHeightMap();
	void EndModifyHeightMap(bool updatePhysicsObject=true);
	void UpdatePhysicsObjects();
	void SetEditorHeightMap(int x, int y, float h);
	float GetEditorHeightMap(int x, int y);
	float SampleEditorHeightMap(float x, float y);		// x, y is world position, return world z

	// attrib is 4 bits grass and 4 bits material
	void BeginModifyAttributeMap();
	void EndModifyAttributeMap();
	void SetAttributeMap(int x, int y, int a);
	int GetAttributeMap(int x, int y);
	void SetGrassAttributeMap(int x, int y, int a);
	int GetGrassAttributeMap(int x, int y);
	void SetMaterialAttributeMap(int x, int y, int a);
	int GetMaterialAttributeMap(int x, int y);

	void BeginModifyColorMap();
	void EndModifyColorMap();
	void SetEditorColorMap(int x, int y, const CVector3 &c);
	void GetEditorColorMap(int x, int y, CVector3 &c);

	void BeginModifyBlendMap();
	void EndModifyBlendMap();
	void SetEditorBlendMap(int x, int y, const CVector3 &c);
	void GetEditorBlendMap(int x, int y, CVector3 &c);

	// used by CTerrainGrassMesh
	CTerrainGrassInfo* GetGrassInfo(int g);
	CTerrainGrassDistribution* GetGrassDistribution(int g);
	bool GetGrassCellMap(int x, int y);
	CTerrainGrassMeshCell* PrepareGrassMesh(int x, int y);
	int GenerateGrassVertices(int x, int y, CTerrainGrassVertex *vertices);
	void GrassMeshDirty();
	CVector4 GetGrassCollider(int index) { return m_GrassCollider; }

private:

	int m_iSize;
	float m_fScale;
	float m_fMaxHeight;

	void SetHeightMap(int x, int y, float h);
	void SetGrassCellMap(int x, int y, bool b);

	CTerrainAsset *m_pMap;

	bool m_bEditorMode;
	bool m_bModified;
	bool m_bModifingHeightMap;
	bool m_bModifingAttributeMap;
	bool m_bModifingColorMap;
	bool m_bModifingBlendMap;
	int m_iModifingMinX;
	int m_iModifingMaxX;
	int m_iModifingMinY;
	int m_iModifingMaxY;

	void CreateMaterial();
	void CreateGrassMaterial();

	void AssignTexture(CTextureAsset *tex, int index);
	void AssignGrassTexture(CTextureAsset *tex, int index);
	bool SaveTexture(CTextureAsset *&tex, const char *fname, EPixelFormat format);

	void CreateNormalMapTexture();
	void UpdateNormalMapTexture(int x, int y, int w, int h);

	void CreateColorMapTexture();
	void UpdateColorMapTexture(int x, int y, int w, int h);

	void CreateBlendMapTexture();
	void UpdateBlendMapTexture(int x, int y, int w, int h);

	void DeleteTextureMaterials();

	void CreateSections();
	void DeleteSections();

	void CreateRenderObjects();
	void DeleteRenderObjects();

	void CreatePhysicsObjects();
	void DeletePhysicsObjects();
	void CreatePhysicsObject(int sectionX, int sectionY, bool addToWorld);

	CVector3 m_Position;
	int m_iCollisionGroup;
	int m_iCollisionMask;

	class CTerrainSection
	{
	public:
		CTerrainSection();
		CPhysicsObject *m_pPhysicsObject;
		CTerrainGrassRenderObject *m_pGrassRenderObject;
		bool m_bPhysicsObjectDirty;
	};
	std::vector<CTerrainSection> m_Sections;

	CPhysicsWorld *m_pPhysicsWorld;
	CRenderWorld *m_pRenderWorld;
	std::vector<CTerrainRenderObject*> m_RenderObjects[TERRAIN_LODS];
	float m_NestedLodDistances[TERRAIN_LODS - 1];

	CTextureAsset *m_pNormalTexture;
	CTextureAsset *m_pColorTexture;
	CTextureAsset *m_pBlendTexture;
	CMaterialAsset *m_pMaterial;
	CMaterialAsset *m_pMaterialAlphaTest;
	CMaterialAsset *m_pGrassMaterial;

	CTerrainGrassInfo m_GrassInfo[TERRAIN_GRASS_INFO_MAX];
	CTerrainGrassDistribution m_GrassDistribution[TERRAIN_GRASS_DISTRIBUTION_MAX];
	float m_fGrassMaxVisibleDistance;
	float m_fGrassFadeDistance;
	CVector4 m_GrassCollider;

	typedef std::map<int, CTerrainGrassMeshCell*> TerrainGrassMeshMap;
	TerrainGrassMeshMap m_GrassMeshes;
};


}//GELib

#endif//__GELIB_EXT_TERRAIN_H__
