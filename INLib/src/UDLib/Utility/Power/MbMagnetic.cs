using UnityEngine;
using System.Collections;

namespace UDLib.Utility
{
    public class MbMagnetic : MonoBehaviour
    {
        public float gravity = -40f;
        public float rotationSpeed = 80.0f;

        public static MbMagnetic MInstance
        {
            get;
            private set;
        }

        protected void Awake()
        {
            MInstance = this;
        }

        public void Attract(Transform body)
        {
            Vector3 gravityUp = (body.position - transform.position).normalized;

            body.GetComponent<Rigidbody>().AddForce(gravityUp * gravity);
        }

        public void AdjustRotation(Transform body)
        {
            Vector3 gravityUp = (body.position - transform.position).normalized;
            Vector3 bodyUp = body.up;

            if (Vector3.Dot(gravityUp, bodyUp) > 0.95f) return; //为了减少抖动

            Quaternion targetRotation = Quaternion.FromToRotation(bodyUp, gravityUp) * body.rotation;
            body.rotation = Quaternion.Slerp(body.rotation, targetRotation, rotationSpeed * Time.deltaTime);
        }
    }
}

