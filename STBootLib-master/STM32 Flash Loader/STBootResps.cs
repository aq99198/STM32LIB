using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace STBootLib
{
    /* response codes*/
    public enum STResps
    {
        /* 命令正常 应答 */
        ACK = 0x79,
        /* 命令异常 应答 */
        NACK = 0x1F,
    }
}
