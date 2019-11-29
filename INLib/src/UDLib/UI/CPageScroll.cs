/// Credit BinaryX
/// Sourced from - http://forum.unity3d.com/threads/scripts-useful-4-6-scripts-collection.264161/page-2#post-1945602
/// Updated by ddreaper - removed dependency on a custom ScrollRect script. Now implements drag interfaces and standard Scroll Rect.
/// wp added 2018-12-14
/// changed 2019-1-10,add supporting for vertical layout

//using LuaInterface;
using System;
using UnityEngine.EventSystems;
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI {

    [RequireComponent(typeof(ScrollRect))]
    [AddComponentMenu("UI/Extensions/Page Scroll")]
    public class CPageScroll : MonoBehaviour, IBeginDragHandler, IEndDragHandler, IDragHandler {
        [Tooltip("The gameobject that contains toggles which suggest pagination. (optional)")]
        public GameObject Pagination;
        [Tooltip("Button to go to the next page. (optional)")]
        public GameObject NextButton;
        [Tooltip("Button to go to the previous page. (optional)")]
        public GameObject PrevButton;
        public Boolean UseFastSwipe = true;
        public int FastSwipeThreshold = 100;
        // wp add
        public int ItemGap                        = 10;                  // 单元项间距
        public bool IsItemConstant                = false;               // 是否滑动时保持单元项大小不变
        protected int _lastPageIndex              = -1;                  // 上一次页码
        protected PageBackFunction _cbPageIndexChanged = null;                // 页码变化时lua回调
        protected bool _isHorizontal              = true;                // 是否列表为水平
        protected int _lastHold                   = 0;                   // 每屏多于一页时的差值
        protected Vector3 _helpVec3               = Vector3.zero;        // 复用vector3
        protected bool _isAutoPage                = false;               // 是否页码循环
        protected float _timeInterval             = -1;                  // 自动翻页时间间隔
        protected float _timeElapsed              = -1;                  // 自动翻页计时
        protected float _lerpSpeed                = 0.5f;                // 翻页速度

        //private int _startingScreen = 1;
        private Transform _screensContainer;
        private int _screens = 1;
        private int _containerSize;
        private Vector2 _itemSize;
        private bool _fastSwipeTimer = false;
        private int _fastSwipeCounter = 0;
        private int _fastSwipeTarget = 30;
        private System.Collections.Generic.List<Vector3> _positions;
        private ScrollRect _scrollRect;
        private Vector3 _lerpTargetPos;
        private bool _lerp;
        private bool _startDrag = true;
        private Vector3 _startPosition = new Vector3();
        private int _currentScreen;

        // Use this for initialization
        void Awake() {
            _scrollRect = gameObject.GetComponent<ScrollRect>();
            _screensContainer = _scrollRect.content;
            _positions = new System.Collections.Generic.List<Vector3>();
            if (NextButton) NextButton.GetComponent<Button>().onClick.AddListener(() => { NextScreen(); });
            if (PrevButton) PrevButton.GetComponent<Button>().onClick.AddListener(() => { PreviousScreen(); });

            // test
            //ResetData();
        }

        void Update() {
            if (_lerp) {
                _screensContainer.localPosition = Vector3.Lerp(_screensContainer.localPosition, _lerpTargetPos, _lerpSpeed);
                //Debugger.Log("------------------------cur pos>>>" + _screensContainer.localPosition.ToString() + "---tar pos>>>" + _lerp_target.ToString());
                if (Vector3.Distance(_screensContainer.localPosition, _lerpTargetPos) < 0.01f) {
                    _lerp = false;

                    // wp added 2018-12-17
                    int tIndex = CurrentScreen() + 1;
                    if(_screens == 1) return;                // 只有一页时特殊处理（wp added 2019-6-28
                    if (_lastPageIndex != tIndex && _cbPageIndexChanged != null) _cbPageIndexChanged(tIndex);
                    _lastPageIndex = tIndex;
                }

                //change the info bullets at the bottom of the screen. Just for visual effect
                if (Vector3.Distance(_screensContainer.localPosition, _lerpTargetPos) < 10f) {
                    ChangeBulletsInfo(CurrentScreen());
                }
            }

            if (_fastSwipeTimer) _fastSwipeCounter++;

            // 自动翻页
            if (_isAutoPage) {
                _timeElapsed += Time.deltaTime;
                if (!_startDrag) _timeElapsed = 0;
                if(_timeElapsed >= _timeInterval) {
                    NextScreen();
                    _timeElapsed = 0;
                }
            }
        }

        //设置自动翻译
        public void ResetAutoPage(bool isAuto)
        {
            _isAutoPage = isAuto;
        }

        // reset data and set selected page index
        // @params startIndex 默认选中索引（start with 1
        // @params lastHold 用于一屏显示多页的情况，值为一屏页数减1。（默认0
        // wp added 2018-12-17
        public void ResetData(int startIndex = 1, int lastHold = 0) {
            _lastHold = lastHold;

            _isHorizontal = _scrollRect.horizontal;
            _itemSize = gameObject.GetComponent<RectTransform>().rect.size;
            //print("---------------------------------item size>>>" + _itemSize.ToString());
            DistributePages();

            _lastPageIndex = -1;
            _lerp = false;
            //_positions = new System.Collections.Generic.List<Vector3>();
            _positions.Clear();

            if (_screens > 0) {
                for (int i = 0; i < _screens; ++i) {
                    if (_isHorizontal) _scrollRect.horizontalNormalizedPosition = i / (float)(_screens - 1);
                    else _scrollRect.verticalNormalizedPosition = i / (float)(_screens - 1);
                    _positions.Add(_screensContainer.localPosition);
                }
            }

            if (_isHorizontal) _scrollRect.horizontalNormalizedPosition = (startIndex - 1) / (float)(_screens - 1);
            else _scrollRect.verticalNormalizedPosition = (startIndex - 1) / (float)(_screens - 1);

            if (!IsItemConstant) ResizeItems(startIndex);
            if (_cbPageIndexChanged != null) _cbPageIndexChanged(startIndex);

            ChangeBulletsInfo(CurrentScreen());
        }

        public void AutoTurnPageInterval(float timeInterval) {
            _isAutoPage = true;
            _timeInterval = timeInterval;
        }

        // page index changed callback
        public void SetPageChangedCallback(PageBackFunction callback,float lerpSpeed = 0) {
            if (callback == null || callback == _cbPageIndexChanged) return;
            if (lerpSpeed != 0) _lerpSpeed = lerpSpeed;
            _cbPageIndexChanged = callback;
        }


        //used for changing between screen resolutions
        //wp changed 2018-12-14(notice!!!keeping item layout middle-left,pivot middle-center
        private void DistributePages() {
            int tGap = ItemGap, tValue = 0, tPos = 0;
            int tOffset = (int)((_isHorizontal ? _itemSize.x : _itemSize.y) / 2), tLength = _screensContainer.transform.childCount;

            if (_isHorizontal) _scrollRect.horizontalNormalizedPosition = 0;
            else _scrollRect.verticalNormalizedPosition = 0;

            int i;
            for (i = 0; i < tLength; i++) {
                RectTransform tChild = _screensContainer.transform.GetChild(i) as RectTransform;
                if (!tChild.gameObject.activeSelf) break;

                tPos = tOffset + i * (tGap + (int)(_isHorizontal ? _itemSize.x : _itemSize.y));
                //tChild.sizeDelta = _itemSize;
                tChild.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, _itemSize.x);
                tChild.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, _itemSize.y);
                tChild.anchoredPosition = _isHorizontal ? new Vector2(tPos, 0) : new Vector2(0, tPos);
            }

            int tRefine = _lastHold < 0 ? 0 : Math.Min(i, _lastHold);
            _screens = i - tRefine;                                        // keep item reused (wp changed 2018-12-21
            tValue = tPos - tOffset * (1 + 2 * _lastHold);
            _containerSize = tValue;
            _screensContainer.GetComponent<RectTransform>().offsetMax = _isHorizontal ? new Vector2(tValue, 0) : new Vector2(0, tValue);

            // Pagination
            if (Pagination) {
                int tCount = Pagination.transform.childCount;
                if (tCount < 1) return;
                Transform tRoot = Pagination.transform, tTranInit = Pagination.transform.GetChild(0), tTranCur;
                Button tButton;

                tTranInit.name = "page_0";
                for (i = 0; i < Math.Max(tCount, _screens); i++) {
                    tTranCur = i < tCount ? tRoot.GetChild(i) : null;
                    if(i < _screens) {
                        if (!tTranCur) {
                            tTranCur = Instantiate(tTranInit.gameObject).transform;
                            tTranCur.name = "page_" + i;
                            tTranCur.SetParent(tRoot, false);
                        }
                        tTranCur.gameObject.SetActive(true);
                        tButton = tTranCur.GetComponent<Button>();
                        if (!tButton) tButton = tTranCur.gameObject.AddComponent<Button>();
                        tButton.onClick.RemoveAllListeners();
                        tButton.onClick.AddListener(() => {
                            onPageItemClickHandler(i);
                        });
                    } else {
                        if(tTranCur) tTranCur.gameObject.SetActive(false);
                    }
                }
            }
        }

        //pagination item click handler
        private void onPageItemClickHandler(int pageIndex) {
            var tObj = EventSystem.current.currentSelectedGameObject;
            string tName = tObj.name;
            int tIndex = int.Parse(tName.Substring(tName.LastIndexOf("_") + 1));
            _lerp = true;
            _lerpTargetPos = _positions[tIndex];
            ChangeBulletsInfo(pageIndex);
            _timeElapsed = 0;                                // 重置自动翻页计时
        }

        //returns the current screen that the is seeing
        private int CurrentScreen() {
            Vector2 tVec = _screensContainer.gameObject.GetComponent<RectTransform>().offsetMin;
            float tPos = _isHorizontal ? Math.Abs(tVec.x) : Math.Abs(tVec.y);

            tPos = Mathf.Clamp(tPos, 1, _containerSize - 1);
            int result = (int)((tPos / _containerSize) * _screens);

            //Debugger.Log("---------------- absPos>>>" + tPos + "---result>>>" + result + "---container size>>>" + _containerSize);

            return Mathf.Clamp(result, 0, _screens);
        }

        //Function for switching screens with buttons
        private void NextScreen() {
            int tIndexCur = CurrentScreen();
            int tIndexNew = tIndexCur < _screens - 1 ? tIndexCur + 1 : (_isAutoPage ? 0 : tIndexCur);
            if (tIndexCur == tIndexNew) return;

            _lerp = true;
            _lerpTargetPos = _positions[tIndexNew];
            ChangeBulletsInfo(tIndexNew);
            _timeElapsed = 0;                                // 重置自动翻页计时
        }

        //Function for switching screens with buttons
        private void PreviousScreen() {
            int tIndexCur = CurrentScreen();
            int tIndexNew = tIndexCur > 0 ? tIndexCur - 1 : (_isAutoPage ? _screens - 1 : tIndexCur);
            if (tIndexCur == tIndexNew) return;

            if (_positions != null && tIndexNew < _positions.Count)
            {
                _lerp = true;
                _lerpTargetPos = _positions[tIndexNew];
                ChangeBulletsInfo(tIndexNew);
                _timeElapsed = 0;                                // 重置自动翻页计时
            }
            else
            {
                if (_positions == null)
                {
                    Debug.LogError( "PreviousScreen() _positions is null !");
                }
                else
                {
                    Debug.LogErrorFormat("_positions Argument is out of range ! tIndexNew :{0} _positions.Count:{1}", tIndexNew, _positions.Count);
                }
            }
        }

        //Because the CurrentScreen function is not so reliable, these are the functions used for swipes
        private void NextScreenCommand() {
            if (_currentScreen < _screens - 1) {
                _lerp = true;
                _lerpTargetPos = _positions[_currentScreen + 1];

                ChangeBulletsInfo(_currentScreen + 1);
            }
        }

        //Because the CurrentScreen function is not so reliable, these are the functions used for swipes
        private void PrevScreenCommand() {
            if (_currentScreen > 0) {
                _lerp = true;
                _lerpTargetPos = _positions[_currentScreen - 1];

                ChangeBulletsInfo(_currentScreen - 1);
            }
        }


        //find the closest registered point to the releasing point
        private Vector3 FindClosestFrom(Vector3 start, System.Collections.Generic.List<Vector3> positions) {
            Vector3 closest = Vector3.zero;
            float distance = Mathf.Infinity;

            foreach (Vector3 position in _positions) {
                if (Vector3.Distance(start, position) < distance) {
                    distance = Vector3.Distance(start, position);
                    closest = position;
                }
            }

            //Debugger.Log("--------------- target index>>>" + _positions.IndexOf(closest));
            return closest;
        }

        //changes the bullets on the bottom of the page - pagination
        private void ChangeBulletsInfo(int currentScreen) {
            if (Pagination) {
                Transform tItem;
                for (int i = 0; i < _screens; i++) {
                    tItem = Pagination.transform.GetChild(i);
                    tItem.GetChild(0).GetComponent<CanvasRenderer>().SetAlpha(i == currentScreen ? 1 : 0);
                }
            }
        }

        // only use to scale unselected item size to 0.8
        private void ResizeItems(int curIndex = -1, bool isReset = false) {
            int tIndex = (curIndex != -1 ? curIndex - 1 : CurrentScreen()), tLength = _screens;
            //print("------------------------- resize index>>>" + tIndex);
            if (tLength <= 1) return;

            float tValue, tDistance;
            for (int i = 0; i < tLength; i++) {
                if(Math.Abs(tIndex - i) <= 1) {// 相邻两项
                    if(_isHorizontal) tDistance = Math.Abs(_positions[i].x - _screensContainer.localPosition.x) / _itemSize.x;
                    else tDistance = Math.Abs(_positions[i].y - _screensContainer.localPosition.y) / _itemSize.y;
                    tValue = Mathf.Lerp(0.8f, 1, Mathf.Clamp(1 - tDistance, 0, 1));
                    if (isReset && i != tIndex) tValue = 0.8f;// 拖拽抬起时重置缩放
                } else {
                    //tValue = i == tIndex ? 1 : 0.8f;
                    tValue = 0.8f;
                }
                _helpVec3.Set(tValue, tValue, tValue);
                _screensContainer.GetChild(i).localScale = _helpVec3;
            }
        }


        public void OnBeginDrag(PointerEventData eventData) {
            _startPosition = _screensContainer.localPosition;
            _fastSwipeCounter = 0;
            _fastSwipeTimer = true;
            _currentScreen = CurrentScreen();
        }

        private bool _fastSwipe = false; //to determine if a fast swipe was performed
       
        public void OnEndDrag(PointerEventData eventData) {// wp changed 2018-12-21
            _startDrag = true;
            float tDistance = _isHorizontal ? (_startPosition.x - _screensContainer.localPosition.x) : (_startPosition.y - _screensContainer.localPosition.y);
            float tLimit = _isHorizontal ? _itemSize.x : _itemSize.y;
            int tPage = -1;

            if (UseFastSwipe) {
                _fastSwipe = false;
                _fastSwipeTimer = false;
                if (_fastSwipeCounter <= _fastSwipeTarget && Math.Abs(tDistance) > FastSwipeThreshold && tDistance < tLimit) {
                    _fastSwipe = true;
                }
                if (_fastSwipe) {
                    if (tDistance > 0) {
                        NextScreenCommand();
                        if (_lerp) tPage = _currentScreen + 1;
                    } else {
                        PrevScreenCommand();
                        if (_lerp) tPage = _currentScreen - 1;
                    }
                } else {
                    _lerp = true;
                    tPage = CurrentScreen();
                    _lerpTargetPos = _positions[tPage];
                }
            } else {
                _lerp = true;
                tPage = CurrentScreen();
                _lerpTargetPos = _positions[tPage];
            }

            if (!IsItemConstant && tPage != -1) ResizeItems(tPage + 1, true);
        }

   
        public void OnDrag(PointerEventData eventData) {
            _lerp = false;
            if (_startDrag) {
                OnBeginDrag(eventData);
                _startDrag = false;
            }

            if (!IsItemConstant) ResizeItems(); // wp added 2018-12-18
        }
    }
}
