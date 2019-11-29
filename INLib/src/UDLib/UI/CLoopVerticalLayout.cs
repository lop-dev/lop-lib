using System;
using System.Collections.Generic;
//using Cinemachine;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace UDLib.UI
{
	public delegate void LoopShowContentDelegate(GameObject gobj, int index, float rowPecent, int highlight);
	public delegate void LoopShowContentHighlightDelegate(GameObject gobj, int highlight);
	public class CLoopVerticalLayout : LayoutGroup, IPointerClickHandler
	{
		private RectTransform m_scrollRectTransform;
		private ScrollRect m_scrollRect;

		public ScrollRect ScrollRect
		{
			get
			{
				if (null == m_scrollRect)
				{
					m_scrollRect = GetComponentInParent<ScrollRect>();
					m_scrollRectTransform = m_scrollRect.transform as RectTransform;
				}

				return m_scrollRect;
			}
		}

		//private List<object> m_dataList = new List<object>();
		private int m_dataCount;
		
		private bool _isDirty;
		
		private Transform m_cacheTransform;
		private RectTransform m_rectTransform;
		
		public LoopShowContentDelegate showContentDelegate;
		public LoopShowContentHighlightDelegate showContentHighlightDelegate;
		
		private Transform m_targetHighlightTrans;
		private float m_HighlightY;
		//滚动选中模式下的前置空白
		private float m_headPlaceHodlerHeight;
		private RectTransform m_headRectTrans;
		//滚动选中模式下的后置空白
		private float m_tailPlaceHodlerHeight;
		private RectTransform m_tailRectTrans;
		public float SelectScrollMinmunSpeedY = 35f;
		
		protected override void Awake()
		{
			Init();
			m_cacheTransform = this.transform;
			m_rectTransform = this.transform as RectTransform;
			ScrollRect.onValueChanged.AddListener(OnScrollRectValueChange);
			m_targetHighlightTrans = ScrollRect.transform.Find("Highlighter");
			if (null != m_targetHighlightTrans)
			{
				float itemHei = ItemHeight;
				float itemwid = ItemWidth;
				
				float initHighlightYOfContent = itemHei * 0.5f + TargetRow * (itemHei + Spacing);
				//print(initHighlightYOfContent);
				Vector3 localPos = new Vector3(0, -initHighlightYOfContent, 0);
				m_targetHighlightTrans.position = m_cacheTransform.TransformPoint(localPos);
				
				GameObject headgobj = new GameObject("HeadPlaceHolder");
				m_headRectTrans = headgobj.AddComponent<RectTransform>();
				m_headPlaceHodlerHeight = itemHei * TargetRow;
				m_headRectTrans.sizeDelta = new Vector2(itemwid, m_headPlaceHodlerHeight);
				m_headRectTrans.SetParent(m_cacheTransform);
				m_headRectTrans.SetAsFirstSibling();
				
				GameObject tailgobj = new GameObject("TailPlaceHolder");
				m_tailRectTrans = tailgobj.AddComponent<RectTransform>();
				m_tailPlaceHodlerHeight = m_scrollRectTransform.rect.height - (itemHei + Spacing) * (TargetRow + 1);
				m_tailRectTrans.sizeDelta = new Vector2(itemwid, m_tailPlaceHodlerHeight);
				m_tailRectTrans.SetParent(m_cacheTransform);
				m_tailRectTrans.SetAsLastSibling();
			}
			else
			{
				TargetRow = 0;
			}
		}
		
		protected override void OnDestroy()
		{
			ScrollRect.onValueChanged.RemoveListener(OnScrollRectValueChange);
		}

		private void OnScrollRectValueChange(Vector2 arg0)
		{
			if (arg0.y < 0 || arg0.y > 1)
			{
				ScrollRect.verticalNormalizedPosition = Mathf.Clamp01(arg0.y);
			}

			SetDirty();
		}

		public override void CalculateLayoutInputVertical()
		{
			SetLayoutInputForAxis(minHeight, minHeight, -1, 1);
		}

		public override void SetLayoutHorizontal()
		{
			var width = rectTransform.rect.width - padding.left - padding.right;

			// Position visible rows at 0 x
			for (var i = 0; i < m_useRows.Count; i++)
			{
				var item = m_useRows[i];

				SetChildAlongAxis(item.RectTrans, 0, padding.left, width);
			}

			// Hide non-active rows to one side. More efficient than enabling/disabling them
			for (var i = 0; i < m_CacheRows.Count; i++)
			{
				var item = m_CacheRows[i];

				SetChildAlongAxis(item.RectTrans, 0, -width - padding.left, width);
			}
		}

		private float m_cacheCalculatePos;
		public override void SetLayoutVertical()
		{
			SetChildAlongAxis(m_headRectTrans, 1, 0, m_headPlaceHodlerHeight);
			for (var i = 0; i < m_useRows.Count; i++)
			{
				var item = m_useRows[i];
				m_cacheCalculatePos = m_headPlaceHodlerHeight + item.Index * ItemHeight + padding.top + Spacing * item.Index;
				SetChildAlongAxis(item.RectTrans, 1, m_cacheCalculatePos, ItemHeight);
			}
			SetChildAlongAxis(m_headRectTrans, 1, m_cacheCalculatePos + ItemHeight, m_tailPlaceHodlerHeight);
		}

		private new void SetDirty()
		{
			base.SetDirty();
			if (!IsActive())
				return;
			_isDirty = true;
		}

		private void Update()
		{
			//Debug.Log(ScrollRect.velocity.y);
			if (_isDirty)
			{
				_isDirty = false;
				UpdateScroll();
				AutoStopScroll();
			}
		}
		
		private void AutoStopScroll()
		{
			//Scroll停止检测
			if (Mathf.Abs(ScrollRect.velocity.y) < SelectScrollMinmunSpeedY)
			{
				//停止它
				ScrollRect.velocity = Vector2.zero;
				//移动rect到目标位置
				m_HighlightY = m_rectTransform.InverseTransformPoint(m_targetHighlightTrans.position).y;
				//find corresponding row
				for (int i = 0; i < m_useRows.Count; ++i)
				{
					float deltaYSign = m_useRows[i].RectTrans.localPosition.y - m_HighlightY;
					float deltaY = Mathf.Abs(deltaYSign);
					float halfSize = (ItemHeight + Spacing) * 0.5f;
					if (deltaY <= halfSize)
					{
						//这里暂时直接改动位置
						//TODO，smooth scroll~
						Vector2 pos = m_cacheTransform.localPosition;
						pos.y -= deltaYSign;
						m_cacheTransform.localPosition = pos;
						break;
					}
				}
			}
		}
		
		private void UpdateScroll()
		{
			if (!Application.isPlaying)
				return;
			bool isRealDrity = false;
			float startY = m_rectTransform.anchoredPosition.y;
			float viewHeight = m_scrollRectTransform.rect.height;
			int lowerIdx = Mathf.FloorToInt(startY / (ItemHeight + Spacing));
			int higherIdx = Mathf.CeilToInt((startY + viewHeight) / (ItemHeight + Spacing));
			lowerIdx -= RowPadding;
			higherIdx += RowPadding;
			lowerIdx = Mathf.Max(0, lowerIdx);
			higherIdx = Mathf.Min(m_dataCount, higherIdx);

			//
			for (int i = m_useRows.Count - 1; i >= 0; --i)
			{
				var row = m_useRows[i];
				//if is visible, left it visible
				if (row.Index >= lowerIdx && row.Index <= higherIdx)
					continue;
				//remove i
				m_useRows.RemoveAt(i);
				m_visibleRowIndexs.Remove(row.Index);
				RecycleRow(row);
				isRealDrity = true;
			}
			for (int i = lowerIdx; i < higherIdx; ++i)
			{
				if (m_visibleRowIndexs.Contains(i))
				{
					continue;
				}
				var addRow = GetRow(i, 0f, 0);
				//add i
				m_useRows.Add(addRow);
				m_visibleRowIndexs.Add(i);
				isRealDrity = true;
			}
			
			if (isRealDrity || _lastDataRows != m_useRows.Count)
			{
				m_tailRectTrans.SetAsLastSibling();
				LayoutRebuilder.MarkLayoutForRebuild(m_rectTransform);
				if (null != m_targetHighlightTrans)
				{
					m_HighlightY = m_rectTransform.InverseTransformPoint(m_targetHighlightTrans.position).y;
					//find corresponding row
					for (int i = 0; i < m_useRows.Count; ++i)
					{
						float deltaY = Mathf.Abs(m_useRows[i].RectTrans.localPosition.y - m_HighlightY);
						float halfSize = (ItemHeight + Spacing) * 0.5f;
						if (deltaY <= halfSize)
						{
							if (null != showContentHighlightDelegate)
								showContentHighlightDelegate(m_useRows[i].RectTrans.gameObject, m_useRows[i].Index);
							break;
						}
					}
				}
			}

			_lastDataRows = m_useRows.Count;
		}
		
		public void SetData(int totalCount)
		{
			m_dataCount = totalCount;
			SetDirty();
			if (Mathf.Approximately(ScrollRect.verticalNormalizedPosition, 0f))
			{
				ScrollRect.normalizedPosition = new Vector2(0, 0);
			}
		}
		
		public void AppendItem()
		{
			++m_dataCount;
			SetDirty();
			if (Mathf.Approximately(ScrollRect.verticalNormalizedPosition, 0f))
			{
				ScrollRect.normalizedPosition = new Vector2(0, 0);
			}
		}

		public void RemoveItem(int removeIdx)
		{
			//var dataIdx = m_dataList.IndexOf(data);
			RemoveRowAtIndex(removeIdx);
			--m_dataCount;
			RefreshIndexs();
			SetDirty();
		}

		private void RefreshIndexs()
		{
//			for (var i = 0; i < m_useRows.Count; i++)
//			{
//				m_useRows[i].Index = m_dataList.IndexOf(m_useRows[i].Data);
//			}
		}

		public void Clear()
		{
			//
			for (int i = m_useRows.Count - 1; i >= 0; --i)
			{
				RemoveRowAtIndex(m_useRows[i].Index);
			}

			m_dataCount = 0;
			SetDirty();
		}

		private void RemoveRowAtIndex(int idx)
		{
			if (!m_visibleRowIndexs.Contains(idx))
				return;
			m_visibleRowIndexs.Remove(idx);
			for (int i = 0; i < m_useRows.Count; ++i)
			{
				if (m_useRows[i].Index == idx)
				{
					RecycleRow(m_useRows[i]);
					m_useRows.RemoveAt(i);
					break;
				}
			}
		}

		private void Init()
		{
			m_dataCount = 0;
			m_useRows = new List<Row>();
			m_visibleRowIndexs = new List<int>();
			m_CacheRows = new List<Row>();
			_lastDataRows = 0;
		}

		public RectTransform RowPrefab;

		public override float minHeight
		{
			get { return m_dataCount * ItemHeight + padding.top + padding.bottom + Spacing * m_dataCount + m_headPlaceHodlerHeight + m_tailPlaceHodlerHeight; }
		}
		
		//TODO, cache itemHeight value.
		private float ItemHeight
		{
			get
			{
				LayoutElement layoutElement = RowPrefab.GetComponent<LayoutElement>();
				if (null != layoutElement)
					return layoutElement.preferredHeight;
				return RowPrefab.rect.height;
			}
		}

		private float ItemWidth
		{
			get
			{
				LayoutElement layoutElement = RowPrefab.GetComponent<LayoutElement>();
				if (null != layoutElement)
					return layoutElement.preferredWidth;
				return RowPrefab.rect.width;
			}
		}

		[SerializeField] public float Spacing;

		[SerializeField] public int TargetRow = 3;
		private int RowPadding = 1;

		private List<Row> m_useRows = new List<Row>();
		private List<int> m_visibleRowIndexs = new List<int>();
		private List<Row> m_CacheRows = new List<Row>();
		private int _lastDataRows;

		public class Row
		{
			public object Data;
			public RectTransform RectTrans;
			public int Index;
		}

		private void RecycleRow(Row row)
		{
			m_CacheRows.Add(row);
		}

		private Row PopRow()
		{
			if (m_CacheRows.Count == 0)
				return null;
			var row = m_CacheRows[m_CacheRows.Count - 1];
			m_CacheRows.RemoveAt(m_CacheRows.Count - 1);
			return row;
		}

		private Row GetRow(int getIndex, float rowPercent, int highlightIdx)
		{
			if (m_CacheRows.Count == 0)
			{
				var newRow = CreateRow();
				PopulateRow(getIndex, newRow, rowPercent, highlightIdx);
				return newRow;
			}

			var oldRow = PopRow();
			PopulateRow(getIndex, oldRow, rowPercent, highlightIdx);
			return oldRow;
		}

		//刷新row
		private void PopulateRow(int index, Row row, float rowPecent, int highlightIdx)
		{
			row.Index = index;
		//	row.Data = m_dataList[index];

			//row.View.ShowContent(row.Data, rowPecent);
			ShowContentView(row.RectTrans.gameObject, row.Index, rowPecent, highlightIdx);
		}

		private void ShowContentView(GameObject gobj, int index, float rowPecent, int highlightIdx)
		{
			if (null != showContentDelegate)
			{
				showContentDelegate(gobj, index, rowPecent, highlightIdx);
			}
			else
			{
				Debug.LogWarning( "showcoentn view delegate is NULL");
			}
		}
		
		private Row CreateRow()
		{
			var row = new Row();
			var rowGobj = InitiateItemPrefab();
			row.RectTrans = rowGobj.GetComponent<RectTransform>();

			row.RectTrans.SetParent(m_cacheTransform);
			row.RectTrans.localPosition = Vector3.zero;
			row.RectTrans.localScale = Vector3.one;
			return row;
		}

		#region initiate-prefabs

		private GameObject InitiateItemPrefab()
		{
			var gobj = GameObject.Instantiate(RowPrefab.gameObject);
			gobj.SetActive(true);
			return gobj;
		}

		#endregion

		#region Select-Function

		[SerializeField] public bool EnableSelect = true;

		[Serializable]
		public class SelectItemEvent : UnityEngine.Events.UnityEvent<object>
		{
		}

		[SerializeField] private SelectItemEvent m_selectItemEvent;

		public SelectItemEvent SelectEvent
		{
			get { return m_selectItemEvent; }
			set { m_selectItemEvent = value; }
		}

		public void OnPointerClick(PointerEventData eventData)
		{
			if (!EnableSelect)
				return;
			var raycastGobj = eventData.pointerPressRaycast.gameObject;
			if (null != raycastGobj)
			{
				var localPos = this.m_rectTransform.InverseTransformPoint(raycastGobj.transform.position);
				int idx = Mathf.FloorToInt(Mathf.Abs(localPos.y) / (ItemHeight + Spacing));
				if (idx >= 0 && idx < m_dataCount)
				{
					return;
					//TriggerSelectEvent(null);
				}
			}
		}

		private void TriggerSelectEvent(object data)
		{
			if (null != SelectEvent)
				SelectEvent.Invoke(data);
		}

		#endregion
	}
}