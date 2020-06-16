//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      
//////////////////////////////////////////////////////////////////////
using UnityEngine;

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