using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RefreshPTPrj
{
    class CRefreshPrj
    {
        static string currentDirectory = Directory.GetCurrentDirectory();
        static string incFilesRoot = currentDirectory + "/inc";
        static string srcFilesRoot = currentDirectory + "/src";
        static string vcxprojPath = currentDirectory + "/prj/PELib/vs15/PELib.vcxproj";
        static string vcxprojFilterPath = currentDirectory + "/prj/PELib/vs15/PELib.vcxproj.filters";

        List<string> vcxprojContent = new List<string>();
        List<string> vcxprojFilterContent = new List<string>();
        List<string> incFiles = new List<string>();
        List<string> srcFiles = new List<string>();

        public void Start()
        {
            incFiles.AddRange(Directory.GetFiles(incFilesRoot, "*.h", SearchOption.AllDirectories));
            srcFiles.AddRange(Directory.GetFiles(srcFilesRoot, "*.cc", SearchOption.AllDirectories));
            srcFiles.AddRange(Directory.GetFiles(srcFilesRoot, "*.cpp", SearchOption.AllDirectories));

            ReadVcxproj();
            ReadVcxprojFilters();

            RefreshVcxproj();
            RefreshVcxprojFilter();

            vcxprojContent.Clear();
            vcxprojFilterContent.Clear();
            incFiles.Clear();
            srcFiles.Clear();
        }

        public void RefreshVcxproj()
        {
            FileStream fileStream = new FileStream(vcxprojPath, FileMode.Create);
            var utf8WithBom = new UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fileStream, utf8WithBom);

            bool write = true;
            int itemGroupIndex = 1;
            for (int i = 0; i < vcxprojContent.Count; i++)
            {
                if (vcxprojContent[i] == "  <ItemGroup>")
                {
                    sw.WriteLine(vcxprojContent[i]);
                    if (itemGroupIndex == 1)
                    {
                        foreach (var f in incFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\inc\\");
                            string line = "    <ClInclude Include=\"..\\..\\.." + file.Substring(subIndex) + "\" />";
                            sw.WriteLine(line);
                        }
                    }
                    else if (itemGroupIndex == 2)
                    {
                        foreach (var f in srcFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\src\\");
                            string line = "    <ClCompile Include=\"..\\..\\.." + file.Substring(subIndex) + "\" />";
                            sw.WriteLine(line);
                        }
                    }
                    itemGroupIndex++;
                    write = false;
                }
                else if (vcxprojContent[i] == "  </ItemGroup>")
                {
                    sw.WriteLine(vcxprojContent[i]);
                    write = true;
                }
                else if (write)
                {
                    sw.WriteLine(vcxprojContent[i]);
                }
            }
            sw.Close();
            fileStream.Close();
        }

        private void RefreshVcxprojFilter()
        {
            FileStream fileStream = new FileStream(vcxprojFilterPath, FileMode.Create);
            var utf8WithBom = new UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fileStream, utf8WithBom);

            bool write = true;
            int itemGroupIndex = 0;
            for (int i = 0; i < vcxprojFilterContent.Count; i++)
            {
                if (vcxprojFilterContent[i] == "  <ItemGroup>")
                {
                    sw.WriteLine(vcxprojFilterContent[i]);
                    if (itemGroupIndex == 0)
                    {
                        itemGroupIndex++;
                        continue;
                    }
                    else if (itemGroupIndex == 1)
                    {
                        foreach (var f in incFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\inc\\");
                            string line = "    <ClInclude Include=\"..\\..\\.." + file.Substring(subIndex) + "\">";
                            sw.WriteLine(line);
                            if (file.Contains("\\protoext\\"))
                            {
                                line = "      <Filter>inc\\protoext</Filter>";
                            }
                            else if (file.Contains("\\commonDefine\\"))
                            {
                                line = "      <Filter>inc\\commonDefine</Filter>";
                            }
                            sw.WriteLine(line);
                            sw.WriteLine("    </ClInclude>");
                        }
                    }
                    else if (itemGroupIndex == 2)
                    {
                        foreach (var f in srcFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\src\\");
                            string line = "    <ClCompile Include=\"..\\..\\.." + file.Substring(subIndex) + "\">";
                            sw.WriteLine(line);
                            if (file.Contains("\\protoext\\"))
                            {
                                line = "      <Filter>src\\protoext</Filter>";
                            }
                            else if (file.Contains("\\commonDefine\\"))
                            {
                                line = "      <Filter>src\\commonDefine</Filter>";
                            }
                            sw.WriteLine(line);
                            sw.WriteLine("    </ClCompile>");
                        }
                    }
                    itemGroupIndex++;
                    write = false;
                }
                else if (vcxprojFilterContent[i] == "  </ItemGroup>")
                {
                    sw.WriteLine(vcxprojFilterContent[i]);
                    write = true;
                }
                else if (write)
                {
                    sw.WriteLine(vcxprojFilterContent[i]);
                }
            }
            sw.Close();
            fileStream.Close();
        }


        /// <summary>
        /// 读取.vcxproj内容
        /// </summary>
        public void ReadVcxproj()
        {
            vcxprojContent = ReadFile(vcxprojPath);
            File.Delete(vcxprojPath);
        }

        /// <summary>
        /// 读取.vcxproj.filters内容
        /// </summary>
        public void ReadVcxprojFilters()
        {
            vcxprojFilterContent = ReadFile(vcxprojFilterPath);
            File.Delete(vcxprojFilterPath);
        }

        public List<string> ReadFile(string path)
        {
            List<string> content = new List<string>();
            string[] files = File.ReadAllLines(path);
            content.AddRange(files);
            //             foreach (var file in files)
            //             {
            //                 content.Add(file.Replace('/','\\'));
            //             }
            return content;
        }
    }
}
