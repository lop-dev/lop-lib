using System.Collections.Generic;

namespace ACGNStudio
{
    public class CResourceDefine : UDLib.Utility.IResourceDefine
    {
        public UDLib.Utility.EBUILD_TYPE[] GetABBuildTypes()
        {
            return UDLib.Resource.CResourcePath.buildTypes;
        }

        public string[][] GetABRootPaths()
        {
            return UDLib.Resource.CResourcePath.rootPaths;
        }

        public string[][] GetABSubPaths()
        {
            return UDLib.Resource.CResourcePath.subPaths;
        }

        public string[][] GetABExts()
        {
            return UDLib.Resource.CResourcePath.exts;
        }

        public string[] GetABSuffix()
        {
            return UDLib.Resource.CResourcePath.suffix;
        }

        public int GetResourceTypeMaxValue()
        {
            return (int)UDLib.Resource.ERESOURCE_TYPE.MAX;
        }

        public string ResourceTypeToString(int resourceType)
        {
            return ((UDLib.Resource.ERESOURCE_TYPE)resourceType).ToString();
        }

        public string ResourceTypeToHash256String(int resourceType, string name)
        {
            return _ResourceTypeToHash256String((UDLib.Resource.ERESOURCE_TYPE)resourceType, name);
        }

        private string _ResourceTypeToHash256String(UDLib.Resource.ERESOURCE_TYPE eResourceType, string name)
        {
            return string.Format("{0}_{1:X2}", eResourceType.ToString(), (byte)CSLib.Security.CHash.GetBKDRHash(name));
        }

        public string ToAssetBundleName(int resourceType, string packageName = null)
        {
            return _ToAssetBundleName((UDLib.Resource.ERESOURCE_TYPE)resourceType, packageName);
        }

        private string _ToAssetBundleName(UDLib.Resource.ERESOURCE_TYPE eResourceType, string packageName = null)
        {
            string asset_bundle_name = eResourceType.ToString();

            if (GetABBuildTypes()[(int)eResourceType] != UDLib.Utility.EBUILD_TYPE.PACK)
            {
                asset_bundle_name += "/" + packageName;
            }

            asset_bundle_name += GetABSuffix()[(int)eResourceType] + ".dat";

            return asset_bundle_name.ToLower();
        }

        /// <summary>
        /// 判断是否是整个游戏生命周期的常驻AB
        /// </summary>
        /// <param name="packageName"></param>
        /// <returns></returns>
        public bool IsPersistABInGame(string packageName)
        {
            return m_setPersistABInGame.Contains(packageName);
        }

        private static HashSet<string> m_setPersistABInGame = new HashSet<string>
        {
            // 通用图集
            "ui_image/common.dat",
            // item图集
            "ui_image/itemicon.dat",
            // 通用提示框
            "ui_prefabs/msgpopupui.dat",
            // 字体文件
            "font/songheavy.dat",
        };

        /// <summary>
        /// 判断是否是场景生命周期的常驻AB
        /// </summary>
        /// <param name="packageName"></param>
        /// <returns></returns>
        public bool IsPersistABInScene(string packageName)
        {
            return m_setPersistABInScene.Contains(packageName);
        }

        private static HashSet<string> m_setPersistABInScene = new HashSet<string>
        {
            // 特效通用
            "vfx_image/frequent.dat",
        };

        /// <summary>
        /// 判断是否是整个游戏生命周期的常驻图集
        /// </summary>
        /// <param name="packageName"></param>
        /// <returns></returns>
        public bool IsPersistAtlasInGame(string packageName)
        {
            return m_setPersistAtlasInGame.Contains(packageName);
        }

        private static HashSet<string> m_setPersistAtlasInGame = new HashSet<string>
        {
            "ui_image/common.dat"
        };

        //图集ab
        private static HashSet<UDLib.Resource.ERESOURCE_TYPE> m_spriteTypes = new HashSet<UDLib.Resource.ERESOURCE_TYPE>
        {
            //UDLib.Resource.ERESOURCE_TYPE.UI_IMAGE,
            //UDLib.Resource.ERESOURCE_TYPE.ROLEHEAD_ROUND,
            //UDLib.Resource.ERESOURCE_TYPE.ROLE_ICON,
            //UDLib.Resource.ERESOURCE_TYPE.UI_BG
        };

        public bool IsSprite(int resourceType)
        {
            return m_spriteTypes.Contains((UDLib.Resource.ERESOURCE_TYPE)resourceType);
        }

        public bool IsScene(int resourceType)
        {
            return UDLib.Resource.ERESOURCE_TYPE.SCENE == (UDLib.Resource.ERESOURCE_TYPE)resourceType;
        }

        public bool IsVideo(int resourceType)
        {
            return UDLib.Resource.ERESOURCE_TYPE.VIDEO == (UDLib.Resource.ERESOURCE_TYPE)resourceType;
        }
    }
}
