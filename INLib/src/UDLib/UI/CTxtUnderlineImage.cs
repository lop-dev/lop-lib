/* ***********************************************
 * TxtUnderlineImage
 * author :  created by asvo
 * function: 文本的下划线图，配合TxtUnderline使用
 * history:  created at .
 * ***********************************************/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
namespace UDLib.UI {
	public class CTxtUnderlineImage : BaseMeshEffect {

		public Color color = Color.white;
		public Sprite underlineSp;
		public float underlineHeight = 10f;  //px

		private List<Vector2> m_renderVertPoss = new List<Vector2>();

		public override void ModifyMesh(VertexHelper helper)
		{
			helper.Clear();

			Vector4 m_SpriteUv = (underlineSp != null) ? UnityEngine.Sprites.DataUtility.GetOuterUV(underlineSp) : Vector4.zero;
			int lineIdex = 0;
			for(int i = 0; i < m_renderVertPoss.Count; i+=2)
			{
				helper.AddVert(new Vector3(m_renderVertPoss[i].x, m_renderVertPoss[i].y - underlineHeight), color, new Vector2(m_SpriteUv.x, m_SpriteUv.y));
				helper.AddVert(new Vector3(m_renderVertPoss[i].x, m_renderVertPoss[i].y), color, new Vector2(m_SpriteUv.x, m_SpriteUv.w));
				helper.AddVert(new Vector3(m_renderVertPoss[i+1].x, m_renderVertPoss[i+1].y), color, new Vector2(m_SpriteUv.z, m_SpriteUv.w));
				helper.AddVert(new Vector3(m_renderVertPoss[i+1].x, m_renderVertPoss[i+1].y - underlineHeight), color, new Vector2(m_SpriteUv.z, m_SpriteUv.y));

				helper.AddTriangle(lineIdex, lineIdex+1, lineIdex+2);
				helper.AddTriangle(lineIdex+2, lineIdex+3, lineIdex);
				lineIdex += 4;
			}
		}
		
		public void SetVertexs(List<Vector2> vertlist)
		{
			m_renderVertPoss = vertlist;

			//this.graphic.SetVerticesDirty();
			//this.OnValidate();
		}
	}
}

