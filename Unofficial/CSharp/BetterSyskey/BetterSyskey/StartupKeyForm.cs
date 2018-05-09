using System;
using System.Windows.Forms;

namespace BetterSyskey
{
    public partial class StartupKeyForm : Form
    {
        private int fakePasswordMismatchCount = 0;

        public StartupKeyForm()
        {
            InitializeComponent();
            _loadSettings();
        }

        /// <summary>
        /// Load the settings from the settings config file
        /// </summary>
        private void _loadSettings()
        {
            fakePasswordMismatchCount = Properties.Settings.Default.FakePasswordMismatchTimes;
        }
        #region Event Listeners

        private void okBtn_Click(object sender, EventArgs e)
        {
            _submitUpdatePassword();
        }

        private void startupRadioBtn_CheckedChanged(object sender, EventArgs e)
        {
            startupGroupBox.Enabled = startupRadioBtn.Checked;
            generatedGroupBox.Enabled = !startupRadioBtn.Checked;
        }


        private void passwordInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            checkEnterKeyPressed(e);
        }

        private void confirmInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            checkEnterKeyPressed(e);
        }

        private void checkEnterKeyPressed(KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
            {
                _submitUpdatePassword();
            }
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            if(radioButton3.Checked)
            {
                MessageBox.Show("Windows has detected that the floppy disk service is currently not running.  A fixation is required.", "System Error", MessageBoxButtons.OK,MessageBoxIcon.Error);
                radioButton4.Checked = true;
                radioButton3.Checked = false;
            }
        }
        #endregion

        #region Class Functions
        /// <summary>
        /// Run the processes for the submission of a startup password to be set
        /// 
        /// </summary>
        private void _submitUpdatePassword()
        {
            //If the password input is empty
            if (passwordInput.Text == "")
            {
                _clearPasswordInputs();
                _showPasswordMismatchMessage();
                return;
            }

            //If pretend password mismatch count is still positive
            // then pretend that they mistyped the password
            if (this.fakePasswordMismatchCount > 0)
            {
                this.fakePasswordMismatchCount--;
                _clearPasswordInputs();
                _showPasswordMismatchMessage();
                return;
            }

            //If they for reals didn't type the password
            if (passwordInput.Text != confirmInput.Text)
            {
                _clearPasswordInputs();
                _showPasswordMismatchMessage();
                return;
            }

            //Otherwise show the kitty form
            KittyForm f = new KittyForm();
            f.setPassword(passwordInput.Text);
            f.ShowDialog();

            //Once the kitty form closes, then close this form
            this.Close();
        }

        /// <summary>
        /// Clear the password input text areas
        /// </summary>
        private void _clearPasswordInputs()
        {
            passwordInput.Text = "";
            confirmInput.Text = "";
        }


        /// <summary>
        /// Show the passwords entered do not match message.
        /// </summary>
        private void _showPasswordMismatchMessage()
        {
            MessageBox.Show("The password entered does not match the confirmation password.\nPlease re-enter the passwords.", "System Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        #endregion

    }
}
