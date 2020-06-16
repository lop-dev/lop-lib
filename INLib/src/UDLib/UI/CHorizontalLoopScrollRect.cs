//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      
//////////////////////////////////////////////////////////////////////
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
    [DisallowMultipleComponent]
    public class CHorizontalLoopScrollRect : CLoopScrollRect
    {
        protected override float GetSize(RectTransform item)
        {
            float size = contentSpacing;
            if (m_GridLayout != null)
            {
                size += m_GridLayout.cellSize.x;
            }
            else
            {
                size += LayoutUtility.GetPreferredWidth(item);
            }
            return size;
        }

        protected override float GetDimension(Vector2 vector)
        {
            return -vector.x;
        }

        protected override Vector2 GetVector(float value)
        {
            return new Vector2(-value, 0);
        }

        protected override void Awake()
        {
            base.Awake();
            directionSign = 1;
            horizontal = true;
            vertical = false;

            itemTemplates.Clear();
            viewport = transform as RectTransform;
            for (int i = 0; i < transform.childCount; i++)
            {
                var trans = transform.GetChild(i);
                if (trans.name.Replace("_", "").StartsWith("Template"))
                {
                    itemTemplates.Add(trans.gameObject);
                    trans.gameObject.SetActive(false);
                }
                else if (trans.name.Replace("_", "").StartsWith("Content"))
                {
                    content = trans as RectTransform;
                }
                else if (trans.name.StartsWith("ViewPort", System.StringComparison.OrdinalIgnoreCase))
                {
                    viewport = trans as RectTransform;
                    for (int j = 0; j < trans.childCount; j++)
                    {
                        var temp = trans.GetChild(j);
                        if (temp.name.Replace("_", "").StartsWith("Content"))
                        {
                            content = temp as RectTransform;
                        }
                    }
                }
                else if (trans.name.Replace("_", "").StartsWith("Scb"))
                {
                    horizontalScrollbar = trans.GetComponent<Scrollbar>();
                    horizontalScrollbarVisibility = ScrollbarVisibility.AutoHide;
                }
            }
            if (itemTemplates.Count > 0)
                itemTemplate = itemTemplates[0];

            // 临时调试日志
            //GridLayoutGroup layout = content.GetComponent<GridLayoutGroup>();
            //if (layout != null && layout.constraint != GridLayoutGroup.Constraint.FixedRowCount)
            //{
            //    Debug.LogError("GridLayoutGroup need FixedColumnCount");
            //}
        }

        //protected override bool UpdateItems(Bounds viewBounds, Bounds contentBounds)
        //{
        //    bool changed = false;
        //    if (viewBounds.max.x > contentBounds.max.x)
        //    {
        //        float size = NewItemAtEnd(), totalSize = size;
        //        while (size > 0 && viewBounds.max.x > contentBounds.max.x + totalSize)
        //        {
        //            size = NewItemAtEnd();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }
        //    else if (viewBounds.max.x < contentBounds.max.x - threshold)
        //    {
        //        float size = DeleteItemAtEnd(), totalSize = size;
        //        while (size > 0 && viewBounds.max.x < contentBounds.max.x - threshold - totalSize)
        //        {
        //            size = DeleteItemAtEnd();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }

        //    if (viewBounds.min.x < contentBounds.min.x)
        //    {
        //        float size = NewItemAtStart(), totalSize = size;
        //        while (size > 0 && viewBounds.min.x < contentBounds.min.x - totalSize)
        //        {
        //            size = NewItemAtStart();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }
        //    else if (viewBounds.min.x > contentBounds.min.x + threshold)
        //    {
        //        float size = DeleteItemAtStart(), totalSize = size;
        //        while (size > 0 && viewBounds.min.x > contentBounds.min.x + threshold + totalSize)
        //        {
        //            size = DeleteItemAtStart();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }
        //    return changed;
        //}

        protected override bool UpdateItems(Bounds viewBounds, Bounds contentBounds)
        {
            bool changed = false;
            float totalSize1 = 0, totalSize2 = 0, totalSize3 = 0, totalSize4 = 0;

            while (true)
            {
                bool bCanJump = true;

                if (viewBounds.max.x > contentBounds.max.x + totalSize1)
                {
                    float size = NewItemAtEnd();
                    totalSize1 += size;

                    if (totalSize1 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }
                else if (viewBounds.max.x < contentBounds.max.x - threshold - totalSize2)
                {
                    float size = DeleteItemAtEnd();
                    totalSize2 += size;

                    if (totalSize2 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }

                if (viewBounds.min.x < contentBounds.min.x - totalSize3)
                {
                    float size = NewItemAtStart();
                    totalSize3 += size;

                    if (totalSize3 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }
                else if (viewBounds.min.x > contentBounds.min.x + threshold + totalSize4)
                {
                    float size = DeleteItemAtStart();
                    totalSize4 += size;

                    if (totalSize4 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }

                if (bCanJump)
                    break;
            }
            return changed;
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

            if (horizontal)
            {
                if (System.Math.Abs(content.anchoredPosition.x - position.x) < 0.01f)
                {
                    return;
                }
            }

            if (position != content.anchoredPosition)
            {
                content.anchoredPosition = position;
                UpdateBounds(true);
            }
        }

        //外部引用，布阵scrollrectbar进度条显示数字
        public void SetScrollRectNum(Text txt)
        {
            txt.text = string.Format("{0}/{1}", Mathf.Round(horizontalScrollbar.value * totalCount), totalCount);
        }
    }
}