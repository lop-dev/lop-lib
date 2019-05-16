using System;
using System.Collections.Generic;
using System.Text;

namespace FSM
{
class CFSMSample02
{
    // 定义状态机需要的变量
    protected CSLib.Framework.CFSMManager m_fsmManager = new CSLib.Framework.CFSMManager();

	// 定义 盒子 的状态（不是必须从1开始，也不是必须以1递增）
	private static byte m_fsmStateShift = 1;

	private static byte FSM_STATE_APPEAR = m_fsmStateShift++;
	private static byte FSM_STATE_ROLLING = m_fsmStateShift++;
	private static byte FSM_STATE_DISAPPEAR = m_fsmStateShift++;
	private static byte FSM_STATE_DEATH = m_fsmStateShift++;

	// 定义 盒子 的状态触发条件
	private static byte FSM_INPUT_APPEAR = 1;
	private static byte FSM_INPUT_ROLLING = 2;
	private static byte FSM_INPUT_DISAPPEAR = 3;
	private static byte FSM_INPUT_DEATH = 4;

    protected int m_iCurStateID = 0;

	public CFSMSample02()
	{
        setFSMStates();
	}

	public void update()
	{
        while (true)
        {
            if (m_fsmManager != null)
            {
                if (m_iCurStateID == FSM_STATE_APPEAR)
                {
                    stateAppearProcessState();
                }
                else if (m_iCurStateID == FSM_STATE_ROLLING)
                {
                    stateRollingProcessState();
                }
                else if (m_iCurStateID == FSM_STATE_DISAPPEAR)
                {
                    stateDisappearProcessState();
                }
                else if (m_iCurStateID == FSM_STATE_DEATH)
                {
                    stateDeathProcessState();
                }
            }
        }
	}

	protected void setFSMStates()
	{
        CSLib.Framework.CFSMState tmpState = null;

		// 出现的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_APPEAR);
		tmpState.addTransition(FSM_INPUT_ROLLING, FSM_STATE_ROLLING);
		m_fsmManager.addState(tmpState);

		// 旋转的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_ROLLING);
		tmpState.addTransition(FSM_INPUT_DISAPPEAR, FSM_STATE_DISAPPEAR);
		m_fsmManager.addState(tmpState);

		// 消失的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_DISAPPEAR);
		tmpState.addTransition(FSM_INPUT_DEATH, FSM_STATE_DEATH);
		m_fsmManager.addState(tmpState);

		// 死亡的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_DEATH);
		tmpState.addTransition(FSM_INPUT_APPEAR, FSM_STATE_APPEAR);
		m_fsmManager.addState(tmpState);

		// 
		fsmSetCurrentStateID(FSM_STATE_APPEAR);

	}

	// //////////////////////////////////////////////////////////////////////////////
	// 改写状态机的开始
	// //////////////////////////////////////////////////////////////////////////////
	
	private void fsmStateProcessIn(int iStateID)
	{
		if (iStateID == FSM_STATE_APPEAR)
		{
			stateAppearProcessIn();
		}
		else if (iStateID == FSM_STATE_ROLLING)
		{
			stateRollingProcessIn();
		}
		else if (iStateID == FSM_STATE_DISAPPEAR)
		{
			stateDisappearProcessIn();
		}
		else if (iStateID == FSM_STATE_DEATH)
		{
			stateDeathProcessIn();
		}
	}
	
	private void fsmStateProcessOut(int iStateID)
	{
		if (iStateID == FSM_STATE_APPEAR)
		{
			stateAppearProcessOut();
		}
		else if (iStateID == FSM_STATE_ROLLING)
		{
			stateRollingProcessOut();
		}
		else if (iStateID == FSM_STATE_DISAPPEAR)
		{
			stateDisappearProcessOut();
		}
		else if (iStateID == FSM_STATE_DEATH)
		{
			stateDeathProcessOut();
		}
	}
	
	private void fsmSetCurrentStateID(int iStateID)
	{
		int iTmpStateID = m_fsmManager.getCurrentStateID();
		fsmStateProcessOut(iTmpStateID);

		m_fsmManager.setCurrentStateID(iStateID);
		m_iCurStateID = m_fsmManager.getCurrentStateID();
		fsmStateProcessIn(m_iCurStateID);
	}
	
	private void fsmStateTransition(int iInputID)
	{
		int iPreStateID = m_fsmManager.getCurrentStateID();

		m_iCurStateID = m_fsmManager.stateTransition(iInputID);

		if (m_iCurStateID != iPreStateID)
		{
			fsmStateProcessOut(iPreStateID);

			fsmStateProcessIn(m_iCurStateID);
		}
	}
	
	//	出现的状态
    float fAppearTime = 0;
	
	private void stateAppearProcessIn()
	{
		m_iCurStateID = FSM_STATE_APPEAR;
        fAppearTime = 0;
	}
	
	private void stateAppearProcessState()
	{
        fAppearTime += 0.1f;

        if (fAppearTime > 3000)
        {
            fsmStateTransition(FSM_INPUT_ROLLING);
        }
	}
	
	private void stateAppearProcessOut()
	{
		;
	}
	
	//	旋转的状态
    float fRollingTime = 0;
	
	private void stateRollingProcessIn()
	{
		m_iCurStateID = FSM_STATE_ROLLING;
        fRollingTime = 0;
	}
	
	private void stateRollingProcessState()
	{
        fRollingTime += 0.1f;

        if (fRollingTime > 3000)
        {
            fsmStateTransition(FSM_INPUT_DISAPPEAR);
        }
	}
	
	private void stateRollingProcessOut()
	{
		;
	}
	
	//	消失的状态
    float fDisappearTime = 0;
	
	private void stateDisappearProcessIn()
	{
		m_iCurStateID = FSM_STATE_DISAPPEAR;
        fDisappearTime = 0;
	}
	
	private void stateDisappearProcessState()
	{
        fDisappearTime += 0.1f;

        if (fDisappearTime > 3000)
        {
            fsmStateTransition(FSM_INPUT_DEATH);
        }
	}
	
	private void stateDisappearProcessOut()
	{
		;
	}
	
	//	死亡的状态
	float fDelayTime = 0;
	
	private void stateDeathProcessIn()
	{
		m_iCurStateID = FSM_STATE_DEATH;
		fDelayTime = 0;
	}
	
	private void stateDeathProcessState()
	{
		fDelayTime += 0.1f;

		if (fDelayTime > 3000)
		{
			fsmStateTransition(FSM_INPUT_APPEAR);
		}
	}
	
	private void stateDeathProcessOut()
	{
		;
	}
	
	// //////////////////////////////////////////////////////////////////////////////
	// 改写状态机的结束
	// //////////////////////////////////////////////////////////////////////////////
}
}
