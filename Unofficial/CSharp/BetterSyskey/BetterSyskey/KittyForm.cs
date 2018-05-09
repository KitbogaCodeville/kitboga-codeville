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
    public partial class KittyForm : Form
    {
        private string entered_password="";

        public KittyForm()
        {
            InitializeComponent();
        }

        public void setPassword(string password)
        {
            entered_password = password;
            refreshPasswordLabel();
        }

        private void refreshPasswordLabel()
        {
            passwordTextBox.Text = entered_password;
        }
    }
}
