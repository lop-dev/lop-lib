using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace UDLib.Utility
{
    class CTransform
    {
        protected Transform m_transform = null;
        protected CTransform m_Parent = null;
        private CSLib.Utility.CQueue<CTransform> m_Children = new CSLib.Utility.CQueue<CTransform>();

        //
        public CTransform()
        {
        }

        public CTransform(Transform refTransform)
        {
            m_transform = refTransform;
        }

        public CTransform(GameObject gameObject)
        {
            m_transform = gameObject.transform;
        }

        //
        public Transform transform
        {
            get { return m_transform; }
            set { m_transform = value; }
        }

        public CTransform Parent
        {
            get { return m_Parent; }
            set { m_Parent = value; }
        }

        public GameObject gameObject
        {
            get { return m_transform.gameObject; }
            set { m_transform = value.transform; }
        }

        //
        public void AddChild(CTransform transform)
        {
            if (transform != null)
            {
                transform.Parent = this;
                m_Children.AddObject(transform);
            }
        }

        public void DelChild(CTransform transform)
        {
            if (transform != null)
            {
                transform.Parent = null;
                m_Children.DelObject(transform);
            }
        }

        public void AddChild(GameObject gameObject)
        {
            if (gameObject != null)
            {
                CTransform transform = new CTransform();
                transform.Parent = this;
                m_Children.AddObject(transform);
            }
        }

        public void DelChild(GameObject gameObject)
        {
            if (gameObject != null)
            {
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    if (tmpObj.m_transform == gameObject.transform)
                    {
                        m_Children.DelObject(tmpObj);
                        break;
                    }
                }
            }
        }

        //
        public void Reset()
        {
            m_transform.localPosition = Vector3.zero;
            m_transform.localScale = Vector3.zero;

            foreach (CTransform tmpObj in m_Children.Objects)
            {
                tmpObj.Reset();
            }
        }

        public void ResetPosition()
        {
            m_transform.localPosition = Vector3.zero;
            foreach (CTransform tmpObj in m_Children.Objects)
            {
                tmpObj.ResetPosition();
            }
        }

        public void ResetScale()
        {
            m_transform.localScale = Vector3.zero;
            foreach (CTransform tmpObj in m_Children.Objects)
            {
                tmpObj.ResetScale();
            }
        }

        //
        public Vector3 Position
        {
            get { return m_transform.localPosition; }
            set
            {
                m_transform.localPosition = value;
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.Scale = value;
                }
            }
        }

        public float PositionX
        {
            get { return m_transform.localPosition.x; }
            set
            {
                m_transform.localPosition = new Vector3(value, m_transform.localPosition.y, m_transform.localPosition.z);
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.PositionX = value;
                }
            }
        }

        public float PositionY
        {
            get { return m_transform.localPosition.y; }
            set
            {
                m_transform.localPosition = new Vector3(m_transform.localPosition.x, value, m_transform.localPosition.z);
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.PositionY = value;
                }
            }
        }

        public float PositionZ
        {
            get { return m_transform.localPosition.z; }
            set
            {
                m_transform.localPosition = new Vector3(m_transform.localPosition.x, m_transform.localPosition.y, value);
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.PositionZ = value;
                }
            }
        }

        //
        public Vector3 Scale
        {
            get { return m_transform.localScale; }
            set
            {
                m_transform.localScale = value;
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.Scale = value;
                }
            }
        }

        public float ScaleX
        {
            get { return m_transform.localScale.x; }
            set
            {
                m_transform.localScale = new Vector3(value, m_transform.localScale.y, m_transform.localScale.z);
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.ScaleX = value;
                }
            }
        }

        public float ScaleY
        {
            get { return m_transform.localScale.y; }
            set
            {
                m_transform.localScale = new Vector3(m_transform.localScale.x, value, m_transform.localScale.z);
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.ScaleY = value;
                }
            }
        }

        public float ScaleZ
        {
            get { return m_transform.localScale.z; }
            set
            {
                m_transform.localScale = new Vector3(m_transform.localScale.x, m_transform.localScale.y, value);
                foreach (CTransform tmpObj in m_Children.Objects)
                {
                    tmpObj.ScaleZ = value;
                }
            }
        }
    }
}