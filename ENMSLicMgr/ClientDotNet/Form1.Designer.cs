namespace ClientDotNet
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            button3 = new Button();
            groupBox3 = new GroupBox();
            button1 = new Button();
            tbError = new TextBox();
            tbReturn = new TextBox();
            label3 = new Label();
            tbFilePath = new TextBox();
            brFilePath = new Button();
            label1 = new Label();
            groupBox2 = new GroupBox();
            textBox5 = new TextBox();
            button4 = new Button();
            label9 = new Label();
            textBoxCs = new TextBox();
            label8 = new Label();
            textBoxCp = new TextBox();
            label7 = new Label();
            buttonCheckout = new Button();
            groupBox1 = new GroupBox();
            m_list = new ListView();
            groupBox3.SuspendLayout();
            groupBox2.SuspendLayout();
            groupBox1.SuspendLayout();
            SuspendLayout();
            // 
            // button3
            // 
            button3.Location = new Point(449, 527);
            button3.Name = "button3";
            button3.Size = new Size(75, 23);
            button3.TabIndex = 6;
            button3.Text = "Close";
            button3.UseVisualStyleBackColor = true;
            button3.Click += button3_Click;
            // 
            // groupBox3
            // 
            groupBox3.Controls.Add(button1);
            groupBox3.Controls.Add(tbError);
            groupBox3.Controls.Add(tbReturn);
            groupBox3.Controls.Add(label3);
            groupBox3.Controls.Add(tbFilePath);
            groupBox3.Controls.Add(brFilePath);
            groupBox3.Controls.Add(label1);
            groupBox3.Controls.Add(groupBox2);
            groupBox3.Controls.Add(buttonCheckout);
            groupBox3.Location = new Point(12, 289);
            groupBox3.Name = "groupBox3";
            groupBox3.Size = new Size(512, 227);
            groupBox3.TabIndex = 7;
            groupBox3.TabStop = false;
            groupBox3.Text = "Licensing ";
            // 
            // button1
            // 
            button1.Location = new Point(411, 25);
            button1.Name = "button1";
            button1.Size = new Size(77, 23);
            button1.TabIndex = 35;
            button1.Text = "Checkout 2";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // tbError
            // 
            tbError.Location = new Point(156, 55);
            tbError.Name = "tbError";
            tbError.Size = new Size(332, 23);
            tbError.TabIndex = 34;
            // 
            // tbReturn
            // 
            tbReturn.Location = new Point(109, 53);
            tbReturn.Name = "tbReturn";
            tbReturn.ReadOnly = true;
            tbReturn.Size = new Size(41, 23);
            tbReturn.TabIndex = 33;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(15, 57);
            label3.Name = "label3";
            label3.Size = new Size(73, 15);
            label3.TabIndex = 32;
            label3.Text = "Return value";
            // 
            // tbFilePath
            // 
            tbFilePath.Location = new Point(109, 25);
            tbFilePath.Name = "tbFilePath";
            tbFilePath.Size = new Size(181, 23);
            tbFilePath.TabIndex = 31;
            tbFilePath.Text = "lic.dat";
            // 
            // brFilePath
            // 
            brFilePath.Location = new Point(292, 25);
            brFilePath.Name = "brFilePath";
            brFilePath.Size = new Size(25, 23);
            brFilePath.TabIndex = 30;
            brFilePath.Text = "...";
            brFilePath.UseVisualStyleBackColor = true;
            brFilePath.Click += brFilePath_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(15, 29);
            label1.Name = "label1";
            label1.Size = new Size(92, 15);
            label1.TabIndex = 28;
            label1.Text = "License file path";
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(textBox5);
            groupBox2.Controls.Add(button4);
            groupBox2.Controls.Add(label9);
            groupBox2.Controls.Add(textBoxCs);
            groupBox2.Controls.Add(label8);
            groupBox2.Controls.Add(textBoxCp);
            groupBox2.Controls.Add(label7);
            groupBox2.Location = new Point(13, 84);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(486, 131);
            groupBox2.TabIndex = 18;
            groupBox2.TabStop = false;
            groupBox2.Text = "Interface securization";
            // 
            // textBox5
            // 
            textBox5.Location = new Point(61, 28);
            textBox5.Name = "textBox5";
            textBox5.Size = new Size(110, 23);
            textBox5.TabIndex = 0;
            textBox5.Text = "12345678";
            // 
            // button4
            // 
            button4.Enabled = false;
            button4.Location = new Point(399, 28);
            button4.Name = "button4";
            button4.Size = new Size(75, 23);
            button4.TabIndex = 8;
            button4.Text = "Integrity";
            button4.UseVisualStyleBackColor = true;
            button4.Click += button4_Click_1;
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new Point(6, 32);
            label9.Name = "label9";
            label9.Size = new Size(46, 15);
            label9.TabIndex = 11;
            label9.Text = "Input =";
            // 
            // textBoxCs
            // 
            textBoxCs.Location = new Point(61, 58);
            textBoxCs.Name = "textBoxCs";
            textBoxCs.Size = new Size(413, 23);
            textBoxCs.TabIndex = 4;
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Location = new Point(8, 92);
            label8.Name = "label8";
            label8.Size = new Size(45, 15);
            label8.TabIndex = 10;
            label8.Text = "C++  =";
            // 
            // textBoxCp
            // 
            textBoxCp.Location = new Point(61, 88);
            textBoxCp.Name = "textBoxCp";
            textBoxCp.Size = new Size(413, 23);
            textBoxCp.TabIndex = 5;
            textBoxCp.TextChanged += textBox7_TextChanged;
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new Point(17, 62);
            label7.Name = "label7";
            label7.Size = new Size(36, 15);
            label7.TabIndex = 9;
            label7.Text = "C#  =";
            // 
            // buttonCheckout
            // 
            buttonCheckout.Location = new Point(330, 25);
            buttonCheckout.Name = "buttonCheckout";
            buttonCheckout.Size = new Size(77, 23);
            buttonCheckout.TabIndex = 3;
            buttonCheckout.Text = "Checkout";
            buttonCheckout.UseVisualStyleBackColor = true;
            buttonCheckout.Click += button6_Click;
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(m_list);
            groupBox1.Location = new Point(12, 12);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(512, 257);
            groupBox1.TabIndex = 8;
            groupBox1.TabStop = false;
            groupBox1.Text = "License options";
            // 
            // m_list
            // 
            m_list.Location = new Point(21, 22);
            m_list.Name = "m_list";
            m_list.Size = new Size(467, 215);
            m_list.TabIndex = 0;
            m_list.UseCompatibleStateImageBehavior = false;
            m_list.View = View.Details;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(541, 560);
            Controls.Add(groupBox1);
            Controls.Add(groupBox3);
            Controls.Add(button3);
            FormBorderStyle = FormBorderStyle.Fixed3D;
            Name = "Form1";
            groupBox3.ResumeLayout(false);
            groupBox3.PerformLayout();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            groupBox1.ResumeLayout(false);
            ResumeLayout(false);
        }

        #endregion
        private Button button3;
        private GroupBox groupBox3;
        private TextBox textBox5;
        private Button buttonCheckout;
        private TextBox textBoxCp;
        private TextBox textBoxCs;
        private Label label8;
        private Label label7;
        private Label label9;
        private TextBox textBox8;
        private TextBox textBox7;
        private Button button4;
        private GroupBox groupBox1;
        private Button brFilePath;
        private Label label1;
        private GroupBox groupBox2;
        private TextBox tbFilePath;
        private ListView m_list;
        private TextBox tbReturn;
        private Label label3;
        private TextBox tbError;
        private Button button1;
    }
}