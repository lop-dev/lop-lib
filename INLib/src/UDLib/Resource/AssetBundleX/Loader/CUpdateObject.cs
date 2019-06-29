namespace UDLib.Resource
{
    // 更新的文件常态
    public enum CFileState
    {
        Normal,
        Add,
        Update,
        Delete
    }

    public struct CUpdateObject
    {
        public string name;
        public string HashOrMD5;
        public string version;
        public long size;
        public CFileState fileState;
    }
}