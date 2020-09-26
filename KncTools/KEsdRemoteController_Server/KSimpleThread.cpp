#include "StdAfx.h"
#include "SimpleIOCP.h"
#include ".\ksimplethread.h"

KSimpleThread::KSimpleThread(void)
: m_hKillEvent( NULL ), m_hThread( NULL ), m_pSimpleThreadManager( NULL ) 
{
}

KSimpleThread::~KSimpleThread(void)
{
}

KIOThread::KIOThread(void)
{
}

KIOThread::~KIOThread(void)
{
	SiKSimpleIOCP()->PostStatus( KSimpleIOCP::SIGNAL_SHUTDOWN_SERVER );
	End( 999999999 );
}

void KIOThread::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 0 );     // 0.05s ����

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

	}// while
	//START_LOG( clog, "Leave Run()" );    // 041229. ���� exception �߻�
}

void KIOThread::Loop() 
{
	LPOVERLAPPED povl       = NULL;
	DWORD dwBytesTransfer   = 0;
	DWORD dwKey             = 0;

	bool bOK = SiKSimpleIOCP()->GetStatus(&dwKey, &dwBytesTransfer, &povl );

	if( dwKey == KSimpleIOCP::SIGNAL_SHUTDOWN_SERVER && povl == NULL )
	{
		// ����� I/O Completion packet���� Ȯ��
		SetEvent( m_hKillEvent );
		return;
	} // if

	if( bOK )
	{
		SiKSimpleIOCP()->OnIOCompleted( dwKey, dwBytesTransfer, povl );
	}

	else // ( !bOK ) // local machine�� ������ ������ ���
	{
		SiKSimpleIOCP()->OnClosedByLocal( dwKey );
	}
}