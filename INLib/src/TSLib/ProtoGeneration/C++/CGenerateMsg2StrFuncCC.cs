using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using TSLib.ProtoGeneration;

namespace TSLib.ProtoGeneration
{
    class CGenerateMsg2StrFuncCC
    {
        private List<string> fileList = new List<string>();
        private string m_writePath_cc;

        private FileStream m_file;
        private StreamWriter m_writer;

        private List<string> m_PTBufList = new List<string>();
        public CGenerateMsg2StrFuncCC(string writeRoot, FileInfo[] fileInfos)
        {
            fileList.Clear();
            m_writePath_cc = writeRoot + "/message2StrFunc.cc";
            foreach (var fileInfo in fileInfos)
            {
                string name = fileInfo.Name.Split('.')[0];
                if (name.ToLower().EndsWith("system"))
                {
                    fileList.Add(name);
                }
            }
        }

        public void StartWriter()
        {
            m_file = new FileStream(m_writePath_cc, FileMode.OpenOrCreate);
            m_writer = new StreamWriter(m_file);

            WriteInclude();
            WriteMain();

            m_writer.Close();
        }

        private void WriteInclude()
        {
            m_writer.WriteLine("#include <MSLib/commonDefine/message2StrFunc.h>");
            foreach (var name in fileList)
            {
                m_writer.WriteLine(string.Format("#include <MSLib/message/{0}Msg.h>", name));
            }
        }

        private void WriteMain()
        {
            m_writer.Write(@"
namespace MSLib
{

PFUN_MSG2STR g_msg2StrFuncs[PTBuf::EFUNC_MAX] = { 0 };

bool registerMsg2StrFunc(BCLib::uint16 funcID, PFUN_MSG2STR pfun)
{
	if (PTBuf::EFUNC_MAX <= funcID)
	{
		return false;
	}

	g_msg2StrFuncs[funcID] = pfun;
	return true;
}

//
void registerAllMsg2StrFunc()
{
");
            foreach (var name in fileList)
            {
                m_writer.WriteLine(string.Format("\tregisterMsg2StrFunc(PTBuf::EFUNC_{0}, {1}Msg2Str);", name.ToUpper(), name));
            }

            m_writer.Write(@"
}

//
BCLIB_SINGLETON_DEFINE(CFuncMsgIDNameHelper);

std::string CFuncMsgIDNameHelper::getFuncMsgIDName(BCLib::uint16 funcID, BCLib::int32 msgID)
{
	if (funcID >= PTBuf::EFUNC_MAX)
	{
		return SFLib::CFuncMsgIDNameHelper::getFuncMsgIDName(funcID, msgID);
	}

	if (g_msg2StrFuncs[funcID] == NULL)
	{
		return SFLib::CFuncMsgIDNameHelper::getFuncMsgIDName(funcID, msgID);
	}

	std::string strName = g_msg2StrFuncs[funcID](msgID);
	if (strName == """")
	{
		return SFLib::CFuncMsgIDNameHelper::getFuncMsgIDName(funcID, msgID);
	}

	return strName;
}
}//MSLib");
        }
    }
}
