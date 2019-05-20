using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RefreshSHLibPrj
{
    abstract class SHLibVCX
    {
        private SHLibInfo info = new SHLibInfo();

        abstract protected void ReplaceFile();
        abstract protected void DeleteLines();
        abstract protected void AddHFlie();
        abstract protected void AddCFile();
        abstract protected void AddPFile();

        public void ReadFiles(string path)
        {
            //path = path.Replace("\\", "/");
            string[] directorys = Directory.GetDirectories(@path);
            foreach (var s in directorys)
            {
                //Console.WriteLine(s);
                GetFiles(s);
            }
        }

        public void ReadVcxproj(string path)
        {
            info.lines = ReadFile(path);
        }
        /// <summary>
        /// 读取文本文件
        /// </summary>
        /// <param name="filePath">文件路径</param>
        /// <returns>每一行的文本内容</returns>
        private List<string> ReadFile(string filePath)
        {
            string line;
            List<string> lines = new List<string>();
            int counter = 0;
            System.IO.StreamReader file =
                new System.IO.StreamReader(@filePath);
            //Console.WriteLine("读取文件中~~~~~~");
            while ((line = file.ReadLine()) != null)
            {
                lines.Add(line);
                //System.Console.WriteLine(line);
                counter++;
            }
            file.Close();
            //System.Console.WriteLine("There were {0} lines.", counter);
            return lines;
        }
        private void GetFiles(string path)
        {
            DirectoryInfo directory = new DirectoryInfo(path);
            foreach (FileInfo file in directory.GetFiles())
            {
                Console.WriteLine(file.Directory.Parent.Name);
                //Console.WriteLine(file.Name);
                //string fileExtension = file.Extension;
                if (file.Extension == ".cc" || file.Extension == ".c" || file.Extension == ".cpp")
                    info.cFiles.Add(file);
                if (file.Extension == ".proto")
                    info.protoFiles.Add(file);
                if (file.Extension == ".h")
                    info.hFiles.Add(file);
            }
        }
        public void showFiles()
        {
            foreach (var f in info.cFiles)
            {
                Console.WriteLine(f.Name);
            }
            foreach (var f in info.hFiles)
            {
                Console.WriteLine(f.Name);
            }
            foreach (var f in info.protoFiles)
            {
                Console.WriteLine(f.Name);
            }
            foreach (string s in info.lines)
            {
                Console.WriteLine(s);
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="id">1是旧表，2是新表</param>
        public void showVCXFile(int id)
        {
            if (id == 1)
            {
                foreach (string s in info.lines)
                {
                    Console.WriteLine(s);
                }
            }
            if (id == 2)
            {
                foreach (string s in info.newLines)
                {
                    Console.WriteLine(s);
                }
            }
        }
    }
}
