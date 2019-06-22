using System;

namespace UDLib.Utility
{
    public class CTimeTicker : CTicker
    {
        private float m_delay;
        private int m_currentCount;
        private int m_repeatCount;
        private float m_tickTime;

        public CTimeTicker(float delay_, int repeatCount_, object data = null)
        {
            if (delay_ < 0f)
            {
                throw new Exception("delay不能为负数");
            }
            this.m_delay = delay_;
            if (repeatCount_ < 0)
            {
                throw new Exception("repeatCount 不能为负数");
            }
            this.m_repeatCount = repeatCount_;
            base.data = data;
            this.Reset();
        }

        public override void Reset()
        {
            this.m_currentCount = 0;
            this.m_tickTime = 0;
        }

        public override void Tick(float time)
        {
            this.m_tickTime += time;
            do
            {
                if (!(this.m_tickTime >= this.m_delay))
                {
                    return;
                }
                this.m_tickTime -= this.m_delay;
                this.m_currentCount++;
                if (base.TimerFuncEmpty != null)
                {
                    base.TimerFuncEmpty();
                }
                else if (base.TimerFuncData != null)
                {
                    base.TimerFuncData(base.data);
                }
            } while (this.m_repeatCount <= 0 || this.m_currentCount < this.m_repeatCount);
            base.ATickCompleteFunc(this);
            base.Dispose();
        }
    }
}