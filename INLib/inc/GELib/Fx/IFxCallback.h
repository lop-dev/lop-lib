//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/IFxCallback.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_IFXCALLBACK_H__
#define __GELIB_FX_IFXCALLBACK_H__

#include <GELib/Math/GeMath.h>
#include <GELib/Ext/DecalMesh.h>


namespace GELib
{


class CFxEventComponent;
class CCharacter;

class IFxCallback
{
public:
    IFxCallback() {};
    virtual ~IFxCallback() {};

	virtual void GetDecalAccepters(const COBox &obox, CDecalAccepterList &accepters) {}
	virtual void Event(CFxEventComponent *c, const std::string &e) {}
	virtual CCharacter* CreateCharacter(const char *name) { return 0; }
	virtual void DeleteCharacter(CCharacter *c) {}
	virtual std::string TranslateAnimation(const char *name) { return name; }
	virtual void HandleAnimationEvent(CCharacter *c) {}
};


}//GELib

#endif//__GELIB_FX_IFXCALLBACK_H__
