using UnityEngine;
using System.Collections.Generic;
/*
namespace UDLib.Utility
{
    /// <summary>
    /// Table Loader - (c) 2014, Lizzy - 异步批量加载表格的方法
    /// 表格读取方法继承自CTable
    /// </summary>
    public abstract class CTableLoader : MonoBehaviour
    {
        /// <summary>
        /// 加载状态
        /// </summary>
        public enum ELoadState
        {
            ELS_IDLE = 0,
            ELS_LODING,
            ELS_LOADCOMPLETE,
        }

        /// <summary>
        /// 代理函数
        /// </summary>
        public delegate void HandleLoadFinish();

        private List<CSLib.Utility.CTable> m_listLoadingTables = new List<CSLib.Utility.CTable>();  //需要加载的表格
        private ELoadState m_loadState = ELoadState.ELS_IDLE;           //当前加载器状态
        private HandleLoadFinish m_callbackFunc;                        //回调函数

        //private static CTableLoader m_instance;
        //public static CTableLoader Instance
        //{
        //    get { return m_instance; }
        //}

        //protected void Awake()
        //{
        //    m_instance = this;
        //}

        protected void Update()
        {
            if (m_loadState == ELoadState.ELS_LODING)
                _checkLoad();
        }

        /// <summary>
        /// 开始新的加载前需要重置一下
        /// </summary>
        public void ResetLoad()
        {
            if (m_loadState == ELoadState.ELS_LODING)
            {
                //如果当前正有任务进行，可以直接添加
                return;
            }

            m_loadState = ELoadState.ELS_IDLE;
            m_listLoadingTables.Clear();
        }

        /// <summary>
        /// 添加需要加载的Table
        /// </summary>
        /// <param name="table"></param>
        /// <returns></returns>
        public bool AddLoadTable(CSLib.Utility.CTable table)
        {
            if (m_loadState == ELoadState.ELS_IDLE || m_loadState == ELoadState.ELS_LODING)
            {
                if (table == null)
                {
                    CDebugOut.LogError("需要加载的表格没有挂载到场景里，请检查下");
                    return false;
                }

                m_listLoadingTables.Add(table);
                return true;
            }

            return false;
        }

        /// <summary>
        /// 添加完Table后，开始调用加载
        /// </summary>
        /// <returns></returns>
        public bool StartLoad(HandleLoadFinish func)
        {
            if (m_loadState == ELoadState.ELS_LODING)
            {
                // already start
                return true;
            }

            foreach (CSLib.Utility.CTable table in m_listLoadingTables)
            {
                if (table == null)
                    return false;

                if (!table.LoadTable(this, null, null))
                    return false;
            }
            m_loadState = ELoadState.ELS_LODING;
            m_callbackFunc = func;

            return true;
        }

        /// <summary>
        /// 检查是否加载完成,完成后会直接调用回调函数
        /// </summary>
        /// <param name="func"></param>
        /// <returns></returns>
        private bool _checkLoad()
        {
            if (m_loadState != ELoadState.ELS_LODING)
                return true;

            for (int i = m_listLoadingTables.Count - 1; i >= 0; i--)
            {
                if (m_listLoadingTables[i].IsLoaded)
                {
                    m_listLoadingTables.RemoveAt(i);
                }
            }

            if (m_listLoadingTables.Count > 0)
                return false;

            m_loadState = ELoadState.ELS_LOADCOMPLETE;
            if (m_callbackFunc != null)
            {
                m_callbackFunc();
            }
            return true;
        }

        /// <summary>
        /// 检查加载是否完成
        /// </summary>
        /// <returns></returns>
        public bool IsLoadComplete()
        {
            return m_loadState == ELoadState.ELS_LOADCOMPLETE;
        }

        /// <summary>
        /// 帮助函数，加载excel格式的xml文件
        /// </summary>
        /// <param name="excelReader"></param>
        /// <param name="pathName"></param>
        /// <param name="func"></param>
        /// <returns></returns>
        public bool AsyncLoadFile(CSLib.Utility.CExcelTableReader excelReader, string pathName, CSLib.Utility.CExcelTableReader.HandleFinishLoad func)
        {
            StartCoroutine(excelReader.LoadFile(pathName, func));
            return true;
        }
    }
}
*/