using System;
using System.Collections;
using System.Collections.Generic;

namespace UDLib.Utility
{
    public enum EEventType
    {
        EEVENT_TYPE_UNDEFINED,

        EEVENT_TYPE_EXAMPLE1,        

        EEVENT_TYPE_USER_DEFINE = 50,
    }

    public class CEvent
    {

        public virtual int GetEventType()
        {
            return (int)EEventType.EEVENT_TYPE_UNDEFINED;
        }

        public virtual void Reset()
        {
            ;
        }
    }

    public class CEventExample1 : CEvent
    {
        public int m_intValue = 0;

        public override int GetEventType()
        {
            return (int)EEventType.EEVENT_TYPE_EXAMPLE1;
        }
    }
}//namespace GameLogic

