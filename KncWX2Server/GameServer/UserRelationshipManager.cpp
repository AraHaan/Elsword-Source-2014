#include ".\userrelationshipmanager.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLItem.h"
#include "X2Data/XSLItemManager.h"

#ifdef SERV_NEW_CHARACTER_EL	// ���볯¥: 2013-07-18
#include "NetError.h"
#endif	// SERV_NEW_CHARACTER_EL

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM


KUserRelationshipManager::KUserRelationshipManager(void)
{
	m_cRelationshipType = SEnum::RT_SOLO;
	m_iLoverUnitUID		= 0;	
}

KUserRelationshipManager::~KUserRelationshipManager(void)
{
}

void KUserRelationshipManager::Init( IN KRelationshipInfo& kInfo )
{
	SetRelationshipType( kInfo.m_cRelationshipType );
	SetLoverUnitUID( kInfo.m_iOtherUnitUID );
	SetLoverNickName( kInfo.m_wstrOtherNickName );
	SetLoveWord( kInfo.m_wstrLoveWord );
	SetAnniversaryDay( CTime( kInfo.m_tDate ) );
	SetLastRewardDate( CTime( kInfo.m_tLastReward ) );
	SetRewardTitleType( kInfo.m_cRewardTitleType );
}

void KUserRelationshipManager::GetLoveWord( std::wstring& wstrWord )
{
	wstrWord = m_wstrLoveWord;
}

void KUserRelationshipManager::GetAnniversaryDay( CTime& tDate )
{
	tDate = m_tAnniversaryDay;
}

bool KUserRelationshipManager::SetRelationshipType( char cType )
{
	m_cRelationshipType = cType;
	return true;
}

void KUserRelationshipManager::SetLastRewardDate( CTime& tDate )
{ 
	CTime tDefault = CTime(2000, 1, 1, 0, 0, 0);
	m_tLastRewardDate = tDefault;

	if( tDate > tDefault )
		m_tLastRewardDate = tDate; 
}

#ifdef SERV_NEW_CHARACTER_EL	// ���볯¥: 2013-07-18
int KUserRelationshipManager::CheckChar( IN char cUnitClassA, IN char cUnitClassB )
{
	int iOK = NetError::NET_OK;

	CXSLUnit::UNIT_TYPE	eUnitTypeA = CXSLUnit::GetUnitClassToUnitType( static_cast< CXSLUnit::UNIT_CLASS >(cUnitClassA) );
	CXSLUnit::UNIT_TYPE	eUnitTypeB = CXSLUnit::GetUnitClassToUnitType( static_cast< CXSLUnit::UNIT_CLASS >(cUnitClassB) );

	// ���ҵ� �� �����ý��� ��� Ŀ���� ���ƾ� �Ѵ�.
	// 1. ��û�ڰ� ���ҵ�, �ǽ�û�ڰ� �����ý�
	if( eUnitTypeA == CXSLUnit::UT_ELSWORD && eUnitTypeB == CXSLUnit::UT_ELESIS )
	{
		iOK = NetError::ERR_RELATIONSHIP_47;
	}

	// 2. ��û�ڰ� �����ý�, �ǽ�û�ڰ� ���ҵ�
	if( eUnitTypeA == CXSLUnit::UT_ELESIS && eUnitTypeB == CXSLUnit::UT_ELSWORD )
	{
		iOK = NetError::ERR_RELATIONSHIP_47;
	}

	// ���������� Ŀ���� ���ƾ� �Ѵ�.
	if( CXSLUnit::IsMan_UnitType( static_cast< CXSLUnit::UNIT_TYPE >(eUnitTypeA) ) == CXSLUnit::IsMan_UnitType( eUnitTypeB ) )
	{
		iOK = NetError::ERR_RELATIONSHIP_05;
	}
	
	return iOK;
}
#else // SERV_NEW_CHARACTER_EL
bool KUserRelationshipManager::CheckChar( IN char cUnitType, IN char cUnitClass )
{
	CXSLUnit::UNIT_TYPE	eType = CXSLUnit::GetUnitClassToUnitType( static_cast< CXSLUnit::UNIT_CLASS >(cUnitClass) );

	return (CXSLUnit::IsMan_UnitType( static_cast< CXSLUnit::UNIT_TYPE >(cUnitType) ) 
		!= CXSLUnit::IsMan_UnitType( eType ) );
}
#endif	// SERV_NEW_CHARACTER_EL

bool KUserRelationshipManager::CheckCoupleDate()
{
	CTime tCurr = CTime::GetCurrentTime();
	CTimeSpan tSpan = CTimeSpan( 2, 0, 0, 0);

	if( tCurr < m_tAnniversaryDay + tSpan )
	{
		return false;
	}

	return true;

}

bool KUserRelationshipManager::IsValiedWeddingOfficiantNpc( IN const char cOfficiantNPC )
{
	if( SEnum::WOT_NPC_A <= cOfficiantNPC && cOfficiantNPC < SEnum::WOT_NPC_END )
	{
		return true;
	}

	return false;
}

bool KUserRelationshipManager::IsValiedWeddingHall( IN const char cWeddingHallType )
{
	if( SEnum::WHT_CASH_A <= cWeddingHallType && cWeddingHallType < SEnum::WHT_END )
	{
		return true;
	}

	return false;
}

bool KUserRelationshipManager::IsWeddingItem( IN const int iItemID, OUT char& cWeddingStatus )
{
	switch( iItemID )
	{
	case CXSLItem::SI_WEDDING_INVITATION_ITEM:
		cWeddingStatus = SEnum::WS_GUEST;
		return true;
	case CXSLItem::SI_WEDDING_RESERVATION_ITEM:
		cWeddingStatus = SEnum::WS_GROOM;
		return true;
	}

	cWeddingStatus = SEnum::WS_NONE;
	return false;
}

int	KUserRelationshipManager::GetBreakUpCost()
{
	CTime tCurr = CTime::GetCurrentTime();

	CTimeSpan tSpan = CTimeSpan( tCurr.GetTime() - m_tAnniversaryDay.GetTime() );
	int iDays = (int)(tSpan.GetTotalHours() / 24) + 1;


	switch( m_cRelationshipType )
	{
	case SEnum::RT_COUPLE:
		{
			return (iDays * BUC_COUPLE < BUC_MAX) ? (iDays * BUC_COUPLE) : BUC_MAX;
		}break;
	case SEnum::RT_MARRIED:
		{
			return (iDays * BUC_MERRIED < BUC_MAX) ? (iDays * BUC_MERRIED) : BUC_MAX;
		}break;
	}

	return 0;
}

void KUserRelationshipManager::SetBreakUp()
{
	m_cRelationshipType = SEnum::RT_SOLO;
	m_iLoverUnitUID = 0;	
	m_wstrLoverNickName.clear();
	m_wstrLoveWord.clear();
	m_tAnniversaryDay = CTime(0);

	m_mapWeddingTicketItem.clear();	
}

bool KUserRelationshipManager::IsRewardDay()
{
	if( m_cRelationshipType == SEnum::RT_SOLO )
	{
		return false;
	}

	CTime tCurr = CTime::GetCurrentTime();
	if( tCurr.GetDayOfWeek() != m_tAnniversaryDay.GetDayOfWeek() )
	{
		return false;
	}

	CTime tLastReward = CTime( m_tLastRewardDate.GetYear(), m_tLastRewardDate.GetMonth(), m_tLastRewardDate.GetDay(), 0, 0, 0 );	// ������ ���� ���� ���� 0�� 0�� 0�ʷ�  ����
	CTimeSpan tWeek = CTimeSpan( 7, 0, 0, 0 );
	CTime tNextLastReward = tLastReward + tWeek;

	// ���� ���� ���� ������ �����°�?
	if( tCurr > tNextLastReward )
	{
		return true;
	}
	
 	return false;
}

int	KUserRelationshipManager::IsRewardTitleItem( OUT int& iTitleRewardStep )
{
	iTitleRewardStep = 0;

	if( m_cRelationshipType != SEnum::RT_MARRIED )
	{
		return 0;
	}

	CTime tCurr = CTime::GetCurrentTime();

	// ���� �޾Ҵ� Ÿ�� ���� ��¥ ��� ����
	CTimeSpan tDays(tCurr.GetTime() - m_tAnniversaryDay.GetTime());
	int iTotalDays = (int)(tDays.GetTotalHours() / 24);

	// ����� ������ 1��° �����̹Ƿ� 100��°��� ǥ�� �Ǵ� �ð��� ���� day - 1 �̴�
	switch( m_cRewardTitleType )
	{
	case 0:
		{
			// ���� ���� ����
			// 99�� �� �Ǿ����� Ÿ��Ʋ ����
			if( iTotalDays >= 99 )
			{
				iTitleRewardStep = 1;
				return SiCXSLItemManager()->GetWeddingAnniversaryDayRewardTitle( 99 );
			}

		}
		break;
	case 1:
		{
			if( iTotalDays >= 149 )
			{
				iTitleRewardStep = 2;
				return SiCXSLItemManager()->GetWeddingAnniversaryDayRewardTitle( 149 );
			}

		}
		break;
	case 2:
		{
			if( iTotalDays >= 364 )
			{
				iTitleRewardStep = 3;
				return SiCXSLItemManager()->GetWeddingAnniversaryDayRewardTitle( 364 );
			}

		}
		break;
	}

	return 0;
}
#endif SERV_RELATIONSHIP_SYSTEM
//}