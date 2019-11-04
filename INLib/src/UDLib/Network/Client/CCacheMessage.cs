using System;
using System.Collections.Generic;

namespace UDLib.Network
{
    public class MessageData
    {
        // 消息
        public CSLib.Framework.CNetMessage netMessage;
        // 发送时间戳
        public long timeStamp;
        // 已重发次数
        public int sentCount;
    }

    public class CCacheMessage
    {
        #region 超时重发缓存
        // 超时检测计时(update计数)
        private int timeOutCheckTime = 0;
        // 发送消息时的缓存,收到消息移除，用于处理超时重连
        private Dictionary<ushort, MessageData> msgDictCache = new Dictionary<ushort, MessageData>();
        //// 处理消息时的临时缓存
        //private List<ushort> tmpMsgList = new List<ushort>();
        // 消息缓存的对象池
        private CSLib.Utility.CObjectPool<MessageData> m_messagePool = new CSLib.Utility.CObjectPool<MessageData>();
        private CTcpClient m_tcpClient;
        // 临时保存msgDictCache的keys
        private List<ushort> m_keys = new List<ushort>();
        // 线程锁
        private static readonly object msgLock = new object();

        public CCacheMessage(CTcpClient tcpClient)
        {
            m_tcpClient = tcpClient;
        }

        // 发送request消息的时候把消息加入队列
        public bool InsertCacheMessage(CSLib.Framework.CNetMessage message)
        {
            lock (msgLock)
            {
                UDLib.Utility.CDebugOut.Log("发送消息时本地缓存消息以便超时重发 > reqIndex : " + message.GetReqIndex());
                ushort reqIndex = message.GetReqIndex();
                if (reqIndex == 0)
                {
                    return false;
                }

                if (msgDictCache.ContainsKey(reqIndex))
                {
                    msgDictCache.Remove(reqIndex);
                }

                var obj = m_messagePool.Obtain();
                obj.timeStamp = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                obj.netMessage = message;
                obj.sentCount = 0;
                msgDictCache.Add(reqIndex, obj);

                //msgDict.Add(message, DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond);
                return true;
            }
        }

        // 收到response消息的时候从消息队列移除
        public bool RemoveCacheMessageByReqIndex(ushort reqIndex)
        {
            lock (msgLock)
            {
                if (!msgDictCache.ContainsKey(reqIndex))
                {
                    msgDictCache.Remove(reqIndex);
                    return true;
                }

                return false;
            }
        }

        // 断线重连时，清除消息缓存队列，避免断线重连的推送和超时重发相互干扰
        public void ClearCacheMessage()
        {
            lock (msgLock)
            {
                if (msgDictCache != null)
                {
                    msgDictCache.Clear();
                }
            }
        }

        // Update 检测超时
        public void Update()
        {
            if (timeOutCheckTime >= CReconnectMgr.Instance.TimeOutCheckFrequency)
            {
                CheckTimeout();
                timeOutCheckTime = 0;
            }

            timeOutCheckTime++;
        }

        // 超时重新发送消息
        private void CheckTimeout()
        {
            if (msgDictCache == null || msgDictCache.Count == 0)
                return;

            lock (msgLock)
            {
                // 重发超时消息，并且更新超时的时间戳和重发次数
                m_keys.Clear();
                m_keys.AddRange(msgDictCache.Keys);
                foreach(ushort reqIndex in m_keys)
                {
                    var messageObject = msgDictCache[reqIndex];
                    if (messageObject.sentCount > CReconnectMgr.Instance.TimeOutRetryCount)
                    {
                        ////TODO, 超时10次判定为断线逻辑，暂不启用
                        //CDebugOut.LogWarning(string.Format("消息reqIndex {0}, id : {1} 已发送{2}次，网络太差，判定为断线，关闭socket", key, messageObject.netMessage.Id, 
                        //    CReconnectMgr.Instance.TimeOutRetryCount));
                        //m_tcpClient.Close();
                        //ClearCacheMessage();
                        //break;
                    }

                    var a = Math.Abs(DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond - messageObject.timeStamp);
                    CSLib.Utility.CDebugOut.Log(string.Format("time passed :{0} ms", a));

                    CSLib.Utility.CStream msgStream = new CSLib.Utility.CStream();

                    messageObject.sentCount++;
                    messageObject.timeStamp = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                    messageObject.netMessage.Serialize(msgStream);
                    SendMessage(msgStream);
                    if (CReconnectMgr.Instance.timeoutCallBack != null)
                    {
                        CReconnectMgr.Instance.timeoutCallBack(reqIndex, messageObject.netMessage.MsgType, messageObject.netMessage.Id);
                    }
                }
            }
        }

        private void SendMessage(CSLib.Utility.CStream msgStream)
        {
            m_tcpClient.SendAsync(msgStream);
        }
        #endregion
    }
}