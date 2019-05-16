//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/AssetLoadTask.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ASSETLOADTASK_H__
#define __GELIB_EXT_ASSETLOADTASK_H__

#include <GELib/Utility/AsyncLoader.h>
#include <GELib/Utility/DataBuffer.h>
#include <string>


namespace GELib
{

class CBaseAsset;

class CLoadParameter
{
public:
	CLoadParameter() { m_ePriority = LOAD_IMMEDIATELY; }

	void SetPathPart(const char *fullFileName, std::string *name=0, std::string *ext=0);
	void SetPath(const char *path);

	ELoadPriority m_ePriority;
	std::string m_Path;
};

class CAssetLoadTask : public CLoadTask
{
public:

    CAssetLoadTask(CBaseAsset *a, const CLoadParameter &param);
    virtual ~CAssetLoadTask();

protected:

	bool LoadFile(const std::string &fname);
	bool LoadFile(const std::string &fname, CDataBuffer &fileData);

    virtual bool DoProcess();
    virtual void DoCleanup();

	CBaseAsset *m_pAsset;
	CLoadParameter m_Param;
	CDataBuffer m_FileData;
};


}//GELib

#endif//__GELIB_EXT_ASSETLOADTASK_H__
