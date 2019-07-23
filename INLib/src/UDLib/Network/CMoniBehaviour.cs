using UnityEngine;
using System;
using System.Collections.Generic;

using UT_ECHOID = System.UInt16;

namespace UDLib.Network
{
    public class CMoniBehaviour
    {
        public CMoniBehaviour()
        {
            m_uEchoID = m_sEchoIDCounter++;
            if ((m_uEchoID + 1) >= UT_ECHOID.MaxValue)
            {
                m_sEchoIDCounter = 1;
            }
        }

        public CMoniBehaviour(UT_ECHOID uEchoID)
        {
            m_uEchoID = uEchoID;
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

            m_tcpClient = tcpClient;
            m_tcpClient.RegisterEchoID(m_uEchoID);

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
            return CSLib.Framework.CMsgFactory.Instance.AddIgnoreTrace(server, func, id);
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

            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > SendMessage");
            {
                byte tmpServer = CSLib.Utility.CBitHelper.GetHighUInt8(msgRequest.MsgType);
                byte tmpFunc = CSLib.Utility.CBitHelper.GetLowUInt8(msgRequest.MsgType);
                if (!CSLib.Framework.CMsgFactory.Instance.IsIgnoreTrace(tmpServer, tmpFunc, msgRequest.Id))
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


            // 这个Cache后检测，保证断网后仍然可以缓存消息，在网络恢复后不会丢失消息（剧情过程中断网如不缓存会丢失消息导致黑屏）
            if (!m_tcpClient.IsValid())
            {
                UDLib.Utility.CDebugOut.LogError("SendMessage : !m_tcpClient.IsValid()");
                return false;
            }

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
                if (!CSLib.Framework.CMsgFactory.Instance.IsIgnoreTrace(tmpServer, tmpFunc, msgRequest.Id))
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
        public void ExecuteMessages()
        {
            if (m_tcpClient == null)
            {
                return;
            }

            //UnityEngine.Profiling.Profiler.BeginSample("CMoniBehaviour > ExecuteMessages");
            CSLib.Framework.CMsgBuffInfoQueue msgBuffInfoQueue = m_tcpClient.GetMsgBuffInfoQueue(m_uEchoID);
            while (msgBuffInfoQueue.Count > 0)
            {
                UDLib.Utility.CDebugOut.Log("ExecuteMessages count : " + msgBuffInfoQueue.Count);
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

        //
        private static UT_ECHOID m_sEchoIDCounter = 1;
        private UT_ECHOID m_uEchoID = 0; // 是客户端用来标识处理不同系统的消息

        private CTcpClient m_tcpClient = null;
        private CSLib.Framework.CMsgExecute m_msgExecute = new CSLib.Framework.CMsgExecute();

        private CSLib.Utility.CStatisticsNum<string> m_sendStatisticsNum = new CSLib.Utility.CStatisticsNum<string>();
        private CSLib.Utility.CStatisticsNum<string> m_recvStatisticsNum = new CSLib.Utility.CStatisticsNum<string>();
    }
}