#include "UserDungeonManager.h"
//{{ 2012. 04. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLDungeonManager.h"
	#include "GSAutoPartyManager.h"
	#include "X2Data/XSLMapData.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_PC_BANG_TYPE
	#include "GameSysVal/GameSysVal.h"
#endif SERV_PC_BANG_TYPE

//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST


KUserDungeonManager::KUserDungeonManager(void)
{
	Clear();
}

KUserDungeonManager::~KUserDungeonManager(void)
{
}

void KUserDungeonManager::Clear()
{
	m_iHenirRewardCount				= 0;				// ��ϸ� ���� ���� Ƚ�� (�Ϲ�)
	m_iHenirRewardPremiumCount		= 0;				// ��ϸ� ���� ���� Ƚ�� (PC��)
	m_iHenirRewardEventCount		= 0;				// ��ϸ� ���� ���� Ƚ�� (�̺�Ʈ) Ƚ��
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	m_iHenirChallengeRewardCount		= 0;				// ��ϸ� ���� ��� ���� ���� Ƚ�� (�Ϲ�)
	m_iHenirChallengeRewardPremiumCount	= 0;				// ��ϸ� ���� ��� ���� ���� Ƚ�� (PC��)
	m_iHenirChallengeRewardEventCount	= 0;				// ��ϸ� ���� ��� ���� ���� Ƚ�� (�̺�Ʈ) Ƚ��
#endif // SERV_HENIR_RENEWAL_2013

	m_iHenirRewardEventLimitCount	= 0;				// ��ϸ� ���� ���� Ƚ�� (�̺�Ʈ) ����
	m_bUnLimitedHenirRewardEvnet	= false;			// ��ϸ� ���� ���� Ƚ�� (�̺�Ʈ) ������

	m_bIsPcBang						= false;
#ifdef SERV_PC_BANG_TYPE
	m_iPcBangType					= -1;
#endif SERV_PC_BANG_TYPE
	//{{ 2012. 02. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_iAutoPartyWaitNumber			= 0;
	m_iAutoPartyUID					= 0;
	m_kDungeonGameInfo.Clear();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

void KUserDungeonManager::Init( IN const bool bUnlimitedEvent			// ������ �ΰ�?
							  , IN const int iLimitEvent				// Event �߰� Ƚ�� MAX
							  , IN const int iNormalCnt					// ���� ���� Ƚ�� �Ϲ�
							  , IN const int iPremiumCnt				// ���� ���� Ƚ�� pc��
							  , IN const int iEventCnt					// ���� ���� Ƚ�� �̺�Ʈ
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
							  , IN const int iChallengeNormalCnt
							  , IN const int iChallengePremiumCnt
							  , IN const int iChallengeEventCnt
#endif // SERV_HENIR_RENEWAL_2013
							  , IN const bool bIsPcBang					// PC������ ���� �ֱ�
#ifdef SERV_PC_BANG_TYPE
								, IN const int iPcBangType				// PC�� Ÿ��
#endif SERV_PC_BANG_TYPE							  
							  )
{
	if( ( iLimitEvent < 0 )
		|| ( iNormalCnt < 0 )
		|| ( iPremiumCnt < 0 )
		|| ( iEventCnt < 0 )
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
		|| ( iChallengeNormalCnt < 0 )
		|| ( iChallengePremiumCnt < 0 )
		|| ( iChallengeEventCnt < 0 )
#endif // SERV_HENIR_RENEWAL_2013
		)
	{
		START_LOG( cerr, L"��ϸ� ���� ȹ�� - 0 ���� ���� ������ �ʱ�ȭ �Ѵ�?!")
			<< BUILD_LOG( bUnlimitedEvent )
			<< BUILD_LOG( iLimitEvent )
			<< BUILD_LOG( iNormalCnt )
			<< BUILD_LOG( iPremiumCnt )
			<< BUILD_LOG( iEventCnt )
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
			<< BUILD_LOG( iChallengeNormalCnt )
			<< BUILD_LOG( iChallengePremiumCnt )
			<< BUILD_LOG( iChallengeEventCnt )
#endif // SERV_HENIR_RENEWAL_2013
			<< END_LOG;
	}

	m_iHenirRewardCount				= iNormalCnt;
	m_iHenirRewardPremiumCount		= iPremiumCnt;
	m_iHenirRewardEventCount		= iEventCnt;
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	m_iHenirChallengeRewardCount		= iChallengeNormalCnt;
	m_iHenirChallengeRewardPremiumCount	= iChallengePremiumCnt;
	m_iHenirChallengeRewardEventCount	= iChallengeEventCnt;
#endif // SERV_HENIR_RENEWAL_2013

	m_iHenirRewardEventLimitCount	= iLimitEvent;
	m_bUnLimitedHenirRewardEvnet	= bUnlimitedEvent;

	m_bIsPcBang						= bIsPcBang;
#ifdef SERV_PC_BANG_TYPE
	m_iPcBangType					= iPcBangType;
#endif SERV_PC_BANG_TYPE
}

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
bool KUserDungeonManager::IncreaseHenirRewardCount( IN const char cDungeonMode )
{
	// ���� ȹ�� ���� ����
	// PC�� > Event > Normal

	int	iHENIR_REWARD_LIMIT_COUNT	= 0;
	int* pHenirRewardCount			= NULL;		// ��ϸ� ���� ����� Ƚ�� (�Ϲ�)
	int* pHenirRewardPremiumCount	= NULL;		// ��ϸ� ���� ����� Ƚ�� (PC��)
	int* pHenirRewardEventCount		= NULL;		// ��ϸ� ���� ����� Ƚ�� (�̺�Ʈ) Ƚ��

	switch( cDungeonMode )
	{
	case CXSLDungeon::DM_HENIR_PRACTICE:
		iHENIR_REWARD_LIMIT_COUNT	= HRLC_NORMAL;
		pHenirRewardCount			= &m_iHenirRewardCount;
		pHenirRewardPremiumCount	= &m_iHenirRewardPremiumCount;
		pHenirRewardEventCount		= &m_iHenirRewardEventCount;
		break;

	case CXSLDungeon::DM_HENIR_CHALLENGE:
		iHENIR_REWARD_LIMIT_COUNT	= HRLC_NORMAL_CHALLENGE;
		pHenirRewardCount			= &m_iHenirChallengeRewardCount;
		pHenirRewardPremiumCount	= &m_iHenirChallengeRewardPremiumCount;
		pHenirRewardEventCount		= &m_iHenirChallengeRewardEventCount;
		break;

	default:
		START_LOG( cerr, L"�߸��� DungeonMode �����Դϴ�." )
			<< BUILD_LOGc( cDungeonMode )
			<< END_LOG;
		return false;
	}

	if( m_bUnLimitedHenirRewardEvnet == true )		// ��ϸ� ������ �̺�Ʈ ���̸�
	{
		return true;
	}

	// 1. PC ��
	if( IsPremiumUser()== true )
	{
#ifdef SERV_PC_BANG_TYPE
		int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
		if( *pHenirRewardPremiumCount < iAdditionalHenirRewardCount )
#else SERV_PC_BANG_TYPE
		if( *pHenirRewardPremiumCount < HRLC_PREMIUM )
#endif SERV_PC_BANG_TYPE
		{
			*pHenirRewardPremiumCount += 1;
			return true;
		}
	}

	// 2. Event
	if( *pHenirRewardEventCount < m_iHenirRewardEventLimitCount )
	{
		*pHenirRewardEventCount += 1;
		return true;
	}

	// 3. Normal
	if( *pHenirRewardCount < iHENIR_REWARD_LIMIT_COUNT )
	{
		*pHenirRewardCount += 1;
		return true;
	}

	return false;
}
#else
bool KUserDungeonManager::IncreaseHenirRewardCount()
{
	if( m_bUnLimitedHenirRewardEvnet == true )		// ��ϸ� ������ �̺�Ʈ ���̸�
	{
		return true;
	}

	// ���� ȹ�� ���� ����
	// PC�� > Event > Normal

	// 1. PC ��
	if( IsPremiumUser()== true )
	{
#ifdef SERV_PC_BANG_TYPE
		int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
		if( m_iHenirRewardPremiumCount < iAdditionalHenirRewardCount )
#else SERV_PC_BANG_TYPE
		if( m_iHenirRewardPremiumCount < HRLC_PREMIUM )
#endif SERV_PC_BANG_TYPE
		{
			m_iHenirRewardPremiumCount += 1;

			return true;
		}
	}
	
	// 2. Event
	if( m_iHenirRewardEventCount < m_iHenirRewardEventLimitCount )
	{
		m_iHenirRewardEventCount += 1;

		return true;
	}

	// 3. Normal
	if( m_iHenirRewardCount < HRLC_NORMAL )
	{
		m_iHenirRewardCount += 1;

		return true;
	}

	return false;
}
#endif // SERV_HENIR_RENEWAL_2013

void KUserDungeonManager::GetHenirRewardCountInfo( IN KEGS_HENIR_REWARD_COUNT_NOT& kNot )
{
	kNot.m_bUnLimited	= m_bUnLimitedHenirRewardEvnet;
	kNot.m_iEvent		= GetPossibleHenirRewardEventCount();
	kNot.m_iPremium		= GetPossibleHenirRewardPremiumCount();
	kNot.m_iNormal		= GetPossibleHenirRewardNormalCount();
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	kNot.m_iChallengeNormal		= GetPossibleHenirChallengeRewardNormalCount();
	kNot.m_iChallengePremium	= GetPossibleHenirChallengeRewardPremiumCount();
	kNot.m_iChallengeEvent		= GetPossibleHenirChallengeRewardEventCount();
#endif // SERV_HENIR_RENEWAL_2013
	kNot.m_iEventMAX	= m_iHenirRewardEventLimitCount;

#ifdef SERV_PC_BANG_TYPE
	int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
	kNot.m_iPremiumMAX	= ( IsPremiumUser() == true )  ? iAdditionalHenirRewardCount : 0;
#else SERV_PC_BANG_TYPE
	kNot.m_iPremiumMAX	= ( IsPremiumUser() == true )  ? HRLC_PREMIUM : 0;
#endif SERV_PC_BANG_TYPE
}

int KUserDungeonManager::GetPossibleHenirRewardNormalCount()
{ 
	if( (HRLC_NORMAL - m_iHenirRewardCount) < 0 )
	{
		return 0;
	}

	return HRLC_NORMAL - m_iHenirRewardCount; 
}

int KUserDungeonManager::GetPossibleHenirRewardEventCount()
{
	if( (m_iHenirRewardEventLimitCount - m_iHenirRewardEventCount) < 0 )
	{
		return 0;
	}

	return m_iHenirRewardEventLimitCount - m_iHenirRewardEventCount; 
}

int KUserDungeonManager::GetPossibleHenirRewardPremiumCount()
{
	if( IsPremiumUser() == true )
	{
#ifdef SERV_PC_BANG_TYPE
		int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
		if( (iAdditionalHenirRewardCount - m_iHenirRewardPremiumCount) < 0 )
		{
			return 0;
		}

		return iAdditionalHenirRewardCount - m_iHenirRewardPremiumCount;
#else SERV_PC_BANG_TYPE
		if( (HRLC_PREMIUM - m_iHenirRewardPremiumCount) < 0 )
		{
			return 0;
		}

		return HRLC_PREMIUM - m_iHenirRewardPremiumCount;
#endif SERV_PC_BANG_TYPE
	}
	
	return 0; 
}

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
int KUserDungeonManager::GetPossibleHenirChallengeRewardNormalCount( void ) const
{
	const int iResult = HRLC_NORMAL_CHALLENGE - m_iHenirChallengeRewardCount;

	if( iResult < 0 )
	{
		return 0;
	}

	return iResult; 
}

int KUserDungeonManager::GetPossibleHenirChallengeRewardEventCount( void ) const
{
	const int iResult = m_iHenirRewardEventLimitCount - m_iHenirChallengeRewardEventCount;

	if( iResult < 0 )
	{
		return 0;
	}

	return iResult; 
}

int KUserDungeonManager::GetPossibleHenirChallengeRewardPremiumCount( void ) const
{
	if( IsPremiumUser() == true )
	{
		const int iResult = HRLC_PREMIUM - m_iHenirChallengeRewardPremiumCount;
		if( iResult < 0 )
		{
			return 0;
		}

		return iResult;
	}

	return 0;
}
#endif // SERV_HENIR_RENEWAL_2013

#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2012. 04. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KUserDungeonManager::SetDungeonGameInfo( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode )
{
    m_kDungeonGameInfo.m_iDungeonID = iDungeonID;
	m_kDungeonGameInfo.m_cDifficulty = cDifficulty;
	m_kDungeonGameInfo.m_cDungeonMode = cDungeonMode;
}

bool KUserDungeonManager::IsPlayingNowHenirChallengeMode() const
{
	if( m_kDungeonGameInfo.m_iDungeonID == 0 )
		return false;

	const CXSLDungeon::DUNGEON_TYPE eDungeonType = SiCXSLDungeonManager()->GetDungeonType( m_kDungeonGameInfo.GetDungeonIDWithDif() );
	if( eDungeonType != CXSLDungeon::DT_HENIR )
	{
		// ��ϸ� �ð��� �ƴϸ� false
		return false;
	}

	//CXSLDungeon::HENIR_DUNGEON_MODE eHenirDungeonMode = CXSLDungeon::HDM_INVALID;
	//if( !SiKPartyManager()->GetHenirDungeonMode( GetPartyUID(), eHenirDungeonMode ) )
	//{
	//	START_LOG( cerr, L"���� ��带 ���� �� �����ϴ�! ������ ���� ����!" )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( GetPartyUID() )
	//		<< END_LOG;

	//	return false;
	//}

	// ��ϸ� �ð� ������� ����
	return ( (CXSLDungeon::DUNGEON_MODE)m_kDungeonGameInfo.m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE );
}

void KUserDungeonManager::GetAutoPartyBonusDungeonList( IN const int iVillageMapID, OUT std::map< int, KAutoPartyDungeonBonusInfo >& mapAutoPartyBonusList )
{
	const CXSLMapData::SLocalMapInfo* pLocalMapIDList = SiCXSLMapData()->GetLocalMapList( iVillageMapID );
	if( IS_NULL( pLocalMapIDList ) )
	{
		START_LOG( cerr, L"���ø� ����Ʈ ������ ������ �����ϴ�!" )
			<< BUILD_LOG( iVillageMapID )
			<< END_LOG;
		return;
	}

	BOOST_TEST_FOREACH( const KDungeonGameSimpleInfo&, kInfo, pLocalMapIDList->m_vecDungeonInfo )
	{
		KAutoPartyDungeonBonusInfo kAutoPartyBonusInfo;
		kAutoPartyBonusInfo.m_cDungeonMode = kInfo.m_cDungeonMode;
		kAutoPartyBonusInfo.m_iDungeonID = kInfo.m_iDungeonID;
		kAutoPartyBonusInfo.m_bActivatedBonus = SiKGSAutoPartyManager()->IsAutoPartyBonus( kInfo.m_cDungeonMode, kInfo.m_iDungeonID );
		mapAutoPartyBonusList.insert( std::make_pair( kInfo.m_iDungeonID, kAutoPartyBonusInfo ) );
	}
}

void KUserDungeonManager::GetAutoPartyBonusDungeonList( IN const std::vector< KDungeonGameSimpleInfo >& vecDungeonList, OUT std::map< int, KAutoPartyDungeonBonusInfo >& mapAutoPartyBonusList )
{
	BOOST_TEST_FOREACH( const KDungeonGameSimpleInfo&, kInfo, vecDungeonList )
	{
		KAutoPartyDungeonBonusInfo kAutoPartyBonusInfo;
		kAutoPartyBonusInfo.m_cDungeonMode = kInfo.m_cDungeonMode;
		kAutoPartyBonusInfo.m_iDungeonID = kInfo.m_iDungeonID;
		kAutoPartyBonusInfo.m_bActivatedBonus = SiKGSAutoPartyManager()->IsAutoPartyBonus( kInfo.m_cDungeonMode, kInfo.m_iDungeonID );
		mapAutoPartyBonusList.insert( std::make_pair( kInfo.m_iDungeonID, kAutoPartyBonusInfo ) );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


