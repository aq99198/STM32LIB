using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace STBootLib
{
    public class STBootException : Exception
    {
       
        public STBootException()
        {
        }

      
        public STBootException(string message)
            : base(message)
        {
        }
    }
}
