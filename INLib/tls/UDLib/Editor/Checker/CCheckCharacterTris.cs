using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.Animations;
using UnityEngine;
namespace UDLib.Editor
{
    public class CCheckCharacterTris
    {
        private const string notice1 = @"-----------------------------------说明-----------------------------------
输出所有模型的三角面数";
        private const string notice2 = @"-----------------------------------说明-----------------------------------
输出选中模型所有子节点的三角面数和场景的三角面数";
        private const string fileName = "角色三角面数";

        //private static string defaultPathRoot = "Assets/App/Art/Character";
        private static bool bShowDetail;
        private static string[] ignoreStr = { "Animation" };
        private static string detailInfo;

        [MenuItem("Assets/美术/检查工具/检查所有角色的三角面数", false, 03030061)]
        [MenuItem("美术/检查工具/检查所有角色的三角面数", false, 03030061)]
        public static void CheckAllCharacterTris()
        {
            bShowDetail = false;
            CCheckTools.ClearLogList();

            string[] files = Directory.GetFiles(CPath.AssetsAppArtCharacter, "*.FBX", SearchOption.AllDirectories);
            foreach (string f in files)
            {
                CheckCharacterTris(f);
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中角色的三角面数", false, 03030061)]
        [MenuItem("美术/检查工具/检查选中角色的三角面数", false, 03030061)]
        public static void CheckSelectedCharacterTris()
        {
            bShowDetail = true;
            CCheckTools.ClearLogList();
            string[] filePath = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath, "*.FBX");
            if (filePath != null && filePath.Length > 0)
            {
                foreach (string s in filePath)
                {
                    CheckCharacterTris(s);
                }
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckCharacterTris(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice2);
        }

        private static void CheckCharacterTris(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".FBX"))
                return;
            detailInfo = "";
            path = path.Replace('\\', '/');
            if (Ignore(path))
                return;
            GameObject rootGO = AssetDatabase.LoadAssetAtPath<UnityEngine.Object>(path) as GameObject;

            path = path.Substring(0, path.Length - ".FBX".Length);
            CCheckTools.AddLogInfo("模型“" + path + "”三角面总数：" + GetChildTris(rootGO));
            if (!string.IsNullOrEmpty(detailInfo))
            {
                CCheckTools.AddLogInfo(detailInfo);
            }
        }

        private static int GetChildTris(GameObject root)
        {
            if (null == root)
                return 0;
            if (!root.activeSelf)
                return 0;
            MeshFilter[] mf = root.GetComponentsInChildren<MeshFilter>();
            SkinnedMeshRenderer[] smr = root.GetComponentsInChildren<SkinnedMeshRenderer>();

            int tcount = 0;
            foreach (var v in mf)
            {
                if (v != null && v.sharedMesh != null && v.transform.GetComponent<MeshRenderer>() != null)
                {
                    tcount += v.sharedMesh.triangles.Length / 3;
                    if (bShowDetail && v.sharedMesh.triangles.Length > 0)
                    {
                        detailInfo = detailInfo + v.name + ":" + v.sharedMesh.triangles.Length / 3 + '\n';
                    }
                }
            }
            foreach (var v in smr)
            {
                if (v != null && v.sharedMesh != null)
                {
                    tcount += v.sharedMesh.triangles.Length / 3;
                    if (bShowDetail && v.sharedMesh.triangles.Length > 0)
                    {
                        detailInfo = detailInfo + v.name + ":" + v.sharedMesh.triangles.Length / 3 + '\n';
                    }
                }
            }
            return tcount;
        }

        private static bool Ignore(string path)
        {
            //path = path.Replace('\\','/');
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

