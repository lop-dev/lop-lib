using UnityEngine;
using System.Collections;

namespace UDLib.Utility
{
    [RequireComponent(typeof(Rigidbody))]
    public class MbGravity : MonoBehaviour
    {
        private MbMagnetic m_mbMagnetic;

        public MbMagnetic Magnetic
        {
            set { m_mbMagnetic = value; }
        }

        protected void Start()
        {
            m_mbMagnetic = MbMagnetic.MInstance;

            if (m_mbMagnetic != null)
            {
                GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
                GetComponent<Rigidbody>().useGravity = false;
            }
            
        }

        protected void LateUpdate()
        {
            if (m_mbMagnetic != null)
                m_mbMagnetic.AdjustRotation(transform);
        }

        protected void FixedUpdate()
        {
            if (m_mbMagnetic != null)
                m_mbMagnetic.Attract(transform);
        }
    }
}
