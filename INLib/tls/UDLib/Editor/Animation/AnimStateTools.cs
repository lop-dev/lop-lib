using UnityEngine;
using UnityEditor;
using UDLib.Editor;
using System.IO;
using UnityEditor.Animations;

public class AnimStateTools : EditorWindow {

    [MenuItem("程序/动画工具/动画状态工具", false, 02060000)]
    private static void Init()
    {
        var window = GetWindow(typeof(AnimStateTools), false, "动画状态工具");
        window.Show();
    }

    private GUIStyle mStyle = new GUIStyle();

    void OnGUI()
    {
        mStyle = GUI.skin.button;
        mStyle.fontStyle = FontStyle.Italic;
        mStyle.fontSize = 16;

        GUILayout.BeginVertical();
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("检查所有的动画状态机的空动画", mStyle,GUILayout.Height(40)))
        {
            CheckAllAnimatorController();
        }
        GUILayout.EndHorizontal();
        GUILayout.EndVertical();

        GUILayout.BeginVertical();
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("检查选中的动画状态机的空动画", mStyle, GUILayout.Height(40)))
        {
            CheckSelectedAnimatorController();
        }
        GUILayout.EndHorizontal();
        GUILayout.EndVertical();


        GUILayout.BeginVertical();
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("查找动画状态机中的空脚本",mStyle,GUILayout.Height(40)))
        {
            CheckAllAniCtrlScript();
        }
        GUILayout.EndHorizontal();
        GUILayout.EndVertical();

        GUILayout.BeginVertical();
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("设置所有Idle、Run、Sleep、Coma动画为循环播放", mStyle, GUILayout.Height(40)))
        {
            SetAniCtrlNodesLoopTime();
        }
        GUILayout.EndHorizontal();
        GUILayout.EndVertical();

        GUILayout.BeginVertical();
        GUILayout.BeginHorizontal();
        if (GUILayout.Button("检查动画状态机中战斗所需的基本状态是否存在", mStyle, GUILayout.Height(40)))
        {
            CheckAnimCtrlBaseStates();
        }
        GUILayout.EndHorizontal();
        GUILayout.EndVertical();
    }

    #region 检查战斗状态

    public static string[] fightBaseStates = { "Invalid", "Sleep", "Coma", "Damage", "Debut", "Victory", "Dead" };
    private static void CheckAnimCtrlBaseStates()
    {
        string[] files = Directory.GetFiles(CPath.AssetsAppProGameResCharacter, "*.controller", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int index = 0;
        foreach (string f in files)
        {
            CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
            index++;
            if (f.Contains("Battle") || f.Contains("battle"))
            {
                CheckAnimCtrlBaseState(f);
            }
        }
        CCheckTools.ClearProgressBar();
    }

    private static void CheckAnimCtrlBaseState(string path)
    {
        if (!File.Exists(path))
        {
            Debug.LogError("文件不存在：" + path);
            return;
        }
        
        path = path.Replace('\\', '/');
        AnimatorController controller = AssetDatabase.LoadAssetAtPath<AnimatorController>(path);
        if (null == controller)
            return;
        foreach (AnimatorControllerLayer layer in controller.layers)
        {
            foreach (var state in fightBaseStates)
            {
                if (!HasState(state, layer.stateMachine.states))
                {
                    Debug.LogError(string.Concat("Message:", path, "-->:", state, "  状态丢失！"));
                }
            }
        }
    }

    public static bool HasState(string stateName, ChildAnimatorState[] childAniStates)
    {
        foreach (var childAniState in childAniStates)
        {
            if (stateName.Equals(childAniState.state.name))
                return true;
        }
        return false;
    }

    #endregion

    #region 设置动画循环

    private static void SetAniCtrlNodesLoopTime()
    {
        string[] files = Directory.GetFiles(CPath.AssetsAppArt, "*.anim", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int index = 0;
        foreach (string f in files)
        {
            CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
            index++;
            CheckSingleNode(f);
        }
        CCheckTools.ClearProgressBar();
    }

    private static void CheckSingleNode(string path)
    {
        string lowerPath = path.ToLower();
        if (lowerPath.IndexOf("idle") < 0 && lowerPath.IndexOf("run") < 0 && lowerPath.IndexOf("sleep") < 0 && lowerPath.IndexOf("coma") < 0)
            return;
        path = path.Replace('\\', '/');
        AnimationClip controller = AssetDatabase.LoadAssetAtPath<AnimationClip>(path);
        if (null == controller)
            return;
        AnimationClipSettings clipsetting = AnimationUtility.GetAnimationClipSettings(controller);
        if (!clipsetting.loopTime)
            Debug.Log(string.Concat("Message:", path, "-->:", "修改设置"));
        clipsetting.loopTime = true;
        AnimationUtility.SetAnimationClipSettings(controller, clipsetting);
    }

    #endregion

    #region 检查动画状态机

    public static string[] motionIgnore = { "Invalid" };      //忽略节点
    public static int fileCount;

    public static void CheckAllAnimatorController()
    {
        string[] files = Directory.GetFiles(CPath.AssetsAppProGameResCharacter, "*.controller", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int index = 0;
        foreach (string f in files)
        {
            CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
            index++;
            CheckSingleAniCtrl(f);
        }
        CCheckTools.ClearProgressBar();
    }

    public static void CheckSelectedAnimatorController()
    {
        string[] files = null;
        string defFile = "";
        CCheckTools.GetSelectedFilePath(ref files, ref defFile, "*.controller");
        int curIndex = 0;
        if (files != null && files.Length > 0)
        {
            fileCount = files.Length;
            foreach (string s in files)
            {
                curIndex++;
                CCheckTools.DisplayProgressBar("检查中", s, curIndex / (float)fileCount);
                CheckSingleAniCtrl(s);
            }
            CCheckTools.ClearProgressBar();
        }
        else if (!string.IsNullOrEmpty(defFile) && defFile.EndsWith(".controller"))
        {
            CheckSingleAniCtrl(defFile);
        }
        else
        {
            Debug.LogError("未选择任何controller文件");
        }
    }

    public static void CheckSingleAniCtrl(string path)
    {
        if (!File.Exists(path))
        {
            Debug.LogError("文件不存在：" + path);
            return;
        }
        path = path.Replace('\\', '/');
        AnimatorController controller = AssetDatabase.LoadAssetAtPath<AnimatorController>(path);
        if (null == controller)
            return;
        foreach (AnimatorControllerLayer layer in controller.layers)
        {
            foreach (ChildAnimatorState childAniState in layer.stateMachine.states)
            {
                if (!isMotionIgnore(childAniState.state.name) && null == childAniState.state.motion)
                {
                    Debug.LogError(string.Concat("Message:", path, "-->:", childAniState.state.name, "  动画丢失！"));
                }
            }
        }
    }

    public static bool isMotionIgnore(string stateName)
    {
        foreach (string s in motionIgnore)
        {
            if (stateName.Equals(s))
                return true;
        }
        return false;
    }

    #endregion

    #region 检查状态脚本

    public static void CheckAllAniCtrlScript()
    {
        string[] files = Directory.GetFiles(CPath.AssetsAppProGameResCharacter, "*.controller", SearchOption.AllDirectories);
        int fileCount = files.Length;
        int index = 0;
        foreach (string f in files)
        {
            CCheckTools.DisplayProgressBar("处理中", f, index / (float)fileCount);
            index++;
            CheckAnimCtrlScript(f);
        }
        CCheckTools.ClearProgressBar();
    }

    public static void CheckAnimCtrlScript(string path)
    {
        if (!File.Exists(path))
        {
            Debug.LogError("文件不存在：" + path);
            return;
        }
        path = path.Replace('\\', '/');
        AnimatorController controller = AssetDatabase.LoadAssetAtPath<AnimatorController>(path);
        if (null == controller)
            return;
        foreach (AnimatorControllerLayer layer in controller.layers)
        {
            foreach (ChildAnimatorState childAniState in layer.stateMachine.states)
            {
                bool hasBehaviours = false;
                foreach (StateMachineBehaviour behaviour in childAniState.state.behaviours)
                {
                    if (behaviour.ToString() == " (ACGNStudio.CAnimatorState)")
                        hasBehaviours = true;
                }
                if (!hasBehaviours)
                {
                    Debug.LogError(string.Concat("Message:", path, "-->:", childAniState.state.name, "  脚本丢失！"));
                }
            }
        }
    }

    #endregion
}
