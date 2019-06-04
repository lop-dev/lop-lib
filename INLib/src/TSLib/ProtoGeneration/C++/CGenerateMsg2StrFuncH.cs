using System;
using System.Collections.Generic;
using System.IO;
using TSLib.ProtoGeneration;

namespace TSLib.ProtoGeneration
{
    class CGenerateMsg2StrFuncH
    {
        private string m_writePath_h;

        private FileStream m_file;
        private StreamWriter m_writer;

        public CGenerateMsg2StrFuncH(string writeRoot)
        {
            m_writePath_h = writeRoot + "/message2StrFunc.h";
        }

        public void StartWriter()
        {
            m_file = new FileStream(m_writePath_h, FileMode.OpenOrCreate);
            m_writer = new StreamWriter(m_file);

            WriteHead();

            m_writer.Close();
        }

        private void WriteHead()
        {
            m_writer.Write(@"#ifndef __MSLIB_MESSAGE2STRFUNC_H__
#define __MSLIB_MESSAGE2STRFUNC_H__
#include <SFLib/message/message.h>
#include <SFLib/commonDefine/baseDef.h>
#include <google/protobuf/message.h>
#include <BCLib/utility/logFile.h>
#include <../PTLib/inc/PTLib/protobuf/message.pb.h>
namespace MSLib
{

// 功能消息号帮助
typedef std::string(*PFUN_MSG2STR)(BCLib::int32 msgID);
bool registerMsg2StrFunc(BCLib::uint16 funcID, PFUN_MSG2STR pfun);

extern void registerAllMsg2StrFunc();

class CFuncMsgIDNameHelper : public SFLib::CFuncMsgIDNameHelper
{
	BCLIB_SINGLETON_DECLARE(CFuncMsgIDNameHelper);
public:
	virtual std::string getFuncMsgIDName(BCLib::uint16 funcID, BCLib::int32 msgID);
};
static CFuncMsgIDNameHelper* m_funcMsgIDNameHelper = &CFuncMsgIDNameHelper::singleton();
}//MSLib
#endif//__MSLIB_MESSAGE2STRFUNC_H__");
        }
    }
}
