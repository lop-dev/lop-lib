/// <summary>
/// Generic Mono singleton.
/// </summary>
using UnityEngine;

namespace UDLib.Utility
{

    public abstract class CMonoSingleton<T> : MonoBehaviour where T : CMonoSingleton<T>
    {
        private static bool applicationIsQuitting = false;
        private static T m_Instance = null;
        
	    public static T Instance
        {
            get
            {
                if (applicationIsQuitting)
                    return null;

			    if( m_Instance == null )
                {
            	    m_Instance = GameObject.FindObjectOfType(typeof(T)) as T;
                    if( m_Instance == null )
                    {
                        m_Instance = new GameObject("Singleton of " + typeof(T).ToString(), typeof(T)).GetComponent<T>();
                        m_Instance.Init();
                    }
                }
                return m_Instance;
            }
        }

        protected virtual void Awake()
        {
            if( m_Instance == null )
            {
                m_Instance = this as T;
                DontDestroyOnLoad(this.gameObject);
            }
        }

        protected void OnDestroy()
        {
            m_Instance = null;
        }


        public virtual void Init()
        {
        }
     
        private void OnApplicationQuit()
        {
            applicationIsQuitting = true;
            m_Instance = null;
        }
    }
}