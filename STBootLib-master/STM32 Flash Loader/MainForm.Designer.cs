namespace STUploader
{
    partial class fMainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(fMainForm));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cbBauds = new System.Windows.Forms.ComboBox();
            this.cbPorts = new System.Windows.Forms.ComboBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tsslStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.bJump = new System.Windows.Forms.Button();
            this.lProgress = new System.Windows.Forms.Label();
            this.bWrite = new System.Windows.Forms.Button();
            this.pbProgress = new System.Windows.Forms.ProgressBar();
            this.ofdOpen = new System.Windows.Forms.OpenFileDialog();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.cbPSize = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.cbxErase = new System.Windows.Forms.CheckBox();
            this.tbAddress = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.ttToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.bOpenFile = new System.Windows.Forms.Button();
            this.tbFileName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.cbBauds);
            this.groupBox1.Controls.Add(this.cbPorts);
            this.groupBox1.Location = new System.Drawing.Point(12, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(293, 46);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "串口";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(148, 19);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "波特率";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "串口号";
            // 
            // cbBauds
            // 
            this.cbBauds.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbBauds.FormattingEnabled = true;
            this.cbBauds.Items.AddRange(new object[] {
            "9600",
            "14400",
            "19200",
            "28800",
            "38400",
            "57600",
            "115200",
            "230400"});
            this.cbBauds.Location = new System.Drawing.Point(194, 17);
            this.cbBauds.Name = "cbBauds";
            this.cbBauds.Size = new System.Drawing.Size(88, 20);
            this.cbBauds.TabIndex = 1;
            this.ttToolTip.SetToolTip(this.cbBauds, "Baudrate used for communication");
            this.cbBauds.SelectedIndexChanged += new System.EventHandler(this.cbBauds_SelectedIndexChanged);
            // 
            // cbPorts
            // 
            this.cbPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbPorts.FormattingEnabled = true;
            this.cbPorts.Location = new System.Drawing.Point(63, 17);
            this.cbPorts.Name = "cbPorts";
            this.cbPorts.Size = new System.Drawing.Size(79, 20);
            this.cbPorts.TabIndex = 0;
            this.ttToolTip.SetToolTip(this.cbPorts, "COM Port Name");
            this.cbPorts.DropDown += new System.EventHandler(this.cbPorts_DropDown);
            this.cbPorts.SelectedIndexChanged += new System.EventHandler(this.cbPorts_SelectedIndexChanged);
            this.cbPorts.DropDownClosed += new System.EventHandler(this.cbPorts_SelectedIndexChanged);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsslStatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 284);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(315, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            this.statusStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.statusStrip1_ItemClicked);
            // 
            // tsslStatus
            // 
            this.tsslStatus.Name = "tsslStatus";
            this.tsslStatus.Size = new System.Drawing.Size(20, 17);
            this.tsslStatus.Text = "   ";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.bJump);
            this.groupBox3.Controls.Add(this.lProgress);
            this.groupBox3.Controls.Add(this.bWrite);
            this.groupBox3.Controls.Add(this.pbProgress);
            this.groupBox3.Location = new System.Drawing.Point(12, 207);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(293, 74);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "进度";
            // 
            // bJump
            // 
            this.bJump.Enabled = false;
            this.bJump.Location = new System.Drawing.Point(149, 44);
            this.bJump.Name = "bJump";
            this.bJump.Size = new System.Drawing.Size(133, 21);
            this.bJump.TabIndex = 3;
            this.bJump.Text = "跳！";
            this.ttToolTip.SetToolTip(this.bJump, "Uploads the firmware and jumps to it.");
            this.bJump.UseVisualStyleBackColor = true;
            this.bJump.Click += new System.EventHandler(this.bJump_Click);
            // 
            // lProgress
            // 
            this.lProgress.AutoSize = true;
            this.lProgress.Location = new System.Drawing.Point(257, 23);
            this.lProgress.Name = "lProgress";
            this.lProgress.Size = new System.Drawing.Size(17, 12);
            this.lProgress.TabIndex = 2;
            this.lProgress.Text = "0%";
            // 
            // bWrite
            // 
            this.bWrite.Enabled = false;
            this.bWrite.Location = new System.Drawing.Point(9, 44);
            this.bWrite.Name = "bWrite";
            this.bWrite.Size = new System.Drawing.Size(133, 21);
            this.bWrite.TabIndex = 0;
            this.bWrite.Text = "烧写并跳转";
            this.ttToolTip.SetToolTip(this.bWrite, "Uploads the firmware and jumps to it.");
            this.bWrite.UseVisualStyleBackColor = true;
            this.bWrite.Click += new System.EventHandler(this.bWrite_Click);
            // 
            // pbProgress
            // 
            this.pbProgress.Location = new System.Drawing.Point(9, 18);
            this.pbProgress.Name = "pbProgress";
            this.pbProgress.Size = new System.Drawing.Size(242, 21);
            this.pbProgress.TabIndex = 0;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.cbPSize);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.cbxErase);
            this.groupBox4.Controls.Add(this.tbAddress);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Location = new System.Drawing.Point(12, 118);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(293, 83);
            this.groupBox4.TabIndex = 2;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "操作";
            // 
            // cbPSize
            // 
            this.cbPSize.FormattingEnabled = true;
            this.cbPSize.Items.AddRange(new object[] {
            "256",
            "128",
            "2048"});
            this.cbPSize.Location = new System.Drawing.Point(66, 49);
            this.cbPSize.Name = "cbPSize";
            this.cbPSize.Size = new System.Drawing.Size(112, 20);
            this.cbPSize.TabIndex = 5;
            this.ttToolTip.SetToolTip(this.cbPSize, "COM Port Name");
            this.cbPSize.SelectedIndexChanged += new System.EventHandler(this.cbPSize_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(11, 52);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 2;
            this.label5.Text = "页大小：";
            this.ttToolTip.SetToolTip(this.label5, "Destination address");
            // 
            // cbxErase
            // 
            this.cbxErase.AutoSize = true;
            this.cbxErase.Location = new System.Drawing.Point(194, 20);
            this.cbxErase.Name = "cbxErase";
            this.cbxErase.Size = new System.Drawing.Size(72, 16);
            this.cbxErase.TabIndex = 1;
            this.cbxErase.Text = "擦除芯片";
            this.ttToolTip.SetToolTip(this.cbxErase, "Writing to flash memory?");
            this.cbxErase.UseVisualStyleBackColor = true;
            this.cbxErase.CheckedChanged += new System.EventHandler(this.cbxErase_CheckedChanged);
            // 
            // tbAddress
            // 
            this.tbAddress.Location = new System.Drawing.Point(66, 18);
            this.tbAddress.Name = "tbAddress";
            this.tbAddress.Size = new System.Drawing.Size(112, 21);
            this.tbAddress.TabIndex = 0;
            this.tbAddress.Text = "0x08000000";
            this.tbAddress.Leave += new System.EventHandler(this.tbAddress_Leave);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 21);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 12);
            this.label4.TabIndex = 0;
            this.label4.Text = "起始地址：";
            this.ttToolTip.SetToolTip(this.label4, "Destination address");
            // 
            // bOpenFile
            // 
            this.bOpenFile.Location = new System.Drawing.Point(194, 15);
            this.bOpenFile.Name = "bOpenFile";
            this.bOpenFile.Size = new System.Drawing.Size(88, 21);
            this.bOpenFile.TabIndex = 1;
            this.bOpenFile.Text = "打开文件";
            this.ttToolTip.SetToolTip(this.bOpenFile, "Open firmware file");
            this.bOpenFile.UseVisualStyleBackColor = true;
            this.bOpenFile.Click += new System.EventHandler(this.bOpenFile_Click);
            // 
            // tbFileName
            // 
            this.tbFileName.Enabled = false;
            this.tbFileName.Location = new System.Drawing.Point(63, 17);
            this.tbFileName.Name = "tbFileName";
            this.tbFileName.Size = new System.Drawing.Size(115, 21);
            this.tbFileName.TabIndex = 0;
            this.ttToolTip.SetToolTip(this.tbFileName, "File Name");
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "位置";
            this.ttToolTip.SetToolTip(this.label1, "File Name");
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tbFileName);
            this.groupBox2.Controls.Add(this.bOpenFile);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(12, 66);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(293, 46);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "文件";
            // 
            // fMainForm
            // 
            this.AcceptButton = this.bWrite;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(315, 306);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "fMainForm";
            this.Text = "stm32 ISP 下载 (jabez)";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox cbPorts;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel tsslStatus;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button bWrite;
        private System.Windows.Forms.ProgressBar pbProgress;
        private System.Windows.Forms.Label lProgress;
        private System.Windows.Forms.OpenFileDialog ofdOpen;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cbBauds;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.CheckBox cbxErase;
        private System.Windows.Forms.TextBox tbAddress;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ToolTip ttToolTip;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button bOpenFile;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tbFileName;
        private System.Windows.Forms.Button bJump;
        private System.Windows.Forms.ComboBox cbPSize;
        private System.Windows.Forms.Label label5;
        
    }
}

