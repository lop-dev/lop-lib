using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
namespace UDLib.Editor
{
    public class CCheckCollider
    {
        [MenuItem("Assets/程序/检查工具/检查所有角色的碰撞器", false, 02010021)]
        [MenuItem("程序/检查工具/检查所有角色的碰撞器", false, 02010021)]
        public static void CheckHaveCollider()
        {
            string dataPath = Application.dataPath.Replace("/", @"\");
            if (Directory.Exists(CPath.InstallPackAppProGameResCharacter))
            {
                DirectoryInfo direction = new DirectoryInfo(CPath.InstallPackAppProGameResCharacter);

                FileInfo[] files = direction.GetFiles("*prefab", SearchOption.AllDirectories);

                for (int i = 0; i < files.Length; i++)
                {
                    string prefabPath = files[i].FullName.Replace(dataPath, "Assets");
                    GameObject perfabObj = AssetDatabase.LoadAssetAtPath(prefabPath, typeof(GameObject)) as GameObject;
                    if (perfabObj.GetComponent<BoxCollider>() == null)
                    {
                        Debug.Log(prefabPath + "没有添加碰撞器，请自行添加~!");
                    }
                }
            }
        }

        [MenuItem("Assets/程序/检查工具/检查选中角色的碰撞器", false, 02010021)]
        [MenuItem("程序/检查工具/检查选中角色的碰撞器", false, 02010021)]
        public static void CheckChooseHaveCollider()
        {
            bool flag = false;
            GameObject go;
            object[] objArray = Selection.GetFiltered(typeof(object), SelectionMode.DeepAssets);
            foreach (object o in objArray)
            {
                if (o is GameObject)
                {
                    go = (GameObject)o;
                    GameObject perfabObj = AssetDatabase.LoadAssetAtPath(AssetDatabase.GetAssetPath(go), typeof(GameObject)) as GameObject;
                    if (perfabObj.GetComponent<BoxCollider>() == null)
                    {
                        flag = true;
                        Debug.Log(AssetDatabase.GetAssetPath(go) + "没有添加碰撞器，请自行添加~!");
                    }
                }
            }
            if (!flag)
            {
                Debug.Log("该文件下所有预制体都已添加碰撞器~");
            }
        }

    }
}

