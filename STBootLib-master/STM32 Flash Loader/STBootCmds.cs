using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace STBootLib
{
    /* command list */
    public enum STCmds
    {
        
        INIT = 0x7F,        //用于让STM32 的bootloader自适应上位机的波特率 

        GET = 0x00,         //获取当前自举程序版本以及支持的指令

        GET_PROT = 0x01,    //读取自举程序版本及Flash的读保护状态
    
        GET_ID = 0x02,      //获得芯片ID
   
        READ = 0x11,        //从应用程序制定的地址开始读取最多256个字节的存储空间

        GO = 0x21,          //跳转到内部FLASH或SRAM代码
     
        WRITE = 0x31,       //从用用程序指定的地址开始讲最多256个字节数据写入RAM或FLASH
        
        ERASE = 0x43,       //擦除一个到全部Flash页面
      
        EXT_ERASE = 0x44,   //使用双字节寻址模式擦除一个到全部Flash页面（仅用于V3.0 usart 自举程序版本及以上版本）

        WR_PROTECT = 0x63,  //使能某些扇区的写保护
    
        WR_UNPROTECT = 0x73,//禁止所有Flash扇区的写保护

        RD_PROTECT = 0x82,  //使能读保护

        RD_UNPROTECT = 0x92 //禁止读保护
    }

    /* 特殊的擦除模式 */
    public enum STEraseMode
    {
        /* 檫除所有 */
        GLOBAL = 0xff,
    }

    /* 特殊的擦除模式 */
    public enum STExtendedEraseMode
    {
        /* 擦除所有*/
        GLOBAL = 0xffff,
        /* 擦除 bank 1 */
        BANK1 = 0xfffe,
        /* 擦除 bank 2 */
        BANK2 = 0xfffd
    }
}
