using UnityEngine.EventSystems;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
    public class CFormationLoopScrollRect : CHorizontalLoopScrollRect
    {
        [Tooltip("ItemTemple")]
        public GameObject ItemFormation = null;
        public int showHandleRectNum = 9;
        public Transform firstObj;
        public Transform firstObj2Vec;
        public Transform firstObj2;
        public Transform lastObj;
        public Transform lastObj2Vec;
        public Transform lastObj2;
        Transform _contentFirstTf = null;
        private Text _hscrollbarNum = null;

        private bool m_isDragOut = false;
        public bool IsDragOut { get { return m_isDragOut; } set { m_isDragOut = value; } }

        //建立字典索引
        Dictionary<int, Transform> m_itemDic = new Dictionary<int, Transform>();

        protected override void Awake()
        {
            base.Awake();
            if(ItemFormation != null)
            {
                itemTemplate = ItemFormation;
            }
            var hscrollbarNum = horizontalScrollbar.transform.Find("num");
            if (hscrollbarNum != null)
                _hscrollbarNum = hscrollbarNum.GetComponent<Text>();


            //extraOffset = new Vector3(112, 0);
        }

        protected override void LateUpdate()
        {
            base.LateUpdate();
            if (!IsDragOut)
            {
                for (int i = 0; i < content.childCount; i++)
                {
                    _contentFirstTf = content.GetChild(i);
                    if (_contentFirstTf.position.x <= firstObj.position.x)
                    {
                        if (_contentFirstTf.position.x <= firstObj2Vec.position.x)
                        {
                            CustomSetParent(_contentFirstTf, firstObj2);
                        }
                        else
                        {
                            CustomSetParent(_contentFirstTf, firstObj);
                        }
                    }
                    else if (_contentFirstTf.position.x >= lastObj.position.x)
                    {
                        if (_contentFirstTf.position.x >= lastObj2Vec.position.x)
                        {
                            CustomSetParent(_contentFirstTf, lastObj2);
                        }
                        else
                        {
                             CustomSetParent(_contentFirstTf, lastObj);
                        }
                    }
                    else
                    {
                        if (_contentFirstTf.childCount == 0)
                        {
                            CustomSetParent(_contentFirstTf);
                        }
                    }
                }
            }

            if (_hscrollbarNum != null)
            {
                if (totalCount > showHandleRectNum)
                {
                    if (!horizontalScrollbar.gameObject.activeSelf)
                        horizontalScrollbar.gameObject.SetActive(true);
                    if (!horizontalScrollbar.handleRect.gameObject.activeSelf)
                        horizontalScrollbar.handleRect.gameObject.SetActive(true);
                    _hscrollbarNum.text = string.Format("{0}/{1}", Mathf.Round(horizontalScrollbar.value * (totalCount- 2 )), totalCount - 2);
                }
                else if (totalCount == 0)
                {
                    if (horizontalScrollbar.gameObject.activeSelf)
                        horizontalScrollbar.gameObject.SetActive(false);
                }
                else
                {
                    if (!horizontalScrollbar.gameObject.activeSelf)
                        horizontalScrollbar.gameObject.SetActive(true);
                    if (horizontalScrollbar.handleRect.gameObject.activeSelf)
                        horizontalScrollbar.handleRect.gameObject.SetActive(false);
                    _hscrollbarNum.text = "";
                }
            }
        }

        protected override void UpdateScrollbars(Vector2 offset)
        {
            if (horizontalScrollbar)
            {
                horizontalScrollbar.value = horizontalNormalizedPosition;
                
            }
            if (verticalScrollbar)
            {
                verticalScrollbar.value = horizontalNormalizedPosition;
            }
        }

        protected override void RefreshItem(Transform go, int idx)
        {
            base.RefreshItem(go, idx);
            if (!m_itemDic.ContainsKey(go.GetInstanceID()))
            {
                m_itemDic.Add(go.GetInstanceID(), go.GetChild(0));
            }
        }

        protected override void RecycleItem(Transform go)
        {
            if (go.childCount == 0)
            {
                CustomSetParent(go);

                base.RecycleItem(go);
                return;
            }
            base.RecycleItem(go);
        }

        private void CustomSetParent(Transform item,Transform parent=null)
        {
            if (m_itemDic.ContainsKey(item.GetInstanceID()))
            {
                Transform temp = m_itemDic[item.GetInstanceID()];
                if (parent == null)
                    parent = item;
                temp.SetParent(parent);
                temp.localPosition = Vector3.zero;
            }
        }

        public Transform GetChildTransform(Transform parent)
        {
            Transform m_child = null;
            if (m_itemDic.ContainsKey(parent.GetInstanceID()))
                m_child = m_itemDic[parent.GetInstanceID()];
            return m_child;
        }
    }
}
