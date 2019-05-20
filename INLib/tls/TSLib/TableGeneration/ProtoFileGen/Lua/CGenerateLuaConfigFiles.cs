using System;
using System.Collections.Generic;
using System.IO;

namespace Proto2Code
{

    class CGenerateLuaConfigFiles
    {
        private string rootpath;
        private string configPath;
        private string parserPath;
        private string protobufPath;
        private string messagePath;
        
 
        public CGenerateLuaConfigFiles(string path)
        {
            rootpath = path;
            configPath = rootpath + "ConfigFiles.lua";
            parserPath = rootpath + "Parser\\";
            protobufPath = rootpath + "Protobuf\\";
            messagePath = rootpath + "Message\\";
        }
        public void TransConfig()
        {
            List<FileInfo> luaList = GetFileList(parserPath);
            List<FileInfo> protoList = GetFileList(protobufPath);
            List<FileInfo> messageList = GetFileList(messagePath);

            List<FileInfo> fileList = MergeList(protoList, luaList);
            fileList = MergeList(fileList, messageList);

            List<string> configLines = GetFileText(configPath);
            List<string> replaceLines = RaplaceFile(configLines, fileList);
            SaveConfigFile(replaceLines, configPath);
        }

        private List<FileInfo> GetFileList(string path)
        {
            if (String.IsNullOrEmpty(path) || !Directory.Exists(path))
            {
                Console.WriteLine("目录不存在");
                return new List<FileInfo>();
            }
            List<FileInfo> fileList = new List<FileInfo>();

            DirectoryInfo directory = new DirectoryInfo(path);
            foreach (var file in directory.GetFiles())
            {
                if (file.Extension.Equals(".lua"))
                {
                    fileList.Add(file);
                }
            }
            return fileList;
        }

        private List<string> GetFileText(string path)
        {
            if (String.IsNullOrEmpty(path) || !File.Exists(path))
            {
                Console.WriteLine("文件不存在");
                return new List<string>();
            }
            List<string> lines = ReadFile(path);
            for (int i = 0; i < lines.Count;)
            {
                if (lines[i].Contains("require") || lines[i].Contains("--- auto generated file,do not edit"))
                {
                    lines.RemoveAt(i);
                    continue;
                }
                i++;
            }            
            return lines;
        }

        private List<FileInfo> MergeList(List<FileInfo> listA, List<FileInfo> listB)
        {
            List<FileInfo> list = new List<FileInfo>();

            foreach (FileInfo f in listA)
            {
                list.Add(f);
            }
            FileInfo temp = new FileInfo("flag");
            list.Add(temp);
            foreach (FileInfo f in listB)
            {
                list.Add(f);
            }
            return list;
        }
        private List<string> MergeList(List<string> listA, List<string> listB)
        {
            List<string> list = new List<string>();

            foreach (var s in listA)
            {
                list.Add(s);
            }
            foreach (var s in listB)
            {
                list.Add(s);
            }
            return list;
        }

        private List<string> ReadFile(string path)
        {
            if (String.IsNullOrEmpty(path) || !File.Exists(path))
            {
                return new List<string>();
            }
            List<string> lines = new List<string>();
            string line;
            StreamReader streamReader = new StreamReader(path);

            while ((line = streamReader.ReadLine())!= null)
            {
                lines.Add(line);
            }
            streamReader.Close();
            return lines;
        }

        private List<string> RaplaceFile(List<string> oldList, List<FileInfo> fileList)
        {
            List<string> newList = new List<string>();

            foreach (var file in fileList)
            {
                string line;
                if (file.Name == "flag")
                {
                    line = "    ";
                }
                else
                {
                    //ItemConfig                = require("Generate.Parser.ItemConfig")
                    line = Path.GetFileNameWithoutExtension(file.FullName) + " = "
                           + "require(\"Generate." + file.Directory.Name
                           + "." + Path.GetFileNameWithoutExtension(file.FullName) + "\")";
                }
                
                newList.Add(line);
            }
            newList = MergeList(newList, oldList);
            return newList;
        }

        private void SaveConfigFile(List<string> lines, string path)
        {
            if (String.IsNullOrEmpty(path))
            {
                return;
            }
            FileStream fileStream = new FileStream(path, FileMode.Create);
            StreamWriter streamWriter = new StreamWriter(fileStream);
            streamWriter.WriteLine("--------------------------------------------------------------------------------\n"
                               + "--  < autogenerated >\n"
                               + "--      This code was generated by a tool.\n"
                               + "--      Changes to this file may cause incorrect behavior and will be lost if\n"
                               + "--      the code is regenerated.\n"
                               + "--  </ autogenerated >\n"
                               + "--------------------------------------------------------------------------------\n");
            foreach (var line in lines)
            {
                streamWriter.WriteLine(line);
            }
            streamWriter.Flush();
            streamWriter.Close();
            fileStream.Close();
        }
    }
}
