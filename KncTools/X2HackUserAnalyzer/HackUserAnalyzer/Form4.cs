using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace HackUserReport
{
	/// <summary>
	/// Form4�� ���� ��� �����Դϴ�.
	/// </summary>
	public class Form4 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ProgressBar progressBar1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		/// <summary>
		/// �ʼ� �����̳� �����Դϴ�.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form4()
		{			
			// Windows Form �����̳� ������ �ʿ��մϴ�.		
			InitializeComponent();

			// TODO: InitializeComponent�� ȣ���� ���� ������ �ڵ带 �߰��մϴ�.
		}

		/// <summary>
		/// ��� ���� ��� ���ҽ��� �����մϴ�.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}


		System.Threading.Thread thread;
		System.Collections.Hashtable dmp = new System.Collections.Hashtable();
		System.Collections.Hashtable ip = new System.Collections.Hashtable();
		System.Collections.Hashtable uid = new System.Collections.Hashtable();
		System.Collections.Hashtable cpu = new System.Collections.Hashtable();
		System.Collections.Hashtable ram = new System.Collections.Hashtable();
		System.Collections.Hashtable vga = new System.Collections.Hashtable();

		//{{ dmlee 2008.05.26 OS ���
		System.Collections.Hashtable htOS = new System.Collections.Hashtable();
		//}} dmlee 2008.05.26 OS ���

		System.Collections.ArrayList screenShot = new System.Collections.ArrayList();


		System.Collections.Hashtable Story = new System.Collections.Hashtable();
		System.Collections.Hashtable GameState = new System.Collections.Hashtable();
		System.Collections.Hashtable GameModeCategory = new System.Collections.Hashtable();
		System.Collections.Hashtable GameMode = new System.Collections.Hashtable();
		System.Collections.Hashtable SubGameMode = new System.Collections.Hashtable();
		

				

		void AddFile( string key, string filename )
		{	
			if( dmp.Contains( key ) )
			{
				((ArrayList)dmp[key]).Add( filename );
			}
			else
			{
				System.Collections.ArrayList al = new ArrayList();
				al.Add( filename );
				dmp.Add( key, al );
			}
		}

		void AddHashCount( Hashtable hash, string key )
		{
			if( hash.Contains( key ) )
			{
				hash[key] = ((int)hash[key]) + 1;
			}
			else
			{
				hash.Add( key, 1 );
			}
		}

		// ���� �α� �м�
		void ParsDmpLog( string filename )
		{
			if( System.IO.File.Exists( filename ) == false )
				return;
			
			System.IO.StreamReader reader = new System.IO.StreamReader( filename );
			string data = reader.ReadToEnd();
			reader.Close();
            
            int s = data.IndexOf( "[" );
            string HackUser = data.Substring( s + 1, data.IndexOf( "]", s ) - s - 1 );						
			AddFile( HackUser, filename );
		}

		void FindStrAndPushHashR( Hashtable hash, string data, string startstr, string endstr )
		{
			int s = 0, e = 0;

			while( data.IndexOf( startstr ) > 0 )
			{
				s = data.IndexOf( startstr );
				e = data.IndexOf( endstr, s );

				if( s < e )
					AddHashCount( hash, data.Substring( s + startstr.Length, e - s - startstr.Length ) );

				data = data.Remove( s, startstr.Length );
			}
		}

		void FindStrAndPushHash( Hashtable hash, string data, string startstr, string endstr )
		{
			int s = 0, e = 0;
			
			s =data.IndexOf(startstr);

			if( s > 0 )
			{
				e = data.IndexOf( endstr, s );

				if( s < e )
					AddHashCount( hash, data.Substring( s + startstr.Length, e - s - startstr.Length ) );

				data = data.Remove( s, startstr.Length );
			}
		}

		// ���� ���� �м�
		void ParseByDmp( ArrayList list )
		{			
            foreach ( string filename in list )
            {	
				string dir = System.IO.Path.GetDirectoryName( filename );

				System.IO.StreamReader reader = new System.IO.StreamReader( dir+"\\HardwareInfo.txt", System.Text.Encoding.ASCII );
				string data = reader.ReadToEnd();
				reader.Close();

				FindStrAndPushHash( cpu, data, 		"CPU                      ", "\r" );
				FindStrAndPushHash( ram, data, 		"RAM                      ", "\r" );
				FindStrAndPushHash( vga, data, 		"??????????               ", "\r" );		// �ѱ��� ������ �׷���. "�׷��� ī��" �κ��̴�

				//{{ dmlee 2008.05.26 OS ���
				FindStrAndPushHash( htOS, data, 	"OS                       ", "\r" );
				//}} dmlee 2008.05.26 OS ���
				
				screenShot.Add( Application.StartupPath + "\\" + dir + "\\HackUser_ScreenShot.jpg" );


				// TO DO : ���� ���� �м� �ڵ� ���⼭ ���� �������..
				
//				reader = new System.IO.StreamReader(dir+"\\HackUserlog.txt");
//				data = reader.ReadToEnd();
//				reader.Close();
//
//
//				FindStrAndPushHashR(uid,data,"Uid :","\r");
//				FindStrAndPushHash(Story,data,"Story :","\r");
//				FindStrAndPushHash(GameState,data,"GameState :","\r");
//				FindStrAndPushHash(GameModeCategory,data,"GameModeCategory :","\r");
//				FindStrAndPushHash(GameMode,data,"GameMode :","\r");
//				FindStrAndPushHash(SubGameMode,data,"SubGameMode :","\r");				


			}
		}

		void WriteData( System.IO.StreamWriter writer, Hashtable hash, int total )
		{
			while( hash.Count != 0 )
			{
				int max = 0;
				string key = "";
				foreach( System.Collections.DictionaryEntry d in hash )
				{
					if( (int)d.Value > max )
					{
						key = (string)d.Key;
						max = (int)d.Value;
					}
				}	
				float freq = max/(float)total*100.0f;
				writer.WriteLine( "</p><p> <font size=4 color=blue><b>{0:0.##}%</b></font>({1}/{2})", freq,max,total );
				
				try 
				{					
					writer.WriteLine( " <b> - " + key + " </b>" );
				}
				catch (ArgumentException)
				{
					writer.WriteLine( " <b> - ????? </b>" );
				}
								
				hash.Remove( key );
			}
		}


		
		void WriteData( System.IO.StreamWriter writer, ArrayList array, int total )
		{
			foreach ( Object obj in array )
			{
				writer.WriteLine( "<br> <img src=\"{0}\" width=800 height=600> <br> ", obj );
			}
		}

        bool CheckHackList(string filename)
        {
            System.Collections.ArrayList hackList = new ArrayList();
            string startPath = System.Windows.Forms.Application.StartupPath + "\\HackPGM.txt";
            System.IO.StreamReader readerOpt = new System.IO.StreamReader( startPath, System.Text.Encoding.Default );

            hackList.Clear();
            while(readerOpt.Peek() > 0)
            {
                string hackPgm = readerOpt.ReadLine();
                hackList.Add(hackPgm);
            }

            readerOpt.Close();
            

            System.IO.StreamReader reader = new System.IO.StreamReader( filename, System.Text.Encoding.Default );

            bool bFind = false;

            while(reader.Peek() > 0)
            {
                string strData = reader.ReadLine();
             
                bFind = false;
                foreach(string strProc in hackList)
                {
                    if(strData.Equals(strProc) == true || strData.IndexOf(strProc) >= 0)
                    {
                        bFind = true;
                        return true;
                    }                    
                }                
            }

            reader.Close();

            return false;
        }

		// ������ ������ �м� �� ��� ó��
		void DataParse()
		{
			lock( thread )
			{
				System.Threading.Thread.Sleep(100);

				string [] dirs = new string[Form2.parselist.Count];
				int i = 0;

				foreach ( System.Collections.DictionaryEntry d in Form2.parselist )
				{
					dirs[i]="HackUser\\"+d.Key;
					i++;
				}


				//string [] dirs = System.IO.Directory.GetDirectories("HackUser\\","*");
				foreach ( string dir in dirs )
				{
					//string []str = Form1.FolderPath.Split('\\');
					string []str = dir.Split('\\');
					string version = str[str.Length-1];

					string [] dirs2 = System.IO.Directory.GetDirectories( dir, "*" );

				
					progressBar1.Maximum = dirs2.Length;
					progressBar1.Minimum = 0;
					progressBar1.Value	 = 0;


					foreach( string dir2 in dirs2 )
					{
						label3.Text = dir2+"\\dmp.html";

						// 1. ���� �α� �м�
						ParsDmpLog( dir2 + "\\dmp.html");
						progressBar1.Value++;
					}

				    int nFindHack = 0;
					System.IO.StreamWriter writer = new System.IO.StreamWriter( "report\\"+version+"_dmp.html" );
					writer.WriteLine( "<body link=darkgreen vlink=lightgreen><font size=2><p> <b> Total {0} HackUser files</b>", dirs2.Length );

					System.Collections.ArrayList fileList = new ArrayList();
                    fileList.Clear();

					while( dmp.Count != 0 )
					{
						int max = 0;
						string key = "";

						foreach( System.Collections.DictionaryEntry d in dmp )
						{
							if(((ArrayList)d.Value).Count>max)
							{
								key = (string)d.Key;
								max = (int)((ArrayList)d.Value).Count;
							}
						}
						
                        writer.WriteLine( "</p><p> <font size=3>" );                        

                        foreach ( string filename in (ArrayList)dmp[key] )
						{							
                            bool bFindHack = CheckHackList(System.Windows.Forms.Application.StartupPath+"\\"+filename);

                            if(bFindHack)
                            {
                                writer.WriteLine( "[<a href='..\\"+filename+"'>"+filename.Split('\\')[2]+"</a>] <font size=2 color=red>Find Hack!!</font>" );
                                ++nFindHack;
                            }							    
                            else
                            {
                                writer.WriteLine( "[<a href='..\\"+filename+"'>"+filename.Split('\\')[2]+"</a>]" );
                            }

                            System.IO.StreamReader reader = new System.IO.StreamReader( System.Windows.Forms.Application.StartupPath+"\\"+filename, System.Text.Encoding.Default );
                                                        
                            fileList.Add(reader);
						}
						writer.WriteLine( "</font>" );

						dmp.Remove(key);
					}
			
                    writer.WriteLine( "<body link=darkgreen vlink=lightgreen><font size=2 color = red><p> <b> Find {0} HackUser!</b>", nFindHack );
/* 
                    System.IO.StreamWriter writerXls = new System.IO.StreamWriter( "report\\"+version+".xls" );
                    for(int index=0; i<1024; ++i)
                    {
                        foreach(System.IO.StreamReader reader in fileList)
                        {
                            
                            if(reader.Peek() > 0)
                            {
                                string strData = reader.ReadLine();

                                if(strData.Equals("") == true)
                                    writerXls.Write( "\t" );
                                else
                                {
                                    
                                    strData = strData.Replace("<body link=darkgreen vlink=lightgreen><font size=4></font><font size=3><pre>","");
                                    strData = strData.Replace("</pre>", "");

                                    if(strData.IndexOf("color=red") >= 0)
                                    {
                                        strData = strData.Replace("<font size=2 color=black>","");
                                        strData = strData.Replace("<font size=2 color=red>","");
                                        strData = strData.Replace("</font>", "");
                                        strData = "<body link=darkgreen vlink=lightgreen><font size=4></font><font size=3><pre><font size=2 color=red>" + strData + "</font></pre></body>";
                                    }
                                    else
                                    {
                                        strData = strData.Replace("<font size=2 color=black>","");
                                        strData = strData.Replace("<font size=2 color=red>","");
                                        strData = strData.Replace("</font>", "");
                                    }
                                    
                                    writerXls.Write( strData + "\t" );
                                }
                            }                                
                        }
                        writerXls.Write("\r\n");
                    }
                    writerXls.Close();


                    foreach(System.IO.StreamReader reader in fileList)
                    {
                        reader.Close();
                    }
*/
                    writer.Close();
				}
				thread = null;
			}
			this.Close();
		}

		public void DataParsing()
		{				
			thread = new System.Threading.Thread( new System.Threading.ThreadStart( DataParse ) );
			thread.Start();
		}

		#region Windows Form �����̳ʿ��� ������ �ڵ�
		/// <summary>
		/// �����̳� ������ �ʿ��� �޼����Դϴ�.
		/// �� �޼����� ������ �ڵ� ������� �������� ���ʽÿ�.
		/// </summary>
		private void InitializeComponent()
		{
			this.progressBar1 = new System.Windows.Forms.ProgressBar();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// progressBar1
			// 
			this.progressBar1.Location = new System.Drawing.Point(40, 48);
			this.progressBar1.Name = "progressBar1";
			this.progressBar1.Size = new System.Drawing.Size(448, 23);
			this.progressBar1.TabIndex = 0;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(40, 16);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(70, 17);
			this.label2.TabIndex = 5;
			this.label2.Text = "������ �Ľ�";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(216, 16);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(272, 17);
			this.label3.TabIndex = 6;
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// Form4
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(522, 95);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.progressBar1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "Form4";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "������ �Ľ�";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
