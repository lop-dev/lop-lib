using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Proto2Code
{
    class Program
    {
        static int Main()
        {
            string strMainModule = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            FileInfo fiMainModule = new FileInfo(strMainModule);
            Environment.CurrentDirectory = fiMainModule.Directory.FullName;
            string strRoot = "../../";
            DirectoryInfo dirRoot = new DirectoryInfo(strRoot);
            Environment.CurrentDirectory = dirRoot.FullName;
            Console.WriteLine("AAAAAAAA = " + Environment.CurrentDirectory);

            Console.WriteLine("检查数据表");
            string dataTableCheckExe = dirRoot.FullName + "/DesTable/DataTableCheck/DataTableCheck.exe";
            FileInfo fileinfo = new FileInfo(dataTableCheckExe);
            System.Diagnostics.ProcessStartInfo Info = new System.Diagnostics.ProcessStartInfo();
            Info.FileName = dataTableCheckExe;
            Info.UseShellExecute = false;
            Info.WorkingDirectory = fileinfo.Directory.FullName;
            Info.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            var proc = System.Diagnostics.Process.Start(Info);
            proc.WaitForExit();
            if(proc.ExitCode!=0)
            {
                Console.WriteLine("配置表异常，请运行\"Shared/SrcGen/DesTable/数据表检查.bat\"进行检查。");
                Console.ReadLine();
                return 1;
            }

            //拷贝proto文件
            Console.WriteLine("拷贝Proto文件");
            Environment.CurrentDirectory = dirRoot.FullName;
            Console.WriteLine("AAAAAAAA = " + Environment.CurrentDirectory);

            string src1 = @".\TableGen\10_ProtobufDef\";
            string src2 = @".\TableGen\10_ProtobufClt\";
            string des1 = @".\TableOut\Temp\1_ProTableGen_Out\";

            DirectoryInfo srcDir1 = new DirectoryInfo(src1);
            DirectoryInfo srcDir2 = new DirectoryInfo(src2);
            DirectoryInfo desDir1 = new DirectoryInfo(des1);

            if (desDir1.Exists) desDir1.Delete(true);
            desDir1.Create();

            if (srcDir1.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir1.FullName, desDir1.FullName));
                foreach (string file in Directory.GetFiles(srcDir1.FullName, "*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = file.Replace('\\', '/');
                    newfile = newfile.Replace(srcDir1.FullName.Replace('\\', '/'), desDir1.FullName.Replace('\\', '/'));
                    File.Copy(file, newfile, true);
                }
            }

            if(srcDir2.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir2.FullName, desDir1.FullName));
                foreach (string file in Directory.GetFiles(srcDir2.FullName, "*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = file.Replace('\\', '/');
                    newfile = newfile.Replace(srcDir2.FullName.Replace('\\', '/'), desDir1.FullName.Replace('\\', '/'));
                    File.Copy(file, newfile, true);
                }
            }

            //
            Environment.CurrentDirectory = dirRoot.FullName;
            CFileList.Instance.ReadFileList();

            //生成Proto，PE文件
            Console.WriteLine("生成Proto，PE文件");
            Environment.CurrentDirectory = dirRoot.FullName;
            Console.WriteLine("AAAAAAAA = " + Environment.CurrentDirectory);

            string csPeDir = @".\TableOut\Temp\1_ProTableGen_Out\C#";
            string ccPeDir = @".\TableOut\Temp\1_ProTableGen_Out\C++";
            string goPeDir = @".\TableOut\Temp\1_ProTableGen_Out\GO";

            if (!Directory.Exists(csPeDir)) Directory.CreateDirectory(csPeDir);
            if (!Directory.Exists(ccPeDir)) Directory.CreateDirectory(ccPeDir);
            if (!Directory.Exists(goPeDir)) Directory.CreateDirectory(goPeDir);

            argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\GlobalTable.xlsx -OutputFile=.\TableOut\Temp\1_ProTableGen_Out\globalEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Description";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Enum -DataFile=.\DesTable\DataTable\MailMsgTable.xlsx -OutputFile=.\TableOut\Temp\1_ProTableGen_Out\mailIDEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Title";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\GlobalServer\ -OutputPath=.\TableOut\Temp\1_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\DatabaseServer\ -OutputPath=.\TableOut\Temp\1_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\LogServer\ -OutputPath=.\TableOut\Temp\1_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\LogicTable\ -OutputPath=.\TableOut\Temp\1_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\ProTable\BattleTable\ -OutputPath=.\TableOut\Temp\1_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=.\DesTable\DescTable\ -OutputPath=.\TableOut\Temp\1_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成PB文件
            Environment.CurrentDirectory = desDir1.FullName;
            Console.WriteLine("AAAAAAAA = " + Environment.CurrentDirectory);

            Console.WriteLine("生成PB文件");
            argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_ProtobufSrc_Out\ -Language=C++";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_ProtobufSrc_Out\ -Language=C#";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '),true);

            argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_ProtobufSrc_Out\ -Language=Lua";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Pbsrc -ProtoPath=.\ -OutputPath=..\2_ProtobufSrc_Out\ -Language=Python";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成bin文件
            Console.WriteLine("生成bin文件");
            Environment.CurrentDirectory = dirRoot.FullName;
            argsStr = @"-GenType=Binary -DataPath=.\DesTable\DataTable\ -DescPath=.\DesTable\DescTable\ -PBSrcPath=.\TableOut\Temp\2_ProtobufSrc_Out\C#\ -OutputPath=.\TableOut\Temp\3_DesTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成Msg文件
            Console.WriteLine("生成Msg文件");
            Environment.CurrentDirectory = dirRoot.FullName;
            argsStr = @"-GenType=Msg -DataPath=.\TableOut\Temp\0_ProtobufDef\ -OutputPath=.\TableOut\Temp\4_ProFileGen_Out\ -Language=cpp_lua_cs";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成LuaCfg文件
            Console.WriteLine("生成LuaCfg文件");
            Environment.CurrentDirectory = dirRoot.FullName;
            argsStr = @"-GenType=LuaConfig -DataPath=.\DesTable\DescTable\ -OutputPath=.\TableOut\Lua\Generate\Parser\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //保存文件信息
            CFileList.Instance.SaveFileList();
            CFileList.Instance.ClearUnuseFile();

            //拷贝到工程目录下
            CSyncFile.Instance.Copy2TableOut();

            #region
            //删掉后端C++相关文件（临时方案）
            string targetDic = "./TableOut/C++/gen/SHLib/";
            string[] protos = Directory.GetFiles("./TableGen/10_ProtobufClt/", "*.proto");
            foreach (var v in protos)
            {
                FileInfo file = new FileInfo(v);
                string name = file.Name.Split('.')[0];
                string targetFileProto = targetDic + "protofile/" + name + ".proto";
                string targetFileCPPH = targetDic + "protobuf/" + name + ".pb.h";
                string targetFileCPPCC = targetDic + "protobuf/" + name + ".pb.cc";
                if (File.Exists(targetFileProto))
                {
                    File.Delete(targetFileProto);
                }
                if (File.Exists(targetFileCPPH))
                {
                    File.Delete(targetFileCPPH);
                }
                if (File.Exists(targetFileCPPCC))
                {
                    File.Delete(targetFileCPPCC);
                }
            }
            #endregion

            //自动生成lua.pb和config文件的require()到configFiles文件
            string path = ".\\TableOut\\Lua\\Generate\\";
            TSLib.ProtoGeneration.CGenerateLuaConfigFiles luaConfig = new TSLib.ProtoGeneration.CGenerateLuaConfigFiles(path);
            luaConfig.TransConfig();
            
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
                CSLib.Utility.CDebugOut.LogError(string.Format("参数个数不对，请输入参数 -help 查看详细信息"));
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
