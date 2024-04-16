using System.Security.Policy;
using System.Text;
using System.Reflection;
using System.Net.NetworkInformation;
using System.ComponentModel;
using System.Diagnostics.Metrics;

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

            addOption("Serial", "");
            addOption("Users", "");
            addOption("Meters", "");
            addOption("Connections", "");
            addOption("Version", "");
            addOption("Product", "");
            addOption("Update", "");
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
            int nreturn = 0;

            string sz = textBox5.Text;

            if (sz.Length != 8 && sz.Length != 12)
                MessageBox.Show("Le SHA256 ne fonctionne dans les deux mondes que pour une clé de 4, 8 ou 12 chars");

            nreturn = m_encryptMgr.CheckOut(sz, fullPath);
            string ResultString = m_encryptMgr.GetResultString(nreturn);

            string result2 = m_encryptMgr.Sha256(sz);

            if (m_encryptMgr.HashValue == result2)
            {
                textBoxCs.ForeColor = textBoxCp.ForeColor = Color.Green;
            }

            tbReturn.Text = nreturn.ToString();
            tbError.Text = ResultString;

            m_list.Items[0].SubItems[1].Text = m_encryptMgr.Serial;
            m_list.Items[1].SubItems[1].Text = m_encryptMgr.Users.ToString();
            m_list.Items[2].SubItems[1].Text = m_encryptMgr.Meters.ToString();
            m_list.Items[3].SubItems[1].Text = m_encryptMgr.Connections.ToString();
            m_list.Items[4].SubItems[1].Text = m_encryptMgr.Version.ToString();
            m_list.Items[5].SubItems[1].Text = m_encryptMgr.Product.ToString();
            m_list.Items[6].SubItems[1].Text = m_encryptMgr.Update.ToString();

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
    }

}