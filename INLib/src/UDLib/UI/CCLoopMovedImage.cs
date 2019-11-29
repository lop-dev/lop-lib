using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace UDLib.UI
{
    public class CCLoopMovedImage : MonoBehaviour
    {
        public float speedX = 0f;
        public float speedY = 0f;

        public float maxOffSetX = 0f;
        public float maxOffSetY = 0f;


        private float offsetX;
        private float offsetY;

        private Vector2 originPos;

        private RectTransform _transform;
        void Awake()
        {
            if (Mathf.Abs(speedX) < float.Epsilon && Mathf.Abs(speedY) < float.Epsilon || gameObject.GetComponent<RectTransform>() == null)
            {
                Debug.LogError("CLoopMovedImage param error");
            }
            _transform = gameObject.GetComponent<RectTransform>();
            originPos = _transform.anchoredPosition;
        }

        void Start()
        {
            offsetX = 0f;
            offsetY = 0f;
        }

        void Update()
        {
            offsetX += Time.deltaTime * speedX;

            if (Mathf.Abs(offsetX) >= maxOffSetX)
            {
                offsetX = 0;
            }

            offsetY += Time.deltaTime * speedY;

            if (Mathf.Abs(offsetY) >= maxOffSetY)
            {
                offsetY = 0;
            }

            _transform.anchoredPosition = originPos + new Vector2(offsetX, offsetY);
        }
    }
}

