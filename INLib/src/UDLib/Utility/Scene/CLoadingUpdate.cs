using UnityEngine;
using System.Collections;

namespace UDLib.Utility
{
    public abstract class CLoadingUpdate : MonoBehaviour
    {
        public abstract void UpdateLoadingTip(string strTip);

        public abstract void UpdateLoadingValue(int iProgress);
    }
}

