using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Message
{
    public enum EMsgType
    {
        EMT_SAMPLE = CSLib.Framework.EMsgType.EMT_EXTEND,
    };

    public enum EMsgID
    {
        EMID_SAMPLE_NTF_OK,
        EMID_SAMPLE_REQ_SAY ,
        EMID_SAMPLE_RES_SAY,
    }

    [Serializable]
    public class CMsgSampleNtfOk : CSLib.Framework.CMessage
    {
        public CMsgSampleNtfOk() : base((ushort)EMsgType.EMT_SAMPLE, (ushort)EMsgID.EMID_SAMPLE_NTF_OK) { }

        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            return true;
        }
    }

    [Serializable]
    public class CMsgSampleReqSay : CSLib.Framework.CMessage
    {
        public CMsgSampleReqSay() : base((ushort)EMsgType.EMT_SAMPLE, (ushort)EMsgID.EMID_SAMPLE_REQ_SAY) { }
        //可传送所有CStream中所列类型，需注意传送byte[]类型前4字节为数组长度，传送DateTime，decimal类型时C++没有相应的类型接受，必须在C++端自行处理
        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            stream.Read(ref m_bool);
            stream.Read(ref m_byte);
            stream.Read(ref m_bytes);
            stream.Read(ref m_double);
            stream.Read(ref m_float);
            stream.Read(ref m_int);
            stream.Read(ref m_long);
            stream.Read(ref m_short);
            stream.Read(ref m_string);
            stream.Read(ref m_uint);
            stream.Read(ref m_ulong);
            stream.Read(ref m_ushort);
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            stream.Write(m_bool);
            stream.Write(m_byte);
            stream.Write(m_bytes);
            stream.Write(m_double);
            stream.Write(m_float);
            stream.Write(m_int);
            stream.Write(m_long);
            stream.Write(m_short);
            stream.Write(m_string);
            stream.Write(m_uint);
            stream.Write(m_ulong);
            stream.Write(m_ushort);
            return true;
        }

        public bool     m_bool;
        public byte     m_byte;
        public byte[]   m_bytes;
        public double   m_double;
        public float    m_float;
        public int      m_int;
        public long     m_long;
        public short    m_short;
        public string   m_string;
        public uint     m_uint;
        public ulong    m_ulong;
        public ushort   m_ushort;
    }

    [Serializable]
    public class CMsgSampleResSay : CSLib.Framework.CMessage
    {
        public CMsgSampleResSay() : base((ushort)EMsgType.EMT_SAMPLE, (ushort)EMsgID.EMID_SAMPLE_RES_SAY) { }

        protected override bool _Deserialize(CSLib.Utility.CStream stream)
        {
            stream.Read(ref m_bool);
            stream.Read(ref m_byte);
            stream.Read(ref m_bytes);
            stream.Read(ref m_double);
            stream.Read(ref m_float);
            stream.Read(ref m_int);
            stream.Read(ref m_long);
            stream.Read(ref m_short);
            stream.Read(ref m_string);
            stream.Read(ref m_uint);
            stream.Read(ref m_ulong);
            stream.Read(ref m_ushort);
            return true;
        }

        protected override bool _Serialize(CSLib.Utility.CStream stream)
        {
            stream.Write(m_bool);
            stream.Write(m_byte);
            stream.Write(m_bytes);
            stream.Write(m_double);
            stream.Write(m_float);
            stream.Write(m_int);
            stream.Write(m_long);
            stream.Write(m_short);
            stream.Write(m_string);
            stream.Write(m_uint);
            stream.Write(m_ulong);
            stream.Write(m_ushort);
            return true;
        }

        public bool m_bool;
        public byte m_byte;
        public byte[] m_bytes;
        public double m_double;
        public float m_float;
        public int m_int;
        public long m_long;
        public short m_short;
        public string m_string;
        public uint m_uint;
        public ulong m_ulong;
        public ushort m_ushort;
    }
}
