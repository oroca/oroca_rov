namespace OjwRov
{
    partial class Main
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.lbDisp = new System.Windows.Forms.Label();
            this.txtMessage = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.txtIp = new System.Windows.Forms.TextBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.lbJoystick = new System.Windows.Forms.Label();
            this.tmrJoystick = new System.Windows.Forms.Timer(this.components);
            this.lbProp = new System.Windows.Forms.Label();
            this.pnDisp = new System.Windows.Forms.Panel();
            this.btnTest = new System.Windows.Forms.Button();
            this.btnStartStreaming = new System.Windows.Forms.Button();
            this.rdStream_Jpeg = new System.Windows.Forms.RadioButton();
            this.rdStream_MJpeg = new System.Windows.Forms.RadioButton();
            this.btnStopStreaming = new System.Windows.Forms.Button();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtPort_Cam = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtTest0 = new System.Windows.Forms.TextBox();
            this.txtTest1 = new System.Windows.Forms.TextBox();
            this.txtTest2 = new System.Windows.Forms.TextBox();
            this.txtTest3 = new System.Windows.Forms.TextBox();
            this.txtTest4 = new System.Windows.Forms.TextBox();
            this.txtTest = new System.Windows.Forms.Button();
            this.pnDisp.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbDisp
            // 
            this.lbDisp.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lbDisp.Location = new System.Drawing.Point(658, 118);
            this.lbDisp.Name = "lbDisp";
            this.lbDisp.Size = new System.Drawing.Size(288, 265);
            this.lbDisp.TabIndex = 0;
            this.lbDisp.Text = "label1";
            // 
            // txtMessage
            // 
            this.txtMessage.Location = new System.Drawing.Point(658, 386);
            this.txtMessage.Multiline = true;
            this.txtMessage.Name = "txtMessage";
            this.txtMessage.Size = new System.Drawing.Size(490, 212);
            this.txtMessage.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(16, 12);
            this.label1.TabIndex = 12;
            this.label1.Text = "IP";
            // 
            // txtPort
            // 
            this.txtPort.Location = new System.Drawing.Point(248, 20);
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(63, 21);
            this.txtPort.TabIndex = 11;
            this.txtPort.Text = "5000";
            // 
            // txtIp
            // 
            this.txtIp.Location = new System.Drawing.Point(64, 20);
            this.txtIp.Name = "txtIp";
            this.txtIp.Size = new System.Drawing.Size(100, 21);
            this.txtIp.TabIndex = 10;
            this.txtIp.Text = "192.168.20.6";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(22, 47);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(290, 42);
            this.btnConnect.TabIndex = 9;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(205, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "Port";
            // 
            // lbJoystick
            // 
            this.lbJoystick.AutoSize = true;
            this.lbJoystick.BackColor = System.Drawing.Color.Transparent;
            this.lbJoystick.Font = new System.Drawing.Font("Georgia", 11F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))));
            this.lbJoystick.ForeColor = System.Drawing.Color.White;
            this.lbJoystick.Location = new System.Drawing.Point(766, 25);
            this.lbJoystick.Name = "lbJoystick";
            this.lbJoystick.Size = new System.Drawing.Size(197, 18);
            this.lbJoystick.TabIndex = 436;
            this.lbJoystick.Text = "Joystick (No Connected)";
            this.lbJoystick.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // tmrJoystick
            // 
            this.tmrJoystick.Tick += new System.EventHandler(this.tmrJoystick_Tick);
            // 
            // lbProp
            // 
            this.lbProp.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lbProp.Location = new System.Drawing.Point(952, 118);
            this.lbProp.Name = "lbProp";
            this.lbProp.Size = new System.Drawing.Size(197, 265);
            this.lbProp.TabIndex = 0;
            this.lbProp.Text = "label1";
            // 
            // pnDisp
            // 
            this.pnDisp.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.pnDisp.Controls.Add(this.txtTest);
            this.pnDisp.Controls.Add(this.txtTest4);
            this.pnDisp.Controls.Add(this.txtTest3);
            this.pnDisp.Controls.Add(this.txtTest2);
            this.pnDisp.Controls.Add(this.txtTest1);
            this.pnDisp.Controls.Add(this.txtTest0);
            this.pnDisp.Controls.Add(this.btnTest);
            this.pnDisp.Location = new System.Drawing.Point(12, 118);
            this.pnDisp.Name = "pnDisp";
            this.pnDisp.Size = new System.Drawing.Size(640, 480);
            this.pnDisp.TabIndex = 437;
            // 
            // btnTest
            // 
            this.btnTest.Location = new System.Drawing.Point(423, 190);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(75, 23);
            this.btnTest.TabIndex = 0;
            this.btnTest.Text = "button1";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // btnStartStreaming
            // 
            this.btnStartStreaming.Location = new System.Drawing.Point(147, 40);
            this.btnStartStreaming.Name = "btnStartStreaming";
            this.btnStartStreaming.Size = new System.Drawing.Size(128, 49);
            this.btnStartStreaming.TabIndex = 438;
            this.btnStartStreaming.Text = "Start Streaming";
            this.btnStartStreaming.UseVisualStyleBackColor = true;
            this.btnStartStreaming.Click += new System.EventHandler(this.btnStartStreaming_Click);
            // 
            // rdStream_Jpeg
            // 
            this.rdStream_Jpeg.AutoSize = true;
            this.rdStream_Jpeg.Location = new System.Drawing.Point(9, 42);
            this.rdStream_Jpeg.Name = "rdStream_Jpeg";
            this.rdStream_Jpeg.Size = new System.Drawing.Size(111, 16);
            this.rdStream_Jpeg.TabIndex = 439;
            this.rdStream_Jpeg.Text = "Jpeg Streaming";
            this.rdStream_Jpeg.UseVisualStyleBackColor = true;
            // 
            // rdStream_MJpeg
            // 
            this.rdStream_MJpeg.AutoSize = true;
            this.rdStream_MJpeg.Checked = true;
            this.rdStream_MJpeg.Location = new System.Drawing.Point(9, 64);
            this.rdStream_MJpeg.Name = "rdStream_MJpeg";
            this.rdStream_MJpeg.Size = new System.Drawing.Size(122, 16);
            this.rdStream_MJpeg.TabIndex = 439;
            this.rdStream_MJpeg.TabStop = true;
            this.rdStream_MJpeg.Text = "MJpeg Streaming";
            this.rdStream_MJpeg.UseVisualStyleBackColor = true;
            // 
            // btnStopStreaming
            // 
            this.btnStopStreaming.Location = new System.Drawing.Point(280, 40);
            this.btnStopStreaming.Name = "btnStopStreaming";
            this.btnStopStreaming.Size = new System.Drawing.Size(128, 49);
            this.btnStopStreaming.TabIndex = 438;
            this.btnStopStreaming.Text = "Stop Streaming";
            this.btnStopStreaming.UseVisualStyleBackColor = true;
            this.btnStopStreaming.Click += new System.EventHandler(this.btnStopStreaming_Click);
            // 
            // txtHeight
            // 
            this.txtHeight.Location = new System.Drawing.Point(321, 16);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new System.Drawing.Size(87, 21);
            this.txtHeight.TabIndex = 446;
            this.txtHeight.Text = "480";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 19);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 12);
            this.label4.TabIndex = 442;
            this.label4.Text = "Port";
            // 
            // txtWidth
            // 
            this.txtWidth.Location = new System.Drawing.Point(188, 16);
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.Size = new System.Drawing.Size(87, 21);
            this.txtWidth.TabIndex = 445;
            this.txtWidth.Text = "640";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(147, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 12);
            this.label5.TabIndex = 441;
            this.label5.Text = "Width";
            // 
            // txtPort_Cam
            // 
            this.txtPort_Cam.Location = new System.Drawing.Point(45, 15);
            this.txtPort_Cam.Name = "txtPort_Cam";
            this.txtPort_Cam.Size = new System.Drawing.Size(87, 21);
            this.txtPort_Cam.TabIndex = 447;
            this.txtPort_Cam.Text = "8081";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(280, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(40, 12);
            this.label6.TabIndex = 440;
            this.label6.Text = "Height";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txtPort);
            this.groupBox1.Controls.Add(this.txtIp);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.btnConnect);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(325, 100);
            this.groupBox1.TabIndex = 448;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Connection";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.txtHeight);
            this.groupBox2.Controls.Add(this.rdStream_Jpeg);
            this.groupBox2.Controls.Add(this.btnStopStreaming);
            this.groupBox2.Controls.Add(this.rdStream_MJpeg);
            this.groupBox2.Controls.Add(this.btnStartStreaming);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.txtWidth);
            this.groupBox2.Controls.Add(this.txtPort_Cam);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(343, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(418, 100);
            this.groupBox2.TabIndex = 449;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "groupBox2";
            // 
            // txtTest0
            // 
            this.txtTest0.Location = new System.Drawing.Point(267, 98);
            this.txtTest0.Name = "txtTest0";
            this.txtTest0.Size = new System.Drawing.Size(58, 21);
            this.txtTest0.TabIndex = 450;
            // 
            // txtTest1
            // 
            this.txtTest1.Location = new System.Drawing.Point(331, 98);
            this.txtTest1.Name = "txtTest1";
            this.txtTest1.Size = new System.Drawing.Size(58, 21);
            this.txtTest1.TabIndex = 450;
            // 
            // txtTest2
            // 
            this.txtTest2.Location = new System.Drawing.Point(404, 98);
            this.txtTest2.Name = "txtTest2";
            this.txtTest2.Size = new System.Drawing.Size(58, 21);
            this.txtTest2.TabIndex = 450;
            // 
            // txtTest3
            // 
            this.txtTest3.Location = new System.Drawing.Point(468, 98);
            this.txtTest3.Name = "txtTest3";
            this.txtTest3.Size = new System.Drawing.Size(58, 21);
            this.txtTest3.TabIndex = 450;
            // 
            // txtTest4
            // 
            this.txtTest4.Location = new System.Drawing.Point(532, 98);
            this.txtTest4.Name = "txtTest4";
            this.txtTest4.Size = new System.Drawing.Size(58, 21);
            this.txtTest4.TabIndex = 450;
            // 
            // txtTest
            // 
            this.txtTest.Location = new System.Drawing.Point(515, 125);
            this.txtTest.Name = "txtTest";
            this.txtTest.Size = new System.Drawing.Size(75, 23);
            this.txtTest.TabIndex = 451;
            this.txtTest.Text = "button1";
            this.txtTest.UseVisualStyleBackColor = true;
            this.txtTest.Click += new System.EventHandler(this.txtTest_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1156, 613);
            this.Controls.Add(this.lbJoystick);
            this.Controls.Add(this.pnDisp);
            this.Controls.Add(this.txtMessage);
            this.Controls.Add(this.lbProp);
            this.Controls.Add(this.lbDisp);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Main";
            this.Text = "OpenRov";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Main_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.pnDisp.ResumeLayout(false);
            this.pnDisp.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbDisp;
        private System.Windows.Forms.TextBox txtMessage;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtPort;
        private System.Windows.Forms.TextBox txtIp;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lbJoystick;
        private System.Windows.Forms.Timer tmrJoystick;
        private System.Windows.Forms.Label lbProp;
        private System.Windows.Forms.Panel pnDisp;
        private System.Windows.Forms.Button btnStartStreaming;
        private System.Windows.Forms.RadioButton rdStream_Jpeg;
        private System.Windows.Forms.RadioButton rdStream_MJpeg;
        private System.Windows.Forms.Button btnStopStreaming;
        private System.Windows.Forms.TextBox txtHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtWidth;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtPort_Cam;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Button txtTest;
        private System.Windows.Forms.TextBox txtTest4;
        private System.Windows.Forms.TextBox txtTest3;
        private System.Windows.Forms.TextBox txtTest2;
        private System.Windows.Forms.TextBox txtTest1;
        private System.Windows.Forms.TextBox txtTest0;
    }
}

