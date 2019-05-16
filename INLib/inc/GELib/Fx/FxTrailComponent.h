//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxTrailComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXTRAILCOMPONENT_H__
#define __GELIB_FX_FXTRAILCOMPONENT_H__

#include <GELib/Utility/CircularBuffer.h>
#include "FxComponent.h"
#include "FxTrailMesh.h"

namespace GELib
{

class CMeshAsset;
class CMaterialAsset;

class CFxTrailRenderObject;


class CFxTrailComponentResource : public CFxComponentResource
{
public:
    CFxTrailComponentResource(CFxAsset *fxRes);
    virtual ~CFxTrailComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_TRAIL; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    virtual void ComputeTimeRange();

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
    float m_fBeginSampleTime;
    float m_fEndSampleTime;
    float m_fLifeTime;
    float m_fFadeOutTime;
    float m_fSmooth;
	float m_fWidth;
    bool m_bFixedWidth;
    CVector3 m_TrailColor;

private:

    CMaterialAsset *m_pMaterial;
};


class CFxTrailSample
{
public:
    float m_fTime;
    float m_fLength;
    short m_iSubDivide;
    char  m_iFixedDummyIndex;
    bool  m_bDirty;
    CVector3 m_Position[2];
    CVector3 m_Tangent;
    CVector3 m_Normal;
};

class CFxTrailCurveVertex
{
public:
    CFxTrailVertex m_Vertex;
    float m_fSampleTime;
};

class CFxTrailComponent : public CFxComponent
{
public:
    CFxTrailComponent(CFx *fx, CFxTrailComponentResource *res);
    virtual ~CFxTrailComponent();

    virtual void SetRelatedComponents();

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float t);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    void SetDummyName(const char *n1, const char *n2);
    void GetDummyName(std::string &n1, std::string &n2);
    void SetSampleTime(float beginTime, float endTime);
    void GetSampleTime(float &beginTime, float &endTime);
    void SetLifeTime(float t);
    float GetLifeTime();
    void SetFadeOutTime(float t);
    float GetFadeOutTime();
    void SetSmooth(float s);
    float GetSmooth();
    void SetWidth(float w);
    float GetWidth();
    void SetColor(const CVector3 &c);
    void GetColor(CVector3 &c);

	virtual bool PrepareRender();
    void UpdateMesh(CFxTrailMesh *mesh);

    CFxTrailComponentResource* TrailResource() { return (CFxTrailComponentResource*)m_pResource; }


private:

    void UpdateRenderObject();
    void UpdateTrailSample();

    CFxTrailRenderObject *m_pRenderObject;

    std::string m_DummyName[2];
    float m_fBeginSampleTime;
    float m_fEndSampleTime;
    float m_fLifeTime;
    float m_fFadeOutTime;
    float m_fSmooth;
    float m_fWidth;
    CVector3 m_TrailColor;

    IAttachPoint *m_pDummy[2];
	CircularBuffer<CFxTrailSample> m_TrailSamples;
    CircularBuffer<CFxTrailCurveVertex> m_TrailVertices;
};



}//GELib

#endif//__GELIB_FX_FXTRAILCOMPONENT_H__
