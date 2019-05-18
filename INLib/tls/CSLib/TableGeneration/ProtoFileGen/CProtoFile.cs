using System;
using System.Collections.Generic;
using System.IO;

namespace Proto2Code
{
    public struct SMsgID
    {
        public string enumName;
        public string enumPTBuf;
        public string annotation;
    }

    public class CProtoFile
    {
        public List<SMsgID> MsgIDList
        {
            get
            {
                return m_MsgIDList;
            }
        }

        public string EFUNC
        {
            get
            {
                return m_EFUNC;
            }
        }

        public CNameUnit NameUnit
        {
            get
            {
                return m_nameUnit;
            }
            set
            {
                m_nameUnit = value;
            }
        }

        private string m_EFUNC;
        private static List<SMsgID> m_MsgIDList = new List<SMsgID>();
        private CNameUnit m_nameUnit;

        public bool LoadFile(string filePath, string file_name, string writePathRoot,bool isNeedCheck=true)
        {
            string EFUNC = string.Empty;
            CNameUnit nameUnit = new CNameUnit();
            nameUnit.ParseNames(file_name);

            NameUnit = nameUnit;

            bool messageHasStart = false;
            bool isCanOverWrite = false;

            string[] fileLines = File.ReadAllLines(filePath);
            foreach (var line in fileLines)
            {
                // 先必须找到 OverWrite
                if (isNeedCheck)
                {
                    if (!isCanOverWrite)
                    {
                        isCanOverWrite = line.Contains("OverWrite");
                        if (!isCanOverWrite)
                        {
                            continue;
                        }

                        if (line.Split('=')[1] == "true")
                        {
                           
                        }
                        else if (line.Split('=')[1] == "false")
                        {
                            //Console.WriteLine(string.Format("LOG: >> file:{0}.proto 不能被重写", file_name));
                            return false;
                        }
                        else
                        {
                            Console.WriteLine("检查OverWrite参数是否填写");
                            return false;
                        }
                    }
                    if (line.Contains("EFUNC"))
                    {
                        EFUNC = line.Split('/')[2];
                        m_EFUNC = EFUNC;
                    }
                }
           
                if (!messageHasStart)
                {
                    messageHasStart = line.Contains(string.Format("EMsgID{0}", CHelper.ToCaptilize(nameUnit.MoudleSysName)));
                    if (!messageHasStart)
                        continue;
                }
                else if (line.StartsWith("}"))
                {
                    break;
                }

                MsgType msgType = CHelper.FindMsgType(line);
                if (msgType == MsgType.None)
                    continue;
                string enumName = line.Split('=')[0];
                string PTBuf = string.Empty;
                string annotation = string.Empty;

                SMsgID unit;
                if (line.Contains("//"))        //判断是否存在备注，(在overwrite=false的proto文件中存在无备注即无//的行)
                {
                    PTBuf = line.Split('/')[2];     //PTBuf = //备注部分
                    if (line.Split('/').Length >= 4)    //如果存在两个备注（第一个为pb类型，第二个为描述）
                    {
                        annotation = line.Split('/')[4];    //annotation = 汉字描述
                    }
                    else
                    {
                        annotation = string.Empty;  //无第二个即无描述
                    }
                }
                else
                {
                    annotation = string.Empty;  //无备注即无描述
                }
                enumName = enumName.Trim();
                unit.enumPTBuf = PTBuf;
                unit.enumName = enumName;
                unit.annotation = annotation;
                m_MsgIDList.Add(unit);
            }            
            return true;
        }      
    }
}
