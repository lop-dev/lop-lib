using System.Collections;
using System.Collections.Generic;

namespace UDLib.Utility
{
    /// <summary>
    /// 此 Event 主要是处理 自定义 相关的事件，于 CEvent 配合使用；
    /// 可以参考文件尾部的 CExampleEventHandler 用例
    /// </summary>
    public class CEventMgr : CSLib.Utility.CSingleton<CEventMgr>
    {
        public delegate void TEventHandler(CEvent e);

        public CEventMgr()
        {
        }

        public void RegisterEventHandler(TEventHandler handler)
        {
            if (_isExist(handler))
            {
                return;
            }

            if (m_iSendingEventDepth == 0)
            {
                m_eventHandlers.Add(handler);
            }
            else
            {
                m_deferRegisterHandles.Add(handler);
            }
        }

        public void UnregisterEventHandler(TEventHandler handler)
        {
            if (m_iSendingEventDepth == 0)
            {
                for (int i = 0; i < m_eventHandlers.Count; ++i)
                {
                    if (m_eventHandlers[i] == handler)
                    {
                        m_eventHandlers.RemoveAt(i);
                        return;
                    }
                }
            }
            else
            {
                m_deferUnRegisterHandles.Add(handler);
            }
        }

        public void SendEvent(CEvent e)
        {
            m_iSendingEventDepth++;
            foreach (TEventHandler handler in m_eventHandlers)
            {
                handler(e);
            }
            m_iSendingEventDepth--;
            _postSendEvent();
        }

        public void Clear()
        {
            m_eventHandlers.Clear();
            m_deferRegisterHandles.Clear();
        }

        private bool _isExist(TEventHandler handler)
        {
            foreach (TEventHandler existHandler in m_eventHandlers)
            {
                if (existHandler == handler)
                {
                    return true;
                }
            }
            return false;
        }

        private void _postSendEvent()
        {
            if (m_iSendingEventDepth > 0)
            {
                return;
            }

            if (m_deferRegisterHandles.Count > 0)
            {
                foreach (TEventHandler handler in m_deferRegisterHandles)
                {
                    RegisterEventHandler(handler);
                }
                m_deferRegisterHandles.Clear();
            }

            if (m_deferUnRegisterHandles.Count > 0)
            {
                foreach (TEventHandler handler in m_deferUnRegisterHandles)
                {
                    UnregisterEventHandler(handler);
                }
                m_deferUnRegisterHandles.Clear();
            }
        }

        //
        private List<TEventHandler> m_eventHandlers = new List<TEventHandler>();
        private List<TEventHandler> m_deferRegisterHandles = new List<TEventHandler>();
        private List<TEventHandler> m_deferUnRegisterHandles = new List<TEventHandler>();
        private int m_iSendingEventDepth = 0;
    }

    //////////////////////////////////////////////////////////////////////////////////
    public class CExampleEventHandler
    {
        public CExampleEventHandler()
        {
            CEventMgr.Instance.RegisterEventHandler(this.ProcessEvent);
        }

        ~CExampleEventHandler()
        {
            CEventMgr.Instance.UnregisterEventHandler(this.ProcessEvent);
        }

        public void ProcessEvent(CEvent e)
        {
            EEventType eventId = (EEventType)e.GetEventType();
            switch (eventId)
            {
                case EEventType.EEVENT_TYPE_EXAMPLE1:
                    {
                        UDLib.Utility.CDebugOut.Log("This is test Event Example 1");
                    }
                    break;
                default:
                    break;
            }
        }
    }
}//namespace GameLogic