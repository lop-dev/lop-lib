using System.IO;

namespace Proto2Code
{
    public static class CGenerateCsp
    {
        public static void Process(string inputDir, string writePathRoot)
        {
            writePathRoot += "C#";
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
            if (!file_name.Contains("System"))
            {
                return;
            }
            CProtoFile cs = new CProtoFile();
            if (cs.LoadFile(filePath, file_name, writePathRoot, false))
            {
                CGenerateCspMessage csMsgModule = new CGenerateCspMessage(cs.NameUnit,writePathRoot);
                csMsgModule.StartWrite(cs.MsgIDList);
                cs.MsgIDList.Clear();
            }
        }
    }
}
