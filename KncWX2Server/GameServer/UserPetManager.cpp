#include "GSUser.h"
#include ".\userpetmanager.h"
#include "BaseServer.h"
#include "X2Data/XSLPetManager.h"
#include "NetError.h"


//{{ 2010. 07. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM


KUserPetManager::KUserPetManager(void)
{
	Clear();
}

KUserPetManager::~KUserPetManager(void)
{
}

void KUserPetManager::Clear()
{
    m_mapPetList.clear();
	m_vecPetList.clear();
	m_mapCreatePetReserve.clear();
	m_iSummonedPetUID = 0;
	m_iLastSummonedPetUID = 0;
	m_iSummonCount = 0;
}

bool KUserPetManager::AddUserPet( IN const KPetInfo& kPetInfo, IN bool bCreatePet /*= false*/ )
{
	std::map< UidType, std::map< UidType, KInventoryItemInfo > > mapDummy;
    return AddUserPet( kPetInfo, mapDummy, bCreatePet );
}

bool KUserPetManager::AddUserPet( IN const KPetInfo& kPetInfo, IN const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, IN bool bCreatePet /*= false*/ )
{
	// �� ��ü ����
	KUserPetPtr spPet( new KUserPet( kPetInfo, mapPetItem, bCreatePet ) );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"�� ������ ���� ����! ���� �� ���� ����!" )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< END_LOG;

		return false;
	}

	// map�����̳ʿ��� �̹� ��ϵ� ������ �˻�
	bool bDelUserPet = false;

	if( m_mapPetList.find( kPetInfo.m_iPetUID ) != m_mapPetList.end() )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ���̳�? ���� �� ���� ����!" )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< END_LOG;

		// ���� ���ϼ� ������ ������!
		bDelUserPet = true;
	}

	// vector�����̳ʿ��� �̹� ��ϵ� ������ �˻�
	BOOST_TEST_FOREACH( KUserPetPtr, spPet, m_vecPetList )
	{
		if( spPet == NULL )
		{
			START_LOG( cerr, L"�� ������ ���� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		if( spPet->GetPetUID() != kPetInfo.m_iPetUID )
			continue;
		
		bDelUserPet = true;
		break;
	}

	// ���� ���ϼ� ������ ������!
	if( bDelUserPet )
	{
		DelUserPet( kPetInfo.m_iPetUID );
	}

#ifdef SERV_PET_SYSTEM_EX1
	// ������ max �� �̺�Ʈ�� �ɼ��̹Ƿ� ���⼭ templet ���ؼ� �������ش�
	spPet->SetAlwaysMaxSatiety(SiCXSLPetManager()->IsAlwaysMaxSatietyPet( kPetInfo.m_iPetID ));
#endif //SERV_PET_SYSTEM_EX1

	// �����̳ʿ� �� ���� ����!
	m_mapPetList.insert( std::make_pair( kPetInfo.m_iPetUID, spPet ) );
	m_vecPetList.push_back( spPet );
	return true;
}

bool KUserPetManager::DelUserPet( IN UidType iPetUID )
{
	// map�����̳ʿ��� ��� �ȵ� ������ �˻�
	if( m_mapPetList.find( iPetUID ) == m_mapPetList.end() )
	{
		START_LOG( cerr, L"��� �ȵ� ���̳�? ���� �� ���� ����!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;
	}

	// vector�����̳ʿ��� �̹� ��ϵ� ������ �˻�
	std::vector< KUserPetPtr >::iterator vit;
	for( vit = m_vecPetList.begin(); vit != m_vecPetList.end(); ++vit )
	{
		if( *vit == NULL )
		{
			START_LOG( cerr, L"�� ������ ���� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		if( (*vit)->GetPetUID() == iPetUID )
		{
			break;
		}
	}

	// �����̳ʿ��� �� ���� ��������!
	m_mapPetList.erase( iPetUID );
	m_vecPetList.erase( vit );
	return true;
}

struct PetListSortFunctor
{
	UidType m_iSummonedPetUID;
	PetListSortFunctor( UidType iSummonedPetUID ) { m_iSummonedPetUID = iSummonedPetUID; }
	UidType GetSummonedPetUID() const { return m_iSummonedPetUID; }

	bool operator()( const KUserPetPtr spFirst, const KUserPetPtr spSecond ) const
	{
		// NULL������ �˻�
		if( spFirst == NULL  ||  spSecond == NULL )
			return false;

		// 1. �ѹ��� ��ȯ���� ���� ��
		if( spFirst->IsNeverSummon() == true  &&  spSecond->IsNeverSummon() == false )
		{
			return true;
		}

		// 2. ��ȯ���� ��
		if( GetSummonedPetUID() == spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		// 3. ���� �ֱٿ� ��ȯ�� ��
		if( spFirst->GetLastSummonDate() > spSecond->GetLastSummonDate()  &&
			GetSummonedPetUID() != spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		// 4. ģ�е��� ���� ��
		if( spFirst->GetIntimacy() > spSecond->GetIntimacy()  &&  
			spFirst->GetLastSummonDate() == spSecond->GetLastSummonDate()  &&
			GetSummonedPetUID() != spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		// 5. �̸� ���� (�����ټ�)
		if( spFirst->GetPetName() < spSecond->GetPetName()  &&
			spFirst->GetIntimacy() == spSecond->GetIntimacy()  &&
			spFirst->GetLastSummonDate() == spSecond->GetLastSummonDate()  &&
			GetSummonedPetUID() != spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		return false;
	}
};

void KUserPetManager::PetListSort()
{
	// [����] ��ü�� �����ϱ� ���ؼ��� std::stable_sort()�Լ��� �����.

	std::stable_sort( m_vecPetList.begin(), m_vecPetList.end(), PetListSortFunctor( GetSummonedPetUID() ) );
}

void KUserPetManager::Init( IN KGSUserPtr spUser, 
						    IN UidType iSummonedPetUID, 
							IN const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, 
							IN OUT std::vector< KPetInfo >& vecPet,
							OUT int& iPetCategorySlotSize,
							OUT std::map< UidType, KInventoryItemInfo >& mapPetInventory,
							OUT std::vector< UidType >& vecCanEvolutionPetList,
							OUT std::vector< UidType >& vecNeverSummonPetList )
{
	iPetCategorySlotSize = 0;
	mapPetInventory.clear();
	vecCanEvolutionPetList.clear();
	vecNeverSummonPetList.clear();

	BOOST_TEST_FOREACH( const KPetInfo&, kPet, vecPet )
	{
		// ���� �����ϴ� PetID���� �˻�!
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		if( SiCXSLPetManager()->IsExistPetID( kPet.m_iPetID ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� PetID�� DB�� �־����ϴ�!" )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOGc( kPet.m_iPetID )
				<< END_LOG;
			continue;
		}
#else //SERV_PETID_DATA_TYPE_CHANGE
		if( SiCXSLPetManager()->IsExistPetID( kPet.m_cPetID ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� PetID�� DB�� �־����ϴ�!" )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOGc( kPet.m_cPetID )
				<< END_LOG;
			continue;
		}
#endif //SERV_PETID_DATA_TYPE_CHANGE
		
		// �� �߰�
		LIF( AddUserPet( kPet, mapPetItem ) );
	}

	BOOST_TEST_FOREACH( KUserPetPtr, spPet, m_vecPetList )
	{
		if( spPet == NULL )
			continue;

		// ��ȭ ������ ������ üũ�ϰ� ����Ʈ�� ������!
		if( spPet->CanEvolution() == true )
		{
			vecCanEvolutionPetList.push_back( spPet->GetPetUID() );
		}

		// ��ȯ�� ���� ������ üũ�ϰ� ����Ʈ�� ������!
		if( spPet->IsNeverSummon() == true )
		{
			vecNeverSummonPetList.push_back( spPet->GetPetUID() );
		}
	}
	
	// Ŭ���̾�Ʈ���� ��ȯ�� �길 �ֱ� ���� vector�� �����!
	vecPet.clear();

	// ��ȯ�� ���� ������ ��ȯ ó�� ����!
	if( iSummonedPetUID > 0 )
	{
		SetSummonedPetUID( iSummonedPetUID );

		KUserPetPtr spSummonedPet = GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"�� ���� �������� �ߴ��� ���� ����? ���� �� ���� ����!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;
			return;
		}

		// �� ��ȯ!
		KPetInfo kSummonedPetInfo;
		spSummonedPet->Summon( kSummonedPetInfo, iPetCategorySlotSize, mapPetInventory );
		vecPet.push_back( kSummonedPetInfo );
	}

	// �� ����Ʈ ����!
	PetListSort();
}

KUserPetPtr KUserPetManager::GetPet( IN UidType iPetUID )
{
	std::map< UidType, KUserPetPtr >::iterator mit;
	mit = m_mapPetList.find( iPetUID );
	if( mit == m_mapPetList.end() )
	{
		START_LOG( cwarn, L"ã�� ���� �����ϴ�.!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return KUserPetPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"�� ������ �̻�.!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return KUserPetPtr();
	}

	return mit->second;
}

bool KUserPetManager::GetPetInfo( IN UidType iPetUID, OUT KPetInfo& kPetInfo )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"���� �ϰ� ���� �ʴ� PetUID�Դϴ�." )
			<< BUILD_LOG( iPetUID )
			<< BUILD_LOG( m_mapPetList.size() )
			<< END_LOG;
		return false;
	}

	spPet->GetPetInfo( kPetInfo );
	return true;
}

void KUserPetManager::SetSummonedPetUID( IN UidType iPetUID )
{
    m_iSummonedPetUID = iPetUID;

	// ��ȯ�� ���!
	if( iPetUID > 0 )
	{
		m_iLastSummonedPetUID = iPetUID;
		++m_iSummonCount;
	}
}

void KUserPetManager::GetSummonedPetInfo( OUT std::vector< KPetInfo >& vecPet )
{
	vecPet.clear();

	if( GetSummonedPetUID() == 0 )
		return;

	KPetInfo kInfo;
	if( GetPetInfo( GetSummonedPetUID(), kInfo ) == false )
	{
		START_LOG( cerr, L"��ȯ�� ������ ������ PetUID�� �����δ� ���� ���̴�? ���� �� ���� ����!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	// ���� ��ȯ�� ���� ����!
	vecPet.push_back( kInfo );
}

void KUserPetManager::GetSummonedPetInfo( OUT std::vector< KFieldPetInfo >& vecPet )
{
	vecPet.clear();

	if( GetSummonedPetUID() == 0 )
		return;

	KPetInfo kInfo;
	if( GetPetInfo( GetSummonedPetUID(), kInfo ) == false )
	{
		START_LOG( cerr, L"��ȯ�� ������ ������ PetUID�� �����δ� ���� ���̴�? ���� �� ���� ����!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	// ���� ��ȯ�� ���� ����!
	KFieldPetInfo kFieldPet;
	kFieldPet.m_iPetUID			= kInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kFieldPet.m_iPetID			= kInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kFieldPet.m_cPetID			= kInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kFieldPet.m_wstrPetName		= kInfo.m_wstrPetName;
	kFieldPet.m_cEvolutionStep	= kInfo.m_cEvolutionStep;
	kFieldPet.m_iIntimacy		= kInfo.m_iIntimacy;
	vecPet.push_back( kFieldPet );
}

bool KUserPetManager::GetPetInventory( IN UidType iPetUID, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ���Դϴ�!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return false;
	}

	spPet->GetPetInventory( mapPetItem );
	return true;
}

void KUserPetManager::GetDBUpdatePetInfo( OUT std::vector< KPetInfo >& vecPetInfo )
{
	vecPetInfo.clear();

	std::map< UidType, KUserPetPtr >::const_iterator mit;
	for( mit = m_mapPetList.begin(); mit != m_mapPetList.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"�� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ��ȯ������ �ѹ��� ������ �������� ����!
		if( mit->second->IsReservedDBUpdate() == false )		
			continue;

		KPetInfo kInfo;
		mit->second->GetDBUpdatePetInfo( kInfo );
		vecPetInfo.push_back( kInfo );
	}
}

void KUserPetManager::RollBackDBUpdatePetInfo( IN const std::vector< KPetInfo >& vecPetInfo )
{
	BOOST_TEST_FOREACH( const KPetInfo&, kPet, vecPetInfo )
	{
        KUserPetPtr spPet = GetPet( kPet.m_iPetUID );
		if( spPet == NULL )
			continue;

		// DB������Ʈ ���з� ���� ���а� �ѹ�!
		spPet->RollBackDBUpdatePetInfo( kPet );
	}
}

bool KUserPetManager::IsExistPet( IN const std::wstring& wstrPetName ) const
{
	BOOST_MAP_CONST_FOREACH( UidType, KUserPetPtr, kPet, m_mapPetList )
	{
		if( kPet.second == NULL )
			continue;

        if( kPet.second->GetPetName() == wstrPetName )
			return true;
	}

	return false;
}

//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
bool KUserPetManager::IsSummonedPetHungry()
{
	if( GetSummonedPetUID() == 0 )
		return false;

    KUserPetPtr spPet = GetSummonedPet();
	if( spPet == NULL )
	{
		START_LOG( cerr, L"��ȯ�� ���� ���µ� ����� �������� üũ�ϳ�?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}
	
	return spPet->IsHungry();
}
#endif SERV_PET_AURA_SKILL
//}}

bool KUserPetManager::UpdatePetInventory( IN UidType iPetUID, IN const std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"�� ������ ������Ʈ �Ϸ��� �ϴµ� ���� ���� ���� �ʽ��ϴ�!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return false;
	}	

	// �� ���� ������Ʈ
	spPet->UpdatePetInventory( mapPetItem );
	return true;
}

void KUserPetManager::OnDeselectUnit( IN KGSUserPtr spUser )
{
	// ��ȯ�� ���� �ֳ�?
	if( GetSummonedPetUID() == 0 )
		return;

    // ��ȯ�� �� ã��!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"��ȯ�� PetUID���� �ִµ� �� ������ ����? �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	// ĳ���� ���� ������ ó���Ǵ� �Լ� ȣ��!
	spSummonedPet->Unsummon();

	// �� ��ȯ ���
	{
		CTime tCurTime = CTime::GetCurrentTime();

		KE_LOCAL_LOG_PET_SUMMON_NOT kNot;
		kNot.m_wstrNickName		= spUser->GetCharName();
		kNot.m_ucLevel			= spUser->GetLevel();
		kNot.m_iPetUID			= spSummonedPet->GetPetUID();
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		kNot.m_iPetID			= spSummonedPet->GetPetID();
#else //SERV_PETID_DATA_TYPE_CHANGE
		kNot.m_cPetID			= spSummonedPet->GetPetID();
#endif //SERV_PETID_DATA_TYPE_CHANGE
		kNot.m_cEvolutionStep	= spSummonedPet->GetEvolutionStep();
		kNot.m_iIntimacy		= spSummonedPet->GetIntimacy();
		kNot.m_sExtroversion	= spSummonedPet->GetExtroversion();
		kNot.m_sEmotion			= spSummonedPet->GetEmotion();
		kNot.m_wstrRegDate		= (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
		KSIManager.QueueingEvent( E_LOCAL_LOG_PET_SUMMON_NOT, kNot );
	}
}

//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
bool KUserPetManager::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, IN const bool& bHaveToy, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#else
bool KUserPetManager::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#endif SERV_PET_TOY
//}}
{
	// ��ȯ�� ���� ���ٸ� �н�!
	if( GetSummonedPetUID() == 0 )
		return false;

	// ����ġ�� ���� ���� ���� 0�̸� �н�!
	if( sSubStageExpNpcNum == 0 )
		return false;

	// ��ȯ�� �� ���� ���!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"�� ������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

	// ������ ����!

	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	spSummonedPet->DecreaseSatietyOnDungeonRoom( cUserCount, sSubStageExpNpcNum, bHaveToy );
#else
	spSummonedPet->DecreaseSatietyOnDungeonRoom( cUserCount, sSubStageExpNpcNum );
#endif SERV_PET_TOY
	//}}

	// ��� ����!
	kNot.m_iPetUID = GetSummonedPetUID();
	kNot.m_sCurrentSatiety = spSummonedPet->GetSatiety();
	kNot.m_iCurrentIntimacy = spSummonedPet->GetIntimacy();
	return true;
}

//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
bool KUserPetManager::DecreaseSatietyOnPvpRoom( IN char cUserCount, IN const bool& bHaveToy, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#else
bool KUserPetManager::DecreaseSatietyOnPvpRoom( IN char cUserCount, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#endif SERV_PET_TOY
//}}
{
	// ��ȯ�� ���� ���ٸ� �н�!
	if( GetSummonedPetUID() == 0 )
		return false;

	// ��ȯ�� �� ���� ���!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"�� ������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

	// ������ ����!

	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	spSummonedPet->DecreaseSatietyOnPvpRoom( cUserCount, bHaveToy );
#else
	spSummonedPet->DecreaseSatietyOnPvpRoom( cUserCount );
#endif SERV_PET_TOY
	//}}

	// ��� ����!
	kNot.m_iPetUID = GetSummonedPetUID();
	kNot.m_sCurrentSatiety = spSummonedPet->GetSatiety();
	kNot.m_iCurrentIntimacy = spSummonedPet->GetIntimacy();
	return true;
}

//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
bool KUserPetManager::CheckAutoFeed( IN KGSUserPtr spUser, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiety, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT KEGS_AUTO_FEED_NOT& kNotAutoFeed, OUT int& iAddIntimacy )
#else
bool KUserPetManager::CheckAutoFeed( IN KGSUserPtr spUser, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiety, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT KEGS_AUTO_FEED_NOT& kNotAutoFeed )
#endif SERV_ADD_KEWPIEL_JELLY
//}}
{
	// 1. ��ȯ�� ���� ���ٸ� �׳� ����!
	if( GetSummonedPetUID() == 0 )
		return false;

	// 2. ��ȯ�� �� ���� ���!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"�� ������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

	// 3. �ڵ� ���� ����� �ִ��� üũ!
	if( spSummonedPet->IsAutoFeed() == false )
	{
		return false;
	}

	// 4. ���� ���� ��θ� �������� üũ! �ڵ����̴� 80�ۼ�Ʈ!
	if( spSummonedPet->IsPetStuffed( 80.f ) == true )
	{
		return false;
	}

	// 5. �� �κ��丮�� ������ �������Ѱ� �ִ��� ã�ƺ���!
	if( spUser->AutoFeedInPetInventory( spSummonedPet->IsPetSpecialFeed(), kNotAutoFeed.m_iFeedItemID, kNotAutoFeed.m_vecPetInventorySlotInfo ) == false )
	{
		return false;
	}
#ifdef SERV_EVENT_PET_INVENTORY
	///�̺�Ʈ ���̴� �ڵ����� ������� ��� �� �� ����.
	if( kNotAutoFeed.m_iFeedItemID == 141000440 )
	{
		return false;
	}
#endif SERV_EVENT_PET_INVENTORY
	// 6. ���̸� ����!
	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	iAddIntimacy = 0;
	if( spSummonedPet->Feed( spUser->GetLevel(), kNotAutoFeed.m_iFeedItemID, bIsFirstFeed, kNotSatiety.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy, iAddIntimacy ) == false )
#else
	if( spSummonedPet->Feed( spUser->GetLevel(), kNotAutoFeed.m_iFeedItemID, bIsFirstFeed, kNotSatiety.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy ) == false )
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	{
		START_LOG( cerr, L"���� �Ա� ����! �̸� üũ �ߴµ� ������ �߳�! ���� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( NetError::GetLastErrMsg() ) 
			<< BUILD_LOGc( spUser->GetLevel() )
			<< BUILD_LOG( kNotAutoFeed.m_iFeedItemID )
			<< END_LOG;
		return false;
	}

	// 7. ��� ������ ����!
	kNotSatiety.m_iUnitUID = spUser->GetCharUID();
	kNotSatiety.m_iPetUID = GetSummonedPetUID();
	return true;
}

//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
void KUserPetManager::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#else
void KUserPetManager::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	if( GetSummonedPetUID() == 0 )
		return;

	KUserPetPtr spPet = GetSummonedPet();
	if( spPet == NULL )
	{
		START_LOG( cerr, L"��ȯ�� ��UI�� �ִµ� ���� �� ������ ����?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	spPet->GetStatByPetSkill( bIsDungeon, kBaseStat, fTotalIncHPRate, kAddStat, uiHP_OnePoint, uiUnitLevel );
#else
	spPet->GetStatByPetSkill( bIsDungeon, kBaseStat, fTotalIncHPRate, kAddStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 
}

void KUserPetManager::GetStatIncreaseRateByPetSkill( IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo )
{
	if( GetSummonedPetUID() == 0 )
		return;

	KUserPetPtr spPet = GetSummonedPet();
	if( spPet == NULL )
	{
		START_LOG( cerr, L"��ȯ�� ��UI�� �ִµ� ���� �� ������ ����?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	spPet->GetStatIncreaseRateByPetSkill( bIsDungeon, kStatIncreaseRateInfo );
}

#endif SERV_PET_AURA_SKILL
//}}

bool KUserPetManager::Handler_EGS_CREATE_PET_REQ( IN const KEGS_CREATE_PET_REQ& kReq, IN int iItemID, OUT KDBE_CREATE_PET_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// 1. �� �̸� �ߺ� üũ
	if( IsExistPet( kReq.m_wstrPetName ) == true )
	{
		SET_ERROR( ERR_PET_01 );
		return false;
	}

	// 2. �� �̸� ���� ���� üũ
	std::string strPetName = KncUtil::toNarrowString( kReq.m_wstrPetName );
	if( strPetName.size() > 12 )
	{
		SET_ERROR( ERR_PET_02 );
		return false;
	}

	// 3. ����� �������� �� ���� �������� �´°�?
#ifdef SERV_PERIOD_PET
	CXSLPetManager::PET_UNIT_ID ePetID;
	SiCXSLPetManager()->GetPetIDByItemID( iItemID, ePetID, kPacketToDB.m_sPeriod );
#else SERV_PERIOD_PET
	const CXSLPetManager::PET_UNIT_ID ePetID = SiCXSLPetManager()->GetPetIDByItemID( iItemID );
#endif SERV_PERIOD_PET

	if( ePetID == CXSLPetManager::PUI_NONE )
	{
		START_LOG( cerr, L"�� ���� �����۵� �ƴϸ鼭 ����Ϸ��� �ߴ�!" )
			<< BUILD_LOG( kReq.m_iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
		return false;
	}

	// 4. �̹� ���� ����� ���� �ִ°�?
	if( m_mapCreatePetReserve.empty() == false )
	{
		SET_ERROR( ERR_PET_03 );
		return false;
	}

	// 5. ���ο� �� ���� ����!
	if( SiCXSLPetManager()->MakeNewPetInfo( ePetID, kReq.m_wstrPetName, kPacketToDB.m_kPetInfo ) == false )
	{
		START_LOG( cerr, L"���ο� �� ���� ���� ����!" )
			<< BUILD_LOG( ePetID )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kReq.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
        return false;
	}

	// DB�� ���� ���� ���� ����Ʈ�� �̸� �־����!
	m_mapCreatePetReserve.insert( std::make_pair( kPacketToDB.m_kPetInfo.m_wstrPetName, kPacketToDB.m_kPetInfo ) );
	return true;
}

bool KUserPetManager::Handler_DBE_CREATE_PET_ACK( IN const KDBE_CREATE_PET_ACK& kAck )
{
	SET_ERROR( NET_OK );

	//////////////////////////////////////////////////////////////////////////	
	// ����� �� ������ �ִ��� Ȯ��
	if( m_mapCreatePetReserve.empty() == true )
	{
		START_LOG( cerr, L"����� �� ������ ���µ� �� ���� ���ó���� �ϳ�? ���� �� ���� ����!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		// �׳� �Ѿ� ����!
	}

	// ����� ���� �´��� Ȯ������!
	if( m_mapCreatePetReserve.find( kAck.m_kPetInfo.m_wstrPetName ) == m_mapCreatePetReserve.end() )
	{
		START_LOG( cerr, L"����� ���� �ִµ� ������û�ߴ� ���� �г����� ����? ���� �� ���� ����!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		// �׳� �Ѿ� ����!
	}

	// ����� ������ ���� ������ �������� ���ฮ��Ʈ�� ���ô�
	m_mapCreatePetReserve.clear();
	
	//////////////////////////////////////////////////////////////////////////	
    // Ȥ�ó� �ؼ� �˻��غ�
	if( IsExistPet( kAck.m_kPetInfo.m_iPetUID ) == true )
	{
		START_LOG( cerr, L"�Ȱ��� PetUID�� �߱� �Ǿ���? �ƴϸ� �ι� ������Ʈ �Ȱǰ�? ���� �� ���� ����!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
		return false;
	}

	// Ȥ�ó� �ؼ� �˻��غ�
	if( IsExistPet( kAck.m_kPetInfo.m_wstrPetName ) == true )
	{
		START_LOG( cerr, L"�Ȱ��� �� �̸��� �ֳ�? �ƴϸ� �ι� ������Ʈ �Ȱǰ�? ���� �� ���� ����!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
		return false;
	}

	// �� ����!
	LIF( AddUserPet( kAck.m_kPetInfo, true ) );

	// �� ����Ʈ ����!
	PetListSort();
	return true;
}

void KUserPetManager::Handler_EGS_GET_PET_LIST_REQ( IN const KEGS_GET_PET_LIST_REQ& kReq, OUT KEGS_GET_PET_LIST_ACK& kAck ) const
{
	kAck.m_vecPetList.clear();
	kAck.m_nViewPage = kReq.m_nViewPage;

	_JIF( kReq.m_nViewPage > 0, return; );

	if( m_vecPetList.empty() )
	{
		kAck.m_nViewPage = 1;
		kAck.m_nTotalPage = 1;
		return;
	}

	kAck.m_nTotalPage = m_vecPetList.size() / PE_PAGE_PER_COUNT;
	if( ( m_vecPetList.size() % PE_PAGE_PER_COUNT ) > 0 )
	{
		++kAck.m_nTotalPage;
	}

	u_int uiBeginIndex = ( kAck.m_nViewPage * PE_PAGE_PER_COUNT ) - PE_PAGE_PER_COUNT;
	if( m_vecPetList.size() <= uiBeginIndex )
	{
		kAck.m_nViewPage = kAck.m_nTotalPage; // ������ ������
		uiBeginIndex = ( kAck.m_nViewPage * PE_PAGE_PER_COUNT ) - PE_PAGE_PER_COUNT;

		if( m_vecPetList.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( kAck.m_nViewPage )
				<< BUILD_LOG( kAck.m_nTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecPetList.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KUserPetPtr >::const_iterator vit = m_vecPetList.begin();
	vit += uiBeginIndex;

	while( vit != m_vecPetList.end() )
	{
		if( (*vit) == NULL )
			continue;

		KPetInfo kInfo;
		(*vit)->GetPetInfo( kInfo );

		kAck.m_vecPetList.push_back( kInfo );

		if( kAck.m_vecPetList.size() >= PE_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

bool KUserPetManager::Handler_EGS_SUMMON_PET_REQ( IN KGSUserPtr spUser, IN const KEGS_SUMMON_PET_REQ& kReq, OUT KDBE_SUMMON_PET_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// SummonPetUID���� 0�̸� ��ȯ ������ �Ϸ��°��̴�!
	if( kReq.m_iSummonPetUID == 0 )
	{
		// �̹� ��ȯ�� ���� �ִ��� Ȯ���Ѵ�.
		if( GetSummonedPetUID() == 0 )
		{
			START_LOG( cerr, L"��ȯ�� ���� ���µ� �� ��ȯ�����ҷ��°ų�!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_06 );
			return false;
		}
	}
	// SummonPetUID���� 0�� �ƴϸ� ��ȯ�� �Ϸ��� �ϴ� ���̴�!
	else
	{
		// �̹� ��ȯ�� ���� �ٽ� ��ȯ�Ϸ��� �ϴ°� �ƴ��� Ȯ���Ѵ�.
		if( GetSummonedPetUID() == kReq.m_iSummonPetUID )
		{
			START_LOG( cerr, L"�̹� ��ȯ�Ǿ� �ִ� ���ε� �� ��ȯ�Ϸ��� �Ѵ�!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_07 );
			return false;
		}
	}

	// ������ ��ȯ�� ���� �־��ٸ� ��ȯ ���� ��Ű��!
	kPacketToDB.m_iBeforeSummonPetUID = GetSummonedPetUID();
	if( kPacketToDB.m_iBeforeSummonPetUID != 0 )
	{
		// ��ȯ ����!
		KUserPetPtr spSummonedPet = GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"��ȯ PetUID�� �����ϴµ� �� ������ ����? ���� �� ���� ����!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_04 );
			return false;
		}

		// �� ��ȯ ���� ó��!
        spSummonedPet->Unsummon();

		// �� ��ȯ ���
		{
			CTime tCurTime = CTime::GetCurrentTime();

			KE_LOCAL_LOG_PET_SUMMON_NOT kNot;
			kNot.m_wstrNickName		= spUser->GetCharName();
			kNot.m_ucLevel			= spUser->GetLevel();
			kNot.m_iPetUID			= spSummonedPet->GetPetUID();
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			kNot.m_iPetID			= spSummonedPet->GetPetID();
#else //SERV_PETID_DATA_TYPE_CHANGE
			kNot.m_cPetID			= spSummonedPet->GetPetID();
#endif //SERV_PETID_DATA_TYPE_CHANGE
			kNot.m_cEvolutionStep	= spSummonedPet->GetEvolutionStep();
			kNot.m_iIntimacy		= spSummonedPet->GetIntimacy();
			kNot.m_sExtroversion	= spSummonedPet->GetExtroversion();
			kNot.m_sEmotion			= spSummonedPet->GetEmotion();
			kNot.m_wstrRegDate		= (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			KSIManager.QueueingEvent( E_LOCAL_LOG_PET_SUMMON_NOT, kNot );
		}
	}

	// ��ȯ�Ϸ� �������� ��ȯ�� PetUID �Ǵ� ��ȯ�������� ������Ʈ
	kPacketToDB.m_iSummonPetUID = kReq.m_iSummonPetUID;
	SetSummonedPetUID( kReq.m_iSummonPetUID );
	return true;
}

bool KUserPetManager::Handler_DBE_SUMMON_PET_ACK( IN const KDBE_SUMMON_PET_ACK& kAck, OUT KPetInfo& kSummonedPetInfo, OUT int& iPetCategorySlotSize, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	SET_ERROR( NET_OK );

	iPetCategorySlotSize = 0;
	mapPetItem.clear();

	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB���� �� ��ȯ ó�� ����!" )
			<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
			<< BUILD_LOG( kAck.m_iSummonPetUID )
			<< END_LOG;

		SET_ERROR( ERR_PET_04 );
		goto end_proc;
	}

    // ��ȯ�Ϸ��� ���� �´��� �˻�!
	if( GetSummonedPetUID() != kAck.m_iSummonPetUID )
	{
		START_LOG( cerr, L"DB������Ʈ �Ϸ� ���� �����ߴ� PutUID�� �ٸ���!? �Ͼ�� �ȵǴ� ���¿���!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOG( kAck.m_iSummonPetUID )
			<< END_LOG;

		// ���� ������!
		SetSummonedPetUID( kAck.m_iSummonPetUID );
	}

	// �� ��ȯ�ϱ�!
	if( kAck.m_iSummonPetUID > 0 )
	{
		KUserPetPtr spSummonedPet = GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"�� ���� �������� �ߴ��� ���� ����? ���� �� ���� ����!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_04 );
			goto end_proc;
		}

		// �� ��ȯ!
		spSummonedPet->Summon( kSummonedPetInfo, iPetCategorySlotSize, mapPetItem );

		// �� ����Ʈ ����!
		PetListSort();
	}
	
    return true;

end_proc:
	// ��ȯ �����ߴٸ� ���� ��ȯ ���¸� �ʱ�ȭ ����!
	SetSummonedPetUID( 0 );
	return false;
}

bool KUserPetManager::Handler_EGS_COMMANDS_FOR_PETS_REQ( IN KGSUserPtr spUser, IN const KEGS_COMMANDS_FOR_PETS_REQ& kReq, OUT CXSLPetManager::PET_ACTION_COMMAND& ePetActionResult )
{
	SET_ERROR( NET_OK );

	// ��ȯ�� ���� �ִ��� üũ!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"��ȯ�� ���� ���µ� ����� ���ȴ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOGc( kReq.m_cActionType )
			<< BUILD_LOG( m_mapPetList.size() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ��ȯ�� ���� ã��!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"��ȯ�� �� ������ �������� �ʽ��ϴ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOGc( kReq.m_cActionType )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// �꿡�� ����� ������!
	if( spSummonedPet->CommandsForPet( static_cast<CXSLPetManager::PET_ACTION_COMMAND>(kReq.m_cActionType), ePetActionResult ) == false )
	{
		START_LOG( cerr, L"�꿡�� ��� �ֱⰡ �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOGc( kReq.m_cActionType )
			<< END_LOG;

		SET_ERROR( ERR_PET_09 );
		return false;
	}

    return true;
}

//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
bool KUserPetManager::Handler_EGS_FEED_PETS_REQ( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiey, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT int& iAddIntimacy )
#else
bool KUserPetManager::Handler_EGS_FEED_PETS_REQ( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiey, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy )
#endif SERV_ADD_KEWPIEL_JELLY
//}}
{
	SET_ERROR( NET_OK );

    // ���� ��ȯ�� ���� �ִ��� üũ!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"��ȯ�� ���� ���µ� ���̸� �ַ��� �ϳ�? ���� �� ���¿���!" )
			<< BUILD_LOG( iFeedItemID )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ��ȯ�� �� �������!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ���� �ֱ�!
	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	if( spSummonedPet->Feed( cLevel, iFeedItemID, bIsFirstFeed, kNotSatiey.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy, iAddIntimacy ) == false )
#else
	if( spSummonedPet->Feed( cLevel, iFeedItemID, bIsFirstFeed, kNotSatiey.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy ) == false )
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	{
		// NetError::GetLastError()
		return false;
	}

	kNotSatiey.m_iPetUID = GetSummonedPetUID();
	kNotIntimacy.m_iPetUID = GetSummonedPetUID();
	return true;
}

bool KUserPetManager::Handler_EGS_SET_AUTO_FEED_PETS_REQ( IN const KEGS_SET_AUTO_FEED_PETS_REQ& kReq )
{
	SET_ERROR( NET_OK );
    
	// ���� ��ȯ�� ���� �ִ��� üũ!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"��ȯ�� ���� ���µ� �ڵ� ���� ��� ������ �Ϸ��� �ϳ�? ���� �� ���¿���!" )
			<< BUILD_LOG( kReq.m_bAutoFeed )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ��ȯ�� �� �������!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// �̹� �Ǿ��ִ� �������� üũ!
	if( spSummonedPet->IsAutoFeed() == kReq.m_bAutoFeed )
	{
		START_LOG( cerr, L"�̹� ���õ� ������ �� �����Ϸ��� �մϴ�." )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOG( kReq.m_bAutoFeed )
			<< END_LOG;

		SET_ERROR( ERR_PET_13 );
		return false;
	}
	
	// ���� ����
    spSummonedPet->UpdateAutoFeed( kReq.m_bAutoFeed );
	return true;
}

bool KUserPetManager::Handler_EGS_PET_EVOLUTION_REQ( IN UidType iPetUID, OUT KEGS_PET_EVOLUTION_ACK& kAck, OUT int& iPetCategorySlotSize, OUT bool& bIsSummonedPet )
{
	SET_ERROR( NET_OK );

	// �� �������!
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ���� ��ȯ�� �� ���� üũ!
	bIsSummonedPet = ( GetSummonedPetUID() == iPetUID );

	// ��ȭ�� �� �� �ִ� ������ �����Ǵ��� üũ�Ѵ�!
	if( spPet->Evolution( kAck.m_kEvolutionResultPetInfo, iPetCategorySlotSize ) == false )
	{
		// NetError::GetLastError()
		return false;
	}

	return true;
}

void KUserPetManager::Handler_ERM_UPDATE_PVP_UNIT_INFO_NOT()
{
	// �� ����Ʈ ����
    PetListSort();
}

void KUserPetManager::Handler_ERM_UPDATE_DUNGEON_UNIT_INFO_NOT()
{
	// �� ����Ʈ ����
	PetListSort();
}

bool KUserPetManager::Handler_EGS_ADMIN_CHANGE_PET_INFO_NOT( IN const KEGS_ADMIN_CHANGE_PET_INFO_NOT& kNot, IN KGSUserPtr spUser, OUT KPetInfo& kPetInfo, OUT int& iPetCategorySlotSize )
{
	// ���� ��ȯ�� ���� �ִ��� üũ!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cout, L"��ȯ�� ���� ���µ� �� ġƮ�� ���? ���ڴ� ġƮ�� ����� ������!" )
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( kNot.m_iCheatType )
			<< BUILD_LOG( kNot.m_iValue );
		return false;
	}

	// ��ȯ�� �� �������!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

    switch( kNot.m_iCheatType )
	{
	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EVOLUTION_STEP:
		{
			// ��ȭ ġƮ
            spSummonedPet->Evolution_Cheat( static_cast<char>(kNot.m_iValue), kPetInfo, iPetCategorySlotSize );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_SATIETY:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseSatiety( static_cast<short>(-kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseSatiety( static_cast<short>(kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_INTIMACY:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseIntimacy( -kNot.m_iValue );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseIntimacy( kNot.m_iValue );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EXTROVERSION:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseExtroversion( static_cast<short>(-kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseExtroversion( static_cast<short>(kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EMOTION:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseEmotion( static_cast<short>(-kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseEmotion( static_cast<short>(kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� �� ġƮŸ���� �Խ��ϴ�! ��ŷ �ƴϾ�?" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( kNot.m_iCheatType )
				<< BUILD_LOG( kNot.m_iValue )
				<< END_LOG;
		}
		return false;
	}

	return true;
}


#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 12.13    ��μ�	���� Ŭ���� �� ������ ���� �̺�Ʈ - ������ �ֹ���(�ߺ� ���� ����)
#if defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT) || defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX)
bool KUserPetManager::ExistItemInPetInventory( IN int iItemID )
{
	std::map< UidType, KUserPetPtr >::iterator mit = m_mapPetList.begin();
	if( mit == m_mapPetList.end() )
	{
		return false;
	}

	KUserPetPtr pUserPet;

	for( ; mit != m_mapPetList.end() ; ++mit )
	{
		pUserPet = mit->second;

		if( pUserPet != NULL )
		{
			if( pUserPet->ExistItemInInventory( iItemID ) == true )
			{
				return true;
			}
		}
	}

	return false;
}
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT || SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
//}}

//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
#ifdef SERV_PET_CHANGE_NAME
int KUserPetManager::GetExistPetNum( void )
{
	if( m_mapPetList.size() != m_vecPetList.size() )
	{
		START_LOG( cerr, L"�����̳ʿ� ����� �� ������ ��ġ���� �ʽ��ϴ�." )
			<< BUILD_LOG( m_mapPetList.size() )
			<< BUILD_LOG( m_vecPetList.size() )
			<< END_LOG;
		
		return -1;
	}
	return m_mapPetList.size();
}
#endif SERV_PET_CHANGE_NAME
//}}

#ifdef SERV_PERIOD_PET
bool KUserPetManager::GetExpirePetUID( OUT std::vector<UidType>& vecPetUID )
{
	vecPetUID.clear();
	BOOST_TEST_FOREACH( const KUserPetPtr, spInfo, m_vecPetList )
	{
		if( spInfo->IsExpirePet() == true )
		{
			vecPetUID.push_back( spInfo->GetPetUID() );
		}
	}

	return true;
}
bool KUserPetManager::IsExpirePet( IN UidType iPetUID, OUT int& iPetID )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"���� �ϰ� ���� �ʴ� PetUID�Դϴ�." )
			<< BUILD_LOG( iPetUID )
			<< BUILD_LOG( m_mapPetList.size() )
			<< END_LOG;
		return false;
	}

	if( spPet->IsExpirePet() == false )
	{
		return false;
	}

	iPetID = spPet->GetPetID();
	return true;
}
bool KUserPetManager::ReleasePet( IN UidType iPetUID )
{
	// ��ȯ�Ǿ� �ִ� ���̵� ���ΰ�?
	if( ( GetSummonedPet() != NULL ) &&
		( GetSummonedPetUID() == iPetUID )
		)
	{
		return false;
	}

	KUserPetPtr spPet = GetPet( iPetUID );

	// ���� ������ �ִ� ���̵� ���ΰ�?
	if( spPet == NULL )
	{
		return false;
	}

	// ����
	return DelUserPet( iPetUID );
}
#endif SERV_PERIOD_PET

#ifdef SERV_EVENT_VC
bool KUserPetManager::Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT int& iUpPercent )
{
	SET_ERROR( NET_OK );

	// ���� ��ȯ�� ���� �ִ��� üũ!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"��ȯ�� ���� ���µ� ģ�� ��¾������� �ַ��� �ϳ�? ���� �� ���¿���!" )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ��ȯ�� �� �������!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// ���� �ֱ�!
	if( false == spSummonedPet->IncreaseIntimacyByItem( kNotIntimacy.m_iCurrentIntimacy, iUpPercent ) )
	{
		START_LOG( cerr, L"�ִ� ģ�е��� �߸��Ǿ� �ֽ��ϴ�." )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_11 );
		return false;
	}

	kNotIntimacy.m_iPetUID = GetSummonedPetUID();
	return true;
}
#endif //SERV_EVENT_VC