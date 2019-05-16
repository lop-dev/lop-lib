//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Renderer/StdVisibility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_RENDERER_STDVISIBILITY_H__
#define __GELIB_RENDERER_STDVISIBILITY_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Math/Frustum.h>
#include <GELib/Utility/Allocator.h>
#include "Visibility.h"
#include "StdVisObject.h"
#include "BoxTree.h"

namespace GELib
{

class CConvexVolume;
class CSoftwareZBuffer;

class CStdVisibility : public CVisibility
{
public:
    friend class CStdVisObject;
    friend class CStdVisLight;
    friend class CStdVisZone;
    friend class CStdVisPortal;

    GELIB_API CStdVisibility();
    GELIB_API virtual ~CStdVisibility();

    GELIB_API void Initialize(const CAABox &worldBound, const CVector3 &gridSize);

    GELIB_API int  AddObject(CRenderObject *obj);
    GELIB_API void RemoveObject(int id);
    GELIB_API void UpdateObject(int id);

    GELIB_API void SetLodParent(int id, int parentId);

    GELIB_API int  AddLight(CRenderLight *lt);
    GELIB_API void RemoveLight(int id);
    GELIB_API void UpdateLight(int id);

    GELIB_API int  AddOccluder(CRenderOccluder *oc);
    GELIB_API void RemoveOccluder(int id);
    GELIB_API void UpdateOccluder(int id);

    GELIB_API int  AddPortal(CRenderPortal *po);
    GELIB_API void RemovePortal(int id);
    GELIB_API void UpdatePortal(int id);

    GELIB_API int  AddZone(CRenderZone *zo);
    GELIB_API void RemoveZone(int id);
    GELIB_API void UpdateZone(int id);

    GELIB_API void ForceDynamicLightingAndShadow(bool force);     // use dynamic lighting and shadow to preview static lighting and shadow

    GELIB_API void EnableOccluder(bool enable);
    GELIB_API void EnablePortal(bool enable);
    GELIB_API void EnableSoftwareZBuffer(bool enable);
    GELIB_API CSoftwareZBuffer* GetSoftwareZBuffer();	// debug
	GELIB_API void SetMaxActiveOccluder(int n);

    GELIB_API void ShowLight(int show); // 1: z on, 2: z off, 4: box 8: screen rect
    GELIB_API void ShowObject(int show); // 1: z on, 2: z off
    GELIB_API void ShowOccluder(int show); // 1: z on, 2: z off
    GELIB_API void ShowPortal(int show); // 1: z on, 2: z off
    GELIB_API void ShowSpatialPartition(int show); // 1: z on, 2: z off
    GELIB_API void ShowSoftwareZBuffer(int show); // 1: z on

    GELIB_API void Freeze(bool freeze);

    class NodeContentList
    {
    public:
        NodeContentList() { m_iNumObjects=0; }
        std::vector<CStdVisObject*> m_Objects;
        std::vector<CStdVisLight*> m_Lights;
        std::vector<CStdVisOccluder*> m_Occluders;
        int m_iNumObjects;
    };
    // used by CStdVisObject and CStdVisLight
    GELIB_API CBoxTree::NodeIndices& GetNodesIntersectBox(CAABox &box);
    GELIB_API NodeContentList& GetNodeContentList(int index) { return m_NodeContentLists[index]; }

    // called by CRenderer
    GELIB_API virtual void GetVisibleSet(const CRendererContext &rc, CVisibleSet &visSet);
    GELIB_API virtual void GetLightMapVisibleSet(const CRendererContext &rc, CVisibleSet &visSet, int objId, bool useAOLightMapColor);
    GELIB_API virtual void GenerateAmbientOcclusionLights(int num);
    GELIB_API virtual void RemoveAmbientOcclusionLights();
    GELIB_API virtual const CVisStatistic* GetStatistic();
    GELIB_API virtual void ResetStatistic();
    GELIB_API virtual void Draw(IPlotter3D &plotter, const CVisibleSet &visSet);
    GELIB_API virtual void Draw(IPlotter2D &canvas, const CVisibleSet &visSet);

private:

    CStdVisObject* GetObject(int id);
    CStdVisLight* GetLight(int id);
    CStdVisOccluder* GetOccluder(int id);
    CStdVisPortal* GetPortal(int id);
    CStdVisZone* GetZone(int id);

    void FreezeVisibleSet(const CRendererContext &rc, CVisibleSet &visSet);

    void IncreaseFrameCounter();
    void IncreaseBoxTreeCounter();
    void IncreaseBoxTreeCounter2();

    void UpdateDirtyObjects();
	bool NeedUpdateNode(const CBoxTree::BoxNodes &nodes, const CAABox &box);
    int  GetZoneIndex(const CVector3 &p, int currentZone=0);
    void AddVisibleVolume(int zone, int portal, const CConvexVolume &volume);
    void AddVisibleVolumeFromPortal(int portal, const CConvexVolume &volume);
    void FindActiveOccluders(const CMatrix &viewProjMatrix);
    void RemoveOccludedVolumes();
    void RemoveOccludedNodes();

	void FindNestedLodVisibleObject(const CRendererContext &rc, CVisibleSet &visSet, CStdVisObject *obj, bool insideFrustum);

    bool IsZoneVisible(int zone);
    bool IsZoneLinked(int zone1, int zone2);
    bool IsPortalZoneLinked(int portal, int zone);
    bool IsBoxIntersectVisibleVolumes(CAABox &box, int zone, bool dontCareZone, bool insideFrustum=false);
    bool IsLightInVisibleZones(CStdVisLight *lt);
    bool IsOccluderVisible(CStdVisOccluder *voc, bool insideFrustum=false);
    bool IsBoxOccluded(const CAABox &box, float viewZDist=0);
    bool IsSphereOccluded(const CSphere &sphere);
    bool IsOccluderOccluded(CStdVisOccluder *voc);

    bool IsRelevant(CStdVisObject *obj, CStdVisLight *lt);
    CVisibleObject* PrepareVisibleObject(CStdVisObject *obj, float viewZDistance, const CRendererContext &rc);
    CVisibleLight* PrepareVisibleLight(CStdVisLight *lt, const CRendererContext &rc);

    int GetNextSerial();

    CChunkObjectAllocator<CStdVisObject> m_ObjectAllocator;
    CChunkObjectAllocator<CStdVisLight> m_LightAllocator;
    CChunkObjectAllocator<CStdVisOccluder> m_OccluderAllocator;
    CChunkObjectAllocator<CStdVisZone> m_ZoneAllocator;
    CChunkObjectAllocator<CStdVisPortal> m_PortalAllocator;

    std::vector<CStdVisObject*> m_Objects;
    std::vector<CStdVisLight*> m_Lights;
    std::vector<CStdVisOccluder*> m_Occluders;
    std::vector<CStdVisZone*> m_Zones;
    std::vector<CStdVisPortal*> m_Portals;

    CBoxTree::NodeIndices m_VisibleNodes;
    std::vector<CStdVisOccluder*> m_VisibleOccluders;
    std::vector<CStdVisVolume> m_VisibleVolumes;
    std::vector<CStdVisZone*> m_ZoneStack;
    std::vector<CStdVisPortal*> m_PortalStack;

    class CActiveOccluder
    {
    public:
        enum { MAX_PLANES = 16 };
        CPlane m_Planes[MAX_PLANES];
        char m_NearCornerIndices[MAX_PLANES];
        int m_iNumPlanes;
        float m_fViewZDistance;

        void ComputeNearCornerIndices();
        bool IsContainBox(const CAABox &box) const;
        bool IsContainSphere(const CSphere &s) const;
        bool IsContainPoints(const CVector3 *p, int numPoints) const;
    };

    enum { MAX_ACTIVE_OCCLUDERS = 32 };
    CActiveOccluder m_ActiveOccluders[MAX_ACTIVE_OCCLUDERS];
	int m_iNumActiveOccluders;
	int m_iMaxActiveOccluders;

    std::vector<CStdVisObject*> m_DirtyObjects;
    std::vector<CStdVisLight*> m_DirtyLights;
    std::vector<CStdVisOccluder*> m_DirtyOccluders;

    void AttachObjectNode(CStdVisObject *obj);
    void DetachObjectNode(CStdVisObject *obj);
    void AttachLightNode(CStdVisLight *obj);
    void DetachLightNode(CStdVisLight *obj);
    void AttachOccluderNode(CStdVisOccluder *obj);
    void DetachOccluderNode(CStdVisOccluder *obj);
    void ClearEmptyNode(CBoxTree::NodeIndex nodeIndex);

    CBoxTree m_BoxTree;
    std::vector<NodeContentList> m_NodeContentLists;
    CBoxTree::NodeIndices m_TempNodes;

	CSoftwareZBuffer *m_pZBuffer;

	bool m_bInitialized;
    bool m_bForceDynamicLightingAndShadow;
    bool m_bEnableOccluder;
    bool m_bEnablePortal;
    bool m_bEnableSoftwareZBuffer;
    bool m_bFreeze;
    int m_iShowOccluder;
    int m_iShowPortal;
    int m_iShowSpatialPartition;
    int m_iShowLight;
    int m_iShowObject;
	int m_iShowSoftwareZBuffer;
	int m_iZBufferTexture;

    CMatrix m_CameraMatrix;
    CFrustum m_Frustum;
    CPlane m_CameraPlane;
    int m_iNearPlaneBoxCornerIndex;

    bool m_bAllDirty;
    int m_iSerial;
    unsigned int m_iFrameCounter;
    unsigned int m_iBoxTreeCounter;
    unsigned int m_iBoxTreeCounter2;

	CVisStatistic m_Statistic;

    std::vector<int> m_AmbientOcclusionLights;
};



}//GELib

#endif//__GELIB_RENDERER_STDVISIBILITY_H__
