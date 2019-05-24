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
            Console.WriteLine("拷贝文件到客户端目录");
            string CLIENT_PB_CS_SRC = @"..\12_ProtobufSrc_Out\C#\";
            string CLIENT_PB_CS_DES = @"..\..\TableOut\C#\Generate\Protobuf\";
            _CopyDirectory(CLIENT_PB_CS_SRC, "*.cs", CLIENT_PB_CS_DES);

            string CLIENT_PE_CS_SRC = @"..\11_ProTableGen_Out\C#\";
            string CLIENT__PE_CS_DES = @"..\..\TableOut\C#\Generate\Protoext\";
            _CopyDirectory(CLIENT_PE_CS_SRC, "*.cs", CLIENT__PE_CS_DES);

            string CLIENT_PB_LUA_SRC = @"..\12_ProtobufSrc_Out\Lua\";
            string CLIENT_PB_LUA_DES = @"..\..\TableOut\Lua\Generate\Protobuf\";
            _CopyDirectory(CLIENT_PB_LUA_SRC,"*.lua", CLIENT_PB_LUA_DES);

            string CLIENT_MSG_DEFINE_SRC = @"..\14_ProFileGen_Out\Lua\Message\";
            string CLIENT_MSG_DEFINE_DES = @"..\..\TableOut\Lua\Generate\Message\";
            _CopyDirectory(CLIENT_MSG_DEFINE_SRC, "*.lua", CLIENT_MSG_DEFINE_DES);

            string CLIENT_MSG_SERVICES_SRC = @"..\14_ProFileGen_Out\Lua\Services\";
            string CLIENT_MSG_SERVICES_DES = @"..\..\TableOut\Lua\Generate\Services\";
            _CopyDirectory(CLIENT_MSG_SERVICES_SRC, "*.lua", CLIENT_MSG_SERVICES_DES);
            
            Console.WriteLine("拷贝文件到服务器目录");
            string SERVER_PROTO_SRC = @"..\11_ProTableGen_Out\";
            string SERVER_PROTO_DES = @"..\..\TableOut\C++\gen\SHLib\protofile\";
            _CopyDirectory(SERVER_PROTO_SRC, "*.proto", SERVER_PROTO_DES);

            string SERVER_PE_TABLE_SRC = @"..\11_ProTableGen_Out\C++\";
            string SERVER_PE_TABLE_DES_H = @"..\..\TableOut\C++\gen\SHLib\protoext\";
            string SERVER_PE_TABLE_DES_CC = @"..\..\TableOut\C++\gen\SHLib\protoext\";
            _CopyDirectory(SERVER_PE_TABLE_SRC, "*.pe.h", SERVER_PE_TABLE_DES_H);
            _CopyDirectory(SERVER_PE_TABLE_SRC, "*.pe.cc", SERVER_PE_TABLE_DES_CC);

            string SERVER_PB_TABLE_SRC = @"..\12_ProtobufSrc_Out\C++\";
            string SERVER_PB_TABLE_DES_H = @"..\..\TableOut\C++\gen\SHLib\protobuf\";
            string SERVER_PB_TABLE_DES_CC = @"..\..\TableOut\C++\gen\SHLib\protobuf\";
            _CopyDirectory(SERVER_PB_TABLE_SRC, "*.pb.h", SERVER_PB_TABLE_DES_H);
            _CopyDirectory(SERVER_PB_TABLE_SRC, "*.pb.cc", SERVER_PB_TABLE_DES_CC);

            string SERVER_PROFILEGEN_SRC = @"..\14_ProFileGen_Out\C++\";
            string SERVER_PROFILEGEN_DES_H = @"..\..\TableOut\C++\gen\SHLib\message\";
            string SERVER_PROFILEGEN_DES_CPP = @"..\..\TableOut\C++\gen\SHLib\message\";
            _CopyDirectory(SERVER_PROFILEGEN_SRC, "*.h", SERVER_PROFILEGEN_DES_H);
            _CopyDirectory(SERVER_PROFILEGEN_SRC, "*.cc", SERVER_PROFILEGEN_DES_CPP);

            //拷贝到PTLib目录
            string SERVER_PTLIB_PROTO_SRC = @"..\11_ProTableGen_Out\";
            string SERVER_PTLIB_PROTO_DES = @"..\..\TableOut\C++\PTLib\inc\PTLib\protofile\";
            _CopyDirectory(SERVER_PTLIB_PROTO_SRC, "*.proto", SERVER_PTLIB_PROTO_DES);

            string SERVER_PTLIB_PE_TABLE_SRC = @"..\11_ProTableGen_Out\C++\";
            string SERVER_PTLIB_PE_TABLE_DES_H = @"..\..\TableOut\C++\PTLib\inc\PTLib\protoext\";
            string SERVER_PTLIB_PE_TABLE_DES_CC = @"..\..\TableOut\C++\PTLib\src\PTLib\protoext\";
            _CopyDirectory(SERVER_PTLIB_PE_TABLE_SRC, "*.pe.h", SERVER_PTLIB_PE_TABLE_DES_H);
            _CopyDirectory(SERVER_PTLIB_PE_TABLE_SRC, "*.pe.cc", SERVER_PTLIB_PE_TABLE_DES_CC);

            string SERVER_PTLIB_PB_TABLE_SRC = @"..\12_ProtobufSrc_Out\C++\";
            string SERVER_PTLIB_PB_TABLE_DES_H = @"..\..\TableOut\C++\PTLib\inc\PTLib\protobuf\";
            string SERVER_PTLIB_PB_TABLE_DES_CC = @"..\..\TableOut\C++\PTLib\src\PTLib\protobuf\";
            _CopyDirectory(SERVER_PTLIB_PB_TABLE_SRC, "*.pb.h", SERVER_PTLIB_PB_TABLE_DES_H);
            _CopyDirectory(SERVER_PTLIB_PB_TABLE_SRC, "*.pb.cc", SERVER_PTLIB_PB_TABLE_DES_CC);

            //拷贝到MSLib目录
            string SERVER_MSLIB_PROFILEGEN_SRC = @"..\14_ProFileGen_Out\C++\";
            string SERVER_MSLIB_PROFILEGEN_DES_H = @"..\..\TableOut\C++\MSLib\inc\MSLib\message\";
            string SERVER_MSLIB_PROFILEGEN_DES_CPP = @"..\..\TableOut\C++\MSLib\src\MSLib\message\";
            _CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, "*.h", SERVER_MSLIB_PROFILEGEN_DES_H);
            _CopyDirectory(SERVER_MSLIB_PROFILEGEN_SRC, "*.cc", SERVER_MSLIB_PROFILEGEN_DES_CPP);

            Console.WriteLine("拷贝bin文件");
            string BIN_SRC = @"..\13_DesTableGen_Out\";
            string BIN_DES = @"..\..\TableOut\Protobin\";
            _CopyDirectory(BIN_SRC, "*.txt", BIN_DES);

            Console.WriteLine("拷贝dbg文件");
            string DBG_SRC = @"..\13_DesTableGen_Out\";
            string DBG_DES= @"..\..\TableOut\Protobindbg\";
            _CopyDirectory(DBG_SRC, "*.dbg", DBG_DES);

            //python 文件
            string CLIENT_PB_PY_SRC = @"..\12_ProtobufSrc_Out\Python\";
            string CLIENT_PB_PY_DES = @"..\..\TableOut\Python\gen\protobuf\";
            _CopyDirectory(CLIENT_PB_PY_SRC, "*.py", CLIENT_PB_PY_DES);
        }

        private int _CopyDirectory(string srcDir,string searchOption, string desDir)
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
