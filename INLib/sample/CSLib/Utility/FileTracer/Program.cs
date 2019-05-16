using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace FileTracer
{
    class Program
    {
        static void TestTracer()
        {
            int i = 0;
            while(true) {
                CSLib.Utility.CTracer.SysDebug("Test File 1_{0}", i);
                if(i % 10 == 0) {
                    System.Threading.Thread.Sleep(1000);
                }
                i++;
            }
        }
        static void Main(string[] args)
        {
            CSLib.Utility.CFileTracer fileTracer = new CSLib.Utility.CFileTracer();

            fileTracer.SetLevel((ushort)CSLib.Utility.ETracerType.ALL, CSLib.Utility.ETracerLevel.DEBUG);

            fileTracer.SetON((ushort)CSLib.Utility.ETracerType.ALL);

            
            fileTracer.SetOutFile("test.log", CSLib.Utility.EFileLogTime.MINUTE);

            CSLib.Utility.CTracer.SysTracer = fileTracer;

            System.Threading.Thread thread = new System.Threading.Thread(TestTracer);
            thread.Start();

            int i=0;
            while (true)
            {
                CSLib.Utility.CTracer.SysDebug("Test File 2_{0}", i);
                if (i % 10 == 0)
                {
                    System.Threading.Thread.Sleep(1000);
                }
                i++;
            }
        }
    }
}
