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
            // 拷贝原始proto文件
            if (Program.ExportType == Program.EExportType.ALL ||
                Program.ExportType == Program.EExportType.PRO ||
                Program.ExportType == Program.EExportType.CPP ||
                Program.ExportType == Program.EExportType.CSP ||
                Program.ExportType == Program.EExportType.LUA)
            {
                Console.WriteLine("拷贝文件之：Proto");
                string PROTO_SRC = @".\TableOut\Temp\1_Protoext\";
                string PROTO_DES = @".\TableOut\Proto\";
                CopyDirectory(PROTO_SRC, PROTO_DES, "*.proto");
            }

            // 拷贝C++源码
            if (Program.ExportType == Program.EExportType.ALL ||
                Program.ExportType == Program.EExportType.PRO ||
                Program.ExportType == Program.EExportType.CPP)
            {
                DirectoryInfo tmpDirectoryInfo = null;

                Console.WriteLine("拷贝文件之：C++");
                List<string> fileterFiles = new List<string>();

                // 拷贝到PBLib目录
                string SERVER_PTLIB_PROTOFILE_SRC = @".\TableOut\Temp\1_Protoext\";
                string SERVER_PTLIB_PROTOFILE_DES = @".\TableOut\C++\PBLib\inc\PBLib\protofile\";
                CopyDirectory(SERVER_PTLIB_PROTOFILE_SRC, SERVER_PTLIB_PROTOFILE_DES, "*.proto");

                string SERVER_PTLIB_PROTOBUF_SRC = @".\TableOut\Temp\2_Protobuf\C++\";
                string SERVER_PTLIB_PROTOBUF_DES_H = @".\TableOut\C++\PBLib\inc\PBLib\protobuf\";
                string SERVER_PTLIB_PROTOBUF_DES_CC = @".\TableOut\C++\PBLib\src\PBLib\protobuf\";
                CopyDirectory(SERVER_PTLIB_PROTOBUF_SRC, SERVER_PTLIB_PROTOBUF_DES_H, "*.pb.h");
                CopyDirectory(SERVER_PTLIB_PROTOBUF_SRC, SERVER_PTLIB_PROTOBUF_DES_CC, "*.pb.cc");

                // 拷贝到PELib目录
                string SERVER_PTLIB_PROTOEXT_SRC = @".\TableOut\Temp\1_Protoext\C++\";
                string SERVER_PTLIB_PROTOEXT_DES_H = @".\TableOut\C++\PELib\inc\PELib\protoext\";
                string SERVER_PTLIB_PROTOEXT_DES_CC = @".\TableOut\C++\PELib\src\PELib\protoext\";
                CopyDirectory(SERVER_PTLIB_PROTOEXT_SRC, SERVER_PTLIB_PROTOEXT_DES_H, "*.pe.h");
                CopyDirectory(SERVER_PTLIB_PROTOEXT_SRC, SERVER_PTLIB_PROTOEXT_DES_CC, "*.pe.cc");

                // 拷贝到LTLib目录
                string SERVER_LTLIB_PROTOEXT_SRC = @".\TableOut\Temp\1_Protoext\C++\";
                string SERVER_LTLIB_PROTOEXT_DES_H = @".\TableOut\C++\LTLib\inc\LTLib\protoext\";
                string SERVER_LTLIB_PROTOEXT_DES_CC = @".\TableOut\C++\LTLib\src\LTLib\protoext\";

                fileterFiles.Clear();
                _GetFileterFilesLTH(ref fileterFiles);
                foreach (string file in Directory.GetFiles(SERVER_LTLIB_PROTOEXT_DES_H, "*.lt.h", SearchOption.TopDirectoryOnly))
                {
                    FileInfo info = new FileInfo(file);
                    fileterFiles.Add(info.Name);
                }

                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(SERVER_LTLIB_PROTOEXT_SRC, SERVER_LTLIB_PROTOEXT_DES_H, "*.lt.h", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(SERVER_LTLIB_PROTOEXT_SRC, SERVER_LTLIB_PROTOEXT_DES_H, "*.lt.h");
                }

                fileterFiles.Clear();
                _GetFileterFilesLTCc(ref fileterFiles);
                foreach (string file in Directory.GetFiles(SERVER_LTLIB_PROTOEXT_DES_H, "*.lt.cc", SearchOption.TopDirectoryOnly))
                {
                    FileInfo info = new FileInfo(file);
                    fileterFiles.Add(info.Name);
                }

                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(SERVER_LTLIB_PROTOEXT_SRC, SERVER_LTLIB_PROTOEXT_DES_CC, "*.lt.cc", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(SERVER_LTLIB_PROTOEXT_SRC, SERVER_LTLIB_PROTOEXT_DES_CC, "*.lt.cc");
                }

                // 拷贝到MSLib目录
                string SERVER_MSLIB_PROFILEGEN_SRC = @".\TableOut\Temp\4_Protomsg\C++\";
                string SERVER_MSLIB_PROFILEGEN_DES_H = @".\TableOut\C++\MSLib\inc\MSLib\message\";
                string SERVER_MSLIB_PROFILEGEN_DES_CPP = @".\TableOut\C++\MSLib\src\MSLib\message\";

                tmpDirectoryInfo = new DirectoryInfo(SERVER_MSLIB_PROFILEGEN_SRC);
                if (tmpDirectoryInfo.Exists)
                {
                    fileterFiles.Clear();
                    fileterFiles.Add("commandSystemMsg.h");
                    fileterFiles.Add("errorSystemMsg.h");
                    fileterFiles.Add("loginSystemMsg.h");
                    foreach (string file in Directory.GetFiles(SERVER_MSLIB_PROFILEGEN_SRC, "*Msg.h", SearchOption.TopDirectoryOnly))
                    {
                        FileInfo info = new FileInfo(file);
                        if (!info.Name.Contains("Table.pb.cs"))
                        {
                            fileterFiles.Add(info.Name);
                        }
                    }

                    if (fileterFiles.Count > 0)
                    {
                        CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_H, "*Msg.h", true, fileterFiles);
                    }
                    else
                    {
                        CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_H, "*Msg.h");
                    }

                    fileterFiles.Clear();
                    fileterFiles.Add("commandSystemMsg.cc");
                    fileterFiles.Add("errorSystemMsg.cc");
                    fileterFiles.Add("loginSystemMsg.cc");
                    foreach (string file in Directory.GetFiles(SERVER_MSLIB_PROFILEGEN_SRC, "*Msg.cc", SearchOption.TopDirectoryOnly))
                    {
                        FileInfo info = new FileInfo(file);
                        if (!info.Name.Contains("Table.pb.cs"))
                        {
                            fileterFiles.Add(info.Name);
                        }
                    }

                    if (fileterFiles.Count > 0)
                    {
                        CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_CPP, "*Msg.cc", true, fileterFiles);
                    }
                    else
                    {
                        CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_CPP, "*Msg.cc");
                    }

                    //
                    string SERVER_MSLIB_MSGDEFINE_H = @".\TableOut\C++\MSLib\inc\MSLib\commonDefine\";
                    string SERVER_MSLIB_MSGDEFINE_CC = @".\TableOut\C++\MSLib\src\MSLib\commonDefine\";
                    CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "msg2StrFunc.h", false);
                    CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "msg2StrFunc.cc", false);
                    CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "err2StrFunc.h", false);
                    CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "err2StrFunc.cc", false);
                    CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_H, "sendMsgToDB.h", false);
                    CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_MSGDEFINE_CC, "sendMsgToDB.cc", false);
                }

                // 拷贝到DTLib目录
                string SERVER_DTLIB_PROFILEGEN_SRC = @".\TableOut\Temp\5_Prototask\C++\";
                string SERVER_DTLIB_PROFILEGEN_DES_H = @".\TableOut\C++\DTLib\inc\DTLib\databaseTask\";
                string SERVER_DTLIB_PROFILEGEN_DES_CPP = @".\TableOut\C++\DTLib\src\DTLib\databaseTask\";

                tmpDirectoryInfo = new DirectoryInfo(SERVER_DTLIB_PROFILEGEN_SRC);
                if (tmpDirectoryInfo.Exists)
                {
                    fileterFiles.Clear();
                    if (fileterFiles.Count > 0)
                    {
                        CopyDirectory(SERVER_DTLIB_PROFILEGEN_SRC, SERVER_DTLIB_PROFILEGEN_DES_H, "*.h", true, fileterFiles);
                    }
                    else
                    {
                        CopyDirectory(SERVER_DTLIB_PROFILEGEN_SRC, SERVER_DTLIB_PROFILEGEN_DES_H, "*.h");
                    }

                    fileterFiles.Clear();
                    if (fileterFiles.Count > 0)
                    {
                        CopyDirectory(SERVER_DTLIB_PROFILEGEN_SRC, SERVER_DTLIB_PROFILEGEN_DES_CPP, "*.cc", true, fileterFiles);
                    }
                    else
                    {
                        CopyDirectory(SERVER_DTLIB_PROFILEGEN_SRC, SERVER_DTLIB_PROFILEGEN_DES_CPP, "*.cc");
                    }
                }
            }

            // 拷贝C#源码
            if (Program.ExportType == Program.EExportType.ALL ||
                Program.ExportType == Program.EExportType.PRO ||
                Program.ExportType == Program.EExportType.CSP)
            {
                Console.WriteLine("拷贝文件之：C#");
                List<string> fileterFiles = new List<string>();

                string CLIENT_PB_CS_SRC = @".\TableOut\Temp\2_Protobuf\C#\";
                string CLIENT_PB_CS_DES = @".\TableOut\C#\Generate\Protobuf\";

                fileterFiles.Clear();
                _GetFileterFilesPBcs(ref fileterFiles);
                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PB_CS_SRC, CLIENT_PB_CS_DES, "*.cs", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PB_CS_SRC, CLIENT_PB_CS_DES, "*.cs");
                }

                string CLIENT_PE_CS_SRC = @".\TableOut\Temp\1_Protoext\C#\";
                string CLIENT_PE_CS_DES = @".\TableOut\C#\Generate\Protoext\";
                fileterFiles.Clear();
                _GetFileterFilesPEcs(ref fileterFiles);
                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PE_CS_SRC, CLIENT_PE_CS_DES, "*.cs", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PE_CS_SRC, CLIENT_PE_CS_DES, "*.cs");
                }
            }

            // 拷贝Lua源码
            if (Program.ExportType == Program.EExportType.ALL ||
                Program.ExportType == Program.EExportType.PRO ||
                Program.ExportType == Program.EExportType.LUA)
            {
                List<string> fileterFiles = new List<string>();

                Console.WriteLine("拷贝文件之：Lua");
                string CLIENT_PB_LUA_SRC = @".\TableOut\Temp\2_Protobuf\Lua\";
                string CLIENT_PB_LUA_DES = @".\TableOut\Lua\Generate\Protobuf\";
                fileterFiles.Clear();
                _GetFileterFilesPBlua(ref fileterFiles);
                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PB_LUA_SRC, CLIENT_PB_LUA_DES, "*.lua", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PB_LUA_SRC, CLIENT_PB_LUA_DES, "*.lua");
                }

                string CLIENT_PB_LUA_SRC_FOR_SERVER = @".\TableOut\Temp\2_Protobuf\Lua\";
                string CLIENT_PB_LUA_DES_FOR_SERVER = @".\TableOut\Lua\Server\Protobuf\";
                fileterFiles.Clear();
                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PB_LUA_SRC_FOR_SERVER, CLIENT_PB_LUA_DES_FOR_SERVER, "*.lua", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PB_LUA_SRC_FOR_SERVER, CLIENT_PB_LUA_DES_FOR_SERVER, "*.lua");
                }

                string CLIENT_PE_LUA_SRC = @".\TableOut\Temp\1_Protoext\Lua\";
                string CLIENT_PE_LUA_DES = @".\TableOut\Lua\Generate\Protoext\";
                fileterFiles.Clear();
                _GetFileterFilesPElua(ref fileterFiles);
                if (fileterFiles.Count > 0)
                {
                    CopyDirectory(CLIENT_PE_LUA_SRC, CLIENT_PE_LUA_DES, "*.lua", true, fileterFiles);
                }
                else
                {
                    CopyDirectory(CLIENT_PE_LUA_SRC, CLIENT_PE_LUA_DES, "*.lua");
                }

                string CLIENT_MSG_DEFINE_SRC = @".\TableOut\Temp\4_Protomsg\Lua\Message\";
                string CLIENT_MSG_DEFINE_DES = @".\TableOut\Lua\Generate\Message\";
                CopyDirectory(CLIENT_MSG_DEFINE_SRC, CLIENT_MSG_DEFINE_DES, "*.lua");

                string CLIENT_MSG_SERVICES_SRC = @".\TableOut\Temp\4_Protomsg\Lua\Services\";
                string CLIENT_MSG_SERVICES_DES = @".\TableOut\Lua\Generate\Services\";
                CopyDirectory(CLIENT_MSG_SERVICES_SRC, CLIENT_MSG_SERVICES_DES, "*.lua");
            }

            // 拷贝Python源码
            if (Program.ExportType == Program.EExportType.ALL ||
                Program.ExportType == Program.EExportType.PRO)
            {
                Console.WriteLine("拷贝文件之：Python");
                List<string> fileterFiles = new List<string>();
                
                string CLIENT_PB_PY_SRC = @".\TableOut\Temp\2_Protobuf\Python\";
                string CLIENT_PB_PY_DES = @".\TableOut\Python\gen\protobuf\";
                CopyDirectory(CLIENT_PB_PY_SRC, CLIENT_PB_PY_DES, "*.py");
            }

            // 拷贝二进制文件
            if (Program.ExportType == Program.EExportType.ALL ||
                Program.ExportType == Program.EExportType.DES)
            {
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

        void _GetFileterFilesPBcs(ref List<string> fileterFiles)
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            string strFileName = fiMainModule.Directory.FullName + "\\fileterFiles.pb.cs";

            FileStream fs = new FileStream(strFileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string strLine = sr.ReadLine();
            while (strLine != null)
            {
                fileterFiles.Add(strLine);
                strLine = sr.ReadLine();
            }
        }

        void _GetFileterFilesPEcs(ref List<string> fileterFiles)
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            string strFileName = fiMainModule.Directory.FullName + "\\fileterFiles.pe.cs";

            FileStream fs = new FileStream(strFileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string strLine = sr.ReadLine();
            while (strLine != null)
            {
                fileterFiles.Add(strLine);
                strLine = sr.ReadLine();
            }
        }

        void _GetFileterFilesPBlua(ref List<string> fileterFiles)
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            string strFileName = fiMainModule.Directory.FullName + "\\fileterFiles.pb.lua";

            FileStream fs = new FileStream(strFileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string strLine = sr.ReadLine();
            while (strLine != null)
            {
                fileterFiles.Add(strLine);
                strLine = sr.ReadLine();
            }
        }

        void _GetFileterFilesPElua(ref List<string> fileterFiles)
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            string strFileName = fiMainModule.Directory.FullName + "\\fileterFiles.pe.lua";

            FileStream fs = new FileStream(strFileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string strLine = sr.ReadLine();
            while (strLine != null)
            {
                fileterFiles.Add(strLine);
                strLine = sr.ReadLine();
            }
        }

        void _GetFileterFilesLTH(ref List<string> fileterFiles)
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            string strFileName = fiMainModule.Directory.FullName + "\\fileterFiles.lt.h";

            FileStream fs = new FileStream(strFileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string strLine = sr.ReadLine();
            while (strLine != null)
            {
                fileterFiles.Add(strLine);
                strLine = sr.ReadLine();
            }
        }

        void _GetFileterFilesLTCc(ref List<string> fileterFiles)
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            string strFileName = fiMainModule.Directory.FullName + "\\fileterFiles.lt.cc";

            FileStream fs = new FileStream(strFileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs);

            string strLine = sr.ReadLine();
            while (strLine != null)
            {
                fileterFiles.Add(strLine);
                strLine = sr.ReadLine();
            }
        }
    }
}
