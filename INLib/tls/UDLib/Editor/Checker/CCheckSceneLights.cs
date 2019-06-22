using System;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;
namespace UDLib.Editor
{
    public class CCheckSceneLights
    {

        private const string notice1 = @"-----------------------------------说明-----------------------------------
输出所有场景的灯光总数";
        private const string notice2 = @"-----------------------------------说明-----------------------------------
输出选中场景的灯光总数" + (bShowDetailSwitch ? "和各个lights的数据" : "");
        private const bool bShowDetailSwitch = false;    //是否输出灯光详细数据
        private const string fileName = "场景灯光总数";

        private static bool bShowDetail = false;
        private static string detailInfo;


        [MenuItem("Assets/美术/检查工具/检查所有场景的灯光数", false, 03030081)]
        [MenuItem("美术/检查工具/检查所有场景的灯光数", false, 03030081)]
        public static void CheckAllScenesLights()
        {
            Debug.Log(notice1);
            bShowDetail = false;
            CCheckTools.ClearLogList();
            int fileCount;

            string[] files = Directory.GetFiles(CPath.AssetsAppArtScenes, "*.unity", SearchOption.AllDirectories);
            fileCount = files.Length;
            int index = 0;
            foreach (string f in files)
            {
                CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
                index++;
                CheckSceneLights(f);
            }
            CCheckTools.ClearProgressBar();
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice1);
        }

        [MenuItem("Assets/美术/检查工具/检查选中场景的灯光数", false, 03030081)]
        [MenuItem("美术/检查工具/检查选中场景的灯光数", false, 03030081)]
        public static void CheckSelectedScenesLights()
        {
            bShowDetail = true;
            CCheckTools.ClearLogList();
            int curIndex = 0;
            string[] files = null;
            string defaultPath = "";
            CCheckTools.GetSelectedFilePath(ref files, ref defaultPath, "*.unity");
            if (files != null && files.Length > 0)
            {
                int fileCount = files.Length;
                foreach (string s in files)
                {
                    curIndex++;
                    CCheckTools.DisplayProgressBar("检查中", s, curIndex / (float)fileCount);
                    CheckSceneLights(s);
                }
                CCheckTools.ClearProgressBar();
            }
            else if (!string.IsNullOrEmpty(defaultPath))
            {
                CheckSceneLights(defaultPath);
            }
            else
            {
                Debug.LogError("未选择任何文件");
            }
            CCheckTools.LogAll();
            CCheckTools.WriteAllInfoToFile(fileName, notice2);
        }

        private static int mode_realTime;
        private static int mode_baked;
        private static int mode_mixed;
        private static void CheckSceneLights(string path)
        {
            if (!File.Exists(path) || !path.EndsWith(".unity"))
            {
                return;
            }
            mode_realTime = 0;
            mode_baked = 0;
            mode_mixed = 0;
            detailInfo = "";

            path = path.Replace('\\', '/');
            Scene scene = EditorSceneManager.OpenScene(path, OpenSceneMode.Single);

            int totalcount = 0;
            foreach (GameObject root in scene.GetRootGameObjects())
            {
                totalcount += GetChildTris(root);
            }

            path = path.Substring(0, path.Length - ".unity".Length);
            CCheckTools.AddLogInfo("场景“" + path + "”灯光总数：" + totalcount + "（Baked：" + mode_baked + " RealTime：" + mode_realTime + " Mixed：" + mode_mixed + "）");
            if (string.IsNullOrEmpty(detailInfo))
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
            Light[] lights = root.GetComponentsInChildren<Light>();
            int tcount = 0;

            foreach (Light l in lights)
            {
                if (null == l)
                    continue;
                tcount++;
                if (l.lightmapBakeType == LightmapBakeType.Baked)
                {
                    mode_baked++;
                }
                else if (l.lightmapBakeType == LightmapBakeType.Realtime)
                {
                    mode_realTime++;
                }
                else if (l.lightmapBakeType == LightmapBakeType.Mixed)
                {
                    mode_mixed++;
                }
                if (bShowDetailSwitch && bShowDetail)
                {
                    string name = l.transform.name + "->" + l.name;
                    Transform temp = l.transform;
                    while (temp.parent != null)
                    {
                        temp = temp.parent;
                        name = temp.name + "/" + name;
                    }
                    string lightInfo = string.Format("Light:{0}\nType:{1};Shadow Type:{2};CullingMask:{3}", name, l.type, l.shadows, l.cullingMask);
                    detailInfo = detailInfo + lightInfo + Environment.NewLine;
                }
            }
            return tcount;

        }
    }
}

