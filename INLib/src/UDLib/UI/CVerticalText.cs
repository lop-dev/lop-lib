//////////////////////////////////////////////////////////////////////
/// author:     League of Perfect (github.com/lop-dev/lop-lib)
/// @brief      文本竖排
//////////////////////////////////////////////////////////////////////
using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

namespace UDLib.UI
{
    [ExecuteInEditMode]
    public class CVerticalText : BaseMeshEffect
    {
        [Tooltip("字和字之间的距离")]
        public float spacing = 1;
        private float lineSpacing = 1;
        private float textSpacing = 1;
        private float xOffset = 0;
        private float yOffset = 0;

        public override void ModifyMesh(VertexHelper helper)
        {
            if (!IsActive())
                return;

            List<UIVertex> verts = new List<UIVertex>();
            helper.GetUIVertexStream(verts);

            Text text = GetComponent<Text>();



            TextGenerator tg = text.cachedTextGenerator;
            textSpacing = text.fontSize * text.lineSpacing;
            lineSpacing = text.fontSize * spacing;

            xOffset = text.rectTransform.sizeDelta.x / 2 - text.fontSize / 2;
            yOffset = text.rectTransform.sizeDelta.y / 2 - text.fontSize / 2;

            int lineWordsCnt = Mathf.FloorToInt(text.rectTransform.sizeDelta.y / (text.lineSpacing + text.fontSize));
            int rows = 0;
            for (int i = 0; i < helper.currentVertCount;)
            {
                for (int j = 0; j < lineWordsCnt; ++j)
                {
                    if (i >= helper.currentVertCount)
                        break;
                    //检查是否是换行符
                    if (text.text[i / 4].Equals('\n'))
                    {
                        i = i + 4;
                        break;
                    }
                    int charXPos = rows;
                    int charYPos = j;

                    UIVertex lb = new UIVertex();
                    helper.PopulateUIVertex(ref lb, i);

                    UIVertex lt = new UIVertex();
                    helper.PopulateUIVertex(ref lt, i + 1);

                    UIVertex rt = new UIVertex();
                    helper.PopulateUIVertex(ref rt, i + 2);

                    UIVertex rb = new UIVertex();
                    helper.PopulateUIVertex(ref rb, i + 3);

                    Vector3 center = Vector3.Lerp(lb.position, rt.position, 0.5f);
                    Matrix4x4 move = Matrix4x4.TRS(-center, Quaternion.identity, Vector3.one);

                    float x = -charXPos * lineSpacing + xOffset;
                    float y = -charYPos * textSpacing + yOffset;

                    Vector3 pos = new Vector3(x, y, 0);
                    Matrix4x4 place = Matrix4x4.TRS(pos, Quaternion.identity, Vector3.one);
                    Matrix4x4 transform = place * move;

                    lb.position = transform.MultiplyPoint(lb.position);
                    lt.position = transform.MultiplyPoint(lt.position);
                    rt.position = transform.MultiplyPoint(rt.position);
                    rb.position = transform.MultiplyPoint(rb.position);

                    helper.SetUIVertex(lb, i);
                    helper.SetUIVertex(lt, i + 1);
                    helper.SetUIVertex(rt, i + 2);
                    helper.SetUIVertex(rb, i + 3);

                    i = i + 4;
                }
                ++rows;
            }
        }
    }


}
