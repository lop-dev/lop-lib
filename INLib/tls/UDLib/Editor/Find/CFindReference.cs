using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
namespace UDLib.Editor
{
    public class CFindReference : EditorWindow
    {
        static private CFindReference instance;

        private Vector2 m_mScroll = Vector2.zero;
        private Dictionary<string, BetterList<string>> m_dict;

        void OnEnable() { instance = this; }
        void OnDisable() { instance = null; }

        protected void OnGUI()
        {
            if (m_dict == null)
            {
                return;
            }

            m_mScroll = GUILayout.BeginScrollView(m_mScroll);

            BetterList<string> list = m_dict["prefab"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Prefab"))
                {
                    foreach (string item in list)
                    {
                        GameObject go = AssetDatabase.LoadAssetAtPath(item, typeof(GameObject)) as GameObject;
                        EditorGUILayout.ObjectField("Prefab", go, typeof(GameObject), false);
                    }
                }
                list = null;
            }

            list = m_dict["fbx"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("FBX"))
                {
                    foreach (string item in list)
                    {
                        GameObject go = AssetDatabase.LoadAssetAtPath(item, typeof(GameObject)) as GameObject;
                        EditorGUILayout.ObjectField("FBX", go, typeof(GameObject), false);
                    }
                }
                list = null;
            }

            list = m_dict["cs"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Script"))
                {
                    foreach (string item in list)
                    {
                        MonoScript go = AssetDatabase.LoadAssetAtPath(item, typeof(MonoScript)) as MonoScript;
                        EditorGUILayout.ObjectField("Script", go, typeof(MonoScript), false);
                    }
                }
                list = null;
            }

            list = m_dict["texture"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Texture"))
                {
                    foreach (string item in list)
                    {
                        Texture2D go = AssetDatabase.LoadAssetAtPath(item, typeof(Texture2D)) as Texture2D;
                        EditorGUILayout.ObjectField("Texture:" + go.name, go, typeof(Texture2D), false);
                    }
                }
                list = null;
            }

            list = m_dict["mat"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Material"))
                {
                    foreach (string item in list)
                    {
                        Material go = AssetDatabase.LoadAssetAtPath(item, typeof(Material)) as Material;
                        EditorGUILayout.ObjectField("Material", go, typeof(Material), false);
                    }
                }
                list = null;
            }

            list = m_dict["shader"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Shader"))
                {
                    foreach (string item in list)
                    {
                        Shader go = AssetDatabase.LoadAssetAtPath(item, typeof(Shader)) as Shader;
                        EditorGUILayout.ObjectField("Shader", go, typeof(Shader), false);
                    }
                }
                list = null;
            }

            list = m_dict["font"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Font"))
                {
                    foreach (string item in list)
                    {
                        Font go = AssetDatabase.LoadAssetAtPath(item, typeof(Font)) as Font;
                        EditorGUILayout.ObjectField("Font", go, typeof(Font), false);
                    }
                }
                list = null;
            }

            list = m_dict["anim"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Animation"))
                {
                    foreach (string item in list)
                    {
                        AnimationClip go = AssetDatabase.LoadAssetAtPath(item, typeof(AnimationClip)) as AnimationClip;
                        EditorGUILayout.ObjectField("Animation:", go, typeof(AnimationClip), false);
                    }
                }
                list = null;
            }

            list = m_dict["animTor"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Animator"))
                {
                    foreach (string item in list)
                    {
                        //Animator go = AssetDatabase.LoadAssetAtPath(item, typeof(Animator)) as Animator;
                        //EditorGUILayout.ObjectField("Animator:", go, typeof(Animator), true);
                        EditorGUILayout.LabelField(item);
                    }
                }
                list = null;
            }

            list = m_dict["level"];
            if (list != null && list.size > 0)
            {
                if (CGUIEditorTools.DrawHeader("Level"))
                {
                    foreach (string item in list)
                    {
                        //SceneView go = AssetDatabase.LoadAssetAtPath(item, typeof(SceneView)) as SceneView;
                        //EditorGUILayout.ObjectField("Animation:" , go, typeof(SceneView), true);
                        EditorGUILayout.LabelField(item);
                    }
                }
                list = null;
            }

            GUILayout.EndScrollView();
        }

        public static bool ShowProgress(float val, int total, int cur)
        {
            return EditorUtility.DisplayCancelableProgressBar("Searching", string.Format("Finding ({0}/{1}), please wait...", cur, total), val);
        }

        /// <summary>
        /// 查找选中文件的引用文件
        /// </summary>
        [MenuItem("程序/检查工具/查找选中文件被哪些文件引用",false,02010101)]
        [MenuItem("Assets/程序/检查工具/查找选中文件被哪些文件引用", false, 02010101)]
        public static void FindScriptReference()
        {
            _FindScriptReference();
        }

        private static void _FindScriptReference()
        {
            string curPathName = AssetDatabase.GetAssetPath(Selection.activeObject.GetInstanceID());
            FindReferenceForFile(curPathName, true);
        }

        public static List<string> FindReferenceForFile(string pathName, bool showResultDialog = false)
        {
            ShowProgress(0, 0, 0);
            List<string> referenceList = new List<string>();
            BetterList<string> prefabList = new BetterList<string>();
            BetterList<string> fbxList = new BetterList<string>();
            BetterList<string> scriptList = new BetterList<string>();
            BetterList<string> textureList = new BetterList<string>();
            BetterList<string> matList = new BetterList<string>();
            BetterList<string> shaderList = new BetterList<string>();
            BetterList<string> fontList = new BetterList<string>();
            BetterList<string> levelList = new BetterList<string>();

            string[] allGuids = AssetDatabase.FindAssets("t:Prefab t:Scene", new string[] { "Assets" });

            int i = 0;
            foreach (string guid in allGuids)
            {
                string assetPath = AssetDatabase.GUIDToAssetPath(guid);
                string[] names = AssetDatabase.GetDependencies(new string[] { assetPath });  // 依赖的东东
                foreach (string name in names)
                {
                    if (name.Equals(pathName))
                    {
                        referenceList.Add(assetPath);
                        UDLib.Utility.CDebugOut.Log("Refer : " + assetPath);
                        if (assetPath.EndsWith(".prefab"))
                        {
                            prefabList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.ToLower().EndsWith(".fbx"))
                        {
                            fbxList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.ToLower().EndsWith(".unity"))
                        {
                            levelList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.EndsWith(".cs"))
                        {
                            scriptList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.EndsWith(".png"))
                        {
                            textureList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.EndsWith(".mat"))
                        {
                            matList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.EndsWith(".shader"))
                        {
                            shaderList.Add(assetPath);
                            break;
                        }
                        else if (assetPath.EndsWith(".ttf"))
                        {
                            fontList.Add(assetPath);
                            break;
                        }
                    }
                }
                if(ShowProgress((float)i / (float)allGuids.Length, allGuids.Length, i))
                {
                    Debug.Log("取消查找");
                    break;
                }
                i++;
            }

            Dictionary<string, BetterList<string>> dic = new Dictionary<string, BetterList<string>>();
            dic.Add("prefab", prefabList);
            dic.Add("fbx", fbxList);
            dic.Add("cs", scriptList);
            dic.Add("texture", textureList);
            dic.Add("mat", matList);
            dic.Add("shader", shaderList);
            dic.Add("font", fontList);
            dic.Add("level", levelList);
            dic.Add("anim", null);
            dic.Add("animTor", null);

            // 是否显示GUI
            if (showResultDialog)
            {
                EditorWindow.GetWindow<CFindReference>(false, "Object Reference", true).Show();
                if (CFindReference.instance.m_dict != null)
                {
                    CFindReference.instance.m_dict.Clear();
                }

                CFindReference.instance.m_dict = dic;
            }

            EditorUtility.ClearProgressBar();
            return referenceList;
        }

        /// <summary>
        /// 查找选中文件的依赖文件
        /// </summary>
        [MenuItem("程序/检查工具/查找选中文件依赖于哪些文件",false, 02010101)]
        [MenuItem("Assets/程序/检查工具/查找选中文件依赖于哪些文件", false, 02010101)]
        public static void FindObjectDependencies()
        {
            _FindObjectDependencies();
        }

        private static void _FindObjectDependencies()
        {
            ShowProgress(0, 0, 0);

            Dictionary<string, BetterList<string>> dic = new Dictionary<string, BetterList<string>>();
            BetterList<string> prefabList = new BetterList<string>();
            BetterList<string> fbxList = new BetterList<string>();
            BetterList<string> scriptList = new BetterList<string>();
            BetterList<string> textureList = new BetterList<string>();
            BetterList<string> matList = new BetterList<string>();
            BetterList<string> shaderList = new BetterList<string>();
            BetterList<string> fontList = new BetterList<string>();
            BetterList<string> animList = new BetterList<string>();
            BetterList<string> animTorList = new BetterList<string>();

            string curPathName = AssetDatabase.GetAssetPath(Selection.activeObject.GetInstanceID());
            string[] names = AssetDatabase.GetDependencies(new string[] { curPathName });  // 依赖的东东

            int i = 0;
            foreach (string name in names)
            {
                if (name.EndsWith(".prefab"))
                {
                    prefabList.Add(name);
                }
                else if (name.ToLower().EndsWith(".fbx"))
                {
                    fbxList.Add(name);
                }
                else if (name.EndsWith(".cs"))
                {
                    scriptList.Add(name);
                }
                else if (name.EndsWith(".png"))
                {
                    textureList.Add(name);
                }
                else if (name.EndsWith(".mat"))
                {
                    matList.Add(name);
                }
                else if (name.EndsWith(".shader"))
                {
                    shaderList.Add(name);
                }
                else if (name.EndsWith(".ttf"))
                {
                    fontList.Add(name);
                }
                else if (name.EndsWith(".anim"))
                {
                    animList.Add(name);
                }
                else if (name.EndsWith(".controller"))
                {
                    animTorList.Add(name);
                }
                Debug.Log("Dependence:" + name);
                if(ShowProgress((float)i / (float)names.Length, names.Length, i))
                {
                    Debug.Log("取消查找");
                    break;
                }
                i++;
            }

            dic.Add("prefab", prefabList);
            dic.Add("fbx", fbxList);
            dic.Add("cs", scriptList);
            dic.Add("texture", textureList);
            dic.Add("mat", matList);
            dic.Add("shader", shaderList);
            dic.Add("font", fontList);
            dic.Add("level", null);
            dic.Add("animTor", animTorList);
            dic.Add("anim", animList);

            EditorWindow.GetWindow<CFindReference>(false, "Object Dependencies", true).Show();
            if (CFindReference.instance.m_dict != null)
            {
                CFindReference.instance.m_dict.Clear();
            }
            CFindReference.instance.m_dict = dic;

            EditorUtility.ClearProgressBar();
        }
    }
}
