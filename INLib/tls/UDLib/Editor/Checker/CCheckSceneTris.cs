using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
namespace UDLib.Editor
{
    public class CCheckSceneTris
    {
        private const string notice1 = @"-----------------------------------说明-----------------------------------
输出所有场景的triangles总数，不包括因为模型材质，光照，摄像机，UI组件产生的triangles";
        private const string notice2 = @"-----------------------------------说明-----------------------------------
输出选中场景所有子节的triangles数量和场景的triangles总数，不包括因为模型材质，光照，摄像机，UI组件产生的triangles";
        private const string fileName = "场景三角面数";
        private static bool bShowDetail;
        private static string detailInfo;

        [MenuItem("Assets/美术/检查工具/检查所有场景的三角面数", false, 03030041)]
        [MenuItem("美术/检查工具/检查所有场景的三角面数", false, 03030041)]
        public static void CheckAllScenesTris()
        {
            Debug.Log(notice1);
            CCheckTools.ClearLogList();
            int fileCount;
            bShowDetail = false;
            string[] files = Directory.GetFiles(CPath.AssetsAppArtScenes, "*.unity", SearchOption.AllDirectories);
            fileCount = files.Length;
            int index = 0;
            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
                index++;
                CheckSceneTris(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中场景的三角面数", false, 03030041)]
        [MenuItem("美术/检查工具/检查选中场景的三角面数", false, 03030041)]
        public static void CheckSelectedScenesTris()
        {
            bShowDetail = true;
            CCheckTools.ClearLogList();
            int index = 0;
            string[] filePath = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref filePath, ref defaultPath, "*.unity");
            if (filePath != null && filePath.Length > 0)
            {
                int fileCount = filePath.Length;
                foreach (string s in filePath)
                {
                    index++;
                    CCheckTools.DisplayProgressBar("检查中", s, index / (float)fileCount);
                    CheckSceneTris(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckSceneTris(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice2);
        }


        //根据路径检查triangles
        private static void CheckSceneTris(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".unity"))
            {
                return;
            }
            detailInfo = "";

            path = path.Replace('\\', '/');
            Scene scene = EditorSceneManager.OpenScene(path, OpenSceneMode.Single);

            int totalcount = 0;
            foreach (GameObject root in scene.GetRootGameObjects())
            {
                totalcount += GetChildTris(root);
            }

            path = path.Substring(0, path.Length - ".unity".Length);
            if (totalcount <= 20000)
            {
                CCheckTools.AddLogInfo("场景“" + path + "”三角面总数：" + totalcount);
                if (!string.IsNullOrEmpty(detailInfo))
                {
                    detailInfo.Remove(detailInfo.Length - 1, 1);
                    CCheckTools.AddLogInfo(detailInfo);
                }
            }
            else if (totalcount > 20000 && totalcount <= 100000)
            {
                CCheckTools.AddWarningInfo("场景“" + path + "”三角面总数：" + totalcount);
                if (!string.IsNullOrEmpty(detailInfo))
                {
                    detailInfo.Remove(detailInfo.Length - 1, 1);
                    CCheckTools.AddWarningInfo(detailInfo);
                }
            }
            else if (totalcount > 100000)
            {
                CCheckTools.AddErrorInfo("场景“" + path + "”三角面总数：" + totalcount);
                if (!string.IsNullOrEmpty(detailInfo))
                {
                    detailInfo.Remove(detailInfo.Length - 1, 1);
                    CCheckTools.AddErrorInfo(detailInfo);
                }
            }
        }

        //获取GameObject所有子节点的triangles总和
        private static int GetChildTris(GameObject root)
        {
            if (root == null)
                return 0;
            if (!root.activeSelf)
                return 0;
            MeshFilter[] mfs = root.GetComponentsInChildren<MeshFilter>();
            SkinnedMeshRenderer[] smrs = root.GetComponentsInChildren<SkinnedMeshRenderer>();

            int tcount = 0;
            foreach (MeshFilter mf in mfs)
            {
                int count = 0;
                if (mf != null && mf.sharedMesh != null && mf.transform.GetComponent<MeshRenderer>() != null)
                {
                    count = mf.sharedMesh.triangles.Length / 3;
                }
                if (bShowDetail && count > 0)
                {
                    detailInfo = detailInfo + mf.transform.name + "_1:" + count + Environment.NewLine;
                }
                tcount += count;
            }
            foreach (SkinnedMeshRenderer smr in smrs)
            {
                int count = 0;
                if (smr != null && smr.sharedMesh != null)
                {
                    count = smr.sharedMesh.triangles.Length / 3;
                }
                if (bShowDetail && count > 0)
                {
                    detailInfo = detailInfo + smr.transform.name + "_2:" + count + Environment.NewLine;
                }
                tcount += count;
            }
            return tcount;
        }
    }
}

