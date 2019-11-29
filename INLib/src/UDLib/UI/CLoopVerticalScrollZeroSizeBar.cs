/* ***********************************************
 * LoopVerticalScrollZeroSizeBar
 * author :  created by asvo
 * function: 不设置滚动条size的垂直循环滚动
 * history:  created at .
 * ***********************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
    [DisallowMultipleComponent]
    public class CLoopVerticalScrollZeroSizeBar : CVerticalLoopScrollRect
    {
        protected override void UpdateScrollbars(Vector2 offset)
        {
            if (horizontalScrollbar)
            {
                horizontalScrollbar.value = horizontalNormalizedPosition;
            }

            if (verticalScrollbar)
            {
                verticalScrollbar.value = verticalNormalizedPosition;
            }
        }
    }
}