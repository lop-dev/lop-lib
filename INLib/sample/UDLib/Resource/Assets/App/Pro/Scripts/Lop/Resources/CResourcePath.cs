// ※此脚本由AssetBundleEnvironment.CreateResourceDefine()自动生成
using UDLib.Utility;

namespace UDLib.Resource
{
  // 资源类型
  public enum ERESOURCE_TYPE
  {
    SCENE,
    VIDEO,

    MAX
  };

  // 资源定义
  public static class CResourcePath
  {
      // CATEGORY_TYPE数组
      public static readonly EBUILD_TYPE[] buildTypes = new EBUILD_TYPE[]
      {
          EBUILD_TYPE.SINGLE, // SCENE
          EBUILD_TYPE.SINGLE, // VIDEO
      };
      // 用于添加至AB包名的后缀
      public static readonly string[] suffix = new string[]
      {
          "", // SCENE
          "", // VIDEO
      };

      // _ASSERT_BUNDLE_txt数组
      public static readonly string[][] rootPaths = new string[][]
      {
          new string[] // SCENE
          {
              "assets/app/pro/gameres/scenes/",
          },
          new string[] // VIDEO
          {
              "assets/app/pro/gameres/video/",
          },
      };
      // Resources.Load()使用的路径数组
      public static readonly string[][] subPaths = new string[][]
      {
          new string[] // SCENE
          {
              "assets/app/pro/gameres/scenes/",
          },
          new string[] // VIDEO
          {
              "assets/app/pro/gameres/video/",
          },
      };
      // 对象扩展名数组
      public static readonly string[][] exts = new string[][]
      {
          new string[] // SCENE
          {
              ".unity",
          },
          new string[] // VIDEO
          {
              ".mp4",
          },
      };
  };
}
