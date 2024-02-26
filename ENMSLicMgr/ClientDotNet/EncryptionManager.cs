using System;
using System.Collections.Generic;
using System.Diagnostics.Metrics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;



namespace ClientDotNet
{
    internal class EncryptionManager
    {
        [DllImport("NatLicMgr.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CheckOut", CharSet = CharSet.Ansi)]
        public static extern int _CheckOut(string challengeQuery, StringBuilder challengeResponse, StringBuilder serial, ref int meters, ref int users, ref int connections, ref int update, ref int product, ref int version, string filePath);

        public static string g_ExpectedStream = "5c065f43a2ef30ec637ebd4129272020";

        public int Connections { get; set; }
        public int Users { get; set; }
        public int Meters { get; set; }
        public int Version { get; set; }
        public int Update  { get; set; }
        public int Product{ get; set; }

        public string HashValue { get; set; }
        public string Serial{ get; set; }

        public string Sha256(string input)
        {
            using (SHA256 sha256 = SHA256.Create())
            {
                byte[] bytes = sha256.ComputeHash(Encoding.UTF8.GetBytes(input));
                StringBuilder builder = new StringBuilder();
                for (int i = 0; i < bytes.Length; i++)
                {
                    builder.Append(bytes[i].ToString("x2"));
                }
                return builder.ToString();
            }
        }

        public string  VerifyStream()
        {
            var stream = File.OpenRead("natlicmgr.dll");
            var md5 = MD5.Create();

            byte[] hashBytes = md5.ComputeHash(stream);
            string sz1 = BitConverter.ToString(hashBytes).Replace("-", "").ToLowerInvariant();

            return sz1;
        
        }


        public int CheckOut(string A, string szpath)
        {
            StringBuilder abuild = new StringBuilder(2000);
            StringBuilder serial = new StringBuilder(2000);
            int nconnections = 0;
            int nusers = 0;
            int nmeters = 0;
            int nupdate = 0;
            int nproduct = 0;
            int nversion= 0;

            HashValue = "?";
            Connections = 0;
            Meters = 0;
            Users = 0;
            Version = 0;
            Update = 0;
            Product = 0;
            Serial = "?";

            int nreturnvalue = 0;

            try
            {
                nreturnvalue = _CheckOut(A, abuild, serial, ref nmeters, ref nusers, ref nconnections, ref nupdate, ref nproduct, ref nversion, szpath);
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
                nreturnvalue = 0;
            }

            if (nreturnvalue == 1 )
            {
                HashValue = abuild.ToString();
                Serial = serial.ToString();
                Connections = nconnections;
                Meters = nmeters;
                Users = nusers;
                Version = nversion;
                Update = nupdate;
                Product = nproduct;

            }

            return nreturnvalue; 
        }

      
    }



}
