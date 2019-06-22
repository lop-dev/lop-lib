using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEditor;
using UnityEngine;
using Object = System.Object;

namespace ACGNStudio
{
    class CResourceSavePostProcess : UnityEditor.AssetPostprocessor
    {
        static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
        {
            foreach (string str in importedAssets)
            {
                string lowerStr = str.ToLower();
                if (lowerStr.EndsWith(".fbx"))
                {
                    ModelImporter mi = ModelImporter.GetAtPath(str) as ModelImporter;
                    if (mi == null)
                        continue;
                    if (mi.isReadable == true)
                    {
                        mi.isReadable = false;
                    }

                    // 关闭Mesh Compression
                    if (mi.meshCompression != ModelImporterMeshCompression.Off)
                    {
                        mi.meshCompression = ModelImporterMeshCompression.Off;
                    }

                    //import Material
                    if (mi.importMaterials == true)
                    {
                        mi.importMaterials = false;
                    }
                }
                else if (lowerStr.EndsWith(".psd") || lowerStr.EndsWith(".tga") ||
                        lowerStr.EndsWith(".exr") || lowerStr.EndsWith(".png"))
                {
                    if (str.ToLower().Contains("t4m"))
                        continue;
                    UDLib.Editor.CCheckAccessRights.SetTextureIsReadable(str, false);
                    Debug.Log(" Texture : " + str + " , has set isReadable false.");
                }
            }
        }
    }
}
