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
        static string m_currentDirectory = Directory.GetCurrentDirectory();
        static string m_incFilesRoot = m_currentDirectory + "/inc/";
        static string m_srcFilesRoot = m_currentDirectory + "/src/";
        static string m_vcxprojPath = m_currentDirectory + "/prj/DTLib/vs15/DTLib.vcxproj";
        static string m_vcxprojFilterPath = m_currentDirectory + "/prj/DTLib/vs15/DTLib.vcxproj.filters";

        List<string> m_vcxprojContent = new List<string>();
        List<string> m_vcxprojFilterContent = new List<string>();
        List<string> m_incFiles = new List<string>();
        List<string> m_srcFiles = new List<string>();

        public void Start()
        {
            m_incFiles.AddRange(Directory.GetFiles(m_incFilesRoot, "*.h", SearchOption.AllDirectories));
            m_srcFiles.AddRange(Directory.GetFiles(m_srcFilesRoot, "*.cc", SearchOption.AllDirectories));
            m_srcFiles.AddRange(Directory.GetFiles(m_srcFilesRoot, "*.cpp", SearchOption.AllDirectories));

            ReadVcxproj();
            ReadVcxprojFilters();

            RefreshVcxproj();
            RefreshVcxprojFilter();

            m_vcxprojContent.Clear();
            m_vcxprojFilterContent.Clear();
            m_incFiles.Clear();
            m_srcFiles.Clear();
        }

        public void RefreshVcxproj()
        {
            FileStream fileStream = new FileStream(m_vcxprojPath, FileMode.Create);
            var utf8WithBom = new UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fileStream, utf8WithBom);

            bool write = true;
            int itemGroupIndex = 1;
            for (int i = 0; i < m_vcxprojContent.Count; i++)
            {
                if (m_vcxprojContent[i] == "  <ItemGroup>")
                {
                    sw.WriteLine(m_vcxprojContent[i]);
                    if (itemGroupIndex == 1)
                    {
                        foreach (var f in m_incFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\inc\\");
                            string line = "    <ClInclude Include=\"..\\..\\.." + file.Substring(subIndex) + "\" />";
                            sw.WriteLine(line);
                        }
                    }
                    else if (itemGroupIndex == 2)
                    {
                        foreach (var f in m_srcFiles)
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
                else if (m_vcxprojContent[i] == "  </ItemGroup>")
                {
                    sw.WriteLine(m_vcxprojContent[i]);
                    write = true;
                }
                else if (write)
                {
                    sw.WriteLine(m_vcxprojContent[i]);
                }
            }
            sw.Close();
            fileStream.Close();
        }

        private void RefreshVcxprojFilter()
        {
            FileStream fileStream = new FileStream(m_vcxprojFilterPath, FileMode.Create);
            var utf8WithBom = new UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fileStream, utf8WithBom);

            bool write = true;
            int itemGroupIndex = 1;
            for (int i = 0; i < m_vcxprojFilterContent.Count; i++)
            {
                if (m_vcxprojFilterContent[i] == "  <ItemGroup>")
                {
                    sw.WriteLine(m_vcxprojFilterContent[i]);
                    if (itemGroupIndex == 2)
                    {
                        foreach (var f in m_incFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\inc\\");
                            string line = "    <ClInclude Include=\"..\\..\\.." + file.Substring(subIndex) + "\">";
                            sw.WriteLine(line);
                            if (file.Contains("\\commonDefine\\"))
                            {
                                line = "      <Filter>inc\\commonDefine</Filter>";
                            }
                            else if (file.Contains("\\databaseTask\\"))
                            {
                                line = "      <Filter>inc\\databaseTask</Filter>";
                            }
                            sw.WriteLine(line);
                            sw.WriteLine("    </ClInclude>");
                        }
                    }
                    else if (itemGroupIndex == 1)
                    {
                        foreach (var f in m_srcFiles)
                        {
                            string file = f.Replace('/', '\\');
                            int subIndex = file.LastIndexOf("\\src\\");
                            string line = "    <ClCompile Include=\"..\\..\\.." + file.Substring(subIndex) + "\">";
                            sw.WriteLine(line);
                            if (file.Contains("\\commonDefine\\"))
                            {
                                line = "      <Filter>src\\commonDefine</Filter>";
                            }
                            else if (file.Contains("\\databaseTask\\"))
                            {
                                line = "      <Filter>src\\databaseTask</Filter>";
                            }
                            sw.WriteLine(line);
                            sw.WriteLine("    </ClCompile>");
                        }
                    }
                    else if(itemGroupIndex == 3)
                    {
                        while (m_vcxprojFilterContent[++i]!= "  </ItemGroup>")
                        {
                            sw.WriteLine(m_vcxprojFilterContent[i]);
                        }
                        i--;
                    }
                    itemGroupIndex++;
                    write = false;
                }
                else if (m_vcxprojFilterContent[i] == "  </ItemGroup>")
                {
                    sw.WriteLine(m_vcxprojFilterContent[i]);
                    write = true;
                }
                else if (write)
                {
                    sw.WriteLine(m_vcxprojFilterContent[i]);
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
            m_vcxprojContent = ReadFile(m_vcxprojPath);
            File.Delete(m_vcxprojPath);
        }

        /// <summary>
        /// 读取.vcxproj.filters内容
        /// </summary>
        public void ReadVcxprojFilters()
        {
            m_vcxprojFilterContent = ReadFile(m_vcxprojFilterPath);
            File.Delete(m_vcxprojFilterPath);
        }

        public List<string> ReadFile(string path)
        {
            List<string> content = new List<string>();
            string[] files = File.ReadAllLines(path);
            content.AddRange(files);
            return content;
        }
    }
}
