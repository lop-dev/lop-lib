using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;


namespace Server
{
class Program
{
    static void Main(string[] args)
    {
        string strEncode = CSLib.Security.CMd5.EncodeString("aaa");
    }
}
}
