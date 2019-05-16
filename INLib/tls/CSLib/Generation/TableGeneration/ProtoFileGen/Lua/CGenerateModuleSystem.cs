using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Proto2Code
{
    public class CGenerateModuleSystem
    {
        private CNameUnit m_nameUnit;
        private string m_writePath;
              
        private List<string> service_reigster_callback_list;

        private FileStream m_file;
        private StreamWriter m_write;
        /// <summary>
        /// 输出模块系统消息处理文件. eg:MailSystem.lua
        /// </summary>
        /// <param name="nameUnit"></param>
        /// <param name="writeRoot"></param>
        public CGenerateModuleSystem(CNameUnit nameUnit, string writeRoot)
        {
            m_nameUnit = nameUnit;
            m_writePath = writeRoot + m_nameUnit.ModuleSystem + ".lua";
            service_reigster_callback_list = new List<string>();
        }

        public void StartWrite(List<SMsgID> systemList)
        {
            m_file = new FileStream(m_writePath, FileMode.OpenOrCreate);
            m_write = new StreamWriter(m_file);

            WriteHead();
            WriteContentRecv(systemList);
            FinishWrite();
            service_reigster_callback_list.Clear();
            m_write.Close();
        }

        public void WriteHead()
        {
            m_write.WriteLine("---------------------------");
            m_write.WriteLine("---");
            m_write.Write("---Below starts {0} ---.", m_nameUnit.ModuleSystem);
            m_write.WriteLine("---");
            m_write.WriteLine("local CURRENT_MODULE_NAME = ...");
            m_write.WriteLine("local {0} = class(\"{0}\", require(\"Game.Message.Services.{1}\"))", m_nameUnit.ModuleSystem, m_nameUnit.ModuleService);
            m_write.WriteLine("function {0}:ctor()", m_nameUnit.ModuleSystem);
            m_write.WriteLine("    {0}.super.ctor(self)", m_nameUnit.ModuleSystem);
            m_write.WriteLine("    self._model = import(\".{0}Model\", CURRENT_MODULE_NAME).New()", m_nameUnit.Module);
            m_write.WriteLine("end");
            m_write.WriteLine();
        }
        
        public void WriteContentRecv(List<SMsgID> systemList)
        {
            foreach (SMsgID item in systemList)
            {
                MsgType msgType = CHelper.FindMsgType(item.enumName);
                if (msgType == MsgType.Recv)
                {
                    string msgTypeDetail = item.enumName.Replace(CConst.CLIENT_RECV_HEAD, string.Empty);
                    msgTypeDetail = msgTypeDetail.ToLower();
                    string transferServiceName = "onCT2GC" + CHelper.ConvertUnderlineStrToCamel(msgTypeDetail);
                    service_reigster_callback_list.Add(transferServiceName);
                }
            }           
        }
        
        public void FinishWrite()
        {
            foreach (var callback_name in service_reigster_callback_list)
            {
                m_write.WriteLine("--");
                m_write.WriteLine("function {0}:{1}(reqLabel, data)", m_nameUnit.ModuleSystem, callback_name);
                m_write.WriteLine("    {0}.super.{1}(reqLabel, data)", m_nameUnit.ModuleSystem, callback_name);
                m_write.WriteLine("end");
            }
            m_write.WriteLine();
            m_write.WriteLine("return {0}", m_nameUnit.ModuleSystem);         
        }
    }
}