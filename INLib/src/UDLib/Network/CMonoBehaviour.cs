using UnityEngine;
using System;

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
            return m_msgExecute.ExecuteMessage(msgLabel, msgBuff, msgSize);
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
                return false;
            }

            if (m_tcpClient == null)
            {
                return false;
            }

            if (m_tcpClient.UseEchoID)
            {
                CSLib.Framework.CEchoMessage echoMessage = (CSLib.Framework.CEchoMessage)netMessage;
                echoMessage.EchoID = m_uEchoID;
            }

            CSLib.Utility.CStream msgStream = new CSLib.Utility.CStream();
            netMessage.Serialize(msgStream);
            bool result = m_tcpClient.SendAsync(msgStream);
            if (result)
            {
                // 禁止操作
                //UDLib.Utility.CDebugOut.LogError("<Trace> 有消息还没处理完，等待ing");
            }
            return result;
        }

        //protected bool _SendMessage(SDelayedMsg delayedMsg)
        //{
        //    if (delayedMsg.msgRequest == null)
        //    {
        //        return false;
        //    }

        //    bool bResult = _SendMessage(delayedMsg.msgRequest);

        //    if (bResult)
        //    {
        //        CDelayedMsgMgr.Instance.DelHeadDelayedMsg();
        //    }

        //    return true;
        //}

        /// <summary>
        /// 插入消息（目前只有重连消息才可以调用这个方法）
        /// </summary>
        /// <param name="msgRquest"></param>
        /// <param name="msgResponse"></param>
        /// <returns></returns>
        //protected bool InsertMessage(CSLib.Framework.CNetMessage msgRequest)
        //{
        //    if (msgRequest == null)
        //    {
        //        return false;
        //    }

        //    if (m_tcpClient == null)
        //    {
        //        return false;
        //    }

        //    if (!m_tcpClient.IsValid())
        //    {
        //        return false;
        //    }

        //    if (!CDelayedMsgMgr.Instance.InsertDelayedMsg(msgRequest))
        //    {
        //        return false;
        //    }

        //    SDelayedMsg delayedMsg = CDelayedMsgMgr.Instance.GetHeadDelayedMsg();
        //    return _SendMessage(delayedMsg);
        //}

        /// <summary>
        /// 发送消息给Server
        /// </summary>
        /// <param name="netMessage">对应的消息类</param>
        /// <returns></returns>
        public virtual bool SendMessage(CSLib.Framework.CNetMessage msgRequest)
        {
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
                return false;
            }

            {
                byte tmpServer = CSLib.Utility.CBitHelper.GetHighUInt8(msgRequest.MsgType);
                byte tmpFunc = CSLib.Utility.CBitHelper.GetLowUInt8(msgRequest.MsgType);
                if (!CSLib.Framework.CMsgExecute.IsIgnoreTrace(tmpServer, tmpFunc, msgRequest.Id))
                {
                    UDLib.Utility.CDebugOut.Log("++++++++++ SendMessage : uServer = " + tmpServer.ToString() + "; uFunc = " + tmpFunc.ToString() + "; uID = " + msgRequest.Id.ToString());
                }
            }

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

            CSLib.Framework.CMsgBuffInfoQueue msgBuffInfoQueue = m_tcpClient.GetMsgBuffInfoQueue(m_uEchoID);
            while (msgBuffInfoQueue.Count > 0)
            {
                CSLib.Framework.CMessageLabel msgLabel = new CSLib.Framework.CMessageLabel();
                CSLib.Framework.CMsgBuffInfo msgBuffInfo = msgBuffInfoQueue.Dequeue();

                // 执行后 msgLabel.Id 才会被赋值
                if (!ExecuteMessage(msgLabel, msgBuffInfo.MsgBuff, msgBuffInfo.MsgSize))
                {
                    UDLib.Utility.CDebugOut.LogError("消息执行错误");
                }
            }
        }

        //
        private static UT_ECHOID m_sEchoIDCounter = 1;
        private UT_ECHOID m_uEchoID = 0;

        private CTcpClient m_tcpClient = null;
        private CSLib.Framework.CMsgExecute m_msgExecute = new CSLib.Framework.CMsgExecute();
    }
}