#pragma once


enum FILE_STATE
{
	FS_NONE = 0,
	FS_INSERTED,
	FS_CHANGED,
	FS_DELETED,
};

enum UNCOMPRESS_MODE
{
	UM_NONE = 0,
	UM_ONE_KOM,
	UM_ALL_KOM,
	UM_SEL_KOM,
	UM_SEL_FILE,
};

struct MemberFileInfo
{
	std::wstring m_wstrFileName;
    int			 m_iFileSize;
	int			 m_iFileState;
	std::wstring m_wstrKomString;
	std::wstring m_wstrRealPath;

	MemberFileInfo()
	{
		m_iFileSize  = 0;
		m_iFileState = FS_NONE;
	}

	// find_if
	struct FIND_IF
	{
		std::wstring keyString;

		FIND_IF( std::wstring& key ) { keyString = key;	}
		bool operator()( MemberFileInfo& memberFile )
		{
			return ( keyString == memberFile.m_wstrFileName );
		}
	};
};

struct ModifyData;
class CX2KomFileManager
{
public:
	CX2KomFileManager(void);
	~CX2KomFileManager(void);

	// �ߺ����� üũ
	bool CheckSameFiles( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, std::vector< std::wstring > >& vecSameFiles );

	// lua ������
	bool CompileLuaScript( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles );
	bool CompileLuaScript( IN std::vector< std::wstring >& vecNewFolderPaths, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles );	// hyunchul, 2009-04-01

	bool RemoveCompiledLuaScript( IN std::map< std::wstring, MemberFileInfo >& mapLuaFiles );

	// ��ũ��Ʈ ��ȣȭ
	bool PrepareTextFileEncrypt( IN std::wstring& wstrNewPath, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles );
	bool PrepareTextFileEncrypt( IN std::vector< std::wstring >& vecNewFolderPaths, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles );	// hyunchul, 2009-04-01
	bool EncryptLuaScript( IN std::map< std::wstring, MemberFileInfo >& mapLuaFiles );		

	// kom ���� ����
	bool CreateKomFile( IN std::wstring& wstrSavePath, IN std::map< std::wstring, std::vector< std::wstring > >& mapPackingFiles );
	bool CreateKomFile( IN std::wstring& wstrSavePath, IN bool bIsMultiThreadMode = false );
	bool CreateKomFileMultiThread( IN std::wstring& wstrSavePath );

	// kom ���� ���� ����
	bool UncompressKomFile( IN std::wstring& wstrUncompressPath, IN std::vector< std::wstring >& vecSeletedFileList, IN std::wstring& wstrCurKomName, IN UNCOMPRESS_MODE eMode, IN bool bResultFullPath, OUT std::map< std::wstring, std::vector< std::wstring > >& mapResultFullPath );
	bool Convert_X_File( IN std::wstring& wstrNewPath, IN bool bIsRestore = false );
	bool Convert_X_File( IN std::vector< std::wstring >& vecNewFolderPaths, IN bool bIsRestore = false );	// hyunchul, 2009-04-01

	// kom������� �����Լ�
	bool ParsingKomFileAndMemberFile( IN std::wstring& wstrOldPath, IN std::wstring& wstrNewPath, IN const std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bIsLuaScriptComplile = false, IN bool bIsFileCrypt = false );
	bool ParsingMemberFile( IN std::wstring& wstrMemberFilePath, IN const std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bIsLuaScriptComplile = false, IN bool bIsFileCrypt = false );
	bool ParsingKomFile( IN std::wstring& wstrKomFilePath );
	bool ReorganizeMemberFile();
	void FindFolderFile( IN WCHAR* wstrFolder, OUT std::vector< std::wstring >& vecFileList );
	bool MemberFileFind( IN std::wstring& wstrMemberFileName, IN bool bIsOld, OUT std::wstring& wstrKomString, OUT int& iFindIndex );

	// kom���� �����
	bool RepackagingKomFile( IN std::wstring& wstrKomSavePath, IN std::map< std::wstring, ModifyData >& mapModifyData, IN bool bLuaCompile = false, IN bool bLuaEncrypt = false );

	bool IsExistFileInNewMember( IN std::wstring& wstrFileName );
	long GetFileSize( const char* strFileName );
	bool IsCompareKom() { return m_bIsCompareKom; }
	std::map< std::wstring, std::vector< MemberFileInfo > >& GetMapOldMemberFile() { return m_mapOldMemberFile;	}
	std::map< std::wstring, std::vector< MemberFileInfo > >& GetMapNewMemberFile() { return m_mapNewMemberFile;	}
	std::vector< MemberFileInfo >& GetInsertedFile() { return m_vecInsertedFile; }
	std::vector< MemberFileInfo >& GetDeletedFile() { return m_vecDeletedFile; }
	int GetKomSize( std::wstring& komName );
	std::map< std::wstring, std::wstring >& GetKomRealPathList() { return m_mapKomRealPath; }

	// Lua Script
	void LoadConfig();
	bool InitKomFileViewerOption_Lua();
	bool AddEncryptionKey_Lua();
	void SetThreadCount_Lua( int iThreadCount ) { m_iThreadCount = iThreadCount; }
	bool SetPathsForCommand_Lua();

	// for multi Thread packing
	bool GetPackingDataForMultiThreading( OUT std::wstring& wstrKomName, OUT std::vector< MemberFileInfo >& vecMemberFiles );
	const std::wstring& GetSavePathForMultiThreading() { return m_wstrSavePathForMT; }

private:
	std::wstring m_wstrOldPath;		// �� ���� ���
	std::wstring m_wstrNewPath;		// �� ���� ���
	bool		 m_bIsCompareKom;

	int m_iKFS_MIN_SIZE;
	int m_iKFS_MAX_SIZE;
	int	m_iKFS_AVERAGE_SIZE;
	std::vector< int > m_vecEncryptionKey;

	// kom member file �� ������
	std::map< std::wstring, std::vector< MemberFileInfo > > m_mapOldMemberFile;		// ������ ���� ��� ������ KOM ���ϵ��� 
																					// ����ü ������ ��Ƶ� MAP(key:fileName, T: MemberFileInfo ����ü)

	std::map< std::wstring, std::vector< MemberFileInfo > > m_mapNewMemberFile;		// �Ź��� ���� ��� ������ ���ϵ��� ������ ��Ģ�� ���� KOM ���ϸ��� �������� map�� �־���
																					// (key: kom ���ϸ�(data*), T: MemberFileInfo ����ü)
	typedef std::map< std::wstring, std::vector< MemberFileInfo > >::iterator MemberFileIterator;

	std::vector< MemberFileInfo > m_vecInsertedFile; // ������, �Ź��� ���� ��� ������ �� �߰��� ����
	std::vector< MemberFileInfo > m_vecDeletedFile;	 // ������ ���ϵ�

	// kom member file ����Ʈ ��� ������
	std::map< std::wstring, int > m_mapKomSize;

	// for log
	std::wofstream m_ofs;

	// for Lua Script Compile
	std::wstring m_wstrLuacCommand;

    std::wstring m_wstrAnsiToUTF8Command;

	// for X Convert
	std::wstring m_wstrXConvertCommand;

	// for depot path
	std::wstring m_wstrDepotPath;

	// kom file real path
	std::map< std::wstring, std::wstring > m_mapKomRealPath;	// ������ ���� ��� ������ KOM ���ϵ��� ��θ� ��Ƶ� MAP(key: fiilName, T: ���)

	// For Multi Thread Packing
	std::map< std::wstring, std::vector< MemberFileInfo > > m_mapNewMemberForMT;
	std::wstring m_wstrSavePathForMT;
	CRITICAL_SECTION m_csNewMemberForMT;
	int	m_iThreadCount;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������: ����ö
// ������: 2009.03.25
// ��������: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//{{ hyunchul // 2009-03-25, ??:??
public:
	std::vector< std::wstring > m_vecKomFileList;
	std::vector< std::wstring > m_vecFolderList;				// new Path
	std::vector< std::wstring > m_vecKomNameList;				// ������� kom ���� ����Ʈ
	std::vector< std::wstring > m_vecPathListToCopy;			// �� ���� �������� ���� �ؾ��� ����Ʈ
	std::vector< std::wstring > m_vecPathListToCompress;		// �ٲ� ���ϵ��� �־ ���� ���� �ؾ��� ����Ʈ

	//BOOL GetKomFileList();
	//BOOL GetFolderList();
	std::vector< std::wstring > GetNewFolderList();
	BOOL GetKomFileListFromLuaScript();						// ��� ��ũ��Ʈ�� ���� ������ kom ������ ��ε��� �޾ƿ���
	BOOL GetFolderToCompressListFromLuaScript( std::vector< std::wstring >& vecNewFolderPaths, std::vector< std::wstring >& vecKomNameList );			// ��� ��ũ��Ʈ�� ���� kom���� ������ ���ο� �������� ��� �޾ƿ���
	BOOL CX2KomFileManager::GetKomFileAndFolderToCompressListFromLuaScript( std::vector< std::wstring >& vecNewFolderPaths, std::vector< std::wstring >& m_vecKomNameList, bool bIsLuaScriptComplile, bool bIsFileCrypt, 
		std::map< std::wstring, MemberFileInfo >& mapLuaFiles );			// ��� ��ũ��Ʈ�� ���� ������ kom ������ ��� �� kom���� ������ ���ο� �������� ��� �޾ƿ���
	void FindFolderFile( IN WCHAR* wstrFolder, OUT std::vector< MemberFileInfo >& vecMemberFileInfoList,
		WCHAR* wstrFolderName, FILE_STATE enumState);		// �ش� ���� ���� ���ϵ� ������ ���(��������)
	void FindFolderKomFile( IN WCHAR* wstrFolder );			//, OUT std::vector< std::wstring >& vecFileList );	// �ش� ���� ���� kom ���� ã��(�Ź�����)
	void FindFolderFileAndSetFileState( IN WCHAR* wstrFolder, OUT std::vector< MemberFileInfo >& vecMemberFileInfoList, 
		WCHAR* pwstrFolderName, MemberFileIterator mit, BOOL bIsSameFolder, bool bIsLuaScriptComplile, bool bIsFileCrypt, 
		std::map< std::wstring, MemberFileInfo >& mapLuaFiles );							// �ش� ���� ���� ���ϵ� ������ ���(������, �Ź��� �Ѵ�)
	// �ߺ����� üũ
	bool CheckSameFiles( OUT std::map< std::wstring, std::vector< std::wstring > >& vecSameFiles );
	BOOL InitNewFolderPaths_Lua();//( const std::wstring wstrNewPath  );
	const std::wstring&			GetDepotPath() const;
//}} hyunchul // 2009-03-25, ??:??


};
