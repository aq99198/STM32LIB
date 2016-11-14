using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace STBootLib
{
    public class STBootProgress
    {
        /* 总共的bytes */
        public readonly int bytesTotal;
        /*  已处理的bytes */
        public readonly int bytesProcessed;

        public STBootProgress(int bytesProcessed, int bytesTotal)
        {
            /* set  */
            this.bytesProcessed = bytesProcessed;
            /* set  */
            this.bytesTotal = bytesTotal;
        }
    }
}
