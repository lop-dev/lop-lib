namespace TSLib.ProtoGeneration
{
    public class CNameUnit
    {
        //mailSystem
        public string MoudleSysName { get; private set; }
        //MailSystemMessage
        public string MoudleSysMsg { get; private set; }
        //MAILSYSTEM
        public string MoudleSysUpper { get; private set; }
        //MailService
        public string ModuleService { get; private set; }
        //MailSystem
        public string ModuleSystem { get; private set; }
        //Mail
        public string Module { get; private set; }
        
        public void ParseNames(string fileName)
        {
            MoudleSysName = fileName;
            MoudleSysMsg = CHelper.ToCaptilize(fileName) + "Message";
            MoudleSysUpper = fileName.ToUpper();
            ModuleService = CHelper.CombineServiceName(fileName);
            ModuleSystem = CHelper.CombineSystemName(fileName);
            Module = ModuleService.Replace("Service", string.Empty);
        }
        
        public override string ToString()
        {
            return string.Format("protoFileName={0},XxxSystemMessage={1},XXXSYSTEM={2},XxxService={3},Xxx={4}",
                this.MoudleSysName, this.MoudleSysMsg, this.MoudleSysUpper, this.ModuleService, this.Module);
        }
    }
}