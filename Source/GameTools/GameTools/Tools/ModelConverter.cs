using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace GameTools.Tools
{
    class ModelConverter
    {
        //native imports--------------------------------------
        [DllImport("ModelFileConverter.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ConvertToAMOD(string input, string output);
        //----------------------------------------------------
        public static bool ConvertFile(String input, String output)
        {
            return ConvertToAMOD(input, output);
        }
    }
}
