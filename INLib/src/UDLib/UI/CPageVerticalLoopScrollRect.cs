//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      
//////////////////////////////////////////////////////////////////////
using System.Collections;
using System.Text.RegularExpressions;
using UnityEngine;

namespace UDLib.UI
{
    public class CPageProperty : MonoBehaviour
    {
        public int pageIndex = -1;
    }
    public class CPageVerticalLoopScrollRect : CVerticalLoopScrollRect
    {
        [Tooltip("item top prefab source")]
        public GameObject itemTemplateTop;

        [Tooltip("item bottom prefab source")]
        public GameObject itemTemplateBottom;

        [Tooltip("page scroll anchor")]
        public Transform pageScrollAnchor;

        protected GameObject _topBottomCache;
        protected GameObject _topItem;
        protected GameObject _bottomItem;

        private int m_currentPage = -1;

        public int CurrentPage
        {
            get
            {
                return m_currentPage;
            }

            set
            {
                m_currentPage = value;
            }
        }

        // 创建Top与Bottom
        protected override RectTransform InstantiateNextItem(int itemIdx)
        {
            if (!_rootCache) Init();

            GameObject tObj;
            if (itemIdx == 0)
            {
                tObj = _topItem;
                tObj.transform.SetParent(content, false);
            }
            else if (itemIdx == totalCount - 1)
            {
                tObj = _bottomItem;
                tObj.transform.SetParent(content, false);
            }
            else if (_rootCache.transform.childCount > 0)
            {
                tObj = _rootCache.transform.GetChild(0).gameObject;
                tObj.name = Regex.Replace(tObj.name, @"_(\d+)", "_" + itemIdx);
                tObj.transform.SetParent(content, false);
            }
            else
            {
                tObj = Instantiate<GameObject>(itemTemplate.gameObject, content, false);
                tObj.name = string.Format("item_{1}({0})", _cacheIndex, itemIdx);
                ++_cacheIndex;
            }

            if (!tObj.activeInHierarchy) tObj.SetActive(true);
            RefreshItem(tObj.transform, itemIdx);
            var pageData = FindComponent<CPageProperty>(tObj);
            pageData.pageIndex = reverseDirection == true ? totalCount - itemIdx - 1 : itemIdx;

            return tObj.transform as RectTransform;
        }

        protected override void RecycleItem(Transform target)
        {
            if (!_rootCache) Init();
            if (itemTypeStart == 0)
            {
                target.SetParent(_topBottomCache.transform, false);
            }
            else if (itemTypeEnd == totalCount)
            {
                target.SetParent(_topBottomCache.transform, false);
            }
            else
            {
                target.SetParent(_rootCache.transform, false);
            }

            if (itemRecycleCallback != null) itemRecycleCallback(target.gameObject);
        }

        protected override void Init()
        {
            base.Init();

            isPageScrollMode = true;

            // 创建top和Bottom的cache节点
            if (_topBottomCache == null)
            {
                GameObject tObjRoot = new GameObject("top_bottom_cache");
                tObjRoot.transform.SetParent(content.parent, false);
                tObjRoot.SetActive(false);
                _topBottomCache = tObjRoot;

                if (itemTemplateTop)
                {
                    _topItem = CreateTopBottom(itemTemplateTop, _topBottomCache.transform);
                }

                if (itemTemplateBottom)
                {
                    _bottomItem = CreateTopBottom(itemTemplateBottom, _topBottomCache.transform);
                }
            }
        }

        private GameObject CreateTopBottom(GameObject prefab, Transform parent)
        {
            var go = Instantiate<GameObject>(prefab, parent, false);

            prefab.SetActive(false);
            prefab.transform.SetParent(content.parent, false);
            prefab.transform.SetAsLastSibling();

            return go;
        }

        // 计算翻页将要停留的位置
        protected override Vector2 GetPageStopPosition()
        {
            Vector2 axisPostion = Vector2.zero;
            if (movementType == MovementType.Unrestricted)
                return axisPostion;

            var root = content.parent;

            Vector2 centerPos = pageScrollAnchor.localPosition;
            RectTransform selectedItem = null;
            if (vertical)
            {
                int count = content.childCount;
                float dis = 0;
                RectTransform rt;
                for (int i = 0; i < count; i++)
                {
                    rt = content.GetChild(i) as RectTransform;
                    if (rt == null || !rt.gameObject.activeSelf)
                    {
                        continue;
                    }

                    Vector3 relativePos = root.transform.InverseTransformPoint(rt.gameObject.transform.position);
                    //Debug.LogErrorFormat("test 1 > relativePos : {0}", relativePos);
                    float curDis = Mathf.Abs(relativePos.y - centerPos.y);
                    if (selectedItem == null)
                    {
                        dis = curDis;
                        axisPostion = new Vector2(relativePos.x, relativePos.y);
                        selectedItem = rt;
                    }
                    else if (curDis < dis)
                    {
                        dis = curDis;
                        axisPostion = new Vector2(relativePos.x, relativePos.y);
                        selectedItem = rt;
                    }
                }
            }

            Vector2 offset = centerPos - axisPostion;
            // Debug.Log("######## GetPageStopPosition start :" + centerPos.ToString() + selectedItem.name + selectedItem.transform.position);
            //Debug.LogErrorFormat("test 2 > min offset : {0}, item: {1}, itemAnchoredPosition : {2}", offset, selectedItem.gameObject.name, selectedItem.anchoredPosition);
            offset.x = 0;
            //Debug.LogErrorFormat("test 3 > GetPageStopPosition : {0}", offset);
            return offset;
        }

        protected override void SetContentAnchoredPosition(Vector2 position)
        {
            if (!horizontal)
            {
                position.x = content.anchoredPosition.x;
            }

            if (!vertical)
            {
                position.y = content.anchoredPosition.y;
            }

            if (vertical)
            {
                if (System.Math.Abs(content.anchoredPosition.y - position.y) < 0.01f)
                {
                    return;
                }
            }

            RefreshSelectedPage();

            if (position != content.anchoredPosition)
            {
                //Debug.LogErrorFormat("SetContentAnchoredPosition : {0}", position);
                content.anchoredPosition = position;
                UpdateBounds(true);
            }
        }

        protected override void RefreshItem(Transform target, int index)
        {
            index = reverseDirection == true ? totalCount - index : index;
            if (itemUpdateCallback != null)
            {
                itemUpdateCallback(target.gameObject, index);
            }

            if (CurrentPage == -1)
            {
                RefreshSelectedPage();
            }
        }

        private void RefreshSelectedPage()
        {
            int selectedPageIdx = GetSelectedPage();
            if (CurrentPage != selectedPageIdx)
            {
                //Debug.LogErrorFormat("Page selected : {0}", selectedPageIdx);
                if (pageSelectedCallback != null)
                {
                    pageSelectedCallback(selectedPageIdx);
                }
                CurrentPage = selectedPageIdx;
            }
        }

        // 计算翻页将要停留的位置
        protected int GetSelectedPage()
        {
            int pageSelected = -1;
            var root = content.parent;
            Vector2 centerPos = pageScrollAnchor.localPosition;
            RectTransform selectedItem = null;
            if (vertical)
            {
                int count = content.childCount;
                float dis = 0;
                RectTransform rt;
                for (int i = 0; i < count; i++)
                {
                    rt = content.GetChild(i) as RectTransform;
                    if (rt == null || !rt.gameObject.activeSelf)
                    {
                        continue;
                    }

                    var relativePos = root.transform.InverseTransformPoint(rt.gameObject.transform.position);
                    float curDis = Vector2.Distance(relativePos, centerPos);
                    if (selectedItem == null)
                    {
                        dis = curDis;
                        selectedItem = rt;
                    }
                    else if (curDis < dis)
                    {
                        dis = curDis;
                        selectedItem = rt;
                    }
                }
            }

            if (selectedItem != null)
            {
                var pageData = selectedItem.GetComponent<CPageProperty>();
                if (pageData)
                {
                    pageSelected = pageData.pageIndex;
                }
            }

            //Debug.LogErrorFormat("selected page : {0}, item: {1}, itemAnchoredPosition : {2}", pageSelected, selectedItem.gameObject.name, selectedItem.anchoredPosition);
            return pageSelected;
        }

        public void ScrollToPage(int index, float speed)
        {
            isDragScroll = false;
            if (index < 0 || index >= totalCount)
            {
                //Debug.LogWarningFormat("invalid index {0}", index);
                return;
            }
            if (speed <= 0)
            {
                //Debug.LogWarningFormat("invalid speed {0}", speed);
                return;
            }
            StopAllCoroutines();
            StartCoroutine(ScrollToPageCoroutine(index, speed));
        }

        IEnumerator ScrollToPageCoroutine(int index, float speed)
        {
            _isScrollingToCell = true;
            isDragPageIn = false;
            bool isScrolling = true;
            while (isScrolling)
            {
                yield return null;
                if (!Dragging)
                {
                    float move = 0;
                    if (index < itemTypeStart)
                    {
                        move = -Time.deltaTime * speed;
                    }
                    else if (index >= itemTypeEnd)
                    {
                        move = Time.deltaTime * speed;
                    }
                    else
                    {
                        ViewBounds = new Bounds(viewRect.rect.center, viewRect.rect.size);
                        Vector2 centerPos = pageScrollAnchor.localPosition;
                        var m_ItemBounds = GetBounds4Item(index);
                        var offset = 0.0f;
                        if (directionSign == -1)
                            offset = centerPos.y - m_ItemBounds.center.y;
                        else if (directionSign == 1)
                            offset = centerPos.x - m_ItemBounds.center.x;
                        offset -= contentPadding;
                        //Debug.Log("-------------------------------------------- scroll offset>>>" + offset);

                        // check if we cannot move on
                        if (totalCount >= 0)
                        {
                            if (offset > 0 && itemTypeEnd == totalCount && !reverseDirection)
                            {
                                m_ItemBounds = GetBounds4Item(totalCount - 1);
                                // reach bottom
                                if ((directionSign == -1 && m_ItemBounds.min.y > ViewBounds.min.y) ||
                                    (directionSign == 1 && m_ItemBounds.max.x < ViewBounds.max.x))
                                {
                                    isScrolling = false;
                                    break;
                                }
                            }
                            else if (offset < 0 && itemTypeStart == 0 && reverseDirection)
                            {
                                m_ItemBounds = GetBounds4Item(0);
                                if ((directionSign == -1 && m_ItemBounds.max.y < ViewBounds.max.y) ||
                                    (directionSign == 1 && m_ItemBounds.min.x > ViewBounds.min.x))
                                {
                                    isScrolling = false;
                                    break;
                                }
                            }
                        }

                        float maxMove = Time.deltaTime * speed;
                        if (Mathf.Abs(offset) < maxMove)
                        {
                            isScrolling = false;
                            move = offset;
                        }
                        else
                            move = Mathf.Sign(offset) * maxMove;
                    }
                    if (move != 0)
                    {
                        Vector2 offset = GetVector(move);
                        content.anchoredPosition += offset;
                        PrevPosition += offset;
                        ContentStartPosition += offset;
                    }
                }
                else
                {
                    isScrolling = false;
                }
            }
            StopMovement();
            UpdatePrevData();
            _isScrollingToCell = false;
            RefreshSelectedPage();
        }
    }
}