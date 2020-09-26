using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using LuaInterface;
using System.Windows.Forms;

namespace ContentSelector
{
    static class ScriptParser
    {
        static ScriptParser()
        {
            lua = new Lua();
            LuaTool.DoFile(lua, ScriptTreeManager.strMajorDir + "Enum.lua");
            m_DicEnumString = new Dictionary<int, string>();
        }

        static public bool ParseIDandValue<T>( string filename, string TableName, out Dictionary<int, T> OutPutDic)
        {
            OutPutDic = new Dictionary<int, T>();
            // 1. ID - Lua File ����
            try
            {
                LuaTool.DoFile(lua, filename);

                LuaTable ltWorld = lua.GetTable(TableName);

                foreach (DictionaryEntry entry in ltWorld)
                {
                    OutPutDic.Add((int)(double)entry.Key, (T)entry.Value);
                }

                return true;
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
        }

        // Ư�� Table�� �ִ� �����̰�, �ش� Table�� ������ Tablename = {} ���� ����� ���
        static public bool ParseIDString( string filename, out Dictionary<int, string> OutputDic, out string TableName )
        {
            // ID - ID String ����
            OutputDic = new Dictionary<int, string>();
            OutputDic.Clear();
            TableName = null;

            StreamReader sr = null;
            try
            {                
                sr = new StreamReader(filename, Encoding.UTF8, false);
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
                sr.Close();
                return false;
            }

            while(true)
            {
                string wstrStringRead;
                try
                {
                    wstrStringRead = sr.ReadLine();
                }
                catch(EndOfStreamException)
                {
                    MessageBox.Show("Table Declaration Not Found");
                    sr.Close();
                    return false;
                }
                catch( Exception e )
                {
                    MessageBox.Show(e.Message);
                    sr.Close();
                    return false;
                }
                
                // � Table�� ������ " = {} " �������� �Ǿ� �ִٰ� �����Ѵ�!
                if(wstrStringRead.IndexOf("{}") != -1)
                {
                    int SubstrEndpos = wstrStringRead.IndexOf(' ');
                    TableName = wstrStringRead.Substring(0, SubstrEndpos);
                    break;
                }
            }

            if(TableName != null)
            {                
                do
                {
                    string wstrStringRead = sr.ReadLine();
                    if(wstrStringRead.IndexOf(TableName + "[") != -1)
                    {
                        int indexStart = wstrStringRead.IndexOf('[');
                        int indexEnd = wstrStringRead.IndexOf(']', indexStart + 1);
                        string EnumString = wstrStringRead.Substring(indexStart + 1, indexEnd - indexStart);
                        EnumString = EnumString.Trim();
                       
                        try
                        {
                            int IDValue = (int)(double)lua[LuaTool.ConvertStringForm(EnumString)];
                            OutputDic.Add(IDValue, EnumString);
                            m_DicEnumString.Add(IDValue, EnumString);       // ������ �����ص��� ����
                        }
                        catch(Exception e)
                        {
                            MessageBox.Show(e.Message);
                            sr.Close();
                            return false;
                        }
                    }
                } while (sr.EndOfStream == false);
            }

            sr.Close();

            return true;
            
        }

        // ���ϳ� Ư�� Table���� ID - ID String ����
        static public bool ParseIDStringFromTable(string filename, string TableName, out Dictionary<int, string> OutputDic )
        {
            OutputDic = new Dictionary<int, string>();

            if (TableName == "" || TableName == null)
                return false;

            OutputDic.Clear();
            
            StreamReader sr = null;
            try
            {
                sr = new StreamReader(filename, Encoding.UTF8, false);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                sr.Close();
                return false;
            }

            while (true)
            {
                string wstrStringRead;
                try
                {
                    wstrStringRead = sr.ReadLine();
                }
                catch (EndOfStreamException)
                {
                    MessageBox.Show("Table Declaration Not Found");
                    sr.Close();
                    return false;
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                    sr.Close();
                    return false;
                }

                // � Table�� ������ " Tablename = " �����̶�� �����Ѵ�!!
                if (wstrStringRead.IndexOf(TableName) != -1 && wstrStringRead.IndexOf('=') != -1)
                {                    
                    break;
                }
            }

            int depth = 0;
            do
            {
                string wstrStringRead = sr.ReadLine();

                wstrStringRead = wstrStringRead.Replace("--", "");
                wstrStringRead = wstrStringRead.Trim();
                
                if (wstrStringRead.Length == 0)
                    continue;

                int length = wstrStringRead.IndexOf(',');
                if( length != -1 )
                    wstrStringRead = wstrStringRead.Substring(0, length);

                int searchindex = 0;
                while (wstrStringRead.IndexOf('{', searchindex) != -1)
                {
                    searchindex = wstrStringRead.IndexOf('{', searchindex) + 1;
                    depth++;
                }
                
                searchindex = 0;
                while (wstrStringRead.IndexOf('}', searchindex) != -1)
                {
                    searchindex = wstrStringRead.IndexOf('}', searchindex) + 1;
                    depth--;
                }


                if (wstrStringRead.IndexOf("[") != -1 && wstrStringRead.IndexOf("]") != -1)
                {
                    try
                    {
                        int IDValue = (int)(double)lua[LuaTool.ConvertStringForm(wstrStringRead)];
                        OutputDic.Add(IDValue, wstrStringRead);
                        m_DicEnumString.Add(IDValue, wstrStringRead);       // ������ �����ص��� ����
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show(e.Message);
                        sr.Close();
                        return false;
                    }
                }
            } while (depth > 0);

            sr.Close();
            return true;

        }
        
        static Lua lua;
        static public Dictionary<int, string> m_DicEnumString;

    }

    
}
