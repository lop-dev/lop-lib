//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/BaseAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_BASEASSET_H__
#define __GELIB_EXT_BASEASSET_H__

#include <string>

namespace GELib
{

class CLoadTask;

class CLoadParameter;

const int ASSETCLASS_BASE = 0;

class CBaseAsset
{
public:
    CBaseAsset();
    virtual ~CBaseAsset();

    // runtime auto generated
    int GetId() const { return m_iId; }
	virtual int GetClassId() { return ASSETCLASS_BASE; }
    virtual void ManagerDelete() {}
	virtual bool CreateLoadedResource() { return true; }

    void SetCacheKey(const char *cacheKey) { if (cacheKey) m_CacheKey = cacheKey; }
    const std::string& GetCacheKey() const { return m_CacheKey; }
	void SetLoadTask(CLoadTask *task) { m_pLoadTask = task; }
    CLoadTask* GetLoadTask() const { return m_pLoadTask; }
    bool IsLoading() { return m_pLoadTask != 0; }
    int IncreaseReferenceCount() { return ++m_iRefCount; }
    int DecreaseReferenceCount() { return --m_iRefCount; }
	int GetReferenceCount() const { return m_iRefCount; }

	void SetName(const char *name) { m_Name = name; }
	const std::string& GetName() { return m_Name; }

    std::string m_Name;
    std::string m_FullFileName;

protected:
    static int s_iIdCounter;

    int m_iRefCount;
    int m_iId;
    CLoadTask *m_pLoadTask;
    std::string m_CacheKey;
};



}//GELib


#endif//__GELIB_EXT_BASEASSET_H__
