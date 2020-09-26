#include ".\squarelist.h"

NiImplementRootRTTI( KSquareList );

KSquareList::KSquareList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum )
: KRoomList( uiRoomListID, eRoomListType, uiPageNum )
{
}

KSquareList::~KSquareList(void)
{
	m_vecSquareList.clear();
}

ImplOstmOperatorW( KSquareList );
ImplToStringW( KSquareList )
{
	return stm_ << GetRTTI()->GetName()
		<< L", SQUARE CHANNEL ID-" << m_uiRoomListID
		<< std::endl;
}

//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID

bool KSquareList::AddSquareInfo( KSquareInfoPtr spSquareInfo_ )
{
	// �߰��Ϸ��� ���������� �Ŵ������� üũ�Ͽ� �ߺ��� ���ٰ� ���� üũ��ƾ�� ����.
	m_vecSquareList.push_back( spSquareInfo_ );

	//////////////////////////////////////////////////////////////////////////
	//�ؼ��ͼ��� �����Ƿ� ���帮��Ʈ�� �����Ѵ�.
	std::vector< KSquareInfoPtr > vecTemp = m_vecSquareList;
	m_vecSquareList.clear();

	BOOST_TEST_FOREACH( KSquareInfoPtr, spAddSquareInfo, vecTemp )
	{
		if( spAddSquareInfo == NULL )
		{
			START_LOG( cerr, L"���� ������ �̻��մϴ�!" )
				<< BUILD_LOG( vecTemp.size() )
				<< END_LOG;
			continue;
		}

		if( m_vecSquareList.empty() == true )
		{
			m_vecSquareList.push_back( spAddSquareInfo );
		}
		else
		{
			std::vector< KSquareInfoPtr >::iterator vit;
			for( vit = m_vecSquareList.begin(); vit != m_vecSquareList.end(); ++vit )
			{
				if( (*vit)->m_cSortIndex > spAddSquareInfo->m_cSortIndex )
				{
					m_vecSquareList.insert( vit, spAddSquareInfo );
					break;
				}
			}

			if( vit == m_vecSquareList.end() )
			{
				m_vecSquareList.push_back( spAddSquareInfo );
			}
		}
	}
	vecTemp.clear();
	//////////////////////////////////////////////////////////////////////////

	// TO DO..
	//���� Ÿ�Ժ� ����Ʈ �����߰�..(����)

	return true;
}

#else

bool KSquareList::AddSquareInfo( KSquareInfoPtr spSquareInfo_ )
{
	// �߰��Ϸ��� ���������� �Ŵ������� üũ�Ͽ� �ߺ��� ���ٰ� ���� üũ��ƾ�� ����.
	m_vecSquareList.push_back( spSquareInfo_ );

	//////////////////////////////////////////////////////////////////////////
	//�ؼ��ͼ��� �����Ƿ� ���帮��Ʈ�� �����Ѵ�.
	std::vector< KSquareInfoPtr > vecTemp = m_vecSquareList;
	m_vecSquareList.clear();

	for( int i = 0; i < (int)vecTemp.size(); ++i )
	{
		if( m_vecSquareList.empty() == true )
			m_vecSquareList.push_back( vecTemp[i] );
		else
		{
			std::vector< KSquareInfoPtr >::iterator vit;
			for( vit = m_vecSquareList.begin(); vit != m_vecSquareList.end(); ++vit )
			{
				if( (*vit)->m_RelayIP.compare( vecTemp[i]->m_RelayIP ) > 0 )
				{
					m_vecSquareList.insert( vit, vecTemp[i] );
					break;
				}
			}

			if( vit == m_vecSquareList.end() )
				m_vecSquareList.push_back( vecTemp[i] );
		}
	}
	vecTemp.clear();
	//////////////////////////////////////////////////////////////////////////

	// TO DO..
	//���� Ÿ�Ժ� ����Ʈ �����߰�..(����)

	return true;
}

#endif SERV_SQUARE_LIST_SERV_UID
//}}

bool KSquareList::DelSquareInfo( KSquareInfoPtr spSquareInfo_ )
{
	std::vector< KSquareInfoPtr >::iterator vit;
	vit = std::find( m_vecSquareList.begin(), m_vecSquareList.end(), spSquareInfo_ );

	if( vit == m_vecSquareList.end() )
	{
		START_LOG( cerr, L"�����ϰ��� �ϴ� ���������� ����." )
			<< BUILD_LOG( spSquareInfo_->m_cSquareType )
			<< BUILD_LOG( spSquareInfo_->m_iSquareUID )
			<< BUILD_LOG( spSquareInfo_->m_wstrRoomName )
			<< END_LOG;

		return false;
	}
	else
	{
		m_vecSquareList.erase( vit );
	}

	//TO DO..
	//����Ÿ�Ժ� ����Ʈ �������ֱ�(����)

	return true;
}

bool KSquareList::GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq_, OUT KEGS_SQUARE_LIST_ACK& kAck_ )
{
	std::vector< KSquareInfoPtr >* pvecSquareList;
	pvecSquareList = &m_vecSquareList;

	//���� �߰��� ����Ÿ�Ժ� ������ ������ �ּ� �״�� �д�.
	switch( kReq_.m_cSquareType )
	{
	case CXSLSquareGame::ST_AREA:
		//pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_TEAM];
		break;

	case CXSLSquareGame::ST_AGE:
		//pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH];
		break;

	case CXSLSquareGame::ST_FREE:
		//pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL];
		break;
	}

	if( pvecSquareList->empty() == true )
	{
		kAck_.m_nTotalPage = 1;
		kAck_.m_nViewPage = 1;

		return true;
	}

	std::vector< KSquareInfoPtr >::iterator vit;

	UINT uiTotalPage;
	uiTotalPage = pvecSquareList->size() / GetPageNum();

	if( uiTotalPage == 0 )
	{
		++uiTotalPage;
	}

	// ������ �������� �븮��Ʈ ������ ���������� �������� ���� ��� �������� �߰��Ѵ�.
	if( ((pvecSquareList->size() % GetPageNum()) > 0) && (pvecSquareList->size() > GetPageNum()) )
	{
		++uiTotalPage;
	}

	//��û�� �������� ������ �������� �Ѿ��� ���
	//������ �������� ���Ե� �������� �ȴ�.
	if( kReq_.m_nViewPage > uiTotalPage )
	{
		kReq_.m_nViewPage = uiTotalPage;
	}

	int nS = GetPageNum() * ( kReq_.m_nViewPage - 1 );
	int nE = nS + GetPageNum();

	KSquareInfo kRoomInfo;

	kAck_.m_nTotalPage	= uiTotalPage;
	kAck_.m_nViewPage	= kReq_.m_nViewPage;
	for(; nS < nE; ++nS )
	{
		if( nS < (int)pvecSquareList->size() )
		{
			kRoomInfo = *((*pvecSquareList)[nS]);
			kAck_.m_vecSquareInfo.push_back( kRoomInfo );
		}
		else
		{
			if( nS != (int)pvecSquareList->size() )
			{
				START_LOG( cerr, L"������ ��û����.!" )
					<< BUILD_LOG( GetPageNum() )
					<< BUILD_LOG( nS )
					<< BUILD_LOG( nE )
					<< BUILD_LOG( pvecSquareList->size() )
					<< BUILD_LOG( m_vecSquareList.size() )
					<< END_LOG;
			}

			break;
		}
	}

	return true;
}



