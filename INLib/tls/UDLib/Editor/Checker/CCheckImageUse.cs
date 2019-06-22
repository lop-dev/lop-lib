using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using UnityEditor;
using UnityEngine;
namespace UDLib.Editor
{
    public static class LinqHelper
    {
        public static TSource Fold<TSource>(this IEnumerable<TSource> source, Func<TSource, TSource, TSource> func, TSource id)
        {
            TSource r = id;
            foreach (var s in source)
            {
                r = func(r, s);
            }
            return r;
        }
        public static void ForEach<T>(this IEnumerable<T> source, Action<T> action)
        {
            foreach (T element in source)
                action(element);
        }
        public static IEnumerable<U> SelectI<U, T>(this IEnumerable<T> source, Func<T, int, U> action)
        {
            int i = 0;
            foreach (var s in source)
            {
                yield return action(s, i);
                i += 1;
            }
        }
        public static TSource Reduce<TSource>(this IEnumerable<TSource> source, Func<TSource, TSource, TSource> func) where TSource : new()
        {
            return Fold<TSource>(source, func, new TSource());
        }
        public static void ForEachI<T>(this IEnumerable<T> source, Action<T, int> action)
        {
            int i = 0;
            foreach (T element in source)
            {
                action(element, i);
                i += 1;
            }

        }
    }
    public static class FindUnUnUsedUITexture
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
        // Use this for initialization
        [MenuItem("程序/检查工具/查找UI冗余图片",false,02010000)]
        public static void Scan()
        {

            var uiPrefabRootDir = EditorUtility.OpenFolderPanel("选择UIPrefab目录", "Assets", "");
            if (string.IsNullOrEmpty(uiPrefabRootDir))
            {
                return;
            }

            var uiPicRootDir = EditorUtility.OpenFolderPanel("选择UIImage目录", "Assets", "");
            if (string.IsNullOrEmpty(uiPicRootDir))
            {
                return;
            }

            //find all meta and pic path
            var uuidReg = new Regex(@"guid: ([a-f0-9]{32})");
            var pngs = Directory.GetFiles(uiPicRootDir, "*.meta", SearchOption.AllDirectories)
                .Select(p => "Assets/" + p.Replace('\\', '/').Substring(Application.dataPath.Length + 1))
                .Where(p => {
                    return p.EndsWith(".png.meta") || p.EndsWith(".jpg.meta") || p.EndsWith(".tag.meta");
                }).ToList();
            var uuid2path = new Dictionary<string, string>();
            pngs.ForEachI((png, i) => {
                var matcher = uuidReg.Match(File.ReadAllText(png));
                var uuid = matcher.Groups[1].Value;
                if (uuid2path.ContainsKey(uuid))
                {
                    Debug.LogError("uuid dup" + uuid + " \n" + png + "\n" + uuid2path[uuid]);
                }
                else
                {
                    uuid2path.Add(uuid, png.Substring(0, png.Length - 5));
                }
                EditorUtility.DisplayProgressBar("扫描图片中", png, (float)i / pngs.Count);

            });

            //find all prefab and search pic uuid
            var prefabs = Directory.GetFiles(uiPrefabRootDir, "*.prefab", SearchOption.AllDirectories);
            var anims = Directory.GetFiles("Assets/", "*.anim", SearchOption.AllDirectories).Where(p => !p.Replace('\\', '/').Contains("Characters/"));
            var allFiles = prefabs.Concat(anims).ToList();
            var alluuids = allFiles
                .SelectI((f, i) => {
                    EditorUtility.DisplayProgressBar("获取引用关系", f, (float)i / allFiles.Count);
                    return getUUIDsInFile(f);
                }).ToList().Aggregate((a, b) => a.Concat(b).ToList()).ToList();
            EditorUtility.ClearProgressBar();
            //rm used pic uuid
            var uuidshashset = new HashSet<string>(alluuids);
            var em = uuidshashset.GetEnumerator();
            while (em.MoveNext())
            {
                var uuid = em.Current;
                uuid2path.Remove(uuid);
            }

            StringBuilder sb = new StringBuilder();
            sb.Append("UnUsedFiles: ");
            sb.Append(uuid2path.Count);
            sb.Append("\n");
            uuid2path.ForEach(kv => sb.Append(kv.Value + "\r\n"));

            File.WriteAllText("Assets/unusedpic.txt", sb.ToString());
            EditorUtility.DisplayDialog("扫描成功", string.Format("共找到{0}个冗余图片\n请在Assets/unsedpic.txt查看结果", uuid2path.Count), "ok");
        }

        public static class CheckImageUse
        {
            [MenuItem("程序/检查工具/检查Prefab使用纹理",false,02010000)]
            public static void Scan()
            {
                var uiPrefabRootDir = EditorUtility.OpenFolderPanel("选择UIPrefab目录", "Assets", "");
                if (string.IsNullOrEmpty(uiPrefabRootDir))
                {
                    return;
                }

                var uiPicRootDir = EditorUtility.OpenFolderPanel("选择UIImage目录", "Assets", "");
                if (string.IsNullOrEmpty(uiPicRootDir))
                {
                    return;
                }

                var pngs = Directory.GetFiles(uiPicRootDir, "*.meta", SearchOption.AllDirectories)
                    .Select(p => "Assets/" + p.Replace('\\', '/').Substring(Application.dataPath.Length + 1))
                    .Where(p => {
                        return p.EndsWith(".png.meta");
                    }).ToList();


                var uuid2path = new Dictionary<string, string>();
                var uuidReg = new Regex(@"guid: ([a-f0-9]{32})");
                pngs.ForEachI((png, i) => {
                    var matcher = uuidReg.Match(File.ReadAllText(png));
                    var uuid = matcher.Groups[1].Value;
                    if (uuid2path.ContainsKey(uuid))
                    {
                        Debug.LogError("uuid dup" + uuid + " \n" + png + "\n" + uuid2path[uuid]);
                    }
                    else
                    {
                        uuid2path.Add(uuid, png.Substring(0, png.Length - 5).Replace("Assets/App/Pro/GameRes/UI/Module/", ""));
                    }
                    EditorUtility.DisplayProgressBar("扫描图片中", png, (float)i / pngs.Count);
                });

                EditorUtility.ClearProgressBar();

                var prefabs = Directory.GetFiles(uiPrefabRootDir, "*.prefab", SearchOption.AllDirectories).ToList();

                var prefabToImage = new Dictionary<string, List<string>>();
                //int j = 0;
                foreach (var prefab in prefabs)
                {
                    //EditorUtility.DisplayProgressBar("获取引用关系", prefab, j / prefabs.Count);
                    var uuids = getUUIDsInFile(prefab);
                    foreach (var uuid in uuids)
                    {
                        if (uuid2path.ContainsKey(uuid))
                        {
                            if (!prefabToImage.ContainsKey(prefab))
                            {
                                prefabToImage[prefab] = new List<string>();
                            }

                            if (!prefabToImage[prefab].Contains(uuid2path[uuid]))
                            {
                                prefabToImage[prefab].Add(uuid2path[uuid]);
                            }
                        }
                    }

                    //++j;
                }

                foreach (var v in prefabToImage)
                {
                    v.Value.Sort();
                }

                EditorUtility.ClearProgressBar();

                StringBuilder sb = new StringBuilder();
                foreach (var keyValuePair in prefabToImage)
                {
                    sb.Append(keyValuePair.Key).Append(":").Append("\r\n");
                    foreach (var s in keyValuePair.Value)
                    {
                        sb.Append("    ").Append(s).Append("\r\n");
                    }
                }
                File.WriteAllText("Assets/111.txt", sb.ToString());
            }
        }
    }
}
