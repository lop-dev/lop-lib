// ※此脚本由AssetBundleEnvironment.CreateResourceDefine()自动生成
using UDLib.Utility;

namespace UDLib.Resource
{
  // 资源类型
  public enum ERESOURCE_TYPE
  {
    MATERIAL,
    SCENE,
    SHADER,
    UI_VFX,
    VIDEO,

    MAX
  };

  // 资源定义
  public static class CResourcePath
  {
      // CATEGORY_TYPE数组
      public static readonly EBUILD_TYPE[] buildTypes = new EBUILD_TYPE[]
      {
          EBUILD_TYPE.SINGLE, // MATERIAL
          EBUILD_TYPE.SINGLE, // SCENE
          EBUILD_TYPE.PACK, // SHADER
          EBUILD_TYPE.SINGLE, // UI_VFX
          EBUILD_TYPE.SINGLE, // VIDEO
      };
      // 用于添加至AB包名的后缀
      public static readonly string[] suffix = new string[]
      {
          "", // MATERIAL
          "", // SCENE
          "", // SHADER
          "", // UI_VFX
          "", // VIDEO
      };

      // _ASSERT_BUNDLE_txt数组
      public static readonly string[][] rootPaths = new string[][]
      {
          new string[] // MATERIAL
          {
              "assets/app/pro/gameres/material/",
          },
          new string[] // SCENE
          {
              "assets/app/pro/gameres/scenes/",
          },
          new string[] // SHADER
          {
              "assets/app/pro/gameres/shaders/",
          },
          new string[] // UI_VFX
          {
              "assets/app/pro/gameres/effects/uifx/",
          },
          new string[] // VIDEO
          {
              "assets/app/pro/gameres/video/",
          },
      };
      // Resources.Load()使用的路径数组
      public static readonly string[][] subPaths = new string[][]
      {
          new string[] // MATERIAL
          {
              "assets/app/pro/gameres/material/",
          },
          new string[] // SCENE
          {
              "assets/app/pro/gameres/scenes/",
          },
          new string[] // SHADER
          {
              "assets/app/pro/gameres/shaders/",
          },
          new string[] // UI_VFX
          {
              "assets/app/pro/gameres/effects/uifx/",
          },
          new string[] // VIDEO
          {
              "assets/app/pro/gameres/video/",
          },
      };
      // 对象扩展名数组
      public static readonly string[][] exts = new string[][]
      {
          new string[] // MATERIAL
          {
              ".mat",
          },
          new string[] // SCENE
          {
              ".unity",
          },
          new string[] // SHADER
          {
              ".shader",
              ".cginc",
          },
          new string[] // UI_VFX
          {
              ".prefab",
          },
          new string[] // VIDEO
          {
              ".mp4",
          },
      };
  };
}
