using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;

public class COptimizeAnim
{
    private const float AnimationPositionError = 0.2f;
    private const float AnimationRotationError = 0.1f;
    private const ModelImporterAnimationCompression Compression =
        ModelImporterAnimationCompression.Optimal;
    private const int DecimalAccuracy = 10000;


    private static string _fbxPath;


    [MenuItem("Assets/Optimize/Optimize FBX", false, 1000)]
    private static void OptimizeFbx()
    {
        EditorSettings.serializationMode = SerializationMode.ForceText;
        var objs = Selection.GetFiltered(typeof(Object), SelectionMode.DeepAssets);
        var count = objs.Length;
        var index = 0;
        foreach (var obj in objs)
        {
            var isCancel = EditorUtility.DisplayCancelableProgressBar("优化FBX文件",
              string.Format("正在优化中...{0}/{1}", ++index, count), (float)index / count);
            if (isCancel)
            {
                AssetDatabase.Refresh();
                EditorUtility.ClearProgressBar();
                return;
            }
            OptimizeObject(obj);
        }
        AssetDatabase.Refresh();
        EditorUtility.ClearProgressBar();
    }

    [MenuItem("Assets/Optimize/检查选中动画的关键帧", false, 1000)]
    private static void CheckSelectedAnim()
    {
        List<string> brokenAnimations = new List<string>();
        EditorSettings.serializationMode = SerializationMode.ForceText;
        var objs = Selection.GetFiltered(typeof(AnimationClip), SelectionMode.DeepAssets);
        var count = objs.Length;
        var index = 0;
        foreach (var obj in objs)
        {
            var isCancel = EditorUtility.DisplayCancelableProgressBar("检查动画文件关键帧",
              string.Format("正在检查动画文件关键帧...{0}/{1}", ++index, count), (float)index / count);
            if (isCancel)
            {
                AssetDatabase.Refresh();
                EditorUtility.ClearProgressBar();
                return;
            }
            var path = AssetDatabase.GetAssetPath(obj);
            if (path != null && Path.GetExtension(path).ToLower() == ".anim")
            {
                _fbxPath = path;
                var anim = AssetDatabase.LoadAssetAtPath<AnimationClip>(path);
                CheckAnimationCurveData(Path.GetFileNameWithoutExtension(path), anim as AnimationClip, ref brokenAnimations);
            }
        }

        AssetDatabase.Refresh();
        EditorUtility.ClearProgressBar();
    }


    [MenuItem("Assets/Optimize/检查所有动画的关键帧", false, 1000)]
    private static void CheckAnim()
    {
        List<string> brokenAnimations = new List<string>();
        string checkPath = "Assets/App/Art/Character/Hero/Animation";
        var files = Directory.GetFiles(checkPath, "*.anim", SearchOption.AllDirectories);
        var count = files.Length;
        var index = 0;
        foreach (string file in files)
        {
            index++;
            if (file.EndsWith(".meta"))
                continue;

            string filepath = file.Replace("\\", "/");
            var isCancel = EditorUtility.DisplayCancelableProgressBar("检查动画文件关键帧",
                string.Format("正在检查动画文件关键帧...{0}/{1}", ++index, count), (float)index / count);
            var anim = AssetDatabase.LoadAssetAtPath<AnimationClip>(filepath);
            CheckAnimationCurveData(Path.GetFileNameWithoutExtension(filepath), anim as AnimationClip, ref brokenAnimations);
        }

        AssetDatabase.Refresh();
        EditorUtility.ClearProgressBar();
    }


    public static void OptimizeObject(Object obj)
    {
        var path = AssetDatabase.GetAssetPath(obj);
        if (path != null && Path.GetExtension(path).ToLower() == ".anim")
        {
            // OptimizeModleImporter(path);
            OptimizeAnimationClip(path);
        }
    }

    /// <summary>
    /// 设置fbx动画导入格式，默认AnimationPositionError = 0.2，AnimationRotationError = 0.1，根据项目调整，值越低优化压缩越少。
    /// </summary>
    /// <param name="fbxPath"></param>
    private static void OptimizeModleImporter(string fbxPath)
    {
        var modelImporter = AssetImporter.GetAtPath(fbxPath) as ModelImporter;
        if (modelImporter != null)
        {
            var isChange = false;
            if (Compression != modelImporter.animationCompression)
            {
                isChange = true;
                modelImporter.animationCompression = Compression;
                modelImporter.animationPositionError = AnimationPositionError;
                modelImporter.animationRotationError = AnimationRotationError;
                modelImporter.resampleCurves = false;
            }
            else
            {
                if (!Mathf.Approximately(modelImporter.animationPositionError, AnimationPositionError))
                {
                    isChange = true;
                    modelImporter.animationPositionError = AnimationPositionError;
                }
                if (!Mathf.Approximately(modelImporter.animationRotationError, AnimationRotationError))
                {
                    isChange = true;
                    modelImporter.animationRotationError = AnimationRotationError;
                }
                if (modelImporter.resampleCurves)
                {
                    isChange = true;
                    modelImporter.resampleCurves = false;
                }
            }
            if (isChange)
            {
                modelImporter.SaveAndReimport();
                AssetDatabase.Refresh();
            }
        }
    }

    public static void OptimizeAnimationClip(string animpath)
    {
        _fbxPath = animpath;
        AnimationClip anim = AssetDatabase.LoadAssetAtPath<AnimationClip>(animpath);
        OptimizeAnimationCurveData(anim, true);
    }

    public static void CheckAnimationCurveData(string fileName, AnimationClip clip, ref List<string> brokenAnimations)
    {
        if (clip == null)
        {
            Debug.LogError("No Clip error！" + "  " + _fbxPath);
            return;
        }

        AnimationClipCurveData[] curveDatas = AnimationUtility.GetAllCurves(clip, true);

        if (curveDatas == null || curveDatas.Length == 0)
        {
            Debug.LogError("No AnimationClipCurveData error!" + "  " + _fbxPath + "  " +
                   clip.name);
            return;
        }

        float _curentTime = 0;
        for (int ci = 0; ci < curveDatas.Length; ci++)
        {
            var dt = curveDatas[ci];
            var keys = dt.curve.keys;
            for (var i = 0; i < keys.Length; i++)
            {
                if (i != 0 && keys[i].time - _curentTime < 0.016)
                {
                    string brokenReason = string.Format("动画可能有问题， 名称 ：{0}， 骨骼:{1}, 参数:{2}", fileName, dt.path, dt.propertyName);
                    Debug.LogError(brokenReason);
                    brokenAnimations.Add(brokenReason);
                    return;
                }
                _curentTime = keys[i].time;
            }
        }
    }

    /// <summary>
    /// 优化动画片段，删除不需要的序列帧，并降低帧信息的精度
    /// </summary>
    /// <param name="clip"></param>
    /// <param name="folderPath"></param>
    /// <param name="isOverride">是否覆盖源文件</param>
    /// <returns></returns>
    public static AnimationClip OptimizeAnimationCurveData(AnimationClip clip, bool isOverride)
    {
        if (clip == null)
        {
            Debug.LogError("No Clip error！" + "  " + _fbxPath);
            return null;
        }

        Debug.Log("优化:" + clip.name);

        AnimationClipCurveData[] curveDatas = AnimationUtility.GetAllCurves(clip, true);

        if (curveDatas == null || curveDatas.Length == 0)
        {
            Debug.LogError("No AnimationClipCurveData error!" + "  " + _fbxPath + "  " +
                   clip.name);
            return null;
        }

        AnimationClip newClip = new AnimationClip();
        EditorUtility.CopySerialized(clip, newClip);

        newClip.name = clip.name;
        newClip.ClearCurves();

        for (int ci = 0; ci < curveDatas.Length; ci++)
        {
            var dt = curveDatas[ci];
            var nodeName = dt.path.ToLower().Split('/').Last();
            // 进行过滤
            if (IsFilterCurveData(dt, nodeName))
                continue;

            var keys = dt.curve.keys;
            for (var i = 0; i < keys.Length; i++)
            {
                keys[i].time = Mathf.Round(keys[i].time * DecimalAccuracy) / DecimalAccuracy;
                keys[i].value = Mathf.Round(keys[i].value * DecimalAccuracy) / DecimalAccuracy;
                keys[i].outTangent = Mathf.Round(keys[i].outTangent * DecimalAccuracy) / DecimalAccuracy;
                keys[i].inTangent = Mathf.Round(keys[i].inTangent * DecimalAccuracy) / DecimalAccuracy;
            }

            //过滤位移值没有变化的帧动画
            //因为帧信息有初始位置，所有要保留头尾两帧，如果全部删除会出现初始位置为默认值的问题
            if (IsFilterApproximateKeyFrame(ref keys) && keys.Length > 2)
            {
                var newKeys = new Keyframe[2];
                newKeys[0] = keys[0];
                newKeys[1] = keys[keys.Length - 1];
                keys = newKeys;
            }
            dt.curve.keys = keys;
            //设置新数据
            newClip.SetCurve(dt.path, dt.type, dt.propertyName, dt.curve);
        }

        if (isOverride)
        {
            EditorUtility.CopySerialized(newClip, clip);
            AssetDatabase.SaveAssets();
        }

        return newClip;
    }


    /// <summary>
    /// 过滤值一样的序列帧
    /// </summary>
    /// <param name="keys"></param>
    /// <returns></returns>
    private static bool IsFilterApproximateKeyFrame(ref Keyframe[] keys)
    {
        for (var i = 0; i < keys.Length - 1; i++)
        {
            if (Mathf.Abs(keys[i].value - keys[i + 1].value) > 0 ||
              Mathf.Abs(keys[i].outTangent - keys[i + 1].outTangent) > 0
              || Mathf.Abs(keys[i].inTangent - keys[i + 1].inTangent) > 0)
            {
                return false;
            }
        }
        return true;
    }


    /// <summary>
    /// 动画默认不导出Scale序列帧，除非该节点包含scale关键词(加scale关键词表示该节点需要进行scale变换)
    /// </summary>
    /// <param name="dt"></param>
    /// <returns></returns>
    private static bool IsFilterCurveData(AnimationClipCurveData dt, string nodeName)
    {
        bool isScale = dt.propertyName.ToLower().Contains("scale");
        var keys = dt.curve.keys;
        // 只有2个Key,且值都是1的也不过滤，之前启用有比较多的情况会动画异常
        //if (keys.Length == 2 && Mathf.Approximately(keys[0].value, 1.0f) && Mathf.Approximately(keys[1].value, 1.0f))
        //{
        //    // Debug.Log("ignore:"+ keys[0].value+","+ keys[1].value);
        //    return true;
        //}

        // if (dt.propertyName.ToLower().Contains("scale") && !nodeName.ToLower().Contains("scale"))
        //     return true;
        return false;
    }
}