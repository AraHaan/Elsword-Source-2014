#include "LoginRMEventProcess.h"
#include "LoginRoomManager.h"

#include "NetError.h"
#include "GameSysVal/GameSysVal.h"
#include "Socket/NetCommon.h"


#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance(); \
	START_LOG( cout, L#className L" ��ũ��Ʈ ������ �Ϸ��Ͽ����ϴ�!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


ImplPfID( KLoginRMEventProcess, PI_LOGIN_ROOM );

#define CLASS_TYPE  KLoginRMEventProcess

KLoginRMEventProcess::KLoginRMEventProcess()
{
}

KLoginRMEventProcess::~KLoginRMEventProcess()
{
}

//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP

ImplToStringW( KLoginRMEventProcess )
{
	return START_TOSTRING_PARENTW( KPerformer );
}

#endif SERV_MAX_QUEUE_SIZE_DUMP
//}}

void KLoginRMEventProcess::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( ERM_OPEN_PSHOP_AGENCY_REQ );
	CASE( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT );
	CASE( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ );

	default:
		START_LOG( cerr, L"�ڵ鷯�� �������� ���� �̺�Ʈ." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_REQ )
{
	UidType iRoomUID = GetKLoginRoomManager()->OpenRoom();
	if( iRoomUID <= 0 )
	{
		KERM_OPEN_PERSONAL_SHOP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_ROOM, anTrace, ERM_OPEN_PSHOP_AGENCY_REQ, kPacket_ );
	GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( iRoomUID, spEvent );
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT )
{
	UidType iRoomUID = GetKLoginRoomManager()->OpenRoom();
	if( iRoomUID <= 0 )
	{
		START_LOG( cerr, L"�������� �ڵ����� �븮���� ������ �Ϸ����ϴµ� �� ���� �����ϴ�! �Ͼ���� �ȵǴ� ����!" )
			<< END_LOG;
		return;
	}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, kPacket_ );
	GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( iRoomUID, spEvent );
}

IMPL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ )
{
    KERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK kPacket;
    
    KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT kData;
    kData.m_iUserUID = kPacket_.m_iUserUID;
    kData.m_iUnitUID = kPacket_.m_iUnitUID;
    kData.m_wstrNickName = kPacket_.m_wstrNickName;
    kData.m_cPersonalShopType = kPacket_.m_cPersonalShopType;
    kData.m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
    kData.m_wstrAgencyOpenDate = kPacket_.m_wstrAgencyOpenDate;
    kData.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
    kData.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
    kData.m_bOnSale = kPacket_.m_bOnSale;

    
    if( GetKLoginRoomManager()->GetPShopAgencyUIDByUnitUID( kPacket_.m_iUnitUID, kPacket.m_iPShopAgencyUID ) == false )
	{
        // ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT �� ����.
        UidType iRoomUID = GetKLoginRoomManager()->OpenRoom();
        if( iRoomUID <= 0 )
        {
            START_LOG( cerr, L"�������� �ڵ����� �븮���� ������ �Ϸ����ϴµ� �� ���� �����ϴ�! �Ͼ���� �ȵǴ� ����!" )
                << END_LOG;
            return;
        }

        if ( kData.m_wstrAgencyExpirationDate.empty() )
        {
            START_LOG( cerr, L"�������� �ڵ����� �븮���� �����õ�(����)���� �������� �� ���� �߻� " )
                << BUILD_LOG( kPacket_.m_iUserUID )
                << BUILD_LOG( kPacket_.m_iUnitUID )
                << END_LOG;
            return;
        }

        KEventPtr spEvent( new KEvent );
        spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, kData );
        GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( iRoomUID, spEvent );

		START_LOG( cerr, L"DB���� �����Ǿ��ٰ� �Ǿ��ִµ� ������ �븮���� ���� ���� �ȵǾ���? ����! �Ǵ� ���ܻ�Ȳ!" )
            << BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

        kPacket.m_iPShopAgencyUID = iRoomUID;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK, kPacket );

		return;
	}

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ, kPacket_ );

    GetKLoginRoomManager()->QueueingEventToRoomByRoomUID( kPacket.m_iPShopAgencyUID, spEvent );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK, kPacket );
}




