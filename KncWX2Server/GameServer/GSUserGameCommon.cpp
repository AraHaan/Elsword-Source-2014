#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "KncSend.h"
#include "GSSimLayer.h"

#include "ExpTable.h"
#include "StatTable.h"
#include "BaseServer.h"
#include "X2Data/XSLGame.h"
#include "X2Data/XSLStringFilter.h"
#include "GameEvent/GameEventManager.h"
#include "TutorialManager.h"
//{{ 2008. 11. 24  ������	��ŷ �Ŵ���
#include "GSRankingManager.h"
//}}
//{{ 2008. 11. 24  ������	�� ����Ʈ
#include "RoomListManager.h"
//}}
//{{ 2009. 7. 16  ������	ä�θŴ���
#include "ChannelManager.h"
//}}

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
#include ".\rewardtable.h"
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
#include "GameSysVal/GameSysVal.h"
#endif SERV_COME_BACK_USER_REWARD
//}} 
//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2012. 04. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	#include "../Common/X2Data/XSLBuffManager.h"
	#include "GSWorldMissionManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
//{{ 2012. 09. 26	������		���� ���� ���� ����
#ifdef SERV_POST_COPY_BUG_FIX
	#include "SMSPhoneNumberManager.h"
#endif SERV_POST_COPY_BUG_FIX
//}}

//{{ 2010. 11. 15  ��ȿ��	GameForge ä�� �α� ����
#ifdef SERV_RECORD_CHAT
#include "GameSysVal/GameSysVal.h"
#endif SERV_RECORD_CHAT
//}}

//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT
#include "RecommendUserTable.h"
#endif SERV_RECOMMEND_LIST_EVENT
//}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-27
	#include "NexonSOAPManager.h"
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	#include "NexonSOAP.h"
#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-07	// �ڼ���
	#include "X2Data/XSLBattleFieldManager.h"
#endif // SERV_BATTLE_FIELD_BOSS
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_MANUFACTURE_PERIOD_FIX
#include "X2Data/XSLManufactureItemManager.h"
#endif //SERV_MANUFACTURE_PERIOD_FIX

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

#include "odbc/Odbc.h"
#include "Enum/Enum.h"


#define CLASS_TYPE      KGSUser

// ����� 2014-01-07 : ���Ա��� ����(ĳ����â) �ϸ鼭 Ŭ���̾�Ʈ���� �Ⱥ����� ��Ŷ�� �Ǿ���, 
// ������ �κ��丮����(���)�� �ٸ� ������ �������� �� ����
IMPL_ON_FUNC_NOPARAM( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	//{{ 2012. 06. 13	��μ�       ������ �˴ٿ���
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	if( m_kUserInfoByNexon.GetRestrictionUser() > KUserInfoByNexon::NONE_AUTH )
	{
		// ������ ���� �ʾҰų� �˴ٿ� �����
		// �޽��� ����( �˾�â )
		KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK kAck;
		
		switch( m_kUserInfoByNexon.GetRestrictionUserMSG() )
		{
		case KUserInfoByNexon::AGE_AUTH_ERROR: // 1
			{
				kAck.m_iOK = NetError::ERR_SHUT_DOWN_02;
			}break;
		case KUserInfoByNexon::NAME_AUTH_ERROR: // 2
			{
				kAck.m_iOK = NetError::ERR_SHUT_DOWN_03;
			}break;
		case KUserInfoByNexon::SHUTDOWN_USER_ERROR: // 3
			{
				kAck.m_iOK = NetError::ERR_SHUT_DOWN_04;
			}break;
		case KUserInfoByNexon::SELECTIVE_SHUTDOWN_USER_ERROR: // 4
			{
				kAck.m_iOK = NetError::ERR_SHUT_DOWN_05;
			}break;
		case KUserInfoByNexon::UNKNOWN_ERROR: // 99
			{
				kAck.m_iOK = NetError::ERR_SHUT_DOWN_01;
			}break;
		default:
			{
				kAck.m_iOK = NetError::ERR_SHUT_DOWN_01;
			}
		}

		SendPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, kAck );

		// ���� ���� ����
		SetDisconnectReason( KStatistics::eSIColDR_Policy_Shutdown_Disconnect );
		ReserveDestroy();
		return;
	}
#else
	 // ���� ������ �ؿ��� �߰�
	/*if( m_kUserInfoByNexon.IsShutdownUser() > KUserInfoByNexon::NONE_AUTH )
	{
		KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK kAck;
		kAck.m_iOK = NetError::ERR_SHUT_DOWN_00;

		SendPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, kAck );

		// ���� ���� ����
		SetDisconnectReason( KStatistics::eSIColDR_Policy_Shutdown_Disconnect );
		ReserveDestroy();
		return;
	}

	if( m_kUserInfoByNexon.GetPeaceOfMindLogin() == true )
	{
		KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK kAck;
		kAck.m_iOK = NetError::ERR_SHUT_DOWN_01;

		SendPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, kAck );

		// ���� ���� ����
		SetDisconnectReason( KStatistics::eSIColDR_PeaceOfMindLogin );
		ReserveDestroy();
		return;
	}*/
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	KDBE_GET_SECOND_SECURITY_INFO_REQ kReq;
	kReq.m_iUserUID = GetUID();
	SiKGameSysVal()->GetComeBackRewardCondition( kReq.m_mapComeBackRewardCondition );
	
	//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	kReq.m_wstrID = GetName();
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}

	SendToAccountDB( DBE_GET_SECOND_SECURITY_INFO_REQ, kReq );	// AccountDB�� �ֱ� ���� ���� ���� ���
#else
	SendToGameDB( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, GetName() );
#endif SERV_SECOND_SECURITY
	//}}
    
}

_IMPL_ON_FUNC( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK )
{
	//{{ 2011. 06. 23	������	2�� ��� ����ó��
#ifdef SERV_CHECK_SECOND_PW_STATE
	VERIFY_STATE( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_CHECK_SECOND_PW ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
#endif SERV_CHECK_SECOND_PW_STATE
	//}}    
	
    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        m_nUnitSlot = kPacket_.m_nUnitSlot;

        // udp ���� ���� p2p ip/port�� ä�������ش�.(�ӽð� �ɼ����� ���Ŀ� �Ȱܾ� �Ҽ����ִ�.!�Ӹ������� )		
        for( int i = 0; i < (int)kPacket_.m_vecUnitInfo.size(); ++i )
        {
            kPacket_.m_vecUnitInfo[i].m_iOwnerUserUID		= GetUID();
            kPacket_.m_vecUnitInfo[i].m_nStraightVictories	= 0;
            kPacket_.m_vecUnitInfo[i].m_wstrIP				= GetP2PIP();
            kPacket_.m_vecUnitInfo[i].m_usPort				= GetP2PPort();
			//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
#else
			kPacket_.m_vecUnitInfo[i].m_iPVPEmblem			= CXSLUnit::GetPVPEmblem( kPacket_.m_vecUnitInfo[i].m_iVSPointMax );
#endif SERV_PVP_NEW_SYSTEM
			//}}			

            const CXSLUnit::UnitTemplet* pUnitTemplet		= SiCXSLUnitManager()->GetUnitTemplet( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_vecUnitInfo[i].m_cUnitClass) );
			if( pUnitTemplet == NULL )
			{
				START_LOG_WITH_NAME( cerr )
					<< BUILD_LOGc( kPacket_.m_vecUnitInfo[i].m_cUnitClass )
					<< END_LOG;

#if defined( _IN_HOUSE_ )
				continue;
#endif 

				kPacket_.m_iOK = NetError::ERR_CREATE_UNIT_03;
				goto end_proc;

			}

			kPacket_.m_vecUnitInfo[i].m_ucLevel				= SiKExpTable()->CheckLevelUp( 1, kPacket_.m_vecUnitInfo[i].m_iEXP );
			kPacket_.m_vecUnitInfo[i].m_nNowBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)kPacket_.m_vecUnitInfo[i].m_ucLevel );
			kPacket_.m_vecUnitInfo[i].m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)(kPacket_.m_vecUnitInfo[i].m_ucLevel + 1) );

			KStatTable::KUnitStatInfo sUnitStat;
			SiKStatTable()->GetUnitStat( kPacket_.m_vecUnitInfo[i].m_cUnitClass, kPacket_.m_vecUnitInfo[i].m_ucLevel, sUnitStat );
			kPacket_.m_vecUnitInfo[i].m_kStat.m_iBaseHP		+= (int)sUnitStat.m_uiHP;
			kPacket_.m_vecUnitInfo[i].m_kStat.m_iAtkPhysic	+= (int)sUnitStat.m_usAtkPhysic;
			kPacket_.m_vecUnitInfo[i].m_kStat.m_iAtkMagic	+= (int)sUnitStat.m_usAtkMagic;
			kPacket_.m_vecUnitInfo[i].m_kStat.m_iDefPhysic	+= (int)sUnitStat.m_usDefPhysic;
			kPacket_.m_vecUnitInfo[i].m_kStat.m_iDefMagic	+= (int)sUnitStat.m_usDefMagic;

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-07	// �ڼ���
			// ���� �ʵ��� ������ �̵� ��Ű��
			if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_vecUnitInfo[i].m_kLastPos.m_iMapID ) == true )
			{
				kPacket_.m_vecUnitInfo[i].m_kLastPos.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( kPacket_.m_vecUnitInfo[i].m_ucLevel, kPacket_.m_vecUnitInfo[i].m_mapDungeonClear );
				kPacket_.m_vecUnitInfo[i].m_kLastPos.m_bIgnoreLastTouch = true;
			}
#endif // SERV_BATTLE_FIELD_BOSS

			//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
			// �����ϰ� �ִ� ĳ���� ����Ʈ ����
			m_setMyUnitUIDList.insert( kPacket_.m_vecUnitInfo[i].m_nUnitUID );
#endif SERV_CHAR_LOG
			//}}

#ifdef SERV_2012_PVP_SEASON2// �۾���¥: 2013-07-01	// �ڼ���
			GetPvpRankForClient( kPacket_.m_vecUnitInfo[i].m_iOfficialMatchCnt, kPacket_.m_vecUnitInfo[i].m_cRank );
#endif // SERV_2012_PVP_SEASON2
        }

        START_LOG( clog, L"my unit list�� ������� p2p data" )
            << BUILD_LOG( GetP2PIP() )
            << BUILD_LOG( GetP2PPort() );

        if( ( int )kPacket_.m_vecUnitInfo.size() > m_nUnitSlot )
        {
            START_LOG( cerr, L"ĳ���� ���� ������ �ʰ��ϴ� ���� ĳ���͸� �����ϰ� ����." )
                << BUILD_LOG( ( int )kPacket_.m_vecUnitInfo.size() )
                << BUILD_LOG( m_nUnitSlot )
                << BUILD_LOG( GetUID() )
                << BUILD_LOG( GetName() );
        }
		//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
		// ĳ���� �����ִ� ���� ������ ����
		std::stable_sort(kPacket_.m_vecUnitInfo.begin(), kPacket_.m_vecUnitInfo.end(), &LastDateSort );
#endif SERV_UNIT_WAIT_DELETE
		//}}

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		// DB���� ������ ���� �����صд�. ������ ������ ��� �� ���� ����
		m_4ThAnnivEventInfo = kPacket_.m_4ThAnnivEventInfo;
		START_LOG(clog, L"[4�ֳ�] ���� �� ����Ǵ��� Ȯ�� �մϴ�")
			<< BUILD_LOG( m_4ThAnnivEventInfo.m_iCountQuestComplete )
			<< END_LOG;
#endif // SERV_4TH_ANNIVERSARY_EVENT
    }

end_proc:

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	KESG_REWARD_DB_DATA_NOT kPacketReward;
	kPacketReward.m_mapTotalRewardData = SiKRewardTable()->GetMapTotalRewardData();
	SendPacket( ESG_REWARD_DB_DATA_NOT, kPacketReward );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	SendPacket( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_CREATE_UNIT_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CREATE_UNIT_REQ, EGS_CREATE_UNIT_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CREATE_UNIT_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

    const CXSLUnit::UnitTemplet* pUnitTemplet = SiCXSLUnitManager()->GetUnitTemplet( (CXSLUnit::UNIT_CLASS)kPacket_.m_iClass );
    KEGS_CREATE_UNIT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_UNKNOWN;
	char charBuf[255] = {0};
	std::string strNickName;

#ifdef SERV_LIMIT_UNIT
	//�������� �����ܿ��� ĳ���� ���´�. 
	const CXSLUnit::UNIT_TYPE eLimitType = static_cast< CXSLUnit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType );
	if( eLimitType <  CXSLUnit::GetUnitClassToUnitType( (CXSLUnit::UNIT_CLASS)kPacket_.m_iClass ) )
	{
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
		goto fail_proc;
	}
#endif //SERV_LIMIT_UNIT

#ifdef SERV_NO_ARA
	if( CXSLUnit::UT_ARA ==  CXSLUnit::GetUnitClassToUnitType( (CXSLUnit::UNIT_CLASS)kPacket_.m_iClass ) )
	{
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
		goto fail_proc;
	}
#endif

    if( !pUnitTemplet ) // 070604. florist. templet�� ���� �ʿ�� ����. iClass�� ��ȿ�� ���� ������ �ǹ̸� ������.
    {
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
        goto fail_proc;
    }

#ifdef SERV_CHECK_CREATE_UNIT_CLASS	// ���볯¥: 2013-07-18
	if( CXSLUnit::IsInitNormalJob( (CXSLUnit::UNIT_CLASS)kPacket_.m_iClass ) == false )
	{
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_11;
		goto fail_proc;
	}
#endif	// SERV_CHECK_CREATE_UNIT_CLASS
	
	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrNickName.c_str(), -1, charBuf, 255, NULL, NULL );
	strNickName = charBuf;

	if( strNickName.empty() || strNickName.size() > 12
#ifdef SERV_NICK_NAME_MINIMUM_CHECK
		|| strNickName.size() < 2
#endif //SERV_NICK_NAME_MINIMUM_CHECK
		)   //ĳ���� �г��� ���� �˻�.
	{
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_04;
        goto fail_proc;
	}

#ifdef SERV_STRING_FILTER_USING_DB
	if ( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if ( GetAuthLevel() < SEnum::UAL_GM && SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
	{
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_08;
		goto fail_proc;
	}

	//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( SiKPvpMatchManager()->IsExistPvpNpcName( kPacket_.m_wstrNickName ) == true )
	{		
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_01;
		goto fail_proc;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

    // ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
    if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
    {
        kPacket.m_iOK = NetError::ERR_ODBC_00;
        goto fail_proc;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << BUILD_LOG( kPacket_.m_iClass );

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		KEGIANT_ROLEREG_CREATE_UNIT_REQ kReq;
		kReq.m_iActorUID = GetUID();
		kReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		kReq.m_CreateUnitReqPacket = kPacket_;
		SendToGiantRoleReg( EGIANT_ROLEREG_CREATE_UNIT_REQ, kReq );
		return;
	}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

	//{{ 2011. 08. 09  ��μ� (2011.08.11) Ư���� ���� ������ ������ ���Ͽ� �ű��ɸ��� ���� �� ������ ���� �̺�Ʈ
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	{
		KDBE_GAME_CREATE_UNIT_REQ kReq;

		KAccountInfo AccountTemp;
		GetAccountInfo( AccountTemp );
		kReq.m_wstrRegDate = AccountTemp.m_wstrRegDate;
		kReq.m_wstrNickName = kPacket_.m_wstrNickName;
		kReq.m_iClass = kPacket_.m_iClass;

		SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kReq );
		return;
	}
#else
	SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kPacket_ );
	return;
#endif SERV_NEW_CREATE_CHAR_EVENT
		//}}

fail_proc:

    LOG_CONDITION( kPacket.m_iOK == NetError::ERR_CREATE_UNIT_03, cerr, cwarn )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_iOK )
        << BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
        << BUILD_LOG( kPacket_.m_iClass)
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << END_LOG;

    SendPacket( EGS_CREATE_UNIT_ACK, kPacket );
    return;
}

_IMPL_ON_FUNC( DBE_GAME_CREATE_UNIT_ACK, KEGS_CREATE_UNIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
#ifdef SERV_2012_PVP_SEASON2// �۾���¥: 2013-07-01	// �ڼ���
		GetPvpRankForClient( kPacket_.m_kUnitInfo.m_iOfficialMatchCnt, kPacket_.m_kUnitInfo.m_cRank );
#endif // SERV_2012_PVP_SEASON2

		SendToAccountDB( DBE_ACCOUNT_CREATE_UNIT_REQ, kPacket_ );
		
		//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT
		// �ּ� �ѹ��� ��õ �����ϵ��� �Ѵ�.
#else
		// ��õ���� �ٽ� ������� �ʵ��� Account DB�� Update
		if( !m_kUserRecommendManager.IsRecommend() )
		{
			SendToAccountDB( DBE_RECOMMEND_USER_NOT );
		}
#endif SERV_RECOMMEND_LIST_EVENT
		//}
		return;
	}
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	//{{ 2012.02.21 ��ȿ��  ���� �̸� ���� ���μ��� �ָ��Ѱ� ���� 
	else if( kPacket_.m_bReqGiant_DeleteName )
	{
		//{{ �÷��� ���ϼ� ������ ��������� DB���� ������ ���� ��� ���ϸ� �÷����� �г��� ������� �Ѵ�
		KEGIANT_ROLEREG_DELETE_UNIT_REQ kPacketReq;
		kPacketReq.m_wstrNickName = kPacket_.m_wstrNickName;
		kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_UNIT_REQ, kPacketReq );
		//}}
	}
	//}}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

	SendPacket( EGS_CREATE_UNIT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_CREATE_UNIT_ACK, KEGS_CREATE_UNIT_ACK )
{
    VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//�г��� �ߺ��� ������ �ƴϱ⶧���� üũ
		if( kPacket_.m_iOK == NetError::ERR_CREATE_UNIT_01 )
		{
			START_LOG_WITH_NAME( clog )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ))
				<< END_LOG;
		}
		else
		{
			START_LOG_WITH_NAME( cerr )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ))
				<< END_LOG;
		}

		SendPacket( EGS_CREATE_UNIT_ACK, kPacket_ );
        return;
	}

    const CXSLUnit::UnitTemplet* pUnitTemplet = SiCXSLUnitManager()->GetUnitTemplet( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_kUnitInfo.m_cUnitClass) );
	if( pUnitTemplet == NULL )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOGc( kPacket_.m_kUnitInfo.m_cUnitClass )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_CREATE_UNIT_03;
		SendPacket( EGS_CREATE_UNIT_ACK, kPacket_ );
	}
	
	kPacket_.m_kUnitInfo.m_ucLevel				= SiKExpTable()->CheckLevelUp( 1, kPacket_.m_kUnitInfo.m_iEXP );
	kPacket_.m_kUnitInfo.m_nNowBaseLevelEXP		= SiKExpTable()->GetRequireTotalExpbyLevel( (int)kPacket_.m_kUnitInfo.m_ucLevel );
	kPacket_.m_kUnitInfo.m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)(kPacket_.m_kUnitInfo.m_ucLevel + 1) );

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	// �����ϰ� �ִ� ĳ���� ����Ʈ�� �߰�
	m_setMyUnitUIDList.insert( kPacket_.m_kUnitInfo.m_nUnitUID );
#endif SERV_CHAR_LOG
	//}}

	KStatTable::KUnitStatInfo sUnitStat;
	SiKStatTable()->GetUnitStat( kPacket_.m_kUnitInfo.m_cUnitClass, kPacket_.m_kUnitInfo.m_ucLevel, sUnitStat );
	kPacket_.m_kUnitInfo.m_kStat.m_iBaseHP		+= (int)sUnitStat.m_uiHP;
	kPacket_.m_kUnitInfo.m_kStat.m_iAtkPhysic	+= (int)sUnitStat.m_usAtkPhysic;	
	kPacket_.m_kUnitInfo.m_kStat.m_iAtkMagic	+= (int)sUnitStat.m_usAtkMagic;
	kPacket_.m_kUnitInfo.m_kStat.m_iDefPhysic	+= (int)sUnitStat.m_usDefPhysic;
	kPacket_.m_kUnitInfo.m_kStat.m_iDefMagic	+= (int)sUnitStat.m_usDefMagic;
	SendPacket( EGS_CREATE_UNIT_ACK, kPacket_ );

	// ĳ���Ͱ� ������ ���, Level1�� ���۽����� DB�� �����.
	KDBE_CHARACTER_LEVELUP_NOT kNot;
	kNot.m_nCharUID         = kPacket_.m_kUnitInfo.m_nUnitUID;
	kNot.m_ucLevelBefore    = kPacket_.m_kUnitInfo.m_ucLevel;
	kNot.m_ucLevelAfter     = kPacket_.m_kUnitInfo.m_ucLevel;
	SendToLogDB( DBE_CHARACTER_LEVELUP_NOT, kNot );

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	CTime tCurTime = CTime::GetCurrentTime();
	KELOG_USER_CHARACTER_LOG_NOT kPacketToLog;
	kPacketToLog.m_iUserUID				 = GetUID();
	kPacketToLog.m_iUnitUID				 = kPacket_.m_kUnitInfo.m_nUnitUID;
	kPacketToLog.m_wstrNickName 		 = kPacket_.m_kUnitInfo.m_wstrNickName;
	kPacketToLog.m_bOnlyOneUnit 		 = ( m_setMyUnitUIDList.size() == 1 );
	kPacketToLog.m_iServerUID			 = KBaseServer::GetKObj()->GetUID();
	kPacketToLog.m_sLogType				 = KELOG_USER_CHARACTER_LOG_NOT::LT_CREATE_CHARACTER;
	kPacketToLog.m_ucLevel				 = kPacket_.m_kUnitInfo.m_ucLevel;
	kPacketToLog.m_cUnitClass			 = kPacket_.m_kUnitInfo.m_cUnitClass;
	kPacketToLog.m_wstrRegDate		 	 = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kPacketToLog.m_wstrUnitCreateDate	 = kPacketToLog.m_wstrRegDate;
	kPacketToLog.m_wstrAccountCreateDate = m_kUserStatistics.GetAccountRegDate();
	SendToLogDB( ELOG_USER_CHARACTER_LOG_NOT, kPacketToLog );
#endif SERV_CHAR_LOG
	//}}

	//{{ 2011. 02. 28	������	�系 ���� ���
#ifdef SERV_CHAR_CREATE_DEL_STAT
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_CHARACTER, kKey, KStatistics::eSIColDB_Char_Create_Count, 1 );
	}
#endif SERV_CHAR_CREATE_DEL_STAT
	//}}
}

IMPL_ON_FUNC( EGS_DELETE_UNIT_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_DELETE_UNIT_REQ, EGS_DELETE_UNIT_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    
	
	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	if( IsUseSecurity() == true )
	{
		if( GetSuccessSecondSecurity() == false )	// 2�� ������ ��� ���ε� ������ ���� �ʾҴٸ� ����
		{
			START_LOG( cerr, L"2�� ������ ��� ���ε� ������ �Ǳ� ���� ���� ������ �Ϸ� �Ѵ�." )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			KEGS_DELETE_UNIT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_21;
			kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
			kPacket.m_iGuildUID = 0;	
			SendPacket( EGS_DELETE_UNIT_ACK, kPacket );
			return;
		}
	}
#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( m_kUserRelationshipManager.GetRelationshipType() != SEnum::RT_SOLO )
	{
		KEGS_DELETE_UNIT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_DELETE_UNIT_06;
		SendPacket( EGS_DELETE_UNIT_ACK, kPacket );
		return;
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	//{{ 2012. 02. 21	��μ�	ĳ���� ���� �� ��� Ż�� ����ó�� ����
#ifdef SERV_UNIT_DELETE_EXCEPTION_MODIFY
	kPacket_.m_iUserUID = GetUID();
#endif SERV_UNIT_DELETE_EXCEPTION_MODIFY
	//}}

    SendToGameDB( DBE_GAME_DELETE_UNIT_REQ, kPacket_ );

	m_kUserTutorialInfo.OnDeleteUnitReq( kPacket_.m_iUnitUID );
}

_IMPL_ON_FUNC( DBE_GAME_DELETE_UNIT_ACK, KEGS_DELETE_UNIT_ACK )
{
    VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		SendPacket( EGS_DELETE_UNIT_ACK, kPacket_ );
		return;
	}

#ifdef SERV_UNIT_WAIT_DELETE
	//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
	if( kPacket_.m_iGuildUID > 0 )
	{
		KDBE_KICK_GUILD_MEMBER_ACK kPacket;
		kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
		kPacket.m_iTargetUnitUID = kPacket_.m_iUnitUID;
		SendToLoginServer( ELG_UPDATE_KICK_GUILD_MEMBER_NOT, kPacket );
	}
#endif GUILD_TEST
	//}}

	//{{ 2010. 02. 02  ������	��� �Խ���
#ifdef SERV_GUILD_AD
	// ��� �Ҽ��� �ƴ϶�� ��� ���� ��û�� �ߴ��� Ȯ���Ϸ� ����!
	if( kPacket_.m_iGuildUID <= 0 )
	{
		KELG_DELETE_APPLY_JOIN_GUILD_REQ kPacketToLg;
		kPacketToLg.m_iDeletedUnitUID = kPacket_.m_iUnitUID;
		kPacketToLg.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacketToLg.m_iGuildUID = 0;
		kPacketToLg.m_bDeleteChar = true;
		SendToLoginServer( ELG_DELETE_APPLY_JOIN_GUILD_REQ, kPacketToLg );
	}
#endif SERV_GUILD_AD

#endif SERV_UNIT_WAIT_DELETE
	//}}

	//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
	SendUpdateUnitInfoToRelayDB( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT::UT_NONE, true, kPacket_.m_iUnitUID );
#endif SERV_RELAY_DB_CONNECTION
	//}}

#ifdef SERV_UNIT_WAIT_DELETE
	SendPacket( EGS_DELETE_UNIT_ACK, kPacket_ );
#else //SERV_UNIT_WAIT_DELETE
	SendToAccountDB( DBE_ACCOUNT_DELETE_UNIT_REQ, kPacket_ );
#endif //SERV_UNIT_WAIT_DELETE
}

#ifdef SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_ACK, KEGS_FINAL_DELETE_UNIT_ACK  )
#else //SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_ACK, KEGS_DELETE_UNIT_ACK )
#endif //SERV_UNIT_WAIT_DELETE
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	//{{ 2008. 1. 23  ������  ���� ���� ���DB �뺸
	if( kPacket_.m_iOK == NetError::NET_OK	&&  ( GetAuthLevel() == SEnum::UAL_NORMAL || GetAuthLevel() == SEnum::UAL_BLOCKED_1 ) )
	{
		KEGS_DELETE_UNIT_REQ kPacketLog;
		kPacketLog.m_iUnitUID = kPacket_.m_iUnitUID;
		SendToLogDB( DBE_DELETE_UNIT_STAT_NOT, kPacketLog );
	}
	//}}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		// �����ϰ� �ִ� ĳ���� ����Ʈ���� ����
		m_setMyUnitUIDList.erase( kPacket_.m_iUnitUID );

		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_USER_CHARACTER_LOG_NOT kPacketToLog;
		kPacketToLog.m_iUserUID				= GetUID();
		kPacketToLog.m_iUnitUID				= kPacket_.m_iUnitUID;
		kPacketToLog.m_wstrNickName			= kPacket_.m_wstrNickName;
		kPacketToLog.m_bOnlyOneUnit 		= ( m_setMyUnitUIDList.size() == 1 );
		kPacketToLog.m_iServerUID			= KBaseServer::GetKObj()->GetUID();
		kPacketToLog.m_sLogType				= KELOG_USER_CHARACTER_LOG_NOT::LT_DELETE_CHARACTER;
		kPacketToLog.m_ucLevel				= kPacket_.m_ucLevel;
		kPacketToLog.m_cUnitClass			= kPacket_.m_cUnitClass;
		kPacketToLog.m_wstrRegDate		 	= ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		kPacketToLog.m_wstrAccountCreateDate = m_kUserStatistics.GetAccountRegDate();
		SendToLogDB( ELOG_USER_CHARACTER_LOG_NOT, kPacketToLog );
#endif SERV_CHAR_LOG
		//}}

		m_kUserTutorialInfo.OnDeleteUnitAck( GetThisPtr<KGSUser>() );

		//{{ 2009. 10. 8  ������	���
#ifndef SERV_UNIT_WAIT_DELETE
#ifdef GUILD_TEST
		if( kPacket_.m_iGuildUID > 0 )
		{
			KDBE_KICK_GUILD_MEMBER_ACK kPacket;
			kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
			kPacket.m_iTargetUnitUID = kPacket_.m_iUnitUID;
			SendToLoginServer( ELG_UPDATE_KICK_GUILD_MEMBER_NOT, kPacket );
		}
#endif GUILD_TEST
#endif SERV_UNIT_WAIT_DELETE
		//}}

		//{{ 2009. 10. 27  ������	��ŷ
		KELG_FIND_DELETE_CHAR_IN_RANKING_NOT kPacketToLg;
		SiKGSRankingManager()->IsInRankingUser( GetCharUID(), kPacketToLg.m_vecRankingType );
		if( !kPacketToLg.m_vecRankingType.empty() )
		{
			kPacketToLg.m_iUnitUID = GetCharUID();
			SendToLoginServer( ELG_FIND_DELETE_CHAR_IN_RANKING_NOT, kPacketToLg );
		}
		//}}

		//{{ 2010. 02. 02  ������	��� �Խ���
#ifndef SERV_UNIT_WAIT_DELETE
#ifdef SERV_GUILD_AD
		// ��� �Ҽ��� �ƴ϶�� ��� ���� ��û�� �ߴ��� Ȯ���Ϸ� ����!
		if( kPacket_.m_iGuildUID <= 0 )
		{
			KELG_DELETE_APPLY_JOIN_GUILD_REQ kPacketToLg;
			kPacketToLg.m_iDeletedUnitUID = kPacket_.m_iUnitUID;
			kPacketToLg.m_iUnitUID = kPacket_.m_iUnitUID;
			kPacketToLg.m_iGuildUID = 0;
			kPacketToLg.m_bDeleteChar = true;
            SendToLoginServer( ELG_DELETE_APPLY_JOIN_GUILD_REQ, kPacketToLg );
		}
#endif SERV_GUILD_AD
#endif SERV_UNIT_WAIT_DELETE
		//}}

		//{{ 2011. 02. 28	������	�系 ���� ���
#ifdef SERV_CHAR_CREATE_DEL_STAT
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( 0 );
			KSIManager.IncreaseCount( KStatistics::SI_CHARACTER, kKey, KStatistics::eSIColDB_Char_Delete_Count, 1 );
		}
#endif SERV_CHAR_CREATE_DEL_STAT
		//}}
	}
#ifdef SERV_UNIT_WAIT_DELETE
	SendPacket( EGS_FINAL_DELETE_UNIT_ACK, kPacket_ );
#else
	SendPacket( EGS_DELETE_UNIT_ACK, kPacket_ );
#endif SERV_UNIT_WAIT_DELETE
}

IMPL_ON_FUNC( EGS_SELECT_UNIT_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_WARN_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_SELECT_UNIT_REQ, EGS_SELECT_UNIT_ACK );
#else
	VERIFY_STATE_ACK_WARN( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_SELECT_UNIT_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	if( IsUseSecurity() == true)
	{
		if( GetSuccessSecondSecurity() == false )	// 2�� ������ ��� ���ε� ������ ���� �ʾҴٸ� ����
		{
			START_LOG( cerr, L"2�� ������ ��� ���ε� ������ �Ǳ� ���� ���� ������ �Ϸ� �Ѵ�." )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			KEGS_SELECT_UNIT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_21;
			SendPacket( EGS_SELECT_UNIT_ACK, kPacket );

			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
			kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SECOND_SECURITY_NOT_AUTH;			
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kNot );

			m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SECOND_SECURITY_NOT_AUTH );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
			kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_SECOND_SECURITY_NOT_AUTH;			
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kNot );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
			return;
		}
	}
#endif SERV_SECOND_SECURITY
	//}}

#ifdef SERV_NEW_YEAR_EVENT_2014
	IF_EVENT_ENABLED( CEI_OLD_YEAR_EVENT_2013 )
	{
		kPacket_.m_setCodeEventScriptID.insert( CEI_OLD_YEAR_EVENT_2013 );
	}
	IF_EVENT_ENABLED( CEI_NEW_YEAR_EVENT_2014 )
	{
		kPacket_.m_setCodeEventScriptID.insert( CEI_NEW_YEAR_EVENT_2014 );
	}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	IF_EVENT_ENABLED( CEI_GATE_OF_DARKNESS_SUPPORT_EVENT )
	{
		kPacket_.m_setCodeEventScriptID.insert( CEI_GATE_OF_DARKNESS_SUPPORT_EVENT );
	}
#endif //SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_EVENT_CHECK_POWER
	IF_EVENT_ENABLED( CEI_CHECK_POWER )
	{
		kPacket_.m_setCodeEventScriptID.insert( CEI_CHECK_POWER );
	}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_GLOBAL_EVENT_TABLE
	kPacket_.m_mapGlobalEventData.clear();
	SiKGameEventManager()->CheckEnableCodeAndSetGlobalEventdata( kPacket_.m_mapGlobalEventData );
#endif //SERV_GLOBAL_EVENT_TABLE

	SendToGameDB( DBE_GAME_SELECT_UNIT_REQ, kPacket_ );
}

//{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
_IMPL_ON_FUNC( DBE_GAME_SELECT_UNIT_ACK, KDBE_SELECT_UNIT_ACK )
#else
_IMPL_ON_FUNC( DBE_GAME_SELECT_UNIT_ACK, KEGS_SELECT_UNIT_ACK )
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}}
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
    
    if( kPacket_.m_iOK != NetError::NET_OK )
    {
		//{{ 2012. 09. 26	������		���� ���� ���� ����
#ifdef SERV_POST_COPY_BUG_FIX
		if( kPacket_.m_iOK == NetError::ERR_SELECT_UNIT_07 )
		{
			START_LOG( cout, L"UserUID�Ҽ��� �ƴ� UnitUID�� ĳ���͸� ������ ������ ��Ÿ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetUserID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			KDBE_SEND_PHONE_MSG_NOT kPacketNot;
			kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"�����Ҽ� �ƴ� ĳ���ͼ���:UserUID(%d),UnitUID(%d) " ) // ,ServerIP(%s)" ) // �ؿ��� �ּ� ó��
                % GetUID() 
                % kPacket_.m_nWrongUnitUID
                //% KBaseServer::GetKObj()->GetPublicIP() // �ؿ��� �ּ� ó��
                );
			//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
			SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_INCORRECT_USER_UNIT, kPacketNot.m_vecPhoneNum );
#else
			SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::PNLT_ELSWORD_DEV_TEAM, kPacketNot.m_vecPhoneNum );
#endif SERV_SMS_TOTAL_MANAGER
			//}}
			if( kPacketNot.m_vecPhoneNum.empty() == false )
			{
				SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
			}

			// �ڵ����� ����� ���
			m_kUserAbuserManager.RegEDAbuser( GetThisPtr<KGSUser>() );
			m_kUserAbuserManager.RegItemAbuser( GetThisPtr<KGSUser>() );
			//HackUserRegPacketMornitor();
		}
#endif SERV_POST_COPY_BUG_FIX
		//}}

		//{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
		KEGS_SELECT_UNIT_ACK kACK;
		kACK.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_SELECT_UNIT_ACK, kACK );

        LOG_SUCCESS( kACK.m_iOK == NetError::NET_OK )
            << BUILD_LOG( GetUID() )
            << BUILD_LOG( kACK.m_iOK );

#else
		SendPacket( EGS_SELECT_UNIT_ACK, kPacket_ );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
		//}}
        return;
    }

    // 2007.12. 28. jseop. �г����� �߸��Ǿ� �����ؾ� �ϴ� ���.
    if( kPacket_.m_kUnitInfo.m_wstrNickName.compare( L"__DELETED__" ) == 0 )
    {
        KEGS_SELECT_UNIT_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_SELECT_UNIT_06;
        SendPacket( EGS_SELECT_UNIT_ACK, kPacket );
        return;
    }

#ifdef SERV_CUSTOM_CONNECT_EVENT
	SetCustonEventID( kPacket_.m_iCustomEventID );
	START_LOG( clog, L"[Ư�������̺�Ʈ] �̺�Ʈ ���ǰ� ����")
		<< BUILD_LOG( GetCustonEventID() )
		<< BUILD_LOG( GetCharName() )
		<< END_LOG;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	switch(kPacket_.m_iNewUnitE)
	{
	case 0:						// ���� ����
		SetNewUnit(true);
		SetNewUnit2(false);
		SetCurrentUnit(false);
		break;
	case 1:						// ���� ������ �ű� ���� �Ⱓ ���̿� �� ���̶� ������ ���� ����
		SetNewUnit(false);
		SetNewUnit2(true);
		SetCurrentUnit(false);
		break;
	case 2:						// �ű� ����
		SetNewUnit(false);
		SetNewUnit2(false);
		SetCurrentUnit(true);
		break;
	default:
		SetNewUnit(false);
		SetNewUnit2(false);
		SetCurrentUnit(true);
		break;
	}
	
	//START_LOG( cerr, L"�輮��_�ű� ���� ������ �����?" )
	//	<< BUILD_LOG( IsNewUnit() )
	//	<< BUILD_LOG( IsNewUnit2() )
	//	<< BUILD_LOG( IsCurrentUnit() )
	//	<< END_LOG;
	
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
	// ���� ���� 1�̸� ���Ӱ��� ����. 0�̸� ���Ӱ��� ����.
	SetConnectExperience(kPacket_.m_iConnectExperienceAck);
	// ���� ���󿩺μ��� 1�̸� ��������, 0�̸� ���� ����.
	//SetRewardBonusItem(kPacket_.m_iReward7DaysItem);

	START_LOG( clog, L"7�� ���� ���� ���� ���ÿϷ�" )
		<< BUILD_LOG( GetConnectExperience() )
		<< END_LOG;

	// �������� 7�� ���� ���� �߿���, ���� ������ ���� ���� ������ �����
	
	if(GetConnectExperience() == 1)
	{
		KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ kPacketToDB7DaysEVENT;
		//KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ kPacketToDB7DaysEVENT;
		kPacketToDB7DaysEVENT.m_iUnitUID = kPacket_.m_kUnitInfo.m_nUnitUID;
		kPacketToDB7DaysEVENT.m_iUserUID = GetUID();
		kPacketToDB7DaysEVENT.m_iUnitLevel = static_cast<int>(kPacket_.m_kUnitInfo.m_ucLevel);
		kPacketToDB7DaysEVENT.m_iGetConnectExperience = GetConnectExperience();
		kPacketToDB7DaysEVENT.m_iRewardBonusItem = GetRewardBonusItem();
		SendToGameDB( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ, kPacketToDB7DaysEVENT );

		START_LOG( clog, L"7�� ���� ���� ���� ��Ŷ ���� ����!" )
			<< BUILD_LOG( kPacketToDB7DaysEVENT.m_iGetConnectExperience )
			<< BUILD_LOG( kPacketToDB7DaysEVENT.m_iUnitUID)
			<< BUILD_LOG( kPacketToDB7DaysEVENT.m_iUserUID )
			<< BUILD_LOG( kPacketToDB7DaysEVENT.m_iUnitLevel )
			<< END_LOG;
	}
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
	m_kUserLocalRankingInfo.GetUserInfo( kPacket_.m_kLocalRankingUserInfo );
	m_kUserLocalRankingInfo.GetLastUpdateDate( kPacket_.m_wstrLocalRankingUserInfoUpdated );
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-08	// �ڼ���
	// ���� �ʵ��� ������ �̵� ��Ű��
	if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kUnitInfo.m_kLastPos.m_iMapID ) == true )
	{
		kPacket_.m_kUnitInfo.m_kLastPos.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( kPacket_.m_kUnitInfo.m_ucLevel, kPacket_.m_kUnitInfo.m_mapDungeonClear );
		kPacket_.m_kUnitInfo.m_kLastPos.m_bIgnoreLastTouch = true;
	}
	// ���� �����̳�, ä�� ���濡�� �����ϰ� ó���Ǵ� ������
	// �Լ��� ó���ؼ� �������� ó���ǵ��� �ϴ°� ���� �� �ϴ�...
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GLOBAL_EVENT_TABLE
	m_mapGlobalEventData = kPacket_.m_mapGlobalEventData;
	GetGlobalEventTableData(m_mapGlobalEventData);
#ifdef SERV_4TH_ANNIVERSARY_EVENT
	m_bGetEventRewardInfo = true;
#endif // SERV_4TH_ANNIVERSARY_EVENT
#endif //SERV_GLOBAL_EVENT_TABLE

	SendToAccountDB( DBE_ACCOUNT_SELECT_UNIT_REQ, kPacket_ );
}

//{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
_IMPL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_ACK, KDBE_SELECT_UNIT_ACK )
#else
_IMPL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_ACK, KEGS_SELECT_UNIT_ACK )
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}}
{
    VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
    {
		//{{ 2009. 5. 13  ������	P2P��Ʈ üũ
		if( GetP2PIP().empty() )
		{
			START_LOG( cerr, L"P2P��Ʈ üũ ����!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
		}
		//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		CTime tReleaseTradeBlockTime;
		LIF( KncUtil::ConvertStringToCTime( kPacket_.m_wstrUnitCreateDate, tReleaseTradeBlockTime ) );
		tReleaseTradeBlockTime += CTimeSpan( SiKGameSysVal()->GetNewUnitTradeBlockDay(), 0, 0, 0 );
		CTime tCurTime = CTime::GetCurrentTime();

		if( SiKGameSysVal()->IsNewUnitTradeBlock() == true
			&& ( ( SiKGameSysVal()->GetNewUnitTradeBlockDay() != 0 && tCurTime < tReleaseTradeBlockTime )
			|| ( SiKGameSysVal()->GetNewUnitTradeBlockUnitClass() != 0 && kPacket_.m_kUnitInfo.m_cUnitClass < SiKGameSysVal()->GetNewUnitTradeBlockUnitClass() ) )
			)
		{
			m_tReleaseTradeBlockTime = tReleaseTradeBlockTime;

			m_kUserAbuserManager.SetTradeBlockUnit( true );
		}
		else
		{
			m_kUserAbuserManager.SetTradeBlockUnit( false );
		}
#endif SERV_NEW_UNIT_TRADE_LIMIT

		// ä�� �̵� ���� �˻�
		if( SiKChannelManager()->IsExistChannelChangeUser( kPacket_.m_kUnitInfo.m_nUnitUID ) == true )
		{
			// �ش� ������ ������ �� �ֵ��� �������� ����
			SiKChannelManager()->DelChannelChangeUser( kPacket_.m_kUnitInfo.m_nUnitUID );
		}


		//////////////////////////////////////////////////////////////////////////
		// ĳ���� ���ÿ� ���õ� ��� ó���� �� �Լ� �ȿ��� �ϵ��� �Ѵ�!
		if( OnAccountSelectUnitAck( kPacket_, false ) == false )
		{
			START_LOG( cerr, L"select unit error" )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
				<< END_LOG;

			goto end_proc;
		}
		//////////////////////////////////////////////////////////////////////////

		//{{ 2012. 09. 12	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		GetPvpRankForClient( kPacket_.m_kUnitInfo.m_iOfficialMatchCnt, kPacket_.m_kUnitInfo.m_cRank );
#endif SERV_2012_PVP_SEASON2
		//}}
		
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
		SetEventMoney( kPacket_.m_iEventMoney );
#endif // SERV_EVENT_MONEY

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
		m_kUserLocalRankingInfo.InitUserInfo( kPacket_.m_kLocalRankingUserInfo, kPacket_.m_wstrLocalRankingUserInfoUpdated );
#endif	// SERV_LOCAL_RANKING_SYSTEM
	}

end_proc:
#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-11	// �ڼ���
	if( JumpingCharacterCheck() == NetError::NET_OK )
	{
		SendID( EGS_JUMPING_CHARACTER_NOT );
	}
#endif // SERV_JUMPING_CHARACTER

	//{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	SendSelectUnitACK( kPacket_ );
#else
	SendPacket( EGS_SELECT_UNIT_ACK, kPacket_ );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	//}}

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
		//////////////////////////////////////////////////////////////////////////
		// ĳ���� ���� ������ ���Ŀ� ó���ϴ� �͵��� ��� �� �Լ� �ȿ��� ó���ϵ��� �Ѵ�!
		OnSelectUnitSuccess( kPacket_, false );
		//////////////////////////////////////////////////////////////////////////
    }
}

//{{ 2010. 01. 04  ������	�κ��丮 ����
IMPL_ON_FUNC_NOPARAM( EGS_GET_MY_INVENTORY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	std::vector< KEGS_SELECT_UNIT_INVENTORY_INFO_NOT > vecInventoryInfo;

	// ���Ժ��� �κ��丮 ���� ���
	m_kInventory.GetSelectUnitInventoryInfo( vecInventoryInfo );

	for( u_int ui = 0; ui < vecInventoryInfo.size(); ++ui )
	{
		SendPacket( EGS_SELECT_UNIT_INVENTORY_INFO_NOT, vecInventoryInfo[ui] );
	}

	START_LOG( clog, L"�κ��丮 ���� ���� ����" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( vecInventoryInfo.size() );

    KEGS_GET_MY_INVENTORY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2010. 01. 08  ������	��õ�θ���Ʈ	
	kPacket.m_bIsRecommend = m_kUserRecommendManager.IsRecommend();
	//}}
	
	SendPacket( EGS_GET_MY_INVENTORY_ACK, kPacket );
}
//}}

_IMPL_ON_FUNC( EGS_ENTER_EQUIP_SHOP_REQ, KEGS_ENTER_NPC_HOUSE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KPacketOK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ENTER_EQUIP_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ENTER_NPC_HOUSE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KPacketOK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ENTER_NPC_HOUSE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_CHAT_REQ )
{
	KEGS_CHAT_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		kPacket_.m_wstrMsg = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrMsg.c_str(), L'��');
	}
#endif //SERV_STRING_FILTER_USING_DB

	if( kPacket_.m_ToUnitUID == GetCharUID() )
	{
		kPacket.m_iOK = NetError::ERR_CHAT_02;
		SendPacket( EGS_CHAT_ACK, kPacket );
		return;
	}

#ifdef SERV_NEW_PUNISHMENT
	if( m_PunishmentManager.IsAppliedPunishment(1) )
	{
		// �ӽ� ä�� ����
		kPacket.m_iOK = NetError::ERR_CHAT_04;		// �ӽ� �޼���. ���� ����
		SendPacket( EGS_CHAT_ACK, kPacket );

		KEGS_CHAT_NOT kPacketNot;
		SendPacket( EGS_CHAT_NOT, kPacketNot );
		return;
	}

	if( m_PunishmentManager.IsAppliedPunishment(6) )
	{
		// �ϴ� ok�� ������ ������ ó������ �ʴ´�
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_CHAT_ACK, kPacket );

		return;
	}
#endif SERV_NEW_PUNISHMENT

#ifdef SERV_CHATTING_OBSERVATION_CN
	switch( kPacket_.m_cChatPacketType )
	{
	case KEGS_CHAT_REQ::CPT_TOTAL:
	case KEGS_CHAT_REQ::CPT_PARTY:
		//	case KEGS_CHAT_REQ::CPT_SYSTEM:
	case KEGS_CHAT_REQ::CPT_GUILD:
		//	case KEGS_CHAT_REQ::CPT_WHISPER:
		//	case KEGS_CHAT_REQ::CPT_MESSENGER:
	case KEGS_CHAT_REQ::CPT_MEGAPHONE:
		{
			KELG_CHAT_OBSERVATION_NOT kNot;
			kNot.m_iServerUID = GetUID();
			kNot.m_cChatPacketType = kPacket_.m_cChatPacketType;
			kNot.m_wstrSenderUnitNickName = GetCharName();
			kNot.m_wstrMsg = kPacket_.m_wstrMsg;

			SendToLoginServer(ELG_CHAT_OBSERVATION_NOT, kNot);
		}
		break;
	}
#endif //SERV_CHATTING_OBSERVATION_CN

	switch( kPacket_.m_cChatPacketType )
	{
	case KEGS_CHAT_REQ::CPT_TOTAL:
		{
			//{{ 2008. 12. 13  ������	ä��
			if( GetStateID() == KGSFSM::S_ROOM )
			{
				SendToCnRoom( ERM_CHAT_REQ, kPacket_ );

				//{{ 2010. 11. 12  ��ȿ��	GameForge ä�� �α� ���� ó��
#ifdef SERV_RECORD_CHAT
				if( SiKGameSysVal()->IsRecordChat() && kPacket_.m_cRoomChatType == CXSLRoom::ROOM_CHAT_TYPE::RCT_ALL )
				{
					KDCHATLOG_RECORD_CHAT_NOT kPacket_chat;
					kPacket_chat.m_iAccountUID = GetUID();
					kPacket_chat.m_iUnitUID = GetCharUID();
					kPacket_chat.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
					kPacket_chat.m_wstrChatText = kPacket_.m_wstrMsg + L"_1";
					kPacket_chat.m_iChatType = KEGS_CHAT_REQ::CPT_TOTAL;

					SendToChatLogDB( DCHATLOG_RECORD_CHAT_NOT, kPacket_chat );
				}
#endif SERV_RECORD_CHAT
				//}}
			}
			else
			{
				if( GetFieldUID() > 0 )
				{
					SendToGSField( ERM_CHAT_REQ, kPacket_ );

					//{{ 2010. 11. 12  ��ȿ��	GameForge ä�� �α� ���� ó��
#ifdef SERV_RECORD_CHAT
					if( SiKGameSysVal()->IsRecordChat() && kPacket_.m_cRoomChatType == CXSLRoom::ROOM_CHAT_TYPE::RCT_ALL )
					{
						KDCHATLOG_RECORD_CHAT_NOT kPacket_chat;
						kPacket_chat.m_iAccountUID = GetUID();
						kPacket_chat.m_iUnitUID = GetCharUID();
						kPacket_chat.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
						kPacket_chat.m_wstrChatText = kPacket_.m_wstrMsg + L"_2";
						kPacket_chat.m_iChatType = KEGS_CHAT_REQ::CPT_TOTAL;

						SendToChatLogDB( DCHATLOG_RECORD_CHAT_NOT, kPacket_chat );
					}
#endif SERV_RECORD_CHAT
					//}}
				}
				else
				{
					START_LOG( cwarn, L"�ʵ� �����ε� fieldUID�� 0�ΰ��?" )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					// �ϴ� ok�� ������.
					kPacket.m_iOK = NetError::NET_OK;
					SendPacket( EGS_CHAT_ACK, kPacket );
				}
			}
			//}}
			return;
		}
		break;

	case KEGS_CHAT_REQ::CPT_PARTY:
		{
			VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

			if( GetPartyUID() == 0 )
			{
				START_LOG( cerr, L"��Ƽ UID �̻�.!" )
					<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_09;
				SendPacket( EGS_CHAT_ACK, kPacket );
				return;
			}

			//{{ 2010. 02. 23  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
			if( SiKPartyListManager()->E_Chat( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
#pragma ELSE_MESSAGE
			//if( SiKPartyManager()->E_Chat( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
			//}}
			{
				START_LOG( cwarn, L"��Ƽä�ý���." )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetPartyUID() )
					<< END_LOG;
			}
			return;
		}
		break;

	case KEGS_CHAT_REQ::CPT_WHISPER:
		{
			VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

			KELG_CHAT_REQ kReq;
			kReq.m_cChatPacketType			= kPacket_.m_cChatPacketType;			
			kReq.m_iSenderUnitUID			= GetCharUID();
			kReq.m_wstrSenderUnitNickName	= GetCharName();
			kReq.m_wstrRecieverUnitNickName	= kPacket_.m_wstrRecieverUnitNickName;
			kReq.m_wstrMsg					= kPacket_.m_wstrMsg;

			SendToLoginServer( ELG_CHAT_REQ, kReq );
			return;
		}
		break;

    case KEGS_CHAT_REQ::CPT_MESSENGER:
        {
            KELG_CHAT_REQ kReq;
            kReq.m_cChatPacketType			= kPacket_.m_cChatPacketType;			
            kReq.m_iSenderUnitUID			= GetCharUID();
            kReq.m_wstrSenderUnitNickName	= GetCharName();
            kReq.m_wstrRecieverUnitNickName	= kPacket_.m_wstrRecieverUnitNickName;
            kReq.m_wstrMsg					= kPacket_.m_wstrMsg;

            SendToLoginServer( ELG_CHAT_REQ, kReq );
            return;
        }
        break;

		//{{ 2009. 9. 18  ������	���
#ifdef GUILD_TEST
	case KEGS_CHAT_REQ::CPT_GUILD:
		{
			VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

			if( GetGuildUID() <= 0 )
			{
				START_LOG( cerr, L"������� �ƴѵ� ���ä���� ������." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetGuildUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_CHAT_03;
				SendPacket( EGS_CHAT_ACK, kPacket );
				return;
			}

			KELG_CHAT_REQ kReq;
			kReq.m_cChatPacketType			= kPacket_.m_cChatPacketType;
			kReq.m_iChatData				= GetGuildUID();
			kReq.m_iSenderUnitUID			= GetCharUID();
			kReq.m_wstrSenderUnitNickName	= GetCharName();
			kReq.m_wstrMsg					= kPacket_.m_wstrMsg;

			SendToLoginServer( ELG_CHAT_REQ, kReq );
			return;
		}
		break;
#endif GUILD_TEST
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	case KEGS_CHAT_REQ::CPT_RELATIONSHIP:
		{
			VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

			KELG_CHAT_REQ kReq;
			kReq.m_cChatPacketType			= kPacket_.m_cChatPacketType;			
			kReq.m_iSenderUnitUID			= GetCharUID();
			kReq.m_wstrSenderUnitNickName	= GetCharName();
			kReq.m_wstrRecieverUnitNickName	= kPacket_.m_wstrRecieverUnitNickName;
			kReq.m_wstrMsg					= kPacket_.m_wstrMsg;

			SendToLoginServer( ELG_CHAT_REQ, kReq );
			return;
		}
		break;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	default:
		{
			START_LOG( cerr, L"�̻��� ä�� ��Ŷ Ÿ��." )
				<< BUILD_LOGc( kPacket_.m_cChatPacketType )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_CHAT_03;
			SendPacket( EGS_CHAT_ACK, kPacket );
			return;
		}
		break;
	}
}

IMPL_ON_FUNC( ERM_CHAT_ACK )
{
	switch( kPacket_.m_cChatPacketType )
	{
	case KEGS_CHAT_REQ::CPT_TOTAL:	
		{
			VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
		}
		break;
	default:
		{
			START_LOG( cerr, L"�̻��� ä�� ��Ŷ Ÿ��." )
				<< BUILD_LOGc( kPacket_.m_cChatPacketType )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		break;
	}

	KEGS_CHAT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CHAT_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_CHAT_NOT, KEGS_CHAT_NOT )
{
	switch( kPacket_.m_cChatPacketType )
	{
	case KEGS_CHAT_REQ::CPT_TOTAL:
		{
			VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
		}
		break;
	default:
		{
			START_LOG( cerr, L"�̻��� ä�� ��Ŷ Ÿ��." )
				<< BUILD_LOGc( kPacket_.m_cChatPacketType )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		break;
	}

	// �� ����Ʈ ä�� ����
	if( m_kUserGameOption.IsChatBlackListUnit( kPacket_.m_SenderUnitUID ) == true )
		return;

	SendPacket( EGS_CHAT_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_CHAT_ACK )
{
	VERIFY_STATE( ( 2,	KGSFSM::S_FIELD_MAP,
						KGSFSM::S_ROOM  ) );

	KEGS_CHAT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_CHAT_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CHAT_NOT )
{
	VERIFY_STATE( ( 2,	KGSFSM::S_FIELD_MAP,
						KGSFSM::S_ROOM  ) );

	// �� ����Ʈ ä�� ����
	if( m_kUserGameOption.IsChatBlackListUnit( kPacket_.m_iSenderUnitUID ) == true )	
		return;

	KEGS_CHAT_NOT kPacket;

	kPacket.m_cChatPacketType			= kPacket_.m_cChatPacketType;
	kPacket.m_SenderUnitUID				= kPacket_.m_iSenderUnitUID;
	kPacket.m_wstrSenderUnitNickName	= kPacket_.m_wstrSenderUnitNickName;
	kPacket.m_wstrRecieverUnitNickName	= kPacket_.m_wstrRecieverUnitNickName;
	kPacket.m_RecieverUnitUID			= kPacket_.m_iReceiverUnitUID;
	kPacket.m_cRoomChatType				= 0;
	kPacket.m_wstrMsg					= kPacket_.m_wstrMsg;


	SendPacket( EGS_CHAT_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ )
{
    // TODO : ���� ���� �ؾ� �Ѵ�.

    SendToAccountDB( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EGS_CHANGE_OPTION_PLAY_GUIDE_ACK, KPacketOK )
{
    // TODO : ���� ���� �ؾ� �Ѵ�.

    SendPacket( EGS_CHANGE_OPTION_PLAY_GUIDE_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_NEW_QUEST_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_NEW_QUEST_REQ, EGS_NEW_QUEST_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		KEGS_NEW_QUEST_ACK kAck;
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_NEW_QUEST_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	//���� üũ�� Handler_EGS_NEW_QUEST_REQ()�ȿ��� �Ѵ�.
	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	m_kUserQuestManager.Handler_EGS_NEW_QUEST_REQ( kPacket_, GetThisPtr<KGSUser>(), false );
#else
	m_kUserQuestManager.Handler_EGS_NEW_QUEST_REQ( kPacket_, GetThisPtr<KGSUser>() );
#endif SERV_REFORM_QUEST
	//}}
}

IMPL_ON_FUNC( DBE_NEW_QUEST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

#ifdef SERV_EPIC_QUEST
	// �ڵ����� ����Ʈ �̸� ACK�� ������ �ʴ´�.(����� ��������Ʈ��..)
	if( SiCXSLQuestManager()->IsEpicQuest(kPacket_.m_iQuestID) == false )
#endif SERV_EPIC_QUEST
	{
		KEGS_NEW_QUEST_ACK kAck;
		kAck.m_iOK	= kPacket_.m_iOK;		
		SendPacket( EGS_NEW_QUEST_ACK, kAck );
	}


	//DB���� ����ó���ǰ� �� ���..
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KQuestInstance kQuest;
		kQuest.m_iID			= kPacket_.m_iQuestID;
		kQuest.m_OwnorUnitUID	= GetCharUID();

		const CXSLQuestManager::QuestTemplet* QuestTem = SiCXSLQuestManager()->GetQuestTemplet( kPacket_.m_iQuestID );
		if( QuestTem == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kPacket_.m_iQuestID )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
			return;
		}

		KSubQuestInstance kSubQuest;
		BOOST_TEST_FOREACH( const int, iSubQuestID, QuestTem->m_vecSubQuest )
		{
			kSubQuest.m_iID = iSubQuestID;
			
			//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
			// ������ ���� ���� �÷��� ī��Ʈ�� ǥ�õǾ�� �ϱ� ������,
			// �̰����� ���� ó���� ���� ��������� �÷��� Ƚ���� ���� �־��ش�.
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< END_LOG;
				continue;
			}

			if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_PVP_PLAY_ARRANGE )	//Ŭ���� ������ ������..
			{
				// pvp info ���̺��� ������ ���� ����⸸ �Ѵ�.
				kSubQuest.m_ucClearData = GetOfficialMatchCount();
				kSubQuest.m_bIsSuccess = ( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay <= kSubQuest.m_ucClearData );
			}
#endif SERV_2012_PVP_SEASON2
			//}}

			kQuest.m_vecSubQuestInstance.push_back( kSubQuest );
		}

		m_kUserQuestManager.AddQuest( kQuest );

		KEGS_NEW_QUEST_NOT kNot;
		kNot.m_QuestInst = kQuest;
		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kNot.m_vecUpdatedInventorySlot );
		kNot.m_vecUpdatedInventorySlot.insert( kNot.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		SendPacket( EGS_NEW_QUEST_NOT, kNot );

		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}

		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		BOOST_TEST_FOREACH( const int, iSubQuestID, QuestTem->m_vecSubQuest )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
			if( IS_NULL( pSubQuestTemplet ) )
			{
				START_LOG( cerr, L"���� ���� �ʴ� ���� ����Ʈ �Դϴ�!" )
					<< BUILD_LOG( iSubQuestID )
					<< END_LOG;
				continue;
			}

			switch( pSubQuestTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_VISIT_VILLAGE:
				{
					// �ʵ� ���忡 ���� ����Ʈ �Ϸ� üũ!
					m_kUserQuestManager.Handler_OnEnterTheVillage( GetThisPtr<KGSUser>() );
				}
				break;

			case CXSLQuestManager::SQT_VISIT_FIELD:
				{
					// ��Ʋ�ʵ� ���忡 ���� ����Ʈ �Ϸ� üũ!
					m_kUserQuestManager.Handler_OnEnterTheBattleField( GetThisPtr<KGSUser>() );
				}
				break;

				//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			case CXSLQuestManager::SQT_VISIT_DUNGEON:
				{
					// ��Ʋ�ʵ� ���忡 ���� ����Ʈ �Ϸ� üũ!
					m_kUserQuestManager.Handler_OnEnterTheDungeon( GetThisPtr<KGSUser>() );
				}
				break;
#endif SERV_REFORM_QUEST
				//}}
			}
		}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_kUserQuestManager.Handler_OnPVPPlay_AccumulatedOfficialMatchCount( GetThisPtr<KGSUser>(), false );
#endif SERV_2012_PVP_SEASON2
		//}}
	}
}

IMPL_ON_FUNC( EGS_TALK_WITH_NPC_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_TALK_WITH_NPC_ACK	kPacket;

	//{{ 2009. 10. 15  ������	�ŷ� ����ó��
	if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
	{
		START_LOG( cwarn, L"�ŷ��߿��� ����� �� ���� ���!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK	= NetError::ERR_ITEM_14;
		SendPacket( EGS_TALK_WITH_NPC_ACK, kPacket );
		return;
	}
	//}}
	
	kPacket.m_iOK	= NetError::NET_OK;
	SendPacket( EGS_TALK_WITH_NPC_ACK, kPacket );

	m_kUserQuestManager.Handler_EGS_TALK_WITH_NPC_REQ( kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );

	//{{ 2008. 10. 10  ������	Ÿ��Ʋ
	m_kUserTitleManager.OnTalkWithNpc( kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );
	//}}
}

IMPL_ON_FUNC( EGS_GIVE_UP_QUEST_REQ )
{
#ifdef SERV_GATHER_GIVE_UP_QUEST
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else // SERV_GATHER_GIVE_UP_QUEST
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GIVE_UP_QUEST_REQ, EGS_GIVE_UP_QUEST_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	
#endif // SERV_GATHER_GIVE_UP_QUEST

	m_kUserQuestManager.Handler_EGS_GIVE_UP_QUEST_REQ( kPacket_, GetThisPtr<KGSUser>() );
}

#ifdef SERV_GATHER_GIVE_UP_QUEST 
IMPL_ON_FUNC( EGS_GATHER_GIVE_UP_QUEST_REQ )
{
#ifdef SERV_GATHER_GIVE_UP_QUEST
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else // SERV_GATHER_GIVE_UP_QUEST
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GATHER_GIVE_UP_QUEST_REQ, EGS_GIVE_UP_QUEST_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	
#endif // SERV_GATHER_GIVE_UP_QUEST

	BOOST_TEST_FOREACH( const int, iQuestID, kPacket_.m_vecGatherQuestID )
	{
		KEGS_GIVE_UP_QUEST_REQ kPacket;
		kPacket.m_iQuestID = iQuestID;
		m_kUserQuestManager.Handler_EGS_GIVE_UP_QUEST_REQ( kPacket, GetThisPtr<KGSUser>() );
	}
}
#endif // SERV_GATHER_GIVE_UP_QUEST

IMPL_ON_FUNC( DBE_GIVE_UP_QUEST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GIVE_UP_QUEST_ACK kAck;
	kAck.m_iOK		= kPacket_.m_iOK;
	kAck.m_iQuestID	= kPacket_.m_iQuestID;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( m_kUserQuestManager.RemoveQuest( kPacket_.m_iQuestID ) == false )
		{
			kAck.m_iOK = NetError::GetLastError();
		}
		else
		{
			SendUpdateDropQuestItemByIngQuest();
		}

		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kPacket_.m_iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"����Ʈ ������ �����ϴ�!" )
				<< BUILD_LOG( kPacket_.m_iQuestID )
				<< END_LOG;            
		}
		else
		{
			std::map< int, int >::const_iterator mit;
			for( mit = pQuestTemplet->m_mapAcceptRewardItem.begin(); mit != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mit )
			{
				std::vector< KInventoryItemInfo > vecInventorySlotInfo;
				m_kInventory.DeleteItemAll( mit->first, vecInventorySlotInfo );
				kAck.m_vecUpdatedInventorySlot.insert( kAck.m_vecUpdatedInventorySlot.end(), vecInventorySlotInfo.begin(), vecInventorySlotInfo.end() );
			}
		}
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
	}

	SendPacket( EGS_GIVE_UP_QUEST_ACK, kAck );
}

//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
IMPL_ON_FUNC( EGS_ALL_COMPLETED_QUEST_COMPLETE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ALL_COMPLETED_QUEST_COMPLETE_REQ, EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacketAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	// ����Ʈ ���� ����!
	m_kUserQuestManager.SetAutoCompleteQuest( kPacket_ );

	// ù��° ����Ʈ ���� ����!
	KEGS_QUEST_COMPLETE_REQ kReq;
	if( m_kUserQuestManager.GetAndDeleteAutoCompleteQuest( kReq ) == false )
	{
		START_LOG( cerr, L"�Ϸ��� ����Ʈ�� ���µ� ���� �� �Գ�?" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacketAck );
		return;
	}
	
	// ����Ʈ �Ϸ� ���� ����!
	m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kReq, GetThisPtr<KGSUser>() );
}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}

IMPL_ON_FUNC( EGS_QUEST_COMPLETE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUEST_COMPLETE_REQ, EGS_QUEST_COMPLETE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_QUEST_COMPLETE_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////
	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	if( SiCXSLQuestManager()->IsAccountQuest( kPacket_.m_iQuestID ) == true )
	{
		m_kUserAccountQuestManager.Handler_EGS_QUEST_COMPLETE_REQ_Account( kPacket_, GetThisPtr<KGSUser>() );
		return;
	}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kPacket_, GetThisPtr<KGSUser>() );
}

IMPL_ON_FUNC( DBE_QUEST_COMPLETE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( kPacket_.m_bAutoComplete )
		{
			// �ʱ�ȭ
			m_kUserQuestManager.ClearAutoCompleteQuest();

			KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacket;
			kPacket.m_iOK = kPacket_.m_iOK;
			SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacket );
		}
		else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		{
			KEGS_QUEST_COMPLETE_ACK	kPacket;
			kPacket.m_iOK		= kPacket_.m_iOK;
			//{{ 2010. 04. 02  ������	��������Ʈ
			//#ifdef SERV_DAILY_QUEST
			kPacket.m_kCompleteQuestInfo = kPacket_.m_kCompleteQuestInfo;
			//#else
			//	//kPacket.m_iQuestID	= kPacket_.m_iQuestID;
			//#endif SERV_DAILY_QUEST
			//}}	
			SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
		}
		return;
	}

	//db���� �Ϸ�üũ�� �ް� ������
	m_kUserQuestManager.Handler_DBE_QUEST_COMPLETE_ACK( kPacket_, GetThisPtr<KGSUser>() );
	//- �������� ���� ���� ���� client���� �˷��ֱ� ���� ������Ʈ�� ���Ѿ� �ϳ� ���ƾ� �ϳ� ��~~

	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( kPacket_.m_bAutoComplete )
	{
		// �״��� ���� ����Ʈ!
		KEGS_QUEST_COMPLETE_REQ kReq;
		if( m_kUserQuestManager.GetAndDeleteAutoCompleteQuest( kReq ) == true )
		{
			// ����Ʈ �Ϸ� ���� ����!
			m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kReq, GetThisPtr<KGSUser>() );			
		}
		else
		{
			// ��� ����Ʈ�� ����ٸ�!
			KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			m_kUserQuestManager.GetAutoCompleteQuestResult( kPacketAck );
			SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacketAck );
		}
	}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	// ù��° ����Ʈ Ŭ���� ���� üũ!
	if( m_kUserQuestManager.GetCompleteQuestSize() == 1 )
	{
		WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_QUEST_COMPLETE );
	}
#endif SERV_CHAR_LOG
	//}}

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
	if( kPacket_.m_kCompleteQuestInfo.m_iQuestID == 74710 || kPacket_.m_kCompleteQuestInfo.m_iQuestID == 74720 || kPacket_.m_kCompleteQuestInfo.m_iQuestID == 74730 )
	{
//		if( GetUnitType() == CXSLUnit::UT_ELSWORD || GetUnitType() == CXSLUnit::UT_RAVEN || GetUnitType() == CXSLUnit::UT_CHUNG )
		if( GetUnitType() == CXSLUnit::UT_ARME || GetUnitType() == CXSLUnit::UT_LIRE || GetUnitType() == CXSLUnit::UT_EVE || GetUnitType() == CXSLUnit::UT_ARA )		
		{
			KDBE_SET_EVENT_INFO_NOT kPacketToDB;
			kPacketToDB.m_iQuestID = kPacket_.m_kCompleteQuestInfo.m_iQuestID;
			kPacketToDB.m_iUnitType = GetUnitType();

			SendToGameDB( DBE_SET_EVENT_INFO_NOT, kPacketToDB );				
		}
	}
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
	IF_EVENT_ENABLED( CEI_BALANCE_FINALITY_SKILL_EVENT )
	{
		if( kPacket_.m_kCompleteQuestInfo.m_iQuestID == 35370 )
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 2048;	// ī�ж��� ������ (�ʱ�)
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

	if( kPacket_.m_bIsChangeJob == true )
	{
#ifdef SERV_ARA_FIRST_CLASS_CHANGE_EVENT
		IF_EVENT_ENABLED( CEI_ARA_FIRST_CLASS_CHANGE_EVENT )
		{
			if( CXSLUnit::UC_ARA_LITTLE_HSIEN == GetUnitClass() )
			{
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = 1553;	// �Ҽ��� ���� Ư�� ���� ť��A
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}
		}
#endif SERV_ARA_FIRST_CLASS_CHANGE_EVENT

#ifdef SERV_ARA_NEW_FIRST_CLASS_EVENT
		IF_EVENT_ENABLED( CEI_ARA_NEW_FIRST_CLASS_EVENT )
		{
			if( CXSLUnit::UC_ARA_LITTLE_DEVIL == GetUnitClass() )
			{
				// �̺�Ʈ ������ ����!
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_FIRST_CLASS_EVENT_REWARD_ID::iClassChangeQuestReward;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}
		}
#endif SERV_ARA_NEW_FIRST_CLASS_EVENT

#ifdef SERV_ARA_NEW_SECOND_CLASS_EVENT
		IF_EVENT_ENABLED( CEI_ARA_NEW_SECOND_CLASS_EVENT )
		{
			if( CXSLUnit::UC_ARA_YAMA_RAJA == GetUnitClass() )
			{
				// �̺�Ʈ ������ ����!
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_SECOND_CLASS_EVENT_REWARD_ID::iAraNewSecondClassChangeReward;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}

			if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
			{
				// �̺�Ʈ ������ ����!
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_SECOND_CLASS_EVENT_REWARD_ID::iSecondClassChangeReward;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}
		}		
#endif SERV_ARA_NEW_SECOND_CLASS_EVENT

#ifdef SERV_EVENT_CHANGE_CLASS
		IF_EVENT_ENABLED( CEI_EVENT_CHANGE_CLASS )
		{
			if( CXSLUnit::UC_ARME_BATTLE_MAGICIAN == GetUnitClass() )
			{
				// �̺�Ʈ ������ ����!
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = _CONST_EVENT_CHANGE_CLASS_ITEM_INT_::iTransFormItem;	//��Ʈ������ ť��(��Ʋ������) ���̵�� �ٲ����
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}
		}
#endif SERV_EVENT_CHANGE_CLASS

#ifdef SERV_ELESIS_NEW_FIRST_CLASS_EVENT
		IF_EVENT_ENABLED( CEI_ELESIS_NEW_FIRST_CLASS_EVENT )
		{
			if( CXSLUnit::UC_ELESIS_SABER_KNIGHT == GetUnitClass() || CXSLUnit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass() )
			{
				// �̺�Ʈ ������ ����!
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = _CONST_ELESIS_NEW_FIRST_CLASS_EVENT_REWARD_ID::iClassChangeQuestReward;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}
		}
#endif SERV_ELESIS_NEW_FIRST_CLASS_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		///���������� �ش� UI�� �ٲ��־�� �ϴϱ� ���⼭ ��Ŷ�� ����
		if( CXSLUnit::UC_CHUNG_FURY_GUARDIAN == GetUnitClass()
			|| CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN == GetUnitClass() 
			|| CXSLUnit::UC_CHUNG_IRON_PALADIN == GetUnitClass()
			|| CXSLUnit::UC_CHUNG_DEADLY_CHASER == GetUnitClass() )
		{
			CTime cNowTime = GetCurrentTime();
			KEGS_EVENT_CHUNG_GIVE_ITEM_NOT kPacketNot;
			kPacketNot.m_bGiveItemGet = false;
			kPacketNot.m_wstrToolTipTime = cNowTime.Format(L"%Y-%m-%d %H:%M:%S");
			kPacketNot.m_cGetUnitClass = GetUnitClass();
			kPacketNot.m_bTwoGiveItem = false;
			SendPacket(EGS_EVENT_CHUNG_GIVE_ITEM_NOT,kPacketNot);
		}		
#endif SERV_EVENT_CHUNG_GIVE_ITEM
		KDBE_UPDATE_UNIT_CLASS_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_cUnitClass = GetUnitClass();

		SendToLogDB( DBE_UPDATE_UNIT_CLASS_NOT, kPacketNot );

		m_kSkillTree.SetUnitClass( (int) GetUnitClass() );

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // ����ö // ���� ����Ʈ �Ϸ� ��, ��� ��ų �������� ���� �ǵ��� ��
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iNewDefaultSkill1, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iNewDefaultSkill2, 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iNewDefaultSkill1, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iNewDefaultSkill2, 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM

		KEGS_CHANGE_MY_UNIT_INFO_NOT kMyInfoNot;
		kMyInfoNot.m_cUnitClass = GetUnitClass();
		kMyInfoNot.m_iNewDefaultSkill_1 = kPacket_.m_iNewDefaultSkill1;
		kMyInfoNot.m_iNewDefaultSkill_2 = kPacket_.m_iNewDefaultSkill2;
		SendPacket( EGS_CHANGE_MY_UNIT_INFO_NOT, kMyInfoNot );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			// �ʵ忡 ������� ���� ���� �����ϱ�
			if( GetFieldUID() > 0 )
			{
				KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
				GetFieldUserInfo( kNot.m_kInfo );
				SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
			}
		}		
		else if( GetStateID() == KGSFSM::S_ROOM )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				//{{ ��Ʋ�ʵ� : �ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				{
					KRoomUserInfo kRoomUserInfo;
					GetRoomUserInfo( kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				{
					KSquareUserInfo kSquareUserInfo;
					GetSquareUserInfo( kSquareUserInfo );
					SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
				}
				break;

			default:
				{
					START_LOG( cerr, L"�̻��� �� Ÿ��." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}
		}

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}

		//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
		SendUpdateUnitInfoToRelayDB( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT::UT_UNITCLASS_CHANGE );
#endif SERV_RELAY_DB_CONNECTION
		//}}
	}

	//{{ 2008. 10. 10  ������	Ÿ��Ʋ
	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	m_kUserTitleManager.OnQuestComplete( kPacket_.m_kCompleteQuestInfo.m_iQuestID, GetThisPtr<KGSUser>() );
#else
#pragma ELSE_MESSAGE
	//m_kUserTitleManager.OnQuestComplete( kPacket_.m_iQuestID, GetThisPtr<KGSUser>() );
#endif SERV_DAILY_QUEST
	//}}
	//}}

	//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	m_kUserEventManager.CheckWebPointEventQuest( kPacket_.m_iQuestID, GetThisPtr<KGSUser>() );
#endif SERV_WEB_POINT_EVENT
	//}}

	// ������� ����Ʈ ���� ������
	std::map< int, KItemInfo >::iterator mit = kPacket_.m_mapInsertedItem.begin();
	for( ; mit != kPacket_.m_mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_QUEST, mit->second.m_iQuantity );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_QUEST, mit->second.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
}

_IMPL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT )
{
	VERIFY_STATE_LOG( clog2, ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 06. 13	��μ�       ������ �˴ٿ���
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
	{
		if( m_kUserInfoByNexon.IsExistRestrictionTime() == true )
		{
			std::wstring wstrRestrictionTime;
			CTime tCurr = CTime::GetCurrentTime();
			CTimeSpan tSpan = CTimeSpan( 0, 1, 0, 0 );
			CTime tUserRestrictionTime;
			m_kUserInfoByNexon.GetRestrictionUserTime( wstrRestrictionTime );
            if( KncUtil::ConvertStringToCTime( wstrRestrictionTime , tUserRestrictionTime ) == true )
			{
				if( tUserRestrictionTime < tCurr+ tSpan && tUserRestrictionTime > tCurr )
				{
					// �˴ٿ��� ���� �������Ը� �����Ѵ�.
					SendPacket( EGS_NOTIFY_MSG_NOT, kPacket_ );	
				}	
			}	
		}
	}
	else
	{
		SendPacket( EGS_NOTIFY_MSG_NOT, kPacket_ );
	}
#else
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
	{
		if( m_kUserInfoByNexon.IsShutdownUserMSG() == true )
		{
			// �˴ٿ��� ���� �������Ը� �����Ѵ�.
			SendPacket( EGS_NOTIFY_MSG_NOT, kPacket_ );			
		}
	}
	else
	{
		SendPacket( EGS_NOTIFY_MSG_NOT, kPacket_ );
	}
#else
	SendPacket( EGS_NOTIFY_MSG_NOT, kPacket_ );
#endif SERV_SHUTDOWN_SYSTEM
	//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}
}
//}}

IMPL_ON_FUNC( EGS_SEARCH_UNIT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_ ) == false )
	{
		KEGS_SEARCH_UNIT_ACK kPacket; 
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_03;
		SendPacket( EGS_SEARCH_UNIT_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//�ڱ� �ڽ��� ������ ��û�� ���.

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(GetCharName(), kPacket_) )
#else
	if( GetCharName().compare( kPacket_ ) == 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_SEARCH_UNIT_ACK kPacket; 
		kPacket.m_iOK = NetError::NET_OK;
		GetConnectionUnitInfo( kPacket.m_kCUnitInfo );

		SendPacket( EGS_SEARCH_UNIT_ACK, kPacket );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_ ) )
	{
		KEGS_SEARCH_UNIT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_SEARCH_UNIT_ACK, kPacket );
		return;
	}

	if( m_kTimer[TM_SEARCH_UNIT_REFRESH].elapsed() < (double)SEARCH_UNIT_REFRESH_TIME_LIMIT)
	{
		KEGS_SEARCH_UNIT_ACK kPacket; 
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_01;
		SendPacket( EGS_SEARCH_UNIT_ACK, kPacket );
		return;
	}
	m_kTimer[TM_SEARCH_UNIT_REFRESH].restart();

	KELG_SEARCH_UNIT_REQ kPacketReq;
	kPacketReq.m_usEventID = EGS_SEARCH_UNIT_REQ;
	kPacketReq.m_wstrNickName = kPacket_;
	SendToLoginServer( ELG_SEARCH_UNIT_REQ, kPacketReq );
}

//{{ 2009. 4. 17  ������	�������� ����ã��
IMPL_ON_FUNC( EGS_GET_CONNECTION_UNIT_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_ ) == false )
	{
		KEGS_GET_CONNECTION_UNIT_INFO_ACK kPacket; 
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_03;
		SendPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//�ڱ� �ڽ��� ������ ��û�� ���.
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(GetCharName(), kPacket_) )
#else
	if( GetCharName().compare( kPacket_ ) == 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_GET_CONNECTION_UNIT_INFO_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		GetConnectionUnitInfo( kPacket.m_kCUnitInfo );

		SendPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK, kPacket );
		return;
	}

	// [����] DB������ ���� ���ڿ��� �ƴ����� Ȥ�ó� �ؼ� �˻��غ�.
	if( KODBC::IsInvalidMarkIn( kPacket_ ) )
	{
		KEGS_GET_CONNECTION_UNIT_INFO_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK, kPacket );
		return;
	}

	if( m_kTimer[TM_SEARCH_UNIT_REFRESH].elapsed() < (double)SEARCH_UNIT_REFRESH_TIME_LIMIT)
	{
		KEGS_GET_CONNECTION_UNIT_INFO_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_01;
		SendPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK, kPacket );
		return;
	}
	m_kTimer[TM_SEARCH_UNIT_REFRESH].restart();

	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() > 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON  ||
			CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_PVP )
		{
			KELG_SEARCH_UNIT_REQ kPacketReq;
			kPacketReq.m_usEventID = EGS_GET_CONNECTION_UNIT_INFO_REQ;
			kPacketReq.m_wstrNickName = kPacket_;
			SendToCnRoom( ERM_SEARCH_UNIT_REQ, kPacketReq );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}

	KELG_SEARCH_UNIT_REQ kPacketReq;
	kPacketReq.m_usEventID = EGS_GET_CONNECTION_UNIT_INFO_REQ;
	kPacketReq.m_wstrNickName = kPacket_;
	SendToLoginServer( ELG_SEARCH_UNIT_REQ, kPacketReq );
}
//}}

//{{ 2009. 6. 23  ������	���캸��
IMPL_ON_FUNC( EGS_WATCH_UNIT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//�ڱ� �ڽ��� ������ ��û�� ���.
	if( kPacket_.m_iUnitUID == GetCharUID() )
	{
		KEGS_WATCH_UNIT_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		GetConnectionUnitInfo( kPacket.m_kCUnitInfo );

		SendPacket( EGS_WATCH_UNIT_ACK, kPacket );
		return;
	}

	if( m_kTimer[TM_SEARCH_UNIT_REFRESH].elapsed() < (double)SEARCH_UNIT_REFRESH_TIME_LIMIT)
	{
		KEGS_WATCH_UNIT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_01;
		SendPacket( EGS_WATCH_UNIT_ACK, kPacket );
		return;
	}
	m_kTimer[TM_SEARCH_UNIT_REFRESH].restart();

	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() > 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON  ||
			CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_PVP )
		{
			KELG_SEARCH_UNIT_REQ kPacketReq;
			kPacketReq.m_usEventID = EGS_WATCH_UNIT_REQ;
			kPacketReq.m_iUnitUID = kPacket_.m_iUnitUID;
			SendToCnRoom( ERM_SEARCH_UNIT_REQ, kPacketReq );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}

	KELG_SEARCH_UNIT_REQ kPacketReq;
	kPacketReq.m_usEventID = EGS_WATCH_UNIT_REQ;
	kPacketReq.m_iUnitUID = kPacket_.m_iUnitUID;
	SendToLoginServer( ELG_SEARCH_UNIT_REQ, kPacketReq );
}
//}}

IMPL_ON_FUNC( ELG_SEARCH_UNIT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_usEventID == EGS_SEARCH_UNIT_REQ )
	{
		//ã�� ĳ���� �������
		if( kPacket_.m_iOK == NetError::NET_OK  ||  kPacket_.m_iOK == NetError::ERR_GM_CHAR_DENY_01 )
		{
			KEGS_SEARCH_UNIT_ACK kPacketAck;
			kPacketAck.m_iOK = kPacket_.m_iOK;
			if( kPacket_.m_iOK == NetError::NET_OK )
			{
				kPacketAck.m_kCUnitInfo = kPacket_.m_kCUnitInfo;
			}
			SendPacket( EGS_SEARCH_UNIT_ACK, kPacketAck );
		}
		//��ã�Ҵٰ� ���°��
		else
		{
			SendToGameDB( DBE_SEARCH_UNIT_REQ, kPacket_.m_kCUnitInfo.m_wstrCharName );
		}
		return;
	}
	else if( kPacket_.m_usEventID == EGS_GET_CONNECTION_UNIT_INFO_REQ )
	{
		KEGS_GET_CONNECTION_UNIT_INFO_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			kPacketAck.m_kCUnitInfo = kPacket_.m_kCUnitInfo;
		}
		SendPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK, kPacketAck );
		return;
	}
	else if( kPacket_.m_usEventID == EGS_WATCH_UNIT_REQ )
	{
		KEGS_WATCH_UNIT_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			kPacketAck.m_kCUnitInfo = kPacket_.m_kCUnitInfo;
		}
		SendPacket( EGS_WATCH_UNIT_ACK, kPacketAck );
		return;
	}
	// 2011.05.05 lygan_������ // �߱��� Ŀ�´�Ƽ�� ���� ���� ����
#ifdef SERV_USER_WATCH_NEW
	else if(kPacket_.m_usEventID == EGS_USER_COMMUNITY_SURVEY_NEW_REQ )
	{
		if( kPacket_.m_iOK == NetError::NET_OK  ||  kPacket_.m_iOK == NetError::ERR_GM_CHAR_DENY_00 )
		{
			KEGS_USER_COMMUNITY_SURVEY_NEW_ACK kPacketAck;
			kPacketAck.m_iOK = kPacket_.m_iOK;
			if( kPacket_.m_iOK == NetError::NET_OK )
			{
				kPacketAck.unitUID = kPacket_.m_kCUnitInfo.m_iUnitUID;
				kPacketAck.m_cCommunity_Tab_Type = m_cCommunity_Tap_type;
			}
			SendPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK , kPacketAck );
			m_cCommunity_Tap_type = 0;
		}
		//��ã�Ҵٰ� ���°��
		else
		{
			SendToGameDB( DBE_SEARCH_UNIT_REQ, kPacket_.m_kCUnitInfo.m_wstrCharName );
			m_cCommunity_Tap_type = 0;
		}
		return;
	}
#endif //SERV_USER_WATCH_NEW
	
	START_LOG( cerr, L"event id�� �̻��մϴ�!" )
		<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
		<< END_LOG;
}

_IMPL_ON_FUNC( DBE_SEARCH_UNIT_ACK, KPacketOK )
{
    KEGS_SEARCH_UNIT_ACK kPacket;
    kPacket.m_iOK = kPacket_.m_iOK;
    SendPacket( EGS_SEARCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_GET_UNIT_INFO_REQ )
{
	//���������� �־�� �Ѵ�.?
	KELG_GET_UNIT_INFO_ACK kPacket;
	kPacket.m_usEventID		 = kPacket_.m_usEventID;
	kPacket.m_iDemandGSUID	 = kPacket_.m_iDemandGSUID;
	kPacket.m_iDemandUserUID = kPacket_.m_iDemandUserUID;

	//{{ 2009. 3. 24  ������	��� ����
	if( GetAuthLevel() == SEnum::UAL_GM )
	{
		kPacket.m_iOK	= NetError::ERR_GM_CHAR_DENY_01;
	}
	else
	{
		kPacket.m_iOK	= NetError::NET_OK;

		GetConnectionUnitInfo( kPacket.m_kCUnitInfo );
	}
	//}}

    SendToLoginServer( ELG_GET_UNIT_INFO_ACK, kPacket );
}

//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

_IMPL_ON_FUNC( ERM_GET_UNIT_INFO_REQ, KELG_GET_UNIT_INFO_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		return;
	}

	//���������� �־�� �Ѵ�.?
	KELG_GET_UNIT_INFO_ACK kPacket;
	kPacket.m_usEventID		 = kPacket_.m_usEventID;
	kPacket.m_iDemandGSUID	 = kPacket_.m_iDemandGSUID;
	kPacket.m_iDemandUserUID = kPacket_.m_iDemandUserUID;

	//{{ 2009. 3. 24  ������	��� ����
	if( GetAuthLevel() == SEnum::UAL_GM )
	{
		kPacket.m_iOK	= NetError::ERR_GM_CHAR_DENY_00;
	}
	else
	{
		kPacket.m_iOK	= NetError::NET_OK;

		GetConnectionUnitInfo( kPacket.m_kCUnitInfo );
	}
	//}}

	SendToCnRoom( ERM_GET_UNIT_INFO_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_SEARCH_UNIT_ACK, KELG_SEARCH_UNIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_usEventID == EGS_GET_CONNECTION_UNIT_INFO_REQ )
	{
		KEGS_GET_CONNECTION_UNIT_INFO_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			kPacketAck.m_kCUnitInfo = kPacket_.m_kCUnitInfo;
		}
		SendPacket( EGS_GET_CONNECTION_UNIT_INFO_ACK, kPacketAck );
		return;
	}
	else if( kPacket_.m_usEventID == EGS_WATCH_UNIT_REQ )
	{
		KEGS_WATCH_UNIT_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			kPacketAck.m_kCUnitInfo = kPacket_.m_kCUnitInfo;
		}
		SendPacket( EGS_WATCH_UNIT_ACK, kPacketAck );
		return;
	}

	START_LOG( cerr, L"event id�� �̻��մϴ�!" )
		<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
		<< END_LOG;
}

#endif SERV_INTEGRATION
//}}

void KGSUser::CheckRepeatEvent( u_short usEventID )
{
	//2008.01.29.hoons.. �켱 1�������� ����� ��Ŷ(req => ack)�� �־��ش�.
	switch( usEventID )
	{
	case EGS_CREATE_ROOM_REQ:
	case EGS_JOIN_ROOM_REQ:
	case EGS_QUICK_JOIN_REQ:
	case EGS_LEAVE_ROOM_REQ:
	case EGS_BAN_USER_REQ:
	case EGS_CHANGE_TEAM_REQ:
	case EGS_CHANGE_READY_REQ:
	case EGS_CHANGE_PITIN_REQ:
	case EGS_CHANGE_MAP_REQ:
	case EGS_CHANGE_SLOT_OPEN_REQ:
	case EGS_CHANGE_PLAY_TIME_LIMIT_REQ:
	case EGS_CHANGE_WINNING_NUM_KILL_REQ:
	case EGS_STATE_CHANGE_GAME_START_REQ:
	case EGS_END_GAME_REQ:
	case EGS_STATE_CHANGE_RESULT_REQ:
	case EGS_LEAVE_GAME_REQ:
	case EGS_USER_UNIT_RE_BIRTH_POS_REQ:
	case EGS_USER_UNIT_DIE_REQ:
	case EGS_STATE_CHANGE_GAME_INTRUDE_REQ:
	case EGS_INTRUDE_START_REQ:
	case EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ:
	case EGS_DUNGEON_STAGE_LOAD_REQ:
	case EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ:
	case EGS_DUNGEON_SUB_STAGE_OPEN_REQ:
	case EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ:
	case EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ:
	case EGS_NPC_UNIT_CREATE_REQ:
	//case EGS_NPC_UNIT_DIE_REQ:	//�������͸� ���̰� ���� ������ ���͵��� ����ó���� ���� ��� die��ų�� �߻��Ҽ��־� �ּ�ó��
	//case EGS_CHANGE_DUNGEON_DIFFICULTY_REQ:
	//���λ�������.
	case EGS_REG_PERSONAL_SHOP_ITEM_REQ:
	case EGS_CREATE_PERSONAL_SHOP_REQ:
	case EGS_JOIN_PERSONAL_SHOP_REQ:
	case EGS_BREAK_PERSONAL_SHOP_REQ:
		{
			if( m_kRepeatEvent.m_usPreEventID == usEventID )
			{
				++m_kRepeatEvent.m_iPreEventIDCnt;

				//SimLayer���� ���..
				GetKGSSimLayer()->AddRepeatEvent( m_kRepeatEvent.m_usPreEventID, m_kRepeatEvent.m_iPreEventIDCnt );
			}
			else
			{
				m_kRepeatEvent.Reset();

				m_kRepeatEvent.m_usPreEventID = usEventID;
				++m_kRepeatEvent.m_iPreEventIDCnt;
			}
		}
		break;

	default:
		{
			m_kRepeatEvent.Reset();
		}
		break;
	}
}

IMPL_ON_FUNC( EGS_CHANGE_SKILL_SLOT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2013. 01. 01	������	�������� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
	if( m_kTimer[TM_SKILL_CHANGE_COOL_TIME_AT_START_COUNT].elapsed() < 3.0 )
	{
		START_LOG( cerr, L"��ų ���� ��Ÿ������ ���� ���� ��ų������ �����Ҽ� �����ϴ�!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_CHANGE_SKILL_SLOT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_28;
		SendPacket( EGS_CHANGE_SKILL_SLOT_ACK, kPacket );
		return;
	}
#endif SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
	//}}

	KEGS_CHANGE_SKILL_SLOT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2012. 12. 3	�ڼ���	��ų ���� ü���� ��Ŷ ����
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	if( m_kSkillTree.ChangeSkillSlot( kPacket_, kPacket ) == false )
#else
	if( m_kSkillTree.ChangeSkillSlot( kPacket_.m_iSlotID, kPacket_.m_iSkillID ) == false )
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	//}}
	{
		kPacket.m_iOK = NetError::GetLastError();

		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< BUILD_LOG( kPacket_.m_iSlotID )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;
	}
	else
	{
		kPacket.m_iSlotID	= kPacket_.m_iSlotID;
		kPacket.m_iSkillID	= kPacket_.m_iSkillID;
	}

	SendPacket( EGS_CHANGE_SKILL_SLOT_ACK, kPacket );


	// ������ȿ��� ��ų ����, ȹ�� ������ �ٲ�� ������ ������ ��������� �Ѵ�.
	if( NetError::GetLastError() == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			//{{ 2012. 02. 01	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;

		default:
			return;
		}

		//{{ 2012. 02. 01	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		if( GetRoomUID() != 0 )
		{
			KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
			GetRoomUserInfo( kPacketNot );
			SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
		}
#else
		KEGS_CHANGE_SKILL_SLOT_NOT kPacketNot;
		kPacketNot.m_iSlotID	= kPacket_.m_iSlotID;
		kPacketNot.m_iSkillID	= kPacket_.m_iSkillID;
		kPacketNot.m_iUnitUID	= GetCharUID();

		// ������ ���� ���� ������Ʈ
		KRoomUserInfo kInfo;
		GetRoomUserInfo( kInfo );

		// ���� �� ������ ���, ������ ����
		//if( ( int )KncUid::ExtractReservedID( GetRoomUID() ) != CXSLRoom::RT_SQUARE )
		{
			//SendToCnRoom( ERM_CHANGE_SKILL_SLOT_NOT, kPacketNot );		// note!! ���ӽ��� ������ unit ������ ���� �ޱ� ������ �ʿ����.

			SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
		}
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
	}
}

IMPL_ON_FUNC( EGS_NEW_BLACKLIST_USER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_NEW_BLACKLIST_USER_ACK kPacket;

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrNickName, GetCharName()) )
#else
	if( kPacket_.m_wstrNickName == GetCharName() )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		// �ڱ� �ڽ��� ������Ʈ�� ����� �� ����.
		kPacket.m_iOK = NetError::ERR_BLACKLIST_05;
		SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacket );
		return;
	}

	if( m_kUserGameOption.IsAddChatBlackListPossible() == false )
	{
		// �ִ� ��� ������ �ο� �� �ʰ�
		kPacket.m_iOK = NetError::ERR_BLACKLIST_00;
		SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacket );
		return;
	}
	
	// �α��� �������� �г������� UnitUID ã��
	KELG_SEARCH_BLACK_LIST_UNIT_REQ kPacketToLg;
	kPacketToLg.m_wstrNickName = kPacket_.m_wstrNickName;
	SendToLoginServer( ELG_SEARCH_BLACK_LIST_UNIT_REQ, kPacketToLg );
}

IMPL_ON_FUNC( ELG_SEARCH_BLACK_LIST_UNIT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_NEW_BLACKLIST_USER_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacket );
		return;
	}

	if( m_kUserGameOption.IsChatBlackListUnit( kPacket_.m_iUnitUID ) == true )
	{
		// �̹� ������Ʈ�� ��ϵ� ������.
		kPacket.m_iOK = NetError::ERR_BLACKLIST_01;
		SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacket );
		return;
	}

	// DB�� ���
	KDBE_NEW_BLACKLIST_USER_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID			= GetCharUID();
	kPacketToDB.m_iBlackListUnitUID = kPacket_.m_iUnitUID;
	SendToGameDB( DBE_NEW_BLACKLIST_USER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_NEW_BLACKLIST_USER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_NEW_BLACKLIST_USER_ACK kPacketAck;
	kPacketAck.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ������Ʈ�� �����߰�
		m_kUserGameOption.AddChatBlackListUnit( kPacket_.m_kChatBlackListUnit.m_iUnitUID, kPacket_.m_kChatBlackListUnit.m_wstrNickName );
		
		kPacketAck.m_kChatBlackListUnit.m_iUnitUID	   = kPacket_.m_kChatBlackListUnit.m_iUnitUID;
		kPacketAck.m_kChatBlackListUnit.m_wstrNickName = kPacket_.m_kChatBlackListUnit.m_wstrNickName;
		SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacketAck );
		return;
	}
	
	SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacketAck );
}

IMPL_ON_FUNC( EGS_DEL_BLACKLIST_USER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	// ���������� UI �ϼ��� �ٽ� �����Ұ�!

	KEGS_DEL_BLACKLIST_USER_ACK kPacket;

	if( kPacket_.m_iUnitUID == GetCharUID() )
	{
		// �ڱ� �ڽ��� ������Ʈ�� ������ �� ����.
		kPacket.m_iOK = NetError::ERR_BLACKLIST_06;
		SendPacket( EGS_NEW_BLACKLIST_USER_ACK, kPacket );
		return;
	}

	if( m_kUserGameOption.IsChatBlackListUnit( kPacket_.m_iUnitUID ) == false )
	{
		// �̹� ������Ʈ���� ������ ������
		kPacket.m_iOK = NetError::ERR_BLACKLIST_02;
		SendPacket( EGS_DEL_BLACKLIST_USER_ACK, kPacket );
		return;
	}

	// DB���� ����
	KDBE_DEL_BLACKLIST_USER_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID			= GetCharUID();
	kPacketToDB.m_iBlackListUnitUID = kPacket_.m_iUnitUID;
	SendToGameDB( DBE_DEL_BLACKLIST_USER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_DEL_BLACKLIST_USER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_DEL_BLACKLIST_USER_ACK kPacket;
	kPacket.m_iOK				= kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kUserGameOption.DelChatBlackListUnit( kPacket_.m_iBlackListUnitUID, kPacket.m_kChatBlackListUnit.m_wstrNickName );
		kPacket.m_kChatBlackListUnit.m_iUnitUID = kPacket_.m_iBlackListUnitUID;
	}	

	SendPacket( EGS_DEL_BLACKLIST_USER_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_UPDATE_COMMUNITY_OPTION_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_UPDATE_COMMUNITY_OPTION_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// Ŀ�´�Ƽ �ɼ� ������Ʈ	
	if( !m_kUserGameOption.SetDenyOptions( kPacket_.m_kDenyOptions ) )
	{
		kPacket.m_iOK = NetError::ERR_COMMUNITY_OPT_01;
		goto end_proc;
	}
	
	kPacket.m_kDenyOptions = kPacket_.m_kDenyOptions;

	//{{ 2009. 4. 1  ������		ģ�� ���� �ɼ�
	KELG_UPDATE_DENY_FRIEND_SHIP_NOT kPacketNot;
	kPacketNot.m_bDenyFriendShip = m_kUserGameOption.IsDenyFriendShip();
	SendToLoginServer( ELG_UPDATE_DENY_FRIEND_SHIP_NOT, kPacketNot );
	//}}

end_proc:
	SendPacket( EGS_UPDATE_COMMUNITY_OPTION_ACK, kPacket );
}

//{{ 2010. 07. 01  ������	������ ����
IMPL_ON_FUNC( ERM_DECREASE_ENDURANCE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	std::vector< KInventoryItemInfo > vecDummy;
	KEGS_DECREASE_ENDURANCE_NOT kNot;

	// �����Ȱ� ��ŭ �������� ��´�
	m_kInventory.DecreaseEquippedItemEndurance( kPacket_.m_iEnduranceDamage, 0, 0, kNot.m_vecEnduranceUpdate, vecDummy );

	// Ŭ���̾�Ʈ���� �Ҹ�� �������� �����ش�!
	if( kNot.m_vecEnduranceUpdate.empty() == false )
	{
		SendPacket( EGS_DECREASE_ENDURANCE_NOT, kNot );
	}
}
//}}

IMPL_ON_FUNC( EGS_FRAME_AVERAGE_REQ )
{
	// ���������� ���� ����� ���´� S_ROOM, ����ġ���� ��Ż�� S_CHANNEL, �������� ��Ż�� S_LOCAL_MAP, S_VILLAGE_MAP
	VERIFY_STATE_WARN( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_FRAME_AVERAGE_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	if( kPacket_.m_iFrameAverage < 0 )
	{
		START_LOG( cerr, L"�߸��� ������ ��հ��Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iFrameAverage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_STATISTICS_00;
		goto end_proc;
	}

	int iGameType;
	switch( kPacket_.m_cGameType )
	{
	case CXSLGame::GT_PVP:
		iGameType = 0; // ����
		break;
	
	case CXSLGame::GT_DUNGEON:
		iGameType = 1; // ����
		break;

		//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case CXSLGame::GT_BATTLE_FIELD:
		iGameType = 2; // ��Ʋ�ʵ�
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	
	case CXSLGame::GT_NONE:
		goto end_proc;

	default:
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� �̻��� ����Ÿ���� ���´�." )
			<< BUILD_LOGc( kPacket_.m_cGameType )
			<< END_LOG;
		goto end_proc;
	}
	
	{
		int iIndex = 0;
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iGameType );
		// �������� ��Ż�ÿ��� ����ä�ο��� ���»����̹Ƿ� ä��Ÿ���� 0�̴�.

		if( kPacket_.m_iFrameAverage >= 0  &&  kPacket_.m_iFrameAverage <= 20 )
		{
			iIndex = KStatistics::eSIColFps_0_20;
		}
		else if( kPacket_.m_iFrameAverage >= 21  &&  kPacket_.m_iFrameAverage <= 30 )
		{
			iIndex = KStatistics::eSIColFps_21_30;
		}
		else if( kPacket_.m_iFrameAverage >= 31  &&  kPacket_.m_iFrameAverage <= 40 )
		{
			iIndex = KStatistics::eSIColFps_31_40;
		}
		else if( kPacket_.m_iFrameAverage >= 41  &&  kPacket_.m_iFrameAverage <= 50 )
		{
			iIndex = KStatistics::eSIColFps_41_50;
		}
		else if( kPacket_.m_iFrameAverage >= 51  &&  kPacket_.m_iFrameAverage <= 60 )
		{
			iIndex = KStatistics::eSIColFps_51_60;
		}
		else if( kPacket_.m_iFrameAverage >= 61  &&  kPacket_.m_iFrameAverage <= 70 )
		{
			iIndex = KStatistics::eSIColFps_61_70;
		}
		else if( kPacket_.m_iFrameAverage >= 71  &&  kPacket_.m_iFrameAverage <= 80 )
		{
			iIndex = KStatistics::eSIColFps_71_80;
		}
		else if( kPacket_.m_iFrameAverage >= 81  &&  kPacket_.m_iFrameAverage <= 90 )
		{
			iIndex = KStatistics::eSIColFps_81_90;
		}
		else
		{
			iIndex = KStatistics::eSIColFps_91_unlimited;
		}

		KSIManager.IncreaseCount( KStatistics::SI_FPS, kKey, iIndex, 1 );

		START_LOG( clog2, L"������ ��� ���" )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( iIndex )
			<< BUILD_LOG( kPacket_.m_iFrameAverage );
	}

end_proc:	
	SendPacket( EGS_FRAME_AVERAGE_ACK, kPacket ); // ���� ��Ŷ
}

IMPL_ON_FUNC( EGS_CHANGE_NICK_NAME_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CHANGE_NICK_NAME_REQ, EGS_CHANGE_NICK_NAME_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

    char charBuf[255] = {0};
    std::string strNickName;

    WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrNickName.c_str(), -1, charBuf, 255, NULL, NULL );
    strNickName = charBuf;

	if( strNickName.empty() || strNickName.size() > 12
#ifdef SERV_NICK_NAME_MINIMUM_CHECK
		|| strNickName.size() < 2
#endif //SERV_NICK_NAME_MINIMUM_CHECK
		)   //ĳ���� �г��� ���� �˻�.
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_06;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_05;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }

    // ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
    if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_05;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }

	//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( SiKPvpMatchManager()->IsExistPvpNpcName( kPacket_.m_wstrNickName ) == true )
	{
		KEGS_CHANGE_NICK_NAME_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_01;
		SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
		return;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}


#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		if(kPacket_.m_bCheckOnly == false)
		{
			KEGS_CREATE_UNIT_REQ kReq2;
			kReq2.m_wstrNickName = kPacket_.m_wstrNickName;
			kReq2.m_iClass = (int)GetUnitClass();

			m_bNickNameCheckOnly = kPacket_.m_bCheckOnly;

			KEGIANT_ROLEREG_CREATE_UNIT_REQ kReq;
			kReq.m_iActorUID = GetUID();
			kReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
			kReq.m_CreateUnitReqPacket = kReq2;

			m_iBackUpUnitUID = kPacket_.m_iUnitUID;

			SendToGiantRoleReg( EGIANT_ROLEREG_CREATE_UNIT_REQ, kReq );
		}
		else
		{
			m_bNickNameCheckOnly = kPacket_.m_bCheckOnly;
			SendToGameDB( DBE_CHANGE_NICK_NAME_REQ, kPacket_ );
		}
	}
	else
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	{
		SendToGameDB( DBE_CHANGE_NICK_NAME_REQ, kPacket_ );
	}
}

_IMPL_ON_FUNC( DBE_CHANGE_NICK_NAME_ACK, KEGS_CHANGE_NICK_NAME_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CHANGE_NICK_NAME_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// �г��� ������ �����ߴٸ� �α׸� ������!
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
        KELOG_CHANGE_NICK_NAME_NOT kPacketLog;
		kPacketLog.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacketLog.m_wstrNickName = kPacket_.m_wstrNickName;
		SendToLogDB( ELOG_CHANGE_NICK_NAME_NOT, kPacketLog );
	}
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	//{{	//2011.10.31 lygan_������ // �÷��� ���ϼ� ������ ��������� DB���� ������ ���� ��� ���ϸ� �÷����� �г��� ������� �Ѵ�
	else // 2011.10.31 lygan_������ // �÷��������� ���� ó���� �ϱ� ���� �÷��� ǥ�÷� ����
	{
		KEGIANT_ROLEREG_DELETE_UNIT_REQ kPacketReq;
		kPacketReq.m_wstrNickName = kPacket_.m_wstrNickName;
		kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_UNIT_REQ, kPacketReq );
	}
	//}}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

    SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_CHANGE_UNIT_CLASS_ACK )
{
	// ���� ���� ����
	KEGS_CHANGE_JOB_CASH_ITEM_NOT kPacket;

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
		// ���� ó��
		SetUnitClass( kPacket_.m_cUnitClass );
		ResetStat();
		m_kSkillTree.SetUnitClass( (int) GetUnitClass() );
		GetUnitInfo( kPacket.m_kUnitInfo );

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

		// �⺻ ��ų �߰�
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // ����ö // ���� ���� ��, ��� ��ų �������� ���� �ǵ��� ��
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iNewDefaultSkill1, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( kPacket_.m_iNewDefaultSkill2, 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iNewDefaultSkill1, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iNewDefaultSkill2, 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM

#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		// ���� �α� �����
        KDBE_UPDATE_UNIT_CLASS_NOT kPacketNot;
        kPacketNot.m_iUnitUID = GetCharUID();
        kPacketNot.m_cUnitClass = GetUnitClass();
        SendToLogDB( DBE_UPDATE_UNIT_CLASS_NOT, kPacketNot );

		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
		{
			WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_JOB_CHANGE );
		}
		else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
		{
			WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_SECOND_JOB_CHANGE );
		}
		else
		{
			if( GetAuthLevel() < SEnum::UAL_GM )
			{
				START_LOG( cerr, L"1�� ������ 2�� ������ �ƴѵ� �α׸� ����Ϸ��� �ϳ�?" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOGc( GetUnitClass() )
					<< END_LOG;
			}
			else
			{
				START_LOG( cout, L"��ڰ� �⺻�������� ġƮ�豺.. �α� ��� ���� ����." )
					<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOGc( GetUnitClass() );
			}
		}

#endif SERV_CHAR_LOG
		//}}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		KEGS_CHANGE_MY_UNIT_INFO_NOT kMyInfoNot;
		kMyInfoNot.m_cUnitClass = GetUnitClass();
		kMyInfoNot.m_iNewDefaultSkill_1 = kPacket_.m_iNewDefaultSkill1;
		kMyInfoNot.m_iNewDefaultSkill_2 = kPacket_.m_iNewDefaultSkill2;
		SendPacket( EGS_CHANGE_MY_UNIT_INFO_NOT, kMyInfoNot );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			// �ʵ忡 ������� ���� ���� �����ϱ�
			if( GetFieldUID() > 0 )
			{
				KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
				GetFieldUserInfo( kNot.m_kInfo );
				SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
			}
		}		
		else if( GetStateID() == KGSFSM::S_ROOM )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
			//{{ ��Ʋ�ʵ� : �ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
				{
					KRoomUserInfo kRoomUserInfo;
					GetRoomUserInfo( kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				{
					KSquareUserInfo kSquareUserInfo;
					GetSquareUserInfo( kSquareUserInfo );
					SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
				}
				break;

			default:
				{
					START_LOG( cerr, L"�̻��� �� Ÿ��." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}
		}

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}

		//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
		SendUpdateUnitInfoToRelayDB( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT::UT_UNITCLASS_CHANGE );
#endif SERV_RELAY_DB_CONNECTION
		//}}
    }

	switch( kPacket_.m_usEventID )
	{
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		case EGS_ADMIN_CHANGE_UNIT_CLASS_REQ:
		{
			// ���� ġƮ
			KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacketAck;
			kPacketAck.m_iOK = kPacket_.m_iOK;
			kPacketAck.m_cUnitClass = kPacket_.m_cUnitClass;	
			SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacketAck );
		}
		break;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	case EGS_BUY_CASH_ITEM_REQ:
		{
			// ���� ĳ�� ���Ÿ� ������ ���θ�� ť�긦 �������� �����ش�!
			{
				CStringW cwstrItemID;
				cwstrItemID.Format( L"%d", CXSLItem::GetCashItemByUnitClass( GetUnitClass() ) );

				//{{ 2009. 11. 10  ������	2������
				int iPromotionCubeItemID = 0;
				if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
				{
#ifdef SERV_PRESENT_SKILL_INIT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
					iPromotionCubeItemID = _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iChangeSecondJob;
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
					iPromotionCubeItemID = 10555;
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
#else // SERV_PRESENT_SKILL_INIT_ITEM
					iPromotionCubeItemID = CXSLItem::SI_SECOND_CHANGE_JOB_CUBE;
#endif // SERV_PRESENT_SKILL_INIT_ITEM
				}
				else
				{
#ifdef SERV_PRESENT_SKILL_INIT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
					iPromotionCubeItemID = _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iChangeFirstJob;
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
					iPromotionCubeItemID = 10554;
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
#else // SERV_PRESENT_SKILL_INIT_ITEM
					iPromotionCubeItemID = CXSLItem::SI_FIRST_CHANGEJOB_CUBE;
#endif // SERV_PRESENT_SKILL_INIT_ITEM
				}
				//}}
#ifdef SERV_EVENT_CHANGE_CLASS
				IF_EVENT_ENABLED( CEI_EVENT_CHANGE_CLASS )
				{
					if( CXSLUnit::UC_ARME_BATTLE_MAGICIAN == GetUnitClass() )		
					{
						//�ý�����Ʈ�� ���� ����� ������ ����
						KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
						kPacketToDB.m_iFromUnitUID = GetCharUID();
						kPacketToDB.m_iToUnitUID   = GetCharUID();
						kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
						kPacketToDB.m_iRewardID    = _CONST_EVENT_CHANGE_CLASS_ITEM_INT_::iTransFormItem; //��Ʈ������ ť��(��Ʋ������) ���̵�� �ٲ����
						SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
					}
				}
#endif SERV_EVENT_CHANGE_CLASS

				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
#ifdef SERV_PRESENT_SKILL_INIT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
#else // SERV_PRESENT_SKILL_INIT_ITEM
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_MESSAGE;
#endif // SERV_PRESENT_SKILL_INIT_ITEM
				kPacketToDB.m_iRewardID	   = iPromotionCubeItemID;
				kPacketToDB.m_sQuantity	   = 1;
				kPacketToDB.m_wstrMessage  = cwstrItemID.GetBuffer();
				kPacketToDB.m_bGameServerEvent = false;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}

#ifdef SERV_ARA_FIRST_CLASS_CHANGE_EVENT
			IF_EVENT_ENABLED( CEI_ARA_FIRST_CLASS_CHANGE_EVENT )
			{
				if( CXSLUnit::UC_ARA_LITTLE_HSIEN == GetUnitClass() )
				{
					KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
					kPacketToDB.m_iFromUnitUID = GetCharUID();
					kPacketToDB.m_iToUnitUID   = GetCharUID();
					kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
					kPacketToDB.m_iRewardID	   = 1554;	// �Ҽ��� ���� Ư�� ���� ť��B
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				}
			}
#endif SERV_ARA_FIRST_CLASS_CHANGE_EVENT

#ifdef SERV_ARA_NEW_FIRST_CLASS_EVENT
			IF_EVENT_ENABLED( CEI_ARA_NEW_FIRST_CLASS_EVENT )
			{
				if( CXSLUnit::UC_ARA_LITTLE_DEVIL == GetUnitClass() )
				{
					KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
					kPacketToDB.m_iFromUnitUID = GetCharUID();
					kPacketToDB.m_iToUnitUID   = GetCharUID();
					kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
					kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_FIRST_CLASS_EVENT_REWARD_ID::iClassChangeReward;
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				}
			}
#endif SERV_ARA_NEW_FIRST_CLASS_EVENT

#ifdef SERV_ARA_NEW_SECOND_CLASS_EVENT
			IF_EVENT_ENABLED( CEI_ARA_NEW_SECOND_CLASS_EVENT )
			{
				if( CXSLUnit::UC_ARA_YAMA_RAJA == GetUnitClass() )
				{
					KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
					kPacketToDB.m_iFromUnitUID = GetCharUID();
					kPacketToDB.m_iToUnitUID   = GetCharUID();
					kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
					kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_SECOND_CLASS_EVENT_REWARD_ID::iAraNewSecondClassChangeReward;
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				}

				if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
				{
					// �̺�Ʈ ������ ����!
					KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
					kPacketToDB.m_iFromUnitUID = GetCharUID();
					kPacketToDB.m_iToUnitUID   = GetCharUID();
					kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
					kPacketToDB.m_iRewardID	   = _CONST_ARA_NEW_SECOND_CLASS_EVENT_REWARD_ID::iSecondClassChangeReward;
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				}
			}			
#endif SERV_ARA_NEW_SECOND_CLASS_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
				///���������� �ش� UI�� �ٲ��־�� �ϴϱ� ���⼭ ��Ŷ�� ����
				///�������� �޾Ҵ� �ȹ޾Ҵ� ��Ŷ�� ��� DB�� ������ �ð��� �����������
				if( CXSLUnit::UC_CHUNG_FURY_GUARDIAN == GetUnitClass()
					|| CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN == GetUnitClass() 
					|| CXSLUnit::UC_CHUNG_IRON_PALADIN == GetUnitClass()
					|| CXSLUnit::UC_CHUNG_DEADLY_CHASER == GetUnitClass() )
				{
					CTime cNowTime = GetCurrentTime();
					KEGS_EVENT_CHUNG_GIVE_ITEM_NOT kPacketNot;
					kPacketNot.m_bGiveItemGet = false;
					kPacketNot.m_wstrToolTipTime = cNowTime.Format(L"%Y-%m-%d %H:%M:%S");
					kPacketNot.m_cGetUnitClass = GetUnitClass();
					kPacketNot.m_bTwoGiveItem = false;
					SendPacket(EGS_EVENT_CHUNG_GIVE_ITEM_NOT,kPacketNot);
				}		
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_ELESIS_NEW_FIRST_CLASS_EVENT
			IF_EVENT_ENABLED( CEI_ELESIS_NEW_FIRST_CLASS_EVENT )
			{
				if( CXSLUnit::UC_ELESIS_SABER_KNIGHT == GetUnitClass() || CXSLUnit::UC_ELESIS_PYRO_KNIGHT == GetUnitClass() )
				{
					KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
					kPacketToDB.m_iFromUnitUID = GetCharUID();
					kPacketToDB.m_iToUnitUID   = GetCharUID();
					kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
					kPacketToDB.m_iRewardID	   = _CONST_ELESIS_NEW_FIRST_CLASS_EVENT_REWARD_ID::iClassChangeReward;
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				}
			}
#endif SERV_ELESIS_NEW_FIRST_CLASS_EVENT

			// ���� ĳ�� ������
			SendPacket( EGS_CHANGE_JOB_CASH_ITEM_NOT, kPacket );
		}
		break;

	default:
		{
			START_LOG( cerr, L"�߸��� �̺�Ʈ�Դϴ�." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_usEventID )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC( EGS_RECOMMEND_USER_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_RECOMMEND_USER_REQ, EGS_RECOMMEND_USER_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
	{
		KEGS_RECOMMEND_USER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_01;
		SendPacket( EGS_RECOMMEND_USER_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//{{ 2008. 5. 16  ������  ü��ID ����

	// �����ҿ� �����ϰ� ó��..

#ifndef SERV_NO_GAMEINVITE
	// ��� ��Ҹ� �� ���
	if( kPacket_.m_bCancel  ||  IsGuestUser() )
#endif SERV_NO_GAMEINVITE
	{
		// �̹� ��� ó��
		m_kUserRecommendManager.SetIsRecommend( true );

		KEGS_RECOMMEND_USER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		return;
	}

	// �ڱ� �ڽ��� ��õ�� ���.
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(GetCharName(), kPacket_.m_wstrNickName) )
#else
	if( GetCharName().compare( kPacket_.m_wstrNickName ) == 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_RECOMMEND_USER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_01;
		SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
	{
		KEGS_RECOMMEND_USER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		return;
	}

	//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// ���볯¥: 2013-07-04
	KDBE_RECOMMEND_USER_GET_NEXON_SN_REQ kPacket;
	kPacket.m_iRecommendUserNexonSN = GetNexonSN();
	kPacket.m_iRecommendUnitUID = GetCharUID();
	kPacket.m_wstrRecommendedUserNickName = kPacket_.m_wstrNickName;
	SendToGameDB( DBE_RECOMMEND_USER_GET_NEXON_SN_REQ, kPacket );
#else	// SERV_RECOMMEND_LIST_EVENT_2013_07
	
#ifdef SERV_RECOMMEND_LIST_EVENT
	if( GetChannelCode() != KNexonAccountInfo::CE_NEXON_ACCOUNT )
	{
		KEGS_RECOMMEND_USER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_09;
		SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		return;
	}

	KDBE_RECOMMEND_USER_GET_NEXON_SN_REQ kPacket;
	kPacket.m_iRecommendUserNexonSN = GetNexonSN();
	kPacket.m_iRecommendUnitUID = GetCharUID();
	kPacket.m_wstrRecommendedUserNickName = kPacket_.m_wstrNickName;
	SendToGameDB( DBE_RECOMMEND_USER_GET_NEXON_SN_REQ, kPacket );
#else
	KDBE_RECOMMEND_USER_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendToGameDB( DBE_RECOMMEND_USER_REQ, kPacket );
#endif SERV_RECOMMEND_LIST_EVENT
	//}
#endif SERV_RECOMMEND_LIST_EVENT_2013_07

}

//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT
IMPL_ON_FUNC( DBE_RECOMMEND_USER_GET_NEXON_SN_ACK )
{
	if( kPacket_.m_iRecommendedUserNexonSN <= 0 )
	{
		START_LOG( cerr, L"��õ �޴� ĳ������ �ؽ� OID �� ����." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iRecommendedUserNexonSN )
			<< END_LOG;

		KEGS_RECOMMEND_USER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_00;
		SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		return;
	}

	// kPacket_.m_iRecommendedUserNexonSN  �� 0 ���� ũ�ٸ� ���������� �޾� �°��̴�.

#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// ���볯¥: 2013-07-04
	// �� ������ �ؽ��� ��쿡�� soap üũ�Ϸ� ����
	if( GetChannelCode() == KNexonAccountInfo::CE_NEXON_ACCOUNT && kPacket_.m_iRecommendedUserChannelCode == KNexonAccountInfo::CE_NEXON_ACCOUNT )
	{
		KSOAP_CHECK_SAME_USER_REQ kPacketToSoap;

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
		kPacketToSoap.m_iServiceCode	= KNexonSOAP::NSC_ELSWORD;
#else // SERV_NEXON_COUPON_SYSTEM
		kPacketToSoap.m_iServiceCode	= 5059;
#endif // SERV_NEXON_COUPON_SYSTEM
		kPacketToSoap.m_iNexonSN_A		= ( __int64 )kPacket_.m_iRecommendUserNexonSN;
		kPacketToSoap.m_iNexonSN_B		= ( __int64 )kPacket_.m_iRecommendedUserNexonSN;
		kPacketToSoap.m_iUnitUID		= kPacket_.m_iRecommendUnitUID;
		kPacketToSoap.m_wstrNickName	= kPacket_.m_wstrRecommendedUserNickName;

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData(PI_NULL, anTrace, SOAP_CHECK_SAME_USER_REQ, kPacketToSoap );

		SiKNexonSOAPManager()->QueueingEvent( spEvent );
	}
	else
	{
		KDBE_RECOMMEND_USER_REQ kPacket;
		kPacket.m_iUnitUID = GetCharUID();
		kPacket.m_wstrNickName = kPacket_.m_wstrRecommendedUserNickName;
		SendToGameDB( DBE_RECOMMEND_USER_REQ, kPacket );
	}
#else	// SERV_RECOMMEND_LIST_EVENT_2013_07
	KSOAP_CHECK_SAME_USER_REQ kPacketToSoap;

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-17	// �ڼ���
	kPacketToSoap.m_iServiceCode	= KNexonSOAP::NSC_ELSWORD;
#else // SERV_NEXON_COUPON_SYSTEM
	kPacketToSoap.m_iServiceCode	= 5059;
#endif // SERV_NEXON_COUPON_SYSTEM
	kPacketToSoap.m_iNexonSN_A		= ( __int64 )kPacket_.m_iRecommendUserNexonSN;
	kPacketToSoap.m_iNexonSN_B		= ( __int64 )kPacket_.m_iRecommendedUserNexonSN;
	kPacketToSoap.m_iUnitUID		= kPacket_.m_iRecommendUnitUID;
	kPacketToSoap.m_wstrNickName	= kPacket_.m_wstrRecommendedUserNickName;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_NULL, anTrace, SOAP_CHECK_SAME_USER_REQ, kPacketToSoap );

	SiKNexonSOAPManager()->QueueingEvent( spEvent );
#endif SERV_RECOMMEND_LIST_EVENT_2013_07
}

IMPL_ON_FUNC( SOAP_CHECK_SAME_USER_ACK )
{
	KEGS_RECOMMEND_USER_ACK kPacketAck;
	
	switch( kPacket_.m_iOK )
	{
	case -1:
	case 1:						// DB ����
	case 5:						// �������� soap ������ �ƴ�
	case 100: case 101:			// input ���� ��ȿ���� ����
	case 4000:					// �������� �ʴ� ����
		{
			kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_10;
			SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		}break;
	case 0:
		{
			kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_08;
			SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		}break;
	case 5034:
		{
			kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_09;
			SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		}break;
	case 5031:case 5039:
		{
			KDBE_RECOMMEND_USER_REQ kPacket;
			kPacket.m_iUnitUID = GetCharUID();
			kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
			SendToGameDB( DBE_RECOMMEND_USER_REQ, kPacket );
		}break;
	default:
		{
			kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_10;
			SendPacket( EGS_RECOMMEND_USER_ACK, kPacketAck );
		}
	}

	START_LOG( clog, L"��õ�ϱ� soap ȣ�� ���� ���� Ȯ��" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( kPacket_.m_wstrNickName )
		<< BUILD_LOG( kPacket_.m_iOK )
		<< END_LOG;
}
#endif SERV_RECOMMEND_LIST_EVENT
//}


IMPL_ON_FUNC( DBE_RECOMMEND_USER_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_RECOMMEND_USER_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_RECOMMEND_USER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ��õ�� UnitUID ���
		m_kUserRecommendManager.SetRecommendedUnitUID( kPacket_.m_iRecommendedUnitUID );

		// �̹� ��� ó��
		m_kUserRecommendManager.SetIsRecommend( true );

		// ��õ ������ ��Ȱ�� 5�� ����
        m_iNumResurrectionStone += 5;
		kPacket.m_iRewardResurrectionStone = 5;

		CTime tCurTime = CTime::GetCurrentTime();
		std::wstring wstrCurTime = ( CStringW )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		// ��õ �޽����� ��õ�ο��� ����.
		KEGS_RECOMMEND_USER_RESULT_NOT kNot;
		kNot.m_iRecommendedUnitUID	= kPacket_.m_iRecommendedUnitUID;
		kNot.m_iRecommendUnitUID	= GetCharUID();
		kNot.m_wstrFromNickName		= GetCharName();
		kNot.m_ucLevel				= GetLevel();
		kNot.m_wstrRecommendDate	= wstrCurTime;
		SendToLoginServer( ELG_RECOMMEND_USER_RESULT_NOT, kNot );

		//{{ 2013. 03. 21	 ��õ�� �ý��� ���� - ��μ�
#ifdef SERV_RECOMMEND_LIST_EVENT
		SendToAccountDB( DBE_RECOMMEND_USER_NOT );

		// ��õ�� ����
		int iRecommendReward = SiKRecommendUserTable()->GetRecommendEventReward();	// ���ҵ� ���谡�� ��¡ ť��
		if( iRecommendReward > 0 )
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID	= GetCharUID();
			kPacketToDB.m_iToUnitUID	= GetCharUID();
			kPacketToDB.m_iRewardType	= KPostItemInfo::LT_EVENT; // ���� Ÿ��
			kPacketToDB.m_iRewardID		= iRecommendReward;

			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
		// ��õ���� ���� - ���� 1
		int iRecommendedReward1 = SiKRecommendUserTable()->GetRecommendedEventReward1();	// ���ҵ� ���谡�� ���� ť��
		if( iRecommendedReward1 > 0 )
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID	= GetCharUID();
			kPacketToDB.m_iToUnitUID	= kPacket_.m_iRecommendedUnitUID;
			kPacketToDB.m_iRewardType	= KPostItemInfo::LT_EVENT; // ���� Ÿ��
			kPacketToDB.m_iRewardID		= iRecommendedReward1;

			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
		// ��õ���� ���� - ���� 2
		int iRecommendedReward2 = SiKRecommendUserTable()->GetRecommendedEventReward2();	// ���ҵ� ���谡�� ��ǥ
		if( iRecommendedReward2 > 0 )
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID	= GetCharUID();
			kPacketToDB.m_iToUnitUID	= kPacket_.m_iRecommendedUnitUID;
			kPacketToDB.m_iRewardType	= KPostItemInfo::LT_EVENT; // ���� Ÿ��
			kPacketToDB.m_iRewardID		= iRecommendedReward2;

			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
#endif SERV_RECOMMEND_LIST_EVENT
		//}
	}

	SendPacket( EGS_RECOMMEND_USER_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_RECOMMEND_USER_RESULT_NOT, KEGS_RECOMMEND_USER_RESULT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2010. 01. 11  ������	��õ�θ���Ʈ
	// �ʱ�ȭ �Ǿ� �������� �߰�����!
	if( m_kUserRecommendManager.IsInitRecommendUserList() == true )
	{
		KRecommendUserInfo kInfo;
		kInfo.m_iUnitUID			= kPacket_.m_iRecommendUnitUID;
		kInfo.m_wstrNickName		= kPacket_.m_wstrFromNickName;
		kInfo.m_ucLevel				= kPacket_.m_ucLevel;
		kInfo.m_wstrRecommendDate	= kPacket_.m_wstrRecommendDate;
		kInfo.m_bIsOnline			= true;
        m_kUserRecommendManager.AddRecommendUser( kInfo );
	}
	//}}

	SendPacket( EGS_RECOMMEND_USER_RESULT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_RESTORE_SPIRIT_NOT, KEGS_RESTORE_SPIRIT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_RESTORE_SPIRIT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DEL_TUTORIAL_REQ )
{
	//���´� �����غ��� ���Ŀ� ����..
	if( m_kUserTutorialInfo.OnDelTutorialReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"�������� ���� ����.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_DEL_TUTORIAL_ACK )
{
	//���´� �����غ��� ���Ŀ� ����..
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( m_kUserTutorialInfo.OnDelTutorialAck( GetThisPtr<KGSUser>(), kPacket_ ) == false )
		{
			START_LOG( cerr, L"��������Ʈ ��������.!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kPacket_.m_cReason) ) )
				<< BUILD_LOG( kPacket_.m_iTeacherUID )
				<< BUILD_LOG( kPacket_.m_iStudentUID )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"�������� ��� ����� ����!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT )
{
	//���´� �����غ��� ���Ŀ� ����..
	if( SiKTutorialManager()->IsExistbylist( GetCharUID() ) == true )
	{
		//������ ����� �����̹Ƿ� ����Ʈ���� ���ش�.
		if( SiKTutorialManager()->DeleteUnit( GetCharUID() ) == false )
		{
			START_LOG( cerr, L"���丮�� ���ڸ���Ʈ ���ֻ��� ����.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
		}
	}

    if( m_kUserTutorialInfo.OnTutorialUpdateUnitInfoNot( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"�������� ������Ʈ ����.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_wstrNickName )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ELG_DEL_TUTORIAL_NOT, KEGS_DEL_TUTORIAL_NOT )
{
	//���´� �����غ��� ���Ŀ� ����..

	if( m_kUserTutorialInfo.OnDelTutorialNot( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"��������Ʈ ����� ����.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_TUTORIAL_STUDENT_LIST_REQ )
{
	//VERIFY_STATE( ( 1, KGSFSM::S_VILLAGE_MAP ) );

	switch( GetStateID() )
	{
	case KGSFSM::S_SERVER_SELECT:
		{
			START_LOG( cerr, L"���� ������Ʈ�� �߸��Ǿ����ϴ�." )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
			return;
		}
	}

	KEGS_TUTORIAL_STUDENT_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//���� ���� �̻��̿��� ��û�Ҽ�����.
	if( GetLevel() >= KTutorialManager::UNIT_LEVEL_TEACHER_MIN )
	{
		if( SiKTutorialManager()->GetTutorPageInfo( kPacket_, kPacket ) == true )
		{
			goto end_proc;
		}

		kPacket.m_iOK = NetError::ERR_TUTORIAL_01;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_TUTORIAL_02;
	}

end_proc:
	SendPacket( EGS_TUTORIAL_STUDENT_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_REQUEST_TUTORIAL_REQ )
{
	//���´� �����غ��� ���Ŀ� ����..

	//����Ŷ�� ������ ������ �������ִ�.
	KEGS_REQUEST_TUTORIAL_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

#ifdef SERV_NO_DISCIPLE
	kPacket.m_iOK = NetError::ERR_TUTORIAL_02;
	SendPacket( EGS_REQUEST_TUTORIAL_ACK, kPacket );
	return;
#endif SERV_NO_DISCIPLE

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_REQUEST_TUTORIAL_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( IsSameServerGroupUID( kPacket_.m_iUnitUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_03;
		SendPacket( EGS_REQUEST_TUTORIAL_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif // DEPRECATED_SERVER_GROUP_MASK

	if( m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER )
	{
		if( m_kUserTutorialInfo.GetTutorialUnitNum() < KTutorialManager::STUDENT_NUM &&
			m_kUserTutorialInfo.IsExistUnit( kPacket_.m_iUnitUID ) == false )
		{
			KELG_REQUEST_TUTORIAL_NOT kNot;
			kNot.m_iReceiverUnitUID = kPacket_.m_iUnitUID;

			kNot.m_kTutorialUnitInfo.m_bIsOnline = true;
			kNot.m_kTutorialUnitInfo.m_iUnitUID = GetCharUID();
			kNot.m_kTutorialUnitInfo.m_ucLevel = GetLevel();
			kNot.m_kTutorialUnitInfo.m_wstrNickName = GetCharName();

			SendToLoginServer( ELG_REQUEST_TUTORIAL_NOT, kNot );
		}
		else
		{
			if( m_kUserTutorialInfo.GetTutorialUnitNum() >= KTutorialManager::STUDENT_NUM )
			{
				kPacket.m_iOK = NetError::ERR_TUTORIAL_04;
			}
			else
			{
				kPacket.m_iOK = NetError::ERR_TUTORIAL_06;
			}
		}
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_TUTORIAL_03;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( clog, L"���� �α� ����.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( (int)GetLevel() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			;
	}

	SendPacket( EGS_REQUEST_TUTORIAL_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_REQUEST_TUTORIAL_NOT )
{
	//���´� �����غ��� ���Ŀ� ����..

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		// �׳� ������..
		goto end_proc;
	}

	//{{ 2009. 3. 24  ������	GM����
	if( GetAuthLevel() == SEnum::UAL_GM )
	{
		// �׳� ������..
		goto end_proc;
	}
	//}}

	//������ ���ڿ��� �������� ��û�� ���������..
	if( m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_STUDENT &&
		GetLevel() < KTutorialManager::UNIT_LEVEL_STUDENT_MAX ) //<< ����� �����ɶ����� �־�д�.
	{
		//�������� �ƴϿ��� ��..
		if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() != 0 )
		{
			KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( GetRoomUID() );
			if( spRoomInfo != NULL )
			{
				if( spRoomInfo->m_RoomState == CXSLRoom::RS_PLAY )
					goto end_proc;
			}
		}

		//������ ����� ��.
		if( m_kUserTutorialInfo.IsExistbyTutorData() == false )	
		{
			KEGS_REQUEST_TUTORIAL_NOT kNot;
			kNot = kPacket_.m_kTutorialUnitInfo;

			SendPacket( EGS_REQUEST_TUTORIAL_NOT, kNot );
			return;
		}
		else
		{
			START_LOG( clog, L"�̹� ������ ����.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( (int)GetLevel() )
				<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_wstrNickName )
				<< BUILD_LOG( (int)kPacket_.m_kTutorialUnitInfo.m_ucLevel )
				<< END_LOG;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"�������迡�� ���ڿ��� �ϴµ�..?��?" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( (int)GetLevel() )
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_wstrNickName )
			<< BUILD_LOG( (int)kPacket_.m_kTutorialUnitInfo.m_ucLevel )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	//�̰��� ���ڰ� �ƴϰų� �̹� ������ �ִ°��.
	KEGS_REQUEST_TUTORIAL_REPLY_NOT kReplyNot;
	kReplyNot.m_bAccept = false;
	kReplyNot.m_iReceiverUnitUID = kPacket_.m_kTutorialUnitInfo.m_iUnitUID;

	kReplyNot.m_kTutorialUnitInfo.m_bIsOnline = true;
	kReplyNot.m_kTutorialUnitInfo.m_iUnitUID = GetCharUID();
	kReplyNot.m_kTutorialUnitInfo.m_ucLevel = GetLevel();
	kReplyNot.m_kTutorialUnitInfo.m_wstrNickName = GetCharName();

	SendToLoginServer( ELG_REQUEST_TUTORIAL_REPLY_NOT, kReplyNot );
}

IMPL_ON_FUNC( EGS_REQUEST_TUTORIAL_REPLY_NOT )
{
#ifdef SERV_NO_DISCIPLE
	return;
#endif SERV_NO_DISCIPLE

	//���´� �����غ��� ���Ŀ� ����..

	//���ڰ� �������踦 ������ ���θ� ������ ������ �˸��� ��Ŷ..
	if( m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_STUDENT )
	{
		//������ ����� ��.
		if( m_kUserTutorialInfo.IsExistbyTutorData() == false )	
		{
			//�ڽ��ڽ��� ������ ���⶧���� ä���� �����ش�. 
			kPacket_.m_kTutorialUnitInfo.m_bIsOnline = true;
			kPacket_.m_kTutorialUnitInfo.m_iUnitUID = GetCharUID();
			kPacket_.m_kTutorialUnitInfo.m_ucLevel = GetLevel();
			kPacket_.m_kTutorialUnitInfo.m_wstrNickName = GetCharName();

			SendToLoginServer( ELG_REQUEST_TUTORIAL_REPLY_NOT, kPacket_ );

			//if( SiKTutorialManager()->IsExistbylist( GetCharUID() ) == true )
			//{
			//	//������ ����� �����̹Ƿ� ����Ʈ���� ���ش�.
			//	if( SiKTutorialManager()->DeleteUnit( GetCharUID() ) == false )
			//	{
			//		START_LOG( cerr, L"���丮�� ���ڸ���Ʈ ���ֻ��� ����.!" )
			//			<< BUILD_LOG( GetCharUID() )
			//			<< BUILD_LOG( GetCharName() )
			//			<< END_LOG;
			//	}
			//}

			return;
		}
	}

	START_LOG( cwarn, L"�������踦 �����ϴ� ������ Ʋ���ų� ������ ���� ����ų�.!" )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( (int)GetLevel() )
		<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
		<< END_LOG;
}

_IMPL_ON_FUNC( ELG_REQUEST_TUTORIAL_REPLY_NOT, KEGS_REQUEST_TUTORIAL_REPLY_NOT )
{
	//���´� �����غ��� ���Ŀ� ����..
	if( m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER )
	{
		if( m_kUserTutorialInfo.GetTutorialUnitNum() < KTutorialManager::STUDENT_NUM )
		{
			if( kPacket_.m_bAccept == true )	//���ڰ� �����Ѱ��.
			{
				//��� �������踦 ������ �������� ���������� �׻� �����̴�.
				KDBE_INSERT_TUTORIAL_REQ	kPacket;
				kPacket.m_iTeacherUID = GetCharUID();
				kPacket.m_kStudentUnitInfo = kPacket_.m_kTutorialUnitInfo;

				SendToGameDB( DBE_INSERT_TUTORIAL_REQ, kPacket );
			}
			else
			{
				SendPacket( EGS_REQUEST_TUTORIAL_REPLY_NOT, kPacket_ );
			}

			return;
		}
	}

	START_LOG( cerr, L"���� �������踦 �����ҷ��� �ϴ� ������ ���¼����� �̻���." )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( (int)m_kUserTutorialInfo.GetTutorUnitType() )
		<< BUILD_LOG( m_kUserTutorialInfo.GetTutorialUnitNum() )
		<< END_LOG;
}

IMPL_ON_FUNC( DBE_INSERT_TUTORIAL_ACK )
{
	//���´� �����غ��� ���Ŀ� ����..

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KEGS_REQUEST_TUTORIAL_REPLY_NOT kNot;
		kNot.m_iReceiverUnitUID = GetCharUID();
		kNot.m_kTutorialUnitInfo = kPacket_.m_kStudentUnitInfo;
		kNot.m_bAccept = true;

		m_kUserTutorialInfo.OnInsertTutorialAck( GetThisPtr<KGSUser>(), kNot.m_kTutorialUnitInfo );

		SendPacket( EGS_REQUEST_TUTORIAL_REPLY_NOT, kNot );

		//�������踦 �ξ����� ��������� ��� ���������� ������Ʈ ��.
		if( GetRoomUID() == 0 )
			return;
			
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				if( m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER  &&  m_kUserTutorialInfo.IsExistbyTutorData() == true )
				{
					KERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT kUpdateNot;
					m_kUserTutorialInfo.GetTutorUnitUIDList( kUpdateNot.m_vecStudentUnitUID );
					SendToCnRoom( ERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT, kUpdateNot );
				}
			}
			break;
			
		default:
			break;
		}

		return;
	}

	START_LOG( cerr, L"����ó�� ����.!" )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_kStudentUnitInfo.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_kStudentUnitInfo.m_wstrNickName )
		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
		<< END_LOG;
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

IMPL_ON_FUNC( EGS_GET_SKILL_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_SKILL_REQ, EGS_GET_SKILL_ACK );

	KEGS_GET_SKILL_ACK kPacket;
	KDBE_INSERT_SKILL_REQ kDBReq;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_GET_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		goto end_proc;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// ����� ��ų�� �ִ°�?
	if( kPacket_.m_mapSkillList.empty() == true )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_09;
		goto end_proc;
	}

	// ���� �˻�
	if( m_kSkillTree.CheckGetNewSkill( kPacket_.m_mapSkillList, static_cast<int>(GetUnitClass()), GetLevel(), kPacket ) == false )
	{
		goto end_proc;
	}

	// CSP�Ⱓ �������� Ȯ��
	if( m_kSkillTree.IsCashSkillPointExpired() == true )
	{
		// ����� SP�� �������ִ���?
#ifdef SERV_SKILL_PAGE_SYSTEM
		if ( m_kSkillTree.GetSPoint() <= 0 )
#else // SERV_SKILL_PAGE_SYSTEM
		if( m_iSPoint <= 0 )
#endif // SERV_SKILL_PAGE_SYSTEM
		{
			kPacket.m_iOK = NetError::ERR_SKILL_17;
			goto end_proc;
		}
	}
	else
	{
		// ����� SP�� CSP�� ������ �ִ���?
#ifdef SERV_SKILL_PAGE_SYSTEM
		if( m_kSkillTree.GetSPoint() + m_kSkillTree.GetCSPoint() <= 0 )
#else // SERV_SKILL_PAGE_SYSTEM
		if( m_iSPoint + m_kSkillTree.GetCSPoint() <= 0 )
#endif // SERV_SKILL_PAGE_SYSTEM
		{
			kPacket.m_iOK = NetError::ERR_SKILL_18;
			goto end_proc;
		}
	}

	// ������ ��ų ������� �ʿ��� sp �� ���
	int iTotalSP = 0;	// ��ų�� ���µ� �ʿ��� �Ϲ� ��ų ����Ʈ
	int iTotalCSP = 0;	// ��ų�� ���µ� �ʿ��� ĳ�� ��ų ����Ʈ

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( m_kSkillTree.GetNecessarySkillPoint( kPacket_.m_mapSkillList, iTotalSP, iTotalCSP, 
		m_kSkillTree.AccessLearnedSkillTree() ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_17;
		goto end_proc;
	}

#else // SERV_SKILL_PAGE_SYSTEM
	if( m_kSkillTree.GetNecessarySkillPoint( kPacket_.m_mapSkillList, iTotalSP, iTotalCSP ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_17;
		goto end_proc;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// sp �� ��� �Ѱ�?
#ifdef SERV_SKILL_PAGE_SYSTEM
	if( iTotalSP > m_kSkillTree.GetSPoint() || iTotalCSP > m_kSkillTree.GetCSPoint() )
#else // SERV_SKILL_PAGE_SYSTEM
	if( iTotalSP > m_iSPoint || iTotalCSP > m_kSkillTree.GetCSPoint() )
#endif // SERV_SKILL_PAGE_SYSTEM
	{
		kPacket.m_iOK = NetError::ERR_SKILL_18;
		goto end_proc;
	}

	// ĳ�� ��ų�� ����(�Ⱓ ����)
	if( m_kSkillTree.IsCashSkillPointExpired() == true )
	{
		if( iTotalCSP > 0 )
		{
			kPacket.m_iOK = NetError::ERR_SKILL_24;
			goto end_proc;
		}
	}
	else
	{
		if( m_kSkillTree.GetCSPoint() > 0 )
		{
			kDBReq.m_iCSPoint = m_kSkillTree.GetCSPoint() - iTotalCSP;
		}
		else
		{
			kDBReq.m_iCSPoint = -1;	// invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
		}
	}

	{
		std::map< int, KGetSkillInfo >::iterator mit = kPacket_.m_mapSkillList.begin();
		for( ; mit != kPacket_.m_mapSkillList.end() ; ++mit )
		{
			int iSkillLevel = 0;
			int iSkillCSPoint = 0;	// ��� ��ų �� ���ڵ� csp ���
						
#ifdef SERV_SKILL_PAGE_SYSTEM
			if ( m_kSkillTree.IsActiveSkillPageNumberValid() )
			{
				if ( m_kSkillTree.GetSkillLevelAndCSP( iSkillLevel, iSkillCSPoint, 
					mit->first, m_kSkillTree.AccessLearnedSkillTree() ) )
					mit->second.m_iSpendSkillCSPoint += iSkillCSPoint;	// ��� ��ų�� �ִٸ�
			}
			else
			{
				AddLogWhenSkillPagesNumberIsWrong( L"EGS_GET_SKILL_REQ" );
				goto end_proc;
			} 
#else // SERV_SKILL_PAGE_SYSTEM
			if( m_kSkillTree.GetSkillLevelAndCSP( mit->first, iSkillLevel, iSkillCSPoint ) == true )
			{
				// ��� ��ų�� �ִٸ�
				mit->second.m_iSpendSkillCSPoint += iSkillCSPoint;
			}
#endif // SERV_SKILL_PAGE_SYSTEM
		}
	}

	// DB �� ������ ���� ��Ű�� ������
	//{{

#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.SubtractSPoint( iTotalSP );
#else // SERV_SKILL_PAGE_SYSTEM
	m_iSPoint -= iTotalSP;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	if( kDBReq.m_iCSPoint >= 0 )		// CSPoint�� ����ؼ� ��ų�� ȹ���� ���
	{
		kDBReq.m_iBeforeCSPoint = m_kSkillTree.GetCSPoint();

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( m_kSkillTree.GetActiveSkillPagesIndex(), kDBReq.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( kDBReq.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
	}
	//}}
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	m_kUserQuestManager.Handler_OnLearnNewSkill( GetThisPtr<KGSUser>(), kPacket_.m_vecNowLearnSkill );
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

	// DB �� ����Ϸ� ����
	kDBReq.m_iUnitUID		= GetCharUID();
	kDBReq.m_iTotalSpendSkillPoint = iTotalSP;						// ����� sp �ѷ�
	kDBReq.m_mapSkillList = kPacket_.m_mapSkillList;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kDBReq.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendToGameDB( DBE_INSERT_SKILL_REQ, kDBReq );
	return;

end_proc:

	// �� ����� ��� ó��
	SendPacket( EGS_GET_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_SKILL_ACK kPacket;
	kPacket.m_iOK	= kPacket_.m_iOK;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, int > mapOldSkillList;

	// ���� �� ������ sp �ѹ�
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iBeforeCSPoint );
		m_kSkillTree.AddSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iTotalSpendSkillPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( kPacket_.m_iBeforeCSPoint );
		m_iSPoint += kPacket_.m_iTotalSpendSkillPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	}
	else//��ų����� �Ҹ��sp ���ش�.
	{
		// ����� ��ų �����ϱ�
		{
			std::map< int, KGetSkillInfo >::iterator mit = kPacket_.m_mapSkillList.begin();
			for( ; mit != kPacket_.m_mapSkillList.end() ; ++mit )
			{
				mapOldSkillList.insert( std::make_pair( mit->first, m_kSkillTree.GetSkillLevel( mit->first ) ) );

#ifdef SERV_SKILL_PAGE_SYSTEM
				// kimhc // ����ö // ��ų �����, ���� ������� ��ų �������� ���� �ǵ��� ��
				m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( mit->second.m_iSkillID, mit->second.m_iSkillLevel, mit->second.m_iSpendSkillCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
				m_kSkillTree.SetSkillLevelAndCSP( mit->second.m_iSkillID, mit->second.m_iSkillLevel, mit->second.m_iSpendSkillCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
			}
		}

		if( NetError::GetLastError() != NetError::NET_OK )
		{
			START_LOG_WITH_NAME( cerr )
				<< BUILD_LOG( NetError::GetLastErrMsg() )
				<< END_LOG;

			goto end_proc;
		}

		//��ų�� ���鼭 ����� ������ �����͵� �ٽ� �Ѱ��ش�.
		kPacket.m_mapSkillList			= kPacket_.m_mapSkillList;
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		kPacket.m_iRemainSP				= m_kSkillTree.GetSPoint();
#else // SERV_SKILL_PAGE_SYSTEM
		kPacket.m_iRemainSP				= m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
		
		kPacket.m_iRemainCSP			= m_kSkillTree.GetCSPoint();

		{
			std::map< int, KGetSkillInfo >::iterator mit = kPacket_.m_mapSkillList.begin();
			for( ; mit != kPacket_.m_mapSkillList.end() ; ++mit )
			{
				//{{ 2010. 10. 29	������	����Ʈ ���� �߰�
				std::map< int, int >::iterator mmit = mapOldSkillList.find( mit->first );
				if( mmit != mapOldSkillList.end() )
				{
					int iSkillCount = m_kSkillTree.GetSkillLevel( mit->first ) - mmit->second;
					for( int i = 0 ; i < iSkillCount ; ++i )
					{
						m_kUserQuestManager.Handler_OnUseSkillPoint( GetThisPtr<KGSUser>(), mit->first );

						//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
						m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_USE_SKILL_POINT );

						const int iUseSkillPointCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_USE_SKILL_POINT );
						if( iUseSkillPointCount == 1 )
						{
							WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_USE_SKILL_POINT );
						}
					}
				}
			}
		}
	}

end_proc:
	kPacket.m_iOK				= NetError::GetLastError();
	SendPacket( EGS_GET_SKILL_ACK, kPacket );

	if( NetError::GetLastError() == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			//{{ 2012. 02. 01	������	��Ʋ�ʵ� �ý���
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
			//}}
			break;

		default:
			return;
		}

		//{{ 2012. 02. 01	������	��Ʋ�ʵ� �ý���
		KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
		GetRoomUserInfo( kPacketNot );
		SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_RESET_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESET_SKILL_REQ, EGS_RESET_SKILL_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_RESET_SKILL_ACK kAck;

		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_RESET_SKILL_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_RESET_SKILL_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_RESET_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;
		
		kPacketAck.m_iOK = NetError::ERR_SKILL_PAGE_01;

		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// �ʱ�ȭ ���� ���
	const CXSLSkillTree::SkillTemplet* pDelSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iSkillID );
	if( pDelSkillTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	const CXSLSkillTree::SkillTreeTemplet* pDelSkillTreeTemplet = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass(), kPacket_.m_iSkillID );
	if( pDelSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"Ʈ���� �������� �ʴ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitClass() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// ���� �˻�
	bool bSKillInitLevel = false;
	if( m_kSkillTree.CheckResetSkill( kPacket_, static_cast<int>(GetUnitClass()), GetLevel(), kPacketAck.m_iOK, bSKillInitLevel ) == false )
	{
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// ���� ��ų ó��
	int iDelSkillLevel = 0;
	int iDelSkillCSPoint = 0;
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( false == m_kSkillTree.GetSkillLevelAndCSP( OUT iDelSkillLevel, OUT iDelSkillCSPoint,
		IN kPacket_.m_iSkillID, IN m_kSkillTree.AccessLearnedSkillTree() ) )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ��ų �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}			
#else // SERV_SKILL_PAGE_SYSTEM
	if( m_kSkillTree.GetSkillLevelAndCSP( kPacket_.m_iSkillID, iDelSkillLevel, iDelSkillCSPoint ) == false )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ��ų �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// �ʱ�ȭ �� ��ų���� Ȯ������. �ƴ϶�� 1������ �Ѵ�.(�⺻ ��ų, ���� ��ų)
	const int iAfterDelSkillLevel = ( bSKillInitLevel ? 0 : 1 );

	// ���� ������ �˻�, �������̶�� ��ų������ 2�̻����� Ȯ���ϱ�	- // note!! ���߿� �������� ��ų�߿� ��ų�ǵ����� ���Ŀ� �������� ��ų�� �ڵ����� Ż���ǵ��� ó������
	std::vector< int > vecSkillData;
	m_kSkillTree.GetSkillSlot( vecSkillData );

	BOOST_TEST_FOREACH( const int, iEquipSkillID, vecSkillData )
	{
		if( iEquipSkillID == kPacket_.m_iSkillID )
		{
			if( iAfterDelSkillLevel <= 0 && iDelSkillLevel > 0 )
			{
				START_LOG( cwarn, L"�������� ��ų�� �ʱ�ȭ �� ���̹Ƿ� ���� �����ؾ� �ʱ�ȭ �� �� �ִ�." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iSkillID )
					<< BUILD_LOG( iDelSkillLevel )
					<< BUILD_LOG( iDelSkillCSPoint )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_05;
				SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
				return;
			}
		}
	}

	// ��ų ����Ʈ ���
	int iAfterSP = 0;
	int iAfterCSP = 0;
	for( int i = 0 ; i < iDelSkillLevel ; ++i )
	{
		if( i == 0 )
		{
			if( iAfterDelSkillLevel > 0 )
				continue;

			iAfterSP += pDelSkillTemplet->m_iRequireLearnSkillPoint;
		}
		else
		{
			iAfterSP += pDelSkillTemplet->m_iRequireUpgradeSkillPoint;
		}
	}

	// ��ų�� ���ڵ� ĳ�� ����Ʈ�� �����Ѵ�
	iAfterSP -= iDelSkillCSPoint;

	if( m_kSkillTree.IsCashSkillPointExpired() == false )
	{
		iAfterCSP = m_kSkillTree.GetCSPoint() + iDelSkillCSPoint; 

		// max ��ġ�� ���� ��ŭ ���� �� �� ����.
		if( m_kSkillTree.GetMaxCSPoint() < iAfterCSP )
		{
			iAfterSP += iAfterCSP - m_kSkillTree.GetMaxCSPoint();
			iAfterCSP = m_kSkillTree.GetMaxCSPoint();
		}
	}
	else
	{
		// kimhc // ����ö // 2013-11-21 // �ٸ� �������� Invaild �� ���� -1�� �����ϴµ� ����� �� 0���� �����ϳ�...
		// �Ʒ� iAfterCSP <= 0 ? -1 : iAfterCSP �� ���� 0�� ������ Invaild �ϴٰ� ���ְڴٴ� ���ε�..
		// 0�� ��찡 Invaild �� ���ΰ�..?
		// ĳ�ý�ų �Ⱓ�� ���� ���� �ʾҾ iAfterCSP�� 0�� ���� ���� �� ������...

		// kimhc // ����ö // 2013-11-21 // ������ �˾���... �����ϳ�..
		// �����޾ƾ��� CSP�� 0�� ��� DB ���� UPDATE ������ �ʱ� ���ؼ� ��
		iAfterCSP = 0; // invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
	}

	// ��ų �ʱ�ȭ ������ �˻�
	KInventoryItemInfo kInventoryItemInfo;
	if( m_kInventory.ResetSkillItem( kInventoryItemInfo ) == false )
	{
		START_LOG( cerr, L"��ų �ʱ�ȭ �������� �κ��丮�� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )			
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_01;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// DB�� ����
	KDBE_RESET_SKILL_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iDelSkillID = kPacket_.m_iSkillID;
	kPacketToDB.m_iDelSkillLevel = iAfterDelSkillLevel;
	kPacketToDB.m_iCSPoint = iAfterCSP <= 0 ? -1 : iAfterCSP;
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iBeforeSPoint = m_kSkillTree.GetSPoint();
	kPacketToDB.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#else // SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iBeforeSPoint = m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	kPacketToDB.m_iBeforeCSPoint =  m_kSkillTree.GetCSPoint();

#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.AddSPoint( kPacket_.m_iActiveSkillPageNumber - 1, iAfterSP );
	m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, iAfterCSP );
#else // SERV_SKILL_PAGE_SYSTEM
	m_iSPoint += iAfterSP;
	m_kSkillTree.SetCSPoint( iAfterCSP );
#endif // SERV_SKILL_PAGE_SYSTEM	

	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInventoryItemInfo );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );	

	SendToGameDB( DBE_RESET_SKILL_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_RESET_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_RESET_SKILL_ACK kPacket;
	kPacket.m_iSPoint = 0;
	kPacket.m_iCSPoint = 0;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iDelSkillID			= kPacket_.m_iDelSkillID;
	kPacket.m_vecInventorySlotInfo	= kPacket_.m_vecUpdatedInventorySlot;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	// DB���� ���е� �������� �ִٸ� �ѹ�.
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// DB ������Ʈ ����
		// ������ sp �ѹ�
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iBeforeCSPoint );
		int iSPoint = m_kSkillTree.GetSPoint() - kPacket_.m_iBeforeSPoint;
		m_kSkillTree.SubtractSPoint( iSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetCSPoint( kPacket_.m_iBeforeCSPoint );
		int iSPoint = m_iSPoint - kPacket_.m_iBeforeSPoint;
		m_iSPoint -= iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM

		goto end_proc;
	}

	// ��ų �ʱ�ȭ
	bool isDefaultSkill = SiCXSLSkillTree()->IsUnitTypeDefaultSkill( kPacket_.m_iDelSkillID );
	bool bAllFollowingSkillLevelZero = m_kSkillTree.IsAllFollowingSkillLevelZero( kPacket_.m_iDelSkillID );

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( m_kSkillTree.IsActiveSkillPageNumberValid() )
	{
		m_kSkillTree.ResetSkill( m_kSkillTree.AccessLearnedSkillTree(), kPacket_.m_iDelSkillID, isDefaultSkill );
	}
	else
	{
		AddLogWhenSkillPagesNumberIsWrong( L"DBE_RESET_SKILL_ACK" );
		goto end_proc;
	}

	if( isDefaultSkill == true || bAllFollowingSkillLevelZero == false )
	{
		// kimhc // ����ö // 2013-11-17 // ������ �˾� � ����
		// ��ų �Ѱ� �ʱ�ȭ ��, ���� ������� ��ų �������� �ʱ�ȭ �ϵ��� ��
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDelSkillID, 1, 0 );
	}

	kPacket.m_iSPoint		= m_kSkillTree.GetSPoint();
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.ResetSkill( kPacket_.m_iDelSkillID, isDefaultSkill );

	if( isDefaultSkill == true || bAllFollowingSkillLevelZero == false )
	{
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDelSkillID, 1, 0 );
	}

	kPacket.m_iSPoint		= m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM

	kPacket.m_iCSPoint		= m_kSkillTree.GetCSPoint();
	
end_proc:
	SendPacket( EGS_RESET_SKILL_ACK, kPacket );

	// �ʱ�ȭ ���� ���
	const CXSLSkillTree::SkillTemplet* pCheckSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iDelSkillID );
	if( pCheckSkillTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ų���ø� �Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iDelSkillID )
			<< END_LOG;
		return;
	}

	//{{ 2012. 02. 03	������	��Ʋ�ʵ� �ý���
	//////////////////////////////////////////////////////////////////////////
	// �нú� ��ų�� ��� RoomUserInfo ������ ������Ʈ �Ѵ�.
	// ����� ��� �ٸ������鿡�Ե� ����
	if( kPacket_.m_iOK == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
			break;

		default:
			return;
		}

		if( GetRoomUID() != 0 )
		{
			KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
			GetRoomUserInfo( kPacketNot );
			SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//}}
}

IMPL_ON_FUNC( EGS_INIT_SKILL_TREE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INIT_SKILL_TREE_REQ, EGS_INIT_SKILL_TREE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_INIT_SKILL_TREE_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_INIT_SKILL_TREE_ACK kAck;
	kAck.m_iOK = NetError::ERR_UNKNOWN;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_INIT_SKILL_TREE_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	if( !m_kInventory.IsExist( kPacket_.m_iItemUID ) )
	{
		START_LOG( cerr, L"��ų�ʱ�ȭ �������� ���µ� ��Ŷ ���ƿ�.!" )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )			
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}

	int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	switch ( iItemID )
	{

	default:
		{
			START_LOG( cerr, L"��ų�ʱ�ȭ ������ID�� �̻���." )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_04;
			SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
			return;

		} break;

	case CXSLItem::CI_SKILL_INIT_ITEM:
	case CXSLItem::CI_SKILL_INIT_EVENT_ITEM:

#ifdef SERV_NEW_SKILL_INIT_EVENT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
	case CXSLItem::CI_SKILL_INIT_EVENT_ITEM2:
	case CXSLItem::CI_SKILL_INIT_EVENT_ITEM3:
	case CXSLItem::CI_SKILL_INIT_EVENT_ITEM4:
#endif // SERV_NEW_SKILL_INIT_EVENT_ITEM
#ifdef SERV_EVENT_INIT_SKILL_TREE_ITEM
	case CXSLItem::CI_INIT_SKILL_TREE_ITEM:
#endif SERV_EVENT_INIT_SKILL_TREE_ITEM
#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
	case CXSLItem::EI_RURIEL_RESET_SKILL_ITEM:
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM
#ifdef SERV_GLOBAL_DEFINE
	case CXSLItem::CI_SKILL_INIT_EVENT_ITEM_JP:
#endif //SERV_GLOBAL_DEFINE
		break;
	}
	
	int iDefaultSkillID[6] = {0,};
	if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), iDefaultSkillID[0], iDefaultSkillID[1], iDefaultSkillID[2], iDefaultSkillID[3], iDefaultSkillID[4], iDefaultSkillID[5] ) )
	{
		START_LOG( cerr, L"����Ÿ���� �̻���." )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}

	const int DEFAULT_SKILL_SP = 0;	// �⺻ ��ų�� ��¥
	int iRetrievedSPoint = 0 ;	// �������� SP
	int iRetrievedCSPoint = 0;	// �������� CSP
	
	if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == false )
	{
		START_LOG( cerr, L"������ ȯ��� sp ������ �̻��մϴ�." )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( static_cast<int>(GetLevel()) )
			<< BUILD_LOG( iRetrievedSPoint )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SKILL_20;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}

	if( false == m_kSkillTree.IsCashSkillPointExpired() )
	{
		iRetrievedCSPoint = m_kSkillTree.GetMaxCSPoint();
	}
	else
	{

#ifdef SERV_SKILL_PAGE_SYSTEM
		/// kimhc // ����ö // 2013-11-21 
		/// ��ų �ʱ�ȭ�� �Ѱ��� ��ų���������� ���� ������
		/// CSPoint �� �󸶳� ���Ҵ����� ��ü�������� �˻��ϵ��� ����
		/// ĳ�� ��ų �Ⱓ�� ���� �ƴµ�, ��� �Ѱ� �������� CSP�� �ʱ�ȭ �Ǿ������� ����
		/// üũ�� �̻��ϴٰ� ���� �Ǽ�...
		if( m_kSkillTree.GetCSPointAnyPage() )
#else // SERV_SKILL_PAGE_SYSTEM
		if( 0 != m_kSkillTree.GetCSPoint() )
#endif // SERV_SKILL_PAGE_SYSTEM
		{
			START_LOG( cerr, L"cash skill point������ �ٵǾ��µ� ��ųƮ���� csp�� �����ִ�." )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iRetrievedSPoint )
				<< BUILD_LOG( iRetrievedCSPoint )
				
#ifdef SERV_SKILL_PAGE_SYSTEM
				<< BUILD_LOG( m_kSkillTree.GetCSPointAnyPage() )
#else // SERV_SKILL_PAGE_SYSTEM
				<< BUILD_LOG( m_kSkillTree.GetCSPoint() )
#endif // SERV_SKILL_PAGE_SYSTEM
				
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_SKILL_12;
			SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
			return;
		}
	}

	KDBE_INIT_SKILL_TREE_REQ kDBReq;
	kDBReq.m_iUnitUID  = GetCharUID();
	kDBReq.m_iItemUID  = kPacket_.m_iItemUID;
	kDBReq.m_iSPoint  = iRetrievedSPoint;
	kDBReq.m_iCSPoint  = iRetrievedCSPoint;
	kDBReq.m_iDefaultSkillID1 = iDefaultSkillID[0];
	kDBReq.m_iDefaultSkillID2 = iDefaultSkillID[1];
	kDBReq.m_iDefaultSkillID3 = iDefaultSkillID[2];
	kDBReq.m_iDefaultSkillID4 = iDefaultSkillID[3];
	kDBReq.m_iDefaultSkillID5 = iDefaultSkillID[4];
	kDBReq.m_iDefaultSkillID6 = iDefaultSkillID[5];
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	kDBReq.m_iBeforeSPoint = m_kSkillTree.GetSPoint();
	kDBReq.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#else // SERV_SKILL_PAGE_SYSTEM
	kDBReq.m_iBeforeSPoint = m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	kDBReq.m_iBeforeCSPoint = m_kSkillTree.GetCSPoint();
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.SetSPoint( iRetrievedSPoint );
	m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, iRetrievedCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	m_iSPoint.SetValue(iRetrievedSPoint);
	m_kSkillTree.SetCSPoint( iRetrievedCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

	SendToGameDB( DBE_INIT_SKILL_TREE_REQ, kDBReq );
}

IMPL_ON_FUNC( DBE_INIT_SKILL_TREE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_INIT_SKILL_TREE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��ų�ʱ�ȭ ����.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iBeforeSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iBeforeCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		// �ѹ�
		m_iSPoint.SetValue( kPacket_.m_iBeforeSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iBeforeCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

	}
	else
	{
		std::vector< KInventoryItemInfo > vecUpdatedSlot;
		KInventoryItemInfo kInfo;

		int iUsageType = 0;
		if( m_kInventory.GetUsageType( kPacket_.m_iItemUID, iUsageType ) == false )
		{
			START_LOG( cerr, L"USAGE TYPE �о���� ����.!" )
				<< BUILD_LOG( kPacket_.m_iItemUID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );
#else
				<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT
		}

		//usage type �о���� ���и� �ص� ������ �����õ��� �غ���.
		if( iUsageType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased;
			int iCategory;
			int iSlotID;
			LIF( m_kInventory.GetCategorySlot( kPacket_.m_iItemUID, iCategory, iSlotID ) );

			m_kInventory.DecreaseQuantity( kPacket_.m_iItemUID, 1, iDecreased, KDeletedItemInfo::DR_INIT_SKILL );
			LIF( 1 == iDecreased );
			m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );
		}
		else
		{
			m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInfo, KDeletedItemInfo::DR_INIT_SKILL );
		}
		vecUpdatedSlot.push_back( kInfo );

		kPacket.m_vecInventorySlotInfo = vecUpdatedSlot;		
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		// ��ų ����.. ������ų ����
		m_kSkillTree.SetSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iCSPoint );
		
		kPacket.m_iSPoint					= kPacket_.m_iSPoint;
		kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
		kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
		
		m_kSkillTree.ResetThisSkillPage( kPacket_.m_iActiveSkillPageNumber );

		// �⺻ ��ų �ֱ�
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDefaultSkillID1, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDefaultSkillID2, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDefaultSkillID3, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDefaultSkillID4, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDefaultSkillID5, 1, 0 );
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iDefaultSkillID6, 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
		// ��ų ����.. ������ų ����
		kPacket.m_iSPoint = m_iSPoint;
		kPacket.m_iCSPoint = m_kSkillTree.GetCSPoint();
		m_kSkillTree.Reset( true, true, false, false, false );

		// �⺻ ��ų �ֱ�
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID1, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID2, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID3, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID4, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID5, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID6, 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM


		// logdb�� ��� ������
		KELOG_INIT_SKILL_TREE_NOT kNot;
		kNot.m_iUnitUID = GetCharUID();
		kNot.m_iSPoint	= kPacket_.m_iSPoint;
		kNot.m_iCSPoint = kPacket_.m_iCSPoint;
		kNot.m_iItemUID = kPacket_.m_iItemUID;

		SendToLogDB( ELOG_INIT_SKILL_TREE_NOT, kNot );
	}

	SendPacket( EGS_INIT_SKILL_TREE_ACK, kPacket );

	// ����� ��� �ٸ������鿡�Ե� ����
	if( kPacket_.m_iOK == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
			break;

		default:
			return;
		}

		//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
		if( GetRoomUID() != 0 )
		{
			KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
			GetRoomUserInfo( kPacketNot );
			SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
		}
	}
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( EGS_GET_SKILL_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_SKILL_REQ, EGS_GET_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	KEGS_GET_SKILL_ACK kPacket;
	KDBE_INSERT_SKILL_REQ kDBReq;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;

	//if( false == IsTransactionEnabled( EGS_GET_SKILL_REQ ) )
	//{
	//	START_LOG( cwarn, L"ĳ�� ��ų ����Ʈ �Ⱓ���� ó���߿� ��ų ȹ�� �õ��ؼ� ����ó��!" )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( GetCharName() )
	//		<< BUILD_LOGc( GetUnitClass() )
	//		<< BUILD_LOG( kPacket_.m_iSkillID )
	//		<< END_LOG;

	//	kPacket.m_iOK = NetError::ERR_INVALID_TIMING;

	//	goto end_proc;
	//}

	// �̹� �ְ������� ��� ��ų���� �˻�
	if( m_kSkillTree.IsMaxSkillLevel( kPacket_.m_iSkillID ) == true )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_00;
		goto end_proc;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iSkillID, 1 );
	if( pSkillTemplet == NULL )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_01;
		goto end_proc;
	}

	// ���ε� ��ų�̶�� ������ Ǯ�ȴ��� üũ
	if( pSkillTemplet->m_bBornSealed == true )
	{
		if( m_kSkillTree.IsSkillUnsealed( kPacket_.m_iSkillID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_SKILL_15;
			goto end_proc;
		}
	}

	// �� �������� ���� �ִ� ��ų���� Ȯ��
	if( m_kSkillTree.IsMyUnitClassSkill( kPacket_.m_iSkillID ) == false )
	{
		START_LOG( cerr, L"�ڽ��� Ŭ������ �ٸ� ��ų�� ȹ���Ϸ���.!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( GetUnitClass() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_16;
		goto end_proc;
	}

	// ���ེų�� �ְ������� �����ߴ��� üũ
	if( m_kSkillTree.IsAllPrecedingSkillMaxLevel( kPacket_.m_iSkillID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_07;
		goto end_proc;
	}

	// ���� tier�� ����� SP�� �Һ��ߴ��� üũ
	if( m_kSkillTree.IsTierOpened( kPacket_.m_iSkillID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SKILL_16;
		goto end_proc;
	}

	// CSP�Ⱓ �������� Ȯ��
	if( m_kSkillTree.IsCashSkillPointExpired() == true )
	{
		// ����� SP�� �������ִ���?
		if( m_iSPoint <= 0 )
		{
			kPacket.m_iOK = NetError::ERR_SKILL_17;
			goto end_proc;
		}
	}
	else
	{
		// ����� SP�� CSP�� ������ �ִ���?
		if( m_iSPoint + m_kSkillTree.GetCSPoint() <= 0 )
		{
			kPacket.m_iOK = NetError::ERR_SKILL_18;
			goto end_proc;
		}
	}

	int iSkillLevel = 0;
	int iSkillCSPoint = 0;

	// ������ ��ų ���� ���
	if( m_kSkillTree.GetSkillLevelAndCSP( kPacket_.m_iSkillID, iSkillLevel, iSkillCSPoint ) == false )
	{
		iSkillLevel = 0;
		iSkillCSPoint = 0;
	}
	
	kDBReq.m_iUnitUID		= GetCharUID();
	kDBReq.m_iSkillID		= kPacket_.m_iSkillID;
	kDBReq.m_iSkillLevel	= iSkillLevel + 1;

	if( m_kSkillTree.IsCashSkillPointExpired() == true )
	{
		kDBReq.m_iSkillCSPoint	= iSkillCSPoint;
		kDBReq.m_iCSPoint = -1;	// invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
	}
	else
	{
		kDBReq.m_iSkillCSPoint	= iSkillCSPoint + 1;

		if( m_kSkillTree.GetCSPoint() > 0 )
		{
			kDBReq.m_iCSPoint = m_kSkillTree.GetCSPoint() - 1;
		}
		else
		{
			kDBReq.m_iCSPoint = -1;	// invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
		}
	}

	SendToGameDB( DBE_INSERT_SKILL_REQ, kDBReq );
	return;

end_proc:

	// �� ����� ��� ó��
	SendPacket( EGS_GET_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_SKILL_ACK kPacket;
	kPacket.m_iOK	= kPacket_.m_iOK;
	

	//��ų����� �Ҹ��sp ���ش�.
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( kPacket_.m_iCSPoint >= 0 )		// CSPoint�� ����ؼ� ��ų�� ȹ���� ���
		{
			m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
		}
		else								// SPoint �� ����ؼ� ��ų ȹ���� ���
		{
			m_iSPoint -= 1;
		}
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iSkillID, kPacket_.m_iSkillLevel, kPacket_.m_iSkillCSPoint );
		

		if( NetError::GetLastError() != NetError::NET_OK )
		{
			START_LOG_WITH_NAME( cerr )
				<< BUILD_LOG( NetError::GetLastErrMsg() )
				<< END_LOG;

			goto end_proc;
		}


		//��ų�� ���鼭 ����� ������ �����͵� �ٽ� �Ѱ��ش�.
		kPacket.m_iSkillID			= kPacket_.m_iSkillID;
		kPacket.m_iSkillLevel		= kPacket_.m_iSkillLevel;
		kPacket.m_iSkillCSPoint		= kPacket_.m_iSkillCSPoint;
		kPacket.m_iSPoint			= m_iSPoint;
		kPacket.m_iCSPoint			= m_kSkillTree.GetCSPoint();

		//{{ 2010. 10. 29	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		m_kUserQuestManager.Handler_OnUseSkillPoint( GetThisPtr<KGSUser>(), kPacket_.m_iSkillID );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_USE_SKILL_POINT );

		const int iUseSkillPointCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_USE_SKILL_POINT );
		if( iUseSkillPointCount == 1 )
		{
			WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_USE_SKILL_POINT );
		}
#endif SERV_CHAR_LOG
		//}}
	}

end_proc:
	kPacket.m_iOK				= NetError::GetLastError();
	SendPacket( EGS_GET_SKILL_ACK, kPacket );

	if( NetError::GetLastError() == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			//{{ 2012. 02. 01	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;

		default:
			return;
		}

		//{{ 2012. 02. 01	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
		GetRoomUserInfo( kPacketNot );
		SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
#else
		// ������ ���� ���� ������Ʈ
		KRoomUserInfo kInfo;
		GetRoomUserInfo( kInfo );
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
}

IMPL_ON_FUNC( EGS_RESET_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESET_SKILL_REQ, EGS_RESET_SKILL_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_RESET_SKILL_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_RESET_SKILL_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_RESET_SKILL_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

	//if( false == IsTransactionEnabled( EGS_RESET_SKILL_REQ ) )
	//{
	//	START_LOG( cwarn, L"ĳ�� ��ų ����Ʈ �Ⱓ���� ó���߿� ��ų �ǵ����� �õ��ؼ� ����ó��!" )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( GetCharName() )
	//		<< BUILD_LOGc( GetUnitClass() )
	//		<< BUILD_LOG( kPacket_.m_iSkillID )
	//		<< END_LOG;

	//	kPacketAck.m_iOK = NetError::ERR_INVALID_TIMING;
	//	SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
	//	return;
	//}

	// �ʱ�ȭ ���� ���
	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iSkillID, 1 );
	if( pSkillTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// �ʱ�ȭ ��ų �� ���� ��ŭ ��ų ������ ������  �˻�
	int iSkillLevel = 0;
	int iSkillCSPoint = 0;
	if( m_kSkillTree.GetSkillLevelAndCSP( kPacket_.m_iSkillID, iSkillLevel, iSkillCSPoint ) == false )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ��ų �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// ��ų ������ �������� �˻�
	if( iSkillLevel < 1 )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ��ų �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// �� ĳ���ͺ� �⺻ ��ų 1������ �ʱ�ȭ �� �� ����	
	if( SiCXSLSkillTree()->IsUnitTypeDefaultSkill( kPacket_.m_iSkillID ) == true  &&  iSkillLevel == 1 )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� �ʱ�ȭ ����Ű�� ��ų�˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_04;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}
	
	// ���� ������ �˻�, �������̶�� ��ų������ 2�̻����� Ȯ���ϱ�	- // note!! ���߿� �������� ��ų�߿� ��ų�ǵ����� ���Ŀ� �������� ��ų�� �ڵ����� Ż���ǵ��� ó������
	std::vector< int > vecSkillData;
	m_kSkillTree.GetSkillSlot( vecSkillData );

	BOOST_TEST_FOREACH( const int, iEquipSkillID, vecSkillData )
	{
		if( iEquipSkillID == kPacket_.m_iSkillID )
		{
			if( iSkillLevel <= 1 )
			{
				START_LOG( cwarn, L"�������� ��ų�� ������ 1�����̱� ������ ��ų�ǵ����⸦ �� �� ����" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iSkillID )
					<< BUILD_LOG( iSkillLevel )
					<< BUILD_LOG( iSkillCSPoint )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_05;
				SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
				return;
			}
		}
	}

	// ���� ��� ���� �˻�
	if( m_kSkillTree.IsAllFollowingSkillLevelZero( kPacket_.m_iSkillID ) == false )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ���ܳ�� �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_04;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass(), kPacket_.m_iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"Ʈ���� �������� �ʴ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitClass() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_00;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// ��ų �ǵ����⸦ �ϰ� �� �Ŀ� ���� tier�� ��ų�� �� ��� �Ǵ� ��� ��ų ������ �� �� ����
	std::vector< int > vecCumulativeTierSPoint;
	m_kSkillTree.CalcCumulativeUsedSPointOnEachTier( vecCumulativeTierSPoint );

	if( pSkillTreeTemplet->m_iTier < 0 ||
		pSkillTreeTemplet->m_iTier > (int)vecCumulativeTierSPoint.size() - 1 )
	{
		START_LOG( cerr, L"��ų ���ø��� tier index�� �̻��մϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( GetUnitClass() )
			<< BUILD_LOG( pSkillTreeTemplet->m_iTier )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_04;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	for( int i = pSkillTreeTemplet->m_iTier+1; i<(int)vecCumulativeTierSPoint.size(); ++i )
	{
		if( vecCumulativeTierSPoint[i-1]-1 < (int)i * MAGIC_SKILL_POINT_PER_TIER )
		{
			START_LOG( cerr, L"���� tier�� �ִ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( GetUnitClass() )
				<< BUILD_LOG( pSkillTreeTemplet->m_iTier )
				<< BUILD_LOG( i )
				<< BUILD_LOG( vecCumulativeTierSPoint[i-1] )
				<< BUILD_LOG( kPacket_.m_iSkillID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_04;
			SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
			return;
		}
	}

	// ��ų �ʱ�ȭ ������ �˻�
	KInventoryItemInfo kInventoryItemInfo;
	if( m_kInventory.ResetSkillItem( kInventoryItemInfo ) == false )
	{
		START_LOG( cerr, L"��ų �ʱ�ȭ �������� �κ��丮�� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )			
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_01;
		SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
		return;
	}

	// DB�� ����
	KDBE_RESET_SKILL_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iSkillID = kPacket_.m_iSkillID;
	kPacketToDB.m_iSkillLevel = iSkillLevel - 1;
	if( iSkillCSPoint > 0 )
	{
		kPacketToDB.m_iSkillCSPoint = iSkillCSPoint - 1;
	}
	else
	{
		kPacketToDB.m_iSkillCSPoint = iSkillCSPoint;
	}

	// Cash skill point�� ��ȿ�� �Ⱓ�̶�� CSP�� �����ְ�, �ƴ϶�� SP�� �����ش�
	if( m_kSkillTree.IsCashSkillPointExpired() == false )
	{
		kPacketToDB.m_iCSPoint = m_kSkillTree.GetCSPoint() + 1; 
	}
	else
	{
		kPacketToDB.m_iCSPoint = -1; // invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
	}

	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInventoryItemInfo );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );	

	//{{ 2010. 10. 04	������	��ų �ʱ�ȭ ���� ����
	//if( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange.empty() && kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted.empty() )
	//{
	//	START_LOG( cerr, L"�������ҵǰų� ������ ��ų �ʱ�ȭ �������� ����?" )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( kPacket_.m_iSkillID )
	//		<< END_LOG;

	//	kPacketAck.m_iOK = NetError::ERR_RESET_SKILL_02;
	//	SendPacket( EGS_RESET_SKILL_ACK, kPacketAck );
	//	return;
	//}
	//}}

	SendToGameDB( DBE_RESET_SKILL_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_RESET_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_RESET_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	// DB���� ���е� �������� �ִٸ� �ѹ�.
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// DB ������Ʈ ����
		goto end_proc;
	}

	// ��ų ����Ʈ �߰�
	if( kPacket_.m_iCSPoint >= 0 )
	{
		m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
	}
	else
	{
		m_iSPoint	+= 1;
	}
	

	// ��ų �ʱ�ȭ
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iSkillID, kPacket_.m_iSkillLevel, kPacket_.m_iSkillCSPoint );

	kPacket.m_iDelSkillID		   = kPacket_.m_iSkillID;
	if( kPacket_.m_iCSPoint >= 0 )
	{
		kPacket.m_iRestoreSPoint	   = 0;
		kPacket.m_iRestoreCSPoint	   = 1;
	}
	else
	{
		kPacket.m_iRestoreSPoint	   = 1;
		kPacket.m_iRestoreCSPoint	   = 0;
	}
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;

end_proc:
	SendPacket( EGS_RESET_SKILL_ACK, kPacket );

	// �ʱ�ȭ ���� ���
	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iSkillID, 1 );
	if( pSkillTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ų���ø� �Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;
		return;
	}

	//{{ 2012. 02. 03	������	��Ʋ�ʵ� �ý���
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// �нú� ��ų�� ��� RoomUserInfo ������ ������Ʈ �Ѵ�.
	// ����� ��� �ٸ������鿡�Ե� ����
	if( kPacket_.m_iOK == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
			break;

		default:
			return;
		}

		if( GetRoomUID() != 0 )
		{
			KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
			GetRoomUserInfo( kPacketNot );
			SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
		}
	}
	//////////////////////////////////////////////////////////////////////////
#else
	//////////////////////////////////////////////////////////////////////////
	// �нú� ��ų�� ��� RoomUserInfo ������ ������Ʈ �Ѵ�.
	switch( pSkillTemplet->m_eType ) 
	{
	case CXSLSkillTree::ST_PASSIVE_MAGIC_ATTACK:
	case CXSLSkillTree::ST_PASSIVE_MAGIC_DEFENCE:
	case CXSLSkillTree::ST_PASSIVE_PHYSIC_ATTACK:
	case CXSLSkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
		{
			// ����� ��� �ٸ������鿡�Ե� ����
			if( kPacket_.m_iOK == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_PVP:
				case CXSLRoom::RT_DUNGEON:
					//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				case CXSLRoom::RT_BATTLE_FIELD:
				case CXSLRoom::RT_TRAININGCENTER:
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}
					break;

				default:
					return;
				}

				//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				if( GetRoomUID() != 0 )
				{
					KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
					GetRoomUserInfo( kPacketNot );
					SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
				}
#else
				// ������ ���� ���� ������Ʈ
				KRoomUserInfo kInfo;
				GetRoomUserInfo( kInfo );
				SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
		}
		break;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//////////////////////////////////////////////////////////////////////////
	//}}
}

IMPL_ON_FUNC( EGS_INIT_SKILL_TREE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INIT_SKILL_TREE_REQ, EGS_INIT_SKILL_TREE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_INIT_SKILL_TREE_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_INIT_SKILL_TREE_ACK kAck;
	kAck.m_iOK = NetError::ERR_UNKNOWN;

	//if( false == IsTransactionEnabled( EGS_INIT_SKILL_TREE_REQ ) )
	//{
	//	START_LOG( cwarn, L"ĳ�� ��ų ����Ʈ �Ⱓ���� ó���߿� ��ų �ʱ�ȭ �õ��ؼ� ����ó��!" )
	//		<< BUILD_LOG( GetCharName() )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( kPacket_.m_iItemUID )
	//		<< END_LOG;

	//	kAck.m_iOK = NetError::ERR_INVALID_TIMING;
	//	SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
	//	return;
	//}

	if( !m_kInventory.IsExist( kPacket_.m_iItemUID ) )
	{
		START_LOG( cerr, L"��ų�ʱ�ȭ �������� ���µ� ��Ŷ ���ƿ�.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )			
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}

	int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( ( iItemID != CXSLItem::CI_SKILL_INIT_ITEM )
		&& ( iItemID != CXSLItem::CI_SKILL_INIT_EVENT_ITEM )
#ifdef SERV_NEW_SKILL_INIT_EVENT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
		&& ( iItemID != CXSLItem::CI_SKILL_INIT_EVENT_ITEM2 )
		&& ( iItemID != CXSLItem::CI_SKILL_INIT_EVENT_ITEM3 )
#endif // SERV_NEW_SKILL_INIT_EVENT_ITEM
		)
	{
		START_LOG( cerr, L"��ų�ʱ�ȭ ������ID�� �̻���." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		
		kAck.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}

	int iDefaultSkillID = 0;
	int iDefaultSkillID2 = 0;
	if( !SiCXSLSkillTree()->GetUnitTypeDefaultSkill( GetUnitType(), iDefaultSkillID, iDefaultSkillID2 ) )
	{
		START_LOG( cerr, L"����Ÿ���� �̻���." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}



	// ��� ��ų�� ���ٸ�..
	int iUsedSPoint = 0;
	int iUsedCSPoint = 0;
	m_kSkillTree.CalcUsedSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );
	if( (iUsedSPoint <= 2 && iUsedCSPoint <= 0 ) ||
		(iUsedSPoint + iUsedCSPoint <= 0) )
	{
		START_LOG( cwarn, L"��ų�� �ϳ��� ����� �ʾҰų� �⺻ ��ų�� �ִ� �����ε�( ��, ���̻� �ʱ�ȭ �� �ʿ䰡 ���µ�) �ʱ�ȭ �Ϸ��� ��." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iUsedSPoint )
			<< BUILD_LOG( iUsedCSPoint )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SKILL_26;
		SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
		return;
	}


	const int DEFAULT_SKILL_SP = 2;		// default skill 2�� 1������ �����ϰ� �������� SP�� ����ؾ��Ѵ�
	int iRetrievedSPoint = 0 ;	// �������� SP
	int iRetrievedCSPoint = 0;	// �������� CSP
	

	if( false == m_kSkillTree.IsCashSkillPointExpired() )
	{
		iRetrievedSPoint	= iUsedSPoint + iUsedCSPoint + m_kSkillTree.GetCSPoint() - m_kSkillTree.GetMaxCSPoint() - DEFAULT_SKILL_SP;
		iRetrievedCSPoint	= m_kSkillTree.GetMaxCSPoint();
	}
	else
	{
		if( 0 == iUsedCSPoint && 0 == m_kSkillTree.GetCSPoint() )
		{
			iRetrievedSPoint	= iUsedSPoint - DEFAULT_SKILL_SP;
			iRetrievedCSPoint	= 0;
		}
		else
		{
			START_LOG( cerr, L"cash skill point������ �ٵǾ��µ� ��ųƮ���� csp�� �����ִ�." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iUsedSPoint )
				<< BUILD_LOG( iUsedCSPoint )
				<< BUILD_LOG( m_kSkillTree.GetCSPoint() )
				<< END_LOG;
			
			kAck.m_iOK = NetError::ERR_SKILL_12;
			SendPacket( EGS_INIT_SKILL_TREE_ACK, kAck );
			return;
		}
	}

	KDBE_INIT_SKILL_TREE_REQ kDBReq;
	kDBReq.m_iUnitUID		 = GetCharUID();
	kDBReq.m_iItemUID		 = kPacket_.m_iItemUID;
	kDBReq.m_iSPoint		 = m_iSPoint + iRetrievedSPoint;
	kDBReq.m_iCSPoint		 = iRetrievedCSPoint;
	kDBReq.m_iDefaultSkillID = iDefaultSkillID;
	kDBReq.m_iDefaultSkillID2 = iDefaultSkillID2;


	SendToGameDB( DBE_INIT_SKILL_TREE_REQ, kDBReq );
}

IMPL_ON_FUNC( DBE_INIT_SKILL_TREE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_INIT_SKILL_TREE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��ų�ʱ�ȭ ����.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}
	else
	{
		std::vector< KInventoryItemInfo > vecUpdatedSlot;
		KInventoryItemInfo kInfo;

		int iUsageType = 0;
		if( m_kInventory.GetUsageType( kPacket_.m_iItemUID, iUsageType ) == false )
		{
			START_LOG( cerr, L"USAGE TYPE �о���� ����.!" )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( GetCharName() );
		}

		//usage type �о���� ���и� �ص� ������ �����õ��� �غ���.
		if( iUsageType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased;
			int iCategory;
			int iSlotID;
			LIF( m_kInventory.GetCategorySlot( kPacket_.m_iItemUID, iCategory, iSlotID ) );

			m_kInventory.DecreaseQuantity( kPacket_.m_iItemUID, 1, iDecreased, KDeletedItemInfo::DR_INIT_SKILL );
			LIF( 1 == iDecreased );
			m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );
		}
		else
		{
			m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInfo, KDeletedItemInfo::DR_INIT_SKILL );
		}
		vecUpdatedSlot.push_back( kInfo );
		kPacket.m_vecInventorySlotInfo = vecUpdatedSlot;		

		//��ų����Ʈ ���� ����.
		m_iSPoint.SetValue( kPacket_.m_iSPoint );
		kPacket.m_iSPoint = m_iSPoint;
		m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
		kPacket.m_iCSPoint = kPacket_.m_iCSPoint;
	

		// ��ų ����.. ������ų ����
		m_kSkillTree.Reset( true, true, false, false, false );
		
		// �⺻ ��ų �ֱ�
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iDefaultSkillID2, 1, 0 );


		// logdb�� ��� ������
		KELOG_INIT_SKILL_TREE_NOT kNot;
		kNot.m_iUnitUID = GetCharUID();
		kNot.m_iSPoint = m_iSPoint;
		kNot.m_iCSPoint = kPacket_.m_iCSPoint;
		kNot.m_iItemUID = kPacket_.m_iItemUID;

		SendToLogDB( ELOG_INIT_SKILL_TREE_NOT, kNot );


	}

	SendPacket( EGS_INIT_SKILL_TREE_ACK, kPacket );

	// ����� ��� �ٸ������鿡�Ե� ����
	if( kPacket_.m_iOK == NetError::NET_OK  &&  GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
		case CXSLRoom::RT_TRAININGCENTER:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;

		default:
			return;
		}
		
		//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		if( GetRoomUID() != 0 )
		{
			KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
			GetRoomUserInfo( kPacketNot );
			SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
		}
#else
		// ������ ���� ���� ������Ʈ
		KRoomUserInfo kInfo;
		GetRoomUserInfo( kInfo );
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

IMPL_ON_FUNC_NOPARAM( EGS_CURRENT_TIME_REQ )
{
    // ���� ���ƾ� ��.

    CTime tCurrent = CTime::GetCurrentTime();
    KEGS_CURRENT_TIME_ACK kPacket;
    kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#ifdef SERVER_CURRENT_TIME_INT64
	kPacket.m_tCurrentTime = tCurrent.GetTime();
#endif //SERVER_CURRENT_TIME_INT64

    SendPacket( EGS_CURRENT_TIME_ACK, kPacket );
}

//{{ 2009. 8. 7  ������		����
IMPL_ON_FUNC( DBE_EXPAND_INVENTORY_SLOT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	switch( kPacket_.m_usEventID )
	{
	case EGS_GET_PURCHASED_CASH_ITEM_REQ:
		{
			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;

			std::map< int, int >::const_iterator mit;
			for( mit = kPacket_.m_mapExpandedSlot.begin(); mit != kPacket_.m_mapExpandedSlot.end(); ++mit )
			{
				int iExpanded;
// �ؿ��� ����				
//#ifdef SERV_REFORM_INVENTORY_INT
				//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
//#ifdef SERV_REFORM_INVENTORY_TEST
//				if( mit->first == CXSLInventory::ST_BANK )
//				{
//					m_kInventory.ExpandSlot( mit->first, mit->second, iExpanded );
//				}
//				else
//				{
//					m_kInventory.ExpandSlot( mit->first, mit->second - GetExpandedSlotSize( mit->first ), iExpanded );
//				}
//#else
				m_kInventory.ExpandSlot( mit->first, mit->second, iExpanded );
//#endif SERV_REFORM_INVENTORY_TEST
				//}}
//#endif SERV_REFORM_INVENTORY_INT
				kPacket.m_mapExpandedCategorySlot.insert( std::make_pair( mit->first, iExpanded ) );
			}

#ifdef SERV_GLOBAL_BILLING
			SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
#else // SERV_GLOBAL_BILLING
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
#endif // SERV_GLOBAL_BILLING
		}
		break;

	case EGS_USE_ITEM_IN_INVENTORY_REQ:
		//{{ 2011. 12. 21	������	���� Ȯ�� ��Ű�� ���� ��ǰ
#ifdef SERV_EXPAND_BANK_ITEM_IN_PACKAGE
	case ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK:
#endif SERV_EXPAND_BANK_ITEM_IN_PACKAGE
		//}}
		{
            KEGS_EXPAND_BANK_SLOT_NOT kNot;

			std::map< int, int >::const_iterator mit;
			for( mit = kPacket_.m_mapExpandedSlot.begin(); mit != kPacket_.m_mapExpandedSlot.end(); ++mit )
			{
				int iExpanded;
// �ؿ��� ����
//#ifdef SERV_REFORM_INVENTORY_INT
				//{{ 2012. 12. 26	�ڼ���	�κ��丮 ���� �׽�Ʈ	- ����� ( Merged by �ڼ��� )
//#ifdef SERV_REFORM_INVENTORY_TEST
//				if( mit->first == CXSLInventory::ST_BANK )
//				{
//					m_kInventory.ExpandSlot( mit->first, mit->second, iExpanded );
//				}
//				else
//				{
//					m_kInventory.ExpandSlot( mit->first, mit->second - GetExpandedSlotSize( mit->first ), iExpanded );
//				}
//#else
				m_kInventory.ExpandSlot( mit->first, mit->second, iExpanded );
//#endif SERV_REFORM_INVENTORY_TEST
				//}}
//#endif SERV_REFORM_INVENTORY_INT

				kNot.m_mapExpandedCategorySlot.insert( std::make_pair( mit->first, iExpanded ) );
			}

			SendPacket( EGS_EXPAND_BANK_SLOT_NOT, kNot );
		}
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� �̺�Ʈ�� �Դ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;
		}
		break;
	}
}
//}}

IMPL_ON_FUNC( DBE_EXPAND_SKILL_SLOT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kSkillTree.ExpandSkillSlotB( kPacket_.m_wstrSkillSlotBEndDate );

		KEGS_EXPAND_SKILL_SLOT_NOT kPacket;
		kPacket.m_iUnitUID = GetCharUID();
		kPacket.m_cSkillSlotBExpirationState = (char) m_kSkillTree.GetSkillSlotBExpirationState();
		kPacket.m_wstrSkillSlotBEndDate = m_kSkillTree.GetSkillSlotBEndDateString();
		SendPacket( EGS_EXPAND_SKILL_SLOT_NOT, kPacket );
		
#ifndef SERV_GLOBAL_BILLING
// 		//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰� // �ؿ� �ƴϸ� ���� �κ��ε� �׳� �ּ� ó��
// #ifdef SERV_ADD_PACKAGE_PRODUCT
// 		if( kPacket_.m_usEventID == ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK )
// #endif SERV_ADD_PACKAGE_PRODUCT
// 		//}}
//		{
#endif //SERV_GLOBAL_BILLING
			if( 0 != kPacket_.m_iPeriodExpire ) // ������ �ƴϸ�, ������ �����Դٴ� ��Ŷ�� ������
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
#ifdef SERV_GLOBAL_BILLING
				SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
#else // SERV_GLOBAL_BILLING
				SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
#endif // SERV_GLOBAL_BILLING
			}
#ifndef SERV_GLOBAL_BILLING
//		}			
#endif //SERV_GLOBAL_BILLING

	}
}

//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
IMPL_ON_FUNC( DBE_EXPAND_CHAR_SLOT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_nUnitSlot = kPacket_.m_iCharSlotSize;

		KEGS_EXPAND_CHAR_SLOT_NOT kPacketNot;
		kPacketNot.m_iCharSlotSize = m_nUnitSlot;
		SendPacket( EGS_EXPAND_CHAR_SLOT_NOT, kPacketNot );
	}
}
//}}

IMPL_ON_FUNC( DBE_UPDATE_EVENT_TIME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		BOOST_TEST_FOREACH( const int, iEventUID, kPacket_.m_setConnectTimeEvent )
		{
			int iRewardID = 0;
			//{{ 2011. 05. 31	������	���� �̺�Ʈ ���� üũ
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
			if( SiKGameEventManager()->GetConnectTimeEventReward( GetLevel(), iEventUID, iRewardID ) == false )
#else
			if( SiKGameEventManager()->GetConnectTimeEventReward( iEventUID, iRewardID ) == false )
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
			//}}			
			{
				START_LOG( cerr, L"�̺�Ʈ ���� ������ ���� ���߽��ϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( iEventUID )
					<< END_LOG;

				continue;
			}

			if( iRewardID <= 0 )
			{
				continue;
			}

			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = iRewardID;
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

			//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
			switch( iRewardID )
			{
			case 10432:
			case 10433:
			case 10434:
			case 10435:
			case 10436:
				SendToAccountDB( DBE_2012_WINTER_VACATION_EVENT_NOT );
				break;
			}
#endif SERV_2012_WINTER_VACATION_EVENT
			//}}
		}

		//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT

		BOOST_TEST_FOREACH( const int, iEventUID, kPacket_.m_setCumulativeTimeEvent )
		{
			int iRewardID = 0;
			if( !SiKGameEventManager()->GetCumulativeTimeEventReward( iEventUID, iRewardID ) )
			{
				START_LOG( cerr, L"�̺�Ʈ ���� ������ ���� ���߽��ϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( iEventUID )
					<< END_LOG;

				continue;
			}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
			if( iRewardID == 10622 )
			{
				CTime tCurr = CTime::GetCurrentTime();

				KDBE_UPDATE_EVENT_MONEY_REQ kReq;
				kReq.m_iUserUID = GetUID();
				kReq.m_iUnitUID = GetCharUID();
				kReq.m_iOldQuantity = GetEventMoney();
				kReq.m_iNewQuantity = 1;
				kReq.m_iRewardItemID = 0;
				kReq.m_wstrRegDate = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
				SendToAccountDB( DBE_UPDATE_EVENT_MONEY_REQ, kReq );

				SetEventMoney( GetEventMoney() + kReq.m_iNewQuantity );
				continue;
			}
#endif // SERV_EVENT_MONEY

			if( iRewardID <= 0 )
			{
				START_LOG( cerr, L"�̺�Ʈ ���������� �̻��մϴ�. �̺�Ʈ���̺� Ȯ�� �ʿ�!" )
					<< BUILD_LOG( iEventUID )
					<< BUILD_LOG( iRewardID )
					<< END_LOG;

				// ���������� �̻��ϸ� ������ ���� ����!
				continue;
			}

			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = iRewardID;
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}

#endif CUMULATIVE_TIME_EVENT
		//}}
	}
}

//{{ 2009. 7. 6  ������		��ŷ ����
IMPL_ON_FUNC_NOPARAM( ELG_NEW_RECORD_RANKING_SUCCESS_NOT )
{
	// ���� ����?

	// Ŭ���̾�Ʈ���� �˷��ֱ�
	SendID( EGS_NEW_RECORD_RANKING_SUCCESS_NOT );
}

IMPL_ON_FUNC( EGS_GET_RANKING_INFO_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_GET_RANKING_INFO_ACK kPacket;
	SiKGSRankingManager()->GetRankingInfo( kPacket_, kPacket );	// ��ŷ ������ ���
	m_kUserRankingManager.GetRankingInfo( GetThisPtr<KGSUser>(), kPacket_, kPacket );	// ���� �ֱ� ��ŷ ���
    SendPacket( EGS_GET_RANKING_INFO_ACK, kPacket );
}
//}}

//{{ 2008. 10. 6  ������	Ÿ��Ʋ
IMPL_ON_FUNC( EGS_EQUIP_TITLE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// Ÿ��Ʋ ����
	m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );

	KEGS_EQUIP_TITLE_ACK kPacket;
	kPacket.m_iOK	   = NetError::GetLastError();
	kPacket.m_iTitleID = kPacket_.m_iTitleID;
	SendPacket( EGS_EQUIP_TITLE_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		//{{ 2008. 12. 3  ������	�ʵ忡�� �Ⱓ�ٵ� ������ ����
		if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetFieldUID() <= 0 )
			{
				// �ʵ� �����̸鼭 fielduid�� 0�ΰ�쵵 �ִ�.
				return;
			}

			KERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
			kPacketNot.m_iEquippedTitleID = m_kUserTitleManager.GetEquippedTitleID();
#else
			kPacketNot.m_sEquippedTitleID = m_kUserTitleManager.GetEquippedTitleID();
#endif SERV_TITLE_DATA_SIZE
			//}}
			SendToGSField( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, kPacketNot );
		}
		//}}
		// ���� Ÿ��Ʋ�� �뿡�� �ٲ����� �� ����鿡�� �˷��ش�.
		else if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( GetRoomUID() <= 0 )
			{
				START_LOG( cerr, L"�� UID �̻�." )
					<< BUILD_LOG( GetRoomUID() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				return;
			}

			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				//{{ 2012. 10. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				{
					KERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kPacketNot;
					//{{ 2010. 12. 23	������	��� ���� ��Ŷ Ÿ��
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
					kPacketNot.m_cChangeEquippedPacketType = KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT::CEPT_CHANGE_TITLE;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
					//}}
					kPacketNot.m_UnitUID = GetCharUID();
					GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, kPacketNot );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				{
					KERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT kPacketNot;
					kPacketNot.m_UnitUID = GetCharUID();
					//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
					kPacketNot.m_iEquippedTitleID = m_kUserTitleManager.GetEquippedTitleID();
#else
					kPacketNot.m_sEquippedTitleID = m_kUserTitleManager.GetEquippedTitleID();
#endif SERV_TITLE_DATA_SIZE
					//}}
					SendToCnRoom( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT, kPacketNot );
				}
				break;

			default:
				{
					START_LOG( cerr, L"�̻��� �� Ÿ��." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}
		}
	}
}

IMPL_ON_FUNC( DBE_INSERT_TITLE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// �������� ���� Īȣ �߰�
		__int64 tCTime = 0;
		bool bInfinity = false;
		if( m_kUserTitleManager.AddTitle( kPacket_.m_iTitleID, kPacket_.m_wstrEndDate, tCTime, bInfinity ) )
		{
			KEGS_REWARD_TITLE_NOT kPacketNot;
			kPacketNot.m_kTitle.m_iTitleID	  = kPacket_.m_iTitleID;
			kPacketNot.m_kTitle.m_wstrEndDate = kPacket_.m_wstrEndDate;
			kPacketNot.m_kTitle.m_kCTime	  = tCTime;
			kPacketNot.m_kTitle.m_bInfinity	  = bInfinity;
			SendPacket( EGS_REWARD_TITLE_NOT, kPacketNot );

			// ����̼� �Ϸ�� ���� ���µǴ� �̼��� �ִ��� �˻�
			m_kUserTitleManager.CheckNewMission( GetThisPtr<KGSUser>() );
		}

		//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
		KE_LOCAL_LOG_TITLE_NOT kNot;
		kNot.m_iTitleID		= kPacket_.m_iTitleID;
		kNot.m_wstrCharName	= GetCharName();
		kNot.m_ucLevel		= GetLevel();
		KSIManager.QueueingEvent( E_LOCAL_LOG_TITLE_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////
#else
		//////////////////////////////////////////////////////////////////////////
		//{{ 2008. 10. 16  ������	Ÿ��Ʋ
#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )

		std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_TITLE );

		fout << L"\n";
		fout << KLocAlign << kPacket_.m_iTitleID << L"\t";
		fout << KLocAlign << GetCharName() << L"\t";
		fout << KLocAlign << GetLevel() << L"\t";

		fout.flush();
		//}}
#endif SERV_STATISTICS_THREAD
		//}}

		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		if( m_kComeBackUserInfo.GetIsComeBackUser() == true 
		 && kPacket_.m_iTitleID == KRewardTable::ERI_COME_BACK_USER_TITLE )
		{
			// Ÿ��Ʋ ����
			m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );
		}
#endif SERV_COME_BACK_USER_REWARD
		//}}

#ifdef SERV_2013_JUNGCHU_TITLE
		IF_EVENT_ENABLED( CEI_2013_JUNGCHU_TITLE )
		{
			if( kPacket_.m_iTitleID == KRewardTable::ERI_2013_JUNGCHU_TITLE )
			{
				// Ÿ��Ʋ ����
				m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );
			}
		}
#endif SERV_2013_JUNGCHU_TITLE
#ifdef SERV_CREATE_CHUNG_REWARD_TITLE

		if( kPacket_.m_iTitleID == KRewardTable::ERI_CREATE_CHUNG_REWARD_TITLE )
		{
			// Ÿ��Ʋ ����
			m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );
		}

#endif SERV_CREATE_CHUNG_REWARD_TITLE

#ifdef SERV_CREATE_NEW_CHARACTER_REWARD_TITLE

		if( kPacket_.m_iTitleID == KRewardTable::ERI_CREATE_NEW_CHARACTER_REWARD_TITLE )
		{
			// Ÿ��Ʋ ����
			m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );
		}

#endif SERV_CREATE_NEW_CHARACTER_REWARD_TITLE

#ifdef SERV_CHANNELING_AERIA_EVENT
		IF_EVENT_ENABLED( CEI_CHANNELING_AERIA_EVENT )
		{
			if( true == IsAeriaUser() && kPacket_.m_iTitleID == KRewardTable::ERI_CHANNELING_AERIA_EVENT_TITLE )
			{
				// Ÿ��Ʋ ����
				m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );
			}
		}
#endif SERV_CHANNELING_AERIA_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
		switch( kPacket_.m_iTitleID )
		{
		case 5730:
		case 5740:
		case 5750:
		case 5760:
		case 5770:
		case 5780:
		case 5790:
		case 5800:
		case 5810:
		case 5820:
			// Ÿ��Ʋ ����
			m_kUserTitleManager.EquipTitle( kPacket_.m_iTitleID );
			START_LOG( cerr, L"�̺�Ʈ Īȣ ����" )
				<< BUILD_LOG( kPacket_.m_iTitleID )
				<< END_LOG;
			break;
		default:
			break;
		}
#endif SERV_ELESIS_UPDATE_EVENT
	}
}

_IMPL_ON_FUNC( ELG_INSERT_TITLE_NOT, KDBE_INSERT_TITLE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( kPacket_.m_iUnitUID != GetCharUID() )
		{
			START_LOG( cerr, L"�α��� �������� �ش� �������� ���´µ� UnitUID�� �ٸ���?" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}

		// �������� ���� Īȣ �߰�
		__int64 tCTime = 0;
		bool bInfinity = false;
		if( m_kUserTitleManager.AddTitle( kPacket_.m_iTitleID, kPacket_.m_wstrEndDate, tCTime, bInfinity ) )
		{
			KEGS_REWARD_TITLE_NOT kPacketNot;
			kPacketNot.m_kTitle.m_iTitleID	  = kPacket_.m_iTitleID;
			kPacketNot.m_kTitle.m_wstrEndDate = kPacket_.m_wstrEndDate;
			kPacketNot.m_kTitle.m_kCTime	  = tCTime;
			kPacketNot.m_kTitle.m_bInfinity	  = bInfinity;
			SendPacket( EGS_REWARD_TITLE_NOT, kPacketNot );

			// ����̼� �Ϸ�� ���� ���µǴ� �̼��� �ִ��� �˻�
			m_kUserTitleManager.CheckNewMission( GetThisPtr<KGSUser>() );
		}
	}    
}
//}}

//{{ 2009. 4. 8  ������		�г��� ����
IMPL_ON_FUNC( DBE_DELETE_NICK_NAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�г��� ���� ����? ������ ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		// �г��� ���� �����ϸ� �ȵȴ�!
		return;
	}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		KEGIANT_ROLEREG_DELETE_UNIT_REQ kPacketReq;
		kPacketReq.m_wstrNickName = GetCharName();
		kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_UNIT_REQ, kPacketReq );
	}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

	// �г��� ���� ���������Ƿ� Log�� ������!
	KELOG_DELETE_NICK_NAME_NOT kPacketLog;
	kPacketLog.m_iUnitUID = GetCharUID();
	kPacketLog.m_wstrNickName = GetCharName();
	SendToLogDB( ELOG_DELETE_NICK_NAME_NOT, kPacketLog );

	// �г����� __DELETED__�� �ٲ�� ĳ���� ����â���� �̵��϶�� �̺�Ʈ�� ������!
    SendID( EGS_DELETE_NICK_NAME_SUCCESS_NOT );
}
//}}

//{{ 2009. 8. 4  ������		���ν�ų
IMPL_ON_FUNC( DBE_UNSEAL_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ��ų ���� ����
		m_kSkillTree.SkillUnseal( kPacket_.m_iSkillID );

		// Ŭ���̾�Ʈ���� ���� ���� �˸���
		KEGS_UNSEAL_SKILL_NOT kNot;
		kNot.m_iSkillID = kPacket_.m_iSkillID;
		SendPacket( EGS_UNSEAL_SKILL_NOT, kNot );
	}
	else
	{
		START_LOG( cerr, L"��ų �������� ����!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;
	}
}
//}}

//{{ 2009. 5. 8  ������		�ǽð� ������
_IMPL_ON_FUNC( ERM_CHAR_LEVEL_UP_NOT, KEGS_CHAR_LEVEL_UP_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

    SendPacket( EGS_CHAR_LEVEL_UP_NOT, kPacket_ );
}
//}}

//{{ 2009. 6. 3  ������		Ȯ����
IMPL_ON_FUNC( EGS_USE_MEGAPHONE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_USE_MEGAPHONE_ACK kAck;

#ifdef SERV_STRING_FILTER_USING_DB
		if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrMessage ) == false )
		{
			kAck.m_iOK = NetError::ERR_STRING_FILTER_00;
			SendPacket( EGS_USE_MEGAPHONE_ACK, kAck );
			return;
		}
#endif //SERV_STRING_FILTER_USING_DB

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_USE_MEGAPHONE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_USE_MEGAPHONE_ACK kPacket;

	// Ȯ���� �������� �´��� �˻�
	const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( iItemID != CXSLItem::CI_NASOD_MEGAPHONE  &&  iItemID != CXSLItem::CI_HIGH_NASOD_MEGAPHONE 
#ifdef SERV_VIP_SYSTEM
		&& iItemID != CXSLItem::CI_EVENT_NASOD_MEGAPHONE  &&  iItemID != CXSLItem::CI_EVENT_HIGH_NASOD_MEGAPHONE 
#endif //SERV_VIP_SYSTEM
		)
	{
		START_LOG( cerr, L"Ȯ���� �������� �ƴմϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_USE_MEGAPHONE_ACK, kPacket );
		return;
	}

	// �޽��� ���� �˻� : multi byte�� ��ȯ�Ͽ� üũ
	std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );

	if( strMessage.size() > 90 )
	{
		START_LOG( cerr, L"�޽��� ���� ����. 90����Ʈ ������ Ŭ���̾�Ʈ���� �˻������ٵ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_wstrMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_13;
		SendPacket( EGS_USE_MEGAPHONE_ACK, kPacket );
		return;
	}
	
	// ������ ���
	if( m_kInventory.UseItemInInventory( kPacket_.m_iItemUID, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"Ȯ���� ������ ��� ����!" )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_USE_MEGAPHONE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_USE_MEGAPHONE_ACK, kPacket );

	KEGS_MEGAPHONE_MESSAGE_NOT kNot;
	kNot.m_wstrSenderNickName = GetCharName();
	kNot.m_iSenderChannelID = SiKChannelManager()->GetChannelID();
	kNot.m_iSenderLocation = GetMapID();	
	kNot.m_wstrMessage = kPacket_.m_wstrMessage;

	// ���ͼ����� ��ε�ĳ������ ���� ����
	switch( iItemID )
	{
	case CXSLItem::CI_NASOD_MEGAPHONE:
#ifdef SERV_VIP_SYSTEM
	case CXSLItem::CI_EVENT_NASOD_MEGAPHONE:
#endif //SERV_VIP_SYSTEM
		{
			KncSend( PI_GS_USER, GetUID(), PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, ECN_MEGAPHONE_MESSAGE_NOT, kNot );
		}
		break;

	case CXSLItem::CI_HIGH_NASOD_MEGAPHONE:
#ifdef SERV_VIP_SYSTEM
	case CXSLItem::CI_EVENT_HIGH_NASOD_MEGAPHONE:
#endif //SERV_VIP_SYSTEM
		{
			SendToCnServer( ECN_MEGAPHONE_MESSAGE_NOT, kNot );
		}
		break;
	}

	//{{ 2009. 7. 22  ������	�ް��� �α�
	KELOG_MEGAPHONE_LOG_NOT kPacketLog;
	kPacketLog.m_iUnitUID = GetCharUID();
	kPacketLog.m_iItemID = iItemID;
	SendToLogDB( ELOG_MEGAPHONE_LOG_NOT, kPacketLog );
	//}}

	//{{ 2010. 11. 12  ��ȿ��	GameForge ä�� �α� ���� ó��
#ifdef SERV_RECORD_CHAT
	if( SiKGameSysVal()->IsRecordChat() )
	{
		KDCHATLOG_RECORD_CHAT_NOT kPacket_chat;
		kPacket_chat.m_iAccountUID = GetUID();
		kPacket_chat.m_iUnitUID = GetCharUID();
		kPacket_chat.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacket_chat.m_wstrChatText = kPacket_.m_wstrMessage;
		kPacket_chat.m_iChatType = KEGS_CHAT_REQ::CPT_MEGAPHONE;

		SendToChatLogDB( DCHATLOG_RECORD_CHAT_NOT, kPacket_chat );
	}
#endif SERV_RECORD_CHAT
	//}}
}

//{{ 2010. 02. 01  ������	�ް��� ���� ����
_IMPL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT )
{
	VERIFY_STATE_LOG( clog2, ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_MEGAPHONE_MESSAGE_NOT, kPacket_ );
}
//}}

//{{ 2009. 12. 8  ������	ũ���������̺�Ʈ
IMPL_ON_FUNC_NOPARAM( EGS_CHECK_TIME_EVENT_COMPLETE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_CHECK_TIME_EVENT_COMPLETE_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_CHECK_TIME_EVENT_COMPLETE_ACK kPacket;

	// ũ�������� �̺�Ʈ�� �Ϸ�Ǿ����� Ȯ��
	if( m_kUserEventManager.IsCompleteDirectRewardEvent() == false )
	{
		kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_00;
		SendPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK, kPacket );
		return;
	}

    // ������ �κ��丮�� �����ִ��� �˻�
	std::map< int, int > mapCheckResultItem;
	mapCheckResultItem.insert( std::make_pair( CXSLItem::EI_XMAS_EVENT_ITEM, 1 ) );

	if( m_kInventory.IsEnoughSpaceExist( mapCheckResultItem ) == false )
	{
		kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_01;
		SendPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK, kPacket );
		return;
	}

	std::map< int, KItemInfo > mapResultItem;
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( CXSLItem::EI_XMAS_EVENT_ITEM );
		if( pItemTemplet == NULL )
		{
			kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_02;
			SendPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK, kPacket );
			return;
		}

		KItemInfo kInfo;
		kInfo.m_iItemID		= CXSLItem::EI_XMAS_EVENT_ITEM;
		kInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
		kInfo.m_iQuantity	= 1;
		kInfo.m_sEndurance	= ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;

		mapResultItem.insert( std::make_pair( kInfo.m_iItemID, kInfo ) );
	}

	KDBE_CHECK_TIME_EVENT_COMPLETE_REQ kPacketToDB;

	if( m_kInventory.PrepareInsert( mapResultItem, kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_01;
		SendPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK, kPacket );
		return;
	}

	// �ش� �̺�Ʈ ������ �����ϰ� DB������Ʈ �Ϸ� ����!
	m_kUserEventManager.DeleteDirectRewardAndGetDBUpdateInfo( kPacketToDB.m_vecCompletedEvent );

	// DB�� ������Ʈ �Ϸ� ����!
	kPacketToDB.m_iUnitUID = GetCharUID();
    m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
#ifdef SERV_ADD_EVENT_DB
	//SendToEventDB( DBE_CHECK_TIME_EVENT_COMPLETE_REQ, kPacketToDB ); // �ش� �̺�Ʈ ��� ���� ����ҷ��� �߰� �۾� �ʿ�
#else //SERV_ADD_EVENT_DB
	SendToGameDB( DBE_CHECK_TIME_EVENT_COMPLETE_REQ, kPacketToDB );
#endif //SERV_ADD_EVENT_DB
}

IMPL_ON_FUNC( DBE_CHECK_TIME_EVENT_COMPLETE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CHECK_TIME_EVENT_COMPLETE_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�̺�Ʈ �Ϸ� ���� DB������Ʈ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iRewardCount = kPacket_.m_iRewardCount;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	SendPacket( EGS_CHECK_TIME_EVENT_COMPLETE_ACK, kPacket );
	
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ������� �̺�Ʈ�� �ִ��� ã�Ƽ� ����� ����!
        m_kUserEventManager.RestartDirectRewardEvent( GetThisPtr<KGSUser>(), kPacket_.m_vecRestartEvent );
	}
}
//}}

//{{ 2010. 01. 07  ������	��õ�θ���Ʈ
IMPL_ON_FUNC( EGS_GET_RECOMMEND_USER_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_RECOMMEND_LIST

	// ��õ�� ����Ʈ�� �ʱ�ȭ �Ǿ����� Ȯ��
	if( m_kUserRecommendManager.IsInitRecommendUserList() == false )
	{
		KDBE_GET_RECOMMEND_USER_LIST_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		SendToGameDB( DBE_GET_RECOMMEND_USER_LIST_REQ, kPacketToDB );
		return;
	}

	//{{ 2012. 01. 17  ��μ�	��õ�� ����Ʈ �ִ� ���� ��Ŷ ũ�� ����
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
	// ��õ�� ����Ʈ ���
	KDBE_GET_RECOMMEND_USER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;

	m_kUserRecommendManager.GetRecommendUserList( kPacket.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecRecommendUserList );

	// �α��μ����� ���� ���� �������� ��õ������ üũ�Ѵ�.
	SendToLoginServer( ELG_GET_RECOMMEND_USER_LIST_REQ, kPacket );
#else
	KEGS_GET_RECOMMEND_USER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;

	// ��õ�� ����Ʈ ���
	m_kUserRecommendManager.GetRecommendUserList( kPacket.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecRecommendUserList );

	SendPacket( EGS_GET_RECOMMEND_USER_LIST_ACK, kPacket );
#endif SERV_RECOMMEND_LIST_OVER_SIZE
	//}}

#endif SERV_RECOMMEND_LIST
}

IMPL_ON_FUNC( DBE_GET_RECOMMEND_USER_LIST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_RECOMMEND_LIST

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��õ�� ����Ʈ �޾ƿ��� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_GET_RECOMMEND_USER_LIST_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_GET_RECOMMEND_USER_LIST_ACK, kPacket );
		return;
	}

	//{{ 2012. 01. 17  ��μ�	��õ�� ����Ʈ �ִ� ���� ��Ŷ ũ�� ����
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
	// ��õ�� ����Ʈ �ʱ�ȭ
	m_kUserRecommendManager.SetRecommendUserList( kPacket_.m_vecRecommendUserList );

	// ��õ�� ����Ʈ ���
	KDBE_GET_RECOMMEND_USER_LIST_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_uiViewPage = 1;

	m_kUserRecommendManager.GetRecommendUserList( kPacket.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecRecommendUserList );

	// �α��μ����� ���� ���� �������� ��õ������ üũ�Ѵ�.
	SendToLoginServer( ELG_GET_RECOMMEND_USER_LIST_REQ, kPacket );
#else
	// �α��μ����� ���� ���� �������� ��õ������ üũ�Ѵ�.
	SendToLoginServer( ELG_GET_RECOMMEND_USER_LIST_REQ, kPacket_ );
#endif SERV_RECOMMEND_LIST_OVER_SIZE
	//}}

#endif SERV_RECOMMEND_LIST
}

_IMPL_ON_FUNC( ELG_GET_RECOMMEND_USER_LIST_ACK, KDBE_GET_RECOMMEND_USER_LIST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_RECOMMEND_LIST

	//{{ 2012. 01. 17  ��μ�	��õ�� ����Ʈ �ִ� ���� ��Ŷ ũ�� ����
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
	KEGS_GET_RECOMMEND_USER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;
	kPacket.m_uiTotalPage = kPacket_.m_uiTotalPage;
	kPacket.m_vecRecommendUserList = kPacket_.m_vecRecommendUserList;
#else
	// ��õ�� ����Ʈ �ʱ�ȭ
	m_kUserRecommendManager.SetRecommendUserList( kPacket_.m_vecRecommendUserList );

	KEGS_GET_RECOMMEND_USER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiViewPage = 1; // ����Ʈ�� ó�� �����°��̹Ƿ� 1�������� ����

	// ��õ�� ����Ʈ ���
	m_kUserRecommendManager.GetRecommendUserList( kPacket.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecRecommendUserList );
#endif SERV_RECOMMEND_LIST_OVER_SIZE
	//}}
	
	SendPacket( EGS_GET_RECOMMEND_USER_LIST_ACK, kPacket );

#endif SERV_RECOMMEND_LIST
}

IMPL_ON_FUNC( ELG_RECOMMEND_USER_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
    // ��õ�� ���� ����
	m_kUserRecommendManager.ModifyRecommendUser( kPacket_ );

	// Ŭ���̾�Ʈ�� �˸�
	KEGS_RECOMMEND_USER_INFO_NOT kPacket;
	kPacket.m_iRecommendUnitUID = kPacket_.m_iRecommendUnitUID;
	kPacket.m_cType = kPacket_.m_cType;
	kPacket.m_iData = kPacket_.m_iData;
    SendPacket( EGS_RECOMMEND_USER_INFO_NOT, kPacket );
}
//}}

//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT

IMPL_ON_FUNC_NOPARAM( EGS_ATTENDANCE_CHECK_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// �⼮ üũ ��� ����
	if( m_kUserEventManager.CheckAttendance( GetThisPtr<KGSUser>() ) == false )
	{
		KEGS_ATTENDANCE_CHECK_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_WEB_POINT_EVENT_00;
        SendPacket( EGS_ATTENDANCE_CHECK_ACK, kPacket );
		return;
	}
}

IMPL_ON_FUNC( DBE_ATTENDANCE_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// �⼮üũ �����ߴٸ�?
		m_kUserEventManager.CheckAttendanceSuccess( GetThisPtr<KGSUser>(), true );
	}
	else if( kPacket_.m_iOK == NetError::ERR_WEB_POINT_EVENT_00 )
	{
		// �̹� �⼮üũ �ȰŶ��..
        m_kUserEventManager.CheckAttendanceSuccess( GetThisPtr<KGSUser>(), false );
	}

	KEGS_ATTENDANCE_CHECK_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_ATTENDANCE_CHECK_ACK, kPacket );
}

#endif SERV_WEB_POINT_EVENT	
//}}

//{{ 2010. 03. 22  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE

IMPL_ON_FUNC( DBE_EXPAND_SKILL_NOTE_PAGE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

    // ����� ��Ʈ ���� Ȯ�� ���
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"����� ��Ʈ ���� Ȯ�� ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( kPacket_.m_cExpandedMaxPageNum )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// �ѹ� ó��
		m_kSkillTree.UpdateSkillNoteMaxPageNum( 0 );
		return;
	}

    // Ȯ��� ������ ���� ������Ʈ �Ѵ�.
	m_kSkillTree.UpdateSkillNoteMaxPageNum( kPacket_.m_cExpandedMaxPageNum );

	// Ŭ���̾�Ʈ���� Ȯ�� ����� �˸���.
	KEGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT kPacketNot;
	kPacketNot.m_cExpandedPageNum = m_kSkillTree.GetSkillNoteMaxPageNum();
	SendPacket( EGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT, kPacketNot );
}

IMPL_ON_FUNC( EGS_REG_SKILL_NOTE_MEMO_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REG_SKILL_NOTE_MEMO_REQ, EGS_REG_SKILL_NOTE_MEMO_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_REG_SKILL_NOTE_MEMO_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_REG_SKILL_NOTE_MEMO_ACK kPacket;

	// �����ϴ� ���������� Ȯ��
	if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
	{
		START_LOG( cerr, L"�������� �ʴ� �������� ����� ��Ʈ�� ����Ϸ� �ߴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_NOTE_00;
		SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket );
		return;
	}

	// �޸�ID��� ( ItemID )
	const int iMemoID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	// ����� ��Ʈ�� �����ϰ� �ִ��� Ȯ���Ѵ�.
	if( m_kSkillTree.IsHaveSkillNote() == false )
	{
		START_LOG( cerr, L"����� ��Ʈ�� �����ϰ� ���� ������ �޸� ����Ϸ� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_NOTE_04;
		SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket );
		return;
	}

	// ����Ϸ��� �������� �����ϴ� ������ ���� Ȯ���Ѵ�.
	if( m_kSkillTree.IsExistSkillNotePage( kPacket_.m_cSkillNotePageNum ) == false )
	{
		START_LOG( cerr, L"�������� �ʴ� �������� ����Ϸ� �ߴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( kPacket_.m_cSkillNotePageNum )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_NOTE_01;
		SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket );
		return;
	}

	// �̹� ��ϵ� �޸����� ã�ƺ���.
	if( m_kSkillTree.IsExistSkillNoteMemoID( iMemoID ) == true )
	{
		START_LOG( cerr, L"�̹� ��ϵ� �޸� �ٽ� ����Ϸ� �ߴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iMemoID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_NOTE_02;
		SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket );
		return;
	}

 //   // ������ �����°ǰ�?
	//if( !kPacket_.m_bForce )
	//{
	//	// ����Ϸ��� �������� �̹� �޸� ��ϵǾ� �ִ��� ã�ƺ���.
	//	if( m_kSkillTree.IsEmptySkillNotePage( kPacket_.m_cSkillNotePageNum ) == false )
	//	{
	//		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	//		SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket );
	//		return;
	//	}
	//}

	// ������ �����⸦ ���������� �κ��丮���� �������� �Һ��Ѵ�.
	KDBE_REG_SKILL_NOTE_MEMO_REQ kPacketToDB;

	if( m_kInventory.UseItemInInventory( kPacket_.m_iItemUID, kPacketToDB.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"����� ��Ʈ ������ ��� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket );
		return;
	}

	// DB�� ������Ʈ �Ϸ� ����!
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_cPageNum = kPacket_.m_cSkillNotePageNum;
	kPacketToDB.m_iMemoID = iMemoID;
	SendToGameDB( DBE_REG_SKILL_NOTE_MEMO_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( DBE_REG_SKILL_NOTE_MEMO_ACK, KEGS_REG_SKILL_NOTE_MEMO_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REG_SKILL_NOTE_MEMO_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"����� ��Ʈ �޸� ������Ʈ ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( kPacket_.m_cSkillNotePageNum )
			<< BUILD_LOG( kPacket_.m_iMemoID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
	else
	{
		// ����� ��Ʈ ������Ʈ
		m_kSkillTree.UpdateSkillNoteMemo( kPacket_.m_cSkillNotePageNum, kPacket_.m_iMemoID );

		//{{ 2012. 10. 29	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_kSkillTree.GetSkillNote( kPacket_.m_vecSkillNote );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		// ���̶�� ��ȿ� �ִ� �����鿡�� �˸���!
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				//{{ 2012. 10. 29	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				{
					KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
					GetRoomUserInfo( kPacketNot );
					SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
				}
				break;
			}
		}
	}
    
	SendPacket( EGS_REG_SKILL_NOTE_MEMO_ACK, kPacket_ );
}

#endif SERV_SKILL_NOTE
//}}

//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
IMPL_ON_FUNC( EGS_RESTORE_UNIT_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_RESTORE_UNIT_REQ, EGS_RESTORE_UNIT_ACK );

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	if( IsUseSecurity() == true )
	{
		if( GetSuccessSecondSecurity() == false )	// 2�� ������ ��� ���ε� ������ ���� �ʾҴٸ� ����
		{
			START_LOG( cerr, L"2�� ������ ��� ���ε� ������ �Ǳ� ���� ���� ������ �Ϸ� �Ѵ�." )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			KEGS_DELETE_UNIT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_21;
			kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
			kPacket.m_iGuildUID = 0;	
			SendPacket( EGS_DELETE_UNIT_ACK, kPacket );
			return;
		}
	}
#endif SERV_SECOND_SECURITY
	//}}

	kPacket_.m_iUserUID = GetUID(); //2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� userUID�� �ʿ��ϰ� ����ʿ� ���� �߰�

	SendToGameDB( DBE_GAME_RESTORE_UNIT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_GAME_RESTORE_UNIT_ACK, KEGS_RESTORE_UNIT_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_RESTORE_UNIT_ACK );


	SendPacket( EGS_RESTORE_UNIT_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_FINAL_DELETE_UNIT_REQ )
{
	// EGS_DELETE_UNIT_REQ �� ���� ��� ���·� ���� Delete ��
	// EGS_FINAL_DELETE_UNIT_REQ �� ���� ���� ��û Delete ��

	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_FINAL_DELETE_UNIT_REQ, EGS_FINAL_DELETE_UNIT_ACK );
	
	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	if( IsUseSecurity() == true )
	{
		if( GetSuccessSecondSecurity() == false )	// 2�� ������ ��� ���ε� ������ ���� �ʾҴٸ� ����
		{
			START_LOG( cerr, L"2�� ������ ��� ���ε� ������ �Ǳ� ���� ���� ���� ������ �Ϸ� �Ѵ�." )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			KEGS_DELETE_UNIT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_21;
			kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
			kPacket.m_iGuildUID = 0;	
			SendPacket( EGS_DELETE_UNIT_ACK, kPacket );
			return;
		}
	}
#endif SERV_SECOND_SECURITY
	//}}
	kPacket_.m_iUserUID = GetUID(); //2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� userUID�� �ʿ��ϰ� ����ʿ� ���� �߰�

	SendToGameDB( DBE_GAME_FINAL_DELETE_UNIT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_GAME_FINAL_DELETE_UNIT_ACK, KEGS_FINAL_DELETE_UNIT_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_FINAL_DELETE_UNIT_ACK );
	
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		SendPacket( EGS_FINAL_DELETE_UNIT_ACK, kPacket_ );
		return;
	}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		// 2011.09.23 lygan_������ // ���ϼ� ���� ó������ 7�ϵڿ� �÷��� ���� �ϴ°� �ƴ϶� �ٷ� ���� �ϵ��� �ϴ� �۾�
		KEGIANT_ROLEREG_DELETE_UNIT_REQ kPacketReq;
		kPacketReq.m_wstrNickName = kPacket_.m_wstrNickName;
		kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_UNIT_REQ, kPacketReq );
	}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

	SendToAccountDB( DBE_ACCOUNT_DELETE_UNIT_REQ, kPacket_ );
}
#endif SERV_UNIT_WAIT_DELETE

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
_IMPL_ON_FUNC( EGB_UPDATE_WORLD_BUFF_NOT, KEGS_UPDATE_WORLD_BUFF_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< KBuffInfo > vecActivateBuff;
	std::vector< int > vecDeleteBuff;

	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	if( kPacket_.m_usSuccessWorldBuff == CXSLBuffManager::BTI_NONE )
	{
		DeactivateBuff( CXSLBuffManager::BTI_BUFF_2013_DEFENSE_DEBUFF, vecDeleteBuff );
	}
	else if( kPacket_.m_usSuccessWorldBuff == CXSLBuffManager::BTI_BUFF_2013_DEFENSE_DEBUFF )
	{
		ActivateBuff( CXSLBuffManager::BTI_BUFF_2013_DEFENSE_DEBUFF, vecActivateBuff, 0, kPacket_.m_usWorldBuffDurationTime );	// ������ ���� �ð��� �����;� �Ѵ�.
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else
	switch( kPacket_.m_usSuccessWorldBuff )
	{
	case CXSLBuffManager::BTI_NONE:		// ���� ���� ���� �� ���� ����
		{
			// ���� ������ �ִٸ� ��� ã�Ƽ� ������
			if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS ) == true )
			{
				DeactivateBuff( CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS, vecDeleteBuff );
			}
			if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON ) == true )
			{
				DeactivateBuff( CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON, vecDeleteBuff );
			}
			if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL ) == true )
			{
				DeactivateBuff( CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL, vecDeleteBuff );
			}
		}break;
	case CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS:		// ���� ���� 1�ܰ�
		{
			ActivateBuff( CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS, vecActivateBuff );
		}break;
	case CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON:		// ���� ���� 2�ܰ�
		{
			ActivateBuff( CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON, vecActivateBuff );
		}break;
	case CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL:		// ���� ���� 3�ܰ�
		{
			ActivateBuff( CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL, vecActivateBuff );
		}break;
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	if( vecActivateBuff.empty() == false || vecDeleteBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecActivateBuff = vecActivateBuff;
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#else
	// �ʵ�, �� �����ΰ�� ��� �˷��ش�
	SendPacket( EGS_UPDATE_WORLD_BUFF_NOT, kPacket_ );

	// �� �����ΰ�츸 �濡 �˷��ش�.
	if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}
//}}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
IMPL_ON_FUNC( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT )
{
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
#else
	int iBuffID = 0;
	std::vector< KBuffInfo > vecActiveBuff;
	std::vector< int > vecDeleteBuff;
	if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_1 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_1;	
		DeactivateBuff( iBuffID, vecDeleteBuff );	
	}
	if(	m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_2 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_2;	
		DeactivateBuff( iBuffID, vecDeleteBuff );	
	}
	if(	m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_3 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_3;	
		DeactivateBuff( iBuffID, vecDeleteBuff );	
	}
	if(	m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_4 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_4;	
		DeactivateBuff( iBuffID, vecDeleteBuff );	
	}

	if( iBuffID != 0 )
	{
		bool bCheckBuff = false;

		switch( kPacket_.m_iTheGateOfDarknessIndividualBuffLevel )
		{
		case 1:	
			{
				iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_1; 
				bCheckBuff = true;
			}break;
		case 2:
			{
				iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_2; 
				bCheckBuff = true;
			}break;
		case 3: 
			{
				iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_3;
				bCheckBuff = true;
			}break;
		case 4: 
			{
				iBuffID = CXSLBuffManager::BTI_BUFF_HERO_OF_ELIOS_LEVEL_4; 
				bCheckBuff = true;
			}break;

		}

		if( bCheckBuff == true )
		{
			ActivateBuff( iBuffID, vecActiveBuff );
		}
	}

	if( vecActiveBuff.empty() == false || vecDeleteBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecActivateBuff = vecActiveBuff;
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
#else
	int iBuffID = 0;

	if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL1 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL1;	
		DeactivateBuff( iBuffID );	
	}
	if(	m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL2 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL2;	
		DeactivateBuff( iBuffID );	
	}
	if(	m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL3 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL3;	
		DeactivateBuff( iBuffID );	
	}
	if(	m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL4 )	== true )
	{ 
		iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL4;	
		DeactivateBuff( iBuffID );	
	}

	if( iBuffID != 0 )
	{
		bool bCheckBuff = false;

		switch( kPacket_.m_iTheGateOfDarknessIndividualBuffLevel )
		{
		case 1:	
			{
				iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL1; 
				bCheckBuff = true;
			}break;
		case 2:
			{
				iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL2; 
				bCheckBuff = true;
			}break;
		case 3: 
			{
				iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL3;
				bCheckBuff = true;
			}break;
		case 4: 
			{
				iBuffID = CXSLBuffManager::BI_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_LEVEL4; 
				bCheckBuff = true;
			}break;

		}

		if( bCheckBuff == true )
		{
			ActivateBuff( iBuffID );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}


//{{ 2012. 05. 17	�ڼ���	Ű���� ���� ������ DB���� �а� ����
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
IMPL_ON_FUNC( EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 4, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ, EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK );

	KDBE_KEYBOARD_MAPPING_INFO_WRITE_REQ kPacket;

	kPacket.m_iUserUID = GetUID();
	kPacket.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo = kPacket_.m_mapKeyboardMappingInfo;

#ifdef SERV_KEY_MAPPING_INT
	kPacket.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo = kPacket_.m_mapGamePadMappingInfo;
#endif //SERV_KEY_MAPPING_INT

	SendToAccountDB( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ, kPacket );
}

_IMPL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_WRITE_ACK, KEGS_KEYBOARD_MAPPING_INFO_WRITE_ACK )
{
	SendPacket( EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_READ_ACK, KEGS_KEYBOARD_MAPPING_INFO_NOT )
{
	SendPacket( EGS_KEYBOARD_MAPPING_INFO_NOT, kPacket_ );
}
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EGS_UPDATE_PLAY_STATUS_NOT )
{
	VERIFY_STATE_WARN( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �÷��� ���� ���� ����!
	bool bChangedHpMp = false;
	KPartyMemberStatus kPartyMemberStatus;
		
	// ��ȿ�� ���� ������ �ε��� ���� -> 0�̻��� ������ ����
	KStatTable::KUnitStatInfo	sUnitStat;
	SiKStatTable()->GetUnitStat( GetUnitClass(), m_ucLevel, sUnitStat );

	/// kimhc // ����ö // ��ŷ ������ HP�� �⺻ HP�� 3��
	/// MP�� 500 ���� ���� ��
	/// ���� ���� ��� (MP����)�� ���� üũ �ϵ��� �ؾ� �� ��...
	kPacket_.m_kGamePlayStatus.m_iMaxHP = max( sUnitStat.m_uiHP * 3, kPacket_.m_kGamePlayStatus.m_iMaxHP );
	kPacket_.m_kGamePlayStatus.m_iCurHP = max( kPacket_.m_kGamePlayStatus.m_iCurHP, kPacket_.m_kGamePlayStatus.m_iMaxHP );
	
	kPacket_.m_kGamePlayStatus.m_iMaxMP = max( 0, kPacket_.m_kGamePlayStatus.m_iMaxMP );
	kPacket_.m_kGamePlayStatus.m_iMaxMP = min( 500, kPacket_.m_kGamePlayStatus.m_iMaxMP );

	kPacket_.m_kGamePlayStatus.m_iCurMP = max( 0, kPacket_.m_kGamePlayStatus.m_iCurMP );
	kPacket_.m_kGamePlayStatus.m_iCurMP = min( kPacket_.m_kGamePlayStatus.m_iCurMP, kPacket_.m_kGamePlayStatus.m_iMaxMP );

	kPacket_.m_kGamePlayStatus.m_iCurHyperGage = max( 0, kPacket_.m_kGamePlayStatus.m_iCurHyperGage );
	kPacket_.m_kGamePlayStatus.m_cCharAbilType = max( 0, static_cast<int>(kPacket_.m_kGamePlayStatus.m_cCharAbilType) );
	
	m_kUserUnitManager.SetGamePlayStatusAndCheckChangedHpMp( kPacket_.m_kGamePlayStatus, bChangedHpMp, kPartyMemberStatus );

	// HpMp������ ����Ǿ��� ��Ƽ�Ҽ� ������� ��Ƽ���鿡�Ե� ����� hpMp�� ��������!
	if( bChangedHpMp  &&  GetPartyUID() != 0 )
	{
        SendToCnParty( EPM_UPDATE_GAME_PLAY_STATUS_NOT, kPartyMemberStatus );
	}

	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		SendToCnRoom( ERM_FIELD_USER_FRAME_INFO_NOT, kPacket_.m_iFrame );
	}
}

_IMPL_ON_FUNC( EPM_BROADCAST_PARTY_MEMBER_STATUS_NOT, KEGS_UPDATE_PARTY_MEMBER_STATUS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_PARTY_MEMBER_STATUS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_USE_SKILL_BUFF_IN_PLAY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
        SendToCnRoom( ERM_USE_SKILL_BUFF_IN_PLAY_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ERM_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT, KEGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT, kPacket_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 29	�ڼ���	ä�� �ɼ� ������ DB���� �а� ����
#ifdef SERV_CHAT_OPTION_INFO_RW
IMPL_ON_FUNC( EGS_CHAT_OPTION_INFO_WRITE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHAT_OPTION_INFO_WRITE_REQ, EGS_CHAT_OPTION_INFO_WRITE_ACK );

	KDBE_CHAT_OPTION_INFO_WRITE_REQ kPacket;

	kPacket.m_iUserUID = GetUID();
	kPacket.m_cIndex = kPacket_.m_cIndex;
	kPacket.m_bValue = kPacket_.m_bValue;
	SendToAccountDB( DBE_CHAT_OPTION_INFO_WRITE_REQ, kPacket );
}

_IMPL_ON_FUNC( DBE_CHAT_OPTION_INFO_WRITE_ACK, KEGS_CHAT_OPTION_INFO_WRITE_ACK )
{
	SendPacket( EGS_CHAT_OPTION_INFO_WRITE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( DBE_CHAT_OPTION_INFO_READ_ACK, KEGS_CHAT_OPTION_INFO_NOT )
{
	SendPacket( EGS_CHAT_OPTION_INFO_NOT, kPacket_ );
}
#endif SERV_CHAT_OPTION_INFO_RW
//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
_IMPL_ON_FUNC( DBE_UPDATE_EVENT_MONEY_ACK, int )
{
	if( GetEventMoney() != kPacket_ )
	{
		START_LOG( cerr, L"�̺�Ʈ ��ȭ - ��� ����� ���� �ʴ�." )
			<< BUILD_LOG( GetUserID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetEventMoney() )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;
	}

	KEGS_UPDATE_EVENT_MONEY_NOT kNot;
	kNot.m_iEventMoney = GetEventMoney();

	SendPacket( EGS_UPDATE_EVENT_MONEY_NOT, kNot );

	START_LOG(clog, L"õ���� ���� ���� Ȯ��")
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetEventMoney() )
		<< END_LOG;
}
#endif // SERV_EVENT_MONEY


//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
IMPL_ON_FUNC( EGS_UPDATE_USER_UNIT_BUFF_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"���� �濡 �������ִ� ���°� �ƴѵ� ���������� �������� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_vecBuffFactor.size() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_UPDATE_USER_UNIT_BUFF_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT, KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"���� �濡 �������ִ� ���°� �ƴѵ� ���������� �������� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_vecNpcUnitBuff.size() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_UPDATE_NPC_UNIT_BUFF_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_UPDATE_USER_UNIT_BUFF_INFO_ACK )
{
	VERIFY_STATE_LOG( clog, ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	m_kUserBuffManager.Init( kPacket_.m_kUnitBuffInfo );

	START_LOG( clog, L"���� ������ ���� ���� �Ǿ����ϴ�." )
		<< BUILD_LOG( GetCharName() )
		<< END_LOG;

	SendUpdateBuffInfo();

	//////////////////////////////////////////////////////////////////////////
	//std::vector< int > vecTest;
	//m_kUserBuffManager.GetBuffIDList( vecTest );
	//START_LOG( cout, L"[����üũ] ���� �ߵ��� ������ ��� �ִ°�?------------------------------" );
	//BOOST_TEST_FOREACH( const int, iBuffID, vecTest )
	//{
	//	START_LOG( cout, L"[����üũ] ���� �ߵ��� ����" )
	//		<< BUILD_LOG( iBuffID );
	//}
	//////////////////////////////////////////////////////////////////////////
}
#endif SERV_SERVER_BUFF_SYSTEM
//}}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
IMPL_ON_FUNC( ERM_UPDATE_QUEST_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_vecNPC.empty() == false )
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), kPacket_.m_vecNPC, true );
}
#endif SERV_REFORM_QUEST
//}}

//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
IMPL_ON_FUNC( EGS_UDP_CHECK_KICK_USER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	/*START_LOG( cout, L"������ �ǿ� ���� ��Ŷ Ȯ��!!")
		<< BUILD_LOG( kPacket_.m_iMyUnitUID )
		<< BUILD_LOG( kPacket_.m_iAccusationUnitUID )
		<< BUILD_LOG( kPacket_.m_byteNum )
		<< BUILD_LOG( kPacket_.m_bNowKickOut )
		<< END_LOG;*/

	SendToCnRoom( ERM_UDP_CHECK_KICK_USER_NOT, kPacket_ );

}
_IMPL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT )
{

	SendPacket( EGS_UDP_KICK_GAMEEDIT_NOT, kPacket_ );
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
_IMPL_ON_FUNC( DBE_2012_WINTER_VACATION_EVENT_NOT, int )
{
	m_iWinterVacationEventCount = kPacket_;
}
#endif SERV_2012_WINTER_VACATION_EVENT
//}}

//{{ 2012. 12. 18	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
IMPL_ON_FUNC( ERM_DEACTIVATE_BUFF_NOT )
{
	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );

	std::vector<int> vecDeleteBuff;

    BOOST_TEST_FOREACH( const int, iBuffID, kPacket_.m_vecDeactivateBuffID )
	{
		// ������ Ȱ��ȭ �������� ���� ���� Ȱ��ȭ ���¶�� ���� ��Ű��!
		if( m_kUserBuffManager.IsBuffActivated( iBuffID ) == true )
		{
            LIF( m_kUserBuffManager.DeactivateBuff( iBuffID, vecDeleteBuff ) );
		}
	}

	if( vecDeleteBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
//}}

//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
IMPL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	// Account DB���� ����ó���ǰ� �� ���..
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// Game DB �� ������ ��� �Ϸ� ����~
		SendToGameDB( DBE_NEW_ACCOUNT_QUEST_GAME_DB_REQ, kPacket_.m_kQuestReq );
	}
}

_IMPL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_GAME_DB_ACK, KDBE_NEW_QUEST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	// ���� ����Ʈ�� �ڵ� ���� �̹Ƿ� 
	// EGS_NEW_QUEST_ACK �� ������ �ʴ´�.

	// Account DB���� ���� ����Ʈ ����ó���ǰ�,
	// Game DB���� ������ ����ó���ǰ� �� ���..
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KQuestInstance kQuest;
		kQuest.m_iID			= kPacket_.m_iQuestID;
		kQuest.m_OwnorUnitUID	= GetCharUID();

		const CXSLQuestManager::QuestTemplet* QuestTem = SiCXSLQuestManager()->GetAccountQuestTemplet( kPacket_.m_iQuestID );
		if( QuestTem == NULL )
		{
			START_LOG( cerr, L"Account QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kPacket_.m_iQuestID )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;
			return;
		}

		KSubQuestInstance kSubQuest;
		BOOST_TEST_FOREACH( const int, iSubQuestID, QuestTem->m_vecSubQuest )
		{
			kSubQuest.m_iID = iSubQuestID;

			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< END_LOG;
				continue;
			}

			kQuest.m_vecSubQuestInstance.push_back( kSubQuest );
		}

		m_kUserAccountQuestManager.AddAccountQuest( kQuest );

		KEGS_NEW_QUEST_NOT kNot;
		kNot.m_QuestInst = kQuest;

		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kNot.m_vecUpdatedInventorySlot );
		kNot.m_vecUpdatedInventorySlot.insert( kNot.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

		SendPacket( EGS_NEW_QUEST_NOT, kNot );


		BOOST_TEST_FOREACH( const int, iSubQuestID, QuestTem->m_vecSubQuest )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
			if( IS_NULL( pSubQuestTemplet ) )
			{
				START_LOG( cerr, L"���� ���� �ʴ� ���� ����Ʈ �Դϴ�!" )
					<< BUILD_LOG( iSubQuestID )
					<< END_LOG;
				continue;
			}

			switch( pSubQuestTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_CHAR_LEVEL_UP:
				{
					// �ʵ� ���忡 ���� ����Ʈ �Ϸ� üũ!
					m_kUserAccountQuestManager.Handler_CharaterLevelUp_Account( GetThisPtr<KGSUser>() );
				}
				break;
			}
		}
	}
}

IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_QUEST_COMPLETE_ACK	kPacket;
		kPacket.m_iOK				 = kPacket_.m_iOK;
		kPacket.m_kCompleteQuestInfo.m_iQuestID = kPacket_.m_kQuestReq.m_iQuestID;
		kPacket.m_kCompleteQuestInfo.m_iCompleteCount = 0;
		kPacket.m_kCompleteQuestInfo.m_tCompleteDate = 0;
		SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
		return;
	}

	// Game DB �� ������ ��� �Ϸ� ����~
	SendToGameDB( DBE_ACCOUNT_QUEST_COMPLETE_GAME_DB_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_GAME_DB_ACK, KDBE_QUEST_COMPLETE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	//db���� �Ϸ�üũ�� �ް� ������
	m_kUserAccountQuestManager.Handler_DBE_QUEST_COMPLETE_ACK_Account( kPacket_, GetThisPtr<KGSUser>() );
	//- �������� ���� ���� ���� client���� �˷��ֱ� ���� ������Ʈ�� ���Ѿ� �ϳ� ���ƾ� �ϳ� ��~~

	// ������� ����Ʈ ���� ������
	std::map< int, KItemInfo >::iterator mit = kPacket_.m_mapInsertedItem.begin();
	for( ; mit != kPacket_.m_mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_QUEST, mit->second.m_iQuantity );
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_QUEST, mit->second.m_iQuantity );
	}
}
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2013. 02. 07   �÷���� ���� ��ȭ �̺�Ʈ �ǽð� ���� - ��μ�
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
IMPL_ON_FUNC_NOPARAM( EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ )
{
	KEGS_SUPPORT_MATERIAL_EVENT_TIME_ACK kAck;
	kAck = SiKGameSysVal()->IsEnchantEvent();
	SendPacket( EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK, kAck );
}
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
//}

// 2011.05.05 lygan_������ // �߱��� Ŀ�´�Ƽ�� ���� ���� ����
#ifdef SERV_USER_WATCH_NEW
IMPL_ON_FUNC( EGS_USER_COMMUNITY_SURVEY_NEW_REQ  )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//�ڱ� �ڽ��� ������ ��û�� ���.
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(GetCharName(), kPacket_.m_wUnitNickName))
#else
	if( GetCharName().compare( kPacket_.m_wUnitNickName ) == 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_USER_COMMUNITY_SURVEY_NEW_ACK kPacket; 
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK , kPacket );
		m_cCommunity_Tap_type = 0;
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wUnitNickName ) )
	{
		KEGS_USER_COMMUNITY_SURVEY_NEW_ACK kPacket; 
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK , kPacket );
		m_cCommunity_Tap_type = 0;
		return;
	}

	if( m_kTimer[TM_SEARCH_UNIT_REFRESH].elapsed() < (double)SEARCH_UNIT_REFRESH_TIME_LIMIT)
	{
		KEGS_USER_COMMUNITY_SURVEY_NEW_ACK kPacket; 
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_01;
		SendPacket( EGS_USER_COMMUNITY_SURVEY_NEW_ACK , kPacket );
		m_cCommunity_Tap_type = 0;

		m_kTimer[TM_SEARCH_UNIT_REFRESH].restart();
		return;
	}

	KELG_SEARCH_UNIT_REQ kPacketReq;
	kPacketReq.m_usEventID = EGS_USER_COMMUNITY_SURVEY_NEW_REQ;
	m_cCommunity_Tap_type = kPacket_.m_cCommunity_Tab_Type;
	kPacketReq.m_wstrNickName = kPacket_.m_wUnitNickName;
	SendToLoginServer( ELG_SEARCH_UNIT_REQ, kPacketReq );

}
#endif //SERV_USER_WATCH_NEW

#ifdef SERV_CATCH_HACKUSER_INFO
IMPL_ON_FUNC( EGS_CATCH_HACKUSER_INFO_NOT )
{
	SendToLogDB( DBE_CATCH_HACKUSER_INTO_NOT, kPacket_);
}
#endif SERV_CATCH_HACKUSER_INFO

#ifdef SERV_ADVERTISEMENT_EVENT
IMPL_ON_FUNC_NOPARAM( EGS_GET_ADVERTISEMENT_EVENT_REQ )
{
	KEGS_GET_ADVERTISEMENT_EVENT_ACK kPacket;

	m_kUserEventManager.GetAdvertisementURL(kPacket.m_vecAdvertisementURL);
	SendPacket( EGS_GET_ADVERTISEMENT_EVENT_ACK, kPacket );
}
#endif SERV_ADVERTISEMENT_EVENT

//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
IMPL_ON_FUNC( EGS_LOCAL_RANKING_USER_INFO_READ_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 4, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_LOCAL_RANKING_USER_INFO_READ_REQ, EGS_LOCAL_RANKING_USER_INFO_READ_ACK );

	if( kPacket_.m_wstrPassword.empty() == true )
	{
		// �����Ͱ� �ʱ�ȭ �Ǿ��ִ°�?
		if( m_kUserLocalRankingInfo.InitCheckUserInfo( GetUID() ) == false )
		{
			// DB���� �����͸� �о�´�.
			KDBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ kPacket;
			kPacket.m_iUserUID	= GetUID();
			kPacket.m_iUnitUID	= GetCharUID();
			kPacket.m_bFiltered	= true;
			SendToGameDB( DBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ, kPacket );
		}
		else
		{
			// �����͸� �о �����ش�.
			KEGS_LOCAL_RANKING_USER_INFO_READ_ACK kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			m_kUserLocalRankingInfo.GetFilteredUserInfo( kPacket.m_kInfo );
			m_kUserLocalRankingInfo.GetLastUpdateDate( kPacket.m_wstrUpdated );
			SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );
		}
		return;
	}
	else if( 50 < kPacket_.m_wstrPassword.size() )
	{
		KEGS_LOCAL_RANKING_USER_INFO_READ_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_08;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );
		return;
	}

	// soap �� �̿��Ͽ� ������ ��й�ȣ Ȯ���� ����
    if ( GetChannelCode() == KNexonAccountInfo::CE_NEXON_ACCOUNT ) 
    {
        KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ kPacketToSoap;
        kPacketToSoap.m_iNexonSN		= ( __int64 )m_kNexonAccountInfo.m_uiNexonSN;
        kPacketToSoap.m_strPassword		= KncUtil::toNarrowString( kPacket_.m_wstrPassword );

        KEventPtr spEvent( new KEvent );
        UidType anTrace[2] = { GetUID(), -1 };
        spEvent->SetData(PI_NULL, anTrace, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ, kPacketToSoap );

        SiKNexonSOAPManager()->QueueingEvent( spEvent );
    }
}

IMPL_ON_FUNC( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_LOCAL_RANKING_USER_INFO_READ_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_07;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );

		START_LOG( clog, L"Soap�� �̿��� ���� ��й�ȣ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		return;
	}

	// �����Ͱ� �ʱ�ȭ �Ǿ��ִ°�?
	if( m_kUserLocalRankingInfo.InitCheckUserInfo( GetUID() ) == false )
	{
		// DB���� �����͸� �о�´�.
		KDBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ kPacket;
		kPacket.m_iUserUID	= GetUID();
		kPacket.m_iUnitUID	= GetCharUID();
		kPacket.m_bFiltered	= false;
		SendToGameDB( DBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ, kPacket );
		return;
	}

	// �����͸� �о �����ش�.
	KEGS_LOCAL_RANKING_USER_INFO_READ_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	m_kUserLocalRankingInfo.GetUserInfo( kPacket.m_kInfo );
	m_kUserLocalRankingInfo.GetLastUpdateDate( kPacket.m_wstrUpdated );
	SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK )
{
	KEGS_LOCAL_RANKING_USER_INFO_READ_ACK kPacket;

	if( m_kUserLocalRankingInfo.InitCheck( GetUID(), GetCharUID() ) == false )
	{
		// ���� ���� ����
		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			kPacket.m_iOK = NetError::ERR_LOCALRANKING_04;
			SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );
			return;
		}
		
		m_kUserLocalRankingInfo.InitInfo( kPacket_.m_iUID, kPacket_.m_kUserInfo, kPacket_.m_wstrUpdated, kPacket_.m_kUnitInfo );
	}

	kPacket.m_iOK = NetError::NET_OK;
	if( kPacket_.m_bFiltered == true )
	{
		m_kUserLocalRankingInfo.GetFilteredUserInfo( kPacket.m_kInfo );
	}
	else
	{
		m_kUserLocalRankingInfo.GetUserInfo( kPacket.m_kInfo );
	}
	m_kUserLocalRankingInfo.GetLastUpdateDate( kPacket.m_wstrUpdated );
	SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK )
{
	if( m_kUserLocalRankingInfo.InitCheck( GetUID(), GetCharUID() ) == false )
	{
		// ���� ���� ����
		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			KEGS_LOCAL_RANKING_INQUIRY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_LOCALRANKING_04;
			SendPacket( EGS_LOCAL_RANKING_INQUIRY_ACK, kPacket );
			return;
		}

		m_kUserLocalRankingInfo.InitInfo( kPacket_.m_iUID, kPacket_.m_kUserInfo, kPacket_.m_wstrUpdated, kPacket_.m_kUnitInfo );
	}

	LocalRankingInquiryProcess( kPacket_.m_nViewPage
		, kPacket_.m_cMainTabIndex
		, kPacket_.m_cSubTabIndex
		, kPacket_.m_byteFilter
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// �۾���¥: 2013-05-15	// �ڼ���
		, kPacket_.m_bRankingButtonClick
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		);
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK )
{
	// ���������� �ʱ�ȭ�Ǿ� �ִ°�?
	if( m_kUserLocalRankingInfo.InitCheck( GetUID(), GetCharUID() ) == false )
	{
		// ���� ���� ����
		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			// DB �б� ���� �� ���� ��ü�� �����Ѵ�.
			return;
		}

		m_kUserLocalRankingInfo.InitInfo( kPacket_.m_iUID, kPacket_.m_kUserInfo, kPacket_.m_wstrUpdated, kPacket_.m_kUnitInfo );
	}

	IncreasedLocalRankingPoint( kPacket_.m_iIncreasedSpirit, kPacket_.m_iIncreasedAP, kPacket_.m_bForce );
}

IMPL_ON_FUNC( EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 4, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ, EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_kInfo.m_wstrProfile ) == false )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_02;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	if( 50 < kPacket_.m_wstrPassword.size() )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_08;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}

    if ( GetChannelCode() == KNexonAccountInfo::CE_NEXON_ACCOUNT ) 
    {
        // soap �� �̿��Ͽ� ������ ��й�ȣ Ȯ���� ����
        KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ kPacketToSoap;
        kPacketToSoap.m_iNexonSN		= ( __int64 )m_kNexonAccountInfo.m_uiNexonSN;
        kPacketToSoap.m_strPassword		= KncUtil::toNarrowString( kPacket_.m_wstrPassword );
        kPacketToSoap.m_kInfo			= kPacket_.m_kInfo;

        KEventPtr spEvent( new KEvent );
        UidType anTrace[2] = { GetUID(), -1 };
        spEvent->SetData(PI_NULL, anTrace, SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ, kPacketToSoap );

        SiKNexonSOAPManager()->QueueingEvent( spEvent );
    }
    else
    {
        // DB�� �����͸� ������Ʈ �Ϸ� ����!
        KDBE_LOCAL_RANKING_USER_INFO_WRITE_REQ kPackeToDB;
        kPackeToDB				= kPacket_.m_kInfo;
        kPackeToDB.m_iUserUID	= GetUID();
        SendToAccountDB( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ, kPackeToDB );
    }	
}

IMPL_ON_FUNC( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_07;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );

		START_LOG( cerr, L"Soap�� �̿��� ���� ��й�ȣ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		return;
	}

	// �����Ͱ� �ʱ�ȭ�Ǿ� �ִ°�?
	if( m_kUserLocalRankingInfo.InitCheckUserInfo( GetUID() ) == false )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_01;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}

	// ���� ������ �� �ִ°�?
#ifdef	SERV_LOCAL_RANKING_SYSTEM_INHOUSE// ���볯¥: �系 ����
#else	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE
	if( m_kUserLocalRankingInfo.CheckUpdate() == false )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_02;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}
#endif	// SERV_LOCAL_RANKING_SYSTEM_INHOUSE

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_kInfo.m_wstrProfile ) )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}

	// ������ ������ ��Ӿ� �˻�
#ifdef SERV_STRING_FILTER_USING_DB
	if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_kInfo.m_wstrProfile ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_kInfo.m_wstrProfile ) == false )
#endif //SERV_STRING_FILTER_USING_DB
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_09;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}

	if( 20 < kPacket_.m_kInfo.m_wstrProfile.size() )
	{
		KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_03;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}

	// DB�� �����͸� ������Ʈ �Ϸ� ����!
	KDBE_LOCAL_RANKING_USER_INFO_WRITE_REQ kPackeToDB;
	kPackeToDB				= kPacket_.m_kInfo;
	kPackeToDB.m_iUserUID	= GetUID();
	SendToAccountDB( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ, kPackeToDB );
}

IMPL_ON_FUNC( DBE_LOCAL_RANKING_USER_INFO_WRITE_ACK )
{
	KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KLocalRankingUserInfo kUserInfo;
		m_kUserLocalRankingInfo.GetUserInfo( kUserInfo );

		if( m_kUserLocalRankingInfo.IsFirstTimeInput() == true )
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 10518;	// ���� ���ҵ� �ް���, ���ҵ� �ް��� 5��
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}

		if( kPacket_.m_kInfo != kUserInfo )
		{
			// ������Ʈ�� �ð��� �Բ� ������ �����Ѵ�.
			m_kUserLocalRankingInfo.InitUserInfo( kPacket_.m_kInfo, kPacket_.m_wstrUpdated );

			// Login Server�� ���� ���� ���� �˸��� ������.
			IncreasedLocalRankingPoint( 0, 0, true );
		}
		
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::ERR_LOCALRANKING_05;
	SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ELG_LOCAL_RANKING_RESET_NOT )
{
	// GS���� DB �ʱ�ȭ�� �����͸� ���� �� ACK��
	// DB �ʱ�ȭ�� �Ǿ��ٴ� Reset ��ȣ ���Ŀ� �����ϸ�
	// ������ ����ġ�� �߻��� �� �ִ�.
	// DB Thread�� ���İ��� ������ �ذ��ϰ��� �Ͽ���.
	m_kUserLocalRankingInfo.ResetUnitInfo();
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_RANK_UP_NOT )
{
	SendPacket( EGS_LOCAL_RANKING_RANK_UP_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_LOCAL_RANKING_INQUIRY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 4, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_LOCAL_RANKING_INQUIRY_REQ, EGS_LOCAL_RANKING_INQUIRY_ACK );

	if( ( kPacket_.m_cMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= kPacket_.m_cMainTabIndex ) )
	{
		START_LOG( cerr, L"�߸��� cMainTabIndex �Դϴ�." )
			<< BUILD_LOG( kPacket_.m_cMainTabIndex )
			<< END_LOG;
		return;
	}

	// �����Ͱ� �ʱ�ȭ �Ǿ��ִ°�?
	if( m_kUserLocalRankingInfo.InitCheck( GetUID(), GetCharUID() ) == false )
	{
		// DB���� �����͸� �о�´�.
		KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ kPacket;
		kPacket.m_iUserUID			= GetUID();
		kPacket.m_iUnitUID			= GetCharUID();
		kPacket.m_nViewPage			= kPacket_.m_nViewPage;
		kPacket.m_cMainTabIndex		= kPacket_.m_cMainTabIndex;
		kPacket.m_cSubTabIndex		= kPacket_.m_cSubTabIndex;
		kPacket.m_byteFilter		= kPacket_.m_byteFilter;
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// �۾���¥: 2013-05-15	// �ڼ���
		kPacket.m_bRankingButtonClick	= kPacket_.m_bRankingButtonClick;
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		SendToGameDB( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ, kPacket );
		return;
	}

	LocalRankingInquiryProcess( kPacket_.m_nViewPage
		, kPacket_.m_cMainTabIndex
		, kPacket_.m_cSubTabIndex
		, kPacket_.m_byteFilter
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// �۾���¥: 2013-05-15	// �ڼ���
		, kPacket_.m_bRankingButtonClick
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		);
}

IMPL_ON_FUNC( EGS_LOCAL_RANKING_INQUIRY_ACK )
{
	SendPacket( EGS_LOCAL_RANKING_INQUIRY_ACK, kPacket_ );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_ADD_WARP_BUTTON
IMPL_ON_FUNC( EGS_WARP_BY_BUTTON_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_WARP_BY_BUTTON_REQ, EGS_WARP_BY_BUTTON_ACK );

	KEGS_WARP_BY_BUTTON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

	int iWarpPointMapID = 0;

	//{{ 2012. 09. 21   ��μ�   ���� ���� ���� �޽��� ����
#ifdef SERV_ENTER_VILLAGE_ERROR
	int iTempLevel = 0;
	int iTempDungeonID = 0;
	if( CheckEnterTheVillage( kPacket_.m_iCurrentVillageWarpIndex , iTempLevel, iTempDungeonID ) == false )
#else
	if( CheckEnterTheVillage( kPacket_.m_iCurrentVillageWarpIndex ) == false )
#endif SERV_ENTER_VILLAGE_ERROR
		//}}
	{
		kPacketAck.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_05;
		SendPacket( EGS_WARP_BY_BUTTON_ACK, kPacketAck );
		return;
	}

	iWarpPointMapID = static_cast<int>( kPacket_.m_iCurrentVillageWarpIndex );

	if( true == kPacket_.m_bFreeWarp )
	{
		// �Ⱓ �ٽ� �ѹ� �˻�
		KUnitInfo kUnitInfo;
		GetUnitInfo(kUnitInfo);
		CTime ctVipTime = kUnitInfo.m_trWarpVipEndData;
		if( ctVipTime < CTime::GetCurrentTime() )
		{
			START_LOG(cerr, L"�ں� VIP �Ⱓ �Ϸ�. ��Ŷ �ӵ��� ������ �߻� �� ���� �ִ�.")
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( ctVipTime.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetString())
				<< BUILD_LOG( ctVipTime.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetString())
				<< END_LOG;

			// VIP �Ⱓ�� �������ϴ�. �ٽ� �ѹ� �õ��� �ּ���
			kPacketAck.m_iOK = NetError::ERR_COBO_EXPRESS_01;
			SendPacket( EGS_WARP_BY_BUTTON_ACK, kPacketAck );
			return;
		}
	}
	else
	{
		if( ( GetMapID() == iWarpPointMapID )
			|| ( SiCXSLMapData()->CheckCOBOExpressTicketMapID( GetMapID() ) == 0 )
			|| ( SiCXSLMapData()->CheckCOBOExpressTicketMapID( iWarpPointMapID ) == 0 )
			)
		{
			kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_WARP_BY_BUTTON_ACK, kPacketAck );
			return;
		}
		else
		{
			int iCOBOExpressTicketCost = SiCXSLMapData()->ComputeCOBOExpressTicketCost( GetMapID(), iWarpPointMapID, GetLevel() );
			if( GetED() < iCOBOExpressTicketCost )
			{
				kPacketAck.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_08;
				SendPacket( EGS_WARP_BY_BUTTON_ACK, kPacketAck );
				return;
			}

			//{{ 2012. 10. 16	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			DecreaseED( iCOBOExpressTicketCost, KUserEDManager::ER_OUT_ED_USE_COBO_EXPRESS_TICKET );
#else
			DecreaseED( iCOBOExpressTicketCost );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}
		}
	}

	// �̵� ����
	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_iED = GetED();
	kPacketAck.m_iWarpPointMapID = iWarpPointMapID;

	SendPacket( EGS_WARP_BY_BUTTON_ACK, kPacketAck );
}
#endif // SERV_ADD_WARP_BUTTON

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-04-05
IMPL_ON_FUNC( EGS_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//�ڱ� �ڽ��� ������ ��û�� ���.
	if( GetCharUID() == kPacket_.m_iUnitUID )
	{
		KEGS_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		GetLocalRankingWatchUnitInfo( kPacket.m_kUserInfo, kPacket.m_kUnitInfo );
		SendPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
		return;
	}

	if( m_kTimer[TM_SEARCH_UNIT_REFRESH].elapsed() < KGSUser::SEARCH_UNIT_REFRESH_TIME_LIMIT )
	{
		KEGS_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_01;
		SendPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
		return;
	}
	m_kTimer[TM_SEARCH_UNIT_REFRESH].restart();

	if( KncUid::ExtractServerGroupID( GetCharUID() ) == KncUid::ExtractServerGroupID( kPacket_.m_iUnitUID ) )
	{
		// �α��� ������ ������!
		KELG_LOCAL_RANKING_WATCH_UNIT_REQ kPacket;
		kPacket.m_iRequestGSUID		= KBaseServer::GetKObj()->GetUID();
		kPacket.m_iRequestUnitUID	= GetCharUID();
		kPacket.m_iTargetUnitUID	= kPacket_.m_iUnitUID;
		SendToLoginServer( ELG_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket );
		return;
	}
	else if( ( GetStateID() == KGSFSM::S_ROOM ) && ( 0 < GetRoomUID() ) )
	{
		// ���� ������ ������!
		KERM_LOCAL_RANKING_WATCH_UNIT_REQ kPacket;
		kPacket.m_iRequestUnitUID	= KBaseServer::GetKObj()->GetUID();
		kPacket.m_iRequestUnitUID	= GetCharUID();
		kPacket.m_iTargetUnitUID	= kPacket_.m_iUnitUID;
		SendToCnRoom( ERM_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket );
		return;
	}

	KEGS_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_LOCALRANKING_12;
	SendPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	KELG_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK				= NetError::NET_OK;
	GetLocalRankingWatchUnitInfo( kPacket.m_kUserInfo, kPacket.m_kUnitInfo );
	kPacket.m_iRequestGSUID		= kPacket_.m_iRequestGSUID;
	kPacket.m_iRequestUnitUID	= kPacket_.m_iRequestUnitUID;

	SendToLoginServer( ELG_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	KEGS_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK		= kPacket_.m_iOK;
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		kPacket.m_kUserInfo	= kPacket_.m_kUserInfo;
		kPacket.m_kUnitInfo	= kPacket_.m_kUnitInfo;
	}
	SendPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	KERM_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK				= NetError::NET_OK;
	GetLocalRankingWatchUnitInfo( kPacket.m_kUserInfo, kPacket.m_kUnitInfo );
	kPacket.m_iRequestGSUID		= kPacket_.m_iRequestGSUID;
	kPacket.m_iRequestUnitUID	= kPacket_.m_iRequestUnitUID;

	SendToCnRoom( ERM_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	KEGS_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK		= kPacket_.m_iOK;
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		kPacket.m_kUserInfo	= kPacket_.m_kUserInfo;
		kPacket.m_kUnitInfo	= kPacket_.m_kUnitInfo;
	}
	SendPacket( EGS_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
IMPL_ON_FUNC( EGS_FORCE_RELAY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( ( SiKGameSysVal()->GetLanBugOutCheck() == true ) &&
		( 0 < GetRoomUID() )
		)
	{
		SendToCnRoom( ERM_FORCE_RELAY_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ERM_FORCE_RELAY_NOT, KEGS_FORCE_RELAY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_FORCE_RELAY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_LEAVE_ROOM_REQ kPacket;
		SendToCnRoom( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_REQ, kPacket );
	}
	else
	{
		KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kPacket;
		kPacket.m_iOK	= NetError::ERR_UDP_01;
		SendPacket( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket );

		START_LOG( clog, L"���� ���� ������ ƨ��� Ȯ��")
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}
#else
	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kPacket;
	kPacket.m_iOK	= NetError::ERR_UDP_01;
	SendPacket( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
IMPL_ON_FUNC( EGS_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT )
{
	KDBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT kPacketLogDB;
	kPacketLogDB.m_iUDPPortSuccessType = kPacket_.m_iUDPPortSuccessType;
	kPacketLogDB.m_iUseUdpPort = kPacket_.m_iUseUdpPort;
	kPacketLogDB.m_iUnitUID = GetCharUID();

	SendToLogDB( DBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT, kPacketLogDB );
}
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG

#ifdef SERV_SKILL_USE_SUBQUEST
IMPL_ON_FUNC( EGS_SKILL_USE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
	
	if( kPacket_.m_iSkillID == -1 )
		return;

	// ����, ����, ��Ʋ�ʵ� ������ üũ�մϴ�.
	if( CXSLRoom::RT_DUNGEON != CXSLRoom::GetRoomType( GetRoomUID() ) &&
		CXSLRoom::RT_PVP != CXSLRoom::GetRoomType( GetRoomUID() ) &&
		CXSLRoom::RT_BATTLE_FIELD != CXSLRoom::GetRoomType( GetRoomUID() ) )
		return;

	if( CXSLDungeon::IsTutorialDungeon( m_kUserDungeonManager.GetDungeonID() ) == true ||
		CXSLDungeon::IsHenirDungeon( m_kUserDungeonManager.GetDungeonID() ) == true ||
		CXSLDungeon::IsRubenDungeon( m_kUserDungeonManager.GetDungeonID() ) == true ||
		CXSLDungeon::IsTCDungeon( m_kUserDungeonManager.GetDungeonID() ) == true)
		return;
	
	m_kUserQuestManager.Handler_EGS_SKILL_USE_REQ( kPacket_.m_iSkillID, GetThisPtr<KGSUser>() );
}
#endif SERV_SKILL_USE_SUBQUEST

#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-10	// �ڼ���
IMPL_ON_FUNC( DBE_JUMPING_CHARACTER_INFO_NOT )
{
	// �ʱ�ȭ
	if( m_kUserJumpingCharacterManager.Init( kPacket_.m_mapJumpingInfo ) == JumpingCharacter::JCR_FAILED )
	{
		START_LOG( cerr, L"���� ĳ���� ���� ���ÿ� ���� �Ͽ����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	if( JumpingCharacterCheck() == NetError::NET_OK )
	{
		SendID( EGS_JUMPING_CHARACTER_GUIDE_NOT );
	}
}

IMPL_ON_FUNC( EGS_JUMPING_CHARACTER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JUMPING_CHARACTER_REQ, EGS_JUMPING_CHARACTER_ACK );

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			break;

		default:
			{
				KEGS_JUMPING_CHARACTER_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_JUMPING_06;	// ������ �����̳� �ʵ忡���� �����մϴ�.
				SendPacket( EGS_JUMPING_CHARACTER_ACK, kPacket );
				return;
			}
			break;
		}
	}

	int		iLevel = 0;
	CTime	tCurrentDate;
	int		iResult = JumpingCharacterCheck( iLevel, tCurrentDate );

	if( iResult != NetError::NET_OK )
	{
		KEGS_JUMPING_CHARACTER_ACK kPacket;
		kPacket.m_iOK = iResult;
		SendPacket( EGS_JUMPING_CHARACTER_ACK, kPacket );
		return;
	}

	// ������ �غ�
	KDBE_JUMPING_CHARACTER_UPDATE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID		= GetCharUID();
	kPacketToDB.m_iLevel		= iLevel;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iTheNumberOfSkillPagesAvailable	= m_kSkillTree.GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

	// ����� ��Ʈ ������ ó��
	if( m_kSkillTree.IsHaveSkillNote() == true )
	{
		kPacketToDB.m_cExpandedMaxPageNum	= -1;
		char cExpandedMaxPageNum			= 0;
		if( m_kSkillTree.GetExpandSkillNotePage( iLevel, cExpandedMaxPageNum ) == true )
		{
			if( m_kSkillTree.GetSkillNoteMaxPageNum() != cExpandedMaxPageNum )
			{
				kPacketToDB.m_cExpandedMaxPageNum = cExpandedMaxPageNum;
			}
		}
	}

	// ���� �⺻ ��ų ó��
	if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>( kPacket_.m_iClass ) ) == false )
	{
		START_LOG( cerr, L"������ ���� �������� ���� �Ϸ� �մϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iClass )
			<< END_LOG;

		KEGS_JUMPING_CHARACTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_JUMPING_07;	// ���� �Ϸ��� ��� ������ �̻��մϴ�.
		SendPacket( EGS_JUMPING_CHARACTER_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iClass		= kPacket_.m_iClass;

	int iDefaultSkill[6] = {0,};
	if( SiCXSLSkillTree()->GetUnitClassDefaultSkill( kPacket_.m_iClass, iDefaultSkill[0], iDefaultSkill[1], iDefaultSkill[2], iDefaultSkill[3], iDefaultSkill[4], iDefaultSkill[5] ) == false )
	{
		START_LOG( cerr, L"������ �����Ϸ��� �⺻ ��ų�� ���� ���Ͽ����ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( kPacket_.m_iClass )
			<< END_LOG;

		KEGS_JUMPING_CHARACTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_JUMPING_08;	// ���� ĳ���� �����Դϴ�.
		SendPacket( EGS_JUMPING_CHARACTER_ACK, kPacket );
		return;
	}

	if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == true )
	{
		// �⺻ �������� 2�� �������� �����Ѵ�.
		kPacketToDB.m_vecNewDefaultSkill.push_back( iDefaultSkill[2] );
		kPacketToDB.m_vecNewDefaultSkill.push_back( iDefaultSkill[3] );
		kPacketToDB.m_vecNewDefaultSkill.push_back( iDefaultSkill[4] );
		kPacketToDB.m_vecNewDefaultSkill.push_back( iDefaultSkill[5] );
	}
	else if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == true )
	{
		// 1�� �������� 2�� �������� �����Ѵ�.
		kPacketToDB.m_vecNewDefaultSkill.push_back( iDefaultSkill[4] );
		kPacketToDB.m_vecNewDefaultSkill.push_back( iDefaultSkill[5] );
	}

	// ���� ��ų ó��
	const CXSLSkillTree::SkillTreeTempletMap* pSkillTreeTempletMap = SiCXSLSkillTree()->GetSkillTreeTemplet( kPacket_.m_iClass );
	if( pSkillTreeTempletMap == NULL )
	{
		START_LOG( cerr, L"�ش� Ŭ������ ��ų ������ ���� �� �����ϴ�.")
			<< BUILD_LOG( GetUnitClass() )
			<< END_LOG;

		KEGS_JUMPING_CHARACTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_JUMPING_08;	// ���� ĳ���� �����Դϴ�.
		SendPacket( EGS_JUMPING_CHARACTER_ACK, kPacket );
		return;
	}

	for( CXSLSkillTree::SkillTreeTempletMap::const_iterator it = pSkillTreeTempletMap->begin(); it != pSkillTreeTempletMap->end(); ++it )
	{
		if( SiCXSLSkillTree()->IsSealSkill( it->first ) == true )
		{
			kPacketToDB.m_vecUnSealedSkill.push_back( it->first );
		}
	}

	kPacketToDB.m_wstrRegDate	= tCurrentDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );	
	
	m_kUserJumpingCharacterManager.SetJumpingCharacter( kPacketToDB.m_iUnitUID, tCurrentDate );
	SendToGameDB( DBE_JUMPING_CHARACTER_UPDATE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_JUMPING_CHARACTER_UPDATE_ACK )
{
	KEGS_JUMPING_CHARACTER_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ĳ���� ������Ʈ ����" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		m_kUserJumpingCharacterManager.ResetJumpingCharacter( kPacket_.m_iUnitUID );
		kPacket.m_iOK = NetError::ERR_JUMPING_08;	// ���� ĳ���� �����Դϴ�.
	}
	else
	{
		// 1. ����ġ, ����, SP Point ó��
		const int iTotalExp = SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_iLevel );
		if( m_kEXP.GetExp() < iTotalExp )
		{
			m_kEXP.AddExp( iTotalExp - m_kEXP.GetExp() );
			CheckJumpingCharLevelUp( kPacket_.m_iLevel );
		}

		// ����� ��Ʈ ������ ó��
		if( 0 <= kPacket_.m_cExpandedMaxPageNum )
		{
			m_kSkillTree.UpdateSkillNoteMaxPageNum( kPacket_.m_cExpandedMaxPageNum );
		}

		// Class ���� ó��
		if( 0 <= kPacket_.m_iClass )
		{
			SetUnitClass( static_cast<char>( kPacket_.m_iClass ) );
			ResetStat();
			m_kSkillTree.SetUnitClass( static_cast<int>( GetUnitClass() ) );
			
			BOOST_TEST_FOREACH( const int, iSkillID, kPacket_.m_vecNewDefaultSkill )
			{
#ifdef SERV_SKILL_PAGE_SYSTEM
				// kimhc // ����ö // ���� ĳ���� ��, ��� ��ų ������ ����
				m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( iSkillID, 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
				m_kSkillTree.SetSkillLevelAndCSP( iSkillID, 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
			}

			//{{ 2013. 09. 24	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
#ifdef SERV_RELAY_DB_CONNECTION
			SendUpdateUnitInfoToRelayDB( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT::UT_UNITCLASS_CHANGE );
#endif SERV_RELAY_DB_CONNECTION
			//}}
		}

		// ���� ��ų ó��
		BOOST_TEST_FOREACH( const short, iSkillID, kPacket_.m_vecUnSealedSkill )
		{
			m_kSkillTree.SkillUnseal( static_cast<int>( iSkillID ) );
		}

		// ���� �α� �����
		KDBE_UPDATE_UNIT_CLASS_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_cUnitClass = GetUnitClass();
		SendToLogDB( DBE_UPDATE_UNIT_CLASS_NOT, kPacketNot );

		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
		{
			WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_JOB_CHANGE );
		}
		else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
		{
			WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_SECOND_JOB_CHANGE );
		}
		else
		{
			START_LOG( cerr, L"1�� ������ 2�� ������ �ƴѵ� �α׸� ����Ϸ��� �ϳ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( GetUnitClass() )
				<< END_LOG;
		}
#endif SERV_CHAR_LOG
		//}}

		if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			// �ʵ忡 ������� ���� ���� �����ϱ�
			if( GetFieldUID() > 0 )
			{
				KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
				GetFieldUserInfo( kNot.m_kInfo );
				SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
			}
		}		
		else if( GetStateID() == KGSFSM::S_ROOM )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				//{{ ��Ʋ�ʵ� : �ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				{
					KRoomUserInfo kRoomUserInfo;
					GetRoomUserInfo( kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				{
					KSquareUserInfo kSquareUserInfo;
					GetSquareUserInfo( kSquareUserInfo );
					SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
				}
				break;

			default:
				{
					START_LOG( cerr, L"�̻��� �� Ÿ��." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}
		}

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}

		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_iRewardID	   = 10507;
		kPacketToDB.m_sQuantity	   = 1;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

		kPacket.m_iOK = NetError::NET_OK;
		GetUnitInfo( kPacket.m_kUnitInfo );
		kPacket.m_cExpandedMaxPageNum	= kPacket_.m_cExpandedMaxPageNum;
		kPacket.m_vecNewDefaultSkill	= kPacket_.m_vecNewDefaultSkill;
		kPacket.m_vecUnSealedSkill		= kPacket_.m_vecUnSealedSkill;
	}

	SendPacket( EGS_JUMPING_CHARACTER_ACK, kPacket );
}
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
IMPL_ON_FUNC( DBE_GET_TIME_CONTROL_ITME_LIST_NOT )
{
	SendPacket( EGS_GET_TIME_CONTROL_ITME_LIST_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_BUY_UI_SETTING_REQ )
{
	KEGS_BUY_UI_SETTING_ACK kPacketBuyUI;

	if ( m_bTimeControlItemCheckDungeonPlay == true )
	{

		kPacketBuyUI.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
		SendPacket( EGS_BUY_UI_SETTING_ACK, kPacketBuyUI );

		return;
	}

	std::set<int> setBanBuyItemInven;
	setBanBuyItemInven.clear();

	CTime m_tStartDate;
	CTime m_tEndDate;

	std::vector<KPacketGetItemOnOff>::iterator vitrGetItemOnOff;
	std::map<int , std::vector<KPacketGetItemOnOff> > mapGetItemOnOff =  GetKGSSimLayer()->GetTimeControlItem_Info();
	std::map<int , std::vector<KPacketGetItemOnOff> >::iterator mit = mapGetItemOnOff.find( kPacket_.m_iTimeControlItemType );

	if ( mit != mapGetItemOnOff.end() )
	{
		for ( vitrGetItemOnOff = mit->second.begin(); vitrGetItemOnOff != mit->second.end(); ++vitrGetItemOnOff )
		{
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrStartTime , m_tStartDate);
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrEndTime , m_tEndDate);

			if( CTime::GetCurrentTime() >= m_tStartDate && CTime::GetCurrentTime() <= m_tEndDate )
			{
				setBanBuyItemInven.insert(vitrGetItemOnOff->m_iItemID);
			}
		}
	}

	kPacketBuyUI.m_iOK = NetError::NET_OK;
	kPacketBuyUI.m_setGetItemOnOff = setBanBuyItemInven;
	kPacketBuyUI.m_iHouseID = kPacket_.m_iHouseID;
	kPacketBuyUI.m_iTimeControlItemType = kPacket_.m_iTimeControlItemType;
	SendPacket( EGS_BUY_UI_SETTING_ACK, kPacketBuyUI );
}

IMPL_ON_FUNC( EGS_GET_TIME_CONTROL_ITME_TALK_LIST_REQ )
{
	if ( m_bTimeControlItemCheckDungeonPlay == true )
	{
		KEGS_BUY_UI_SETTING_ACK kPacketBuyUI;	// 2012.12.20 lygan_������ // ������ �ݿ����� �߼� �ؾ� ������ �� ��Ŷ ��Ȱ��
		kPacketBuyUI.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
		SendPacket( EGS_BUY_UI_SETTING_ACK, kPacketBuyUI );
		return;
	}

	KEGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK kPacket;
	CTime m_tStartDate;
	CTime m_tEndDate;
	std::map<int , std::vector<KPacketGetItemOnOff> > mapGetItemOnOff =  GetKGSSimLayer()->GetTimeControlItem_Info();
	std::map<int , std::vector<KPacketGetItemOnOff> >::iterator mit = mapGetItemOnOff.begin();

	for ( ; mit != mapGetItemOnOff.end(); ++mit )
	{
		std::vector<KPacketGetItemOnOff> vecTempItemOnOff;

		BOOST_TEST_FOREACH( KPacketGetItemOnOff , kTimeControlItem, mit->second )
		{
			KncUtil::ConvertStringToCTime(kTimeControlItem.m_wstrStartTime , m_tStartDate);
			KncUtil::ConvertStringToCTime(kTimeControlItem.m_wstrEndTime , m_tEndDate);

			if( CTime::GetCurrentTime() >= m_tStartDate && CTime::GetCurrentTime() <= m_tEndDate )
			{
				vecTempItemOnOff.push_back(kTimeControlItem);
			}
		}

		kPacket.m_mapGetItemOnOff.insert(std::make_pair(mit->first, vecTempItemOnOff ));
	}

	SendPacket( EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK, kPacket );
}
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
IMPL_ON_FUNC( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ������ �޾����Ƿ� �̺�Ʈ ��󿡼� ����
		SetRewardBonusItem(kPacket_.m_iRewardBonusItem);
	}
}
#endif SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_MOMOTI_EVENT
//m_iCheckReward ( 0 : �̹�������, 1 : �̹� �������� �Ϸ�, 2 : ���� Ʋ�� )

IMPL_ON_FUNC( EGS_MOMOTI_QUIZ_EVENT_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KDBE_MOMOTI_QUIZ_EVENT_REQ kPacketDBEMomotiQuizEventReq;
	
	kPacketDBEMomotiQuizEventReq.m_iOK = NetError::NET_OK;
	kPacketDBEMomotiQuizEventReq.m_iUserUID = GetUID();
	kPacketDBEMomotiQuizEventReq.m_iUnitUID = GetCharUID();
	kPacketDBEMomotiQuizEventReq.m_istrReply = kPacket_.m_istrReply;
	SendToGameDB( DBE_MOMOTI_QUIZ_EVENT_REQ, kPacketDBEMomotiQuizEventReq );

}
IMPL_ON_FUNC( DBE_MOMOTI_QUIZ_EVENT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// ������ ���� Ŭ���̾�Ʈ ����
	KEGS_MOMOTI_QUIZ_EVENT_ACK kPacketEGSMomotiQuizEventAck;
	kPacketEGSMomotiQuizEventAck.m_iOK = NetError::NET_OK;
	kPacketEGSMomotiQuizEventAck.m_iCheckReward = kPacket_.m_iCheckReward;
	SendPacket( EGS_MOMOTI_QUIZ_EVENT_ACK, kPacketEGSMomotiQuizEventAck );
}
#endif SERV_MOMOTI_EVENT

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
IMPL_ON_FUNC( DBE_CHANGE_PET_ID_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	KDBE_CHANGE_PET_ID_LOG_NOT kPacketLog;
	kPacketLog.m_iUnitUID		= GetCharUID();
	kPacketLog.m_iPetUID		= kPacket_.m_iPetUID;
	kPacketLog.m_iBeforePetID	= kPacket_.m_iBeforePetID;
	kPacketLog.m_iAfterPetID	= kPacket_.m_iAfterPetID;
	SendToLogDB( DBE_CHANGE_PET_ID_LOG_NOT, kPacketLog );

	KEGS_CHANGE_PET_ID_NOT kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPetUID = kPacket_.m_iPetUID;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = NetError::ERR_PET_25;

		START_LOG( cerr, L"�ҷ��� ���� �� ���� ���� DB ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iBeforePetID )
			<< BUILD_LOG( kPacket_.m_iAfterPetID )
			<< END_LOG;
	}

	SendPacket( EGS_CHANGE_PET_ID_NOT, kPacket );
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_CHINA_SPIRIT_EVENT
IMPL_ON_FUNC( EGS_USE_SPIRIT_REWARD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_SPIRIT_REWARD_REQ, EGS_USE_SPIRIT_REWARD_ACK );

	KEGS_USE_SPIRIT_REWARD_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

	IF_EVENT_ENABLED( CEI_2013_CHINA_SPIRIT_EVENT )
	{
	}
	ELSE
	{
		START_LOG( cerr, L"�̺�Ʈ �Ⱓ�� �ƴѵ� ��Ŷ�� ����. ��Ŀ!")
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		SendPacket( EGS_USE_SPIRIT_REWARD_ACK, kPacketAck );
		return;
	}

	IF_EVENT_ENABLED( CEI_2013_CHINA_SPIRIT_EVENT )
	{
	}
	ELSE
	{
		START_LOG( cerr, L"�̺�Ʈ �Ⱓ�� �ƴѵ� ��Ŷ�� ����. ��Ŀ!")
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		SendPacket( EGS_USE_SPIRIT_REWARD_ACK, kPacketAck );
		return;
	}

	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_iToUnitUID   = GetCharUID();
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;

	int iChinaSpirit = GetChinaSpirit( kPacket_.m_iLocationIndex );
	if( iChinaSpirit < 0 )
	{
		START_LOG( cerr, L"m_iLocationIndex ���� ������. ��Ŀ!")
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iLocationIndex )
			<< END_LOG;

		SendPacket( EGS_USE_SPIRIT_REWARD_ACK, kPacketAck );
		return;
	}

	if( iChinaSpirit < 2350 )
	{
		START_LOG( cerr, L"�׷����� �� �� ä���µ� ������ �޶��? ��Ŀ!")
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iLocationIndex )
			<< BUILD_LOG( iChinaSpirit )
			<< END_LOG;

		SendPacket( EGS_USE_SPIRIT_REWARD_ACK, kPacketAck );
		return;
	}

	// �˻� ���. ���� ���� �� �׷��� �ʱ�ȭ. ���� ack ����
	SetChinaSpirit( kPacket_.m_iLocationIndex, 0 );

	// ù��° Ƽ���� ������ID 1492
	// ���� +1��
	kPacketToDB.m_iRewardID = 1492 + kPacket_.m_iLocationIndex;
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_iLocationIndex = kPacket_.m_iLocationIndex;
	for( int i = 0; i < 6; ++i )
	{
		kPacketAck.m_arrChinaSpirit[ i ] = GetChinaSpirit( i );
	}

	SendPacket( EGS_USE_SPIRIT_REWARD_ACK, kPacketAck );
}
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_RECRUIT_EVENT_BASE
_IMPL_ON_FUNC( ELG_REGISTER_RECRUITER_NOT, KEGS_REGISTER_RECRUITER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserRecommendManager.IsInitRecruitRecruiterList() == true )
	{
		KRecommendUserInfo kRecruitUnitInfo;
		kRecruitUnitInfo = kPacket_.m_kRecruitUnitInfo;
		kRecruitUnitInfo.m_bIsOnline = true;
		m_kUserRecommendManager.AddRecruitUnitInfo( kRecruitUnitInfo );
	}

	SendPacket( EGS_REGISTER_RECRUITER_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_RECRUIT_RECRUITER_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��õ�� ����Ʈ�� �ʱ�ȭ �Ǿ����� Ȯ��
	if( m_kUserRecommendManager.IsInitRecruitRecruiterList() == false )
	{
		KDBE_GET_RECRUIT_RECRUITER_LIST_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		SendToGameDB( DBE_GET_RECRUIT_RECRUITER_LIST_REQ, kPacketToDB );
		return;
	}

	KEGS_GET_RECRUIT_RECRUITER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// ��õ�� ����Ʈ ���
	m_kUserRecommendManager.GetRecruitUnitList( kPacket.m_vecRecruitUnitInfo );
	m_kUserRecommendManager.GetRecruiterUnitList( kPacket.m_vecRecruiterUnitInfo );

	// �α��μ����� ���� ���� �������� ��õ������ üũ�Ѵ�.
	SendToLoginServer( ELG_GET_RECRUIT_RECRUITER_LIST_REQ, kPacket );
}

_IMPL_ON_FUNC( DBE_GET_RECRUIT_RECRUITER_LIST_ACK, KEGS_GET_RECRUIT_RECRUITER_LIST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��õ�� ����Ʈ �޾ƿ��� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_GET_RECRUIT_RECRUITER_LIST_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_GET_RECRUIT_RECRUITER_LIST_ACK, kPacket );
		return;
	}

	// �α��μ����� ���� ���� �������� ��õ������ üũ�Ѵ�.
	SendToLoginServer( ELG_GET_RECRUIT_RECRUITER_LIST_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GET_RECRUIT_RECRUITER_LIST_ACK, KEGS_GET_RECRUIT_RECRUITER_LIST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��õ�� ����Ʈ �ʱ�ȭ
	m_kUserRecommendManager.SetRecruitUnitList( kPacket_.m_vecRecruitUnitInfo );
	m_kUserRecommendManager.SetRecruiterUnitList( kPacket_.m_vecRecruiterUnitInfo );

	KEGS_GET_RECRUIT_RECRUITER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// ��õ�� ����Ʈ ���
	kPacket.m_vecRecruitUnitInfo = kPacket_.m_vecRecruitUnitInfo;
	kPacket.m_vecRecruiterUnitInfo = kPacket_.m_vecRecruiterUnitInfo;

	SendPacket( EGS_GET_RECRUIT_RECRUITER_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_RECRUIT_RECRUITER_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��õ�� ���� ����
	m_kUserRecommendManager.ModifyRecruitRecruiterState( kPacket_ );

	// Ŭ���̾�Ʈ�� �˸�
	KEGS_RECRUIT_RECRUITER_INFO_NOT kPacket;
	kPacket.m_iSenderUnitUID = kPacket_.m_iSenderUnitUID;
	kPacket.m_cType = kPacket_.m_cType;
	kPacket.m_iData = kPacket_.m_iData;
	SendPacket( EGS_RECRUIT_RECRUITER_INFO_NOT, kPacket );
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
IMPL_ON_FUNC_NOPARAM( EGS_GET_EVENT_INFO_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_GET_EVENT_INFO_REQ, EGS_GET_EVENT_INFO_ACK );

	SendToGameDB( DBE_GET_EVENT_INFO_REQ );
}

_IMPL_ON_FUNC( DBE_GET_EVENT_INFO_ACK, KEGS_GET_EVENT_INFO_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_EVENT_INFO_ACK, kPacket_ );
}
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
IMPL_ON_FUNC( ESG_REWARD_DB_DATA_NOT )
{

	SendPacket( ESG_REWARD_DB_DATA_NOT, kPacket_ );

}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_NEW_YEAR_EVENT_2014
IMPL_ON_FUNC( EGS_2013_EVENT_MISSION_COMPLETE_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_2013_EVENT_MISSION_COMPLETE_ACK );

	IF_EVENT_ENABLED( CEI_OLD_YEAR_EVENT_2013 )
	{
	}
	ELSE
	{
		START_LOG( cwarn, L"�̺�Ʈ�� �����µ� �Ϸ� ��û? ��Ŀ! �Ǵ� �ణ �ʰ� ��û�� �ֵ�" )
			<< END_LOG;

		KEGS_2013_EVENT_MISSION_COMPLETE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_QUEST_09;
		SendPacket( EGS_2013_EVENT_MISSION_COMPLETE_ACK, kPacket );
		return;
	}

	KEGS_2013_EVENT_MISSION_COMPLETE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_QUEST_09;

	if( true == kPacket_.m_bLevelUpEvent )
	{
		if( GetLevel() > m_ucOldYearMissionRewardedLevel &&
			GetLevel() >= 26 &&
			CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == false )
		{
			if( GetLevel() == SiKGameSysVal()->GetLimitsLevel() )
			{
				if( m_ucOldYearMissionRewardedLevel == GetLevel() - 1 )
				{
					bool bClassChange = false;
					if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == true )
						bClassChange = true;

					IF_EVENT_ENABLED( CEI_ALLOW_EVE_FIRST_CLASS_OLD_YEAR_EVENT )
					{
						if( static_cast< CXSLUnit::UNIT_TYPE >( GetUnitType() ) == CXSLUnit::UT_EVE )
							bClassChange = true;
					}
					IF_EVENT_ENABLED( CEI_ALLOW_ARA_FIRST_CLASS_OLD_YEAR_EVENT )
					{
						if( static_cast< CXSLUnit::UNIT_TYPE >( GetUnitType() ) == CXSLUnit::UT_ARA )
							bClassChange = true;
					}
#ifdef SERV_NEW_CHARACTER_EL
					IF_EVENT_ENABLED( CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT )
					{
						if( static_cast< CXSLUnit::UNIT_TYPE >( GetUnitType() ) == CXSLUnit::UT_ELESIS )
							bClassChange = true;
					}
#endif SERV_NEW_CHARACTER_EL

					if( bClassChange )
					{
						// �����̸�, ����Ʈ ������ ���� ������ �� ���� ������ �ش�.
						KDBE_2013_EVENT_MISSION_COMPLETE_REQ kPacketToDB;
						kPacketToDB.m_iUnitUID	   = GetCharUID();
						kPacketToDB.m_ucLevel	   = GetLevel();
						SendToGameDB( DBE_2013_EVENT_MISSION_COMPLETE_REQ, kPacketToDB );
						return;
					}
				}
				else
				{
					// �����̸�, ����Ʈ ������ �� �� ���� ������ �� ����Ʈ ������ �ѹ� �� ��� �Ѵ�.
					KDBE_2013_EVENT_MISSION_COMPLETE_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID	   = GetCharUID();
					kPacketToDB.m_ucLevel	   = GetLevel() - 1;
					SendToGameDB( DBE_2013_EVENT_MISSION_COMPLETE_REQ, kPacketToDB );
					return;
				}
			}
			else
			{
				KDBE_2013_EVENT_MISSION_COMPLETE_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID	   = GetCharUID();
				kPacketToDB.m_ucLevel	   = GetLevel();
				SendToGameDB( DBE_2013_EVENT_MISSION_COMPLETE_REQ, kPacketToDB );
				return;
			}
		}

		START_LOG( cwarn, L"�Ϸ� ���ǿ� �ش��� �� �Ǵµ� �Ϸ� ��û? ��Ŀ!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetLevel() )
			<< BUILD_LOG( (int)m_ucOldYearMissionRewardedLevel )
			<< BUILD_LOG( (int)GetUnitClass() )
			<< BUILD_LOG( SiKGameSysVal()->GetLimitsLevel() )
			<< END_LOG;

		// ���ǿ� �� �´� ��ŷ���� ���
		kPacket.m_iOK = NetError::ERR_QUEST_09;
	}
	else if( m_ucOldYearMissionRewardedLevel <= SiKGameSysVal()->GetLimitsLevel() )
	{
		// ��Ŷ ������.
		KDBE_2013_EVENT_MISSION_COMPLETE_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID	   = GetCharUID();
		kPacketToDB.m_ucLevel	   = 99;
		SendToGameDB( DBE_2013_EVENT_MISSION_COMPLETE_REQ, kPacketToDB );
		return;
	}

	SendPacket( EGS_2013_EVENT_MISSION_COMPLETE_ACK, kPacket );
}
IMPL_ON_FUNC( DBE_2013_EVENT_MISSION_COMPLETE_ACK )
{
	KEGS_2013_EVENT_MISSION_COMPLETE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ������� ���� ����(����)
		m_ucOldYearMissionRewardedLevel = kPacket_.m_ucLevel;

		if( kPacket_.m_ucLevel > SiKGameSysVal()->GetLimitsLevel() )
		{
			std::map< int, int > mapItem;
			mapItem.insert( std::make_pair( _CONST_SERV_NEW_YEAR_EVENT_2014_::iMaxLevelSpecialMissionItemID, 1 ) );
			if( true == m_kInventory.DeleteItemOnlyInventory( mapItem, kPacket.m_vecInventorySlotInfo, KDeletedItemInfo::DR_QUEST_COMPLETE ) )
			{
				// ���� ������ �����Ͽ����� ������ ����
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = GetCharUID();
				kPacketToDB.m_iToUnitUID   = GetCharUID();
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
				kPacketToDB.m_iRewardID	   = 2004;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

				kPacket.m_iOK = NetError::NET_OK;
			}
			else
			{
				kPacket.m_iOK = NetError::ERR_QUEST_09;
			}
		}
		else
		{
			// DB�� �� ����Ͽ����� ������ ����
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			if( kPacket_.m_ucLevel >= SiKGameSysVal()->GetLimitsLevel() )
				kPacketToDB.m_iRewardID = 2003;
			else
				kPacketToDB.m_iRewardID = 2002;

			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}

	kPacket.m_iRewardedLevel = static_cast< int >( m_ucOldYearMissionRewardedLevel );
	SendPacket( EGS_2013_EVENT_MISSION_COMPLETE_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_2014_EVENT_MISSION_COMPLETE_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_2014_EVENT_MISSION_COMPLETE_ACK );

	IF_EVENT_ENABLED( CEI_NEW_YEAR_EVENT_2014 )
	{
	}
	ELSE
	{
		KEGS_2014_EVENT_MISSION_COMPLETE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_QUEST_09;
		SendPacket( EGS_2014_EVENT_MISSION_COMPLETE_ACK, kPacket );
		return;
	}

	bool bCompleted = false;
	switch( m_iNewYearMissionStepID )
	{
	case 0:
	case 2:
	case 4:
	case 6:
	case 8:
		bCompleted = true;
		break;
	case 1:
		{
			// �ܰ�0 : 1�� ���� �޼��ϸ� �Ϸ�
			if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == true ||
				CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == true )
			{
				bCompleted = true;
			}
		} break;
	case 3:
		{
			// �ܰ�2 : 20���� �޼��ϸ� �Ϸ�
			if( GetLevel() >= 20 )
			{
				 bCompleted = true;
			}
		} break;
	case 5:
		{
			// �ܰ�4 : 30���� �޼��ϸ� �Ϸ�
			if( GetLevel() >= 30 )
			{
				bCompleted = true;
			}
		} break;
	case 7:
		{
#ifdef SERV_NEW_CHARACTER_EL
			IF_EVENT_ENABLED( CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT )
			{
				if( GetLevel() >= 35 && static_cast< CXSLUnit::UNIT_TYPE >( GetUnitType() ) == CXSLUnit::UT_ELESIS )
					bCompleted = true;
			}
#endif SERV_NEW_CHARACTER_EL

			// �ܰ�6 : 2�� ���� �޼��ϸ� �Ϸ�
			if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() ) ) == true )
			{
				bCompleted = true;
			}
		} break;
	case 9:
		{
			// �ܰ�8 : 2�� ���� �޼��ϸ� �Ϸ�
			if( GetLevel() >= 45 )
			{
				bCompleted = true;
			}
		} break;
	default:
		break;
	}

	if( bCompleted == true )
	{
		KDBE_2014_EVENT_MISSION_COMPLETE_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_iUserUID = GetUID();
		kPacketToDB.m_iNewYearMissionStepID = m_iNewYearMissionStepID + 1;
		SendToGameDB( DBE_2014_EVENT_MISSION_COMPLETE_REQ, kPacketToDB );
	}
	else
	{
		START_LOG( cwarn, L"�Ϸ� ���ǿ� �ش��� �� �Ǵµ� �Ϸ� ��û? ��Ŀ!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( (int)GetUnitClass() )
			<< BUILD_LOG( GetLevel() )
			<< END_LOG;

		KEGS_2014_EVENT_MISSION_COMPLETE_ACK kAck;
		kAck.m_iOK = NetError::ERR_QUEST_09;
		SendPacket( EGS_2014_EVENT_MISSION_COMPLETE_ACK, kAck );
	}	
}

_IMPL_ON_FUNC( DBE_2014_EVENT_MISSION_COMPLETE_ACK, KEGS_2014_EVENT_MISSION_COMPLETE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK)
	{
		// ���������� �ܰ踦 ������Ű��
		++m_iNewYearMissionStepID;

		int iRewardID = 0;

		// ������ �ݽô�.
		switch( m_iNewYearMissionStepID )
		{
		case 1:
			{
				iRewardID = 2005;
			} break;
		case 2:
			{
				iRewardID = 2006;
			} break;
		case 3:
			{
				iRewardID = 2007;
			} break;
		case 4:
			{
				iRewardID = 2008;
			} break;
		case 5:
			{
				iRewardID = 2009;
			} break;
		case 6:
			{
				iRewardID = 2010;
			} break;
		case 7:
			{
				iRewardID = 2011;
			} break;
		case 8:
			{
				iRewardID = 2012;
			} break;
		case 9:
			{
				iRewardID = 2013;
			} break;
		case 10:
			{
				iRewardID = 2014;
			} break;
		default :
			break;
		}

		// �̺�Ʈ ������ ����!
		if( iRewardID > 0 )
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = iRewardID;			
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}

	SendPacket( EGS_2014_EVENT_MISSION_COMPLETE_ACK, kPacket_ );
}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_ELESIS_UPDATE_EVENT
IMPL_ON_FUNC_NOPARAM( EGS_EVENT_NOTE_VIEW_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EVENT_NOTE_VIEW_REQ, EGS_EVENT_NOTE_VIEW_ACK );

	if( m_iNoteViewCount > 0 )
	{
		m_iNoteViewCount--;

		int iTitleID = 5730 + ((rand()%10) * 10);
		bool bExistTitle = m_kUserTitleManager.IsExistTitle( iTitleID );

		if( bExistTitle )
		{
			// Īȣ �Ⱓ�� ��������!
			KDBE_INSERT_TITLE_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_iTitleID = iTitleID;
			kPacketToDB.m_sPeriod  = 1;
			kPacketToDB.m_bGameServerEvent = false;
			kPacketToDB.m_bExpandPeriod = true;
			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
		}
		else
		{
			// �Ȱ����� ������ Ÿ��Ʋ ����
			KDBE_INSERT_TITLE_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_iTitleID = iTitleID;
			kPacketToDB.m_sPeriod  = 1;
			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
		}

		KEGS_EVENT_NOTE_VIEW_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iTitleID = iTitleID;
		kPacketAck.m_iNoteViewCount = m_iNoteViewCount;
		SendPacket( EGS_EVENT_NOTE_VIEW_ACK, kPacketAck );

		START_LOG( cerr, L"�̺�Ʈ Īȣ ȹ��" )
			<< BUILD_LOG( m_iNoteViewCount )
			<< BUILD_LOG( iTitleID )
			<< BUILD_LOG( bExistTitle )
			<< END_LOG;
	}
	else
	{
		KEGS_EVENT_NOTE_VIEW_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_EVENT_NOTE_VIEW_ACK, kPacketAck );

		START_LOG( cerr, L"Ŭ���̾�Ʈ���� �� �� Ȯ�������ٵ�..." )
			<< BUILD_LOG( m_iNoteViewCount )
			<< END_LOG;
	}
}
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
IMPL_ON_FUNC( EGS_UNLIMITED_SECOND_CHANGE_JOB_NOT )
{
	KDBE_BUY_UNIT_CLASS_CHANGE_REQ kPacketToDB;
	
	if( GetUnlimitedClassChangeInfo( kPacket_.m_iUnlimitedSecondChangeJob, kPacketToDB ) == true )
	{
		SendToGameDB( DBE_BUY_UNIT_CLASS_CHANGE_REQ, kPacketToDB );
	}
	else
	{
		START_LOG( cerr, L"2�� ������ ������ ���� �ߴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnlimitedSecondChangeJob )
			<< END_LOG;
	}
}
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_EVENT_CHECK_POWER
IMPL_ON_FUNC( EGS_START_CHECK_POWER_REQ )
{
	START_LOG( cwarn, L"��Ŷ ������ �˻�")
		<< BUILD_LOG( kPacket_.m_bStart )
		<< END_LOG;

	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_START_CHECK_POWER_ACK );

	IF_EVENT_ENABLED( CEI_CHECK_POWER )
	{
	}
	ELSE
	{
		KEGS_START_CHECK_POWER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_QUEST_01;

		SendPacket( EGS_START_CHECK_POWER_ACK, kPacket );
		return;
	}

	KDBE_START_CHECK_POWER_REQ kPacketToDB;
	if( kPacket_.m_bStart == true )
	{
		unsigned char ucMaxCount = 2;

		if( static_cast< CXSLUnit::UNIT_TYPE >( GetUnitType() ) == CXSLUnit::UT_ELESIS )
			ucMaxCount = 3;

		if( m_ucCheckPowerCount >= ucMaxCount )
		{
			KEGS_START_CHECK_POWER_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_QUEST_01;

			SendPacket( EGS_START_CHECK_POWER_ACK, kPacket );
			return;
		}

		// ��¥�� �����ٸ� ī��Ʈ �ϱ� �� �ʱ�ȭ�� �� �ش�.
		CTime tCurrentTime = CTime::GetCurrentTime();
		if( tCurrentTime.GetDay() != CTime( m_iCheckPowerTime ).GetDay() )
		{
			m_ucCheckPowerCount = 0;
		}

		m_iCheckPowerTime = tCurrentTime.GetTime();
		++m_ucCheckPowerCount;
		m_ucCheckPowerScore = 0;
	}
	// ��¥ ���� ����ϴ� �������� �˻�
	else if( m_iCheckPowerTime < CTime( 2013, 12, 1, 0, 0, 0 ).GetTime() )
	{
		m_iCheckPowerTime = CTime( 2013, 12, 17, 0, 0, 0 ).GetTime();
		m_ucCheckPowerScore = 255;
	}
	else
	{
		KEGS_START_CHECK_POWER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_QUEST_01;

		SendPacket( EGS_START_CHECK_POWER_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iCheckPowerTime = m_iCheckPowerTime;
	kPacketToDB.m_ucCheckPowerCount = m_ucCheckPowerCount;
	kPacketToDB.m_ucCheckPowerScore = m_ucCheckPowerScore;

	SendToGameDB( DBE_START_CHECK_POWER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_START_CHECK_POWER_ACK )
{
	KEGS_START_CHECK_POWER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_START_CHECK_POWER_ACK, kPacket );

	START_LOG( cwarn, L"��Ŷ ������ �˻�")
		<< BUILD_LOG( kPacket_.m_iOK )
		<< END_LOG;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_bCheckPowerShowPopUp		= false;

		KEGS_UPDATE_CHECK_POWER_NOT kNot;
		kNot.m_ucCheckPowerCount	= m_ucCheckPowerCount;
		kNot.m_iCheckPowerTime		= m_iCheckPowerTime;
		kNot.m_ucCheckPowerScore	= m_ucCheckPowerScore;

		SendPacket( EGS_UPDATE_CHECK_POWER_NOT, kNot );
	}
}

IMPL_ON_FUNC( DBE_UPDATE_CHECK_POWER_ACK )
{
	START_LOG( cwarn, L"��Ŷ ������ �˻�")
		<< BUILD_LOG( kPacket_.m_iOK )
		<< END_LOG;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_bCheckPowerShowPopUp		= false;

		KEGS_UPDATE_CHECK_POWER_NOT kNot;
		kNot.m_ucCheckPowerCount	= m_ucCheckPowerCount;
		kNot.m_iCheckPowerTime		= m_iCheckPowerTime;
		kNot.m_ucCheckPowerScore	= m_ucCheckPowerScore;

		SendPacket( EGS_UPDATE_CHECK_POWER_NOT, kNot );
	}
}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
IMPL_ON_FUNC( EGS_KOM_FILE_CHECK_LOG_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_KOM_FILE_CHECK_LOG_REQ, EGS_KOM_FILE_CHECK_LOG_ACK );

	KELOG_KOM_FILE_CHECK_LOG_NOT kPacket;
	kPacket.m_iUserUID = GetUID();
	kPacket.m_wstrInvaildKomName = kPacket_.m_wstrInvalidKomName;
	SendToLogDB( ELOG_KOM_FILE_CHECK_LOG_NOT, kPacket );

	KEGS_KOM_FILE_CHECK_LOG_ACK kPacket2;
	kPacket2.m_wstrInvalidKomName = kPacket_.m_wstrInvalidKomName;
	kPacket2.m_iOK = NetError::NET_OK;
	SendPacket( EGS_KOM_FILE_CHECK_LOG_ACK, kPacket2 );
}
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_SKILL_PAGE_SYSTEM	// �۾���¥: 2013-11-14	// ����ö

IMPL_ON_FUNC_NOPARAM( EGS_GET_NEXT_SKILL_PAGE_ED_REQ )
{
	// ���� üũ
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_NEXT_BANK_ED_ACK );
	KEGS_GET_NEXT_BANK_ED_ACK kPacket;

	const UCHAR ucTheNumberOfSkillPagesAvailable
		= m_kSkillTree.GetTheNumberOfSkillPagesAvailable();

	int iNextED = 0;

	switch ( ucTheNumberOfSkillPagesAvailable )
	{
	case 1:	/// ���� ��� ������ ������ ���� �Ѱ��� 2���� Ȯ�� �Ϸ��� ���
	case 2:	/// ���� ��� ������ ������ ���� �ΰ��� 3���� Ȯ�� �Ϸ��� ���
		iNextED = KUserSkillTree::ED_NEEDED_TO_EXPAND_SECOND_SKILL_PAGE;
		break;

	default:
		{
			SET_ERROR( ERR_SKILL_PAGE_02 );
			goto end_proc;
		} break;
	}

	kPacket.m_iED = iNextED;
	SET_ERROR( NET_OK );

end_proc:

	kPacket.m_iOK = NetError::GetLastError();
	SendPacket( EGS_GET_NEXT_SKILL_PAGE_ED_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_EXPAND_SKILL_PAGE_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_EXPAND_INVENTORY_ED_ACK );
	
	KDBE_EXPAND_SKILL_PAGE_REQ kPacket;

	// ��ų �������� Ȯ���� ������ ������ �ִ� Ȯ��
	if ( !m_kSkillTree.CanExpandSkillPage() )
	{
		SET_ERROR( ERR_SKILL_PAGE_02 );
		goto end_proc;
	}

	// Ȯ���� �� �ִ� ED�� �����ϰ� �ִ��� üũ
	if ( m_iED < KUserSkillTree::ED_NEEDED_TO_EXPAND_SECOND_SKILL_PAGE || m_iED < 0 )
	{
		// �ϴ� �κ��丮 ���� ��� ����
		SET_ERROR( ERR_EXPAND_INVENTORY_ED_01 );
		goto end_proc;
	}
	
	// �⺻ ��ų ��ȸ
	if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), 
		kPacket.m_iDefaultSkill[0], kPacket.m_iDefaultSkill[1], 
		kPacket.m_iDefaultSkill[2], kPacket.m_iDefaultSkill[3], 
		kPacket.m_iDefaultSkill[4], kPacket.m_iDefaultSkill[5] ) )
	{
		START_LOG( cerr, L"����Ÿ���� �̻���." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		SET_ERROR( ERR_SKILL_12 );
		goto end_proc;
	}

	kPacket.m_usEventID = EGS_EXPAND_SKILL_PAGE_REQ;
	kPacket.m_iUnitUID	= GetCharUID();
	/// �߰��Ǿ�� �� ������ �ѹ�
	kPacket.m_vecSkillPageNumberToBeAdded.push_back( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() + 1 );
	kPacket.m_cUnitClass = GetUnitClass();

	// ��ų ����Ʈ ������
	SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), kPacket.m_iSPoint );
	// ĳ�� ��ų ����Ʈ ������
	kPacket.m_iCSPoint	=	m_kSkillTree.GetMaxCSPoint();

	// ED ����
	m_iED -= KUserSkillTree::ED_NEEDED_TO_EXPAND_SECOND_SKILL_PAGE;

	// DB�� ��ų������ Ȯ�� ��û
	SendToGameDB( DBE_EXPAND_SKILL_PAGE_REQ, kPacket );	

end_proc:
	if ( NetError::GetLastError() != NetError::NET_OK ) 
	{
		KEGS_EXPAND_SKILL_PAGE_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_EXPAND_SKILL_PAGE_ACK, kAck );

		START_LOG( cerr, L"��ų ������ Ȯ�� ���� , User ID :" << GetUserID() )
			// ���� �̿� ������ ��ų ������ ��
			// ���� ���� ED ��
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_EXPAND_SKILL_PAGE_ACK )
{
	 VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP ), EGS_EXPAND_SKILL_PAGE_ACK );

	 KEGS_EXPAND_SKILL_PAGE_ACK kPacket;
	 KELOG_EXPAND_SKILL_PAGE_NOT kLog;

	 if ( kPacket_.m_iOK != NetError::NET_OK )
	 {
		 START_LOG( cerr, L"��ų ������ Ȯ�� ����(DBE_EXPAND_SKILL_PAGE_ACK) , User ID :" << GetUserID() )
			 << BUILD_LOG( GetUID() )
			 << BUILD_LOG( GetCharName() )
			 << BUILD_LOG( GetCharUID() )
			 << BUILD_LOG( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() )
			 << BUILD_LOG( kPacket_.m_iSkillPageNumberToBeAdded )
			 << END_LOG;
		 goto end_proc;
	 }

	 if ( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() >= kPacket_.m_iSkillPageNumberToBeAdded )
	 {
		 /// kimhc // ����ö // �̹� DB ó������ �Ǿ� ���� ���̹Ƿ� �α׸� �������.
		START_LOG( cerr, L"�̹� Ȯ���� �Ǿ� �ִٴµ�?(DBE_EXPAND_SKILL_PAGE_ACK) , User ID :" << GetUserID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( "m_kSkillTree.GetTheNumberOfSkillPagesAvailable() >= kPacket_.m_iSkillPageNumberToBeAdded" )
			<< BUILD_LOG( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() )
			<< BUILD_LOG( kPacket_.m_iSkillPageNumberToBeAdded )
			<< END_LOG;
	 }

	 const int iHowManyTimesToBeExpanded
		 = kPacket_.m_iSkillPageNumberToBeAdded - m_kSkillTree.GetTheNumberOfSkillPagesAvailable();

	 for ( int i = 0; i < iHowManyTimesToBeExpanded; i++ )
		 m_kSkillTree.ExpandSkillPage( kPacket_.m_iDefaultSkill, kPacket_.m_iSPoint, kPacket_.m_iCSPoint );

	 kPacket.m_iCSPointAvailable				= kPacket_.m_iCSPoint;
	 kPacket.m_iSPointAvailable					= kPacket_.m_iSPoint;
	 kPacket.m_iTheNumberOfSkillPagesAvailable	= m_kSkillTree.GetTheNumberOfSkillPagesAvailable();

	 m_kSkillTree.GetUnSealedSkillList( kPacket.m_vecUnsealedSkillID );
	 
	 for ( int i = 0; i < THE_NUMBER_OF_DEFAULT_SKILLS; i++ )
		 kPacket.m_vecSkillListLearned.push_back( kPacket_.m_iDefaultSkill[i] );

	 kLog.m_iUnitUID				= GetCharUID();
	 kLog.m_wstrSenderNickName		= GetCharName();
	 kLog.m_ucLevel					= GetLevel();
	 kLog.m_ucUnitClass				= GetUnitClass();
	 kLog.m_ucBeforeTheNumberOfSkillPagesAvailable	= m_kSkillTree.GetTheNumberOfSkillPagesAvailable() - iHowManyTimesToBeExpanded;

	 if ( kPacket_.m_usEventID == EGS_EXPAND_SKILL_PAGE_REQ )
	 {		 
		 kLog.m_ucPaymentType = 0;	// ED ����
		 // DB��� ED
		 KStatisticsKey kKey;
		 kKey.m_vecIntKey.push_back( 0 );
		 KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_ExpandSkillPage, ( KUserSkillTree::ED_NEEDED_TO_EXPAND_SECOND_SKILL_PAGE ) );
	 }
	 else
	 {
		 kPacket.m_bPayWithED = false;
		 kLog.m_ucPaymentType = 1;	// ĳ�� ����	 
	 }

	 SendToLogDB( ELOG_EXPAND_SKILL_PAGE_NOT, kLog );	 		 

end_proc:
	 // ���� �� ���
	 if ( kPacket_.m_iOK != NetError::NET_OK )
	 {
		 // ED�� �������� ���� ED ���� ���
		 if ( kPacket_.m_usEventID == EGS_EXPAND_SKILL_PAGE_REQ )
			 m_iED += KUserSkillTree::ED_NEEDED_TO_EXPAND_SECOND_SKILL_PAGE;
	 }

	 kPacket.m_iOK = kPacket_.m_iOK;
	 kPacket.m_iED = m_iED;	 
	 
	 SendPacket( EGS_EXPAND_SKILL_PAGE_ACK, kPacket );	 
}

IMPL_ON_FUNC( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ )
{
	// ����, ���Ϳ����� ����
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK );
	
	
	if ( kPacket_.m_iSkillPagesNumberDecidedToUse < 1 ||
		kPacket_.m_iSkillPagesNumberDecidedToUse > m_kSkillTree.GetTheNumberOfSkillPagesAvailable() )
	{
		SET_ERROR( ERR_SKILL_PAGE_03 );
		goto end_proc;
	}

	KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ kReq( GetCharUID(), kPacket_.m_iSkillPagesNumberDecidedToUse );
	SendToGameDB( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ, kReq );
	return;

end_proc:
	if ( NetError::GetLastError() != NetError::NET_OK ) 
	{
		KEGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK, kAck );

		START_LOG( cerr, L"EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ) , User ID :" << GetUserID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() )
			<< BUILD_LOG( kPacket_.m_iSkillPagesNumberDecidedToUse )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK )
{
	// ����, ���Ϳ����� ����
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK );

	KEGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK kAck;

	if ( kPacket_.m_iSkillPagesNumberToBeActive < 1 ||
		kPacket_.m_iSkillPagesNumberToBeActive > m_kSkillTree.GetTheNumberOfSkillPagesAvailable() )
	{
		SET_ERROR( ERR_SKILL_PAGE_03 );
		goto end_proc;
	}
	
	m_kSkillTree.SetActiveSkillPageNumber( kPacket_.m_iSkillPagesNumberToBeActive );
	kAck.m_iOK								= NetError::NET_OK;	
	kAck.m_iCSPointAvailable				= m_kSkillTree.GetCSPoint();
	kAck.m_iSPointAvailable					= m_kSkillTree.GetSPoint();
	kAck.m_iSkillPagesNumberDecidedToUse	= kPacket_.m_iSkillPagesNumberToBeActive;

	m_kSkillTree.GetKUserSkillPageData( kAck.m_kUserSkillPageData, kPacket_.m_iSkillPagesNumberToBeActive );
	SendPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK, kAck );
	return;

end_proc:
	if ( NetError::GetLastError() != NetError::NET_OK ) 
	{
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK, kAck );

		START_LOG( cerr, L"EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ) , User ID :" << GetUserID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() )
			<< BUILD_LOG( kPacket_.m_iSkillPagesNumberToBeActive )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;
	}
}

#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ENTRY_POINT
IMPL_ON_FUNC_NOPARAM( EGS_CHARACTER_LIST_REQ )
{
    VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT ), EGS_CHARACTER_LIST_REQ, EGS_CHARACTER_LIST_ACK );
    SendToGameDB( DBE_CHARACTER_LIST_REQ, GetName() );

#ifdef SERV_SECOND_SECURITY
    KDBE_GET_SECOND_SECURITY_INFO_REQ kReq;
    kReq.m_iUserUID = GetUID();
    SiKGameSysVal()->GetComeBackRewardCondition( kReq.m_mapComeBackRewardCondition );

    //{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
    kReq.m_wstrID = GetName();
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
    //}}

    SendToAccountDB( DBE_GET_SECOND_SECURITY_INFO_REQ, kReq );	// AccountDB�� �ֱ� ���� ���� ���� ���

#endif SERV_SECOND_SECURITY

}

IMPL_ON_FUNC( EGS_CHARACTER_LIST_1ST_ACK )
{
    switch( GetKGSSimLayer()->GetServerGroupNum() )
    {
    case KGSSimLayer::SINGLE_SERVER_GROUP: 
        // EGS_CHARACTER_LIST_ACK ó���� �����ϰ� �ϰ�..
        ON_EGS_CHARACTER_LIST_ACK( anTrace_, kPacket_ );
        break;
    case KGSSimLayer::DOUBLE_SERVER_GROUP: // �ѱ�, �߱�
        SendToGameDB2nd( DBE_CHARACTER_LIST_2ND_REQ, kPacket_ );
        break;
    default:
        break;
    }
}

IMPL_ON_FUNC( EGS_CHARACTER_LIST_ACK )
{
    VERIFY_STATE( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_CHECK_SECOND_PW ) );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        m_nUnitSlot = kPacket_.m_mapServerGroupUnitSlot[KBaseServer::GetKObj()->GetServerGroupID()];

        std::map< int, std::vector<KUnitInfo> >::iterator mit;
        for ( mit = kPacket_.m_mapServerGroupUnitInfo.begin(); mit != kPacket_.m_mapServerGroupUnitInfo.end() ; ++mit )
        {
            std::vector<KUnitInfo>::iterator vit;
            for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
            {
                vit->m_iOwnerUserUID		= GetUID();
                vit->m_nStraightVictories	= 0;
                vit->m_wstrIP				= GetP2PIP();
                vit->m_usPort				= GetP2PPort();

                const CXSLUnit::UnitTemplet* pUnitTemplet = SiCXSLUnitManager()->GetUnitTemplet( static_cast<CXSLUnit::UNIT_CLASS>(vit->m_cUnitClass) );
                if( pUnitTemplet == NULL )
                {
                    START_LOG_WITH_NAME( cerr )
                        << BUILD_LOGc( vit->m_cUnitClass )
                        << END_LOG;

#if defined( _IN_HOUSE_ )
                    continue;
#endif 

                    kPacket_.m_iOK = NetError::ERR_CREATE_UNIT_03;
                    goto end_proc;

                }

                vit->m_ucLevel				= SiKExpTable()->CheckLevelUp( 1, vit->m_iEXP );
                vit->m_nNowBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)(vit->m_ucLevel) );
                vit->m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)(vit->m_ucLevel + 1) );

                KStatTable::KUnitStatInfo sUnitStat;
                SiKStatTable()->GetUnitStat( vit->m_cUnitClass, vit->m_ucLevel, sUnitStat );
                vit->m_kStat.m_iBaseHP		+= (int)sUnitStat.m_uiHP;
                vit->m_kStat.m_iAtkPhysic	+= (int)sUnitStat.m_usAtkPhysic;
                vit->m_kStat.m_iAtkMagic	+= (int)sUnitStat.m_usAtkMagic;
                vit->m_kStat.m_iDefPhysic	+= (int)sUnitStat.m_usDefPhysic;
                vit->m_kStat.m_iDefMagic	+= (int)sUnitStat.m_usDefMagic;

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-07	// �ڼ���
                // ���� �ʵ��� ������ �̵� ��Ű��
                if( SiCXSLBattleFieldManager()->IsBossFieldID( vit->m_kLastPos.m_iMapID ) == true )
                {
                    vit->m_kLastPos.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( vit->m_ucLevel, vit->m_mapDungeonClear );
                    vit->m_kLastPos.m_bIgnoreLastTouch = true;
                }
#endif // SERV_BATTLE_FIELD_BOSS

                // �����ϰ� �ִ� ĳ���� ����Ʈ ����
                m_setMyUnitUIDList.insert( vit->m_nUnitUID );

                GetPvpRankForClient( vit->m_iOfficialMatchCnt, vit->m_cRank );
            }

            START_LOG( clog, L"EGS_CHARACTER_LIST_ACK �� ������� p2p data" )
                << BUILD_LOG( GetP2PIP() )
                << BUILD_LOG( GetP2PPort() );

            if( ( int )mit->second.size() > kPacket_.m_mapServerGroupUnitSlot[mit->first] )
            {
                START_LOG( cerr, L"ĳ���� ���� ������ �ʰ��ϴ� ���� ĳ���͸� �����ϰ� ����." )
                    << BUILD_LOG( ( int )mit->second.size() )
                    << BUILD_LOG( kPacket_.m_mapServerGroupUnitSlot[mit->first] )
                    << BUILD_LOG( GetUID() )
                    << BUILD_LOG( GetName() );
            }

        }
        // udp ���� ���� p2p ip/port�� ä�������ش�.(�ӽð� �ɼ����� ���Ŀ� �Ȱܾ� �Ҽ����ִ�.!�Ӹ������� )		
    }
end_proc:

    START_LOG( clog, L"ĳ���� ����Ʈ ���(����) : " )
        << BUILD_LOG( kPacket_.m_mapServerGroupUnitSlot[0] )
        << BUILD_LOG( kPacket_.m_mapServerGroupUnitSlot[1] )
        << END_LOG;

    SendPacket( EGS_CHARACTER_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_CREATE_NEW_UNIT_REQ )
{
    VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CREATE_NEW_UNIT_REQ, EGS_CREATE_UNIT_ACK );

    const CXSLUnit::UnitTemplet* pUnitTemplet = SiCXSLUnitManager()->GetUnitTemplet( (CXSLUnit::UNIT_CLASS)kPacket_.m_iClass );
    KEGS_CREATE_UNIT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_UNKNOWN;
    char charBuf[255] = {0};
    std::string strNickName;

    KDBE_GAME_CREATE_UNIT_REQ kDBPacket;
    KAccountInfo AccountTemp;

    if( !pUnitTemplet ) // 070604. florist. templet�� ���� �ʿ�� ����. iClass�� ��ȿ�� ���� ������ �ǹ̸� ������.
    {
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
        goto end_proc;
    }

    if( CXSLUnit::IsInitNormalJob( (CXSLUnit::UNIT_CLASS)kPacket_.m_iClass ) == false )
    {
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_11;
        goto end_proc;
    }

    WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrNickName.c_str(), -1, charBuf, 255, NULL, NULL );
    strNickName = charBuf;
    
    if( strNickName.empty() || strNickName.size() > 12 )   //ĳ���� �г��� ���� �˻�.
    {
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_04;
        goto end_proc;
    }

    if ( GetAuthLevel() < SEnum::UAL_GM && SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
    {
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_08;
        goto end_proc;
    }

    if( SiKPvpMatchManager()->IsExistPvpNpcName( kPacket_.m_wstrNickName ) == true )
    {		
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_01;
        goto end_proc;
    }

    // ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
    if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
    {
        kPacket.m_iOK = NetError::ERR_ODBC_00;
        goto end_proc;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << BUILD_LOG( kPacket_.m_iClass );

    GetAccountInfo( AccountTemp );
    kDBPacket.m_wstrRegDate = AccountTemp.m_wstrRegDate;
    kDBPacket.m_wstrNickName = kPacket_.m_wstrNickName;
    kDBPacket.m_iClass = kPacket_.m_iClass;

    // 1st �ַ���, 2nd ���̾ư� �ƴ϶� 1st �� ���� ���� ���, 2nd �� �ٸ� ����� ���� ����
    switch ( KBaseServer::GetKObj()->GetServerGroupID() )
    { // �ؿ��� ������1�� ���������� ����=�ַ���, kPacket_.m_iServerGroup=�ַ��� �� ������ ��
        case SEnum::SGI_SOLES:
            if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
            {   // ���� ��� �Լ��� ť���Ѵٴ� �Ϳ� ����.
                SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kDBPacket ); 
            }
            else if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA 
                && GetKGSSimLayer()->GetServerGroupNum() == KGSSimLayer::DOUBLE_SERVER_GROUP )
            {
                SendToGameDB2nd( DBE_GAME_CREATE_UNIT_REQ, kDBPacket );
            }
            break;
        case SEnum::SGI_GAIA:
            if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA )
            {
                SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kDBPacket );
            }
            else if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
            {
                SendToGameDB2nd( DBE_GAME_CREATE_UNIT_REQ, kDBPacket );
            }
            
            break;
        default:
            if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )
            {
                SendToGameDB( DBE_GAME_CREATE_UNIT_REQ, kDBPacket ); 
            }
    }
    kPacket.m_iOK = NetError::NET_OK;

end_proc:
    LOG_CONDITION( kPacket.m_iOK == NetError::ERR_CREATE_UNIT_03, cerr, cwarn )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_iOK )
        << BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
        << BUILD_LOG( kPacket_.m_iClass)
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << END_LOG;

    if ( kPacket.m_iOK != NetError::NET_OK ) 
    {
        SendPacket( EGS_CREATE_UNIT_ACK, kPacket );    
    }    
}

IMPL_ON_FUNC( EGS_ENTRY_POINT_DELETE_UNIT_REQ )
{
    VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT ), EGS_DELETE_UNIT_REQ, EGS_DELETE_UNIT_ACK );

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY	// �ؿ��� �߰�
    if( IsUseSecurity() == true )
    {
        if( GetSuccessSecondSecurity() == false )	// 2�� ������ ��� ���ε� ������ ���� �ʾҴٸ� ����
        {
            START_LOG( cerr, L"2�� ������ ��� ���ε� ������ �Ǳ� ���� ���� ������ �Ϸ� �Ѵ�." )
                << BUILD_LOG( GetUID() )
                << END_LOG;

            KEGS_DELETE_UNIT_ACK kPacket;
            kPacket.m_iOK = NetError::ERR_VERIFY_21;
            kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
            kPacket.m_iGuildUID = 0;	
            SendPacket( EGS_DELETE_UNIT_ACK, kPacket );
            return;
        }
    }
#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM	// �ؿ��� �߰�
    if( m_kUserRelationshipManager.GetRelationshipType() != SEnum::RT_SOLO )
    {
        KEGS_DELETE_UNIT_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_DELETE_UNIT_06;
        SendPacket( EGS_DELETE_UNIT_ACK, kPacket );
        
        START_LOG( cerr, L"���Ա������� ĳ���� ���� ����, UID : " << GetUID() )
            << BUILD_LOG( kPacket.m_iOK )
            //<< BUILD_LOG( NetError::GetLastErrMsg() )
            << END_LOG;
        
        return;
    }
#endif SERV_RELATIONSHIP_SYSTEM
	//}

    KEGS_DELETE_UNIT_REQ kDBPacket;
    kDBPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	//{{ 2012. 02. 21	��μ�	ĳ���� ���� �� ��� Ż�� ����ó�� ����
#ifdef SERV_UNIT_DELETE_EXCEPTION_MODIFY // �ؿ��� �߰�
    kDBPacket.m_iUserUID = GetUID();
#endif SERV_UNIT_DELETE_EXCEPTION_MODIFY
	//}}

    switch ( KBaseServer::GetKObj()->GetServerGroupID() )
    {
    case SEnum::SGI_SOLES:
        if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
        {   
            SendToGameDB( DBE_GAME_DELETE_UNIT_REQ, kDBPacket );
        }
        else if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA && GetKGSSimLayer()->GetServerGroupNum() == KGSSimLayer::DOUBLE_SERVER_GROUP )
        {
            SendToGameDB2nd( DBE_GAME_DELETE_UNIT_REQ, kDBPacket );
        }
        break;
    case SEnum::SGI_GAIA:
        if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA )
        {
            SendToGameDB( DBE_GAME_DELETE_UNIT_REQ, kDBPacket );
        }
        else if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
        {
            SendToGameDB2nd( DBE_GAME_DELETE_UNIT_REQ, kDBPacket );
        }

        break;
    default:
        if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )
        {
            SendToGameDB( DBE_GAME_DELETE_UNIT_REQ, kDBPacket );
        }
    }

    m_kUserTutorialInfo.OnDeleteUnitReq( kPacket_.m_iUnitUID );

}

IMPL_ON_FUNC( EGS_ENTRY_POINT_CHECK_NICK_NAME_REQ )
{
    // �г��� ���� üũ
    char charBuf[255] = {0};
    std::string strNickName;

    WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrNickName.c_str(), -1, charBuf, 255, NULL, NULL );
    strNickName = charBuf;
    
    KEGS_CHECK_NICK_NAME_ACK kPacket;

    if( strNickName.empty() || strNickName.size() > 12 )   //ĳ���� �г��� ���� �˻�.
    { // TODO : �ؿ��� �ڵ忡�� �г��� �ּ� ���ص� �ִµ�, �̷� �κ��� �����κ��ٴ� gssimlayer�� ��Ʒ� �����ϸ� ���ڴ�.
        
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_06;
        goto end_proc;        
    }

    if( GetAuthLevel() < SEnum::UAL_GM && SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
    {
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_05;
        goto end_proc;        
    }

    if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
    {
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_05;
        goto end_proc;
    }
    // ��� �ִ� �г����ΰ� , ���� ť���ؾ� �� �� �ִ�.
    // �г��� ���� ��Ÿ������ üũ.
    kPacket.m_iOK = NetError::NET_OK;

    switch ( KBaseServer::GetKObj()->GetServerGroupID() )
    {
    case SEnum::SGI_SOLES:
        if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
        {   
            SendToGameDB( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, kPacket_ );
        }
        else if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA )
        {
            SendToGameDB2nd( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, kPacket_ );
        }
        break;
    case SEnum::SGI_GAIA:
        if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA )
        {
            SendToGameDB( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, kPacket_ );
        }
        else if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
        {
            SendToGameDB2nd( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, kPacket_ );
        }

        break;
    default:
        if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )
		{
			START_LOG( cerr, L"�系���� ������ �̻�" )
				<< BUILD_LOG( KBaseServer::GetKObj()->GetServerGroupID() )
				<< BUILD_LOG( kPacket_.m_iServerGroup)
				<< END_LOG;
        }
		else
		{
			kPacket.m_iOK = NetError::ERR_HERO_PVP_USER_LIST_01;
			goto end_proc;        
		}
    }

end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( kPacket_.m_iServerGroup )
        << END_LOG;

    if ( kPacket.m_iOK != NetError::NET_OK ) 
    {
        SendPacket( EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK, kPacket );    
    }    
}

IMPL_ON_FUNC( EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK )
{
    SendPacket( EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK, kPacket_ );

    LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ )
{
    KDBE_GET_CREATE_UNIT_TODAY_COUNT_REQ kDBPacket;
    kDBPacket.m_iUserUID = GetUID();
    // �� �������� ���� ���� ���� �ҷ������� 1st, 2nd ť�� ���. ����� ���ļ� Ŭ���̾�Ʈ�� �����ֱ�
    SendToGameDB( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, kDBPacket );
}

_IMPL_ON_FUNC( EGS_GET_CREATE_UNIT_TODAY_COUNT_1ST_ACK, KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK )
{
    switch( GetKGSSimLayer()->GetServerGroupNum() )
    {
    case KGSSimLayer::SINGLE_SERVER_GROUP: 
        SendPacket( EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK, kPacket_ );
        break;
    case KGSSimLayer::DOUBLE_SERVER_GROUP: // �������� 2���� ����
        SendToGameDB2nd( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, kPacket_ );
        break;
    default:
        break;
    }
}

IMPL_ON_FUNC( EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK )
{
    SendPacket( EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ )
{
    //{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
    VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CHANGE_NICK_NAME_REQ, EGS_CHANGE_NICK_NAME_ACK );
#else
    VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );
#endif SERV_ADD_REPEAT_FILTER
    //}}

    char charBuf[255] = {0};
    std::string strNickName;
    KEGS_CHANGE_NICK_NAME_REQ kDBPacket;

    WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrNickName.c_str(), -1, charBuf, 255, NULL, NULL );
    strNickName = charBuf;

    if( strNickName.empty() || strNickName.size() > 12 )   //ĳ���� �г��� ���� �˻�.
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_06;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }

    if( GetAuthLevel() < SEnum::UAL_GM && SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_05;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }

    // ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
    if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_05;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }

    //{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
    if( SiKPvpMatchManager()->IsExistPvpNpcName( kPacket_.m_wstrNickName ) == true )
    {
        KEGS_CHANGE_NICK_NAME_ACK kPacket;
        kPacket.m_iOK = NetError::ERR_CREATE_UNIT_01;
        SendPacket( EGS_CHANGE_NICK_NAME_ACK, kPacket );
        return;
    }
#endif SERV_PVP_NEW_SYSTEM
    //}}

    kDBPacket.m_wstrNickName = kPacket_.m_wstrNickName;
    kDBPacket.m_iUnitUID = kPacket_.m_iUnitUID;
    kDBPacket.m_bCheckOnly = kPacket_.m_bCheckOnly;

    switch ( KBaseServer::GetKObj()->GetServerGroupID() )
    {
    case SEnum::SGI_SOLES:
        if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
        {   
            SendToGameDB( DBE_CHANGE_NICK_NAME_REQ, kDBPacket );
        }
        else if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA )
        {
            SendToGameDB2nd( DBE_CHANGE_NICK_NAME_REQ, kDBPacket );
        }
        break;
    case SEnum::SGI_GAIA:
        if ( kPacket_.m_iServerGroup == SEnum::SGI_GAIA )
        {
            SendToGameDB( DBE_CHANGE_NICK_NAME_REQ, kDBPacket );
        }
        else if ( kPacket_.m_iServerGroup == SEnum::SGI_SOLES )
        {
            SendToGameDB2nd( DBE_CHANGE_NICK_NAME_REQ, kDBPacket );
        }

        break;
    default:
        if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )
        {

        }
    }
}

#endif SERV_ENTRY_POINT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
IMPL_ON_FUNC( EGS_EVENT_CHUNG_GIVE_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EVENT_CHUNG_GIVE_ITEM_REQ, EGS_EVENT_CHUNG_GIVE_ITEM_ACK );
	
	KDBE_EVENT_CHUNG_GIVE_ITEM_REQ kChungPacket;
	CTime TempTime = CTime::GetCurrentTime();
	switch(kPacket_.iChoice)
	{
	case 1:
		{
			kChungPacket.m_wstrGiveItemTime_One = TempTime.Format(L"%Y-%m-%d %H:%M:%S");
		}break;
	case 2:
		{
			kChungPacket.m_wstrGiveItemTime_Two = TempTime.Format(L"%Y-%m-%d %H:%M:%S");
		}break;
	case 3:
		{
			kChungPacket.m_wstrGiveItemTime_Tree = TempTime.Format(L"%Y-%m-%d %H:%M:%S");
		}break;
	}

	kChungPacket.m_iUnitUID = GetCharUID();
	kChungPacket.m_bTwoGiveItem = kPacket_.bTwoGiveItem;
	kChungPacket.m_iChoice	= kPacket_.iChoice;
	SendToGameDB(DBE_EVENT_CHUNG_GIVE_ITEM_REQ,kChungPacket);
}
IMPL_ON_FUNC( DBE_EVENT_CHUNG_GIVE_ITEM_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"û ������ �����ð� ��� �����ؼ� �����ָ� �ȵ�!!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iChoice )
			<< BUILD_LOG( kPacket_.m_bTwoGiveItem )
			<< END_LOG;

		SetChungGiveItem(false);
		CTime TempTime = CTime::GetCurrentTime();
		KEGS_EVENT_CHUNG_GIVE_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrGetItemTime = TempTime.Format(L"%Y-%m-%d %H:%M:%S"); 
		SendPacket( EGS_EVENT_CHUNG_GIVE_ITEM_ACK , kPacketAck );
	}
	else
	{
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		switch(kPacket_.m_iChoice)
		{
		case 1:
			{
				if(kPacket_.m_bTwoGiveItem == false)
				{
					kPacketToDB.m_iRewardID	   = 1000214;
				}
				else
				{
					kPacketToDB.m_iRewardID	   = 1000215;
				}
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}break;
		case 2:
			{
				if(kPacket_.m_bTwoGiveItem == false)
				{
					kPacketToDB.m_iRewardID	   = 1000216;
				}
				else
				{
					kPacketToDB.m_iRewardID	   = 1000217;
				}
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}break;
		case 3:
			{
				if(kPacket_.m_bTwoGiveItem == false)
				{
					kPacketToDB.m_iRewardID	   = 1000218;
				}
				else
				{
					kPacketToDB.m_iRewardID	   = 1000219;
				}
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}break;
		}
	}
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
IMPL_ON_FUNC( DBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK )
{
	//���⼭ ��� ���忡 ���������� �������� ������ �����Ѵ�
	KEGS_EVENT_COBO_DUNGEON_FIELD_NOT kPacketCoboNot;
	kPacketCoboNot.m_iOk = kPacket_.m_iOk;
	kPacketCoboNot.m_DungeonCount = 0;
	kPacketCoboNot.m_FieldMonsterKillCount = 0;
	kPacketCoboNot.m_iRemaindTime = -1;
	SetCoboItemGive(true); //���� �޾Ҵٴ� ó�� 
	SetDungeonCount(0);
	SetFieldMosterKillCount(0);
	SetDungeonClearUI(false);
	SetFieldCountUI(false);
	SetStartButtonPush(true);
	SetRemaindTime(-1);
	START_LOG( clog, L"���⼭ ���� �ð� �ʱ�ȭ �ϳ�?" )
		<< BUILD_LOG( GetRemaindTime() )
		<< BUILD_LOG( kPacket_.m_NowDay )
		<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
		<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
		<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
		<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
		<< END_LOG;
	switch(kPacket_.m_NowDay)
	{
	case 1:
		{
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_DungeonCountUI =true;
			kPacketCoboNot.m_FieldCountUI = false;
		}
		break;
	case 2:
		{
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_DungeonCountUI =true;
			kPacketCoboNot.m_FieldCountUI = true;
		}
		break;
	case 3:
	case 4:
		{
			kPacketCoboNot.m_StartButtonUI = false;
			kPacketCoboNot.m_DungeonCountUI =false;
			kPacketCoboNot.m_FieldCountUI = false;
		}
		break;
	case 5:
		{
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_DungeonCountUI =false;
			kPacketCoboNot.m_FieldCountUI = true;
		}
		break;
	case 6:
		{
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_DungeonCountUI =true;
			kPacketCoboNot.m_FieldCountUI = true;
		}
		break;
	case 7:
	case 8:
		{
			kPacketCoboNot.m_StartButtonUI = false;
			kPacketCoboNot.m_DungeonCountUI =false;
			kPacketCoboNot.m_FieldCountUI = false;
		}
		break;
	}
	if(kPacket_.m_iOk != NetError::NET_OK)
	{
		START_LOG( cerr, L"�ں� ������ ���� ��� �����ؼ� �����ָ� �ȵ�!!" )
			<< BUILD_LOG( kPacket_.m_iOk )
			<< END_LOG;
		
		SendPacket( EGS_EVENT_COBO_DUNGEON_FIELD_NOT, kPacketCoboNot );
	}
	else
	{
		if( kPacket_.m_bWeekEndItem == true) //�ָ��� ���� ���� ����
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			//�ָ��� �ʵ�� ���� Ŭ���� ī��Ʈ �Ѵ� Ȯ��
			if(kPacket_.m_iDungeonClearCount > 1)
			{
				if(kPacket_.m_iDungeonClearCount > 1 && kPacket_.m_iDungeonClearCount < 4)
				{
					kPacketToDB.m_iRewardID	   = 1000282;
				}
				else if(kPacket_.m_iDungeonClearCount >= 4 && kPacket_.m_iDungeonClearCount < 6 )
				{
					kPacketToDB.m_iRewardID	   = 1000283;
				}
				else if(kPacket_.m_iDungeonClearCount >= 6)
				{
					kPacketToDB.m_iRewardID	   = 1000284;
				}
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				START_LOG( clog, L"ä�� �̵� �׽�Ʈ" )
					<< BUILD_LOG( kPacket_.m_bWeekEndItem )
					<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
					<< BUILD_LOG( kPacket_.m_iOk )
					<< BUILD_LOG( kPacket_.m_NowDay )
					<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
					<< BUILD_LOG( kPacket_.m_wstrButtonClickTime_One )
					<< END_LOG;
			}
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB2;
			kPacketToDB2.m_iFromUnitUID = GetCharUID();
			kPacketToDB2.m_iToUnitUID   = GetCharUID();
			kPacketToDB2.m_iRewardType  = KPostItemInfo::LT_EVENT;
			if(kPacket_.m_iFieldMonsterKillCount > 99)
			{
				if(kPacket_.m_iFieldMonsterKillCount > 99 && kPacket_.m_iFieldMonsterKillCount < 200)
				{
					kPacketToDB2.m_iRewardID	   = 1000282;
				}
				else if(kPacket_.m_iFieldMonsterKillCount >= 200 && kPacket_.m_iFieldMonsterKillCount < 300)
				{
					kPacketToDB2.m_iRewardID	   = 1000283;
				}
				else if(kPacket_.m_iFieldMonsterKillCount >= 300)
				{
					kPacketToDB2.m_iRewardID	   = 1000284;
				}
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB2 );
				START_LOG( clog, L"ä�� �̵� �׽�Ʈ" )
					<< BUILD_LOG( kPacket_.m_bWeekEndItem )
					<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
					<< BUILD_LOG( kPacket_.m_iOk )
					<< BUILD_LOG( kPacket_.m_NowDay )
					<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
					<< BUILD_LOG( kPacket_.m_wstrButtonClickTime_One )
					<< END_LOG;
			}
		}
		else
		{
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			//������ �ش� ��¥�� ���� �͸� Ȯ��
			CTime tChangeEventTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
			CTime tNowTime = CTime::GetCurrentTime();; //�ָ��� �ƴϸ� Ŭ�� �� ��¥�� �޾ƿ´�.
			if(tNowTime < tChangeEventTime)
			{
				//�̷��� ���� ī��Ʈ ���� ����
				//�ָ��� �ʵ�� ���� Ŭ���� ī��Ʈ �Ѵ� Ȯ��
				if(kPacket_.m_iDungeonClearCount > 1)
				{
					if(kPacket_.m_iDungeonClearCount > 1 && kPacket_.m_iDungeonClearCount < 4)
					{
						kPacketToDB.m_iRewardID	   = 1000282;
					}
					else if(kPacket_.m_iDungeonClearCount >= 4 && kPacket_.m_iDungeonClearCount < 6 )
					{
						kPacketToDB.m_iRewardID	   = 1000283;
					}
					else if(kPacket_.m_iDungeonClearCount >= 6)
					{
						kPacketToDB.m_iRewardID	   = 1000284;
					}
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
					START_LOG( clog, L"ä�� �̵� �׽�Ʈ ����" )
						<< BUILD_LOG( kPacket_.m_bWeekEndItem )
						<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
						<< BUILD_LOG( kPacket_.m_iOk )
						<< BUILD_LOG( kPacket_.m_NowDay )
						<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
						<< BUILD_LOG( kPacket_.m_wstrButtonClickTime_One )
						<< END_LOG;
				}
			}
			else
			{
				//���� �ʵ� ī��Ʈ ���� ����
				if(kPacket_.m_iFieldMonsterKillCount > 99)
				{
					if(kPacket_.m_iFieldMonsterKillCount > 99 && kPacket_.m_iFieldMonsterKillCount < 200)
					{
						kPacketToDB.m_iRewardID	   = 1000282;
					}
					else if(kPacket_.m_iFieldMonsterKillCount >= 200 && kPacket_.m_iFieldMonsterKillCount < 300)
					{
						kPacketToDB.m_iRewardID	   = 1000283;
					}
					else if(kPacket_.m_iFieldMonsterKillCount >= 300)
					{
						kPacketToDB.m_iRewardID	   = 1000284;
					}
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
					START_LOG( clog, L"ä�� �̵� �׽�Ʈ" )
						<< BUILD_LOG( kPacket_.m_bWeekEndItem )
						<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
						<< BUILD_LOG( kPacket_.m_iOk )
						<< BUILD_LOG( kPacket_.m_NowDay )
						<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
						<< BUILD_LOG( kPacket_.m_wstrButtonClickTime_One )
						<< END_LOG;
				}
			}

		}
		SendPacket( EGS_EVENT_COBO_DUNGEON_FIELD_NOT, kPacketCoboNot );
		START_LOG( clog, L"����� ������ ��������" )
			<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
			<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
			<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
			<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
			<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
			<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
			<< END_LOG;
	}
}
IMPL_ON_FUNC( EGS_EVENT_COBO_DUNGEON_FIELD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EVENT_COBO_DUNGEON_FIELD_REQ, EGS_EVENT_COBO_DUNGEON_FIELD_ACK );
	CTime tNowTime = CTime::GetCurrentTime();
	if(  GetLevel() < 10 || ( tNowTime.GetHour() == 23 && tNowTime.GetMinute() >= 30 && tNowTime.GetMinute() <= 59 ) )
	{
		KEGS_EVENT_COBO_DUNGEON_FIELD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_EVENT_COBO_DUNGEON_FIELD_ACK, kPacket );
		std::wstring  TempTime = tNowTime.Format(L"%Y-%m-%d %H:%M:%S");
		START_LOG( clog, L"���� 10 �̸� �ƴϸ� Ŭ�� ��ŷ " )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetLevel() )
			<< BUILD_LOG( TempTime )
			<< END_LOG;
	}
	else
	{
		//���� ���� ��Ŷ�� ������.
		KDBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_wstrButtonClickTime_One = tNowTime.Format(L"%Y-%m-%d %H:%M:%S");
		kPacketToDB.m_iDungeonClearCount = 0;
		kPacketToDB.m_iFieldMonsterKillCount = 0;
		kPacketToDB.m_bItemGive = false;
		kPacketToDB.m_bStartButton = kPacket_.m_EventStart;
		SetCoboItemGive(kPacketToDB.m_bItemGive); //���� ���� ���⼭ ó�� 
		SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ, kPacketToDB );
	}
}
IMPL_ON_FUNC( EGS_EVENT_COBO_DUNGEON_FIELD_ACK )
{
	KEGS_EVENT_COBO_DUNGEON_FIELD_ACK kPacket;
	kPacket.m_bStartUI = kPacket_.m_bStartUI; //��ŸƮ ��ư �������ϱ� false���� �Ѵ�
	kPacket.m_DungeonCount = kPacket_.m_DungeonCount; //�翬�� 0�̰���
	kPacket.m_FieldMonsterKillCount = kPacket_.m_FieldMonsterKillCount; //�̰͵� 0�̰���
	kPacket.m_iOK = kPacket_.m_iOK; //���� �����ߴ���
	kPacket.m_iRemaindTime = SiKGameEventScriptManager()->GetRemainTime(); //ó������ ���� 30���̴ϱ� 30�� ���
	CTime tButtonPush;
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrPushTime, tButtonPush ) == false )
	{
		tButtonPush = CTime::GetCurrentTime(); 
	}
	//���� UI Ȱ�� ���� ���⼭ �ѹ��� üũ���ִ°� ���ڴ�.
	CTime tEventChangeTime = SiKGameEventScriptManager()->GetCoboEventData()[0]; //����ī��Ʈ �̺�Ʈ���� �ʵ� ī��Ʈ �̺�Ʈ�� �ٲ����� ��¥
	if( tButtonPush < tEventChangeTime) //���� �Ⱓ�� ���� �ʾҴ�
	{
		//���� UI Ȱ��
		kPacket.m_DungeonCountUI = true;
		CTime tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[1];
		CTime tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[2];
		if( tButtonPush > tWeekEndTimeStart && tWeekEndTimeEnd > tButtonPush ) //�̷��� �ָ��̴�
		{
			//�ʵ� UI�� Ȱ�� 
			kPacket.m_FieldCountUI = true;
		}
		else
		{
			kPacket.m_FieldCountUI = false;
		}
	}
	else
	{
		//�ʵ� UI Ȱ��
		kPacket.m_FieldCountUI = true;
		CTime tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[3];
		CTime tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[4];
		if( tButtonPush > tWeekEndTimeStart && tWeekEndTimeEnd > tButtonPush ) //�̷��� �ָ��̴�
		{
			//���� UI�� Ȱ�� 
			kPacket.m_DungeonCountUI = true;
		}
		else
		{
			kPacket.m_DungeonCountUI = false;
		}
	}
	///���⼭ ������ ������ ������ �־�� �Ѵ�.
	///�ð� üũ�� �ؾ� �ϱ� �����̴�.���� �������� �ٲ���
	//Ŭ��� �ܼ��� ��ư ������ �ð� üũ �ϰ� UI�����ְ� �Ѵ�.
	//���Ͷ� ���� Ŭ���� ī��Ʈ�� üũ�ؾ��Ѵ�.
	//��ư�� �������� ó�� �����̴�!!
	SetDungeonCount(kPacket.m_DungeonCount);
	SetFieldMosterKillCount(kPacket.m_FieldMonsterKillCount);
	SetDungeonClearUI(kPacket.m_DungeonCountUI);
	SetFieldCountUI(kPacket.m_FieldCountUI);
	SetButtonClickTime(tButtonPush);
	SetRemaindTime(kPacket.m_iRemaindTime);
	START_LOG( clog, L"���⼭ ���� �ð� �ʱ�ȭ �ϳ�?" )
		<< BUILD_LOG( GetRemaindTime() )
		<< END_LOG;
	SetStartButtonPush(kPacket.m_bStartUI);
	//���⼭ Ŭ��� ���� �ð� ����.
	kPacket.m_tPushTime = tButtonPush.GetTime();
	SendPacket( EGS_EVENT_COBO_DUNGEON_FIELD_ACK, kPacket );
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
IMPL_ON_FUNC( DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK )
{
	if(kPacket_.m_iOk != NetError::NET_OK)
	{
		START_LOG( clog, L"Ŭ�� ���� ���� �߷�Ÿ�� ����" )
			<< BUILD_LOG( kPacket_.m_iOk )
			<< END_LOG;
		return;
	}
	SetValentineItemCount(kPacket_.m_iValenTineItemCount);
	KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT kPacketValen;
	kPacketValen.m_iValentineItemCount = kPacket_.m_iValenTineItemCount;
	SendPacket(EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT,kPacketValen);
	START_LOG( clog, L"Ŭ�� ���� ���� �߷�Ÿ�� ����" )
		<< BUILD_LOG( kPacketValen.m_iValentineItemCount)
		<< END_LOG;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_MANUFACTURE_PERIOD_FIX
IMPL_ON_FUNC( EGS_MANUFACTURE_PERIOD_SETTING_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_MANUFACTURE_PERIOD_SETTING_REQ, EGS_MANUFACTURE_PERIOD_SETTING_ACK );

	KEGS_MANUFACTURE_PERIOD_SETTING_ACK kPacketPeriodGroup;

	kPacketPeriodGroup.m_mapPeriodGroup.clear();
	kPacketPeriodGroup.m_mapPeriodGroup.insert( SiCXSLManufactureItemManager()->m_mapPeriodGroup.begin(), SiCXSLManufactureItemManager()->m_mapPeriodGroup.end() );

	kPacketPeriodGroup.m_iHouseID = kPacket_.m_iHouseID;

	SendPacket(EGS_MANUFACTURE_PERIOD_SETTING_ACK, kPacketPeriodGroup);
	START_LOG( clog, L"Ŭ�� �Ⱓ�� �׷� ���� ���� ���� �Ʒ��� ������" )
		<< BUILD_LOG( kPacketPeriodGroup.m_mapPeriodGroup.size())
		<< BUILD_LOG( kPacketPeriodGroup.m_iHouseID)
		<< END_LOG;
}
#endif //SERV_MANUFACTURE_PERIOD_FIX


#ifdef SERV_4TH_ANNIVERSARY_EVENT
IMPL_ON_FUNC( EGS_4TH_ANNIV_EVENT_REWARD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_4TH_ANNIV_EVENT_REWARD_REQ, EGS_4TH_ANNIV_EVENT_REWARD_ACK );

	KEGS_4TH_ANNIV_EVENT_REWARD_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
	kPacketAck.m_iSeletedIndex = -1;

	if( kPacket_.m_iSeletedIndex < 0 || kPacket_.m_iSeletedIndex > 11 )
	{
		// TODO : �ݿ��� �߱��ؾ� �մϴ�.
		// �߸��� ���� ��ȣ �Դϴ�.
		kPacketAck.m_iOK = NetError::ERR_4TH_ANNIV_EVENT_02;
		SendPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK, kPacketAck );
		return;
	}
			
	// �Ϸ翡 �ѹ� �����ϵ��� üũ �մϴ�.
	const int iResetHour = _CONST_SERV_4TH_ANNIVERSARY_EVENT_REWARD_ID::iRewardResetHour;
	CTime tResetTime = m_tLastRewardTime - CTimeSpan(0, iResetHour, 0, 0);
	tResetTime = CTime(tResetTime.GetYear(), tResetTime.GetMonth(), tResetTime.GetDay(), iResetHour, 0, 0) + CTimeSpan(1,0,0,0);
	CTime tCurrentTime = CTime::GetCurrentTime();

	if( tCurrentTime < tResetTime )
	{
		// �Ϸ翡 �ѹ� ���� ���� �����մϴ�.
		kPacketAck.m_iOK = NetError::ERR_4TH_ANNIV_EVENT_01;
		SendPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK, kPacketAck );
		return;
	}

	// ������ ������ ���� ������ ������Ʈ
	if( m_vec4ThAnnivEventRewardInfo.size() != 12 )
	{
		START_LOG(cerr, L"[4�ֳ�] ���� vec ����� 12 �� �ƴϴ�. ������ ���� ����")
			<< BUILD_LOG( m_vec4ThAnnivEventRewardInfo.size() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_4TH_ANNIV_EVENT_02;
		SendPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK, kPacketAck );
		return;
	}
	
	if( false == m_vec4ThAnnivEventRewardInfo[kPacket_.m_iSeletedIndex] )
		m_vec4ThAnnivEventRewardInfo[kPacket_.m_iSeletedIndex] = true;
	else
	{
		START_LOG(cerr, L"[4�ֳ�] �̹� ���� �����̴�? ������ ���� ����")
			<< BUILD_LOG( kPacket_.m_iSeletedIndex )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_4TH_ANNIV_EVENT_01;
		SendPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK, kPacketAck );
		return;
	}

	m_tLastRewardTime = tCurrentTime;

	// ���� ���� �����ϴ�.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_iToUnitUID   = GetCharUID();
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
	kPacketToDB.m_iRewardID	   = _CONST_SERV_4TH_ANNIVERSARY_EVENT_REWARD_ID::iRewardID + kPacket_.m_iSeletedIndex;
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

	// ���� ��Ŷ
	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_iSeletedIndex = kPacket_.m_iSeletedIndex;

	SendPacket( EGS_4TH_ANNIV_EVENT_REWARD_ACK, kPacketAck );
}
#endif // SERV_4TH_ANNIVERSARY_EVENT