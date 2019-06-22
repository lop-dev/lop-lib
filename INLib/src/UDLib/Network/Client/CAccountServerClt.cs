namespace UDLib.Network
{
    public class CAccountServerClt : CSLib.Utility.CSingleton<CAccountServerClt>
    {
        public CAccountServerClt()
        {
            m_TcpClient.UseEchoID = true;
        }

        public CTcpClient TcpClient
        {
            get { return m_TcpClient; }
        }

        //
        private CTcpClient m_TcpClient = new CTcpClient();
    }
}
