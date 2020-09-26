#include <WinSock2.h>   // wsaBuffer

#include "Accepter.h"
#include "NetCommon.h"
#include "IOCP.h"
#include <iostream>
#include <dbg/dbg.hpp>

KAccepter::KAccepter()
:
    m_fnOnAccept( NULL ),
    m_bNagleAlgOn( true )
{
}

void KAccepter::Init( u_short usPort_, PFNOnAccept fnOnAccept_, bool bNagleAlgOn_ /* = true  */)
{
    m_fnOnAccept = fnOnAccept_;
    m_bNagleAlgOn = bNagleAlgOn_;

    m_sock = ::WSASocket( AF_INET, 
        SOCK_STREAM, 
        IPPROTO_TCP, 
        NULL,
        0, 
        WSA_FLAG_OVERLAPPED ); // overlapped I/O

    if( m_sock == INVALID_SOCKET )
    {        
        ::WSACleanup();
        return;
    } // if

    // server restart�� ���� ��� ������ address�� ����� �� �ְ� ����Ѵ�.
    char bOK = true;
    int nRet;
    nRet = ::setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, &bOK, sizeof(char) );

    SOCKADDR_IN local;  // Ư�� port�� binding�� �Ѵ�.
    local.sin_family      = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port        = htons( usPort_ );

    if( ::bind( m_sock, (SOCKADDR *)&local, sizeof(local) ) == SOCKET_ERROR)
    {        
        CLOSE_SOCKET( m_sock );
        ::WSACleanup();
        return;
    } // if

    if( SOCKET_ERROR == ::listen(m_sock, SOMAXCONN) )
    {        
        ::shutdown( m_sock, SD_BOTH );
        ::closesocket( m_sock );
        m_sock = INVALID_SOCKET;
        ::WSACleanup();
        return;
    } // if

    return;
}

void KAccepter::Run()
{
    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 0 );     // 0.5s ����

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Loop();

        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

    }// while
}

void KAccepter::End( DWORD dwTimeout_ /*dwTimeout = 3000*/ )
{
    if( NULL != m_hThread )
    {
        CLOSE_SOCKET( m_sock );     // �߰�. accept ���� ������ �ݴ´�.

        SetEvent( m_hKillEvent );
        ::WaitForSingleObject( m_hThread, dwTimeout_ );
        std::cout << "close KAccepter Thread.\n" << std::endl;
        ::CloseHandle( m_hThread );
        m_hThread = NULL;
    } // if
} // End()

void KAccepter::Loop()
{
    SOCKET      socket      = INVALID_SOCKET;
    SOCKADDR_IN sinRemote;
    int         size        = sizeof(sinRemote);

    socket = ::accept( m_sock, (SOCKADDR*)&sinRemote, &size );

    if( socket == INVALID_SOCKET )
    {
        START_LOG( cerr, L"INVALID_SOCKET. WSAError : " << GET_WSA_MSG << END_LOG );
        return;
    }

    if( !m_bNagleAlgOn )    // Nagle option�� ����. �����͸� ��Ƽ� ������ �ʰ� �� ��� �����ϵ��� �Ѵ�.
    {
        int sock_opt = 1;
        if( ::setsockopt( socket, IPPROTO_TCP, TCP_NODELAY, (char*)&sock_opt, sizeof(sock_opt) ) == SOCKET_ERROR )
        {
            START_LOG( cerr, GET_WSA_MSG )
                << BUILD_LOG( WSAGetLastError() )
                << END_LOG;
            return;
        }
    }

    m_fnOnAccept( socket, sinRemote );
}