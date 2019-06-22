using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
namespace UDLib.Editor
{
    public class ReferenceAsset : ScriptableObject
    {
        public List<ReferenceInfo> infoList;
    }

    [System.Serializable]
    public class ReferenceInfo
    {
        /// <summary>
        /// 资源路径
        /// </summary>
        public string path;
        /// <summary>
        /// 资源guid
        /// </summary>
        public string guid;
        /// <summary>
        /// 被XX引用
        /// </summary>
        public List<string> parents;
    }
}