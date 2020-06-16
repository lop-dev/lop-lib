//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      
//////////////////////////////////////////////////////////////////////
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
    [DisallowMultipleComponent]
    public class CVerticalLoopScrollRect : CLoopScrollRect
    {
        protected override float GetSize(RectTransform item)
        {
            float size = contentSpacing;
            if (m_GridLayout != null)
            {
                size += m_GridLayout.cellSize.y;
            }
            else
            {
                size += LayoutUtility.GetPreferredHeight(item);
                //  Debug.Log("+++++++++++GetSize:" + LayoutUtility.GetPreferredHeight(item) +"name:"+ item.name);
            }
            return size;
        }

        protected override float GetDimension(Vector2 vector)
        {
            return vector.y;
        }

        protected override Vector2 GetVector(float value)
        {
            return new Vector2(0, value);
        }

        protected override void Awake()
        {
            base.Awake();
            directionSign = -1;
            horizontal = false;
            vertical = true;


            viewport = transform as RectTransform;
            itemTemplates.Clear();
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
                    verticalScrollbar = trans.GetComponent<Scrollbar>();
                    verticalScrollbarVisibility = ScrollbarVisibility.AutoHide;
                }
            }
            if (itemTemplates.Count > 0)
                itemTemplate = itemTemplates[0];

            // 临时调试日志
            //GridLayoutGroup layout = content.GetComponent<GridLayoutGroup>();
            //if (layout != null && layout.constraint != GridLayoutGroup.Constraint.FixedColumnCount)
            //{
            //    Debug.LogError("GridLayoutGroup need FixedColumnCount");
            //}
        }

        //protected override bool UpdateItems(Bounds viewBounds, Bounds contentBounds)
        //{
        //    bool changed = false;
        //    if (viewBounds.min.y < contentBounds.min.y)
        //    {
        //        float size = NewItemAtEnd(), totalSize = size;
        //        while (size > 0 && viewBounds.min.y < contentBounds.min.y - totalSize)
        //        {
        //            size = NewItemAtEnd();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }
        //    else if (viewBounds.min.y > contentBounds.min.y + threshold)
        //    {
        //        float size = DeleteItemAtEnd(), totalSize = size;
        //        while (size > 0 && viewBounds.min.y > contentBounds.min.y + threshold + totalSize)
        //        {
        //            size = DeleteItemAtEnd();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }

        //    if (viewBounds.max.y > contentBounds.max.y)
        //    {
        //        float size = NewItemAtStart(), totalSize = size;
        //        while (size > 0 && viewBounds.max.y > contentBounds.max.y + totalSize)
        //        {
        //            size = NewItemAtStart();
        //            totalSize += size;
        //        }
        //        if (totalSize > 0)
        //            changed = true;
        //    }
        //    else if (viewBounds.max.y < contentBounds.max.y - threshold)
        //    {
        //        float size = DeleteItemAtStart(), totalSize = size;
        //        while (size > 0 && viewBounds.max.y < contentBounds.max.y - threshold - totalSize)
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

                if (viewBounds.min.y < contentBounds.min.y - totalSize1)
                {
                    float size = NewItemAtEnd();
                    totalSize1 += size;

                    if (totalSize1 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }
                else if (viewBounds.min.y > contentBounds.min.y + threshold + totalSize2)
                {
                    float size = DeleteItemAtEnd();
                    totalSize2 += size;

                    if (totalSize2 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }

                if (viewBounds.max.y > contentBounds.max.y + totalSize3)
                {
                    float size = NewItemAtStart();
                    totalSize3 += size;

                    if (totalSize3 > 0 && size > 0)
                    {
                        changed = true;

                        bCanJump = false;
                    }
                }
                else if (viewBounds.max.y < contentBounds.max.y - threshold - totalSize4)
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

            if (vertical)
            {
                if (System.Math.Abs(content.anchoredPosition.y - position.y) < 0.01f)
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
    }
}