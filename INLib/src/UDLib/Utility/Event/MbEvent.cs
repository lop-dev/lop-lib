using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MbEvent : MonoBehaviour
{
    public string m_eventTag = null;
    public UnityEngine.Events.UnityAction<string, GameObject> m_eventCallback = null;

    public void OnTriggerEvent(string tag)
    {
        if (m_eventTag != tag) return;
        if (m_eventCallback != null) m_eventCallback(tag, this.gameObject);

        Dispose();
    }

    public void Dispose()
    {
        Destroy(this);
    }
}
