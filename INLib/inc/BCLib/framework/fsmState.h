//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/framework/fsmState.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_FRAMEWORK_FSMSTATE_H__
#define __BCLIB_FRAMEWORK_FSMSTATE_H__

#include <BCLib/framework/baseDef.h>

namespace BCLib
{
namespace Framework
{

class CFSMStateProcess;

class BCLIB_FRAMEWORK_API CFSMState
{
public:
	CFSMState(int stateID);
    virtual ~CFSMState();

public:
	int getStateID();
	CFSMStateProcess * getStateProcess();

	void addTransition(int iInput, int iOutputID);
	void delTransition(int iOutputID);
	int getOutput(int iInput);
	void addStateProcess(CFSMStateProcess *fsmStateProcess);
	void processState(float fDeltaTime);

private:
	int m_iStateID;
	int m_iTransNum;

	int* m_aInput;
	int* m_aOutputState;

	CFSMStateProcess * m_fsmStateProcess;
};

}//Framework
}//BCLib


#endif//__BCLIB_FRAMEWORK_FSMSTATE_H__
