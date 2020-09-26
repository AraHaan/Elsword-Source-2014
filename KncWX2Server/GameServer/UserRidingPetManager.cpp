#include "UserRidingPetManager.h"
#include "X2Data/XSLRidingPetManager.h"
#include <boost/test/utils/foreach.hpp>

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-20
KUserRidingPetManager::KUserRidingPetManager(void)
: m_bInit( false )
{
}

KUserRidingPetManager::~KUserRidingPetManager(void)
{
}

void KUserRidingPetManager::Clear( void )
{
	SetInit( false );
	m_vecRidingPetList.clear();
	m_spSummoned = KUserRidingPetPtr();
}

int KUserRidingPetManager::Init( IN const std::vector<KRidingPetInfo>& vecRidingPetList, IN const bool bForce /*= false */)
{
	if( ( InitCheck() == true ) &&
		( bForce == false )
		)
	{
		return RIDING_PET_ALREADY_INITED;
	}

	Clear();

	SetInit( true );

	BOOST_TEST_FOREACH( const KRidingPetInfo&, kInfo, vecRidingPetList )
	{
		if( AddRidingPet( kInfo ) != RIDING_PET_SUCCEED )
		{
			START_LOG( cerr, L"Riding Pet System: �� ���� �ʱ�ȭ�� �����Ͽ����ϴ�." )
				<< BUILD_LOG( kInfo.m_iRidingPetUID )
				<< END_LOG;
		}
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::GetUserRidingPetList( IN const byte byteViewPage, OUT std::vector<KRidingPetInfo>& vecRidingPetList )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	if( byteViewPage == 0 )
	{
		BOOST_TEST_FOREACH( const KUserRidingPetPtr, spUserRidingPet, m_vecRidingPetList )
		{
			KRidingPetInfo kInfo;
			spUserRidingPet->GetData( kInfo );
			vecRidingPetList.push_back( kInfo );
		}
	}
	else
	{
		int iOK = Sort();

		if( iOK != RIDING_PET_SUCCEED )
		{
			return iOK;
		}

		byte bytePrevPage = byteViewPage - 1;
		UINT uiIndex = bytePrevPage * CXSLRidingPetManager::RPE_PAGE_PER_COUNT;

		std::vector<KUserRidingPetPtr>::const_iterator it = m_vecRidingPetList.begin();
		while( 0 < uiIndex )
		{
			if( it == m_vecRidingPetList.end() )
			{
				break;
			}

			--uiIndex;
			++it;
		}

		uiIndex = CXSLRidingPetManager::RPE_PAGE_PER_COUNT;
		for( ; it != m_vecRidingPetList.end(); ++it )
		{
			if( uiIndex == 0 )
			{
				break;
			}

			KRidingPetInfo kInfo;
			( *it )->GetData( kInfo );
			vecRidingPetList.push_back( kInfo );

			--uiIndex;
		}
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::SummonRidingPet( IN const UidType iRidingPetUID, OUT KRidingPetInfo& kRidingPetInfo )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	KUserRidingPetPtr spRidingPet = KUserRidingPetPtr();
	if( iRidingPetUID == 0 )
	{
		// 0�̸� ���� �� �ֻ��� ��ü�� ��ȯ�Ѵ�.
		int iOK = Sort();

		if( iOK != RIDING_PET_SUCCEED )
		{
			return iOK;
		}

		iOK = GetFirstRidingPet( spRidingPet );
		if( iOK != RIDING_PET_SUCCEED )
		{
			return iOK;
		}
	}
	else
	{
		// 0�� �ƴϸ� ������ ���� �����ϴ��� �˻��Ѵ�.
		int iOK = GetRidingPet( iRidingPetUID, spRidingPet );
		if( iOK != RIDING_PET_SUCCEED )
		{
			return iOK;
		}
	}

	if( spRidingPet == NULL )
	{
		return RIDING_PET_NOT_EXIST;
	}

	// ��ȯ ��û
	if( m_spSummoned != NULL )
	{
		if( m_spSummoned->GetUID() == spRidingPet->GetUID() )
		{
			return RIDING_PET_ALREADY_SUMMONED;
		}

		return RIDING_PET_SOMETHING_SUMMONED;
	}

	m_spSummoned = spRidingPet;
	m_spSummoned->GetData( kRidingPetInfo );

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::UnSummonRidingPet( IN const UidType iRidingPetUID, IN const float fStamina )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	// ��ȯ ���� ��û
	if( m_spSummoned == NULL )
	{
		return RIDING_PET_ALREADY_UNSUMMONED;
	}

	// 0�̸� ���� �Ƶ� ������ ��ȯ ����
	if( iRidingPetUID != 0 )
	{
		if( m_spSummoned->GetUID() != iRidingPetUID )
		{
			return RIDING_PET_NOT_SUMMONED;
		}
	}

	KRidingPetInfo kInfo;
	CTime tCurrentDate = CTime::GetCurrentTime();

	m_spSummoned->GetData( kInfo );
	kInfo.m_fStamina = fStamina;
	kInfo.m_iLastUnSummonDate = tCurrentDate.GetTime();
	m_spSummoned->SetData( kInfo );

	m_spSummoned = KUserRidingPetPtr();

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::RidingPetCreateCheck( void ) const
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	if( CXSLRidingPetManager::RPE_MAX_PET_COUNT <= GetRidingPetNum() )
	{
		return RIDING_PET_CREATE_COUNT_LIMITED;
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::AddRidingPet( IN const KRidingPetInfo& kInfo, IN const bool bCreated /*= false */)
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	KUserRidingPetPtr spInfo( new KUserRidingPet( kInfo, bCreated ) );
	m_vecRidingPetList.push_back( spInfo );

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::GetRidingPet( IN const UidType iRidingPetUID, OUT KUserRidingPetPtr& spRidingPet )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	BOOST_TEST_FOREACH( const KUserRidingPetPtr, spRidingPetInfo, m_vecRidingPetList )
	{
		if( spRidingPetInfo->GetUID() == iRidingPetUID )
		{
			spRidingPet = spRidingPetInfo;
			return RIDING_PET_SUCCEED;
		}
	}

	return RIDING_PET_NOT_EXIST;
}

int KUserRidingPetManager::GetFirstRidingPet( OUT KUserRidingPetPtr& spRidingPet )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	if( m_vecRidingPetList.empty() == true )
	{
		return RIDING_PET_EMPTY;
	}

	spRidingPet = m_vecRidingPetList.front();

	return RIDING_PET_SUCCEED;
}

struct RidingPetListSortFunctor
{
	UidType m_iSummonedRidingPetUID;

	RidingPetListSortFunctor( IN const UidType iSummonedRidingPetUID )
		: m_iSummonedRidingPetUID( iSummonedRidingPetUID )
	{
	}

	UidType GetSummonedRidingPetUID() const{	return m_iSummonedRidingPetUID;	}

	bool operator()( const KUserRidingPetPtr spFirst, const KUserRidingPetPtr spSecond ) const
	{
		// NULL������ �˻�
		if( spFirst == NULL  ||  spSecond == NULL )
			return false;

		// ��ȯ���� ���̵� ��
		if( ( GetSummonedRidingPetUID() == spFirst->GetUID() ) &&  ( GetSummonedRidingPetUID() != spSecond->GetUID() ) )
		{
			return true;
		}

		// ��� ������ Ż��
		if( ( spFirst->EnableCheck() == true ) && ( spSecond->EnableCheck() == false ) )
		{
			return true;
		}

		// ���� �ֱٿ� ��ȯ�� ��
		return ( spFirst->GetLastUnSummonDate() > spSecond->GetLastUnSummonDate() );
	}
};

int KUserRidingPetManager::Sort( void )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	if( m_spSummoned == NULL )
	{
		std::stable_sort( m_vecRidingPetList.begin(), m_vecRidingPetList.end(), RidingPetListSortFunctor( 0 ) );
	}
	else
	{
		std::stable_sort( m_vecRidingPetList.begin(), m_vecRidingPetList.end(), RidingPetListSortFunctor( m_spSummoned->GetUID() ) );
	}

	return RIDING_PET_SUCCEED;
}

void KUserRidingPetManager::GetDBUpdateInfo( OUT std::vector<KRidingPetInfo>& vecRidingPetList )
{
	BOOST_TEST_FOREACH( const KUserRidingPetPtr, spUserRidingPet, m_vecRidingPetList )
	{
		if( spUserRidingPet->GetChanged() == false )
		{
			continue;
		}

		KRidingPetInfo kInfo;
		spUserRidingPet->GetData( kInfo );
		vecRidingPetList.push_back( kInfo );
		spUserRidingPet->SetChanged( false );
	}
}

void KUserRidingPetManager::DBUpdateFailedProcess( IN const std::vector<UidType> vecRidingPetUID )
{
	BOOST_TEST_FOREACH( const UidType, iRidingPetUID, vecRidingPetUID )
	{
		KUserRidingPetPtr spInfo = KUserRidingPetPtr();
		if( GetRidingPet( iRidingPetUID, spInfo ) == RIDING_PET_SUCCEED )
		{
			spInfo->SetChanged( true );
		}
	}
}

void KUserRidingPetManager::OnDeselectUnit( void )
{
}

int KUserRidingPetManager::ReleaseRidingPetCheck( IN const UidType iRidingPetUID )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	// ��ȯ�Ǿ� �ִ� ���̵� ���ΰ�?
	if( ( m_spSummoned != NULL ) &&
		( m_spSummoned->GetUID() == iRidingPetUID )
		)
	{
		return RIDING_PET_ALREADY_SUMMONED;
	}

	KUserRidingPetPtr spInfo = KUserRidingPetPtr();
	
	const int iOK = GetRidingPet( iRidingPetUID, spInfo );

	if( iOK != RIDING_PET_SUCCEED )
	{
		return iOK;
	}

	// ���� ������ �ִ� ���̵� ���ΰ�?
	if( spInfo == NULL )
	{
		return RIDING_PET_NOT_EXIST;
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::ReleaseRidingPet( IN const UidType iRidingPetUID )
{
	const int iOK = ReleaseRidingPetCheck( iRidingPetUID );

	if( iOK != RIDING_PET_SUCCEED )
	{
		return iOK;
	}

	std::vector<KUserRidingPetPtr>::iterator it;
	for( it = m_vecRidingPetList.begin(); it != m_vecRidingPetList.end(); ++it )
	{
		if( (*it)->GetUID() == iRidingPetUID )
		{
			m_vecRidingPetList.erase( it );
			break;
		}
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::GetSummonedRidingPetInfo( OUT UidType& iRidingPetUID, OUT USHORT& usRidingPetID )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	// ��ȯ�Ǿ� �ִ� ���̵� ���ΰ�?
	if( m_spSummoned != NULL )
	{
		iRidingPetUID = m_spSummoned->GetUID();
		usRidingPetID = m_spSummoned->GetID();
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::GetExpirationPetUID( OUT std::vector<UidType>& vecRidingPetUID )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	vecRidingPetUID.clear();
	BOOST_TEST_FOREACH( const KUserRidingPetPtr, spInfo, m_vecRidingPetList )
	{
		if( spInfo->IsExpirationRidingPet() == true )
		{
			vecRidingPetUID.push_back( spInfo->GetUID() );
		}
	}

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::IsExpirationRidingPet( IN const UidType iRidingPetUID, OUT USHORT& usRidingPetID )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	KUserRidingPetPtr spRidingPet = KUserRidingPetPtr();
	
	const int iOK = GetRidingPet( iRidingPetUID, spRidingPet );

	if( iOK != RIDING_PET_SUCCEED )
	{
		return iOK;
	}

	if( spRidingPet == NULL )
	{
		return RIDING_PET_NOT_EXIST;
	}

	if( spRidingPet->IsExpirationRidingPet() == false )
	{
		return RIDING_PET_FAILED;
	}

	usRidingPetID = spRidingPet->GetID();

	return RIDING_PET_SUCCEED;
}

int KUserRidingPetManager::UpdateSummonedRidingPetInfo( IN const UidType iRidingPetUID, IN const float fStamina )
{
	if( InitCheck() == false )
	{
		return RIDING_PET_NOT_INITED;
	}

	if( m_spSummoned == NULL )
	{
		return RIDING_PET_NOT_SUMMONED;
	}

	if( m_spSummoned->GetUID() != iRidingPetUID )
	{
		return RIDING_PET_SOMETHING_SUMMONED;
	}

	if( m_spSummoned->CheckRidingPetStamina( fStamina ) == false )
	{
		return RIDING_PET_INCORRECTED_STAMINA;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	KRidingPetInfo kInfo;
	m_spSummoned->GetData( kInfo );
	kInfo.m_fStamina			= fStamina;
	kInfo.m_iLastUnSummonDate	= tCurrentTime.GetTime();
	m_spSummoned->SetData( kInfo );

	return RIDING_PET_SUCCEED;
}
#endif	// SERV_RIDING_PET_SYSTM
