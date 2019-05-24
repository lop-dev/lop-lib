using System.IO;

namespace TSLib.ProtoGeneration
{
    public static class CGenerateLua
    {
        public static void Process(string inputDir, string writePathRoot)
        {
            writePathRoot += "Lua";
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
            if(!file_name.Contains("System"))
            {
                return;
            }

            CProtoFile c = new CProtoFile();
            if (c.LoadFile(filePath, file_name,writePathRoot,false))
            {
                string msgDefinePath = writePathRoot + "Message/";
                Directory.CreateDirectory(msgDefinePath);
                CGenerateLuaMessage msgDefinesWriter = new CGenerateLuaMessage(c.NameUnit, msgDefinePath);
                msgDefinesWriter.StartWrite(c.MsgIDList);

                msgDefinePath = writePathRoot + "Services/";
                Directory.CreateDirectory(msgDefinePath);
                CGenerateLuaService serviceBaseWriter = new CGenerateLuaService(c.NameUnit, msgDefinePath);
                serviceBaseWriter.StartWrite(c.MsgIDList);

                CGenerateLuaSystem seriviceWirter = new CGenerateLuaSystem(c.NameUnit, writePathRoot);
                seriviceWirter.StartWrite(c.MsgIDList);


                c.MsgIDList.Clear();
                //Console.WriteLine("Success~~~~");
            }
            else
            {
                //Console.WriteLine(string.Format("LOG: >> file:{0}.proto is NOT a DefinedMsgProtoFile, passed it.", file_name));
            }
        }
    }
}