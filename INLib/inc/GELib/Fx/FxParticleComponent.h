//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxParticleComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXPARTICLECOMPONENT_H__
#define __GELIB_FX_FXPARTICLECOMPONENT_H__

#include <vector>
#include "FxComponent.h"
#include <GELib/Renderer/RendererTypes.h>

namespace GELib
{

class CMeshAsset;
class CMaterialAsset;
class CFxParticleMesh;
class CFxParticleRenderObject;
class CFxMeshRenderObject;
class CFxInstanceMeshRenderObject;
class CFxParticleMesh;


enum EParticleSort
{
	PARTICLE_SORT_NONE = 0,
	PARTICLE_SORT_REVERSE = 1,
	PARTICLE_SORT_CAMERA = 2
};

enum EParticleOrientation
{
    PARTICLE_ORIENTATION_CAMERA = 0,
    PARTICLE_ORIENTATION_VELOCITY = 1,
    PARTICLE_ORIENTATION_SPIN = 2,
    PARTICLE_ORIENTATION_VELOCITY_ZFIXED = 3
};

enum EParticleRenderType
{
	PARTICLE_RENDER_NONE = 0,
	PARTICLE_RENDER_BOARD = 1,
	PARTICLE_RENDER_INSTANCE_MESH = 2,
	PARTICLE_RENDER_MESH = 3,
	PARTICLE_RENDER_FX = 4
};

enum EParticleSizeType
{
	PARTICLE_SIZE_RECTANGLE = 0,
	PARTICLE_SIZE_SQUARE = 1,
	PARTICLE_SIZE_STRIP = 2
};

class CFxParticleComponentResource : public CFxComponentResource
{
public:
    CFxParticleComponentResource(CFxAsset *fxRes);
    virtual ~CFxParticleComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_PARTICLE; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    virtual void ComputeTimeRange();

    bool LoadMaterial(const char *fname, const CLoadParameter &param);
    void SetMaterial(CMaterialAsset *material);
    CMaterialAsset* GetMaterial();
    void DeleteMaterial();

    bool LoadMesh(const char *fname, const CLoadParameter &param);
    void SetMesh(CMeshAsset *mesh);
    CMeshAsset* GetMesh();
    void DeleteMesh();

    bool LoadFx(const char *fname, const CLoadParameter &param);
    void SetFx(CFxAsset *fx);
    CFxAsset* GetFx();
    void DeleteFx();

    int m_iSpawnCount;
    float m_fSpawnDuration;
    char m_iSourcePositionType;
    bool m_bLocal;
    EParticleSizeType m_eSizeType;
	bool m_bParticleFxLoop;
	EParticleSort m_eSort;
    EParticleOrientation m_eOrientation;
    float m_fMinLifeTime, m_fMaxLifeTime;
    CVector2 m_MinSize, m_MaxSize;
    CVector3 m_MinDirection, m_MaxDirection;
    float m_fMinSpeed, m_fMaxSpeed;
    float m_fMinRotateSpeed, m_fMaxRotateSpeed;
    CVector3 m_TurbulenceVelocity;
    float m_fMinTurbulenceTime, m_fMaxTurbulenceTime;
    CVector3 m_Force;
	float m_fInheritEmitterVelocity;
    float m_fSinkPower;
    float m_fFadeInTime;
    float m_fFadeOutTime;
    float m_fMinSpawnDistance;
    float m_fMaxSpawnDistance;
    std::string m_SourceName;
    std::string m_SinkName;
    bool m_bKillWhenSink;
    char m_iSinkPositionType;
    char m_iSinkForceType;
	char m_iTextureArrayType;
	char m_iTextureArrayFps;
	char m_iTextureArrayLoopMode;

    bool m_bCollision;
    float m_fCollideSpeedDamping;
    float m_fCollideLifeDamping;

	bool m_bWrap;
	CVector3 m_WrapMin;
	CVector3 m_WrapMax;

    bool m_bCastDynamicShadow;
    bool m_bMotionBlur;
	bool m_bUseEnvironmentLight;
	EDrawMeshMode m_eDrawMeshMode;
    float m_fShadowResolutionFactor;
    int m_iLightGroups;

    CInterpolationVector3 m_ParticleColorKeys;
    CInterpolationFloat m_ParticleOpacityKeys;
    CInterpolationFloat m_ParticleSpeedKeys;
    CInterpolationFloat m_ParticleScaleKeys;
    CInterpolationFloat m_ParticleSinkPowerKeys;

	enum
	{
		TABLE_NUM = 8
	};
	CVector3 m_ParticleColorTable[TABLE_NUM];
	float m_ParticleOpacityTable[TABLE_NUM];
	float m_ParticleSpeedTable[TABLE_NUM];
	float m_ParticleScaleTable[TABLE_NUM];
	float m_ParticleSinkPowerTable[TABLE_NUM];
	void PrecomputeResource();

	// computed
    bool m_bAlphaBlend;
    EParticleRenderType m_eRenderType;
    float m_fMeshRadius;

private:

	bool CanUseInstanceMesh();

    CMeshAsset *m_pMesh;
    CMaterialAsset *m_pMaterial;
    CFxAsset *m_pParticleFx;
};

class CFxParticle
{
public:
    float m_fLifeTime;
    float m_fAge;
    float m_fTurbulenceTimer;
    float m_fRotation;
    float m_fRotateSpeed;
    float m_fScale;
	float m_fSpawnRandom;
	CRGBAColor m_Color;
	CRGBAColor m_SpawnColor;
    CVector3 m_Position;
    CVector3 m_PrevPosition;
    CVector3 m_Velocity;
    CVector2 m_Size;
    CVector3 m_SinkPosition;
    CVector3 m_RotateAxis;
};

class CFxParticleComponent : public CFxComponent
{
public:
    CFxParticleComponent(CFx *fx, CFxParticleComponentResource *res);
    virtual ~CFxParticleComponent();

    virtual void SetRelatedComponents();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxParticleComponentResource* ParticleResource() { return (CFxParticleComponentResource*)m_pResource; }
    CFxParticle* GetParticle(int index);
	void ComputeParticleMatrix(CFxParticle *p, CMatrix &matrix, bool scale);

    void UpdateMesh(CFxParticleMesh *mesh);
    int GetNumActiveParticles() { return m_iNumActiveParticles; }

private:

    void UpdateRenderObject();
    void SpawnParticles(int count, float time);
    void UpdateParticles();

    CFxParticleRenderObject *m_pParticleRenderObject;
    CFxInstanceMeshRenderObject *m_pInstanceMeshRenderObject;
    std::vector<CFxMeshRenderObject*> m_MeshRenderObjects;
    std::vector<CFx*> m_ParticleFxs;
    std::vector<CFx*> m_HaltedParticleFxs;
    CFxComponent *m_pSourceComponent;
    CFxComponent *m_pSinkComponent;

    int m_iNumActiveParticles;
    float m_fSpawnFrac;
    CVector3 m_EmitterVelocity;
    bool m_bFirstSpawned;
    std::vector<CFxParticle> m_Particles;
};


}//GELib

#endif//__GELIB_FX_FXPARTICLECOMPONENT_H__
