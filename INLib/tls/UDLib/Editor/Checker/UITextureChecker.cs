using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using UDLib.Editor;
using UnityEditor;
using UnityEngine;

public class TextureInfo
{
    public string imgName;
    public List<imageInfo> imgList = new List<imageInfo>();


    public class imageInfo
    {
        public string UUID;
        public List<string> prefab = new List<string>();
    }
}

public class TextureRef
{
    public string uuid;
    public int refCount;
}

public static class UITextureChecker
{
    static List<string> getUUIDsInFile(string path)
    {
        StreamReader file = new StreamReader(path);
        List<string> uuids = new List<string>();
        string line;
        while ((line = file.ReadLine()) != null)
        {
            var reg = new Regex(@"([a-f0-9]{32})");
            var m = reg.Match(line);
            if (m.Success)
            {
                uuids.Add(m.Groups[0].Value);
            }
        }
        file.Close();
        return uuids;
    }

    static List<TextureInfo> GetAllDataList(string uiPicRootDir, string prefabRoot)
    {
        EditorUtility.ClearProgressBar();

        if (string.IsNullOrEmpty(prefabRoot))
        {
            return null;
        }

        if (string.IsNullOrEmpty(uiPicRootDir))
        {
            return null;
        }

        //获取所有纹理的uuid
        var pngs = Directory.GetFiles(uiPicRootDir, "*.meta", SearchOption.AllDirectories)
            .Select(p => "Assets/" + p.Replace('\\', '/').Substring(Application.dataPath.Length + 1))
            .Where(p => p.EndsWith(".png.meta")).ToList();

        var imgUuid2path = new Dictionary<string, string>();

        List<TextureInfo> list = new List<TextureInfo>();

        var uuidReg = new Regex(@"guid: ([a-f0-9]{32})");

        for (var i = 0; i < pngs.Count; ++i)
        {
            var png = pngs[i];

            var matcher = uuidReg.Match(File.ReadAllText(png));
            var uuid = matcher.Groups[1].Value;
            if (imgUuid2path.ContainsKey(uuid))
            {
                Debug.LogError("uuid dup" + uuid + " \n" + png + "\n" + imgUuid2path[uuid]);
            }
            else
            {
                string _path = png.Substring(0, png.Length - 5).Replace("Assets/App/Pro/GameRes/UI/Module/", "");
                imgUuid2path.Add(uuid, _path);

                if (!_path.Contains("config") && !_path.Contains("common") && !_path.Contains("timeline"))
                {
                    string fileName = Path.GetFileNameWithoutExtension(_path);

                    var t = list.Find(l => l.imgName == fileName);
                    if (t == null)
                    {
                        t = new TextureInfo { imgName = fileName };
                        list.Add(t);
                    }

                    TextureInfo.imageInfo img = new TextureInfo.imageInfo { UUID = uuid };

                    t.imgList.Add(img);
                }
            }
            if (EditorUtility.DisplayCancelableProgressBar("扫描图片中", Path.GetFileNameWithoutExtension(png),
                (float)i / pngs.Count))
            {
                EditorUtility.ClearProgressBar();
                return null;
            }
        }


        EditorUtility.ClearProgressBar();

        var prefabs = Directory.GetFiles(prefabRoot, "*.prefab", SearchOption.AllDirectories).ToList();

        for (var i = 0; i < prefabs.Count; i++)
        {
            var prefab = prefabs[i];

            if (EditorUtility.DisplayCancelableProgressBar("获取引用关系", Path.GetFileNameWithoutExtension(prefab),
                (float)i / prefabs.Count))
            {
                EditorUtility.ClearProgressBar();
                return null;
            }

            var uuids = getUUIDsInFile(prefab);

            var fullPath = prefab;

            prefab = prefab.Replace('\\', '/').Substring(prefabRoot.Length + 1);

            var moduleName = prefab.Remove(prefab.IndexOf(Path.AltDirectorySeparatorChar));
            moduleName = moduleName.ToLower();

            foreach (var uuid in uuids)
            {
                //是图片
                if (imgUuid2path.ContainsKey(uuid))
                {
                    var imgPath = imgUuid2path[uuid];
                    if (!imgPath.Contains("config") && !imgPath.Contains("common") && !imgPath.Contains("timeline"))
                    {
                        var fileName = Path.GetFileNameWithoutExtension(imgPath);

                        var tex = list.Find(l => l.imgName.Equals(fileName));

                        if (tex == null)
                        {
                            Debug.LogError("Error");
                        }

                        var uuid1 = uuid;
                        var img = tex.imgList.Find(im => im.UUID == uuid1);
                        if (img == null)
                        {
                            img = new TextureInfo.imageInfo { UUID = uuid1 };
                        }

                        if (!img.prefab.Contains(fullPath))
                            img.prefab.Add(fullPath);
                    }
                }
            }
        }

        EditorUtility.ClearProgressBar();

        return list;
    }

    public static void CheckUnusedTexture(string uiRoot, string prefabRoot)
    {
        List<TextureInfo> list = GetAllDataList(uiRoot, prefabRoot);

        if (list == null)
        {
            EditorUtility.DisplayDialog("提示", "操作已取消", "确定");
            return;
        }

        List<string> unUsedGuid = new List<string>();
        StringBuilder sb = new StringBuilder();

        foreach (TextureInfo textureInfo in list)
        {
            var imgList = textureInfo.imgList;
            foreach (var imgInfo in imgList)
            {
                if (imgInfo.prefab.Count == 0)
                {
                    sb.Append(AssetDatabase.GUIDToAssetPath(imgInfo.UUID)).Append(Environment.NewLine);
                    unUsedGuid.Add(imgInfo.UUID);
                }
            }
        }


        string title = "分析完毕";
        string msg = "共发现" + unUsedGuid.Count + "个未使用的资源，是否删除？";
        string ok = "查看并删除";
        string cancel = "查看";

        if (unUsedGuid.Count == 0)
        {
            msg = "未发现未使用的资源，太棒了！";
            ok = "赞";
            cancel = "赞";

            EditorUtility.DisplayDialog(title, msg, ok, cancel);
            return;
        }

        bool option = EditorUtility.DisplayDialog(title, msg, ok, cancel);
        {
            string retName = "fileList.txt";
            string filePath = Path.Combine(Application.temporaryCachePath, retName);
            if (File.Exists(filePath))
            {
                File.Delete(filePath);
            }

            File.WriteAllText(filePath, sb.ToString());
            System.Diagnostics.Process.Start(filePath);
        }

        if (option)
        {
            for (var i = 0; i < unUsedGuid.Count; i++)
            {
                string path = AssetDatabase.GUIDToAssetPath(unUsedGuid[i]);

                if (EditorUtility.DisplayCancelableProgressBar("删除中", path,
                    (float)i / unUsedGuid.Count))
                {
                    EditorUtility.ClearProgressBar();
                    return ;
                }

                bool success = AssetDatabase.DeleteAsset(path);
                if (!success)
                {
                    Debug.LogWarning("failed to remove " + path);
                }
            }

            EditorUtility.ClearProgressBar();
        }
    }

    public static void CheckRepeatUsedTexture(string uiRoot, string prefabRoot)
    {
        List<TextureInfo> list = GetAllDataList(uiRoot, prefabRoot);

        if (list == null)
        {
            EditorUtility.DisplayDialog("提示", "操作已取消", "确定");
            return;
        }

        List<TextureInfo> commonUsed = new List<TextureInfo>();
        for (var i = 0; i < list.Count; i++)
        {
            if (list[i].imgList.Count >= 3)
            {
                commonUsed.Add(list[i]);
            }
        }

        commonUsed.Sort((a,b) => -a.imgList.Count.CompareTo(b.imgList.Count));

        int count = commonUsed.Count;
        string title = "提示";
        string msg = "";
        string ok = "";
        string cancel = "";
        if (count == 0)
        {
            msg = "未找到";
            ok = "确定";
            EditorUtility.DisplayDialog(title, msg, ok);
            return;
        }

        msg = "共找到" + count + "个重复使用>=3次的纹理，是否挪到common中？";
        ok = "查看并移动";
        cancel = "查看";

        StringBuilder sb = new StringBuilder();
        for (var i = 0; i < commonUsed.Count; i++)
        {
            sb.Append(commonUsed[i].imgName).Append(Environment.NewLine);

            for (var i1 = 0; i1 < commonUsed[i].imgList.Count; i1++)
            {
                var img = commonUsed[i].imgList[i1];
                string _path = AssetDatabase.GUIDToAssetPath(img.UUID);

                int index = _path.IndexOf("Module", StringComparison.Ordinal);
                if (-1 != index)
                {
                    _path = _path.Substring(index + 7);
                }
                sb.Append("        ").Append(_path).Append(Environment.NewLine);

                for (var i2 = 0; i2 < img.prefab.Count; i2++)
                {
                    sb.Append("                ").Append(img.prefab[i2]).Append(Environment.NewLine);
                }
            }

            sb.Append(Environment.NewLine).Append(Environment.NewLine).Append(Environment.NewLine)
                .Append(Environment.NewLine);
        }

        bool ret = EditorUtility.DisplayDialog(title, msg, ok, cancel);
        if (ret)
        {
            for (var i = 0; i < commonUsed.Count; i++)
            {
                EditorUtility.DisplayCancelableProgressBar("移动中", commonUsed[i].imgName, (float) i / commonUsed.Count);

                string newGuid = "";

                for (var i1 = 0; i1 < commonUsed[i].imgList.Count; i1++)
                {

                    string oldPath = "";
                    string guid = commonUsed[i].imgList[i1].UUID;
                    string path = AssetDatabase.GUIDToAssetPath(guid);
                    oldPath = path;

                    //先把第一个移动到common里
                    if (0 == i1)
                    {
                        string newPath = "";

                        string fileName = Path.GetFileName(path);
                        int index = path.IndexOf("Module", StringComparison.Ordinal);
                        string _path = path.Remove(index + 7);

                        newPath = _path + "common/" + fileName;

                        string result = AssetDatabase.MoveAsset(oldPath, newPath);

                        AssetDatabase.Refresh();

                        if (!string.IsNullOrEmpty(result))
                        {
                            Debug.LogError(string.Format("move asset from {0} to {1} error", oldPath, newPath));
                        }
                        else
                        {
                            newGuid = AssetDatabase.AssetPathToGUID(newPath);
                        }
                    }
                    //其他的则删除
                    else
                    {
                        AssetDatabase.DeleteAsset(oldPath);
                        AssetDatabase.Refresh();
                    }
                }

                //处理prefab文件
                for (var i1 = 0; i1 < commonUsed[i].imgList.Count; i1++)
                {
                    var img = commonUsed[i].imgList[i1];
                    for (var i2 = 0; i2 < img.prefab.Count; i2++)
                    {
                        var prefab = img.prefab[i2];
                        ReplaceGuid(prefab, img.UUID, newGuid);
                    }
                }
                AssetDatabase.Refresh();
            }

            EditorUtility.ClearProgressBar();
        }

        
        string retName = "fileList.txt";
        string filePath = Path.Combine(Application.temporaryCachePath, retName);
        if (File.Exists(filePath))
        {
            File.Delete(filePath);
        }

        File.WriteAllText(filePath, sb.ToString());
        System.Diagnostics.Process.Start(filePath);
    }

    private static void ReplaceGuid(string path, string oldGuid, string newGuid)
    {
        if (!File.Exists(path))
        {
            Debug.LogError("file not exist " + path);
            return;
        }

        if (string.IsNullOrEmpty(oldGuid) || string.IsNullOrEmpty(newGuid))
            return;

        if (oldGuid.Equals(newGuid))
            return;

        StreamReader sr = new StreamReader(path);
        string content = sr.ReadToEnd();
        sr.Close();

        var replace = content.Replace(oldGuid, newGuid);
        StreamWriter sw = new StreamWriter(path, false);
        sw.WriteLine(replace);
        sw.Close();
    }


    public static void CheckCommonFiles()
    {
        string commonUIPath = Application.dataPath + "/App/Pro/GameRes/UI/Module/common";
        string prefabPath = Application.dataPath + "/App/Pro/GameRes/UIPrefabs";

        var pngs = Directory.GetFiles(commonUIPath, "*.meta", SearchOption.AllDirectories)
            .Select(p => "Assets/" + p.Replace('\\', '/').Substring(Application.dataPath.Length + 1))
            .Where(p => p.EndsWith(".png.meta")).ToList();

        var imgGuid2path = new Dictionary<string, string>();

        var uuidReg = new Regex(@"guid: ([a-f0-9]{32})");

        EditorUtility.ClearProgressBar();

        for (var i = 0; i < pngs.Count; ++i)
        {
            var png = pngs[i];

            var matcher = uuidReg.Match(File.ReadAllText(png));
            var guid = matcher.Groups[1].Value;
            if (imgGuid2path.ContainsKey(guid))
            {
                Debug.LogError("uuid dup" + guid + " \n" + png + "\n" + imgGuid2path[guid]);
            }
            else
            {
                string _path = png.Substring(0, png.Length - 5).Replace("Assets/App/Pro/GameRes/UI/Module/", "");
                imgGuid2path.Add(guid, _path);
            }

            EditorUtility.DisplayProgressBar("扫描图片中", Path.GetFileNameWithoutExtension(png),
                (float) i / pngs.Count);
        }

        EditorUtility.ClearProgressBar();

        var prefabs = Directory.GetFiles(prefabPath, "*.prefab", SearchOption.AllDirectories).ToList();

        Dictionary<string, int> guidToNum = new Dictionary<string, int>();

        prefabs.ForEachI((prefab, i) =>
        {
            EditorUtility.DisplayProgressBar("分析prefab", Path.GetFileNameWithoutExtension(prefab), (float)i/ prefabs.Count);

            var guids = getUUIDsInFile(prefab);
            foreach (var guid in guids)
            {
                if (imgGuid2path.ContainsKey(guid))
                {
                    if (!guidToNum.ContainsKey(guid))
                    {
                        guidToNum.Add(guid, 0);
                    }
                    else
                    {
                        guidToNum[guid]++;
                    }
                }
            }
        });

        EditorUtility.ClearProgressBar();

        List<TextureRef> list = new List<TextureRef>();
        foreach (var keyValuePair in guidToNum)
        {
            TextureRef tex = new TextureRef {refCount = keyValuePair.Value, uuid = keyValuePair.Key};

            list.Add(tex);
        }

        list.Sort((a,b) => a.refCount.CompareTo(b.refCount));

        StringBuilder sb = new StringBuilder();

        list.ForEach(l =>
        {
            sb.Append(l.refCount).Append("    ").Append(AssetDatabase.GUIDToAssetPath(l.uuid)).Append(Environment.NewLine);
        });

        string retName = "fileList.txt";
        string filePath = Path.Combine(Application.temporaryCachePath, retName);
        if (File.Exists(filePath))
        {
            File.Delete(filePath);
        }

        File.WriteAllText(filePath, sb.ToString());
        System.Diagnostics.Process.Start(filePath);
    }
}
