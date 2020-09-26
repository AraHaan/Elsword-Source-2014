#include ".\userspiritmanager.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLBattleFieldManager.h"
#include "GameEvent/GameEventManager.h"
#include "SpiritTable.h"


#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
#include "UserQuestManager.h"
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST

//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KUserSpiritManager::KUserSpiritManager(void)
{
	Clear();
}

KUserSpiritManager::~KUserSpiritManager(void)
{
}

void KUserSpiritManager::Clear()
{
	SetSpiritMax( 0 );
	SetSpirit( 0 );
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	SetAccumulationSpirit( 0 );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
	SetIsSpiritUpdated( false );
}

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
void KUserSpiritManager::Init( IN const int iSpirit, IN const int iSpiritMax, IN const int iAccumulationSpirit )
#else
void KUserSpiritManager::Init( IN const int iSpirit, IN const int iSpiritMax )
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
{
	SetSpirit( iSpirit );
	SetSpiritMax( iSpiritMax );
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	SetAccumulationSpirit( iAccumulationSpirit );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
}

void KUserSpiritManager::GetDBUpdateInfo( OUT int& iSpirit, OUT bool& bIsSpiritUpdated )
{
    iSpirit = GetSpirit();
	bIsSpiritUpdated = IsSpiritUpdated();
}

int	KUserSpiritManager::RestoreSpirit( IN const int iRestoreSpirit )
{
	// �ټ��� �ִ�ġ �˻�
	if( GetSpiritMax() < ( GetSpirit() + iRestoreSpirit ) )
	{
		m_iSpirit = m_iSpiritMax;
	}
	else
	{
		m_iSpirit += iRestoreSpirit;
	}
	
	return GetSpirit();
}

bool KUserSpiritManager::CheckingSpirit( IN const int iDungeonID, IN const bool bIsPcBang ) const
{
	// 0. ����ID���� ��ȿ���� üũ����!
	if( iDungeonID == 0 )
	{
		START_LOG( cerr, L"����ID���� ��ȿ���� �ʴ�!" )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
		return false;
	}

	// �ټ����� ���þ��� �����̸� �ٷ� ����ó��
	if( CXSLDungeon::IsTutorialDungeon( iDungeonID )  ||
		CXSLDungeon::IsEventDungeon( iDungeonID ) )
	{
		return true;
	}

	//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( iDungeonID ) )
	{
		return true;
	}
#endif SERV_SHIP_DUNGEON
	//}}

	//{{ 2009. 7. 3  ������		��ϸ� �ð�
	if( SiCXSLDungeonManager()->GetDungeonType( iDungeonID ) == CXSLDungeon::DT_HENIR )
	{
		return true;
	}
	//}}

	//{{ 2009. 10. 26  ������	PC�� ����
	if( bIsPcBang == true )
	{
		return true;
	}
	//}}

	// [�̺�Ʈ] 080611.hoons.�ټ��� �̺�Ʈ ���ϰ��..
	if( SiKGameEventManager()->CheckSpiritEvent() == true )
		return true;
	//}}

	// �ټ����� �����ִ��� �˻�
	if( GetSpirit() > 0 )
		return true;

	return false;
}

int	KUserSpiritManager::CalculateDecreaseSpirit( IN const char cUserCount, IN const short sStageNpcCount, IN const bool bIsTutorial, IN const bool bHalfDecreaseEvent, IN const bool bIsBattleField )
{
	//////////////////////////////////////////////////////////////////////////
	// �ټ��� ����
	//
	// Y = (42-(2*X+A))/30*B
	// ( ���Ͱ� �Ѹ��� ���� �� ��� ��Ƽ���� �ټ��� ���ҷ� = Y )
	// Y �� 1 �� ���, Y = 1�� ó�� �Ѵ�. 

	if( sStageNpcCount <= 0 )
		return 0;

	float fDecreaseSpirit = 0.0f;

	// ��Ʋ �ʵ忡���� sStageNpcCount ��� 30���� ����	2013-09-11	�ڼ���
	const float fCopyStageNpcCount = ( bIsBattleField == true ) ? 30.0f : static_cast<float>( sStageNpcCount );

	if( bHalfDecreaseEvent )
	{
		fDecreaseSpirit = (float)( 22 - ( 2 * static_cast<int>(cUserCount) + ( bIsTutorial ? 26 : 0 ) ) ) / fCopyStageNpcCount;
	}
	else
	{
		fDecreaseSpirit = (float)( 42 - ( 2 * static_cast<int>(cUserCount) + ( bIsTutorial ? 46 : 0 ) ) ) / fCopyStageNpcCount;
	}

	// ��Ʋ�ʵ��� ��Ʋ�ʵ� ������� ������!
	if( bIsBattleField )
	{
		//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
		fDecreaseSpirit = fDecreaseSpirit * SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor();
#else
		fDecreaseSpirit = fDecreaseSpirit * SiCXSLBattleFieldManager()->GetBattleFieldFactor();
#endif SERV_FIELD_ED_EXP_FACTOR
		//}
	}

	if( 0.f < fDecreaseSpirit  &&  fDecreaseSpirit < 1.f )
	{
		fDecreaseSpirit = 1.f;
	}
	else if( 0.f > fDecreaseSpirit  &&  fDecreaseSpirit > -1.f  )
	{
		fDecreaseSpirit = -1.f;
	}

	return static_cast<int>(fDecreaseSpirit);
}

#if defined( SERV_LOCAL_RANKING_SYSTEM ) || defined( SERV_CHINA_SPIRIT_EVENT )
bool KUserSpiritManager::DecreaseSpirit( IN const int iDungeonID
									   , IN const char cUserCount
									   , IN const short sStageNpcCount
									   , IN const bool bIsTutorial
									   , IN const bool bIsPcBang
									   , IN const bool bIsBattleField
									   , OUT int& iDecreaseSpirit
									   , OUT bool& bIsSpiritUpdated
									   , OUT int& iAbsoluteDecreaseSpirit
									   )
{
	iDecreaseSpirit = 0;
	bIsSpiritUpdated = false;

	// �Ҹ��ų �ټ��� ���
	//{{ 2011. 09. 23	������	������ ���� �ټ��� �Ҹ� �̺�Ʈ
#ifdef SERV_COEXISTENCE_FESTIVAL
	iAbsoluteDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, true, bIsBattleField ); // [����] ���������ϰ�� �ټ����� ����ȴ�.
#else
	iAbsoluteDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField ); // [����] ���������ϰ�� �ټ����� ����ȴ�.
#endif SERV_COEXISTENCE_FESTIVAL
	//}}
	const int iRemain = GetSpirit() - iAbsoluteDecreaseSpirit;
	if( iRemain < 0 )
	{
		iAbsoluteDecreaseSpirit += iRemain;
	}

	// �Ϲ� ������ ���
	if( bIsBattleField == false )
	{
		// ����ID���� ��ȿ���� üũ����!
		if( iDungeonID == 0 )
		{
			START_LOG( cerr, L"����ID���� ��ȿ���� �ʴ�!" )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
			return false;
		}

		// 1. �ټ��� �Ҹ�ʹ� ���� ������� ������ ��츦 üũ�Ͽ� return��.
		if( CXSLDungeon::IsTutorialDungeon( iDungeonID )  ||
			CXSLDungeon::IsEventDungeon( iDungeonID ) )
		{
			return false;
		}

		//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) )
		{
			return false;
		}
#endif SERV_SHIP_DUNGEON
		//}}

		// 2. ��ϸ� �ð��� �ټ����ʹ� ���� ����.	
		if( SiCXSLDungeonManager()->GetDungeonType( iDungeonID ) == CXSLDungeon::DT_HENIR )
		{
			return false;
		}
	}		

	// 3. ���������� npc�� �Ѹ����� ���ٸ� �ټ��� �Ҹ� ����
	if( sStageNpcCount <= 0 )
	{
		return false;
	}

	// 5. �������谡 �ƴѰ�� �ټ��� �̺�Ʈ�� PC������ �˻�
	if( bIsTutorial == false )
	{
		// [�̺�Ʈ] 080611.hoons.�ټ��� �̺�Ʈ ���ϰ��..
		if( SiKGameEventManager()->CheckSpiritEvent() == true )
		{
			return false;
		}

		//{{ 2009. 10. 26  ������	PC�� ����
		if( bIsPcBang == true )
		{
			return false;
		}
		//}}
	}

	// ���� ���� ��ġ�� �ݿ�
	iDecreaseSpirit = iAbsoluteDecreaseSpirit;

	// �ټ��� �Ҹ� ( ���������ϰ�� ����ó�� )
	m_iSpirit -= iDecreaseSpirit;

	// �ټ��� ũ�� ����
	if( GetSpirit() <= 0 )
	{
		SetSpirit( 0 );

		// �ټ��� �Ϸ�ġ ��� �Ҹ�� ī��Ʈ [�ѹ� ī��Ʈ �ö󰡸� �ٽ� ���ž���]
		if( IsSpiritUpdated() == false )
		{
			SetIsSpiritUpdated( true );
		}
	}
	else if( GetSpirit() > SiKSpiritTable()->GetMaxSpirit() )
	{
		SetSpirit( SiKSpiritTable()->GetMaxSpirit() );
	}

	return true;
}
#else	// SERV_LOCAL_RANKING_SYSTEM
bool KUserSpiritManager::DecreaseSpirit( IN const int iDungeonID, 
										 IN const char cUserCount, 
					   					 IN const short sStageNpcCount, 
					   					 IN const bool bIsTutorial, 
										 IN const bool bIsPcBang,
					   					 IN const bool bIsBattleField,
										 OUT int& iDecreaseSpirit,
										 OUT bool& bIsSpiritUpdated
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
										 , OUT bool& bReward
#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
										 ,IN const bool bCharQuest
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
										 )
{
	iDecreaseSpirit = 0;
	bIsSpiritUpdated = false;

	// �Ϲ� ������ ���
	if( bIsBattleField == false )
	{
		// ����ID���� ��ȿ���� üũ����!
		if( iDungeonID == 0 )
		{
			START_LOG( cerr, L"����ID���� ��ȿ���� �ʴ�!" )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
			return false;
		}

		// 1. �ټ��� �Ҹ�ʹ� ���� ������� ������ ��츦 üũ�Ͽ� return��.
		if( CXSLDungeon::IsTutorialDungeon( iDungeonID )  ||
			CXSLDungeon::IsEventDungeon( iDungeonID ) )
		{
			return false;
		}

		//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) )
		{
			return false;
		}
#endif SERV_SHIP_DUNGEON
		//}}

		// 2. ��ϸ� �ð��� �ټ����ʹ� ���� ����.	
		if( SiCXSLDungeonManager()->GetDungeonType( iDungeonID ) == CXSLDungeon::DT_HENIR )
		{
			return false;
		}
	}		

	// 3. ���������� npc�� �Ѹ����� ���ٸ� �ټ��� �Ҹ� ����
	if( sStageNpcCount <= 0 )
	{
		return false;
	}

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
	// Ư�� ���� ����Ʈ �������� �� �ټ��� ��´�.
	// ���� ����Ʈ ���� ������ ĳ���� ���� ���� �񱳿��� ���� ��쿡�� ������ ����
	if(true == bCharQuest)
	{
		CalculateAccumulationSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField, bReward );

		START_LOG( cout2, L"�輮��_ĳ���� �����̺�Ʈ_���������� ���³�" )
			<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestSpiritQuest )
			<< END_LOG;
	}
#else //SERV_BURNING_CHAR_EVENT_SUB_QUEST
	IF_EVENT_ENABLED( CEI_ACCUMULATION_SPIRIT_EVENT )
	{
		CalculateAccumulationSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField, bReward );
	}	
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

	// 5. �������谡 �ƴѰ�� �ټ��� �̺�Ʈ�� PC������ �˻�
	if( bIsTutorial == false )
	{
		// [�̺�Ʈ] 080611.hoons.�ټ��� �̺�Ʈ ���ϰ��..
		if( SiKGameEventManager()->CheckSpiritEvent() == true )
		{
			return false;
		}

		//{{ 2009. 10. 26  ������	PC�� ����
		if( bIsPcBang == true )
		{
			return false;
		}
		//}}
	}

	// 6. �Ҹ��ų �ټ��� ���
	//{{ 2011. 09. 23	������	������ ���� �ټ��� �Ҹ� �̺�Ʈ
#ifdef SERV_COEXISTENCE_FESTIVAL
	iDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, true, bIsBattleField ); // [����] ���������ϰ�� �ټ����� ����ȴ�.
#else
	iDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, false, bIsBattleField ); // [����] ���������ϰ�� �ټ����� ����ȴ�.
#endif SERV_COEXISTENCE_FESTIVAL
	//}}
	const int iRemain = GetSpirit() - iDecreaseSpirit;
	if( iRemain < 0 )
	{
		iDecreaseSpirit += iRemain;
	}

	// �ټ��� �Ҹ� ( ���������ϰ�� ����ó�� )
	m_iSpirit -= iDecreaseSpirit;

	// �ټ��� ũ�� ����
	if( GetSpirit() <= 0 )
	{
		SetSpirit( 0 );

		// �ټ��� �Ϸ�ġ ��� �Ҹ�� ī��Ʈ [�ѹ� ī��Ʈ �ö󰡸� �ٽ� ���ž���]
		if( IsSpiritUpdated() == false )
		{
			SetIsSpiritUpdated( true );
		}
	}
	else if( GetSpirit() > SiKSpiritTable()->GetMaxSpirit() )
	{
		SetSpirit( SiKSpiritTable()->GetMaxSpirit() );
	}

	return true;
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
void KUserSpiritManager::GetDBUpdateInfo2( OUT int& iAccumulationSpirit )
{
	iAccumulationSpirit = m_iAccumulationSpirit;
}

void KUserSpiritManager::CalculateAccumulationSpirit( IN char cUserCount, IN short sStageNpcCount, IN bool bIsTutorial, IN bool bHalfDecreaseEvent, IN bool bIsBattleField, OUT bool& bReward )
{
	int iDecreaseSpirit = CalculateDecreaseSpirit( cUserCount, sStageNpcCount, bIsTutorial, bHalfDecreaseEvent, bIsBattleField ); // [����] ���������ϰ�� �ټ����� ����ȴ�.

	const int iRemain = GetSpirit() - iDecreaseSpirit;
	if( iRemain < 0 )
	{
		iDecreaseSpirit += iRemain;
	}

	if( iDecreaseSpirit > 0 )
	{
		m_iAccumulationSpirit += iDecreaseSpirit;

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
		START_LOG( cout2, L"�輮��_ĳ���� �����̺�Ʈ_���������� ���³� Ȯ��" )
			<< BUILD_LOG( m_iAccumulationSpirit )
			<< BUILD_LOG( iDecreaseSpirit )
			<< END_LOG;

		if( m_iAccumulationSpirit >= 3360 )					// 960�� �ִ� �ټ����� 70% ��ġ
		{			
			bReward = true;
			m_iAccumulationSpirit -= 3360;
		}
#else //SERV_BURNING_CHAR_EVENT_SUB_QUEST
		if( m_iAccumulationSpirit >= 960 )					// 960�� �ִ� �ټ����� 20% ��ġ
		{			
			bReward = true;
			m_iAccumulationSpirit -= 960;
		}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
	}
}
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


