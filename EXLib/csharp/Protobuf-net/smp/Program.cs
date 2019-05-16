using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;

using Message;

namespace PBNTest
{
    class Program
    {
        static void Main(string[] args)
        {
            CPerson outPerson = new CPerson();
            CPerson inPerson = null;

            outPerson.ID = 1;
            outPerson.Name = "First";
            outPerson.Email = "Address";

            CResult tmpResult = null;
            tmpResult = new CResult();
            tmpResult.Title = "Title01";
            outPerson.Result.Add(tmpResult);
            tmpResult = new CResult();
            tmpResult.Title = "Title02";
            outPerson.Result.Add(tmpResult);

            if(false)
            {
                // ProtoBuf序列化
                using (var outFile = System.IO.File.Create("Person.bin"))
                {
                    ProtoBuf.Serializer.Serialize<CPerson>(outFile, outPerson);
                }

                // ProtoBuf反序列化
                using (var inFile = System.IO.File.OpenRead("Person.bin"))
                {
                    inPerson = ProtoBuf.Serializer.Deserialize<CPerson>(inFile);
                }
            }
            else
            {
                MemoryStream outStream = new MemoryStream();
                MemoryStream inStream = new MemoryStream();

                ProtoBuf.Serializer.Serialize<CPerson>(outStream, outPerson);
                outStream.Seek(0,SeekOrigin.Begin);

                inStream.Write(outStream.ToArray(), 0, outStream.ToArray().Length);
                inStream.Seek(0, SeekOrigin.Begin);
                inPerson = ProtoBuf.Serializer.Deserialize<CPerson>(inStream);
            }

            System.Console.WriteLine(inPerson.Name);
            for(int i=0;i<inPerson.Result.Count;i++)
            {
                System.Console.WriteLine(inPerson.Result[i].Title);
            } 

            Thread.Sleep(3000);
        }
    }
}
