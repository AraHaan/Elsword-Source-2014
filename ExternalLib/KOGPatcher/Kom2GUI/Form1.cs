using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Kom2GUI
{
    public partial class Form1 : Form
    {
        static int newfilenum = 0;
        static string GetNewFileName()
        {
            newfilenum++;
            return string.Format("�� ������ {0}.kom", newfilenum);
        }

        void AddNewKom()
        {
            string newfilename = GetNewFileName();
            Crownwood.Magic.Controls.TabPage page = tabControl1.TabPages.Add(new Crownwood.Magic.Controls.TabPage(newfilename));

            KomView kom = new KomView();
            kom.Dock = DockStyle.Fill;           
            page.Controls.Add(kom);
            tabControl1.SelectedTab = page;
        }

        void OpenKom(string filename)
        {
            string title = System.IO.Path.GetFileName(filename);
            KomView kom = new KomView();
            DialogResult re = kom.LoadKom(filename);
            if(re == DialogResult.OK)
            {
                kom.Dock = DockStyle.Fill;
                Crownwood.Magic.Controls.TabPage page = tabControl1.TabPages.Add(new Crownwood.Magic.Controls.TabPage(title));
                page.Controls.Add(kom);
            }            
            else if(re == DialogResult.Abort)
            {
                MessageBox.Show(string.Format("{0} ������ ���� �� �����ϴ�.",filename), "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public Form1(string [] args)
        {
            InitializeComponent();

            foreach(string filename in args)
            {
                Kom2.NET.Komfile kom = new Kom2.NET.Komfile();
                bool iskom = kom.Open(filename);
                kom.Close();

                if (iskom)
                    OpenKom(filename);
            }
        }

        private void tabControl1_DragDrop(object sender, DragEventArgs e)
        {
            string[] filenames = (string[])e.Data.GetData(DataFormats.FileDrop, false);

            foreach(string filename in filenames)
            {
                
                if(filename.ToLower().EndsWith(".kom")==true)
                {                    
                    OpenKom(filename);
                }
                else
                {                   
                    if(tabControl1.SelectedTab != null && tabControl1.SelectedTab.Controls[0] is KomView)
                    {
                        KomView komview = tabControl1.SelectedTab.Controls[0] as KomView;
                        string key = System.IO.Path.GetFileName(filename);
                        if(komview.Contains(key))
                        {
                            if(MessageBox.Show(string.Format("{0} ������ �̹� �����մϴ�. ���� ����ðڽ��ϱ�?", key), "���", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                            {
                                komview.Remove(key);
                            }
                            else
                            {
                                break;
                            }
                        }

                        komview.AddFile(filename);
                    }                    
                }
                
            }
        }

        private void tabControl1_DragOver(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.All;
        }

        private void ������ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddNewKom();
        }

        private void ����ToolStripMenuItem_Click(object sender, EventArgs e)
        {            
            if(this.tabControl1.SelectedTab != null)
            {
                ����ToolStripMenuItem.Enabled = true;
                �ٸ��̸���������AToolStripMenuItem.Enabled = true;
            }
            else
            {
                ����ToolStripMenuItem.Enabled = false;
                �ٸ��̸���������AToolStripMenuItem.Enabled = false;
            }
        }

        private void tabControl1_ClosePressed(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedTab != null)
            {
                if(tabControl1.SelectedTab.Controls[0] is KomView)
                {
                    KomView komview = tabControl1.SelectedTab.Controls[0] as KomView;
                    if(komview.Changed == true)
                    {
                        DialogResult re = MessageBox.Show("�� ������ �ٲ�����ϴ�. �����Ͻðڽ��ϱ�?", "���", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning);
                        if(re == DialogResult.Yes)
                        {
                            string filename = komview.Filename;

                            if(komview.Filename == null || komview.Filename.Trim().Length == 0)
                            {
                                if (this.saveFileDialog1.ShowDialog() != DialogResult.OK)
                                    return;

                                filename = saveFileDialog1.FileName;
                            }

                            komview.Save(filename);
                        }
                        else if(re == DialogResult.Cancel)
                        {
                            return;
                        }
                    }

                    this.tabControl1.TabPages.Remove(this.tabControl1.SelectedTab);                                       
                }                
            }
        }

        private void ����ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(this.openFileDialog1.ShowDialog()== DialogResult.OK)
            {
                foreach (string filename in openFileDialog1.FileNames)
                {
                    OpenKom(filename);
                }                
            }
        }

        private void ����ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (tabControl1.SelectedTab.Controls[0] is KomView)
            {
                KomView komview = tabControl1.SelectedTab.Controls[0] as KomView;               
                string filename = komview.Filename;

                if (komview.Filename == null || komview.Filename.Trim().Length == 0)
                {
                    if (this.saveFileDialog1.ShowDialog() != DialogResult.OK)
                        return;

                    filename = saveFileDialog1.FileName;
                }
                komview.Save(filename);              
            } 
        }

        private void �ٸ��̸���������AToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (tabControl1.SelectedTab.Controls[0] is KomView)
            {
                KomView komview = tabControl1.SelectedTab.Controls[0] as KomView;
                string filename = komview.Filename;
               
                if (this.saveFileDialog1.ShowDialog() != DialogResult.OK)
                   return;

                filename = saveFileDialog1.FileName;
               
                komview.Save(filename);
                tabControl1.SelectedTab.Title = System.IO.Path.GetFileName(filename);
            }           
        }
    }
}