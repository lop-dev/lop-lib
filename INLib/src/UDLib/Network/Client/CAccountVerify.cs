using System;
using System.Collections.Generic;
using System.Text;

namespace UDLib.Network
{
    public class CAccountVerify : CSLib.Utility.CSingleton<CAccountVerify>
    {
        public CAccountVerify()
        {
        }

        public bool IsValid()
        {
            //return m_accountID != 0 && m_key.Length > 0 && m_data != 0;
            //return m_AccountID != 0 ;
            return true;
        }

        public void Reset()
        {
            m_AccountID = 0;
            m_PlatformName = "";
            m_ChannelName = "";
            m_AccountName = "";
            m_AccountPwd = "";
            m_AccountKey = "";

            m_EntityID = 0;
            m_ReconnectKey = "";
            m_ReconnectMsg = null;
        }

        public UInt64 AccountID
        {
            set { m_AccountID = value; }
            get { return m_AccountID; }
        }

        public string PlatformName
        {
            set { m_PlatformName = value; }
            get { return m_PlatformName; }
        }

        public string ChannelName
        {
            set { m_ChannelName = value; }
            get { return m_ChannelName; }
        }

        public string AccountName
        {
            set { m_AccountName = value; }
            get { return m_AccountName; }
        }

        public string AccountPwd
        {
            set { m_AccountPwd = value; }
            get { return m_AccountPwd; }
        }

        public string AccountKey
        {
            set { m_AccountKey = value; }
            get { return m_AccountKey; }
        }

        //
        public UInt64 EntityID
        {
            set { m_EntityID = value; }
            get { return m_EntityID; }
        }

        public string ReconnectKey
        {
            set { m_ReconnectKey = value; }
            get { return m_ReconnectKey; }
        }

        public CSLib.Framework.CNetMessage ReconnectMsg
        {
            set { m_ReconnectMsg = value; }
            get { return m_ReconnectMsg; }
        }

        //
        private UInt64 m_AccountID = 0;
        private string m_PlatformName = "";
        private string m_ChannelName = "";
        private string m_AccountName = "";
        private string m_AccountPwd = "";
        private string m_AccountKey = "";

        private UInt64 m_EntityID = 0;
        private string m_ReconnectKey = "";
        private CSLib.Framework.CNetMessage m_ReconnectMsg = null;
    }
}

