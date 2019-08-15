using System;
using System.Collections.Generic;
using System.IO;

namespace Proto2Code
{
    class Program
    {
        static public bool EXPORT_ALL = true;

        static int Main()
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            Environment.CurrentDirectory = fiMainModule.Directory.FullName;
            string strRoot = "../../";
            DirectoryInfo dirRoot = new DirectoryInfo(strRoot);
            Environment.CurrentDirectory = dirRoot.FullName;
            CFileList.Instance.RootDirectory = Environment.CurrentDirectory;
            CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

            if (EXPORT_ALL)
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

            Console.WriteLine("********** 拷贝 *.proto 文件 **********");
            string exe1 = @".\TableGen\01_LopGeneration\";
            string src1 = @".\TableGen\10_ProtobufDef\";
            string src2 = @".\TableGen\11_ProtobufClt\";
            string des1 = @".\TableOut\Temp\1_Protoext\";

            DirectoryInfo exeDir1 = new DirectoryInfo(exe1);
            DirectoryInfo srcDir1 = new DirectoryInfo(src1);
            DirectoryInfo srcDir2 = new DirectoryInfo(src2);
            DirectoryInfo desDir1 = new DirectoryInfo(des1);

            if (!desDir1.Exists)
            {
                desDir1.Create();
            }

            List<string> protoList = new List<string>(); // 所有的proto文件
            if (srcDir1.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir1.FullName, desDir1.FullName));
                foreach (var fileInfo in srcDir1.GetFiles("*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = fileInfo.FullName.Replace('\\', '/');
                    protoList.Add(fileInfo.Name.ToLower());
                    newfile = newfile.Replace(srcDir1.FullName.Replace('\\', '/'), desDir1.FullName.Replace('\\', '/'));
                    File.Copy(fileInfo.FullName, newfile, true);
                }
            }

            if (srcDir2.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir2.FullName, desDir1.FullName));
                foreach (var fileInfo in srcDir2.GetFiles("*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = fileInfo.FullName.Replace('\\', '/');
                    protoList.Add(fileInfo.Name.ToLower());
                    newfile = newfile.Replace(srcDir2.FullName.Replace('\\', '/'), desDir1.FullName.Replace('\\', '/'));
                    File.Copy(fileInfo.FullName, newfile, true);
                }
            }

            DirectoryInfo desTableDir = new DirectoryInfo("./DesTable/DataTable");
            DirectoryInfo proTableDir = new DirectoryInfo("./ProTable");
            foreach (var fileInfo in desTableDir.GetFiles("*.xlsx", SearchOption.AllDirectories))
            {
                protoList.Add(fileInfo.Name.Replace("xlsx", "proto").ToLower());
            }
            foreach (var fileInfo in proTableDir.GetFiles("*.xlsx", SearchOption.AllDirectories))
            {
                protoList.Add(fileInfo.Name.Replace("xlsx", "proto").ToLower());
            }

            protoList.Add("globalenum.proto");
            protoList.Add("mailidenum.proto");
            foreach (var fileInfo in desDir1.GetFiles("*.proto", SearchOption.TopDirectoryOnly))
            {
                if (!protoList.Contains(fileInfo.Name.ToLower()))
                {
                    fileInfo.Delete();
                }
            }

            CFileList.Instance.ReadFileList();

            Console.WriteLine("********** 生成 *.proto 和 *.pe 文件 **********");
            string csPeDir = @".\TableOut\Temp\1_Protoext\C#";
            string ccPeDir = @".\TableOut\Temp\1_Protoext\C++";
            string goPeDir = @".\TableOut\Temp\1_Protoext\GO";
            string luaPeDir = @".\TableOut\Temp\1_Protoext\Lua";

            if (!Directory.Exists(csPeDir)) Directory.CreateDirectory(csPeDir);
            if (!Directory.Exists(ccPeDir)) Directory.CreateDirectory(ccPeDir);
            if (!Directory.Exists(goPeDir)) Directory.CreateDirectory(goPeDir);
            if (!Directory.Exists(luaPeDir)) Directory.CreateDirectory(luaPeDir);

            argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\GlobalTable.xlsx -OutputFile=.\TableOut\Temp\1_Protoext\globalEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Description";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\MailMsgTable.xlsx -OutputFile=.\TableOut\Temp\1_Protoext\mailIDEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Title";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\AccountServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\GlobalServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\DatabaseServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\LogServer\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\LogicTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\BattleTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\DesTable\DescTable\ -OutputPath=.\TableOut\Temp\1_Protoext\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            Console.WriteLine("********** 生成 *.pb 文件 **********");
            Environment.CurrentDirectory = desDir1.FullName;
            CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

            argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=C++";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            if (EXPORT_ALL)
            {
                argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=C#";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '), true);
            }

            argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=Lua";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            if (EXPORT_ALL)
            {
                argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_Protobuf\ -Language=Python";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            if (EXPORT_ALL)
            {
                Environment.CurrentDirectory = dirRoot.FullName;
                CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

                Console.WriteLine("********** 生成 *.bin 文件 **********");
                Environment.CurrentDirectory = exeDir1.FullName;
                CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

                argsStr = @"-GenType=Binary -DataPath=..\..\DesTable\DataTable\ -DescPath=..\..\DesTable\DescTable\ -PBSrcPath=..\..\TableOut\Temp\2_Protobuf\C#\ -OutputPath=..\..\TableOut\Temp\3_Protobin\";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            Environment.CurrentDirectory = dirRoot.FullName;
            CGeneration.Instance.RootDirectory = Environment.CurrentDirectory;

            Console.WriteLine("********** 生成 Msg 文件 **********");
            argsStr = @"-GenType=Msg -DataPath=.\TableGen\10_ProtobufDef\ -OutputPath=.\TableOut\Temp\4_Protomsg\ -Language=cpp_lua_cs";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            if (EXPORT_ALL)
            {
                Console.WriteLine("********** 生成 LuaCfg 文件 **********");
                argsStr = @"-GenType=LuaConfig -DataPath=.\DesTable\DescTable\ -OutputPath=.\TableOut\Lua\Generate\Parser\";
                Console.WriteLine(argsStr);
                Execute(argsStr.Split(' '));
            }

            //保存文件信息
            CFileList.Instance.SaveFileList();
            CFileList.Instance.ClearUnuseFile();

            //拷贝到工程目录下
            CSyncFile.Instance.Copy2TableOut();

            //自动生成lua.pb和config文件的require()到configFiles文件
            string path = ".\\TableOut\\Lua\\Generate\\";
            TSLib.ProtoGeneration.CGenerateLuaConfigFiles luaConfig = new TSLib.ProtoGeneration.CGenerateLuaConfigFiles(path);
            luaConfig.TransConfig();

            Console.WriteLine("刷新VS工程文件");
            string vsRefresh1 = dirRoot.FullName + "/TableOut/C++/PTLib/RefPTLibPrj.exe";
            System.Diagnostics.ProcessStartInfo ref1 = new System.Diagnostics.ProcessStartInfo();
            ref1.FileName = vsRefresh1;
            ref1.UseShellExecute = false;
            ref1.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/PTLib/";
            var proc1 = System.Diagnostics.Process.Start(ref1);
            proc1.WaitForExit();

            string vsRefresh2 = dirRoot.FullName + "/TableOut/C++/LTLib/RefLTLibPrj.exe";
            System.Diagnostics.ProcessStartInfo ref2 = new System.Diagnostics.ProcessStartInfo();
            ref2.FileName = vsRefresh2;
            ref2.UseShellExecute = false;
            ref2.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/LTLib/";
            var proc2 = System.Diagnostics.Process.Start(ref2);
            proc2.WaitForExit();

            string vsRefresh3 = dirRoot.FullName + "/TableOut/C++/MSLib/RefMSLibPrj.exe";
            System.Diagnostics.ProcessStartInfo ref3 = new System.Diagnostics.ProcessStartInfo();
            ref3.FileName = vsRefresh3;
            ref3.UseShellExecute = false;
            ref3.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            Environment.CurrentDirectory = dirRoot.FullName + "/TableOut/C++/MSLib/";
            var proc3 = System.Diagnostics.Process.Start(ref3);
            proc3.WaitForExit();

            Console.WriteLine("按任意键继续...");
            Console.ReadKey();
            return 0;
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
