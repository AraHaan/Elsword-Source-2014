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

    �� ���ҵ� ��׶��� �ε� ���� �۾����� - Ods306�� ������, jintaeks on 2008-10-18, 11:38

    @date   jintaeks on 2008-10-16, 16:42
            - ���ҵ��� �ε��� ��׶���� �����ϱ� ���� �۾��� �����ϴ�.
            - �ߺ��Ǵ� �Լ����� �����Ͽ� �б� ����, ������ �����ϰ� �ٲٴ�.
            - thread handle�� dependent�ϵ��� ���۸� ����ϴ� ����� ������ �����ϴ�.
    @date   jintaeks on 2008-10-18, 11:36
            - thread safe�� ���� �Ŵ��� ���� �Ϸ�
                -- KGCMassFileBufferManager�� ������ Ŭ���� ���Ϸ� ����
                -- �� �����忡�� �ִ� 5���� ���۰� ���ǰ� ������ Ȯ��
            - CMassFile::MASSFILE_MEMBERFILEINFO m_Info�� ������� �ʵ��� ����
                -- multi threadȯ�濡�� ������ thread�� ������ MASSFILE_MEMBERFILEINFO�� �򵵷�
                    ����. MASSFILE_MEMBERFILEINFO�� �����ϴ� ������ ����ϳ� ������
                    �������̽��� �����ϴ� ���� �����ϱ� ���� MASSFILE_MEMBERFILEINFO_POINTER��
                    ����ϵ��� �ϴ� -> �۾��� �Ϸ�Ǹ� MASSFILE_MEMBERFILEINFO���� ����ϵ���
                    �ٲ� ��.
*/

#pragma once

#include "KGCMassFileBufferManager.h"
#include "AssertTrace.h"

#define MASSFILE_VERSION		(1)
#define MASSFILE_NAMESIZE		(60)
#define MFI_REALDATA			(1)
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
#define MFI_COMPRESSEDDATA      (2)
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
#define MFI_LOCALFILE           (4)
#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE


#define  MASS_FILE_MANAGER_MAX_NUM_THREADS	2

class KGCMassFileManager  
{
	public:
		enum
		{
			FILESEEK_CUR = 0,
			FILESEEK_SET,
			FILESEEK_END,
		};

#ifdef MASSFILE_MAPPING_FUNCTION
		struct MASSFILE_MAPPING_TIME
		{
			CTime tStartTime;
			CTime tEndTime;
		};

		bool MassFileMapping();
#ifdef WCHAR_DIR
		bool ExternFileMapping( wstring& strExternFileName );
#else WCHAR_DIR
		bool ExternFileMapping(string& strExternFileName);
#endif WCHAR_DIR

		bool AddMappingList_LUA(int nIndex, const char* strDestMappingFileName, const char* strSrcMappingFileName);
		bool AddMappingTime_LUA(int nIndex, const char* szStartDate, const char* szEndDate);
		bool SetWebAddress_LUA(const char* szAddress);

		int GetUsableMappingIndex();

		std::map<int, std::map<string, string>> m_mapMassFileMappingList;
		std::map<int, MASSFILE_MAPPING_TIME> m_mapMassFileMappingTime;
#endif MASSFILE_MAPPING_FUNCTION
		//class KGCMemoryFile
		//{
		//	public:
		//		KGCMemoryFile();
		//		~KGCMemoryFile();

		//		void SetFile( char* pData,int size );
		//		int ReadFile(void* pDest, int ReadBytes);
		//		char* GetDataPointer() { return m_pCursor; }
		//		int FileSeek(int iPos, int Pivot);

		//	private:
		//		char* m_pData;
		//		char* m_pCursor;
		//		int  m_iSize;
		//		int  m_iCursorPos;
		//};

//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
////{{AFX:
//		class KGCBufferManager  
//		{
//		public:
//			struct Buffer
//			{
//				bool	m_bInUse;
//				int		m_iMemorySize;
//				char*	m_pData;
//
//				Buffer()
//				{
//					m_bInUse = false;
//					m_iMemorySize = 1024;
//					m_pData = new char[m_iMemorySize];
//				}
//
//				~Buffer()
//				{
//					flush();
//				}
//
//				void flush()
//				{
//					if( m_pData != NULL )
//					{
//						delete[] m_pData;
//						m_pData = NULL;
//					}
//				}
//
//				char* GetMemory( int iSize, bool bClear = false )
//				{
//					if( m_iMemorySize < iSize )
//					{
//						if( m_pData != NULL )
//						{		
//							delete []m_pData;
//							m_pData = NULL;
//						}
//
//						while( (m_iMemorySize *= 2) < iSize )
//						{
//						}
//
//						m_pData = new char[m_iMemorySize];
//					}
//
//					if( true == bClear )
//					{
//						ZeroMemory( m_pData, sizeof(char) * m_iMemorySize );
//					}
//
//					return m_pData;
//				}
//			};
//
//		public:
//			KGCBufferManager();
//			~KGCBufferManager();
//
//			Buffer* GetCurrBuffer() 
//			{
//				CSLock locker( m_csBufferManager );
//				if( m_iCurrBufferIndex < 0 || m_iCurrBufferIndex > m_iBufferCount-1 )
//					return NULL;
//
//				return &m_aBuffer[m_iCurrBufferIndex];
//			}
//			char* GetBuffer(int iSize, bool bForceCurrent = false );
//			int GetBufferSize()		// ���� ���� ����� return 
//			{
//				CSLock locker( m_csBufferManager );
//				if( m_iCurrBufferIndex < 0 || m_iCurrBufferIndex > m_iBufferCount-1 )
//					return 0;
//
//				return m_aBuffer[m_iCurrBufferIndex].m_iMemorySize;
//			}
//
//			void  Flush();
//
//		private:
//			int m_iCurrBufferIndex;
//			int m_iBufferCount;			// m_aBuffer�� ũ��
//			
//			Buffer m_aBuffer[5];
//
//			CRITICAL_SECTION m_csBufferManager;
//		};//class KGCBufferManager  
////}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

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
#ifdef WCHAR_DIR
					WCHAR strFileName[MASSFILE_NAMESIZE];
#else WCHAR_DIR
					char strFileName[MASSFILE_NAMESIZE];
#endif WCHAR_DIR
					long RealSize;
					long CompressSize;
					long offSet;		//RealData�κп����� OffSet�Դϴ�

					MASSFILE_MEMBERFILEHEADER()
					{
						ZeroMemory( strFileName, sizeof(char) * MASSFILE_NAMESIZE );
						RealSize = 0;
						CompressSize = 0;
						offSet = 0;
					}
				};

			public:
				
				struct MASSFILE_MEMBERFILEINFO
				{
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
                    KGCMassFileBufferPtr    pBuffer;
//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//                    //{{AFX
//					KGCBufferManager::Buffer* pBuffer;
//                    //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16

#ifdef WCHAR_DIR
					WCHAR strFileName[MASSFILE_NAMESIZE];
#else WCHAR_DIR
					char strFileName[MASSFILE_NAMESIZE];
#endif WCHAR_DIR

					long size;
					char *pRealData;
					DWORD dwFlag;
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
                    long compressedSize;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS

					MASSFILE_MEMBERFILEINFO()
					{

						ZeroMemory( strFileName, sizeof(char) * MASSFILE_NAMESIZE );

						size			= 0;
						pRealData		= NULL;
						dwFlag			= MFI_REALDATA;
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
                        compressedSize = 0;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS

//#ifndef BACKGROUND_LOADING_TEST // 2008-10-16
//                        //{{AFX
//						pBuffer			= NULL;
//                        //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16
					}//MASSFILE_MEMBERFILEINFO()
					~MASSFILE_MEMBERFILEINFO()
                    {
                        //TRACEA( "%s\r\n", __FUNCTION__ );
                    }//~MASSFILE_MEMBERFILEINFO()
				};



//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17

                struct MASSFILE_MEMBERFILEINFO_POINTER : public MASSFILE_MEMBERFILEINFO
				{
                    /// constructor.
                    MASSFILE_MEMBERFILEINFO_POINTER(){}

                    /// copy constructor.
                    MASSFILE_MEMBERFILEINFO_POINTER(const MASSFILE_MEMBERFILEINFO_POINTER& rhs_)
                    {
                        operator=( rhs_ );
                    }//MASSFILE_MEMBERFILEINFO_POINTER()

                    MASSFILE_MEMBERFILEINFO_POINTER& operator=(const MASSFILE_MEMBERFILEINFO_POINTER& rhs_)
                    {
                        pBuffer     = rhs_.pBuffer;
#ifdef WCHAR_DIR
						StringCchCopyW( strFileName, ARRAY_SIZE(strFileName), rhs_.strFileName );
#else WCHAR_DIR
					    StringCchCopyA( strFileName, ARRAY_SIZE(strFileName), rhs_.strFileName );
#endif WCHAR_DIR
					    size        = rhs_.size;
					    pRealData   = rhs_.pRealData;
					    dwFlag      = rhs_.dwFlag;
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
                        compressedSize = rhs_.compressedSize;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
                        return *this;
                    }//operator=()

					//MASSFILE_MEMBERFILEINFO_POINTER& operator=(MASSFILE_MEMBERFILEINFO* ptr_) 
					//{
     //                   if ( ptr_ )
     //                   {
     //                       pBuffer     = ptr_->pBuffer;
					//        StringCchCopyA( strFileName, ARRAY_SIZE(strFileName), ptr_->strFileName );
					//        size        = ptr_->size;
					//        pRealData   = ptr_->pRealData;
					//        dwFlag      = ptr_->dwFlag;
     //                   }
     //                   else
     //                   {
     //                       //pBuffer.reset();
					//        strFileName[0]  = NULL;
					//        size            = 0;
					//        pRealData       = NULL;
					//        dwFlag          = 0L;
     //                   }//if
					//	return *this;
					//}

					MASSFILE_MEMBERFILEINFO* operator->()
					{
						return this;
					}//operator->()

					operator const MASSFILE_MEMBERFILEINFO*() const
					{
						return this;
					}//operator const MASSFILE_MEMBERFILEINFO*()

					bool operator!()
					{
						return pRealData == NULL;
					}//operator!()

					bool operator==(const MASSFILE_MEMBERFILEINFO* ptr)
					{
                        if ( ptr )
						    return pRealData == ptr->pRealData;
                        else
                            return pRealData == NULL;
					}//operator==()

					bool operator!=(const MASSFILE_MEMBERFILEINFO* ptr)
					{
                        if ( ptr )
						    return pRealData != ptr->pRealData;
                        else
                            return pRealData != NULL;
					}//operator!=()
				};//struct MASSFILE_MEMBERFILEINFO_POINTER

				typedef MASSFILE_MEMBERFILEINFO_POINTER     MASSFILE_MEMBERFILEINFO_PTR;

//#else // BACKGROUND_LOADING_TEST // 2008-10-17
//
//                typedef MASSFILE_MEMBERFILEINFO*    MASSFILE_MEMBERFILEINFO_PTR;
//
//                //{{AFX
//				struct MASSFILE_MEMBERFILEINFO_POINTER
//				{
//					KGCBufferManager::Buffer* pBuffer;
//					MASSFILE_MEMBERFILEINFO* m_pInfo;	
//
//					CRITICAL_SECTION m_csMASSFILE_MEMBERFILEINFO_POINTER;
//
//					MASSFILE_MEMBERFILEINFO_POINTER()
//					{
//						::InitializeCriticalSection( &m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						pBuffer = NULL;
//						m_pInfo = NULL;
//					}
//
//					~MASSFILE_MEMBERFILEINFO_POINTER()
//					{
//						UnRef();
//
//						::DeleteCriticalSection( &m_csMASSFILE_MEMBERFILEINFO_POINTER );
//					}
//
//					MASSFILE_MEMBERFILEINFO_POINTER& operator= (MASSFILE_MEMBERFILEINFO* ptr) 
//					{
//						CSLock locker( m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						UnRef();
//						m_pInfo = ptr;
//						if( NULL != ptr )
//							pBuffer = ptr->pBuffer;
//						return *this;
//					}
//
//					void UnRef()
//					{
//						CSLock locker( m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						if( NULL != pBuffer )
//						{
//							pBuffer->m_bInUse = false;
//						}
//					}
//
//					MASSFILE_MEMBERFILEINFO* operator ->()
//					{
//						CSLock locker( m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						return m_pInfo;
//					}
//
//					operator MASSFILE_MEMBERFILEINFO* () const
//					{
//						
//
//						return m_pInfo;
//					}
//
//					bool operator !()
//					{
//						CSLock locker( m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						return m_pInfo == NULL;
//					}
//					bool operator ==(const MASSFILE_MEMBERFILEINFO* ptr)
//					{
//						CSLock locker( m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						return m_pInfo == ptr;
//					}
//					bool operator !=(const MASSFILE_MEMBERFILEINFO* ptr)
//					{
//						CSLock locker( m_csMASSFILE_MEMBERFILEINFO_POINTER );
//
//						return m_pInfo != ptr;
//					}
//				};
//                //}}AFX
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

			public:
				CMassFile();
				~CMassFile();
				UINT GetFileCount();		//MassFile�� ��� File�� ���ԵǾ��ִ��� �˾ƺ��ϴ�.

#ifdef WCHAR_DIR
				bool LoadMassFile( const WCHAR* strMassFile );
#else WCHAR_DIR
				bool LoadMassFile( const char* strMassFile);
#endif WCHAR_DIR

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
                //{{ seojt // 2008-10-16, 15:15
                // GetMemberFile()�� ���������� ȣ���ϴ� �Լ��̴�.
                // ���������� ȣ���ϴ� �Լ��� ���ʸ� ���� �Լ� �̸��տ� underscore(_)�� ���δ�.
                // - jintaeks on 2008-10-16, 15:16
                bool _GetMemberFile( KGCMassFileManager& kManager, const MASSFILE_MEMBERFILEHEADER& MEMBERFILEHEADER_, MASSFILE_MEMBERFILEINFO* pOut_);
                //}} seojt // 2008-10-16, 15:15
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16

#ifdef WCHAR_DIR
				bool GetMemberFile( KGCMassFileManager& kManager, const WCHAR* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
#else WCHAR_DIR
				bool GetMemberFile( KGCMassFileManager& kManager, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
#endif WCHAR_DIR
				bool GetMemberFile( KGCMassFileManager& kManager, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
//                //{{AFX
//				static KGCBufferManager m_BufferManager1;
//				static KGCBufferManager m_BufferManager2;
//                //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

/*
#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
                //{{ seojt // 2008-10-16, 15:39
                // GetMemberFile()�� ���������� ȣ���ϴ� �Լ��̴�.
                bool _GetMemberFileFromKom(HANDLE hFile_, MASSFILE_HEADER& Header, MASSFILE_MEMBERFILEHEADER& MEMBERFILEHEADER_, MASSFILE_MEMBERFILEINFO* pOut_);
                //}} seojt // 2008-10-16, 15:39
#endif // BACKGROUND_LOADING_TEST // 2008-10-16

				//�Ʒ� �� �Լ��� MassFile�� Load���ѵ��� �ʰ� �ϳ��� �о� ���̰��� �Ҷ�..
				//������ ����ҽ� �ӵ� ������ ������ ��
				bool GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
				bool GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);
*/

#ifdef WCHAR_DIR
				bool SaveMassFile(WCHAR* strFileName, std::vector<WCHAR*> *pVecFile, bool bZip = false);
				bool ModifyMassFile( KGCMassFileManager& kManager, CMassFile* pMassFile,WCHAR* strFileName, std::vector<WCHAR*> *pVecFile, bool bZip = true);

				//FILE����...Help
				long GetFileSize(WCHAR* strFileName);
				void GetFileName(WCHAR* Buffer, const WCHAR* strFilePath);
#else WCHAR_DIR
				bool SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip = false);
				bool ModifyMassFile( KGCMassFileManager& kManager, CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile, bool bZip = true);

				//FILE����...Help
				long GetFileSize(char* strFileName);
				void GetFileName(char* Buffer, const char* strFilePath);
#endif WCHAR_DIR

				bool IsCompressed(){ return m_MassFileHeader.bCompressed; }
				void SetCompressed( bool bCompressed ){ m_MassFileHeader.bCompressed = bCompressed; }

#ifdef WCHAR_DIR
				__forceinline const WCHAR* GetMemberFileName( const UINT uiIndex_ ) const
				{
					return m_vecMemberFileHeader[uiIndex_].strFileName;
				}
#else WCHAR_DIR
				__forceinline const char* GetMemberFileName( const UINT uiIndex_ ) const
				{
					return m_vecMemberFileHeader[uiIndex_].strFileName;
				}				
#endif WCHAR_DIR

			private:
				std::vector<MASSFILE_MEMBERFILEHEADER> m_vecMemberFileHeader;
				MASSFILE_HEADER						   m_MassFileHeader;
#ifdef WCHAR_DIR
				WCHAR								   m_strMassFileName[MAX_PATH];
#else WCHAR_DIR
				char								   m_strMassFileName[MAX_PATH];
#endif WCHAR_DIR
				
//{{ robobeg : 2011-08-16
//              HANDLE                                 m_hFile;
//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
				HANDLE									m_ahFilePerThread[MASS_FILE_MANAGER_MAX_NUM_THREADS];
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO				
//				HANDLE								   m_hFileForegroundSync; 
//				HANDLE                                 m_hFileBackgroundAsync;
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//}} robobeg : 2011-08-16
				int									   m_iFileVersion;
				//CRITICAL_SECTION					   m_csMassFile;
		};//class CMassFile

	private:
		struct SMassFileIndex		// file�̸����� MassFile�� �����Ϳ� Index�� ������ �ϱ�����
		{
			CMassFile* pMassFile;
			unsigned int Index;
		};
		
	public:
		KGCMassFileManager();
		~KGCMassFileManager();

		void AddDataDirectory(const std::wstring& wstrDirectory, bool bInsertFront = false );
		void AddDataDirectory(const std::string& strDirectory, bool bInsertFront = false );

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
		void		LockMassFileMap();
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

#ifdef NEW_MAIL_LOG
#ifdef WCHAR_DIR
		CMassFile* AddMassFile(IN const std::wstring& strKomFileName, OUT bool& bIsDuplicationError_  );
#else WCHAR_DIR
		CMassFile* AddMassFile(IN const std::wstring& strKomFileName, OUT bool& bIsDuplicationError_  )
		{
			std::string fileName;
			ConvertWCHARToChar( fileName, strKomFileName.c_str() );
			return AddMassFile( fileName, bIsDuplicationError_ );
		}
		CMassFile* AddMassFile(IN const std::string& strKomFileName, OUT bool& bIsDuplicationError_ );
#endif WCHAR_DIR
#else
#ifdef WCHAR_DIR
		CMassFile* AddMassFile(const std::wstring& strKomFileName);
#else WCHAR_DIR
		CMassFile* AddMassFile(const std::wstring& strKomFileName)
		{
			std::string fileName;
			ConvertWCHARToChar( fileName, strKomFileName.c_str() );
			return AddMassFile( fileName );
		}
		CMassFile* AddMassFile(const std::string& strKomFileName);
#endif WCHAR_DIR
#endif // NEW_MAIL_LOG
#ifdef X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
		void AddNewFolderFile( const char* szSearchDir );
#endif//X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE


        // �� �Ʒ��� �� ������ ��Ű������ ������ Ȯ���ϰų� �д� API �̴�.
        // ���� ���������� ���� �������� ��Ű���� ���� �� ���� ���丮���� Ȯ���ϰų� ������,
        // ���� ���������� ��Ű���� ó���ϰ�, ���� ������ Ȯ������ �ʴ´�. robobeg - 2014.01.21
		bool KGCMassFileManager::IsValidFile( std::wstring wstrFileName );
		CMassFile::MASSFILE_MEMBERFILEINFO_PTR LoadDataFile( std::string strFileName, bool bRealData = true 
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
            , bool bKeepCompressedData = false
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS            
            );
		CMassFile::MASSFILE_MEMBERFILEINFO_PTR LoadDataFile( std::wstring wstrFileName, bool bRealData = true
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
            , bool bKeepCompressedData = false
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS              
            );

        // �� �Ʒ��� �� ������ ������ ���� ���丮���� ������ Ȯ���ϰų� �д´�. ���񽺹���, ���߹��� �����̴�. robobeg - 2014.01.21
		bool KGCMassFileManager::IsValidFile_LocalFile( std::wstring wstrFileName );
		CMassFile::MASSFILE_MEMBERFILEINFO_PTR LoadDataFile_LocalFile( std::string strFileName, bool bRealData = true );
		CMassFile::MASSFILE_MEMBERFILEINFO_PTR LoadDataFile_LocalFile( std::wstring wstrFileName, bool bRealData = true );

		
#ifdef  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
        // �� ���� map �� Ȥ�� ���� ��ġ�� adjacent �� ���� ����Ÿ �ε��� ���� �������̽�
        // �ؽ������ϰ� .TET ���� �ε��� ���� ����Ѵ�.
        CMassFile::MASSFILE_MEMBERFILEINFO_PTR
            LoadTwoDataFiles( std::string strFileName, std::string strFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO_PTR& info2, 
            bool bRealData = true );

        CMassFile::MASSFILE_MEMBERFILEINFO_PTR
            LoadTwoDataFiles( const std::wstring& wstrFileName, const std::wstring& wstrFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO_PTR& info2, 
            bool bRealData = true );

#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES



		int  GetTotalFileCount();

		//note : ���� ����Ǵ� ������ ����������θ� ��������
		bool SetSubDirectory();
		//note : ������ ������ �ش������� ��θ��� ����
		WCHAR* GetPullPath( WCHAR* wcFileName );
		char*  GetPullPath( char* cFileName );

		static void LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const char* szDesc1, const char* szDesc2 );
#ifdef WCHAR_DIR
		static void LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const WCHAR* szDesc1, const WCHAR* szDesc2 );
		static void LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const WCHAR* szDesc1, const char* szDesc2 );
#endif WCHAR_DIR

		//{{ robobeg : 2011-08-16
//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
		bool	RegisterCurrentThread();
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//		void    SetForegroundThreadId( DWORD dwThreadId )   { m_dwForegroundThreadId = dwThreadId; }
//		DWORD   GetForegroundThreadId() { return m_dwForegroundThreadId; }
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
		//}} robobeg : 2011-08-16

#ifdef	CHECK_FILE_DUPLICATION
		bool ExtractFileDuplicatedToTextFile();
		void ClearFileDuplicatedList() { m_vecFileNameDuplicated.clear(); }
#endif	CHECK_FILE_DUPLICATION

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
		void	ReleaseAllMemoryBuffers();
//#ifdef	BACKGROUND_LOADING_TEST
		static KGCMassFileBufferManager&	GetMassFileBufMan() { return m_massFileBufMan; }
//#endif	BACKGROUND_LOADING_TEST
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

#ifdef MASSFILE_MAPPING_FUNCTION //SERV_MASSFILE_MAPPING_FUNCTION
		void InitServerCurrentTime();
		__time64_t	GetServerCurrentTime();
		void SetServerCurrentTime( wstring wstrTime );
#endif MASSFILE_MAPPING_FUNCTION //SERV_MASSFILE_MAPPING_FUNCTION
#ifdef ALWAYS_MAPPING_MUSIC
public:
	std::string  MappingFileMusic(std::string inName);
#endif ALWAYS_MAPPING_MUSIC
	private:

//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
		int		GetCurrentThreadIndex();
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

#ifdef WCHAR_DIR
		//KOM������ �ƴ� Data�������� ���� ������ �о� ���̴°��..
		bool LoadRealFile( const std::wstring& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO* pOut );
		//note : ���� ���� ��θ� �޾� ����������θ� ����
		bool GetSubDirectory( WCHAR* szSearchDir );
#else WCHAR_DIR
		//KOM������ �ƴ� Data�������� ���� ������ �о� ���̴°��..
		bool LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO* pOut);
		//note : ���� ���� ��θ� �޾� ����������θ� ����
		bool GetSubDirectory( char* szSearchDir);
#endif WCHAR_DIR

#ifdef X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
#ifdef WCHAR_DIR
		bool    LoadTwoRealFiles(const std::wstring& strFileName, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info,
			const std::wstring& strFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info2 );
#else
        bool    LoadTwoRealFiles(const std::string& strFileName, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info,
            const std::string& strFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info2 );
#endif WCHAR_DIR
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES

#ifdef WCHAR_DIR
		static HANDLE OpenReadFileHandle( const WCHAR* pszFilename, unsigned uRetryCount = 0, bool bAsync = false );
		static BOOL ReadFile( HANDLE hFile, const WCHAR* pLogInfo, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
			DWORD dwOffset
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//			, bool bAsync = false 
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
			);
#else WCHAR_DIR
		static HANDLE OpenReadFileHandle( const char* pszFilename, unsigned uRetryCount = 0, bool bAsync = false );
		static BOOL ReadFile( HANDLE hFile, const char* pLogInfo, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
			DWORD dwOffset
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//			, bool bAsync = false 
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
			);
#endif WCHAR_DIR

//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//		static void WINAPI _CompletionRoutine( DWORD dwErrorCode, DWORD dwNumTrans, LPOVERLAPPED lpOverlapped );
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
		struct	KTLSData;
		const KTLSData&		GetUpdatedTLSData();
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK



    private:

//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//		struct  KGCMASSFILEMANAGER_OVERLAPPED
//		{
//			enum    EFinishCode
//			{
//				NOT_FINISHED = 0,
//				FINISHED = 1,
//				FINISHED_WITH_ERROR = 2,
//			};
//			OVERLAPPED  m_overlapped;
//			DWORD       m_dwNumBytesTransferred;
//			EFinishCode m_eFinishCode;  // 0:not finished, 1:finish, 
//		};//struct  KGCMASSFILEMANAGER_OVERLAPPED : OVERLAPPED
//
//		__declspec(thread)  static  KGCMASSFILEMANAGER_OVERLAPPED   m_overlapped;       
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//		CRITICAL_SECTION						m_csFileManager;
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

#ifdef WCHAR_DIR
		typedef stdext::hash_map< std::wstring, SMassFileIndex>   MassFileMap;
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
		std::map< std::wstring, SMassFileIndex >	m_mapMassFileBackup;
#endif SERV_MASSFILE_MAPPING_FUNCTION
#else WCHAR_DIR
		typedef stdext::hash_map< std::string, SMassFileIndex>   MassFileMap;
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
		std::map< std::string, SMassFileIndex >	m_mapMassFileBackup;
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif WCHAR_DIR

		MassFileMap					m_mapMassFile;
		std::vector<CMassFile*>		m_vecMassFile;
#ifdef WCHAR_DIR
		std::vector<std::wstring>	m_vecstrDataDirectory;
#else WCHAR_DIR
		std::vector<std::string>	m_vecstrDataDirectory;
#endif WCHAR_DIR

#ifdef X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
		std::map< std::string, CMassFile::MASSFILE_MEMBERFILEINFO_POINTER > m_mapNewFolderFile;
#endif//X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

		// mass file manager �ʱ�ȭ �Ŀ��� m_mapMassFile �� �������� �ʴ� ���� �Է�Ƽ �ϱ� ���� �Ʒ� �÷��׸� �߰��մϴ�.
		bool						m_bLockMassFileMap;
		
		// ���߹��������ۿ� ���ȵ����� �ϴ� lock ���Ÿ� ���� ���� ����� �߰��մϴ�.
		// m_vecstrDataDirectory �� ����� ������ stamp ���� ���ϰ� �ϰ�
		// �� thread ������ stamp ���� ��ȭ�� ������ m_vecstrDataDirectory �� ������ ī���մϴ�. 
		// thread �� ����Ÿ�� ������ ����� ���� ��� dangling ��ŵ�ϴ�. --;

		DWORD                       m_dwDataDirectoryStamp;
		MemberCriticalSection       m_csStrDataDirectory;

		struct KTLSData
		{
#ifdef WCHAR_DIR
			std::vector<std::wstring> m_vecstrDataDirectory;
#else WCHAR_DIR
			std::vector<std::string> m_vecstrDataDirectory;
#endif WCHAR_DIR
			DWORD                   m_dwDataDirectoryStamp;

			KTLSData()
				: m_dwDataDirectoryStamp(0) {}
		};
		__declspec(thread)  static KTLSData*  ms_pkTLSData;


//#endif  X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK



//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
//        //{{AFX
//		static KGCBufferManager					m_BufferManager;
//        //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
//        //{{AFX
//		CMassFile::MASSFILE_MEMBERFILEINFO		m_Info;
//        //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17


		//KGCMemoryFile							m_MemoryFile;

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
        static KGCMassFileBufferManager         m_massFileBufMan;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16


//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
		volatile DWORD							m_adwThreadId[ MASS_FILE_MANAGER_MAX_NUM_THREADS ];
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//		//{{ robobeg : 2011-08-16
//		DWORD                                   m_dwForegroundThreadId;
//		//}} robobeg : 2011-08-16
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
#ifdef MASSFILE_MAPPING_FUNCTION
		std::string								m_strWebAddress;
		std::map< std::string, std::vector< char > > m_mapWebBuffer;
		std::map< std::string, std::string > m_mapWebMapper;
#endif MASSFILE_MAPPING_FUNCTION
#ifdef MASSFILE_MAPPING_FUNCTION //SERV_MASSFILE_MAPPING_FUNCTION
		CTime					m_ServerCurrentTime;
		DWORD					m_TickCountAtGetTime;
#endif MASSFILE_MAPPING_FUNCTION //SERV_MASSFILE_MAPPING_FUNCTION
	
#ifdef	CHECK_FILE_DUPLICATION
		vector<string>							m_vecFileNameDuplicated;
#endif	CHECK_FILE_DUPLICATION
};//class KGCMassFileManager

