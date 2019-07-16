using UnityEngine;
using System;
using System.Collections.Generic;

using UT_ECHOID = System.UInt16;

namespace UDLib.Network
{
    public class CMonoBehaviour : MonoBehaviour
    {
        public CMonoBehaviour()
        {
            m_uEchoID = m_sEchoIDCounter++;
            if ((m_uEchoID + 1) >= UT_ECHOID.MaxValue)
            {
                m_sEchoIDCounter = 1;
            }
            m_msgExecute.MsgFactory = new CSLib.Framework.CMsgFactory();
        }

        /// <summary>
        /// EchoID是客户端用来标识处理不同系统的消息, 统一在ELogicSysEchoID处定义
        /// </summary>
        public UT_ECHOID ECHO
        {
            get { return m_uEchoID; }
            set { m_uEchoID = value; }
        }

        public CTcpClient TcpClient
        {
            get { return m_tcpClient; }
            set
            {
                m_tcpClient = value;
                m_tcpClient.MsgExecute = m_msgExecute;
            }
        }

        public CSLib.Framework.CMsgExecute MsgExecute
        {
            get { return m_msgExecute; }
        }

        public CSLib.Utility.CStatisticsNum<string> SendStatisticsNum
        {
            get { return m_sendStatisticsNum; }
        }

        public CSLib.Utility.CStatisticsNum<string> RecvStatisticsNum
        {
            get { return m_recvStatisticsNum; }
        }

        /// <summary>
        /// 初始化TcpClient
        /// </summary>
        /// <param name="tcpClient">对应Server的Tcp连接，例如CGatewayServerClt.Instance.TcpClient</param>
        /// <returns></returns>
        public bool Initialize(CTcpClient tcpClient)
        {
            if (tcpClient == null)
            {
                return false;
            }

            TcpClient = tcpClient;
            tcpClient.RegisterEchoID(m_uEchoID);

            return true;
        }

        /// <summary>
        /// 添加消息处理函数
        /// </summary>
        /// <param name="type">消息类型</param>
        /// <param name="id">消息ID</param>
        /// <param name="msgExecObj">处理函数委托</param>
        /// <returns></returns>
        public bool AddMsgExecFunc(UInt16 type, UInt16 id, CSLib.Framework.DMsgExecFunc msgExecObj)
        {
            return m_msgExecute.AddMsgExecFunc(type, id, msgExecObj);
        }

        /// <summary>
        /// 此接口慎用，目前是为了给 U3D 中 L# 脚本使用，而创建的接口
        /// </summary>
        /// <param name="msgLabel"></param>
        /// <param name="msgBuff"></param>
        /// <param name="msgSize"></param>
        /// <returns></returns>
        public bool CreateMessage(ref CSLib.Framework.CMessageLabel msgLabel, Byte[] msgBuff, Int32 msgSize)
        {
            return m_msgExecute.CreateMessage(ref msgLabel, msgBuff, msgSize);
        }

        /// <summary>
        /// 添加消息处理函数（逻辑层主要使用）
        /// </summary>
        /// <param name="server">消息的目标服务器类型，客户端是EServerType.ESERVER_GAMECLIENT</param>
        /// <param name="func">消息的系统类型。例如EFuncType.EFUNC_LOGINSYSTEM</param>
        /// <param name="id">消息的枚举ID，定义在各系统消息的。例如EMsgIDLoginSYstem.EMID_XX2XX_MSG</param>
        /// <param name="msgExecObj">处理函数委托</param>
        /// <returns></returns>
        public bool AddMsgExecFunc(byte server, byte func, UInt16 id, CSLib.Framework.DMsgExecFunc msgExecObj)
        {
            return m_msgExecute.AddMsgExecFunc(server, func, id, msgExecObj);
        }

        public bool AddIgnoreTrace(byte server, byte func, UInt16 id)
        {
            return CSLib.Framework.CMsgExecute.AddIgnoreTrace(server, func, id);
        }

        /// <summary>
        /// 这里设置成 virtual 接口，就是给上层一个截获消息机会，方便用其他方式来处理消息
        /// </summary>
        /// <param name="msgLabel"></param>
        /// <param name="msgBuff"></param>
        /// <param name="msgSize"></param>
        /// <returns></returns>
        virtual protected bool ExecuteMessage(CSLib.Framework.CMessageLabel msgLabel, Byte[] msgBuff, Int32 msgSize)
        {
            bool ret = m_msgExecute.ExecuteMessage(msgLabel, msgBuff, msgSize);
            // 处理完收到的消息，更新已收到的最大response消息号，用于作为断线重连的开始位置
            if (msgLabel.ResIndex != 0)
            {
                UDLib.Utility.CDebugOut.Log("收到系统消息，更新断线重连位置 : ResIndex = " + msgLabel.ResIndex);
                CReconnectMgr.Instance.LatestResIndex = msgLabel.ResIndex;
            }

            return ret;
        }

        /// <summary>
        /// 发送消息给Server
        /// </summary>
        /// <param name="netMessage">对应的消息类</param>
        /// <returns></returns>
        protected bool _SendMessage(CSLib.Framework.CNetMessage netMessage)
        {
            if (netMessage == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : netMessage == null");
                return false;
            }

            if (m_tcpClient == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : m_tcpClient == null");
                return false;
            }

            if (m_tcpClient.UseEchoID)
            {
                CSLib.Framework.CEchoMessage echoMessage = (CSLib.Framework.CEchoMessage)netMessage;
                echoMessage.EchoID = m_uEchoID;
            }

            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > _SendMessage 1");
            CSLib.Utility.CStream msgStream = new CSLib.Utility.CStream();
            netMessage.Serialize(msgStream);
            //UnityEngine.Profiling.Profiler.EndSample();
            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > _SendMessage 2");
            bool result = m_tcpClient.SendAsync(msgStream);
            //UnityEngine.Profiling.Profiler.EndSample();
            if (result)
            {
                // 禁止操作
                //UDLib.Utility.CDebugOut.LogError("<Trace> 有消息还没处理完，等待ing");
            }
            return result;
        }

        /// <summary>
        /// 发送消息给Server
        /// </summary>
        /// <param name="netMessage">对应的消息类</param>
        /// <returns></returns>
        public virtual bool SendMessage(CSLib.Framework.CNetMessage msgRequest)
        {
            if(CReconnectMgr.Instance.SimulateSocketClose)
            {
                UDLib.Utility.CDebugOut.Log("客户端模拟断网");
                m_tcpClient.Close();
                m_tcpClient.CbTerminate();
                return false;
            }

            if (msgRequest == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : msgRquest == null");
                return false;
            }

            if (m_tcpClient == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : m_tcpClient == null");
                return false;
            }

            if (!m_tcpClient.IsValid())
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : !m_tcpClient.IsValid()");
                return false;
            }

            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > SendMessage");
            {
                byte tmpServer = CSLib.Utility.CBitHelper.GetHighUInt8(msgRequest.MsgType);
                byte tmpFunc = CSLib.Utility.CBitHelper.GetLowUInt8(msgRequest.MsgType);
                if (!CSLib.Framework.CMsgExecute.IsIgnoreTrace(tmpServer, tmpFunc, msgRequest.Id))
                {
                    //发送消息前加入本地缓存,在这里做可以过滤掉心跳
                    UDLib.Utility.CDebugOut.Log("SendMessage : uServer = " + tmpServer.ToString() + "; uFunc = " + tmpFunc.ToString() + "; uID = " + msgRequest.Id.ToString() + ", reqIndex" + msgRequest.GetReqIndex());
                    // 超时重发机制, 非ack消息，缓存序列好，待收到ack返回从缓存移除
                    m_tcpClient.CacheMessage(msgRequest);
                }
#if DEBUG
                m_sendStatisticsNum.AddNum(tmpServer.ToString() + " -> " + tmpFunc.ToString());
#endif
            }
            //UnityEngine.Profiling.Profiler.EndSample();

            return _SendMessage(msgRequest);
        }

        public bool SendMessage(CSLib.Framework.CNetMessage msgRequest, CSLib.Framework.CMessage msgResponse, Int32 delaySecond = 3)
        {
            if (CReconnectMgr.Instance.SimulateSocketClose)
            {
                UDLib.Utility.CDebugOut.Log("客户端模拟断网");
                m_tcpClient.Close();
                m_tcpClient.CbTerminate();
                return false;
            }

            if (msgRequest == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : msgRquest == null");
                return false;
            }

            if (msgResponse == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : msgResponse == null");
                return false;
            }

            if (m_tcpClient == null)
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : m_tcpClient == null");
                return false;
            }

            if (!m_tcpClient.IsValid())
            {
                return false;
            }

            Int64 curTicks = DateTime.Now.Ticks / 10000; // 转成毫秒
            if (m_tcpClient.DelayedMsg.ContainsKey(msgResponse.UniqueID))
            {
                if (curTicks < m_tcpClient.DelayedMsg.GetObject(msgResponse.UniqueID))
                {
                    return false;
                }
                else
                {
                    m_tcpClient.DelayedMsg.DelObject(msgResponse.UniqueID);
                }
            }

            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > SendMessage");
            {
                byte tmpServer = CSLib.Utility.CBitHelper.GetHighUInt8(msgRequest.MsgType);
                byte tmpFunc = CSLib.Utility.CBitHelper.GetLowUInt8(msgRequest.MsgType);
                if (!CSLib.Framework.CMsgExecute.IsIgnoreTrace(tmpServer, tmpFunc, msgRequest.Id))
                {
                    //发送消息前加入本地缓存,在这里做可以过滤掉心跳
                    UDLib.Utility.CDebugOut.Log("SendMessage : uServer = " + tmpServer.ToString() + "; uFunc = " + tmpFunc.ToString() + "; uID = " + msgRequest.Id.ToString() + ", reqIndex" + msgRequest.GetReqIndex());
                    // 超时重发机制, 非ack消息，缓存序列好，待收到ack返回从缓存移除
                    m_tcpClient.CacheMessage(msgRequest);
                    m_tcpClient.DelayedMsg.AddObject(msgResponse.UniqueID, curTicks + delaySecond * 1000); // 第二个参数转成毫秒
#if DEBUG
                    m_sendStatisticsNum.AddNum(tmpServer.ToString() + " -> " + tmpFunc.ToString());
#endif
                }
            }
            //UnityEngine.Profiling.Profiler.EndSample();

            return _SendMessage(msgRequest);
        }

        /// <summary>
        /// 处理消息
        /// 逻辑层需要在Update里调用这个接口，以处理接收消息
        /// </summary>
        protected void ExecuteMessages()
        {
            if (m_tcpClient == null)
            {
                return;
            }

            CheckTimeout();

            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > ExecuteMessages");
            CSLib.Framework.CMsgBuffInfoQueue msgBuffInfoQueue = m_tcpClient.GetMsgBuffInfoQueue(m_uEchoID);
            while (msgBuffInfoQueue.Count > 0)
            {
                CSLib.Framework.CMessageLabel msgLabel = new CSLib.Framework.CMessageLabel();
                CSLib.Framework.CMsgBuffInfo msgBuffInfo = msgBuffInfoQueue.Dequeue();
                // 执行后 msgLabel.Id 才会被赋值
                if (!ExecuteMessage(msgLabel, msgBuffInfo.MsgBuff, msgBuffInfo.MsgSize))
                {
                    UDLib.Utility.CDebugOut.LogError("ExecuteMessages : 消息执行错误");
                }
#if DEBUG
                CSLib.Framework.CNetMessage msgResult = (CSLib.Framework.CNetMessage)msgLabel.MsgObject;
                byte tmpServer = CSLib.Utility.CBitHelper.GetHighUInt8(msgResult.MsgType);
                byte tmpFunc = CSLib.Utility.CBitHelper.GetLowUInt8(msgResult.MsgType);
                m_recvStatisticsNum.AddNum(tmpServer.ToString() + " -> " + tmpFunc.ToString());
#endif
            }
            //UnityEngine.Profiling.Profiler.EndSample();
        }

        #region 超时重发缓存
        // 超时检测计时(update计数)
        private int timeOutCheckTime = 0;
        // 发送消息时的缓存,收到消息移除，用于处理超时重连
        private Dictionary<CSLib.Framework.CNetMessage, long> msgDict = new Dictionary<CSLib.Framework.CNetMessage, long>();
        // 待清除字典
        private Dictionary<CSLib.Framework.CNetMessage, long> tempMsgDict = new Dictionary<CSLib.Framework.CNetMessage, long>();

        // 发送request消息的时候把消息加入队列
        public bool InsertCacheMessage(CSLib.Framework.CNetMessage message)
        {
            if (msgDict.ContainsKey(message))
            {
                msgDict.Remove(message);
            }

            msgDict.Add(message, DateTime.Now.Ticks/TimeSpan.TicksPerMillisecond);
            return true;
        }

        // 收到response消息的时候从消息队列移除
        public bool RemoveCacheMessageByReqIndex(ushort reqIndex)
        {
            foreach (KeyValuePair<CSLib.Framework.CNetMessage, long> kvp in msgDict)
            {
                if (kvp.Key.GetReqIndex() == reqIndex)
                {

                    return msgDict.Remove(kvp.Key);
                }
            }

            return false;
        }

        // 断线重连时，清除消息缓存队列，避免断线重连的推送和超时重发相互干扰
        public void ClearCacheMessage()
        {
            if (msgDict != null)
            {
                msgDict.Clear();
            }
        }

        // CheckTimeout 检测超时
        public void CheckTimeout()
        {
            if (timeOutCheckTime >= CReconnectMgr.Instance.TimeOutCheckFrequency)
            {
                _CheckTimeout();
                timeOutCheckTime = 0;
            }

            timeOutCheckTime++;
        }

        // 超时重新发送消息
        private void _CheckTimeout()
        {
            if (msgDict == null || msgDict.Count == 0)
                return;

            tempMsgDict.Clear();
            foreach (KeyValuePair<CSLib.Framework.CNetMessage, long> kvp in msgDict)
            {
                if (Math.Abs(DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond - kvp.Value) > CReconnectMgr.Instance.TimeOutDuration)
                {
                    var a = Math.Abs(DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond - kvp.Value);
                    Debug.LogErrorFormat("time passed :{0} ms", a);
                    tempMsgDict.Add(kvp.Key, kvp.Value);
                }
            }

            foreach (KeyValuePair<CSLib.Framework.CNetMessage, long> kvp in tempMsgDict)
            {
                msgDict.Remove(kvp.Key);
                SendMessage(kvp.Key);
                if(CReconnectMgr.Instance.timeoutCallBack != null){
                    CReconnectMgr.Instance.timeoutCallBack(kvp.Key.GetReqIndex(), kvp.Key.MsgType, kvp.Key.Id);
                }
            }
        }
        #endregion

        //
        private static UT_ECHOID m_sEchoIDCounter = 1;
        private UT_ECHOID m_uEchoID = 0;

        private CTcpClient m_tcpClient = null;
        private CSLib.Framework.CMsgExecute m_msgExecute = new CSLib.Framework.CMsgExecute();

        private CSLib.Utility.CStatisticsNum<string> m_sendStatisticsNum = new CSLib.Utility.CStatisticsNum<string>();
        private CSLib.Utility.CStatisticsNum<string> m_recvStatisticsNum = new CSLib.Utility.CStatisticsNum<string>();
    }
}