/* KGCMassFileManager.h: interface for the KGCMassFileManager class.

by KOG GC TEAM �����
===========================================================================================
									GCMassFileManager
===========================================================================================
���� : 
		- MassFile�� ����Ͽ�, Kom���� ���ο� �ִ� Ȥ�� Data���� �ȿ� �ִ��� ���ο� �������
		  �����͸� ������ �� �ֵ�����
		- ���� ������ ���� �Ҽ� ��������.


===========================================================================================
										��뿹��
===========================================================================================
KGCMassFileManager Manager;
Manager.SetDataDirectory("./Data");
if(!Manager.AddMassFile("./Data/Abta.kom")) return 0;
if(!Manager.AddMassFile("./Data/Texture.kom")) return 0;
if(!Manager.AddMassFile("./Data/Motion.kom")) return 0;

CMassFile::MASSFILE_MEMBERFILEINFO Info;
for(int i = 0; i < Hits; i++)
{	
	if( !Manager.LoadDataFile("ui_game.dds",&Info) )
		return 0;
	
	//Info�� ����Ͻÿ�
	//Info.pRealData �������� ����...Manager�� ���� ������
}

===========================================================================================
									�ӵ� ���� ��� ����Ʈ
===========================================================================================
ȯ��
OS - WindowXP
CPU - P4 3.0GHz
Mem - 512MB
HDD - 7200 RPM(Samsung)

3���� kom���Ͽ��� �˻�, �� 4457���� ������ ���ԵǾ� �־��� (10000Hits) - In Debug Mode


Mon_x07.frm : 206 ms
Abta000.dds : 217 ms
ui_game.dds : 253 ms

Average : 10000Hit Time - 225 ms

============================================================================================
						FILE*���� fseek�� �ɸ��� �ð� ����(Debug Mode)
============================================================================================

100000ȸ �ݺ��� 76ms : ���� ���ٰ� ����ɵ�...
*/
#pragma once

#define MASSFILE_VERSION		(1)
#define MASSFILE_NAMESIZE		(60)
#define MFI_REALDATA			(1)

class KGCMassFileManager  
{
	public:
		enum
		{
			FILESEEK_CUR = 0,
			FILESEEK_SET,
			FILESEEK_END,
		};

		class KGCMemoryFile
		{
			public:
				KGCMemoryFile();
				virtual ~KGCMemoryFile();

				void SetFile( char* pData,int size );
				int ReadFile(void* pDest, int ReadBytes);
				char* GetDataPointer() { return m_pCursor; }
				int FileSeek(int iPos, int Pivot);

			private:
				char* m_pData;
				char* m_pCursor;
				int  m_iSize;
				int  m_iCursorPos;
		};

		class KGCBufferManager  
		{
			public:
				KGCBufferManager();
				virtual ~KGCBufferManager();
				char* GetBuffer(int iSize);
				int   GetBufferSize() { return m_iMemorySize; }
				void  Flush();

			private:
				int m_iMemorySize;
				char* m_pData;
		};
	
        class CMassFile
		{
			private:
				struct MASSFILE_HEADER
				{
					char strVersionInfo[50];
					UINT iTotalFileNo;
					bool bCompressed;
				};
				struct MASSFILE_MEMBERFILEHEADER
				{
					char strFileName[MASSFILE_NAMESIZE];
					long RealSize;
					long CompressSize;
					long offSet;		//RealData�κп����� OffSet�Դϴ�
				};

			public:
				struct MASSFILE_MEMBERFILEINFO
				{
					char strFileName[MASSFILE_NAMESIZE];
					long size;
					char *pRealData;
					DWORD dwFlag;

					MASSFILE_MEMBERFILEINFO()
					{
						ZeroMemory( strFileName, sizeof(char) * MASSFILE_NAMESIZE );
						size			= 0;
						pRealData		= NULL;
						dwFlag			= MFI_REALDATA;
					}
				};

			public:
				CMassFile();
				virtual ~CMassFile();
				UINT GetFileCount();		//MassFile�� ��� File�� ���ԵǾ��ִ��� �˾ƺ��ϴ�.

				bool LoadMassFile( const char* strMassFile);
				bool GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
				bool GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

				static KGCBufferManager m_BufferManager1;
				static KGCBufferManager m_BufferManager2;

				//�Ʒ� �� �Լ��� MassFile�� Load���ѵ��� �ʰ� �ϳ��� �о� ���̰��� �Ҷ�..
				//������ ����ҽ� �ӵ� ������ ������ ��
				bool GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
				bool GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);


				bool SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip = false);
				bool ModifyMassFile(CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile, bool bZip = true);

				//FILE����...Help
				long GetFileSize(char* strFileName);
				void GetFileName(char* Buffer,char* strFilePath);

				bool IsCompressed(){ return m_MassFileHeader.bCompressed; }
				void SetCompressed( bool bCompressed ){ m_MassFileHeader.bCompressed = bCompressed; }

			private:
				std::vector<MASSFILE_MEMBERFILEHEADER> m_vecMemberFileHeader;
				MASSFILE_HEADER						   m_MassFileHeader;
				char								   m_strMassFileName[MAX_PATH];
				FILE*								   m_File;
				int									   m_iFileVersion;
		};

	private:
		struct SMassFileIndex		// file�̸����� MassFile�� �����Ϳ� Index�� ������ �ϱ�����
		{
			CMassFile* pMassFile;
			unsigned int Index;
		};
		
	public:
		KGCMassFileManager();
		virtual ~KGCMassFileManager();

		void AddDataDirectory(const std::string& strDirectory);
		CMassFile* AddMassFile(const std::string& strKomFileName);

		bool KGCMassFileManager::IsValidFile( std::wstring wstrFileName );
		CMassFile::MASSFILE_MEMBERFILEINFO* LoadDataFile( std::string strFileName, bool bRealData = true );
		CMassFile::MASSFILE_MEMBERFILEINFO* LoadDataFile( std::wstring wstrFileName, bool bRealData = true );
		
		KGCMemoryFile* LoadMemoryFile( std::string strFileName );
		KGCMemoryFile* LoadMemoryFile( std::wstring wstrFileName );

		int  GetTotalFileCount();

		//note : ���� ����Ǵ� ������ ����������θ� ��������
		bool SetSubDirectory();
		//note : ������ ������ �ش������� ��θ��� ����
		WCHAR* GetPullPath( WCHAR* wcFileName );
		char*  GetPullPath( char* cFileName );

	private:
		//KOM������ �ƴ� Data�������� ���� ������ �о� ���̴°��..
		bool LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut);
		//note : ���� ���� ��θ� �޾� ����������θ� ����
		bool GetSubDirectory( char* szSearchDir);

		std::map< std::string, SMassFileIndex >	m_mapMassFile;
		std::vector< CMassFile* >				m_vecMassFile;
		std::vector< string	>					m_strDataDirectory;

		static KGCBufferManager					m_BufferManager;
		CMassFile::MASSFILE_MEMBERFILEINFO		m_Info;
		KGCMemoryFile							m_MemoryFile;
};

