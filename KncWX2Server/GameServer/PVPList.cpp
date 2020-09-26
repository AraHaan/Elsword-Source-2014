#include ".\pvplist.h"

NiImplementRootRTTI( KPVPList );

KPVPList::KPVPList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum )
:KRoomList( uiRoomListID, eRoomListType, uiPageNum ),
m_ePVPChannelClass( KPVPChannelInfo::PCC_NONE )
{
}

KPVPList::~KPVPList(void)
{
	m_vecRoomList.clear();
	m_vecTypeRoomList[CXSLRoom::PGT_TEAM].clear();
	m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].clear();
	m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].clear();
}

ImplOstmOperatorW( KPVPList );
ImplToStringW( KPVPList )
{
	return stm_ << GetRTTI()->GetName()
		<< L", PVP Room List ID-" << m_uiRoomListID
		<< std::endl;
}

UidType KPVPList::GetWaitingRoomUID( IN KEGS_QUICK_JOIN_REQ& kReq )
{
	std::vector< KRoomInfoPtr >::iterator vit;
	KRoomInfoPtr spRoomInfo;

	std::vector< UidType > vecRoomUID;

	switch( kReq.m_iOption )
	{
	case CXSLRoom::PGT_TEAM:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_TEAM].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM].end(); ++vit )
			{
				spRoomInfo = ( *vit );
				if( !spRoomInfo )
				{
					START_LOG( cerr, L"�� ���� ������ �̻�." );
					continue;
				}

				if( spRoomInfo->m_RoomState == CXSLRoom::RS_WAIT && ( spRoomInfo->m_MaxSlot > spRoomInfo->m_JoinSlot ) )
				{
					// �������� ������ �̸� �������� ���߿��� ����
#ifdef DUNGEON_ITEM
					if( kReq.m_bIsItemMode == true )
					{
						if( spRoomInfo->m_bIsItemMode == true )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
					}
					// �ƴϸ� �Ŀ��� ���߿��� ����!
					else
					{
						//{{ 2010. 10. 12	������	������ ������ ���� ����
						if( spRoomInfo->m_bIsItemMode == false )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
						//}}
					}
#else
					vecRoomUID.push_back( spRoomInfo->m_RoomUID );
#endif DUNGEON_ITEM
				}
			}

			int iNumRoom = ( int )vecRoomUID.size();
			if( iNumRoom > 0 )
			{
				int iIndex = rand() % iNumRoom;
				_JIF( iIndex < iNumRoom, iIndex = 0 );
				return vecRoomUID[iIndex];
			}
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
	case CXSLRoom::PGT_SURVIVAL:
		{
			for( vit = m_vecTypeRoomList[kReq.m_iOption].begin(); vit != m_vecTypeRoomList[kReq.m_iOption].end(); ++vit )
			{
				spRoomInfo = ( *vit );
				if( !spRoomInfo )
				{
					START_LOG( cerr, L"�� ���� ������ �̻�." );
					continue;
				}

				if( spRoomInfo->m_MaxSlot > spRoomInfo->m_JoinSlot )
				{
					//vecRoomUID.push_back( spRoomInfo->m_RoomUID );
					// �������� ������ �̸� �������� ���߿��� ����
#ifdef DUNGEON_ITEM
					if( kReq.m_bIsItemMode == true )
					{
						if( spRoomInfo->m_bIsItemMode == true )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
					}
					// �ƴϸ� �Ŀ��� ���߿��� ����!
					else
					{
						//{{ 2010. 10. 12	������	������ ������ ���� ����
						if( spRoomInfo->m_bIsItemMode == false )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
						//}}
					}
#else
					vecRoomUID.push_back( spRoomInfo->m_RoomUID );
#endif DUNGEON_ITEM
				}
			}

			int iNumRoom = ( int )vecRoomUID.size();
			if( iNumRoom > 0 )
			{
				int iIndex = rand() % iNumRoom;
				_JIF( iIndex < iNumRoom, iIndex = 0 );
				return vecRoomUID[iIndex];
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"����ä�� Ÿ���� �̻���.!" )
				<< BUILD_LOG( GetID() )
				<< BUILD_LOG( GetType() )
				<< BUILD_LOG( kReq.m_iOption )
#ifdef DUNGEON_ITEM
				<< BUILD_LOG( static_cast<int>(kReq.m_bIsItemMode) )
#endif DUNGEON_ITEM
				<< END_LOG;				
		}
		break;
	}

	return 0;
}

bool KPVPList::AddRoomInfo( KRoomInfoPtr spRoomInfo_ )
{
	// ���߰��Ϸ��� �������� �Ŵ������� üũ�Ͽ� �ߺ��� ���ٰ� ���� üũ��ƾ�� ����.

	// ���� ������� �������� ���� �ֱ� ����.
	PreInsertVector( m_vecRoomList, spRoomInfo_ );

	//��������ϰ�� ����Ÿ�Ժ� ä�ΰ����� ���� �����صд�.
	switch( spRoomInfo_->m_PVPGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			PreInsertVector( m_vecTypeRoomList[CXSLRoom::PGT_TEAM], spRoomInfo_ );
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			PreInsertVector( m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH], spRoomInfo_ );
		}
		break;

	case CXSLRoom::PGT_SURVIVAL:
		{
			PreInsertVector( m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL], spRoomInfo_ );
		}
		break;

	default:
		{
			START_LOG( cerr, L"����Ÿ�Ժ� ������ �߰��� �̻���." )
				<< BUILD_LOGc( spRoomInfo_->m_PVPGameType )
				<< END_LOG;

			return false;
		}
	}

	return true;
}

bool KPVPList::DelRoomInfo( KRoomInfoPtr spRoomInfo_ )
{
	std::vector< KRoomInfoPtr >::iterator vit;

	for( vit = m_vecRoomList.begin(); vit != m_vecRoomList.end(); ++vit )
	{
		if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
		{
			break;
		}
	}

	if( vit == m_vecRoomList.end() )
	{
		START_LOG( cerr, L"�����ϰ��� �ϴ� �������� ����." )
			<< BUILD_LOG(	spRoomInfo_->m_RoomUID )
			<< BUILD_LOG(	spRoomInfo_->m_uiRoomListID )
			<< BUILD_LOG(	spRoomInfo_->m_RoomName )
			<< BUILD_LOGc(	spRoomInfo_->m_RoomState )
			<< END_LOG;

		return false;
	}
	else
	{
		m_vecRoomList.erase( vit );
	}

	switch( spRoomInfo_->m_PVPGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_TEAM].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM].end(); ++vit )
			{
				if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
				{
					break;
				}
			}

			if( vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM].end() )
			{
				m_vecTypeRoomList[CXSLRoom::PGT_TEAM].erase( vit );
			}
			else
			{
				START_LOG( cerr, L"�����ҷ��� Ÿ�Ժ� �������� ����." )
					<< L"CXSLRoom::PGT_TEAM" << dbg::endl
					<< BUILD_LOG( spRoomInfo_->m_RoomUID )
					<< BUILD_LOG( spRoomInfo_->m_uiRoomListID )
					<< BUILD_LOG( spRoomInfo_->m_RoomName )
					<< BUILD_LOGc( spRoomInfo_->m_RoomState )
					<< END_LOG;
			}
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].end(); ++vit )
			{
				if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
				{
					break;
				}
			}

			if( vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].end() )
			{
				m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].erase( vit );
			}
			else
			{
				START_LOG( cerr, L"�����ҷ��� Ÿ�Ժ� �������� ����." )
					<< L"CXSLRoom::PGT_TEAM_DEATH" << dbg::endl
					<< BUILD_LOG(	spRoomInfo_->m_RoomUID )
					<< BUILD_LOG(	spRoomInfo_->m_uiRoomListID )
					<< BUILD_LOG(	spRoomInfo_->m_RoomName )
					<< BUILD_LOGc(	spRoomInfo_->m_RoomState )
					<< END_LOG;
			}
		}
		break;

	case CXSLRoom::PGT_SURVIVAL:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].end(); ++vit )
			{
				if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
				{
					break;
				}
			}

			if( vit != m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].end() )
			{
				m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].erase( vit );
			}
			else
			{
				START_LOG( cerr, L"�����ҷ��� Ÿ�Ժ� �������� ����." )
					<< L"CXSLRoom::PGT_SURVIVAL" << dbg::endl
					<< BUILD_LOG(	spRoomInfo_->m_RoomUID )
					<< BUILD_LOG(	spRoomInfo_->m_uiRoomListID )
					<< BUILD_LOG(	spRoomInfo_->m_RoomName )
					<< BUILD_LOGc(	spRoomInfo_->m_RoomState )
					<< END_LOG;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"�����ҷ��� ���� ��Ÿ���� �̻���.!" )
				<< BUILD_LOGc( spRoomInfo_->m_PVPGameType )
				<< END_LOG;

			return false;
		}
	}

	//�Ŵ������� ����� �Լ�(DeleteRoomInfo())�� ���ؼ� ������ ������ ����ī���Ͱ� 3�̸�
	//�̸� �ѱ�� �ȵǴ�.
	if( spRoomInfo_.use_count() > 3 )
	{
		START_LOG( cerr, L"RoomInfo Invalid Ref-count." )
			<< BUILD_LOG( spRoomInfo_.use_count() )
			<< BUILD_LOG( spRoomInfo_->m_RoomName )
			<< BUILD_LOG( spRoomInfo_->m_RoomUID )
			<< BUILD_LOG( GetType() )
			;
	}
	START_LOG( clog, L" Ref-count" )
		<< BUILD_LOG( spRoomInfo_.use_count() );

	return true;
}

bool KPVPList::GetRoomInfoPage( IN KEGS_ROOM_LIST_REQ& kReq_, OUT KEGS_ROOM_LIST_ACK& kAck_ )
{
	std::vector< KRoomInfoPtr >* pvecRoomList;
	pvecRoomList = &m_vecRoomList;

	switch( kReq_.m_cPVPGameType )
	{
	case CXSLRoom::PGT_TEAM:
		pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_TEAM];
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH];
		break;

	case CXSLRoom::PGT_SURVIVAL:
		pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL];
		break;
	}

	if( pvecRoomList->empty() == true )
	{
		kAck_.m_nTotalPage = 1;
		kAck_.m_nViewPage = 1;

		return true;
	}

	std::vector< KRoomInfoPtr >::iterator vit;

	UINT uiTotalPage = 0;
	uiTotalPage = pvecRoomList->size() / GetPageNum();

	if( uiTotalPage <= 0 )
	{
		uiTotalPage = 1;
	}

	// ������ �������� �븮��Ʈ ������ ���������� �������� ���� ��� �������� �߰��Ѵ�.
	if( ((pvecRoomList->size() % GetPageNum()) > 0) && (pvecRoomList->size() > GetPageNum()) )
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

	KRoomInfo kRoomInfo;

	kAck_.m_nTotalPage	= uiTotalPage;
	kAck_.m_nViewPage	= kReq_.m_nViewPage;
	for(; nS < nE; ++nS )
	{
		if( nS < (int)pvecRoomList->size() && nS >= 0 )
		{
			kRoomInfo = *((*pvecRoomList)[nS]);
			kAck_.m_vRoomInfo.push_back( kRoomInfo );
		}
		else
		{
			if( nS != (int)pvecRoomList->size() )
			{
				START_LOG( cerr, L"������ ��û����.!" )
					<< BUILD_LOG( GetPageNum() )
					<< BUILD_LOG( nS )
					<< BUILD_LOG( nE )
					<< BUILD_LOG( pvecRoomList->size() )
					<< BUILD_LOG( m_vecRoomList.size() )
					<< END_LOG;
			}

			break;
		}
	}

	if( kAck_.m_vRoomInfo.size() > GetPageNum() )
	{
		START_LOG( cerr, L"������ ���� �̻�.!(PVP)" )
			<< BUILD_LOG( kAck_.m_vRoomInfo.size() )
			<< BUILD_LOG( GetPageNum() )
			<< BUILD_LOG( nS )
			<< BUILD_LOG( nE )
			<< BUILD_LOG( pvecRoomList->size() )
			<< BUILD_LOG( (int)m_ePVPChannelClass )
			<< END_LOG;
	}

	return true;
}



