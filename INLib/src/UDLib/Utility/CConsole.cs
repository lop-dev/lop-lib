using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace UDLib.Utility
{
    public class CConsole
    {
        // 为解决 AOT 问题，为 WIN 而生
        // 如果要做给 U3D 用的版本，需要把函数全部注释掉，并且打开对应 U3D 的函数
        //public static void WriteLine(object value)
        //{
        //    Console.WriteLine(value);
        //}

        //public static void WriteLine(string value)
        //{
        //    Console.WriteLine(value);
        //}

        //public static void WriteLine(CSLib.Utility.CDebugOut.LEVEL eLevel, object value)
        //{
        //    if (eLevel == CDebugOut.LEVEL.WARNING)
        //    {
        //        Console.ForegroundColor = ConsoleColor.Yellow;
        //    }
        //    else if (eLevel == CDebugOut.LEVEL.ERROR)
        //    {
        //        Console.ForegroundColor = ConsoleColor.Red;
        //    }
        //    else if (eLevel == CDebugOut.LEVEL.EXCEPTION)
        //    {
        //        Console.ForegroundColor = ConsoleColor.DarkRed;
        //    }
        //    Console.WriteLine(value);
        //    Console.ResetColor();
        //}

        // 为解决 AOT 问题，为 U3D 而生
        // 如果要做给 WIN 用的版本，需要把函数全部注释掉，并且打开对应 WIN 的函数
        public static void WriteLine(object value)
        {
            Debug.Log(value);
        }

        public static void WriteLine(string value)
        {
            Debug.Log(value);
        }

        public static void WriteLine(CSLib.Utility.CDebugOut.LEVEL eLevel, object value)
        {
            if (eLevel == CSLib.Utility.CDebugOut.LEVEL.WARNING)
            {
                Debug.LogWarning(value);
            }
            else if (eLevel == CSLib.Utility.CDebugOut.LEVEL.ERROR)
            {
                Debug.LogError(value);
            }
            else if (eLevel == CSLib.Utility.CDebugOut.LEVEL.EXCEPTION)
            {
                Debug.LogError(value);
            }
            else
            {
                Debug.Log(value);
            }
        }
    }
}
