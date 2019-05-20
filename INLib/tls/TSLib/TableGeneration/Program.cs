using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Proto2Code
{
    public enum EFileType
    {
        Deleted,    //已被删除
        New,        //新文件或被修改过
        UnModified,   //未被修改
    }

    class CFileInfo
    {
        public string name;
        public string md5;
        public EFileType type;
        public CFileInfo(string name,string md5,EFileType type)
        {
            this.name = name;
            this.md5 = md5;
            this.type = type;
        }
    }
    class Program
    {
        static int Main()
        {
            Console.WriteLine("检查数据表");
            System.Diagnostics.ProcessStartInfo Info = new System.Diagnostics.ProcessStartInfo();
            string dataTableCheckExe = Info.FileName = Environment.CurrentDirectory + "/../../DesTable/DataTableCheck/DataTableCheck.exe";
            FileInfo fileinfo = new FileInfo(dataTableCheckExe);
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
            string src1 = @"..\10_ProtobufDef\";
            string src2 = @"..\10_ProtobufClt\";
            string des = @"..\11_ProTableGen_Out\";
            DirectoryInfo srcDir1 = new DirectoryInfo(src1);
            DirectoryInfo srcDir2 = new DirectoryInfo(src2);
            DirectoryInfo desDir = new DirectoryInfo(des);
            if (!desDir.Exists)
            {
                desDir.Create();
            }
            if(srcDir1.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir1.FullName, desDir.FullName));
                foreach (string file in Directory.GetFiles(srcDir1.FullName, "*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = file.Replace('\\', '/');
                    newfile = newfile.Replace(srcDir1.FullName.Replace('\\', '/'), desDir.FullName.Replace('\\', '/'));
                    //if(IsNewFile(file))
                    {
                        File.Copy(file, newfile, true);
                    }
                }
            }

            if(srcDir2.Exists)
            {
                Console.WriteLine(string.Format("Copy {0}*.proto to {1}", srcDir2.FullName, desDir.FullName));
                foreach (string file in Directory.GetFiles(srcDir2.FullName, "*.proto", SearchOption.TopDirectoryOnly))
                {
                    string newfile = file.Replace('\\', '/');
                    newfile = newfile.Replace(srcDir2.FullName.Replace('\\', '/'), desDir.FullName.Replace('\\', '/'));
                    //if(IsNewFile(file))
                    {
                        File.Copy(file, newfile, true);
                    }
                }
            }
            
            GetHistoryFileInfo();

            //生成Proto，PE文件
            Console.WriteLine("生成Proto，PE文件");
            string csPeDir = @"..\11_ProTableGen_Out\C#";
            string ccPeDir = @"..\11_ProTableGen_Out\C++";
            string goPeDir = @"..\11_ProTableGen_Out\GO";
            if (!Directory.Exists(csPeDir)) Directory.CreateDirectory(csPeDir);
            if (!Directory.Exists(ccPeDir)) Directory.CreateDirectory(ccPeDir);
            if (!Directory.Exists(goPeDir)) Directory.CreateDirectory(goPeDir);

            argsStr = @"-GenType=Enum -DataFile=..\..\DesTable\DataTable\GlobalTable.xlsx -OutputFile=..\11_ProTableGen_Out\globalEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Description";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Enum -DataFile=..\..\DesTable\DataTable\MailMsgTable.xlsx -OutputFile=..\11_ProTableGen_Out\mailIDEnum.proto -EnumName=EnumName -EnumValue=ID -EnumDesc=Title";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=..\..\ProTable\GlobalServer\ -OutputPath=..\11_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=..\..\ProTable\DatabaseServer\ -OutputPath=..\11_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=..\..\ProTable\LogServer\ -OutputPath=..\11_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=..\..\ProTable\LogicTable\ -OutputPath=..\11_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=..\..\ProTable\BattleTable\ -OutputPath=..\11_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Proto -DescPath=..\..\DesTable\DescTable\ -OutputPath=..\11_ProTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成PB文件
            Console.WriteLine("生成PB文件");
            argsStr = @"-GenType=Pbsrc -ProtoPath=..\11_ProTableGen_Out -OutputPath=..\12_ProtobufSrc_Out\ -Language=C++";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Pbsrc -ProtoPath=..\11_ProTableGen_Out -OutputPath=..\12_ProtobufSrc_Out\ -Language=C#";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '),true);

            argsStr = @"-GenType=Pbsrc -ProtoPath=..\11_ProTableGen_Out -OutputPath=..\12_ProtobufSrc_Out\ -Language=Lua";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            argsStr = @"-GenType=Pbsrc -ProtoPath=..\11_ProTableGen_Out -OutputPath=..\12_ProtobufSrc_Out\ -Language=Python";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成bin文件
            Console.WriteLine("生成bin文件");
            argsStr = @"-GenType=Binary -DataPath=..\..\DesTable\DataTable\ -DescPath=..\..\DesTable\DescTable\ -PBSrcPath=..\12_ProtobufSrc_Out\C#\ -OutputPath=..\13_DesTableGen_Out\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成Msg文件
            Console.WriteLine("生成Msg文件");
            argsStr = @"-GenType=Msg -DataPath=..\10_ProtobufDef\ -OutputPath=..\14_ProFileGen_Out\ -Language=cpp_lua_cs";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //生成LuaCfg文件
            Console.WriteLine("生成LuaCfg文件");
            argsStr = @"-GenType=LuaConfig -DataPath=..\..\DesTable\DescTable\ -OutputPath=..\..\TableOut\Lua\Generate\Parser\";
            Console.WriteLine(argsStr);
            Execute(argsStr.Split(' '));

            //保存文件信息
            SaveFileInfo();

            //拷贝到工程目录下
            Copy2WorkDir();

            #region
            //删掉后端C++相关文件（临时方案）
            string targetDic = "../../TableOut/C++/gen/SHLib/";
            string[] protos = Directory.GetFiles("../10_ProtobufClt/","*.proto");
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
            string path = new DirectoryInfo("../../").FullName + "\\TableOut\\Lua\\Generate\\";
            CGenerateLuaConfigFiles luaConfig = new CGenerateLuaConfigFiles(path);
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

        //拷贝到工程目录下
        private static void Copy2WorkDir()
        {
            Console.WriteLine("拷贝文件到客户端目录");
            string CLIENT_PB_CS_SRC = @"..\12_ProtobufSrc_Out\C#\";
            string CLIENT_PB_CS_DES = @"..\..\TableOut\C#\Generate\Protobuf\";
            Copy2Dir(CLIENT_PB_CS_SRC, "*.cs", CLIENT_PB_CS_DES);

            string CLIENT_PE_CS_SRC = @"..\11_ProTableGen_Out\C#\";
            string CLIENT__PE_CS_DES = @"..\..\TableOut\C#\Generate\Protoext\";
            Copy2Dir(CLIENT_PE_CS_SRC, "*.cs", CLIENT__PE_CS_DES);

            string CLIENT_PB_LUA_SRC = @"..\12_ProtobufSrc_Out\Lua\";
            string CLIENT_PB_LUA_DES = @"..\..\TableOut\Lua\Generate\Protobuf\";
            Copy2Dir(CLIENT_PB_LUA_SRC,"*.lua", CLIENT_PB_LUA_DES);

            string CLIENT_MSG_DEFINE_SRC = @"..\14_ProFileGen_Out\Lua\Message\";
            string CLIENT_MSG_DEFINE_DES = @"..\..\TableOut\Lua\Generate\Message\";
            Copy2Dir(CLIENT_MSG_DEFINE_SRC, "*.lua", CLIENT_MSG_DEFINE_DES);

            string CLIENT_MSG_SERVICES_SRC = @"..\14_ProFileGen_Out\Lua\Services\";
            string CLIENT_MSG_SERVICES_DES = @"..\..\TableOut\Lua\Generate\Services\";
            Copy2Dir(CLIENT_MSG_SERVICES_SRC, "*.lua", CLIENT_MSG_SERVICES_DES);
            
            Console.WriteLine("拷贝文件到服务器目录");
            string SERVER_PROTO_SRC = @"..\11_ProTableGen_Out\";
            string SERVER_PROTO_DES = @"..\..\TableOut\C++\gen\SHLib\protofile\";
            Copy2Dir(SERVER_PROTO_SRC, "*.proto", SERVER_PROTO_DES);

            string SERVER_PE_TABLE_SRC = @"..\11_ProTableGen_Out\C++\";
            string SERVER_PE_TABLE_DES_H = @"..\..\TableOut\C++\gen\SHLib\protoext\";
            string SERVER_PE_TABLE_DES_CC = @"..\..\TableOut\C++\gen\SHLib\protoext\";
            Copy2Dir(SERVER_PE_TABLE_SRC, "*.pe.h", SERVER_PE_TABLE_DES_H);
            Copy2Dir(SERVER_PE_TABLE_SRC, "*.pe.cc", SERVER_PE_TABLE_DES_CC);

            string SERVER_PB_TABLE_SRC = @"..\12_ProtobufSrc_Out\C++\";
            string SERVER_PB_TABLE_DES_H = @"..\..\TableOut\C++\gen\SHLib\protobuf\";
            string SERVER_PB_TABLE_DES_CC = @"..\..\TableOut\C++\gen\SHLib\protobuf\";
            Copy2Dir(SERVER_PB_TABLE_SRC, "*.pb.h", SERVER_PB_TABLE_DES_H);
            Copy2Dir(SERVER_PB_TABLE_SRC, "*.pb.cc", SERVER_PB_TABLE_DES_CC);

            string SERVER_PROFILEGEN_SRC = @"..\14_ProFileGen_Out\C++\";
            string SERVER_PROFILEGEN_DES_H = @"..\..\TableOut\C++\gen\SHLib\message\";
            string SERVER_PROFILEGEN_DES_CPP = @"..\..\TableOut\C++\gen\SHLib\message\";
            Copy2Dir(SERVER_PROFILEGEN_SRC, "*.h", SERVER_PROFILEGEN_DES_H);
            Copy2Dir(SERVER_PROFILEGEN_SRC, "*.cc", SERVER_PROFILEGEN_DES_CPP);


            //拷贝到PTLib目录
            string SERVER_PTLIB_PROTO_SRC = @"..\11_ProTableGen_Out\";
            string SERVER_PTLIB_PROTO_DES = @"..\..\TableOut\C++\PTLib\inc\PTLib\protofile\";
            Copy2Dir(SERVER_PTLIB_PROTO_SRC, "*.proto", SERVER_PTLIB_PROTO_DES);

            string SERVER_PTLIB_PE_TABLE_SRC = @"..\11_ProTableGen_Out\C++\";
            string SERVER_PTLIB_PE_TABLE_DES_H = @"..\..\TableOut\C++\PTLib\inc\PTLib\protoext\";
            string SERVER_PTLIB_PE_TABLE_DES_CC = @"..\..\TableOut\C++\PTLib\src\PTLib\protoext\";
            Copy2Dir(SERVER_PTLIB_PE_TABLE_SRC, "*.pe.h", SERVER_PTLIB_PE_TABLE_DES_H);
            Copy2Dir(SERVER_PTLIB_PE_TABLE_SRC, "*.pe.cc", SERVER_PTLIB_PE_TABLE_DES_CC);

            string SERVER_PTLIB_PB_TABLE_SRC = @"..\12_ProtobufSrc_Out\C++\";
            string SERVER_PTLIB_PB_TABLE_DES_H = @"..\..\TableOut\C++\PTLib\inc\PTLib\protobuf\";
            string SERVER_PTLIB_PB_TABLE_DES_CC = @"..\..\TableOut\C++\PTLib\src\PTLib\protobuf\";
            Copy2Dir(SERVER_PTLIB_PB_TABLE_SRC, "*.pb.h", SERVER_PTLIB_PB_TABLE_DES_H);
            Copy2Dir(SERVER_PTLIB_PB_TABLE_SRC, "*.pb.cc", SERVER_PTLIB_PB_TABLE_DES_CC);

            //string SERVER_PTLIB_PROFILEGEN_SRC = @"..\14_ProFileGen_Out\C++\";
            //string SERVER_PTLIB_PROFILEGEN_DES_H = @"..\..\TableOut\C++\PTLib\inc\PTLib\message\";
            //string SERVER_PTLIB_PROFILEGEN_DES_CPP = @"..\..\TableOut\C++\PTLib\src\PTLib\message\";
            //Copy2Dir(SERVER_PTLIB_PROFILEGEN_SRC, "*.h", SERVER_PTLIB_PROFILEGEN_DES_H);
            //Copy2Dir(SERVER_PTLIB_PROFILEGEN_SRC, "*.cc", SERVER_PTLIB_PROFILEGEN_DES_CPP);
            
            Console.WriteLine("拷贝bin文件");
            string BIN_SRC = @"..\13_DesTableGen_Out\";
            string BIN_DES = @"..\..\TableOut\Protobin\";
            Copy2Dir(BIN_SRC, "*.txt", BIN_DES);

            Console.WriteLine("拷贝dbg文件");
            string DBG_SRC = @"..\13_DesTableGen_Out\";
            string DBG_DES= @"..\..\TableOut\Protobindbg\";
            Copy2Dir(DBG_SRC, "*.dbg", DBG_DES);

            //python 文件
            string CLIENT_PB_PY_SRC = @"..\12_ProtobufSrc_Out\Python\";
            string CLIENT_PB_PY_DES = @"..\..\TableOut\Python\gen\protobuf\";
            Copy2Dir(CLIENT_PB_PY_SRC, "*.py", CLIENT_PB_PY_DES);
        }

        private static int Copy2Dir(string srcDir,string searchOption, string desDir)
        {
            if(!Directory.Exists(srcDir))
            {
                Console.WriteLine(string.Format("源目录不存在，请检查{0}", srcDir));
                return -1;
            }
            if(!Directory.Exists(desDir))
            {
                Directory.CreateDirectory(desDir);
            }
            DirectoryInfo dd = new DirectoryInfo(desDir);
            Console.WriteLine(string.Format("Copy {0}\\{1} to {2}", srcDir,searchOption, desDir));
            List<string> fileList = new List<string>();
            foreach (string file in Directory.GetFiles(srcDir, searchOption, SearchOption.TopDirectoryOnly))
            {
                FileInfo info = new FileInfo(file);
                string desFile = Path.Combine(dd.FullName, info.Name).Replace('\\','/');
                File.Copy(file, desFile, true);
                fileList.Add(desFile);
                fileList.Add(desFile+".meta");
            }
            foreach(string file in Directory.GetFiles(dd.FullName, searchOption, SearchOption.TopDirectoryOnly))
            {
                if (!fileList.Contains(file.Replace('\\', '/')))
                    File.Delete(file);
            }
            foreach (string file in Directory.GetFiles(dd.FullName, searchOption +".meta", SearchOption.TopDirectoryOnly))
            {
                if (!fileList.Contains(file.Replace('\\', '/')))
                    File.Delete(file);
            }
            return 0;
        }

        /// <summary>
        /// 获取之前的生成记录
        /// </summary>
        private static void GetHistoryFileInfo()
        {
            m_fileInfoDic.Clear();
            string tool = Environment.CurrentDirectory + "/TableGeneration.exe";
            string DLL_1 = Environment.CurrentDirectory + "/cspGeneration.dll";
            if (!File.Exists(m_historyFileInfoPath))
            {
                File.Create(m_historyFileInfoPath).Close();
                IsNewFile(tool);    //把工具文件信息放进去
                IsNewFile(DLL_1);
                return;
            }
            StreamReader reader = new StreamReader(m_historyFileInfoPath, Encoding.UTF8);
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrEmpty(line))
                    continue;
                string[] infos = line.Split('|');
//                 if (infos[0].Split('/')[infos[0].Split('/').Length - 1] == "petTable.proto")
//                     Console.ReadLine();
                if(!File.Exists(infos[0]))
                {
                    m_fileInfoDic.Add(infos[0],new CFileInfo(infos[0],infos[1],EFileType.Deleted));
                    continue;
                }
                string newMd5 = "";
                newMd5 = CSLib.Security.CMd5.EncodeFile(infos[0]);
                if (newMd5 == infos[1])
                {
                    m_fileInfoDic.Add(infos[0], new CFileInfo(infos[0], infos[1], EFileType.UnModified));
                }else
                {
                    m_fileInfoDic.Add(infos[0], new CFileInfo(infos[0], newMd5, EFileType.New));
                }
            }
            reader.Close();
            if (IsNewFile(tool) || IsNewFile(DLL_1))
            {
                m_fileInfoDic.Clear();
                IsNewFile(tool);
                IsNewFile(DLL_1);
            }
        }

        /// <summary>
        /// 保存生成记录
        /// </summary>
        private static void SaveFileInfo()
        {
            StreamWriter writer = new StreamWriter(m_historyFileInfoPath, false,Encoding.UTF8);
            foreach (var v in m_fileInfoDic)
            {
                if(v.Value.type != EFileType.Deleted)
                {
                    writer.WriteLine(v.Key + "|" + v.Value.md5);
                }
                else
                {
                    //残留的生成文件要删掉，先写死。
                    if (v.Key.EndsWith(".xlsx"))
                    {
                        //删掉proto,pe,txt,dbg文件
                        Console.WriteLine("删除"+v.Key+"生成的残留文件");
                        string name = GetFileNameAndFirstCharToLower(v.Key);

                        string proto = Environment.CurrentDirectory + string.Format("/../11_ProTableGen_Out/{0}.proto",name);
                        string cspe = Environment.CurrentDirectory + string.Format("/../11_ProTableGen_Out/C#/{0}.pe.cs",name);
                        string ccpeh = Environment.CurrentDirectory + string.Format("/../11_ProTableGen_Out/C++/{0}.pe.h",name);
                        string ccpecc = Environment.CurrentDirectory + string.Format("/../11_ProTableGen_Out/C++/{0}.pe.cc",name);
                        string gope = Environment.CurrentDirectory + string.Format("/../11_ProTableGen_Out/GO/{0}.pe.go",name);

                        string txt = Environment.CurrentDirectory + string.Format("/../13_DesTableGen_Out/{0}.txt",name);
                        string dbg = Environment.CurrentDirectory + string.Format("/../13_DesTableGen_Out/{0}.txt.dbg",name);

                        string cspb = Environment.CurrentDirectory + string.Format("/../12_ProtobufSrc_Out/C#/{0}.pb.cs", name);
                        string ccpbh = Environment.CurrentDirectory + string.Format("/../12_ProtobufSrc_Out/C++/{0}.pb.h", name);
                        string ccpbcc = Environment.CurrentDirectory + string.Format("/../12_ProtobufSrc_Out/C++/{0}.pb.cc", name);
                        string luapb = Environment.CurrentDirectory + string.Format("/../12_ProtobufSrc_Out/Lua/{0}_pb.lua", name);

                        if (File.Exists(proto)) File.Delete(proto);
                        if (File.Exists(cspe)) File.Delete(cspe);
                        if (File.Exists(ccpeh)) File.Delete(ccpeh);
                        if (File.Exists(ccpecc)) File.Delete(ccpecc);
                        if (File.Exists(gope)) File.Delete(gope);
                        if (File.Exists(txt)) File.Delete(txt);
                        if (File.Exists(dbg)) File.Delete(dbg);
                        if (File.Exists(cspb)) File.Delete(cspb);
                        if (File.Exists(ccpbh)) File.Delete(ccpbh);
                        if (File.Exists(ccpbcc)) File.Delete(ccpbcc);
                        if (File.Exists(luapb)) File.Delete(luapb);
                    }
                }
            }
            writer.Close();
        }
        
        /// <summary>
        /// 文件是否被修改过
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        public static bool IsNewFile(string file)
        {
            string mfile = file.Replace('\\', '/');
            if (!File.Exists(mfile))
            {
                CSLib.Utility.CDebugOut.LogError(string.Format("文件不存在。\r{0}", file));
                Console.WriteLine("文件不存在。\r{0}", file);
                return false;
            }
            FileInfo fi = new FileInfo(mfile);
            mfile = fi.FullName.Replace('\\','/');
            string md5 = CSLib.Security.CMd5.EncodeFile(mfile);
            CFileInfo cfi = null;
            m_fileInfoDic.TryGetValue(mfile, out cfi);
            if (null == cfi)
            {
                m_fileInfoDic.Add(mfile, new CFileInfo(mfile,md5,EFileType.New));
                return true;
            }
            if (cfi.type == EFileType.New)
            {
                return true;
            }else if(cfi.type == EFileType.UnModified)
            {
                return false;
            }
            return false;
        }

        //将文件标记为新文件
        public static bool SetFileNew(string file)
        {
            string f = file.Replace('\\','/');
            FileInfo fi = new FileInfo(f);
            if(!fi.Exists)
            {
                //CSLib.Utility.CDebugOut.LogError("文件不存在:"+f);
                return false;
            }
            f = fi.FullName.Replace('\\','/');
            if(m_fileInfoDic.ContainsKey(f))
            {
                m_fileInfoDic[f].type = EFileType.New;
                return true;
            }
            return false;
        }

        //将文件名首字母转为小写
        public static string FirstCharToLower(string str)
        {
            if (string.IsNullOrEmpty(str))
                return "";
            string s = str.Replace('\\', '/');
            string[] strArray = s.Split('/');
            string result = "";
            for(int i=0;i<strArray.Length-1;i++)
            {
                result = result + strArray[i] + "/";
            }
            return result + char.ToLower(strArray[strArray.Length-1][0]) + strArray[strArray.Length - 1].Substring(1);
        }

        //根据文件的绝对路径获取首字母小写的文件名
        public static string GetFileNameAndFirstCharToLower(string file)
        {
            if (string.IsNullOrEmpty(file))
                return "";
            string s = file.Replace('\\', '/');
            string[] strArray = s.Split('/');
            string temp = char.ToLower(strArray[strArray.Length - 1][0]) + strArray[strArray.Length - 1].Substring(1);
            return temp.Substring(0,temp.IndexOf('.'));
        }

        private static string argsStr = "";
        private static readonly string m_historyFileInfoPath = @"..\\HistoryFileInfo.txt";
        public static Dictionary<string, CFileInfo> m_fileInfoDic = new Dictionary<string, CFileInfo>();
    }
}
