namespace BetterSyskey
{
    partial class StartupKeyForm
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
            this.startupGroupBox = new System.Windows.Forms.GroupBox();
            this.confirmInput = new System.Windows.Forms.TextBox();
            this.passwordInput = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.startupRadioBtn = new System.Windows.Forms.RadioButton();
            this.generatedGroupBox = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.label4 = new System.Windows.Forms.Label();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.okBtn = new System.Windows.Forms.Button();
            this.cancelBtn = new System.Windows.Forms.Button();
            this.startupGroupBox.SuspendLayout();
            this.generatedGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // startupGroupBox
            // 
            this.startupGroupBox.Controls.Add(this.confirmInput);
            this.startupGroupBox.Controls.Add(this.passwordInput);
            this.startupGroupBox.Controls.Add(this.label3);
            this.startupGroupBox.Controls.Add(this.label2);
            this.startupGroupBox.Controls.Add(this.label1);
            this.startupGroupBox.Enabled = false;
            this.startupGroupBox.Location = new System.Drawing.Point(12, 12);
            this.startupGroupBox.Name = "startupGroupBox";
            this.startupGroupBox.Size = new System.Drawing.Size(280, 115);
            this.startupGroupBox.TabIndex = 0;
            this.startupGroupBox.TabStop = false;
            // 
            // confirmInput
            // 
            this.confirmInput.Location = new System.Drawing.Point(92, 78);
            this.confirmInput.Name = "confirmInput";
            this.confirmInput.Size = new System.Drawing.Size(182, 20);
            this.confirmInput.TabIndex = 6;
            this.confirmInput.UseSystemPasswordChar = true;
            this.confirmInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.confirmInput_KeyPress);
            // 
            // passwordInput
            // 
            this.passwordInput.Location = new System.Drawing.Point(92, 52);
            this.passwordInput.Name = "passwordInput";
            this.passwordInput.Size = new System.Drawing.Size(182, 20);
            this.passwordInput.TabIndex = 5;
            this.passwordInput.UseSystemPasswordChar = true;
            this.passwordInput.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.passwordInput_KeyPress);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(27, 81);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Confirm:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(27, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Password:";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(27, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(247, 33);
            this.label1.TabIndex = 2;
            this.label1.Text = "Requires a password to be entered during system start.";
            // 
            // startupRadioBtn
            // 
            this.startupRadioBtn.AutoSize = true;
            this.startupRadioBtn.Location = new System.Drawing.Point(19, 10);
            this.startupRadioBtn.Name = "startupRadioBtn";
            this.startupRadioBtn.Size = new System.Drawing.Size(108, 17);
            this.startupRadioBtn.TabIndex = 1;
            this.startupRadioBtn.TabStop = true;
            this.startupRadioBtn.Text = "Password Startup";
            this.startupRadioBtn.UseVisualStyleBackColor = true;
            this.startupRadioBtn.CheckedChanged += new System.EventHandler(this.startupRadioBtn_CheckedChanged);
            // 
            // generatedGroupBox
            // 
            this.generatedGroupBox.Controls.Add(this.label5);
            this.generatedGroupBox.Controls.Add(this.radioButton4);
            this.generatedGroupBox.Controls.Add(this.label4);
            this.generatedGroupBox.Controls.Add(this.radioButton3);
            this.generatedGroupBox.Location = new System.Drawing.Point(12, 133);
            this.generatedGroupBox.Name = "generatedGroupBox";
            this.generatedGroupBox.Size = new System.Drawing.Size(280, 150);
            this.generatedGroupBox.TabIndex = 1;
            this.generatedGroupBox.TabStop = false;
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(50, 104);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(223, 33);
            this.label5.TabIndex = 3;
            this.label5.Text = "Stores a key as part of the operating system, and no interaction is required duri" +
    "ng system start.";
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.Checked = true;
            this.radioButton4.Location = new System.Drawing.Point(32, 84);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(144, 17);
            this.radioButton4.TabIndex = 2;
            this.radioButton4.TabStop = true;
            this.radioButton4.Text = "Store Startup Key Locally";
            this.radioButton4.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(50, 48);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(223, 33);
            this.label4.TabIndex = 1;
            this.label4.Text = "Requires a floppy disk to be inserted during system start.";
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(32, 28);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(181, 17);
            this.radioButton3.TabIndex = 0;
            this.radioButton3.Text = "Store Startup Key on Floppy Disk\r\n";
            this.radioButton3.UseVisualStyleBackColor = true;
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Checked = true;
            this.radioButton2.Location = new System.Drawing.Point(19, 131);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(161, 17);
            this.radioButton2.TabIndex = 0;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "System Generated Password";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // okBtn
            // 
            this.okBtn.Location = new System.Drawing.Point(75, 289);
            this.okBtn.Name = "okBtn";
            this.okBtn.Size = new System.Drawing.Size(75, 23);
            this.okBtn.TabIndex = 2;
            this.okBtn.Text = "OK";
            this.okBtn.UseVisualStyleBackColor = true;
            this.okBtn.Click += new System.EventHandler(this.okBtn_Click);
            // 
            // cancelBtn
            // 
            this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelBtn.Location = new System.Drawing.Point(156, 289);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(75, 23);
            this.cancelBtn.TabIndex = 3;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.UseVisualStyleBackColor = true;
            // 
            // StartupKeyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelBtn;
            this.ClientSize = new System.Drawing.Size(304, 315);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.okBtn);
            this.Controls.Add(this.radioButton2);
            this.Controls.Add(this.startupRadioBtn);
            this.Controls.Add(this.generatedGroupBox);
            this.Controls.Add(this.startupGroupBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "StartupKeyForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "Startup Key";
            this.startupGroupBox.ResumeLayout(false);
            this.startupGroupBox.PerformLayout();
            this.generatedGroupBox.ResumeLayout(false);
            this.generatedGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox startupGroupBox;
        private System.Windows.Forms.TextBox confirmInput;
        private System.Windows.Forms.TextBox passwordInput;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton startupRadioBtn;
        private System.Windows.Forms.GroupBox generatedGroupBox;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.Button okBtn;
        private System.Windows.Forms.Button cancelBtn;
    }
}