//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/ActAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ACTASSET_H__
#define __GELIB_EXT_ACTASSET_H__

#include <GELib/Ext/BaseAsset.h>
#include <string>
#include <vector>

namespace GELib
{

class CActManager;
class CIOStream;
class CAssetHelper;
class CFxAsset;
class CCharacter;

const int ASSETCLASS_ACT = 14;


class CActAsset : public CBaseAsset
{
public:

    CActAsset(CActManager *manager=0, const char *cacheKey=0);
    virtual ~CActAsset();

	virtual int GetClassId() { return ASSETCLASS_ACT; }
    CActManager* GetManager() const { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
	bool Load(CIOStream &stream, const CLoadParameter &param);
    void Unload();
	virtual bool CreateLoadedResource();

    void GetAssets(CAssetHelper &helper);

	unsigned short AppendString(const std::string &s);
	const std::string& GetString(unsigned short id);

	std::vector<unsigned char>& GetFrameData() { return m_FrameData; }
	void AppendFrame(float t);
	int GetNumFrames() { return m_iNumFrames; }
	float GetDuration() { return m_fDuration; }

private:

	void LoadAssets(const CLoadParameter &param);

	std::vector<std::string> m_StringTable;
	std::vector<CCharacter*> m_Characters;
	std::vector<CFxAsset*> m_Fxs;
	std::vector<unsigned char> m_FrameData;
	int m_iNumFrames;
	float m_fDuration;

    CActManager *m_pManager;
	bool m_bNeedCreateLoadedResource;
};


}//GELib

#endif//__GELIB_EXT_ACTASSET_H__
