#include "Temp.h"
#include <boost/test/utils/foreach.hpp>
#include <map>
#include "NetError.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
ImplementSingleton( KJackPotEventManager );

KJackPotCuponState::KJackPotCuponState()
: m_iUserUID( 0 )
, m_iUnitUID( 0 )
, m_iState( static_cast<int>( KJackPotEvent::STATE0 ) )
{
}
bool KJackPotEvent::CheckTheBeginEndDate( IN const std::wstring& wstrBeginDate, IN const std::wstring& wstrEndDate ) const
{
	if( ( m_wstrBeginDate == wstrBeginDate ) && ( m_wstrEndDate == wstrEndDate ) )
	{
		return true;
	}
	return false;
}

void KJackPotEvent::EndProcess( OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo ) const
{
	if( 0 < m_iValidCuponCount )
	{
		vecJackPotEndProcessInfo.push_back( KJackPotEndProcessInfo( m_wstrBeginDate, m_wstrEndDate, m_iValidCuponCount ) );
	}
}

void KJackPotEvent::AddCupon( IN const std::wstring& wstrCuponNumber, IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iState )
{
	m_vecCuponInfo.push_back( KJackPotCuponState( wstrCuponNumber, iUserUID, iUnitUID, iState ) );

	if( iState != STATE0 )
	{
		return;
	}
		
	++m_iValidCuponCount;

	CTime tBeginDate;
	if( KncUtil::ConvertStringToCTime( m_wstrBeginDate, tBeginDate ) == false )
	{
		START_LOG( cerr, L"���� �ð� ��ȯ ����. ���� �̺�Ʈ �ߵ� ���� ������ �� �����ϴ�.")
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< END_LOG;
		return;
	}

	CTime tEndDate;
	if( KncUtil::ConvertStringToCTime( m_wstrEndDate, tEndDate ) == false )
	{
		START_LOG( cerr, L"���� �ð� ��ȯ ����. ���� �̺�Ʈ �ߵ� ���� ������ �� �����ϴ�.")
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< END_LOG;
		return;
	}

	CTimeSpan	tGab = tEndDate - tBeginDate;
	m_iBeginProcessGab = tGab.GetTotalSeconds() / m_iValidCuponCount;
}

bool KJackPotEvent::IsValidEvent( void ) const
{
	if( m_iValidCuponCount < 1 )
	{
		return false;
	}

	CTime tEndDate;
	if( KncUtil::ConvertStringToCTime( m_wstrEndDate, tEndDate ) == false )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ���� �ð� ��ȯ ����( �ʱ�ȭ �κп��� �˻��� �����̱� ������ �߻��ؼ� �ȵȴ�! )" )
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< END_LOG;
	}
	
	if( tEndDate <= CTime::GetCurrentTime() )
	{
		int iNonPaymentCount = 0;
		BOOST_TEST_FOREACH( const KJackPotCuponState&, kJackPotCuponState, m_vecCuponInfo )
		{
			if( kJackPotCuponState.m_iState == 0 )
			{
				++iNonPaymentCount;
			}
		}
		
		START_LOG( cout, L"���� ó���� ���� �̺�Ʈ." )
			<< BUILD_LOG( m_wstrBeginDate )
			<< BUILD_LOG( m_wstrEndDate )
			<< BUILD_LOG( iNonPaymentCount )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KJackPotEvent::ExtractCuponNumber( IN const UidType iUserUID, IN const UidType iUnitUID, OUT std::wstring& wstrCuponNumber, OUT int& iState )
{
	BOOST_TEST_FOREACH( KJackPotCuponState&, kJackPotCuponState, m_vecCuponInfo )
	{
		if( kJackPotCuponState.m_iState != 0 )
		{
			continue;
		}

		kJackPotCuponState.m_iUserUID	= iUserUID;
		kJackPotCuponState.m_iUnitUID	= iUnitUID;
		kJackPotCuponState.m_iState		= STATE1;

		--m_iValidCuponCount;

		wstrCuponNumber					= kJackPotCuponState.m_wstrCuponNumber;
		iState							= kJackPotCuponState.m_iState;
		return true;
	}
	return false;
}

bool KJackPotEvent::BeginProcess( void )
{
	if( m_bEventStart == false )
	{
		CTime tBeginDate;
		if( KncUtil::ConvertStringToCTime( m_wstrBeginDate, tBeginDate ) == false )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ���� �ð� ��ȯ ����( �ʱ�ȭ �κп��� �˻��� �����̱� ������ �߻��ؼ� �ȵȴ�! )" )
				<< BUILD_LOG( m_wstrBeginDate )
				<< BUILD_LOG( m_wstrEndDate )
				<< END_LOG;
		}

		if( CTime::GetCurrentTime() < tBeginDate )
		{
			return false;
		}

		m_bEventStart = true;
	}

	if( m_tTimer.elapsed() < m_iBeginProcessGab )
	{
		return false;
	}
	return true;
}

void KJackPotEvent::ConfirmProcess( void )
{
	m_tTimer.restart();
}

bool KJackPotEventManager::Init( IN const std::vector<KJackPotEventInitInfo>& vecJackpotEventInitInfo )
{
	// ���� �̺�Ʈ�� ������ �ڵ�� ������� �ʴ´�.
	// ��, ���� �̺�Ʈ �Ⱓ �߿��� �ּ��� �ϳ� �̻��� ���� �̺�Ʈ ������ �о�;� �Ѵ�.
	if( vecJackpotEventInitInfo.empty() == true )
	{
		START_LOG( cerr, L"���� �̺�Ʈ ������ �ϳ��� �����ϴ�." )
			<< END_LOG;
		return false;
	}

	std::set<std::wstring> setCuponNumber;
	BOOST_TEST_FOREACH( const KJackPotEventInitInfo&, kJackPotEventInitInfo, vecJackpotEventInitInfo )
	{
		CTime tDate;
		if( KncUtil::ConvertStringToCTime( kJackPotEventInitInfo.m_wstrBeginDate, tDate ) == false )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ���� �ð��� �̻��մϴ�. ��ȯ ����!" )
				<< BUILD_LOG( kJackPotEventInitInfo.m_wstrBeginDate )
				<< END_LOG;
			return false;
		}

		if( KncUtil::ConvertStringToCTime( kJackPotEventInitInfo.m_wstrEndDate, tDate ) == false )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ���� �ð��� �̻��մϴ�. ��ȯ ����!" )
				<< BUILD_LOG( kJackPotEventInitInfo.m_wstrEndDate )
				<< END_LOG;
			return false;
		}

		if( setCuponNumber.find( kJackPotEventInitInfo.m_wstrCuponNumber ) != setCuponNumber.end() )
		{
			START_LOG( cerr, L"���� �̺�Ʈ ���� ������ �ߺ��Ǿ� �ֽ��ϴ�." )
				<< BUILD_LOG( kJackPotEventInitInfo.m_wstrCuponNumber )
				<< END_LOG;
			return false;
		}
		setCuponNumber.insert( kJackPotEventInitInfo.m_wstrCuponNumber );


		// ��÷�� ���� ���
		if( kJackPotEventInitInfo.m_iUserUID != 0 )
		{
			if( m_setWinnerUserUID.insert( kJackPotEventInitInfo.m_iUserUID ).second == false )
			{
				START_LOG( cerr, L"�ߺ� ��÷�ڰ� �����Ѵ�? �־ �ȵǴ� ����!!" )
					<< BUILD_LOG( kJackPotEventInitInfo.m_iUserUID )
					<< END_LOG;
				// �ϴ��� �������
				// return false
			}
		}

		// �ش��ϴ� �Ⱓ�� �̺�Ʈ�� ã�ƺ���.
		std::vector<KJackPotEvent>::iterator it;
		for( it=m_vecJackPotEvent.begin(); it != m_vecJackPotEvent.end(); ++it )
		{
			if( it->CheckTheBeginEndDate( kJackPotEventInitInfo.m_wstrBeginDate, kJackPotEventInitInfo.m_wstrEndDate ) == true )
			{
				break;
			}
		}
		
		// �ش��ϴ� �Ⱓ�� �̺�Ʈ�� �����Ѵٸ�, ���� ���� �߰�
		if( it != m_vecJackPotEvent.end() )
		{
			it->AddCupon( kJackPotEventInitInfo.m_wstrCuponNumber, kJackPotEventInitInfo.m_iUserUID, kJackPotEventInitInfo.m_iUnitUID, kJackPotEventInitInfo.m_iState );
		}
		else	// ���ٸ� �̺�Ʈ�� �߰�
		{
			KJackPotEvent kJackPotEvent( kJackPotEventInitInfo.m_wstrBeginDate, kJackPotEventInitInfo.m_wstrEndDate );
			kJackPotEvent.AddCupon( kJackPotEventInitInfo.m_wstrCuponNumber, kJackPotEventInitInfo.m_iUserUID, kJackPotEventInitInfo.m_iUnitUID, kJackPotEventInitInfo.m_iState );
			m_vecJackPotEvent.push_back( kJackPotEvent );
		}
	}

	return true;
}

void KJackPotEventManager::Tick( OUT std::vector<KJackPotEventWinInfo>& vecJackPotEventWinInfo, OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo )
{
	// 1�ʿ� �ѹ� �����Ѵ�.
	if( m_tTimer.elapsed() < 1.0 )
	{
		return;
	}
	m_tTimer.restart();

	// Tick�� ���鼭 �� ���� �̺�Ʈ���� ���� ������ �̴´�.
	for( std::vector<KJackPotEvent>::iterator it=m_vecJackPotEvent.begin(); it != m_vecJackPotEvent.end(); )
	{
		// ����� �̺�Ʈ��� ��� �����Ѵ�.
		if( it->IsValidEvent() == false )
		{
			// ������ ���� �� sms �˸��� ���� ����� �̺�Ʈ�� ����� �м�����
			it->EndProcess( vecJackPotEndProcessInfo );
			it = m_vecJackPotEvent.erase( it );
			continue;
		}

		KJackPotEvent& kJackPotEvent = *it;
		++it;

		if( kJackPotEvent.BeginProcess() == false )
		{
			continue;
		}

		// ���� ������ ��÷ �غ���.
		KJackPotEventWinInfo kJackPotEventWinInfo;
		if( LotteryUser( kJackPotEventWinInfo.m_iUserUID, kJackPotEventWinInfo.m_iUnitUID ) == false )
		{
			continue;
		}

		// ���� ������ ���� �غ���.
		if( kJackPotEvent.ExtractCuponNumber( kJackPotEventWinInfo.m_iUserUID, kJackPotEventWinInfo.m_iUnitUID, kJackPotEventWinInfo.m_wstrCuponNumber, kJackPotEventWinInfo.m_iState ) == false )
		{
			ReInsertUser( kJackPotEventWinInfo.m_iUserUID );
			continue;
		}

		// ��÷ ������ �����Ѵ�.
		vecJackPotEventWinInfo.push_back( kJackPotEventWinInfo );
		kJackPotEvent.ConfirmProcess();
	}
}

void KJackPotEventManager::AddUser( IN const UidType iUserUID, IN const UidType iUnitUID )
{
	std::pair<std::map<UidType, UidType>::iterator, bool> pairResult;
	
	pairResult = m_mapConnectUserAndUnitUID.insert( std::map<UidType, UidType>::value_type( iUserUID, iUnitUID ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ������ �� ��ϵɸ��� ���µ�... Ȥ�� ���� ���� ����? �ϴ� ���� ���쵵�� ����!")
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( pairResult.first->second )
			<< END_LOG;
		pairResult.first->second = iUnitUID;
	}

	// ������ �����̸鼭 ��÷ ������ ���ٸ� ����Ѵ�.
	if( m_setWinnerUserUID.find( iUserUID ) != m_setWinnerUserUID.end() )
	{
		return;
	}

	pairResult = m_mapConnectUserInfoExceptWinner.insert( std::map<UidType, UidType>::value_type( iUserUID, iUnitUID ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ������ �� ��ϵɸ��� ���µ�... Ȥ�� ���� ���� ����? �ϴ� ���� ���쵵�� ����!(1)")
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( pairResult.first->second )
			<< END_LOG;
		pairResult.first->second = iUnitUID;
	}
}

void KJackPotEventManager::RemoveUser( IN const UidType iUserUID, IN const UidType iUnitUID )
{
	// ������ ������ ������ ����Ʈ���� ���ܽ�Ų��.
	m_mapConnectUserAndUnitUID.erase( iUserUID );
	m_mapConnectUserInfoExceptWinner.erase( iUserUID );
}

bool KJackPotEventManager::LotteryUser( OUT UidType& iUserUID, OUT UidType& iUnitUID )
{
	if( m_mapConnectUserInfoExceptWinner.empty() == true )
	{
		return false;
	}

	// ��÷�� ������ ��÷�Ѵ�.
	int iCount = rand() % m_mapConnectUserInfoExceptWinner.size();
	std::map<UidType, UidType>::const_iterator it = m_mapConnectUserInfoExceptWinner.begin();
	for( int i=0; i < iCount; ++i )
	{
		++it;
	}

	iUserUID = it->first;
	iUnitUID = it->second;

	// ó���Ǵ� ���� �ٽ� ������ �ʵ��� �ϱ� ���� ��÷�� ��Ͽ� �߰�.
	m_setWinnerUserUID.insert( it->first );
	m_mapConnectUserInfoExceptWinner.erase( it->first );

	return true;
}

void KJackPotEventManager::ReInsertUser( IN const UidType iUserUID )
{
	// ���� ���� ���� �ƴ϶��, �߰��� �ʿ� ����.
	std::map<UidType, UidType>::iterator it = m_mapConnectUserAndUnitUID.find( iUserUID );
	if( it == m_mapConnectUserAndUnitUID.end() )
	{
		return;
	}

	// Ȥ�� �� ���� ��Ȳ ó��
	std::pair<std::map<UidType, UidType>::iterator, bool> pairResult = m_mapConnectUserInfoExceptWinner.insert( std::map<UidType, UidType>::value_type( it->first, it->second ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ������ �� ��ϵɸ��� ���µ�... �ϴ� ���� ���쵵�� ����!")
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( it->second )
			<< BUILD_LOG( pairResult.first->second )
			<< END_LOG;
		pairResult.first->second = it->second;
	}
}
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
ImplementRefreshSingleton( KGSBingoEventInfo );

KGSBingoEventInfo::KGSBingoEventInfo()
{
}

KGSBingoEventInfo::~KGSBingoEventInfo()
{
}

ImplToStringW( KGSBingoEventInfo )
{
	stm_	<< L"----------[ Bingo Event Info ]----------" << std::endl
		<< TOSTRINGW( m_mapPackageItemPriceForGift.size() )
		<< TOSTRINGW( m_mapPackageItemPrice.size() )
		<< TOSTRINGW( m_setPresent.size() )
		<< TOSTRINGW( m_setBanPresent.size() )
		<< TOSTRINGW( m_mapAllPresentItemID.size() )
		<< TOSTRINGW( m_mapAllPresentProductNo.size() )
		<< TOSTRINGW( m_setBanSpot.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KGSBingoEventInfo )
{
	lua_tinker::class_add<KGSBingoEventInfo>( GetLuaState(), "KGSBingoEventInfo" );

	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddPackageItemPriceForGift",	&KGSBingoEventInfo::AddPackageItemPriceForGift_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddPackageItemPrice",			&KGSBingoEventInfo::AddPackageItemPrice_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddPresentInfo",				&KGSBingoEventInfo::AddPresentInfo_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "AddBanSpotInfo",				&KGSBingoEventInfo::AddBanSpotInfo_LUA );
	lua_tinker::class_def<KGSBingoEventInfo>( GetLuaState(), "CheckBillingIP",				&KGSBingoEventInfo::CheckBillingIP_LUA );

	lua_tinker::decl( GetLuaState(), "KGSBingoEventInfo", this );
}

void KGSBingoEventInfo::AddPackageItemPriceForGift_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice )
{
	if( m_mapPackageItemPriceForGift.insert( std::map<unsigned long, unsigned long>::value_type( ulProductNo, ulSalePrice ) ).second == false )
	{
		START_LOG( cerr, L"���� �̺�Ʈ - ������ ProductNo ������ �̹� ��ϵǾ� �ֽ��ϴ�." )
			<< BUILD_LOG( ulProductNo )
			<< BUILD_LOG( ulSalePrice )
			<< END_LOG;
	}
}

void KGSBingoEventInfo::GetPackageItemPriceForGift( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const
{
	std::map<unsigned long, unsigned long>::const_iterator it = m_mapPackageItemPriceForGift.find( ulProductNo );
	if( it != m_mapPackageItemPriceForGift.end() )
	{
		ulSalePrice = it->second;
	}
}

void KGSBingoEventInfo::AddPackageItemPrice_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice )
{
	if( m_mapPackageItemPrice.insert( std::map<unsigned long, unsigned long>::value_type( ulProductNo, ulSalePrice ) ).second == false )
	{
		START_LOG( cerr, L"���� �̺�Ʈ - ������ ProductNo ������ �̹� ��ϵǾ� �ֽ��ϴ�." )
			<< BUILD_LOG( ulProductNo )
			<< BUILD_LOG( ulSalePrice )
			<< END_LOG;
	}
}

void KGSBingoEventInfo::GetPackageItemPrice( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const
{
	std::map<unsigned long, unsigned long>::const_iterator it = m_mapPackageItemPrice.find( ulProductNo );
	if( it != m_mapPackageItemPrice.end() )
	{
		ulSalePrice = it->second;
	}
}

void KGSBingoEventInfo::AddPresentInfo_LUA( IN int iItemID, IN unsigned long ulProductNo, IN bool bIsBanPresent /*= false */)
{
	if( bIsBanPresent == true )
	{
		if( m_setPresent.find( ulProductNo ) != m_setPresent.end() )
		{
			START_LOG( cerr, L"���� �̺�Ʈ - �ش� ProductNo�� �̹� �Ϲ� ��ǰ���� ��ϵǾ� �ֽ��ϴ�." )
				<< BUILD_LOG( ulProductNo )
				<< BUILD_LOG( bIsBanPresent )
				<< END_LOG;
			return;
		}
		m_setBanPresent.insert( ulProductNo );
	}
	else
	{
		if( m_setBanPresent.find( ulProductNo ) != m_setBanPresent.end() )
		{
			START_LOG( cerr, L"���� �̺�Ʈ - �ش� ProductNo�� �̹� ���� ��ǰ���� ��ϵǾ� �ֽ��ϴ�." )
				<< BUILD_LOG( ulProductNo )
				<< BUILD_LOG( bIsBanPresent )
				<< END_LOG;
			return;
		}
		m_setPresent.insert( ulProductNo );
	}

	m_mapAllPresentItemID.insert( std::map<int, unsigned long>::value_type( iItemID, ulProductNo ) );
	m_mapAllPresentProductNo.insert( std::map<unsigned long, int>::value_type( ulProductNo, iItemID ) );
}

bool KGSBingoEventInfo::GetPresentProductNoFromItemID( IN const int iItemID, OUT unsigned long& ulProductNo ) const
{
	std::map<int, unsigned long>::const_iterator it = m_mapAllPresentItemID.find( iItemID );
	if( it == m_mapAllPresentItemID.end() )
	{
		return false;
	}
	
	ulProductNo = it->second;
	return true;
}

int KGSBingoEventInfo::GetBingoEventItemID( IN const unsigned long ulProductNo ) const
{
	std::map<unsigned long, int>::const_iterator it = m_mapAllPresentProductNo.find( ulProductNo );
	if( it == m_mapAllPresentProductNo.end() )
	{
		return 0;
	}

	return it->second;
}

void KGSBingoEventInfo::AddBanSpotInfo_LUA( IN byte bytePos )
{
	m_setBanSpot.insert( bytePos );
}

void KGSBingoEventInfo::GetPresentInfo( OUT std::vector<int>& vecPresent )
{
	vecPresent.clear();

	int iItemID;
	BOOST_TEST_FOREACH( const unsigned long, ulProductNo, m_setPresent )
	{
		iItemID = GetBingoEventItemID( ulProductNo );
		if( iItemID != 0 )
		{
			vecPresent.push_back( iItemID );
		}
	}
}

void KGSBingoEventInfo::GetBanPresentInfo( OUT std::vector<int>& vecBanPresent )
{
	vecBanPresent.clear();

	int iItemID;
	BOOST_TEST_FOREACH( const unsigned long, ulProductNo, m_setBanPresent )
	{
		iItemID = GetBingoEventItemID( ulProductNo );
		if( iItemID != 0 )
		{
			vecBanPresent.push_back( iItemID );
		}
	}
}

bool KGSBingoEventInfo::IsBanSpot( IN const byte byteSpotIndex )
{
	return m_setBanSpot.find( byteSpotIndex ) != m_setBanSpot.end();
}

bool KGSBingoEventInfo::IsBingoPresent( IN unsigned long ulProductNo )
{
	return m_mapAllPresentProductNo.find( ulProductNo ) != m_mapAllPresentProductNo.end();
}

bool KGSBingoEvent::CheckBingoEventReward( IN const unsigned long& ulProductNo )
{
	int iItemID = SiKGSBingoEventInfo()->GetBingoEventItemID( ulProductNo );

	if( iItemID == 0 )
	{
		return true;
	}
	else if( m_setGetThePresent.find( iItemID ) != m_setGetThePresent.end() )
	{
		m_setGetThePresent.erase( iItemID );
		return true;
	}
	return false;
}

KGSBingoEvent::KGSBingoEvent()
: m_bBusy( false )
{
	m_iLotteryChance.SetValue( 0 );
	m_iLotteryRecord.SetValue( 0 );
	m_iMixChance.SetValue( 0 );
	m_iResetChance.SetValue( 0 );
	m_iBalance.SetValue( 0 );

	m_vecBingoBoard.reserve( BSV_BINGOBOARD_SLOT_NUM );
	m_vecPresentInfo.reserve( BSV_PRESENT_NUM );
}

KGSBingoEvent::BingoState KGSBingoEvent::GetBingoEventInfoForDB( IN const unsigned short usHandlingEventID, OUT KDBE_BINGO_EVENT_INFO_WRITE_REQ& kPacket )
{
	BingoState bsResult = BS_NONE_CHANCE;

	// ���ƿ� �� Ÿ�� �� ��Ŷ ���� ����
	kPacket.m_usAckEventID = usHandlingEventID;

	// 1. ��ȸ ���� ����
	if( m_iBalance.IsChanged()			|| 
		m_iLotteryChance.IsChanged()	|| 
		m_iLotteryRecord.IsChanged()	|| 
		m_iMixChance.IsChanged()		|| 
		m_iResetChance.IsChanged()
		)
	{
		kPacket.m_iBalance			= m_iBalance.GetChangeValue();
		kPacket.m_iLotteryChance	= m_iLotteryChance.GetChangeValue();
		kPacket.m_iLotteryRecord	= m_iLotteryRecord.GetChangeValue();
		kPacket.m_iMixChance		= m_iMixChance.GetChangeValue();
		kPacket.m_iResetChance		= m_iResetChance.GetChangeValue();
		kPacket.m_bChanceChanged	= true;

		m_iBalance.SetValue( m_iBalance );				// �ܾ� ����
		m_iLotteryChance.SetValue( m_iLotteryChance );	// �̱� ���� Ƚ��
		m_iLotteryRecord.SetValue( m_iLotteryRecord );	// ���� Ƚ��
		m_iMixChance.SetValue( m_iMixChance );			// �ڼ��� ���� Ƚ��
		m_iResetChance.SetValue( m_iResetChance );		// �ʱ�ȭ ���� Ƚ��

		bsResult = BS_OK;
	}

	byte i;

	// 2. ������ ���� ����
	for( i=0; i < m_vecBingoBoard.size(); ++i )
	{
		if( m_vecBingoBoard[i].IsChanged() == true )
		{
			kPacket.m_mapBingoBoard.insert( std::map<byte, byte>::value_type( i, m_vecBingoBoard[i] ) );
			m_vecBingoBoard[i].SetValue( m_vecBingoBoard[i] );
			
			bsResult = BS_OK;
		}
	}

	// 3. ���� ���� ����
	for( i=0; i < m_vecPresentInfo.size(); ++i )
	{
		if( ( m_vecPresentInfo[i].first.IsChanged() == true ) || ( m_vecPresentInfo[i].second.IsChanged() == true ) )
		{
			kPacket.m_mapPresentInfo.insert( std::map<byte, std::pair<int, bool> >::value_type( i, std::make_pair( m_vecPresentInfo[i].first, m_vecPresentInfo[i].second ) ) );
			m_vecPresentInfo[i].first.SetValue( m_vecPresentInfo[i].first );
			m_vecPresentInfo[i].second.SetValue( m_vecPresentInfo[i].second );

			bsResult = BS_OK;
		}
	}

	// 4. �α� ���� ����
	if( m_vecLog.empty() == false )
	{
		kPacket.m_vecLog = m_vecLog;
		m_vecLog.clear();
		bsResult = BS_OK;
	}
	
	return bsResult;
}

bool KGSBingoEvent::Complete( IN const KDBE_BINGO_EVENT_INFO_WRITE_ACK& kPacket_ )
{
	bool iOK = true;
	
	m_iBalance.AdjustInitValue( m_iBalance.GetInitValue() - kPacket_.m_iBalance );
	m_iLotteryChance.AdjustInitValue( m_iLotteryChance.GetInitValue() - kPacket_.m_iLotteryChance );
	m_iLotteryRecord.AdjustInitValue( m_iLotteryRecord.GetInitValue() - kPacket_.m_iLotteryRecord );
	m_iMixChance.AdjustInitValue( m_iMixChance.GetInitValue() - kPacket_.m_iMixChance );
	m_iResetChance.AdjustInitValue( m_iResetChance.GetInitValue() - kPacket_.m_iResetChance );

	for( byte i=0; i < m_vecBingoBoard.size(); ++i )
	{
		std::map<byte, byte>::const_iterator it = kPacket_.m_mapBingoBoard.find( i );
		if( it == kPacket_.m_mapBingoBoard.end() )
		{
			continue;
		}

		// �ٽ� DB�� ���� ���� Init Value�� ���Ƿ� �����ؼ� ������ �����.
		m_vecBingoBoard[i].AdjustInitValue( !m_vecBingoBoard[i].GetInitValue() );
	}

	for( byte i=0; i < m_vecPresentInfo.size(); ++i )
	{
		std::map<byte, std::pair<int, bool> >::const_iterator it = kPacket_.m_mapPresentInfo.find( i );
		if( it == kPacket_.m_mapPresentInfo.end() )
		{
			continue;
		}

		// �ٽ� DB�� ���� ���� Init Value�� ���Ƿ� �����ؼ� ������ �����.
		m_vecPresentInfo[i].first.AdjustInitValue( !m_vecPresentInfo[i].first );
		m_vecPresentInfo[i].second.AdjustInitValue( !m_vecPresentInfo[i].second );
	}

	// �α� ���� ����
	m_vecLog = kPacket_.m_vecLog;

	int iResult = ( iOK ) ? NetError::NET_OK : NetError::ERR_UNKNOWN;
	
	switch( kPacket_.m_usAckEventID )
	{
	case DBE_BINGO_EVENT_LOTTERY_ACK:
		m_TempLotteryAck.m_iOK	= iResult;
		m_bBusy					= false;
		break;

	case DBE_BINGO_EVENT_MIX_ACK:
		m_TempMixAck.m_iOK		= iResult;
		m_bBusy					= false;
		break;

	case DBE_BINGO_EVENT_RESET_ACK:
		m_TempResetAck.m_iOK	= iResult;
		m_bBusy					= false;
		break;

	case DBE_BINGO_EVENT_INIT_ACK:
		m_TempInitAck.m_iOK		= iResult;
		break;
	}

	return iOK;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetBingoEventInfo( OUT KEGS_BINGO_EVENT_INFO_ACK& kPacket ) const
{
	if( ( m_vecBingoBoard.empty() == true ) && ( m_vecPresentInfo.empty() == true ) )
	{
		return BS_NONEDATA;
	}
	else if(( m_vecBingoBoard.size() == BSV_BINGOBOARD_SLOT_NUM	) &&
			( m_vecPresentInfo.size() == BSV_PRESENT_NUM		) )
	{
		kPacket.m_iLotteryChance	= m_iLotteryChance;
		kPacket.m_iLotteryRecord	= m_iLotteryRecord;
		kPacket.m_iMixChance		= m_iMixChance;
		kPacket.m_iResetChance		= m_iResetChance;
		CopyTheBingoBoard( kPacket.m_vecBingoBoard );
		CopyThePresentInfo( kPacket.m_vecPresentInfo );
		return BS_OK;
	}
	else
	{
		// ������ ���⳪ �б⿡ ������ ������ �����̴�.
		// ������ ���ֱ� ������ �ƹ��͵� �� �� ����!
		return BS_UNKNOWN;
	}
}

//{{ 2012. 09. 25	�ڼ���	���� �̺�Ʈ �׽�Ʈ
KGSBingoEvent::BingoState KGSBingoEvent::SetBingoEventInfo( IN const UidType iUnitUID , IN const KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket_ )
{
	m_vecBingoBoard.clear();
	m_vecPresentInfo.clear();

	// ��Ÿ ���� ����
	m_iBalance.SetValue( kPacket_.m_iBalance );
	m_iLotteryChance.SetValue( kPacket_.m_iLotteryChance );
	m_iLotteryRecord.SetValue( kPacket_.m_iLotteryRecord );
	m_iMixChance.SetValue( kPacket_.m_iMixChance );
	m_iResetChance.SetValue( kPacket_.m_iResetChance );

	// DB�� ���� �����Ͱ� �������� �ʴ� ���
	if( kPacket_.m_vecBingoBoard.empty() && kPacket_.m_vecPresentInfo.empty() )
	{
		InitTheBoard( iUnitUID );
	}
	else if(( kPacket_.m_vecBingoBoard.size() == BSV_BINGOBOARD_SLOT_NUM	) &&
			( kPacket_.m_vecPresentInfo.size() == BSV_PRESENT_NUM			) )
	{
		// ���� �����Ͱ� �����ϴ� ���
		
		// ������ ��ȣ ����
		SetTheBingoBoard( kPacket_.m_vecBingoBoard );

		// ���� ���� ����
		SetThePresentInfo( kPacket_.m_vecPresentInfo );

		// ���� ���� �ϼ��Ǿ� �ִٸ�, �ʱ�ȭ �ѹ� ������!
		// check the complete line
		unsigned int iCompleteLineCount = 0;
		for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
		{
			if( it->second == true )
			{
				++iCompleteLineCount;
			}
		}

		if( BSV_PRESENT_NUM <= iCompleteLineCount )
		{
			InitTheBoard( iUnitUID );
		}
	}
	else
	{
		// ������ ���⳪ �б⿡ ������ ������ �����̴�.
		// ������ ���ֱ� ������ �ƹ��͵� �� �� ����!
		return BS_UNKNOWN;
	}
	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const
{
	vecBingoBoard.clear();
	for( std::vector< KCacheData<byte> >::const_iterator it=m_vecBingoBoard.begin(); it != m_vecBingoBoard.end(); ++it )
	{
		vecBingoBoard.push_back( *it );
	}
	return BS_OK;
}
KGSBingoEvent::BingoState KGSBingoEvent::GetPresentInfo( OUT std::vector<int>& vecPresentInfo ) const
{
	vecPresentInfo.clear();
	for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::const_iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
	{
		vecPresentInfo.push_back( it->first );
	}
	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetLotteryNum( IN const UidType iUnitUID )
{
	if( m_bBusy == true )
	{
		return BS_BUSY;
	}
	m_bBusy = true;

	m_TempLotteryAck.clear();

	if( m_iLotteryChance < 1 )
	{
		return BS_NONE_CHANCE;
	}
	else if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	// check the complete line
	unsigned int iCompleteLineCount = 0;
	for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
	{
		if( it->second == true )
		{
			++iCompleteLineCount;
		}
	}

	if( BSV_PRESENT_NUM <= iCompleteLineCount )
	{
		return BS_UNKNOWN;
	}

	m_iLotteryChance -= 1;
	m_iLotteryRecord += 1;

	//
	m_TempLotteryAck.m_iLotteryChance	= m_iLotteryChance;
	m_TempLotteryAck.m_iLotteryRecord	= m_iLotteryRecord;

	// ������ ���� �ϳ� �̾Ƽ� �ش� ��ġ�� ȹ�� ǥ�ø� �Ѵ�.
	m_TempLotteryAck.m_byteNum = rand() % BSV_NUM_MAX;
	int iPos = _InsertLotteryNum( m_TempLotteryAck.m_byteNum, m_TempLotteryAck.m_vecAcquiredPresent );
	
	WriteEventLog( iUnitUID, BSV_ACTION_LOG_LOTTERY, m_TempLotteryAck.m_byteNum, iPos, 0 );
	BOOST_TEST_FOREACH( const byte, iPos, m_TempLotteryAck.m_vecAcquiredPresent )
	{
		WriteEventLog( iUnitUID, BSV_ACTION_LOG_REWARD, m_TempLotteryAck.m_byteNum, iPos, m_vecPresentInfo[iPos].first );
	}
	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::MixTheBoard( IN const UidType iUnitUID )
{
	if( m_bBusy == true )
	{
		return BS_BUSY;
	}
	m_bBusy = true;

	m_TempMixAck.clear();

	if( m_iMixChance < 1 )
	{
		return BS_NONE_CHANCE;
	}
	else if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	// check the complete line
	for( std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
	{
		if( it->second == true )
		{
			return BS_ERR_MIX_RESTRICT;
		}
	}

	int iOpenNumCount = 0;
	for( std::vector< KCacheData<byte> >::iterator it=m_vecBingoBoard.begin(); it != m_vecBingoBoard.end(); ++it )
	{
		if( BSV_NUM_MAX <= *it )
		{
			++iOpenNumCount;
		}
	}
	if( BSV_MIX_RESTRICT_OPEN_COUNT <= iOpenNumCount )
	{
		return BS_ERR_MIX_RESTRICT;
	}

	m_iMixChance -= 1;

	//
	m_TempMixAck.m_iMixChance = m_iMixChance;

	std::vector<byte> vecOpenNum;
	_MixTheBoard( true, vecOpenNum );

	BOOST_TEST_FOREACH( byte, byteNum, vecOpenNum )
	{
		_InsertLotteryNum( byteNum, m_TempMixAck.m_vecAcquiredPresent );

		WriteEventLog( iUnitUID, BSV_ACTION_LOG_MIX, -1, -1, -1 );
		BOOST_TEST_FOREACH( const byte, iPos, m_TempMixAck.m_vecAcquiredPresent )
		{
			WriteEventLog( iUnitUID, BSV_ACTION_LOG_REWARD, -1, iPos, m_vecPresentInfo[iPos].first );
		}
	}

	GetBingoBoard( m_TempMixAck.m_vecBingoBoard );

	return BS_OK;
}

KGSBingoEvent::BingoState KGSBingoEvent::ResetTheBoard( IN const UidType iUnitUID )
{
	if( m_bBusy == true )
	{
		return BS_BUSY;
	}
	m_bBusy = true;

	m_TempResetAck.clear();

	if( m_iResetChance < 1 )
	{
		return BS_NONE_CHANCE;
	}
	else if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	if( _MixThePresent() == BS_OK )
	{
		_MixTheBoard( false );
		// ������ 4������ �⺻������ �����ش�.
		m_vecBingoBoard[0]													+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_LINE_SIZE - 1]						+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - BSV_BINGOBOARD_LINE_SIZE] += BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - 1]						+= BSV_NUM_MAX;
		
		m_iResetChance -= 1;

		//
		m_TempResetAck.m_iResetChance = m_iResetChance;

		GetBingoBoard( m_TempResetAck.m_vecBingoBoard );
		GetPresentInfo( m_TempResetAck.m_vecPresentInfo );
		WriteEventLog( iUnitUID, BSV_ACTION_LOG_RESET, -1, -1, -1 );

		return BS_OK;
	}
	else
	{
		return BS_ERR_MIX_THE_PRESENT;
	}
}

KGSBingoEvent::BingoState KGSBingoEvent::InitTheBoard( IN const UidType iUnitUID )
{
	m_TempInitAck.clear();

	if( _MixThePresent() == BS_OK )
	{
		_MixTheBoard( false );
		// ������ 4������ �⺻������ �����ش�.
		m_vecBingoBoard[0]													+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_LINE_SIZE - 1]						+= BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - BSV_BINGOBOARD_LINE_SIZE] += BSV_NUM_MAX;
		m_vecBingoBoard[BSV_BINGOBOARD_SLOT_NUM - 1]						+= BSV_NUM_MAX;

		GetBingoBoard( m_TempInitAck.m_vecBingoBoard );
		GetPresentInfo( m_TempInitAck.m_vecPresentInfo );
		WriteEventLog( iUnitUID, BSV_ACTION_LOG_INIT, -1, -1, -1 );
		return BS_OK;
	}
	else
	{
		return BS_ERR_MIX_THE_PRESENT;
	}
}

void KGSBingoEvent::AddBalance( IN const int iBalance )
{
	m_iBalance			+= iBalance;
	m_iLotteryChance	+= ( m_iBalance / BSV_LOTTERY_BALANCE );
	m_iBalance.SetAliveValue( m_iBalance % BSV_LOTTERY_BALANCE );
}

void KGSBingoEvent::_MixTheBoard( IN const bool bOnlyMove )
{
	std::vector<byte> vecOpenNum;
	_MixTheBoard( bOnlyMove, vecOpenNum );
}

void KGSBingoEvent::_MixTheBoard( IN const bool bOnlyMove, OUT std::vector<byte>& vecOpenNum )
{
	std::list<byte>::iterator it;

	if( bOnlyMove == true )
	{
		std::list<byte> listCandidate;
		BOOST_TEST_FOREACH( byte, iBingoNum, m_vecBingoBoard )
		{
			if( BSV_NUM_MAX <= iBingoNum )
			{
				iBingoNum -= BSV_NUM_MAX;
				vecOpenNum.push_back( iBingoNum );
			}
			listCandidate.push_back( iBingoNum );
		}
		m_vecBingoBoard.clear();

		byte iNum = 0;
		while( listCandidate.empty() == false )
		{
			it = listCandidate.begin();
			iNum = rand() % listCandidate.size();
			while( 0 < iNum )
			{
				++it;
				--iNum;
			}
			KCacheData<byte> temp;
			temp.SetValue( -1 );
			temp.SetAliveValue( *it );
			m_vecBingoBoard.push_back( temp );
			listCandidate.erase( it );
		}
	}
	else
	{
		std::list<byte> listCandidate;
		for( byte i=0; i < BSV_NUM_MAX; ++i )
		{
			listCandidate.push_back( i );
		}
		m_vecBingoBoard.clear();

		byte iNum = 0;
		for( int i=0; i < BSV_BINGOBOARD_SLOT_NUM; ++i )
		{
			it = listCandidate.begin();
			iNum = rand() % listCandidate.size();
			while( 0 < iNum )
			{
				++it;
				--iNum;
			}
			KCacheData<byte> temp;
			temp.SetValue( -1 );
			temp.SetAliveValue( *it );
			m_vecBingoBoard.push_back( temp );
			listCandidate.erase( it );
		}
	}
}

KGSBingoEvent::BingoState KGSBingoEvent::_MixThePresent( void )
{
	std::vector<int>	vecPresent;
	std::vector<int>	vecPresentBan;
	std::list<byte>		listSpot;

	SiKGSBingoEventInfo()->GetPresentInfo( vecPresent );
	SiKGSBingoEventInfo()->GetBanPresentInfo( vecPresentBan );

	// ���� ������ ����.
	std::vector< std::pair< KCacheData<int>, KCacheData<bool> > > vecPresentInfoBackUp;
	vecPresentInfoBackUp = m_vecPresentInfo;
	
	if( m_vecPresentInfo.size() < BSV_PRESENT_NUM )
	{
		m_vecPresentInfo.resize( BSV_PRESENT_NUM );
	}

	// �ڸ� �ε����� �����Ѵ�.
	for( int i=0; i < BSV_PRESENT_NUM; ++i )
	{
		listSpot.push_back( i );
	}

	int iNum;
	std::list<byte>::iterator it;
	// �ڸ� �ε����� ���� �ִٸ� ������ ����.
	while( listSpot.empty() == false )
	{
		// ���� �ڸ� �ε��� �� ������ �ϳ��� ������.
		iNum = rand() % listSpot.size();
		it = listSpot.begin();
		while( 0 < iNum )
		{
			++it;
			--iNum;
		}

		// �ڸ� �ε��� ����
		iNum = *it;
		listSpot.erase( it );

		// BanSpot���� �Ϲ� �����۸� ���� �� �ִ�.
		if( SiKGSBingoEventInfo()->IsBanSpot( iNum ) == true )
		{
			if( vecPresent.empty() == true )
			{
				// �Ϲ� �������� �� �̻� �������� �ʴ�. ���� ����ġ
				m_vecPresentInfo = vecPresentInfoBackUp;
				return BS_ERR_MIX_THE_PRESENT;
			}
			else
			{
				// �Ϲ� ������ �� �ϳ��� ������ ���� ������ �����Ѵ�.
				m_vecPresentInfo[iNum].first.SetAliveValue( vecPresent.back() );
				m_vecPresentInfo[iNum].second.SetAliveValue( false );
				vecPresent.pop_back();
			}
		}
		else
		{
			// BanSpot�� �ƴ϶�� BanPresent�� �Ϲ� Present�� ���� �� �ִ�.
			// �켱������ BanPresent�� �־� ����.
			if( vecPresentBan.empty() == false )
			{
				// BanPresent �� �ϳ��� ������ ���� ������ �����Ѵ�.
				m_vecPresentInfo[iNum].first.SetAliveValue( vecPresentBan.back() );
				m_vecPresentInfo[iNum].second.SetAliveValue( false );
				vecPresentBan.pop_back();
			}
			else if( vecPresent.empty() == false )
			{
				// �Ϲ� ������ �� �ϳ��� ������ ���� ������ �����Ѵ�.
				m_vecPresentInfo[iNum].first.SetAliveValue( vecPresent.back() );
				m_vecPresentInfo[iNum].second.SetAliveValue( false );
				vecPresent.pop_back();
			}
			else
			{
				// ������ ���Ҵµ� ���̻� ���� ������ ����.
				m_vecPresentInfo = vecPresentInfoBackUp;
				return BS_ERR_MIX_THE_PRESENT;
			}
		}
	}

	return BS_OK;
}

int KGSBingoEvent::_InsertLotteryNum( IN const byte byteNum, IN OUT std::vector<byte>& vecAcquiredPresent )
{
	if( m_vecBingoBoard.empty() || m_vecPresentInfo.empty() )
	{
		return BS_UNKNOWN;
	}

	byte i;
	for( i=0; i < m_vecBingoBoard.size(); ++i )
	{
		if( m_vecBingoBoard[i] == byteNum )
		{
			break;
		}
	}

	// ������ �ȿ� �ִ� ��������, �̹� ���� �ִ��� �˻��Ѵ�.
	if( i == m_vecBingoBoard.size() )
	{
		// �����ǿ� ���ų� �̹� ���� ���ڴ�.
		return -1;
	}

	byte iPos = i;
	m_vecBingoBoard[iPos] += BSV_NUM_MAX;

	// �ϼ��� �� üũ
	// 1. �� �˻�
	byte iValue = ( iPos / BSV_BINGOBOARD_LINE_SIZE ) * BSV_BINGOBOARD_LINE_SIZE;
	for( i=0; i < BSV_BINGOBOARD_LINE_SIZE; ++i )
	{
		if( m_vecBingoBoard[iValue+i] < BSV_NUM_MAX )
		{
			break;
		}
	}

	// ���� �ϼ��Ǿ���.
	if( i == BSV_BINGOBOARD_LINE_SIZE )
	{
		iValue /= BSV_BINGOBOARD_LINE_SIZE;
		// �ش� ���� ������ ���� ǥ�ø� �ϰ�, ��Ŷ�� ���� ��ġ�� �����Ѵ�.
		m_vecPresentInfo[iValue].second.SetAliveValue( true );
		vecAcquiredPresent.push_back( iValue );
	}

	// 2. �� �˻�
	iValue = iPos % BSV_BINGOBOARD_LINE_SIZE;
	for( i=0; i < BSV_BINGOBOARD_SLOT_NUM; i+=BSV_BINGOBOARD_LINE_SIZE )
	{
		if( m_vecBingoBoard[iValue+i] < BSV_NUM_MAX )
		{
			break;
		}
	}

	// ���� �ϼ��Ǿ���.
	if( i == BSV_BINGOBOARD_SLOT_NUM )
	{
		// �ش� ���� ������ ���� ǥ�ø� �ϰ�, ��Ŷ�� ���� ��ġ�� �����Ѵ�.
		iValue += BSV_BINGOBOARD_LINE_SIZE;
		m_vecPresentInfo[iValue].second.SetAliveValue( true );
		vecAcquiredPresent.push_back( iValue );
	}

	// 3. �밢��1 �˻�
	iValue = BSV_BINGOBOARD_LINE_SIZE + 1;

	// �밢1�� ���ԵǴ� �����ΰ�?
	if( ( ( iPos % iValue ) == 0 ) && ( ( iPos / iValue ) < BSV_BINGOBOARD_LINE_SIZE ) )
	{
		for( i=0; i < BSV_BINGOBOARD_LINE_SIZE; ++i )
		{
			if( m_vecBingoBoard[i*iValue] < BSV_NUM_MAX )
			{
				break;
			}
		}

		// �밢���� �ϼ��Ǿ���.
		if( i == BSV_BINGOBOARD_LINE_SIZE )
		{
			iValue = BSV_PRESENT_NUM-1;
			// �ش� ���� ������ ���� ǥ�ø� �ϰ�, ��Ŷ�� ���� ��ġ�� �����Ѵ�.
			m_vecPresentInfo[iValue].second.SetAliveValue( true );
			vecAcquiredPresent.push_back( iValue );
		}
	}

	// 3. �밢��2 �˻�
	iValue = BSV_BINGOBOARD_LINE_SIZE - 1;

	// �밢2�� ���ԵǴ� �����ΰ�?
	if( ( ( iPos % iValue ) == 0 ) && ( ( iPos / iValue ) <= BSV_BINGOBOARD_LINE_SIZE ) )
	{
		for( i=1; i <= BSV_BINGOBOARD_LINE_SIZE; ++i )
		{
			if( m_vecBingoBoard[i*iValue] < BSV_NUM_MAX )
			{
				break;
			}
		}

		// �밢���� �ϼ��Ǿ���.
		if( BSV_BINGOBOARD_LINE_SIZE < i )
		{
			iValue = BSV_PRESENT_NUM-2;
			// �ش� ���� ������ ���� ǥ�ø� �ϰ�, ��Ŷ�� ���� ��ġ�� �����Ѵ�.
			m_vecPresentInfo[iValue].second.SetAliveValue( true );
			vecAcquiredPresent.push_back( iValue );
		}
	}
	return iPos;
}

void KGSBingoEvent::SetTheBingoBoard( IN const std::vector<byte>& vecBingoBoard )
{
	m_vecBingoBoard.clear();
	KCacheData<byte> temp;

	BOOST_TEST_FOREACH( const byte, iValue, vecBingoBoard )
	{
		temp.SetValue( iValue );
		m_vecBingoBoard.push_back( temp );
	}
}

void KGSBingoEvent::SetThePresentInfo( IN const std::vector< std::pair<int, bool> >& vecPresentInfo )
{
	m_vecPresentInfo.clear();
	std::pair< KCacheData<int>, KCacheData<bool> > temp;
	
	for( size_t i=0; i < vecPresentInfo.size(); ++i )
	{
		temp.first.SetValue( vecPresentInfo[i].first );
		temp.second.SetValue( vecPresentInfo[i].second );
		m_vecPresentInfo.push_back( temp );
	}
}

void KGSBingoEvent::CopyTheBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const
{
	vecBingoBoard.clear();
	BOOST_TEST_FOREACH( const KCacheData<byte>&, iValue, m_vecBingoBoard )
	{
		vecBingoBoard.push_back( iValue );
	}
}
void KGSBingoEvent::CopyThePresentInfo( OUT std::vector< std::pair<int, bool> >& vecPresentInfo ) const
{
	vecPresentInfo.clear();
	for( size_t i=0; i < m_vecPresentInfo.size(); ++i )
	{
		vecPresentInfo.push_back( m_vecPresentInfo[i] );
	}
}

void KGSBingoEvent::WriteEventLog(	IN const UidType iUnitUID, 
									IN const byte iActionType, 
									IN const char iOpenNum,	
									IN const char iPos, 
									IN const int iItemID
									)
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	KBingoEventLog kBingoEventLog;
	kBingoEventLog.iUnitUID		= iUnitUID;
	kBingoEventLog.iActionType	= iActionType;
	kBingoEventLog.iOpenNum		= iOpenNum;
	kBingoEventLog.iPos			= iPos;
	kBingoEventLog.iItemID		= iItemID;
	kBingoEventLog.wstrRegDate	= tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	m_vecLog.push_back( kBingoEventLog );
}

void KGSBingoEvent::AddMixChance( IN const unsigned short iQuantity )
{
	m_iMixChance += iQuantity;
}

void KGSBingoEvent::AddResetChance( IN const unsigned short iQuantity )
{
	m_iResetChance += iQuantity;
}

KGSBingoEvent::BingoState KGSBingoEvent::GetProductNo( IN const byte iPos, OUT unsigned long& ulProductNo )
{
	if( SiKGSBingoEventInfo()->GetPresentProductNoFromItemID( m_vecPresentInfo[iPos].first, ulProductNo ) == false )
	{
		return BS_UNKNOWN;
	}
	return BS_OK;
}

int KGSBingoEvent::GetProductItemID( IN const byte iPos )
{
	if( iPos < m_vecPresentInfo.size() )
	{
		return m_vecPresentInfo[iPos].first;
	}
	return 0;
}

void KGSBingoEvent::SetPresentInfoForBuy( IN const std::vector<byte>& vecAcquiredPresent )
{	
	BOOST_TEST_FOREACH( byte, iPos, vecAcquiredPresent )
	{
		m_setGetThePresent.insert( m_vecPresentInfo[iPos].first );
	}
}

//void KGSBingoEvent::GetPresentInfoForBuy( OUT std::vector<byte>& vecAcquiredPresent )
//{
//	int i=0;
//	std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >::iterator it;
//	
//	for( it=m_vecPresentInfo.begin(); it != m_vecPresentInfo.end(); ++it )
//	{
//		if( ( it->second.IsChanged() == true ) && ( it->second == true ) )
//		{
//			vecAcquiredPresent.push_back( i );
//			m_setGetThePresent.insert( m_vecPresentInfo[i].first );
//		}
//		++i;
//	}
//}

bool KGSBingoEvent::CheckAndRemoveIsPresentedItem( IN const unsigned long ulOrderNo )
{
	for( std::vector<unsigned long>::iterator it=m_vecPresentOrderNo.begin(); it != m_vecPresentOrderNo.end(); ++it )
	{
		if( *it == ulOrderNo )
		{
			m_vecPresentOrderNo.erase( it );
			return true;
		}
	}

	return false;
}

bool KGSBingoEvent::IsBingoComplete( void )
{
	BOOST_TEST_FOREACH( const KCacheData<byte>&, iValue, m_vecBingoBoard )
	{
		if( iValue < BSV_NUM_MAX )
		{
			return false;
		}
	}
	return true;
}

bool KGSBingoEvent::BingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo )
{
	std::vector< std::pair< std::vector< KNXBTPurchaseReqInfo >, byte > >::iterator it;
	for( it = m_vecRewardRepeateInfo.begin(); it != m_vecRewardRepeateInfo.end(); ++it )
	{
		bool bFind = true;

		// ACK�� ��ǰ ������ ��ȸ�ϸ鼭,
		BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kInfo, vecPurchaseReqInfo )
		{
			bool bCheck = false;

			// ����.
			BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kNXBTPurchaseReqInfo, it->first )
			{
				if( ( kInfo.m_ulProductNo == kNXBTPurchaseReqInfo.m_ulProductNo ) &&
					( kInfo.m_usOrderQuantity == kNXBTPurchaseReqInfo.m_usOrderQuantity )
					)
				{
					// �̰� �����Ѵ�. ���� �������� �˻�����
					bCheck = true;
					break;
				}
			}

			if( bCheck == false )
			{
				// �� �׷쿣 ����. ���� �׷� �˻�����!
				bFind = false;
				break;
			}
		}

		if( bFind == true )
		{
			// ��ġ�ϴ� ���� ã�Ҵ�.
			return true;
		}
	}
	
	return false;
}

bool KGSBingoEvent::IsBingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo )
{
	if( vecPurchaseReqInfo.empty() == true )
	{
		return false;
	}

	// ���� ���� �̺�Ʈ ������ �´°�?
	BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kInfo, vecPurchaseReqInfo )
	{
		if( SiKGSBingoEventInfo()->IsBingoPresent( kInfo.m_ulProductNo ) == false )
		{
			return false;
		}
	}

	std::vector< std::pair< std::vector< KNXBTPurchaseReqInfo >, byte > >::iterator it;
	for( it = m_vecRewardRepeateInfo.begin(); it != m_vecRewardRepeateInfo.end(); ++it )
	{
		bool bFind = true;

		// ACK�� ��ǰ ������ ��ȸ�ϸ鼭,
		BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kInfo, vecPurchaseReqInfo )
		{
			bool bCheck = false;

			// ����.
			BOOST_TEST_FOREACH( KNXBTPurchaseReqInfo, kNXBTPurchaseReqInfo, it->first )
			{
				if( ( kInfo.m_ulProductNo == kNXBTPurchaseReqInfo.m_ulProductNo ) &&
					( kInfo.m_usOrderQuantity == kNXBTPurchaseReqInfo.m_usOrderQuantity )
					)
				{
					// �̰� �����Ѵ�. ���� �������� �˻�����
					bCheck = true;
					break;
				}
			}

			if( bCheck == false )
			{
				// �� �׷쿣 ����. ���� �׷� �˻�����!
				bFind = false;
				break;
			}
		}

		if( bFind == true )
		{
			// ��ġ�ϴ� ���� ã�Ҵ�.
			if( it->second < SiKGameSysVal()->GetBingoEventPayoutRepeate() )
			{
				++it->second;
				START_LOG( cwarn, L"TESTLOG - ���� ���� ���� ��õ� Ƚ��" )
					<< BUILD_LOG( it->second )
					<< END_LOG;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	m_vecRewardRepeateInfo.push_back( std::make_pair( vecPurchaseReqInfo, 1 ) );
	return true;
}
#endif SERV_EVENT_BINGO
//}}