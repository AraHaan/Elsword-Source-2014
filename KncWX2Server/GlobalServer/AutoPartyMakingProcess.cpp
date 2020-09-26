#include "AutoPartyMakingProcess.h"
#include "NetError.h"

#include "GlobalServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "X2Data/XSLRoom.h"
#include "AutoPartyFSM.h"
#include "AutoPartyUserFSM.h"
//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#else
	#include "AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}

#include "X2Data/XSLDungeon.h"


//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM



class KAutoPartyUidDispenser
{
public:
	KAutoPartyUidDispenser()
	{
		m_iLastUID = 0;
	}
	~KAutoPartyUidDispenser()
	{
	}

	UidType GetNewAutoPartyUID()
	{
		return ++m_iLastUID;
	}

private:
	UidType		m_iLastUID;
};


static KAutoPartyUidDispenser g_autoPartyUidDispenser;


//////////////////////////////////////////////////////////////////////////
KAutoPartyCompleteTimeManager::KAutoPartyCompleteTimeManager()
{
}

KAutoPartyCompleteTimeManager::~KAutoPartyCompleteTimeManager()
{
}

//{{ ���� �ڵ� ��Ī ���� �ð� ���� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
void KAutoPartyCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	KAutoPartyTime kTemp;
	kTemp.m_tAutoPartyTime = CTime::GetCurrentTime();
	kTemp.m_iWatiTime = iCompleteTime;	

	m_listCompleteTime.push_back( kTemp );

	// 12���� �ɶ����� �������� �����.
	while( CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KAutoPartyCompleteTimeManager::GetAutoPartyMakingEstimatedTime() const
{
	// �ѹ��� ��ġ�� ���������� ����
	if( m_listCompleteTime.empty() )
		return 2000; // Ŭ���̾�Ʈ���� 30�� �̻��̸� '�˼� ����'���� ǥ���Ѵ�

	int iSumCompleteTime = 0;												// �ֱ� ���� ��� �ð� �� ��
	int iEstimatedTime	 = 0;												// �ֱ� ���� ��� �ð� ���
	int	iSumCount		 = 0;												// ��� �߰��� Ƚ��
	CTime tCurr = CTime::GetCurrentTime();									// ���� �ð�
	CTime tTwoHourAgo = tCurr - CTimeSpan( 0, 2, 0, 0 );					// ���� �ð����� �νð� �� �ð�

	BOOST_TEST_FOREACH( const KAutoPartyTime, kCompleteTime, m_listCompleteTime )
	{
		if( tTwoHourAgo <= kCompleteTime.m_tAutoPartyTime )	// �ֱ� 2�ð� �̳� ���
		{
			iSumCompleteTime += kCompleteTime.m_iWatiTime;
			++iSumCount;
		}
	}

	if( iSumCount <= 0 )	// ��꿡 �߰� �� Ƚ���� ���� -> ��Ī ���� ������ ������ 2�ð��� ���� �����̹Ƿ� ������
	{
		return 2000; // Ŭ���̾�Ʈ���� 30�� �̻��̸� '�˼� ����'���� ǥ���Ѵ�
	}

	iEstimatedTime = ( iSumCompleteTime / iSumCount );
	const int iMiddleResult = iEstimatedTime % 10; // 10�ʴ��� �ø�
	if( iMiddleResult > 0 )
	{
		iEstimatedTime = ( iEstimatedTime - iMiddleResult ) + 10;			// ��հ��� �ø��Ѵ�.
	}

	return iEstimatedTime;
}

#else

void KAutoPartyCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	m_listCompleteTime.push_back( iCompleteTime );

	// 5���� �ɶ����� �������� �����.
	while( COMPLETE_TIME_ENUM::CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KAutoPartyCompleteTimeManager::GetAutoPartyMakingEstimatedTime() const
{
	// �ѹ��� ��ġ�� ���������� ����
	if( m_listCompleteTime.empty() )
	return 10; // ����Ʈ�� 10��

	const int iCompleteCount = static_cast<int>(m_listCompleteTime.size());
	int iSumCompleteTime = 0;

	BOOST_TEST_FOREACH( const int, iTime, m_listCompleteTime )
	{
		iSumCompleteTime += iTime;
	}

	int iEstimatedTime = ( iSumCompleteTime / iCompleteCount );
	const int iMiddleResult = iEstimatedTime % 10; // 10�ʴ��� �ø�
	if( iMiddleResult > 0 )
	{
		iEstimatedTime = ( iEstimatedTime - iMiddleResult ) + 10;
	}

	return iEstimatedTime;
}
#endif SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
//}}



bool KAutoPartyMakingProcess::GetListTypeByLevelCondition( IN const u_char ucLevel, OUT int& iListType ) const
{
	iListType = 0;

	BOOST_TEST_FOREACH( const SLevelCondition&, sCondition, m_vecLevelCondition )
	{
		if( sCondition.m_iBeginLevel > ucLevel  ||  sCondition.m_iEndLevel < ucLevel )
			continue;
        
		iListType = sCondition.m_iListType;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

UidType KAutoPartyMakingProcess::ExtractDungeonIDAtWaitNumber( IN const UidType iUID_ )
{
    return ( iUID_ >> 40 ) & ( UidType )0x0000000000ffffff;
}

UidType KAutoPartyMakingProcess::ExtractDungeonModeAtWaitNumber( IN const UidType iUID_ )
{
	return ( iUID_ >> 32 ) & ( UidType )0x00000000000000ff;
}

void KAutoPartyMakingProcess::SetDungeonIDAtWaitNumber( UidType& iDestUID_, IN const UidType iSrcUID_ )
{
	iDestUID_ = ( iDestUID_ & 0x000000ffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x0000000000ffffff ) << 40 ) & 0xffffff0000000000 );
}

void KAutoPartyMakingProcess::SetDungeonModeAtWaitNumber( UidType& iDestUID_, IN const UidType iSrcUID_ )
{
	iDestUID_ = ( iDestUID_ & 0xffffff00ffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000000ff ) << 32 ) & 0x000000ff00000000 );
}

//////////////////////////////////////////////////////////////////////////


FSMclassPtr KAutoPartyMakingProcess::ms_spUserFSM = KAutoPartyUserFSMPtr( new KAutoPartyUserFSM );

KAutoPartyMakingProcess::KAutoPartyMakingProcess( IN const KAutoPartyMakingProcessCondition& kInfo ) :
m_iLastWaitNumber( 0 ),

m_iDungeonMode( kInfo.m_iDungeonMode ),
m_bActivatedAutoPartyBonus( false )
{
	if( m_iDungeonMode == CXSLDungeon::DM_SECRET_NORMAL || m_iDungeonMode == CXSLDungeon::DM_SECRET_HELL )
	{
		// ���
		m_iDungeonID = kInfo.m_iDungeonIDWithDif;
		m_iDifficultyLevel = 0;
	}
	else
	{	
		// �Ϲ� ����
		m_iDungeonID = ( ( kInfo.m_iDungeonIDWithDif / 10 ) * 10 );
		m_iDifficultyLevel = ( kInfo.m_iDungeonIDWithDif % 10 );
	}

	AddLevelCondition( kInfo.m_iBeginLevel, kInfo.m_iEndLevel );

	// ���ʷ� �����Ǵ� ������ LastWaitNumber���� ó���Ѵ�.
	SetDungeonIDAtWaitNumber( m_iLastWaitNumber, m_iDungeonID + m_iDifficultyLevel );
	SetDungeonModeAtWaitNumber( m_iLastWaitNumber, m_iDungeonMode );
}

KAutoPartyMakingProcess::~KAutoPartyMakingProcess()
{
}

void KAutoPartyMakingProcess::ClearAllAutoPartyInfo()
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout2, L"[���!] ��� �ڵ� ��Ƽ ������ �ʱ�ȭ ��ŵ�ϴ�!" );
	//////////////////////////////////////////////////////////////////////////	
	
	//////////////////////////////////////////////////////////////////////////
	// ����� ����Ʈ ���� ó��
	std::vector< UidType > vecWaitNumberLIst;
	std::map< UidType, KAutoPartyUserWaitListPtr >::const_iterator mit;	
	for( mit = m_mapAutoPartyWaitList.begin(); mit != m_mapAutoPartyWaitList.end(); ++mit )
	{
		vecWaitNumberLIst.push_back( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iWaitNumber, vecWaitNumberLIst )
	{
		UnRegAutoPartyWaitListByWaitNumber( iWaitNumber, NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01 );
	}
}

void KAutoPartyMakingProcess::Tick()
{
	// ���� ���ʽ� ���� �����ε�
	if( IsActivatedAutoPartyBonus() )
	{
		// ���ʽ� �ð��� �ƴϴ�!
		if( IsAutoPartyBonusTime() == false )
		{
			// ���ʽ� ������ ����!
			SetActivatedAutoPartyBonus( false );

			// ��� �������� �˸���.
			KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kPacketNot;
			kPacketNot.m_iDungeonIDWithDif = GetDungeonIDWithDif();
			kPacketNot.m_cDungeonMode = GetDungeonMode();
			kPacketNot.m_bActivatedBonus = IsActivatedAutoPartyBonus();
			BroadCastGameServer( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, kPacketNot );
		}
	}
	// ���� ���ʽ� ���� ���°� �ƴѵ�
	else
	{
		// ���ʽ� ���� �ð��̴�!
		if( IsAutoPartyBonusTime() )
		{
			// ���ʽ� ������ ����!
			SetActivatedAutoPartyBonus( true );

			// ��� �������� �˸���
			KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kPacketNot;
			kPacketNot.m_iDungeonIDWithDif = GetDungeonIDWithDif();
			kPacketNot.m_cDungeonMode = GetDungeonMode();
			kPacketNot.m_bActivatedBonus = IsActivatedAutoPartyBonus();
			BroadCastGameServer( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, kPacketNot );
		}
	}
}

void KAutoPartyMakingProcess::AddLevelCondition( IN const int iBeginLevel, IN const int iEndLevel )
{
    SLevelCondition sInfo;
	sInfo.m_iBeginLevel = iBeginLevel;
	sInfo.m_iEndLevel = iEndLevel;
	sInfo.m_iListType = static_cast<int>(m_vecLevelCondition.size()); // ���� �����̳��� size���� ListType�� �ȴ�.
	m_vecLevelCondition.push_back( sInfo );
}

//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
int	KAutoPartyMakingProcess::GetWaitUserCountByListType( IN const int iListType ) const
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapAutoPartyWaitUserCount.find( iListType );
	if( mit == m_mapAutoPartyWaitUserCount.end() )
	{		
		return 0;
	}

	return mit->second;
}

int	KAutoPartyMakingProcess::GetWaitUserCountByWaitNumber( IN const UidType iWaitNumber )
{
    KAutoPartyUserWaitListPtr spWaitList = GetAutoPartyWaitListByWaitNumber( iWaitNumber );
	if( IS_NULL( spWaitList ) )
	{
		START_LOG( cwarn, L"���� ���� �ʴ� ��⸮��Ʈ �Դϴ�." )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;
		return 0;
	}

	return GetWaitUserCountByListType( spWaitList->GetListType() );
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
//}}

// WaitNumber
UidType	KAutoPartyMakingProcess::NewWaitNumber()
{
    return ++m_iLastWaitNumber;
}

//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
KAutoPartyPtr KAutoPartyMakingProcess::MakeAutoParty( IN const KAutoPartyConditionInfo& kInfo )
{
	KAutoPartyConditionInfo kConditionInfo;
	kConditionInfo.m_iDungeonID = kInfo.m_iDungeonID;
	kConditionInfo.m_cDifficultyLevel = kInfo.m_cDifficultyLevel;
	kConditionInfo.m_cGetItemType = static_cast<char>(CXSLRoom::DGIT_PERSON);
	kConditionInfo.m_cDungeonMode = kInfo.m_cDungeonMode;
	kConditionInfo.m_cLimitPlayerCount = SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	const UidType iAutoPartyUID = g_autoPartyUidDispenser.GetNewAutoPartyUID();

	KAutoPartyPtr spAutoParty( new KAutoParty( iAutoPartyUID, kConditionInfo ) );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"KAutoParty new �Ҵ����." )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// FSM����
	FSMclassPtr spFSMClass = boost::shared_ptr< KAutoPartyFSM >( new KAutoPartyFSM );
	spAutoParty->SetFSM( spFSMClass );
	return spAutoParty;
}
#else
KAutoPartyPtr KAutoPartyMakingProcess::MakeAutoParty()
{
	KAutoPartyConditionInfo kConditionInfo;
	kConditionInfo.m_iDungeonID = GetDungeonID();
	kConditionInfo.m_cDifficultyLevel = GetDifficultyLevel();
	kConditionInfo.m_cGetItemType = static_cast<char>(CXSLRoom::DGIT_PERSON);
	kConditionInfo.m_cDungeonMode = GetDungeonMode();
	kConditionInfo.m_cLimitPlayerCount = SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	const UidType iAutoPartyUID = g_autoPartyUidDispenser.GetNewAutoPartyUID();

	KAutoPartyPtr spAutoParty( new KAutoParty( iAutoPartyUID, kConditionInfo ) );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"KAutoParty new �Ҵ����." )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// FSM����
	FSMclassPtr spFSMClass = boost::shared_ptr< KAutoPartyFSM >( new KAutoPartyFSM );
	spAutoParty->SetFSM( spFSMClass );
	return spAutoParty;
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

KAutoPartyUserPtr KAutoPartyMakingProcess::MakeUser( IN const KAutoPartyUserInfo& kInfo )
{
	KAutoPartyUserPtr  spAutoPartyUser( new KAutoPartyUser( kInfo ) );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"�ڵ� ��Ƽ ���� ���� ����!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr();
	}

	spAutoPartyUser->SetFSM( ms_spUserFSM );
	return spAutoPartyUser;
}

KAutoPartyUserPtr KAutoPartyMakingProcess::GetAutoPartyWaitUserByUnitUID( IN const UidType iUnitUID )
{
	std::map< UidType, KAutoPartyUserPtr  >::iterator mit = m_mapAutoPartyWaitUser.find( iUnitUID );
	if( mit == m_mapAutoPartyWaitUser.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ������ �����ϴ�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr ();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ������ �̻�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr();
	}

	return mit->second;
}

KAutoPartyUserWaitListPtr KAutoPartyMakingProcess::GetAutoPartyWaitListByWaitNumber( IN const UidType iWaitNumber )
{
	std::map< UidType, KAutoPartyUserWaitListPtr >::iterator mit = m_mapAutoPartyWaitList.find( iWaitNumber );
	if( mit == m_mapAutoPartyWaitList.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ ����� ����Ʈ�� �����ϴ�.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KAutoPartyUserWaitListPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ��ġ ����� ����Ʈ ������ �̻�.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KAutoPartyUserWaitListPtr();
	}

	return mit->second;
}

//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
KAutoPartyPtr KAutoPartyMakingProcess::CreateAutoParty( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo, IN const KAutoPartyConditionInfo& kInfo )
{
	KAutoPartyPtr spAutoParty = MakeAutoParty( kInfo );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ����!" )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// �ڵ� ��Ƽ ���� �ʱ�ȭ
	spAutoParty->Enter( vecAutoPartyUserInfo );

	// �ڵ� ��Ƽ�� ó�� �����Ǹ� �ڵ� ��Ƽ ����ŷ �����̴�!
	spAutoParty->StateTransition( KAutoPartyFSM::I_TO_AUTO_PARTY_MAKING );
	return spAutoParty;
}
#else
KAutoPartyPtr KAutoPartyMakingProcess::CreateAutoParty( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo )
{
	KAutoPartyPtr spAutoParty = MakeAutoParty();
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ����!" )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// �ڵ� ��Ƽ ���� �ʱ�ȭ
	spAutoParty->Enter( vecAutoPartyUserInfo );

	// �ڵ� ��Ƽ�� ó�� �����Ǹ� �ڵ� ��Ƽ ����ŷ �����̴�!
	spAutoParty->StateTransition( KAutoPartyFSM::I_TO_AUTO_PARTY_MAKING );
	return spAutoParty;
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
bool KAutoPartyMakingProcess::RegAutoPartyWaitList( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList, 
												    IN const KAutoPartyConditionInfo& kAutoPartyCondition, 
													OUT UidType& iWaitNumber, 
													IN const bool bRegFrontOfList /*= false*/ )
#else
bool KAutoPartyMakingProcess::RegAutoPartyWaitList( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList, 
												    OUT UidType& iWaitNumber, 
												    IN const bool bRegFrontOfList /*= false*/ )
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
{
	// �ʱ�ȭ
	iWaitNumber = 0;

	// 1. �ڵ� ��Ƽ �ο� ���� �´��� üũ!
	if( SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT < (int)vecAutoPartyUserList.size() )
	{
		START_LOG( cerr, L"��ġ ����� ����Ʈ ���� ����!" )
			<< BUILD_LOG( SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
			<< BUILD_LOG( vecAutoPartyUserList.size() )
			<< END_LOG;
		return false;
	}

	// 2. �ڵ� ��Ƽ ����� ����Ʈ�� ���� �մϴ�.
	KAutoPartyUserWaitListPtr spWaitList( new KAutoPartyUserWaitList( NewWaitNumber() ) );
	if( spWaitList == NULL )
	{
		START_LOG( cerr, L"��ġ ����� ����Ʈ ���� ����!" )
			<< END_LOG;
		return false;
	}
	
	// 3. �ڵ� ��Ƽ ����� ����Ʈ�� ��û�ڵ��� �ϳ��� �ֽ��ϴ�!
	BOOST_TEST_FOREACH( const KAutoPartyUserInfo&, kInfo, vecAutoPartyUserList )
	{
		// �̹� ��ϵ� ĳ���� ���� �˻�
		if( IsExistInWaitListByUnitUID( kInfo.m_iUnitUID ) == true )
		{
			START_LOG( cerr, L"�̹� ��ġ�� ��ϵǾ��ִ� ����!" )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

        KAutoPartyUserPtr spAutoPartyUser = MakeUser( kInfo );
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ ���� �� ��ġ ���� ���� ����!" )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

		// ����� ����Ʈ�� �ڵ� ��Ƽ ������ �������!
		spWaitList->AddAutoPartyUser( spAutoPartyUser );

		// ��ü ���� ����Ʈ���� �������!
		m_mapAutoPartyWaitUser.insert( std::make_pair( spAutoPartyUser->GetCharUID(), spAutoPartyUser ) );
	}

	if( spWaitList->GetNumMember() == 0 )
	{
		START_LOG( cerr, L"����� ����Ʈ�� �������� ���Ͽ����ϴ�." )
			<< END_LOG;
		return false;
	}

	// 4. ListType���� ������!
	int iListType = 0;
	//{{ 2012. 12. 04 ���� ��Ī ���� ��� ������ ���� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_LEVEL
	if( GetListTypeByLevelCondition( spWaitList->GetAverageUnitLevel(), iListType ) == false )
	{
		START_LOG( cwarn, L"����Ʈ Ÿ���� ������ ���߽��ϴ�!" )			
			<< BUILD_LOGc( spWaitList->GetAverageUnitLevel() )
			<< BUILD_LOGc( GetDungeonMode() )
			<< BUILD_LOG( GetDungeonIDWithDif() )
			<< END_LOG;

		// �ϴ� 0���� ����!
		spWaitList->SetListType( 0 );
	}
#else
	if( GetListTypeByLevelCondition( spWaitList->GetLevelForListType(), iListType ) == false )
	{
		START_LOG( cerr, L"����Ʈ Ÿ���� ������ ���߽��ϴ�!" )			
			<< BUILD_LOG( spWaitList->GetLevelForListType() )
			<< BUILD_LOG( GetDungeonMode() )
			<< BUILD_LOG( GetDungeonIDWithDif() )
			<< END_LOG;

		// �ϴ� 0���� ����!
		spWaitList->SetListType( 0 );
	}
#endif SERV_AUTO_PARTY_MATCHING_LEVEL
	//}
	else
	{
        spWaitList->SetListType( iListType );
	}

	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	// 5. �ڵ� ��Ƽ ��û ���� ����!
	spWaitList->SetAutoPartyCondition( kAutoPartyCondition );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	// 6. �ش� Ÿ���� ����ڸ���Ʈ�� �ֽ��ϴ�!
	if( bRegFrontOfList )
	{
		// ���û�̶�� ����Ʈ �ֻ����� ����!
		m_listAutoPartyWaitList.push_front( spWaitList );
	}
	else
	{
		// ���ο� ��û�̶�� ����Ʈ �������� ����!
		m_listAutoPartyWaitList.push_back( spWaitList );
	}

	// 7. ����ȣ�� key���ϴ� �����̳ʿ��� �ֽ��ϴ�!
	m_mapAutoPartyWaitList.insert( std::make_pair( spWaitList->GetWaitNumber(), spWaitList ) );
	
	iWaitNumber = spWaitList->GetWaitNumber();


	// 8. �ڵ� ��Ƽ ���ʽ� ����!
	if( IsActivatedAutoPartyBonus() == true  ||  IsActivatedDelayTimeBonus() == true )
	{
		spWaitList->SetAutoPartyBonus( true );
	}

	//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	// 9. ����� �� ī��Ʈ�� �ø���!
	IncreaseAutoPartyWaitUserCount( spWaitList->GetListType(), spWaitList->GetNumMember() );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}
	
	return true;
}

bool KAutoPartyMakingProcess::UnRegAutoPartyWaitListByUnitUID( IN const UidType iUnitUID, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KAutoPartyUserPtr spAutoPartyUser = GetAutoPartyWaitUserByUnitUID( iUnitUID );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"����� ����Ʈ ��������� �Ϸ��� �ϴµ� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	// �ش� ��ġ ���� ����
    m_mapAutoPartyWaitUser.erase( iUnitUID );

	// ����� ����Ʈ������ ����
	std::list< KAutoPartyUserWaitListPtr >::iterator lit = m_listAutoPartyWaitList.begin();
	for( ; lit != m_listAutoPartyWaitList.end(); ++lit )
	{
		if( (*lit) == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�" )
				<< END_LOG;
			continue;
		}

        if( (*lit)->IsExistAutoPartyUser( iUnitUID ) )
		{
            (*lit)->DelAutoPartyUser( iUnitUID );
			break;
		}
	}

	if( lit != m_listAutoPartyWaitList.end() )
	{
		if( (*lit)->GetNumMember() == 0 )
		{
			const UidType iAutoPartyWaitNumber = (*lit)->GetWaitNumber();
			m_listAutoPartyWaitList.erase( lit );
            m_mapAutoPartyWaitList.erase( iAutoPartyWaitNumber );
		}
	}
	else
	{
		START_LOG( cerr, L"����� ����Ʈ�� ��ġ������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	// ��� ó���� ������!
	KEGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT kPacketNot;
	kPacketNot.m_iReason = iUnRegReason;
	kPacketNot.m_wstrCancelUnitNickName = wstrCancelUnitNickName;
	spAutoPartyUser->SendToGSCharacter( EGB_UNREG_AUTO_PARTY_WAIT_LIST_NOT, kPacketNot );

	// �ڵ� ��Ƽ�� �����ؼ� ����� ����Ʈ���� �����°Ŷ�� �ڵ���Ƽ ���� ��踦 ������!
	if( iUnRegReason == NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 )
	{
		m_kCompleteTimeManager.AddCompleteTime( static_cast<int>(spAutoPartyUser->GetAutoPartyCompleteSeconds()) );
	}
	return true;
}

bool KAutoPartyMakingProcess::UnRegAutoPartyWaitListByWaitNumber( IN const UidType iWaitNumber, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KAutoPartyUserWaitListPtr spAutoPartyWaitList = GetAutoPartyWaitListByWaitNumber( iWaitNumber );
	if( spAutoPartyWaitList == NULL )
	{
		START_LOG( cerr, L"����� ����Ʈ ��������� �Ϸ��� �ϴµ� ����Ʈ�� ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;
		return false;
	}

	//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	// ����� �� ī��Ʈ�� ������!
	DecreaseAutoPartyWaitUserCount( spAutoPartyWaitList->GetListType(), spAutoPartyWaitList->GetNumMember() );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}

	// ĳ���� ����Ʈ�� �� �ϳ��� �����������!
	std::vector< UidType > vecUnitUIDList;
	spAutoPartyWaitList->GetUnitUIDList( vecUnitUIDList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDList )
	{
		UnRegAutoPartyWaitListByUnitUID( iUnitUID, iUnRegReason, wstrCancelUnitNickName );
	}
	
	return true;
}

void KAutoPartyMakingProcess::MakingAutoPartyProcessing( OUT std::vector< KAutoPartyPtr >& vecMadeAutoParty )
{
	vecMadeAutoParty.clear();

	//////////////////////////////////////////////////////////////////////////	
	// �״����� ���� ����ڵ��� ������ ���ο� ��ġ�� ������!
	const int AUTO_PARTY_MAKING_BEGIN_COUNT = SiKAutoPartyScriptManager()->GetAutoPartyMakingBeginCount();
	const int CHECK_WAIT_USER_COUNT = SiKAutoPartyScriptManager()->GetCheckWaitUserCount();

	// ����� ����Ʈ�� �����ؿ���!
	std::list< KAutoPartyUserWaitListPtr > listAutoPartyMakingWaitList = m_listAutoPartyWaitList;

	// ��� ������ ��ġ ����ŷ�� �Ѵ�!
	const u_int uiTotalListSize = listAutoPartyMakingWaitList.size();
	u_int uiLoopCount = 0;

	while( listAutoPartyMakingWaitList.empty() == false )
	{
		++uiLoopCount;
		if( uiLoopCount > uiTotalListSize )
		{
			START_LOG( cerr, L"�߸��� ������ �Ͽ����ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( uiLoopCount )
				<< BUILD_LOG( uiTotalListSize )
				<< END_LOG;
			break;
		}


		//////////////////////////////////////////////////////////////////////////
		// 1. �ڵ� ��Ƽ�� ����� �ִ� �ο������� ����ڼ��� ���� ��� ���̻� üũ�� �ʿ䰡 ����!
		//////////////////////////////////////////////////////////////////////////
		if( (int)listAutoPartyMakingWaitList.size() < AUTO_PARTY_MAKING_BEGIN_COUNT )
			break;


		//////////////////////////////////////////////////////////////////////////
		// 2. ����� ����Ʈ���� �� ó������ �ϳ� ������.
		//////////////////////////////////////////////////////////////////////////
		KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser = listAutoPartyMakingWaitList.front();
		listAutoPartyMakingWaitList.pop_front();
		if( spFrontAutoPartyWaitUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}


		//////////////////////////////////////////////////////////////////////////
		// 3. ��� �ð��� ��� ����.
		//////////////////////////////////////////////////////////////////////////
		const double fElapsedWaitTime = spFrontAutoPartyWaitUser->ElapsedWaitTime();
		

		//////////////////////////////////////////////////////////////////////////
		// 4. ������ ���� ������ �����Ͽ� ����� ����Ʈ�� �̾� ����.
		//////////////////////////////////////////////////////////////////////////
		KCompareLimitList< KAutoPartyUserWaitListPtr > listOptimalAutoPartyWaitList( CHECK_WAIT_USER_COUNT ); // ������ ����� ����Ʈ�� ����!
		
		// ������ ���� ������ CHECK_WAIT_USER_COUNT�� ��ŭ�� ����Ʈ�� �̾� ����!
		AutoPartyProcess_GetWaitUserListByOptimalConditionSort( spFrontAutoPartyWaitUser, fElapsedWaitTime, listAutoPartyMakingWaitList, listOptimalAutoPartyWaitList );


		//////////////////////////////////////////////////////////////////////////
		// 5. ���ĵ� ����Ʈ�� �������� �� ��Ƽ �ο��� ���߾� �߷�����!
		//////////////////////////////////////////////////////////////////////////
		std::list< KAutoPartyUserWaitListPtr > listSortedWaitUserList;
		listOptimalAutoPartyWaitList.GetValueList( listSortedWaitUserList );

		// üũ�� ��ġ �ο� ��
		std::vector< UidType > vecSelectedWaitUserList;
		int iSumAutoPartyPlayerCount;
		//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		char iDifficulty;
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		//}}

		// �ڵ� ��Ƽ�� �� ����ڹ�ȣ ����Ʈ�� ����!
		AutoPartyProcess_GetWaitNumberListForCompleteMakeAutoParty( spFrontAutoPartyWaitUser, 
																	listSortedWaitUserList, 
																	vecSelectedWaitUserList, 
																	iSumAutoPartyPlayerCount
																	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
															#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																	, iDifficulty
															#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																	//}}
																	);


		//////////////////////////////////////////////////////////////////////////
		// 6. �ش� ������ ��� �ð��� ���� �ٷ� �������� ��Ƽ ������ �� �� ��ٸ��� ���� �Ѵ�.
		//////////////////////////////////////////////////////////////////////////

		// ���� ���ð��� ���� ���������Ƿ� �ִ� ��Ƽ ���� �ο��� üũ����!
		if( fElapsedWaitTime < SiKAutoPartyScriptManager()->GetDirectGameStartWaitTime() )
		{
			//{{ 2012. 11. 29	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
			const KAutoPartyConditionInfo& kAutoPartyConditionInfo = spFrontAutoPartyWaitUser->GetAutoPartyCondition();
			int iDungeon = kAutoPartyConditionInfo.m_iDungeonID + static_cast<int>( iDifficulty );

			if( iSumAutoPartyPlayerCount < SiKAutoPartyScriptManager()->GetAutoPartyMatchingSuccessNum( iDungeon ) )
				continue;
#else
			if( iSumAutoPartyPlayerCount < SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
				continue;
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
			//}}
		}
		//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		else
		{
			if( SiKAutoPartyScriptManager()->IsCheckAllMemberDirectGameStartTime() == true )
			{
				// ���� �ο��� �ִ°�츸 üũ
				if( vecSelectedWaitUserList.empty() == false )
				{
					// ������ ��Ƽ���� ��� �ð��� �������� �˻�!
					bool bDirectGameStart = false;

					BOOST_TEST_FOREACH( const UidType, iAutoPartyWaitNumber, vecSelectedWaitUserList )
					{
						KAutoPartyUserWaitListPtr spWaitList = GetAutoPartyWaitListByWaitNumber( iAutoPartyWaitNumber );
						if( IS_NULL( spWaitList ) )
						{
							START_LOG( cerr, L"����� ����Ʈ�� �������� �ʴ´�! ����ͼ� �̷��� ���.. �Ͼ�� �ȵǴ� ����!" )
								<< BUILD_LOG( iAutoPartyWaitNumber )
								<< END_LOG;
							break;
						}

						if( spWaitList->ElapsedWaitTime() < SiKAutoPartyScriptManager()->GetDirectGameStartWaitTime() )
						{
							bDirectGameStart = false;
							break;
						}

						bDirectGameStart = true;
					}

					// ���� �ð��� �ȵ� ������ �����Ѵ�.
					if( bDirectGameStart == false )
						continue;
				}
			}			
		}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}


		//////////////////////////////////////////////////////////////////////////
		// 7. ������ ��Ƽ ���� ������ �ο����� ���߾� �ϳ��� ��Ƽ�� �ϼ���Ų��.
		//////////////////////////////////////////////////////////////////////////
		{
			std::vector< KAutoPartyUserInfo > vecSourceUserInfo;
			spFrontAutoPartyWaitUser->GetAutoPartyUserInfoList( vecSourceUserInfo );

			// ����� ����Ʈ���� ��ġ ��û�� ����!
			if( UnRegAutoPartyWaitListByWaitNumber( spFrontAutoPartyWaitUser->GetWaitNumber(), NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 ) == false )
			{
				START_LOG( cerr, L"����� ����Ʈ���� ��ġ ��û�� ���� ����! �Ͼ� ������ �ȵǴ� ����!" )
					<< BUILD_LOG( spFrontAutoPartyWaitUser->GetWaitNumber() )
					<< END_LOG;
				break;
			}

			// ���ο� �ڵ���Ƽ ����!
			//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			KAutoPartyPtr spNewAutoParty = CreateAutoParty( vecSourceUserInfo, spFrontAutoPartyWaitUser->GetAutoPartyCondition() );
#else
			KAutoPartyPtr spNewAutoParty = CreateAutoParty( vecSourceUserInfo );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			//}}
			if( spNewAutoParty == NULL )
			{
				START_LOG( cerr, L"���ο� ��ġ ���� ����!" )
					<< END_LOG;
				break;
			}

			// ������ ������ �ڵ� ��Ƽ�� ���� ó��!
			BOOST_TEST_FOREACH( const UidType, iAutoPartyWaitNumber, vecSelectedWaitUserList )
			{
                KAutoPartyUserWaitListPtr spWaitList = GetAutoPartyWaitListByWaitNumber( iAutoPartyWaitNumber );
				if( spWaitList == NULL )
				{
					START_LOG( cerr, L"����� ����Ʈ�� �������� �ʴ´�! ����ͼ� �̷��� ���.. �Ͼ�� �ȵǴ� ����!" )
						<< BUILD_LOG( iAutoPartyWaitNumber )
						<< END_LOG;
					break;
				}

				std::vector< KAutoPartyUserInfo > vecNewEnterUser;
				spWaitList->GetAutoPartyUserInfoList( vecNewEnterUser );

				//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
				// �� ���� ���̵��� �����Ѵ�!
				if( spNewAutoParty->GetDifficultyLevel() < spWaitList->GetDifficultyLevel() )
				{
					spNewAutoParty->SetDifficultyLevel( spWaitList->GetDifficultyLevel() );
				}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
				//}}

				// ����� ����Ʈ���� ��������!
				if( UnRegAutoPartyWaitListByWaitNumber( iAutoPartyWaitNumber, NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 ) == false )
				{
					START_LOG( cerr, L"����� ����Ʈ���� �ڵ� ��Ƽ ��û�� ���� ����!" )
						<< BUILD_LOG( spWaitList->GetWaitNumber() )
						<< END_LOG;
					break;
				}

				// �ڵ� ��Ƽ�� ���� ��Ű��!
				if( spNewAutoParty->Enter( vecNewEnterUser ) == false )
				{
					START_LOG( cerr, L"�߰��ο� ���� ����! ���󿡷��ΰ�?" )
						<< BUILD_LOG( vecNewEnterUser.size() )
						<< END_LOG;
					break;
				}

				// �̾Ƴ��� ����Ʈ���� ����
				std::list< KAutoPartyUserWaitListPtr >::iterator litD = listAutoPartyMakingWaitList.begin();
				for( ; litD != listAutoPartyMakingWaitList.end(); ++litD )
				{
					if( (*litD) == NULL )
					{
						START_LOG( cerr, L"�����Ͱ��� �̻��մϴ�." )
							<< END_LOG;
						continue;
					}

					if( (*litD)->GetWaitNumber() == iAutoPartyWaitNumber )
						break;
				}

				if( litD != listAutoPartyMakingWaitList.end() ) 
				{
					listAutoPartyMakingWaitList.erase( litD );
				}
			}

			// ���������� �ڵ���Ƽ�� ����Ǿ����� Ȯ������!
			if( spNewAutoParty->CheckAutoPartyMakingSuccess() == false )
			{
				START_LOG( clog, L"�ڵ� ��Ƽ ���� ó���� �����ߴ�!" )
					<< BUILD_LOG( spNewAutoParty->GetAutoPartyUID() )
					<< END_LOG;
			}
			else
			{
				// �ڵ� ��Ƽ�� ���� �����Ǿ���!
				vecMadeAutoParty.push_back( spNewAutoParty );
			}
		}
	}
}

void KAutoPartyMakingProcess::AutoPartyProcess_GetWaitUserListByOptimalConditionSort( IN const KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser,
																					  IN const double fElapsedWaitTime,
																					  IN const std::list< KAutoPartyUserWaitListPtr >& listAutoPartyMakingWaitList, 																					  
																					  OUT KCompareLimitList< KAutoPartyUserWaitListPtr >& listOptimalAutoPartyWaitList )
{
	// ���� ��� �ð��� ���� ���� Ÿ�� ���� �ð����� ũ�ٸ� ��Ƽ ��Ī�ÿ� ���� ���� Ÿ�Ե� ��Ī �ǵ��� �Ѵ�.
	const bool bSameUnitTypeWaitTimeOver = ( fElapsedWaitTime > SiKAutoPartyScriptManager()->GetSameUnitTypeWaitTime() );
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	const bool bSameDifficultyLevelWaitTimeOver = ( fElapsedWaitTime > SiKAutoPartyScriptManager()->GetSameDifficultyLevelWaitTime() );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	const u_int uiMaxSize = listOptimalAutoPartyWaitList.MaxSize();

	BOOST_TEST_FOREACH( KAutoPartyUserWaitListPtr, spTargetUserList, listAutoPartyMakingWaitList )
	{
		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		// (1) �ٸ� ���̵��� ��Ī�� ���� ���� ( �켱������ ���� )
		if( bSameDifficultyLevelWaitTimeOver == false )
		{
			// �ٸ� ���̵���� ��Ī ��� ���Ե��� �ʴ´�.
			if( spFrontAutoPartyWaitUser->CheckSameDifficultyLevel( spTargetUserList ) == false )
				continue;
		}
		else
		{
			if( SiKAutoPartyScriptManager()->IsCheckAllMemberSameDifficultyLevelTime() == true )
			{
				// Ÿ�� ����� ����Ʈ�� �ð��� �����°�?
				if( spTargetUserList->ElapsedWaitTime() < SiKAutoPartyScriptManager()->GetSameDifficultyLevelWaitTime() )
					continue;
			}
		}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}

		// (2) ���� ListType���� �˻�! ( ���� ���� üũ )
		if( spTargetUserList->GetListType() != spFrontAutoPartyWaitUser->GetListType() )
			continue;

		// (3) �ٸ� Ÿ���� ĳ���� ��Ī�� ���� ����
		if( bSameUnitTypeWaitTimeOver == false )
		{
			// ���� ���� Ÿ���̸� ����� �ɼ� ����!
			if( spFrontAutoPartyWaitUser->CheckSameUnitType( spTargetUserList ) == true )
				continue;
		}
		//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		else
		{
			if( SiKAutoPartyScriptManager()->IsCheckAllMemberSameUnitTypeTime() == true )
			{
				// Ÿ�� ����� ����Ʈ�� �ð��� �����°�?
				if( spTargetUserList->ElapsedWaitTime() < SiKAutoPartyScriptManager()->GetSameUnitTypeWaitTime() )
					continue;
			}
		}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}

		// �׷��ٸ� ����!
		listOptimalAutoPartyWaitList.PushBack( spTargetUserList );

		// ����Ʈ�� ���� á���� Ȯ������!
		if( listOptimalAutoPartyWaitList.Size() >= uiMaxSize )
			break;
	}
}

void KAutoPartyMakingProcess::AutoPartyProcess_GetWaitNumberListForCompleteMakeAutoParty( IN const KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser,
																						  IN const std::list< KAutoPartyUserWaitListPtr >& listSortedWaitUserList,
																						  OUT std::vector< UidType >& vecSelectedWaitUserList,
																						  OUT int& iSumAutoPartyPlayerCount
																						  //{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
																				#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																						  , OUT char& iDifficulty
																				#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																						  //}}
																						  )
{
	vecSelectedWaitUserList.clear();

	iSumAutoPartyPlayerCount = spFrontAutoPartyWaitUser->GetNumMember();
	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	iDifficulty = spFrontAutoPartyWaitUser->GetDifficultyLevel();
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	BOOST_TEST_FOREACH( KAutoPartyUserWaitListPtr, spTargetUserList, listSortedWaitUserList )
	{
		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		// �� �ο� ���� �� 4������ ���¾ƾ� �Ѵ�!
		if( ( iSumAutoPartyPlayerCount + spTargetUserList->GetNumMember() ) > SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
			continue;

		iSumAutoPartyPlayerCount += spTargetUserList->GetNumMember();
		vecSelectedWaitUserList.push_back( spTargetUserList->GetWaitNumber() );

		//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		if( iDifficulty < spTargetUserList->GetDifficultyLevel() )
		{
			iDifficulty = spTargetUserList->GetDifficultyLevel();
		}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		//}}

		// ��� �ο��� �� ä�������� ��Ż!
		if( iSumAutoPartyPlayerCount == SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
			break;
	}
}

bool KAutoPartyMakingProcess::IsActivatedDelayTimeBonus()
{
	if( m_vecDelayBonusTimer.empty() )
		return false;

	std::vector< boost::timer >::iterator vit;
	for( vit = m_vecDelayBonusTimer.begin(); vit != m_vecDelayBonusTimer.end(); ++vit )
	{
		if( vit->elapsed() < 5.0 )
			return true;
	}
	
	m_vecDelayBonusTimer.clear();
	return false;
}

bool KAutoPartyMakingProcess::IsAutoPartyBonusTime() const
{
	return( m_tGameStartLastTime.elapsed() > SiKAutoPartyScriptManager()->GetAutoPartyBonusTime() );
}

void KAutoPartyMakingProcess::SetActivatedAutoPartyBonus( IN const bool bActivate )
{
	const bool bBefore = m_bActivatedAutoPartyBonus;
	m_bActivatedAutoPartyBonus = bActivate;

	if( bBefore == true  &&  bActivate == false )
	{
		m_vecDelayBonusTimer.push_back( boost::timer() );
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
void KAutoPartyMakingProcess::IncreaseAutoPartyWaitUserCount( IN const int iListType, IN const int iCount )
{
	std::map< int, int >::iterator mit;
	mit = m_mapAutoPartyWaitUserCount.find( iListType );
	if( mit == m_mapAutoPartyWaitUserCount.end() )
	{
		m_mapAutoPartyWaitUserCount.insert( std::make_pair( iListType, iCount ) );
		return;
	}

	// ����!
	mit->second += iCount;
}

void KAutoPartyMakingProcess::DecreaseAutoPartyWaitUserCount( IN const int iListType, IN const int iCount )
{
	std::map< int, int >::iterator mit;
	mit = m_mapAutoPartyWaitUserCount.find( iListType );
	if( mit == m_mapAutoPartyWaitUserCount.end() )
	{
		m_mapAutoPartyWaitUserCount.insert( std::make_pair( iListType, 0 ) );
		return;
	}

	// ����!
	mit->second -= iCount;

	if( mit->second < 0 )
	{
		START_LOG( cerr, L"��� 0���� �۰� ���ü� ����?" )
			<< BUILD_LOG( mit->second )
			<< END_LOG;

		mit->second = 0;
	}
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
//}}


