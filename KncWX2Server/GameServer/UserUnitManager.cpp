#include "GSUser.h"
#include ".\userunitmanager.h"



//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KUserUnitManager::KUserUnitManager(void)
{
	Clear();
}

KUserUnitManager::~KUserUnitManager(void)
{
}

void KUserUnitManager::Clear()
{
	m_kGamePlayStatusContainer.Clear();
	m_vecReservedReturnToFieldInfo.clear();

	//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
	m_bEnterCashShop = false;
#endif SERV_VISIT_CASH_SHOP
	//}}
}

void KUserUnitManager::Init( IN OUT KGamePlayStatusContainer& kNewGamePlayStatus, IN const KRoomUserInfo& kRoomUserInfo )
{
	// DB���� ������ Status������ ���������� [ĳ���� ���� ����]
	KGamePlayStatus kGamePlayStatus;
	if( kNewGamePlayStatus.Get( kGamePlayStatus ) == false )
	{
		kGamePlayStatus.m_iMaxMP = 300;
		kGamePlayStatus.m_iCurMP = 300;

		// �ִ� HP������ ��Ʋ�ʵ� �������� ���� ������ �־�����!
		kGamePlayStatus.m_iMaxHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
		kGamePlayStatus.m_iCurHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
	}
	// DB���� ������ Status������ ������� ���� ���
	else
	{
#ifdef SERV_CREATE_UNIT_INIT_STAT	// ���볯¥: 2013-07-11
		if( kGamePlayStatus.m_iMaxHP <= 0 )		// maxhp �� 0 ���ϴ� ���� �ȵȴ�.
#else
		if( kGamePlayStatus.m_iCurHP == -1 )
#endif	// SERV_CREATE_UNIT_INIT_STAT
		{
			kGamePlayStatus.m_iMaxMP = 300;
			kGamePlayStatus.m_iCurMP = 300;

			// �ִ� HP������ ��Ʋ�ʵ� �������� ���� ������ �־�����!
			kGamePlayStatus.m_iMaxHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
			kGamePlayStatus.m_iCurHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
		}
		else
		{
			// �ִ� HP������ ��Ʋ�ʵ� �������� ���� ������ �־�����!
			kGamePlayStatus.m_iMaxHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
		}
	}    

	// DB���� ������ �÷��� ���°��� �����Ѵ�.
	m_kGamePlayStatusContainer.Set( kGamePlayStatus );

	// Ŭ�󿡵� ���� �������� ��������.
	kNewGamePlayStatus.Set( kGamePlayStatus );

	{
		// HP & MP�� ��������!
		m_kPartyMemberStatus.m_fHpPercent = static_cast<float>(kGamePlayStatus.m_iCurHP) / static_cast<float>(kGamePlayStatus.m_iMaxHP) * 100.f;
		m_kPartyMemberStatus.m_fMpPercent = static_cast<float>(kGamePlayStatus.m_iCurMP) / static_cast<float>(kGamePlayStatus.m_iMaxMP) * 100.f;
	}
}

void KUserUnitManager::SetGamePlayStatusAndCheckChangedHpMp( IN const KGamePlayStatus& kNewGamePlayStatus, OUT bool& bChangedHpMp, OUT KPartyMemberStatus& kHpMpInfo )
{
	bChangedHpMp = false;

	//////////////////////////////////////////////////////////////////////////	
	// ���� ������ ����!
	KGamePlayStatus kOldStatusInfo;
    if( m_kGamePlayStatusContainer.Get( kOldStatusInfo ) == false )
	{        
		// Hp Mp������ ����Ǿ��ٰ� ����!
		bChangedHpMp = true;
	}
	else
	{
		// Hp Mp�� ����Ǿ����� Ȯ��
        if( kOldStatusInfo.m_iMaxHP != kNewGamePlayStatus.m_iMaxHP  ||
			kOldStatusInfo.m_iCurHP != kNewGamePlayStatus.m_iCurHP  ||
			kOldStatusInfo.m_iMaxMP != kNewGamePlayStatus.m_iMaxMP  ||
			kOldStatusInfo.m_iCurMP != kNewGamePlayStatus.m_iCurMP )
		{
			// Hp Mp������ ����Ǿ���!
			bChangedHpMp = true;
		}
	}

	// ��������!
	m_kGamePlayStatusContainer.Set( kNewGamePlayStatus );

	//////////////////////////////////////////////////////////////////////////	
	// Hp Mp �ۼ�Ƽ���� ������!
	if( bChangedHpMp )
	{
		kHpMpInfo.m_fHpPercent = static_cast<float>(kNewGamePlayStatus.m_iCurHP) / static_cast<float>(kNewGamePlayStatus.m_iMaxHP) * 100.f;
		kHpMpInfo.m_fMpPercent = static_cast<float>(kNewGamePlayStatus.m_iCurMP) / static_cast<float>(kNewGamePlayStatus.m_iMaxMP) * 100.f;

		// ��������!
		m_kPartyMemberStatus = kHpMpInfo;
	}
}

void KUserUnitManager::ReservedReturnToFieldInfo( IN const KReturnToFieldInfo& kInfo )
{
    if( m_vecReservedReturnToFieldInfo.empty() == false )
	{
		START_LOG( cerr, L"�ʵ� ���� ������ �����Ϸ��µ� �̹� ����Ǿ��ִ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( m_vecReservedReturnToFieldInfo.size() )
			<< END_LOG;
	}

	// �ϴ� �������� �ʱ�ȭ!
	m_vecReservedReturnToFieldInfo.clear();

	// �׸��� ����!
	m_vecReservedReturnToFieldInfo.push_back( kInfo );
}

bool KUserUnitManager::GetAndDeleteResevedReturnToFieldInfo( OUT KReturnToFieldInfo& kInfo )
{
	if( m_vecReservedReturnToFieldInfo.empty() )
		return false;

    BOOST_TEST_FOREACH( const KReturnToFieldInfo&, kReservedInfo, m_vecReservedReturnToFieldInfo )
	{
		kInfo = kReservedInfo;
		break;
	}

	m_vecReservedReturnToFieldInfo.clear();
	return true;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}



