using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;
using System.Runtime.Serialization.Formatters.Binary;

namespace FSM
{
class Program
{
    static void Main(string[] args)
    {
        //CFSMSample01 fsmSample01 = new CFSMSample01();
        //fsmSample01.update();

        CFSMSample02 fsmSample02 = new CFSMSample02();
        fsmSample02.update();
    }
}
}
