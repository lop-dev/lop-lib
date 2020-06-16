//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      
//////////////////////////////////////////////////////////////////////
using System;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace UDLib.UI
{
    [RequireComponent(typeof(ScrollRect))]
    [AddComponentMenu("UI/Extensions/ScrollRectCircle")]
    public class CScrollRectCircle : MonoBehaviour, IBeginDragHandler, IEndDragHandler, IDragHandler
    {
        //private int _startingScreen = 1;
        private Transform _container;
        private System.Collections.Generic.List<float> _childrenPos = new System.Collections.Generic.List<float>();
        private ScrollRect _scrollRect;
        private Vector3 _startPosition = new Vector3();
        private int _currentScreen;

        public enum AxisType
        {
            Vertical,
            Horizontal
        }

        public AxisType m_AxisType = AxisType.Horizontal;

        //CenterOnChild
        public bool centerOnChild = true;        //是否子节点居中
        public float centerSpeed = 10f;        //居中速度
        public float offsetY = 50;
        public float offsetScale = 0.8f;

        //注册该事件获取当拖动结束时位于中心位置的子物体
        public delegate void OnCenterHandler(GameObject centerChild);
        public event OnCenterHandler onCenter;
        public Action onCenterOnece;

        private float _targetPos;
        private bool _centering = false;
        private Vector2 _childSizeDelta = new Vector2();
        private System.Collections.Generic.List<RectTransform> _childList = new System.Collections.Generic.List<RectTransform>();
        private RectTransform _containerRectTransform;
        private int _centerIndex = 0;
        private bool _isInited = false;
        //HorizontalLayoutGroup _hGroup;
        //add 特殊需求
        public Transform RotateGO;


        // Use this for initialization
        void Awake()
        {
            _scrollRect = gameObject.GetComponent<ScrollRect>();
            _container = _scrollRect.content;
            _containerRectTransform = _container.GetComponent<RectTransform>();

            _scrollRect.movementType = ScrollRect.MovementType.Unrestricted;

        }
        private void Start()
        {
            if (_container.childCount > 0 && _container.GetChild(0) != null)
            {
                _childSizeDelta = _container.GetChild(0).GetComponent<RectTransform>().sizeDelta;
            }
            else
            {
                return;//yield break;
            }
            /*
            for (int i = 0; i < _container.childCount; i++)
            {
                _chilrenMask.Add(_container.GetChild(i).Find("mask").GetComponent<Image>());
            }
            */
            ResetPosition();
            ChangeChildPosScale(_container.localPosition.x);
            //OnEndDrag(null);
            //CenterOnChild(0);
            _isInited = true;
        }

        void Update()
        {
            if (_centering && centerOnChild)
            {
                Vector3 v = _container.localPosition;
                switch (m_AxisType)
                {
                    case AxisType.Vertical:
                        v.y = Mathf.Lerp(_container.localPosition.y, _targetPos, centerSpeed * Time.deltaTime);
                        _container.localPosition = v;
                        if (Mathf.Abs(_container.localPosition.y - _targetPos) < 0.01f)
                        {
                            _centering = false;
                        }
                        break;
                    case AxisType.Horizontal:
                        v.x = Mathf.Lerp(_container.localPosition.x, _targetPos, centerSpeed * Time.deltaTime);
                        _container.localPosition = v;
                        FixTargetPos();
                        if (Mathf.Abs(_container.localPosition.x - _targetPos) < 0.01f)
                        {
                            _centering = FixTargetPos();
                        }
                        if (onCenterOnece != null && Mathf.Abs(_container.localPosition.x - _targetPos) < 5)
                        {
                            onCenterOnece();
                            onCenterOnece = null;
                        }
                        break;
                }
            }

            //子节点弧形位移
            ChangeChildPosScale(_container.localPosition.x);
            //旋转头顶图标
            RotateGameObject();
        }

        private void RotateGameObject()
        {
            if (RotateGO)
            {
                RotateGO.rotation = Quaternion.Euler(0, 0, _container.localPosition.x);
            }
        }

        private void ResetPosition()
        {
            if (_container.childCount <= 0) return;
            _childList.Clear();
            float containerX = 0;
            for (int i = 0; i < _container.childCount; i++)
            {
                RectTransform childRT = _container.GetChild(i).GetComponent<RectTransform>();
                if (!childRT.gameObject.activeSelf) continue;
                childRT.pivot = new Vector2(0.5f, 1);
                float value = _container.GetChild(i).localScale.x * _childSizeDelta.x / 2;
                value += GetChildPosByIndex(i, _childSizeDelta.x);
                childRT.anchoredPosition = new Vector2(value, _isInited ? childRT.anchoredPosition.y : 0);
                containerX += childRT.sizeDelta.x;
                _childList.Add(childRT);
            }
            Vector2 containerSize = _childList[0].sizeDelta;
            containerSize.x = containerX;
            _containerRectTransform.sizeDelta = containerSize;
        }


        public void OnBeginDrag(PointerEventData eventData)
        {
        }

        public void OnEndDrag(PointerEventData eventData)
        {
            _centering = true;
            if (_container.childCount <= 0) return;
            switch (m_AxisType)
            {
                case AxisType.Vertical:
                    _targetPos = FindClosestPos(_container.localPosition.y);
                    break;
                case AxisType.Horizontal:
                    ResetChildrenPos();//计算变化后子节点的坐标
                    _targetPos = FindClosestPos(_container.localPosition.x);//计算目标点
                    break;
            }

        }


        public void OnDrag(PointerEventData eventData)
        {
            _centering = false;
        }
        private float FindClosestPos(float currentPos)
        {
            int childIndex = 0;
            float closest = 0;
            float distance = Mathf.Infinity;

            for (int i = 0; i < _childrenPos.Count; i++)
            {
                float p = _childrenPos[i];
                float d = Mathf.Abs(p - currentPos);
                if (d < distance)
                {
                    distance = d;
                    closest = p;
                    childIndex = i;
                }
            }
            _centerIndex = childIndex;

            return closest;
        }
        /// <summary>
        /// 改变子节点的坐标跟缩放
        /// </summary>
        void ChangeChildPosScale(float currentPos)
        {
            if (_container.childCount <= 0) return;
            Transform child = _container.GetChild(0);
            for (int i = 0; i < _childrenPos.Count; i++)
            {
                child = _container.GetChild(i);
                float p = _childrenPos[i];
                float d = Mathf.Abs(p - currentPos);
                Vector2 pos = _childList[i].anchoredPosition;

                float scale = 0;
                if ((int)(d / 150) > 0)
                {
                    float changeValue = (d - 150) / 150 > 3 ? 3 : (d - 150) / 150;
                    scale = changeValue / 2.5f;
                }
                child.localScale = Vector3.one * (1f - (1 - offsetScale) * scale);
                pos.y = offsetY * scale;
                ChangeChildMask(i, ((1 - offsetScale) * scale) * 3);

                _childList[i].anchoredPosition = pos;
                Vector2 size = _childList[i].sizeDelta;
                size.x = _childSizeDelta.x * child.localScale.x;
                _childList[i].sizeDelta = size;
            }
            ResetPosition();
        }
        //由于子节点会有大小变化坐标会一直变
        private void ResetChildrenPos()
        {
            float childTemplateSize = 0;
            switch (m_AxisType)
            {
                case AxisType.Vertical:
                    childTemplateSize = _childSizeDelta.y;
                    break;
                case AxisType.Horizontal:
                    childTemplateSize = _childSizeDelta.x;
                    break;
            }
            _childrenPos.Clear();
            for (int i = 0; i < _container.childCount; i++)
            {
                float value = _container.GetChild(i).localScale.x * childTemplateSize / 2;
                value += GetChildPosByIndex(i, childTemplateSize);
                value = GetContainerWidth() / 2 - value;
                _childrenPos.Add(value);
            }
        }

        private float GetContainerWidth()
        {
            return _containerRectTransform.sizeDelta.x;
        }

        private float GetChildPosByIndex(int index, float childSize)
        {
            float result = 0;
            while (index-- > 0)
            {
                result += _container.GetChild(index).localScale.x * childSize;
            }
            return result;
        }

        System.Collections.Generic.List<Image> _chilrenMask = new System.Collections.Generic.List<Image>();
        private void ChangeChildMask(int index, float value)
        {
            if (_container.childCount > index)
            {
                if (_chilrenMask.Count > index)
                {
                    Color temp = _chilrenMask[index].color;
                    temp.a = value;
                    _chilrenMask[index].color = temp;
                }
            }
        }

        /// <summary>
        /// 居中某个子节点
        /// </summary>
        public void CenterOnChild(GameObject child)
        {
            int index = 0;
            for (int i = 0; i < _childList.Count; i++)
            {
                if (_childList[i].gameObject == child)
                {
                    index = i;
                }
            }
            CenterOnChild(index);
        }

        public void CenterOnChild(int childIndex)
        {
            ResetChildrenPos();//计算变化后子节点的坐标
            //Vector3 temp = _container.localPosition;
            //temp.x = _childrenPos[childIndex];
            //_container.localPosition = temp;
            //OnEndDrag(null);
            _centerIndex = childIndex;
            _centering = true;
            _targetPos = _childrenPos[childIndex];
        }

        private bool FixTargetPos()
        {
            ResetChildrenPos();//计算变化后子节点的坐标
            if (Mathf.Abs(_childrenPos[_centerIndex] - _targetPos) > 0.01f)
            {
                _targetPos = _childrenPos[_centerIndex];
                return true;
            }
            else
            {
                GameObject centerChild = _container.GetChild(_centerIndex).gameObject;
                if (onCenter != null && _isInited)
                    onCenter(centerChild);
                return false;
            }
        }
    }
}
