using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RefreshSHLibPrj
{
    class SHLibInfo
    {
        public List<FileInfo> hFiles = new List<FileInfo>();
        public List<FileInfo> protoFiles = new List<FileInfo>();
        public List<FileInfo> cFiles = new List<FileInfo>();
        public List<string> lines = new List<string>();
        public List<string> newLines = new List<string>();

        //public List<string> fliterLines = new List<string>();
        public List<string> fliterLines = new List<string>();

        

    }
}
