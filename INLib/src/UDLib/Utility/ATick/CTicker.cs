namespace UDLib.Utility
{
    public class CTicker
    {
        public object data;
        public int id;
        public CSLib.Utility.Action TimerFuncEmpty;
        public CSLib.Utility.Action<object> TimerFuncData;
        public CSLib.Utility.Action<CTicker> ATickCompleteFunc;
        public CSLib.Utility.Action CompleteFuncEmpty;
        public CSLib.Utility.Action<object> CompleteFuncData;

        public void Start()
        {
            MbTickManager.Instance.AddTicker(this);
        }

        public void Stop()
        {
            MbTickManager.Instance.RemoveTicker(this);
        }

        public virtual void Reset()
        { }

        public virtual void Tick(float time)
        { }

        public void Dispose()
        {
            this.Stop();
            this.Reset();
            this.TimerFuncEmpty = null;
            this.TimerFuncData = null;
            this.ATickCompleteFunc = null;
        }
    }
}