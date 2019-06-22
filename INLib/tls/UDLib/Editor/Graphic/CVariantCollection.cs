using System.IO;
using UnityEditor;
using UnityEngine;

namespace UDLib.Editor
{
    public class CVariantCollection : EditorWindow
    {

        static private Transform root;
        static private string dirPath = "";

        static private ShaderVariantCollection sv;

        [MenuItem("图形/VariantCollection")] //% (ctrl) , # (shift), & (alt)
        static public void VariantCollection()
        {
            root = GameObject.Find("BattleFX").transform;

            //calculate select file's directory
            UnityEngine.Object[] arr = Selection.GetFiltered(typeof(UnityEngine.Object), SelectionMode.TopLevel);
            string selectFilePath = Application.dataPath.Substring(0, Application.dataPath.LastIndexOf('/')) + "/" + AssetDatabase.GetAssetPath(arr[0]);
            dirPath = selectFilePath.Substring(0, selectFilePath.LastIndexOf('/'));

            //Check special directory name
            string checkDirectoryName = dirPath.Substring(dirPath.LastIndexOf('/') + 1);
            if (checkDirectoryName != "BattleFX")
            {
                Debug.Log("You must instantiate the directory containing the FX prefabs");
                return;
            }

            // 1 Update Scene FX Prefabs. Please Open the scene Firstly
            _UpdateFXPrefabs();
            // 2 Save current scenes shadervariants
            _UpdateShaderVariants();
        }

        static private void _UpdateFXPrefabs()
        {
            int index = 0;
            string[] files = Directory.GetFiles(dirPath);
            foreach (string path in files)
            {
                EditorUtility.DisplayProgressBar("实例化中", files[index++], (float)index / (float)files.Length);
                if (Path.GetExtension(path) == ".prefab")
                {
                    string projectPath = path.Substring(path.IndexOf("Assets"));
                    Object o = AssetDatabase.LoadMainAssetAtPath(projectPath);
                    GameObject gameObject;
                    try
                    {
                        gameObject = (GameObject)o;
                        if (root.Find(gameObject.name) == false)
                        {
                            GameObject go = GameObject.Instantiate(gameObject);
                            go.name = gameObject.name;
                            go.transform.parent = root;
                        }
                    }
                    catch
                    {
                        Debug.Log("预制体 “" + projectPath + "“ 没有GameObject");
                    }
                }
            }
            EditorUtility.ClearProgressBar();
        }

        static private void _UpdateShaderVariants()
        {
            string[] files = Directory.GetFiles(dirPath);
            foreach (string path in files)
            {
                if (Path.GetExtension(path) == ".shadervariants")
                {
                    Object o = AssetDatabase.LoadMainAssetAtPath(path.Substring(path.IndexOf("Assets")));
                    try
                    {
                        sv = (ShaderVariantCollection)o;
                    }
                    catch
                    {

                    }
                    break;
                }
            }
        }
    }
}