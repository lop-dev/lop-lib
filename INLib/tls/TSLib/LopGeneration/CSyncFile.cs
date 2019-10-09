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
            Console.WriteLine("拷贝文件之：C++");

            //拷贝到PBLib目录
            string SERVER_PTLIB_PROTOFILE_SRC = @".\TableOut\Temp\1_Protoext\";
            string SERVER_PTLIB_PROTOFILE_DES = @".\TableOut\C++\PBLib\inc\PBLib\protofile\";
            CopyDirectory(SERVER_PTLIB_PROTOFILE_SRC, SERVER_PTLIB_PROTOFILE_DES, "*.proto");

            string SERVER_PTLIB_PROTOBUF_SRC = @".\TableOut\Temp\2_Protobuf\C++\";
            string SERVER_PTLIB_PROTOBUF_DES_H = @".\TableOut\C++\PBLib\inc\PBLib\protobuf\";
            string SERVER_PTLIB_PROTOBUF_DES_CC = @".\TableOut\C++\PBLib\src\PBLib\protobuf\";
            CopyDirectory(SERVER_PTLIB_PROTOBUF_SRC, SERVER_PTLIB_PROTOBUF_DES_H, "*.pb.h");
            CopyDirectory(SERVER_PTLIB_PROTOBUF_SRC, SERVER_PTLIB_PROTOBUF_DES_CC, "*.pb.cc");

            //拷贝到PELib目录
            string SERVER_PTLIB_PROTOEXT_SRC = @".\TableOut\Temp\1_Protoext\C++\";
            string SERVER_PTLIB_PROTOEXT_DES_H = @".\TableOut\C++\PELib\inc\PELib\protoext\";
            string SERVER_PTLIB_PROTOEXT_DES_CC = @".\TableOut\C++\PELib\src\PELib\protoext\";
            CopyDirectory(SERVER_PTLIB_PROTOEXT_SRC, SERVER_PTLIB_PROTOEXT_DES_H, "*.pe.h");
            CopyDirectory(SERVER_PTLIB_PROTOEXT_SRC, SERVER_PTLIB_PROTOEXT_DES_CC, "*.pe.cc");

            //拷贝到LTLib目录
            string SERVER_LTLIB_PROTOEXT_SRC = @".\TableOut\Temp\1_Protoext\C++\";
            string SERVER_LTLIB_PROTOEXT_DES_H = @".\TableOut\C++\LTLib\inc\LTLib\protoext\";
            string SERVER_LTLIB_PROTOEXT_DES_CC = @".\TableOut\C++\LTLib\src\LTLib\protoext\";
            CopyDirectory(SERVER_LTLIB_PROTOEXT_SRC, SERVER_LTLIB_PROTOEXT_DES_H, "*.lt.h");
            CopyDirectory(SERVER_LTLIB_PROTOEXT_SRC, SERVER_LTLIB_PROTOEXT_DES_CC, "*.lt.cc");

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
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "msg2StrFunc.h",false);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "msg2StrFunc.cc", false);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "err2StrFunc.h", false);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "err2StrFunc.cc", false);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "sendMsgToDB.h", false);
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "sendMsgToDB.cc", false);

            if (Program.EXPORT_ALL)
            {
                List<string> fileterFiles = new List<string>();

                Console.WriteLine("拷贝文件之：C#");
                string CLIENT_PB_CS_SRC = @".\TableOut\Temp\2_Protobuf\C#\";
                string CLIENT_PB_CS_DES = @".\TableOut\C#\Generate\Protobuf\";

                //fileterFiles.Clear();
                //fileterFiles.Add("aITable.pb.cs");
                //fileterFiles.Add("petTable.pb.cs");
                //fileterFiles.Add("roleTable.pb.cs");
                //fileterFiles.Add("petSkinTable.pb.cs");
                //fileterFiles.Add("sceneTable.pb.cs");
                //fileterFiles.Add("storyChapterTable.pb.cs");
                //fileterFiles.Add("skillTable.pb.cs");
                //fileterFiles.Add("skillBuffTable.pb.cs");
                //fileterFiles.Add("skillStateTable.pb.cs");
                //fileterFiles.Add("skillSummonTable.pb.cs");
                //fileterFiles.Add("sensitivewordTable.pb.cs");
                //fileterFiles.Add("globalTable.pb.cs");
                //fileterFiles.Add("monsterGroupTable.pb.cs");
                //fileterFiles.Add("monsterGroupBattleTable.pb.cs");
                //fileterFiles.Add("npcTable.pb.cs");
                //fileterFiles.Add("stringTable.pb.cs");
                //fileterFiles.Add("petAttacherTable.pb.cs");
                //fileterFiles.Add("particleTable.pb.cs");
                //fileterFiles.Add("petPropertyTable.pb.cs");
                //fileterFiles.Add("chapterLevelTable.pb.cs");
                //fileterFiles.Add("battleFormationTable.pb.cs");
                //fileterFiles.Add("houseEventTable.pb.cs");
                //fileterFiles.Add("petAwakeUpTable.pb.cs");
                //fileterFiles.Add("chapterTable.pb.cs");
                //fileterFiles.Add("inappPaymentTable.pb.cs");
                //fileterFiles.Add("objectTable.pb.cs");
                //fileterFiles.Add("dialogTable.pb.cs");
                //fileterFiles.Add("mythicalAnimalTable.pb.cs");
                //fileterFiles.Add("chapterTaskTable.pb.cs");
                //foreach (string file in Directory.GetFiles(CLIENT_PB_CS_SRC, "*.cs", SearchOption.TopDirectoryOnly))
                //{
                //    FileInfo info = new FileInfo(file);
                //    if(!info.Name.Contains("Table.pb.cs"))
                //    {
                //        fileterFiles.Add(info.Name);
                //    }
                //}

                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PB_CS_SRC, CLIENT_PB_CS_DES, "*.cs", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PB_CS_SRC, CLIENT_PB_CS_DES, "*.cs");
                }

                //
                string CLIENT_PE_CS_SRC = @".\TableOut\Temp\1_Protoext\C#\";
                string CLIENT_PE_CS_DES = @".\TableOut\C#\Generate\Protoext\";

                //fileterFiles.Clear();
                //fileterFiles.Add("aITable.pe.cs");
                //fileterFiles.Add("petTable.pe.cs");
                //fileterFiles.Add("roleTable.pe.cs");
                //fileterFiles.Add("petSkinTable.pe.cs");
                //fileterFiles.Add("sceneTable.pe.cs");
                //fileterFiles.Add("storyChapterTable.pe.cs");
                //fileterFiles.Add("skillTable.pe.cs");
                //fileterFiles.Add("skillBuffTable.pe.cs");
                //fileterFiles.Add("skillStateTable.pe.cs");
                //fileterFiles.Add("skillSummonTable.pe.cs");
                //fileterFiles.Add("sensitivewordTable.pe.cs");
                //fileterFiles.Add("globalTable.pe.cs");
                //fileterFiles.Add("monsterGroupTable.pe.cs");
                //fileterFiles.Add("monsterGroupBattleTable.pe.cs");
                //fileterFiles.Add("npcTable.pe.cs");
                //fileterFiles.Add("stringTable.pe.cs");
                //fileterFiles.Add("petAttacherTable.pe.cs");
                //fileterFiles.Add("particleTable.pe.cs");
                //fileterFiles.Add("petPropertyTable.pe.cs");
                //fileterFiles.Add("chapterLevelTable.pe.cs");
                //fileterFiles.Add("battleFormationTable.pe.cs");
                //fileterFiles.Add("houseEventTable.pe.cs");
                //fileterFiles.Add("petAwakeUpTable.pe.cs");
                //fileterFiles.Add("chapterTable.pe.cs");
                //fileterFiles.Add("inappPaymentTable.pe.cs");
                //fileterFiles.Add("objectTable.pe.cs");
                //fileterFiles.Add("dialogTable.pe.cs");
                //fileterFiles.Add("mythicalAnimalTable.pe.cs");
                //fileterFiles.Add("chapterTaskTable.pe.cs");
                //foreach (string file in Directory.GetFiles(CLIENT_PE_CS_SRC, "*.cs", SearchOption.TopDirectoryOnly))
                //{
                //    FileInfo info = new FileInfo(file);
                //    if (!info.Name.Contains("Table.pe.cs"))
                //    {
                //        fileterFiles.Add(info.Name);
                //    }
                //}

                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PE_CS_SRC, CLIENT_PE_CS_DES, "*.cs", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PE_CS_SRC, CLIENT_PE_CS_DES, "*.cs");
                }
            }

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

            if (Program.EXPORT_ALL)
            {
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
                    srcFileList.Add(desFile + ".meta");
                }

                foreach (string file in Directory.GetFiles(strSrc, searchOption, SearchOption.TopDirectoryOnly))
                {
                    FileInfo info = new FileInfo(file);
                    string desFile = Path.Combine(dirDes.FullName, info.Name).Replace('\\', '/');
                    File.Copy(file, desFile, true);
                }
            }
            else
            {
                foreach (string file in Directory.GetFiles(strSrc, searchOption, SearchOption.TopDirectoryOnly))
                {
                    FileInfo info = new FileInfo(file);
                    string desFile = Path.Combine(dirDes.FullName, info.Name).Replace('\\', '/');
                    srcFileList.Add(desFile);
                    srcFileList.Add(desFile + ".meta");

                    File.Copy(file, desFile, true);
                }
            }

            if (delOtherFiles)
            {
                foreach (string file in Directory.GetFiles(dirDes.FullName, searchOption, SearchOption.TopDirectoryOnly))
                {
                    if (!srcFileList.Contains(file.Replace('\\', '/'))) File.Delete(file);
                }

                foreach (string file in Directory.GetFiles(dirDes.FullName, searchOption + ".meta", SearchOption.TopDirectoryOnly))
                {
                    if (!srcFileList.Contains(file.Replace('\\', '/'))) File.Delete(file);
                }
            }
            
            return 0;
        }
    }
}
