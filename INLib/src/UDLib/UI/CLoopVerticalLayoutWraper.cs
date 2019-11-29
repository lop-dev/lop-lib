using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
	public class CLoopVerticalLayoutWraper : MonoBehaviour
	{
		public CLoopVerticalLayout LoopVerticalLayout;
		private Vector2? m_scrollVec;
		private bool m_isDirty;
		private int m_totalCnt;
		
		private void Start()
		{
			LoopVerticalLayout.SelectEvent.AddListener(OnSelectLogItem);
		}
		
		private void OnSelectLogItem(object data)
		{
			//DMLogEntry logEntry = data as DMLogEntry;
			
		}
		
		public void InitLoop(LoopShowContentDelegate showContentDelegate, LoopShowContentHighlightDelegate highlightDelegate)
		{
			LoopVerticalLayout.showContentDelegate = showContentDelegate;
			LoopVerticalLayout.showContentHighlightDelegate = highlightDelegate;
		}
		
		public void SetLoopData(int totalCnt)
		{
			m_totalCnt = totalCnt;
			SetDirty();
		}
		
		private void SetDirty()
		{
			m_isDirty = true;
		}
		
		private void Update()
		{
			if (m_scrollVec.HasValue)
			{
				LoopVerticalLayout.ScrollRect.normalizedPosition = m_scrollVec.Value;
				m_scrollVec = null;
			}
			if (m_isDirty)
			{
				m_isDirty = false;
				ShowItems();
			}
		}
		
		private static bool CheckApproximateValue(float targetVal, float compVal)
		{
			return Mathf.Abs(compVal - targetVal) < 0.05f;
		}
		
		private void ShowItems()
		{
			//保持置顶或置底
			if (CheckApproximateValue(0f, LoopVerticalLayout.ScrollRect.normalizedPosition.y)
			    || CheckApproximateValue(1f, LoopVerticalLayout.ScrollRect.normalizedPosition.y))
			{
				m_scrollVec = LoopVerticalLayout.ScrollRect.normalizedPosition;
			}
			LoopVerticalLayout.Clear();
			if (0 != m_totalCnt)
				LoopVerticalLayout.SetData(m_totalCnt);
		}
	}
}