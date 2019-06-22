/**
 * 
 * create at 2019-5-5
 * 断线重连管理器，用于处理断线重连记录
 *  
 * */
using System;

namespace UDLib.Network {
    public class CReconnectMgr : CSLib.Utility.CSingleton<CReconnectMgr>
    {
        // ACK系统的号，在逻辑层的proto定义，底层无法获取，这里用直接用数字了
        public static int EFUNC_MESSAGESYSTEM = 5;
        // 保存一个最新的response 序列号，用于断线重连的时候告诉服务器从哪里开始重发
        private UInt16 m_latestResIndex = 0;
        // 超时时间3s
        private float m_timeOutDuration = 3000;
        // 超时重发最大次数，超过这个次数，判定为断线
        private float m_timeOutRetryCount = 10;
        // 超时检测频率（几帧检查一次）
        private int m_timeOutCheckFrequency = 5;
        public delegate void DTimeoutCallback(UInt16 index, ushort msgType, ushort msgId);
        public DTimeoutCallback timeoutCallBack;
        //
        public static UInt16 m_Index = 0;

        public float TimeOutDuration
        {
            set { m_timeOutDuration = value; }
            get { return m_timeOutDuration; }
        }

        public int TimeOutCheckFrequency
        {
            set { m_timeOutCheckFrequency = value; }
            get { return m_timeOutCheckFrequency; }
        }

        public UInt16 LatestResIndex
        {
            set
            {
                if (value > m_latestResIndex || m_latestResIndex == UInt16.MaxValue)
                {
                    m_latestResIndex = value;
                }
            }
            get { return m_latestResIndex; }
        }

        public float TimeOutRetryCount
        {
            set { m_timeOutRetryCount = value; }
            get { return m_timeOutRetryCount; }
        }



        public void AddTimeoutCallBack(DTimeoutCallback callback)
        {
            if(timeoutCallBack == null)
            {
                timeoutCallBack = callback;
            }else
            {
                timeoutCallBack += callback;
            }
        }

        // 模拟断网
        private bool m_simulateSocketClose = false;
        public bool SimulateSocketClose
        {
            set { m_simulateSocketClose = value; }
            get { return m_simulateSocketClose; }
        }
    }
}