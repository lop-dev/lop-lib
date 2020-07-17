using System;
using System.Collections.Generic;
using System.IO;

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
            CConfigFile.Instance.LoadFile();

            Console.WriteLine("********** 拷贝 *.xlsx 描述文件 **********");
            DirectoryInfo desDir01 = new DirectoryInfo(@".\TableOut\Temp\0_Excelfile\DescTable\");
            if (!desDir01.Exists) desDir01.Create();

            foreach (FileInfo fileInfo in desDir01.GetFiles("*.xlsx", SearchOption.TopDirectoryOnly))
            {
                fileInfo.Delete();
            }

            foreach (string strDir in CConfigFile.Instance.DescPath)
            {
                Console.WriteLine(strDir);
                DirectoryInfo srcDir = new DirectoryInfo(strDir);
                _CopyExcelFile(srcDir, desDir01);
            }

            //
            Console.WriteLine("********** 拷贝 *.xlsx 数据文件 **********");
            DirectoryInfo desDir02 = new DirectoryInfo(@".\TableOut\Temp\0_Excelfile\DataTable\");
            if (!desDir02.Exists) desDir02.Create();

            foreach (FileInfo fileInfo in desDir02.GetFiles("*.xlsx", SearchOption.TopDirectoryOnly))
            {
                fileInfo.Delete();
            }

            foreach (string strDir in CConfigFile.Instance.DataPath)
            {
                Console.WriteLine(strDir);
                DirectoryInfo srcDir = new DirectoryInfo(strDir);
                _CopyExcelFile(srcDir, desDir02);
            }

            //
            Console.WriteLine("********** 拷贝 *.proto 文件 **********");
            DirectoryInfo desDir1 = new DirectoryInfo(@".\TableOut\Temp\1_Protoext\");
            if (!desDir1.Exists) desDir1.Create();

            foreach (FileInfo fileInfo in desDir1.GetFiles("*.proto", SearchOption.TopDirectoryOnly))
            {
                fileInfo.Delete();
            }

            foreach (string strDir in CConfigFile.Instance.ProtoPath)
            {
                DirectoryInfo srcDir = new DirectoryInfo(strDir);
                _CopyProtoFile(srcDir, desDir1);
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

                Console.WriteLine("********** 生成 *.proto 和 *.pe 文件 **********");

                DirectoryInfo tmpDirectoryInfo = new DirectoryInfo(@".\TableOut\Temp\0_Excelfile\DescTable\");
                if (tmpDirectoryInfo.Exists)
                {
                    argsStr = @"-GenType=Proto -DescPath=.\TableOut\Temp\0_Excelfile\DescTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }

                //
                foreach (string strFile in CConfigFile.Instance.ProtoXlsx)
                {
                    Console.WriteLine(strFile);
                    string[] arrString = strFile.Split(',');
                    if (arrString.Length != 4) continue;

                    arrString[0] = arrString[0].Trim();
                    arrString[1] = arrString[1].Trim();
                    arrString[2] = arrString[2].Trim();
                    arrString[3] = arrString[3].Trim();

                    FileInfo tmpFileInfo = new FileInfo(arrString[0]);
                    if (!tmpFileInfo.Exists) continue;

                    string strProto = tmpFileInfo.Name.Replace("Table.xlsx", "Enum.proto").Trim();
                    strProto = strProto[0].ToString().ToLower() + strProto.Substring(1);

                    // argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\GlobalTable.xlsx -OutputFile=.\TableOut\Temp\1_Protoext\globalEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Description";
                    argsStr = @"-GenType=Enum -DataFile=" + arrString[0] + " -OutputFile=.\\TableOut\\Temp\\1_Protoext\\" + strProto + " -EnumName=" + arrString[1] + " -EnumValue=" + arrString[2] + " -EnumDesc=" + arrString[3];
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

                argsStr = @"-GenType=Binary -DataPath=..\..\TableOut\Temp\0_Excelfile\DataTable\ -DescPath=..\..\TableOut\Temp\0_Excelfile\DescTable\ -PBSrcPath=..\..\TableOut\Temp\2_Protobuf\C#\ -OutputPath=..\..\TableOut\Temp\3_Protobin\";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));

                Environment.CurrentDirectory = dirRoot.FullName;
                CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;
            }

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP || ExportType == EExportType.CSP || ExportType == EExportType.LUA)
            {
                Console.WriteLine("********** 生成 Msg 文件 **********");

                DirectoryInfo tmpDir = new DirectoryInfo(@".\TableOut\Temp\1_Protoext\");
                if (tmpDir.Exists)
                {
                    argsStr = @"-GenType=Msg -DataPath=.\TableOut\Temp\1_Protoext\ -OutputPath=.\TableOut\Temp\4_Protomsg\ -Language=cpp_cs_lua";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }
            }
            else if (ExportType == EExportType.CPP)
            {
                Console.WriteLine("********** 生成 Msg 文件 **********");

                DirectoryInfo tmpDir = new DirectoryInfo(@".\TableOut\Temp\1_Protoext\");
                if (tmpDir.Exists)
                {
                    argsStr = @"-GenType=Msg -DataPath=.\TableOut\Temp\1_Protoext\ -OutputPath=.\TableOut\Temp\4_Protomsg\ -Language=cpp";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }
            }

            //
            if (ExportType == EExportType.ALL || ExportType == EExportType.PRO || ExportType == EExportType.CPP)
            {
                Console.WriteLine("********** 生成 Task 文件 **********");

                DirectoryInfo tmpDir = new DirectoryInfo(@".\TableOut\Temp\1_Protoext\");
                if (tmpDir.Exists)
                {
                    argsStr = @"-GenType=Task -DataPath=.\TableOut\Temp\1_Protoext\ -OutputPath=.\TableOut\Temp\5_Prototask\ -Language=cpp";
                    Console.WriteLine(argsStr);
                    Execute(argsStr.Split(' '));
                }
            }

            //
            CFileList.Instance.ClearUnuseFileFromFileterFiles();

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

                _RefreshVSProject(dirRoot.FullName + "/TableOut/C++/PBLib/RefPBLibPrj.exe");
                _RefreshVSProject(dirRoot.FullName + "/TableOut/C++/PELib/RefPELibPrj.exe");
                _RefreshVSProject(dirRoot.FullName + "/TableOut/C++/LTLib/RefLTLibPrj.exe");
                _RefreshVSProject(dirRoot.FullName + "/TableOut/C++/MSLib/RefMSLibPrj.exe");
                _RefreshVSProject(dirRoot.FullName + "/TableOut/C++/DTLib/RefDTLibPrj.exe");
            }

            Console.WriteLine("按任意键继续...");
            Console.ReadKey();
            return 0;
        }

        private static void _CopyExcelFile(DirectoryInfo srcDir, DirectoryInfo desDir)
        {
            if (srcDir.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.xlsx to {1}", srcDir.FullName, desDir.FullName));
                foreach (var fileInfo in srcDir.GetFiles("*.xlsx", SearchOption.AllDirectories))
                {
                    string newfile = desDir.FullName.Replace('\\', '/') + fileInfo.Name;
                    File.Copy(fileInfo.FullName, newfile, true);
                }
            }
        }

        private static void _CopyProtoFile(DirectoryInfo srcDir, DirectoryInfo desDir)
        {
            if (srcDir.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir.FullName, desDir.FullName));
                foreach (var fileInfo in srcDir.GetFiles("*.proto", SearchOption.AllDirectories))
                {
                    string newfile = desDir.FullName.Replace('\\', '/') + fileInfo.Name;
                    File.Copy(fileInfo.FullName, newfile, true);
                }
            }
        }

        private static void _RefreshVSProject(string exefile)
        {
            FileInfo tmpFileInfo = new FileInfo(exefile);
            if (!tmpFileInfo.Exists)
            {
                return;
            }

            System.Diagnostics.ProcessStartInfo tmpProcessStartInfo = new System.Diagnostics.ProcessStartInfo();
            tmpProcessStartInfo.FileName = exefile;
            tmpProcessStartInfo.UseShellExecute = false;
            tmpProcessStartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            Environment.CurrentDirectory = tmpFileInfo.DirectoryName;

            var tmpProcess = System.Diagnostics.Process.Start(tmpProcessStartInfo);
            tmpProcess.WaitForExit();
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
