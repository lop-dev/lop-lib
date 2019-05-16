//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/fsmStateProcess.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_FSMSTATEPROCESS_H__
#define __BCLIB_FRAMEWORK_FSMSTATEPROCESS_H__

#include <BCLib/framework/baseDef.h>
#include <BCLib/utility/singleton.h>
#include <BCLib/utility/hashMap.h>
#include <BCLib/framework/message.h>

namespace BCLib
{
namespace Framework
{

class CFSMManager;

class BCLIB_FRAMEWORK_API CFSMStateProcess
{
public:
	CFSMStateProcess(CFSMManager * fsmManager);
    virtual ~CFSMStateProcess();

public:
	virtual void processIn();
	virtual void processInput(int iInputID);
	virtual void processState(float fDeltaTime);
	virtual void processOut();

protected:
	int _stateTransition(int iInputID);

public:
	void setOwner(void * pOwner) { m_objOwner = pOwner;	}
	void * getOwner() { return m_objOwner; }

private:
	CFSMManager * m_fsmManager;
	void * m_objOwner;
};

}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_FSMSTATEPROCESS_H__
