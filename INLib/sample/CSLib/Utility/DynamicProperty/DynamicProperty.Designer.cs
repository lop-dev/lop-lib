namespace DynamicProperty
{
    partial class DynamicProperty
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.BrowseTypeTextBox = new System.Windows.Forms.TextBox();
            this.BrowseTypeComboBox = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // BrowseTypeTextBox
            // 
            this.BrowseTypeTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.BrowseTypeTextBox.Location = new System.Drawing.Point(3, 4);
            this.BrowseTypeTextBox.Multiline = true;
            this.BrowseTypeTextBox.Name = "BrowseTypeTextBox";
            this.BrowseTypeTextBox.Size = new System.Drawing.Size(475, 286);
            this.BrowseTypeTextBox.TabIndex = 0;
            // 
            // BrowseTypeComboBox
            // 
            this.BrowseTypeComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.BrowseTypeComboBox.FormattingEnabled = true;
            this.BrowseTypeComboBox.Items.AddRange(new object[] {
            "Type1",
            "Type2"});
            this.BrowseTypeComboBox.Location = new System.Drawing.Point(484, 4);
            this.BrowseTypeComboBox.Name = "BrowseTypeComboBox";
            this.BrowseTypeComboBox.Size = new System.Drawing.Size(121, 20);
            this.BrowseTypeComboBox.TabIndex = 1;
            this.BrowseTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.BrowseTypeComboBox_SelectedIndexChanged);
            // 
            // DynamicProperty
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(609, 293);
            this.Controls.Add(this.BrowseTypeComboBox);
            this.Controls.Add(this.BrowseTypeTextBox);
            this.Name = "DynamicProperty";
            this.Text = "DynamicProperty";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox BrowseTypeTextBox;
        private System.Windows.Forms.ComboBox BrowseTypeComboBox;
    }
}

