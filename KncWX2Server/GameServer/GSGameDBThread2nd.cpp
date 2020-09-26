#include <dbg/dbg.hpp>

#include "GSGameDBThread2nd.h"
#include "GameServer.h"

#include "NetError.h"

#include "X2Data/XSLUnit.h"


#define CLASS_TYPE KGSGameDBThread2nd

ImplPfID( KGSGameDBThread2nd, PI_GS_GAME_DB_2ND );

IMPL_PROFILER_DUMP( KGSGameDBThread2nd )
{

}

KGSGameDBThread2nd::~KGSGameDBThread2nd()
{

}

void KGSGameDBThread2nd::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {       
        _CASE( DBE_CHARACTER_LIST_2ND_REQ, KEGS_CHARACTER_LIST_1ST_ACK );

        //_CASE( DBE_CHANNEL_CHECK_NICK_NAME_REQ, KEGS_CHECK_NICK_NAME_REQ );
        _CASE( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK );
        _CASE( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ );
        CASE( DBE_GAME_CREATE_UNIT_REQ );
        _CASE( DBE_GAME_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_REQ );
        _CASE( DBE_CHANGE_NICK_NAME_REQ, KEGS_CHANGE_NICK_NAME_REQ );

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );

    }
}


void KGSGameDBThread2nd::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSGameDBThread2nd::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

_IMPL_ON_FUNC( DBE_CHARACTER_LIST_2ND_REQ, KEGS_CHARACTER_LIST_1ST_ACK )
{
    kPacket_.m_iOK = NetError::ERR_ODBC_01;

    std::vector< KUnitInfo >::iterator vit;

    UidType nUserUID = FIRST_SENDER_UID; //������ ���������� ����ID�� ����UID ������

    // ���� �������� �ݴ� �������� �Ҵ�.
    int nAnotherServerGroupID = ( KBaseServer::GetKObj()->GetServerGroupID() == SEnum::SGI_SOLES ? SEnum::SGI_GAIA : SEnum::SGI_SOLES );
    
    if ( FIRST_SENDER_UID != -1 ) 
    {
        int nUnitSlot = m_kSP.GetUnitSlotNum( nUserUID, kPacket_.m_strUserID ); // �ַ��� ĳ���� ���� ��

        kPacket_.m_mapServerGroupUnitSlot.insert( std::make_pair( nAnotherServerGroupID, nUnitSlot ) );

        std::vector<KUnitInfo> vecUnitInfo;
        m_kSP.GetUnitInfo( nUserUID, vecUnitInfo );
        m_kSP.GetEquipItemList( vecUnitInfo );
        m_kSP.GetLastPosition( vecUnitInfo );
        kPacket_.m_mapServerGroupUnitInfo.insert( std::map< int, std::vector<KUnitInfo> >::value_type( nAnotherServerGroupID, vecUnitInfo ) );
    }
    else
    {
        START_LOG( cerr, L"ĳ���� ����Ʈ ��û�� UserUID �̻�, 2nd DBThread " )
            << BUILD_LOG( nUserUID )
            << END_LOG;
    }
    
    kPacket_.m_iOK = NetError::NET_OK;
    
    SendToUser( LAST_SENDER_UID, EGS_CHARACTER_LIST_ACK, kPacket_ );
}

//_IMPL_ON_FUNC( DBE_CHANNEL_CHECK_NICK_NAME_REQ, KEGS_CHECK_NICK_NAME_REQ )
//{
//    KEGS_CHECK_NICK_NAME_ACK kPacket;
//    kPacket.m_iOK = NetError::ERR_ODBC_01;
//    kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
//
//    DO_QUERY( L"exec dbo.gup_check_nickname", L"N\'%s\'", % kPacket_.m_wstrNickName );
//    
//    if( m_kODBC.BeginFetch() )
//    {
//        FETCH_DATA( kPacket.m_iOK );
//        m_kODBC.EndFetch();
//    }
//
//    switch ( kPacket.m_iOK )
//    {
//    case -1:
//        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
//        break;
//    case -2: 
//        break;
//        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
//    default:
//        ;
//    }
//
//
//end_proc:
//    SendToUser( LAST_SENDER_UID, ECH_CHECK_NICK_NAME_ACK, kPacket );
//}

_IMPL_ON_FUNC( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ )
{
    KEGS_CHECK_NICK_NAME_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_wstrNickName = kPacket_.m_wstrNickName;

#ifdef SERV_ALL_RENEWAL_SP
	int iNickKeepDay = 14;	// �⺻ ����
#if defined( SERV_COUNTRY_TWHK )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_JP )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_EU )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_US )
	iNickKeepDay = 7;
#elif defined( SERV_COUNTRY_CN )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_TH )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_ID )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_BR )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_PH )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_IN )
	iNickKeepDay = 14;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDeletedNickNameHistory_SEL", L"N\'%s\', %d", % kPacket_.m_wstrNickName % iNickKeepDay );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_check_nickname", L"N\'%s\'", % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    switch ( kPacket.m_iOK )
    {
    case -1:
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
        break;
    case -2: 
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
        break;
    default:
        ;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_GAME_CREATE_UNIT_REQ )
{
    KEGS_CREATE_UNIT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;

    START_LOG( clog, L"���� ����" )
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << BUILD_LOG( kPacket_.m_iClass );

#ifdef SERV_NICK_NAME_DOUBLE_CHECK
	for ( int i = 0; i < (int)kPacket_.m_wstrNickName.size(); i++ )
	{
		WCHAR tempChar = kPacket_.m_wstrNickName[i];

		if ( !((tempChar >= 'a' && tempChar <= 'z') || (tempChar >= 'A' && tempChar <= 'Z') || (tempChar >= '0' && tempChar <= '9')))
		{
			START_LOG( cerr, L"[ERROR] �г��ӿ� �߸��� ���� ����!" )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_CREATE_UNIT_08;
			kPacket.m_kUnitInfo.m_wstrNickName = kPacket_.m_wstrNickName;
			kPacket.m_kUnitInfo.m_cUnitClass = kPacket_.m_iClass;
			goto end_proc;
		}
	}
#endif //SERV_NICK_NAME_DOUBLE_CHECK

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_INS", L"%d, N\'%s\', %d",
		% LAST_SENDER_UID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iClass
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_unit", L"%d, N\'%s\', %d",
		% LAST_SENDER_UID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iClass
		);
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK
            >> kPacket.m_kUnitInfo.m_nUnitUID
            >> kPacket.m_wstrEnableDate
        );

        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        switch( kPacket.m_iOK )
        {
        case -1:    kPacket.m_iOK = NetError::ERR_CREATE_UNIT_00;   break;
        case -2:    kPacket.m_iOK = NetError::ERR_CREATE_UNIT_01;   break;
        case -3:    kPacket.m_iOK = NetError::ERR_CREATE_UNIT_05;   break;
        case -10:   kPacket.m_iOK = NetError::ERR_CREATE_UNIT_02;   break;
        case -222:  kPacket.m_iOK = NetError::ERR_CREATE_UNIT_06;	break;
        case -23:   kPacket.m_iOK = NetError::ERR_CREATE_UNIT_09;	break;
#ifdef SERV_LIMIT_TO_CREATE_NEW_CHARACTER_EVENT
		case -99:   kPacket.m_iOK = NetError::NOT_CREATE_CHARACTER_BY_COUNT;	break;
		case -900:   kPacket.m_iOK = NetError::NOT_CREATE_CHARACTER_BY_COUNT;	break;
		case -901:   kPacket.m_iOK = NetError::NOT_CREATE_CHARACTER_BY_COUNT;	break;
#endif //SERV_LIMIT_TO_CREATE_NEW_CHARACTER_EVENT
        default:
            START_LOG( cerr, L"gup_create_unit: ���ǵ��� ���� ������Դϴ�." )
                << BUILD_LOG( kPacket.m_iOK )
                << END_LOG;
            break;
        }

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		kPacket.m_bReqGiant_DeleteName = true;
		kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

        goto end_proc;
    }

    kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    
    if( m_kODBC.BeginFetch() )
    {
        bool bIsSpiritUpdated = false;
        UidType iRecommendUnitUID = 0;
        int iMapIDDummy = 0;

        FETCH_DATA( kPacket.m_kUnitInfo.m_nUnitUID
            >> kPacket.m_kUnitInfo.m_cUnitClass
            >> kPacket.m_kUnitInfo.m_iEXP
            >> kPacket.m_kUnitInfo.m_ucLevel
            >> kPacket.m_kUnitInfo.m_iED
            >> kPacket.m_kUnitInfo.m_kStat.m_iBaseHP
            >> kPacket.m_kUnitInfo.m_kStat.m_iAtkPhysic
            >> kPacket.m_kUnitInfo.m_kStat.m_iAtkMagic
            >> kPacket.m_kUnitInfo.m_kStat.m_iDefPhysic
            >> kPacket.m_kUnitInfo.m_kStat.m_iDefMagic
            >> kPacket.m_kUnitInfo.m_iSPoint
            >> kPacket.m_kUnitInfo.m_wstrNickName
            >> iMapIDDummy
            >> kPacket.m_kUnitInfo.m_iSpirit
            >> bIsSpiritUpdated
            >> iRecommendUnitUID
            );

        m_kODBC.EndFetch();

        kPacket.m_iOK = NetError::NET_OK;
    }
    else
    {
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;

        goto end_proc;
    }

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_SEL", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_GET", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_kUnitInfo.m_iOfficialMatchCnt
            >> kPacket.m_kUnitInfo.m_iRating
            >> kPacket.m_kUnitInfo.m_iMaxRating
            >> kPacket.m_kUnitInfo.m_iRPoint
            >> kPacket.m_kUnitInfo.m_iAPoint
            >> kPacket.m_kUnitInfo.m_bIsWinBeforeMatch
            >> kPacket.m_kUnitInfo.m_cRank
            >> kPacket.m_kUnitInfo.m_iWin
            >> kPacket.m_kUnitInfo.m_iLose
            >> kPacket.m_kUnitInfo.m_fKFactor
            >> kPacket.m_kUnitInfo.m_bIsRedistributionUser
            >> kPacket.m_kUnitInfo.m_iPastSeasonWin
            );
        m_kODBC.EndFetch();
    }
    else
    {
        START_LOG( cerr, L"P_GUnitPVP_Season2_SEL ȣ�� ����!" )
            << BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
        goto end_proc;
    }

    //{{ 2009. 3. 31  ������	ĳ���� ������ ���θ�� ������
    int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemPeriod_INS", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_unit_set_promotion", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK	);
        m_kODBC.EndFetch();
    }
    else
    {
        START_LOG( cerr, L"ĳ���� ������ �������� ���� ����!" )
            << BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
        goto end_proc;
    }

    if( iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"ĳ���� ������ �������� ���� ����!" )
            << BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
            << BUILD_LOG( iOK )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
        goto end_proc;
    }
    //}}
end_proc:

    SendToUser( LAST_SENDER_UID, DBE_GAME_CREATE_UNIT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK )
{
    //    [dbo].[P_GUnit_CreateCNT_GET]
    //    ���� ����
    //    @iUnitUID         BIGINT
    //    ���� ����
    //    CNT                INT
    //KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK kPacket;

    // ���� �������� �ݴ� �������� �Ҵ�.
    int nAnotherServerGroupID = ( KBaseServer::GetKObj()->GetServerGroupID() == SEnum::SGI_SOLES ? SEnum::SGI_GAIA : SEnum::SGI_SOLES );
    int nCreateUnitCountToday = 0;

    DO_QUERY( L"exec dbo.P_GUnit_CreateCNT_GET", L"%d", % kPacket_.m_iUserUID );
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( nCreateUnitCountToday );
        m_kODBC.EndFetch();
    }

    kPacket_.m_mapCreateCharCountToday.insert( std::make_pair( nAnotherServerGroupID, nCreateUnitCountToday ) );
end_proc:

    SendToUser( LAST_SENDER_UID, EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK, kPacket_ );
    ;
}

_IMPL_ON_FUNC( DBE_GAME_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_REQ )
{
    KEGS_DELETE_UNIT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
#ifdef GUILD_TEST
    kPacket.m_iGuildUID = 0;	
    std::wstring wstrGuildName;
#endif GUILD_TEST

	//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
	//{{	//2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� ���� ������ ������ �� �˾� ���� ����
	std::wstring wstrDelDate  = L"";
	std::wstring wstrDelAbleDate  = L"";
	std::wstring wstrRestoreAbleDate  = L"";
	std::wstring wstrReDelAbleDate = L"";
	CTime tReDelAbleDateTime;
	CTime tDelAbleDateTime;

	//}}//2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� ���� ������ ������ �� �˾� ���� ����
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_SEL_Check", L"%d, %d",	% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_ReDelAbleDate_CHK", L"%d, %d",	% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK 
			>> wstrReDelAbleDate
			);

		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"ĳ���� ���� ���� ���� üũ ��� ���� �� ����??" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DELETE_UNIT_00;

		goto end_proc;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ���� ���� ���� üũ ����!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		if ( kPacket.m_iOK == -1 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_00;
		}
		else if( kPacket.m_iOK == -22 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_02;

			//{{ // 2012.03.06 lygan_������ // ���� ���ڿ��ð��� �״�� �޴� �κ��� �������� ���ڷ� �����ؼ� Ŭ��� ���� ������ ����
			if( KncUtil::ConvertStringToCTime( wstrReDelAbleDate, tReDelAbleDateTime ) == true )
				kPacket.m_tReDelAbleDate = tReDelAbleDateTime.GetTime();
			else
				kPacket.m_tReDelAbleDate = 0LL;
			//}}
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}

		goto end_proc;
	}
#endif SERV_UNIT_WAIT_DELETE
	//}}

#ifdef SERV_RECRUIT_EVENT_BASE
	DO_QUERY( L"exec dbo.gup_delete_recommend", L"%d", % kPacket_.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��õ�� ���̺��� ĳ���� ���� ����!" )
			<< BUILD_LOG( NetError::ERR_RECOMMEND_USER_06 )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
#endif SERV_RECRUIT_EVENT_BASE

    //{{ 2011. 02. 23	������	ĳ���� �α�
#ifdef SERV_CHAR_LOG
    // ĳ���� ������ ��´�.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    
    if( m_kODBC.BeginFetch() )
    {
        KUnitInfo kUnitInfo;
        bool bIsSpiritUpdated = false;
        UidType iRecommendUnitUID = 0;
        int iMapIDDummy = 0;

        FETCH_DATA( kUnitInfo.m_nUnitUID
            >> kPacket.m_cUnitClass
            >> kUnitInfo.m_iEXP
            >> kPacket.m_ucLevel
            >> kUnitInfo.m_iED
            >> kUnitInfo.m_kStat.m_iBaseHP
            >> kUnitInfo.m_kStat.m_iAtkPhysic
            >> kUnitInfo.m_kStat.m_iAtkMagic
            >> kUnitInfo.m_kStat.m_iDefPhysic
            >> kUnitInfo.m_kStat.m_iDefMagic
            >> kUnitInfo.m_iSPoint
            >> kPacket.m_wstrNickName
            >> iMapIDDummy
            >> kUnitInfo.m_iSpirit
            >> bIsSpiritUpdated
            >> iRecommendUnitUID
            );

        m_kODBC.EndFetch();
    }
    else
    {
        kPacket.m_iOK = NetError::ERR_SELECT_UNIT_00;
        goto end_proc;
    }
#endif SERV_CHAR_LOG
    //}}

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
	// ������ ���ϵ��� �߰��߽��ϴ�. by ������
	if( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>( kPacket.m_cUnitClass ) ) == CXSLUnit::UT_ELESIS )
	{
		kPacket.m_iOK = NetError::ERR_DELETE_UNIT_07;
		goto end_proc;
	}
#endif SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_UNIT_WAIT_DELETE //2012.06.08 lygan_������ // �г��� ���� ��� ���� ĳ���ʹ� ������ ���ϰ� ó�� �ϱ� ����
	if(kPacket.m_wstrNickName.length() != 0)
	{
		if(kPacket.m_wstrNickName.find(L"_") != -1)
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_05;
			goto end_proc;
		}
	}
#endif //SERV_UNIT_WAIT_DELETE

    //{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
    // ��� ���� ���
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_my_guild_info", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iGuildUID
            >> wstrGuildName );

        m_kODBC.EndFetch();
    }

    if( kPacket.m_iGuildUID > 0 )
    {
        //////////////////////////////////////////////////////////////////////////
        // ��� ������ ��� �˻�
        u_char ucGuildMemberShipGrade = 0;

        if( m_kSP.Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
        {
            START_LOG( cerr, L"���� ��� ���� ��� ����. GuildUID�� ��ȿ�ѵ� DB���� ��������� ����? ������ ���¿���!" )
                << BUILD_LOG( kPacket.m_iOK )
                << BUILD_LOG( kPacket_.m_iUnitUID )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_GUILD_19; // ��忡 ���� ������ �ƴմϴ�.
            goto end_proc;
        }

        // ��� �����ʹ� ĳ���� ������ �Ҽ� ����!
        if( ucGuildMemberShipGrade == SEnum::GUG_MASTER )
        {
            kPacket.m_iOK = NetError::ERR_GUILD_35;
            goto end_proc;
        }

        //////////////////////////////////////////////////////////////////////////
        // ��� Ż�� ó��
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_DEL", L"%d, %d", 
			% kPacket_.m_iUnitUID
			% kPacket.m_iGuildUID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_guild_member", L"%d, %d", 
			% kPacket_.m_iUnitUID
			% kPacket.m_iGuildUID
			);
#endif //SERV_ALL_RENEWAL_SP
        
        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( kPacket.m_iOK );
            m_kODBC.EndFetch();
        }

        if( kPacket.m_iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"��� Ż�� ����." )
                << BUILD_LOG( kPacket.m_iOK )
                << BUILD_LOG( kPacket_.m_iUnitUID )
                << BUILD_LOG( kPacket.m_iGuildUID )
                << END_LOG;

            switch( kPacket.m_iOK )
            {
            case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // �ش� ��忡 ���� ������ �ƴմϴ�.
            case -2: kPacket.m_iOK = NetError::ERR_GUILD_26; break; // Ʈ������ ����
            default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // ��� Ż�� ����
            }

            goto end_proc;
        }
    }
#endif GUILD_TEST
    //}}

#ifdef SERV_ALL_RENEWAL_SP
	int iDelableDay = 1;	// �⺻ ����
	int iRestoreableDay = 1;
#if defined( SERV_COUNTRY_TWHK )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_JP )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_EU )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_US )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_CN )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_TH )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_ID )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_BR )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_PH )
	iDelableDay = 1;
	iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_IN )
	iDelableDay = 1;
	iRestoreableDay = 1;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

	//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitWaitDelete_INS", L"%d, %d, %d, %d", % kPacket_.m_iUnitUID 
		% kPacket_.m_iUserUID
		% iDelableDay
		% iRestoreableDay );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitWaitDelete_INT", L"%d, %d", % kPacket_.m_iUnitUID 
		% kPacket_.m_iUserUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}


	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ���� ��� ó�� ����!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		if( kPacket.m_iOK == -1 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_01;
		}
		else if ( kPacket.m_iOK == -11 || kPacket.m_iOK == -12 )
		{
			// -11 : ĳ���� ���� ���� ����, -12 : ���� ���� ���� ����
			// ���� ���忡���� DB������ ������
			kPacket.m_iOK = NetError::ERR_ODBC_01;
		} 
		else if ( kPacket.m_iOK == -22 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_02;
		}
	}


	//{{ //2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� ���� ������ ������ �� �˾� ���� ����
	DO_QUERY( L"exec dbo.P_GUnitWaitDelete_SEL", L"%d",	% kPacket_.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( wstrDelDate 
			>> wstrDelAbleDate
			>> wstrRestoreAbleDate
			);

		m_kODBC.EndFetch();
	}

	//{{ // 2012.03.06 lygan_������ // ���� ���ڿ��ð��� �״�� �޴� �κ��� �������� ���ڷ� �����ؼ� Ŭ��� ���� ������ ����
	if( KncUtil::ConvertStringToCTime( wstrDelAbleDate, tDelAbleDateTime ) == true )
		kPacket.m_tDelAbleDate = tDelAbleDateTime.GetTime();
	else
		kPacket.m_tDelAbleDate = 0LL;
	//}}

	//}} //2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� ���� ������ ������ �� �˾� ���� ����
#else SERV_UNIT_WAIT_DELETE
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2012. 02. 21	��μ�	ĳ���� ���� �� ��� Ż�� ����ó�� ����
#ifdef SERV_UNIT_DELETE_EXCEPTION_MODIFY
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_Delete", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_unit", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_UNIT_DELETE_EXCEPTION_MODIFY
	DO_QUERY( L"exec dbo.gup_delete_unit", L"%d", % kPacket_.m_iUnitUID );
#endif SERV_UNIT_DELETE_EXCEPTION_MODIFY
	//}}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ���� ����!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( m_kODBC.GetLastQuery() )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_DELETE_UNIT_00; break; // �ش� ĳ���͸� ã�� �� �����ϴ�.
			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		case -2: kPacket.m_iOK = NetError::ERR_DELETE_UNIT_06; break; // �ο� ���¿����� ���� �� �� �����ϴ�.
#endif SERV_RELATIONSHIP_SYSTEM
			//}
		default: kPacket.m_iOK = NetError::ERR_DELETE_UNIT_05; break; // ĳ���� ���� ����
		}
	}
#endif SERV_UNIT_WAIT_DELETE
	//}}

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_GAME_DELETE_UNIT_ACK, kPacket );
}

//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_GAME_FINAL_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_REQ )
{
	KEGS_FINAL_DELETE_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	//{{ 2012. 06. 11	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int iMapIDDummy = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 02. 23	������	ĳ���� �α�
#ifdef SERV_CHAR_LOG
	// ĳ���� ������ ��´�.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		KUnitInfo kUnitInfo;
		bool bIsSpiritUpdated = false;
		UidType iRecommendUnitUID = 0;

		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kPacket.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kPacket.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kPacket.m_wstrNickName
			>> iMapIDDummy
			>> kUnitInfo.m_iSpirit				
			>> bIsSpiritUpdated
			>> iRecommendUnitUID			
			);

		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_00;
		goto end_proc;
	}
#endif SERV_CHAR_LOG
	//}}

	// ��� ������ �̹� ���� �Ǿ��ٰ� �� //
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_Delete_DelSystem", L"%d, %d", % kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_unit", L"%d, %d", % kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ���� ���� ó�� ����!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		if( kPacket.m_iOK == -21 || kPacket.m_iOK == - 23)
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_04;
		}

		else if ( kPacket.m_iOK == -22 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_03;

		}
		else if (	kPacket.m_iOK == -24 || kPacket.m_iOK == -11 || 
			kPacket.m_iOK == -13 || kPacket.m_iOK == -15 )
		{
			kPacket.m_iOK = NetError::ERR_ODBC_01;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GAME_FINAL_DELETE_UNIT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_GAME_RESTORE_UNIT_REQ, KEGS_RESTORE_UNIT_REQ )
{
	KEGS_RESTORE_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	std::wstring wstrRestoreAbleDate  = L"";
	CTime tRestoreAbleDateTime;

	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_INS", L"%d, %d",% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_SET", L"%d, %d",% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK 
			>> wstrRestoreAbleDate
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ĳ���� ���� ó�� ����!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		if( kPacket.m_iOK == -31 || kPacket.m_iOK == -33)
		{
			kPacket.m_iOK = NetError::ERR_RESTORE_UNIT_01;
		}

		else if ( kPacket.m_iOK == -32 )
		{
			kPacket.m_iOK = NetError::ERR_RESTORE_UNIT_02;
			//{{ // 2012.03.06 lygan_������ // ���� ���ڿ��ð��� �״�� �޴� �κ��� �������� ���ڷ� �����ؼ� Ŭ��� ���� ������ ����
			if( KncUtil::ConvertStringToCTime( wstrRestoreAbleDate, tRestoreAbleDateTime ) == true )
				kPacket.m_tRestoreAbleDate = tRestoreAbleDateTime.GetTime();
			else
				kPacket.m_tRestoreAbleDate = 0LL;
			//}}

		}
		else if ( kPacket.m_iOK == -34 || kPacket.m_iOK == -11 )
		{
			kPacket.m_iOK = NetError::ERR_ODBC_01;
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GAME_RESTORE_UNIT_ACK, kPacket );
}
#endif SERV_UNIT_WAIT_DELETE
//}}

_IMPL_ON_FUNC( DBE_CHANGE_NICK_NAME_REQ, KEGS_CHANGE_NICK_NAME_REQ )
{
    KEGS_CHANGE_NICK_NAME_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
    kPacket.m_bCheckOnly = kPacket_.m_bCheckOnly;

#ifdef SERV_ALL_RENEWAL_SP
	int iNickKeepDay = 14;	// �⺻ ����
#if defined( SERV_COUNTRY_TWHK )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_JP )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_EU )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_US )
	iNickKeepDay = 7;
#elif defined( SERV_COUNTRY_CN )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_TH )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_ID )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_BR )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_PH )
	iNickKeepDay = 14;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

    // ��밡���� �г������� üũ�� �ϱ�
    if( kPacket_.m_bCheckOnly )
    {
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GDeletedNickNameHistory_SEL", L"N\'%s\', %d", % kPacket_.m_wstrNickName % iNickKeepDay );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_check_nickname", L"N\'%s\'", % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( kPacket.m_iOK );
            m_kODBC.EndFetch();
        }
        else
        {
            kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
            goto end_proc;
        }

        if( kPacket.m_iOK != NetError::NET_OK )
        {
            switch( kPacket.m_iOK )
            {
            case -1:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
                break;
            case -2:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
                break;			

            default:
                {
                    START_LOG( cerr, L"���ǵ��� ���� ����! sp�� �ٲ���?" )
                        << BUILD_LOG( kPacket.m_iOK )
                        << BUILD_LOG( kPacket_.m_iUnitUID )
                        << BUILD_LOG( kPacket_.m_wstrNickName )
                        << END_LOG;

                    kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
                }
            }

            goto end_proc;
        }
    }
    // ������ �г��� �����ϱ�
    else
    {
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnitNickName_UPD_Restore", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrNickName );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_restore_nickname", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( kPacket.m_iOK );
            m_kODBC.EndFetch();
        }
        else
        {
            kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
            goto end_proc;
        }

        if( kPacket.m_iOK != NetError::NET_OK )
        {
            switch( kPacket.m_iOK )
            {
            case -1:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_00;
                break;
            case -2:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_01;
                break;
            case -3:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
                break;
            case -4:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
                break;
            case -5:
                kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
                break;

            default:
                {
                    START_LOG( cerr, L"���ǵ��� ���� ����! sp�� �ٲ���?" )
                        << BUILD_LOG( kPacket.m_iOK )
                        << BUILD_LOG( kPacket_.m_iUnitUID )
                        << BUILD_LOG( kPacket_.m_wstrNickName )
                        << END_LOG;

                    kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
                }
            }

            goto end_proc;
        }
        else
        {
            kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
        }
    }    

end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
        << BUILD_LOG( kPacket.m_iOK )
        << BUILD_LOG( m_kODBC.GetLastQuery() )
        << END_LOG;

    SendToUser( LAST_SENDER_UID, DBE_CHANGE_NICK_NAME_ACK, kPacket );
}