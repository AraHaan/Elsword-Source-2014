#include "SocketObject.h"
#include "IOThread.h"
#include "dbg/dbg.hpp"
#include "Iocp.h"

_ImplementException( KIOThread )

KIOThread::KIOThread(void)
{
}

KIOThread::~KIOThread(void)
{
    SiKIocp()->PostStatus( KIocp::SIGNAL_SHUTDOWN_SERVER );
    End();
}

void KIOThread::Run()
{
    START_LOG( clog, L"Enter Run()" );

    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 0 );     // 0.05s ����

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Loop();

        else std::cout <<  "*** WaitForSingleObject() - return : "<< ret << std::endl;

    }// while
    //START_LOG( clog, L"Leave Run()" );    // 041229. ���� exception �߻�
}

void KIOThread::Loop() 
{
    _BEGIN_TRY_BLOCK;

    LPOVERLAPPED povl       = NULL;
    DWORD dwBytesTransfer   = 0; 
    DWORD dwKey             = 0;

    bool bOK = SiKIocp()->GetStatus( &dwKey, &dwBytesTransfer, &povl );

    if( dwKey == KIocp::SIGNAL_SHUTDOWN_SERVER && povl == NULL )
    {
        // ����� I/O Completion packet���� Ȯ�� 
        std::cout << "*** SIGNAL_SHUTDOWN_SERVER entered.\n" << std::endl;
        SetEvent( m_hKillEvent );
        return;
    } // if

    if( bOK )
    {
        SiKIocp()->OnIOCompleted( dwKey, dwBytesTransfer, povl );
    }

    else // ( !bOK ) // local machine�� ������ ������ ���
    {
        START_LOG( clog2, L"closed by local machine. dwKey : " << dwKey );
		//std::wstring wstrErr = KncUtil::GetLastErrMsg();

		//LOG_SUCCESS( wstrErr.compare(L"������ ��� ����.!") == 0 ) 
		//	<< L"closed by local machine. dwKey : " << dwKey << L", Err Msg : " << wstrErr << dbg::endl;

        SiKIocp()->OnClosedByLocal( dwKey );
    }

    _END_TRY_BLOCK( L"" );
}