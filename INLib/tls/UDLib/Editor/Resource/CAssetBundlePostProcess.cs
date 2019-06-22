using UnityEngine;
using UnityEditor;
using System.Collections;
using System.IO;
using UDLib.Utility;

namespace UDLib.Editor
{
    public class CAssetBundlePostProcess : AssetPostprocessor
    {

        static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
        {
            foreach (string asset_path in importedAssets)
            {
                string file_name = Path.GetFileName(asset_path);
                if (file_name == "_ASSET_BUNDLE_.txt")
                {
                    CDebugOut.Log(asset_path + "变更了");
                    CAssetBundleEditor.CreateResourcePath();
                    return;
                }
            }

            foreach (string asset_path in deletedAssets)
            {
                string file_name = Path.GetFileName(asset_path);
                if (file_name == "_ASSET_BUNDLE_.txt")
                {
                    CDebugOut.Log(asset_path + "删除了");
                    CAssetBundleEditor.CreateResourcePath();
                    return;
                }
            }

            foreach (string asset_path in movedAssets)
            {
                string file_name = Path.GetFileName(asset_path);
                if (file_name == "_ASSET_BUNDLE_.txt")
                {
                    CDebugOut.Log(asset_path + "移动了");
                    CAssetBundleEditor.CreateResourcePath();
                    return;
                }
            }

            foreach (string asset_path in movedFromAssetPaths)
            {
                string file_name = Path.GetFileName(asset_path);
                if (file_name == "_ASSET_BUNDLE_.txt")
                {
                    CDebugOut.Log(asset_path + "路径变更了");
                    CAssetBundleEditor.CreateResourcePath();
                    return;
                }
            }
        }
    }
}