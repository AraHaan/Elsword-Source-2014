#include "CnSimLayer.h"
#include "RoomUser.h"

#include "Room/RoomUserFSM.h"
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLDungeon.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLQuestManager.h"
#include "CnRoomManager.h"
#include "NetError.h"

//{{ 2012. 04. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	#include "CnServer.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "x2data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

#ifdef SERV_NEW_YEAR_EVENT_2014
	#include "x2data/XSLBattleFieldManager.h"
#endif SERV_NEW_YEAR_EVENT_2014

KRoomUser::KRoomUser() :
    m_iTeam( 0 ),
    m_bHost( false ),
    m_bReady( false ),
	m_bPitIn( false ),
    m_bTrade( false ),
    m_iLoadingProgress( -1 ),
    m_bStageLoaded( false ),
    m_nKill( 0 ),
	m_nMDKill( 0 ),
    m_nDie( 0 ),
    m_bDie( false ),
    m_fHP( -1.f ),
    m_iSlotID( -1 ),
	m_iNowStageID( -1 ),
    m_iRebirthPos( 0 ),
	m_nKillNPC( 0 ),
	m_bDungeonUnitInfo( false ),
	m_iOldEXP( 0 ),
	m_iOldED( 0 ),
	m_iRewardEXP( 0 ),
	m_iRewardPartyEXP( 0 ),	
	m_nRewardED( 0 ),
	m_nRewardVP( 0 ),
	m_bIsResult( false ),
	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_bActivatedAutoPartyBonus( false ),
	m_bBattleFieldNpcLoad( false ),
	m_bBattleFieldNpcSyncSubjects( false ),
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_bHenirReward( false ),
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2007. 11. 5  ������  
	m_iUsedResurrectionStoneCount( 0 ),
	m_iPassedStageCount( 0 ),
	m_iPassedSubStageCount( 0 ),
	//}}
	//{{ 2007. 12. 17  ������  ���� ����
	m_bIsIntrude( false ),
	//}}
	//{{ 2008. 1. 11  ������  �÷��� Ÿ�� �ʱ�ȭ
	m_fPlayTime( 0.0f ),
	//}}
	//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
	m_fLastStagePlayTime( 0.0 ),
	m_iStageDamage( 0 ),
#endif SERV_DUNGEON_STAGE_LOG
	//}}
	//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_iPvpNpcID( 0 ),
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	m_iStartTimeNumResurrectionStone( 0 ),
	m_iFirstUseResurrectionStoneStageID( -1 ),
	m_iFirstUseResurrectionStoneSubStageID( -1 ),
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	m_bRecivedPingScore( false ),
#endif SERV_CHOOSE_FASTEST_HOST
    //}}
    //{{ 2013.08.05    ������   ������ - �ʵ� ��� ���屳ü�� ����
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_bZombieAlert( false ),
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    //}} 
	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
	m_bEndPlay( false ),
	m_bCashContinueReady( false ),
	m_fDelayPacketTime( 15.0f )
	//}}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	, m_bPrepareForDefence( false )	
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	, m_bRecvEnterPopupReply( false )
	, m_bEnterDefenceDungeon( false )
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}
	//{{ 2012. 11. 20	�ڼ���	Field Frame �α� �߰�
#ifdef SERV_FIELD_FRAME_LOG
	, m_iFieldFrame( 0 )
#endif SERV_FIELD_FRAME_LOG
	//}}
#ifdef SERV_PVP_REMATCH
	, m_bAcceptRematch( false )
#endif SERV_PVP_REMATCH
{

//#ifdef PVP_BOSS_COMBAT_TEST
//	m_bIsBoss = false;
//#endif PVP_BOSS_COMBAT_TEST

	//{{ 2009. 7. 21  ������	��н�������
	m_eAgreeEnterSecretStage = CXSLRoom::ESS_NONE;
	//}}

	//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
	m_bEnterCashShopInBattleField = false;
#endif SERV_VISIT_CASH_SHOP
	//}}
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	m_fAddQuestDropBouns = 0.f;
#endif SERV_SERVER_BUFF_SYSTEM
	//}
	//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	m_iAutoPartyWaitTime = 0;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
}

KRoomUser::~KRoomUser()
{
}
//{{ 2012. 10. 31	�ڼ���	Merge ������ ���� �۾�-�����̸� �⺻������ ����Ѵ�.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
bool KRoomUser::IsOnlyPlaying()
{
	if( m_pkCurrentState->CheckState( 1, KRoomUserFSM::S_PLAY ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

bool KRoomUser::IsPlaying()
{
    if( m_pkCurrentState->CheckState( 2, KRoomUserFSM::S_LOAD, KRoomUserFSM::S_PLAY ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void KRoomUser::StartGame()
{
    m_iLoadingProgress	= 0;
    m_bStageLoaded		= false;
    m_iNowStageID		= -1;
    m_iRebirthPos		= 0;
	m_nKill				= 0;
	m_nMDKill			= 0;
	m_nDie				= 0;
	m_bDie				= false;
	m_nKillNPC			= 0;
	m_iRewardEXP		= 0;
	m_iRewardPartyEXP	= 0;
	m_nRewardED			= 0;
	m_nRewardVP			= 0;
	m_bIsResult			= false;
	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM	
	m_bBattleFieldNpcLoad = false;
	m_bBattleFieldNpcSyncSubjects = false;
	m_setIntrudeUnitLoadUserList.clear();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_bHenirReward		= false;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2007. 11. 5  ������  
	m_iUsedResurrectionStoneCount = 0;
	m_iPassedStageCount	= 0;
	m_iPassedSubStageCount = 0;
	//}}
	//{{ 2007. 12. 18  ������  ���Ի��� �ʱ�ȭ -> StartGame()�� ȣ��ǰ��� ���Ի��¸� ������.
	m_bIsIntrude		= false;
	//}}
	//{{ 2009. 7. 21  ������	��н�������
	m_eAgreeEnterSecretStage = CXSLRoom::ESS_NONE;
	//}}
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	m_iStartTimeNumResurrectionStone = 0;
	m_iFirstUseResurrectionStoneStageID = -1;
	m_iFirstUseResurrectionStoneSubStageID = -1;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	m_mapGetItemList.clear();

	//{{ 2010. 05. 12  ������	���� �ڵ� ����
	m_kDungeonUnitInfo.Clear();
	//}}
	m_bDungeonUnitInfo = false;

	//{{ 2010. 12. 7	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT
	m_kAppropriateLevelInfo.Clear();

	std::map< int, KInventoryItemInfo >::const_iterator mit;
	for( mit = m_kRoomUserInfo.m_mapEquippedItem.begin(); mit != m_kRoomUserInfo.m_mapEquippedItem.end(); ++mit )
	{
		if( mit->first != CXSLUnit::NESI_WEAPON_HAND )
			continue;

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->second.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ������ �Դϴ�!" )
				<< BUILD_LOG( mit->second.m_kItemInfo.m_iItemID )
				<< END_LOG;
			continue;
		}

		m_kAppropriateLevelInfo.m_iWeaponLevel = pItemTemplet->m_UseLevel;
		m_kAppropriateLevelInfo.m_iWeaponGrade = pItemTemplet->m_ItemGrade;
		break;
	}
	m_kAppropriateLevelInfo.m_iStartedAtkPhysic = m_kRoomUserInfo.m_kGameStat.m_iAtkPhysic;
	m_kAppropriateLevelInfo.m_iStartedAtkMagic = m_kRoomUserInfo.m_kGameStat.m_iAtkMagic;
#endif SERV_APPROPRIATE_LEVEL_STAT
	//}}

	//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
	m_mapBuff.clear();
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	m_bRecivedPingScore = false;
	m_vecPingScore.clear();
#endif SERV_CHOOSE_FASTEST_HOST
	//}
    //{{ 2013.08.05    ������   ������ - �ʵ� ��� ���屳ü�� ����
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_bZombieAlert = false;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    //}}
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	m_bPrepareForDefence = false;	
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	m_bRecvEnterPopupReply = false;
	m_bEnterDefenceDungeon = false;
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

    StateTransition( KRoomUserFSM::I_TO_LOAD );
    m_kLoadingTimer.restart();
}

void KRoomUser::StartPlay()
{
    m_iLoadingProgress		= -1;
    m_nKill					= 0;
	m_nMDKill				= 0;
    m_nDie					= 0;
    m_bDie					= false;
    m_fHP					= -1.f;

    StateTransition( KRoomUserFSM::I_TO_PLAY );
    m_kPlayTimer.restart();
	m_fPlayTime				= 0.0f;
	//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
	m_kStagePlayTimer.restart();
	m_fLastStagePlayTime	= 0.0;
	m_iStageDamage			= 0;
#endif SERV_DUNGEON_STAGE_LOG
	//}}

	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
	m_bEndPlay				= false;
	//}}
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	m_iStartTimeNumResurrectionStone = m_kRoomUserInfo.m_iNumResurrectionStone;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
}

void KRoomUser::EndPlay()
{
    m_iLoadingProgress	= 0;
    m_bStageLoaded		= false;
    m_iNowStageID		= -1;
    m_iRebirthPos		= 0;

	m_fPlayTime			= static_cast<float>(m_kPlayTimer.elapsed());

	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
	m_bEndPlay			= true;
	//}}

    StateTransition( KRoomUserFSM::I_TO_RESULT );
}

void KRoomUser::EndGame()
{
    if( !SetReady( false ) )
    {
        START_LOG( cerr, L"���� ���� ���� ����." )
            << BUILD_LOG( GetCID() )
            << END_LOG;
    }

    m_iLoadingProgress = -1;

    StateTransition( KRoomUserFSM::I_TO_INIT );
}

bool KRoomUser::IsLoadingTimeOut()
{
	// XUAL_SPECIAL_USER �� ���, �ε� �ð� ���Ѵ�
	if ( GetAuthLevel() >= 1 )
		return false;

	//{{ 2010. 11. 23	������	�� �ε� ���� ����
#ifdef SERV_ROOM_LOADING_BUG_FIX
	// �ε��� �Ϸ�� ������ �濡�� ƨ���� �ʴ´�!
	if( GetLoadingProgress() == 100 )
		return false;
#endif SERV_ROOM_LOADING_BUG_FIX
	//}}

    if( GetStateID() == KRoomUserFSM::S_LOAD )
    {
        if( m_kLoadingTimer.elapsed() >= LOADING_TIME_OUT )
        {
            return true;
        }
    }

    return false;
}

//�������̸� ���� ����ð��� �ְ�
//������ ���� ������ ��� ���������ϴ�[EndPlay()�Լ��� ȣ����] ���ֿ��Ը� �÷����� �ð���
//�����ش�.
float KRoomUser::GetPlayTime()
{
	if( GetStateID() != KRoomUserFSM::S_PLAY )
	{
		return m_fPlayTime;
	}

	m_fPlayTime = static_cast<float>(m_kPlayTimer.elapsed());

	return m_fPlayTime;
}

//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
void KRoomUser::IncreaseUsedResurrectionStoneCount()
{
	++m_iUsedResurrectionStoneCount;

	if( m_iFirstUseResurrectionStoneStageID == -1 )
	{
		m_iFirstUseResurrectionStoneStageID = GetStateID();
	}

	if( m_iFirstUseResurrectionStoneSubStageID == -1 )
	{
		m_iFirstUseResurrectionStoneSubStageID = GetSubStageID();
	}
}
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//}}

void KRoomUser::SetDungeonUnitInfo( IN const KDungeonPlayResultInfo& kDungeonUnitInfo )
{
	m_kDungeonUnitInfo = kDungeonUnitInfo;
	m_bDungeonUnitInfo = true;
}

//{{ 2008. 9. 19  ������	���
void KRoomUser::SetDungeonPlayInfo( KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT& kDungeonPlayInfo )
{
	m_kDungeonUnitInfo.m_nDamageCount		  = kDungeonPlayInfo.m_nDamageCount;
	m_kDungeonUnitInfo.m_ComboScore	  		  = kDungeonPlayInfo.m_ComboScore;
	m_kDungeonUnitInfo.m_TechScore	  	 	  = kDungeonPlayInfo.m_TechScore;
	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	m_kDungeonUnitInfo.m_iTotalAttackedDamage = kDungeonPlayInfo.m_iTotalAttackedDamage;
	m_kDungeonUnitInfo.m_iTotalRecoveryHP	  = kDungeonPlayInfo.m_iTotalRecoveryHP;
	m_kDungeonUnitInfo.m_iUsingKindOfSkill	  = kDungeonPlayInfo.m_iUsingKindOfSkill;
#endif SERV_DUNGEON_RANK_NEW
	//}}
}
//}}

//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
void KRoomUser::SetPvpPlayResultInfo( IN const KPvpPlayResultInfo& kPvpUnitInfo )
{
	m_kPvpUnitInfo = kPvpUnitInfo;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

void KRoomUser::SetUserEquipItem( std::vector< KInventoryItemInfo >& vecChangedInvenSlotInfo )
{
    std::vector< KInventoryItemInfo >::iterator vit;
    for( vit = vecChangedInvenSlotInfo.begin(); vit != vecChangedInvenSlotInfo.end(); ++vit )
    {
        if( vit->m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
        {
			//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			std::map< int, KInventoryItemInfo >::iterator mit = m_kRoomUserInfo.m_mapEquippedItem.find( vit->m_sSlotID );
#else
			std::map< int, KInventoryItemInfo >::iterator mit = m_kRoomUserInfo.m_mapEquippedItem.find( vit->m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			if ( mit != m_kRoomUserInfo.m_mapEquippedItem.end() )
			{
				m_kRoomUserInfo.m_mapEquippedItem.erase( mit );
			}
			
            // �������� ���� �ϴ� �͸� insert �Ѵ�.
            if( vit->m_iItemUID > 0 )
            {
				//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				m_kRoomUserInfo.m_mapEquippedItem[vit->m_sSlotID] = ( *vit );
#else
				m_kRoomUserInfo.m_mapEquippedItem[vit->m_cSlotID] = ( *vit );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}

				START_LOG( clog, L"���ü ��." )
					<< BUILD_LOG( vit->m_iItemUID )
					<< BUILD_LOG( vit->m_kItemInfo.m_iItemID )
					;
            }
        }
    }
}

void KRoomUser::AddItem( int iItemID, int iQuantity )
{
	LIF( iQuantity > 0 );

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return;
	}

	std::map< int, int >::iterator mit = m_mapGetItemList.find( iItemID );
	if( mit == m_mapGetItemList.end() )
	{
		m_mapGetItemList.insert( std::make_pair( iItemID, iQuantity ) );
	}
	else
	{
		mit->second += iQuantity;		
	}		
}

int KRoomUser::GetItemCount()
{
	int nCount = 0;

	std::map< int, int >::const_iterator mit;

	for( mit = m_mapGetItemList.begin(); mit != m_mapGetItemList.end(); ++mit )
	{
		nCount += mit->second;
	}

	return nCount;
}

//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KRoomUser::ZU_Tick( IN const CXSLRoom::ROOM_TYPE eRoomType )
{
	if( eRoomType == CXSLRoom::RT_BATTLE_FIELD )
	{
		if( m_TimerZU.elapsed() >= GetKCnRoomManager()->GetBattleFieldZUTickTime() )
		{
			START_LOG( cout2, L" as a Zombie. delete unit.!!" )
				<< BUILD_LOG( GetKCnRoomManager()->GetBattleFieldZUTickTime() )
				<< BUILD_LOG( m_TimerZU.elapsed() )
				<< BUILD_LOG( m_kRoomUserInfo.m_iGSUID )
				<< BUILD_LOG( m_kRoomUserInfo.m_nUnitUID )
				<< BUILD_LOG( m_kRoomUserInfo.m_wstrNickName )
				<< END_LOG;

			return true;
		}
	}
	else
	{
		if( m_TimerZU.elapsed() >= GetKCnRoomManager()->GetZUTickTime() )
		{
			START_LOG( cout2, L" as a Zombie. delete unit.!!" )
				<< BUILD_LOG( GetKCnRoomManager()->GetZUTickTime() )
				<< BUILD_LOG( m_TimerZU.elapsed() )
				<< BUILD_LOG( m_kRoomUserInfo.m_iGSUID )
				<< BUILD_LOG( m_kRoomUserInfo.m_nUnitUID )
				<< BUILD_LOG( m_kRoomUserInfo.m_wstrNickName )
				<< END_LOG;

			return true;
		}
	}	

	return false;
}



#else
bool KRoomUser::ZU_Tick()
{
	if( m_TimerZU.elapsed() >= GetKCnRoomManager()->GetZUTickTime() )
	{
		START_LOG( cout2, L" as a Zombie. delete unit.!!" )
			<< BUILD_LOG( m_kRoomUserInfo.m_iGSUID )
			<< BUILD_LOG( m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( m_kRoomUserInfo.m_wstrNickName )
			<< END_LOG;

		return true;
	}	

	return false;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
bool    KRoomUser::ZombieAlert_Tick()
{
    if ( ZombieAlert_Get() == false && m_TimerZombieAlert.elapsed() >= GetKCnRoomManager()->GetBattleFieldZombieAlertTickTime() )
    {
        ZombieAlert_Set( true );
    }
    return ZombieAlert_Get();
}

void    KRoomUser::ZombieAlert_Refresh()
{
	m_TimerZombieAlert.restart();
}

bool KRoomUser::ZombieAlert_Get()
{
    return  m_bZombieAlert;
}


void KRoomUser::ZombieAlert_Set( bool bZombieAlert )
{
    m_bZombieAlert = bZombieAlert;
}

#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

void KRoomUser::ZU_Refresh()
{
	m_TimerZU.restart();
}




bool KRoomUser::RequestTradeTo( UidType nCID )
{
    if( m_mapTradeRequest.empty() && m_bTrade )
    {
        // �ŷ��� �������ε� �ٸ� �ŷ��� ��û��.
        return false;
    }

    std::map< UidType, boost::timer >::iterator mit = m_mapTradeRequest.find( nCID );
    if( mit != m_mapTradeRequest.end() )
    {
        return false;
    }

    boost::timer kTimer;
    m_mapTradeRequest.insert( std::make_pair( nCID, kTimer ) );
    SetTrade( true );

    return true;
}

bool KRoomUser::TradeAcceptedBy( UidType nCID )
{
    std::map< UidType, boost::timer >::iterator mit = m_mapTradeRequest.find( nCID );
    if( mit != m_mapTradeRequest.end() )
    {
        m_mapTradeRequest.clear();
        return true;
    }

    return false;
}

bool KRoomUser::TradeRejectedBy( UidType nCID )
{
    std::map< UidType, boost::timer >::iterator mit = m_mapTradeRequest.find( nCID );
    if( mit == m_mapTradeRequest.end() )
    {
        START_LOG( cwarn, L"�ŷ� ��û ���� �ƴѵ� ���� ���ߴ�." )
            << BUILD_LOG( GetCID() )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    m_mapTradeRequest.erase( mit );
    if( m_mapTradeRequest.empty() )
    {
        SetTrade( false );
    }

    return true;
}

bool KRoomUser::ExpireTradeRequest()
{
    bool bFlag = !m_mapTradeRequest.empty();

    std::map< UidType, boost::timer >::iterator mit = m_mapTradeRequest.begin();
    while( mit != m_mapTradeRequest.end() )
    {
        if( mit->second.elapsed() > 10.0 )
        {
            mit = m_mapTradeRequest.erase( mit );
            continue;
        }

        mit++;
    }

    if( bFlag && m_mapTradeRequest.empty() )
    {
        SetTrade( false );
        return true;
    }

    return false;
}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
bool KRoomUser::GetQuestDropItem( IN const int iDungeonID, 
								 IN const char cDifficulty,
								 IN const char cDungeonMode,
								 IN const int iNPCID, 
								 OUT QUEST_ITEM_DROP_INFO& sInfo, 
								 IN const bool bComeBackUserInRoom /*false*/ 
								 //{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
								 , IN float fEventDropRate
#endif SERV_QUEST_ITEM_DROP_EVENT
							  //}
#ifdef SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
								 , IN bool bSuitableLevel
#endif //SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
								 )
{
	SET_ERROR( NET_OK );

	sInfo.m_iGSUID = GetGSUID();
	sInfo.m_iUnitUID = GetCID();
	sInfo.m_vecDropQuestItem.clear();


	if( m_kRoomUserInfo.m_mapOngoingQuest.size() <= 0 )
		return false;

	std::map< int, KSubQuestInfo >::iterator mitQ;
	std::map< int, KSubQuestData >::iterator mitS;	//##arg 1. sub quest id | ##arg 2. quest item num

	//2���̻��� ����Ʈ�� �������ϼ��ְ�
	//����Ʈ �������� �����Ѵ� ���������� 2���̻��ϼ� �ִٴ� �����Ͽ� ����..

	for( mitQ = m_kRoomUserInfo.m_mapOngoingQuest.begin(); mitQ != m_kRoomUserInfo.m_mapOngoingQuest.end(); ++mitQ )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitQ->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ����Ʈ�Դϴ�." )
				<< BUILD_LOG( mitQ->first )
				<< END_LOG;
			continue;
		}

		// ���� ����Ʈ �̸鼭 ĳ���� ���� ������ �������� ������ ������ ������� �ʴ´�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType  &&  pQuestTemplet->m_iPlayLevel > GetLevel() )
			continue;

		for( mitS = mitQ->second.m_mapSubQuestInfo.begin(); mitS != mitQ->second.m_mapSubQuestInfo.end(); ++mitS )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( mitS->first );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ �̻�.!" )
					<< BUILD_LOG( mitS->first )
					<< BUILD_LOG( mitS->second.m_iInInventoryItemCount )
					<< BUILD_LOG( mitS->second.m_bSussece )
					<< BUILD_LOG( GetNickName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// ������ ���� ����Ʈ�� üũ�Ѵ�!
			if( pSubTemplet->m_eClearType != CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
				continue;

#ifdef SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
			if( pSubTemplet->m_iID == 92050 )
			{
				if( false == IsEquippedItem( 90003031 ) || false == bSuitableLevel )
					continue;
			}
			else if( pSubTemplet->m_iID == 92060 )
			{
				if( false == IsEquippedItem( 90003032 ) || false == bSuitableLevel )
					continue;
			}
#endif SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER

			//{{ 2009. 4. 1  ������		���̵� �̻�
			if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == true )
			{
				// �ش� ��д����� �´��� �˻�
				if( pSubTemplet->m_ClearCondition.m_setDungeonID.size() > 0 )
				{
					// Dungeon �˻�����
					std::set<SEnum::DUNGEON_ID>::const_iterator sit = pSubTemplet->m_ClearCondition.m_setDungeonID.find( static_cast<SEnum::DUNGEON_ID>(iDungeonID + static_cast<int>(cDifficulty)) );
					if( sit == pSubTemplet->m_ClearCondition.m_setDungeonID.end() )
					{
						continue;							
					}
				}

				// ���̵� �˻�
				if( pSubTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID )
				{
					if( pSubTemplet->m_ClearCondition.m_bUpperDifficulty == false )
					{
						if( pSubTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode )
							continue;
					}
				}
			}
			else		
			{
				// Ư�� Dungeon ������ �Ϸ� �����ϴٸ�
				if( pSubTemplet->m_ClearCondition.m_setDungeonID.size() > 0 )
				{
					// ���̵� ���� �̻��̶�� �˻�����
					int iDungeonIDAndDiff = iDungeonID + static_cast<int>(cDifficulty);
					if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( mitS->first, iDungeonIDAndDiff, pSubTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					{
						continue;
					}
				}
			}
#ifdef SERV_HENIR_EVENT_ITEM_NO_DROP
			if( pQuestTemplet->m_iID == 121000 || pQuestTemplet->m_iID == 121010 || pQuestTemplet->m_iID == 121020 || pQuestTemplet->m_iID == 121120 || pQuestTemplet->m_iID == 121550)
			{
				//��ϸ� �����̸� ��� �ȵǵ���
				if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
				{
					continue;
				}
			}
			else if( pQuestTemplet->m_iID == 131100 || pQuestTemplet->m_iID == 131110 || 
				pQuestTemplet->m_iID == 121020 || pQuestTemplet->m_iID == 131130 )
			{
				//��ϸ� ������ �ƴϸ� ��� �ȵǵ���
				if( CXSLDungeon::IsHenirDungeon( iDungeonID ) != true )
				{
					continue;
				}
			}
#endif SERV_HENIR_EVENT_ITEM_NO_DROP
#ifdef SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
			if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
			{
				//�Ϲ� ��� �϶� 
				if( pSubTemplet->m_iID == 131140 )
				{
					//���� ����̸� �ø���
					if( CXSLDungeon::IsHenirChallengeMode( static_cast<int>( cDungeonMode ) ) == true )
					{
						continue;
					}
				}
				//���� ��� �϶�
				else if( pSubTemplet->m_iID == 131150 )
				{
					//�Ϲ� ����̸� �ø���
					if( CXSLDungeon::IsHenirPracticeMode( static_cast<int>( cDungeonMode) ) == true )
					{
						continue;
					}

				}
			}
#endif //SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
			//{{ 2009. 8. 3  ������		����Ʈ ���� ����
			if( pSubTemplet->m_ClearCondition.m_setKillNPCID.empty() == false  &&
				pSubTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNPCID) ) == pSubTemplet->m_ClearCondition.m_setKillNPCID.end() )
			{
				continue;
			}
			//}}

			if( pSubTemplet->m_ClearCondition.m_iCollectionItemNum <= mitS->second.m_iInInventoryItemCount )
				continue;

			if( pSubTemplet->m_ClearCondition.m_iItemID <= 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			std::vector<int> vecUnderGroupSubQuest;
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pSubTemplet->m_iID, vecUnderGroupSubQuest ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			//{{ 2009. 10. 5  ������	0% Ȯ���ε� �������� ���ü� �ֳ�? ����� ��ġ��!
			float fDropRate = static_cast<float>( rand()%10000 ) + 1.f; //�⺻ Ȯ���� 10000/1 �� ��´�. ( 1 ~ 10000 ���� )
			fDropRate /= 100.0f;
			//}}

				//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				float fQuestItemDropRate = pSubTemplet->m_ClearCondition.m_fQuestItemDropRate * fEventDropRate;
#else
				float fQuestItemDropRate = pSubTemplet->m_ClearCondition.m_fQuestItemDropRate;
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}

			// ����Ʈ ��� Ȯ�� ���� �ߺ��� ���� �ʰ� ó��
			if( bComeBackUserInRoom == true )
			{
				fQuestItemDropRate = fQuestItemDropRate * 2.0f;      // ��ӷ� 2�� ����
			}
#ifdef SERV_REMOVE_ADDITIONAL_DROP_RATE_IN_GAIA
#else SERV_REMOVE_ADDITIONAL_DROP_RATE_IN_GAIA
			else if( GetServerGroupIDThisUser() == SEnum::SGI_GAIA )
			{
				fQuestItemDropRate = fQuestItemDropRate * 1.5f;      // ���̾� ������ ������� ��ӷ� 1.5�� ����
			}
#endif SERV_REMOVE_ADDITIONAL_DROP_RATE_IN_GAIA
			else if( m_fAddQuestDropBouns > 0.f )
			{
				fQuestItemDropRate = fQuestItemDropRate + m_fAddQuestDropBouns;      // ����Ʈ ��� ���ʽ�
			}

			if( fDropRate <= fQuestItemDropRate )
			{
				sInfo.m_vecDropQuestItem.push_back( pSubTemplet->m_ClearCondition.m_iItemID );

				++mitS->second.m_iInInventoryItemCount;
			}
		}
	}

	if( sInfo.m_vecDropQuestItem.size() > 0 )
	{
		START_LOG( clog, L"����Ʈ ������ ���.!" )
			<< BUILD_LOG( sInfo.m_vecDropQuestItem.size() )
			;

		return true;
	}

	return false;
}

#else				// SERV_REFORM_QUEST

//{{ 2011. 08. 31  ��μ�	��д��� ����
//#ifdef SERV_RANDOM_DAY_QUEST
bool KRoomUser::GetQuestDropItem( IN const int iDungeonID, 
								  IN const char cDifficulty,
								  IN const char cDungeonMode,
								  IN const int iNPCID, 
								  OUT QUEST_ITEM_DROP_INFO& sInfo, 
								  IN const bool bComeBackUserInRoom /*false*/ )
//#endif SERV_RANDOM_DAY_QUEST
//}}
{
	SET_ERROR( NET_OK );

	sInfo.m_iGSUID = GetGSUID();
	sInfo.m_iUnitUID = GetCID();
	sInfo.m_vecDropQuestItem.clear();

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	if( m_kRoomUserInfo.m_mapOngoingQuest.size() <= 0 )
		return false;
#else
	if( m_kRoomUserInfo.m_mapDropQuestItembyIngQuest.size() <= 0 )
		return false;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	std::map< int, KSubQuestInfo >::iterator mitQ;
	std::map< int, int >::iterator mitS;	//##arg 1. sub quest id | ##arg 2. quest item num

	//2���̻��� ����Ʈ�� �������ϼ��ְ�
	//����Ʈ �������� �����Ѵ� ���������� 2���̻��ϼ� �ִٴ� �����Ͽ� ����..
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	for( mitQ = m_kRoomUserInfo.m_mapOngoingQuest.begin(); mitQ != m_kRoomUserInfo.m_mapOngoingQuest.end(); ++mitQ )
#else
	for( mitQ = m_kRoomUserInfo.m_mapDropQuestItembyIngQuest.begin(); mitQ != m_kRoomUserInfo.m_mapDropQuestItembyIngQuest.end(); ++mitQ )
#endif SERV_QUEST_CLEAR_EXPAND
	//}}	
	{
		//{{ 2010. 10. 20	������	���� ����Ʈ ���� ���� üũ ����
#ifdef SERV_EPIC_QUEST_BUG_FIX
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitQ->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ����Ʈ�Դϴ�." )
				<< BUILD_LOG( mitQ->first )
				<< END_LOG;
			continue;
		}

		// ���� ����Ʈ �̸鼭 ĳ���� ���� ������ �������� ������ ������ ������� �ʴ´�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType  &&  pQuestTemplet->m_iPlayLevel > GetLevel() )
			continue;
#endif SERV_EPIC_QUEST_BUG_FIX
		//}}

		for( mitS = mitQ->second.m_mapSubQuestInfo.begin(); mitS != mitQ->second.m_mapSubQuestInfo.end(); ++mitS )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( mitS->first );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ �̻�.!" )
					<< BUILD_LOG( mitS->first )
					<< BUILD_LOG( mitS->second )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCID() )
#else
					<< BUILD_LOG( GetNickName() )
#endif SERV_PRIVACY_AGREEMENT
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// ������ ���� ����Ʈ�� üũ�Ѵ�!
			if( pSubTemplet->m_eClearType != CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
				continue;

			if( pSubTemplet->m_ClearCondition.m_eDungeonID != SEnum::DI_NONE &&
				pSubTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
			{
				continue;
			}

			//{{ 2009. 4. 1  ������		���̵� �̻�

			//{{ 2011. 08. 31  ��μ�	��д��� ����
#ifdef SERV_RANDOM_DAY_QUEST
			if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == true )
			{
				// �ش� ��д����� �´��� �˻�
				if( ( pSubTemplet->m_ClearCondition.m_eDungeonID != SEnum::DI_NONE ) && ( pSubTemplet->m_ClearCondition.m_eDungeonID / 10 ) != ( iDungeonID / 10 ) )
					continue;

				// ���̵� �˻�
				if( pSubTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID )
				{
					if( pSubTemplet->m_ClearCondition.m_bUpperDifficulty == false )
					{
						if( pSubTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode )
							continue;
					}
				}
			}
			else
#endif SERV_RANDOM_DAY_QUEST
			//}}				
			{
				// m_eDungeonID �� DI_NONE �� ��� �̺�Ʈ ����Ʈ �̴�.
				// ��ϸ�, �纥�� �����Ѵ�.
				if( pSubTemplet->m_ClearCondition.m_eDungeonID == SEnum::DI_NONE )
				{
					if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true || CXSLDungeon::IsRubenDungeon( iDungeonID ) == true )
					{
						continue;
					}	
				}

				if( pSubTemplet->m_ClearCondition.m_bUpperDifficulty == false )
				{
					if( pSubTemplet->m_ClearCondition.m_cDifficulty >= CXSLDungeon::DL_NORMAL &&
						pSubTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
					{
						continue;
					}
				}
				else
				{
					if( pSubTemplet->m_ClearCondition.m_cDifficulty >= CXSLDungeon::DL_NORMAL &&
						pSubTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
					{
						continue;
					}
				}
			}

			//}}

			//{{ 2009. 8. 3  ������		����Ʈ ���� ����
			if( pSubTemplet->m_ClearCondition.m_setKillNPCID.empty() == false  &&
				pSubTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNPCID) ) == pSubTemplet->m_ClearCondition.m_setKillNPCID.end() )
			{
				continue;
			}
			//}}

			if( pSubTemplet->m_ClearCondition.m_iCollectionItemNum <= mitS->second )
				continue;

			if( pSubTemplet->m_ClearCondition.m_iItemID <= 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			std::vector<int> vecUnderGroupSubQuest;
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pSubTemplet->m_iID, vecUnderGroupSubQuest ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			//{{ 2009. 10. 5  ������	0% Ȯ���ε� �������� ���ü� �ֳ�? ����� ��ġ��!
			float fDropRate = static_cast<float>( rand()%10000 ) + 1.f; //�⺻ Ȯ���� 10000/1 �� ��´�. ( 1 ~ 10000 ���� )
			fDropRate /= 100.0f;
			//}}

			float fQuestItemDropRate = pSubTemplet->m_ClearCondition.m_fQuestItemDropRate;

			//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
			if( bComeBackUserInRoom == true )
			{
				fQuestItemDropRate = fQuestItemDropRate * 2.0f;      // ��ӷ� 2�� ����
			}
#endif SERV_COME_BACK_USER_REWARD
			//}}

			//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_GAIA_QUEST_ITEM_DROP_EVENT
			else if( GetServerGroupIDThisUser() == SEnum::SGI_GAIA )
			{
				fQuestItemDropRate = fQuestItemDropRate * 1.5f;      // ���̾� ������ ������� ��ӷ� 1.5�� ����
			}
#endif SERV_GAIA_QUEST_ITEM_DROP_EVENT
			//}}

			if( fDropRate <= fQuestItemDropRate )
			{
				sInfo.m_vecDropQuestItem.push_back( pSubTemplet->m_ClearCondition.m_iItemID );

				++mitS->second;
			}
		}
	}

	if( sInfo.m_vecDropQuestItem.size() > 0 )
	{
		START_LOG( clog, L"����Ʈ ������ ���.!" )
			<< BUILD_LOG( sInfo.m_vecDropQuestItem.size() )
			;

		return true;
	}

	return false;
}
#endif SERV_REFORM_QUEST
//}}


//{{ 2012. 07. 16	������		��Ʋ�ʵ� ���� ����Ʈ
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
bool KRoomUser::GetQuestDropItemInBattleField( IN const int iBattleFieldID, IN const int iNpcID, IN const float fEventRate, OUT QUEST_ITEM_DROP_INFO& sInfo )
#else
bool KRoomUser::GetQuestDropItemInBattleField( IN const int iBattleFieldID, IN const int iNpcID, OUT QUEST_ITEM_DROP_INFO& sInfo )
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
{
	SET_ERROR( NET_OK );

	sInfo.m_iGSUID = GetGSUID();
	sInfo.m_iUnitUID = GetCID();
	sInfo.m_vecDropQuestItem.clear();

	// ���� �������� ����Ʈ�� �ִ��� Ȯ��!
	if( m_kRoomUserInfo.m_mapOngoingQuest.size() <= 0 )
		return false;

	std::map< int, KSubQuestInfo >::iterator mitQ;
//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	std::map< int, KSubQuestData >::iterator mitS;	//##arg 1. sub quest id | ##arg 2. quest item num
#else
	std::map< int, int >::iterator mitS;	//##arg 1. sub quest id | ##arg 2. quest item num
#endif SERV_REFORM_QUEST
//}}

	//2���̻��� ����Ʈ�� �������ϼ��ְ�
	//����Ʈ �������� �����Ѵ� ���������� 2���̻��ϼ� �ִٴ� �����Ͽ� ����..
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
	for( mitQ = m_kRoomUserInfo.m_mapOngoingQuest.begin(); mitQ != m_kRoomUserInfo.m_mapOngoingQuest.end(); ++mitQ )
	{
		//{{ 2010. 10. 20	������	���� ����Ʈ ���� ���� üũ ����
#ifdef SERV_EPIC_QUEST_BUG_FIX
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitQ->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ����Ʈ�Դϴ�." )
				<< BUILD_LOG( mitQ->first )
				<< END_LOG;
			continue;
		}

		// ���� ����Ʈ �̸鼭 ĳ���� ���� ������ �������� ������ ������ ������� �ʴ´�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType  &&  pQuestTemplet->m_iPlayLevel > GetLevel() )
			continue;
#endif SERV_EPIC_QUEST_BUG_FIX
		//}}

		for( mitS = mitQ->second.m_mapSubQuestInfo.begin(); mitS != mitQ->second.m_mapSubQuestInfo.end(); ++mitS )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( mitS->first );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ �̻�.!" )
					<< BUILD_LOG( mitS->first )
//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
					<< BUILD_LOG( mitS->second.m_iInInventoryItemCount )
					<< BUILD_LOG( mitS->second.m_bSussece )
#else
					<< BUILD_LOG( mitS->second )
#endif SERV_REFORM_QUEST
//}}
					<< BUILD_LOG( GetNickName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// ������ ���� ����Ʈ�� üũ�Ѵ�!
			if( pSubTemplet->m_eClearType != CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
				continue;

#ifdef SERV_NEW_YEAR_EVENT_2014
			if( pSubTemplet->m_iID == _CONST_SERV_NEW_YEAR_EVENT_2014_::iMaxLevelCertificationSubQuestID )
			{
				int iStandardMonsterLevel = SiCXSLBattleFieldManager()->GetStandardMonsterLevel( static_cast< SEnum::BATTLE_FIELD_ID >( iBattleFieldID ) );
				if( GetLevel() < iStandardMonsterLevel - 10 || GetLevel() > iStandardMonsterLevel + 10 )
					continue;
			}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_QUEST_DROP_ON_REASONABLE_LEVEL
			if( pSubTemplet->m_iID == _CONST_SERV_EVENT_QUEST_DROP_ON_REASONABLE_LEVEL::iMaxLevelCertificationSubQuestID )
			{
				int iStandardMonsterLevel = SiCXSLBattleFieldManager()->GetStandardMonsterLevel( static_cast< SEnum::BATTLE_FIELD_ID >( iBattleFieldID ) );
				if( GetLevel() < iStandardMonsterLevel - 10 || GetLevel() > iStandardMonsterLevel + 10 )
					continue;
			}
#endif SERV_EVENT_QUEST_DROP_ON_REASONABLE_LEVEL

#ifdef SERV_RNW_ELDRASIL_EVENT
			if( pSubTemplet->m_iID == _CONST_SERV_RNW_ELDRASIL_EVENT::iMaxLevelCertificationSubQuestID )
			{
				int iStandardMonsterLevel = SiCXSLBattleFieldManager()->GetStandardMonsterLevel( static_cast< SEnum::BATTLE_FIELD_ID >( iBattleFieldID ) );
				if( GetLevel() < iStandardMonsterLevel - 10 || GetLevel() > iStandardMonsterLevel + 10 )
					continue;
			}
#endif SERV_RNW_ELDRASIL_EVENT

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// Ư�� Field ������ �Ϸ� �����ϴٸ�
			if( pSubTemplet->m_ClearCondition.m_setBattleFieldID.size() > 0 )
			{
				// Field �˻�����
				std::set<int>::const_iterator sit = pSubTemplet->m_ClearCondition.m_setBattleFieldID.find( iBattleFieldID );
				if( sit == pSubTemplet->m_ClearCondition.m_setBattleFieldID.end() )
				{
					continue;							
				}
			}
#else
			if( pSubTemplet->m_ClearCondition.m_iBattleFieldID != SEnum::BFI_INVALID &&
				pSubTemplet->m_ClearCondition.m_iBattleFieldID != iBattleFieldID )
			{
				continue;
			}
#endif SERV_REFORM_QUEST
			//}}
		
			//{{ 2009. 8. 3  ������		����Ʈ ���� ����
			if( pSubTemplet->m_ClearCondition.m_setKillNPCID.empty() == false  &&
				pSubTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) ) == pSubTemplet->m_ClearCondition.m_setKillNPCID.end() )
			{
				continue;
			}
			//}}

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			if( pSubTemplet->m_ClearCondition.m_iCollectionItemNum <= mitS->second.m_iInInventoryItemCount )
				continue;

			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			std::vector<int> vecUnderGroupSubQuest;
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pSubTemplet->m_iID, vecUnderGroupSubQuest ) == false )
				continue;
#else
			if( pSubTemplet->m_ClearCondition.m_iCollectionItemNum <= mitS->second )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( pSubTemplet->m_ClearCondition.m_iItemID <= 0 )
				continue;

			//{{ 2009. 10. 5  ������	0% Ȯ���ε� �������� ���ü� �ֳ�? ����� ��ġ��!
			float fDropRate = static_cast<float>( rand()%10000 ) + 1.f; //�⺻ Ȯ���� 10000/1 �� ��´�. ( 1 ~ 10000 ���� )
			fDropRate /= 100.0f;
			//}}

			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			float fQuestItemDropRate = pSubTemplet->m_ClearCondition.m_fQuestItemDropRate * fEventRate;
#else
			float fQuestItemDropRate = pSubTemplet->m_ClearCondition.m_fQuestItemDropRate;
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}

			//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_GAIA_QUEST_ITEM_DROP_EVENT
			if( GetServerGroupIDThisUser() == SEnum::SGI_GAIA )
			{
				fQuestItemDropRate = fQuestItemDropRate * 1.5f;      // ���̾� ������ ������� ��ӷ� 1.5�� ����
			}
#endif SERV_GAIA_QUEST_ITEM_DROP_EVENT
			//}}

			if( fDropRate <= fQuestItemDropRate )
			{
				sInfo.m_vecDropQuestItem.push_back( pSubTemplet->m_ClearCondition.m_iItemID );

				//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
				++mitS->second.m_iInInventoryItemCount;
				if( pSubTemplet->m_ClearCondition.m_iCollectionItemNum <= mitS->second.m_iInInventoryItemCount )
					mitS->second.m_bSussece = true;
#else
				++mitS->second;
#endif SERV_REFORM_QUEST
				//}}
			}
		}
	}

	if( sInfo.m_vecDropQuestItem.size() > 0 )
	{
		START_LOG( clog, L"����Ʈ ������ ���.!" )
			<< BUILD_LOG( sInfo.m_vecDropQuestItem.size() )
			;

		return true;
	}

	return false;
}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//}}

bool KRoomUser::SetUnitQuestInvenInfo( std::map<int, KSubQuestInfo>& mapInvenInfo )
{
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	m_kRoomUserInfo.m_mapOngoingQuest = mapInvenInfo;
#else
	m_kRoomUserInfo.m_mapDropQuestItembyIngQuest = mapInvenInfo;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}	

	return true;
}

//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
void KRoomUser::ReserveEndPlay()
{
	m_bEndPlay			 = true;
	m_fDelayPacketTime	 = 15.0f;
	m_bCashContinueReady = false;
	m_tDelayPacketTimer.restart();
}

bool KRoomUser::CheckEndPlay()
{
	if( !m_bEndPlay )
		return false;
	
	if( m_bCashContinueReady )
	{
		if( m_tCashContinueTimer.elapsed() >= 11.0 )
			return true;
	}
	else
	{
		if( m_tDelayPacketTimer.elapsed() >= m_fDelayPacketTime ) // �⺻ 15��
			return true;
	}

	return false;
}

void KRoomUser::StopDungeonContinueTime( bool bIsStop )
{
	if( bIsStop )
	{
		// 1. ��Ƽ�� Ÿ�� ���� '������Ÿ�� = ������Ÿ�� - ����ð�'
		m_fDelayPacketTime -= m_tDelayPacketTimer.elapsed();

		// 2. �ǽð� ��Ȱ�� �غ�
		m_bCashContinueReady = true;

		// 3. �ǽð� ��Ȱ�� �غ� Ÿ�� üũ
		m_tCashContinueTimer.restart();
	}
	else
	{
		// 1. ��Ƽ�� Ÿ�� ���
		m_bCashContinueReady = false;

		// 2. ������ �ð��� �ʱ�ȭ
		m_tDelayPacketTimer.restart();
	}
}
//}}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

void KRoomUser::UpdatePetInfo( IN const std::vector< KPetInfo >& vecPetInfo )
{
	m_kRoomUserInfo.m_vecPet.clear();

	if( vecPetInfo.size() > 1 )
	{
		START_LOG( cerr, L"�� ������ 1������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;

		// �׳� ���!
	}

	m_kRoomUserInfo.m_vecPet = vecPetInfo;
}

bool KRoomUser::UpdatePetSatiety( IN UidType iPetUID, IN short sSatiety )
{
	if( m_kRoomUserInfo.m_vecPet.empty() == true )
	{
		START_LOG( cerr, L"�굵 ���µ� �������� ������Ʈ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;
		return false;
	}
	
	if( m_kRoomUserInfo.m_vecPet.size() > 1 )
	{
		START_LOG( cerr, L"�� ������ 1������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;

		// �׳� ���!
	}

    BOOST_TEST_FOREACH( KPetInfo&, kPet, m_kRoomUserInfo.m_vecPet )
	{
		if( kPet.m_iPetUID != iPetUID )
		{
			START_LOG( cerr, L"�� �������� ������Ʈ �Ϸ��� �ϴµ� PetUID���� �ٸ���?!" )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOG( iPetUID )
				<< END_LOG;
			return false;
		}

		// ������ ������Ʈ!
        kPet.m_sSatiety = sSatiety;
		break;
	}

	return true;
}

bool KRoomUser::UpdatePetIntimacy( IN UidType iPetUID, IN int iIntimacy )
{
	if( m_kRoomUserInfo.m_vecPet.empty() == true )
	{
		START_LOG( cerr, L"�굵 ���µ� �������� ������Ʈ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;
		return false;
	}

	if( m_kRoomUserInfo.m_vecPet.size() > 1 )
	{
		START_LOG( cerr, L"�� ������ 1������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;

		// �׳� ���!
	}

	BOOST_TEST_FOREACH( KPetInfo&, kPet, m_kRoomUserInfo.m_vecPet )
	{
		if( kPet.m_iPetUID != iPetUID )
		{
			START_LOG( cerr, L"�� ģ�е��� ������Ʈ �Ϸ��� �ϴµ� PetUID���� �ٸ���?!" )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOG( iPetUID )
				<< END_LOG;
			return false;
		}

		// ģ�е� ������Ʈ!
		kPet.m_iIntimacy = iIntimacy;
		break;
	}

	return true;
}

#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
int KRoomUser::GetPetID() const
#else //SERV_PETID_DATA_TYPE_CHANGE
char KRoomUser::GetPetID() const
#endif //SERV_PETID_DATA_TYPE_CHANGE
{
	if( m_kRoomUserInfo.m_vecPet.empty() == true )
	{
		return 0;
	}

	if( m_kRoomUserInfo.m_vecPet.size() > 1 )
	{
		START_LOG( cerr, L"�� ������ 1������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;

		// �׳� ���!
	}

	BOOST_TEST_FOREACH( const KPetInfo&, kPet, m_kRoomUserInfo.m_vecPet )
	{
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		return kPet.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
		return kPet.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	}

	return 0;
}

char KRoomUser::GetPetEvolutionStep() const
{
	if( m_kRoomUserInfo.m_vecPet.empty() == true )
	{
		return 0;
	}

	if( m_kRoomUserInfo.m_vecPet.size() > 1 )
	{
		START_LOG( cerr, L"�� ������ 1������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCID() )
			<< END_LOG;

		// �׳� ���!
	}

	BOOST_TEST_FOREACH( const KPetInfo&, kPet, m_kRoomUserInfo.m_vecPet )
	{
		return kPet.m_cEvolutionStep;
	}

	return 0;
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
void KRoomUser::GetOngoingQuest( IN int iSubQuestType, OUT std::map< int, KSubQuestInfo >& mapOngoingQuest ) const
{
	mapOngoingQuest.clear();

	std::map< int, KSubQuestInfo >::const_iterator mitQ;
	for( mitQ = m_kRoomUserInfo.m_mapOngoingQuest.begin(); mitQ != m_kRoomUserInfo.m_mapOngoingQuest.end(); ++mitQ )
	{
		const KSubQuestInfo& kSubQuestInfo = mitQ->second;
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		std::map< int, KSubQuestData >::const_iterator mitSQ;
#else
		std::map< int, int >::const_iterator mitSQ;
#endif SERV_REFORM_QUEST
		//}}

		bool bCorrectSubQuestType = false;
		for( mitSQ = kSubQuestInfo.m_mapSubQuestInfo.begin(); mitSQ != kSubQuestInfo.m_mapSubQuestInfo.end(); ++mitSQ )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( mitSQ->first );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ���� ����Ʈ �Դϴ�!" )
					<< BUILD_LOG( mitSQ->first )
					<< END_LOG;
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != iSubQuestType )
				continue;

			bCorrectSubQuestType = true;			
		}

		// �ش� ��������Ʈ Ÿ���� �´� ����Ʈ��� ����!
		if( bCorrectSubQuestType )
		{
			mapOngoingQuest.insert( std::make_pair( mitQ->first, kSubQuestInfo ) );
		}
	}
}

void KRoomUser::CheckItemEquipQuestCondition( IN bool bIsGameStart )
{
	std::map< int, KSubQuestInfo >::iterator mitQ;
	for( mitQ = m_kRoomUserInfo.m_mapOngoingQuest.begin(); mitQ != m_kRoomUserInfo.m_mapOngoingQuest.end(); ++mitQ )
	{
        KSubQuestInfo& kSubQuestInfo = mitQ->second;
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		std::map< int, KSubQuestData >::iterator mitSQ;
#else
		std::map< int, int >::iterator mitSQ;
#endif SERV_REFORM_QUEST
		//}}
		for( mitSQ = kSubQuestInfo.m_mapSubQuestInfo.begin(); mitSQ != kSubQuestInfo.m_mapSubQuestInfo.end(); ++mitSQ )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( mitSQ->first );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ���� ����Ʈ �Դϴ�!" )
					<< BUILD_LOG( mitSQ->first )
					<< END_LOG;
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR )
				continue;
			
			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			if( bIsGameStart )
			{
				// �ش� �������� �����ϰ� �ִٸ�?
				if( IsEquippedItem( pSubQuestTemplet->m_ClearCondition.m_iItemID ) == true )
				{
					mitSQ->second.m_iInInventoryItemCount = 1;
				}
				// �ش� �������� ���� ���ϰ� �ִٸ�?
				else
				{
					mitSQ->second.m_iInInventoryItemCount = 0;
				}
			}
			else
			{
				// ���� ���� ���
				if( mitSQ->second.m_iInInventoryItemCount == 1 )
				{
					// �ش� �������� ���� ���ϰ� �ִٸ�?
					if( IsEquippedItem( pSubQuestTemplet->m_ClearCondition.m_iItemID ) == false )
					{
						mitSQ->second.m_iInInventoryItemCount = 0;
					}
				}
			}
#else
			if( bIsGameStart )
			{
				// �ش� �������� �����ϰ� �ִٸ�?
				if( IsEquippedItem( pSubQuestTemplet->m_ClearCondition.m_iItemID ) == true )
				{
					mitSQ->second = 1;
				}
				// �ش� �������� ���� ���ϰ� �ִٸ�?
				else
				{
					mitSQ->second = 0;
				}
			}
			else
			{
				// ���� ���� ���
				if( mitSQ->second == 1 )
				{
					// �ش� �������� ���� ���ϰ� �ִٸ�?
					if( IsEquippedItem( pSubQuestTemplet->m_ClearCondition.m_iItemID ) == false )
					{
						mitSQ->second = 0;
					}
				}
			}
#endif SERV_REFORM_QUEST
			//}}
		}
	}
}

bool KRoomUser::IsEquippedItem( IN int iItemID ) const
{
	std::map< int, KInventoryItemInfo >::const_iterator mit;
	for( mit = m_kRoomUserInfo.m_mapEquippedItem.begin(); mit != m_kRoomUserInfo.m_mapEquippedItem.end(); ++mit )
	{
        if( iItemID == mit->second.m_kItemInfo.m_iItemID )
			return true;
	}

	return false;
}
#endif SERV_QUEST_CLEAR_EXPAND
//}}

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
bool KRoomUser::HasSpecialItem( IN int iItemID ) const
{
	std::map< int, KInventoryItemInfo >::const_iterator mit;
	for( mit = m_kRoomUserInfo.m_mapSpecialItem.begin(); mit != m_kRoomUserInfo.m_mapSpecialItem.end(); ++mit )
	{
		if( iItemID == mit->second.m_kItemInfo.m_iItemID )
			return true;
	}

	return false;
}
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
bool KRoomUser::AddAllyNPC( IN const KNPCUnitReq& kNPCUnit )
{
	m_mapAllyNPCUnit.insert( std::make_pair( kNPCUnit.m_UID, kNPCUnit ) );

	START_LOG( clog, L"ģ��NPC ��Ͽ� �߰��Ǿ����ϴ�." )
		<< BUILD_LOG( m_kRoomUserInfo.m_nUnitUID )
		<< BUILD_LOG( kNPCUnit.m_UID );
		;

	return true;
}

bool KRoomUser::IsAllyNPC( IN const int iNPCID )
{
	std::map< UidType, KNPCUnitReq >::iterator mitAllyNPCUnit;

	for( mitAllyNPCUnit = m_mapAllyNPCUnit.begin(); mitAllyNPCUnit != m_mapAllyNPCUnit.end(); ++mitAllyNPCUnit )
	{
		if( mitAllyNPCUnit->second.m_NPCID == iNPCID )
			return true;
	}

	return false;
}

void KRoomUser::DeleteAllyNPC( IN const UidType iNPCUID )
{
	std::map< UidType, KNPCUnitReq >::iterator mitAllyNPCUnit = m_mapAllyNPCUnit.find( iNPCUID );

	if( mitAllyNPCUnit != m_mapAllyNPCUnit.end() )
	{
		START_LOG( clog, L"ģ��NPC�� �׾����ϴ�." )
			<< BUILD_LOG( iNPCUID )
			<< BUILD_LOG( mitAllyNPCUnit->second.m_NPCID );
		;

		m_mapAllyNPCUnit.erase( mitAllyNPCUnit );
	}
}

#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

//{{ 2010. 12. 7	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT
void KRoomUser::GetAppropriateLevelInfo( OUT KUserAppropriateLevelInfo& kInfo )
{
    kInfo = m_kAppropriateLevelInfo;
	kInfo.m_iUnitUID = GetCID();
	kInfo.m_ucLevel = GetLevel();
}
#endif SERV_APPROPRIATE_LEVEL_STAT
//}}

//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
bool KRoomUser::AddCaughtBuffSkill( IN const int iBuffEnum, IN const float fBuffTime, IN const bool bBuffState, IN const UidType iCastingUnitUID, IN const char cBuffLevel )
{
	std::map< int, BUFF_SKILL_INFO >::iterator mit = m_mapBuff.find( iBuffEnum );
	if( mit != m_mapBuff.end() )
	{
		BUFF_SKILL_INFO _BuffInfo = mit->second;

		float Remaintime = _BuffInfo.m_fBuffTime - static_cast<float>(_BuffInfo.m_tBuffTimer.elapsed());
		if( Remaintime < 0.0f )
		{
			START_LOG( cerr, L"������ ���� �ð��� �̻��ϴ�!!" )
				<< BUILD_LOG( _BuffInfo.m_fBuffTime )
				<< BUILD_LOG( _BuffInfo.m_tBuffTimer.elapsed() )
				<< END_LOG;
			return false;
		}

		// ������ ��ų�� ��� ���� �ð��� ����� ����
		if( Remaintime < fBuffTime )
		{
			_BuffInfo.m_bBuffState		= bBuffState;
			_BuffInfo.m_cBuffLevel		= cBuffLevel;
			_BuffInfo.m_fBuffTime		= fBuffTime;
			_BuffInfo.m_iBuffEnum		= iBuffEnum;
			_BuffInfo.m_iCastingUnitUID = iCastingUnitUID;
			_BuffInfo.m_tBuffTimer.restart();

			mit->second = _BuffInfo;
			return true;
		}
		return false;
	}
	
	BUFF_SKILL_INFO _BuffInfo;
	_BuffInfo.m_iBuffEnum		= iBuffEnum;
	_BuffInfo.m_fBuffTime		= fBuffTime;
	_BuffInfo.m_bBuffState		= bBuffState;
	_BuffInfo.m_iCastingUnitUID	= iCastingUnitUID;
	_BuffInfo.m_cBuffLevel		= cBuffLevel;
	_BuffInfo.m_tBuffTimer.restart();

	m_mapBuff.insert( std::make_pair( iBuffEnum, _BuffInfo ) );
	
	return true;
}

bool KRoomUser::DeleteCaughtBuffSkill( IN const int iBuffEnum )
{
	std::map< int, BUFF_SKILL_INFO >::iterator mit = m_mapBuff.find( iBuffEnum );
	if( mit == m_mapBuff.end() )
	{
		return false;
	}
	
	m_mapBuff.erase( mit );
	
	return true;
}

bool KRoomUser::GetCaughtSpecificBuffSkill( IN const int iBuffEnum, OUT BUFF_SKILL_INFO& BuffInfo )
{
	std::map< int, BUFF_SKILL_INFO >::iterator mit = m_mapBuff.find( iBuffEnum );
	if( mit == m_mapBuff.end() )
	{
		return false;
	}

	BuffInfo = mit->second;
	return true;
}

bool KRoomUser::GetFinishBuffSkill( OUT std::vector< KBuffSkillInfo >& vecBuff )
{
	bool ret = false;

	vecBuff.clear();

	BUFF_SKILL_INFO tempBuff;
	KBuffSkillInfo tempBuffInfo;
	 
	if( m_mapBuff.empty() )
	{
		return false;
	}

	std::map< int, BUFF_SKILL_INFO >::iterator mit;
	for( mit = m_mapBuff.begin() ; mit != m_mapBuff.end() ; ++mit )
	{
		tempBuff = mit->second;
		if( tempBuff.m_bBuffState && tempBuff.m_tBuffTimer.elapsed() >= tempBuff.m_fBuffTime )
		{
			// Ÿ���� �� �� ���� ��ų�� �����Ѵٸ� ó�� �� ��
			tempBuffInfo.m_iBuffEnum		= tempBuff.m_iBuffEnum;
			tempBuffInfo.m_bIsBuffCasting	= false;
			tempBuffInfo.m_cBuffLevel		= tempBuff.m_cBuffLevel;
			tempBuffInfo.m_iCastingUnitUID	= tempBuff.m_iCastingUnitUID;
			tempBuffInfo.m_iCaughtUnitUID	= GetCID();

			vecBuff.push_back( tempBuffInfo );

			DeleteCaughtBuffSkill( tempBuff.m_iBuffEnum );

			mit = m_mapBuff.begin();	// ���� ���� �ּҰ� �ٲ�� ������.

			ret = true;
		}
	}
	return ret;
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
double KRoomUser::GetStagePlayTime()
{
	const double fElapsedStagePlayTime = m_kStagePlayTimer.elapsed();
	const double fStagePlayTime = fElapsedStagePlayTime - m_fLastStagePlayTime;
	m_fLastStagePlayTime = fElapsedStagePlayTime;

	return fStagePlayTime;
}
#endif SERV_DUNGEON_STAGE_LOG
//}}

//{{ 2012. 04. 21	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
void KRoomUser::UpdateGameStat( IN const KStat& kGameStat )
{
    m_kRoomUserInfo.m_kGameStat = kGameStat;
}

void KRoomUser::UpdateBuffInfo( IN const std::vector< KBuffInfo >& vecActivatedBuff )
{
	m_kRoomUserInfo.m_vecBuffInfo.clear();
	m_kRoomUserInfo.m_vecBuffInfo = vecActivatedBuff;

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	m_fAddQuestDropBouns = 0.f;
	BOOST_TEST_FOREACH( KBuffInfo, kBuff, m_kRoomUserInfo.m_vecBuffInfo )
	{
		// ���� ���� 3�ܰ��� ����Ʈ ��ӷ��� 50% �ø���.
		if( CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL == kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
			m_fAddQuestDropBouns += 0.5f;
			continue;
		}
		else if( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO == kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
			m_fAddQuestDropBouns += 0.3f;
			continue;
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

bool KRoomUser::ExistActiveBuff( IN const int iBuffID )
{
	BOOST_TEST_FOREACH( KBuffInfo, kBuff, m_kRoomUserInfo.m_vecBuffInfo )
	{
		if( iBuffID == kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
			return true;
		}
	}

	return false;
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
bool KRoomUser::CheckCompleteSubQuest_BeforGroup( IN int iQuestID, IN int iSubQuestID, OUT std::vector<int>& vecUnderGroupSubQuest )
{
	vecUnderGroupSubQuest.clear();

	// �׷��� �� ��������Ʈ �� �� �׷��� ��� Ŭ���� �Ǿ����� Ȯ���Ѵ�.
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"QuestTemplet ������ �̻�.!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		return false;
	}

	// ���� ���� ����Ʈ�� ���° �׷����� ����
	int iMainGroupID = 0;		// �׷��ȣ�� 1~ �����Ѵ�.
	std::map< int,std::vector<int> >::const_iterator mitGroup = pQuestTemplet->m_mapSubQuestGroup.begin();
	for( ; mitGroup != pQuestTemplet->m_mapSubQuestGroup.end() ; ++mitGroup )
	{
		BOOST_TEST_FOREACH( const int, iSub, mitGroup->second )
		{
			if( iSub == iSubQuestID )
			{
				iMainGroupID = mitGroup->first;
				break;
			}
		}

		if( iMainGroupID != 0 )
			break;
	}

	// �׷��� ã�� ���ߴ�
	if( iMainGroupID == 0 )
	{
		START_LOG( cwarn, L"���� ����Ʈ�� �׷� ������ ã�� ���ߴ�. " )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( iSubQuestID )
			<< END_LOG;

		return false;
	}


	// ã�Ҵٸ�...�� �׷���  subquest ���� ��� �Ϸ� �Ǿ����� Ȯ������
	std::map< int, KSubQuestInfo >::iterator mit = m_kRoomUserInfo.m_mapOngoingQuest.find( iQuestID );
	if( mit == m_kRoomUserInfo.m_mapOngoingQuest.end() )
	{
		return false;
	}

	std::map< int, KSubQuestData >::iterator mitSub = mit->second.m_mapSubQuestInfo.begin();
	for( ; mitSub != mit->second.m_mapSubQuestInfo.end() ; ++mitSub )
	{
		// ���� ���� ����Ʈ�� �˻翡�� ����
		if( iSubQuestID == mitSub->first )
			continue;

		// ���� ����Ʈ�� ���° �׷����� ����
		int iGroupID = 0;		// �׷��ȣ�� 1~ �����Ѵ�.
		std::map< int,std::vector<int> >::const_iterator mitGroup = pQuestTemplet->m_mapSubQuestGroup.begin();
		for( ; mitGroup != pQuestTemplet->m_mapSubQuestGroup.end() ; ++mitGroup )
		{
			BOOST_TEST_FOREACH( const int, iSub, mitGroup->second )
			{
				if( iSub == mitSub->first )
				{
					iGroupID = mitGroup->first;
					break;
				}
			}

			if( iGroupID != 0 )
				break;
		}

		// ���� ���� ����Ʈ ���� ���� �׷��� ���� ����Ʈ��� �Ϸ� �˻�����
		if( iGroupID != 0 && iGroupID < iMainGroupID )
		{
			if( mitSub->second.m_bSussece == false )
			{
				const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( mitSub->first );
				if( pSubQuestTemplet == NULL )
				{
					START_LOG( cerr, L"SubQuestTemplet ������ �̻�.!" )
						<< BUILD_LOG( mitSub->first )
						<< END_LOG;

					return false;
				}

				if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION 
				 || pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum > mitSub->second.m_iInInventoryItemCount )
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			// �Ϸ� �� sub quest ���
			vecUnderGroupSubQuest.push_back( mitGroup->first );
		}
	}

	return true;
}

bool KRoomUser::CheckFindNPCQuest( IN int iMapID, IN std::vector< int >& vecNpc, IN bool IsDungeon )
{
	if( m_kRoomUserInfo.m_mapOngoingQuest.empty() == true )
		return false;

	std::map< int, KSubQuestInfo >::iterator mit;
	for( mit = m_kRoomUserInfo.m_mapOngoingQuest.begin(); mit != m_kRoomUserInfo.m_mapOngoingQuest.end(); ++mit )
	{		
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( m_kRoomUserInfo.m_nUnitUID )
				<< END_LOG;

			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[uiIndexSub] )
					<< BUILD_LOG( m_kRoomUserInfo.m_nUnitUID )
					<< END_LOG;

				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_FIND_NPC )
				continue;


			// 3. �ش� ������ �´��� Ȯ��!
			if( IsDungeon == true )
			{
				// ������ �����Ѵٸ�
				if( pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], iMapID, pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == true )
						return true;
				}
			}
			// 3. �ش� ��Ʋ �ʵ尡 �´��� Ȯ��!
			else
			{
				// ��Ʋ �ʵ尡 �����Ѵٸ�
				if( pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], iMapID ) == true )
						return true;
				}
			}			
		}
	}
	return false;
}
#endif SERV_REFORM_QUEST
//}}

//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
void KRoomUser::UpdateUserCashShopInfo( IN bool bEnter )
{
	m_bEnterCashShopInBattleField = bEnter;
}
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
DWORD KRoomUser::GetPingScore()							
{ 
	if( m_bRecivedPingScore )
	{
		int iSize = static_cast<int>( m_vecPingScore.size() );

		DWORD dwTotalPingScore = 0;
		DWORD dwAvgPingScore = 0;

		BOOST_TEST_FOREACH( const DWORD, dwScore, m_vecPingScore )
		{
			dwTotalPingScore += dwScore;
		}

		dwAvgPingScore = static_cast<DWORD>(dwTotalPingScore / iSize);

		return dwAvgPingScore; 
	}
	else
		return 99999;
}
void KRoomUser::SetPingScore( DWORD dwPingScore )		
{ 
	m_bRecivedPingScore = true; 
	m_vecPingScore.push_back( dwPingScore ); 
}

void KRoomUser::ClearPingScore()
{
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    // ���� ping score ���� �� �� �������� ping score �� �ʹ� ������ ���� ������ �߸��ǰ� �ϴ� ���� ����
    if ( m_bRecivedPingScore == true )
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    {
	    DWORD dwAvgPingScore = GetPingScore();

	    m_vecPingScore.clear();
	    m_vecPingScore.push_back( dwAvgPingScore );
    }
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
void KRoomUser::SetPingScoreForForceHost( DWORD dwPingScore )
{
	m_vecPingScore.clear();
	
	SetPingScore( dwPingScore );
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK


