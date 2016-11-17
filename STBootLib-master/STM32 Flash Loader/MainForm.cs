using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Threading;
using System.Media;
using STBootLib;

namespace STUploader
{
    public partial class fMainForm : Form
    {
        string fileName;
        string portName;
        uint baudRate;
        uint address;
        uint page;

        /* 默认地址 */
        const uint baseAddress = 0x08000000;

        public fMainForm()
        {
            /* 初始化 */
            InitializeComponent();

            /* 设置默认的波特率 */
            cbBauds.SelectedIndex = 6;
            cbPSize.SelectedIndex = 0;
            /* 设置默认的地址地址 */
            address = baseAddress;
        }



        /******************************************************************************
         * 单击下拉列表事件服务函数
         ******************************************************************************/
        private void cbPorts_DropDown(object sender, EventArgs e)
        {
            /*  */
            cbPorts.DataSource = SerialPort.GetPortNames();
        }



        /******************************************************************************
        * 端口发生改变服务函数
        ******************************************************************************/
        private void cbPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            /* 选择是否正确 */
            if (cbPorts.SelectedIndex == -1) {
                /* 禁用下载按钮 */
                bWrite.Enabled = bJump.Enabled = false;
            } else {
                /* 赋值端口号 */
                portName = (string)cbPorts.SelectedItem;
                /* 使能下载按钮 */
                bWrite.Enabled = bJump.Enabled = true;
            }
        }




        /******************************************************************************
        * 单击打开文件按钮事件服务函数
        ******************************************************************************/
        private void bOpenFile_Click(object sender, EventArgs e)
        {
            /* 设置支持的数据类型 */
            ofdOpen.Title = "选择数据文件";
            ofdOpen.Filter = "(*.BIN;*.bin;*.HEX;*.hex)|*.BIN;*.bin;*.HEX;*.hex";
            /* 是否选择完成 */
            if (ofdOpen.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
                /* 文件名赋值 */
                tbFileName.Text = ofdOpen.SafeFileName;
                /* 工具栏提示 */
                ttToolTip.SetToolTip(tbFileName, ofdOpen.FileName);
                /* 赋值文件名 */
                fileName = ofdOpen.FileName;
            }
        }




        /******************************************************************************
        * 单击跳转按钮事件服务函数
        ******************************************************************************/
        private async void bJump_Click(object sender, EventArgs e)
        {
            /* 禁用按钮，防止再次点击 */
            bJump.Enabled = bWrite.Enabled = false;
            /* 获得端口号 */
            string pName = (string)cbPorts.SelectedItem;
            /* 获得波特率 */
            uint bauds = uint.Parse((string)cbBauds.SelectedItem);
            /* 获得stm32起始地址 */
            uint address = Convert.ToUInt32(tbAddress.Text, 16);

            try {
                /* 跳转指令 */
                await Jump(address);
            } catch (Exception ex) {
                /*  */
                UpdateStatus(true, ex.Message);
            } finally {
                bJump.Enabled = bWrite.Enabled = true;
            }
        }



        /******************************************************************************
        * 单击下载按钮事件服务函数
        ******************************************************************************/
        private async void bWrite_Click(object sender, EventArgs e)
        {
            /* 禁用按钮，防止再次点击 */
            bJump.Enabled = bWrite.Enabled = false;
            /* 获得端口号 */
            string pName = (string)cbPorts.SelectedItem;
            /* 获得波特率 */
            uint bauds = uint.Parse((string)cbBauds.SelectedItem);
            /* 获得stm32起始地址 */
            uint address = Convert.ToUInt32(tbAddress.Text, 16);

            try {
                /* 读文件 */
                var bin = await ReadFile(fileName);
                /*  */
                await UploadFile(pName, bauds, bin, address, address);
            } catch (Exception ex) {
                /*  */
                UpdateStatus(true, ex.Message);
            } finally {
                bJump.Enabled = bWrite.Enabled = true;
            }
        }



        /******************************************************************************
        * 波特率改变事件服务函数
        ******************************************************************************/
        private void cbBauds_SelectedIndexChanged(object sender, EventArgs e)
        {
            /* string 转 uint */
            baudRate = uint.Parse((string)cbBauds.SelectedItem);
        }



        /******************************************************************************
        * 解析地址
        ******************************************************************************/
        private void tbAddress_Leave(object sender, EventArgs e)
        {
            /*  */
            uint addr;

            /*  */
            try {
                /* 地址转换成uint32数据类型 */
                addr = Convert.ToUInt32(tbAddress.Text, 16);
            /* 地址错误 */
            } catch (OverflowException) {
                /*  */
                tsslStatus.Text = "Address too large!";
                /* 设置为初始值 */
                addr = baseAddress;
            /* 其他异常 */
            } catch (Exception) {
                /* 输入的值类型不合法 */
                tsslStatus.Text = "Incorrect hex value";
                /* 设置为初始值 */
                addr = baseAddress;
            }

            /* 去掉最后8位 */
            address = addr & 0xffffff00;
            /* 计算页 */
            page = (address - baseAddress) / 256;

            /* 重新赋值 */
            tbAddress.Text = string.Format("0x{0:X8}", address);
        }

       /******************************************************************************
       * 解析文件
       ******************************************************************************/
        private async Task<byte[]> ReadFile(string fname)
        {
            byte[] bin;

            /* 打开文件 */
            using (var s = new FileStream(fname, FileMode.Open,
                    FileAccess.Read)) {
                /* 申请内存 */
                bin = new byte[s.Length];
                /* 读文件内容 */
                await s.ReadAsync(bin, 0, bin.Length);
            }

            /* 返回二进制镜像文件 */
            return bin;
        }

       /******************************************************************************
       * 向下位机烧写固件
       ******************************************************************************/
        private async Task UploadFile(string portName, uint baudRate,
            byte[] bin, uint address, uint jumpAddress)
        {
            /* 获得页的大小 */
            uint psize = uint.Parse(cbPSize.SelectedItem as string);

            /* 初始化ST boot对象 */
            using (var uc = new STBoot()) {
                /* 打开设备 */
                uc.Open(portName, baudRate);
                /* 通信初始化 */
                await uc.Initialize();
                /* 更新状态栏信息 */
                UpdateStatus(false, string.Format("Connected: Ver: {0}, PID: 0x{1:X4}",
                    uc.Version, uc.ProductID));
                /* 延时一会儿，使状态的信息可以被看见 */
                await Task.Delay(500);

                /* 擦除信息 */
                UpdateStatus(false, "Erasing...");

                /* 全局擦除是否被勾选 */
                if (cbxErase.Checked) {
                    await uc.GlobalErase();
                } else {
                    /* 擦除操作 */
                    for (uint i = 0; i < bin.Length; i += psize) {
                        /* 擦除页 */
                        await uc.ErasePage((i + address - 0x08000000) / psize);
                        /* 更新精度条 */
                        UpdateProgress((int)i * 100 / bin.Length);
                    }
                }

                /* 状态栏显示 编程 提示信息 */
                UpdateStatus(false, "Programming...");
                /* 创建进度报告对象 */
                var p = new Progress<STBootProgress>(UpdateProgress);
                /* 写Flash */
                await uc.WriteMemory(address, bin, 0, bin.Length, p,
                    CancellationToken.None);
                /* 烧写成功 */
                UpdateStatus(false, string.Format("Success: {0} bytes written",
                    bin.Length));

                /* 跳转到用户程序地址! */
                await uc.Jump(jumpAddress);

                /* 结束通信 */
                uc.Close();
            }
        }

       /******************************************************************************
       * 跳转至用户代码
       ******************************************************************************/
        private async Task Jump(uint address)
        {
            /* 初始化ST boot对象 */
            using (var uc = new STBoot()) {
                /* 打开设备 */
                uc.Open(portName, baudRate);
                /* 通信初始化 */
                await uc.Initialize();
                /* 跳转! */
                await uc.Jump(address);
                /* 结束通信 */
                uc.Close();
            }
        }

       /******************************************************************************
       * 更新进度条
       ******************************************************************************/
        private void UpdateProgress(STBootProgress p)
        {
            /* 转换成百分制 */
            UpdateProgress(100 * p.bytesProcessed / p.bytesTotal);
        }

        /******************************************************************************
        * 更新进度条
        ******************************************************************************/
        private void UpdateProgress(int percent)
        {
            /* 设置进度条值 */
            pbProgress.Value = percent;
            /* 设置值 */
            lProgress.Text = percent.ToString() + "%";
        }

       /******************************************************************************
       * 更新状态栏
       ******************************************************************************/
        private void UpdateStatus(bool ding, string text)
        {
            /* 文字 */
            tsslStatus.Text = text;
            /* 发送系统声音？ */
            if (ding) {
                /* ^^ ding! */
                SystemSounds.Exclamation.Play();
            }
        }

        private void cbxErase_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void cbPSize_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void statusStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }
    }
}
