#include "NProtectFunc.h"

#include <string>
#include <boost/timer.hpp>

bool			npbStartUpdateTime	= false;
bool			npbEnable			= false;
boost::timer	npkUpdateTimer;

#define VERIFY_ENABLE( ret ) \
if( npbEnable == false ) \
{ \
ret; \
} \

void KNP_Enable(bool bEnable )
{
	npbEnable = bEnable;

	if( npbEnable == true )
	{
		START_LOG( cout, L"KOREA nProtect Start.!" );
	}
}

bool KNP_Init()
{
	VERIFY_ENABLE( return false );

	DWORD dwRet = InitGameguardAuth( ".\\nProtect", 50, true, 0x03 );

	if( dwRet != ERROR_SUCCESS )
	{
		std::wstring wstrErr;
		switch( dwRet )
		{
		case 1:
			wstrErr = L"�޸� �Ҵ� ����.!";
			break;

		case 2:
			wstrErr = L"ggauth##.dll �ε� ����.!";
			break;

		case 3:
			wstrErr = L"ggauth##.dll�� Export �Լ� �������� ����.!";
			break;

		case 4:
			wstrErr = L"ggauth##.dll�� �ʱ�ȭ �Ǳ����� Export �Լ� ȣ���Ͽ���.!";
			break;

		case 5:
			wstrErr = L"csauth2.cfg ���Ϸκ��� ���� �ε�����.!";
			break;

		default:
			{
				WCHAR wstr[50] = L"";
				wsprintf( wstr, L"�ʱ�ȭ ����[ERRORCODE : %d]", dwRet );
				wstrErr = wstr;
			}
		}

		START_LOG( cerr, L"nProtect �ʱ�ȭ ����.!" )
			<< BUILD_LOG( wstrErr )
			<< END_LOG;

		return false;
	}

	SetUpdateCondition( 30, 50 ); //30�� ���� 50% �̻�.

	npbStartUpdateTime = true;
	npkUpdateTimer.restart();

	return true;
}

void KNP_CleanupGameguardAuth()
{
	VERIFY_ENABLE( return );

	CleanupGameguardAuth();
}

void KNP_Tick()
{
	VERIFY_ENABLE( return );

	if( npbStartUpdateTime == true )
	{
		if( npkUpdateTimer.elapsed() > KNP_UPDATE_TIME )
		{
			GGAuthUpdateTimer();

			npkUpdateTimer.restart();

			START_LOG( clog, L"nProtect ��ȣȭ ��� ������Ʈ.!" );
		}
	}
}

GGAUTHS_API void NpLog( int mode, char* msg )
{
	VERIFY_ENABLE( return );

	if( mode & (NPLOG_DEBUG | NPLOG_ERROR) ) // select log mode.
	{
		std::string strMsg = msg;

		START_LOG( clog, L"[nProtect Log]" )
			<< BUILD_LOG( strMsg )
			<< END_LOG;
	}
}

GGAUTHS_API void GGAuthUpdateCallback( PGG_UPREPORT report )
{
	VERIFY_ENABLE( return );

	char szMsg[MAX_PATH] = "";

	sprintf( szMsg, "GGAuth version update [%s] : [%ld] -> [%ld] \n",
					report->nType==1?"GameGuard Ver" : "Protocol Num",
					report->dwBefore, report->dwNext );

	std::string strMsg = szMsg;

	START_LOG( clog, L"nProtect update info :" )
		<< BUILD_LOG( strMsg )
		;
}