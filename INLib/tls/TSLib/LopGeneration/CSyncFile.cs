using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Proto2Code
{
    class CSyncFile : CSLib.Utility.CSingleton<CSyncFile>
    {
        //拷贝到工程目录下
        public void Copy2TableOut()
        {
            Console.WriteLine("拷贝文件之：C#");
            string CLIENT_PB_CS_SRC = @".\TableOut\Temp\2_Protobuf\C#\";
            string CLIENT_PB_CS_DES = @".\TableOut\C#\Generate\Protobuf\";
            CopyDirectory(CLIENT_PB_CS_SRC, CLIENT_PB_CS_DES, "*.cs");

            string CLIENT_PE_CS_SRC = @".\TableOut\Temp\1_Protoext\C#\";
            string CLIENT__PE_CS_DES = @".\TableOut\C#\Generate\Protoext\";
            CopyDirectory(CLIENT_PE_CS_SRC, CLIENT__PE_CS_DES, "*.cs");

            Console.WriteLine("拷贝文件之：Lua");
            string CLIENT_PB_LUA_SRC = @".\TableOut\Temp\2_Protobuf\Lua\";
            string CLIENT_PB_LUA_DES = @".\TableOut\Lua\Generate\Protobuf\";
            CopyDirectory(CLIENT_PB_LUA_SRC, CLIENT_PB_LUA_DES, "*.lua");

            string CLIENT_PE_LUA_SRC = @".\TableOut\Temp\1_Protoext\Lua\";
            string CLIENT_PE_LUA_DES = @".\TableOut\Lua\Generate\Protoext\";
            CopyDirectory(CLIENT_PE_LUA_SRC, CLIENT_PE_LUA_DES, "*.lua");

            string CLIENT_MSG_DEFINE_SRC = @".\TableOut\Temp\4_Protomsg\Lua\Message\";
            string CLIENT_MSG_DEFINE_DES = @".\TableOut\Lua\Generate\Message\";
            CopyDirectory(CLIENT_MSG_DEFINE_SRC, CLIENT_MSG_DEFINE_DES, "*.lua");

            string CLIENT_MSG_SERVICES_SRC = @".\TableOut\Temp\4_Protomsg\Lua\Services\";
            string CLIENT_MSG_SERVICES_DES = @".\TableOut\Lua\Generate\Services\";
            CopyDirectory(CLIENT_MSG_SERVICES_SRC, CLIENT_MSG_SERVICES_DES, "*.lua");

            Console.WriteLine("拷贝文件之：C++");
            //拷贝到PTLib目录
            string SERVER_PTLIB_PROTO_SRC = @".\TableOut\Temp\1_Protoext\";
            string SERVER_PTLIB_PROTO_DES = @".\TableOut\C++\PTLib\inc\PTLib\protofile\";
            CopyDirectory(SERVER_PTLIB_PROTO_SRC, SERVER_PTLIB_PROTO_DES, "*.proto");

            string SERVER_PTLIB_PE_TABLE_SRC = @".\TableOut\Temp\1_Protoext\C++\";
            string SERVER_PTLIB_PE_TABLE_DES_H = @".\TableOut\C++\PTLib\inc\PTLib\protoext\";
            string SERVER_PTLIB_PE_TABLE_DES_CC = @".\TableOut\C++\PTLib\src\PTLib\protoext\";
            CopyDirectory(SERVER_PTLIB_PE_TABLE_SRC, SERVER_PTLIB_PE_TABLE_DES_H, "*.pe.h");
            CopyDirectory(SERVER_PTLIB_PE_TABLE_SRC, SERVER_PTLIB_PE_TABLE_DES_CC, "*.pe.cc");

            string SERVER_PTLIB_PB_TABLE_SRC = @".\TableOut\Temp\2_Protobuf\C++\";
            string SERVER_PTLIB_PB_TABLE_DES_H = @".\TableOut\C++\PTLib\inc\PTLib\protobuf\";
            string SERVER_PTLIB_PB_TABLE_DES_CC = @".\TableOut\C++\PTLib\src\PTLib\protobuf\";
            CopyDirectory(SERVER_PTLIB_PB_TABLE_SRC, SERVER_PTLIB_PB_TABLE_DES_H, "*.pb.h");
            CopyDirectory(SERVER_PTLIB_PB_TABLE_SRC, SERVER_PTLIB_PB_TABLE_DES_CC, "*.pb.cc");

            //拷贝到MSLib目录
            List<string> fileter = new List<string>();
            fileter.Add("commandSystemMsg.h");
            fileter.Add("commandSystemMsg.cc");
            fileter.Add("commandSystemMsg.cpp");
            fileter.Add("errorSystemMsg.h");
            fileter.Add("errorSystemMsg.cc");
            fileter.Add("errorSystemMsg.cpp");
            fileter.Add("loginSystemMsg.h");
            fileter.Add("loginSystemMsg.cc");
            fileter.Add("loginSystemMsg.cpp");
            string SERVER_MSLIB_PROFILEGEN_SRC = @".\TableOut\Temp\4_Protomsg\C++\";
            string SERVER_MSLIB_PROFILEGEN_DES_H = @".\TableOut\C++\MSLib\inc\MSLib\message\";
            string SERVER_MSLIB_PROFILEGEN_DES_CPP = @".\TableOut\C++\MSLib\src\MSLib\message\";
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_H, "*Msg.h",true,fileter);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_CPP, "*Msg.cc",true, fileter);
            
            string SERVER_MSLIB_MSGDEFINE_H = @".\TableOut\C++\MSLib\inc\MSLib\commonDefine\";
            string SERVER_MSLIB_MSGDEFINE_CC = @".\TableOut\C++\MSLib\src\MSLib\commonDefine\";
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "message2StrFunc.h",false);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "message2StrFunc.cc",false);

            Console.WriteLine("拷贝文件之：Python");
            string CLIENT_PB_PY_SRC = @".\TableOut\Temp\2_Protobuf\Python\";
            string CLIENT_PB_PY_DES = @".\TableOut\Python\gen\protobuf\";
            CopyDirectory(CLIENT_PB_PY_SRC, CLIENT_PB_PY_DES, "*.py");

            Console.WriteLine("拷贝文件之：Proto");
            string PROTO_SRC = @".\TableOut\Temp\1_Protoext\";
            string PROTO_DES = @".\TableOut\Proto\";
            CopyDirectory(PROTO_SRC, PROTO_DES, "*.proto");

            Console.WriteLine("拷贝文件之：Protobin");
            string PROTOBIN_SRC = @".\TableOut\Temp\3_Protobin\";
            string PROTOBIN_DES = @".\TableOut\Protobin\";
            CopyDirectory(PROTOBIN_SRC, PROTOBIN_DES, "*.txt");

            Console.WriteLine("拷贝文件之：Protobindbg");
            string PROTOBINDBG_SRC = @".\TableOut\Temp\3_Protobin\";
            string PROTOBINDBG_DES = @".\TableOut\Protobindbg\";
            CopyDirectory(PROTOBINDBG_SRC, PROTOBINDBG_DES, "*.dbg");
        }

        public int CopyDirectory(string strSrc, string strDes, string searchOption,bool delOtherFiles = true,List<string> fileterFiles  = null)
        {
            if(!Directory.Exists(strSrc))
            {
                Console.WriteLine(string.Format("源目录不存在，请检查{0}", strSrc));
                return -1;
            }

            if(!Directory.Exists(strDes))
            {
                Directory.CreateDirectory(strDes);
            }

            Console.WriteLine(string.Format("Copy {0}{1} to {2}", strSrc, searchOption, strDes));

            //
            DirectoryInfo dirDes = new DirectoryInfo(strDes);
            List<string> srcFileList = new List<string>();
            if(fileterFiles != null)
            {
                foreach(var f in fileterFiles)
                {
                    string desFile = Path.Combine(dirDes.FullName, f).Replace('\\','/');
                    srcFileList.Add(desFile);
                }
            }

            foreach (string file in Directory.GetFiles(strSrc, searchOption, SearchOption.TopDirectoryOnly))
            {
                FileInfo info = new FileInfo(file);
                string desFile = Path.Combine(dirDes.FullName, info.Name).Replace('\\','/');
                File.Copy(file, desFile, true);

                srcFileList.Add(desFile);
                srcFileList.Add(desFile+".meta");
            }
            if(delOtherFiles)
            {
                foreach (string file in Directory.GetFiles(dirDes.FullName, searchOption, SearchOption.TopDirectoryOnly))
                {
                    if (!srcFileList.Contains(file.Replace('\\', '/')))
                        File.Delete(file);
                }

                foreach (string file in Directory.GetFiles(dirDes.FullName, searchOption + ".meta", SearchOption.TopDirectoryOnly))
                {
                    if (!srcFileList.Contains(file.Replace('\\', '/')))
                        File.Delete(file);
                }
            }
            
            return 0;
        }
    }
}
