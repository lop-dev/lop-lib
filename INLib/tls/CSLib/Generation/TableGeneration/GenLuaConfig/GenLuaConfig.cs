using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace TableGeneration
{
    class GenLuaConfig
    {
        //ConfigFiles.lua 生成模板
        public static string FILE_TEMPLATE = @"--- auto generated file,do not edit

local ConfigFiles = {
    {0}
}

return ConfigFiles";
        //XXXConfig.lua 生成模板
        public static string LUA_CONFIG_TEMPLATE = @"--- 仅首次通过工具生成，可修改

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

        public static string regex1 = @"['.xml', '.xlsx', '.xls']$";    //正则表达式 
        public static string regex2 = @"^~";
        public Regex re1 = new Regex(regex1);
        public Regex re2 = new Regex(regex2);
        public static string pathInput = @"..\..\DesTable\DescTable";   //配置表目录
        public static string desPath = "";      //拷贝目标目录
        public void Start(string dataPath,string outputPath)
        {
            GenLuaConfig p = new GenLuaConfig();
            pathInput = Path.Combine(Environment.CurrentDirectory ,dataPath);
            desPath = Path.Combine(Environment.CurrentDirectory , outputPath);
//             pathInput = curDir + "\\..\\..\\DesTable\\DescTable";
//             desPath = curDir + @"\..\..\..\..\Client\Dev\Assets\App\Pro\Lua\Game\Table\Parser\";
            p.genFile();
        }

        public void genFile()
        {
            //----------------------- 生成XxxConfig.lua文件 -------------------------------------
            string tOutPath = Environment.CurrentDirectory + @"\..\15_CfgFileGen_Out\Lua";
            //string tOutPath = @"D:\Client\Shared\SrcGen\TableGen\15_CfgFileGen_Out\Lua";
            if (!Directory.Exists(tOutPath))
            {
                Directory.CreateDirectory(tOutPath);
            }
            //先删除当前目录下lua文件
            removeSpecifiedFiles(tOutPath, ".lua");
            string[] fileNamesPro = filterFiles(pathInput).ToArray();
            string tContent = "", tStrImport = "", tStrDefine = "", tFileName = "";
            foreach (string fnp in fileNamesPro)
            {
                tFileName = fnp + "Table";
                tStrDefine = tStrDefine + tFileName + " = '" + tFileName.ToLower()[0] + tFileName.Substring(1, tFileName.Length - 1) + "',\n\t";
                tContent = FILE_TEMPLATE.Replace("{0}", tStrDefine);
                //tFileName = fnp.ToLower()[0] + fnp.Substring(1, fnp.Length - 1) + "Table_pb";
                //tStrImport = tStrImport + "{0} = require(\"Game.Generate.Protobuf.{0}\")\n".Replace("{0}", tFileName);
                //tContent = tContent.Replace("{1}", tStrImport);

                //生成xxxConfig.lua
                string fileName = fnp + "Config.lua";
                FileStream fs = File.Open(tOutPath + "\\" + fileName, FileMode.OpenOrCreate);
                string code = LUA_CONFIG_TEMPLATE.Replace("{0}", fnp);
                byte[] data = System.Text.Encoding.UTF8.GetBytes(code);
                fs.Write(data, 0, data.Length);
                fs.Flush();
                fs.Close();
                if (!File.Exists(desPath + fileName))
                {
                    File.Copy(tOutPath + "\\" + fileName, desPath + fileName);
                }
            }
            if (File.Exists(Environment.CurrentDirectory + @"\..\15_CfgFileGen_Out\ConfigFiles.lua"))
                File.Delete(Environment.CurrentDirectory + @"\..\15_CfgFileGen_Out\ConfigFiles.lua");
            FileStream fs2 = File.Open(Environment.CurrentDirectory + @"\..\15_CfgFileGen_Out\ConfigFiles.lua", FileMode.OpenOrCreate);
            byte[] data2 = System.Text.Encoding.UTF8.GetBytes(tContent);
            fs2.Write(data2, 0, data2.Length);
            fs2.Flush();
            fs2.Close();
            File.Copy(Environment.CurrentDirectory + @"\..\15_CfgFileGen_Out\ConfigFiles.lua", desPath + @"\..\" + "ConfigFiles.lua",true);
        }

        //获取指定目录下指定文件类型的文件名
        public List<string> filterFiles(string tarPath)
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
                    if (re1.Match(fi.Name).Success && !re2.Match(fi.Name).Success)
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
        public bool removeSpecifiedFiles(string tarPath, string fileType)
        {
            if (Directory.Exists(tarPath))    //如果是文件夹
            {
                foreach (string file in Directory.GetFileSystemEntries(tarPath, "*.*"))
                {
                    removeSpecifiedFiles(file, fileType);
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
