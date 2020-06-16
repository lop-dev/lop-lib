//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      
//////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace UDLib.UI
{
    public enum ScrollDir
    {
        Horizontal,
        Vertical
    }

    public class CScrollRectCenter : MonoBehaviour, IEndDragHandler, IDragHandler, IBeginDragHandler
    {
        public ScrollDir Dir = ScrollDir.Horizontal;

        public RecycleBackFunction OnCenterChanged;
        /// <summary>
        /// 是否正在居中
        /// </summary>
        private bool _isCentering = false;

        /// <summary>
        /// 居中过程移动速度
        /// </summary>
        public float MoveToCenterSpeed = 10f;

        private ScrollRect _scrollView;

        private Transform _content;
        private LayoutGroup _layoutGroup;
        public List<float> _childrenPos = new List<float>();
        public float _targetPos;

        /// <summary>
        /// 当前中心child索引
        /// </summary>
        public int _curCenterChildIndex = -1;

        public int _childCount = 0;
        public int _startPositionIndex = 0;
        /// <summary>
        /// 当前中心ChildItem
        /// </summary>
        public GameObject CurCenterChildItem
        {
            get
            {
                GameObject centerChild = null;
                if (_content != null && _curCenterChildIndex >= 0 && _curCenterChildIndex < _content.childCount)
                {
                    centerChild = _content.GetChild(_curCenterChildIndex).gameObject;
                }
                return centerChild;
            }
        }

        void Awake()
        {
            _scrollView = GetComponent<ScrollRect>();
            if (_scrollView == null)
            {
                Debug.LogError("ScrollRect is null");
                return;
            }
            _content = _scrollView.content;
            _scrollView.movementType = ScrollRect.MovementType.Unrestricted;
            _layoutGroup = _content.GetComponent<LayoutGroup>();
            if (_layoutGroup == null)
            {
                Debug.LogError("LayoutGroup component is null");
            }
            _childCount = _content.childCount;
        }
        private void OnEnable()
        {
            Reset();
        }
        private void Reset()
        {
            _childrenPos.Clear();
            float spacing = 0f;
            //根据dir计算坐标，Horizontal：存x，Vertical：存y
            switch (Dir)
            {
                case ScrollDir.Horizontal:
                    if (_layoutGroup is HorizontalLayoutGroup)
                    {
                        float childPosX = _scrollView.GetComponent<RectTransform>().rect.width * 0.5f - GetChildItemWidth(0) * 0.5f;
                        spacing = (_layoutGroup as HorizontalLayoutGroup).spacing;
                        _childrenPos.Add(childPosX);
                        for (int i = 1; i < _content.childCount; i++)
                        {
                            childPosX -= GetChildItemWidth(i) * 0.5f + GetChildItemWidth(i - 1) * 0.5f + spacing;
                            _childrenPos.Add(childPosX);
                        }
                    }
                    else if (_layoutGroup is GridLayoutGroup)
                    {
                        GridLayoutGroup grid = _layoutGroup as GridLayoutGroup;
                        float childPosX = _scrollView.GetComponent<RectTransform>().rect.width * 0.5f - grid.cellSize.x * 0.5f;
                        _childrenPos.Add(childPosX);
                        for (int i = 0; i < _content.childCount - 1; i++)
                        {
                            childPosX -= grid.cellSize.x + grid.spacing.x;
                            _childrenPos.Add(childPosX);
                        }
                    }
                    else
                    {
                        Debug.LogError("Horizontal ScrollView is using VerticalLayoutGroup");
                    }
                    FindClosestChildPos(_content.localPosition.x, out _curCenterChildIndex);
                    break;
                case ScrollDir.Vertical:
                    if (_layoutGroup is VerticalLayoutGroup)
                    {
                        float childPosY = -_scrollView.GetComponent<RectTransform>().rect.height * 0.5f + GetChildItemHeight(0) * 0.5f;
                        spacing = (_layoutGroup as VerticalLayoutGroup).spacing;
                        _childrenPos.Add(childPosY);
                        for (int i = 1; i < _content.childCount; i++)
                        {
                            childPosY += GetChildItemHeight(i) * 0.5f + GetChildItemHeight(i - 1) * 0.5f + spacing;
                            _childrenPos.Add(childPosY);
                        }
                    }
                    else if (_layoutGroup is GridLayoutGroup)
                    {
                        GridLayoutGroup grid = _layoutGroup as GridLayoutGroup;
                        float childPosY = -_scrollView.GetComponent<RectTransform>().rect.height * 0.5f + grid.cellSize.y * 0.5f;
                        _childrenPos.Add(childPosY);
                        for (int i = 1; i < _content.childCount; i++)
                        {
                            childPosY += grid.cellSize.y + grid.spacing.y;
                            _childrenPos.Add(childPosY);
                        }
                    }
                    else
                    {
                        Debug.LogError("Vertical ScrollView is using HorizontalLayoutGroup");
                    }
                    break;
            }
            SetStartPosition(_startPositionIndex);
        }
        private float GetChildItemWidth(int index)
        {
            if (_content.childCount <= index)
                return 0;
            return (_content.GetChild(index) as RectTransform).sizeDelta.x;
        }

        private float GetChildItemHeight(int index)
        {
            if (_content.childCount <= index)
                return 0;
            return (_content.GetChild(index) as RectTransform).sizeDelta.y;
        }
        public void SetStartPosition(int index)
        {
            if (index < 0)
                index = _childrenPos.Count - 1;
            if (_content != null && index >= 0 && index < _content.childCount && index < _childrenPos.Count)
            {
                _curCenterChildIndex = index;
                _targetPos = _childrenPos[index];

                Vector3 v = _content.localPosition;
                switch (Dir)
                {
                    case ScrollDir.Horizontal:
                        v.x = _targetPos;
                        _content.localPosition = v;
                        break;
                    case ScrollDir.Vertical:
                        v.y = _targetPos;
                        _content.localPosition = v;
                        break;
                }

                if (OnCenterChanged != null)
                    OnCenterChanged(CurCenterChildItem);
            }
        }
        void Start()
        {

        }

        void Update()
        {
            if (_childCount != _content.childCount)
            {
                Reset();
                _childCount = _content.childCount;
            }

            if (_isCentering)
            {
                Vector3 v = _content.localPosition;
                switch (Dir)
                {
                    case ScrollDir.Horizontal:
                        v.x = Mathf.Lerp(_content.localPosition.x, _targetPos, MoveToCenterSpeed * Time.deltaTime);
                        _content.localPosition = v;
                        if (Math.Abs(_content.localPosition.x - _targetPos) < 0.01f)
                        {
                            _isCentering = false;
                        }
                        break;
                    case ScrollDir.Vertical:
                        v.y = Mathf.Lerp(_content.localPosition.y, _targetPos, MoveToCenterSpeed * Time.deltaTime);
                        _content.localPosition = v;
                        if (Math.Abs(_content.localPosition.y - _targetPos) < 0.01f)
                        {
                            _isCentering = false;
                        }
                        break;
                }


            }
        }

        public void OnDrag(PointerEventData eventData)
        {
            _isCentering = false;
        }

        public void OnEndDrag(PointerEventData eventData)
        {
            switch (Dir)
            {
                case ScrollDir.Horizontal:
                    _targetPos = FindClosestChildPos(_content.localPosition.x, out _curCenterChildIndex);
                    break;
                case ScrollDir.Vertical:
                    _targetPos = FindClosestChildPos(_content.localPosition.y, out _curCenterChildIndex);
                    break;
            }
            _isCentering = true;
        }

        public void OnBeginDrag(PointerEventData eventData)
        {
            _isCentering = false;
            _curCenterChildIndex = -1;
        }

        private float FindClosestChildPos(float currentPos, out int curCenterChildIndex)
        {
            float closest = 0;
            float distance = Mathf.Infinity;
            curCenterChildIndex = -1;
            for (int i = 0; i < _childrenPos.Count; i++)
            {
                float p = _childrenPos[i];
                float d = Mathf.Abs(p - currentPos);
                if (d < distance)
                {
                    distance = d;
                    closest = p;
                    curCenterChildIndex = i;
                }
            }

            if (OnCenterChanged != null)
                OnCenterChanged(CurCenterChildItem);

            return closest;
        }

        public void SetOnCenterChangedFunction(RecycleBackFunction callback)
        {
            if (callback == OnCenterChanged)
                return;
            OnCenterChanged = callback;
        }

        void OnDestroy()
        {
            //	if (OnCenterChanged != null) OnCenterChanged.Dispose();
            OnCenterChanged = null;
        }
    }

}

