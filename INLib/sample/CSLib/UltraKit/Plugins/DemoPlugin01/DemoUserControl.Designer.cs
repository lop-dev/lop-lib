namespace CSLib.UltraKit.DemoPlugin
{
    partial class DemoUserControl
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
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonSayHello = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.textBoxMsg = new System.Windows.Forms.TextBox();
            this.buttonClear = new System.Windows.Forms.Button();
            this.buttonSendEvent = new System.Windows.Forms.Button();
            this.toolTipEx1 = new BCGSoft.Controls.Shared.ToolTipEx();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.PluginMenuStrip = new System.Windows.Forms.MenuStrip();
            this.testToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sayHelloToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.PluginMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonSayHello
            // 
            this.buttonSayHello.Location = new System.Drawing.Point(13, 28);
            this.buttonSayHello.Name = "buttonSayHello";
            this.buttonSayHello.Size = new System.Drawing.Size(75, 23);
            this.buttonSayHello.TabIndex = 0;
            this.buttonSayHello.Text = "Say Hello";
            this.toolTipEx1.SetToolTipImage(this.buttonSayHello, null);
            this.toolTipEx1.SetToolTipParams(this.buttonSayHello, null);
            this.buttonSayHello.UseVisualStyleBackColor = true;
            this.buttonSayHello.Click += new System.EventHandler(this.buttonSayHello_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.textBoxMsg);
            this.groupBox2.Controls.Add(this.buttonClear);
            this.groupBox2.Controls.Add(this.buttonSendEvent);
            this.groupBox2.Location = new System.Drawing.Point(13, 178);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(323, 265);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Event Sample";
            this.toolTipEx1.SetToolTipImage(this.groupBox2, null);
            this.toolTipEx1.SetToolTipParams(this.groupBox2, null);
            // 
            // textBoxMsg
            // 
            this.textBoxMsg.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxMsg.Location = new System.Drawing.Point(6, 83);
            this.textBoxMsg.Multiline = true;
            this.textBoxMsg.Name = "textBoxMsg";
            this.textBoxMsg.ReadOnly = true;
            this.textBoxMsg.Size = new System.Drawing.Size(301, 176);
            this.textBoxMsg.TabIndex = 2;
            this.textBoxMsg.Text = "No Msg";
            this.toolTipEx1.SetToolTipImage(this.textBoxMsg, null);
            this.toolTipEx1.SetToolTipParams(this.textBoxMsg, null);
            // 
            // buttonClear
            // 
            this.buttonClear.Location = new System.Drawing.Point(232, 39);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(55, 23);
            this.buttonClear.TabIndex = 0;
            this.buttonClear.Text = "Clear";
            this.toolTipEx1.SetToolTipImage(this.buttonClear, null);
            this.toolTipEx1.SetToolTipParams(this.buttonClear, null);
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.buttonClear_Click);
            // 
            // buttonSendEvent
            // 
            this.buttonSendEvent.Location = new System.Drawing.Point(10, 39);
            this.buttonSendEvent.Name = "buttonSendEvent";
            this.buttonSendEvent.Size = new System.Drawing.Size(196, 23);
            this.buttonSendEvent.TabIndex = 0;
            this.buttonSendEvent.Text = "Send Event to Plugin02";
            this.toolTipEx1.SetToolTip(this.buttonSendEvent, "Show the Communication between Modules and Framework");
            this.toolTipEx1.SetToolTipImage(this.buttonSendEvent, null);
            this.toolTipEx1.SetToolTipParams(this.buttonSendEvent, null);
            this.buttonSendEvent.UseVisualStyleBackColor = true;
            this.buttonSendEvent.Click += new System.EventHandler(this.buttonSendEvent_Click);
            // 
            // toolTipEx1
            // 
            this.toolTipEx1.BorderWidth = 0F;
            this.toolTipEx1.Data = null;
            this.toolTipEx1.IsStandard = false;
            this.toolTipEx1.ObjectType = BCGSoft.Controls.Shared.TooltipObjectType.Undefined;
            // 
            // 
            // 
            this.toolTipEx1.Options.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(254)))), ((int)(((byte)(254)))), ((int)(((byte)(254)))));
            this.toolTipEx1.Options.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(118)))), ((int)(((byte)(118)))), ((int)(((byte)(118)))));
            this.toolTipEx1.Options.DescriptionForeColor = System.Drawing.SystemColors.InfoText;
            this.toolTipEx1.Options.ForeColor = System.Drawing.SystemColors.InfoText;
            this.toolTipEx1.Options.GradientAngle = 90F;
            this.toolTipEx1.Options.GradientColor = System.Drawing.Color.FromArgb(((int)(((byte)(201)))), ((int)(((byte)(217)))), ((int)(((byte)(239)))));
            this.toolTipEx1.OwnerDraw = true;
            this.toolTipEx1.TransparentColor = System.Drawing.Color.Magenta;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.button3);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Location = new System.Drawing.Point(13, 57);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(323, 98);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Command Sample";
            this.toolTipEx1.SetToolTipImage(this.groupBox1, null);
            this.toolTipEx1.SetToolTipParams(this.groupBox1, null);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(37, 68);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(196, 23);
            this.button3.TabIndex = 1;
            this.button3.Text = "Send Executed Command";
            this.toolTipEx1.SetToolTip(this.button3, "Executed the Command by Modules but insert the command into Undo/Redo list");
            this.toolTipEx1.SetToolTipImage(this.button3, null);
            this.toolTipEx1.SetToolTipParams(this.button3, null);
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(37, 30);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(196, 23);
            this.button2.TabIndex = 0;
            this.button2.Text = "Send UnExecuted Command";
            this.toolTipEx1.SetToolTip(this.button2, "Executed the command by Framework Command Manager");
            this.toolTipEx1.SetToolTipImage(this.button2, null);
            this.toolTipEx1.SetToolTipParams(this.button2, null);
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // PluginMenuStrip
            // 
            this.PluginMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.testToolStripMenuItem});
            this.PluginMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.PluginMenuStrip.Name = "PluginMenuStrip";
            this.PluginMenuStrip.Size = new System.Drawing.Size(346, 25);
            this.PluginMenuStrip.TabIndex = 6;
            this.PluginMenuStrip.Text = "menuStrip1";
            this.toolTipEx1.SetToolTipImage(this.PluginMenuStrip, null);
            this.toolTipEx1.SetToolTipParams(this.PluginMenuStrip, null);
            // 
            // testToolStripMenuItem
            // 
            this.testToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sayHelloToolStripMenuItem});
            this.testToolStripMenuItem.Name = "testToolStripMenuItem";
            this.testToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.testToolStripMenuItem.Text = "Test";
            // 
            // sayHelloToolStripMenuItem
            // 
            this.sayHelloToolStripMenuItem.Name = "sayHelloToolStripMenuItem";
            this.sayHelloToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.sayHelloToolStripMenuItem.Text = "SayHello";
            this.sayHelloToolStripMenuItem.Click += new System.EventHandler(this.sayHelloToolStripMenuItem_Click);
            // 
            // DemoUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.PluginMenuStrip);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.buttonSayHello);
            this.Name = "DemoUserControl";
            this.Size = new System.Drawing.Size(346, 449);
            this.toolTipEx1.SetToolTipImage(this, null);
            this.toolTipEx1.SetToolTipParams(this, null);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.PluginMenuStrip.ResumeLayout(false);
            this.PluginMenuStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonSayHello;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonSendEvent;
        private BCGSoft.Controls.Shared.ToolTipEx toolTipEx1;
        private System.Windows.Forms.TextBox textBoxMsg;
        private System.Windows.Forms.Button buttonClear;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.MenuStrip PluginMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem testToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sayHelloToolStripMenuItem;
    }
}
