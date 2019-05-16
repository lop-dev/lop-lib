//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/PortalSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_PORTALSCENEENTITY_H__
#define __GELIB_SCENE_PORTALSCENEENTITY_H__

#include <string>
#include <GELib/Utility/ColladaFile.h>
#include <GELib/Renderer/RendererTypes.h>
#include "SceneEntity.h"

namespace GELib
{

class CIOStream;
class CColladaFile;
class IPlotter3D;
class IPlotter2D;
class CBaseRenderOccluder;
class CBaseRenderPortal;
class CBaseRenderZone;


class CScene;


class COccluderSceneEntity : public CSceneEntity
{
public:
    COccluderSceneEntity(CScene *scene);
    virtual ~COccluderSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_OCCLUDER; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual void PostLoad();

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    bool ImportMesh(CColladaFile::Mesh *colladaMesh);
	void CreateBox(const CAABox &box);

    CBaseRenderOccluder* GetRenderObject() { return m_pRenderObject; }

private:
    void UpdateRenderObject();
    void DeleteRenderObject();
    CMatrix m_Matrix;
    CBaseRenderOccluder *m_pRenderObject;
};

class CPortalSceneEntity : public CSceneEntity
{
public:
    CPortalSceneEntity(CScene *scene);
    virtual ~CPortalSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_PORTAL; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual void PostLoad();

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    bool ImportMesh(CColladaFile::Mesh *colladaMesh);
	void CreateBox(const CAABox &box);

    CBaseRenderPortal* GetRenderObject() { return m_pRenderObject; }

private:
    void UpdateRenderObject();
    void DeleteRenderObject();
    CMatrix m_Matrix;
    CBaseRenderPortal *m_pRenderObject;
};

class CZoneSceneEntity : public CSceneEntity
{
public:
    CZoneSceneEntity(CScene *scene);
    virtual ~CZoneSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_ZONE; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual void PostLoad();

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    bool ImportMesh(CColladaFile::Mesh *colladaMesh);
	void CreateBox(const CAABox &box);

    CBaseRenderZone* GetRenderObject() { return m_pRenderObject; }

private:
    void UpdateRenderObject();
    void DeleteRenderObject();
    CMatrix m_Matrix;
    CBaseRenderZone *m_pRenderObject;
};



}//GELib

#endif//__GELIB_SCENE_PORTALSCENEENTITY_H__
