using UnityEngine;
using System.Collections;

namespace UDLib.Utility
{
    public class CGameObject
    {
        private GameObject m_GameObject = null;

        //
        public CGameObject()
        {
            ;
        }

        public CGameObject(GameObject gameObject)
        {
            m_GameObject = gameObject;
        }

        //
        public GameObject gameObject
        {
            get { return m_GameObject; }
            set { m_GameObject = value; }
        }

        public CGameObject parent
        {
            get
            {
                if ((m_GameObject == null) || (m_GameObject.transform.parent == null))
                {
                    return null;
                }
                return new CGameObject(m_GameObject.transform.parent.gameObject);
            }
            set
            {
                if (m_GameObject != null)
                {
                    if (value == null)
                    {
                        m_GameObject.transform.parent = null;
                    }
                    else
                    {
                        m_GameObject.transform.parent = value.gameObject.transform;
                    }
                }
            }
        }

        //
        public Vector3 localPosition
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero;
                }
                return m_GameObject.transform.localPosition;
            }
            set
            {
                if (m_GameObject != null)
                {
                    m_GameObject.transform.localPosition = value;
                }
            }
        }

        public float localPositionX
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero.x;
                }
                return m_GameObject.transform.localPosition.x;
            }
            set
            {
                m_GameObject.transform.localPosition = new Vector3(value, m_GameObject.transform.localPosition.y, m_GameObject.transform.localPosition.z);
            }
        }

        public float localPositionY
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero.y;
                }
                return m_GameObject.transform.localPosition.y;
            }
            set
            {
                m_GameObject.transform.localPosition = new Vector3(m_GameObject.transform.localPosition.x, value, m_GameObject.transform.localPosition.z);
            }
        }

        public float localPositionZ
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero.z;
                }
                return m_GameObject.transform.localPosition.z;
            }
            set
            {
                m_GameObject.transform.localPosition = new Vector3(m_GameObject.transform.localPosition.x, m_GameObject.transform.localPosition.y, value);
            }
        }

        //
        public Vector3 localScale
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero;
                }
                return m_GameObject.transform.localScale;
            }
            set
            {
                if (m_GameObject != null)
                {
                    m_GameObject.transform.localScale = value;
                }
            }
        }

        public float localScaleX
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero.x;
                }
                return m_GameObject.transform.localScale.x;
            }
            set
            {
                m_GameObject.transform.localScale = new Vector3(value, m_GameObject.transform.localScale.y, m_GameObject.transform.localScale.z);
            }
        }

        public float localScaleY
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero.y;
                }
                return m_GameObject.transform.localScale.y;
            }
            set
            {
                m_GameObject.transform.localScale = new Vector3(m_GameObject.transform.localScale.x, value, m_GameObject.transform.localScale.z);
            }
        }

        public float localScaleZ
        {
            get
            {
                if (m_GameObject == null)
                {
                    return Vector3.zero.z;
                }
                return m_GameObject.transform.localScale.z;
            }
            set
            {
                m_GameObject.transform.localScale = new Vector3(m_GameObject.transform.localScale.x, m_GameObject.transform.localScale.y, value);
            }
        }

        /// <summary>
        /// 根据路径，获取子物件
        /// </summary>
        /// <param name="curObj"></param>
        /// <param name="strPath"></param>
        /// <returns></returns>
        public static GameObject FindChildObject(GameObject curObj, string strPath)
        {
            if (strPath == "")
            {
                return null;
            }

            Transform tmpTrans = null;
            if (curObj != null)
            {
                tmpTrans = curObj.transform.Find(strPath);
            }
            else
            {
                GameObject go = GameObject.Find(strPath);
                if (go != null)
                {
                    tmpTrans = go.transform;
                }
            }

            if (tmpTrans == null)
            {
                UDLib.Utility.CDebugOut.LogWarning(string.Format("Can't Find Child Obj of ({0}) at path [{1}]", curObj, strPath));
                return null;
            }

            return tmpTrans.gameObject;
        }

        /// <summary>
        /// 根据路径，获取子物件的组件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="curObj"></param>
        /// <param name="strPath"></param>
        /// <returns></returns>
        public static T FindChildComponent<T>(GameObject curObj, string strPath) where T : Component
        {
            GameObject tmpl = FindChildObject(curObj, strPath);
            if (tmpl == null) return null;

            return tmpl.GetComponent<T>();
        }

        //
        public static void SetLocalPositionX(GameObject gameObject, float value)
        {
            if (gameObject != null)
            {
                gameObject.transform.localPosition = new Vector3(value, gameObject.transform.localPosition.y, gameObject.transform.localPosition.z);
            }
        }

        public static void SetLocalPositionY(GameObject gameObject, float value)
        {
            if (gameObject != null)
            {
                gameObject.transform.localPosition = new Vector3(gameObject.transform.localPosition.x, value, gameObject.transform.localPosition.z);
            }
        }

        public static void SetLocalPositionZ(GameObject gameObject, float value)
        {
            if (gameObject != null)
            {
                gameObject.transform.localPosition = new Vector3(gameObject.transform.localPosition.x, gameObject.transform.localPosition.y, value);
            }
        }

        //
        public static void SetLocalScaleX(GameObject gameObject, float value)
        {
            if (gameObject != null)
            {
                gameObject.transform.localScale = new Vector3(value, gameObject.transform.localScale.y, gameObject.transform.localScale.z);
            }
        }

        public static void SetLocalScaleY(GameObject gameObject, float value)
        {
            if (gameObject != null)
            {
                gameObject.transform.localScale = new Vector3(gameObject.transform.localScale.x, value, gameObject.transform.localScale.z);
            }
        }

        public static void SetLocalScaleZ(GameObject gameObject, float value)
        {
            if (gameObject != null)
            {
                gameObject.transform.localScale = new Vector3(gameObject.transform.localScale.x, gameObject.transform.localScale.y, value);
            }
        }
    }
}

