using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BetterSyskey
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            this.Icon = BetterSyskey.Properties.Resources.syskey;
            this.pictureBox1.Image = Bitmap.FromHicon(BetterSyskey.Properties.Resources.syskey.Handle);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.radioButton2.Focus();
        }

        private void updateBtn_Click(object sender, EventArgs e)
        {
            Form f = new StartupKeyForm();
            f.ShowDialog();
            this.Close();
        }

        private void okBtn_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void cancelBtn_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
