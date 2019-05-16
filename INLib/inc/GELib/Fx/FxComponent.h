//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXCOMPONENT_H__
#define __GELIB_FX_FXCOMPONENT_H__

#include <string>

#include <GELib/Math/GeMath.h>
#include <GELib/Math/InterpolationValue.h>
#include <GELib/Ext/IAttachable.h>
#include "FxTypes.h"

namespace GELib
{

class CIOStream;
class CLoadTask;
class CRenderMesh;
class IPlotter3D;
class IPlotter2D;
class CAssetHelper;
class CLoadParameter;
class CMaterialAsset;
class CFxAsset;
class CFxComponent;
class CFx;

class CFxComponentResource
{
public:
    CFxComponentResource(CFxAsset *fx);
    virtual ~CFxComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_BASE; }
    virtual CFxComponent* CreateInstance(CFx *fx);
    CFxAsset* GetFxAsset() { return m_pFxAsset; }

    virtual void GetAssets(CAssetHelper &helper);

    virtual void ComputeTimeRange();
    void GetTimeRange(float &beginTime, float &endTime) const;

    friend class CFxComponent;

    std::string m_Name;
    std::string m_AttachPointName;
    bool m_bShow;
    bool m_bAttack;
	bool m_bAlwaysUpdate;
	EFxHaltBehavior m_eHaltBehavior;
    char m_iLod;            // 0: visible if FxPlayer's lod is low, medium, hight
                            // 1: visible if FxPlayer's lod is medium or high
                            // 2: visible if FxPlayer's lod is high

    int m_iAttachTransform;
    EFxAttachTime m_eAttachTime;
    EFxRotateOrder m_eRotateOrder;

	CInterpolationVector3 m_PositionKeys;
    CInterpolationVector3 m_RotationKeys;
    CInterpolationVector3 m_ScaleKeys;
    CInterpolationVector3 m_ColorKeys;
    CInterpolationFloat m_OpacityKeys;
    CInterpolationVector4 m_VectorKeys;
    CInterpolationString m_NameKeys;

    float m_fBeginTime;
    float m_fEndTime;

protected:

    void SaveKeys(CIOStream &stream, CInterpolationFloat &keys);
    void SaveKeys(CIOStream &stream, CInterpolationVector3 &keys);
    void SaveKeys(CIOStream &stream, CInterpolationVector4 &keys);
    void SaveKeys(CIOStream &stream, CInterpolationBool &keys);
    void SaveKeys(CIOStream &stream, CInterpolationString &keys);
    void LoadKeys(CIOStream &stream, CInterpolationFloat &keys);
    void LoadKeys(CIOStream &stream, CInterpolationVector3 &keys);
    void LoadKeys(CIOStream &stream, CInterpolationVector4 &keys);
    void LoadKeys(CIOStream &stream, CInterpolationBool &keys);
    void LoadKeys(CIOStream &stream, CInterpolationString &keys);

	int m_iVersion;
    CFxAsset *m_pFxAsset;
};


class CFxComponent : public IAttachPoint
{
public:
    CFxComponent(CFx *fx, CFxComponentResource *fxRes);
    virtual ~CFxComponent();

    int GetClassId();
    virtual const char * GetName();			// implement IAttachPoint
    CFxComponentResource* GetResource();
    CFx* GetFx();

    // random number
    unsigned int GetRandom();
    float GetRandomF();
    float GetRandom(float minValue, float maxValue);
    CVector2 GetRandom(const CVector2 &minValue, const CVector2 &maxValue);
    CVector3 GetRandom(const CVector3 &minValue, const CVector3 &maxValue);

    virtual void GetRandomPoint(CVector3 &p, bool onSurface, bool local, float frac);
    virtual void GetClosestPoint(CVector3 &p, const CVector3 &src, bool local, float frac);
    virtual bool IsInside(const CVector3 &p);

	virtual IAttachable* GetAttachable();
    virtual void SetRelatedComponents();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);
	virtual void DrawSelected(IPlotter3D &plotter, IPlotter2D &canvas);
	virtual bool PrepareRender();		// called by fx render object

	void ComputeTrajectory(float t, CMatrix &matrix);
	void ComputeLocalMatrix(float t, CMatrix &matrix);

    float GetAge();
	void SetVectorParameter(const CVector4 &v) { m_VectorParameter = v; }
    const CVector4& GetVectorParameter() { return m_VectorParameter; }
    const CVector4& GetFxMaterialParameter();

    const CMatrix& GetMatrix() { return m_Matrix; }
    virtual void GetMatrix(CMatrix &matrix) { matrix = m_Matrix; }		// implement IAttachPoint
    const CMatrix& GetInverseMatrix();
    const CMatrix& GetPrevMatrix() { return m_PrevMatrix; }
    void SetOffsetMatrix(const CMatrix &matrix, EFxComponentOffsetMode mode);

    const CAABox& GetBoundingBox() { return m_Box; }

    void Attach(const char *name);
    void Detach();

	CFxComponent* GetAttachComponent();
	IAttachPoint* GetAttachPoint() { return m_pAttachPoint; }

    bool IsVisible();
	bool IsInTimeRange() { return m_bInTimeRange; }

	static bool IsMaterialNoLighting(CMaterialAsset *m);

protected:

    std::string m_AttachPointName;
    IAttachPoint *m_pAttachPoint;
    CFx *m_pFx;
    CFxComponentResource *m_pResource;
    CMatrix m_Matrix;
    CMatrix m_PrevMatrix;
    CMatrix m_InverseMatrix;
    CMatrix m_AttachBaseMatrix;
    CMatrix m_OffsetMatrix;
    CVector4 m_VectorParameter;
    CVector3 m_Color;
    float m_fOpacity;
    float m_fDetachTime;
    bool m_bUseAttachBaseMatrix;
    bool m_bAttachToComponent;
    bool m_bInverseMatrixDirty;
    bool m_bInitPrevMatrix;
	bool m_bUseGlobalMatrix;
	bool m_bInTimeRange;
    EFxComponentOffsetMode m_eOffsetMatrixMode;
    CAABox m_Box;
};

inline int CFxComponent::GetClassId()
{
    return m_pResource->GetClassId();
}

inline const char * CFxComponent::GetName()
{
    return m_pResource->m_Name.c_str();
}

inline CFxComponentResource* CFxComponent::GetResource()
{
    return m_pResource;
}

inline CFx* CFxComponent::GetFx()
{
    return m_pFx;
}



}//GELib


#endif//__GELIB_FX_FXCOMPONENT_H__
