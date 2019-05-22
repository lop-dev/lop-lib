using System;
using System.IO;

namespace Proto2Code
{
    internal class CProtoFileGen
    {
        public static void Generate(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("请输入inputDir和outputDir和生成类型");
                return;
            }
            string inputDir = args[0];
            if (!Directory.Exists(inputDir))
            {
                Console.WriteLine(string.Format("Directory: {0} not exist", inputDir));
                return;
            }
            string outputDir = args[1];
            if (Directory.Exists(outputDir))
            {
                Directory.Delete(outputDir, true);
            }
            Directory.CreateDirectory(outputDir);

            string createType = args[2];
            string[] type = createType.Split('_');
            for (int i = 0; i < type.Length; i++)
            {
                createType = type[i];
                //Console.WriteLine(createType);
                string writePathRoot = outputDir + "/";
                if (createType == "lua")
                {
                    CGenerateLua.Process(inputDir, writePathRoot);
                }
                else if (createType == "cpp")
                {
                    CGenerateCpp.Process(inputDir, writePathRoot);
                }
                else if(createType == "cs" )
                {
                    CGenerateCsp.Process(inputDir, writePathRoot);
                }
                else
                {
                    Console.WriteLine("请检查输入生成类型是否正确");
                    return;
                }
            }
        }
    }
}