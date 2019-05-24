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
            string CLIENT_PB_CS_SRC = @".\TableOut\Temp\2_ProtobufSrc_Out\C#\";
            string CLIENT_PB_CS_DES = @".\TableOut\C#\Generate\Protobuf\";
            CopyDirectory(CLIENT_PB_CS_SRC, CLIENT_PB_CS_DES, "*.cs");

            string CLIENT_PE_CS_SRC = @".\TableOut\Temp\1_ProTableGen_Out\C#\";
            string CLIENT__PE_CS_DES = @".\TableOut\C#\Generate\Protoext\";
            CopyDirectory(CLIENT_PE_CS_SRC, CLIENT__PE_CS_DES, "*.cs");

            Console.WriteLine("拷贝文件之：Lua");
            string CLIENT_PB_LUA_SRC = @".\TableOut\Temp\2_ProtobufSrc_Out\Lua\";
            string CLIENT_PB_LUA_DES = @".\TableOut\Lua\Generate\Protobuf\";
            CopyDirectory(CLIENT_PB_LUA_SRC, CLIENT_PB_LUA_DES, "*.lua");

            string CLIENT_MSG_DEFINE_SRC = @".\TableOut\Temp\4_ProFileGen_Out\Lua\Message\";
            string CLIENT_MSG_DEFINE_DES = @".\TableOut\Lua\Generate\Message\";
            CopyDirectory(CLIENT_MSG_DEFINE_SRC, CLIENT_MSG_DEFINE_DES, "*.lua");

            string CLIENT_MSG_SERVICES_SRC = @".\TableOut\Temp\4_ProFileGen_Out\Lua\Services\";
            string CLIENT_MSG_SERVICES_DES = @".\TableOut\Lua\Generate\Services\";
            CopyDirectory(CLIENT_MSG_SERVICES_SRC, CLIENT_MSG_SERVICES_DES, "*.lua");

            Console.WriteLine("拷贝文件之：C++");
            string SERVER_PROTO_SRC = @".\TableOut\Temp\1_ProTableGen_Out\";
            string SERVER_PROTO_DES = @".\TableOut\C++\gen\SHLib\protofile\";
            CopyDirectory(SERVER_PROTO_SRC, SERVER_PROTO_DES, "*.proto");

            string SERVER_PE_TABLE_SRC = @".\TableOut\Temp\1_ProTableGen_Out\C++\";
            string SERVER_PE_TABLE_DES_H = @".\TableOut\C++\gen\SHLib\protoext\";
            string SERVER_PE_TABLE_DES_CC = @".\TableOut\C++\gen\SHLib\protoext\";
            CopyDirectory(SERVER_PE_TABLE_SRC, SERVER_PE_TABLE_DES_H, "*.pe.h");
            CopyDirectory(SERVER_PE_TABLE_SRC, SERVER_PE_TABLE_DES_CC, "*.pe.cc");

            string SERVER_PB_TABLE_SRC = @".\TableOut\Temp\2_ProtobufSrc_Out\C++\";
            string SERVER_PB_TABLE_DES_H = @".\TableOut\C++\gen\SHLib\protobuf\";
            string SERVER_PB_TABLE_DES_CC = @".\TableOut\C++\gen\SHLib\protobuf\";
            CopyDirectory(SERVER_PB_TABLE_SRC, SERVER_PB_TABLE_DES_H, "*.pb.h");
            CopyDirectory(SERVER_PB_TABLE_SRC, SERVER_PB_TABLE_DES_CC, "*.pb.cc");

            string SERVER_PROFILEGEN_SRC = @".\TableOut\Temp\4_ProFileGen_Out\C++\";
            string SERVER_PROFILEGEN_DES_H = @".\TableOut\C++\gen\SHLib\message\";
            string SERVER_PROFILEGEN_DES_CPP = @".\TableOut\C++\gen\SHLib\message\";
            CopyDirectory(SERVER_PROFILEGEN_SRC, SERVER_PROFILEGEN_DES_H, "*.h");
            CopyDirectory(SERVER_PROFILEGEN_SRC, SERVER_PROFILEGEN_DES_CPP, "*.cc");

            //拷贝到PTLib目录
            string SERVER_PTLIB_PROTO_SRC = @".\TableOut\Temp\1_ProTableGen_Out\";
            string SERVER_PTLIB_PROTO_DES = @".\TableOut\C++\PTLib\inc\PTLib\protofile\";
            CopyDirectory(SERVER_PTLIB_PROTO_SRC, SERVER_PTLIB_PROTO_DES, "*.proto");

            string SERVER_PTLIB_PE_TABLE_SRC = @".\TableOut\Temp\1_ProTableGen_Out\C++\";
            string SERVER_PTLIB_PE_TABLE_DES_H = @".\TableOut\C++\PTLib\inc\PTLib\protoext\";
            string SERVER_PTLIB_PE_TABLE_DES_CC = @".\TableOut\C++\PTLib\src\PTLib\protoext\";
            CopyDirectory(SERVER_PTLIB_PE_TABLE_SRC, SERVER_PTLIB_PE_TABLE_DES_H, "*.pe.h");
            CopyDirectory(SERVER_PTLIB_PE_TABLE_SRC, SERVER_PTLIB_PE_TABLE_DES_CC, "*.pe.cc");

            string SERVER_PTLIB_PB_TABLE_SRC = @".\TableOut\Temp\2_ProtobufSrc_Out\C++\";
            string SERVER_PTLIB_PB_TABLE_DES_H = @".\TableOut\C++\PTLib\inc\PTLib\protobuf\";
            string SERVER_PTLIB_PB_TABLE_DES_CC = @".\TableOut\C++\PTLib\src\PTLib\protobuf\";
            CopyDirectory(SERVER_PTLIB_PB_TABLE_SRC, SERVER_PTLIB_PB_TABLE_DES_H, "*.pb.h");
            CopyDirectory(SERVER_PTLIB_PB_TABLE_SRC, SERVER_PTLIB_PB_TABLE_DES_CC, "*.pb.cc");

            //拷贝到MSLib目录
            string SERVER_MSLIB_PROFILEGEN_SRC = @".\TableOut\Temp\4_ProFileGen_Out\C++\";
            string SERVER_MSLIB_PROFILEGEN_DES_H = @".\TableOut\C++\MSLib\inc\MSLib\message\";
            string SERVER_MSLIB_PROFILEGEN_DES_CPP = @".\TableOut\C++\MSLib\src\MSLib\message\";
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_H, "*.h");
            CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, SERVER_MSLIB_PROFILEGEN_DES_CPP, "*.cc");

            Console.WriteLine("拷贝文件之：Proto");
            string PROTO_SRC = @".\TableOut\Temp\1_ProTableGen_Out\";
            string PROTO_DES = @".\TableOut\Proto\";
            CopyDirectory(PROTO_SRC, PROTO_DES, "*.proto");

            Console.WriteLine("拷贝文件之：Protobin");
            string PROTOBIN_SRC = @".\TableOut\Temp\3_DesTableGen_Out\";
            string PROTOBIN_DES = @".\TableOut\Protobin\";
            CopyDirectory(PROTOBIN_SRC, PROTOBIN_DES, "*.txt");

            Console.WriteLine("拷贝文件之：Protobindbg");
            string PROTOBINDBG_SRC = @".\TableOut\Temp\3_DesTableGen_Out\";
            string PROTOBINDBG_DES = @".\TableOut\Protobindbg\";
            CopyDirectory(PROTOBINDBG_SRC, PROTOBINDBG_DES, "*.dbg");

            Console.WriteLine("拷贝文件之：Python");
            string CLIENT_PB_PY_SRC = @".\TableOut\Temp\2_ProtobufSrc_Out\Python\";
            string CLIENT_PB_PY_DES = @".\TableOut\Python\gen\protobuf\";
            CopyDirectory(CLIENT_PB_PY_SRC, CLIENT_PB_PY_DES, "*.py");
        }

        public int CopyDirectory(string srcDir, string desDir, string searchOption)
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

            Console.WriteLine(string.Format("Copy {0}\\{1} to {2}", srcDir, searchOption, desDir));

            //
            DirectoryInfo dd = new DirectoryInfo(desDir);
            
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
    }
}
