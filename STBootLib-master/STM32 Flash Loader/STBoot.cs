using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO.Ports;

namespace STBootLib
{
    public class STBoot : IDisposable
    {
        /* 串口对象 */
        SerialPort sp;
        /* 申明一个互斥量 */
        SemaphoreSlim sem;
        /* 所支持的命名 */
        List<STCmds> Commands;


        /* bootloader 的版本 */
        public string Version;
        /* 芯片ID */
        public ushort ProductID;
        

        /* 构造函数 */
        public STBoot()
        {
            Commands = new List<STCmds>();
            /* initialize mutex */
            sem = new SemaphoreSlim(1);
        }

        /* 析构函数 用于释放串口 */
        ~STBoot()
        {
            Dispose();
        }

        public void Dispose()
        {
            /* 关闭串口 */
            Close();
        }

       /******************************************************************************
        * 打开串口函数
        ******************************************************************************/
        public void Open(string portName, uint baudRate)
        {
            /* 初始化串口 */
            sp = new SerialPort(portName, (int)baudRate, Parity.Even, 8);
            /* 打开串口 */
            sp.Open();

            /* 清除串行驱动程序接收和发送缓冲区的数据 */
            sp.DiscardInBuffer();
            sp.DiscardOutBuffer();
        }

        /******************************************************************************
        * 关闭串口函数
        ******************************************************************************/
        public void Close()
        {
            /* 判断是允许关闭 */
            if (sp != null && sp.IsOpen)
                sp.Close();
        }

        /******************************************************************************
        * 初始化 任务
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        public async Task Initialize()
        {
            /* 发送一个7F给STM32，STM32根据这个字符自动设置自身的波特率 */
            await Init();
            /* 发送GET（00 FF）命令给STM32，获得STM32内部bootloader的版本和所支持的命令 */
            await Get();

            /* 判断是否支持GET_ID命令 */
            if (!Commands.Contains(STCmds.GET_ID)) {
                /* 如果不支持则抛出异常 */
                throw new STBootException("Command not supported");
            }

            /* 发送GET_ID 指令获得芯片ID信息 */
            await GetID();
        }

        /******************************************************************************
        * 解锁 内部的Flash
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        public async Task Unprotect()
        {
            /* 判断是否支持解锁Flash指令 */
            if (!Commands.Contains(STCmds.WR_UNPROTECT))
                throw new STBootException("Command not supported");

            /* no support for unprotect? */
            if (!Commands.Contains(STCmds.RD_UNPROTECT))
                throw new STBootException("Command not supported");

            await ReadUnprotect();
            await WriteUnprotect();
        }

        /******************************************************************************
        * 读芯片的Flash
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        public async Task ReadMemory(uint address, byte[] buf, int offset, 
            int size, IProgress<int> p, CancellationToken ct)
        {
            
            int bread = 0;
            /* 判断是否支持读Flash指令 */
            if (!Commands.Contains(STCmds.READ))
                throw new STBootException("Command not supported");

            /* 判断设置的块的大小是否合理 */
            while (size > 0 && !ct.IsCancellationRequested) {
              
                int csize = Math.Min(size, 256);
                /* 读某一块 */
                await Read(address, buf, offset, csize);
         
                /* 更新偏移量 */
                size -= csize; offset += csize; address += (uint)csize; 
               
                bread += csize;

                /* 报告进展 */
                if (p != null)
                    p.Report(bread);
            }

            /* 当读被取消了，抛出异常 */
            if (ct.IsCancellationRequested)
                throw new OperationCanceledException("Read cancelled");
        }

        /******************************************************************************
       * 写芯片的Flash
       * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
       * 
       ******************************************************************************/
        public async Task WriteMemory(uint address, byte[] buf, int offset,
            int size, IProgress<STBootProgress> p, CancellationToken ct)
        {
           
            int bwritten = 0, btotal = size;

            /* 判断是否支持写指令 */
            if (!Commands.Contains(STCmds.WRITE))
                throw new STBootException("Command not supported");

            /* 当块中有数据 */
            while (size > 0 && !ct.IsCancellationRequested) {
               
                int csize = Math.Min(size, 256);
                /* 写一个块 */
                await Write(address, buf, offset, csize);

                /* 更新状态 */
                size -= csize; offset += csize; address += (uint)csize;
                
                bwritten += csize;

                /* 报告进展 */
                if (p != null)
                    p.Report(new STBootProgress(bwritten, btotal));
            }

            
            if (ct.IsCancellationRequested)
                throw new OperationCanceledException("Write cancelled");
        }

        /******************************************************************************
        * 擦除芯片的Flash
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        public async Task ErasePage(uint pageNumber)
        {
            /* 判断是否支持擦除命令 */
            if (Commands.Contains(STCmds.ERASE)) {
                await Erase(pageNumber);
             /* 判断是否支持擦除命令 */
            } else if (Commands.Contains(STCmds.EXT_ERASE)) {
                await ExtendedErase(pageNumber);
            /* 如果不支持，则抛出一个异常 */
            } else {
                throw new STBootException("Command not supported");
            }
        }

        /******************************************************************************
        * 擦除全部
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        public async Task GlobalErase()
        {
            
            if (Commands.Contains(STCmds.ERASE)) {
                await EraseSpecial(STEraseMode.GLOBAL);
               
            } else if (Commands.Contains(STCmds.EXT_ERASE)) {
                await ExtendedEraseSpecial(STExtendedEraseMode.GLOBAL);
                
            } else {
                throw new STBootException("Command not supported");
            }
        }

        /******************************************************************************
        * 跳转到制定地址
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        public async Task Jump(uint address)
        {
            /* 判断是否支持GO 指令*/
            if (!Commands.Contains(STCmds.GO))
                throw new STBootException("Command not supported");

            /* 去吧!!! */
            await Go(address);
        }

       /******************************************************************************
       * 初始化
       * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
       * 
       ******************************************************************************/
        private async Task Init()
        {
            /* 发送 */
            var tx = new byte[1];
            /* 回复 */
            var ack = new byte[1];

            tx[0] = (byte)STCmds.INIT;

            /* 等待指令发送线程，将数据发送完毕 */
            await sem.WaitAsync();

            /* 发送一条指令，并等待应答 */
            try {
                /* 发送命令 */
                await SerialWrite(tx, 0, tx.Length);

                /* 等待数据读取 */
                await SerialRead(ack, 0, 1);
                /* 判断回复的命令 */
                if (ack[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");
            /* 当发送异常 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                
                throw;
            }

            /* 正常释放信号量 */
            sem.Release();
        }

        /******************************************************************************
        * get 指令
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        private async Task Get()
        {
            /* 发送数组 */
            var tx = new byte[2];
            /* 接收缓存数组 */
            var tmp = new byte[1];
            /* 回复字节的个数 */
            int nbytes;
            /* 接收的数组 */
            byte[] rx;
           
            /* 赋值 */
            tx[0] = (byte)STCmds.GET;
            /* 做异或校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 等待发送线程将上一个命令发送完成
             * command */
            await sem.WaitAsync();

            /* 发送一个指令，并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, tx.Length);

                /* 接受一个1字符 */
                await SerialRead(tmp, 0, 1);
                /* 判断应答信息第一个值是否为ACK 79 */
                if (tmp[0] != (byte)STResps.ACK) 
                    throw new STBootException("Command Rejected");

                /* 接受第二个字符 */
                await SerialRead(tmp, 0, 1);
                /*  */
                nbytes = tmp[0] + 2;
                /* 在STM32中 nbytes 必定为13（协议规定） */
                if (nbytes != 13)
                    throw new STBootException("Invalid length");

                /* 创建一个新的接受数组 */
                rx = new byte[nbytes];
                /* 接受剩余的字符 */
                await SerialRead(rx, 0, rx.Length);
            /* 啊噢，出异常了 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                
                throw;
            }

            /* 获得bootloader的版本 */
            Version = (rx[0] >> 4).ToString() + "." + 
                (rx[0] & 0xf).ToString();

            /* 初始化命令表，用于以后判断 */
            Commands = new List<STCmds>();
            /* 添加所有的指令 */
            for (int i = 1; i < nbytes - 1; i++)
                Commands.Add((STCmds)rx[i]);

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
        * get_id 指令
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        private async Task GetID()
        {
            /* 发送数组 */
            var tx = new byte[2];
            /* 接收缓存数组 */
            var tmp = new byte[1];
            /* 回复字节的个数 */
            int nbytes;
            /* 接受数组 */
            byte[] rx;

            /* 赋值 */
            tx[0] = (byte)STCmds.GET_ID;
            /* XOR 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 发送命令，并等待应答 */
            try {
                /* 发送命令 */
                await SerialWrite(tx, 0, tx.Length);

                /* 等待应答信号 */
                await SerialRead(tmp, 0, 1);
                /* 检查ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 等待长度字节 */
                await SerialRead(tmp, 0, 1);
                /* 计算数据长度*/
                nbytes = tmp[0] + 2;
                /* nbytes 必定等于3 (协议规定) */
                if (nbytes != 3)
                    throw new STBootException("Invalid length");

                /* 初始化一个新的接受数组*/
                rx = new byte[nbytes];
                /* 等待接受字符 */
                await SerialRead(rx, 0, rx.Length);
            /* 出错了 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                /* re-throw */
                throw;
            }

            /* 获得产品的ID */
            ProductID = (ushort)(rx[0] << 8 | rx[1]);

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
        * 读 指令
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        private async Task Read(uint address, byte[] buf, int offset, int length)
        {
            /* 发送数组 */
            var tx = new byte[9];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.READ;
            /* xor校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 地址移位 */
            tx[2] = (byte)((address >> 24) & 0xff);
            tx[3] = (byte)((address >> 16) & 0xff);
            tx[4] = (byte)((address >> 8) & 0xff);
            tx[5] = (byte)(address & 0xff);
            /* 为什么要地址校验？因为这是ST! */
            tx[6] = (byte)~ComputeChecksum(tx, 2, 4);

            /* 长度字节 */
            tx[7] = (byte)(length - 1);
            /* 校验 */
            tx[8] = ComputeChecksum(tx, 7, 1);

            /* 发送一个指令并等待应答 */
            try {
                /* 发送*/
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否应答了ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送地址 */
                await SerialWrite(tx, 2, 5);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否应答了ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Address Rejected");

                /* 发送长度和校验 */
                await SerialWrite(tx, 7, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否应答了ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Size Rejected");

                /* 接受数组 */
                await SerialRead(buf, offset, length);
                /* 出错 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
               
                throw;
            }

            /* 是否信号量 */
            sem.Release();
        }

        /******************************************************************************
        * GO 指令
        * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
        * 
        ******************************************************************************/
        private async Task Go(uint address)
        {
            /* 发送数组 */
            var tx = new byte[7];
            /* 等待回应 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.GO;
            /* 校验*/
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 地址拆分 */
            tx[2] = (byte)((address >> 24) & 0xff);
            tx[3] = (byte)((address >> 16) & 0xff);
            tx[4] = (byte)((address >> 8) & 0xff);
            tx[5] = (byte)(address & 0xff);
            /* 地址校验 */
            tx[6] = (byte)~ComputeChecksum(tx, 2, 4);

            /* 发送命令并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否应答了ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送地址 */
                await SerialWrite(tx, 2, 5);
                /* 等待应答码 */
                await SerialRead(tmp, 0, 1);
                /* 等待应答 */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Address Rejected");
                /* 出错 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                /*  */
                throw;
            }

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
         * Write 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task Write(uint address, byte[] data, int offset, int length)
        {
            /* 发送数组 */
            var tx = new byte[9];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.WRITE;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 地址拆分 */
            tx[2] = (byte)((address >> 24) & 0xff);
            tx[3] = (byte)((address >> 16) & 0xff);
            tx[4] = (byte)((address >> 8) & 0xff);
            tx[5] = (byte)(address & 0xff);
            /* 校验 */
            tx[6] = (byte)~ComputeChecksum(tx, 2, 4);

            /* 长度字节 */
            tx[7] = (byte)(length - 1);
            /* 校验 */
            tx[8] = (byte)(~(ComputeChecksum(data, offset, length) ^ tx[7]));

            /* 发送指令并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送地址 */
                await SerialWrite(tx, 2, 5);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Address Rejected");

                /* 发用长度字节 */
                await SerialWrite(tx, 7, 1);
                /* 发送数据 */
                await SerialWrite(data, offset, length);
                /* 写校验 */
                await SerialWrite(tx, 8, 1);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Data Rejected");

                /* 出错 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                /*  */
                throw;
            }

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
         * 擦除 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task Erase(uint pageNumber)
        {
            /* 发送数组 */
            var tx = new byte[5];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.ERASE;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 擦除单页 */
            tx[2] = 0;
            /* 设置页序 */
            tx[3] = (byte)pageNumber;
            /* 校验 */
            tx[4] = (byte)~ComputeChecksum(tx, 2, 2);

            /* 发送并等待应答 */
            try {
                /* 发送命令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送地址 */
                await SerialWrite(tx, 2, 3);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Page Rejected");

                /* 出错 */
            } catch (Exception) {
                /* 是否信号量 */
                sem.Release();
                /*  */
                throw;
            }

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
         * 特定擦除 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task EraseSpecial(STEraseMode mode)
        {
            /* 发送数组 */
            var tx = new byte[4];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.ERASE;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 擦除单页 */
            tx[2] = (byte)((int)mode);
            /* 校验 */
            tx[3] = (byte)~ComputeChecksum(tx, 2, 2);

            /* 发送指令并等待回复 */
            try {
                /* 发送命令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送地址 */
                await SerialWrite(tx, 2, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Special Code Rejected");

                /* 发生错误 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                /* re-throw */
                throw;
            }

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
         * 擦除制定页 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task ExtendedErase(uint pageNumber)
        {
            /* 发送数组 */
            var tx = new byte[7];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.EXT_ERASE;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 檫除单页 */
            tx[2] = 0;
            tx[3] = 0;
            /* 设置页序 */
            tx[4] = (byte)(pageNumber >> 8);
            tx[5] = (byte)(pageNumber >> 0);
            /* 校验 */
            tx[6] = (byte)~ComputeChecksum(tx, 2, 5);

            /* 发送指令并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送页序 */
                await SerialWrite(tx, 2, 5);
                /* 等待应答，擦除比较耗时，将超时时间设为3000ms */
                await SerialRead(tmp, 0, 1, 3000);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK) 
                    throw new STBootException("Page Rejected");

            /* 出错 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                /*  */
                throw;
            }

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
         * 擦除制定页特殊模式 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task ExtendedEraseSpecial(STExtendedEraseMode mode)
        {
            /* 发送数组 */
            var tx = new byte[5];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.EXT_ERASE;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 拆分 */
            tx[2] = (byte)((int)mode >> 8);
            tx[3] = (byte)((int)mode >> 0);
            /* 校验 */
            tx[4] = (byte)~ComputeChecksum(tx, 2, 3);

            /* 发送命令并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 发送模式字节 */
                await SerialWrite(tx, 2, 3);
                /* 等待，设置超时时间为1000ms */
                await SerialRead(tmp, 0, 1, 10000);
                /* 是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Special code Rejected");

                /* 出错 */
            } catch (Exception) {
                /* 释放信号量 */
                sem.Release();
                /*  */
                throw;
            }

            /* 释放信号量 */
            sem.Release();
        }

        /******************************************************************************
         * 写FLASH保护 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task WriteUnprotect()
        {
            /* 发送数组 */
            var tx = new byte[2];
            /* 接收缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.WR_UNPROTECT;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 发送并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 应答是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 等待应答 */
                await SerialRead(tmp, 0, 1);
                /* 判断应答是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Write Unprotect Rejected");

                /* 出错 */
            } finally {
                /* 释放信号量 */
                sem.Release();
            }
        }

        /******************************************************************************
         * 解锁FLASH 指令
         * async 是.NET 4.5之后才用的关键字，用于方便地处理异步操作
         * 
         ******************************************************************************/
        private async Task ReadUnprotect()
        {
            /* 发送数组 */
            var tx = new byte[2];
            /* 发送缓冲数组 */
            var tmp = new byte[1];

            /* 赋值 */
            tx[0] = (byte)STCmds.RD_UNPROTECT;
            /* 校验 */
            tx[1] = ComputeChecksum(tx, 0, 1);

            /* 发送指令并等待应答 */
            try {
                /* 发送指令 */
                await SerialWrite(tx, 0, 2);
                /* 等待应答码 */
                await SerialRead(tmp, 0, 1);
                /* 判断是否为ACK */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Command Rejected");

                /* 等待应答，设置超时时间为10000ms */
                await SerialRead(tmp, 0, 10000);
                /* 等待应答 */
                if (tmp[0] != (byte)STResps.ACK)
                    throw new STBootException("Write Unprotect Rejected");

                /* 出错 */
            } finally {
                /* 释放信号量 */
                sem.Release();
            }
        }

        /******************************************************************************
         * 计算校验
         * 使用了XOR 异或校验
         ******************************************************************************/
        private byte ComputeChecksum(byte[] data, int offset, int count)
        {
            /* 初始化 */
            byte xor = 0xff;
            /* 计算 */
            for (int i = offset; i < count + offset; i++)
                xor ^= data[i];

            /* 返回结果 */
            return xor;
        }

        /******************************************************************************
         * 串口写数据
         * 
         ******************************************************************************/
        private async Task SerialWrite(byte[] data, int offset, int count)
        {
            /* 流 */
            var bs = sp.BaseStream;

            /* 写操作 */
            await bs.WriteAsync(data, offset, count);
        }

       /******************************************************************************
        * 标准串口读数据
        * 默认设置1s超时时间
        ******************************************************************************/
        private async Task SerialRead(byte[] data, int offset, int count)
        {
            await SerialRead(data, offset, count, 1000);
        }

       /******************************************************************************
       * 读制定长度的值
       * 
       ******************************************************************************/
        private async Task SerialRead(byte[] data, int offset, int count, int timeout)
        {
            /* 流 */
            var bs = sp.BaseStream;
            /* 读到数据的长度 */
            int br = 0;

            /* 从串口读取,直到所有字节都被取完 */
            while (br < count) {
                /* 这里使用try 是为了超时处理 */
                try {
                    /* 读数据 */
                    br += await bs.ReadAsync(data, offset + br, count - br).
                        WithTimeout(timeout);
                /* 是否超时 */
                } catch (OperationCanceledException) {
                    /* 抛出超时异常 */
                    throw new STBootException("Timeout");
                }
            }
        }
    }
}
