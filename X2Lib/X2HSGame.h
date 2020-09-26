#pragma once



//{{ ����� : [2009/11/12] //	�ٽ��� ������ ���̼��� Ű
#if	 defined( _NEXON_KR_ )		//	�ѱ�
	#define HS_LICENSE_KEY	L"A6E13604BC3E9D3838CAC0BA"
	#define HS_GAME_CODE	9928
#elif defined( CLIENT_COUNTRY_TW )	//	�븸
	#define HS_LICENSE_KEY L"8F791743D021D76DEDE1D892"
	#define HS_GAME_CODE	5820
#elif defined( CLIENT_COUNTRY_HK )	// ȫ��
	#define HS_LICENSE_KEY L"00476EC7ED0A77525B360F0E"
	#define HS_GAME_CODE	5825
#elif defined( CLIENT_COUNTRY_EU )	// ����
	#define HS_LICENSE_KEY L"3364D9435F7FFBC188C74E66"
	#define HS_GAME_CODE	5960
#else							//	��Ÿ
	#define HS_LICENSE_KEY L""
	#define HS_GAME_CODE	0
#endif
//}} ����� : [2009/11/12] //	�ٽ��� ������ ���̼��� Ű

//{{ 2010/10/14	��ȿ��	�ٽ��� ������Ʈ ���
#ifdef HACKSHIELD_AUTO_UPDATE
BOOL	File_Copy_Overwrite(const std::wstring& wstrSrcFilePath, const std::wstring& wstrDstFilePath);
BOOL	HS_PrepareUpdate();
BOOL	HS_Update();
#endif HACKSHIELD_AUTO_UPDATE
//}}

//{{ 2011.3.21	��ȿ��	�ٽ��� ����͸� ���� ����
#ifdef HACKSHIELD_MORNITORING
BOOL	HS_StartMonitor();
#endif HACKSHIELD_MORNITORING
//}}

//{{ ����� : [2009/8/3] //	�ٽ��� ���� �Լ�
//	�ٽ��� �ʱ�ȭ
BOOL	HS_Init();

//	�ٽ��� ���� ����
BOOL	HS_StartService();

//	�ٽ��� ���� �Ͻ� ����
BOOL	HS_PauseService();

//	�ٽ��� ���� �簳
BOOL	HS_ResumeService();

//	�ٽ��� ���� ����
BOOL	HS_StopService();

//	�ٽ��� ����
BOOL	HS_UnInit();

//	�ݹ��Լ�
int	__stdcall	HS_CallbackProc( long lCode, long lParamSize, void* pParam );

//	�����޽��� ���
void ShowErrorMsg( int iErrorMsg );

//	������ ����� ó�� �Լ�
LONG WINAPI GAME_UnhandledExceptionHandler(struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter);

//	���� �߽߰� ���� ���� �Լ�
void	SendHackShieldLog( int iError, char *wszMsg );
