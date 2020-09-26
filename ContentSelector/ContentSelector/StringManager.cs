using System;
using System.Collections.Generic;

using System.IO;
using System.Text;

namespace ContentSelector
{
    static class StringManager
    {
        static public bool InitializeStringTable(string wstrGeneralStringFileName, string wstrScriptStringFileName)
        {
            return LoadStringTable(wstrGeneralStringFileName);
        }

        static public string GetGeneralString(int iID)
        {
            int iTableSize = m_vecGeneralStringTable.Count;

            // STR_ID_EMPTY ������ -1�� �Ѿ�� ���� �ִ�.
            if (iID < 0 || iID >= iTableSize)
            {
                return m_wstrEmpty;
            }

            return m_vecGeneralStringTable[iID];
        }
        //public string GetScriptString( int iID );
        //public string GetReplacedString( int iID, char* szParam, ... );

        static bool LoadStringTable(string wstrFileName)
        {
            m_vecGeneralStringTable.Clear();

            // �����ڵ� ������ �����Ƿ� ���̳ʸ� ���� �����Ѵ�.
            StreamReader sr;
            sr = new StreamReader(wstrFileName, Encoding.Unicode, false);

            //             char[] buf = new char[2];
            //             
            //             // UTF-16LE�� BOM( Byte Order Mark )�� �پ� �Ѵ´�.
            //             sr.Read(buf, 0, 2);

            // string table ���� �� ���� ������ ������ ���� ����̴�.
            // [index]\t[string]\r\n
            const int ciMaxStringLength = 2048;
            char[] wszStringRead = new char[ciMaxStringLength];
            int iIndex = 0;

            do
            {
                string wstrStringRead;
                wstrStringRead = sr.ReadLine();

                // �ִ� ��Ʈ�� ���̸�ŭ �о��ٸ� ������ �ִ��� �ǽ��غ��� �Ѵ�.
                //ASSERT(wstrStringRead.Length < (ciMaxStringLength - 1));

                //09. 07. 03 ������  XOR��ȣȭ �� ���� ��ȣȭ
                wstrStringRead = wstrStringRead.Replace("\r\n", "");
                wszStringRead = wstrStringRead.ToCharArray();

                short nKey = 16;
                int nSize = wstrStringRead.Length;

                for (int i = 0; i < nSize; i++)
                {
                    wszStringRead[i] = (char)(wstrStringRead[i] ^ nKey);
                }
                wstrStringRead = new string(wszStringRead);


                // tab �� �о index�� �����س���.
                int iTabPos = wstrStringRead.IndexOf("\t");
                string wstrIndex = wstrStringRead.Substring(0, iTabPos);
                int iIndexRead = int.Parse(wstrIndex);

                string wstrContent = "";
                // ������ index�� ����� �����Ǿ� �ִ��� �˻��Ѵ�.
                //ASSERT(iIndexRead == iIndex);
                if (iIndexRead == iIndex)
                {
                    // tab �� ã�� ��쿡�� �� ���� ������ �о�� �Ѵ�.
                    if (iTabPos != -1)
                    {
                        //09. 07. 03 ������  �̸� ���ֱ� ������ ����

                        //ASSERT(iTabPos < wstrStringRead.size());
                        // tab �ڿ� �ִ� �͵鸸 �д´�.
                        if ((iTabPos + 1) < wstrStringRead.Length)
                        {
                            wstrContent = wstrStringRead.Substring(iTabPos + 1);
                        }
                    }
                }
                // \n ���� ǥ���� �κп� ������ �ٹٲ� ���ش�.
                wstrContent.Replace("\\n", "\r\n");

                m_vecGeneralStringTable.Add(wstrContent);
                iIndex++;
            } while (sr.EndOfStream == false);

            sr.Close();

            return true;
        }

        static List<string> m_vecGeneralStringTable = new List<string>();
        static List<string> m_vecScriptStringTable = new List<string>();
        static string m_wstrEmpty = "";
    };
}