using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Kom2GUI
{
    public partial class KomView : UserControl
    {

        bool changed;
        bool encryptchanged = false;

        public bool Changed { get { return changed;} }

        public string Filename
        {
            get { return this.textBox1.Text; }
        }

        public KomView()
        {
            InitializeComponent();
        }

        public void Save(string filename)
        {
            this.textBox1.Text = System.IO.Path.GetFullPath(filename);
            kom.Save(Filename);
            if(kom.ECB== null)
            {
                kom.Close();
            }            
            kom.Open(Filename);
            foreach (ListViewItem item in listView1.Items)
            {
                item.BackColor = System.Drawing.Color.White;
            }
            changed = false;
            encryptchanged = false;
        }

        public DialogResult LoadKom(string filename)
        {
            this.textBox1.Text = System.IO.Path.GetFullPath(filename);

            Kom2.NET.Komfile.EKomType t = Kom2.NET.Komfile.IsKom(filename);

            if (t == Kom2.NET.Komfile.EKomType.notkom)
                return DialogResult.Abort;

            DialogResult re = DialogResult.Abort;

            if(t == Kom2.NET.Komfile.EKomType.encrypt)
            {
                Form2 form2 = new Form2();
                if (form2.ShowDialog() == DialogResult.OK)
                {
                    kom.SetECB(form2.PW);
                }
                else
                {
                    return DialogResult.Cancel;
                }
            }
            
            bool open = kom.Open(filename);

            if(open == true)
            {
                this.listView1.Items.Clear();

                foreach(System.Collections.Generic.KeyValuePair<string,Kom2.NET.Kom2SubFile> KeyValue in kom.Subfiles)
                {
                    this.listView1.Items.Add(new KonmListViewItem(KeyValue.Key, KeyValue.Value));
                }
                changed = false;
                re = DialogResult.OK;
                if (kom.KomType == Kom2.NET.Komfile.EKomType.encrypt)
                    checkBox1.Checked = true;
            }
            return re;
        }        

        public void AddFile(string filename)
        {
            string key = System.IO.Path.GetFileName(filename);
            Kom2.NET.Kom2SubFile subfile = Kom2.NET.Kom2SubFile.ReadSubFileFromFile(filename);
            kom.Add(key, subfile,true);            
            ListViewItem item = listView1.Items.Add(new KonmListViewItem(key, subfile));
            item.BackColor = System.Drawing.Color.LightSkyBlue;

            changed = true;
        }

        public bool Contains(string key)
        {
            return kom.Subfiles.ContainsKey(key);
        }

        public void Remove(string key)
        {
            kom.Subfiles.Remove(key);
            System.Windows.Forms.ListViewItem deleteitem = null;
            foreach (System.Windows.Forms.ListViewItem item in listView1.Items)
            {
                if (item.Text == key)
                {
                    listView1.Items.Remove(item);
                    break;
                }
            }

            changed = true;
        }

        public bool Extract(string key,string path)
        {
            if(kom.Subfiles.ContainsKey(key)==true)
            {
                string filename = System.IO.Path.Combine(path,key);
                System.IO.FileStream file = new System.IO.FileStream(filename, System.IO.FileMode.Create);
                kom.Subfiles[key].WriteDecompressed(file);
                file.Close();
                return true;
            }
            return false;
        }

        Kom2.NET.Komfile kom = new Kom2.NET.Komfile();

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            if (listView1.SelectedItems.Count > 0)
            {
                this.����Ǯ��ToolStripMenuItem2.Enabled = true;
                this.����ToolStripMenuItem2.Enabled = true;
            }
            else
            {
                this.����Ǯ��ToolStripMenuItem2.Enabled = false;
                this.����ToolStripMenuItem2.Enabled = false;
            }
        }

        private void ����Ǯ��ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                foreach (ListViewItem item in listView1.Items)
                {
                    Extract(item.Text, folderBrowserDialog1.SelectedPath);
                }
            }
        }

        private void ����Ǯ��ToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                foreach (ListViewItem item in listView1.SelectedItems)
                {
                    Extract(item.Text, folderBrowserDialog1.SelectedPath);
                }
            }
        }


        private void ����ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(MessageBox.Show("������ ���� �Ͻðڽ��ϱ�?","����", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                this.kom.Subfiles.Clear();
                this.listView1.Clear();
            }            
        }

        private void ����ToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("������ ���� �Ͻðڽ��ϱ�?", "����", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                System.Collections.ArrayList dellist = new System.Collections.ArrayList();
                foreach (ListViewItem item in listView1.SelectedItems)
                {
                    dellist.Add(item.Text);
                }

                foreach(string key in dellist)
                {
                    Remove(key);
                }
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            // ��ũ��Ʈ�̰� 
            if (kom.ECB != null)
            {
                if (checkBox1.Checked == false) // ��ũ��Ʈ�� Ǯ���� �ϸ� 
                {
                    if (encryptchanged == true)
                    {
                        MessageBox.Show("���� �ϱ� ���� �� ���� �ٲ� �� �ֽ��ϴ�", "����", MessageBoxButtons.OK);
                        checkBox1.Checked = !checkBox1.Checked;
                        return;
                    }                    
                    if (MessageBox.Show("��ȣȭ�� ���������� ���ŵ˴ϴ�.\n���� �Ͻðڽ��ϱ�?", "���", MessageBoxButtons.YesNo, MessageBoxIcon.Warning)
                        == DialogResult.Yes)
                    {
                        kom.Decrypt();
                        encryptchanged = true;
                    }
                    else
                    {
                        checkBox1.Checked = true;
                    }
                }
            }
            else
            {
                if (checkBox1.Checked == true)
                {
                    if (encryptchanged == true)
                    {
                        MessageBox.Show("���� �ϱ� ���� �� ���� �ٲ� �� �ֽ��ϴ�", "����", MessageBoxButtons.OK);
                        checkBox1.Checked = !checkBox1.Checked;
                        return;
                    }          

                    Form2 form2 = new Form2();
                    if (form2.ShowDialog() == DialogResult.OK)
                    {
                        kom.SetECB(form2.PW);
                        kom.Encrypt();
                        encryptchanged = true;
                    }
                    else
                    {
                        checkBox1.Checked = false;
                    }
                }
            }
        

        }

     
       
           
    }
}
