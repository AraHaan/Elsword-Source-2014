#include "BattleFieldGameManager.h"


//{{ 2011. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldGameManager::KBattleFieldGameManager() : 
m_iDangerousValue( 0 )
//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
, m_iOldDangerousValue( 0 )
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}
{
}

KBattleFieldGameManager::~KBattleFieldGameManager()
{
}

void KBattleFieldGameManager::StartGame()
{
	ResetDangerousValue();
	m_kDangerousEvent.Clear();
	m_kLotEliteMonsterDrop.Clear();
}

void KBattleFieldGameManager::EndGame()
{
	ResetDangerousValue();
	m_kDangerousEvent.Clear();
	m_kLotEliteMonsterDrop.Clear();
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
#else // SERV_BATTLE_FIELD_BOSS
void KBattleFieldGameManager::IncreaseDangerousValue( IN const int iIncreaseValue )
{
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_iOldDangerousValue = m_iDangerousValue;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	
	//{{ 2013. 03. 07	��Ʋ �ʵ� ���赵 �̺�Ʈ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	m_iDangerousValue += iIncreaseValue * SiCXSLBattleFieldManager()->GetDangerousValueEventRate();
#else
	m_iDangerousValue += iIncreaseValue;
#endif SERV_BATTLE_FIELD_DANGEROUS_VALUE_EVENT
	//}

	if( ( m_iDangerousValue < 0 ) || ( SiCXSLBattleFieldManager()->GetDangerousValueMax() <= m_iDangerousValue ) )
	{
		ResetDangerousValue();
	}
}

void KBattleFieldGameManager::OnNpcUnitDie( IN const int iPlayerCount, 
										    IN const bool bIsAttribNpc, 
											IN const char cDifficultyLevel, 
											IN const char cMonsterGrade
											//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
											, IN const bool bIncreaseDanger 
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
											//}
											)
{
	// �������� ���赵 ����
	const int iBeforeDangeroursValue = GetDangerousValue();

	// ���� ������ ���� Ÿ�� ������� ���Ѵ�.
	const int iMonsterTypeFactor = CXSLUnitManager::GetMonsterTypeFactor( bIsAttribNpc, cDifficultyLevel, cMonsterGrade );

	// ���赵�� ���� ��Ų��.
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	if( bIncreaseDanger == true )
	{
		IncreaseDangerousValue( iMonsterTypeFactor );
	}
	else
	{
		return;
	}
#else
	IncreaseDangerousValue( iMonsterTypeFactor );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	//IncreaseDangerousValue( 500 ); //-- �׽�Ʈ�� �ϴ� 500�� ������Ű��!

	// ��� �޽��� �̺�Ʈ üũ
	CheckReserveWarningEvent( iBeforeDangeroursValue );	

	// ����Ʈ ���� ���� �̺�Ʈ üũ
	CheckReserveEliteMonsterDropEvent( iBeforeDangeroursValue );

	// �߰� ���� ���� �̺�Ʈ üũ
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CheckReserveMiddleBossDropEvent( iPlayerCount );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	CheckReserveEventBossDropEvent( iPlayerCount );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	
	// ���� ���� �̺�Ʈ üũ
	CheckReserveBossDropEvent( iPlayerCount );
}
#endif // SERV_BATTLE_FIELD_BOSS

void KBattleFieldGameManager::CheckReserveWarningEvent( IN const int iBeforeDangerousValue )
{
	// ���赵 ��ġ�� ������ ������ ������ ��� �̺�Ʈ�� �߻� ��Ų��!
	if( iBeforeDangerousValue < SiCXSLBattleFieldManager()->GetDangerousValueWarning()  &&  GetDangerousValue() >= SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_WARNING_MESSAGE );
	}
}

void KBattleFieldGameManager::CheckReserveEliteMonsterDropEvent( IN const int iBeforeDangerousValue )
{
	// ���赵�� �ּ��� 5�� �Ѿ�� �Ѵ�.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() )
		return;

	if( SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() == 0 )
	{
		START_LOG( cerr, L"����Ʈ ���� ���� ������� 0�̴�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() )
			<< END_LOG;
		return;
	}

	// ���赵�� ������� ����ΰ�?
	const bool bIsMultiplesOf = ( ( GetDangerousValue() % SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() ) == 0 );

	// ���赵 ������� ����� �����ƴ°�? �׷� �������� ����� �̻� �����ϸ� �װ� ����ģ�� �ƴѰ�?
	const bool bIsGap = ( ( GetDangerousValue() - iBeforeDangerousValue ) > SiCXSLBattleFieldManager()->GetEliteMonsterDropValue() );

	// �� ���׿� ��� �ش��� �ȵǸ� ������!
	if( bIsMultiplesOf == false  &&  bIsGap == false )
		return;

	// ������ Ȯ���� 20%�� ���ؼ� ��������!
	static int ELITE_MONSTER_DROP_CASE = 1;
	LIF( m_kLotEliteMonsterDrop.AddCase( ELITE_MONSTER_DROP_CASE, 20.f ) );

	// ���� ����� ok�� Ȯ�� �ʱ�ȭ, �ƴϸ� �н�!
	const int iResult = m_kLotEliteMonsterDrop.Decision();
	if( iResult != ELITE_MONSTER_DROP_CASE )
		return;

	// ��� �����̸� Ȯ���� �ʱ�ȭ ����!
	m_kLotEliteMonsterDrop.Clear();

	// ����Ʈ ���� ��� ����!
	m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_ELITE_MONSTER_DROP );
}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void KBattleFieldGameManager::CheckReserveMiddleBossDropEvent( IN const int iPlayerCount )
{
	// ���� �ο��� n�� ���ϸ� ������!
	if( iPlayerCount < SiCXSLBattleFieldManager()->GetBossCheckUserCount() )
		return;

	// ���赵 ��ġ�� �ּ��� ���� ���� �̻��̾�� �Ѵ�.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
		return;

	// �̹� ���� �̺�Ʈ�� ���� ����� ����Ǿ��ִٸ� ������!
	if( m_kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP ) == true )
		return;

	//////////////////////////////////////////////////////////////////////////
	// ���� �̺�Ʈ �������� �غ�!
	// 4300 ���ķδ� 50�����Ҷ����� �̺�Ʈ �߻� Ȯ���� 10%�� �����Ѵ�.
	static int BOSS_DROP_EVENT = 1;
	const float fBossDropRate = SiCXSLBattleFieldManager()->GetMiddleBossMonsterDropRate( GetDangerousValue(), GetOldDangerousValue() );
	//////////////////////////////////////////////////////////////////////////

	KLottery kBossDropLot;
	LIF( kBossDropLot.AddCase( BOSS_DROP_EVENT, fBossDropRate ) );

	// ���� �̺�Ʈ ����!
	const int iResult = kBossDropLot.Decision();
	if( iResult == BOSS_DROP_EVENT )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_MIDDLE_BOSS_MONSTER_DROP );
	}
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
void KBattleFieldGameManager::CheckReserveEventBossDropEvent( IN const int iPlayerCount )
{
	// ���� �ο��� n�� ���ϸ� ������!
	if( iPlayerCount < SiCXSLBattleFieldManager()->GetBossCheckUserCount() )
		return;

	// ���赵 ��ġ�� �ּ��� ���� ���� �̻��̾�� �Ѵ�.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
		return;

	// �̹� ���� �̺�Ʈ�� ���� ����� ����Ǿ��ִٸ� ������!
	if( m_kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP ) == true )
		return;

	//////////////////////////////////////////////////////////////////////////
	// ���� �̺�Ʈ �������� �غ�!
	// 4300 ���ķδ� 50�����Ҷ����� �̺�Ʈ �߻� Ȯ���� 10%�� �����Ѵ�.
	static int BOSS_DROP_EVENT = 1;
	const float fBossDropRate = SiCXSLBattleFieldManager()->GetEventBossMonsterDropRate( GetDangerousValue(), GetOldDangerousValue() );
	//////////////////////////////////////////////////////////////////////////

	KLottery kBossDropLot;
	LIF( kBossDropLot.AddCase( BOSS_DROP_EVENT, fBossDropRate ) );

	// ���� �̺�Ʈ ����!
	const int iResult = kBossDropLot.Decision();
	if( iResult == BOSS_DROP_EVENT )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_EVENT_BOSS_MONSTER_DROP );
	}
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

void KBattleFieldGameManager::CheckReserveBossDropEvent( IN const int iPlayerCount )
{
	// ���� �ο��� n�� ���ϸ� ������!
	if( iPlayerCount < SiCXSLBattleFieldManager()->GetBossCheckUserCount() )
		return;

	// ���赵 ��ġ�� �ּ��� ���� ���� �̻��̾�� �Ѵ�.
	if( GetDangerousValue() < SiCXSLBattleFieldManager()->GetDangerousValueWarning() )
		return;

	// �̹� ���� �̺�Ʈ�� ���� ����� ����Ǿ��ִٸ� ������!
	if( m_kDangerousEvent.IsEventReserved( KDangerousEventInfo::DE_BOSS_MONSTER_DROP ) == true )
		return;

	//////////////////////////////////////////////////////////////////////////
	// ���� �̺�Ʈ �������� �غ�!
	// 4300 ���ķδ� 50�����Ҷ����� �̺�Ʈ �߻� Ȯ���� 10%�� �����Ѵ�.
	static int BOSS_DROP_EVENT = 1;
	const float fBossDropRate = SiCXSLBattleFieldManager()->GetBossMonsterDropRate( GetDangerousValue() );
	//////////////////////////////////////////////////////////////////////////

	KLottery kBossDropLot;
	LIF( kBossDropLot.AddCase( BOSS_DROP_EVENT, fBossDropRate ) );

	// ���� �̺�Ʈ ����!
	const int iResult = kBossDropLot.Decision();
	if( iResult == BOSS_DROP_EVENT )
	{
		m_kDangerousEvent.ReserveEvent( KDangerousEventInfo::DE_BOSS_MONSTER_DROP );
	}
}

bool KBattleFieldGameManager::CheckAndDeleteReservedDangerousEvent( IN const KDangerousEventInfo::DANGEROUS_EVENT eEventEnum )
{
    const bool bRet = m_kDangerousEvent.IsEventReserved( eEventEnum );
	if( bRet )
	{
		m_kDangerousEvent.DeleteEvent( eEventEnum );
	}
	return bRet;
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
void KBattleFieldGameManager::UpdateDangerousValue( IN const int iDangerousValue )
{
	m_iOldDangerousValue	= m_iDangerousValue;
	m_iDangerousValue		= iDangerousValue;
}
#endif // SERV_BATTLE_FIELD_BOSS

#endif SERV_BATTLE_FIELD_SYSTEM
//}}
