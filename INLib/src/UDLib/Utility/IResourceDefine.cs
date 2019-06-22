using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UDLib.Utility
{
    public enum EBUILD_TYPE
    {
        SINGLE,
        FOLDER,
        PACK,
        HASH256
    };

    public interface IResourceDefine
    {
        EBUILD_TYPE[] GetABBuildTypes();
        string[][] GetABRootPaths();
        string[][] GetABSubPaths();
        string[][] GetABExts();
        string[] GetABSuffix();
        string ToAssetBundleName(int resourceType, string packageName = null);
        int GetResourceTypeMaxValue();
        string ResourceTypeToString(int resourceType);
        string ResourceTypeToHash256String(int resourceType, string name);
        bool IsPersistABInGame(string packageName);
        bool IsPersistABInScene(string packageName);
        bool IsPersistAtlasInGame(string packageName);
        bool IsSprite(int resourceType);
        bool IsScene(int resourceType);
        bool IsVideo(int resourceType);
    }
}
