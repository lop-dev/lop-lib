/* ***********************************************
 * author :  created by fanghy
 * function: 
 * history:  created at  2018/2/6 17:38:22
 * ***********************************************/

using System;
using UnityEngine;
using ACGNStudio;

namespace ACGNStudio
{
    public enum EMaterialType
    {
        EMT_Origin = 1,     //原本的材质
        EMT_Freeze = 2,     //冰冻
        EMT_Disappear = 3,  //消失
        EMT_Ghost = 4,      //灵魂形态（奢比死亡召唤物）
        EMT_InShadow = 5,   //葱茏隐于影召唤物
    }

    public class CResourceManager: CSLib.Utility.CSingleton<CResourceManager>
    {
        /// <summary>
        /// 加载一个资源
        /// </summary>
        /// <param name="eResourceType"></param>
        /// <param name="resource_name"></param>
        /// <param name="callback"></param>
        /// <param name="priority"></param>
        /// <param name="needInstantiate"></param>
        public void LoadPrefabAsync(UDLib.Resource.ERESOURCE_TYPE eResourceType, 
            string resource_name, 
            System.Action<UnityEngine.Object, System.Object> callback = null, 
            UDLib.Resource.CLoadPriority priority = UDLib.Resource.CLoadPriority.NORMAL, 
            bool needInstantiate = false, 
            System.Object args = null,
            UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal,
            bool isAsynMode = true)
        {
            new UDLib.Resource.CLoadObject((int)eResourceType, resource_name, (isOK, objs,loadedArgs)=> {
                UnityEngine.Object ret = null;
                if (isOK == true && objs.Length > 0)
                {
                    UnityEngine.Object unityObj = objs[0].obj;
                    if (null == unityObj)
                    {
                        UDLib.Utility.CDebugOut.LogError("load unityobj NULL!" + resource_name);
                    }
                    ret = needInstantiate == true ? GameObject.Instantiate(objs[0].obj) : objs[0].obj;
                }
                else
                {
                    // 被取消的加载
                    UDLib.Utility.CDebugOut.LogWarning("LoadPrefabAsync error, resource_name: " + resource_name);
                    return;
                }
                if (null != callback)
                    callback(ret, loadedArgs);
            }, priority, args, releaseType,isAsynMode);
        }

        public void LoadObjectAsync<T>(UDLib.Resource.ERESOURCE_TYPE eResourceType, 
                                        string resource_name,
                                        System.Action<T, System.Object> callback = null, 
                                        UDLib.Resource.CLoadPriority priority = UDLib.Resource.CLoadPriority.NORMAL, 
                                        bool needInstantiate = false, 
                                        System.Object args = null,
                                        UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal) 
                                        where T : UnityEngine.Object //Note this !
        {
            new UDLib.Resource.CLoadObject((int)eResourceType, resource_name, (isOK, objs, loadedArgs) =>
            {
                if (isOK == true && objs.Length > 0)
                {
                    UnityEngine.Object ret = needInstantiate == true ? GameObject.Instantiate(objs[0].obj) : objs[0].obj;
                    T obj = ret as T;
                    callback(obj, loadedArgs);
                }
                else
                {
                    UDLib.Utility.CDebugOut.LogError("LoadPrefabAsync error, resource_name: " + resource_name);
                }
            }, priority, args, releaseType);
        }

        /// <summary>
        /// 加载一组资源
        /// </summary>
        /// <param name="eResourceType"></param>
        /// <param name="package_name"></param>
        /// <param name="resource_names"></param>
        /// <param name="callback"></param>
        /// <param name="priority"></param>
        public void LoadPrefabsAsync(UDLib.Resource.ERESOURCE_TYPE eResourceType, 
            string package_name, 
            string[] resource_names, 
            System.Action<UDLib.Resource.CAssetObject[], System.Object> callback = null, 
            UDLib.Resource.CLoadPriority priority = UDLib.Resource.CLoadPriority.NORMAL, 
            System.Object args = null,
            UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal)
        {
            new UDLib.Resource.CLoadObject((int)eResourceType, package_name, resource_names, (isOK, objs,loadedArgs) => {
                if (isOK == true && objs.Length > 0)
                {
                    callback(objs, loadedArgs);
                }
                else
                {
                    UDLib.Utility.CDebugOut.LogError("LoadPrefabAsync error, resource_name: " + resource_names);
                }
            }, priority, args, releaseType);
        }
        
        public UDLib.Resource.CLoadObject LoadObjectsAsync<T>(UDLib.Resource.ERESOURCE_TYPE eResourceType,
                                        string package_name, 
                                        string[] resource_names,
                                        System.Action<T[], System.Object> callback = null,
                                        UDLib.Resource.CLoadPriority priority = UDLib.Resource.CLoadPriority.NORMAL,
                                        bool needInstantiate = false,
                                        System.Object args = null,
                                        UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal)
                                        where T : UnityEngine.Object//Note this !
        {
            return new UDLib.Resource.CLoadObject((int)eResourceType, package_name, resource_names, (isOK, objs, loadedArgs) =>
            {
                if (isOK == true && objs.Length > 0)
                {
                    T[] array = new T[objs.Length];

                    for (int i = 0; i < objs.Length; ++i)
                    {
                        if(objs[i] == null)
                        {
                               continue;
                        }
                        UnityEngine.Object ret = needInstantiate == true ? GameObject.Instantiate(objs[i].obj) : objs[i].obj;
                        array[i] = ret as T;
                    }
                    
                    callback(array, loadedArgs);
                }
                else
                {
                    UDLib.Utility.CDebugOut.LogWarning("LoadPrefabAsync failed, packageName：{0},resource_name:{1} ", package_name, resource_names[0]);
                }
            }, priority, args, releaseType);
        }

        //从folder里加载单个资源，主要是为了给lua调用，避免传数组
        public void LoadAPrefabAsyncFromFolder(UDLib.Resource.ERESOURCE_TYPE eResourceType, 
            string package_name, 
            string resource_name, 
            System.Action<UDLib.Resource.CAssetObject[],
            System.Object> callback = null,
            UDLib.Resource.CLoadPriority priority = UDLib.Resource.CLoadPriority.NORMAL, 
            System.Object args = null,
            UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal)
        {
            if (string.IsNullOrEmpty(resource_name))
            {
                UDLib.Utility.CDebugOut.LogError("resource_name不能为空！package_name= {0},resourceType = {1}", package_name, eResourceType.ToString());
                return;
            }

            new UDLib.Resource.CLoadObject((int)eResourceType, package_name, new string[1] { resource_name }, (isOK, objs, loadedArgs) => {
                if (isOK == true && objs.Length > 0)
                {
                    callback(objs, loadedArgs);
                }
                else
                {
                    UDLib.Utility.CDebugOut.LogError("LoadPrefabAsync error, resource_name: " + resource_name);
                }
            }, priority, args, releaseType);
        }


        public void LoadTextAssetAsync(UDLib.Resource.ERESOURCE_TYPE eResourceType,
                                string package_name,
                                string[] resource_names,
                                System.Action<TextAsset[], System.Object> callback = null,
                                UDLib.Resource.CLoadPriority priority = UDLib.Resource.CLoadPriority.NORMAL,
                                bool needInstantiate = false,
                                System.Object args = null,
                                UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal)
        {
            new UDLib.Resource.CLoadObject((int)eResourceType, package_name, resource_names, (isOK, objs, loadedArgs) =>
            {
                if (isOK == true && objs.Length > 0)
                {
                    TextAsset[] array = new TextAsset[objs.Length];

                    for (int i = 0; i < objs.Length; ++i)
                    {
                        if (objs[i].obj == null)
                            UDLib.Utility.CDebugOut.LogError("资源加载出错：{0}",objs[i].name);
                        UnityEngine.Object ret = needInstantiate == true ? GameObject.Instantiate(objs[i].obj) : objs[i].obj;
                        array[i] = ret as TextAsset;
                    }

                    callback(array, loadedArgs);
                }
                else
                {
                    UDLib.Utility.CDebugOut.LogError("LoadPrefabAsync error, resource_name: " + resource_names);
                }
            }, priority, args, releaseType);
        }

        ////
        /// 供lua层调用的UI_VFX加载 因为UI_VFX是单个文件打包。所以这里直接用vfxName作为包名
        ///
        //public void LoadUIVfx(string vfxName, GameObject parent, System.Action<GameObject> callback = null, UDLib.Resource.CReleaseType releaseType = UDLib.Resource.CReleaseType.Normal)
        //{
        //    new UDLib.Resource.CLoadObject((int)UDLib.Resource.ERESOURCE_TYPE.UI_VFX, vfxName, new string[1] { vfxName }, (isOK, objs, loadedArgs) =>
        //    {
        //        if (isOK == true && objs.Length > 0)
        //        {
        //            GameObject gobj = null;
        //            if (objs[0].obj != null)
        //            {
        //                gobj = GameObject.Instantiate(objs[0].obj) as GameObject;
        //                if (gobj != null)
        //                {
        //                    gobj.transform.SetParent(parent.transform);
        //                    gobj.transform.localPosition = Vector3.zero;
        //                    gobj.transform.localScale = Vector3.one;
        //                    gobj.transform.localEulerAngles = Vector3.zero;
        //                    if (null != callback)
        //                        callback(gobj);
        //                }
        //                else
        //                    UDLib.Utility.CDebugOut.LogError("Instantiate ui_vfx error, resource_name: " + vfxName);
        //            }
        //            else
        //                UDLib.Utility.CDebugOut.LogError("Load ui_vfx error, resource_name: " + vfxName);
        //        }
        //        else
        //        {
        //            UDLib.Utility.CDebugOut.LogError("Load ui_vfx error, resource_name: " + vfxName);
        //        }
        //    }, UDLib.Resource.CLoadPriority.NORMAL, null, releaseType);
        //}

        //[LuaInterface.NoToLua]
        public void LoadMaterial(EMaterialType matType, string matName, System.Action<Material, EMaterialType> loadedCallback)
        {
            LoadPrefabAsync(UDLib.Resource.ERESOURCE_TYPE.MATERIAL, matName, (obj, arg) =>
            {
                var mat = obj as Material;
                if (mat != null && null != loadedCallback)
                {
                    loadedCallback.Invoke(mat, matType);
                }
            }, needInstantiate: false, isAsynMode: false);
        }

        public void LoadScene(string sceneName,System.Action callback)
        {
            new UDLib.Resource.CLoadObject().LoadScene((int)UDLib.Resource.ERESOURCE_TYPE.SCENE, sceneName, callback);
        }

        public void DoGC()
        {
            Resources.UnloadUnusedAssets();
            GC.Collect();
        }

        public void ReleaseAssetbundle(UDLib.Resource.ERESOURCE_TYPE resourceType, string packageName)
        {
            UDLib.Resource.MbAssetBundleManager.Instance.ReleaseAssetbundle((int)resourceType, packageName);
        }

        public void ReleaseUIAtlas(string packageName)
        {
            UDLib.Utility.CDebugOut.Log("ReleaseUIAtlas >> packageName : " + packageName);
            UDLib.Resource.MbAssetBundleManager.Instance.ReleaseUIAtlas(packageName);
        }

        // warnup Shader varible collection, shaderCollectionName SVC名称： 如battlefx
        public void WarmUpShaderCollection(string shaderCollectionName, System.Action callback)
        {
            new UDLib.Resource.CLoadObject((int)UDLib.Resource.ERESOURCE_TYPE.SHADER, "shader", new string[1] { shaderCollectionName }, (isOK, objs, loadedArgs) => {
                if (isOK == true && objs.Length > 0)
                {
                    var svc = (ShaderVariantCollection)objs[0].obj;
                    if (svc != null)
                    {
                        svc.WarmUp();
                    }
                }
                else
                {
                    UDLib.Utility.CDebugOut.LogError("LoadPrefabAsync error, resource_name:battlefx ");
                }

                if(callback != null)
                {
                    callback();
                }
            });
        }
    }
}
