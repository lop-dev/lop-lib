//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxParticleRenderObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXPARTICLERENDEROBJECT_H__
#define __GELIB_FX_FXPARTICLERENDEROBJECT_H__

#include <GELib/Ext/BaseRenderObject.h>
#include "FxParticleMesh.h"

namespace GELib
{


class CFxParticleComponent;

class CFxParticleRenderObject : public CBaseRenderObject
{
public:
    CFxParticleRenderObject();
    virtual ~CFxParticleRenderObject();

    void SetComponent(CFxParticleComponent *c);
    void SetMaterial(CMaterialAsset *material);

    virtual void Render(CRenderer *renderer);
    virtual void UpdateRenderData(CRenderer *renderer);

private:

    CFxParticleMesh m_Mesh;
    CMaterialAsset *m_pMaterial;
    CFxParticleComponent *m_pComponent;
    CFxParticleMeshShaderData m_ShaderData;
};


}//GELib

#endif//__GELIB_FX_FXPARTICLERENDEROBJECT_H__
