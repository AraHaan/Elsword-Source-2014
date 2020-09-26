#include "stdafx.h"
#include "HShield.h"
#include "X2HSGame.h"
#include <atlconv.h>

//{{ ��ȿ�� : [2010/10/24] //	�ٽ��� �ڵ� ������Ʈ ���
#ifdef HACKSHIELD_AUTO_UPDATE
#include <boost/filesystem.hpp>
#pragma comment( lib, "HSUpChk.lib" )
#include "HSUpChk.h"
#endif HACKSHIELD_AUTO_UPDATE
//}}

extern CX2SMTPMail* g_pX2SMTPMail; // ���� ���� ��ü (x2.cpp�� ���ǵǾ�����)

//{{ 2010/10/14	��ȿ��	 �ٽ��� ������Ʈ ���
#ifdef HACKSHIELD_AUTO_UPDATE
BOOL	File_Copy_Overwrite(const std::wstring& wstrSrcFilePath, const std::wstring& wstrDstFilePath)
{
	// OverWrite ������� ó����.
	// �ش� ������ ���ų�, ������ �ƴ϶� ���丮�̸� ������
	if( false == boost::filesystem::exists(wstrSrcFilePath) || true == boost::filesystem::is_directory(wstrDstFilePath) )
	{
		return false;
	}

	// ���� ���丮�� �̹� ������ ������. ���� ���丮���� ���� ���ص� ��.
	// ���簡 ������ �̻� ���� ������ �־ �����ϰ� �ϱ� ���� ����� ���ο� ���Ϸ� ��ü�Ѵ�. 	
	if ( true == boost::filesystem::exists(wstrDstFilePath) )
	{
		boost::filesystem::remove(wstrDstFilePath);
	}

	boost::filesystem::copy_file(wstrSrcFilePath,wstrDstFilePath);
	return true;
}


BOOL	HS_PrepareUpdate()
{
	WCHAR	wszFullFilePath[MAX_PATH] = L"";
	WCHAR	wszFilePath[MAX_PATH] = L"";
	WCHAR	wszDriveName[10] = L"";
	WCHAR	wszMsg[MAX_PATH] = L"";

	//	�������� ��ġ ������
	GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
	_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);

	wcscpy( wszFullFilePath, L"" );
	//	�ٽ��� ������ EhSvc.dll ��ġ ����
	wcscat( wszFullFilePath, wszDriveName );
	wcscat( wszFullFilePath, wszFilePath );

	// Data ���� �н� 
	std::wstring wstrDataFolderFullPath(wszFullFilePath);
	
	//�ٽǵ� ���� ��� ����		// HShield ���丮
	std::wstring wstrSupportFolderFullPath(wszFullFilePath);
	wstrSupportFolderFullPath += L".\\SupportFiles";
		
	std::wstring wstrHShieldFolderFullPath(wszFullFilePath);
	wstrHShieldFolderFullPath += L".\\HShield";
	std::wstring wstrUpdateFolderFullPath = wstrHShieldFolderFullPath + L"\\Update";
		
	// SupportFiles ���� üũ. ���� ��� �Ʒ� �ٸ� �ൿ���� �� ���� ����
	if( false == boost::filesystem::exists(wstrSupportFolderFullPath) ||
		false == boost::filesystem::is_directory(wstrSupportFolderFullPath) )
	{
		return false;
	}

	// hShield ���� üũ �� ���� 
	if( true == boost::filesystem::exists(wstrHShieldFolderFullPath) )
	{
		// hShield ������ ����, hShield��� ������ �ִ� ���
		if( false == boost::filesystem::is_directory(wstrHShieldFolderFullPath) )
		{
			// ���� ����
			boost::filesystem::remove(wstrHShieldFolderFullPath);
			// hShield ���� ����
			boost::filesystem::create_directory(wstrHShieldFolderFullPath);
		}
	}
	else
	{
		// hShield ���� ����
		boost::filesystem::create_directory(wstrHShieldFolderFullPath);
	}

	// hShield\Update ���� üũ �� ���� 
	if( true == boost::filesystem::exists(wstrUpdateFolderFullPath) )
	{
		// hShield ������ ����, hShield��� ������ �ִ� ���
		if( false == boost::filesystem::is_directory(wstrUpdateFolderFullPath) )
		{
			// ���� ����
			boost::filesystem::remove(wstrUpdateFolderFullPath);
			// hShield ���� ����
			boost::filesystem::create_directory(wstrUpdateFolderFullPath);
		}
	}
	else
	{
		// hShield ���� ����
		boost::filesystem::create_directory(wstrUpdateFolderFullPath);
	}	

	std::wstring wstrTmpFullFilePath;
	std::vector<std::wstring> vecHackshieldFiles;
	std::vector<std::wstring> vecSupportFiles;

	// HShield ���丮�� ���ϵ�
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\AhnUpCtl.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\AhnUpGS.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\ehsvc.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\HSInst.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\hslogo.jpg";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\HSUpdate.env";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\HSUpdate.exe";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\V3Hunt.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\V3InetGS.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrHShieldFolderFullPath + L"\\v3pro32s.dll";
	vecHackshieldFiles.push_back(wstrTmpFullFilePath);
	
	// Support ���丮�� ���ϵ�
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\AhnUpCtl.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\AhnUpGS.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\ehsvc.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\HSInst.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\hslogo.jp-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\HSUpdate.en-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\HSUpdate.ex-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\V3Hunt.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\V3InetGS.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	wstrTmpFullFilePath = wstrSupportFolderFullPath + L"\\v3pro32s.dl-";
	vecSupportFiles.push_back(wstrTmpFullFilePath);
	
	// ��ü ���� ��Ī ���� Ȯ��
	bool bAllEquivalent = true;

	for( std::vector<std::wstring>::const_iterator const_it = vecHackshieldFiles.begin(), const_it2 = vecSupportFiles.begin(); (const_it != vecHackshieldFiles.end()) && (const_it2 != vecSupportFiles.end()) ; const_it++, const_it2++)
	{
		if( false == boost::filesystem::exists(*const_it) || false == boost::filesystem::equivalent(*const_it, *const_it2)) 
		{
			bAllEquivalent = false;
			break;
		}
	}

	if( false == bAllEquivalent )
	{
		for( std::vector<std::wstring>::const_iterator const_dst = vecHackshieldFiles.begin(), const_src = vecSupportFiles.begin(); (const_dst != vecHackshieldFiles.end()) && (const_src != vecSupportFiles.end()) ; const_dst++, const_src++)
		{
			File_Copy_Overwrite(*const_src, *const_dst);
		}
	}

	return true;
}

BOOL	HS_Update()
{
#ifdef THEMIDA_BY_TOOL_TEAM
	int hackingCheckVariable; 
#endif THEMIDA_BY_TOOL_TEAM
	
	THEMIDA_VM_START

	DWORD	dwRet = 0;
	WCHAR	wszFullFilePath[MAX_PATH] = L"";
	WCHAR	wszFilePath[MAX_PATH] = L"";
	WCHAR	wszDriveName[10] = L"";
	WCHAR	wszMsg[MAX_PATH] = L"";
	
	//	�������� ��ġ ������
	GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
	_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);
	wcscpy( wszFullFilePath, L"" );

	//	�ٽ��� ������ EhSvc.dll ��ġ ����
	wcscat( wszFullFilePath, wszDriveName );
	wcscat( wszFullFilePath, wszFilePath );

	// �ٽǵ� ���� ǥ��
	WCHAR wszClientVersion[256];
	FILE *fp = _wfopen(L".\\HShield\\client_version.txt", L"rb");
	if(fp != NULL)
	{
		fwscanf(fp, L"%s", wszClientVersion);
		// ���� ������ �������� ��� 255�� �Ѵ� ���� ���ٰ� ����
		wszClientVersion[255]  = NULL;	

		fclose(fp);
	}
	
	//	�ٽ��� ������ EhSvc.dll ��ġ ����
#ifdef _DEBUG
	wcscat( wszFullFilePath, L"..\\..\\HShield" );
#else
	//	������ ����� ��� ��ġ���� (GameLuncher/Data/ ����)
	wcscat( wszFullFilePath, L".\\HShield" );
#endif

	AHNHS_EXT_ERRORINFOW HsExtError;
	ZeroMemory(&HsExtError, sizeof(AHNHS_EXT_ERRORINFOW));

	//Monitoring ������ �����մϴ�. 
#ifdef CLIENT_COUNTRY_TW
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"112.121.93.4" );	//����͸��ּ�
#elif CLIENT_COUNTRY_HK
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"202.80.109.185" );//����͸��ּ�
#elif CLIENT_COUNTRY_EU
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"79.110.95.65" );
#else
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"79.110.88.116" );	//����͸��ּ�
#endif
	StringCchPrintf( HsExtError.szUserId, MAX_PATH, L"" );						//���� ID 
	StringCchPrintf( HsExtError.szGameVersion, MAX_PATH, wszClientVersion );	//����

	INT64 i64GameCode = (INT64)HS_GAME_CODE;

	//  _AhnHS_HSUpdate �Լ� ȣ��
	THEMIDA_CHECK_PROTECTION( hackingCheckVariable, 0xa6dc5610 ) 

	dwRet = _AhnHS_HSUpdateExW( wszFullFilePath,  // �ٽ��� ���� ���
								1000 * 600,		// ������Ʈ ��ü Ÿ�� �ƿ� 
								i64GameCode,			//HS_GAME_CODE,   // ���� �ڵ� 
								AHNHSUPDATE_CHKOPT_HOSTFILE| AHNHSUPDATE_CHKOPT_GAMECODE,
								HsExtError, 
								1000* 20 ); // ���� ���� Ÿ�Ӿƿ�

	THEMIDA_REPORT_IF_TAMPERED( hackingCheckVariable, 0xa6dc5610, ANTI_HACK_WSTRING_BYPASS_HACKSHIELD )

	THEMIDA_VM_END

	// Ex �Լ��� ����ϽǶ��� �ݵ�� HSUpSetEnv.exe ���� ���� env ���Ͽ�
	// ���� �ڵ带 �Է��ϼž� �մϴ�.
	if ( dwRet != ERROR_SUCCESS )
	{
		// ���� ó��
		switch ( dwRet )
		{
		case HSERROR_ENVFILE_NOTREAD:
			{
				wcscat( wszMsg, L"Update Failed ERR_CODE : 00" );
				//wcscat( wszMsg, L"Update Failed ERR_CODE : HSERROR_ENVFILE_NOTREAD" );
			}
			break;
		case HSERROR_ENVFILE_NOTWRITE:
			{
				wcscat( wszMsg, L"Update Failed ERR_CODE : 01" );
				//wcscat( wszMsg, L"Update Failed ERR_CODE : HSERROR_ENVFILE_NOTWRITE" );
			}
			break;
		case HSERROR_NETWORK_CONNECT_FAIL:
			{
				wcscat( wszMsg, L"Update Failed ERR_CODE : 02" );
				//wcscat( wszMsg, L"Update Failed ERR_CODE : HSERROR_NETWORK_CONNECT_FAIL" );
			}
			break;
		case HSERROR_HSUPDATE_TIMEOUT:
			{
				wcscat( wszMsg, L"Update Failed ERR_CODE : 03" );
				//wcscat( wszMsg, L"Update Failed ERR_CODE : HSERROR_HSUPDATE_TIMEOUT" );
			}
			break;
		case HSERROR_MISMATCH_ENVFILE:
			{
				wcscat( wszMsg, L"Update Failed ERR_CODE : 04" );
				//wcscat( wszMsg, L"Update Failed ERR_CODE : HSERROR_MISMATCH_ENVFILE" );
			}
			break;
		case HSERROR_HOSTFILE_MODIFICATION:
			{
				wcscat( wszMsg, L"Update Failed ERR_CODE : 05" );
				//wcscat( wszMsg, L"Update Failed ERR_CODE : HSERROR_HOSTFILE_MODIFICATION" );
			}
			break;
		default:
			{
				//wsprintfW( wszMsg, L"Update Failed ERR_CODE : %d", ( dwRet  ) ); // �������� ret���� �˾ƺ��� ���ϵ��� ������ ����� ����
				StringCchPrintf( wszMsg, MAX_PATH, L"Update Failed ERR_CODE : %d", ( dwRet ) );
				//StringCchPrintf( wszMsg, MAX_PATH, L"Update Failed ERR_CODE : %d", ( dwRet + 15778421 ) ); // �������� ret���� �˾ƺ��� ���ϵ��� ������ ����� ����
			}
			break;
		}
		MessageBoxW( NULL, wszMsg, NULL, MB_OK );
	}

	return dwRet;
}
#endif HACKSHIELD_AUTO_UPDATE
//}}

//{{ 2011.3.21	��ȿ��	�ٽ��� ����͸� ���� ����
#ifdef HACKSHIELD_MORNITORING
BOOL	HS_StartMonitor()
{
#ifdef THEMIDA_BY_TOOL_TEAM
	int hackingCheckVariable; 
#endif THEMIDA_BY_TOOL_TEAM

	THEMIDA_VM_START

	DWORD	dwRet = 0;
	WCHAR	wszFullFilePath[MAX_PATH] = L"";
	WCHAR	wszFilePath[MAX_PATH] = L"";
	WCHAR	wszDriveName[10] = L"";
	WCHAR	wszMsg[MAX_PATH] = L"";

	//	�������� ��ġ ������
	GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
	_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);
	wcscpy( wszFullFilePath, L"" );

	//	�ٽ��� ������ EhSvc.dll ��ġ ����
	wcscat( wszFullFilePath, wszDriveName );
	wcscat( wszFullFilePath, wszFilePath );

	// �ٽǵ� ���� ǥ��
	WCHAR wszClientVersion[MAX_PATH] = { 0, };

	FILE *fp = _wfopen(L".\\HShield\\client_version.txt", L"rb");
	if(fp != NULL)
	{
		fwscanf(fp, L"%s", wszClientVersion);
		// ���� ������ �������� ��� 255�� �Ѵ� ���� ���ٰ� ����
		wszClientVersion[MAX_PATH-1]  = NULL;	

		dbg::clog << wszClientVersion << dbg::endl;
		fclose(fp);
	}

	//	�ٽ��� ������ EhSvc.dll ��ġ ����
#ifdef _DEBUG
	wcscat( wszFullFilePath, L"..\\..\\HShield\\Ehsvc.dll" );
#else
	//	������ ����� ��� ��ġ���� (GameLuncher/Data/ ����)
	wcscat( wszFullFilePath, L".\\HShield\\Ehsvc.dll" );
#endif

	AHNHS_EXT_ERRORINFOW HsExtError;
	ZeroMemory(&HsExtError, sizeof(AHNHS_EXT_ERRORINFOW));

	//Monitoring ������ �����մϴ�. 
#ifdef CLIENT_COUNTRY_TW
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"112.121.93.4" );	//����͸��ּ�
#elif CLIENT_COUNTRY_HK
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"202.80.109.185" );//����͸��ּ�
#elif CLIENT_COUNTRY_EU
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"79.110.95.65" );	//����͸��ּ�
#else
	StringCchPrintf( HsExtError.szServer, MAX_PATH, L"79.110.88.116" );	//����͸��ּ�
#endif
	StringCchPrintf( HsExtError.szUserId, MAX_PATH, L"" );						//���� ID 
	StringCchPrintf( HsExtError.szGameVersion, MAX_PATH, wszClientVersion );	//����

	THEMIDA_CHECK_PROTECTION( hackingCheckVariable, 0x186db035 ) 

	dwRet = _AhnHS_StartMonitor( HsExtError, wszFullFilePath );

	THEMIDA_REPORT_IF_TAMPERED( hackingCheckVariable, 0x186db035, ANTI_HACK_WSTRING_BYPASS_HACKSHIELD )

	THEMIDA_VM_END

	// Ex �Լ��� ����ϽǶ��� �ݵ�� HSUpSetEnv.exe ���� ���� env ���Ͽ�
	// ���� �ڵ带 �Է��ϼž� �մϴ�.
	if ( dwRet != ERROR_SUCCESS )
	{
		WCHAR ErrorMsg[MAX_PATH] = L"_AhnHS_StartMonitor Error";
		dbg::clog << ErrorMsg << dbg::endl;
	}

	return dwRet;
}
#endif HACKSHIELD_MORNITORING
//}}

//	����� : [2009/8/3] //	�ٽ��� ���� �Լ�
//	�ٽ��� �ʱ�ȭ
BOOL	HS_Init()
{
	int		iRet = HS_ERR_OK;
	WCHAR	wszFullFilePath[MAX_PATH] = L"";
	WCHAR	wszFilePath[MAX_PATH] = L"";
	WCHAR	wszDriveName[10] = L"";
	WCHAR	wszMsg[MAX_PATH];
	DWORD	dwOption = 0;

	//	�������� ��ġ ������
	GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
	_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);


	wcscpy( wszFullFilePath, L"" );
	//	�ٽ��� ������ EhSvc.dll ��ġ ����
	wcscat( wszFullFilePath, wszDriveName );
	wcscat( wszFullFilePath, wszFilePath );

	//	�ٽ��� ������ EhSvc.dll ��ġ ����
#ifdef _DEBUG
	wcscat( wszFullFilePath, L"..\\..\\HShield\\Debug\\EhSvc.dll" );
#else
	//	������ ����� ��� ��ġ���� (GameLuncher/Data/ ����)
	wcscat( wszFullFilePath, L".\\HShield\\EhSvc.dll" );
#endif

	dwOption =	AHNHS_CHKOPT_ALL |					//	AHNHS_CHKOPT_SPEEDHACK | AHNHS_CHKOPT_READWRITEPROCESSMEMORY | AHNHS_CHKOPT_KDTRACER |
													//	AHNHS_CHKOPT_OPENPROCESS | AHNHS_CHKOPT_AUTOMOUSE | AHNHS_CHKOPT_PROCESSSCAN
		AHNHS_USE_LOG_FILE |				//	�α����� ����

		
#ifdef _SERVICE_
		AHNHS_CHKOPT_STANDALONE |			//	�ٽ��� �ܵ� ���� �ɼ�(���� ���������� ����)
#endif
											//{{ ����� : [2009/10/26] //	LMP ���
#ifdef HS_LMP
		AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION |
#endif	//	HS_LMP

#ifdef HS_ADD_OPTION
		//AHNHS_CHKOPT_LOADLIBRARY |
		//AHNHS_CHKOPT_ABNORMAL_FUNCTION_CALL |
		
		AHNHS_CHKOPT_SELF_DESTRUCTION |

		AHNHS_DISPLAY_HACKSHIELD_TRAYICON |

#endif
#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
		AHNHS_CHKOPT_ABNORMAL_FUNCTION_CALL_V2 |
#endif

#ifdef ADDITIONAL_HS
		AHNHS_CHKOPT_MESSAGEHOOK |
		//AHNHS_ALLOW_SVCHOST_OPENPROCESS |
		//AHNHS_ALLOW_LSASS_OPENPROCESS |
		//AHNHS_ALLOW_CSRSS_OPENPROCESS |
		//AHNHS_CHKOPT_PROTECT_D3DX |
#endif
#ifdef HS_REQUEST_OPTION
		//AHNHS_CHKOPT_PROTECT_D3DX |		// �� �ɼ� Ȱ��ȭ�ϸ� Ŭ���̾�Ʈ ����� ����.
		AHNHS_CHKOPT_UPDATED_FILE_CHECK |
		AHNHS_CHKOPT_SEND_MONITOR_DELAY |
		AHNHS_CHKOPT_DETECT_VIRTUAL_MACHINE |
#endif HS_REQUEST_OPTION

		AHNHS_DISPLAY_HACKSHIELD_LOGO;		//	�ٽ��� �ΰ� ȭ�鿡 ��Ÿ��


	iRet = _AhnHS_InitializeW (	wszFullFilePath,					
		HS_CallbackProc,						//	�ݹ� �Լ�
		HS_GAME_CODE,							//	���� �ڵ�
		HS_LICENSE_KEY,							//	���̼��� Ű
		dwOption,								//	�ɼ� �÷���
		AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL || AHNHS_SPEEDHACK_EXOPT_ALLOW_SLOW );	//	�ٽ��� ����


	if( iRet != HS_ERR_OK )
	{
		switch( iRet )
		{
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:
			{
				wcscat( wszMsg, L"Start Failed ERR_CODE : 00" );
			}
			break;
		case HS_ERR_NEED_ADMIN_RIGHTS:
			{
				wcscat( wszMsg, L"Start Failed ERR_CODE : 01" );
				//{{ 2010/01/13 ���� : Admin ������ �ƴϴ��� �����Ų��
#ifdef HS_NO_ADMIN_NEED
				return HS_ERR_OK;
#endif	//	NOT_USE_ADMIN_NEED
				//{{ 2010/01/13 ����
			}
			break;
		case HS_ERR_INVALID_FILES:
			{
				wcscat( wszMsg, L"Start Failed ERR_CODE : 02" );
			}
			break;
		case HS_ERR_INIT_DRV_FAILED:
			{
				wcscat( wszMsg, L"Start Failed ERR_CODE : 03" );
			}
			break;
		case HS_ERR_DEBUGGER_DETECT:
			{
				wcscat( wszMsg, L"Start Failed ERR_CODE : 04" );
			}
			break;
		case HS_ERR_NOT_INITIALIZED:
			{
				wcscat( wszMsg, L"Start Failed ERR_CODE : 05" );
			}
			break;
		default:
			{
				StringCchPrintf( wszMsg, MAX_PATH, L"Start Failed ERR_CODE : %d", ( iRet + 15778421 ) ); // �������� ret���� �˾ƺ��� ���ϵ��� ������ ����� ����
				//wsprintfW( wszMsg, L"Start Failed ERR_CODE : %d", ( iRet + 15778421 ) ); // �������� ret���� �˾ƺ��� ���ϵ��� ������ ����� ����
			}
			break;
		}
		MessageBoxW( NULL, wszMsg, NULL, MB_OK );
	}

	return iRet;
}

//	�ٽ��� ���� ����
BOOL	HS_StartService()
{

	int		iRet = HS_ERR_OK;
	WCHAR	wszMsg[MAX_PATH];

	iRet = _AhnHS_StartService();

	if( iRet != HS_ERR_OK )
	{
		switch( iRet )
		{
		case HS_ERR_ALREADY_GAME_STARTED:
			{
				wcscat( wszMsg, L"Already Game Started!" );
			}
			break;
		case HS_ERR_START_ENGINE_FAILED:
			{
				wcscat ( wszMsg, L"Start Failed ERR_CODE : 10" );
			}
			break;
		case HS_ERR_DRV_FILE_CREATE_FAILED:
			{
				wcscat ( wszMsg, L"Start Failed ERR_CODE : 11" );
			}
			break;
		case HS_ERR_REG_DRV_FILE_FAILED:
			{
				wcscat ( wszMsg, L"Start Failed ERR_CODE : 12" );
			}
			break;
		case HS_ERR_START_DRV_FAILED:
			{
				wcscat ( wszMsg, L"Start Failed ERR_CODE : 13" );
			}
			break;
		default:
			{
				StringCchPrintf ( wszMsg, MAX_PATH, L"Start Failed ERR_CODE : %d", ( iRet + 15778421 ) ); // �������� ret���� �˾ƺ��� ���ϵ��� ������ ����� ����
				//wsprintfW ( wszMsg, L"Start Failed ERR_CODE : %d", ( iRet + 15778421 ) ); // �������� ret���� �˾ƺ��� ���ϵ��� ������ ����� ����
			}
			break;
		}
		MessageBoxW( NULL, wszMsg, NULL, MB_OK );
	}
	
	return iRet;
}

//	�ٽ��� ���� �Ͻ� ����
BOOL	HS_PauseService( DWORD dwPauseOption )
{
	return _AhnHS_PauseService( dwPauseOption );
}

//	�ٽ��� ���� �簳
BOOL	HS_ResumeService( DWORD dwResumeOption )
{
	return _AhnHS_ResumeService( dwResumeOption );
}

//	�ٽ��� ���� ����
BOOL	HS_StopService()
{
	return _AhnHS_StopService();
}

//	�ٽ��� ����
BOOL	HS_UnInit()
{
	return _AhnHS_Uninitialize();
}

//	�ݹ��Լ�
int	__stdcall	HS_CallbackProc( long lCode, long lParamSize, void* pParam )
{
	WCHAR	wszMsg[MAX_PATH];

	USES_CONVERSION;


	switch( lCode )
	{
		//	���� �� �߰�(���� GameGuard Dialog���� �� �̸��� ���� �� ���α׷� ����)
	case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			wstring wstrProgramName = A2CW((LPCSTR)pParam);

			wcscat( wszMsg, GET_REPLACED_STRING( ( STR_ID_4428, "L", wstrProgramName ) ) );

			//	���� ����
			//	SendHackShieldLog( lCode, W2A(wszMsg) );

			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), wszMsg, GET_STRING( STR_ID_4427 ), MB_OK );

#ifndef _DEBUG
			g_pKTDXApp->SetFindHacking( true );
#endif

			//	�޽��� �α� ���
			ShowErrorMsg( lCode );
		}	break;
		//	���� ��ũ��
	case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			wcscat( wszMsg, GET_STRING( STR_ID_4429 ));
			
			//	���� ����
			//	SendHackShieldLog( lCode, W2A(wszMsg) );

			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), wszMsg, GET_STRING( STR_ID_4427 ), MB_OK );

#ifndef _DEBUG
			g_pKTDXApp->SetFindHacking( true );
#endif

			//	�޽��� �α� ���
			ShowErrorMsg( lCode );
		}	break;
		//	���ǵ� ��
	case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			wcscat( wszMsg, GET_STRING( STR_ID_4430 ));
			
#ifndef _DEBUG
			//	���� ����
			//	SendHackShieldLog( lCode, W2A(wszMsg) );

			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), wszMsg, GET_STRING( STR_ID_4427 ), MB_OK );

			g_pKTDXApp->SetFindHacking( true );
#endif

			//	�޽��� �α� ���
			ShowErrorMsg( lCode );
		}	break;
		//	����� ����
// #ifdef ADDITIONAL_HS
// 	case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
// 		{
// #ifdef ADD_COLLECT_CLIENT_INFO
// 			g_pMain->SendHackInfo3( ANTIHACKING_ID::ANTIHACKING_GAME_51, "Msg", false, true );
// #endif
// 		}
// 		break;
// #endif
	case AHNHS_ACTAPC_DETECT_KDTRACE:
// 	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
// 		{
// 			wcscat( wszMsg, GET_STRING( STR_ID_4431 ));
// 
// 			//	���� ����
// 			//	SendHackShieldLog( lCode, W2A(wszMsg) );
// 
// 			//	�޽��� ȣ��
// 			MessageBoxW( g_pKTDXApp->GetHWND(), wszMsg, GET_STRING( STR_ID_4427 ), MB_OK );
// 
// #ifdef ADD_COLLECT_CLIENT_INFO
// 			g_pMain->SendHackInfo4( ANTIHACKING_ID::ANTIHACKING_GAME_10, "", true, false );
// #else
// 			g_pMain->SendHackMail_DamageHistory( W2A( wszMsg) );
// #endif
// 
// #ifndef _DEBUG
// 			g_pKTDXApp->SetFindHacking( true );
// #endif
// 
// 			//	�޽��� �α� ���
// 			ShowErrorMsg( lCode );
// 		}	break;
	
		//	��ŷ ���� ��� �̻�	
	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
	//case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
	case AHNHS_ACTAPC_DETECT_LMP_FAILED:
	case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
	case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
	case AHNHS_ACTAPC_DETECT_ANTIFREESERVER:
	case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
		{
			//wcscat( wszMsg, GET_STRING( STR_ID_4432 ) );
			// kimhc // 2011-04-01 // �����ڵ� ����ϵ��� ����
#if 0
			wstringstream wstrMsgStream;
			wstrMsgStream << GET_STRING( STR_ID_4432 ) << L"(" << std::hex << lCode << L")";


			//	���� ����
			//	SendHackShieldLog( lCode, W2A(wszMsg) );

			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), wstrMsgStream.str().c_str(), GET_STRING( STR_ID_4427 ), MB_OK );

			// �ٸ��� ����			
			g_pMain->SendHackMail_DamageHistory( W2A(wstrMsgStream.str().c_str() ) );
#endif

#ifndef _DEBUG
			g_pKTDXApp->SetFindHacking( true );
#endif

			//	�޽��� �α� ���
			ShowErrorMsg( lCode );

		}
		break;
#ifdef HS_ADD_OPTION
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_WRITE:
#endif
		{
#if 0
			//wcscat( wszMsg, GET_STRING( STR_ID_4432 ) );
			// kimhc // 2011-04-01 // �����ڵ� ����ϵ��� ����
			wstring wstrProgramName = L""; //A2CW((LPCSTR)pParam);
			ConvertCharToWCHAR( wstrProgramName, (char*)pParam, lParamSize );

			wstringstream wstrMsgStream;
			wstrMsgStream << GET_STRING( STR_ID_4432 ) << L"(" << std::hex << lCode << L")";

//			wstringstream wstrMsgStream;
//			wstrMsgStream << GET_STRING( STR_ID_4432 ) << 
//				L"( " << std::hex << lCode << L" : " << wstrProgramName.c_str() << L" )";
			
			
			//	���� ����
			//	SendHackShieldLog( lCode, W2A(wszMsg) );

			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), wstrMsgStream.str().c_str(), GET_STRING( STR_ID_4427 ), MB_OK );

			// �ٸ��� ����			
			g_pMain->SendHackMail_DamageHistory( W2A(wstrMsgStream.str().c_str() ) );
#endif

#ifndef _DEBUG
			g_pKTDXApp->SetFindHacking( true );
#endif

			//	�޽��� �α� ���
			ShowErrorMsg( lCode );
		}	break;

		//{{ ����� : [2009/10/26] //	LMP���. note : pParam = ���۵� ����̸�(���Base�ּ�. ���۵� ���� ������ �ּ�)
#ifdef HS_LMP
	case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
		{
			wcscat( wszMsg, GET_STRING( STR_ID_4463 ));

			//	���� ����
			//	SendHackShieldLog( lCode, W2A(wszMsg) );

			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), wszMsg, GET_STRING( STR_ID_4427 ), MB_OK );

#ifdef ADD_COLLECT_CLIENT_INFO
			g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_11, "", true, false );
#else
			g_pMain->SendHackMail_DamageHistory( W2A( wszMsg ) );
			g_pKTDXApp->SetFindHacking( true );
#endif

			//	�޽��� �α� ���
			ShowErrorMsg( lCode );
		}	break;
#endif	//	HS_LMP
		//}} ����� : [2009/10/26] //

		//	���� ó�� ����
// 	case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
// 	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
// 		{
// 		}	break;
#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
	case AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL:
		{
#ifdef _IN_HOUSE_
			//	�޽��� ȣ��
			MessageBoxW( g_pKTDXApp->GetHWND(), L"�ܺθ�⿡�� Call", GET_STRING( STR_ID_4427 ), MB_OK );
#endif

#ifndef _DEBUG
			if( g_pData != NULL && g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
			}
			g_pKTDXApp->SetFindHacking( true );
#endif

			// �ٸ��� ����			
#ifdef ADD_COLLECT_CLIENT_INFO
			g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_12, "", true, false );
#else
			g_pMain->SendHackMail_DamageHistory( "-- FM Call --" );
			g_pKTDXApp->SetFindHacking( true );
#endif
		}
		break;
	case AHNHS_ACTAPC_STATUS_HACKSHIELD_RUNNING:
		{
			g_pMain->SetHSStatusTime(0.f);
		}
		break;
#endif //ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
	}

	return 1;
}

//	�����޽��� ���(����� �����ڰ��� ���ӽ� �ܼ�â���� ���, ���� �α� ��� ����� ��� �κ� ����)
void ShowErrorMsg( int iErrorMsg )
{
	WCHAR ErrorMsg[MAX_PATH] = L"";

	SYSTEMTIME st; 
	GetLocalTime (&st); 

	StringCchPrintf( ErrorMsg, MAX_PATH , L"HackShield Error(0x%04x) [%02d:%02d:%02d], Msg  : ", iErrorMsg, st.wHour, st.wMinute, st.wSecond);
	//wsprintf( ErrorMsg, L"HackShield Error(0x%04x) [%02d:%02d:%02d], Msg  : ", iErrorMsg, st.wHour, st.wMinute, st.wSecond);

	switch( iErrorMsg )
	{
		//	�ٽǵ� ��������
	case HS_ERR_INVALID_PARAM:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4433 ) );
		}	break;
	case HS_ERR_COMPATIBILITY_MODE_RUNNING:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4434 ) );
		}	break;
	case HS_ERR_INVALID_FILES:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4435 ) );
		}	break;
	case HS_ERR_INVALID_LICENSE:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4436 ) );
		}	break;
	case HS_ERR_INIT_DRV_FAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4437 ) );
		}	break;
	case HS_ERR_DEBUGGER_DETECT:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4438 ) );
		}	break;
	case HS_ERR_NEED_ADMIN_RIGHTS:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4439 ) );
		}	break;
	case HS_ERR_NOT_INITIALIZED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4440 ) );
		}	break;
	case HS_ERR_START_ENGINE_FAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4441 ) );
		}	break;
	case HS_ERR_ALREADY_SERVICE_RUNNING:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4442 ) );
		}	break;
	case HS_ERR_DRV_FILE_CREATE_FAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4443 ) );
		}	break;
	case HS_ERR_REG_DRV_FILE_FAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4444 ) );
		}	break;
	case HS_ERR_START_DRV_FAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4445 ) );
		}	break;
	case HS_ERR_SERVICE_NOT_RUNNING:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4446 ) );
		}	break;
	case HS_ERR_SERVICE_STILL_RUNNING:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4447 ) );
		}	break;
	case HS_ERR_ALREADY_GAME_STARTED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4448 ) );
			break;
		}

		//	�ݹ��Լ� ����
	case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4449 ) );
		}break;
	case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4450 ) );
			return;
		}break;
// 	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
// 		{
// 			wcscat( ErrorMsg, GET_STRING( STR_ID_4451 ) );
// 		}break;
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4452 ) );
		}break;
// 	case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
// 		{
// 			wcscat( ErrorMsg, GET_STRING( STR_ID_4453 ) );
// 		}break;
	case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4454 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4455 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4456 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_KDTRACE:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4457 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
	case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_WRITE:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4458 ) );
		}break;
// 	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
// 		{
// 			wcscat( ErrorMsg, GET_STRING( STR_ID_4459 ) );
// 		}break;
// 	case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
// 		{
// 			wcscat( ErrorMsg, GET_STRING( STR_ID_4460 ) );
// 		}break;
	case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4461 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4462 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4463 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_LMP_FAILED:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4464 ) );
		}break;
	case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4465 ) );
		}break;
// 	case AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED:
// 		{
// 			wcscat( ErrorMsg, GET_STRING( STR_ID_4466 ) );
// 		}break;
	case AHNHS_ACTAPC_STATUS_HACKSHIELD_RUNNING:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4467 ) );
		}break;


		//	��Ÿ �˼����� ����
	case HS_ERR_UNKNOWN:
	default:
		{
			wcscat( ErrorMsg, GET_STRING( STR_ID_4468 ) );
		}	break;
	}

#ifndef _SERVICE_
	//	�����α� ���
	dbg::clog << ErrorMsg << dbg::endl;
#endif

#if defined(ADDITIONAL_HS) && defined(ADD_COLLECT_CLIENT_INFO)
	if( g_pMain != NULL )
	{
		string strErrMsg = "";
		ConvertWCHARToChar(strErrMsg, ErrorMsg);
		g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_51, strErrMsg.c_str(), false, true );
	}	
#endif
}

//	������ ����� ó�� �Լ�
LONG WINAPI GAME_UnhandledExceptionHandler(struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter)
{
	HS_StopService();
	HS_UnInit();

	return 0;
}

//	���� �߽߰� ���� ���� �Լ�
void	SendHackShieldLog( int iError, char *szMsg )
{
	//g_pX2SMTPMail->SetReceiverEMail( "sulzi@kogstudios.com" ); 
	g_pX2SMTPMail->SetReceiverEMail( "" ); 

	USES_CONVERSION;

	if( g_pData != NULL && g_pData->GetMyUser() != NULL)
	{
		g_pX2SMTPMail->SetSenderName( W2A(g_pData->GetMyUser()->GetSelectUnit()->GetNickName()) );
	}
	else
	{
		//g_pX2SMTPMail->SetSenderName( "���ҵ�����" );
	}
	//g_pX2SMTPMail->SetSubject( "�ٽ��� ��������Ʈ�Դϴ�." );

	char szDesc[MAX_PATH];

	StringCchPrintfA(szDesc, MAX_PATH , "%s(0x%x)", szMsg, iError);
	//sprintf(szDesc, "%s(0x%x)", szMsg, iError);

	g_pX2SMTPMail->SetDesc( szDesc );
	g_pX2SMTPMail->ClearFile();			

	bool bResult = g_pX2SMTPMail->SendMail( g_pKTDXApp->GetHWND() );
}