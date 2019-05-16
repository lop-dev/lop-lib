//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/StdVisObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_STDVISOBJECT_H__
#define __GELIB_RENDERER_STDVISOBJECT_H__

#include <vector>

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/Frustum.h>
#include <GELib/Math/ConvexVolume.h>
#include <GELib/Math/PolyMesh.h>
#include <GELib/Renderer/Visibility.h>
#include <GELib/Renderer/BoxTree.h>


namespace GELib
{


class CStdVisibility;
class CStdVisLight;


class CStdVisVolume
{
public:
    CStdVisVolume() {}
    CStdVisVolume(int zone, int portal, const CConvexVolume &volume) { m_iZone=zone; m_iPortal=portal; m_Volume=volume; }
    int m_iZone;
    int m_iPortal;
    CConvexVolume m_Volume;
};

class CStdVisObject
{
public:
    CStdVisObject();
    virtual ~CStdVisObject();

    void UpdateRelevantLights();
    void AddRelevantLight(CStdVisLight *vlt);
    void RemoveRelevantLight(CStdVisLight *vlt);
    void AddRelevantLights();
    void RemoveRelevantLights();

	void SetLodParent(CStdVisObject *parent);
    void RemoveLodHierarchy();
	bool IsAncestor(CStdVisObject *a);

    CRenderObject *m_pRenderObject;
    CVisibleObject *m_pVisibleObject;
    CStdVisibility *m_pVisibility;
    int m_iId;
    int m_iZone;
    bool m_bOccluded;
    CAABox m_Box;
    CSphere m_Sphere;

    unsigned int m_iVisibleFrame;
    unsigned int m_iBoxTreeCounter;
    unsigned int m_iBoxTreeCounter2;
    unsigned char m_iUpdateCount;
    CBoxTree::BoxNodes m_BoxNodes;

    std::vector<CStdVisLight*> m_RelevantLights;
    bool m_bRelevantLightsDirty;
    bool m_bDirty;
    bool m_bDynamic;

	// hierarchical lod
	CStdVisObject *m_pLodParent;
    std::vector<CStdVisObject*> m_LodChildren;

private:
    void AddRelevantLightsHelper(const std::vector<CStdVisLight*> &lightList, size_t beginIndex=0);
};

class CStdVisLight
{
public:
    CStdVisLight();
    virtual ~CStdVisLight();

    void UpdateBoundingBox();
    void UpdateFrustum();
    void UpdateVolumes();
    void AddVolume(int zone, int portal, const CConvexVolume &volume);
    void AddVolumeFromPortal(int portal, const CConvexVolume &volume);
    bool IsIntersectBox(const CAABox &box);			// consider m_fBoundingThreshold and frustum
    bool IsSphereIntersectBox(const CAABox &box);	// only consider m_fRange
    bool IsIntersectFrustum(const CFrustum &f);
    bool IsAffectBox(CAABox &box, int zone, bool dontCareZone=false);
    bool IsInfiniteDirectional() { return m_pRenderLight->m_eType == CRenderLight::DIRECTIONAL_LIGHT && m_pRenderLight->m_fRange <= 0; }

    void UpdateRelevantObjects();
    void AddRelevantObject(CStdVisObject *vobj);
    void RemoveRelevantObject(CStdVisObject *vobj);
    void AddRelevantObjects();
    void RemoveRelevantObjects();

    CRenderLight *m_pRenderLight;
    CVisibleLight *m_pVisibleLight;
    CStdVisibility *m_pVisibility;
    int m_iId;
    int m_iZone;

    CMatrix m_ViewMatrix;
    CMatrix m_ProjMatrix;
    CMatrix m_ViewProjMatrix;
    CFrustum m_Frustum;
    CAABox m_Box;

    unsigned int m_iVisibleFrame;
    unsigned int m_iBoxTreeCounter;
    unsigned int m_iBoxTreeCounter2;
    unsigned char m_iUpdateCount;
    CBoxTree::BoxNodes m_BoxNodes;

    std::vector<CStdVisVolume> m_Volumes;
    std::vector<CStdVisObject*> m_RelevantObjects;
    std::vector<CStdVisObject*> m_DynamicRelevantObjects;
    std::vector<CStdVisObject*> m_DynamicShadowRelevantObjects;
    bool m_bRelevantObjectsDirty;
    bool m_bDirty;
    bool m_bDynamic;
};

class CStdVisOccluder
{
public:
    CStdVisOccluder();
    virtual ~CStdVisOccluder();

    void UpdateWorldVerticesAndBoundingBox();
    int ComputeOccludeVolume(const CVector3 &cameraPosition, CPlane *planes, int maxPlanes);

    CRenderOccluder *m_pRenderOccluder;
    CStdVisibility *m_pVisibility;

    int m_iId;
    int m_iZone;
    int m_iSortKey;
    float m_fViewZDistance;
    CAABox m_Box;
    std::vector<CVector3> m_WorldVertices;
    std::vector<CPlane> m_Planes;

    unsigned int m_iVisibleFrame;
    unsigned int m_iBoxTreeCounter;
    unsigned int m_iBoxTreeCounter2;
    unsigned char m_iUpdateCount;
    CBoxTree::BoxNodes m_BoxNodes;

    bool m_bDirty;
    bool m_bDynamic;
};

class CStdVisZone
{
public:
    CStdVisZone();
    virtual ~CStdVisZone();

    void UpdateVolume();

    CRenderZone *m_pRenderZone;
    CStdVisibility *m_pVisibility;

    CAABox m_Box;

    CConvexVolume m_Volume;
    unsigned int m_iVisibleFrame;
    int m_iId;
};

class CStdVisPortal
{
public:
    CStdVisPortal();
    virtual ~CStdVisPortal();

    CRenderPortal *m_pRenderPortal;
    CStdVisibility *m_pVisibility;

    void UpdateLinkZone();
    bool IsValid() { return m_pRenderPortal && m_iLinkZone[0]!=m_iLinkZone[1]; }
    bool IsLinkToOutdoor() { return m_iLinkZone[1]==0; }

    CAABox m_Box;

    int m_iId;
    int m_iLinkZone[2];
    std::vector<CVector3> m_Polygon[2];
    CPlane m_Plane[2];
    std::vector<CVector3> m_ClippedPolygon;
    CConvexVolume m_Volume;
    unsigned int m_iVisibleFrame;
};



}//GELib

#endif//__GELIB_RENDERER_STDVISOBJECT_H__
