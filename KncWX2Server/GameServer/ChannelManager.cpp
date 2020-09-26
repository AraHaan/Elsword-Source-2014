#include "GSSimLayer.h"		// �ؿ��� �߰� �ǵ������� ������ ��������
#include "GameServer.h"
#include "ChannelManager.h"


ImplementSingleton( KChannelManager );

KChannelManager::KChannelManager()
:
m_iGameServChannelID( 0 ),
m_iCurPartyCount( 0 ),
m_iPlayGamePartyCount( 0 ),
//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
m_iMaxUser( 0 ),
#endif SERV_REALTIME_MAX_USER
//}}
m_fMaxWaitingTime( 0.0f )
{	
}

KChannelManager::~KChannelManager()
{
}

ImplToStringW( KChannelManager )
{
	return stm_;
}

void KChannelManager::Tick()
{
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

	std::vector< UidType > vecDeleteList;

	std::map< UidType, KChannelChangeUserInfo >::iterator mit;
	for( mit = m_mapChannelChangeUserList.begin(); mit != m_mapChannelChangeUserList.end(); ++mit )
	{
		double fWaitingTime = mit->second.m_tWaitingTimer.elapsed();
		if( fWaitingTime > 15.f )
		{
			vecDeleteList.push_back( mit->first );

			// ���� �����ɸ� ä���̵��ð��� ����
			if( m_fMaxWaitingTime < fWaitingTime )
			{
				m_fMaxWaitingTime = fWaitingTime;

				START_LOG( cout, L"ä���̵�����ð��� 15�ʰ� �Ѱ� �ɸ� WaitingTime : " << m_fMaxWaitingTime );
			}
		}
	}

	std::vector< UidType >::const_iterator vit;
	for( vit = vecDeleteList.begin(); vit != vecDeleteList.end(); ++vit )
	{
		m_mapChannelChangeUserList.erase( *vit );
	}

	//{{ 2009. 2. 13  ������	ä�θ���Ʈ
	if( m_kChannelListRefreshTimer.elapsed() > 60.0 )
	{
		KDBE_CHANNEL_LIST_REQ kPacketToDB;
		kPacketToDB.m_iServerGroupID = m_iServerGroupID;
		SendToLogDB( DBE_CHANNEL_LIST_REQ, kPacketToDB );

#ifdef SERV_ENTRY_POINT	// �ؿ��� �߰�
		// �������� 2�� ���������� ����
		if ( GetKGSSimLayer()->GetServerGroupNum() == KGSSimLayer::DOUBLE_SERVER_GROUP )
		{
			KDBE_CHANNEL_LIST_REQ kPacketToDB2nd;
			kPacketToDB2nd.m_iServerGroupID = ( m_iServerGroupID == SEnum::SGI_SOLES ? SEnum::SGI_GAIA : SEnum::SGI_SOLES );
			SendToLogDB2nd( DBE_CHANNEL_LIST_REQ, kPacketToDB2nd );
		}
#endif SERV_ENTRY_POINT

		m_kChannelListRefreshTimer.restart();
	}
	//}}

	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

void KChannelManager::Init( int iServerGroupID, int iChannelID, const KChannelBonusInfo& kInfo )
{
	m_iServerGroupID = iServerGroupID;
	m_iGameServChannelID = iChannelID;

	START_LOG( cout, L"���Ӽ��� ä��ID ���� �Ϸ�!" )
		<< BUILD_LOG( m_iServerGroupID )
		<< BUILD_LOG( m_iGameServChannelID );

	if( kInfo.m_bEnable )
	{
		m_kBonusInfo = kInfo;

		START_LOG( cout, L"���Ӽ��� ä�� ���ʽ� ����!" )
			<< BUILD_LOG( m_kBonusInfo.m_iBeginLv )
			<< BUILD_LOG( m_kBonusInfo.m_iEndLv )
			<< BUILD_LOG( m_kBonusInfo.m_iPerExp )
			<< BUILD_LOG( m_kBonusInfo.m_iPerED );
	}
}

void KChannelManager::UpdateChannelList( const std::map< int, KChannelInfo >& mapChannelList )
{
	m_mapChannelList.clear();
	m_vecChannelList.clear();

#ifdef SERV_CHANNEL_LIST_RENEWAL
	m_vecChannelInfo.clear();
#endif //SERV_CHANNEL_LIST_RENEWAL

	m_iCurPartyCount = 0;
	m_iPlayGamePartyCount = 0;
	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
	const UidType iServerUID = KBaseServer::GetKObj()->GetUID();
	bool bUpdatedMaxUser = false;
#endif SERV_REALTIME_MAX_USER
	//}}

	m_mapChannelList = mapChannelList;

	std::map< int, KChannelInfo >::iterator mit;
	for( mit = m_mapChannelList.begin(); mit != m_mapChannelList.end(); ++mit )
	{
		//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
		if( mit->second.m_iServerUID == iServerUID )
		{
			bUpdatedMaxUser = true;

			if( GetMaxUser() != mit->second.m_iMaxUser )
			{
				START_LOG( cout, L"ä�� �ִ� ���� �ο� ������ ����Ǿ����ϴ�!" )
					<< BUILD_LOG( iServerUID )
					<< BUILD_LOG( GetMaxUser() )
					<< BUILD_LOG( mit->second.m_iMaxUser );

                SetMaxUser( mit->second.m_iMaxUser );
			}
		}
#endif SERV_REALTIME_MAX_USER
		//}}

		// ä�� ����
		KChannelSimpleInfo kInfo;
		kInfo.m_iChannelID = mit->second.m_iChannelID;
		kInfo.m_iCurrentUser = mit->second.m_iCurrentUser;
		kInfo.m_iMaxUser = mit->second.m_iMaxUser;
		m_vecChannelList.push_back( kInfo );

		// ���� ��Ƽ �ο� ����
		m_iCurPartyCount += mit->second.m_iCurPartyCount;
		m_iPlayGamePartyCount += mit->second.m_iPlayGamePartyCount;

		mit->second.m_iCurPartyCount = 0;
		mit->second.m_iPlayGamePartyCount = 0;

		// 2012.09.11 lygan_������ // �̴ϸ� �� UI �ǽð� ���ſ� ����
#ifdef SERV_CHANNEL_LIST_RENEWAL
		m_vecChannelInfo.push_back(mit->second);
#endif //SERV_CHANNEL_LIST_RENEWAL

	}

	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
	if( bUpdatedMaxUser == false )
	{
		START_LOG( cerr, L"ä�θ���Ʈ�� ���� ä�� ������ �����ϴ�!" )
			<< BUILD_LOG( iServerUID )
			<< BUILD_LOG( mapChannelList.size() )
			<< END_LOG;
	}
#endif SERV_REALTIME_MAX_USER
	//}}
}

#ifdef SERV_ENTRY_POINT
void KChannelManager::UpdateChannelList2nd( const std::map< int, KChannelInfo >& mapChannelList_ )
{
    m_mapChannelList2nd.clear();
    m_mapChannelList2nd = mapChannelList_;

    std::map< int, KChannelInfo >::iterator mit;
    for( mit = m_mapChannelList2nd.begin(); mit != m_mapChannelList2nd.end(); ++mit )
    {
        KChannelSimpleInfo kInfo;
        kInfo.m_iChannelID = mit->second.m_iChannelID;
        kInfo.m_iCurrentUser = mit->second.m_iCurrentUser;
        kInfo.m_iMaxUser = mit->second.m_iMaxUser;
        m_vecChannelList2nd.push_back( kInfo );

    }

}

#endif SERV_ENTRY_POINT



//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
void KChannelManager::UpdateChannelBonusList( const std::map< int, KChannelBonusInfo >& mapChannelBonusList )
{
	m_mapChannelBonusList.clear();

	m_mapChannelBonusList = mapChannelBonusList;

	std::map< int, KChannelBonusInfo >::const_iterator mit;
	mit = mapChannelBonusList.find( m_iGameServChannelID );

	if( mit == mapChannelBonusList.end() )
	{
#if defined (SERV_COUNTRY_EU) || defined (SERV_COUNTRY_IN)
			//������ ä�� ���ʽ��� ����.
#else //SERV_COUNTRY_EU
		START_LOG(cerr, L"���Ӽ��� ä�� ���ʽ� ���� �Ҽ��� ����")
			<< BUILD_LOG( m_iGameServChannelID )
			<< BUILD_LOG( m_mapChannelBonusList.size() )
			<< END_LOG;
#endif //defined (SERV_COUNTRY_EU) || defined (SERV_COUNTRY_IN)
		return;
	}
	else
	{
		m_kBonusInfo = mit->second;
	}

}

#endif SERV_CHANNEL_LIST_RENEWAL
//}}

#ifdef SERV_ENTRY_POINT
void KChannelManager::UpdateChannelBonusList2nd( const std::map< int, KChannelBonusInfo >& mapChannelBonusList_ )
{
    m_mapChannelBonusList2nd.clear();

    m_mapChannelBonusList2nd = mapChannelBonusList_;

}
#endif SERV_ENTRY_POINT

bool KChannelManager::GetServerUIDByChannelID( int iChannelID, UidType& iServerUID )
{
	std::map< int, KChannelInfo >::const_iterator mit;
	mit = m_mapChannelList.find( iChannelID );
	if( mit == m_mapChannelList.end() )
		return false;

	iServerUID = mit->second.m_iServerUID;
	return true;
}

bool KChannelManager::NewChannelChangeUser( UidType iUnitUID, const KChannelChangeInfo& kInfo )
{
	if( IsExistChannelChangeUser( iUnitUID ) )
	{
		START_LOG( cerr, L"�̹� ä���̵� ������ �Ǿ��ִ� ������" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	KChannelChangeUserInfo kReserveInfo( kInfo );
	m_mapChannelChangeUserList.insert( std::make_pair( iUnitUID, kReserveInfo ) );
	return true;
}

bool KChannelManager::DelChannelChangeUser( UidType iUnitUID, KChannelChangeInfo& kInfo )
{
	std::map< UidType, KChannelChangeUserInfo >::iterator mit = m_mapChannelChangeUserList.find( iUnitUID );
	if( mit == m_mapChannelChangeUserList.end() )
	{
		START_LOG( cerr, L"��ϵ��� ���� ���� ������ �����Ϸ��� �߽��ϴ�" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	// ���� �����ɸ� ä���̵��ð��� ����
	double fWaitingTime = mit->second.m_tWaitingTimer.elapsed();
	if( fWaitingTime > 5.f )
	{
		if( m_fMaxWaitingTime < fWaitingTime )
		{
			m_fMaxWaitingTime = fWaitingTime;

			START_LOG( cout, L"ä���̵��ð��� 5�ʰ� ���� �ִ�ð� ���� WaitingTime : " << m_fMaxWaitingTime );
		}
	}

	// �����س��Ҵ� ä���̵��� �������� ���� ���
	kInfo = mit->second.m_kChannelChangeInfo;

	// ���� ���� ����
	m_mapChannelChangeUserList.erase( mit );
	return true;
}

bool KChannelManager::DelChannelChangeUser( UidType iUnitUID )
{
	KChannelChangeInfo kInfo;
    return DelChannelChangeUser( iUnitUID, kInfo );
}

void KChannelManager::GetChannelBonus( u_char ucLevel, float& fAddExpRate, float& fAddEDRate )
{
	fAddExpRate = 0.0f;
	fAddEDRate = 0.0f;

	if( m_kBonusInfo.m_bEnable == false )
		return;

	if( static_cast<int>(ucLevel) < m_kBonusInfo.m_iBeginLv  ||  static_cast<int>(ucLevel) > m_kBonusInfo.m_iEndLv )
		return;

	fAddExpRate = ( static_cast<float>(m_kBonusInfo.m_iPerExp) / 100.f );
	fAddEDRate = ( static_cast<float>(m_kBonusInfo.m_iPerED) / 100.f );
}

void KChannelManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
void KChannelManager::GetServerUIDByChannelName( OUT std::wstring& wstrName )
{
	std::map< int, KChannelInfo >::iterator mit = m_mapChannelList.find( m_iGameServChannelID );
	if( mit != m_mapChannelList.end() )
	{
		wstrName = mit->second.m_wstrChannelName;
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}
