using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/// <summary>
/// 此 Event 主要是处理 MonoBehaviour 相关的事件，于 MbEvent 配合使用
/// </summary>
public class MbEventMgr : MonoBehaviour
{
    public UnityEngine.Events.UnityEvent onStartEvent = new UnityEngine.Events.UnityEvent();
    public UnityEngine.Events.UnityEvent onUpdateEvent = new UnityEngine.Events.UnityEvent();
    public UnityEngine.Events.UnityEvent onClickEvent = new UnityEngine.Events.UnityEvent();
    public UnityEngine.Events.UnityEvent onEnableEvent = new UnityEngine.Events.UnityEvent();
    public UnityEngine.Events.UnityEvent onDisableEvent = new UnityEngine.Events.UnityEvent();
    public UnityEngine.Events.UnityEvent onDestroyEvent = new UnityEngine.Events.UnityEvent();

    public bool RegisterEvent(GameObject target, string eventTag, UnityEngine.Events.UnityAction<string, GameObject> eventCallback)
    {
        MbEvent mbTag = target.AddComponent<MbEvent>();
        mbTag.m_eventTag = eventTag;
        mbTag.m_eventCallback = eventCallback;
        return true;
    }

    public void UnRegisterEvent(GameObject target, string eventTag)
    {
        MbEvent[] mbTagList = target.GetComponents<MbEvent>();
        foreach (MbEvent mbTag in mbTagList)
        {
            if (mbTag.m_eventTag == eventTag)
            {
                mbTag.Dispose();
                break;
            }
        }
    }

    public void Start()
    {
        if (onStartEvent != null)
        {
            onStartEvent.Invoke();
        }
    }

    public void Update()
    {
        if (onUpdateEvent != null)
        {
            onUpdateEvent.Invoke();
        }
    }

    public void OnClick()
    {
        if (onClickEvent != null)
        {
            onClickEvent.Invoke();
        }
    }

    public void OnEnable()
    {
        if (onEnableEvent != null)
        {
            onEnableEvent.Invoke();
        }
    }

    public void OnDisable()
    {
        if (onDisableEvent != null)
        {
            onDisableEvent.Invoke();
        }
    }

    public void OnDestroy()
    {
        if (onDestroyEvent != null)
        {
            onDestroyEvent.Invoke();
        }
    }
}
