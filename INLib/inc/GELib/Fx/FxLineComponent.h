//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxLineComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXLINECOMPONENT_H__
#define __GELIB_FX_FXLINECOMPONENT_H__

#include <GELib/Utility/CircularBuffer.h>
#include "FxComponent.h"
#include "FxTrailMesh.h"

namespace GELib
{

class CMeshAsset;
class CMaterialAsset;

class CFxLineRenderObject;


class CFxLineComponentResource : public CFxComponentResource
{
public:
    CFxLineComponentResource(CFxAsset *fxRes);
    virtual ~CFxLineComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_LINE; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

    bool m_bCastDynamicShadow;
	bool m_bUseEnvironmentLight;
    float m_fShadowResolutionFactor;
    int m_iLightGroups;
	char m_iTextureArrayType;
	char m_iTextureArrayFps;
	char m_iTextureArrayLoopMode;

    std::string m_DummyName[2];
	float m_fWidth;
	bool m_bFaceCamera;

private:

    CMaterialAsset *m_pMaterial;
};



class CFxLineComponent : public CFxComponent
{
public:
    CFxLineComponent(CFx *fx, CFxLineComponentResource *res);
    virtual ~CFxLineComponent();

    virtual void SetRelatedComponents();
    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float t);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	void UpdateMesh(CFxTrailMesh *mesh);
	float GetLineLength();

    CFxLineComponentResource* LineResource() { return (CFxLineComponentResource*)m_pResource; }

private:

	void UpdateLine(float t);
    void UpdateRenderObject();
    void UpdateRopeSample();

    CFxLineRenderObject *m_pRenderObject;

    IAttachPoint *m_pDummy[2];
	CVector3 m_Line[2];
	float m_fSpawnRandom;
	CVector3 m_SideVector;
};



}//GELib

#endif//__GELIB_FX_FXLINECOMPONENT_H__
