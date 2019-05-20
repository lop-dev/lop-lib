using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace RefreshSHLibPrj
{
    class SHLibManage
    {
        SHLibInfo sHLibInfo = new SHLibInfo();
        /// <summary>
        /// 把所有文件名按类别保存 已实现
        /// </summary>
        /// <param name="path"></param>
        public void ReadFiles(string path)
        {
            //path = path.Replace("\\", "/");
            string protobuf = path + "protobuf\\";
            string protoext = path + "protoext\\";
            string protofile = path + "protofile\\";
            GetFiles(protobuf);
            GetFiles(protoext);
            GetFiles(protofile);
            //string[] directorys = Directory.GetDirectories(@path);
            //foreach (var s in directorys)
            //{
            //    //Console.WriteLine(s);
            //    GetFiles(s);
            //}
        }

        public void ReadVcxproj(string path)
        {
            sHLibInfo.lines = ReadFile(path);
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
                    sHLibInfo.cFiles.Add(file);
                if (file.Extension == ".proto")
                    sHLibInfo.protoFiles.Add(file);
                if (file.Extension == ".h")
                    sHLibInfo.hFiles.Add(file);
            }
        }
        public void showFiles()
        {
            foreach (var f in sHLibInfo.cFiles)
            {
                Console.WriteLine(f.Name);
            }
            foreach (var f in sHLibInfo.hFiles)
            {
                Console.WriteLine(f.Name);
            }
            foreach (var f in sHLibInfo.protoFiles)
            {
                Console.WriteLine(f.Name);
            }
            foreach (string s in sHLibInfo.lines)
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
            if(id == 1)
            {
                foreach(string s in sHLibInfo.lines)
                {
                    Console.WriteLine(s);
                }
            }
            if(id == 2)
            {
                foreach (string s in sHLibInfo.newLines)
                {
                    Console.WriteLine(s);
                }
            }
        }
        /// <summary>
        /// 删掉应被替换的行
        /// </summary>
        private void deleteLines()
        {
            for(int i = 0; i < sHLibInfo.lines.Count;)
            {
                if ((sHLibInfo.lines[i].Contains("ClInclude") && sHLibInfo.lines[i].Contains("protobuf") )
                    ||(sHLibInfo.lines[i].Contains("protobuf") && sHLibInfo.lines[i].Contains("ClCompile"))
                    || (sHLibInfo.lines[i].Contains("ClCompile") && sHLibInfo.lines[i].Contains("protoext") )
                    || (sHLibInfo.lines[i].Contains("protoext") && sHLibInfo.lines[i].Contains("ClInclude"))
                    || (sHLibInfo.lines[i].Contains("None") && sHLibInfo.lines[i].Contains("protofile")))
                {
                    sHLibInfo.lines.RemoveAt(i);
                }else
                {
                    i++;
                }
            }
        }
        private void AddHFile()
        {
            foreach(FileInfo f in sHLibInfo.hFiles)
            {
                string line = "    <ClInclude Include=\"..\\..\\..\\gen\\SHLib\\" + f.Directory.Name+"\\" + f.Name + "\" />";
                sHLibInfo.newLines.Add(line);
            }
        }
        private void AddCFile()
        {
            foreach(FileInfo f in sHLibInfo.cFiles)
            {
                string line = "    <ClCompile Include=\"..\\..\\..\\gen\\SHLib\\" + f.Directory.Name + "\\" + f.Name + "\" />";
                sHLibInfo.newLines.Add(line);
            }
        }
        private void AddPFile()
        {
            foreach (FileInfo f in sHLibInfo.protoFiles)
            {
                string line = "    <None Include=\"..\\..\\..\\gen\\SHLib\\" + f.Directory.Name + "\\" + f.Name + "\" />";
                sHLibInfo.newLines.Add(line);
            }
        }
        public void ReplaceVCXFile()
        {
            deleteLines();
            bool cflag = true;
            bool hflag = true;
            //bool pflag = true;
            int itemGroupNum = 0;
            foreach(string s in sHLibInfo.lines)
            {
                if (s.Trim().Equals("<ItemGroup>"))
                {
                    itemGroupNum++;
                    sHLibInfo.newLines.Add(s);
                    continue;
                }
                if(s.Trim().StartsWith("<ClInclude Include"))
                {
                    if (hflag)
                    {
                        AddHFile();
                        hflag = false;
                    }
                }
                if(s.Trim().StartsWith("<ClCompile Include"))
                {
                    if (cflag)
                    {
                        AddCFile();
                        cflag = false;
                    }
                }
                if(itemGroupNum == 3)
                {
                    AddPFile();
                    itemGroupNum = 0;
                }
                sHLibInfo.newLines.Add(s);
            }
        }
        public void saveVCXFile(string path)
        {
            FileStream fs = new FileStream(path, FileMode.Create);
            var utf8WithBom = new System.Text.UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fs,utf8WithBom);
            
            //开始写入
            int i = 0;
            foreach (string s in sHLibInfo.newLines)
            {
                if (i == sHLibInfo.newLines.Count - 1)
                {
                    sw.Write(s);
                }
                else
                {
                    sw.WriteLine(s);
                }
                i++;
            }
            
            //清空缓冲区
            sw.Flush();
            //关闭流
            sw.Close();
            fs.Close();
        }

        private void deleteFliterLines()
        {
            for (int i = 0; i < sHLibInfo.lines.Count;)
            {
                if ((sHLibInfo.lines[i].Contains("ClInclude") && sHLibInfo.lines[i].Contains("protobuf"))
                    || (sHLibInfo.lines[i].Contains("protobuf") && sHLibInfo.lines[i].Contains("ClCompile"))
                    || (sHLibInfo.lines[i].Contains("ClCompile") && sHLibInfo.lines[i].Contains("protoext"))
                    || (sHLibInfo.lines[i].Contains("protoext") && sHLibInfo.lines[i].Contains("ClInclude"))
                    || (sHLibInfo.lines[i].Contains("None") && sHLibInfo.lines[i].Contains("protofile")))
                {
                    sHLibInfo.lines.RemoveAt(i);
                    sHLibInfo.lines.RemoveAt(i);
                    sHLibInfo.lines.RemoveAt(i);
                }
                else
                {
                    i++;
                }
            }
        }
        private void AddFliterHFile()
        {
            foreach (FileInfo f in sHLibInfo.hFiles)
            {
                string line = "    <ClInclude Include=\"..\\..\\..\\gen\\SHLib\\" + f.Directory.Name + "\\" + f.Name + "\">";
                sHLibInfo.fliterLines.Add(line);
                if (f.Directory.Parent.Name.Equals("SHLib"))
                {
                    line = "      <Filter>"+f.Directory.Name+"</Filter>";
                }else
                {
                    line = "      <Filter>" +f.Directory.Parent.Name+"\\" +f.Directory.Name + "</Filter>";
                }
                
                sHLibInfo.fliterLines.Add(line);
                line = "    </ClInclude>";
                sHLibInfo.fliterLines.Add(line);
            }
        }
        private void AddFliterCFile()
        {
            foreach (FileInfo f in sHLibInfo.cFiles)
            {
                string line = "    <ClCompile Include=\"..\\..\\..\\gen\\SHLib\\" + f.Directory.Name + "\\" + f.Name + "\">";
                sHLibInfo.fliterLines.Add(line);
                if (f.Directory.Parent.Name.Equals("SHLib"))
                {
                    line = "      <Filter>" + f.Directory.Name + "</Filter>";
                }
                else
                {
                    line = "      <Filter>" + f.Directory.Parent.Name + "\\" + f.Directory.Name + "</Filter>";
                }

                sHLibInfo.fliterLines.Add(line);
                line = "    </ClCompile>";
                sHLibInfo.fliterLines.Add(line);
            }
        }
        private void AddFliterPFile()
        {
            foreach (FileInfo f in sHLibInfo.protoFiles)
            {
                string line = "    <None Include=\"..\\..\\..\\gen\\SHLib\\" + f.Directory.Name + "\\" + f.Name + "\">";
                sHLibInfo.fliterLines.Add(line);
                if (f.Directory.Parent.Name.Equals("SHLib"))
                {
                    line = "      <Filter>" + f.Directory.Name + "</Filter>";
                }
                else
                {
                    line = "      <Filter>" + f.Directory.Parent.Name + "\\" + f.Directory.Name + "</Filter>";
                }

                sHLibInfo.fliterLines.Add(line);
                line = "    </None>";
                sHLibInfo.fliterLines.Add(line);
            }
        }
        public void ReplaceVCXFliterFile()
        {
            deleteFliterLines();
            bool cflag = true;
            bool hflag = true;
            //bool pflag = true;
            int itemGroupNum = 0;
            foreach (string s in sHLibInfo.lines)
            {
                if (s.Trim().Equals("<ItemGroup>"))
                {
                    itemGroupNum++;
                    sHLibInfo.fliterLines.Add(s);
                    continue;
                }
                if (s.Trim().StartsWith("<ClInclude Include"))
                {
                    if (hflag)
                    {
                        AddFliterHFile();
                        hflag = false;
                    }
                }
                if (s.Trim().StartsWith("<ClCompile Include"))
                {
                    if (cflag)
                    {
                        AddFliterCFile();
                        cflag = false;
                    }
                }
                if (itemGroupNum == 4)
                {
                    AddFliterPFile();
                    itemGroupNum = 0;
                }
                sHLibInfo.fliterLines.Add(s);
            }
        }
        public void saveVCXFliterFile(string path)
        {

            FileStream fs = new FileStream(path, FileMode.Create);
            var utf8WithBom = new System.Text.UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fs, utf8WithBom);
            
            //开始写入
            int i = 0;
            foreach (string s in sHLibInfo.fliterLines)
            {
                if (i == sHLibInfo.fliterLines.Count - 1)
                {
                    sw.Write(s);
                }
                else
                {
                    sw.WriteLine(s);
                }
                i++;
            }

            //清空缓冲区
            sw.Flush();
            //关闭流
            sw.Close();
            fs.Close();
        }


    }
}
