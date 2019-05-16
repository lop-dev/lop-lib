using System;
using System.Collections.Generic;
using System.Text;

namespace FSM
{
class CFSMSample01
{
    // 定义状态机需要的变量
    protected CSLib.Framework.CFSMManager m_fsmManager = new CSLib.Framework.CFSMManager();

    // 定义 盒子 的状态（不是必须从1开始，也不是必须以1递增）
	private static byte m_fsmStateShift = 5;

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


    public class CFSMStateProcessAppear : CSLib.Framework.CFSMStateProcess
    {
        public CFSMStateProcessAppear(CSLib.Framework.CFSMManager fsmManager)
            : base(fsmManager)
        {
            ;
        }

        public override void processIn()
        {
            m_fDelayTime = 0;
        }

        public override void processInput(int iInputID)
        {
        }

        public override void processState(float fDeltaTime)
        {
            m_fDelayTime += fDeltaTime;

            if (m_fDelayTime > 3000)
            {
                stateTransition(FSM_INPUT_ROLLING);
            }
        }

        public override void processOut()
        {
        }

        private float m_fDelayTime = 0;
    }

    public class CFSMStateProcessRolling : CSLib.Framework.CFSMStateProcess
    {
        public CFSMStateProcessRolling(CSLib.Framework.CFSMManager fsmManager)
            : base(fsmManager)
        {
            ;
        }

        public override void processIn()
        {
            m_fDelayTime = 0;
        }

        public override void processInput(int iInputID)
        {
        }

        public override void processState(float fDeltaTime)
        {
            m_fDelayTime += fDeltaTime;

            if (m_fDelayTime > 3000)
            {
                stateTransition(FSM_INPUT_DISAPPEAR);
            }
        }

        public override void processOut()
        {
        }

        private float m_fDelayTime = 0;
    }

    public class CFSMStateProcessDisappear : CSLib.Framework.CFSMStateProcess
    {
        public CFSMStateProcessDisappear(CSLib.Framework.CFSMManager fsmManager)
            : base(fsmManager)
        {
            ;
        }

        public override void processIn()
        {
            m_fDelayTime = 0;
        }

        public override void processInput(int iInputID)
        {
        }

        public override void processState(float fDeltaTime)
        {
            m_fDelayTime += fDeltaTime;

            if (m_fDelayTime > 3000)
            {
                stateTransition(FSM_INPUT_DEATH);
            }
        }

        public override void processOut()
        {
        }

        private float m_fDelayTime = 0;
    }

    public class CFSMStateProcessDeath : CSLib.Framework.CFSMStateProcess
    {
        public CFSMStateProcessDeath(CSLib.Framework.CFSMManager fsmManager)
            : base(fsmManager)
        {
            ;
        }

        public override void processIn()
        {
            m_fDelayTime = 0;
        }

        public override void processInput(int iInputID)
        {
        }

        public override void processState(float fDeltaTime)
        {
            m_fDelayTime += fDeltaTime;

            if (m_fDelayTime > 3000)
            {
                stateTransition(FSM_INPUT_APPEAR);
            }
        }

        public override void processOut()
        {
        }

        private float m_fDelayTime = 0;
    }

    //////////////////////////////////////////////////////////////////////////

	public CFSMSample01()
	{
        setFSMStates();
	}

	public void update()
	{
        while (true)
        {
            if (m_fsmManager != null)
            {
                float fDeltaTime = 0.1f;
                m_fsmManager.update(fDeltaTime);
            }
        }
	}

	protected void setFSMStates()
	{
        CSLib.Framework.CFSMState tmpState = null;

		// 出现的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_APPEAR);
		tmpState.addTransition(FSM_INPUT_ROLLING, FSM_STATE_ROLLING);
        CFSMStateProcessAppear fsmStateProcessAppear = new CFSMStateProcessAppear(m_fsmManager);
        tmpState.addStateProcess(fsmStateProcessAppear);
		m_fsmManager.addState(tmpState);

		// 旋转的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_ROLLING);
		tmpState.addTransition(FSM_INPUT_DISAPPEAR, FSM_STATE_DISAPPEAR);
        CFSMStateProcessRolling fsmStateProcessRolling = new CFSMStateProcessRolling(m_fsmManager);
        tmpState.addStateProcess(fsmStateProcessRolling);
		m_fsmManager.addState(tmpState);

		// 消失的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_DISAPPEAR);
		tmpState.addTransition(FSM_INPUT_DEATH, FSM_STATE_DEATH);
        CFSMStateProcessDisappear fsmStateProcessDisappear = new CFSMStateProcessDisappear(m_fsmManager);
        tmpState.addStateProcess(fsmStateProcessDisappear);
		m_fsmManager.addState(tmpState);

		// 死亡的状态
        tmpState = new CSLib.Framework.CFSMState(FSM_STATE_DEATH);
		tmpState.addTransition(FSM_INPUT_APPEAR, FSM_STATE_APPEAR);
        CFSMStateProcessDeath fsmStateProcessDeath = new CFSMStateProcessDeath(m_fsmManager);
        tmpState.addStateProcess(fsmStateProcessDeath);
		m_fsmManager.addState(tmpState);

		// 
        m_fsmManager.setCurrentStateID(FSM_STATE_APPEAR);
	}
}
}
