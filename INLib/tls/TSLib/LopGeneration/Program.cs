using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;

namespace Proto2Code
{
    class Program
    {
        public enum EExportType
        {
            ALL,
            DES, // 默认就导出数据的二进制
            PRO,
            CPP,
            CSP,
            LUA,
        }
        static public EExportType ExportType = EExportType.ALL;

        static int Main(string[] args)
        {
            // 设置默认的工作路径
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            Environment.CurrentDirectory = fiMainModule.Directory.FullName;
            string strRoot = "../../";
            DirectoryInfo dirRoot = new DirectoryInfo(strRoot);
            Environment.CurrentDirectory = dirRoot.FullName;
            CFileList.Instance.RootDirectory = Environment.CurrentDirectory;
            CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

            //
            if (args.Length == 1)
            {
                if (args[0] == "DES")
                {
                    ExportType = EExportType.DES;
                    CFileList.Instance.m_strFileList = dirRoot.FullName + @".\TableOut\Temp\FileList.des";
                }
                else if (args[0] == "PRO")
                {
                    ExportType = EExportType.PRO;
                    CFileList.Instance.m_strFileList = dirRoot.FullName + @".\FileList.pro";
                }
                else if (args[0] == "CPP")
                {
                    ExportType = EExportType.CPP;
                    CFileList.Instance.m_strFileList = dirRoot.FullName + @".\FileList.cpp";
                }
                else if (args[0] == "CSP")
                {
                    ExportType = EExportType.CSP;
                    CFileList.Instance.m_strFileList = dirRoot.FullName + @".\FileList.csp";
                }
                else if (args[0] == "LUA")
                {
                    ExportType = EExportType.LUA;
                    CFileList.Instance.m_strFileList = dirRoot.FullName + @".\FileList.lua";
                }
            }

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.DES)
            {
                Console.WriteLine("********** 检查数据表 **********");
                string dataTableCheckExe = dirRoot.FullName + "/DesTable/DataTableCheck/DataTableCheck.exe";
                FileInfo fileinfo = new FileInfo(dataTableCheckExe);
                System.Diagnostics.ProcessStartInfo Info = new System.Diagnostics.ProcessStartInfo();
                Info.FileName = dataTableCheckExe;
                Info.UseShellExecute = false;
                Info.WorkingDirectory = fileinfo.Directory.FullName;
                Info.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                var proc = System.Diagnostics.Process.Start(Info);
                proc.WaitForExit();
                if (proc.ExitCode != 0)
                {
                    Console.WriteLine("配置表异常，请运行\"Shared/SrcGen/DesTable/数据表检查.bat\"进行检查。");
                    Console.ReadLine();
                    return 1;
                }
            }

            //
            DirectoryInfo desDir1 = new DirectoryInfo(@".\TableOut\Temp\1_Protoext\");
            if (!desDir1.Exists) desDir1.Create();

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP || ExportType == EExportType.CSP || ExportType == EExportType.LUA)
            {
                Console.WriteLine("********** 拷贝 *.proto 文件 **********");
                DirectoryInfo srcDir = null;
                List<string> protoList = new List<string>(); // 所有的proto文件

                //
                srcDir = new DirectoryInfo(@".\TableGen\10_BasicsSystem\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                srcDir = new DirectoryInfo(@".\TableGen\10_BasicsSystem\SpecialClient\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                srcDir = new DirectoryInfo(@".\TableGen\10_BasicsSystem\SpecialServer\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                srcDir = new DirectoryInfo(@".\TableGen\11_ExtendSystem\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                srcDir = new DirectoryInfo(@".\TableGen\11_ExtendSystem\SpecialClient\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                srcDir = new DirectoryInfo(@".\TableGen\11_ExtendSystem\SpecialServer\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                // 兼容老版文件目录
                srcDir = new DirectoryInfo(@".\TableGen\10_ProtobufDef\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                srcDir = new DirectoryInfo(@".\TableGen\11_ProtobufClt\");
                _CopyProtoFile(srcDir, desDir1, ref protoList);

                //
                DirectoryInfo desTableDir = new DirectoryInfo("./DesTable/DescTable");
                DirectoryInfo proTableDir = new DirectoryInfo("./ProTable");
                foreach (var fileInfo in desTableDir.GetFiles("*.xlsx", SearchOption.AllDirectories))
                {
                    protoList.Add(fileInfo.Name.Replace("xlsx", "proto").ToLower());
                }
                foreach (var fileInfo in proTableDir.GetFiles("*.xlsx", SearchOption.AllDirectories))
                {
                    protoList.Add(fileInfo.Name.Replace("xlsx", "proto").ToLower());
                }

                //
                FileInfo tmpFileInfo = null;

                tmpFileInfo = new FileInfo(@".\DesTable\DataTable\GlobalTable.xlsx");
                if (tmpFileInfo.Exists)
                {
                    protoList.Add("globalenum.proto");
                }

                tmpFileInfo = new FileInfo(@".\DesTable\DataTable\MailMsgTable.xlsx");
                if (tmpFileInfo.Exists)
                {
                    protoList.Add("mailidenum.proto");
                }

                // 清理 desDir1 中多余的 proto 文件
                foreach (FileInfo fileInfo in desDir1.GetFiles("*.proto", SearchOption.TopDirectoryOnly))
                {
                    if (!protoList.Contains(fileInfo.Name.ToLower()))
                    {
                        fileInfo.Delete();
                    }
                }
            }

            // 
            CFileList.Instance.ReadFileList();

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP || ExportType == EExportType.CSP || ExportType == EExportType.LUA)
            {
                string csPeDir = @".\TableOut\Temp\1_Protoext\C#";
                string ccPeDir = @".\TableOut\Temp\1_Protoext\C++";
                string goPeDir = @".\TableOut\Temp\1_Protoext\GO";
                string luaPeDir = @".\TableOut\Temp\1_Protoext\Lua";

                if (!Directory.Exists(csPeDir)) Directory.CreateDirectory(csPeDir);
                if (!Directory.Exists(ccPeDir)) Directory.CreateDirectory(ccPeDir);
                if (!Directory.Exists(goPeDir)) Directory.CreateDirectory(goPeDir);
                if (!Directory.Exists(luaPeDir)) Directory.CreateDirectory(luaPeDir);

                Thread.Sleep(1000); // 不等待一下，有可能报 proto 文件锁住
                Console.WriteLine("********** 生成 *.proto 和 *.pe 文件 **********");

                //
                FileInfo tmpFileInfo = null;

                tmpFileInfo = new FileInfo(@".\DesTable\DataTable\GlobalTable.xlsx");
                if (tmpFileInfo.Exists)
                {
                    argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\GlobalTable.xlsx -OutputFile=.\TableOut\Temp\1_Protoext\globalEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Description";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpFileInfo = new FileInfo(@".\DesTable\DataTable\MailMsgTable.xlsx");
                if (tmpFileInfo.Exists)
                {
                    argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\MailMsgTable.xlsx -OutputFile=.\TableOut\Temp\1_Protoext\mailIDEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Title";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                //
                DirectoryInfo tmpDirectoryInfo = null;

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\AccountServer\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\ProTable\AccountServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\GlobalServer\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\ProTable\GlobalServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\DatabaseServer\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\ProTable\DatabaseServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\LogServer\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\ProTable\LogServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\LogicTable\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\ProTable\LogicTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\BattleTable\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\ProTable\BattleTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                tmpDirectoryInfo = new DirectoryInfo(@".\ProTable\DescTable\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\DesTable\DescTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }
            }

            //
            Environment.CurrentDirectory = desDir1.FullName;
            CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP)
            {
                Console.WriteLine("********** 生成 C++ 的 *.pb 文件 **********");
                argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=C++";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CSP)
            {
                Console.WriteLine("********** 生成 C# 的 *.pb 文件 **********");
                argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=C#";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '), true);
            }

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.LUA)
            {
                Console.WriteLine("********** 生成 Lua 的 *.pb 文件 **********");
                argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=Lua";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO)
            {
                Console.WriteLine("********** 生成 Python 的 *.pb 文件 **********");
                argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=Python";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            Environment.CurrentDirectory = dirRoot.FullName;
            CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.DES)
            {
                Console.WriteLine("********** 生成 *.bin 文件 **********");
                DirectoryInfo exeDir1 = new DirectoryInfo(@".\TableGen\01_LopGeneration\");
                Environment.CurrentDirectory = exeDir1.FullName; // 因为要找 cspProtobuf.dll 文件
                CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

                argsStr = @"-GenType=Binary -DataPath=..\..\DesTable\DataTable\ -DescPath=..\..\DesTable\DescTable\ -PBSrcPath=..\..\TableOut\Temp\2_Protobuf\C#\ -OutputPath=..\..\TableOut\Temp\3_Protobin\";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));

                Environment.CurrentDirectory = dirRoot.FullName;
                CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;
            }

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP || ExportType == EExportType.CSP || ExportType == EExportType.LUA)
            {
                Console.WriteLine("********** 生成 Msg 文件 **********");
                argsStr = @"-GenType=Msg -DataPath=.\TableGen\10_ProtobufDef\ -OutputPath=.\TableOut\Temp\4_Protomsg\ -Language=cpp_cs_lua";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }
            else if (ExportType == EExportType.CPP)
            {
                Console.WriteLine("********** 生成 Msg 文件 **********");
                argsStr = @"-GenType=Msg -DataPath=.\TableGen\10_ProtobufDef\ -OutputPath=.\TableOut\Temp\4_Protomsg\ -Language=cpp";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP)
            {
                Console.WriteLine("********** 生成 Task 文件 **********");
                argsStr = @"-GenType=Task -DataPath=.\TableGen\10_ProtobufDef\ -OutputPath=.\TableOut\Temp\5_Prototask\ -Language=cpp";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            //
            CFileList.Instance.ClearUnuseFile();

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.LUA)
            {
                Console.WriteLine("********** 生成 LuaCfg(Parser) 文件 **********");
                argsStr = @"-GenType=LuaConfig -DataPath=.\DesTable\DescTable\ -OutputPath=.\TableOut\Lua\Generate\Parser\";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            //
            CFileList.Instance.SaveFileList();

            //拷贝到工程目录下
            CSyncFile.Instance.Copy2TableOut();

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.LUA)
            {
                Console.WriteLine("********** 生成 ConfigFiles.lua 文件 **********");
                string path = ".\\TableOut\\Lua\\Generate\\";
                TSLib.ProtoGeneration.CGenerateLuaConfigFiles luaConfig = new TSLib.ProtoGeneration.CGenerateLuaConfigFiles(path);
                luaConfig.TransConfig();
            }

            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP)
            {
                Console.WriteLine("********** 刷新VS工程文件 **********");

                string vsRefresh1 = dirRoot.FullName + "/TableOut/C++/PBLib/RefPBLibPrj.exe";
                System.Diagnostics.ProcessStartInfo ref4 = new System.Diagnostics.ProcessStartInfo();
                ref4.FileName = vsRefresh1;
                ref4.UseShellExecute = false;
                ref4.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/PBLib/";
                var proc4 = System.Diagnostics.Process.Start(ref4);
                proc4.WaitForExit();

                string vsRefresh2 = dirRoot.FullName + "/TableOut/C++/PELib/RefPELibPrj.exe";
                System.Diagnostics.ProcessStartInfo ref5 = new System.Diagnostics.ProcessStartInfo();
                ref5.FileName = vsRefresh2;
                ref5.UseShellExecute = false;
                ref5.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/PELib/";
                var proc5 = System.Diagnostics.Process.Start(ref5);
                proc5.WaitForExit();

                string vsRefresh3 = dirRoot.FullName + "/TableOut/C++/LTLib/RefLTLibPrj.exe";
                System.Diagnostics.ProcessStartInfo ref2 = new System.Diagnostics.ProcessStartInfo();
                ref2.FileName = vsRefresh3;
                ref2.UseShellExecute = false;
                ref2.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/LTLib/";
                var proc2 = System.Diagnostics.Process.Start(ref2);
                proc2.WaitForExit();

                string vsRefresh4 = dirRoot.FullName + "/TableOut/C++/MSLib/RefMSLibPrj.exe";
                System.Diagnostics.ProcessStartInfo ref3 = new System.Diagnostics.ProcessStartInfo();
                ref3.FileName = vsRefresh4;
                ref3.UseShellExecute = false;
                ref3.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/MSLib/";
                var proc3 = System.Diagnostics.Process.Start(ref3);
                proc3.WaitForExit();

                string vsRefresh5 = dirRoot.FullName + "/TableOut/C++/DTLib/RefDTLibPrj.exe";
                System.Diagnostics.ProcessStartInfo ref6 = new System.Diagnostics.ProcessStartInfo();
                ref6.FileName = vsRefresh5;
                ref6.UseShellExecute = false;
                ref6.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/DTLib/";
                var proc6 = System.Diagnostics.Process.Start(ref6);
                proc6.WaitForExit();
            }

            Console.WriteLine("按任意键继续...");
            Console.ReadKey();
            return 0;
        }

        private static void _CopyProtoFile(DirectoryInfo srcDir, DirectoryInfo desDir, ref List<string> protoList)
        {
            if (srcDir.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir.FullName, desDir.FullName));
                foreach (var fileInfo in srcDir.GetFiles("*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = fileInfo.FullName.Replace('\\', '/');
                    protoList.Add(fileInfo.Name.ToLower());
                    newfile = newfile.Replace(srcDir.FullName.Replace('\\', '/'), desDir.FullName.Replace('\\', '/'));
                    File.Copy(fileInfo.FullName, newfile, true);
                }
            }
        }

        private static int Execute(string[] args, bool bSync = false)
        {
            if (args.Length == 1)
            {
                string strTemp = args[0].ToLower();
                if ((strTemp == "-h") || (strTemp == "-help") || (strTemp == "h") || (strTemp == "help"))
                {
                    Console.WriteLine("USAGE:");
                    Console.WriteLine("");
                    Console.WriteLine("把数据文件转成Proto文件:");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DataFile=*  -OutputFile=*  -EnumName=*  -EnumValue=*  -EnumDesc=*");
                    Console.WriteLine("");
                    Console.WriteLine("把描述文件转成Proto文件:");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DescPath=*  -OutputPath=*");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DescPath=*  -OutputPath=*  -Language=[|CC|CS|GO|LUA]");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DescFile=*  -OutputFile=*");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DescFile=*  -OutputFile=*  -Language=[|CC|CS|GO|LUA]");
                    Console.WriteLine("");
                    Console.WriteLine("把数据文件转成代码文件:");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DataPath=*  -OutputPath=*");
                    Console.WriteLine("");
                    Console.WriteLine("把Proto文件转成代码文件:");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -ProtoPath=*  -OutputPath=*");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -ProtoPath=*  -OutputPath=*  -Language=[|CC|CS|GO|LUA]");
                    Console.WriteLine("");
                    Console.WriteLine("把数据文件转成二进制文件:");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DataPath=*  -DescPath=*  -PBSrcPath=*  -OutputPath=*");
                    Console.WriteLine("TableGeneration  -GenType=[Enum|Proto|Pbsrc|Binary|Msg|LuaConfig]  -DataFile=*  -DescFile=*  -PBSrcFile=*  -OutputFile=*");
                }
                return 0;
            }

            //
            if (args.Length < 3)
            {
                Console.WriteLine("参数个数不对，请输入参数 -help 查看详细信息");
                CSLib.Utility.CDebugOut.LogError("参数个数不对，请输入参数 -help 查看详细信息");
                return 1; // 参数个数不对
            }

            //
            if (!CGeneration.Instance.Analysis(args))
            {
                return 1; // 参数解析失败
            }

            return CGeneration.Instance.Generation(bSync);
        }

        private static string argsStr = "";
    }
}
