//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/GameObjectSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_GAMEOBJECTSCENEENTITY_H__
#define __GELIB_SCENE_GAMEOBJECTSCENEENTITY_H__

#include <string>
#include <GELib/Renderer/RendererTypes.h>
#include <GELib/Utility/ObjectProperty.h>
#include "SceneEntity.h"


namespace GELib
{

class CIOStream;
class IPlotter3D;
class IPlotter2D;
class CFxAsset;
class CFx;
class CMeshRenderObject;
class CAssetHelper;
class CCharacter;


class CScene;


class CLocatorSceneEntity : public CSceneEntity
{
public:
    CLocatorSceneEntity(CScene *scene);
    virtual ~CLocatorSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_LOCATOR; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    void SetSize(const CVector3 &s) { m_Size = s; }
    const CVector3& GetSize() const { return m_Size; }
	void SetColor(const CVector3 &c) { m_Color = c; }
    const CVector3& GetColor() const { return m_Color; }

	COBox GetOBox();

protected:

    CMatrix m_Matrix;
    CVector3 m_Size;
    CVector3 m_Color;
};

class CCameraSceneEntity : public CSceneEntity
{
public:
    CCameraSceneEntity(CScene *scene);
    virtual ~CCameraSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_CAMERA; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    void SetFov(float fov) { m_fFov = fov; }
    float GetFov() const { return m_fFov; }
    void SetNear(float n) { m_fNear = n; }
    float GetNear() const { return m_fNear; }
    void SetFar(float f) { m_fFar = f; }
    float GetFar() const { return m_fFar; }
    void SetOrthoHeight(float h) { m_fOrthoHeight = h; }
    float GetOrthoHeight() const { return m_fOrthoHeight; }
    void SetPerspective(bool p) { m_bPerspective = p; }
    float GetPerspective() const { return m_bPerspective; }
    void SetShowFrustum(bool show) { m_bShowFrustum = show; }
    bool GetShowFrustum() const { return m_bShowFrustum; }

	COBox GetOBox();

protected:

    float m_fFov;
    float m_fNear;
    float m_fFar;
    float m_fOrthoHeight;
    bool m_bPerspective;
    bool m_bShowFrustum;
    CMatrix m_Matrix;
};


class CGameObjectSceneEntity : public CSceneEntity
{
public:
    CGameObjectSceneEntity(CScene *scene);
    virtual ~CGameObjectSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_GAME_OBJECT; }

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();
    virtual void PostLoad();

    virtual void GetAssets(CAssetHelper &helper);

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

    virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
    virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);
    virtual bool IntersectConvexVolume(const CConvexVolume &c);

    virtual void Update(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual void UpdateVisible();

    void SetSize(const CVector3 &s) { m_Size = s; }
    const CVector3& GetSize() const { return m_Size; }
    void SetColor(const CVector3 &c);
    const CVector3& GetColor() const { return m_Color; }
	bool IsCylinder() const { return m_bCylinder; }

    void SetSpawn(bool spawn) { m_bSpawn = spawn; }
    bool GetSpawn() const { return m_bSpawn; }
    void SetIgnoreStaticLight(bool ignore);
    bool GetIgnoreStaticLight() const { return m_bIgnoreStaticLight; }

    void SetDefaultPropertySet(const char *propertySetName);
    CObjectPropertySet& GetPropertySet() { return m_PropertySet; }
    void PropertySetModified(const CLoadParameter &param);

	COBox GetOBox();
	bool IsContainPoint(const CVector3 &p);

protected:

    void UpdateRenderObject();
    void DeleteRenderObject();

    CMeshRenderObject *m_pMeshRenderObject;
    CCharacter *m_pCharacter;
    CFx *m_pFx;
    CFxAsset *m_pFxAsset;
    CVector3 m_Size;
    CVector3 m_Color;
    CMatrix m_Matrix;

    bool m_bSpawn;
    bool m_bIgnoreStaticLight;
    bool m_bFillColor;
	bool m_bCylinder;
    CVector3 m_Offset;

    CObjectPropertySet m_PropertySet;
};



}//GELib

#endif//__GELIB_SCENE_GAMEOBJECTSCENEENTITY_H__
