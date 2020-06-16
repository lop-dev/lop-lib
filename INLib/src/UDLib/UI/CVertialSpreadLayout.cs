//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      垂直方向展开
//////////////////////////////////////////////////////////////////////
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
    public class CVertialSpreadLayout : MonoBehaviour
    {
        public GameObject BigOneTempl;
        public GameObject SmallOneTempl;
        public Transform Content;

        public Transform BigParent;
        public Transform SmallParent;

        public delegate void ShowBigoneDelegate(GameObject gobj, int index);
        public ShowBigoneDelegate showBigoneFunc;
        public delegate void ShowSmallDelegate(GameObject gobj, int index);
        public ShowSmallDelegate showSmallFunc;

        private int m_totoalShowCnt { get; set; }
        private GameObject mLastSlectBigGo;
        private float m_lastSpreadHeight;
        private int m_lastSibindex;

        private RectTransform m_smallRect;
        private Canvas m_canvas;
        private GridLayoutGroup m_samllRectGrid;
        private readonly string s_SmallContentName = "tabNpcContent";

        private SpreadCache m_bigGoCache;
        private SpreadCache m_smallGoCache;

        void Awake()
        {
            Transform trans = this.transform.Find(s_SmallContentName);
            m_smallRect = trans as RectTransform;
            m_smallRect.transform.SetParent(Content);
            m_canvas = GetComponent<Canvas>();
            mLastSlectBigGo = null;
            m_lastSpreadHeight = 0f;
            m_lastSibindex = 0;

            m_samllRectGrid = m_smallRect.GetComponent<GridLayoutGroup>();
        }

        public void ResetCom()
        {
            m_lastSibindex = 0;
        }

        /// <summary>
        /// 设置bigone的个数
        /// </summary>
        /// <param name="bigCount"></param>
        public void SetBigCount(int bigCount)
        {
            if (null == m_bigGoCache)
            {
                m_bigGoCache = new SpreadCache();
                m_bigGoCache.InitCache(BigOneTempl, BigParent);
                m_smallGoCache = new SpreadCache();
                m_smallGoCache.InitCache(SmallOneTempl, SmallParent);
            }
            ClearChildsExceptName(Content, s_SmallContentName);
            for (int i = 0; i < bigCount; ++i)
            {
                GameObject cloneGobj = SpawnGobj(m_bigGoCache, Content);
                if (null != showBigoneFunc)
                    showBigoneFunc(cloneGobj, i);
            }
            UpdateViewContentRectHeight(bigCount * 100f);
        }

        public void OnClickBigOne(GameObject gobj, int spreadCnt)
        {
            m_samllRectGrid.gameObject.SetActive(true);
            //判断要不要展开，或者收缩
            //TODO, 先不处理点击上一个
            //if (mLastSlectBigGo != gobj)
            {
                OnSpreadBigOne(gobj, spreadCnt);
                mLastSlectBigGo = gobj;
            }
        }

        public void FolderSpread()
        {
            m_samllRectGrid.gameObject.SetActive(false);
        }

        /// <summary>
        /// bigone展开调用
        /// </summary>
        /// <param name="gobj"></param>
        /// <param name="spreadCnt">展开孩子数</param>
        private void OnSpreadBigOne(GameObject gobj, int spreadCnt)
        {
            float height = (Content as RectTransform).sizeDelta.y;
            m_smallRect.gameObject.SetActive(true);

            int sibIdx = gobj.transform.GetSiblingIndex();
            if (sibIdx <= m_lastSibindex)
            {
                ++sibIdx;
            }
            m_lastSibindex = sibIdx;
            m_smallRect.SetSiblingIndex(sibIdx);

            ClearSmallChilds(m_smallRect);
            for (int i = 0; i < spreadCnt; ++i)
            {
                GameObject cloneGobj = SpawnGobj(m_smallGoCache, m_smallRect);
                if (null != showSmallFunc)
                    showSmallFunc(cloneGobj, i);
            }
            float curHeight = spreadCnt * m_samllRectGrid.cellSize.y + (spreadCnt - 1) * m_samllRectGrid.spacing.y + m_samllRectGrid.padding.top;
            float changedHeight = curHeight - m_lastSpreadHeight;
            UpdateSmallRectHeight(curHeight);
            UpdateViewContentRectHeight(height + changedHeight);
        }

        private void UpdateViewContentRectHeight(float curHeight)
        {
            (Content as RectTransform).sizeDelta = new Vector2((Content as RectTransform).sizeDelta.x, curHeight);
            this.GetComponent<VerticalLayoutGroup>().CalculateLayoutInputHorizontal();
            this.GetComponent<VerticalLayoutGroup>().SetLayoutVertical();
        }

        private void UpdateSmallRectHeight(float curHeight)
        {
            m_lastSpreadHeight = curHeight;
            m_smallRect.sizeDelta = new Vector2(m_smallRect.sizeDelta.x, curHeight);
            m_smallRect.GetComponent<GridLayoutGroup>().CalculateLayoutInputHorizontal();
            m_smallRect.GetComponent<GridLayoutGroup>().SetLayoutVertical();
        }

        private void ClearSmallChilds(Transform parentTrans)
        {
            int childCount = parentTrans.childCount;
            for (int i = childCount - 1; i >= 0; --i)
            {
                var gobj = parentTrans.GetChild(i);
                m_smallGoCache.DespawnGo(gobj.gameObject);
            }
        }

        private void ClearChildsExceptName(Transform parentTrans, string exceptName)
        {
            int childCount = parentTrans.childCount;
            for (int i = childCount - 1; i >= 0; --i)
            {
                var gobj = parentTrans.GetChild(i);
                if (!gobj.name.Equals(exceptName))
                {
                    m_bigGoCache.DespawnGo(gobj.gameObject);
                }
            }
        }

        private GameObject SpawnGobj(SpreadCache spreadCache, Transform parent)
        {
            GameObject gobj = spreadCache.SpawnGo();
            gobj.transform.SetParent(parent);
            gobj.transform.localPosition = Vector3.zero;
            gobj.transform.localScale = Vector3.one;
            return gobj;
        }



        public class SpreadCache
        {
            private GameObject m_templateGo;
            private Transform m_cacheParent;
            private Stack<GameObject> m_cache;

            public void InitCache(GameObject templateGo, Transform cacheParent)
            {
                m_templateGo = templateGo;
                m_cacheParent = cacheParent;
                m_cache = new Stack<GameObject>();
            }

            public void Clear()
            {
                m_cache.Clear();
                m_cache = null;
                m_cacheParent = null;
            }

            public GameObject SpawnGo()
            {
                if (m_cache.Count == 0)
                {
                    return CreateNew();
                }
                return m_cache.Pop();
            }

            private GameObject CreateNew()
            {
                GameObject gobj = GameObject.Instantiate(m_templateGo);
                gobj.SetActive(true);
                return gobj;
            }

            public void DespawnGo(GameObject gobj)
            {
                gobj.transform.SetParent(m_cacheParent, false);
                gobj.transform.localPosition = Vector3.zero;
                m_cache.Push(gobj);
            }
        }
    }


}
