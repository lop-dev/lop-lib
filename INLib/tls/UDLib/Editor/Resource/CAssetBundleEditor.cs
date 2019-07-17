//#define REFRENCE_MODE

using UnityEngine;
using System;
using UnityEditor;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace UDLib.Editor
{
    public class CAssetBundleEditor
    {
        private static UDLib.Utility.IResourceDefine _iResourceDefine;
        public static UDLib.Utility.IResourceDefine ResourceDefine
        {
            get
            {

                return _iResourceDefine;
            }

            set
            {
                _iResourceDefine = value;
            }
        }

        [MenuItem("程序/热更工具/设置资源加载模式/Resources    模式", false, 02030000)]
        public static void DirectMode()
        {
            List<string> defineSymbols = new List<string>(PlayerSettings.GetScriptingDefineSymbolsForGroup(EditorUserBuildSettings.selectedBuildTargetGroup).Split(';'));
            if (!defineSymbols.Contains("DIRECT_MODE"))
            {
                defineSymbols.Add("DIRECT_MODE");
            }

            PlayerSettings.SetScriptingDefineSymbolsForGroup( EditorUserBuildSettings.selectedBuildTargetGroup, string.Join(";", defineSymbols.ToArray()) );

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }

        [MenuItem("程序/热更工具/设置资源加载模式/AssetBundle 模式", false, 02030000)]
        public static void AssetBundleMode()
        {
            List<string> defineSymbols = new List<string>(PlayerSettings.GetScriptingDefineSymbolsForGroup(EditorUserBuildSettings.selectedBuildTargetGroup).Split(';'));
            if (defineSymbols.Contains("DIRECT_MODE"))
            {
                defineSymbols.Remove("DIRECT_MODE");
            }

            PlayerSettings.SetScriptingDefineSymbolsForGroup( EditorUserBuildSettings.selectedBuildTargetGroup, string.Join(";", defineSymbols.ToArray()) );

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }

        public class AssetBundleInfo
        {
            public string categoryName;
            public UDLib.Utility.EBUILD_TYPE type;
            public string exts;           // 文件扩展名
            public string suffix;         // ab名后以它结尾
            public string[] rootPaths;    // _ASSERT_BUNDLE_根目录
            public string[] subPaths;     // _ASSERT_BUNDLE_子目录
        };


        static string getTargetPlatformName(BuildTarget target)
        {
            switch (target)
            {
                case BuildTarget.Android:
                    return "android";
                case BuildTarget.iOS:
                    return "ios";
                case BuildTarget.StandaloneWindows:
                case BuildTarget.StandaloneWindows64:
                    return "win";
                case BuildTarget.StandaloneOSX:
                    return "osx";
                default:
                    return null;
            }
        }

        //由于生成完此文件需要等待编译器重新编译，尚未知道什么时候编译完，可能打包的资源是上次的配置。
        //所以全部打包此不执行此函数，此函数放在postProcess里自动调用
        [MenuItem("程序/热更工具/生成CResourcePath.cs", false, 02030000)]
        public static void CreateResourcePath()
        {
            // 输出路径
            const string OUT_PATH = "Assets/App/Pro/Scripts/Lop/Resources/CResourcePath.cs";
            // 取得信息
            List<AssetBundleInfo> list = GetAssetBundleInfoList();
            if (list == null)
                return;

            DateTime startTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("开始 生成ResourceDefine");

            using (StreamWriter sw = new StreamWriter(OUT_PATH, false, new System.Text.UTF8Encoding(false)))
            {
                sw.NewLine = "\n";
                sw.WriteLine("// ※此脚本由AssetBundleEnvironment.CreateResourceDefine()自动生成");
                sw.WriteLine("using UDLib.Utility;");
                sw.WriteLine("");
                sw.WriteLine("namespace UDLib.Resource");
                sw.WriteLine("{");

                sw.WriteLine("  // 资源类型");
                sw.WriteLine("  public enum ERESOURCE_TYPE");
                sw.WriteLine("  {");

                foreach (AssetBundleInfo info in list)
                {
                    sw.WriteLine("    " + info.categoryName + ",");
                }

                sw.WriteLine();
                sw.WriteLine("    MAX");

                sw.WriteLine("  };");

                sw.WriteLine();
                sw.WriteLine("  // 资源定义");
                sw.WriteLine("  public static class CResourcePath");
                sw.WriteLine("  {");

                sw.WriteLine("      // CATEGORY_TYPE数组");
                sw.WriteLine("      public static readonly EBUILD_TYPE[] buildTypes = new EBUILD_TYPE[]");
                sw.WriteLine("      {");
                foreach (AssetBundleInfo info in list)
                {
                    sw.WriteLine("          EBUILD_TYPE." + info.type.ToString() + ", // " + info.categoryName);
                }
                sw.WriteLine("      };");

                sw.WriteLine("      // 用于添加至AB包名的后缀");
                sw.WriteLine("      public static readonly string[] suffix = new string[]");
                sw.WriteLine("      {");
                foreach (AssetBundleInfo info in list)
                {
                    sw.WriteLine("          \"" + info.suffix + "\", // " + info.categoryName);
                }
                sw.WriteLine("      };");

                sw.WriteLine();
                //  sw.WriteLine("    #if INTERNAL_ASSETS");  //内部加载模式时用

                sw.WriteLine("      // _ASSERT_BUNDLE_txt数组");
                sw.WriteLine("      public static readonly string[][] rootPaths = new string[][]");
                sw.WriteLine("      {");
                foreach (AssetBundleInfo info in list)
                {
                    sw.WriteLine("          new string[] // " + info.categoryName);
                    sw.WriteLine("          {");
                    foreach (string path in info.rootPaths)
                    {
                        if (path.Contains("internalui"))
                        {
                            // internaluiはResourceDefineに含めない   [不要在ResourceDefine中包含internalui]
                            continue;
                        }
                        sw.WriteLine("              \"" + path.Replace("\\", "/") + "\",");
                    }
                    sw.WriteLine("          },");
                }
                sw.WriteLine("      };");

                sw.WriteLine("      // Resources.Load()使用的路径数组");
                sw.WriteLine("      public static readonly string[][] subPaths = new string[][]");
                sw.WriteLine("      {");
                foreach (AssetBundleInfo info in list)
                {
                    sw.WriteLine("          new string[] // " + info.categoryName);
                    sw.WriteLine("          {");
                    foreach (string path in info.subPaths)
                        sw.WriteLine("              \"" + path.Replace("\\", "/") + "\",");
                    sw.WriteLine("          },");
                }
                sw.WriteLine("      };");

                sw.WriteLine("      // 对象扩展名数组");
                sw.WriteLine("      public static readonly string[][] exts = new string[][]");
                sw.WriteLine("      {");
                foreach (AssetBundleInfo info in list)
                {
                    sw.WriteLine("          new string[] // " + info.categoryName);
                    sw.WriteLine("          {");
                    foreach (string ext in info.exts.Split(new char[] { ',', ';' }, System.StringSplitOptions.RemoveEmptyEntries))
                        sw.WriteLine("              \"" + ext + "\",");
                    sw.WriteLine("          },");
                }
                sw.WriteLine("      };");

                // sw.WriteLine("    #endif");

                sw.WriteLine("  };"); // class
                sw.WriteLine("}"); // namespace
                sw.Flush();
                sw.Close();
            }

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();

            DateTime endTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("结束 生成ResourceDefine");
            UDLib.Utility.CDebugOut.Log("耗时：" + (endTime - startTime).TotalSeconds + "秒");
        }

        // 收集abtxt信息
        private static List<AssetBundleInfo> GetAssetBundleInfoList()
        {
            DateTime startTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("开始 收集abtxt信息");

            List<AssetBundleInfo> list = new List<AssetBundleInfo>();

            string data_path = Application.dataPath + "/";

            int data_path_len = data_path.Length - 7;
            string[] files = Directory.GetFiles(data_path, "_ASSET_BUNDLE_.txt", SearchOption.AllDirectories);
            foreach (string fname in files)
            {
                using (StreamReader sr = new StreamReader(fname))
                {
                    // 第一行:种类名称
                    string category_name = sr.ReadLine();
                    // 第二行:类别
                    UDLib.Utility.EBUILD_TYPE category_type = (UDLib.Utility.EBUILD_TYPE)System.Enum.Parse(typeof(UDLib.Utility.EBUILD_TYPE), sr.ReadLine());
                    // 第三行:搜索扩展名(包括点,小写字母,多个逗号分隔)
                    string exts = sr.ReadLine().ToLower();
                    // 第四行:填加后缀到AB包包名
                    string suffix = sr.ReadLine();
                    if (suffix != null)
                        suffix = suffix.ToLower();

                    string temp = fname.Substring(data_path_len);//截取  只留下 Asset/  这个几个字符串
                    string root_path = Path.GetDirectoryName(temp).ToLower().Replace("\\", "/");// Asset/到_ASSET_BUNDLE_所在目录

                    string[] sub_paths = null;
                    if (category_type != UDLib.Utility.EBUILD_TYPE.FOLDER)
                    {
                        sub_paths = Directory.GetDirectories(root_path, "*", SearchOption.AllDirectories);
                        if (sub_paths.Length > 0)
                        {
                            for (int i = 0, n = sub_paths.Length; i < n; ++i)
                            {
                                sub_paths[i] = GetSubPath(sub_paths[i].ToLower());
                            }
                            bool inroot = false;
                            foreach (var ext in exts.Split(','))
                            {
                                //在_ASSET_BUNDLE_.txt同级目录搜索符合后缀的资源
                                var rootFiles = Directory.GetFiles(root_path, "*" + ext, SearchOption.TopDirectoryOnly);
                                if (rootFiles.Length > 0)
                                {
                                    inroot = true;
                                    break;
                                }
                            }
                            if (inroot)//把root_path也加入到subpaths里
                            {
                                var subpaths = new string[sub_paths.Length + 1];
                                subpaths[0] = GetSubPath(root_path);
                                System.Array.Copy(sub_paths, 0, subpaths, 1, sub_paths.Length);
                                sub_paths = subpaths;
                            }

                        }
                        else
                        {
                            sub_paths = null;
                        }
                    }
                    if (sub_paths == null)
                    {
                        sub_paths = new string[] { GetSubPath(root_path) };
                    }

                    AssetBundleInfo info = list.Find(o => o.categoryName == category_name);
                    if (info == null)
                    {
                        info = new AssetBundleInfo();
                        list.Add(info);
                    }

                    root_path = root_path + "/";

                    if (info.categoryName == null)
                    {
                        // 新的category
                        info.categoryName = category_name;
                        info.type = category_type;
                        info.exts = exts;
                        info.suffix = suffix;
                        info.rootPaths = new string[] { root_path };
                        info.subPaths = sub_paths;
                    }
                    else
                    {
                        // 相通性检测
                        if (info.type != category_type || info.exts != exts || info.suffix != suffix)
                        {
                            UDLib.Utility.CDebugOut.LogError("具有相同类别的_ASSERT_BUNDLE_.txt，但类型，扩展名，后缀之一是不同的");
                            return null;
                        }

                        //把别处的同列别的路径加入进来
                        System.Array.Resize(ref info.rootPaths, info.rootPaths.Length + 1);
                        info.rootPaths[info.rootPaths.Length - 1] = root_path;

                        if (sub_paths.Length >= 1)
                        {
                            string[] new_sub_root_paths = new string[info.subPaths.Length + sub_paths.Length];
                            System.Array.Copy(info.subPaths, new_sub_root_paths, info.subPaths.Length);
                            System.Array.Copy(sub_paths, 0, new_sub_root_paths, info.subPaths.Length, sub_paths.Length);
                            info.subPaths = new_sub_root_paths;
                        }
                    }
                }
            }

            //按categoryName排序
            list.Sort((AssetBundleInfo a, AssetBundleInfo b) =>
            {
                return a.categoryName.CompareTo(b.categoryName);
            }
            );

            DateTime endTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("结束 收集abtxt信息");
            UDLib.Utility.CDebugOut.Log("耗时：" + (endTime - startTime).TotalSeconds + "秒");

            return list;
        }

        // 刷新AB包名
        //[MenuItem("AssetBundleX/1、自动设置资源ab名字")]
        public static void RefreshAllAssetBundleNames()
        {

            DateTime startTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("开始 设置ab资源名字");

            string title = "设置所有文件的AB名";

            EditorUtility.DisplayProgressBar(title, "搜索文件及文件夹", 0.0f);

            string[] files = Directory.GetFiles("Assets", "*", SearchOption.AllDirectories);
            string[] dirs = Directory.GetDirectories("Assets", "*", SearchOption.AllDirectories);

            float total = 1.0f / (float)(files.Length + dirs.Length);
            int p = 0;
#if REFRENCE_MODE
             CFindReferencesInProject.Clear();
#endif
            for (int i = 0, n = files.Length; i < n; ++i, ++p)
            {
                if ((i & 127) == 0)
                    EditorUtility.DisplayProgressBar(title, string.Format("文件处理进度 {0} / {1}", i, n), (float)p * total);
                //如果是引用模式，则这里处理的是查找引用关系
                SetAssetBundleName(files[i].Replace("\\", "/"));
            }
            for (int i = 0, n = dirs.Length; i < n; ++i, ++p)
            {
                if ((i & 63) == 0)
                    EditorUtility.DisplayProgressBar(title, string.Format("文件夹处理进度 {0} / {1}", i, n), (float)p * total);

                //清除文件夹的ab名字
                AssetImporter importer = AssetImporter.GetAtPath(dirs[i]);
                if (importer != null)
                {
                    if (importer.assetBundleName.Length > 0)
                        importer.assetBundleName = string.Empty;
                }
            }

#if REFRENCE_MODE
             //生成引用关系asset
             CFindReferencesInProject.CreateReferenceAsset();
             //查找根节点
             List<string> roots = CFindReferencesInProject.FindRoots();
             SetAssetBundleNamesReferenceMode(roots);
#endif

            EditorUtility.ClearProgressBar();
            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();

            DateTime endTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("结束 设置ab资源名字");
            UDLib.Utility.CDebugOut.Log("耗时：" + (endTime - startTime).TotalSeconds + "秒");
        }
#if REFRENCE_MODE
         //设置引用模式下的ab名字
         private static void SetAssetBundleNamesReferenceMode(List<string> roots)
         {
             alreadyProcessedPath = new List<string>();
             foreach (string path in roots)
             {
                 SetAssetBundleNameReferenceMode(path);
             }
         }
 
         private static List<String> alreadyProcessedPath;
 
         private static void SetAssetBundleNameReferenceMode(string path)
         {
             if (alreadyProcessedPath.Contains(path)) return;
 
             alreadyProcessedPath.Add(path);
 
             ReferenceInfo info = CFindReferencesInProject.getReferenceInfoByPath(path);
             AssetImporter importer = AssetImporter.GetAtPath(path);
             Assert.AreNotEqual(importer, null,"path not found:"+path);
 
             if (info.parents.Count == 0)
             {//没有被引用
                  setAssetBundleNameFilter(importer, info.path);
             }
             else if (info.parents.Count == 1)
             {//只被一个对象引用了
                 setAssetBundleNameFilter(importer, info.parents[0]);  
             }
             else
             {//被多个对象引用了
                 bool allParentsSameAbName = true;
                 string parentAbName = null;
                 for (int i = 0; i < info.parents.Count; i++)
                 {
                     AssetImporter parentImporter = AssetImporter.GetAtPath(info.parents[i]);
                     if (parentAbName == null)
                         parentAbName = parentImporter.assetBundleName;
                     else if (parentAbName != parentImporter.assetBundleName)
                     {
                         allParentsSameAbName = false;
                         break;
                     }
                 }
                 if(allParentsSameAbName)
                 {//多个对象属于同一个ab,设置为父对象的ab名字
                     importer.assetBundleName = parentAbName;
                 }
                 else
                 {//多对象属于不同的ab,单独打包
                     setAssetBundleNameFilter(importer, info.path);
                 }
             }
 
             //处理引用的其他资源
             SetAssetBundleNameReferenceModeDependencies(info.path);
         }
 
         //处理引用的资源
         private static void SetAssetBundleNameReferenceModeDependencies(string path)
         {
             string[] dependencies = AssetDatabase.GetDependencies(path, false);
             foreach (var dependency in dependencies)
             {
                 SetAssetBundleNameReferenceMode(dependency);
             }
         }
 
         private static void setAssetBundleNameFilter(AssetImporter importer,string path)
         {
             string ab_name = Path.GetFileNameWithoutExtension(path).ToLower() + ".dat";
             if (importer.assetBundleName != ab_name || importer.assetBundleVariant.Length > 0)
             {
                 importer.assetBundleName = ab_name;
                 if (importer.assetBundleVariant.Length > 0)
                     importer.assetBundleVariant = string.Empty;
             }
         }
#endif

        //设置单个文件的Ab名字
        public static AssetImporter SetAssetBundleName(string path)
        {
#if REFRENCE_MODE
             string origin_path = path;
#endif
            path = path.ToLower();

            AssetImporter importer = AssetImporter.GetAtPath(path);
            if (importer != null)
            {
                // 不要排除Art目录，把AB机制扩张到Asset全目录
                if (path.Contains(".") == true && path.Contains("/streamingassets/") == false &&
                    path.Contains("/editor/") == false /*&& path.StartsWith("assets/app/art") == false */&&
                    path.IndexOf("_asset_bundle_.txt") == -1)
                {
                    string[][] root_paths = ResourceDefine.GetABRootPaths();
                    for (int i = 0, n = root_paths.Length; i < n; ++i)
                    {
                        for (int ii = 0, nn = root_paths[i].Length; ii < nn; ++ii)
                        {
                            string _root_paths = root_paths[i][ii];
                            if (path.StartsWith(_root_paths) == true)//检测资源路径是否在打包文件夹内
                            {
                                string ext = Path.GetExtension(path);
                                string[] exts = ResourceDefine.GetABExts()[i];
                                string suffix = ResourceDefine.GetABSuffix()[i];

                                for (int j = 0, m = exts.Length; j < m; ++j)
                                {
                                    if (ext == exts[j]) //检测扩展名是否在打包列表内
                                    {
                                        string name = Path.GetFileNameWithoutExtension(path);
                                        string folder = ResourceDefine.ResourceTypeToString(i);


#if REFRENCE_MODE
                                         //设置引用关系
                                         CFindReferencesInProject.Find(origin_path);
#else
                                        string ab_name = null;
                                        switch (ResourceDefine.GetABBuildTypes()[i])
                                        {
                                            case UDLib.Utility.EBUILD_TYPE.SINGLE:
                                                ab_name = folder + "/" + name;
                                                break;

                                            case UDLib.Utility.EBUILD_TYPE.HASH256:
                                                ab_name = folder + "/" + ResourceDefine.ResourceTypeToHash256String(i, name);
                                                break;

                                            case UDLib.Utility.EBUILD_TYPE.FOLDER:
                                                {
                                                    bool isSet = false;
                                                    string _path = path.ToLower();
                                                    if (_path.Contains("ui/module") && _path.Contains("config/"))
                                                    {
                                                        // 判断是ui/module下的config目录 ，向上一级取目录名来组成ab_name
                                                        isSet = SetABNameWithParentFolder(folder, _path, "ui/module", ref ab_name);
                                                    }
                                                    else if (_path.Contains("art/character/hero/model"))
                                                    {
                                                        // 判断是art下的charater Texture ，向上一级取目录名来组成ab_name
                                                        isSet = SetABNameWithParentFolder(folder, _path, "art/character/hero/model", ref ab_name);
                                                    }

                                                    if (!isSet)
                                                        ab_name = folder + "/" + Path.GetFileName(Path.GetDirectoryName(path));
                                                }

                                                break;

                                            case UDLib.Utility.EBUILD_TYPE.PACK:
                                                ab_name = folder;
                                                break;
                                        }

                                        ab_name = ab_name.ToLower() + suffix + ".dat";
                                        ab_name = ab_name.Replace(" ", "");

                                        // set file attribute
                                        RemoveMetaReadOnly(path + ".meta");
                                        if (importer.assetBundleName != ab_name || importer.assetBundleVariant.Length > 0)
                                        {
                                            importer.assetBundleName = ab_name;
                                            if (importer.assetBundleVariant.Length > 0)
                                                importer.assetBundleVariant = string.Empty;
                                            UDLib.Utility.CDebugOut.Log("SetAssetBundleName\t" + importer.assetBundleName + "\t" + ab_name + "\t" + path);
                                        }
#endif
                                        return importer;
                                    }
                                }
                            }
                        }
                    }

                }

                if (importer.assetBundleName.Length > 0)
                {
                    // 该资源无需ab名字了
                    RemoveMetaReadOnly(path + ".meta");
                    importer.assetBundleName = string.Empty;
                }
            }

            return null;
        }

        // 移除meta文件的只读属性
        private static void RemoveMetaReadOnly(string metaPath)
        {
            var attr = File.GetAttributes(metaPath);
            if ((attr & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
            {
                UDLib.Utility.CDebugOut.Log("+++++++++++++remove readOnly+++++++++++++++" + metaPath);
                File.SetAttributes(metaPath, FileAttributes.Normal);
            }
        }

        static string GetModuleName(string folder, string _path, string subStr)
        {
            string configString = "/config";

            int pos = _path.IndexOf(subStr, StringComparison.OrdinalIgnoreCase);
            if (pos != -1)
            {
                string tmp = _path.Substring(pos + subStr.Length + 1);
                pos = tmp.IndexOf(configString, StringComparison.Ordinal);
                if (-1 == pos)
                {
                    pos = tmp.IndexOf("/", StringComparison.Ordinal);
                    if (-1 != pos)
                    {
                        return tmp.Remove(pos);
                    }
                }
                else
                {
                    tmp = tmp.Remove(pos);
                    pos = tmp.IndexOf("/", StringComparison.Ordinal);
                    if (-1 == pos)
                    {
                        return tmp;
                    }

                    return tmp.Substring(pos + 1);
                }
            }

            return string.Empty;
        }

        // 退回上一级取AB名称，针对ui/module/config, art/character/hero/model/xxx/Texture等
        private static bool SetABNameWithParentFolder(string folder, string _path, string subStr, ref string ab_name)
        {
            bool isSet = false;
            int pos = _path.IndexOf(subStr, StringComparison.OrdinalIgnoreCase);
            if (pos != -1)
            {
                // 判断是art下的charater Texture ，向上一级取目录名来组成ab_name
                //string moduleName = _path.Substring(pos + subStr.Length + 1);
                //pos = moduleName.IndexOf("/", StringComparison.Ordinal);
                //if (pos != -1)
                //{
                //    moduleName = moduleName.Remove(pos);
                //    ab_name = folder + "/" + moduleName;
                //    isSet = true;
                //}
                string moduleName = GetModuleName(folder, _path, subStr);
                if (!string.IsNullOrEmpty(moduleName))
                {
                    isSet = true;
                    ab_name = folder + "/" + moduleName;
                }
            }

            return isSet;
        }

        //打包ab文件
        public static void CreateAssetBundles(string path, string versionCode)
        {
            if (string.IsNullOrEmpty(versionCode))
            {
                UDLib.Utility.CDebugOut.LogError("没有传入版本号，打包错误！");
                return;
            }
            DateTime startTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("开始 生成ab文件");

            string platformName = getTargetPlatformName(EditorUserBuildSettings.activeBuildTarget);
            if (platformName == null)
            {
                UDLib.Utility.CDebugOut.LogError("打包发生错误，未知平台！");
                return;
            }

            if (path == null)
                path = Application.dataPath;

            string assetBundleDirectory = path + "/AssetBuild/" + platformName + "/AssetBundle";

            if (!Directory.Exists(assetBundleDirectory))
            {
                Directory.CreateDirectory(assetBundleDirectory);
            }


            //video使用无压缩格式
            string[] files = Directory.GetFiles("Assets", "*", SearchOption.AllDirectories);
            Dictionary<string, AssetBundleBuild> defaultBuildDic = new Dictionary<string, AssetBundleBuild>();
            Dictionary<string, AssetBundleBuild> videoBuildDic = new Dictionary<string, AssetBundleBuild>();
            int index = 0;
            foreach (var f in files)
            {
                EditorUtility.DisplayProgressBar("资源分类", index + "/" + files.Length, index++ / (float)files.Length);
                string file = f.ToLower();
                if (file.Contains(".") == true && file.Contains("/streamingassets/") == false &&
                    file.Contains("/editor/") == false /*&& path.StartsWith("assets/app/art") == false */&&
                    file.IndexOf("_asset_bundle_.txt") == -1)
                {
                    AssetImporter importer = AssetImporter.GetAtPath(f);
                    if (importer != null && !string.IsNullOrEmpty(importer.assetBundleName))
                    {
                        List<string> assetNames = new List<string>();
                        if (defaultBuildDic.ContainsKey(importer.assetBundleName))
                        {
                            assetNames.AddRange(defaultBuildDic[importer.assetBundleName].assetNames);
                            assetNames.Add(importer.assetPath);
                            AssetBundleBuild tempBuild = defaultBuildDic[importer.assetBundleName];
                            tempBuild.assetNames = assetNames.ToArray();
                            defaultBuildDic[importer.assetBundleName] = tempBuild;
                        }
                        else if (videoBuildDic.ContainsKey(importer.assetBundleName))
                        {
                            assetNames.AddRange(videoBuildDic[importer.assetBundleName].assetNames);
                            assetNames.Add(importer.assetPath);
                            AssetBundleBuild tempBuild = videoBuildDic[importer.assetBundleName];
                            tempBuild.assetNames = assetNames.ToArray();
                            videoBuildDic[importer.assetBundleName] = tempBuild;
                        }
                        else
                        {
                            AssetBundleBuild build = new AssetBundleBuild();
                            build.assetBundleName = importer.assetBundleName;
                            build.assetBundleVariant = importer.assetBundleVariant;
                            string[] asset = new string[1];
                            asset[0] = importer.assetPath;
                            build.assetNames = asset;
                            if (build.assetBundleName.ToLower().Replace('\\', '/').Contains("video/"))
                            {
                                videoBuildDic.Add(build.assetBundleName, build);
                            }
                            else
                            {
                                defaultBuildDic.Add(build.assetBundleName, build);
                            }
                        }
                    }
                }
            }
            List<AssetBundleBuild> defaultBuildList = new List<AssetBundleBuild>();
            foreach (var v in defaultBuildDic)
            {
                defaultBuildList.Add(v.Value);
            }
            List<AssetBundleBuild> videoBuildList = new List<AssetBundleBuild>();
            foreach (var v in videoBuildDic)
            {
                videoBuildList.Add(v.Value);
            }
            EditorUtility.ClearProgressBar();
            //打包ab文件
            BuildPipeline.BuildAssetBundles(assetBundleDirectory, videoBuildList.ToArray(), BuildAssetBundleOptions.UncompressedAssetBundle, EditorUserBuildSettings.activeBuildTarget);
            var videoManifestPath = Path.Combine(assetBundleDirectory, "AssetBundle");
            var videoManifestPathNew = Path.Combine(assetBundleDirectory, "VideoAssetBundle");
            if (File.Exists(videoManifestPath))
            {
                var bytes = File.ReadAllBytes(videoManifestPath);
                if (File.Exists(videoManifestPathNew))
                {
                    File.Delete(videoManifestPathNew);
                }
                File.WriteAllBytes(videoManifestPathNew, bytes);
            }

            BuildPipeline.BuildAssetBundles(assetBundleDirectory, defaultBuildList.ToArray(), BuildAssetBundleOptions.ChunkBasedCompression | BuildAssetBundleOptions.IgnoreTypeTreeChanges,
                EditorUserBuildSettings.activeBuildTarget);

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();

            DateTime endTime = System.DateTime.Now;
            UDLib.Utility.CDebugOut.Log("结束 生成ab文件");
            UDLib.Utility.CDebugOut.Log("耗时：" + (endTime - startTime).TotalSeconds + "秒");
            string assetsBundleRootDir = path + "/AssetBuild/";

            UDLib.Utility.CDebugOut.Log("AB包出包提示: 2.检测清单文件是否出存");
            var manifestPath = Path.Combine(assetBundleDirectory, "AssetBundle");    // ...../AssetBuild/platform/AssetBundle/platform
            var manifestPathPlatform = Path.Combine(assetBundleDirectory, "ResourceManifest");
            if (File.Exists(manifestPath))
            {
                UDLib.Utility.CDebugOut.Log("AB包出包提示: 3.版本文件更新");
                var bytes = File.ReadAllBytes(manifestPath);
                var versionManifestFilePath = manifestPath + "_v" + versionCode;          // ...../AssetBuild/platform/AssetBundle/platform_vx 这个文件就是manifest的复制品
                if (File.Exists(versionManifestFilePath))
                {
                    File.Delete(versionManifestFilePath);
                }
                File.WriteAllBytes(versionManifestFilePath, bytes);

                if (File.Exists(manifestPathPlatform))
                {
                    File.Delete(manifestPathPlatform);
                }
                File.WriteAllBytes(manifestPathPlatform, bytes);

                AssetDatabase.SaveAssets();
                AssetDatabase.Refresh();

                var versionHashFilePath = assetsBundleRootDir + "/" + platformName + "/AssetBundle/ResourceManifest_hash.txt";  //...../AssetBuild/platform/platform_hash_vx
                UDLib.Utility.CDebugOut.Log("AB包出包提示: 5.生成Hash检查文件");

                StringBuilder sbDefault = ReadManifestToSB(versionManifestFilePath, platformName, path, versionCode);
                StringBuilder sbVideo = ReadManifestToSB(videoManifestPathNew, platformName, path, versionCode);
                SaveSBToManifest(sbDefault.Append(sbVideo), versionHashFilePath);

                //不再把整个清单拷贝到streamAsset下，根据设置拷贝进入底包的AB资源以控制包体大小
                //CreatePackgeManifestFile(versionHashFilePath);
            }
            else
            {
                UDLib.Utility.CDebugOut.LogError("manifest清单文件没有找到");
                return;
            }

            UDLib.Utility.CDebugOut.Log("AB包出包提示: 6.生成本次变更列表文件");
            //生成本次AB修改列表文件
            //CreateAssetsbundleDiff(path);

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }

        //生成ab压缩文件
        private static void CreateZipAbFiles(string assetBundleDirectory, string platformName)
        {
            //DateTime startTime = System.DateTime.Now;
            //CLog.LogWithTime("开始 压缩AB文件");

            //string manifestPath = assetBundleDirectory + "/" + platformName;

            //AssetBundle manifestBundle1 = AssetBundle.LoadFromFile(manifestPath);
            //AssetBundleManifest manifest = (AssetBundleManifest)manifestBundle1.LoadAsset("AssetBundleManifest");
            //string[] assetBundles = manifest.GetAllAssetBundles();

            //foreach (var assetBundle in assetBundles)
            //{
            //    string abPath = assetBundleDirectory + "/" + assetBundle;
            //    byte[] bytes = File.ReadAllBytes(abPath);
            //    byte[] compressedBytes = Ionic.Zlib.GZipStream.CompressBuffer(bytes);
            //    File.Delete(abPath);
            //    File.WriteAllBytes(abPath, compressedBytes);
            //}
            //manifestBundle1.Unload(true);

            ////manifest也压缩
            //byte[] mBytes = File.ReadAllBytes(assetBundleDirectory + "/" + platformName);
            //byte[] compressedmBytes = Ionic.Zlib.GZipStream.CompressBuffer(mBytes);
            //File.WriteAllBytes(manifestPath, compressedmBytes);

            //DateTime endTime = System.DateTime.Now;
            //CLog.LogWithTime("结束 压缩AB文件");
            //CDebugOut.Log(SysLogType.Other, "耗时：" + (endTime - startTime).TotalSeconds + "秒");
        }

        public static void CreateAssetsbundleDiff(string outPath)
        {
            if (outPath == null)
                outPath = Application.dataPath;

            string platformName = getTargetPlatformName(EditorUserBuildSettings.activeBuildTarget);
            if (platformName == null)
            {
                UDLib.Utility.CDebugOut.LogError("打包发生错误，未知平台！");
                return;
            }

            // string assetBundleDirectory = outPath + "/AssetBuild/" + platformName;

            string latestVersionPath = platformName + "/LatestVersion.txt";
            string assetsBundleRootDir = outPath + "/AssetBuild/";

            int lastV = 0;
            FileInfo lastVersionFile = new FileInfo(Path.Combine(assetsBundleRootDir, latestVersionPath));
            if (lastVersionFile.Exists)
            {
                StreamReader srV = new StreamReader(lastVersionFile.OpenRead());
                lastV = CSLib.Utility.CStringHelper.Convert2Value<int>(srV.ReadLine());
                if (lastV < 2)
                {
                    UDLib.Utility.CDebugOut.Log("Ignored: 初始版本,没有检测到修改");
                    srV.Close();
                    return;
                }
                srV.Close();

                DateTime startTime = System.DateTime.Now;
                UDLib.Utility.CDebugOut.Log("开始 生成本次AB修改列表文件");

                // 取得HASH值记录文件
                var versionHashFilePath = Path.Combine(assetsBundleRootDir + "/" + platformName, platformName) + "_hash_v" + lastV + ".txt";

                //获取当前的Hash字典
                Dictionary<string, string> currenHashDic = new Dictionary<string, string>();
                StreamReader srcurrent = new StreamReader(versionHashFilePath);
                while (!srcurrent.EndOfStream)
                {
                    string line = srcurrent.ReadLine();
                    string[] parms = line.Split('\t');
                    if (parms.Length >= 2)
                    {
                        currenHashDic.Add(parms[0], parms[1]);
                    }
                }
                srcurrent.Close();

                var preVersionHashFilePath = Path.Combine(assetsBundleRootDir + "/" + platformName, platformName) + "_hash_v" + (lastV - 1) + ".txt";
                // 取得上一个版本的Hash字典
                Dictionary<string, string> preHashDic = new Dictionary<string, string>();
                StreamReader srPre = new StreamReader(preVersionHashFilePath);
                while (!srPre.EndOfStream)
                {
                    string line = srPre.ReadLine();
                    string[] parms = line.Split('\t');
                    if (parms.Length >= 2)
                    {
                        preHashDic.Add(parms[0], parms[1]);
                    }
                }
                srPre.Close();

                List<string> abnames_list = new List<string>();

                foreach (string key in currenHashDic.Keys)
                {
                    if (preHashDic.ContainsKey(key))
                    {
                        if (preHashDic[key] != currenHashDic[key])
                        {
                            abnames_list.Add("modified  " + key);
                        }
                    }
                    else
                        abnames_list.Add("add  " + key);
                }

                foreach (string key in preHashDic.Keys)
                {
                    if (currenHashDic.ContainsKey(key) == false)
                        abnames_list.Add("delete  " + key);
                }

                string diffPath = Path.Combine(assetsBundleRootDir + "/" + platformName, platformName) + "_diff_v" + lastV;
                StreamWriter sw = new StreamWriter(diffPath + ".txt");
                if (abnames_list.Count == 0)
                {
                    sw.WriteLine("本次打包ab没有任何修改");
                }
                else
                {
                    foreach (var change in abnames_list)
                    {
                        sw.WriteLine(change);
                    }
                }

                sw.Flush();
                sw.Close();

                DateTime endTime = System.DateTime.Now;
                UDLib.Utility.CDebugOut.Log("结束 生成本次AB修改列表文件");
                UDLib.Utility.CDebugOut.Log("耗时：" + (endTime - startTime).TotalSeconds + "秒");
            }
        }

        /// <summary>
        /// 生成HASH对比文件
        /// </summary>
        /// <param name="versionManifestFilePath">Manifest路径</param>
        /// <param name="versionCheckFilePath">版本比较文件路径</param>
        static StringBuilder ReadManifestToSB(string versionManifestFilePath, string platform, string rootPath, string version)
        {
            byte[] compressedManifestBytes = File.ReadAllBytes(versionManifestFilePath);
            AssetBundle manifestBundle1 = AssetBundle.LoadFromMemory(compressedManifestBytes/**Ionic.Zlib.GZipStream.UncompressBuffer(compressedManifestBytes)*/);
            AssetBundleManifest manifest = (AssetBundleManifest)manifestBundle1.LoadAsset("AssetBundleManifest");
            string[] assetBundles = manifest.GetAllAssetBundles();
            StringBuilder sb = new StringBuilder();

            foreach (var assetBundle in assetBundles)
            {
                string filePath = string.Format("{0}/AssetBuild/{1}/AssetBundle/{2}", rootPath, platform, assetBundle);
                FileInfo fi = new FileInfo(filePath);
                sb.Append(assetBundle).Append('\t').Append(version).Append('\t').Append(manifest.GetAssetBundleHash(assetBundle).ToString());
                if (fi.Exists)
                {
                    sb.Append('\t').Append(fi.Length);
                }
                sb.Append("\n");
            }

            manifestBundle1.Unload(true);

            return sb;
        }

        private static void SaveSBToManifest(StringBuilder sb, string versionCheckFilePath)
        {
            if (File.Exists(versionCheckFilePath))
            {
                File.Delete(versionCheckFilePath);
            }

            using (StreamWriter sw = new StreamWriter(versionCheckFilePath))
            {
                sw.Write(sb.ToString());
            }

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }

        //如果资源在Recources下，则目录从Recources后面算起
        static string GetSubPath(string path)
        {
            int i = path.IndexOf("/resources/");

            if (i == -1)
                return path + "/";

            return path.Substring(i + 11) + "/";
        }

        //创建streamingAssetsPath目录的ab清单(含hash)
        [MenuItem("程序/热更工具/创建Stream目录清单", false, 02030000)]
        public static void CreateZipList()
        {
            int count = 0;

            string[] files = Directory.GetFiles(Application.streamingAssetsPath + "/assets/", "*.dat*", SearchOption.AllDirectories);
            StreamWriter sw = new StreamWriter(Application.streamingAssetsPath + "/zip_ab_all_list.txt");

            StreamWriter shaderAndFontSW = new StreamWriter(Application.streamingAssetsPath + "/shader_and_font.txt");

            bool isIncludeShader = false;
            bool isIncludeFont = false;

            for (int i = 0; i < files.Length; i++)
            {
                string file = files[i];
                if (file.EndsWith(".manifest")) continue;
                if (file.EndsWith(".meta")) continue;
                file = file.Replace("\\", "/");
                int startIndex = file.IndexOf("assets/");
                file = file.Substring(startIndex + "assets/".Length);
                if (file.IndexOf("shader.dat") != -1)
                {
                    shaderAndFontSW.WriteLine(file);
                    isIncludeShader = true;
                }

                if (file.IndexOf("ui_font.dat") != -1)
                {
                    shaderAndFontSW.Write(file);
                    isIncludeFont = true;
                }

                sw.WriteLine(file);
                count++;
            }

            sw.Flush();
            sw.Close();

            shaderAndFontSW.Flush();
            shaderAndFontSW.Close();


            //StreamWriter sw_count = new StreamWriter(Application.streamingAssetsPath + "/zip_ab_count.txt");
            //sw_count.Write(count);
            //sw_count.Flush();
            //sw_count.Close();

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();

            UDLib.Utility.CDebugOut.Log(string.Format("<color=green>创建清单完成，共计{0}个ab文件</color>", count));
            if (isIncludeFont == false)
                UDLib.Utility.CDebugOut.LogError("发生错误：ui_font.dat文件不在清单内,请检查！");
            if (isIncludeShader == false)
                UDLib.Utility.CDebugOut.LogError("发生错误：shader.dat文件不在清单内,请检查！");
        }

        /// <summary>
        /// 生成底包的资源清单文件
        /// </summary>
        /// <param name="versionManifestFilePath">Manifest路径</param>
        /// <param name="assetBundlePath">assetBundle相对路径</param>
        public static void CreatePackgeManifestFile(string versionHashFilePath)
        {
            string packageManifestPath = Application.streamingAssetsPath + "/AssetBundleManifest.txt";
            File.Copy(versionHashFilePath, packageManifestPath, true);
        }
    }
}
