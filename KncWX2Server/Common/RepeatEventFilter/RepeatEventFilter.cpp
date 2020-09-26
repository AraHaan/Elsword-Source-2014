#include ".\repeateventfilter.h"
#include <dbg/dbg.hpp>
#include "Event.h"

//////////////////////////////////////////////////////////////////////////
KRepeatEventFilter::KRepeatEventFilter(void)
{
	m_iRegCount = 0;
}

KRepeatEventFilter::~KRepeatEventFilter(void)
{
}

bool KRepeatEventFilter::RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	++m_iRegCount; // ��� �õ� ī��Ʈ

	boost::shared_ptr< bool > spRepeatEvent( new bool );

	if( spRepeatEvent.get() == NULL )
		return false;

	*spRepeatEvent = false;
	m_mapCheckRepeatEventReq.insert( std::make_pair( usReqEventID, spRepeatEvent ) );
	m_mapCheckRepeatEventAck.insert( std::make_pair( usAckEventID, spRepeatEvent ) );
	return true;
}

bool KRepeatEventFilter::CheckReqEvent( unsigned short usEventID )
{
	// req �̺�Ʈ ���� ã�ƺ���
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ�� �ɷ����� �ʴٸ� true�� �ٲٰ� event�� ��� ��Ų��.
            *mit->second = true;
			return true;
		}
		else
		{
			// REQ�� �ɷ��ִٸ� event�� �����Ű�� �ʴ´�.
			return false;
		}
	}

	// ��ϵȰ� ���ٸ� �׳� �����Ų��
	return true;
}

void KRepeatEventFilter::CheckAckEvent( unsigned short usEventID )
{
	// req�� ��ϵȰ� ������ ack���� �̺�Ʈ�� ã�ƺ���
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventAck.find( usEventID );
	if( mit != m_mapCheckRepeatEventAck.end() )
	{
		// ack�̺�Ʈ �߰�! false������ ����
		*mit->second = false;
	}
}



//////////////////////////////////////////////////////////////////////////
KRepeatEventFilterNew::KRepeatEventFilterNew(void)
{
	m_iRegCount = 0;
}

KRepeatEventFilterNew::~KRepeatEventFilterNew(void)
{
}

bool KRepeatEventFilterNew::RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	++m_iRegCount; // ��� �õ� ī��Ʈ

	boost::shared_ptr< bool > spRepeatEvent( new bool );

	if( spRepeatEvent.get() == NULL )
		return false;

	*spRepeatEvent = true; // �ʱⰪ�� true
	m_mapCheckRepeatEventReq.insert( std::make_pair( usReqEventID, spRepeatEvent ) );
	m_mapCheckRepeatEventAck.insert( std::make_pair( usAckEventID, spRepeatEvent ) );
	return true;
}

bool KRepeatEventFilterNew::CheckReqEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	// req �̺�Ʈ ���� ã�ƺ���
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usReqEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ�� �ɷ����� �ʴٸ� true�� �ٲٰ� event�� ��� ��Ų��.
			*mit->second = true;
			return true;
		}
		else
		{
			// REQ�� �ɷ��ִٸ� event�� �����Ű�� �ʴ´�.
			return false;
		}
	}

	// ��ϵȰ� ���ٸ� ���� ����Ѵ�!
	return RegRepeatEvent( usReqEventID, usAckEventID );
}

void KRepeatEventFilterNew::CheckAckEvent( unsigned short usEventID )
{
	// req�� ��ϵȰ� ������ ack���� �̺�Ʈ�� ã�ƺ���
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventAck.find( usEventID );
	if( mit != m_mapCheckRepeatEventAck.end() )
	{
		// ack�̺�Ʈ �߰�! false������ ����
		*mit->second = false;
	}
}

////{{ 2012. 02. 22	�ڼ���	��� �̸� �����
//#ifdef SERV_GUILD_CHANGE_NAME
bool KRepeatEventFilterNew::JustVerifyReqEvent( unsigned short usReqEventID )
{
	// req �̺�Ʈ ���� ã�ƺ���
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usReqEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ�� �ɷ����� �ʴٸ� event�� �����Ų��.
			return true;
		}
		else
		{
			// REQ�� �ɷ��ִٸ� event�� �����Ű�� �ʴ´�.
			return false;
		}
	}

	// ��ϵȰ� ���ٸ� �����Ų��.
	return true;
}

bool KRepeatEventFilterNew::NegativeCheckReqEvent( unsigned short usReqEventID )
{
	// req �̺�Ʈ ���� ã�ƺ���
	KRepeatEventMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usReqEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( (*mit->second) == false )
		{
			// REQ�� �ɷ����� �ʴٸ� event�� �����Ű�� �ʴ´�.
			return false;
		}
		else
		{
			// REQ�� �ɷ��ִٸ� event�� �����Ų��.
			return true;
		}
	}

	// ��ϵȰ� ���ٸ� �����Ű�� �ʴ´�
	return false;
}

void KRepeatEventFilterNew::RemoveReqEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	// req �̺�Ʈ ���� ã�ƺ���
	m_mapCheckRepeatEventReq.erase( usReqEventID );
	m_mapCheckRepeatEventAck.erase( usAckEventID );
}
//#endif SERV_GUILD_CHANGE_NAME
////}}


//////////////////////////////////////////////////////////////////////////
KRepeatEventCountFilter::KRepeatEventCountFilter(void)
{
	m_iRegCount = 0;
	m_iCheckedFilteringCount = 0;
	m_fFilteringTime = 0.0;
}

KRepeatEventCountFilter::~KRepeatEventCountFilter(void)
{
}

bool KRepeatEventCountFilter::RegRepeatEvent( unsigned short usReqEventID, unsigned short usAckEventID )
{
	++m_iRegCount; // ��� �õ� ī��Ʈ

	boost::shared_ptr< KEventFilterInfo > spRepeatEvent( new KEventFilterInfo );

	if( spRepeatEvent.get() == NULL )
		return false;

	m_mapCheckRepeatEventReq.insert( std::make_pair( usReqEventID, spRepeatEvent ) );
	m_mapCheckRepeatEventAck.insert( std::make_pair( usAckEventID, spRepeatEvent ) );
	return true;
}

bool KRepeatEventCountFilter::CheckReqEvent( unsigned short usEventID )
{
	// req �̺�Ʈ ���� ã�ƺ���
	KRepeatEventFilterMap::iterator mit;
	mit = m_mapCheckRepeatEventReq.find( usEventID );
	if( mit != m_mapCheckRepeatEventReq.end() )
	{
		if( mit->second == NULL )
		{
			// �����Ͱ��� �̻��ϸ� event�� �����Ű�� �ʴ´�.
			return false;
		}

		if( mit->second->m_bRecvReq == false )
		{
			// REQ�� �ɷ����� �ʴٸ� true�� �ٲٰ� event�� ��� ��Ų��.
			mit->second->m_bRecvReq = true;
			return true;
		}
		else
		{
			// ���͸� ī��Ʈ�� 0�̶�� Ÿ�̸Ӹ� �ʱ�ȭ ��Ų��!
			if( mit->second->m_iFilteringCount == 0 )
			{
				mit->second->m_kTimer.restart();
			}

			// ���͸� ī��Ʈ�� ������Ų��.
			++mit->second->m_iFilteringCount;

			// ī��Ʈ�� ���� ���� �����Ѵ�.
			m_usCheckedEventID			= usEventID;
			m_iCheckedFilteringCount	= mit->second->m_iFilteringCount;
			m_fFilteringTime			= mit->second->m_kTimer.elapsed();

			// REQ�� �ɷ��ִٸ� event�� �����Ű�� �ʴ´�.
			return false;
		}
	}

	// ��ϵȰ� ���ٸ� �׳� �����Ų��
	return true;
}

void KRepeatEventCountFilter::CheckAckEvent( unsigned short usEventID )
{
	// req�� ��ϵȰ� ������ ack���� �̺�Ʈ�� ã�ƺ���
	KRepeatEventFilterMap::iterator mit;
	mit = m_mapCheckRepeatEventAck.find( usEventID );
	if( mit != m_mapCheckRepeatEventAck.end() )
	{
		if( mit->second == NULL )
		{
			// �����Ͱ��� �̻��ϸ� �ٷ� ����.
			return;
		}

		// ack�̺�Ʈ �߰�! false������ ����
		mit->second->m_bRecvReq = false;

		// ���͸� ī��Ʈ���� 0���� �ٲ۴�.
		mit->second->m_iFilteringCount = 0;
	}
}



