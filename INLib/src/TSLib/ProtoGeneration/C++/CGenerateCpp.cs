using System.IO;

namespace TSLib.ProtoGeneration
{
    public static class CGenerateCpp
    {
        public static void Process(string inputDir, string writePathRoot)
        {
            writePathRoot += "C++";
            if (!Directory.Exists(writePathRoot))
            {
                Directory.CreateDirectory(writePathRoot);
            }
            writePathRoot += "/";
            
            DirectoryInfo directoryInfo = new DirectoryInfo(inputDir);
            FileInfo[] fileInfos = directoryInfo.GetFiles();
            foreach (var fileInfo in fileInfos)
            {
                _ProcessFile(fileInfo.FullName, fileInfo.Name.Replace(".proto", string.Empty), writePathRoot);
            }
        }
        
        private static void _ProcessFile(string filePath, string file_name, string writePathRoot)
        {
            CProtoFile load = new CProtoFile();
            
            if (load.LoadFile(filePath, file_name, writePathRoot))
            {
                CGenerateModuleMessageCC cpp = new CGenerateModuleMessageCC(load.NameUnit, writePathRoot);
                CGenerateModuleMessageH h = new CGenerateModuleMessageH(load.NameUnit, writePathRoot);

                cpp.StartWrite(load.MsgIDList);
                h.StartWriter(load.MsgIDList, load.EFUNC);

                load.MsgIDList.Clear();

                //Console.WriteLine("Success~~~~");
            }
            else
            {
                //Console.WriteLine(string.Format("LOG: >> file:{0}.proto is NOT a DefinedMsgProtoFile, passed it.", file_name));
            }
        }
    }
}