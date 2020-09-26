#include ".\userpet.h"
#include "BaseServer.h"
#include "Lottery.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "NetError.h"


//{{ 2010. 07. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM


KUserPet::KUserPet( const KPetInfo& kInfo, const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, bool bCreatePet )
{
	//{{edit : ����
	m_sExtroversion.SetVerifyMin(static_cast<short>(CXSLPetManager::PE_MIN_EXTROVERSION));
	m_sEmotion.SetVerifyMin(static_cast<short>(CXSLPetManager::PE_MIN_EMOTION));
	//}}edit : ����

	// �ʱ�ȭ
	Clear();

	// �� ���� ������Ʈ
	UpdatePetInfo( kInfo );

	// �� �κ��丮 ������Ʈ
	std::map< UidType, std::map< UidType, KInventoryItemInfo > >::const_iterator mit;
	mit = mapPetItem.find( kInfo.m_iPetUID );
	if( mit != mapPetItem.end() )
	{
		UpdatePetInventory( mit->second );
	}

	// �� �ִ� ģ�е� ����
	if( SiCXSLPetManager()->GetEvolutionStepMaxIntimacy( GetPetID(), GetEvolutionStep(), m_iMaxIntimacy ) == false )
	{
		START_LOG( cerr, L"���� �ִ� ģ�е��� ��Ⱑ �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}

	// �� �����ϴ°Ŷ��?
	if( bCreatePet )
	{
		// DB������Ʈ ����!
		m_bReservedDBUpdate = true;
	}
}

KUserPet::~KUserPet(void)
{
}

void KUserPet::Clear()
{
	m_iPetUID				= 0;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	m_iPetID				= 0;
#else //SERV_PETID_DATA_TYPE_CHANGE
	m_cPetID				= 0;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	m_cEvolutionStep		= 0;
	m_bAutoFeed				= false;
	m_iCumulativeSatiety	= 0;
	m_iFeedCount			= 0;
	m_iCumulativeIntimacy	= 0;
	m_iMaxIntimacy			= 0;
	m_bReservedDBUpdate		= false;
	m_tPetSommonTimer.restart();
	m_mapPetInventory.clear();
#ifdef SERV_PERIOD_PET
	m_bDestroy				= false;
#endif SERV_PERIOD_PET
#ifdef SERV_PET_SYSTEM_EX1
	m_bAlwaysMaxSatiety = false;
#endif //SERV_PET_SYSTEM_EX1
}

void KUserPet::GetPetInfo( OUT KPetInfo& kInfo ) const
{
	kInfo.m_iPetUID				= m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kInfo.m_iPetID				= m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_cPetID				= m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_wstrPetName			= m_wstrPetName;
	kInfo.m_cEvolutionStep		= m_cEvolutionStep;
	kInfo.m_sSatiety			= m_sSatiety;
	kInfo.m_iIntimacy			= m_iIntimacy;
	kInfo.m_sExtroversion		= m_sExtroversion;
	kInfo.m_sEmotion			= m_sEmotion;
	kInfo.m_bAutoFeed			= m_bAutoFeed;
	//kInfo.m_wstrRegDate			= ( std::wstring )( m_tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	//kInfo.m_wstrLastSummonDate	= ( std::wstring )( m_tLastSummonDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
#ifdef SERV_PET_AUTO_LOOTING
	kInfo.m_bAutoLooting		= m_bAutoLooting;
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_EVENT_PET_INVENTORY
	kInfo.m_bEventFoodEat		= m_bEventFoodEat;
	kInfo.m_bIsEventPetID       = m_bIsEventPetID;
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_PERIOD_PET
	kInfo.m_wstrDestroyDate		= L"";
	if( m_bDestroy == true )
	{
		kInfo.m_wstrDestroyDate = ( std::wstring )( m_tDestroyDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	}
#endif SERV_PERIOD_PET
#ifdef SERV_PET_SYSTEM_EX1
	kInfo.m_bAlwaysMaxSatiety	= m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1
}

int KUserPet::GetPetInventorySlotSize() const
{
	// �� ������ ��ȭ�ܰ迡 ���� �κ��丮 ũ�� ���
	return SiCXSLPetManager()->GetPetInventorySlotSize( GetPetID(), GetEvolutionStep() );
}

//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
void KUserPet::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#else
void KUserPet::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	// ���� ����� ������ ��� 
	if( IsHungry() == true )
	{
		// ���� ��ų ���� �ȵ�!
		return;
	}

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int > vecAuraSkillOption;
#else
	std::vector< short > vecAuraSkillOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	// �� ���� �ɼ� ���
	if( SiCXSLPetManager()->GetAuraSkillOption( static_cast<CXSLPetManager::PET_UNIT_ID>(GetPetID()), GetEvolutionStep(), vecAuraSkillOption ) == false )
	{
		START_LOG( cerr, L"���� ���� �ʴ� PetID�Դϴ�!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
		return;
	}

	// ���� �ɼ� ���� ���
	KStat kSocketOptionStat;
	//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	SiCXSLSocketItem()->GetSocketOptionStat( vecAuraSkillOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
#else
	SiCXSLSocketItem()->GetSocketOptionStat( vecAuraSkillOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	// ���� ���ϱ�
	kAddStat.AddStat( kSocketOptionStat );
}

void KUserPet::GetStatIncreaseRateByPetSkill( IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo )
{
	// ���� ����� ������ ���
	if( IsHungry() == true )
	{
		// ���� ��ų ���� �ȵ�!
		return;
	}

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int > vecAuraSkillOption;
#else

	std::vector< short > vecAuraSkillOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	// �� ���� �ɼ� ���
	if( SiCXSLPetManager()->GetAuraSkillOption( static_cast<CXSLPetManager::PET_UNIT_ID>(GetPetID()), GetEvolutionStep(), vecAuraSkillOption ) == false )
	{
		START_LOG( cerr, L"���� ���� �ʴ� PetID�Դϴ�!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
		return;
	}

	// ���� �ɼ� ���� ���
	SiCXSLSocketItem()->GetSocketOptionIncreaseRate( vecAuraSkillOption, bIsDungeon, kStatIncreaseRateInfo );
}

#endif SERV_PET_AURA_SKILL
//}}

void KUserPet::GetDBUpdatePetInfo( OUT KPetInfo& kInfo )
{
	// ���簪
	kInfo.m_iPetUID				= m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kInfo.m_iPetID				= m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_cPetID				= m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_wstrPetName			= m_wstrPetName;
	kInfo.m_cEvolutionStep		= m_cEvolutionStep;
	kInfo.m_bAutoFeed			= m_bAutoFeed;
	kInfo.m_wstrLastFeedDate	= ( std::wstring )( m_tFirstFeedDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kInfo.m_wstrLastSummonDate	= ( std::wstring )( m_tLastSummonDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
    
	// ���а�
	kInfo.m_sSatiety		= m_sSatiety.GetChangeValue();
	m_sSatiety.SetValue( m_sSatiety );

	kInfo.m_iIntimacy		= m_iIntimacy.GetChangeValue();
	m_iIntimacy.SetValue( m_iIntimacy );

	kInfo.m_sExtroversion	= m_sExtroversion.GetChangeValue();
	m_sExtroversion.SetValue( m_sExtroversion );

	kInfo.m_sEmotion		= m_sEmotion.GetChangeValue();
	m_sEmotion.SetValue( m_sEmotion );
}

void KUserPet::RollBackDBUpdatePetInfo( IN const KPetInfo& kInfo )
{
	// ���а� �ѹ�
	m_sSatiety.AdjustInitValue( m_sSatiety.GetInitValue() - kInfo.m_sSatiety );
	m_iIntimacy.AdjustInitValue( m_iIntimacy.GetInitValue() - kInfo.m_iIntimacy );
	m_sExtroversion.AdjustInitValue( m_sExtroversion.GetInitValue() - kInfo.m_sExtroversion );
	m_sEmotion.AdjustInitValue( m_sEmotion.GetInitValue() - kInfo.m_sEmotion );
}

void KUserPet::Summon( OUT KPetInfo& kPetInfo, OUT int& iPetCategorySlotSize, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	//////////////////////////////////////////////////////////////////////////	
	// ��ȯ�� üũ�ϴ� �Լ��� ȣ��!
    
	// �� ���� ���
	GetPetInfo( kPetInfo );

	// �� �κ��丮 ���� ���� ���
	iPetCategorySlotSize = GetPetInventorySlotSize();

	// ���� ������ŭ �������� �ִ��� üũ!
	LIF( iPetCategorySlotSize >= static_cast<int>(mapPetItem.size()) );

	// �� �κ��丮 ���
	GetPetInventory( mapPetItem );

	// ��ȯ�� �Ǿ��ٸ� DB������Ʈ ���� ����!
	m_bReservedDBUpdate = true;

	// �� ��ȯ �ð� ����!
	m_tLastSummonDate = CTime::GetCurrentTime();
}

void KUserPet::Unsummon()
{
	//////////////////////////////////////////////////////////////////////////	
	// ��ȯ ������ �����ϴ� �Լ��� ȣ��!
    
	// �������� ����!
	CheckExtroversion();

	// �������� ����!
	CheckEmotion();
}

void KUserPet::UpdatePetInfo( IN const KPetInfo& kPetInfo )
{
	m_iPetUID			= kPetInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	m_iPetID			= kPetInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	m_cPetID			= kPetInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	m_wstrPetName		= kPetInfo.m_wstrPetName;
	m_cEvolutionStep	= kPetInfo.m_cEvolutionStep;		// ��ȭ �ܰ�
	m_sSatiety.SetValue( kPetInfo.m_sSatiety );				// ������
	m_iIntimacy.SetValue( kPetInfo.m_iIntimacy );			// ģ�е�
	m_sExtroversion.SetValue( kPetInfo.m_sExtroversion );	// ���⼺
	m_sEmotion.SetValue( kPetInfo.m_sEmotion );				// ������
	m_bAutoFeed			= kPetInfo.m_bAutoFeed;
	//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
#ifdef SERV_PET_AUTO_LOOTING
	m_bAutoLooting		= kPetInfo.m_bAutoLooting;			// ���� ���� ��� ����
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_PET_SYSTEM_EX1
	m_bAlwaysMaxSatiety = kPetInfo.m_bAlwaysMaxSatiety;	// �׻� �ִ� ������
#endif//SERV_PET_SYSTEM_EX1
#ifdef SERV_EVENT_PET_INVENTORY
		m_bEventFoodEat		= kPetInfo.m_bEventFoodEat;
		m_bIsEventPetID		= kPetInfo.m_bIsEventPetID;
#endif SERV_EVENT_PET_INVENTORY
	if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrLastFeedDate, m_tFirstFeedDate ) == false )
	{
		START_LOG( cerr, L"ù ���� �ð� ���� ��ȯ ����." )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< BUILD_LOG( kPetInfo.m_wstrLastFeedDate )
			<< END_LOG;

		// ��ȯ �����ϸ� ���� 6�ø� �������� ù ���� �ð��� ����
		CTime tCurTime = CTime::GetCurrentTime();
		CTime tFirstFeedDate = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 6, 0, 0, 0 );
		if( tFirstFeedDate < tCurTime )
		{
			tFirstFeedDate += CTimeSpan( 1, 0, 0, 0	);
		}

		m_tFirstFeedDate = tFirstFeedDate;
	}

	if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrLastSummonDate, m_tLastSummonDate ) == false )
	{
		START_LOG( cerr, L"������ ��ȯ �ð� ���� ��ȯ ����." )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< BUILD_LOG( kPetInfo.m_wstrLastSummonDate )
			<< END_LOG;

		// ��ȯ �����ϸ� ���� �ð����� ����
		m_tLastSummonDate = CTime::GetCurrentTime();
	}

	if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrRegDate, m_tRegDate ) == false )
	{
		START_LOG( cerr, L"�� ���� �ð� ���� ��ȯ ����." )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< BUILD_LOG( kPetInfo.m_wstrRegDate )
			<< END_LOG;

		// ��ȯ �����ϸ� ���� �ð����� ����
		m_tRegDate = CTime::GetCurrentTime();
	}

#ifdef SERV_PERIOD_PET
	m_bDestroy = false;
	if( kPetInfo.m_wstrDestroyDate != L"" )
	{
		if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrDestroyDate, m_tDestroyDate ) == true )
		{
			m_bDestroy = true;
		}
		else
		{
			START_LOG( cerr, L"���� �ð� ���� ��ȯ ����." )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOG( kPetInfo.m_wstrDestroyDate )
				<< END_LOG;
		}
	}
#endif SERV_PERIOD_PET
}

void KUserPet::UpdatePetInventory( IN const std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
    m_mapPetInventory.clear();
	m_mapPetInventory = mapPetItem;
}

bool KUserPet::CommandsForPet( IN CXSLPetManager::PET_ACTION_COMMAND eActionCommand, OUT CXSLPetManager::PET_ACTION_COMMAND& ePetActionResult )
{
	ePetActionResult = CXSLPetManager::PAC_NONE;

	// �������� ������� üũ!
	if( CXSLPetManager::IsValidCommand( eActionCommand ) == false )
	{
		START_LOG( cerr, L"�������� ��ɰ��� �ƴմϴ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOG( eActionCommand )
			<< END_LOG;

		return false;
	}

	// �̰����� ���� �ȵǴ� ���
	switch( eActionCommand )
	{
	case CXSLPetManager::PAC_EAT:
	case CXSLPetManager::PAC_STUFFED:
		{
			START_LOG( cerr, L"�̰����� ���� �ȵǴ� ����Դϴ�!" )
				<< BUILD_LOG( GetPetUID() )
				<< BUILD_LOG( eActionCommand )
				<< END_LOG;
		}
		return false;
	}

	// ������ �׼��� ���ϴ� ������� �˻�
	if( CXSLPetManager::IsUnconditionalCommand( eActionCommand ) == true )
	{
		ePetActionResult = eActionCommand;
		return true;
	}

	// �ִ� ģ�е��� �˻�
	if( GetMaxIntimacy() <= 0 )
	{
		START_LOG( cerr, L"�ִ� ģ�е����� �̻��մϴ�. �꿡�� ��ɳ����� ����!" )
			<< BUILD_LOG( GetPetUID() )			
			<< BUILD_LOG( GetMaxIntimacy() )
			<< BUILD_LOG( eActionCommand )
			<< END_LOG;

		return false;
	}

	// ������ �ۼ�Ƽ���� 40% �����϶� ����� �׼��� ��������!
	if( ( static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f ) <= 40.f )
	{
		//{{ 2010. 10. 06	������	�� ��� ���� �Լ�
#ifdef SERV_PET_COMMAND_RANDOM
		const int iHungryActionRate = static_cast<int>( 100.f - static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f );

		// �׼� ����!
		const int iRandResult = ( rand() % 100 ) + 1;
		if( iRandResult < iHungryActionRate )
		{
			ePetActionResult = CXSLPetManager::PAC_HUNGRY;
			return true;
		}
#else
		const float fHungryActionRate = 100.f - static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f;

		// ����� �׼��� ���� Ȯ���� ����!
		KLottery kLot;
		kLot.AddCase( 0, fHungryActionRate );

		// �׼� ����!
		if( kLot.Decision() != KLottery::CASE_BLANK )
		{
			ePetActionResult = CXSLPetManager::PAC_HUNGRY;
			return true;
		}
#endif SERV_PET_COMMAND_RANDOM
		//}}
	}
	
	// ģ�е� �ۼ�Ƽ���� �׼��� ��������!
	{
		//{{ 2010. 10. 06	������	�� ��� ���� �Լ�
#ifdef SERV_PET_COMMAND_RANDOM
		int iActionRate = static_cast<int>( static_cast<float>(GetIntimacy()) / static_cast<float>(GetMaxIntimacy()) * 100.f );
		if( iActionRate < 20 )
		{
			// �ּ� 20%�� ����Ȯ���� ����
			iActionRate = 20;
		}

		// �׼� ����!
		const int iRandResult = ( rand() % 100 ) + 1;
		if( iRandResult > iActionRate )
		{
			ePetActionResult = CXSLPetManager::PAC_DONOTKNOW;
			return true;
		}
#else
		float fActionRate = static_cast<float>(GetIntimacy()) / static_cast<float>(GetMaxIntimacy()) * 100.f;
		if( fActionRate < 20.f )
		{
			// �ּ� 20%�� ����Ȯ���� ����
			fActionRate = 20.f;
		}

		// ����� �˾Ƶ��� Ȯ���� ����!
		KLottery kLot;
		kLot.AddCase( 0, fActionRate );

		// �׼� ����!
		if( kLot.Decision() == KLottery::CASE_BLANK )
		{
			ePetActionResult = CXSLPetManager::PAC_DONOTKNOW;
			return true;
		}
#endif SERV_PET_COMMAND_RANDOM
		//}}		
	}	
    
	ePetActionResult = eActionCommand;
	return true;
}

//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
void KUserPet::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, IN const bool& bHaveToy )
#else
void KUserPet::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum )
#endif SERV_PET_TOY
//}}
{
	if( sSubStageExpNpcNum <= 0 )
	{
		// ����ġ �ִ� npc�� �����Ƿ� �������� ���� ����.
		return;
	}

	// �������� ���� ���� ���!
	//const short sDecreaseSatiety = static_cast<short>( ( 21.0 - static_cast<double>(cUserCount) ) / static_cast<double>(sSubStageExpNpcNum) );
	//{{ 2011. 04. 25	������	�� ������ �Ҹ� ����
#ifdef SERV_PET_SATIETY_NEW
	//{{ 2012. 06. 01	��μ�       Ư�� ������������ �� �������� �������� �ʴ� ���� ����
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnDungeonRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep(), sSubStageExpNpcNum );
#else
	const short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnDungeonRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep(), sSubStageExpNpcNum );
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}
#else
	const short sDecreaseSatiety = static_cast<short>( 30.0 / static_cast<double>(sSubStageExpNpcNum) ); // ������ ����
#endif SERV_PET_SATIETY_NEW
	//}}

	//{{ 2012. 06. 01	��μ�       Ư�� ������������ �� �������� �������� �ʴ� ���� ����
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	if( sDecreaseSatiety < 1 )
	{
		START_LOG( clog, L"�� ������ ���Ұ��� �̻��մϴ�!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< BUILD_LOG( sSubStageExpNpcNum )
			<< END_LOG;

		sDecreaseSatiety = 1;
	}
#else
	if( sDecreaseSatiety == 0 )
	{
		START_LOG( cerr, L"�� ������ ���Ұ��� �̻��մϴ�!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< BUILD_LOG( sSubStageExpNpcNum )
			<< END_LOG;
	}
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}
	

	// �������� ����!
	DecreaseSatiety( sDecreaseSatiety );

	// ģ�е� ������ ���� �̸� ����ؼ� ������Ű��!
	
	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_DUNGEON, cUserCount, GetSatiety(), bHaveToy );
#else
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_DUNGEON, cUserCount, GetSatiety() );
#endif SERV_PET_TOY
	//}}

	if( iCalcIntimacy > 0 )
	{
        IncreaseIntimacy( iCalcIntimacy );
	}
	else if( iCalcIntimacy < 0 )
	{
        DecreaseIntimacy( abs( iCalcIntimacy ) );
	}
}

//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
void KUserPet::DecreaseSatietyOnPvpRoom( IN char cUserCount, IN const bool& bHaveToy )
#else
void KUserPet::DecreaseSatietyOnPvpRoom( IN char cUserCount )
#endif SERV_PET_TOY
//}}
{
	// �������� ���� ���� ���!
	//const short sDecreaseSatiety = static_cast<short>( ( 21.0 - static_cast<double>(cUserCount) ) / 2.5 );
	//{{ 2011. 04. 25	������	�� ������ �Ҹ� ����
#ifdef SERV_PET_SATIETY_NEW
	//{{ 2012. 06. 01	��μ�       Ư�� ������������ �� �������� �������� �ʴ� ���� ����
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnPvpRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep() );
#else
	const short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnPvpRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep() );
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}
#else
	const short sDecreaseSatiety = static_cast<short>( 30.0 / 2.5 ); // ������ ����
#endif SERV_PET_SATIETY_NEW
	//}}

	//{{ 2012. 06. 01	��μ�       Ư�� ������������ �� �������� �������� �ʴ� ���� ����
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	if( sDecreaseSatiety < 1 )
	{
		START_LOG( clog, L"�� ������ ���Ұ��� �̻��մϴ�!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;

		sDecreaseSatiety = 1;
	}
#else
	if( sDecreaseSatiety == 0 )
	{
		START_LOG( cerr, L"�� ������ ���Ұ��� �̻��մϴ�!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}

	// �������� ����!
	DecreaseSatiety( sDecreaseSatiety );

	// ����Ǵ� ģ�е� ����ؼ� �����ϱ�!

	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_PVP, cUserCount, GetSatiety(), bHaveToy );
#else
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_PVP, cUserCount, GetSatiety() );
#endif SERV_PET_TOY
	//}}

	if( iCalcIntimacy > 0 )
	{
		IncreaseIntimacy( iCalcIntimacy );
	}
	else if( iCalcIntimacy < 0 )
	{
		DecreaseIntimacy( abs( iCalcIntimacy ) );
	}
}

short KUserPet::IncreaseSatiety( IN short sIncreaseSatiety )
{
	short sIncreased = 0;

	if( sIncreaseSatiety <=  0 )
	{
		START_LOG( cwarn, L"������Ű���� ������ ���� �̻���." )
			<< BUILD_LOG( sIncreaseSatiety )
			<< END_LOG;

		return GetSatiety();
	}

	if( GetSatiety() < 0 )
	{
		START_LOG( cerr, L"������ ���� �̻���." )
			<< BUILD_LOG( GetSatiety() )
			<< END_LOG;

		return GetSatiety();
	}

	// �ʹ� ���� ������ �ִ�ġ�� ���� �ʵ��� �Ѵ�!
	sIncreased = std::min< int >( ( CXSLPetManager::PE_MAX_SATIETY - GetSatiety() ), sIncreaseSatiety );

	// ������!
	m_sSatiety += sIncreased;

	// ����Ǵ� �� ����
	m_iCumulativeSatiety += sIncreased;

	// ������ ���� Ƚ�� ����
	++m_iFeedCount;

	// ����� ���� ����������!
	return GetSatiety();
}

short KUserPet::DecreaseSatiety( IN short sDecreaseSatiety )
{
	short sDecreased = 0;

	if( sDecreaseSatiety <=  0 )
	{
		START_LOG( cerr, L"���ҽ�Ű���� ������ ���� �̻���." )
			<< BUILD_LOG( sDecreaseSatiety )
			<< END_LOG;

		return GetSatiety();
	}

	if( GetSatiety() < 0 )
	{
		START_LOG( cerr, L"������ ���� �̻���." )
			<< BUILD_LOG( GetSatiety() )
			<< END_LOG;

		return GetSatiety();
	}

	// �ʹ� ���� ��Ƽ� ���̳ʽ����� ���� �ʵ��� �Ѵ�!
	sDecreased = std::min< short >( GetSatiety(), sDecreaseSatiety );

#ifdef SERV_PET_SYSTEM_EX1
	if( m_bAlwaysMaxSatiety == true )
		sDecreased = 0;
#endif //SERV_PET_SYSTEM_EX1
	// ����!
	m_sSatiety -= sDecreased;

	// ����Ǵ� �� ���� - ��� ���� ������ ���� ����.
	//m_iCumulativeSatiety -= sDecreased;

	// ����� ���� ����������!
    return GetSatiety();
}

int KUserPet::IncreaseIntimacy( IN int iIncreaseIntimacy )
{
	int iIncreased = 0;

	if( iIncreaseIntimacy <=  0 )
	{
		START_LOG( cerr, L"������Ű���� ģ�е� ���� �̻���." )
			<< BUILD_LOG( iIncreaseIntimacy )
			<< END_LOG;

		return GetIntimacy();
	}

	if( GetIntimacy() < 0 )
	{
		START_LOG( cerr, L"������ ���� �̻���." )
			<< BUILD_LOG( GetIntimacy() )
			<< END_LOG;

		return GetIntimacy();
	}

	// �ʹ� ���� ������ �ִ�ġ�� ���� �ʵ��� �Ѵ�!
	iIncreased = std::min< int >( ( GetMaxIntimacy() - GetIntimacy() ), iIncreaseIntimacy );

	// ������!
	m_iIntimacy += iIncreased;

	// ����Ǵ� �� ����
	m_iCumulativeIntimacy += iIncreased;

	// ����� ���� ����������!
	return GetIntimacy();
}

int KUserPet::DecreaseIntimacy( IN int iDecreaseIntimacy )
{
	int iDecreased = 0;

	if( iDecreaseIntimacy <=  0 )
	{
		START_LOG( cerr, L"���ҽ�Ű���� ģ�е� ���� �̻���." )
			<< BUILD_LOG( iDecreaseIntimacy )
			<< END_LOG;

		return GetIntimacy();
	}

	if( GetIntimacy() < 0 )
	{
		START_LOG( cerr, L"ģ�е� ���� �̻���." )
			<< BUILD_LOG( GetIntimacy() )
			<< END_LOG;

		return GetIntimacy();
	}

	// �ʹ� ���� ��Ƽ� ���̳ʽ����� ���� �ʵ��� �Ѵ�!
	iDecreased = std::min< int >( GetIntimacy(), iDecreaseIntimacy );

	// ����!
	m_iIntimacy -= iDecreased;

	// ����Ǵ� �� ����
	m_iCumulativeIntimacy -= iDecreased;

	// ����� ���� ����������!
	return GetIntimacy();
}

short KUserPet::IncreaseExtroversion( IN short sIncreaseExtroversion )
{
	//short sIncreased = 0;

	if( sIncreaseExtroversion <=  0 )
	{
		START_LOG( cerr, L"������Ű���� ���⼺���� �̻���." )
			<< BUILD_LOG( sIncreaseExtroversion )
			<< END_LOG;

		return GetExtroversion();
	}

	//{{edit : ����
	if( GetExtroversion() < CXSLPetManager::PE_MIN_EXTROVERSION ||
		GetExtroversion() > CXSLPetManager::PE_MAX_EXTROVERSION )
	{
		START_LOG( cerr, L"���⼺ ���� �̻���." )
			<< BUILD_LOG( GetExtroversion() )
			<< END_LOG;
		return GetExtroversion();
	}

	if( (GetExtroversion() + sIncreaseExtroversion) > CXSLPetManager::PE_MAX_EXTROVERSION )
	{
		//START_LOG( cerr, L"���⼺ ���� �̻���." )
		//	<< BUILD_LOG( GetExtroversion() )
		//	<< END_LOG;

		short sTemp = (GetExtroversion() + sIncreaseExtroversion) - static_cast<short>(CXSLPetManager::PE_MAX_EXTROVERSION);
		m_sExtroversion += (sIncreaseExtroversion - sTemp);

		return GetExtroversion();
	}
	
	m_sExtroversion += sIncreaseExtroversion;

	// �ʹ� ���� ������ �ִ�ġ�� ���� �ʵ��� �Ѵ�!
	//sIncreased = std::min< int >( ( CXSLPetManager::PE_MAX_EXTROVERSION - GetExtroversion() ), sIncreaseExtroversion );

	// ������!
	//m_sExtroversion += sIncreased;
	//}}edit : ����

	// ����� ���� ����������!
	return GetExtroversion();
}

short KUserPet::DecreaseExtroversion( IN short sDecreaseExtroversion )
{
	//short sDecreased = 0;

	if( sDecreaseExtroversion <=  0 )
	{
		START_LOG( cerr, L"���ҽ�Ű���� ���⼺ ���� �̻���." )
			<< BUILD_LOG( sDecreaseExtroversion )
			<< END_LOG;

		return GetExtroversion();
	}

	//{{edit : ����
	if( GetExtroversion() < CXSLPetManager::PE_MIN_EXTROVERSION ||
		GetExtroversion() > CXSLPetManager::PE_MAX_EXTROVERSION )
	{
		START_LOG( cerr, L"���⼺ ���� �̻���." )
			<< BUILD_LOG( GetExtroversion() )
			<< END_LOG;
	}

	if( (GetExtroversion() - sDecreaseExtroversion) < CXSLPetManager::PE_MIN_EXTROVERSION )
	{
		//START_LOG( cerr, L"���⼺ ���� �̻���." )
		//	<< BUILD_LOG( GetExtroversion() )
		//	<< END_LOG;

		short sTemp = (GetExtroversion() - sDecreaseExtroversion) - static_cast<short>(CXSLPetManager::PE_MIN_EXTROVERSION);
		m_sExtroversion -= (sDecreaseExtroversion + sTemp); // sDes ������ ����

		return GetExtroversion();
	}

	m_sExtroversion -= sDecreaseExtroversion;

	// �ʹ� ���� ��Ƽ� ���̳ʽ����� ���� �ʵ��� �Ѵ�!
	//sDecreased = std::min< short >( GetExtroversion(), sDecreaseExtroversion );

	// ����!
	//m_sExtroversion -= sDecreased;

	//}}edit : ����

	// ����� ���� ����������!
	return GetExtroversion();
}

short KUserPet::IncreaseEmotion( IN short sIncreaseEmotion )
{
	//short sIncreased = 0;

	if( sIncreaseEmotion <=  0 )
	{
		START_LOG( cerr, L"������Ű���� ������ ���� �̻���." )
			<< BUILD_LOG( sIncreaseEmotion )
			<< END_LOG;

		return GetEmotion();
	}
	//{{edit : ����
	if( GetEmotion() < CXSLPetManager::PE_MIN_EMOTION ||
		GetEmotion() > CXSLPetManager::PE_MAX_EMOTION )
	{
		START_LOG( cerr, L"������ ���� �̻���." )
			<< BUILD_LOG( GetEmotion() )
			<< END_LOG;
	}

	if( (GetEmotion() + sIncreaseEmotion) > CXSLPetManager::PE_MAX_EMOTION )
	{
		//START_LOG( cerr, L"������ ���� �̻���." )
		//	<< BUILD_LOG( GetEmotion() )
		//	<< END_LOG;

		short sTemp = (GetEmotion() + sIncreaseEmotion) - static_cast<short>(CXSLPetManager::PE_MAX_EMOTION);
		m_sEmotion += (sIncreaseEmotion - sTemp);

		return GetEmotion();
	}

	m_sEmotion += sIncreaseEmotion;

	// �ʹ� ���� ������ �ִ�ġ�� ���� �ʵ��� �Ѵ�!
	//sIncreased = std::min< int >( ( CXSLPetManager::PE_MAX_EMOTION - GetEmotion() ), sIncreaseEmotion );

	// ������!
	//m_sEmotion += sIncreased;

	//}}edit : ����

	// ����� ���� ����������!
	return GetEmotion();
}

short KUserPet::DecreaseEmotion( IN short sDecreaseEmotion )
{
	//short sDecreased = 0;

	if( sDecreaseEmotion <=  0 )
	{
		START_LOG( cerr, L"���ҽ�Ű���� ������ ���� �̻���." )
			<< BUILD_LOG( sDecreaseEmotion )
			<< END_LOG;

		return GetEmotion();
	}

	//{{edit : ����
	if( GetEmotion() < CXSLPetManager::PE_MIN_EMOTION ||
		GetEmotion() > CXSLPetManager::PE_MAX_EMOTION )
	{
		START_LOG( cerr, L"������ ���� �̻���." )
			<< BUILD_LOG( GetEmotion() )
			<< END_LOG;
	}

	if( (GetEmotion() - sDecreaseEmotion) < CXSLPetManager::PE_MIN_EMOTION )
	{
		//START_LOG( cerr, L"������ ���� �̻���." )
		//	<< BUILD_LOG( GetEmotion() )
		//	<< END_LOG;
		short sTemp = (GetEmotion() - sDecreaseEmotion) - static_cast<short>(CXSLPetManager::PE_MIN_EMOTION);
		m_sEmotion -= (sDecreaseEmotion + sTemp);

		return GetEmotion();
	}

	m_sEmotion -= sDecreaseEmotion;

	// �ʹ� ���� ��Ƽ� ���̳ʽ����� ���� �ʵ��� �Ѵ�!
	//sDecreased = std::min< short >( GetEmotion(), sDecreaseEmotion );

	// ����!
	//m_sEmotion -= sDecreased;

	//}}edit : ����

	// ����� ���� ����������!
	return GetEmotion();
}

bool KUserPet::IsPetStuffed( IN float fStuffedPercent /*= 80.f*/ ) const
{
	// �������� 80%�� ������ ��θ� �����̴�!
	return ( ( static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f ) >= fStuffedPercent );
}

bool KUserPet::IsHungry() const
{
    // �������� 40%���� ������ ����� �����̴�!
	return ( ( static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f ) < 40.f );
}

bool KUserPet::IsPetSpecialFeed() const
{
	// �ִ� ģ�е� �� �˻�
	if( GetMaxIntimacy() <= 0 )
	{
		START_LOG( cerr, L"�ִ� ģ�е� ���� �̻��մϴ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOG( GetMaxIntimacy() )
			<< END_LOG;
		return false;
	}

	// ģ�е��� 70%�� ������ Ư���� �����۸� �Ծ��!
	return ( ( static_cast<float>(GetIntimacy()) / static_cast<float>(GetMaxIntimacy()) * 100.f ) >= 70.f );
}

//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
bool KUserPet::Feed( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT short& sCurrentSatiety, OUT int& iCurrentIntimacy, OUT int& iAddIntimacy )
#else
bool KUserPet::Feed( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT short& sCurrentSatiety, OUT int& iCurrentIntimacy )
#endif SERV_ADD_KEWPIEL_JELLY
//}}
{
	SET_ERROR( NET_OK );	
#ifdef SERV_EVENT_PET_INVENTORY
///���� �� ���̰� �̺�Ʈ ���̰� �� ID�� �̺�Ʈ �� ID��� ���̸� ���̰� true�� ��ȯ�Ѵ�.
///��� �ѹ� �Ծ��� ���̸� �� �԰� �ϸ� �ȵǴϱ� �Ծ����� �ȸԾ����� üũ �Ѵ�.
///�̺�Ʈ ���� �ƴѵ� �� ���̰� �̺�Ʈ ���̶�� ������ ���� ���� ��Ų��.
	if( iFeedItemID == 141000440 )
	{
		if( m_bIsEventPetID )
		{
			if( m_bEventFoodEat == false ) //���̸� ���� �ʾҴ�.
			{
				m_bEventFoodEat = true; //���̸� ���δ�.
				return true;
			}
			else
			{
				//�̺�Ʈ �������� �̹� ���̸� �Ծ��� ������ �н��Ѵ�
				SET_ERROR( ERR_PET_11 );
				return false;
			}
		}
		else
		{
			SET_ERROR( ERR_PET_11 );
			return false;
		}
	}
#endif SERV_EVENT_PET_INVENTORY
	// �������� 80% �̻��̸� ���̸� �ȸԾ��!
	if( IsPetStuffed() == true )
	{
		SET_ERROR( ERR_PET_10 );
		return false;
	}

	// ģ�е��� 70%�� �ʸ����� Ư���� �����۸� �Ծ��!
	if( IsPetSpecialFeed() == true )
	{
		if( SiCXSLPetManager()->IsSpecialFeedItemID( iFeedItemID ) == false )
		{
			SET_ERROR( ERR_PET_12 );
			return false;
		}
	}

	// �����ϴ� ���� ���� üũ!
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iFeedItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �������� ���̷� �ַ��� �߽��ϴ�!" )
			<< BUILD_LOG( iFeedItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}

	// �ƹ�Ÿ���� �˻�
	if( pItemTemplet->m_bFashion == true )
	{
		SET_ERROR( ERR_PET_16 );
		return false;
	}	

	short sIncreaseSatiety = 0;

	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	int iAddIncreaseIntimacy = 0;	// �ʱ�ȭ
	iAddIntimacy = 0;				// �ʱ�ȭ
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

	// ���̷� �ټ� �ִ� ���������� �˻�
	switch( pItemTemplet->m_ItemType )
	{
	case CXSLItem::IT_WEAPON:
	case CXSLItem::IT_DEFENCE:
		{
			// ������ ���� ������ ĳ���� �������� 11�� ������ ���̷� �ټ� ����.
			if( static_cast<int>(cLevel) > pItemTemplet->m_UseLevel )
			{
				if( ( static_cast<int>(cLevel) - pItemTemplet->m_UseLevel ) > CXSLPetManager::PE_PET_FEED_ITEM_LEVEL )
				{
					SET_ERROR( ERR_PET_17 );
					return false;
				}
			}

			// �Ļ��� ��ŭ ������ �������� ����!
			if( SiCXSLPetManager()->CalcIncreaseSatietyForFeed( GetPetID(), iFeedItemID, cLevel, sIncreaseSatiety ) == false )
			{
				START_LOG( cerr, L"��� ���� ������ ������ ��� ����!" )
					<< BUILD_LOG( GetPetUID() )
					<< BUILD_LOGc( GetPetID() )
					<< BUILD_LOG( pItemTemplet->m_Price )
					<< BUILD_LOGc( cLevel )
					<< END_LOG;

				SET_ERROR( ERR_PET_11 );
				return false;
			}
			//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
			if( SiCXSLItemManager()->IsFantasticKewpielJelly( pItemTemplet->m_ItemID ) == true )
			{
				float fResult = 0.0f;
				float fIncreasRate = SiCXSLPetManager()->GetSpecialFeedIncreaseIntimacyRate();
				iAddIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * fIncreasRate );
				IncreaseIntimacy( iAddIncreaseIntimacy );
				fResult = fIncreasRate * 100.f;
				iAddIntimacy = static_cast<int>( fResult );

			}
			else if( SiCXSLItemManager()->IsOddKewpielJelly( pItemTemplet->m_ItemID ) == true )
			{
				float fResult = 0.0f;
				float fDecreasRate = SiCXSLPetManager()->GetSpecialFeedDecreaseIntimacyRate();
				iAddIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * fDecreasRate );
				DecreaseIntimacy( iAddIncreaseIntimacy );
				fResult = fDecreasRate * (-100.f);
				iAddIntimacy = static_cast<int>( fResult );
			}
#endif SERV_ADD_KEWPIEL_JELLY
			//}}
		}
		break;

	case CXSLItem::IT_SPECIAL:
		{
			// �� ���� ĳ�������� Ȯ���ϰ� ������ �������� ����!
			if( SiCXSLPetManager()->GetCashPetFeedItemInfo( iFeedItemID, sIncreaseSatiety ) == false )
			{
				START_LOG( clog, L"ĳ�� ���� ������ ������ ��� ����! ���� �������� �ƴѰ�?" )
					<< BUILD_LOG( GetPetUID() )
					<< BUILD_LOG( iFeedItemID )
					<< END_LOG;

				SET_ERROR( ERR_PET_18 );
				return false;
			}
		}
		break;

	default:
		START_LOG( clog, L"���� ���̷� �� �� ���� ������ Ÿ���Դϴ�." )
			<< BUILD_LOG( iFeedItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_PET_16 );
		return false;
	}	

	// ������ �ø���!
	IncreaseSatiety( sIncreaseSatiety );

	//////////////////////////////////////////////////////////////////////////	
	// ù �������� üũ�Ͽ� ù���̶�� ģ�е��� 2% �ø����� ����!
	// ���� ���� AM 06:00 ���� ù �Ļ綧 ������ �̿ܿ� ģ�е��� 2% �����մϴ�.
	//////////////////////////////////////////////////////////////////////////	

    // ���� �ð� ���
	CTime tCurTime = CTime::GetCurrentTime();

	// ���� �ð��� ù ���� üũ �ð����� ũ�� ù���̴�!
	if( m_tFirstFeedDate <= tCurTime )
	{
		// 2%��ŭ ģ�е� ���� ��Ű��!
        const int iIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * 0.0201 );

        IncreaseIntimacy( iIncreaseIntimacy );

		bIsFirstFeed = true;

		// ���� �ֱⰡ ���� �����Ƿ� ���� ù ���� �ð��� ��������!
		CTime tNextFirstFeedDate = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 6, 0, 0, 0 );
		if( tNextFirstFeedDate < tCurTime )
		{
			tNextFirstFeedDate += CTimeSpan( 1, 0, 0, 0	);
		}

		m_tFirstFeedDate = tNextFirstFeedDate;
	}
	else
	{
		bIsFirstFeed = false;
	}

	// ���� �������� ģ�е� ���
	sCurrentSatiety = GetSatiety();
	iCurrentIntimacy = GetIntimacy();
    return true;
}

bool KUserPet::CanEvolution()
{
	// ���⼭ �� ��ȭ�� �� �ִ��� üũ!
	char cNextEvolutionStep = 0;
	if( SiCXSLPetManager()->CheckNextEvolution( GetPetID(), GetEvolutionStep(), cNextEvolutionStep ) == false )
	{
		return false;
	}

	// ģ�е��� 100%�� �Ǿ����� ��ȭ�� �����ϴ�!
	if( GetIntimacy() != GetMaxIntimacy() )
	{
		return false;
	}

	return true;
}

bool KUserPet::Evolution( OUT KPetInfo& kResultPetInfo, OUT int& iPetCategorySlotSize )
{
	SET_ERROR( NET_OK );

	// ���⼭ �� ��ȭ�� �� �ִ��� üũ!
	char cNextEvolutionStep = 0;
	if( SiCXSLPetManager()->CheckNextEvolution( GetPetID(), GetEvolutionStep(), cNextEvolutionStep ) == false )
	{
		START_LOG( cerr, L"��ȭ �� �� ���� ��ȭ�ܰ��Դϴ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;

		SET_ERROR( ERR_PET_14 );
		return false;
	}

	// ģ�е��� 100%�� �Ǿ����� ��ȭ�� �����ϴ�!
	if( GetIntimacy() != GetMaxIntimacy() )
	{
		START_LOG( cerr, L"ģ�е��� 100%�� �ƴѵ� ��ȭ ��û�� ���Դ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOG( GetIntimacy() )
			<< BUILD_LOG( GetMaxIntimacy() )
			<< END_LOG;

		SET_ERROR( ERR_PET_15 );
		return false;
	}

	// ���� �ܰ�� ��ȭ ��Ű��!
	m_cEvolutionStep = cNextEvolutionStep;

	// �� �ִ� ģ�е� �缳��
	if( SiCXSLPetManager()->GetEvolutionStepMaxIntimacy( GetPetID(), GetEvolutionStep(), m_iMaxIntimacy ) == false )
	{
		START_LOG( cerr, L"���� �ִ� ģ�е��� ��Ⱑ �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}

	// ģ�е��� 0���� �ʱ�ȭ ����!
	if( GetIntimacy() > 0 )
	{
		const int iCurIntimacy = DecreaseIntimacy( GetIntimacy() );
		LIF( iCurIntimacy == 0 );
	}

	// ��ȭ ��� PetInfo�� ����!
	GetPetInfo( kResultPetInfo );

	// �� �κ��丮 ũ�� ����!
	iPetCategorySlotSize = GetPetInventorySlotSize();

	// ��ȭ�� �Ǿ��ٸ� DB������Ʈ ���� ����!
	m_bReservedDBUpdate = true;
    return true;
}

bool KUserPet::Evolution_Cheat( IN char cEvolutionStep, OUT KPetInfo& kResultPetInfo, OUT int& iPetCategorySlotSize )
{
	// ���⼭ �� ��ȭ�� �� �ִ��� üũ!
	char cNextEvolutionStep = 0;

	if( cEvolutionStep != 0 )
	{
		if( SiCXSLPetManager()->CheckNextEvolution( GetPetID(), ( cEvolutionStep - 1 ), cNextEvolutionStep ) == false )
		{
			START_LOG( cout, L"��ȭ �� �� ���� ��ȭ �ܰ� �Դϴ�! �����ڴ� ġƮ ����� ���ּ���!" )
				<< BUILD_LOG( GetPetUID() )
				<< BUILD_LOGc( GetPetID() )
				<< BUILD_LOGc( cEvolutionStep )
				<< END_LOG;
			return false;
		}
	}

	// ���� �ܰ�� ��ȭ ��Ű��!
	m_cEvolutionStep = cNextEvolutionStep;

	// �� �ִ� ģ�е� �缳��
	if( SiCXSLPetManager()->GetEvolutionStepMaxIntimacy( GetPetID(), GetEvolutionStep(), m_iMaxIntimacy ) == false )
	{
		START_LOG( cerr, L"���� �ִ� ģ�е��� ��Ⱑ �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}

	// ģ�е��� 0���� �ʱ�ȭ ����!
	if( GetIntimacy() > 0 )
	{
		const int iCurIntimacy = DecreaseIntimacy( GetIntimacy() );
		LIF( iCurIntimacy == 0 );
	}	

	// ��ȭ ��� PetInfo�� ����!
	GetPetInfo( kResultPetInfo );

	// �� �κ��丮 ũ�� ����!
	iPetCategorySlotSize = GetPetInventorySlotSize();
	return true;    
}

void KUserPet::CheckExtroversion()
{
	// �α׾ƿ��� �ϰų� ���� ��ȯ �����Ҷ� üũ�Լ��� ȣ���մϴ�!

	short sChangeExtroversion = 0;
	if( SiCXSLPetManager()->CheckExtroversion( GetPetID(), GetExtroversion(), m_iCumulativeIntimacy, sChangeExtroversion ) == false )
	{
		START_LOG( cerr, L"�������� �ʴ� PetID���Դϴ�." )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< END_LOG;
		return;
	}

	if( sChangeExtroversion > 0 )
	{
		IncreaseExtroversion( sChangeExtroversion );
	}
	else if( sChangeExtroversion < 0 )
	{
		//IncreaseExtroversion( -sChangeExtroversion );
		DecreaseExtroversion( -sChangeExtroversion );
	}

	// ���� ģ�е��� �ʱ�ȭ ��Ų��!
	m_iCumulativeIntimacy = 0;
}

void KUserPet::CheckEmotion()
{
	// �꿡�� ���̸� �ٶ����� ����Ǵ� �������� ��ճ��� �α׾ƿ� �ϰų� ���� ��ȯ�Ҷ� ����!

	short sChangeEmotion = 0;
	if( SiCXSLPetManager()->CheckEmotion( GetPetID(), GetEmotion(), m_iCumulativeSatiety, m_iFeedCount, sChangeEmotion ) == false )
	{
		START_LOG( cerr, L"�������� �ʴ� PetID���Դϴ�." )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< END_LOG;
		return;
	}

	if( sChangeEmotion > 0 )
	{
		IncreaseEmotion( sChangeEmotion );
	}
	else if( sChangeEmotion < 0 )
	{
		DecreaseEmotion( -sChangeEmotion );
	}

	// ���� �������� ���� ī��Ʈ�� �ʱ�ȭ ��Ų��!
	m_iCumulativeSatiety = 0;
	m_iFeedCount = 0;
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 12.13    ��μ�	���� Ŭ���� �� ������ ���� �̺�Ʈ - ������ �ֹ���(�ߺ� ���� ����)
#if defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT) || defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX)
bool KUserPet::ExistItemInInventory( IN int& iItemID )
{
	std::map< UidType, KInventoryItemInfo >::iterator mit =	m_mapPetInventory.begin();
	if( mit == m_mapPetInventory.end() )
	{
		return false;
	}

	for( ; mit != m_mapPetInventory.end() ; ++mit )
	{
		if( iItemID == mit->second.m_kItemInfo.m_iItemID )
		{
			return true;
		}
	}

	return false;
}
#endif // SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT || SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
//}}

#ifdef SERV_EVENT_VC
bool KUserPet::IncreaseIntimacyByItem( OUT int& iCurrentIntimacy, OUT int& iUpPercent )
{
	// 2%��ŭ ģ�е� ���� ��Ű��!
	// ���߿� �������� �����ۺ��� �ٸ��� �ϰ� ������, Į�� �߰��ϰ� ���⼭ ��������
	int iIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * 0.0201 );

	if( iIncreaseIntimacy == 0 )
	{
		// �ִ� ģ�е��� 200 �����ϼ��� ������ �̰��� 100% ��½��� �ش�
		iIncreaseIntimacy = GetMaxIntimacy();
		iUpPercent = 100;
	}
	else
	{
		iUpPercent = 2;
	}

	if( iIncreaseIntimacy != 0 )
	{

		IncreaseIntimacy( iIncreaseIntimacy );

		// ���� �������� ģ�е� ���
		iCurrentIntimacy = GetIntimacy();
		return true;
	}
	else
		return false;
}
#endif //SERV_EVENT_VC