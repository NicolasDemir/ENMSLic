using System.Security.Policy;
using System.Text;
using System.Reflection;
using System.Net.NetworkInformation;
using System.ComponentModel;
using System.Diagnostics.Metrics;
using System;

namespace ClientDotNet
{
    public partial class Form1 : Form
    {
        EncryptionManager m_encryptMgr;

        public Form1()
        {
            InitializeComponent();
            m_encryptMgr = new EncryptionManager();
            m_list.Columns.Add("Options");
            m_list.Columns.Add("Values");
            m_list.Columns[0].Width = 150;
            m_list.Columns[1].Width = m_list.ClientSize.Width - 150;

        

        }


        int addOption(string option, string value)
        {
            ListViewItem item = new ListViewItem(option);
            item.SubItems.Add(value);
            m_list.Items.Add(item);
            return 0;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }


        private void button6_Click(object sender, EventArgs e)
        {
            string currentDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

            string szPath = tbFilePath.Text;
            string fullPath = Path.Combine(currentDirectory, szPath);

            if (!File.Exists(fullPath))
            {
                MessageBox.Show("unable to find a license file at " + fullPath);
            }

            textBoxCp.Text = "";
            textBoxCs.Text = "";
            tbError.Text = "";
            textBoxCs.ForeColor = Color.Red;
            textBoxCp.ForeColor = Color.Red;

            tbReturn.ForeColor = Color.Black;
            tbError.ForeColor = Color.Black;

            int nreturn = 0;

            string sz = textBox5.Text;

            if (sz.Length != 8 && sz.Length != 12)
                MessageBox.Show("Le SHA256 ne fonctionne dans les deux mondes que pour une clé de 4, 8 ou 12 chars");

            nreturn = m_encryptMgr.CheckOut(sz, fullPath);
            string ResultString = m_encryptMgr.GetResultString(nreturn);

            if (nreturn != 1)
            {
                tbReturn.ForeColor = Color.Red;
                tbError.ForeColor = Color.Red;
            }

            string result2 = m_encryptMgr.Sha256(sz);

            if (m_encryptMgr.HashValue == result2)
            {
                textBoxCs.ForeColor = textBoxCp.ForeColor = Color.Green;
            }

            tbReturn.Text = nreturn.ToString();
            tbError.Text = ResultString;

            int nmajeur = 1;
            int nMineur = 0;
            if (m_encryptMgr.Version >= 100)
            {
                nmajeur = m_encryptMgr.Version / 100;
                nMineur = m_encryptMgr.Version % 100;
            }

            string szVersion = string.Format("{0}.{1}", nmajeur, nMineur);

            m_list.Items.Clear();

            int i = 0;

            addOption("Serial", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Serial;

            addOption("Version", "");
            m_list.Items[i++].SubItems[1].Text = szVersion;

            addOption("Users", "");
            if (m_encryptMgr.Users > 0)
                m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Users.ToString();
            else
                m_list.Items[i++].SubItems[1].Text = "Unlimited";

            addOption("Meters", "");
            if (m_encryptMgr.Meters > 0)
                m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Meters.ToString();
            else
                m_list.Items[i++].SubItems[1].Text = "Unlimited";

            addOption("Connections", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Connections.ToString();

            addOption("Product", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Product.ToString();

            addOption("Update", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Update.ToString();

            addOption("Activation date", "");
            m_list.Items[i++].SubItems[1].Text = "Permanent";

            textBoxCp.Text = m_encryptMgr.HashValue;
            textBoxCs.Text = result2;
        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            string stream = m_encryptMgr.VerifyStream();

            textBoxCp.Text = stream;
            textBoxCs.Text = EncryptionManager.g_ExpectedStream;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string currentDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

            string szPath = tbFilePath.Text;
            string fullPath = Path.Combine(currentDirectory, szPath);

            if (!File.Exists(fullPath))
            {
                MessageBox.Show("unable to find a license file at " + fullPath);
            }

            textBoxCp.Text = "";
            textBoxCs.Text = "";
            tbError.Text = "";
            textBoxCs.ForeColor = Color.Red;
            textBoxCp.ForeColor = Color.Red;
            tbReturn.ForeColor = Color.Black;
            tbError.ForeColor = Color.Black;


            int nreturn = 0;

            string sz = textBox5.Text;

            if (sz.Length != 8 && sz.Length != 12)
                MessageBox.Show("Le SHA256 ne fonctionne dans les deux mondes que pour une clé de 4, 8 ou 12 chars");

            nreturn = m_encryptMgr.CheckOut2(sz, fullPath);
            string ResultString = m_encryptMgr.GetResultString(nreturn);

            if (nreturn != 1)
            {
                tbReturn.ForeColor = Color.Red;
                tbError.ForeColor = Color.Red;
            }


            string result2 = m_encryptMgr.Sha256(sz);

            if (m_encryptMgr.HashValue == result2)
            {
                textBoxCs.ForeColor = textBoxCp.ForeColor = Color.Green;
            }

            tbReturn.Text = nreturn.ToString();
            tbError.Text = ResultString;

            int nmajeur = 1;
            int nMineur = 0;
            if (m_encryptMgr.Version >= 100)
            {
                nmajeur = m_encryptMgr.Version / 100;
                nMineur = m_encryptMgr.Version % 100;
            }

            string szVersion = string.Format("{0}.{1}", nmajeur, nMineur);

            m_list.Items.Clear();

            int i = 0;
            addOption("Serial", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Serial;

            addOption("Version", "");
            m_list.Items[i++].SubItems[1].Text = szVersion;

            addOption("Users", "");
            if(m_encryptMgr.Users > 0)
                m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Users.ToString();
            else
                m_list.Items[i++].SubItems[1].Text = "Unlimited";

            addOption("Meters", "");
            if(m_encryptMgr.Meters > 0)
                m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Meters.ToString();
            else
                m_list.Items[i++].SubItems[1].Text = "Unlimited";

            addOption("Connections", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Connections.ToString();

            addOption("Product", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Product.ToString();

            addOption("Update", "");
            m_list.Items[i++].SubItems[1].Text = m_encryptMgr.Update.ToString();

        
            if (m_encryptMgr.TemporaryLicense)
            {
                addOption("Activation date", "");
                DateTime ActDate = new DateTime(m_encryptMgr.templicinfo.ActivationYear, m_encryptMgr.templicinfo.ActivationMonth, m_encryptMgr.templicinfo.ActivationDay);
                m_list.Items[i++].SubItems[1].Text = ActDate.ToShortDateString(); ;
                
                addOption("Expiration date", "");
                DateTime expDate = new DateTime(m_encryptMgr.templicinfo.ExpirationYear, m_encryptMgr.templicinfo.ExpirationMonth, m_encryptMgr.templicinfo.ExpirationDay);
                m_list.Items[i++].SubItems[1].Text = expDate.ToShortDateString(); ;
            }
            else
            {
                addOption("Activation date", "");
                m_list.Items[i++].SubItems[1].Text = "Permanent";
            }

            textBoxCp.Text = m_encryptMgr.HashValue;
            textBoxCs.Text = result2;
        }

        private void brFilePath_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "Select a File";
            openFileDialog.Filter = "All Files (*.*)|*.*"; // Change filter as needed

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Get the selected file path
                string filePath = openFileDialog.FileName;
                tbFilePath.Text = filePath;
            }
        }
    }
}