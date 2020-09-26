#include "GSUser.h"
#include "HackingProtectionManager.h"
//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
#include "GameSysVal/GameSysVal.h"
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST


KUserHackingManager::KUserHackingManager(void)
{
	// �ʱⰪ ����
	SetRandomCheckTime();

	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	m_bCheckedDeserializeFail = false;
	m_iTotalDeserializeFailCount = 0;
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	m_setRecvLetterList.clear();
	m_setRecvTradeList.clear();
#endif SERV_MONITORING_LETTER_AND_TRADE
	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	m_bEDMonitoringMode = false;
#endif SERV_ED_MONITORING_IN_GAME
	//}}
	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	m_cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
	m_tHackingUserTimer.restart();
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
}

KUserHackingManager::~KUserHackingManager(void)
{
}

void KUserHackingManager::SetRandomCheckTime()
{
	int iRandNum = rand() % static_cast<int>(HE_MAX_TIME - HE_MIN_TIME);
	iRandNum += static_cast<int>(HE_MIN_TIME);

	m_fHackingToolCheckTime = static_cast<double>(iRandNum);
}

void KUserHackingManager::GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList )
{
	// ��ŷ�� ����Ʈ�� ��´�.
	SiKHackingProtectionManager()->GetHackingToolList( iRandomKey, vecHackingToolList );

	// ���� üũ Ÿ���� �����Ѵ�.
    SetRandomCheckTime();    
}

//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
bool KUserHackingManager::AddLetterMonitoringList( IN const UidType& iForomUnit )
{
	m_setRecvLetterList.insert( iForomUnit );

	if( m_setRecvLetterList.size() >= (u_int)SiKGameSysVal()->GetLimitsCountOfLetter() )
	{
		return true;
	}
	return false;
}

bool KUserHackingManager::AddTradeMonitoringList( IN const UidType& iForomUnit )
{
	m_setRecvTradeList.insert( iForomUnit );

	if( m_setRecvTradeList.size() >= (u_int)SiKGameSysVal()->GetLimitsCountOfTrade() )
	{
		return true;
	}
	return false;
}
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
bool KUserHackingManager::HackingCheckDeserializeFailCount()
{
	// �̹� üũ�Ǿ��ٸ�!
	if( m_bCheckedDeserializeFail )	
        return false;

	// Deserialize üũ�ؼ� �̻������ ������ �ʱ�ȭ!
	const bool bRet = ( m_iTotalDeserializeFailCount > SiKGameSysVal()->GetDeserializeFailCheckCount() );
	if( bRet )
	{
		START_LOG( cout, L"------------------------- Deserialize Fail Statistics -------------------------" );

		std::map< u_short, int >::const_iterator mit;
		for( mit = m_mapDeserializeFailList.begin(); mit != m_mapDeserializeFailList.end(); ++mit )
		{
			START_LOG( cout, KEvent::GetIDStr( mit->first ) )
				<< BUILD_LOG( mit->second );
		}

		m_bCheckedDeserializeFail = true;
	}
	
	m_iTotalDeserializeFailCount = 0;
	m_mapDeserializeFailList.clear();
	return bRet;
}

void KUserHackingManager::IncreaseDeserializeFailCount( IN const u_short usEventID )
{
	// Ư�� ��Ŷ Ÿ�Կ� ���ؼ��� ����!
	switch( usEventID )
	{
	case EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT:
		break;

	default:
		return;
	}

	// ��ü ī��Ʈ 1����!
	++m_iTotalDeserializeFailCount;

	// ��� ����
	std::map< u_short, int >::iterator mit;
	mit = m_mapDeserializeFailList.find( usEventID );
	if( mit == m_mapDeserializeFailList.end() )
	{
		m_mapDeserializeFailList.insert( std::make_pair( usEventID, 1 ) );
	}
	else
	{
		++mit->second;
	}
}
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
//}}

//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
bool KUserHackingManager::CheckHackingUserRecivedPacket( OUT char& cLastSendType )
{
	if( m_cLastSendType != KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE )
	{
		if( m_tHackingUserTimer.elapsed() < 10 )
		{
			return true;
		}

		// 10�� ���� ������ ����
		cLastSendType = m_cLastSendType;
		m_cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
		m_tHackingUserTimer.restart();
		return false;
	}
	return true;
}

void KUserHackingManager::SetHackingUserSendType( char cType )
{
	m_cLastSendType = cType;
	m_tHackingUserTimer.restart();
}

bool KUserHackingManager::SetHackingUserReciveType( IN char cType, OUT char& cLastSendType )
{
	cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;

	// ���� �����Ƿ� �ʱ�ȭ ����
	if( m_cLastSendType == cType )
	{
		m_cLastSendType		= KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
		m_tHackingUserTimer.restart();
		return true;
	}
	else
	{
		// �߸��� ������ �Դ�
		cLastSendType = m_cLastSendType;
		m_cLastSendType		= KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
		m_tHackingUserTimer.restart();
		return false;
	}
}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}


#endif SERV_HACKING_TOOL_LIST
//}}