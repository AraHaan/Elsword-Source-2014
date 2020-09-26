#include "Socket/Session.h"
#include "TRIOThread.h"
#include "dbg/dbg.hpp"
#include "TRIOCP.h"

_ImplementException( KTRIOThread )

KTRIOThread::KTRIOThread(void)
{
}

KTRIOThread::~KTRIOThread(void)
{
	SiKTRIocp()->PostStatus( KTRIocp::SIGNAL_SHUTDOWN_SERVER );
	End();
}

void KTRIOThread::Run()
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

void KTRIOThread::Loop() 
{
	_BEGIN_TRY_BLOCK;

	LPOVERLAPPED povl       = NULL;
	DWORD dwBytesTransfer   = 0; 
	DWORD dwKey             = 0;

	bool bOK = SiKTRIocp()->GetStatus(&dwKey, &dwBytesTransfer, &povl );

	if( dwKey == KTRIocp::SIGNAL_SHUTDOWN_SERVER && povl == NULL )
	{
		// ����� I/O Completion packet���� Ȯ�� 
		std::cout << "*** SIGNAL_SHUTDOWN_SERVER entered.\n" << std::endl;
		SetEvent( m_hKillEvent );
		return;
	} // if

	if( bOK )
	{
		SiKTRIocp()->OnIOCompleted( dwKey, dwBytesTransfer, povl );
	}

	else // ( !bOK ) // local machine�� ������ ������ ���
	{
		START_LOG( clog2, L"closed by local machine. dwKey : " << dwKey );
		//std::wstring wstrErr = KncUtil::GetLastErrMsg();

		//LOG_SUCCESS( wstrErr.compare(L"������ ��� ����.!") == 0 ) 
		//	<< L"closed by local machine. dwKey : " << dwKey << L", Err Msg : " << wstrErr << dbg::endl;

		SiKTRIocp()->OnClosedByLocal( dwKey );
	}

	_END_TRY_BLOCK( L"" );
}