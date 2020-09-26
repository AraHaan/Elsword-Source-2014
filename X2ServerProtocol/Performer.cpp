#include "StdAfx.h"

#include "Performer.h"

NiImplementRTTI( KPerformer, KSimObject );

KPerformer::KPerformer(void)
{
	InitializeCriticalSection( &m_csEventQueue );
}

KPerformer::~KPerformer(void)
{
	{
		KLocker lock( m_csEventQueue );

		if( !m_queEvent.empty() )
		{
			START_LOG( cwarn, "Object destroy with un-processed Events. Name : " << m_strName )
				<< BUILD_LOG( m_queEvent.size() )
				<< END_LOG;
		}
	}

	DeleteCriticalSection( &m_csEventQueue );
}

ImplToStringW( KPerformer )
{
	KLocker lock( m_csEventQueue );

	return START_TOSTRING_PARENTW( KSimObject )
		<< TOSTRINGW( m_queEvent.size() );
}

void KPerformer::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	// consume event queue
	//KLocker lock( m_csEventQueue );
	::EnterCriticalSection( &m_csEventQueue );  // 050630. �̺�Ʈ�� ó���ϴ� ���� unlock �ǵ��� ����.

	// traverse all event
	KEventPtr spEvent;
	while( !m_queEvent.empty() )
	{
		// 050912. florist. 050911�� �ݸ��� �׼��� �̰����� crash�� ����Ŵ. ���� ����ó��.
		try { spEvent = m_queEvent.front(); } catch( ... ) { m_queEvent.pop(); continue; }

		m_queEvent.pop();

		::LeaveCriticalSection( &m_csEventQueue );

		// 050220. �̺�Ʈ �����Ͱ� NULL���� �����Ѵ�.
		if( spEvent != NULL )
		{
			// 2. call event handler
			ProcessEvent( spEvent.get() );
		}

		::EnterCriticalSection( &m_csEventQueue );
	}

	::LeaveCriticalSection( &m_csEventQueue );
}

void KPerformer::QueueingEvent( const KEventPtr& spEvent_ )
{
	//dbg::clog << L"�� " << spEvent_->GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

	KLocker lock( m_csEventQueue );

	m_queEvent.push( spEvent_ );
}
/*
void KPerformer::QueueingID( UidType nSenderUID_, unsigned short usEventID_ )
{
    KEventPtr spEvent( new KEvent );

    spEvent->m_usEventID    = usEventID_;
    spEvent->m_nSenderUID   = nSenderUID_;

    KLocker lock( m_csEventQueue );
    m_queEvent.push( spEvent );
}*/
