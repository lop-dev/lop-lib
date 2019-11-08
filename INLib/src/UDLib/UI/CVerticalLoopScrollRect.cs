using UnityEngine;
using UnityEngine.UI;
using System.Collections;

namespace UDLib.UI
{
    //[AddComponentMenu("UI/Loop Vertical Scroll Rect", 51)]
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
                else if(trans.name.Replace("_", "").StartsWith("Content"))
                {
                    content = trans as RectTransform;
                }
                else if(trans.name.StartsWith("ViewPort", System.StringComparison.OrdinalIgnoreCase))
                {
                    viewport = trans as RectTransform;
                    for (int j = 0; j<trans.childCount;j++)
                    {
                        var temp = trans.GetChild(j);
                        if (temp.name.Replace("_", "").StartsWith("Content"))
                        {
                            content = temp as RectTransform;
                        }
                    }
                  
                }
                else if(trans.name.Replace("_", "").StartsWith("Scb"))
                {
                    verticalScrollbar = trans.GetComponent<Scrollbar>();
                    verticalScrollbarVisibility = ScrollbarVisibility.AutoHide;
                }
            }
            if (itemTemplates.Count > 0)
                itemTemplate = itemTemplates[0];

            
#if ACG_ENABLE_LOG_LOG //@ÅÓÐ£³¤ ÐÞ¸Ä
            GridLayoutGroup layout = content.GetComponent<GridLayoutGroup>();
            if (layout != null && layout.constraint != GridLayoutGroup.Constraint.FixedColumnCount)
            {
                Debug.LogError("GridLayoutGroup need FixedColumnCount");
            }
#endif
        }

        protected override bool UpdateItems(Bounds viewBounds, Bounds contentBounds)
        {
            bool changed = false;
            if (viewBounds.min.y < contentBounds.min.y)
            {
                float size = NewItemAtEnd(), totalSize = size;
                while (size > 0 && viewBounds.min.y < contentBounds.min.y - totalSize)
                {
                    size = NewItemAtEnd();
                    totalSize += size;
                }
                if (totalSize > 0)
                    changed = true;
            }
            else if ( viewBounds.min.y > contentBounds.min.y + threshold)
            {
                float size = DeleteItemAtEnd(), totalSize = size;
                while (size > 0 && viewBounds.min.y > contentBounds.min.y + threshold + totalSize)
                {
                    size = DeleteItemAtEnd();
                    totalSize += size;
                }
                if (totalSize > 0)
                    changed = true;
            }

            if (viewBounds.max.y > contentBounds.max.y)
            {
                float size = NewItemAtStart(), totalSize = size;
                while (size > 0 && viewBounds.max.y > contentBounds.max.y + totalSize)
                {
                    size = NewItemAtStart();
                    totalSize += size;
                }
                if (totalSize > 0)
                    changed = true;
            }
            else if ( viewBounds.max.y < contentBounds.max.y - threshold)
            {
                float size = DeleteItemAtStart(), totalSize = size;
                while (size > 0 && viewBounds.max.y < contentBounds.max.y - threshold - totalSize)
                {
                    size = DeleteItemAtStart();
                    totalSize += size;
                }
                if (totalSize > 0)
                    changed = true;
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

            if(vertical)
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