using System;
using System.Collections;
using UnityEngine;

namespace UDLib.Utility
{
    public class MbTickManager : MonoBehaviour
    {
        private static MbTickManager m_instance;

        private float m_lastTime;
        private bool m_isRunning;
        private ArrayList m_tickerList;

        public static MbTickManager Instance
        {
            get
            {
                return MbTickManager.m_instance;
            }
        }

        private void Awake()
        {
            if (MbTickManager.m_instance != null)
            {
                throw new Exception("SingleTon Class!Get it by Instance attribute!");
            }

            MbTickManager.m_instance = this;
            this.m_tickerList = new ArrayList();
        }

        private void FixedUpdate()
        {
            if (this.m_isRunning)
            {
                float time = Time.time;
                float dtime = time - this.m_lastTime;
                this.m_lastTime = time;
                this.DoTick(dtime);

            }
        }

        private void DoTick(float dtime)
        {
            for (int i = 0; i < this.m_tickerList.Count; i++)
            {
                CTicker ticker = (CTicker)this.m_tickerList[i];
                if (ticker != null)
                {
                    ticker.Tick(dtime);
                }
            }
        }

        public void AddTicker(CTicker t)
        {
            this.m_tickerList.Add(t);
        }

        public void RemoveTicker(CTicker t)
        {
            int num = this.m_tickerList.IndexOf(t);
            if (num != -1)
            {
                this.m_tickerList.RemoveAt(num);
            }
        }

        public void StopTick()
        {
            this.m_isRunning = false;
        }

        public void StartTick()
        {
            this.m_lastTime = Time.time;
            this.m_isRunning = true;
        }
    }
}

