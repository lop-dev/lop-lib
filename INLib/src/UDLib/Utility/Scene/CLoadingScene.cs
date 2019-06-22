using UnityEngine;
using System.Collections;

namespace UDLib.Utility
{
    public class CLoadingScene : MonoBehaviour
    {
        /// <summary>
        /// 目标场景的名字
        /// </summary>
        protected static string m_TargetSceneName = "";

        /// <summary>
        /// 当前场景的背景图
        /// </summary>
        protected static string m_LoadingSceneBg = "";

        /// <summary>
        /// 当前场景的Tip信息
        /// </summary>
        protected static string m_LoadingSceneTip = "";

        /// <summary>
        /// 当前场景的名字（在子类中要设置的变量）
        /// </summary>
        public static string m_LoadingSceneName = "Loading01";

        /// <summary>
        /// 当前场景的更新操作（在子类中要设置的变量）
        /// </summary>
        protected CLoadingUpdate m_LoadingUpdate = null;

        private AsyncOperation m_async = null;

        public static bool AsncyLoadScene(string loadName, string loadBg, string loadTip)
        {
            m_TargetSceneName = loadName;
            m_LoadingSceneBg = loadBg;
            m_LoadingSceneTip = loadTip;

            //打开场景
            Application.LoadLevel(m_LoadingSceneName);
            return true;
        }

        protected void _LoadScene()
        {
            //
            if (m_LoadingUpdate != null) m_LoadingUpdate.UpdateLoadingTip(CLoadingScene.m_LoadingSceneTip);

            StartCoroutine(_LoadLevelAsync());
        }

        private IEnumerator _LoadLevelAsync()
        {
            m_async = Application.LoadLevelAsync(CLoadingScene.m_TargetSceneName);

            m_async.allowSceneActivation = false;
            int totalGress = 0;
            int currtGress = 0;

            int safeIndex = 0;
            while (m_async.progress < 0.9f)
            {
                totalGress = Mathf.FloorToInt(m_async.progress * 100.0f);

                while (currtGress < totalGress)
                {
                    safeIndex = 0;

                    ++currtGress; ++currtGress;
                    m_LoadingUpdate.UpdateLoadingValue(currtGress);

                    yield return new WaitForEndOfFrame();
                }

                safeIndex++;
                if ((currtGress >= totalGress) && safeIndex >= 2)
                {
                    yield return new WaitForEndOfFrame();
                }
            }

            totalGress = 100;
            while (currtGress < totalGress)
            {
                ++currtGress;
                m_LoadingUpdate.UpdateLoadingValue(currtGress);

                yield return new WaitForEndOfFrame();
            }
            m_async.allowSceneActivation = true;
            yield return m_async;
        }
    }
}

