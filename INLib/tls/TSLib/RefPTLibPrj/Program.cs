using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RefreshSHLibPrj
{
    class Program
    {
        static void Main(string[] args)
        {
            //D:\liweilong\Shared\ToolKit\RefreshSHLibPrj\RefreshSHLibPrj\bin\Debug
            //string currentPath = Directory.GetCurrentDirectory();
            //string rootPath = new DirectoryInfo("../../../../").FullName;
            string rootPath = Directory.GetCurrentDirectory();
            
            //rootPath = rootPath + "RefreshSHPrj\\SHLib\\";
            string genPath = rootPath + "\\gen\\SHLib\\";
            string prjPath = rootPath + "\\prj\\SHLib\\vs15\\";
            Console.WriteLine(rootPath);
            Console.WriteLine(genPath);
            Console.WriteLine(prjPath);
            

            SHLibManage sHLibManager = new SHLibManage();

            sHLibManager.ReadFiles(genPath);

            sHLibManager.ReadVcxproj(prjPath + "SHLib.vcxproj");
            sHLibManager.showVCXFile(1);
            //sHLibManager.saveVCXFile(prjPath + "原文件备份.vcxproj");
            sHLibManager.ReplaceVCXFile();
            sHLibManager.showVCXFile(2);
            //sHLibManager.saveVCXFile(prjPath + "原文件备份.vcxproj");
            sHLibManager.saveVCXFile(prjPath + "SHLib.vcxproj");


            sHLibManager.ReadVcxproj(prjPath + "SHLib.vcxproj.filters");
            //sHLibManager.saveVCXFliterFile(prjPath + "原文件备份.vcxproj.filters");
            sHLibManager.ReplaceVCXFliterFile();
            //sHLibManager.saveVCXFliterFile(prjPath + "原文件备份.vcxproj.filters");
            sHLibManager.saveVCXFliterFile(prjPath + "SHLib.vcxproj.filters");
            //sHLibManager.showFiles();
            Console.ReadKey();
        }
    }
}
