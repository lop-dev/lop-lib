using System.Collections;
using System.Collections.Generic;
using UnityEngine;
namespace UDLib.UI {
	public class CUILookAtCamera : MonoBehaviour {

		public bool isFaceCamera = false;//水平方向看向摄像机

		// Use this for initialization
		void Start()
		{

		}

		// Update is called once per frame
		void Update()
		{
			if (gameObject.activeSelf && Camera.main != null)
			{
				if (isFaceCamera)
					transform.forward = Camera.main.transform.forward;
				else
					transform.forward = transform.position - Camera.main.transform.position;
			}
		}

		private void OnEnable()
		{
			if (Camera.main != null)
			{
				if (isFaceCamera)
					transform.forward = Camera.main.transform.forward;
				else
					transform.forward = transform.position - Camera.main.transform.position;
			}
		}

	}

}


