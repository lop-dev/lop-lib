using System.Text;

namespace TSLib.ProtoGeneration
{
    public enum MsgType
    {
        None = 0,
        Send,
        Recv,
        Service
    }
    
    public static class CConst
    {
        public static readonly string CLIENT_SEND_HEAD = "EMID_GC2CT_";
        public static readonly string CLIENT_RECV_HEAD = "EMID_CT2GC_";
        public static readonly string SERVICE_HEAD = "EMID";
        public static readonly string MSG_2_STR = "_CASE_MSG_2_STR_";
    }
    
    public static class CHelper
    {
        public static string GerateSendStrOfMsg(string msgTypeStr, bool isSend, string message_type_name, string file_name, string enum_name)
        {
            string transferClassName = isSend ? "MsgGC2CT" : "MsgCT2GC";
            transferClassName += msgTypeStr;
            StringBuilder sb = new StringBuilder();
            sb.AppendLine();
            sb.AppendLine();
            sb.AppendFormat("{0} = class(\"{0}\", NetMsgPTBuf)", transferClassName);
            sb.AppendLine();
            sb.AppendFormat("function {0}:ctor()", transferClassName);
            string serverType = isSend ? "ESERVER_CENTER" : "ESERVER_GAMECLIENT";
            sb.AppendLine();
            sb.AppendFormat("    self._buffMsg = CBufMessage.New(NetworkConst.EServerType.{3}, message_pb.EFUNC_{0}, {1}_pb.{2})",
                message_type_name,
                file_name, enum_name, serverType);
            sb.AppendLine();
            sb.Append("end");
            return sb.ToString();
        }
        
        public static string ConvertUnderlineStrToCamel(string str)
        {
            string result = string.Empty;
            string[] splitsStrings = str.Split('_');
            foreach (var splitsString in splitsStrings)
            {
                result += ToCaptilize(splitsString);
            }
            return result;
        }
        
        public static string ToCaptilize(string str)
        {
            str = str.Trim();
            char ch = str[0];
            if(ch >= 97 && ch <= 122)
            {
                ch = (char)(ch - 32);
                char[] charArray = str.ToCharArray();
                charArray[0] = ch;
                return new string(charArray);
            }
            return str;
        }

        public static string CombineServiceName(string str)
        {
            return ToCaptilize(str).Replace("System", "Service");
        }

        public static string CombineSystemName(string str)
        {
            return ToCaptilize(str);
        }

        public static MsgType FindMsgType(string str)
        {
            if (string.IsNullOrEmpty(str))
                return MsgType.None;
            //过滤掉注释行
            if (str.TrimStart().StartsWith("/"))
                return MsgType.None;
            if (str.Trim().Contains(CConst.CLIENT_SEND_HEAD))
                return MsgType.Send;
            if (str.Trim().Contains(CConst.CLIENT_RECV_HEAD))
                return MsgType.Recv;
            if (str.Trim().Contains(CConst.SERVICE_HEAD))
                return MsgType.Service;
            return MsgType.None;
        }
        public static string ChangeEnum(string enumName,string head)
        {
            string newEnumName = string.Empty;
            string[] strArray = enumName.Split('_');
            strArray[0] = head;
            for (int i = 2; i < strArray.Length; i++)
            {
                strArray[i] = ToHump(strArray[i]);
            }
            for (int i = 0; i < strArray.Length; i++)
            {
                newEnumName += strArray[i];
            }
            return newEnumName;
        }

        public static string ToHump(string str)
        {
            string lower = str.ToLower();
            return ToCaptilize(lower);

        }

        public static string WriteHead(string ptbuf)
        {
            ptbuf = ptbuf.Replace("List", string.Empty);
            ptbuf = ptbuf.Remove(0, 1);
            ptbuf = ptbuf.Trim();
            char ch = ptbuf[0];
            if (ch >= 65 && ch <= 90)
            {
                ch = (char)(ch + 32);
                char[] charArray = ptbuf.ToCharArray();
                charArray[0] = ch;
                return new string(charArray);
            }
            return ptbuf;
        }

        public static string FindELGCServerType(string type)
        {
            string serverType = string.Empty;
            switch (type)
            {
                case "UNKNOW":
                    serverType = "ELGCSERVER_UNKNOW";
                    break;
                case "XX":
                    serverType = "ELGCSERVER_ANYXX";
                    break;
                case "XS":
                    serverType = "ELGCSERVER_ANYXS";
                    break;
                case "GC":
                    serverType = "ELGCSERVER_GAMECLIENT";
                    break;
                case "MS":
                    serverType = "ELGCSERVER_MASTER";
                    break;
                case "GW":
                    serverType = "ELGCSERVER_GATEWAY";
                    break;
                case "DB":
                    serverType = "ELGCSERVER_DATABASE";
                    break;
                case "LG":
                    serverType = "ELGCSERVER_LOGIN";
                    break;
                case "CT":
                    serverType = "ELGCSERVER_CENTER";
                    break;
                case "LO":
                    serverType = "ELGCSERVER_LOG";
                    break;
                case "GB":
                    serverType = "EEXTSERVER_GLOBAL";
                    break;
                case "CS":
                    serverType = "EEXTSERVER_CROSS";
                    break;
                case "CA":
                    serverType = "EEXTSERVER_CHAT";
                    break;
                case "BL":
                    serverType = "EEXTSERVER_BILL";
                    break;
                case "MT":
                    serverType = "EEXTSERVER_MATCH";
                    break;
                case "BT":
                    serverType = "EEXTSERVER_BATTLE";
                    break;
                case "GM":
                    serverType = "ELGCSERVER_GM";
                    break;
                case "CHAT":
                    serverType = "EEXTSERVER_CHAT";
                    break;
                default:
                    break;
            }
            return serverType;
        }
    }
}