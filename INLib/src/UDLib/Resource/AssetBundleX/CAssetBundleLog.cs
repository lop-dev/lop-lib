namespace UDLib.Resource
{
    public class CAssetBundleLog
    {
        public static void Log(object message)
        {
            if (MbAssetBundleManager.Instance.EnableLog == false)
                return;

            UDLib.Utility.CDebugOut.Log(message);
        }

        public static void LogError(object message)
        {
            //if (MbAssetBundleManager.Instance.EnableLog == false)
            //    return;

            UDLib.Utility.CDebugOut.LogError(message);
        }

        public static void LogWarning(object message)
        {
            //if (MbAssetBundleManager.Instance.EnableLog == false)
            //    return;

            UDLib.Utility.CDebugOut.LogWarning(message);
        }
    }
}
