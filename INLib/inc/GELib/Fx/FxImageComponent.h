//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxImageComponent.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXIMAGECOMPONENT_H__
#define __GELIB_FX_FXIMAGECOMPONENT_H__

#include "FxComponent.h"

namespace GELib
{

class CTextureAsset;

class CFxImageComponentResource : public CFxComponentResource
{
public:
    CFxImageComponentResource(CFxAsset *fxRes);
    virtual ~CFxImageComponentResource();

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
	virtual bool CreateLoadedResource();

    virtual int GetClassId() { return FXCLASS_IMAGE; }
    virtual CFxComponent* CreateInstance(CFx *fx);

    virtual void GetAssets(CAssetHelper &helper);

    bool LoadTexture(const char *fname, const CLoadParameter &param);
    void DeleteTexture();
    CTextureAsset* GetTexture();

	void SetExternalTexture(int t) { m_iExternalTextureId = t; }
	int GetExternalTexture() { return  m_iExternalTextureId; }

	int m_iBlendMode;	// 0: none, 1: alpha, 2: additive, 3: subtractive, 4: multiply
	int m_iCoordinateMode;	// 0: default, 1: width major, 2: height major

private:

    CTextureAsset *m_pTexture;
	int m_iExternalTextureId;
};


class CFxImageComponent : public CFxComponent
{
public:
    CFxImageComponent(CFx *fx, CFxImageComponentResource *res);
    virtual ~CFxImageComponent();

    virtual void SetTime(float t);
    virtual void Draw(IPlotter3D &plotter, IPlotter2D &canvas);
	virtual void DrawSelected(IPlotter3D &plotter, IPlotter2D &canvas);

    CFxImageComponentResource* ImageResource() { return (CFxImageComponentResource*)m_pResource; }

private:

};


}//GELib

#endif//__GELIB_FX_FXIMAGECOMPONENT_H__
