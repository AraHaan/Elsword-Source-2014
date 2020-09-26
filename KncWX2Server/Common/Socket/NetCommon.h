#pragma once
#include <Windows.h>
#include <string>
// SD_BOTH flag�� shutdown�ϴ� ���, shutdown ���� �ٷ� closesocket ȣ���ϴ� �Ͱ� �����ϴ�.
// ��/���� ������ �ִ� �����͸� �ս��� ���ɼ��� �ִ�.
#define CLOSE_SOCKET( socket ) \
    if( socket != INVALID_SOCKET ) \
    { \
    ::shutdown( socket, SD_BOTH ); \
    ::closesocket( socket ); \
    socket = INVALID_SOCKET; \
    } 

#define GET_WSA_MSG NetCommon::GetWsaMsg()

namespace NetCommon
{
    const wchar_t* GetWsaMsg();
    const char* GetLocalIP();
	std::wstring GetLocalIPW();
    bool InitWinSock();
    const char* CalcIP( DWORD dwIP );

}