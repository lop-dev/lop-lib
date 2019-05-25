using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace TSLib.ProtoGeneration
{
    public class CGenerateLuaConfig
    {
        //ConfigFiles.lua 生成模板
        public static string FILE_TEMPLATE = @"--- auto generated file,do not edit

local ConfigFiles = {
    {0}
}

return ConfigFiles";

        //XXXConfig.lua 生成模板
        private static string LUA_CONFIG_TEMPLATE = @"--- 仅首次通过工具生成，可修改

local {0}Config = {}

-- cache cfg data
_G.cached{0}Table       = _G.cached{0}Table or {}
local cfgTable          = _G.cached{0}Table


--- 获取配置数据
function {0}Config:getCfgById(id)
    local result = cfgTable[id]
    if result then return result end

    result = ConfigManager:getInstance():getConfigById(ConfigFiles.{0}Table, id)
    cfgTable[id] = result

    return result
end

return {0}Config
";

        private static string m_strRegex1 = @"['.xml', '.xlsx', '.xls']$";    //正则表达式 
        private static string m_strRegex2 = @"^~";
        private Regex m_regex1 = new Regex(m_strRegex1);
        private Regex m_regex2 = new Regex(m_strRegex2);

        private string m_strProtobufDef = "";   //配置文件目录
        private string m_strTempDirectory = "";
        private string m_strOutputPath = "";    //拷贝目标目录

        /// <summary>
        /// 需要传入绝对地址
        /// </summary>
        /// <param name="strProtobufDef"></param>
        /// <param name="strOutputPath"></param>
        public void Start(string strProtobufDef, string strTempDirectory, string strOutputPath)
        {
            DirectoryInfo di1 = new DirectoryInfo(strProtobufDef);
            m_strProtobufDef = di1.FullName;
            m_strProtobufDef = m_strProtobufDef.Replace('\\', '/');

            DirectoryInfo di2 = new DirectoryInfo(strTempDirectory);
            m_strTempDirectory = di2.FullName;
            m_strTempDirectory = m_strTempDirectory.Replace('\\', '/');

            DirectoryInfo di3 = new DirectoryInfo(strOutputPath);
            m_strOutputPath = di3.FullName;
            m_strOutputPath = m_strOutputPath.Replace('\\', '/');

            _GenFile();
        }

        private void _GenFile()
        {
            //----------------------- 生成XxxConfig.lua文件 -------------------------------------
            string strTempLuaPath = m_strTempDirectory + @"/Lua";
            if (!Directory.Exists(strTempLuaPath))
            {
                Directory.CreateDirectory(strTempLuaPath);
            }

            //先删除当前目录下lua文件
            _RemoveSpecifiedFiles(strTempLuaPath, ".lua");
            string[] fileNamesPro = _FilterFiles(m_strProtobufDef).ToArray();
            string tContent = "", tStrDefine = "", tFileName = "";
            foreach (string fnp in fileNamesPro)
            {
                tFileName = fnp + "Table";
                tStrDefine = tStrDefine + tFileName + " = '" + tFileName.ToLower()[0] + tFileName.Substring(1, tFileName.Length - 1) + "',\n\t";
                tContent = FILE_TEMPLATE.Replace("{0}", tStrDefine);

                //生成xxxConfig.lua
                string fileName = fnp + "Config.lua";
                FileStream fs = File.Open(strTempLuaPath + "\\" + fileName, FileMode.OpenOrCreate);
                string code = LUA_CONFIG_TEMPLATE.Replace("{0}", fnp);
                byte[] data = System.Text.Encoding.UTF8.GetBytes(code);
                fs.Write(data, 0, data.Length);
                fs.Flush();
                fs.Close();
                if (!File.Exists(m_strOutputPath + fileName))
                {
                    File.Copy(strTempLuaPath + "\\" + fileName, m_strOutputPath + fileName);
                }
            }
            if (File.Exists(m_strTempDirectory + @"/ConfigFiles.lua"))
                File.Delete(m_strTempDirectory + @"/ConfigFiles.lua");
            FileStream fs2 = File.Open(m_strTempDirectory + @"/ConfigFiles.lua", FileMode.OpenOrCreate);
            byte[] data2 = System.Text.Encoding.UTF8.GetBytes(tContent);
            fs2.Write(data2, 0, data2.Length);
            fs2.Flush();
            fs2.Close();
            File.Copy(m_strTempDirectory + @"/ConfigFiles.lua", m_strOutputPath + @"\..\" + "ConfigFiles.lua",true);
        }

        //获取指定目录下指定文件类型的文件名
        private List<string> _FilterFiles(string tarPath)
        {
            List<string> fileList = new List<string>();
            if (!Directory.Exists(tarPath))
            {
                Console.WriteLine("不存在文件夹：" + tarPath);
            }
            else
            {
                string[] files = Directory.GetFiles(tarPath);
                //Console.WriteLine(string.Format("---------------item length>>>{0} items", files.Length));
                foreach (string file in files)
                {
                    FileInfo fi = new FileInfo(file);
                    //Console.WriteLine(string.Format("---------------------item name>>>{0}", fi.Name));
                    if (m_regex1.Match(fi.Name).Success && !m_regex2.Match(fi.Name).Success)
                    {
                        int index = fi.Name.IndexOf("Table");
                        if (index < 0)
                        {
                            Console.WriteLine("文件命名错误：" + fi.Name);
                            continue;
                        }
                        string fileName = fi.Name.Substring(0, index);
                        fileList.Add(fileName);
                    }
                }
            }
            return fileList;
        }

        //删除指定目录下的指定类型文件
        private bool _RemoveSpecifiedFiles(string tarPath, string fileType)
        {
            if (Directory.Exists(tarPath))    //如果是文件夹
            {
                foreach (string file in Directory.GetFileSystemEntries(tarPath, "*.*"))
                {
                    _RemoveSpecifiedFiles(file, fileType);
                }
            }
            else if (File.Exists(tarPath))  //如果是文件
            {
                FileInfo info = new FileInfo(tarPath);
                if (info.Extension.ToLower().Equals(fileType.ToLower()))
                {
                    //System.Console.WriteLine("删除：" + info.FullName);
                    File.Delete(tarPath);
                }
            }
            else
            {
                System.Console.WriteLine("路径错误");
                return false;
            }
            return true;
        }
    }
}
