//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/fsmManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_FSMMANAGER_H__
#define __BCLIB_FRAMEWORK_FSMMANAGER_H__

#include <BCLib/framework/baseDef.h>
#include <BCLib/utility/hashMap.h>

namespace BCLib
{
namespace Framework
{

class CFSMState;

class BCLIB_FRAMEWORK_API CFSMManager
{
public:
	CFSMManager();
    virtual ~CFSMManager();

public:
	int getPreviousStateID();
	int getCurrentStateID();
	void setCurrentStateID(int iStateID);
	CFSMState * getState(int iStateID);
	void addState(CFSMState * state);
	void delState(int iStateID);
	int stateTransition(int iInputID);
	void update(float fDeltaTime);
	void release();

	//
	void setOwner(void * pOwner) { m_objOwner = pOwner; }
	void * getOwner() { return m_objOwner; }

private:
	typedef BCLib::Utility::CHashMap<int, CFSMState *> THashMap;
	int m_iPreStateID;
	int m_iCurStateID;
	THashMap m_hashMap;
	void * m_objOwner;
};

}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_FSMMANAGER_H__
