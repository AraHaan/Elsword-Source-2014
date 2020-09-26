/********************************************************************
	created:	2005/02/22
	created:	22:2:2005   16:40
	author:		Florist.
	
	description:	
        050129. �ʱ� release.
        050222. ���� COM �ʱ�ȭ ó�� ����. ( �ʱ�ȭ flag�� ������. Ŭ���̾�Ʈ���� ���� �ʱ�ȭ)

*********************************************************************/
#pragma once

#undef _DLLAPI

#if defined(_USRDLL)    // DLL library compile ��
#   define _DLLAPI __declspec(dllexport)
#else
#   define _DLLAPI __declspec(dllimport)
#endif

//#ifndef _USRDLL
//#   ifdef _DEBUG
//#       pragma comment( lib, "KncFirewallD" )
//#   else
//#       pragma comment( lib, "KncFirewall" )
//#   endif
//#endif

#pragma comment( lib, "KncFirewall" )

#define _WIN32_DCOM 
#include <tchar.h>
#include <wchar.h>

#include <windows.h>

extern "C"
{
    namespace KncFirewall 
    {
        enum ENUM_RETURN 
        {
            KNC_OK = 0,                 // ��������.
            __INIT_COM_FAILED,          // COM �ʱ�ȭ ����. ���� �̷� ��쵵 ���� ���ΰ�? // 050222. ���� COM �ʱ�ȭ ����.
            NOT_WINXP_SP2,              // Windows XP SP2 �� �ƴ�.
            NO_EXCEPTION_ALLOWED,       // '��� ���� ��� ����' ��� ������ - ���� ���� �Ұ���
            NOT_SYSADMIN,               // ���� �׸��� �����Ϸ� ������ �ý��� ADMIN ������ ����
        };

        // 050222. �Ʒ� �� �Լ� ��� ����ϱ� ���� �ݵ�� CoInitialize �� �� ���¿��� �Ѵ�.
        // �׷��� ������ WindowXP sp2 ������ WindowXP sp2 �� �ƴ϶�� �ν��ϰ� �ȴ�.
		_DLLAPI bool OnInstallApplication( WCHAR* szwFullFilePath, WCHAR* szwName );
        _DLLAPI bool OnUninstallApplication( WCHAR* szwFullFilePath );
        _DLLAPI int GuarantedFireWallException( WCHAR* szwFullFilePath, WCHAR* szwName );
    }
}
