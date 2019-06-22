using System;
using System.Collections.Generic;
using System.Text;
using CSLib.Utility;

namespace UDLib.Network
{
    public class CTcpClient : CSLib.Framework.CTcpClient
    {
        public CTcpClient()
        {
            m_cacheMessage = new CCacheMessage(this);
        }

        public CSLib.Framework.CMsgExecute MsgExecute
        {
            get { return m_msgExecute; }
            set { m_msgExecute = value; }
        }

        protected override Boolean _CbParseMsg(Byte[] msgBuff, Int32 msgSize)
        {
            if (m_msgExecute == null)
            {
                return true;
            }

            CSLib.Framework.CMessageLabel msgLabel = new CSLib.Framework.CMessageLabel();
            bool ret = m_msgExecute.CreateMessage(ref msgLabel, msgBuff, msgSize);

            CSLib.Framework.CNetMessage theMsg = (CSLib.Framework.CNetMessage)(msgLabel.MsgObject);
            // ack 的 response不做解析和向服务器发送ack
            if(theMsg != null && theMsg.Func == CReconnectMgr.EFUNC_MESSAGESYSTEM)
            {
                UDLib.Utility.CDebugOut.Log("收到ack消息， 从消息重发队列移除消息 > reqIndex : " + theMsg.GetReqIndex());
                m_cacheMessage.RemoveCacheMessageByReqIndex(msgLabel.ReqIndex);
                return true;
            }

            // 根据这边的 Index 数据，发送 Ack 信息, 排除心跳为0
            if (msgLabel.ResIndex != 0)
            {
                // 10 Gateway, 5 ACK系统号， 2 客户端发送给服务的消息号，在逻辑层的proto定义，底层无法获取，这里用直接用数字了
                UDLib.Network.CBufMessage netMessage = new UDLib.Network.CBufMessage(10, 5, 2);
                netMessage.SetReqIndex(0);
                netMessage.SetResIndex(msgLabel.ResIndex);
                UDLib.Utility.CDebugOut.Log("收到系统消息消息， 给服务器发送ACK确认 : " + msgLabel.ResIndex);
                CSLib.Utility.CStream msgStream = new CSLib.Utility.CStream(); ;
                netMessage.Serialize(msgStream);
                SendAsync(msgStream);
            }

            base._CbParseMsg(msgBuff, msgSize);
            return true;
        }

        private CSLib.Framework.CMsgExecute m_msgExecute = null;
        private CCacheMessage m_cacheMessage = null;

        public void Update()
        {
            m_cacheMessage.Update();
        }

        public void CacheMessage(CSLib.Framework.CNetMessage msg)
        {
            m_cacheMessage.InsertCacheMessage(msg);
        }

        public void ClearCacheMessage()
        {
            m_cacheMessage.ClearCacheMessage();
        }
    }
}
