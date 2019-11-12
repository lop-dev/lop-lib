using System;

namespace UDLib.Network
{
    public class CTcpClient : CSLib.Framework.CTcpClient
    {
        public CTcpClient()
        {
            m_cacheMessage = new CCacheMessage(this);
        }

        public CSLib.Utility.CDictionary<UInt32, Int64> DelayedMsg
        {
            get { return m_dicDelayedMsg; }
            set { m_dicDelayedMsg = value; }
        }

        protected override Boolean _CbParseMsg(Byte[] msgBuff, Int32 msgSize)
        {
            CSLib.Framework.CMessageLabel msgLabel = CSLib.Framework.CMsgFactory.Instance.CreateMsg(msgBuff, msgSize);
            if (msgLabel == null)
            {
                return true;
            }

            CSLib.Framework.CNetMessage theMsg = (CSLib.Framework.CNetMessage)(msgLabel.MsgObject);

            // 删除发送时做的消息缓存,判空应对一些不规范的返回（GM命令系统返回是空）
            if (theMsg != null)
            {
                m_dicDelayedMsg.DelObject(theMsg.UniqueID);
            }

            // ack 的 response不做解析和向服务器发送ack
            if (theMsg != null && theMsg.Func == CReconnectMgr.EFUNC_MESSAGESYSTEM)
            {
#if DEBUG
                UDLib.Utility.CDebugOut.Log("收到ack消息， 从消息重发队列移除消息 > reqIndex : " + theMsg.GetReqIndex());
#endif
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
#if DEBUG
                UDLib.Utility.CDebugOut.Log("收到系统消息消息， 给服务器发送ACK确认 : " + msgLabel.ResIndex);
#endif
                CSLib.Utility.CStream msgStream = new CSLib.Utility.CStream(); ;
                netMessage.Serialize(msgStream);
                SendAsync(msgStream);
            }

            base._CbParseMsg(msgBuff, msgSize);
            return true;
        }

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

        private CSLib.Utility.CDictionary<UInt32, Int64> m_dicDelayedMsg = new CSLib.Utility.CDictionary<UInt32, Int64>(); // 第二个参数转成毫秒
    }
}
