using System;
using System.Collections;
using System.Collections.Generic;

namespace UDLib.Utility
{
    public class CTimer
    {
        private static int sTickerID = 0;
        private static Dictionary<int, CTicker> sTickerMap = new Dictionary<int, CTicker>();
        private static ArrayList funcList = new ArrayList();

        public static int SetTimeout(CSLib.Utility.Action closure, float delay, int repeatCount = 1)
        {
            CTimeTicker timeTicker = new CTimeTicker(delay, repeatCount, null);
            timeTicker.CompleteFuncEmpty = closure;
            timeTicker.ATickCompleteFunc = SetTimeoutComplete;
            timeTicker.Start();
            int num = timeTicker.id = ++sTickerID;
            sTickerMap[num] = timeTicker;
            return num;
        }

        public static int SetTimeout(CSLib.Utility.Action<object> closure, float delay, object data = null, int repeatCount = 1)
        {
            CTimeTicker timeTicker = new CTimeTicker(delay, repeatCount, data);
            timeTicker.CompleteFuncData = closure;
            timeTicker.ATickCompleteFunc = SetTimeoutComplete;
            timeTicker.Start();
            int num = timeTicker.id = ++sTickerID;
            sTickerMap[num] = timeTicker;
            return num;
        }

        private static void SetTimeoutComplete(CTicker ticker)
        {
            if (ticker.CompleteFuncEmpty != null)
            {
                ticker.CompleteFuncEmpty();
            }
            else if (ticker.CompleteFuncData != null)
            {
                ticker.CompleteFuncData(ticker.data);
            }
            ClearTimeout(ticker.id);
        }

        public static int SetInterval(CSLib.Utility.Action UpdateFunc, float delay)
        {
            if (funcList.IndexOf(UpdateFunc) == -1)
            {
                funcList.Add(UpdateFunc);
                CTimeTicker timeTicker = new CTimeTicker(delay, 0, null);
                timeTicker.TimerFuncEmpty = UpdateFunc;
                timeTicker.Start();
                int num = timeTicker.id = ++sTickerID;
                sTickerMap[num] = timeTicker;
                return num;
            }
            throw new Exception("this function couldn't be setInterval twice!");
        }

        public static int SetInterval(CSLib.Utility.Action<object> UpdateFunc, float delay, object data = null)
        {
            if (funcList.IndexOf(UpdateFunc) == -1)
            {

                funcList.Add(UpdateFunc);
                CTimeTicker timeTicker = new CTimeTicker(delay, 0, data);
                timeTicker.TimerFuncData = UpdateFunc;
                timeTicker.Start();
                int num = timeTicker.id = ++sTickerID;
                sTickerMap[num] = timeTicker;
                return num;
            }
            throw new Exception("this function couldn't be setInterval twice!");
        }

        public static void ClearTimeout(int id)
        {
            CTicker ticker = default(CTicker);
            if (sTickerMap.TryGetValue(id, out ticker))
            {
                int num = funcList.IndexOf(ticker);
                if (num > -1)
                {
                    funcList.RemoveAt(num);
                }
                sTickerMap.Remove(id);
                ticker.Dispose();
                ticker = null;
            }
        }

        public static void ClearInterval(int id)
        {
            ClearTimeout(id);
        }
    }
}
