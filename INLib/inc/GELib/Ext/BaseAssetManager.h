//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/BaseAssetManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_BASEASSETMANAGER_H__
#define __GELIB_EXT_BASEASSETMANAGER_H__

#include <string>
#include <GELib/Utility/SystemUtility.h>
#include <GELib/Utility/CircularBuffer.h>
#include "AssetLoadTask.h"

namespace GELib
{

class CBaseAsset;
class CLoadParameter;

class CBaseAssetManager
{
public:
    CBaseAssetManager();
    virtual ~CBaseAssetManager();

    // replace exist resource when load or import
    void SetReplaceExist(bool replace);
    bool GetReplaceExist();

    void IgnoreLoad(bool ignore);

    virtual void DeleteWaitObjects();

protected:

    void ForceDeleteWaitObjects();

    bool m_bReplaceExist;
    bool m_bIgnoreLoad;
	bool m_bRequireGraphicDevice;
    CCriticalSectionObject m_CS;
	CircularBuffer<CBaseAsset*> m_WaitDeleteObjects;
};


void ProcessLoadParameter(const char *fname, const CLoadParameter &param, std::string &outName, CLoadParameter &outParam, std::string *cacheKey);

void SetBaseAssetPath(const char *path);	// BaseAssetPath do not end with '/'
const std::string& GetBaseAssetPath();

std::string GetAssetRelativePath(CBaseAsset *a, CBaseAsset *container);

}//GELib


#endif//__GELIB_EXT_BASEASSETMANAGER_H__
