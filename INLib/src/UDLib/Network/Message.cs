using System;
using System.IO;

namespace UDLib.Network
{
    /// <summary>
    /// 处理 Protobuf 的序列化和反序列化
    /// </summary>
    /// <typeparam name="CProtoBuf"></typeparam>
    [Serializable]
    public class CBufMessage<CProtoBuf> : CSLib.Framework.CNetMessage
    {
        public CBufMessage(UInt16 type, UInt16 id)
            : base(type, id)
        {
            m_ReqIndex = ++CReconnectMgr.m_Index;
            if(CReconnectMgr.m_Index >= UInt16.MaxValue)
            {
                CReconnectMgr.m_Index = 1;
            }
        }

        public CBufMessage(byte server, byte func, UInt16 id)
            : base(CSLib.Utility.CBitHelper.MergeUInt8(server, func), id)
        {
            // TODO, 暂时注释消息的序列号
            m_ReqIndex = ++CReconnectMgr.m_Index;
            if (CReconnectMgr.m_Index >= ushort.MaxValue)
            {
                CReconnectMgr.m_Index = 1;
            }
        }

        // 支持对 Protobuf 的序列化和反序列化
        public bool Serialize(ref CProtoBuf ptBuf)
        {
            MemoryStream tmpStream = new MemoryStream();
            ProtoBuf.Serializer.Serialize<CProtoBuf>(tmpStream, ptBuf);
            m_Buf = tmpStream.ToArray();
            return true;
        }

        public bool Deserialize(ref CProtoBuf ptBuf)
        {
            if (m_Buf.Length == 0) return false;
            MemoryStream tmpStream = new MemoryStream(m_Buf);
            ptBuf = ProtoBuf.Serializer.Deserialize<CProtoBuf>(tmpStream);
            return true;
        }

        public void SetReqIndex(UInt16 index)
        {
            m_ReqIndex = index;
        }

        public void SetResIndex(UInt16 index)
        {
            m_ResIndex = index;
        }

        public override UInt16 GetReqIndex()
        {
            return m_ReqIndex;
        }
        public override UInt16 GetResIndex()
        {
            return m_ResIndex;
        }

        //
        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            base._Deserialize(stream);
            stream.Read(ref m_Buf);
            stream.Read(ref m_Result);
            stream.Read(ref m_ReqIndex);
            stream.Read(ref m_ResIndex);
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            base._Serialize(stream);
            stream.Write(m_Buf);
            stream.Write(m_Result);
            stream.Write(m_ReqIndex);
            stream.Write(m_ResIndex);
            return true;
        }

        private UInt16 m_ReqIndex = 0;
        private UInt16 m_ResIndex = 0;
    }

    /// <summary>
    /// 处理 Protobuf 的序列化和反序列化
    /// </summary>
    /// <typeparam name="CProtoBuf"></typeparam>
    [Serializable]
    public class CEchoMessage<CProtoBuf> : CSLib.Framework.CEchoMessage
    {
        public CEchoMessage(UInt16 type, UInt16 id)
            : base(type, id)
        {
        }

        public CEchoMessage(byte server, byte func, UInt16 id)
            : base(CSLib.Utility.CBitHelper.MergeUInt8(server, func), id)
        {
        }

        // 支持对 Protobuf 的序列化和反序列化
        public bool Serialize(ref CProtoBuf ptBuf)
        {
            MemoryStream tmpStream = new MemoryStream();
            ProtoBuf.Serializer.Serialize<CProtoBuf>(tmpStream, ptBuf);
            m_Buf = tmpStream.ToArray();
            return true;
        }

        public bool Deserialize(ref CProtoBuf ptBuf)
        {
            if (m_Buf.Length == 0) return false;
            MemoryStream tmpStream = new MemoryStream(m_Buf);
            ptBuf = ProtoBuf.Serializer.Deserialize<CProtoBuf>(tmpStream);
            return true;
        }

        //
        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            base._Deserialize(stream);
            stream.Read(ref m_Buf);
            stream.Read(ref m_Result);
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            base._Serialize(stream);
            stream.Write(m_Buf);
            stream.Write(m_Result);
            return true;
        }
    }

    //////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 处理 Byte[] 的序列化和反序列化
    /// </summary>
    [Serializable]
    public class CBufMessage : CSLib.Framework.CNetMessage
    {
        public CBufMessage()
            : base(0, 0)
        {
            m_ReqIndex = ++CReconnectMgr.m_Index;
            if (CReconnectMgr.m_Index >= UInt16.MaxValue)
            {
                CReconnectMgr.m_Index = 1;
            }
        }

        public CBufMessage(UInt16 type, UInt16 id)
            : base(type, id)
        {
            m_ReqIndex = ++CReconnectMgr.m_Index;
            if (CReconnectMgr.m_Index >= ushort.MaxValue)
            {
                CReconnectMgr.m_Index = 1;
            }
        }

        public CBufMessage(byte server, byte func, UInt16 id)
            : base(CSLib.Utility.CBitHelper.MergeUInt8(server, func), id)
        {
            // TODO, 暂时注释消息的序列号
            m_ReqIndex = ++CReconnectMgr.m_Index;
            if (CReconnectMgr.m_Index >= ushort.MaxValue)
            {
                CReconnectMgr.m_Index = 0;
            }
        }

        public void SetReqIndex(UInt16 index)
        {
            m_ReqIndex = index;
        }

        public void SetResIndex(UInt16 index)
        {
            m_ResIndex = index;
        }

        public override UInt16 GetReqIndex()
        {
            return m_ReqIndex;
        }
        public override UInt16 GetResIndex()
        {
            return m_ResIndex;
        }

        //
        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            base._Deserialize(stream);
            stream.Read(ref m_Buf);
            stream.Read(ref m_Result);
            // TODO, 暂时注释消息的序列号
            stream.Read(ref m_ReqIndex);
            stream.Read(ref m_ResIndex);
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            base._Serialize(stream);
            stream.Write(m_Buf);
            stream.Write(m_Result);
            // TODO, 暂时注释消息的序列号
            stream.Write(m_ReqIndex);
            stream.Write(m_ResIndex);
            return true;
        }

        private UInt16 m_ReqIndex = 0;
        private UInt16 m_ResIndex = 0;
    }

    /// <summary>
    /// 处理 Byte[] 的序列化和反序列化
    /// </summary>
    [Serializable]
    public class CEchoMessage : CSLib.Framework.CEchoMessage
    {
        public CEchoMessage()
            : base(0, 0)
        {
        }

        public CEchoMessage(UInt16 type, UInt16 id)
            : base(type, id)
        {
        }

        public CEchoMessage(byte server, byte func, UInt16 id)
            : base(CSLib.Utility.CBitHelper.MergeUInt8(server, func), id)
        {
        }

        //
        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            base._Deserialize(stream);
            stream.Read(ref m_Buf);
            stream.Read(ref m_Result);
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            base._Serialize(stream);
            stream.Write(m_Buf);
            stream.Write(m_Result);
            return true;
        }
    }
}