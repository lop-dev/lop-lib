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
    
    class CFileList : CSLib.Utility.CSingleton<CFileList>
    {
        /// <summary>
        /// 读取生成记录
        /// </summary>
        public void ReadFileList()
        {
            m_dicFileList.Clear();

            string tool = Environment.CurrentDirectory + "/LopGeneration.exe";
            string DLL_1 = Environment.CurrentDirectory + "/tableGeneration.dll";
            if (!File.Exists(m_strFileList))
            {
                File.Create(m_strFileList).Close();
                IsNewFile(tool);    //把工具文件信息放进去
                IsNewFile(DLL_1);
                return;
            }

            //
            if (IsNewFile(tool) || IsNewFile(DLL_1))
            {
                m_dicFileList.Clear();
                IsNewFile(tool);
                IsNewFile(DLL_1);
            }

            //
            StreamReader reader = new StreamReader(m_strFileList, Encoding.UTF8);
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrEmpty(line))
                {
                    continue;
                }
                    
                string[] infos = line.Split('|');
                if(!File.Exists(infos[0]))
                {
                    m_dicFileList.Add(infos[0],new CFileInfo(infos[0],infos[1],EFileType.Deleted));
                    continue;
                }

                string newMd5 = CSLib.Security.CMd5.EncodeFile(infos[0]);
                if (newMd5 == infos[1])
                {
                    m_dicFileList.Add(infos[0], new CFileInfo(infos[0], infos[1], EFileType.UnModified));
                }
                else
                {
                    m_dicFileList.Add(infos[0], new CFileInfo(infos[0], newMd5, EFileType.New));
                }
            }
            reader.Close();
        }

        /// <summary>
        /// 保存生成记录
        /// </summary>
        public void SaveFileList()
        {
            StreamWriter writer = new StreamWriter(m_strFileList, false,Encoding.UTF8);
            foreach (var v in m_dicFileList)
            {
                if(v.Value.type != EFileType.Deleted)
                {
                    writer.WriteLine(v.Key + "|" + v.Value.md5);
                }
            }
            writer.Close();
        }

        public void ClearUnuseFile()
        {
            foreach (var v in m_dicFileList)
            {
                if (v.Value.type != EFileType.Deleted)
                {
                    continue;
                }

                //残留的生成文件要删掉，先写死。
                if (v.Key.EndsWith(".xlsx"))
                {
                    //删掉proto,pe,txt,dbg文件
                    Console.WriteLine("删除" + v.Key + "生成的残留文件");
                    string name = GetFileNameAndFirstCharToLower(v.Key);

                    string proto = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/1_ProTableGen_Out/{0}.proto", name);
                    string cspe = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/1_ProTableGen_Out/C#/{0}.pe.cs", name);
                    string ccpeh = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/1_ProTableGen_Out/C++/{0}.pe.h", name);
                    string ccpecc = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/1_ProTableGen_Out/C++/{0}.pe.cc", name);
                    string gope = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/1_ProTableGen_Out/GO/{0}.pe.go", name);

                    string txt = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/3_DesTableGen_Out/{0}.txt", name);
                    string dbg = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/3_DesTableGen_Out/{0}.txt.dbg", name);

                    string cspb = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/C#/{0}.pb.cs", name);
                    string ccpbh = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/C++/{0}.pb.h", name);
                    string ccpbcc = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/C++/{0}.pb.cc", name);
                    string luapb = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/Lua/{0}_pb.lua", name);

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
                if (v.Key.EndsWith(".proto"))
                {
                    //删掉proto,pe,txt,dbg文件
                    Console.WriteLine("删除" + v.Key + "生成的残留文件");
                    string name = GetFileNameAndFirstCharToLower(v.Key);

                    string cspb = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/C#/{0}.pb.cs", name);
                    string ccpbh = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/C++/{0}.pb.h", name);
                    string ccpbcc = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/C++/{0}.pb.cc", name);
                    string luapb = Environment.CurrentDirectory + string.Format("/./TableOut/Temp/2_ProtobufSrc_Out/Lua/{0}_pb.lua", name);

                    if (File.Exists(cspb)) File.Delete(cspb);
                    if (File.Exists(ccpbh)) File.Delete(ccpbh);
                    if (File.Exists(ccpbcc)) File.Delete(ccpbcc);
                    if (File.Exists(luapb)) File.Delete(luapb);
                }
            }
        }

        /// <summary>
        /// 文件是否被修改过
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        public bool IsNewFile(string file)
        {
            string mfile = file.Replace('\\', '/');
            if (!File.Exists(mfile))
            {
                CSLib.Utility.CDebugOut.LogError(string.Format("文件不存在。\r{0}", file));
                Console.WriteLine("文件不存在。\r{0}", file);
                return false;
            }

            CFileInfo cfi = null;
            m_dicFileList.TryGetValue(mfile, out cfi);
            if (null == cfi)
            {
                FileInfo fi = new FileInfo(mfile);
                mfile = fi.FullName.Replace('\\', '/');
                string md5 = CSLib.Security.CMd5.EncodeFile(mfile);

                cfi = new CFileInfo(mfile, md5, EFileType.New);
                m_dicFileList.Add(mfile, cfi);
            }

            if (cfi.type == EFileType.New)
            {
                return true;
            }

            return false;
        }

        //将文件标记为新文件
        public bool SetNewFile(string file)
        {
            string f = file.Replace('\\','/');
            FileInfo fi = new FileInfo(f);
            if(!fi.Exists)
            {
                //CSLib.Utility.CDebugOut.LogError("文件不存在:"+f);
                return false;
            }

            f = fi.FullName.Replace('\\','/');
            if(m_dicFileList.ContainsKey(f))
            {
                m_dicFileList[f].type = EFileType.New;
                return true;
            }

            return false;
        }

        //将文件名首字母转为小写
        public string FirstCharToLower(string str)
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
        public string GetFileNameAndFirstCharToLower(string file)
        {
            if (string.IsNullOrEmpty(file))
                return "";
            string s = file.Replace('\\', '/');
            string[] strArray = s.Split('/');
            string temp = char.ToLower(strArray[strArray.Length - 1][0]) + strArray[strArray.Length - 1].Substring(1);
            return temp.Substring(0,temp.IndexOf('.'));
        }

        private readonly string m_strFileList = @".\TableOut\Temp\FileList.txt";
        private Dictionary<string, CFileInfo> m_dicFileList = new Dictionary<string, CFileInfo>();
    }
}
