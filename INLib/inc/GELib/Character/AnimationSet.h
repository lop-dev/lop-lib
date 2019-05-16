//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimationSet.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMATIONSET_H__
#define __GELIB_CHARACTER_ANIMATIONSET_H__

#include <GELib/Utility/StringMap.h>
#include <GELib/Ext/BaseAsset.h>

#include <vector>
#include <string>


namespace GELib
{

class CIOStream;


class CAnimationAsset;
class CAnimationSetManager;

class CAnimValue
{
public:
	CAnimValue() { m_fNumber = 0; }
	CAnimValue(float v) { m_fNumber = v; }
	CAnimValue(int v) { m_fNumber = (float)v; }
	CAnimValue(bool v) { m_fNumber = v ? 1.0f : 0; }
	CAnimValue(const char *s) { m_String = s; m_fNumber = 0; }

	float m_fNumber ;
	std::string m_String;
};

class CAnimParamList
{
public:
	CAnimParamList() { m_iId = 0; }
	int Size() { return (int)m_Values.size(); }
	float GetFloat(int index) { return index < Size() ? m_Values[index].m_fNumber : 0; }
	int GetInt(int index) { return index < Size() ? (int)m_Values[index].m_fNumber : 0; }
	bool GetBool(int index) { return index < Size() ? m_Values[index].m_fNumber != 0 : false; }
	const char * GetString(int index) { return index < Size() ? m_Values[index].m_String.c_str() : ""; }

	short m_iId;
	std::vector<CAnimValue> m_Values;
};

class CAnimDescBase
{
public:
	CAnimDescBase() { m_iId = m_iIndex = 0; }
	CAnimParamList* GetParamList(int id)
	{
		size_t s = m_Params.size();
		for (size_t i=0; i<s; ++i)
		{
			if (m_Params[i].m_iId == id)
				return &m_Params[i];
		}
		return 0;
	}

	short m_iId;
	short m_iIndex;
	std::vector<CAnimParamList> m_Params;
};

class CAnimDescEvent : public CAnimDescBase
{
public:
	CAnimDescEvent() { m_fTime = 0; }
	float m_fTime;

	bool operator<(const CAnimDescEvent &e) const { return m_fTime < e.m_fTime; }
};

typedef std::vector<CAnimDescEvent*> AnimationEventList;

class CAnimDescPeriod : public CAnimDescBase
{
public:
	CAnimDescPeriod() { m_fFromTime = m_fToTime = 0; }
	float m_fFromTime;
	float m_fToTime;

	bool operator<(const CAnimDescPeriod &p) const { return m_fFromTime < p.m_fFromTime; }
};

class CAnimationDesc : public CAnimDescBase
{
public:
	CAnimationDesc() { m_pAnimation = 0; }
	std::string m_Name;
	std::string m_AnimationFileName;
	CAnimationAsset *m_pAnimation;
	std::vector<CAnimDescEvent> m_Events;   // sort by time
	std::vector<CAnimDescPeriod> m_Periods; // sort by time
};

const int ASSETCLASS_ANIMATION_SET = 9;


class CAnimationSet : public CBaseAsset
{
public:
    CAnimationSet(CAnimationSetManager *manager=0, const char *cacheKey=0);
    virtual ~CAnimationSet();

	virtual int GetClassId() { return ASSETCLASS_ANIMATION_SET; }
    CAnimationSetManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool LoadScript(const void *file, int fileSize, const char *path=0);
    void Unload();

    int GetNumAnimationDescs();
    CAnimationDesc* GetAnimationDescByIndex(int index);
    CAnimationDesc* GetAnimationDescByName(const char *name);

    // used by script
    CAnimationDesc* AddAnimationDesc(const char *name);

 private:
    CAnimationSetManager *m_pManager;
    std::vector<CAnimationDesc> m_AnimationDescs;
    StringVectorMap<short> m_AnimationNameMap;
};


}//GELib


#endif//__GELIB_CHARACTER_ANIMATIONSET_H__
