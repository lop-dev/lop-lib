using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckBonesCount
    {
        private const string notice1 = "检查角色骨骼数量，低模50，高模100，超过数量，红字提示。高模低模根据文件名是否“_H”结尾区分";
        private const string fileName = "角色骨骼数量";

        private static string[] ignoreStr = { "Animation" };

        [MenuItem("Assets/美术/检查工具/检查所有角色骨骼数量", false, 03030161)]
        [MenuItem("美术/检查工具/检查所有角色骨骼数量", false, 03030161)]
        public static void CheckAllCharacterBones()
        {
            CCheckTools.ClearLogList();

            string[] files = Directory.GetFiles(CPath.AssetsAppArtCharacter, "*.FBX", SearchOption.AllDirectories);
            int fileCount = files.Length;
            int index = 0;
            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
                index++;
                CheckCharacterBones(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中角色骨骼数量", false, 03030161)]
        [MenuItem("美术/检查工具/检查选中角色骨骼数量", false, 03030161)]
        public static void CheckSelectedCharacterBones()
        {
            CCheckTools.ClearLogList();
            int index = 0;
            int fileCount = 0;
            string[] filePath = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath, "*.FBX");
            if (filePath != null && filePath.Length > 0)
            {
                fileCount = filePath.Length;
                foreach (string s in filePath)
                {
                    index++;
                    CCheckTools.DisplayProgressBar("检查中", s, index / (float)fileCount);
                    CheckCharacterBones(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckCharacterBones(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        private static void CheckCharacterBones(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".FBX"))
                return;
            path = path.Replace('\\', '/');
            if (Ignore(path))
                return;
            GameObject rootGO = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(path) as GameObject;
            if (null == rootGO)
                return;

            path = path.Substring(0, path.Length - ".FBX".Length);
            int totalCount = GetBonesCountByRoot(rootGO);
            if (path.EndsWith("_H"))
            {
                if (totalCount > 100)
                    CCheckTools.AddErrorInfo(path + "->count:" + totalCount);
                else
                    CCheckTools.AddLogInfo(path + "->count:" + totalCount);
            }
            else
            {
                if (totalCount > 50)
                    CCheckTools.AddErrorInfo(path + "->count:" + totalCount);
                else
                    CCheckTools.AddLogInfo(path + "->count:" + totalCount);
            }
        }

        private static int GetBonesCountByRoot(GameObject root)
        {
            if (null == root)
                return 0;
            int count = 0;
            SkinnedMeshRenderer[] smr = root.GetComponentsInChildren<SkinnedMeshRenderer>();
            foreach (var v in smr)
            {
                if (v != null && v.bones != null)
                    count += v.bones.Length;
            }
            return count;
        }

        private static bool Ignore(string path)
        {
            string[] clips = path.Split('/');
            foreach (string str in ignoreStr)
            {
                if (clips.Length >= 3 && clips[clips.Length - 3] == str)
                    return true;
            }
            return false;
        }
    }
}

