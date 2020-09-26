using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using System.Reflection; 


using Outlook = Microsoft.Office.Interop.Outlook;


namespace CrashReport
{	
	/// <summary>
	/// Form1�� ���� ��� �����Դϴ�.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		private System.ComponentModel.Container components = null;

		Outlook.Application oApp;
		Outlook.NameSpace oNS;
		//Outlook.MAPIFolder oRoot;

		public Form1()
		{			
			// Windows Form �����̳� ������ �ʿ��մϴ�.		
			InitializeComponent();
			
			// TODO: InitializeComponent�� ȣ���� ���� ������ �ڵ带 �߰��մϴ�.

			oApp = new Outlook.Application();
			oNS = oApp.GetNamespace( "mapi" );
			oNS.Logon( Missing.Value, Missing.Value, false, true );
		}


		void AddTree( TreeNodeCollection nodes, Outlook.Folders folders )
		{
			for( int i = 1; i <= folders.Count; ++i )
			{
				TreeNode child = new OFTreeNode( folders[i].Name, folders[i] );
				AddTree( child.Nodes, folders[i].Folders );
				nodes.Add( child );
			}
		}

		public void UpdateOulookFolder()
		{
			this.treeView1.Nodes.Clear();

			//TreeNode root = new OFTreeNode(oRoot.Name,oRoot);
			
			AddTree( treeView1.Nodes, oNS.Folders );
			//treeView1.Nodes.Add(root);

			treeView1.ExpandAll();
		}

		public Outlook.MAPIFolder GetSelectFolder()
		{
			return ((OFTreeNode)treeView1.SelectedNode).folder;
		}

		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form �����̳ʿ��� ������ �ڵ�
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		private void InitializeComponent()
		{
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.button1 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// treeView1
			// 
			this.treeView1.ImageIndex = -1;
			this.treeView1.Location = new System.Drawing.Point(0, 0);
			this.treeView1.Name = "treeView1";
			this.treeView1.SelectedImageIndex = -1;
			this.treeView1.Size = new System.Drawing.Size(320, 352);
			this.treeView1.TabIndex = 0;
			// 
			// button1
			// 
			this.button1.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.button1.Location = new System.Drawing.Point(160, 360);
			this.button1.Name = "button1";
			this.button1.TabIndex = 1;
			this.button1.Text = "OK";
			// 
			// button2
			// 
			this.button2.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.button2.Location = new System.Drawing.Point(240, 360);
			this.button2.Name = "button2";
			this.button2.TabIndex = 2;
			this.button2.Text = "Cancel";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(322, 391);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.treeView1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Select Folder";
			this.ResumeLayout(false);

		}
		#endregion

		private void treeView1_DoubleClick( object sender, System.EventArgs e )
		{
			this.DialogResult =System.Windows.Forms.DialogResult.OK;
		}


		class OFTreeNode:TreeNode
		{
			public OFTreeNode(string name,Outlook.MAPIFolder folder):base(name)		
			{		
				this.folder = folder;
			}
			public Outlook.MAPIFolder folder;
		}
	}
}
