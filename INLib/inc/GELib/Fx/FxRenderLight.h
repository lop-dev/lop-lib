//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxRenderLight.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXRENDERLIGHT_H__
#define __GELIB_FX_FXRENDERLIGHT_H__

#include <GELib/Ext/BaseRenderObject.h>

namespace GELib
{

class CFxComponent;

class CFxRenderLight : public CBaseRenderLight
{
public:
    CFxRenderLight();
    virtual ~CFxRenderLight();

    virtual CRenderMaterial* GetMaterial();

    void SetComponent(CFxComponent *c);

private:
    CFxComponent *m_pComponent;
};



}//GELib


#endif//__GELIB_FX_FXRENDERLIGHT_H__
