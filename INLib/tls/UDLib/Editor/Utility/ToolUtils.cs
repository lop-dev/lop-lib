using UnityEngine;
using UnityEditor;
using System.IO;

namespace UDLib.Editor
{
    public class ToolUtils
    {
        // 移除meta文件的只读属性
        public static void RemoveMetaReadOnly(string metaPath)
        {
            var attr = File.GetAttributes(metaPath);
            if ((attr & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
            {
                File.SetAttributes(metaPath, FileAttributes.Normal);
            }
        }
    }
}