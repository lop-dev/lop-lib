using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
namespace UDLib.UI
{
	[RequireComponent(typeof(ParticleSystem))]
	public class CParticleMask : MonoBehaviour
	{
	   public RectMask2D mask;
	   public Material mt;
	   private void Awake()
	   {
		   mt = GetComponent<ParticleSystem>().GetComponent<Renderer>().material;
		   mask = GetComponentInParent<RectMask2D>();
		   // ScrollView位置变化时重新计算裁剪区域

		   var rect = GetComponentInParent<CLoopScrollRect>();

		   rect.onValueChanged.AddListener((e) => { setClip(); });

		   setClip();
	   }

	   void setClip()
	   {
		   Vector3[] wc = new Vector3[4];
		   mask.GetComponent<RectTransform>().GetWorldCorners(wc);        // 计算world space中的点坐标
		   var clipRect = new Vector4(wc[0].x, wc[0].y, wc[2].x, wc[2].y);// 选取左下角和右上角
		   mt.SetVector("_ClipRect", clipRect);                           // 设置裁剪区域
		   mt.SetFloat("_UseClipRect", 1.0f);                             // 开启裁剪
	   }
	}
}
