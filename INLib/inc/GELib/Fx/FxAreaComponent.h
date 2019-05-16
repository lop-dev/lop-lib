//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxAreaComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXAREACOMPONENT_H__
#define __GELIB_FX_FXAREACOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CMaterialAsset;
class CFxAreaMesh;
class CFxAreaRenderObject;

class CFxAreaComponentResource : public CFxComponentResource
{
public:
    CFxAreaComponentResource(CFxAsset *fxRes);
    virtual ~CFxAreaComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_AREA; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

	char m_iShape;
	int m_iSide;
    float m_fRadius;
    float m_fHeight;
	float m_fBias;
	bool m_bAlignNormal;
	bool m_bCheckBlocking;
	CVector2 m_QuadSize;

    int m_iLightGroups;
	bool m_bUseEnvironmentLight;
	char m_iTextureArrayType;
	char m_iTextureArrayFps;
	char m_iTextureArrayLoopMode;

private:
    CMaterialAsset *m_pMaterial;
};


class CFxAreaComponent : public CFxComponent
{
public:
    CFxAreaComponent(CFx *fx, CFxAreaComponentResource *res);
    virtual ~CFxAreaComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

	virtual bool PrepareRender();
    void UpdateMesh(CFxAreaMesh *mesh);

    CFxAreaComponentResource* AreaResource() { return (CFxAreaComponentResource*)m_pResource; }

private:

    void UpdateRenderObject();
    CFxAreaRenderObject *m_pRenderObject;
	float m_fSpawnRandom;
	bool m_bTriangleDirty;

	std::vector<CTriangle> m_Triangles;

	class CDot
	{
	public:
		CVector3 m_Position;
		CVector3 m_Tangent;
		CVector3 m_Normal;
	};
	std::vector<CDot> m_Dots;
};



}//GELib

#endif//__GELIB_FX_FXAREACOMPONENT_H__
