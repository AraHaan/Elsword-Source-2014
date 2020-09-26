#include "GSUser.h"
#include "NetError.h"

#include ".\userquestmanager.h"
#include "x2data/XSLItemManager.h"
#include "x2data/XSLSocketItem.h"
#include "ExpTable.h"
//{{ 2010. 06. 20  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	#include "ThreadStatisticsManager.h"
#else
	#include "StatisticsManager.h"
#endif SERV_STATISTICS_THREAD
//}}
#include "GameSysVal/GameSysVal.h"
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
#include "GameEvent/GameEventManager.h"
//}}
//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	#include "RoomListManager.h"
#endif SERV_INTEGRATION
//}}

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#include "ClassChangeTable.h"
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

KUserQuestManager::KUserQuestManager(void) :
m_iRewardItemCount( 0 )
{
}

KUserQuestManager::~KUserQuestManager(void)
{
	ClearUnitQuest();
}

//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
void KUserQuestManager::SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser )
#else
void KUserQuestManager::SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< int >& vecCompleteQuest, IN KGSUserPtr spUser )
#endif SERV_DAILY_QUEST
//}}
{
	ClearUnitQuest();

	//������ ����Ʈ �����ϱ�
	BOOST_TEST_FOREACH( const KQuestInstance&, kQuestInstance, vecQuest )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET // ����Ʈ ���ø� ����
			CTime kRegDate = CTime::GetCurrentTime();
			KE_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT kNot;
			kNot.m_iQuestID	= kQuestInstance.m_iID;
			kNot.m_wstrRegDate = (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
			KSIManager.QueueingEvent( E_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT, kNot );
			continue;
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET 

			START_LOG( cerr, L"QUEST TEMPLET�� ����." )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
				<< END_LOG;
			continue;
		}

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
		if ( SiKGameEventManager()->IsEnableEventQuest(kQuestInstance.m_iID) == false )
		{
			continue; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
		}
#endif //SERV_ALLOW_EVENT_ERASE

		KQuestInstance kQuest;
		kQuest.m_iID			= kQuestInstance.m_iID;
		kQuest.m_OwnorUnitUID	= kQuestInstance.m_OwnorUnitUID;

		//sub quest seting(�Ϸ��ߴ°��� �ƴѰ����� �ľ�)
		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			KSubQuestInstance kSub;

			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET�� ����." )
					<< BUILD_LOG( kQuest.m_iID )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[uiIndexSub] )
					<< BUILD_LOG( kQuest.m_OwnorUnitUID )
					<< END_LOG;

				continue;
			}

			kSub.m_iID			= pSubTemplet->m_iID;
			kSub.m_ucClearData	= kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData;

			switch( pSubTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_DUNGEON_TIME:
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
#else // SERV_UPGRADE_SKILL_SYSTEM_2013
/*
			case CXSLQuestManager::SQT_DUNGEON_RANK:
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			case CXSLQuestManager::SQT_NPC_TALK:
				//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
			case CXSLQuestManager::SQT_ITEM_ENCHANT:
			case CXSLQuestManager::SQT_ITEM_ATTRIB:
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
				//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			case CXSLQuestManager::SQT_VISIT_VILLAGE:
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}
				//{{ 2012. 07. 16	������		��Ʋ�ʵ� ���� ����Ʈ
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
			case CXSLQuestManager::SQT_VISIT_FIELD:
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
				//}}
				//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			case CXSLQuestManager::SQT_VISIT_DUNGEON:
			case CXSLQuestManager::SQT_FIND_NPC:
#endif SERV_REFORM_QUEST
					//}}
				{
					if( kSub.m_ucClearData != 0 )
						kSub.m_bIsSuccess	= true;
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_DAMAGE:
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData <= pSubTemplet->m_ClearCondition.m_iDungeonDamage )
						kSub.m_bIsSuccess = true;						
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
				//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
				//}}
				//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
			case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
				//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
				//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			case CXSLQuestManager::SQT_DUNGEON_RANK:
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iDungeonClearCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#ifdef SERV_SUB_QUEST_USE_ITEM
			case CXSLQuestManager::SQT_ITEM_USE:
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iUseItemNum )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_SUB_QUEST_USE_ITEM
			case CXSLQuestManager::SQT_NPC_HUNT:
				{
					if( kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iKillNum )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_ITEM_COLLECTION:
			case CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION:
				{
					if( kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iCollectionItemNum )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_PVP_PLAY:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPPlay )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_PVP_WIN:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPWin )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_PVP_KILL:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPKill )
						kSub.m_bIsSuccess = true;
				}
				break;

				//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
			case CXSLQuestManager::SQT_ITEM_SOCKET:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iSocketCount )
						kSub.m_bIsSuccess = true;
				}
				break;
			
			case CXSLQuestManager::SQT_ITEM_RESOLVE:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iResolveCount )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_USE_SKILL_POINT:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iSpUseCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
				//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
			case CXSLQuestManager::SQT_PVP_NPC_HUNT:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPWin )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_NEW_PVP_QUEST
				//}} 

#ifdef SERV_SKILL_USE_SUBQUEST
			case CXSLQuestManager::SQT_SKILL_USE:
				{
					if( kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iUseSkillCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_SKILL_USE_SUBQUEST

#ifdef SERV_POINT_COUNT_SYSTEM
			case CXSLQuestManager::SQT_POINT_COUNT:
				{
					if ( spUser != NULL )
					{			
						//���⿡ ���� ���� �޾� �ָ� �ȴ�.
						if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iPointCount )
							kSub.m_bIsSuccess = true;
					}
				}
				break;
#endif SERV_POINT_COUNT_SYSTEM
				//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
			case CXSLQuestManager::SQT_PVP_PLAY_ARRANGE:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPPlay )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_2012_PVP_SEASON2
				//}}

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
			case CXSLQuestManager::SQT_PVP_HERO_NPC_KILL:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPKill )
						kSub.m_bIsSuccess = true;
				}
				break;
			case CXSLQuestManager::SQT_PVP_HERO_NPC_PLAY:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPPlay )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND

#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
			case CXSLQuestManager::SQT_LEARN_NEW_SKILL:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iLearnNewSkillCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

			default:
				{
					START_LOG( cerr, L"���� ��������Ʈ Ÿ��." )
						<< BUILD_LOG( pSubTemplet->m_eClearType )
						<< BUILD_LOG( kQuest.m_iID )
						<< BUILD_LOG( kQuest.m_OwnorUnitUID )
						<< END_LOG;
				}
			}

			kQuest.m_vecSubQuestInstance.push_back( kSub );
		}

		m_mapQuesting[kQuest.m_iID] = kQuest;
	}

	//������ ��������Ʈ �����͸� ���� ����
	vecQuest.clear();
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		vecQuest.push_back( mit->second );
	}

	//�Ϸ� ����Ʈ �����ؼ� �ֱ�
	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecCompleteQuest )
	{
		std::map< int, KCompleteQuestInfo >::iterator mitQC = m_mapCompleteQuest.find( kInfo.m_iQuestID );
		if( mitQC == m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest.insert( std::make_pair( kInfo.m_iQuestID, kInfo ) );
		}
		else
		{
			++(mitQC->second.m_iCompleteCount);
		}
	}
//#else
//	for( int i = 0; i < (int)vecCompleteQuest.size(); ++i )
//	{
//		const int iSubQuestID = vecCompleteQuest[i];
//
//		std::map< int, int >::iterator mitQC = m_mapCompleteQuest.find( iSubQuestID );
//		if( mitQC == m_mapCompleteQuest.end() )
//		{
//			m_mapCompleteQuest[iSubQuestID] = 1;
//		}
//		else
//		{
//			++(mitQC->second);
//		}
//	}
#endif SERV_DAILY_QUEST
	//}}

	// DB�κ��� ������ �Ϸ�� ����Ʈ ����Ʈ�� ������ After Quest�� open������ üũ�Ѵ�!
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecCompleteQuest )
	{
		CheckEpicQuest_NewQuestByAfterQuest( kInfo.m_iQuestID, spUser, false, false );
	}

	// Before Quest�� open������ üũ�Ѵ�!
	CheckEpicQuest_NewQuestByBeforeQuest( spUser, false );
}

bool KUserQuestManager::AddQuest( IN KQuestInstance& kInfo )
{
	SET_ERROR( NET_OK );

	if( IsQuest( kInfo.m_iID ) == true )
	{
		START_LOG( cerr, L"������ QUEST ������.. �� �ǿԳ�..?" )
			<< BUILD_LOG( kInfo.m_iID )
			<< BUILD_LOG( kInfo.m_OwnorUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_01 );

		return false;
	}

	m_mapQuesting[kInfo.m_iID] = kInfo;
	return true;
}

bool KUserQuestManager::RemoveQuest( IN int iQuestID )
{
	SET_ERROR( NET_OK );

	std::map< int, KQuestInstance >::iterator mit;
	mit = m_mapQuesting.find( iQuestID );
	if( mit == m_mapQuesting.end() )
	{
		START_LOG( cerr, L"���°� ����� .��~!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );

		return false;
	}

	m_mapQuesting.erase( mit );
	return true;
}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
bool KUserQuestManager::Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool IsAfterQuest/*= true*/, IN const bool bSendAck /*= true*/ )
{
	SET_ERROR( NET_OK );

	KEGS_NEW_QUEST_ACK	kPacket;

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ����Ʈ.!" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// PC�� ���� ����Ʈ���� üũ
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_16 );
			goto error_proc;
		}
	}

	// �̺�Ʈ ����Ʈ��� ���� �������� �̺�Ʈ���� Ȯ���Ѵ�.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// ���� �������� �̺�Ʈ���� Ȯ��!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	int iRecruitEventQuestIDForNewUserSize = sizeof( arriRecruitEventQuestIDForNewUser ) / sizeof( arriRecruitEventQuestIDForNewUser[0] );
	for( int iRecruitEventQuestIDForNewUserIndex = 0; iRecruitEventQuestIDForNewUserIndex < iRecruitEventQuestIDForNewUserSize; ++iRecruitEventQuestIDForNewUserIndex )
	{
		// ��õ���� �� ���� ����Ʈ���� Ȯ��
		if( arriRecruitEventQuestIDForNewUser[iRecruitEventQuestIDForNewUserIndex] == kReq.m_iQuestID )
		{
			// ��õ���� �ڰ� �ƴ϶�� ����Ʈ�� �� �޴´�!
			if( spUser->IsRecruit() == false )
			{
				SET_ERROR( ERR_QUEST_21 );
				goto error_proc;
			}

			break;
		}
	}
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

	//# ���� ����Ʈ �̸� �ִ���� ������ ������ ���� �ʴ´�.
	//# �̺�Ʈ ����Ʈ �̸� �ִ���� ������ ������ ���� �ʴ´�.
	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType
	 && CXSLQuestManager::QT_EVENT != pQuestTemplet->m_eQuestType )
	{
		if( GetNormalQuestSize() >= MAX_ING_QUEST_NUM )
		{
			SET_ERROR( ERR_QUEST_28 );
			goto error_proc;
		}
	}

	//�̹� �ϰ� �ִ� ����Ʈ����..
	if( IsQuest( kReq.m_iQuestID ) == true )
	{
		START_LOG( cwarn, L"�̹� ������ �ִ� ����Ʈ�� ����Ϸ���" )
			<< BUILD_LOG( kReq.m_iQuestID );

		SET_ERROR( ERR_QUEST_01 );
		goto error_proc;
	}

	//�������ϰ�� �ٸ� ��������Ʈ�� �ϰ������� �ʴ���..
	if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
	{
		if( IsChangeJobQuest() == true )
		{
			START_LOG( cerr, L"�������� �ϰ��ִµ� ���ο� ��������Ʈ�� ���� �ҷ�����.!" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_17 );
			goto error_proc;
		}
	}

	//�ݺ��������� ���� QUEST�� ���
	// �Ϲ� ����Ʈ
	if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
	{
		if( GetQuestCompleteCount( kReq.m_iQuestID ) > 0 )
		{
			START_LOG( cwarn, L"�̹� Ŭ������ ����Ʈ.!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_04 );
			goto error_proc;
		}
	}
	// �Ϸ� ���� ����Ʈ
	else if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
	{
		// �̹� �Ϸ��� ����Ʈ �ΰ�?
		KCompleteQuestInfo* pCompleteQuestInfo = GetCompleteQuestInfo( kReq.m_iQuestID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}

			// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
			{
				// 6�ú��� �Ѵ� �ð��̶�� �������� ����
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// ���� ���� �����Ѱ���?
			if( tCheckTime > tCurTime )
			{
				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}
#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
			BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
			{
				if( CheckCompletedQuestToday( iBeforeQuestID ) == false )
				{
					START_LOG( clog, L"���� ���� ����Ʈ���� �Ϸ� ������" )
						<< BUILD_LOG( kReq.m_iQuestID )
						<< BUILD_LOG( iBeforeQuestID )
						;

					SET_ERROR( ERR_QUEST_06 );
					goto error_proc;
				}
			}
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX	
		}
	}

	//��� ������ �Ҽ��ִ� ����Ʈ�� �ƴҰ��

	//#1. ���� �׷��� �Ҽ��ִ� ���.
	if( pQuestTemplet->m_Condition.m_eUnitType != CXSLUnit::UT_NONE )
	{
		if( pQuestTemplet->m_Condition.m_eUnitType != spUser->GetUnitType() )
		{
			START_LOG( cerr, L"QUEST ���డ�� ������ �ƴ�.! ������.. ��." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_eUnitType )
				<< BUILD_LOGc( spUser->GetUnitType() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	//#2. Ư�������� �Ҽ��ִ� ���.
	if( pQuestTemplet->m_Condition.m_setUnitClass.empty() == false )
	{
		//���ְ� ���� �ʴ� ����Ʈ�� üũ�Ͽ� �����Ҽ� ����.
		if( pQuestTemplet->m_Condition.m_setUnitClass.find( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == pQuestTemplet->m_Condition.m_setUnitClass.end() )
		{
			START_LOG( cerr, L"QUEST ���డ�� ������ �ƴ�.! ������.. ��." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_setUnitClass.size() )
				<< BUILD_LOGc( spUser->GetUnitClass() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	// To do...
	//[ing]QUEST ������ ���� ��ƾ��� ������ üũ
	//if( pQuestTemplet->m_Condition.m_iConditionItemID)

	//QUEST ������ ���� ������ �Ϸ��ؾ��� ����Ʈ �˻�.
	BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
	{
		if( GetQuestCompleteCount( iBeforeQuestID ) == 0 )
		{
			START_LOG( clog, L"����� �ϳ� ���ؿ���.. !!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iBeforeQuestID )
				;

			SET_ERROR( ERR_QUEST_06 );
			goto error_proc;
		}
	}

	// �������� ���� ��û�ϴ� ����Ʈ�� �˻����� �ʵ�������
	if( IsAfterQuest == false )
	{
		if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_eStartNPCID != kReq.m_iTalkNPCID &&
			kReq.m_iTalkNPCID != CXSLUnitManager::NUI_BILLBOARD
			)
		{
			START_LOG( cerr, L"�߸��� NPC���� QUEST�� �޾���..����..?" )
				<< BUILD_LOG( pQuestTemplet->m_eStartNPCID )
				<< BUILD_LOG( kReq.m_iTalkNPCID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_07 );
			goto error_proc;
		}
	}

	// ���� �˻�
	// ����Ʈ�� �ޱ� ���� �����̶��
	if( pQuestTemplet->m_Condition.m_iLevel	> spUser->GetLevel() )
	{
		START_LOG( clog, L"���� �� �� �÷�����..�Ѥ�^" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_12 );
		goto error_proc;
	}
	// ����Ʈ�� �ޱ� ���� �����̶��
	//	m_iLimitLevel �� -1 �̸� �������� �ʴ´ٴ� �ǹ�
	if( pQuestTemplet->m_Condition.m_iLimitLevel != -1 && pQuestTemplet->m_Condition.m_iLimitLevel < spUser->GetLevel() )
	{
		START_LOG( clog, L"���� �� ���ݾ�..�Ѥ�^" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLimitLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_27 );
		goto error_proc;
	}

	// ���� ���� ����Ʈ �ΰ�?
	if( pQuestTemplet->m_iRandomGroupID > 0 )
	{
		int iTodayQuest = SiCXSLQuestManager()->GetTodayRandomQuest( pQuestTemplet->m_iRandomGroupID );

		if( iTodayQuest <= 0 || pQuestTemplet->m_iID != iTodayQuest )
		{
			START_LOG( clog, L"���� �� �� �ִ� ���� ���� ����Ʈ�� �ƴϴ�!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( pQuestTemplet->m_iRandomGroupID )
				<< BUILD_LOGc( iTodayQuest )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_26 ); 
			goto error_proc;
		}
	}

	{
		KDBE_NEW_QUEST_REQ kDBReq;

		if( pQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// ����Ʈ ���� ������ �κ��丮�� ���� ������ ����Ʈ ���� ����
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"����Ʈ ������ �޴� ������ �̹� �κ��丮�� ������ ����Ʈ ���� ����." )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_24 );
					goto error_proc;
				}
			}

			// ����Ʈ ���� ������ ������ �󽽷��� �ִ��� �˻�
			if( spUser->IsEnoughSpaceExist( pQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"����Ʈ ������ �޴� ������ �� �κ��丮 ������� �����մϴ�." )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_25 );
				goto error_proc;
			}

			std::map< int, KItemInfo > mapReward;

			std::map< int, int >::const_iterator mit;
			for( mit = pQuestTemplet->m_mapAcceptRewardItem.begin(); mit != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mit )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"������ ���ø� ������ �̻�.!" )
						<< BUILD_LOG( mit->first )
						<< END_LOG;
					continue;
				}

				KItemInfo kNewItem;
				kNewItem.m_iItemID = mit->first;
				kNewItem.m_iQuantity = mit->second;
				kNewItem.m_cUsageType = pItemTemplet->m_PeriodType;
				mapReward.insert( std::make_pair( mit->first, kNewItem ) );

				// ��� : Local Item
				{
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( mit->first );
					KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_QUEST, mit->second );
					KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_QUEST, mit->second );

				}

				// ���� ��� ���� ������
				spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, mit->second );

				// ������ �����
				spUser->CheckItemAbuser( KAbuserLogManager::RS_QUEST, kDBReq.m_mapInsertedItem );
			}

			LIF( spUser->m_kInventory.PrepareInsert( mapReward, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo ) );

			// DB������Ʈ
			spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
			spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
		}

		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_iTalkNPCID	= kReq.m_iTalkNPCID;
		//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		// �Ⱦ��� �������� �׷��� �ʱ�ȭ������ ���ش�.
		kDBReq.m_UserUID	= spUser->GetUID();
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		spUser->SendToGameDB( DBE_NEW_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  ������  DB ��� ����Ʈ
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Accept, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_AcceptLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  ������  ���� ���
		int iColumnIndex;
		if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
		{
			iColumnIndex = KUserStatistics::US_Etc_NQuestAccept;
		}
		else
		{
			iColumnIndex = KUserStatistics::US_Etc_RQuestAccept;
		}

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return true;

error_proc:
	kPacket.m_iOK	= NetError::GetLastError();

	if( bSendAck )
	{
		spUser->SendPacket( EGS_NEW_QUEST_ACK, kPacket );
	}

	return false;
}

#else					// SERV_REFORM_QUEST

//{{ 2011. 01. 03	������	���� ����Ʈ
//#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
bool KUserQuestManager::Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool bSendAck /*= true*/ )
//#else
//bool KUserQuestManager::Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser )
//#endif SERV_EPIC_QUEST_CHECK_UPDATE
//}}
{
	SET_ERROR( NET_OK );

	KEGS_NEW_QUEST_ACK	kPacket;

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ����Ʈ.!" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2010. 01. 29  ������	PC�� ���� ����Ʈ
#ifdef SERV_PC_BANG_QUEST
	//////////////////////////////////////////////////////////////////////////	

	// PC�� ���� ����Ʈ���� üũ
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_16 );
			goto error_proc;
		}
	}

	//////////////////////////////////////////////////////////////////////////
#endif SERV_PC_BANG_QUEST
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

	// �̺�Ʈ ����Ʈ��� ���� �������� �̺�Ʈ���� Ȯ���Ѵ�.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// ���� �������� �̺�Ʈ���� Ȯ��!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

#endif SERV_DAY_QUEST
	//}}
	//////////////////////////////////////////////////////////////////////////	

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	int iRecruitEventQuestIDForNewUserSize = sizeof( arriRecruitEventQuestIDForNewUser ) / sizeof( arriRecruitEventQuestIDForNewUser[0] );
	for( int iRecruitEventQuestIDForNewUserIndex = 0; iRecruitEventQuestIDForNewUserIndex < iRecruitEventQuestIDForNewUserSize; ++iRecruitEventQuestIDForNewUserIndex )
	{
		// ��õ���� �� ���� ����Ʈ���� Ȯ��
		if( arriRecruitEventQuestIDForNewUser[iRecruitEventQuestIDForNewUserIndex] == kReq.m_iQuestID )
		{
			// ��õ���� �ڰ� �ƴ϶�� ����Ʈ�� �� �޴´�!
			if( spUser->IsRecruit() == false )
			{
				SET_ERROR( ERR_QUEST_21 );
				goto error_proc;
			}
			break;
		}
	}
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

	//����Ʈ�� �ѹ��� �����Ҽ��ִ� �ִ밳��
#ifdef SERV_EPIC_QUEST
	//# ��������Ʈ �̸� �ִ���� ������ ������ ���� �ʴ´�.
	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType )
	{
		if( GetNormalQuestSize() >= MAX_ING_QUEST_NUM )
		{
			SET_ERROR( ERR_QUEST_28 );
			goto error_proc;
		}
	}
#else SERV_EPIC_QUEST
	if( m_mapQuesting.size() >= MAX_ING_QUEST_NUM )
	{
		SET_ERROR( ERR_QUEST_28 );
		goto error_proc;
	}
#endif SERV_EPIC_QUEST

	//�̹� �ϰ� �ִ� ����Ʈ����..
	if( IsQuest( kReq.m_iQuestID ) == true )
	{
		START_LOG( cwarn, L"�̹� ������ �ִ� ����Ʈ�� ����Ϸ���" )
			<< BUILD_LOG( kReq.m_iQuestID );

		SET_ERROR( ERR_QUEST_01 );
		goto error_proc;
	}

	//�������ϰ�� �ٸ� ��������Ʈ�� �ϰ������� �ʴ���..
	if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
	{
		if( IsChangeJobQuest() == true )
		{
			START_LOG( cerr, L"�������� �ϰ��ִµ� ���ο� ��������Ʈ�� ���� �ҷ�����.!" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_17 );
			goto error_proc;
		}
	}

	//�ݺ��������� ���� QUEST�� ���
	//{{ 2010. 04. 01  ������	��д��� ����
#ifdef SERV_DAILY_QUEST
	// �Ϲ� ����Ʈ
	if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
	{
		if( GetQuestCompleteCount( kReq.m_iQuestID ) > 0 )
		{
			START_LOG( cwarn, L"�̹� Ŭ������ ����Ʈ.!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_04 );
			goto error_proc;
		}
	}
	// �Ϸ� ���� ����Ʈ
	else if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
	{
		// �̹� �Ϸ��� ����Ʈ �ΰ�?
		KCompleteQuestInfo* pCompleteQuestInfo = GetCompleteQuestInfo( kReq.m_iQuestID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}

			// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
			{
				// 6�ú��� �Ѵ� �ð��̶�� �������� ����
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// ���� ���� �����Ѱ���?
			if( tCheckTime > tCurTime )
			{
				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}
		}
	}
#else
	if( pQuestTemplet->m_bRepeat == false )
	{
		if( GetQuestCompleteCount( kReq.m_iQuestID ) > 0 )
		{
			START_LOG( cwarn, L"�̹� Ŭ������ ����Ʈ.!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_04 );
			goto error_proc;
		}
	}
#endif SERV_DAILY_QUEST
	//}}	

	//��� ������ �Ҽ��ִ� ����Ʈ�� �ƴҰ��

	//#1. ���� �׷��� �Ҽ��ִ� ���.
	if( pQuestTemplet->m_Condition.m_eUnitType != CXSLUnit::UT_NONE )
	{
		if( pQuestTemplet->m_Condition.m_eUnitType != spUser->GetUnitType() )
		{
			START_LOG( cerr, L"QUEST ���డ�� ������ �ƴ�.! ������.. ��." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_eUnitType )
				<< BUILD_LOGc( spUser->GetUnitType() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	//#2. Ư�������� �Ҽ��ִ� ���.
	if( pQuestTemplet->m_Condition.m_setUnitClass.empty() == false )
	{
		//���ְ� ���� �ʴ� ����Ʈ�� üũ�Ͽ� �����Ҽ� ����.
		if( pQuestTemplet->m_Condition.m_setUnitClass.find( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == pQuestTemplet->m_Condition.m_setUnitClass.end() )
		{
			START_LOG( cerr, L"QUEST ���డ�� ������ �ƴ�.! ������.. ��." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_setUnitClass.size() )
				<< BUILD_LOGc( spUser->GetUnitClass() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	// To do...
	//[ing]QUEST ������ ���� ��ƾ��� ������ üũ
	//if( pQuestTemplet->m_Condition.m_iConditionItemID)

	//QUEST ������ ���� ������ �Ϸ��ؾ��� ����Ʈ �˻�.
	BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
	{
		if( GetQuestCompleteCount( iBeforeQuestID ) == 0 )
		{
			START_LOG( clog, L"����� �ϳ� ���ؿ���.. !!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iBeforeQuestID )
				;

			SET_ERROR( ERR_QUEST_06 );
			goto error_proc;
		}
	}

	//������ npc���� ����Ʈ�� �޾ƾ���
#ifdef SERV_EPIC_QUEST
	//# ������ �ڵ������̶� ������ npc�� �������� ����.
	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_eStartNPCID != kReq.m_iTalkNPCID &&
		kReq.m_iTalkNPCID != CXSLUnitManager::NUI_BILLBOARD
		)
	{
		START_LOG( cerr, L"�߸��� NPC���� QUEST�� �޾���..����..?" )
			<< BUILD_LOG( pQuestTemplet->m_eStartNPCID )
			<< BUILD_LOG( kReq.m_iTalkNPCID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_07 );
		goto error_proc;
	}
#else SERV_EPIC_QUEST
	if( pQuestTemplet->m_eStartNPCID != kReq.m_iTalkNPCID )
	{
		START_LOG( cerr, L"�߸��� NPC���� QUEST�� �޾���..����..?" )
			<< BUILD_LOG( pQuestTemplet->m_eStartNPCID )
			<< BUILD_LOG( kReq.m_iTalkNPCID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_07 );

		goto error_proc;
	}
#endif SERV_EPIC_QUEST

	//���� �˻�
	if( pQuestTemplet->m_Condition.m_iLevel	> spUser->GetLevel() )
	{
		START_LOG( clog, L"���� �� �� �÷�����..�Ѥ�^" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_12 );
		goto error_proc;
	}

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	// ���� ���� ����Ʈ �ΰ�?
	if( pQuestTemplet->m_iRandomGroupID > 0 )
	{
		int iTodayQuest = SiCXSLQuestManager()->GetTodayRandomQuest( pQuestTemplet->m_iRandomGroupID );

		if( iTodayQuest <= 0 || pQuestTemplet->m_iID != iTodayQuest )
		{
			START_LOG( clog, L"���� �� �� �ִ� ���� ���� ����Ʈ�� �ƴϴ�!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( pQuestTemplet->m_iRandomGroupID )
				<< BUILD_LOGc( iTodayQuest )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_26 ); 
			goto error_proc;
		}
	}
#endif SERV_RANDOM_DAY_QUEST
	//}}


	{
		KDBE_NEW_QUEST_REQ kDBReq;

		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		if( pQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// ����Ʈ ���� ������ �κ��丮�� ���� ������ ����Ʈ ���� ����
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"����Ʈ ������ �޴� ������ �̹� �κ��丮�� ������ ����Ʈ ���� ����." )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_24 );
					goto error_proc;
				}
			}

			// ����Ʈ ���� ������ ������ �󽽷��� �ִ��� �˻�
			if( spUser->IsEnoughSpaceExist( pQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"����Ʈ ������ �޴� ������ �� �κ��丮 ������� �����մϴ�." )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_25 );
				goto error_proc;
			}

			std::map< int, KItemInfo > mapReward;

			std::map< int, int >::const_iterator mit;
			for( mit = pQuestTemplet->m_mapAcceptRewardItem.begin(); mit != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mit )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"������ ���ø� ������ �̻�.!" )
						<< BUILD_LOG( mit->first )
						<< END_LOG;
					continue;
				}

				KItemInfo kNewItem;
				kNewItem.m_iItemID = mit->first;
				kNewItem.m_iQuantity = mit->second;
				kNewItem.m_cUsageType = pItemTemplet->m_PeriodType;
				mapReward.insert( std::make_pair( mit->first, kNewItem ) );

				// ��� : Local Item
				{
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( mit->first );
					KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_QUEST, mit->second );
					//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
					KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_QUEST, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
					//}}
				}

				// ���� ��� ���� ������
				spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, mit->second );

				//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
				// ������ �����
				spUser->CheckItemAbuser( KAbuserLogManager::RS_QUEST, kDBReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
				//}}
			}

			LIF( spUser->m_kInventory.PrepareInsert( mapReward, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo ) );

			// DB������Ʈ
			spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
			spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
		}
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_iTalkNPCID	= kReq.m_iTalkNPCID;
		//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		// �Ⱦ��� �������� �׷��� �ʱ�ȭ������ ���ش�.
		kDBReq.m_UserUID	= spUser->GetUserUID();
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		spUser->SendToGameDB( DBE_NEW_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  ������  DB ��� ����Ʈ
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Accept, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_AcceptLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  ������  ���� ���
		int iColumnIndex;
		//{{ 2010. 04. 01  ������	��д��� ����
#ifdef SERV_DAILY_QUEST
		if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
#else
		if( pQuestTemplet->m_bRepeat == false )
#endif SERV_DAILY_QUEST
			//}}
		{
			iColumnIndex = KUserStatistics::US_Etc_NQuestAccept;
		}
		else
		{
			iColumnIndex = KUserStatistics::US_Etc_RQuestAccept;
		}

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return true;

error_proc:
	kPacket.m_iOK	= NetError::GetLastError();

	//{{ 2011. 01. 03	������	���� ����Ʈ
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	if( bSendAck )
#endif SERV_EPIC_QUEST_CHECK_UPDATE
		//}}
	{
		spUser->SendPacket( EGS_NEW_QUEST_ACK, kPacket );
	}

	return false;
}
#endif SERV_REFORM_QUEST
//}}

void KUserQuestManager::Handler_EGS_GIVE_UP_QUEST_REQ( IN KEGS_GIVE_UP_QUEST_REQ& kReq, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	//�������� ����Ʈ�� ���µ� �����ҷ��� �Ұ��
	if( IsQuest( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_02 );
		goto error_proc;
	}

	{
		KDBE_GIVE_UP_QUEST_REQ kDBReq;
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		spUser->SendToGameDB( DBE_GIVE_UP_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  ������  DB ��� ����Ʈ
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_GiveUp, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_GiveUpLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  ������  ���� ���
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ����ƮID�Դϴ�." )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;
			return; // ��谪�� ó������ �ʰ� ����
		}

		int iColumnIndex;
		//{{ 2010. 04. 01  ������	��д��� ����
#ifdef SERV_DAILY_QUEST
		if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
#else
		if( pQuestTemplet->m_bRepeat == false )
#endif SERV_DAILY_QUEST
		//}}
		{
			iColumnIndex = KUserStatistics::US_Etc_NQuestGiveUp;
		}
		else
		{
			iColumnIndex = KUserStatistics::US_Etc_RQuestGiveUp;
		}

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return;

error_proc:
	KEGS_GIVE_UP_QUEST_ACK kAck;
	kAck.m_iOK = NetError::GetLastError();
	spUser->SendPacket( EGS_GIVE_UP_QUEST_ACK, kAck );
}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::Handler_ERM_NPC_UNIT_DIE_NOT( IN const int iDungeonID,
													 IN const char cDifficulty,
													 IN const int iMonsterID,
													 IN KGSUserPtr spUser,
													 IN const char cDungeonMode
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
													 , IN const bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST
													 )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() )
			continue;

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType 
		 &&	pQuestTemplet->m_iPlayLevel > spUser->GetLevel()	)
			continue;

		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_NPC_HUNT )
				continue;

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
			if( pSubQuestTemplet->m_iID == PLAY_TOGETHER_KILL_FIELD_BOSS_SUBQUEST_ID && bHasFriend == false )
			{
				continue;
			}
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
			// ���̵� �� Ÿ�� �ʵ庸�� 3���� ���
			if( pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID1)
			{
				// ���̵� �� Ÿ�� ������
				int iOnRidingPet = spUser->GetiRidingPetSummoned();

				//START_LOG( clog2, L"�輮��_���̵��� �̺�Ʈ ���� ����Ʈ2!" )
				//	<<BUILD_LOG( iOnRidingPet )
				//	<<BUILD_LOG( pSubQuestTemplet->m_iID )
				//	<<END_LOG;

				// �̺�Ʈ�� ���̵� �� Ÿ�� ���� ������
				if(iOnRidingPet != 2)
					continue;
			}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

			//{{ 2009. 8. 3  ������		����Ʈ ���� ����
			if( pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iMonsterID) ) == pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.end() )
				continue;
			//}}

			if( spUser->GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( spUser->GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )  // ��Ʋ �ʵ���
			{
				// ��ȿ�� ��Ʋ �ʵ���
				if( SiCXSLQuestManager()->CheckValidBattleFiledID( pQuestTemplet->m_vecSubQuest[iIndexSub] ) == true ) 
				{
					// SubQuest �� �ִ� ��Ʋ �ʵ��� �÷��� �� ���� �ƴ϶��
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[iIndexSub], spUser->GetMapID() ) == false )
					{
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
						if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
						{
							START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: ��Ʈ������ ��Ҵµ� ���� �ʵ尡 �ƴϴ�?" )
								<< BUILD_LOG( spUser->GetCharUID() )
								<< BUILD_LOG( spUser->GetCharName() )
								<< BUILD_LOG( spUser->GetRoomUID() )
								<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
								<< BUILD_LOG( spUser->GetMapID() )
								<< END_LOG;
						}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
						continue;
					}
				}
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
				else if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: ��Ʈ������ ��Ҵµ� ���� �ʵ尡 �ƴϴ�?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
			}
			else
			{
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
				if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: ��Ʈ������ ��Ҵµ� ���� �ʵ尡 �ƴϴ�?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
				if( ( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID ) && ( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode ) )
					continue;
				
				// ��ȿ�� �����̶��
				if( SiCXSLQuestManager()->CheckValidDungeonID( pQuestTemplet->m_vecSubQuest[iIndexSub] ) == true )
				{
					// SubQuest �� �ִ� ������ �÷��� �� ���� �ƴ϶��
					if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[iIndexSub], iDungeonID + static_cast<int>(cDifficulty), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
						continue;
				}
			}
			
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == false )
			{
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
				if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: ��Ʈ������ ��Ҵµ� ���� �׷� sub quest�� �Ϸ���� �ʾҴ�?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
				continue;
			}

#ifdef SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
			//�Ϲ� ��常
			if( pSubQuestTemplet->m_iID == 131141 || pSubQuestTemplet->m_iID == 131190 ||
				pSubQuestTemplet->m_iID == 131200 || pSubQuestTemplet->m_iID == 131210  )
			{
				if( CXSLDungeon::IsHenirChallengeMode( static_cast<int>( cDungeonMode ) ) == true )
				{
					continue;
				}
			} 
			//���� ��常
			else if( pSubQuestTemplet->m_iID == 131151 || pSubQuestTemplet->m_iID == 131160 ||
				pSubQuestTemplet->m_iID == 131170 || pSubQuestTemplet->m_iID == 131180 || pSubQuestTemplet->m_iID == 131220 )
			{
				if( CXSLDungeon::IsHenirPracticeMode( static_cast<int>( cDungeonMode ) ) == true )
				{
					continue;
				}
			}
#endif SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
			if( pSubQuestTemplet->m_ClearCondition.m_iKillNum > kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
			{
				++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;

				if( pSubQuestTemplet->m_ClearCondition.m_iKillNum <= kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
				else if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: ��Ʈ������ ��Ҵµ� 1ȸ�� �ƴϴ�?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
						<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_NPC_HUNT ����" )
					<< BUILD_LOG( iMonsterID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess );
			}
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
			else if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
			{
				START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: ����Ʈ �Ϸ����� �����ð� ��Ʈ���� ������ ��" )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetRoomUID() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< END_LOG;
			}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

#else		// SERV_REFORM_QUEST

//{{ 2011. 08. 31  ��μ�	��д��� ����
//#ifdef SERV_RANDOM_DAY_QUEST
void KUserQuestManager::Handler_ERM_NPC_UNIT_DIE_NOT( IN const int iDungeonID, IN const char cDifficulty, IN const int iMonsterID, IN KGSUserPtr spUser, IN const char cDungeonMode )
//#else
//void KUserQuestManager::Handler_ERM_NPC_UNIT_DIE_NOT(IN int iDungeonID, IN char cDifficulty, IN int iMonsterID, IN KGSUserPtr spUser )
//#endif SERV_RANDOM_DAY_QUEST
//}}
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() )
			continue;

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_NPC_HUNT )
				continue;

			//{{ 2009. 8. 3  ������		����Ʈ ���� ����
			if( pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iMonsterID) ) == pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.end() )
				continue;
			//}}

			//���� ������ �ִ���
			if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID > 0 )
			{
				//{{ 2011. 08. 31  ��μ�	��д��� ����
#ifdef SERV_RANDOM_DAY_QUEST
				// ��д����̶��..
				if( CXSLDungeon::IsSecretDungeon( pSubQuestTemplet->m_ClearCondition.m_eDungeonID ) == true )
				{
					// �ش� ��д����� �´��� �˻�
					if( ( pSubQuestTemplet->m_ClearCondition.m_eDungeonID / 10 ) != ( iDungeonID / 10 ) )
						continue;

					// ���̵� �˻�
					if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID )
					{
						if( pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode )
								continue;
						}
					}
				}
				else
				{
					if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
						continue;

					//�׸��� ���̵� ������ �ִ��� Ȯ���Ѵ�.
					if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty >= 0 )
					{
						//{{ 2009. 3. 31  ������	Ư�� ���̵� �̻��� ���ǿ� ����
						if( pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
								continue;
						}
						else
						{
							if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
								continue;
						}
						//}}
					}
				}

#else
				//				if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
				//					continue;
				//
				//				//�׸��� ���̵� ������ �ִ��� Ȯ���Ѵ�.
				//				if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty >= 0 )
				//				{
				//					//{{ 2009. 3. 31  ������	Ư�� ���̵� �̻��� ���ǿ� ����
				//					if( pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty == false )
				//					{
				//						if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
				//							continue;
				//					}
				//					else
				//					{
				//						if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
				//							continue;
				//					}
				//					//}}
				//				}
#endif SERV_RANDOM_DAY_QUEST
				//}}
			}

			//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			if( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID > 0 )
			{
				if( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID != spUser->GetMapID() )
					continue;
			}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}

			//{{ 2012. 07. 16	������		��Ʋ�ʵ� ���� ����Ʈ
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
			if( pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID > 0 )
			{
				if( pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID != spUser->GetMapID() )
					continue;
			}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
			//}}

			if( pSubQuestTemplet->m_ClearCondition.m_iKillNum > kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
			{
				++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;

				if( pSubQuestTemplet->m_ClearCondition.m_iKillNum <= kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_NPC_HUNT ����" )
					<< BUILD_LOG( iMonsterID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess );
			}
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_REFORM_QUEST
//}}

void KUserQuestManager::Handler_EGS_TALK_WITH_NPC_REQ( IN int iNPCID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT	kNot;

	std::map<int, KQuestInstance>::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); mit++ )
	{
		KQuestInstance* pkQuestInstance = &(mit->second);

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );

		if( pQuestTemplet != NULL )
		{

#ifdef SERV_EPIC_QUEST
			// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
			if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
				pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
				)
				continue;
#endif SERV_EPIC_QUEST

			if( pQuestTemplet->m_vecSubQuest.empty() == false )
			{
				for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
				{
					const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );

					if( pSubQuestTemplet != NULL )
					{
						if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_NPC_TALK )
							continue;

						//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
						// Ư�� Village ������ �Ϸ� �����ϴٸ�
						if( pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.size() > 0 )
						{
							// Village �˻�����
							std::set<int>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.find( spUser->GetMapID() );
							if( sit == pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.end() )
							{
								continue;							
							}
						}
#endif SERV_REFORM_QUEST
						//}}

						if( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID != iNPCID )
							continue;

						//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
						// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
						if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == false )
							continue;
#endif SERV_REFORM_QUEST
						//}}

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
						// 1�������� �ƴϸ�
						if( pSubQuestTemplet->m_iID == _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBadCharQuestLevel1 )
						{
							if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == false && CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == false)
								continue;
						}
						// 2�������� �ƴϸ�
						else if( pSubQuestTemplet->m_iID == _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBadCharQuestLevel2 )
						{
							if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == false )
								continue;
						}
						// 50������ �ƴϸ�
						else if( pSubQuestTemplet->m_iID == _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBadCharQuestLevel3 )
						{
							if( spUser->GetLevel() <= 49)
								continue;
						}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
					
						if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData == 0 )
						{
							pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData = 1;
							pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

							kNot.m_vecQuestInst.push_back( *pkQuestInstance );

							START_LOG( clog, L"����Ʈ SQT_NPC_TALK ����" )
								<< BUILD_LOG( iNPCID )
#ifdef SERV_PRIVACY_AGREEMENT
								<< BUILD_LOG( spUser->GetCharUID() )
								<< BUILD_LOG( spUser->GetUID() )
#else
								<< BUILD_LOG( spUser->GetCharName() )
								<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
								<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
								<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
								<< BUILD_LOGc( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
								<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
								;
						}
						else
							continue;
					}
					else
					{
						START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
							<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
							<< BUILD_LOG( spUser->GetCharUID() )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;

						SET_ERROR( ERR_QUEST_08 );
					}
				}
			}
		}
		else
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::Handler_OnDungeonClear( IN const KGSUserPtr spUser, 
											   IN const int iDungeonID, 
											   IN const char cDifficulty, 
											   IN const char cDungeonMode, 
											   IN const char cRank, 
											   IN const int iPlayTime, 
											   IN const int iDamage, 
											   IN const std::set< UidType >& setPartyMembers, 
											   IN const std::map< int, KSubQuestInfo >& mapOngoingQuest,
											   //{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
											   IN const std::map< UidType, bool >& mapHaveExpInDungeon,
											   IN const std::map< UidType, bool >& mapSuitableLevelInfo
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
											   ,IN const bool bSChar1
											   ,IN const bool bSChar2
#endif //SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
											   ,IN const std::vector<UidType> vecRelation
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
											   ,IN const bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST
#ifdef SERV_THREE_COLOR_EVENT
											   ,IN const std::set< int >& setEquippedTitle
#endif SERV_THREE_COLOR_EVENT
											   )
#else
											   IN const std::map< UidType, bool >& mapHaveExpInDungeon )
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
											   //}}
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType 
		 &&	pQuestTemplet->m_iPlayLevel > spUser->GetLevel()	)
			continue;

		for( u_int iIndexSub = 0; iIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const int iSubQuestID = pQuestTemplet->m_vecSubQuest[iIndexSub];

			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			bool bRVal = false;
			bool bValidDungeonID = false;

			// ��ȿ�� �����̶��
			if( SiCXSLQuestManager()->CheckValidDungeonID( iSubQuestID ) == true ) 
			{
				bValidDungeonID = true;

				// SubQuest �� �ִ� ������ �÷��� �� ���� �ƴ϶��
				if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( iSubQuestID, iDungeonID + static_cast<int>(cDifficulty), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					continue;
			}

			switch( pSubQuestTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_DUNGEON_TIME:
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearTime >= iPlayTime )
						bRVal = true;
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_RANK:
				{
					if( (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
					{
#ifdef SERV_EVENT_TITLE_SUBQUEST
						if(pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1)
						{
							if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
							{
								bRVal = true;
							}
							else
							{
								bRVal = false;
							}
						}
						else
						{
							bRVal = true;
						}
#else //SERV_EVENT_TITLE_SUBQUEST
						bRVal = true;
#endif SERV_EVENT_TITLE_SUBQUEST
					}
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_DAMAGE:
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iDungeonDamage >= iDamage )
						bRVal = true;
				}
				break;

#ifndef DEPRECATED_SERVER_GROUP_MASK
			case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
				{
					BOOST_TEST_FOREACH( const UidType, iPartyMemberUnitUID, setPartyMembers )
					{
						if( iPartyMemberUnitUID == spUser->GetCharUID() )
							continue;

						// �ٸ� ������ ������ �÷����Ѱ��̶��..?
						if( spUser->IsSameServerGroupUID( iPartyMemberUnitUID ) == false )
						{
							bRVal = true;
						}
					}
				}
				break;
#endif DEPRECATED_SERVER_GROUP_MASK

			case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
				{
					if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT  ||
						pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_PCBANG )
					{

						if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false && 
							CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
						{
							 
							if( bValidDungeonID == false )
							{
								if( ExistHaveExpInDungeon( spUser->GetCharUID(), mapHaveExpInDungeon ) )
								{
#ifdef SERV_EVENT_TITLE_SUBQUEST		
										//Īȣ�� ���� ��������Ʈ ������ �ִ�.
										if(pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1 && pSubQuestTemplet->m_ClearCondition.m_eDungeonRank == CXSLDungeon::RT_NONE) 
										{
											if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
											{
												bRVal = true;
											}
											else
											{
												bRVal = false;
											}
										}
										//��ũ�� ���� ��������Ʈ ������ �ִ�.
										else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID == -1)
										{
											if( (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
											{
												bRVal = true;
											}
											else
											{
												bRVal = false;
											}
										}
										//Īȣ ��ũ�� ��������Ʈ ������ �ִ�.
										else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1) 
										{
											if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID && (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank)
											{
												bRVal = true;
											}
											else
											{
												bRVal = false;
											}
										}
										else
										{
											bRVal = true;
										}
#else //SERV_EVENT_TITLE_SUBQUEST
									bRVal = true;
#endif SERV_EVENT_TITLE_SUBQUEST
								}
							}
							else
							{
#ifdef SERV_EVENT_TITLE_SUBQUEST	
									//Īȣ�� ���� ��������Ʈ ������ �ִ�.
									if(pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1 && pSubQuestTemplet->m_ClearCondition.m_eDungeonRank == CXSLDungeon::RT_NONE) 
									{
										if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
										{
											bRVal = true;
										}
										else
										{
											bRVal = false;
										}
									}
									//��ũ�� ���� ��������Ʈ ������ �ִ�.
									else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID == -1)
									{
										if( (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
										{
											bRVal = true;
										}
										else
										{
											bRVal = false;
										}
									}
									//Īȣ ��ũ�� ��������Ʈ ������ �ִ�.
									else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1) 
									{
										if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID && (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank)
										{
											bRVal = true;
										}
										else
										{
											bRVal = false;
										}
									}
									else
									{
										bRVal = true;
									}
#else //SERV_EVENT_TITLE_SUBQUEST
								bRVal = true;
#endif SERV_EVENT_TITLE_SUBQUEST
							}
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
							if( pQuestTemplet->m_iID == 63780 )
							{
								bRVal = false;

								for(int i = 0; i < vecRelation.size(); i++ )
								{
									if( vecRelation[i] == spUser->GetCharUID() )
									{
										bRVal = true;
										break;
									}
								}					
							}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
							START_LOG( cout, L"��õ�� ���� ���� ��������Ʈ��!" )
								<< BUILD_LOG( pSubQuestTemplet->m_iID )
								<< BUILD_LOG( bRVal )
								<< BUILD_LOG( bHasFriend )
								<< END_LOG;

							if( pSubQuestTemplet->m_iID == PLAY_TOGETHER_DUNGEON_CLEAR_SUBQUEST_ID )
							{
								bRVal = bHasFriend;
							}
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_THREE_COLOR_EVENT
							if( pQuestTemplet->m_iID == 63900 )
							{
								bRVal = false;
								
								if( setEquippedTitle.size() == 3 && setPartyMembers.size() >= 3 )
								{
									bRVal = true;
								}
							}
#endif SERV_THREE_COLOR_EVENT
						}
					}				
					else
					{
						//���� Ŭ���� ����� �¸��� �ϸ� �Ǳ⶧���� �̰����� ó��
						//## �ʽ����� ���� ����ó���Ѵ�.
						if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL && 
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
						{
							bRVal = true;
						}
					}
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
					for( int i = 0; i < _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::iSubQuestMaxNum; ++i )
					{
						if( pSubQuestTemplet->m_iID == _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::aSubQuestID[i] )
						{
							if( setPartyMembers.size() >= _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::cRequiredMembers )
							{
								// ����
								bRVal = bSChar1 & bSChar2;
							}
							else
							{
								bRVal = false;
							}
						}
					}
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT

#ifdef SERV_PET_WITH_DUNGEON_CLEAR_COUNT
					if( pSubQuestTemplet->m_iID == 85560 )
					{
						KPetInfo kPetInfo;
						const UidType iSummonedPetUID = spUser->GetSummonedPetUID();
						kPetInfo = spUser->GetPetInfo();
											
						if( iSummonedPetUID != 0 && kPetInfo.m_iPetID == 30004 && kPetInfo.m_cEvolutionStep == 3 )
						{
							bRVal = true;
						}
						else
						{
							bRVal = false;
						}
					}
#endif
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
					// ���̵� �� Ÿ�� 3ȸ���� Ŭ����
					if( pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID2 || pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID3 )
					{
						// ���̵� �� Ÿ�� ������
						int iOnRidingPet = spUser->GetiRidingPetSummoned();
						bool bOnRidingPet = false;

						if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false && 
							CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
						{
							if( ExistHaveExpInDungeon( spUser->GetCharUID(), mapHaveExpInDungeon ) )
							{
								if( pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID2 )
								{
									// �̺�Ʈ�� ���̵� ��
									if(iOnRidingPet == 2)
									{
										bOnRidingPet = true;
									}
								}
								else if(pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID3)
								{
									// ĳ�ÿ� ���̵� ��
									if(iOnRidingPet == 1)
									{
										bOnRidingPet = true;
									}
								}

								
							}
						}

						bRVal = bOnRidingPet;

						//START_LOG( clog2, L"�輮��_���̵��� �̺�Ʈ ���� ����Ʈ!" )
							//<<BUILD_LOG( pSubQuestTemplet->m_iID )
							//<<BUILD_LOG( iOnRidingPet )
							//<<BUILD_LOG( bRVal )
							//<<BUILD_LOG( iDungeonID )
							//<<END_LOG;
					}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST
#ifdef SERV_RIDING_PET_WITH_SUB_QUEST
					// Ư�� ���̵� �� Ÿ�� ���� Ŭ����
					if( pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::iHaveSubQuestID || pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::iHaveSubQuestID )
					{
						// ���̵� �� Ÿ�� ������
						int iOnRidingPetID = spUser->GetiRidingSummonedPetID();
						bool bOnRidingPet = false;

						if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false && 
							CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
						{
							if( ExistHaveExpInDungeon( spUser->GetCharUID(), mapHaveExpInDungeon ) )
							{
								if( pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::iHaveSubQuestID )
								{
									if(_CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::bAllRidingPet == true)
									{
										bOnRidingPet = true;
									}
									else
									{
										if( _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::iRidingPetID == iOnRidingPetID)
											bOnRidingPet = true;
									}
									
								}
								else if(pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::iHaveSubQuestID)
								{
									if(_CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::bAllRidingPet == true)
									{
										bOnRidingPet = true;
									}
									else
									{
										if( _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::iRidingPetID == iOnRidingPetID)
											bOnRidingPet = true;
									}
								}
							}
						}

						bRVal = bOnRidingPet;

						//START_LOG( clog2, L"�輮��_���̵��� �̺�Ʈ ���� ����Ʈ!" )
						//<<BUILD_LOG( pSubQuestTemplet->m_iID )
						//<<BUILD_LOG( iOnRidingPetID )
						//<<BUILD_LOG( bRVal )
						//<<BUILD_LOG( iDungeonID )
						//<<END_LOG;
					}
#endif //SERV_RIDING_PET_WITH_SUB_QUEST
				}
				break;
				//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
				{
					if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
						CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
						CXSLDungeon::IsTCDungeon( iDungeonID ) == false &&
						//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
						CXSLDungeon::IsValentineDungeon( iDungeonID ) == false &&
#endif SERV_EVENT_VALENTINE_DUNGEON
						//}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
						CXSLDungeon::IsValentineDungeonInt( iDungeonID ) == false &&
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / ������
						CXSLDungeon::IsHalloweenDungeon( iDungeonID ) == false &&
#endif //SERV_HALLOWEEN_EVENT_2013
						CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
					{

						// ���� ���� ������ �÷��� �� ���� �´°�?
						if( IsSuitableLevelUser( spUser->GetCharUID(), mapSuitableLevelInfo ) == true )
						{
#ifdef SERV_THREE_COLOR_EVENT
							if( pQuestTemplet->m_iID == 63870 )
							{
								if( setPartyMembers.size() == 1 && spUser->GetEquippedTitleID() == 5470 )
								{
									bRVal = true;
								}
							}	
							else if( pQuestTemplet->m_iID == 63880 )
							{
								if( setPartyMembers.size() == 1 && spUser->GetEquippedTitleID() == 5480 )
								{
									bRVal = true;
								}			
							}
							else if( pQuestTemplet->m_iID == 63890 )
							{
								if( setPartyMembers.size() == 1 && spUser->GetEquippedTitleID() == 5490 )
								{
									bRVal = true;
								}			
							}
							else
							{
								bRVal = true;
							}
#else
							bRVal = true;
#endif SERV_THREE_COLOR_EVENT
						}
					}
				}
				break;
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
				//}}

			case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
				{
					if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT  ||
						pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_PCBANG )
					{
						if( iDungeonID != SEnum::DI_ELDER_HALLOWEEN_NORMAL  &&
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false )
						{
							// ���� ������ �Ǿ����� �ʴٸ�
							if( bValidDungeonID == false )
							{
								// ����ġ�� ȹ���� �� �ִ� �������� üũ�մϴ�.
								if( spUser->IsDungeonGetExp( iDungeonID, cDifficulty ) == true )
								{
									bRVal = true;
								}
							}
							// ���� ������ �Ǿ��ִٸ�?
							else
							{
								bRVal = true;
							}
						}
					}				
					else
					{
						//���� Ŭ���� ����� �¸��� �ϸ� �Ǳ⶧���� �̰����� ó��
						//## �ʽ����� ���� ����ó���Ѵ�.
						if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL && 							
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
						{
							bRVal = true;
						}
					}					
				}
			
				if( bRVal )
				{
					bRVal = false;

					// �������� ������ä ������ Ŭ���� �ߴ��� Ȯ���غ���!
					std::map< int, KSubQuestInfo >::const_iterator mitQ;
					mitQ = mapOngoingQuest.find( kQuestInstance.m_iID );
					if( mitQ == mapOngoingQuest.end() )
					{
						std::map< int, KSubQuestData >::const_iterator mitSQ;
						mitSQ = mitQ->second.m_mapSubQuestInfo.find( iSubQuestID );
						if( mitSQ != mitQ->second.m_mapSubQuestInfo.end() )
						{
							if( mitSQ->second.m_iInInventoryItemCount == 1 )
							{
								bRVal = true;
							}
						}
					}
				}
				break;
#ifdef SERV_POINT_COUNT_SYSTEM
			case CXSLQuestManager::SQT_POINT_COUNT:
				{
					//���⼭ ���� �ߴ��� ���ߴ��� üũ�� �ָ� �ȴ�.
					//�ϴ� ���� Ŭ���� �ϸ� ������ ��������
					 bRVal = true;
				}
				break;
#endif SERV_POINT_COUNT_SYSTEM

				// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���¶��
				if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == true )
					bRVal = true;
				
			}

			if( bRVal == true )
			{
				

				switch( pSubQuestTemplet->m_eClearType )
				{
				case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
				case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
				case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
					//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
				case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
					//}}
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
				case CXSLQuestManager::SQT_DUNGEON_RANK:
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
					{
						//{{ 2010. 05. 03  ������	���� Ŭ���� ī��Ʈ ���� ����
						// Ŭ���� ���� ī��Ʈ�� ���� �ʰ� �����Ѵ�.
						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
						{
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData += 1;
						}
						//}}

						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

						kNot.m_vecQuestInst.push_back( kQuestInstance );
					}
					break;
#ifdef SERV_POINT_COUNT_SYSTEM
				case CXSLQuestManager::SQT_POINT_COUNT:
					{
						//������ ���� Ŭ���� ī��Ʈ �� ��������� ������ �Ѿ ��� ����.
						if( pSubQuestTemplet->m_ClearCondition.m_iPointCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
						{
							//����� �̺�Ʈ ������ ���� �ֱ� ������ ����Ʈ�� ������ ��Ȯ�� ������ �����ϴ�.
							//���� ���� �߰��� �ϰڽ��ϴ�.
						}
						//�ش� ����Ʈ�� ����Ʈ�� �Ѿ��� ��쿡 �������� ���� �Ѵ�.
						if( pSubQuestTemplet->m_ClearCondition.m_iPointCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

						kNot.m_vecQuestInst.push_back( kQuestInstance );
					}
					break;
#endif SERV_POINT_COUNT_SYSTEM


				default:
					{
						if( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData == 0 )
						{
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData = 1;
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

							kNot.m_vecQuestInst.push_back( kQuestInstance );
						}
					}
					break;
				}

				START_LOG( clog, L"����Ʈ SQT_DUNGEON_TIME or SQT_DUNGEON_RANK ����" )
					<< BUILD_LOGc( cRank )
					<< BUILD_LOG( iPlayTime )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
					;
			}
		}		
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

#endif SERV_REFORM_QUEST
//}}

void KUserQuestManager::Handler_OnPVPPlay( IN int iGameType
										 , IN KGSUserPtr spUser
										 //{{ 2012. 09. 18   ��μ�   ���� �÷��� ����Ʈ ���� ����
#ifdef SERV_PVP_PLAY_QUEST
										 , IN bool bOfficialMatch
#endif SERV_PVP_PLAY_QUEST
										 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
										 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
										 , IN bool bCouplePvp
										 , IN bool bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
										 , IN const bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST
										 )
{
	//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	const int iPvpChannelClass = SiKRoomListManager()->GetPVPChannelClassByRoomListID( spUser->GetRoomListID() );
	if( iPvpChannelClass == KPVPChannelInfo::PCC_PLAY )
	{
		START_LOG( clog, L"����ä���� ����Ʈ Ŭ���� ���ǿ� �������� ����." );
		return;
	}
#endif SERV_INTEGRATION
	//}}

	OnPVP( CXSLQuestManager::SQT_PVP_PLAY
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
		 //{{ 2012. 09. 18   ��μ�   ���� �÷��� ����Ʈ ���� ����
#ifdef SERV_PVP_PLAY_QUEST
		 , bOfficialMatch
#else
		 , fasle
#endif SERV_PVP_PLAY_QUEST
		 //}}
#endif SERV_NEW_PVP_QUEST
		 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
		 , iPvpChannelClass
		 , bCouplePvp
		 , bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		 , bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
	OnPVP( CXSLQuestManager::SQT_PVP_HERO_NPC_PLAY
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
		 //{{ 2012. 09. 18   ��μ�   ���� �÷��� ����Ʈ ���� ����
#ifdef SERV_PVP_PLAY_QUEST
		 , bOfficialMatch
#else
		 , false
#endif SERV_PVP_PLAY_QUEST
		 //}}
#endif SERV_NEW_PVP_QUEST
		 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		 , bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND
}

//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
void KUserQuestManager::Handler_OnPVPPlay_AccumulatedOfficialMatchCount( IN KGSUserPtr spUser, IN const bool bIncreaseCount )

{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	//����Ʈ�� ���鼭
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance* pkQuestInstance = &mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );
		if( pQuestTemplet == NULL )		
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		//��������Ʈ�� ������ �˻��Ѵ�.
		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_PVP_PLAY_ARRANGE )	//Ŭ���� ������ ������..
				continue;

			if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
				continue;

			// pvp info ���̺��� ������ ���� ����⸸ �Ѵ�.
			pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData = spUser->GetOfficialMatchCount();
			if( bIncreaseCount == true )
			{
				++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
				pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( *pkQuestInstance );

			START_LOG( clog, L"���� ���� ����Ʈ ����!" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay )
				<< BUILD_LOG( spUser->GetOfficialMatchCount() )
				<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_2012_PVP_SEASON2
//}}

void KUserQuestManager::Handler_OnPVPWin( IN int iGameType
										, IN KGSUserPtr spUser
										//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
										//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
										, IN bool bMatchNPC
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
										//}}
										, IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
										//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
										, IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
										)
{
	//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	const int iPvpChannelClass = SiKRoomListManager()->GetPVPChannelClassByRoomListID( spUser->GetRoomListID() );
	if( iPvpChannelClass == KPVPChannelInfo::PCC_PLAY )
	{
		START_LOG( clog, L"����ä���� ����Ʈ Ŭ���� ���ǿ� �������� ����." );
		return;
	}
#endif SERV_INTEGRATION
	//}}

	//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
	OnPVP( CXSLQuestManager::SQT_PVP_WIN
		   , iGameType
		   , spUser
		   , bOfficialMatch
#ifdef PVP_QUEST_HERO_KILL_COUNT	// 2013.02.08 lygan_������ //( SERV_PVP_NPC_HUNT_QUEST_EXTEND ���� �ؾ� �� ) ��ȹ ������ ������ �����̶� ���⿡ �ɸ��� �����̶� ���̰� �ִ� ��ȹ���� win�� ������ ���� �ִµ� ���� üũ�� kill �ʿ��� �ϰ��־ �ǹ̸� �ϳ��� ���� ��Ų��.
		   , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		   , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		   , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		   );

#ifdef PVP_QUEST_HERO_KILL_COUNT	// 2013.02.08 lygan_������ //( SERV_PVP_NPC_HUNT_QUEST_EXTEND ���� �ؾ� �� ) ��ȹ ������ ������ �����̶� ���⿡ �ɸ��� �����̶� ���̰� �ִ� ��ȹ���� win�� ������ ���� �ִµ� ���� üũ�� kill �ʿ��� �ϰ��־ �ǹ̸� �ϳ��� ���� ��Ų��.
	OnPVP( CXSLQuestManager::SQT_PVP_NPC_HUNT
		 , iGameType
		 , spUser
		 , bOfficialMatch
		 , bIsHeroNPC
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );
#endif //PVP_QUEST_HERO_KILL_COUNT

#else
	if( bMatchNPC == true )
	{
		OnPVP( CXSLQuestManager::SUB_QUEST_TYPE::SQT_PVP_NPC_HUNT, iGameType, spUser, bMatchNPC );
	}
	else
	{
		OnPVP( CXSLQuestManager::SUB_QUEST_TYPE::SQT_PVP_WIN, iGameType, spUser, bOfficialMatch );
	}
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
//}}
#else
	OnPVP( CXSLQuestManager::SUB_QUEST_TYPE::SQT_PVP_WIN, iGameType, spUser );
#endif SERV_NEW_PVP_QUEST
	//}} 
}

void KUserQuestManager::Handler_OnPVPKill( IN int iGameType
										 , IN KGSUserPtr spUser
										 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
										 , IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
										 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
										 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
										 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
										 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
										 )
{
	//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	const int iPvpChannelClass = SiKRoomListManager()->GetPVPChannelClassByRoomListID( spUser->GetRoomListID() );
	if( iPvpChannelClass == KPVPChannelInfo::PCC_PLAY )
	{
		START_LOG( clog, L"����ä���� ����Ʈ Ŭ���� ���ǿ� �������� ����." );
		return;
	}
#endif SERV_INTEGRATION
	//}}

	OnPVP( CXSLQuestManager::SQT_PVP_KILL
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
		 , bOfficialMatch
#endif SERV_NEW_PVP_QUEST
		 //}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT // 2013.02.08 lygan_������ // SERV_PVP_NPC_HUNT_QUEST_EXTEND ���� �ؾ� �� 
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );

	//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
	OnPVP(
		//{{ 2013. 2. 14	Merge	�ڼ���	 ���� SQT_PVP_NPC_HUNT ����Ʈ Ÿ�Կ� ���� NPC �� �÷���, ���� NPC �¸����ǵ� �߰�
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
		   CXSLQuestManager::SQT_PVP_HERO_NPC_KILL
#else
		   CXSLQuestManager::SQT_PVP_NPC_HUNT
#endif SERV_PVP_NPC_HUNT_QUEST_EXTEND
		   //}}
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
		 , bOfficialMatch
#endif SERV_NEW_PVP_QUEST
		 //}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT // 2013.02.08 lygan_������ // SERV_PVP_NPC_HUNT_QUEST_EXTEND ���� �ؾ� �� 
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
	//}}
}

void KUserQuestManager::OnPVP( IN CXSLQuestManager::SUB_QUEST_TYPE eSubQuestType
							 , IN int iGameType
							 , IN KGSUserPtr spUser
							 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
							 , IN bool bMatch
#endif SERV_NEW_PVP_QUEST
							 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
							 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
							 , IN int iPvpChannelClass
							 , IN bool bCouplePvp
							 , IN bool bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
							 , IN bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
							 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
							 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
							 )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	//����Ʈ�� ���鼭
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance* pkQuestInstance = &mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );
		if( pQuestTemplet == NULL )		
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}
		
		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST
		
		//��������Ʈ�� ������ �˻��Ѵ�.
		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
			if( pSubQuestTemplet->m_iID == PLAY_TOGETHER_PLAY_PVP_SUBQUEST_ID && bHasFriend == false )
			{
				continue;
			}
#endif SERV_RECRUIT_EVENT_SUBQUEST
			
			if( pSubQuestTemplet->m_eClearType != eSubQuestType )	//Ŭ���� ������ ������..
				continue;

			if( pSubQuestTemplet->m_ClearCondition.m_ePVPType != CXSLRoom::PGT_NULL )
			{
				if( pSubQuestTemplet->m_ClearCondition.m_ePVPType != iGameType )	//����Ÿ���� ������..
					continue;
			}

			//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
			if( pSubQuestTemplet->m_ClearCondition.m_ePvpChannelClass == KPVPChannelInfo::PCC_OFFICIAL )
			{
				if( bMatch != true )
					continue;
			}
#endif SERV_NEW_PVP_QUEST
			//}} 

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
				continue;
			//{{ 2013. 2. 21	�ڼ���	���� NPC ����Ʈ�� ���� NPC �� ī��Ʈ �ǰ� ����
#ifdef PVP_QUEST_HERO_KILL_COUNT
#else
			else
				++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
			//}}

			switch( eSubQuestType )
			{
			case CXSLQuestManager::SQT_PVP_PLAY:
				{
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
					if( pQuestTemplet->m_iID == 63790 )
					{
						if( bCouplePvp == true && iPvpChannelClass == KPVPChannelInfo::PCC_TOURNAMENT && bIsDrawn == false )
							++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
					}
					else
					{
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
					//{{ 2013. 2. 21	�ڼ���	���� NPC ����Ʈ�� ���� NPC �� ī��Ʈ �ǰ� ����
#ifdef PVP_QUEST_HERO_KILL_COUNT
					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
					//}}
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
					}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;									
				}
				break;
			case CXSLQuestManager::SQT_PVP_WIN:
				{
					//{{ 2013. 2. 21	�ڼ���	���� NPC ����Ʈ�� ���� NPC �� ī��Ʈ �ǰ� ����
#ifdef PVP_QUEST_HERO_KILL_COUNT
					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
					//}}

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPWin <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
				}
				break;
			case CXSLQuestManager::SQT_PVP_KILL:
				{
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL	// ���̺��� ų�Ҷ� Ƚ���� ���̺��� �ƴ� ĳ���͸� ų�Ҷ� Ƚ�� ����Ʈ
					CXSLUnit::UNIT_TYPE killUserUnitType = (CXSLUnit::UNIT_TYPE)spUser->GetUnitType();
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
					CXSLUnit::UNIT_CLASS killUserUnitClass = static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass());
					int ikillUserUnitClass = static_cast<int>(killUserUnitClass);
					int ikilledUserUnitClass = static_cast<int>(killedUserUnitClass);
					if (pSubQuestTemplet->m_iID == 121210 ) //û�� ���� ����Ʈ ���̵�
#else //SERV_EVENT_QUEST_CHUNG_PVP_KILL
					if (pSubQuestTemplet->m_iID == 130641 )
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					{	
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
						// ���� û 2�������� �ƴϰ� ������ 2������ �߿� �ϳ��̰� ���ΰ� û2���̴�.
						if( ikillUserUnitClass >= 100 && 118 >= ikillUserUnitClass ) //�̾ȿ� ������ 2�� ����
						{
							if( killUserUnitClass != CXSLUnit::UC_CHUNG_IRON_PALADIN &&  killUserUnitClass != CXSLUnit::UC_CHUNG_DEADLY_CHASER )
							{
								if( CXSLUnit::UNIT_CLASS::UC_CHUNG_IRON_PALADIN == killedUserUnitClass || killedUserUnitClass == CXSLUnit::UNIT_CLASS::UC_CHUNG_DEADLY_CHASER )
								{
									++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
								}
							}
							
						}
#else SERV_EVENT_QUEST_CHUNG_PVP_KILL
						// ���� ���̺��� �ƴϰ�, ų ��밡 ���̺��̸�,
						if( CXSLUnit::UT_RAVEN != killUserUnitType
							 && CXSLUnit::UT_RAVEN == killedUserUnitType
							)
						{
							++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
						}
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					}
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
					else if (pSubQuestTemplet->m_iID == 121220) //���� û 2�� �����̰� ų ��밡 û 2�� ������ �ƴϸ� ����Ʈ Ŭ���� 
#else //SERV_EVENT_QUEST_CHUNG_PVP_KILL
					else if (pSubQuestTemplet->m_iID == 130640)
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					{
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
						if( killUserUnitClass == CXSLUnit::UC_CHUNG_IRON_PALADIN  ||  killUserUnitClass == CXSLUnit::UC_CHUNG_DEADLY_CHASER ) //�� û 2���̴�
						{
							if( ikilledUserUnitClass >= 100 && 118 >= ikilledUserUnitClass ) //2�� ���� ĳ���� �׿���?
							{
								if( killedUserUnitClass != CXSLUnit::UC_CHUNG_IRON_PALADIN  &&  killedUserUnitClass != CXSLUnit::UC_CHUNG_DEADLY_CHASER ) //2�� �����߿��� ���ΰ� û2���� �ƴϳ�?
								{
									++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
								}
							}
						}
#else //SERV_EVENT_QUEST_CHUNG_PVP_KILL
						// ���� ���̺��̰�, ų ��밡 ���̺�ƴϸ�,
						if(CXSLUnit::UT_RAVEN == killUserUnitType
							&& CXSLUnit::UT_RAVEN != killedUserUnitType)
							++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					}
					else
					{
						++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
					}
#else
						//{{ 2013. 2. 21	�ڼ���	���� NPC ����Ʈ�� ���� NPC �� ī��Ʈ �ǰ� ����
	#ifdef PVP_QUEST_HERO_KILL_COUNT
						++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
	#endif PVP_QUEST_HERO_KILL_COUNT
						//}}
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPKill <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
				}
				break;
				//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
			case CXSLQuestManager::SQT_PVP_NPC_HUNT:
				//{{ 2013. 2. 22	Merge	�ڼ���	 ���� SQT_PVP_NPC_HUNT ����Ʈ Ÿ�Կ� ���� NPC �� �÷���, ���� NPC �¸����ǵ� �߰�
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
			case CXSLQuestManager::SQT_PVP_HERO_NPC_KILL:
			case CXSLQuestManager::SQT_PVP_HERO_NPC_PLAY:
#endif SERV_PVP_NPC_HUNT_QUEST_EXTEND
				//}}
				{
					//{{ 2013. 2. 21	�ڼ���	���� NPC ����Ʈ�� ���� NPC �� ī��Ʈ �ǰ� ����
#ifdef PVP_QUEST_HERO_KILL_COUNT
					if( bIsHeroNPC == false )
					{
						break;
					}

					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
					//}}

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPWin <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					{
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
					}
				}
				break;
#endif SERV_NEW_PVP_QUEST
				//}}
				
				//{{ 2013. 2. 21	�ڼ���	���� NPC ����Ʈ�� ���� NPC �� ī��Ʈ �ǰ� ����
#ifdef PVP_QUEST_HERO_KILL_COUNT
			default:
				{
					START_LOG( cwarn, L"OnPvp�� ó������ �ʴ� ���� ����Ʈ Ÿ���Դϴ�." )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( eSubQuestType )
						<< END_LOG;
				}
				break;
#endif PVP_QUEST_HERO_KILL_COUNT
				//}}
			}

			kNot.m_vecQuestInst.push_back( *pkQuestInstance );

			START_LOG( clog, L"����Ʈ ������ ����.!" )
				<< BUILD_LOG( iGameType )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
				<< BUILD_LOGc( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
				<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
				;			
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
void KUserQuestManager::Handler_OnEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iEnchantLevel )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )		
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ ��ȭ ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_ENCHANT )
				continue;

			// 3. � ����������?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			// 4. �ش� ��ȭ ������ �´���?
			if( pSubQuestTemplet->m_ClearCondition.m_iEnchantLevel != iEnchantLevel )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData == 0 )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_ITEM_ENCHANT ����" )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( iEnchantLevel )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
		
	}
}

void KUserQuestManager::Handler_OnSocketItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iSocketUseCount )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_SOCKET )
				continue;

			// 3. �ش� �������� �´���?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			// ����Ʈ Ŭ���� ���� ī��Ʈ�� �����ʰ� �Ѵ�.
			if( pSubQuestTemplet->m_ClearCondition.m_iSocketCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				// ���� ��� ī��Ʈ ��ŭ �ø���!
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += static_cast<UCHAR>(iSocketUseCount);

				// Ŭ���� ������ �Ѿ�ٸ� ��������!
				if( pSubQuestTemplet->m_ClearCondition.m_iSocketCount < static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData ) )
					kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = static_cast<UCHAR>(pSubQuestTemplet->m_ClearCondition.m_iSocketCount);
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iSocketCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"����Ʈ SQT_ITEM_SOCKET ����" )
				<< BUILD_LOG( iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;			
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

void KUserQuestManager::Handler_OnAttribEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const KItemAttributeEnchantInfo& kItemAttribEnchantInfo )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ �Ӽ� ��ȭ ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_ATTRIB )
				continue;

			// 3. �ش� �������� �´���?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			// 4-1. �Ӽ� �ܰ�����?
			if( pSubQuestTemplet->m_ClearCondition.m_eAttribCountType != CXSLAttribEnchantItem::ACT_NONE )
			{
				if( SiCXSLAttribEnchantItem()->GetAttribCountType( kItemAttribEnchantInfo ) != pSubQuestTemplet->m_ClearCondition.m_eAttribCountType )
					continue;
			}
			// 5-2. Ư�� �Ӽ� ����?
			else
			{
				if( pSubQuestTemplet->m_ClearCondition.m_kAttribEnchantInfo.Compare( kItemAttribEnchantInfo ) == false )
					continue;
			}

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData == 0 )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_ITEM_ATTRIB ����" )
					<< BUILD_LOG( iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}    
}

void KUserQuestManager::Handler_OnResolveItem( IN KGSUserPtr spUser, IN const int iItemID )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_RESOLVE )
				continue;

			// 3. �ش� �������� �´���?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			// ����Ʈ Ŭ���� ���� ī��Ʈ�� �����ʰ� �Ѵ�.
			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"����Ʈ SQT_ITEM_RESOLVE ����" )
				<< BUILD_LOG( iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
void KUserQuestManager::Handler_OnResolveItem( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_RESOLVE )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}
			// ����Ʈ Ŭ���� ���� ī��Ʈ�� �����ʰ� �Ѵ�.
			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"����Ʈ SQT_ITEM_RESOLVE ����" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_MULTI_RESOLVE
//}}

void KUserQuestManager::Handler_OnUseSkillPoint( IN KGSUserPtr spUser, IN const int iSkillID )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_USE_SKILL_POINT )
				continue;

			// 3. �ش� ��ų�� �´���?
			if( pSubQuestTemplet->m_ClearCondition.m_iSkillID != iSkillID  &&  pSubQuestTemplet->m_ClearCondition.m_iSkillID != 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			// ����Ʈ Ŭ���� ���� ī��Ʈ�� �����ʰ� �Ѵ�.
			if( pSubQuestTemplet->m_ClearCondition.m_iSpUseCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iSpUseCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"����Ʈ SQT_USE_SKILL_POINT ����" )
				<< BUILD_LOG( iSkillID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess );
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_QUEST_CLEAR_EXPAND
//}}

//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
void KUserQuestManager::Handler_OnEnterTheVillage( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. �ʵ� ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_VISIT_VILLAGE )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// 3. �ش� �ʵ尡 �´��� Ȯ��!
			if( SiCXSLQuestManager()->IsExistVillageInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
				continue;
#else
			// 3. �ش� �ʵ尡 �´��� Ȯ��!
			if( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID != spUser->GetMapID() )
				continue;
#endif SERV_REFORM_QUEST
			//}}
			
			// 4. �̹� �Ϸ��� ����Ʈ ���� Ȯ��!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_VISIT_VILLAGE ����" )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}

//{{ 2012. 07. 16	������		��Ʋ�ʵ� ���� ����Ʈ
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
void KUserQuestManager::Handler_OnEnterTheBattleField( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. �ʵ� ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_VISIT_FIELD )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// 3. �ش� �ʵ尡 �´��� Ȯ��!
			if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
				continue;
#else
			// 3. �ش� �ʵ尡 �´��� Ȯ��!
			if( pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID != spUser->GetMapID() )
				continue;
#endif SERV_REFORM_QUEST
			//}}
#ifdef  SERV_EVENT_TITLE_SUBQUEST
			if( pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1 ) //Īȣ�� ���õȰ� �ִ�.
			{
				//���� �����Ҷ� ������ Īȣ�� ����Ʈ Ŭ���� Īȣ�� �ƴ϶��....
				if( spUser->GetEquippedTitleID() !=  pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
				{
					continue;
				}
			}
#endif  SERV_EVENT_TITLE_SUBQUEST
			// 4. �̹� �Ϸ��� ����Ʈ ���� Ȯ��!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_VISIT_FIELD ����" )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//}}
//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::Handler_EGS_QUEST_COMPLETE_REQ( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin /*= false*/ )
{
	SET_ERROR( NET_OK );

	bool bCheckAfterQuest = true;
	bool bIsVisitVillageQuest = false;
	std::map<int,int> mapAcceptRewardItem;
	std::vector<int>::const_iterator vitAfterQuest;
    
	if( m_mapQuesting.empty() == true )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ���������� Ȯ���Ѵ�.	
	if( IsQuest( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ����Ʈ �ν��Ͻ� ���
	KQuestInstance* pKQuestInst = GetQuestInstance( kReq.m_iQuestID );
	if( pKQuestInst == NULL )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ����Ʈ ���ø� ���
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"����Ʈ �������ε� ���ø� �����Ͱ� ����." )
			<< BUILD_LOG( kReq.m_iQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetUID() )
#else
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SET_ERROR( ERR_QUEST_08 );
		goto error_proc;
	}


	// PC�� ���� ����Ʈ ���� �˻�
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_20 );
			goto error_proc;
		}
	}


	// �̺�Ʈ ����Ʈ��� ���� �������� �̺�Ʈ���� Ȯ���Ѵ�.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// ���� �������� �̺�Ʈ���� Ȯ��!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

	// ���� ����Ʈ �ϰ�� ������ �ȵǸ� ����Ʈ �Ϸ� �Ҽ�����.
	if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_Condition.m_iLevel > spUser->GetLevel() )
		goto error_proc;

	vitAfterQuest = pQuestTemplet->m_vecAfterQuestID.begin();
	for( ; vitAfterQuest != pQuestTemplet->m_vecAfterQuestID.end() ; ++vitAfterQuest )
	{
		if( bCheckAfterQuest == false )
			break;

		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( *vitAfterQuest );
		if( pAfterQuestTemplet == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ����Ʈ�� after����Ʈ�� ��ϵǾ��ֽ��ϴ�!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( *vitAfterQuest )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );

			bCheckAfterQuest = false;
			continue;
		}

		// ���� ������ �ִ��� Ȯ���غ���!
		if( pAfterQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// ����Ʈ ���� ������ �κ��丮�� ���� ������ ����Ʈ ���� ����
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pAfterQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pAfterQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"����Ʈ ������ �޴� ������ �̹� �κ��丮�� ������ ����Ʈ ���� ����." )
						<< BUILD_LOG( pAfterQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_22 );

					bCheckAfterQuest = false;
					continue;
				}
				mapAcceptRewardItem.insert( std::make_pair( mitRI->first, mitRI->second ) );
			}
		}
	}

	// AfterQuest(���� �ϼ��� ����) ���� ������ ������ �󽽷��� �ִ��� �˻�
	if( spUser->IsEnoughSpaceExist( mapAcceptRewardItem ) == false )
	{
		START_LOG( clog, L"����Ʈ ������ �޴� ������ �� �κ��丮 ������� �����մϴ�." )
			<< BUILD_LOG( mapAcceptRewardItem.size() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_23 );

		goto error_proc;
	}

	// AfterQuest �˻翡�� ���и� �ߴٸ�
	if( bCheckAfterQuest == false )
		goto error_proc;
	
	for( int iIndexSub = 0; iIndexSub < (int)pKQuestInst->m_vecSubQuestInstance.size(); ++iIndexSub )
	{
		// item ���� ����Ʈ ����Ϸ� üũ�� ������ �־����.(�Ϸ�üũ�ÿ��� �ϱ⶧��)
		// ������ ������ ������ ��� ���� ���� �Ϸ�üũ�� �ϴ� �������� üũ�ؾ� �ϱ� ����
		const int iSubQuestID = pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_iID;

		const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
		if( pSubQuestTemplet == NULL )
		{
			START_LOG( cerr, L"����Ʈ �������ε� �������ø� �����Ͱ� ����." )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_VISIT_VILLAGE )
			bIsVisitVillageQuest = true;

		//��� ����̸� �������� �ʴ´�.
		if( bIsAdmin == true )
			continue;

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION  ||
			pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
		{
			//�����ؾ��� ������(������ŭ) �ִ��� Ȯ���Ͽ� ��������Ʈ �Ϸ�üũ�� �Ѵ�.
			std::map<int,int> mapItemInfo;
			mapItemInfo.insert( std::make_pair( pSubQuestTemplet->m_ClearCondition.m_iItemID, pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum ) );

			//{{ 2009. 8. 7  ������		����
			if( spUser->IsEnoughItemExistOnlyInventory( mapItemInfo ) == false )
			{
				START_LOG( cerr, L"������ ����Ʈ�� �ƴѵ� ����üũ�� ���ƿԴ�?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
			//}}
		}
		else
		{
			if( pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == false )
			{
				//{{ 2009. 4. 6  ������		�����α� �߰�
				START_LOG( cerr, L"������ ����Ʈ�� �ƴѵ� ����üũ�� ���ƿԴ�?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< END_LOG;
				//}}

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
		}
	}

	//�Ϸ� ����Ʈ ����ó��
	if( NetError::GetLastError() == NetError::NET_OK )
	{
		//QUEST REWARD DB ó��
		KDBE_QUEST_COMPLETE_REQ kDBReq;
		kDBReq.m_bAutoComplete = kReq.m_bAutoComplete;
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_bIsRepeat	= ( pQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_NORMAL );	
		kDBReq.m_bIsNew		= false;
		kDBReq.m_bIsChangeJob = false;

#ifdef SERV_SKILL_PAGE_SYSTEM
		kDBReq.m_iTheNumberOfSkillPagesAvailable 
			= spUser->GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB &&
			pQuestTemplet->m_Reward.m_eChangeUnitClass != CXSLUnit::UC_NONE &&
			spUser->GetUnitClass() != pQuestTemplet->m_Reward.m_eChangeUnitClass )
		{
			kDBReq.m_bIsChangeJob = true;
			kDBReq.m_cChangeUnitClass = pQuestTemplet->m_Reward.m_eChangeUnitClass;

			if( kDBReq.m_cChangeUnitClass == spUser->GetUnitClass() )
			{
				START_LOG( cerr, L"�����ҷ��� ����Ʈ ���� Unit Class�� ���� class�� ������.?" )
					<< BUILD_LOG( spUser->GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT					
					<< BUILD_LOGc( spUser->GetUnitClass() )
					<< BUILD_LOG( kDBReq.m_cChangeUnitClass )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_18 );
				goto error_proc;
			}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			int iDefaultSkill[6] = {0,};
			if( SiCXSLSkillTree()->GetUnitClassDefaultSkill( kDBReq.m_cChangeUnitClass, iDefaultSkill[0], iDefaultSkill[1], iDefaultSkill[2], iDefaultSkill[3], iDefaultSkill[4], iDefaultSkill[5] ) == false )
			{
				START_LOG( cerr, L"������ �����Ϸ��� �⺻ ��ų�� �̻���" )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOGc( spUser->GetUnitClass() )
					<< BUILD_LOG( kDBReq.m_cChangeUnitClass )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_18 );
				goto error_proc;
			}

			// iDefaultSkill[0], iDefaultSkill[1] ->  �� ��ų���� ������ ��ų �̹Ƿ� ����Ʈ�� ���� ������ ���� �ƴϴ�.
			if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kDBReq.m_cChangeUnitClass) ) == true )
			{
				kDBReq.m_iNewDefaultSkill1 = iDefaultSkill[2];
				kDBReq.m_iNewDefaultSkill2 = iDefaultSkill[3];
				
			}
			else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kDBReq.m_cChangeUnitClass) ) == true )
			{
				kDBReq.m_iNewDefaultSkill1 = iDefaultSkill[4];
				kDBReq.m_iNewDefaultSkill2 = iDefaultSkill[5];
			}
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			
		}

		std::map<int,KItemInfo> mapItem;

		//���ú��� ó���κ�..
		if( kReq.m_mapSelectItemID.size() > 0 )
		{
			//���ú��� ������ �ִ뼱�� �������� ������ ����ó��..
			if( (int)kReq.m_mapSelectItemID.size() > pQuestTemplet->m_SelectReward.m_iSelectionCount )
			{
				START_LOG( cerr, L"����Ʈ ���ú��� ������ �ִ뼱�� �������� ����.!" )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< BUILD_LOG( pQuestTemplet->m_SelectReward.m_iSelectionCount )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_14 );
				goto error_proc;
			}

			bool bIsSelectItem	= false;
			std::map<int,int>::const_iterator mit;

			//���ú��� �����۵��� ������ ������.
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestSelectReward, pQuestTemplet->m_SelectReward.m_vecSelectItem )
			{
				mit = kReq.m_mapSelectItemID.find( kQuestSelectReward.m_iItemID );
				if( mit != kReq.m_mapSelectItemID.end() )
				{
					bIsSelectItem = true;

					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestSelectReward.m_iItemID );
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"������ ���ø� ������ �̻�.!" )
							<< BUILD_LOG( kQuestSelectReward.m_iItemID )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;
						continue;
					}

					KItemInfo kItemInfo;
					kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
					kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
					kItemInfo.m_iQuantity	= kQuestSelectReward.m_iQuantity;

					//����������� �Ⱓ�� �ϰ��..
					if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
					{
						//�����ϼ��� �ִ��� �˻��Ͽ� ���ڸ� �������ش�.
						if( kQuestSelectReward.m_iPeriod > 0 )
						{
							kItemInfo.m_sPeriod = static_cast<short>(kQuestSelectReward.m_iPeriod);
						}
					}

					//{{ 2008. 8. 25  ������	��������ۿ� ���Ͽɼ��� �ִ°��
					const int iRewardSocketOption = kQuestSelectReward.m_iSocketOption1;
					if( iRewardSocketOption > 0 )
					{
						if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
						{
							kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
						}
						else
						{
							START_LOG( cerr, L"�������� �ʴ� ���Ͽɼ��� ����Ʈ ���� �����ۿ� ������ �߽��ϴ�." )											
								<< BUILD_LOG( kReq.m_iQuestID )
								<< BUILD_LOG( kItemInfo.m_iItemID )
								<< BUILD_LOG( iRewardSocketOption )
								<< END_LOG;
						}
					}

					mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );

					START_LOG( clog, L"����Ʈ ���� ������ ó��" )
						<< BUILD_LOG( kQuestSelectReward.m_iItemID );
				}
			}

			if( bIsSelectItem == false )
			{
				SET_ERROR( ERR_QUEST_15 );

				START_LOG( cerr, L"����Ʈ ���ø����� ���ú��� ������ ã�� ����.!" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< END_LOG;

				goto error_proc;
			}
		}

		//�Ϲݺ��� ó���κ�..
		if( pQuestTemplet->m_Reward.m_vecItem.size() > 0 )
		{
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestRewardData, pQuestTemplet->m_Reward.m_vecItem )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestRewardData.m_iItemID );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"������ ���ø� ������ �̻�.!" )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( kQuestRewardData.m_iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( spUser->GetCharUID() )
#else
						<< BUILD_LOG( spUser->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT						
						<< END_LOG;
					continue;
				}

				KItemInfo kItemInfo;
				kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
				kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
				kItemInfo.m_iQuantity	= kQuestRewardData.m_iQuantity;

				//����������� �Ⱓ�� �ϰ��..
				if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
				{
					//�Ⱓ�� �ִ��� �˻��Ͽ� ���ڸ� �������ش�.
					if( kQuestRewardData.m_iPeriod > 0 )
					{
						kItemInfo.m_sPeriod = static_cast<short>(kQuestRewardData.m_iPeriod);
					}
				}

				//{{ 2008. 8. 25  ������	��������ۿ� ���Ͽɼ��� �ִ°��
				const int iRewardSocketOption = kQuestRewardData.m_iSocketOption1;
				if( iRewardSocketOption > 0 )
				{
					if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
					{
						kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
					}
					else
					{
						START_LOG( cerr, L"�������� �ʴ� ���Ͽɼ��� ����Ʈ ���� �����ۿ� ������ �߽��ϴ�." )											
							<< BUILD_LOG( kReq.m_iQuestID )
							<< BUILD_LOG( kItemInfo.m_iItemID )
							<< BUILD_LOG( iRewardSocketOption )
							<< END_LOG;
					}
				}

				mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );//pQuestTemplet->m_Reward.m_vecItem[i].m_iItemID, pQuestTemplet->m_Reward.m_vecItem[i].m_iQuantity ) );
			}
		}

		if( mapItem.size() > 0 || SiCXSLQuestManager()->IsSubItemCondition(kDBReq.m_iQuestID) == true )
		{
			std::map<int,int> mapQuestConditionItem;
			kDBReq.m_mapInsertedItem = mapItem;	//������� ������ ����Ʈ.

			//��ڸ� ������ ��������Ʈ ��쵵 �ش� �������� �������� �ʱ�����.
			if( bIsAdmin == false )
			{
				SiCXSLQuestManager()->GetQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
			}

			std::map< UidType, int > mapDummy;
			if( spUser->m_kInventory.DeleteAndInsert( mapQuestConditionItem, mapDummy, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo, true, KDeletedItemInfo::DR_QUEST_COMPLETE ) == true )
			{
				if( kDBReq.m_vecItemInfo.empty() == false )
				{
					kDBReq.m_bIsNew = true;
					spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
					spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
				}
				else
				{
					kDBReq.m_bIsNew = false;
				}
			}
			else
			{
				
				if( bIsVisitVillageQuest )
				{
					std::map< int, KItemInfo >::const_iterator mit;
					for( mit = kDBReq.m_mapInsertedItem.begin(); mit != kDBReq.m_mapInsertedItem.end(); ++mit )
					{
						// �������� ����!
						spUser->SendQuestRewardLetter( kDBReq.m_iQuestID, mit->second.m_iItemID, mit->second.m_iQuantity );
					}

					// �ӽ� �κ����� ���޵Ǿ��� ������ ���� �κ��� ���õ� �����ʹ� ��� �����!
					kDBReq.m_mapInsertedItem.clear();
				}
				else
				{
					//�κ��� �����ϸ� �׳� �������� �˷��ְ� �ϷḦ ���� �ʴ´�.
					SET_ERROR( ERR_QUEST_13 );
					goto error_proc;
				}
			}

			//{{ 2007. 11. 25  ������  ������� ����Ʈ ���� ������ ������
			std::map< int, int >::iterator mit = mapQuestConditionItem.begin();
			for( ; mit != mapQuestConditionItem.end(); mit++ )
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( mit->first );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, mit->second );
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, mit->second );

			}
			//}}
		}

		//{{ 2007. 12. 26  ������  DB ��� ����Ʈ
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Complete, 1 );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_CompleteLevel, static_cast<int>(spUser->GetLevel()) );

			//{{ 2007. 12. 26  ������  ���� ���
			int iColumnIndex;

			if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
			{
				iColumnIndex = KUserStatistics::US_Etc_NQuestComplete;
			}
			else
			{
				iColumnIndex = KUserStatistics::US_Etc_RQuestComplete;
			}

			spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
			//}}
		}
		//}}

		spUser->SendToGameDB( DBE_QUEST_COMPLETE_REQ, kDBReq );

		return;
	}

error_proc:
	
	if( kReq.m_bAutoComplete )
	{
		// �ʱ�ȭ
		ClearAutoCompleteQuest();

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kAck;
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kAck );
	}
	else
	{
		KEGS_QUEST_COMPLETE_ACK kAck;
		//�Ϸ�üũ�� ���� ����Ʈ ID�� �ٽ� �����ش�.
		kAck.m_kCompleteQuestInfo.m_iQuestID = kReq.m_iQuestID;
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
	}
}

#else

void KUserQuestManager::Handler_EGS_QUEST_COMPLETE_REQ( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin /*= false*/ )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ���������� Ȯ���Ѵ�.	
	if( IsQuest( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ����Ʈ �ν��Ͻ� ���
	KQuestInstance* pKQuestInst = GetQuestInstance( kReq.m_iQuestID );
	if( pKQuestInst == NULL )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ����Ʈ ���ø� ���
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"����Ʈ �������ε� ���ø� �����Ͱ� ����." )
			<< BUILD_LOG( kReq.m_iQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetUID() )
#else
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SET_ERROR( ERR_QUEST_08 );
		goto error_proc;
	}

	//{{ 2010. 01. 29  ������	PC�� ���� ����Ʈ
#ifdef SERV_PC_BANG_QUEST

	// PC�� ���� ����Ʈ ���� �˻�
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_20 );
			goto error_proc;
		}
	}

#endif SERV_PC_BANG_QUEST
	//}}

	//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

	// �̺�Ʈ ����Ʈ��� ���� �������� �̺�Ʈ���� Ȯ���Ѵ�.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// ���� �������� �̺�Ʈ���� Ȯ��!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

#endif SERV_DAY_QUEST
	//}}

#ifdef SERV_EPIC_QUEST
	// ���� ����Ʈ �ϰ�� ������ �ȵǸ� ����Ʈ �Ϸ� �Ҽ�����.
	if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_Condition.m_iLevel > spUser->GetLevel()
		)
		goto error_proc;
#endif SERV_EPIC_QUEST

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	if( pQuestTemplet->m_iAfterQuestID != 0 )
	{
		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pQuestTemplet->m_iAfterQuestID );
		if( pAfterQuestTemplet == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ����Ʈ�� after����Ʈ�� ��ϵǾ��ֽ��ϴ�!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( pQuestTemplet->m_iAfterQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		// ���� ������ �ִ��� Ȯ���غ���!
		if( pAfterQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// ����Ʈ ���� ������ �κ��丮�� ���� ������ ����Ʈ ���� ����
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pAfterQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pAfterQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"����Ʈ ������ �޴� ������ �̹� �κ��丮�� ������ ����Ʈ ���� ����." )
						<< BUILD_LOG( pAfterQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_22 );
					goto error_proc;
				}
			}

			// ����Ʈ ���� ������ ������ �󽽷��� �ִ��� �˻�
			if( spUser->IsEnoughSpaceExist( pAfterQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"����Ʈ ������ �޴� ������ �� �κ��丮 ������� �����մϴ�." )
					<< BUILD_LOG( pAfterQuestTemplet->m_iID )
					<< BUILD_LOG( pAfterQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_23 );
				goto error_proc;
			}
		}
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool bIsVisitVillageQuest = false;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	for( int iIndexSub = 0; iIndexSub < (int)pKQuestInst->m_vecSubQuestInstance.size(); ++iIndexSub )
	{
		// item ���� ����Ʈ ����Ϸ� üũ�� ������ �־����.(�Ϸ�üũ�ÿ��� �ϱ⶧��)
		// ������ ������ ������ ��� ���� ���� �Ϸ�üũ�� �ϴ� �������� üũ�ؾ� �ϱ� ����

		const int iSubQuestID = pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_iID;

		const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
		if( pSubQuestTemplet == NULL )
		{
			START_LOG( cerr, L"����Ʈ �������ε� �������ø� �����Ͱ� ����." )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_VISIT_VILLAGE )
			bIsVisitVillageQuest = true;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//��� ����̸� �������� �ʴ´�.
		if( bIsAdmin == true )
			continue;

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION  ||
			pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
		{
			//�����ؾ��� ������(������ŭ) �ִ��� Ȯ���Ͽ� ��������Ʈ �Ϸ�üũ�� �Ѵ�.
			std::map<int,int> mapItemInfo;
			mapItemInfo.insert( std::make_pair( pSubQuestTemplet->m_ClearCondition.m_iItemID, pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum ) );

			//{{ 2009. 8. 7  ������		����
			if( spUser->IsEnoughItemExistOnlyInventory( mapItemInfo ) == false )
			{
				START_LOG( cerr, L"������ ����Ʈ�� �ƴѵ� ����üũ�� ���ƿԴ�?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
			//}}
		}
#ifdef SERV_POINT_COUNT_SYSTEM
		else if ( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_POINT_COUNT )
		{

		}
#endif //SERV_POINT_COUNT_SYSTEM
		else
		{
			if( pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == false )
			{
				//{{ 2009. 4. 6  ������		�����α� �߰�
				START_LOG( cerr, L"������ ����Ʈ�� �ƴѵ� ����üũ�� ���ƿԴ�?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;
				//}}

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
		}
	}

	//�Ϸ� ����Ʈ ����ó��
	if( NetError::GetLastError() == NetError::NET_OK )
	{
		//QUEST REWARD DB ó��
		KDBE_QUEST_COMPLETE_REQ kDBReq;
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		kDBReq.m_bAutoComplete = kReq.m_bAutoComplete;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		//{{ 2010. 04. 01  ������	��д��� ����
		//#ifdef SERV_DAILY_QUEST
		kDBReq.m_bIsRepeat	= ( pQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_NORMAL );
		//#else
		//		kDBReq.m_bIsRepeat	= pQuestTemplet->m_bRepeat;
		//#endif SERV_DAILY_QUEST
		//}}		
		kDBReq.m_bIsNew		= false;
		kDBReq.m_bIsChangeJob = false;

#ifdef SERV_SKILL_PAGE_SYSTEM
		kDBReq.m_iTheNumberOfSkillPagesAvailable = spUser->GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB &&
			pQuestTemplet->m_Reward.m_eChangeUnitClass != CXSLUnit::UC_NONE &&
			spUser->GetUnitClass() != pQuestTemplet->m_Reward.m_eChangeUnitClass )
		{
			kDBReq.m_bIsChangeJob = true;
			kDBReq.m_cChangeUnitClass = pQuestTemplet->m_Reward.m_eChangeUnitClass;

			if( kDBReq.m_cChangeUnitClass == spUser->GetUnitClass() )
			{
				START_LOG( cerr, L"�����ҷ��� ����Ʈ ���� Unit Class�� ���� class�� ������.?" )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOGc( spUser->GetUnitClass() )
					<< BUILD_LOG( kDBReq.m_cChangeUnitClass )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_18 );
				goto error_proc;
			}
		}

		std::map<int,KItemInfo> mapItem;

		//���ú��� ó���κ�..
		if( kReq.m_mapSelectItemID.size() > 0 )
		{
			//���ú��� ������ �ִ뼱�� �������� ������ ����ó��..
			if( (int)kReq.m_mapSelectItemID.size() > pQuestTemplet->m_SelectReward.m_iSelectionCount )
			{
				START_LOG( cerr, L"����Ʈ ���ú��� ������ �ִ뼱�� �������� ����.!" )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< BUILD_LOG( pQuestTemplet->m_SelectReward.m_iSelectionCount )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_14 );
				goto error_proc;
			}

			bool bIsSelectItem	= false;
			std::map<int,int>::const_iterator mit;

			//���ú��� �����۵��� ������ ������.
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestSelectReward, pQuestTemplet->m_SelectReward.m_vecSelectItem )
			{
				mit = kReq.m_mapSelectItemID.find( kQuestSelectReward.m_iItemID );
				if( mit != kReq.m_mapSelectItemID.end() )
				{
					bIsSelectItem = true;

					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestSelectReward.m_iItemID );
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"������ ���ø� ������ �̻�.!" )
							<< BUILD_LOG( kQuestSelectReward.m_iItemID )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;
						continue;
					}

					KItemInfo kItemInfo;
					kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
					kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
					kItemInfo.m_iQuantity	= kQuestSelectReward.m_iQuantity;

					//����������� �Ⱓ�� �ϰ��..
					if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
					{
						//�����ϼ��� �ִ��� �˻��Ͽ� ���ڸ� �������ش�.
						if( kQuestSelectReward.m_iPeriod > 0 )
						{
							kItemInfo.m_sPeriod = static_cast<short>(kQuestSelectReward.m_iPeriod);
						}
					}

					//{{ 2008. 8. 25  ������	��������ۿ� ���Ͽɼ��� �ִ°��
					//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
					//#ifdef SERV_ITEM_OPTION_DATA_SIZE
					const int iRewardSocketOption = kQuestSelectReward.m_iSocketOption1;
					if( iRewardSocketOption > 0 )
					{
						if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
						{
							kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
						}
						else
						{
							START_LOG( cerr, L"�������� �ʴ� ���Ͽɼ��� ����Ʈ ���� �����ۿ� ������ �߽��ϴ�." )											
								<< BUILD_LOG( kReq.m_iQuestID )
								<< BUILD_LOG( kItemInfo.m_iItemID )
								<< BUILD_LOG( iRewardSocketOption )
								<< END_LOG;
						}
					}
					//#else
					//					short sRewardSocketOption = pQuestTemplet->m_SelectReward.m_vecSelectItem[i].m_sSocketOption1;
					//					if( sRewardSocketOption > 0 )
					//					{
					//						if( SiCXSLSocketItem()->GetSocketData( sRewardSocketOption ) != NULL )
					//						{
					//							kItemInfo.m_vecItemSocket.push_back( sRewardSocketOption );
					//						}
					//						else
					//						{
					//							START_LOG( cerr, L"�������� �ʴ� ���Ͽɼ��� ����Ʈ ���� �����ۿ� ������ �߽��ϴ�." )
					//								<< BUILD_LOG( kReq.m_iQuestID )
					//								<< BUILD_LOG( kItemInfo.m_iItemID )
					//								<< BUILD_LOG( sRewardSocketOption )
					//								<< END_LOG;
					//						}
					//					}
					//#endif SERV_ITEM_OPTION_DATA_SIZE
					//}} 
					//}}

					//{{ 2012. 11. 04	�ڼ���	����Ʈ ���� ������ ��ȭ ���� ����
#ifdef SERV_QUEST_REWARD_ITEM_ENCHANT
					kItemInfo.m_cEnchantLevel	= kQuestSelectReward.m_cEnchantLevel;
#endif SERV_QUEST_REWARD_ITEM_ENCHANT
					//}}

					mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );//pQuestTemplet->m_SelectReward.m_vecSelectItem[i].m_iItemID, pQuestTemplet->m_SelectReward.m_vecSelectItem[i].m_iQuantity ) );

					START_LOG( clog, L"����Ʈ ���� ������ ó��" )
						<< BUILD_LOG( kQuestSelectReward.m_iItemID );
				}
			}

			if( bIsSelectItem == false )
			{
				SET_ERROR( ERR_QUEST_15 );

				START_LOG( cerr, L"����Ʈ ���ø����� ���ú��� ������ ã�� ����.!" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< END_LOG;

				goto error_proc;
			}
		}

		//�Ϲݺ��� ó���κ�..
		if( pQuestTemplet->m_Reward.m_vecItem.size() > 0 )// || SiCXSLQuestManager()->IsSubItemCondition(kDBReq.m_iQuestID) == true )
		{
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestRewardData, pQuestTemplet->m_Reward.m_vecItem )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestRewardData.m_iItemID );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"������ ���ø� ������ �̻�.!" )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( kQuestRewardData.m_iItemID )
						<< BUILD_LOG( spUser->GetCharName() )
						<< END_LOG;
					continue;
				}

				KItemInfo kItemInfo;
				kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
				kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
				kItemInfo.m_iQuantity	= kQuestRewardData.m_iQuantity;

				//����������� �Ⱓ�� �ϰ��..
				if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
				{
					//�Ⱓ�� �ִ��� �˻��Ͽ� ���ڸ� �������ش�.
					if( kQuestRewardData.m_iPeriod > 0 )
					{
						kItemInfo.m_sPeriod = static_cast<short>(kQuestRewardData.m_iPeriod);
					}
					//����Ⱓ�� ������ ����ó���� ��.
					//else
					//{
					//	SET_ERROR( ERR_QUEST_14 );

					//	START_LOG( cerr, L"����Ʈ �Ⱓ�������� ����ó���� �Ⱓ������ ����.!" )
					//		<< BUILD_LOG( pQuestTemplet->m_iID )
					//		<< BUILD_LOG( pQuestTemplet->m_Reward.m_vecItem[i].m_iPeriod )
					//		<< END_LOG;

					//	goto error_proc;
					//}
				}

				//{{ 2008. 8. 25  ������	��������ۿ� ���Ͽɼ��� �ִ°��
				//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
				//#ifdef SERV_ITEM_OPTION_DATA_SIZE
				const int iRewardSocketOption = kQuestRewardData.m_iSocketOption1;
				if( iRewardSocketOption > 0 )
				{
					if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
					{
						kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
					}
					else
					{
						START_LOG( cerr, L"�������� �ʴ� ���Ͽɼ��� ����Ʈ ���� �����ۿ� ������ �߽��ϴ�." )											
							<< BUILD_LOG( kReq.m_iQuestID )
							<< BUILD_LOG( kItemInfo.m_iItemID )
							<< BUILD_LOG( iRewardSocketOption )
							<< END_LOG;
					}
				}
				//#else
				//				short sRewardSocketOption = pQuestTemplet->m_Reward.m_vecItem[i].m_sSocketOption1;
				//				if( sRewardSocketOption > 0 )
				//				{
				//					if( SiCXSLSocketItem()->GetSocketData( sRewardSocketOption ) != NULL )
				//					{
				//						kItemInfo.m_vecItemSocket.push_back( sRewardSocketOption );
				//					}
				//					else
				//					{
				//						START_LOG( cerr, L"�������� �ʴ� ���Ͽɼ��� ����Ʈ ���� �����ۿ� ������ �߽��ϴ�." )
				//							<< BUILD_LOG( kReq.m_iQuestID )
				//							<< BUILD_LOG( kItemInfo.m_iItemID )
				//							<< BUILD_LOG( sRewardSocketOption )
				//							<< END_LOG;
				//					}
				//				}
				//#endif SERV_ITEM_OPTION_DATA_SIZE
				//}} 
				//}}

				mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );//pQuestTemplet->m_Reward.m_vecItem[i].m_iItemID, pQuestTemplet->m_Reward.m_vecItem[i].m_iQuantity ) );
			}
		}

		if( mapItem.size() > 0 || SiCXSLQuestManager()->IsSubItemCondition(kDBReq.m_iQuestID) == true )
		{
			std::map<int,int> mapQuestConditionItem;
			kDBReq.m_mapInsertedItem = mapItem;	//������� ������ ����Ʈ.

			//��ڸ� ������ ��������Ʈ ��쵵 �ش� �������� �������� �ʱ�����.
			if( bIsAdmin == false )
			{
				SiCXSLQuestManager()->GetQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
			}

			std::map< UidType, int > mapDummy;
			if( spUser->m_kInventory.DeleteAndInsert( mapQuestConditionItem, mapDummy, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo, true, KDeletedItemInfo::DR_QUEST_COMPLETE ) == true )
			{
				if( kDBReq.m_vecItemInfo.empty() == false )
				{
					kDBReq.m_bIsNew = true;
					spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
					spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
				}
				else
				{
					kDBReq.m_bIsNew = false;
				}
			}
			else
			{
				//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
				if( bIsVisitVillageQuest )
				{
					std::map< int, KItemInfo >::const_iterator mit;
					for( mit = kDBReq.m_mapInsertedItem.begin(); mit != kDBReq.m_mapInsertedItem.end(); ++mit )
					{
						// �������� ����!
						spUser->SendQuestRewardLetter( kDBReq.m_iQuestID, mit->second.m_iItemID, mit->second.m_iQuantity );
					}

					// �ӽ� �κ����� ���޵Ǿ��� ������ ���� �κ��� ���õ� �����ʹ� ��� �����!
					kDBReq.m_mapInsertedItem.clear();
				}
				else
				{
					//�κ��� �����ϸ� �׳� �������� �˷��ְ� �ϷḦ ���� �ʴ´�.
					SET_ERROR( ERR_QUEST_13 );
					goto error_proc;
				}
#else
				//�κ��� �����ϸ� �׳� �������� �˷��ְ� �ϷḦ ���� �ʴ´�.
				SET_ERROR( ERR_QUEST_13 );
				goto error_proc;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}
			}

			//{{ 2007. 11. 25  ������  ������� ����Ʈ ���� ������ ������
			std::map< int, int >::iterator mit = mapQuestConditionItem.begin();
			for( ; mit != mapQuestConditionItem.end(); mit++ )
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( mit->first );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, mit->second );
				//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
				//}}
			}
			//}}
		}

		//{{ 2007. 12. 26  ������  DB ��� ����Ʈ
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Complete, 1 );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_CompleteLevel, static_cast<int>(spUser->GetLevel()) );

			//{{ 2007. 12. 26  ������  ���� ���
			int iColumnIndex;
			//{{ 2010. 04. 01  ������	��д��� ����
#ifdef SERV_DAILY_QUEST
			if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
#else
			if( pQuestTemplet->m_bRepeat == false )
#endif SERV_DAILY_QUEST
				//}}
			{
				iColumnIndex = KUserStatistics::US_Etc_NQuestComplete;
			}
			else
			{
				iColumnIndex = KUserStatistics::US_Etc_RQuestComplete;
			}

			spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
			//}}
		}
		//}}

		spUser->SendToGameDB( DBE_QUEST_COMPLETE_REQ, kDBReq );
		return;
	}

error_proc:
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( kReq.m_bAutoComplete )
	{
		// �ʱ�ȭ
		ClearAutoCompleteQuest();

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kAck;
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kAck );
	}
	else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
	{
		KEGS_QUEST_COMPLETE_ACK kAck;
		//�Ϸ�üũ�� ���� ����Ʈ ID�� �ٽ� �����ش�.
		//{{ 2010. 04. 05  ������	��������Ʈ
		//#ifdef SERV_DAILY_QUEST
		kAck.m_kCompleteQuestInfo.m_iQuestID = kReq.m_iQuestID;
		//#else
		//		kAck.m_iQuestID	= kReq.m_iQuestID;
		//#endif SERV_DAILY_QUEST
		//}}
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
	}
}

#endif SERV_REFORM_QUEST
//}}

//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST

void KUserQuestManager::Handler_DBE_QUEST_COMPLETE_ACK( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser )
{
	const int iQuestID = kAck.m_kCompleteQuestInfo.m_iQuestID;

	//DB���� ���ó�� ������ ���
	if( kAck.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( kAck.m_bAutoComplete )
		{
			// �ʱ�ȭ
			ClearAutoCompleteQuest();

			KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacket;
			kPacket.m_iOK = kAck.m_iOK;
			spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacket );
		}
		else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		{
			KEGS_QUEST_COMPLETE_ACK	kPacket;
			kPacket.m_iOK = kAck.m_iOK;
			spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
		}
		return;
	}

	//DBó�� ������ ���
	SET_ERROR( NET_OK );

	KEGS_QUEST_COMPLETE_ACK	kPacket;
	kPacket.m_kCompleteQuestInfo.m_iQuestID = iQuestID;

	if( IsQuest( iQuestID ) == false )
	{
		START_LOG( cerr, L"+��+;; �Ϸ��� ����Ʈ�� ������ ����Ʈ�� ����." )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );
		goto end_proc;
	}
	
	START_LOG( clog, L"����Ʈ �Ϸ� ::::" )
		<< BUILD_LOG( spUser->GetCharUID() )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( spUser->GetUID() )
#else
		<< BUILD_LOG( spUser->GetCharName() )
		<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( iQuestID );

	// Quest Instance ����
	if( RemoveQuest( iQuestID ) == true )
	{
		// �Ϸ� ó��
		std::map< int, KCompleteQuestInfo >::iterator mitCQ;
		mitCQ = m_mapCompleteQuest.find( iQuestID );
		if( mitCQ == m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest.insert( std::make_pair( iQuestID, kAck.m_kCompleteQuestInfo ) );
		}
		else
		{
			++(mitCQ->second.m_iCompleteCount); // �Ϸ� ī��Ʈ�� �ø���..
			mitCQ->second.m_tCompleteDate = kAck.m_kCompleteQuestInfo.m_tCompleteDate; // �Ϸ� ��¥�� �����Ѵ�.
		}

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto end_proc;
		}		
		
		kPacket.m_iOK		= NetError::GetLastError();
		//spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );

		//����ó��.
		if( kAck.m_bIsChangeJob == true )
		{
			spUser->SetUnitClass( kAck.m_cChangeUnitClass );
			spUser->ResetStat();
		}

		//{{ ���� - ���� ���� ����Ʈ, ĳ�� �۾� 
#ifdef SERV_SHARING_BANK_QUEST_CASH
		// ����Ʈ�� ���� Ȯ�� ����Ʈ���, ���� Ȯ���ߴٴ� sp �� ȣ���ϵ��� ����
		if(pQuestTemplet->m_iID == 60710)
		{
			KDBE_SHARING_BACK_OPEN_REQ kPackToDB;
			kPackToDB.m_iUnitUID = spUser->GetCharUID();
			kPackToDB.m_iUserUID = spUser->GetUID();
			kPackToDB.m_iOpenType = KDBE_SHARING_BACK_OPEN_REQ::SBOT_QUEST;
			spUser->SendToGameDB(DBE_SHARING_BACK_OPEN_REQ, kPackToDB);
		}
#endif SERV_SHARING_BANK_QUEST_CASH
		//}}
		
		//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		spUser->IncreaseED( pQuestTemplet->m_Reward.m_iED, KUserEDManager::ER_IN_ED_QUEST_REWARD );
#else
		const int iBeforeED = spUser->GetED();

		//ĳ���� �Ӽ� ����ó��
		spUser->m_iED	+= pQuestTemplet->m_Reward.m_iED;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		spUser->CheckEDAbuser( KAbuserLogManager::ELS_QUEST_REWARD, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		// DB��� ED
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PQuest, pQuestTemplet->m_Reward.m_iED );

		//{{ 2007. 12. 26  ������  ���� ���
#ifdef SERV_USER_STATISTICS_RENEWAL
		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PQuest, pQuestTemplet->m_Reward.m_iED );
#else //SERV_USER_STATISTICS_RENEWAL
		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PQuestED, pQuestTemplet->m_Reward.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
		//}}

		//{{ 2009. 1. 20  ������	��������ġ ����ó��
		if( spUser->GetLevel() < SiKGameSysVal()->GetLimitsLevel() )
		{
			spUser->m_kEXP.AddExp( pQuestTemplet->m_Reward.m_iEXP );
		}
		//}}

#ifdef SERV_PVP_NEW_SYSTEM // �߸��� ������ �ؿ��� ����
		spUser->AddAPoint( pQuestTemplet->m_Reward.m_iSP );
#else
		spUser->m_iSPoint += pQuestTemplet->m_Reward.m_iSP;
#endif SERV_PVP_NEW_SYSTEM
	
		spUser->CheckCharLevelUp();

#ifdef SERV_POINT_COUNT_SYSTEM	
		if(spUser != NULL && pQuestTemplet != NULL )
		{			
			SetUpdateQuestInstance(spUser);
		}
#endif //SERV_POINT_COUNT_SYSTEM


		//������ ����ó��
		if( kAck.m_bIsNew == true )
		{
			spUser->m_kInventory.InsertItem( kAck.m_mapItemInfo, kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo );
			kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo.insert( kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo.end(), kAck.m_vecUpdatedInventorySlot.begin(), kAck.m_vecUpdatedInventorySlot.end() );						
		}
		else
		{
			kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo	= kAck.m_vecUpdatedInventorySlot;
		}

		//ȹ���� ������ ������ Ŭ���̾�Ʈ������ ��ȯ�Ͽ� �ش�.
		std::map< int, KItemInfo >::const_iterator mitITEM;
		for( mitITEM = kAck.m_mapInsertedItem.begin(); mitITEM != kAck.m_mapInsertedItem.end(); ++mitITEM )
		{
			kPacket.m_kUpdateUnitInfo.m_mapItemObtained.insert( std::make_pair( mitITEM->second.m_iItemID, mitITEM->second.m_iQuantity ) );
		}

		spUser->GetUnitInfo( kPacket.m_kUpdateUnitInfo.m_kUnitInfo );
		kPacket.m_kUpdateUnitInfo.m_kUnitInfo.m_nNowBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)spUser->GetLevel() );
		kPacket.m_kUpdateUnitInfo.m_kUnitInfo.m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)(spUser->GetLevel() + 1) );

		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( kAck.m_bAutoComplete )
		{
			AddAutoCompleteQuestResult( kPacket );
		}
		else
		{
			spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
		}
#else
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		// ��� : Character Info, Quest
		{
			int iCount = 0;
			std::map< int, int >::iterator iO;
			for ( iO = kPacket.m_kUpdateUnitInfo.m_mapItemObtained.begin(); iO != kPacket.m_kUpdateUnitInfo.m_mapItemObtained.end(); ++iO )
			{
				iCount += (*iO).second;
			}

			// ���� ��� ����Ʈ ���� ������ ī��Ʈ
			m_iRewardItemCount += iCount;
		}

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ������ �����
		spUser->CheckItemAbuser( KAbuserLogManager::RS_QUEST, kPacket.m_kUpdateUnitInfo.m_mapItemObtained );
#endif SERV_USER_ABUSER_MANAGER
		//}}

		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		// �Ϸ�� ����Ʈ�� AfterQuestID�� üũ�Ͽ� ���ο� ����Ʈ�� �����Ѵ�!
		CheckEpicQuest_NewQuestByAfterQuest( pQuestTemplet->m_iID, spUser, true, true );

		// BeforeQuestID�� ��ϵ� ������ ���� �Ϸ� ����Ʈ�� �ִ��� Ȯ���Ͽ� ����Ʈ�� �����Ѵ�.
		CheckEpicQuest_NewQuestByBeforeQuest( spUser, true );	

		// �̺�Ʈ �ݺ� ����Ʈ��� ����� �޵��� ���� 
		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT
		 && pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_REPEAT )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pQuestTemplet->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, false ) == false )
			{
				START_LOG( cwarn, L"[����] New Quest Req Fail.!(After)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< END_LOG;
			}
		}
#endif SERV_REFORM_QUEST
		//}}

		return;		
	}	

end_proc:
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( kAck.m_bAutoComplete )
	{
		// �ʱ�ȭ
		ClearAutoCompleteQuest();

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacketAck );
	}
	else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	{
		kPacket.m_iOK = NetError::GetLastError();
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
	}
}

#endif SERV_DAILY_QUEST
//}}

bool KUserQuestManager::IsQuest( const IN int iQuestID )
{
	return (GetQuestInstance( iQuestID ) != NULL );
}

bool KUserQuestManager::IsChangeJobQuest()
{
	std::map<int, KQuestInstance>::iterator	mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );

		if( pQuestTemplet != NULL )
		{
			if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
			{
				return true;
			}
		}
		else
		{
			START_LOG( cerr, L"����Ʈ ���ø� ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}

	return false;
}

//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

bool KUserQuestManager::IsExistEventQuest( IN int iQuestID )
{
	std::map< int, KEventQuestInfo >::const_iterator mit;
	for( mit = m_mapEventQuest.begin(); mit != m_mapEventQuest.end(); ++mit )
	{
        if( mit->second.m_iQuestID == iQuestID )
			return true;
	}

	return false;
}

#endif SERV_DAY_QUEST
//}}

KQuestInstance* KUserQuestManager::GetQuestInstance( const IN int iQuestID )
{
	std::map<int, KQuestInstance>::iterator	mit;
	mit = m_mapQuesting.find( iQuestID );

	if( mit == m_mapQuesting.end() )
		return NULL;

	return &(mit->second);
}

//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST

int KUserQuestManager::GetQuestCompleteCount( IN const int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::const_iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );
	if( mit == m_mapCompleteQuest.end() )
	{
		return 0;
	}

	return mit->second.m_iCompleteCount;
}

#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
bool KUserQuestManager::CheckCompletedQuestToday( IN const int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::const_iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );
	if( mit == m_mapCompleteQuest.end() )
	{
		return false;
	}

	CTime tCompleteDate( mit->second.m_tCompleteDate ); // ���� ����Ʈ�� �Ϸ� �ð�
	CTime tCurTime = CTime::GetCurrentTime();			// ���� �ð�
	CTime tCheckTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );	// ������ ����Ʈ ���� �ð�
	if( tCurTime.GetHour() < CXSLQuestManager::QE_DAILY_REPEAT_HOUR )	// ���� �� < ���� ��
	{
		// 6�ú��� �۴ٸ� �ð��̶�� �������� ����
		tCheckTime -= CTimeSpan( 1, 0, 0, 0 );
	}

	// ���� �ϷḦ �ߴٸ�
	if( tCheckTime < tCompleteDate )
	{
		return true;
	}

	return false; 
}
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX

#else

int KUserQuestManager::GetQuestCompleteCount( IN const int iQuestID )
{
	std::map<int,int>::iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );

	if( mit == m_mapCompleteQuest.end() )
		return 0;

	return (mit->second);
}

#endif SERV_DAILY_QUEST
//}}

void KUserQuestManager::GetUpdatedQuestInfo( OUT std::vector< KQuestUpdate >& vecQuestData )
{
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const KQuestInstance& kQuest = mit->second;

		KQuestUpdate kQuestUpdate;
		kQuestUpdate.m_iQuestID = kQuest.m_iID;

		std::vector< KSubQuestInstance >::const_iterator vit;
		vit = kQuest.m_vecSubQuestInstance.begin();
		//db�� �Ѿ�� �� �����ʹ� ������ �ֱ⶧����(5��) ������ 0���� ä���.
		for( int i = 0; i < CXSLQuestManager::MAX_SUB_QUEST_NUM; ++i )
		{
			if( vit != kQuest.m_vecSubQuestInstance.end() )
			{
				kQuestUpdate.m_vecClearData.push_back( vit->m_ucClearData );

				++vit;
			}
			else
			{
				kQuestUpdate.m_vecClearData.push_back( 0 );
			}
		}

		vecQuestData.push_back( kQuestUpdate );
	}
}

int	KUserQuestManager::GetQuestRewardItemCount()
{
	int iQuestRewardItemCount = m_iRewardItemCount;
	m_iRewardItemCount = 0;
	return iQuestRewardItemCount;
}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::GetOngoingQuestForRoom( IN KGSUserPtr spUser, OUT std::map< int, KSubQuestInfo >& mapOngoingQuest )
{
	mapOngoingQuest.clear();

	std::map< int, KQuestInstance >::iterator mitQ;

	//������ ����Ʈ �˻�
	for( mitQ = m_mapQuesting.begin(); mitQ != m_mapQuesting.end(); ++mitQ )
	{
		const KQuestInstance& kQuestInstance = mitQ->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET�� ����." )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
				<< END_LOG;

			continue;
		}

		// PC�� ���� ����Ʈ�ε� PC�� ������ �ƴ϶�� �������Ʈ����Ʈ�� ���� �ʴ´�.
		if( pQuestTemplet->m_bIsPcBang == true )
		{
			if( spUser->IsPcBang() == false )
				continue;
		}

		//��������Ʈ�� ����Ʈ ������ ������ �ִ��� ã�´�.
		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			KSubQuestInstance kSub;

			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET�� ����." )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
					<< END_LOG;

				continue;
			}

			switch( pSubTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_ITEM_COLLECTION:
			case CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION:
				{
					std::map< int, KSubQuestInfo >::iterator mitQ;
					mitQ = mapOngoingQuest.find( kQuestInstance.m_iID );

					//���� ���� ����Ʈ��� mapDropQuestitembyIngQuest ���� �ٷ� �־��ش�.
					if( mitQ == mapOngoingQuest.end() )
					{
						KSubQuestInfo kInfo;

						// �ش� ����Ʈ�� ���� ����Ʈ���� data�� ���� ��´�.
                        for( u_int uiSub = 0; uiSub < kQuestInstance.m_vecSubQuestInstance.size(); ++uiSub )
						{
							int iQuantity = 0;
							//�ش�������� ����Ȯ��
							if( spUser->m_kInventory.GetQuantity( pSubTemplet->m_ClearCondition.m_iItemID, iQuantity ) == true )
							{								
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = iQuantity;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiSub].m_bIsSuccess;
								kInfo.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiSub].m_iID, kData ) );

							}
							else
							{
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = 0;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiSub].m_bIsSuccess;
								kInfo.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiSub].m_iID, kData ) );
							}
						}

						mapOngoingQuest.insert( std::make_pair( kQuestInstance.m_iID, kInfo ) );
					}
					else	// ������ �ֽ�ȭ �Ѵ�.
					{
						std::map< int, KSubQuestData>::iterator mitS = mitQ->second.m_mapSubQuestInfo.find( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID );
						if( mitS == mitQ->second.m_mapSubQuestInfo.end() )
						{
							int iQuantity = 0;
							//�ش�������� ����Ȯ��
							if( spUser->m_kInventory.GetQuantity( pSubTemplet->m_ClearCondition.m_iItemID, iQuantity ) == true )
							{								
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = iQuantity;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
								mitQ->second.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID, kData ) );

							}
							else
							{
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = 0;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
								mitQ->second.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID, kData ) );
							}
						}
						else
						{
							int iQuantity = 0;
							//�ش�������� ����Ȯ��
							if( spUser->m_kInventory.GetQuantity( pSubTemplet->m_ClearCondition.m_iItemID, iQuantity ) == true )
							{																	
								mitS->second.m_iInInventoryItemCount = iQuantity;
								mitS->second.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
							}
							else
							{
								mitS->second.m_iInInventoryItemCount = 0;
								mitS->second.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
							}
						}
					}
				}
				break;

			case CXSLQuestManager::SQT_NPC_HUNT:
			case CXSLQuestManager::SQT_NPC_TALK:
			case CXSLQuestManager::SQT_DUNGEON_TIME:
			case CXSLQuestManager::SQT_DUNGEON_RANK:
			case CXSLQuestManager::SQT_DUNGEON_DAMAGE:
			case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
			case CXSLQuestManager::SQT_PVP_PLAY:
			case CXSLQuestManager::SQT_PVP_WIN:
			case CXSLQuestManager::SQT_PVP_KILL:
			case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
			case CXSLQuestManager::SQT_ITEM_ENCHANT:
			case CXSLQuestManager::SQT_ITEM_SOCKET:
			case CXSLQuestManager::SQT_ITEM_ATTRIB:
			case CXSLQuestManager::SQT_ITEM_RESOLVE:
			case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
			case CXSLQuestManager::SQT_USE_SKILL_POINT:
			case CXSLQuestManager::SQT_FEED_PET:
			case CXSLQuestManager::SQT_USER_DIE:
			case CXSLQuestManager::SQT_PVP_NPC_HUNT:
			case CXSLQuestManager::SQT_ITEM_USE:
			case CXSLQuestManager::SQT_SKILL_USE:
			case CXSLQuestManager::SQT_VISIT_VILLAGE:
			case CXSLQuestManager::SQT_VISIT_FIELD:
			case CXSLQuestManager::SQT_VISIT_DUNGEON:
			case CXSLQuestManager::SQT_FIND_NPC:
				//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
			case CXSLQuestManager::SQT_LEARN_NEW_SKILL:
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
				//}}
				{
					std::map< int, KSubQuestInfo >::iterator mitQ;
					mitQ = mapOngoingQuest.find( kQuestInstance.m_iID );

					//���� ���� ����Ʈ��� mapDropQuestitembyIngQuest ���� �ٷ� �־��ش�.
					if( mitQ == mapOngoingQuest.end() )
					{
						KSubQuestInfo kInfo;

						// �ش� ����Ʈ�� ���� ����Ʈ���� data�� ���� ��´�.
						for( u_int uiSub = 0; uiSub < kQuestInstance.m_vecSubQuestInstance.size(); ++uiSub )
						{
							KSubQuestData kData;
							kData.m_iInInventoryItemCount = 0;
							kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiSub].m_bIsSuccess;
							kInfo.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiSub].m_iID, kData ) );
						}

						mapOngoingQuest.insert( std::make_pair( kQuestInstance.m_iID, kInfo ) );
					}
					//�̹� �ִ� ����Ʈ��� ���� ����Ʈ�� �����׸� ä���ش�.
					else
					{
						std::map< int, KSubQuestData>::iterator mitS = mitQ->second.m_mapSubQuestInfo.find( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID );
						if( mitS == mitQ->second.m_mapSubQuestInfo.end() )
						{
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = 0;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
								mitQ->second.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID, kData ) );
						}
						else
						{
							mitS->second.m_iInInventoryItemCount = 0;
							mitS->second.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
						}
					}
				}
				break;
			}
		}
	}
}

#else
	//void KUserQuestManager::GetDropQuestitembyIngQuest( OUT std::map< int, KSubQuestInfo >& mapDropQuestitembyIngQuest, IN KGSUserPtr spUser )
#endif SERV_REFORM_QUEST
//}}

//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

void KUserQuestManager::InitEventQuest( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	// �̺�Ʈ ����Ʈ �ʱ�ȭ
	CheckQuestEvent( spUser );

	// �ʱ�ȭ ���� ���� �̺�Ʈ ����Ʈ�� �����Ѵ�!
	std::vector< int > vecDeleteEventQuest;

	std::map< int, KEventQuestInfo >::iterator mit;
	for( mit = m_mapEventQuest.begin(); mit != m_mapEventQuest.end(); ++mit )
	{		
		if( mit->second.m_bInit == false )
		{
            vecDeleteEventQuest.push_back( mit->first );
		}
	}

	for( u_int ui = 0; ui < vecDeleteEventQuest.size(); ++ui )
	{
		m_mapEventQuest.erase( vecDeleteEventQuest[ui] );
	}

	// ���� �������� �̺�Ʈ ����Ʈ�߿� �ʱ�ȭ �ȵ� ����Ʈ�� ã�Ƴ���.
	std::vector< int > vecDeletedQuest;

	std::map< int, KQuestInstance >::const_iterator mitQT;
	for( mitQT = m_mapQuesting.begin(); mitQT != m_mapQuesting.end(); ++mitQT )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitQT->second.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"����Ʈ ���ø� ������ �����ϴ�." )
				<< BUILD_LOG( mitQT->second.m_iID )
				<< END_LOG;

			continue;
		}
		
		// �̺�Ʈ ����Ʈ���� �˻�
		if( pQuestTemplet->m_bIsTimeEvent == false )
			continue;

		// �ʱ�ȭ�� �̺�Ʈ ����Ʈ���� �˻�
		if( IsExistEventQuest( mitQT->second.m_iID ) == true )
			continue;

        // �ʱ�ȭ ���� �ʾҴٸ� �������� ����Ʈ���� ��������!
		vecDeletedQuest.push_back( mitQT->second.m_iID );
	}

	for( u_int ui = 0; ui < vecDeletedQuest.size(); ++ui )
	{
		RemoveQuest( vecDeletedQuest[ui] );

		// DB���� ���� ����
		m_vecReservedGiveUpQuest.push_back( vecDeletedQuest[ui] );
	}

	if( !vecDeletedQuest.empty() )
	{
		KEGS_EVENT_QUEST_INFO_NOT kPacketNotEnd;
		kPacketNotEnd.m_cType = KEGS_EVENT_QUEST_INFO_NOT::EQIT_END;
		kPacketNotEnd.m_vecQuestID = vecDeletedQuest;
		spUser->SendPacket( EGS_EVENT_QUEST_INFO_NOT, kPacketNotEnd );
	}
}

void KUserQuestManager::CheckQuestEvent( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	KEGS_EVENT_QUEST_INFO_NOT kPacketNotBegin;
	KEGS_EVENT_QUEST_INFO_NOT kPacketNotEnd;

    // GameEventManager�κ��� ���� �������� ����Ʈ �̺�Ʈ�� �ִ��� üũ�Ѵ�!
	SiKGameEventManager()->CheckEnableEventQuest( m_mapEventQuest, m_mapCompleteQuest, kPacketNotBegin.m_vecQuestID, kPacketNotEnd.m_vecQuestID );

	// ����Ǵ� �̺�Ʈ�� �ִٸ� ���� Ŭ���̾�Ʈ�� �˸���.
	if( !kPacketNotEnd.m_vecQuestID.empty() )
	{
		for( u_int ui = 0; ui < kPacketNotEnd.m_vecQuestID.size(); ++ui )
		{
			const int iEndQuestID = kPacketNotEnd.m_vecQuestID[ui];

			// ����� �̺�Ʈ ����Ʈ�� ���� �������̶��..
			if( IsQuest( iEndQuestID ) == true )
			{
#ifdef SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
				KEGS_GIVE_UP_QUEST_REQ kPacket;
				kPacket.m_iQuestID = iEndQuestID;
				Handler_EGS_GIVE_UP_QUEST_REQ( kPacket, spUser );
#else // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
				// ����ó��
				RemoveQuest( iEndQuestID );

				// DB���� ���� ����
				m_vecReservedGiveUpQuest.push_back( iEndQuestID );
#endif // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT

				// ����¶�� ��� ����Ʈ ���� ������Ʈ �ϱ� - ����Ʈ�߿� ��� ������ ����Ʈ�� �����Ƿ� ������ �ʿ����.
				//spUser->SendUpdateDropQuestItemByIngQuest();
			}
		}

		kPacketNotEnd.m_cType = KEGS_EVENT_QUEST_INFO_NOT::EQIT_END;
		spUser->SendPacket( EGS_EVENT_QUEST_INFO_NOT, kPacketNotEnd );
	}

	// ���� ���۵Ǵ� �̺�Ʈ�� �ִٸ� Ŭ���̾�Ʈ�� �˸���
	if( !kPacketNotBegin.m_vecQuestID.empty() )
	{
#ifdef SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
		BOOST_TEST_FOREACH( const int&, iQuestID, kPacketNotBegin.m_vecQuestID )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = iQuestID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, false ) == false )
			{
				START_LOG( cwarn, L"[����] New Quest Req Fail.!(After)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( iQuestID )
					<< END_LOG;
			}
		}
#endif // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT

		kPacketNotBegin.m_cType = KEGS_EVENT_QUEST_INFO_NOT::EQIT_BEGIN;
		spUser->SendPacket( EGS_EVENT_QUEST_INFO_NOT, kPacketNotBegin );
	}

#ifdef SERV_CHECK_TIME_QUEST
	if( spUser->GetAuthLevel() == SEnum::UAL_DEVELOPER )
	{
		// ���⼭ ����Ʈ �̺�Ʈ üũ �ؼ�, TimeEvent ���� �ȵ� ����Ʈ�� �˷�����
		KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT kPacket;

		SiKGameEventManager()->GetWarningEventQuest( kPacket );
		if( false == kPacket.m_vecWarningQuestID.empty() )
		{
			// Ŭ�� ������
			spUser->SendPacket( EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT, kPacket );
		}
	}
#endif // SERV_CHECK_TIME_QUEST
}

void KUserQuestManager::GetReservedGiveUpQuest( OUT std::vector< int >& vecGiveUpQuest )
{
	vecGiveUpQuest.clear();
    
	vecGiveUpQuest = m_vecReservedGiveUpQuest;
	m_vecReservedGiveUpQuest.clear();
}

#endif SERV_DAY_QUEST
//}}

//{{ 2010. 04. 05  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST

KCompleteQuestInfo* KUserQuestManager::GetCompleteQuestInfo( IN int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );
	if( mit == m_mapCompleteQuest.end() )
		return NULL;

	return &(mit->second);
}

#endif SERV_DAILY_QUEST
//}}

#ifdef SERV_EPIC_QUEST
void KUserQuestManager::CheckEpicQuest_SuccessStateChange( IN int iMapID, IN KGSUserPtr spUser )
{
	//# iMapID �� ���� ��ȿ�� �˻�� 
	//# EGS_STATE_CHANGE_FIELD_REQ ���� �ϰ� ���Ա� ������ ���� �ʴ´�.
	std::vector<CXSLQuestManager::QuestTemplet*>::const_iterator cvit;
	std::vector<CXSLQuestManager::QuestTemplet*> vecEpicList;

	SiCXSLQuestManager()->GetEpicQuestTempletList(vecEpicList);

	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pTemplet, vecEpicList )
	{
		if( NULL == pTemplet )
			continue;

		// ������ ����Ʈ X
		if( IsQuest( pTemplet->m_iID ) == true )
			continue;

		// �Ϸ�� ����Ʈ X
		if( IsCompleteQuest( pTemplet->m_iID ) == true )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
		if ( SiKGameEventManager()->IsEnableEventQuest(pTemplet->m_iID) == false )
			continue; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
#endif //SERV_ALLOW_EVENT_ERASE

		// ���� ���̵� ������.
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		if( pTemplet->m_Condition.m_setEnableVillage.size() > 0 )
		{
			std::set<int>::const_iterator sit = pTemplet->m_Condition.m_setEnableVillage.find( iMapID );
			if( sit == pTemplet->m_Condition.m_setEnableVillage.end() )
				continue;
		}
		else
			continue;
#else
		if( iMapID != pTemplet->m_Condition.m_iEnableVillage )
			continue;
#endif SERV_REFORM_QUEST
		//}}
				
		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pTemplet->m_iID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

		if( false == Handler_EGS_NEW_QUEST_REQ(kTempReq, spUser) )
		{
			START_LOG( cwarn, L"[����] New Quest Req Fail.!(State Change)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pTemplet->m_iID )
				<< BUILD_LOG( iMapID )
				<< END_LOG;
		}		
	}
}

#endif SERV_EPIC_QUEST

//{{ 2011. 01. 03	������	���� ����Ʈ
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE

void KUserQuestManager::CheckEpicQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser, IN const bool bSendAck )
{
	//{{Begin} ����Ʈ Ŭ����� ��������Ʈ�� BeforeQuestID�� �˻��Ͽ� �ڵ����� ���ִ� ����ۼ�.
	std::vector< CXSLQuestManager::QuestTemplet* > vecEpicQuestTemplet;
	SiCXSLQuestManager()->GetExistBeforeEpicQuestTempletList( vecEpicQuestTemplet );

	// ���� ����Ʈ ����Ʈ�� ��ȸ�ϸ鼭 BeforeQuestID �� �˻���.
	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pEpicQuest, vecEpicQuestTemplet )
	{
		if( pEpicQuest == NULL )
		{
			START_LOG( cerr, L"���� ����Ʈ �����Ͱ��� �̻��մϴ�!" )
				<< BUILD_LOG( vecEpicQuestTemplet.size() )
				<< END_LOG;
			continue;
		}	

		// �Ϸ��� �̸� �˻翡�� ����
		if( IsCompleteQuest( pEpicQuest->m_iID ) == true )
			continue;

		// ������ �̸� �˻翡�� ����
		if( IsQuest( pEpicQuest->m_iID ) == true )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
		if ( SiKGameEventManager()->IsEnableEventQuest(pEpicQuest->m_iID) == false )
			continue; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
#endif //SERV_ALLOW_EVENT_ERASE

		// ���� �������� ������ ������ �ִ� ����Ʈ�� üũ
		bool bIsSuccessEpic = true;

		// oasis907 : ����� [2010.7.10] // m_vecBeforeQuestID�� ������ ������� �˻翡�� ����
		if( pEpicQuest->m_Condition.m_vecBeforeQuestID.size() == 0 )
		{
			bIsSuccessEpic = false;
		}

		BOOST_TEST_FOREACH( const int, iBeforeQuestID, pEpicQuest->m_Condition.m_vecBeforeQuestID )
		{
			if( iBeforeQuestID != 0  &&  IsCompleteQuest( iBeforeQuestID ) == false )
			{
				bIsSuccessEpic = false;
				break;
			}
		}

		if( bIsSuccessEpic )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pEpicQuest->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, bSendAck ) == false )
#endif SERV_REFORM_QUEST
			//}}
			{
				START_LOG( cwarn, L"[����] New Quest Req Fail.!(Before)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( kTempReq.m_iQuestID )
					<< END_LOG;
			}
		}		
	}
}

void KUserQuestManager::CheckEpicQuest_NewQuestByAfterQuest( IN const int iQuestID, IN KGSUserPtr spUser, IN const bool bSendAck, IN const bool bForce )
{
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cwarn, L"�������� �ʴ� ����Ʈ ID�Դϴ�!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;
		return;
	}

	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	// �Ϸ�� ����Ʈ�� �����̶��
	// 	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType )
	// 		return;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	// ��������Ʈ�� �������� �ʴ´ٸ�
	if( pQuestTemplet->m_vecAfterQuestID.size() <= 0 )
		return;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
	if ( SiKGameEventManager()->IsEnableEventQuest(iQuestID) == false )
		return; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
#endif //SERV_ALLOW_EVENT_ERASE

	// ���� ����Ʈ�� ��������Ʈ���� �˻�
	BOOST_TEST_FOREACH( const int, iAfterQuestID, pQuestTemplet->m_vecAfterQuestID )
	{
		//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iAfterQuestID );
		if( IS_NULL( pAfterQuestTemplet ) )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ����Ʈ �����Դϴ�!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( iAfterQuestID )
				<< END_LOG;
			continue;
		}

		// after����Ʈ�� �ݺ��̳� �ƴϳĸ� üũ!
		if( pAfterQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_REPEAT 
#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
			&& pAfterQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_DAY
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX
		)
		{
			// after �� ���� �Ǿ� ������ ���� Ÿ�Կ� ������� ��� ���� �޵��� ����
			if( bForce == false )	// ���� ������ �ƴ϶��
			{
				// ���Ȱ� �̺�Ʈ�� �����ϰ� �Ѵ�.
				if( ( pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EPIC || pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT ) == false )
				{
					START_LOG( clog, L"���� ������ �ƴѵ�(ĳ���ͼ��ý�)...After Quest �� ���õ� ����Ʈ�� ����, �̺�Ʈ�� �ƴϴ�." )
						<< BUILD_LOG( iAfterQuestID )
						<< END_LOG;
					continue;
				}
			}
			
			// �Ϸ��� �̸� �˻翡�� ����
			if( IsCompleteQuest( iAfterQuestID ) == true )
				continue;
		}
#else
		//if( SiCXSLQuestManager()->IsEpicQuest( iAfterQuestID ) == false )
		//{
		//	START_LOG( clog, L"������ ���� ����Ʈ�� ����Ÿ���� �ƴ�" )
		//		<< BUILD_LOG( iAfterQuestID )
		//		<< END_LOG;
		//	continue;
		//}

		//// �Ϸ��� �̸� �˻翡�� ����
		//if( IsCompleteQuest( iAfterQuestID ) == true )
		//	return;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		// ������ �̸� �˻翡�� ����
		if( IsQuest( iAfterQuestID ) == true )
			continue;
	
		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = iAfterQuestID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
		if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, bSendAck ) == false )
		{
			START_LOG( cwarn, L"[����] New Quest Req Fail.!(After)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;
		}
	}

#else
	// ��������Ʈ�� �����Ѵٸ�
	if( pQuestTemplet->m_iAfterQuestID == 0 )
		return;

	// ���� ����Ʈ�� ��������Ʈ���� �˻�
	if( SiCXSLQuestManager()->IsEpicQuest( pQuestTemplet->m_iAfterQuestID ) == false )
	{
		START_LOG( cerr, L"������ ���� ����Ʈ�� ����Ÿ���� �ƴ�" )
			<< BUILD_LOG( pQuestTemplet->m_iAfterQuestID )
			<< END_LOG;
		return;
	}

	// �Ϸ��� �̸� �˻翡�� ����
	if( IsCompleteQuest( pQuestTemplet->m_iAfterQuestID ) == true )
		return;

	// ������ �̸� �˻翡�� ����
	if( IsQuest( pQuestTemplet->m_iAfterQuestID ) == true )
		return;

	{
		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pQuestTemplet->m_iAfterQuestID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
		if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, bSendAck ) == false )
		{
			START_LOG( cwarn, L"[����] New Quest Req Fail.!(After)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;
		}
	}
#endif SERV_REFORM_QUEST
	//}}
}

#endif SERV_EPIC_QUEST_CHECK_UPDATE
//}}

void KUserQuestManager::ClearUnitQuest()
{
	m_mapQuesting.clear();
	m_mapCompleteQuest.clear();
	//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST
	m_mapEventQuest.clear();
	m_vecReservedGiveUpQuest.clear();
#endif SERV_DAY_QUEST
	//}}
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	ClearAutoCompleteQuest();
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
}


#ifdef SERV_EPIC_QUEST
int KUserQuestManager::GetNormalQuestSize()
{
	int iSize = 0;
	std::map< int, KQuestInstance >::const_iterator cmit;

	cmit = m_mapQuesting.begin();

	while(cmit != m_mapQuesting.end())
	{
		const CXSLQuestManager::QuestTemplet *pTemplet = SiCXSLQuestManager()->GetQuestTemplet(cmit->second.m_iID);
		if( NULL != pTemplet 
		 && CXSLQuestManager::QT_EPIC != pTemplet->m_eQuestType 
		 && CXSLQuestManager::QT_EVENT != pTemplet->m_eQuestType )
			++iSize;

		++cmit;
	}

	return iSize;
}
#endif SERV_EPIC_QUEST

//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
bool KUserQuestManager::ExistHaveExpInDungeon( IN UidType CharUID, IN const std::map< UidType, bool >& mapHaveExpInDungeon )
{
	std::map< UidType, bool >::const_iterator mit = mapHaveExpInDungeon.find( CharUID );
	if( mit == mapHaveExpInDungeon.end() )
	{
		START_LOG( cwarn, L"����ġ ������ ����. ����ġ�� ����� �����ΰ�?" )
			<< BUILD_LOG( CharUID )
			<< BUILD_LOG( mapHaveExpInDungeon.size() )
			<< END_LOG;
		return false;
	}

	return mit->second;
}
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
//}}

//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
void KUserQuestManager::GetUserAllQuest( OUT std::set< int >& mapPaymentQuest )
{
	mapPaymentQuest.clear();

	std::map< int, KQuestInstance >::iterator mitQ;

	//������ ����Ʈ �˻�
	for( mitQ = m_mapQuesting.begin(); mitQ != m_mapQuesting.end(); ++mitQ )
	{
		const KQuestInstance& kQuestInstance = mitQ->second;
		mapPaymentQuest.insert( kQuestInstance.m_iID );
	}
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
void KUserQuestManager::GetUserGoingQuest( IN KGSUserPtr spUser, OUT std::set< int >& mapPaymentQuest )
{
	mapPaymentQuest.clear();

	std::map< int, KQuestInstance >::iterator mitQ;

	//������ ����Ʈ �˻�
	for( mitQ = m_mapQuesting.begin(); mitQ != m_mapQuesting.end(); ++mitQ )
	{
		const KQuestInstance& kQuestInstance = mitQ->second;

		if( CheckIsGoingComplete( kQuestInstance.m_iID, spUser ) == true )
			mapPaymentQuest.insert( kQuestInstance.m_iID );
	}
}
bool KUserQuestManager::CheckIsGoingComplete( IN const int iQuestID, IN KGSUserPtr spUser )
{
	if( m_mapQuesting.empty() == true )
	{
		return false;
	}

	// ���������� Ȯ���Ѵ�.	
	if( IsQuest( iQuestID ) == false )
	{
		return false;
	}

	// ����Ʈ �ν��Ͻ� ���
	KQuestInstance* pKQuestInst = GetQuestInstance( iQuestID );
	if( pKQuestInst == NULL )
	{
		return false;
	}

	// ����Ʈ ���ø� ���
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"����Ʈ �������ε� ���ø� �����Ͱ� ����." )
			<< BUILD_LOG( iQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetUID() )
#else
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		return false;
	}

	// ���� ����Ʈ �ϰ�� ������ �ȵǸ� ����Ʈ �Ϸ� �Ҽ�����.
	if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_Condition.m_iLevel > spUser->GetLevel() )
		return false;

	for( int iIndexSub = 0; iIndexSub < (int)pKQuestInst->m_vecSubQuestInstance.size(); ++iIndexSub )
	{
		// item ���� ����Ʈ ����Ϸ� üũ�� ������ �־����.(�Ϸ�üũ�ÿ��� �ϱ⶧��)
		// ������ ������ ������ ��� ���� ���� �Ϸ�üũ�� �ϴ� �������� üũ�ؾ� �ϱ� ����
		const int iSubQuestID = pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_iID;

		const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
		if( pSubQuestTemplet == NULL )
		{
			START_LOG( cerr, L"����Ʈ �������ε� �������ø� �����Ͱ� ����." )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			return false;
		}

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION  ||
			pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
		{
			//�����ؾ��� ������(������ŭ) �ִ��� Ȯ���Ͽ� ��������Ʈ �Ϸ�üũ�� �Ѵ�.
			std::map<int,int> mapItemInfo;
			mapItemInfo.insert( std::make_pair( pSubQuestTemplet->m_ClearCondition.m_iItemID, pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum ) );

			//{{ 2009. 8. 7  ������		����
			if( spUser->IsEnoughItemExist( mapItemInfo, false ) == true )
			{
				return false;
			}
			//}}
		}
		else
		{
			if( pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
			{
				return false;
			}
		}
	}

	return true;
}
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_SUB_QUEST_USE_ITEM
void KUserQuestManager::CountUseItem( IN KGSUserPtr spUser, IN const int iDungeonID, IN const char cDifficulty, IN const int iItemID )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )		
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ������ ��� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_USE )
				continue;

			// ������ �����Ѵٸ�
			if( pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() == false )
			{
				if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetDungeonIDAndDungeonDifficulty(), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					continue;
			}
			// 4. �ش� ��Ʋ �ʵ尡 �´��� Ȯ��!
			else
			{
				// ��Ʋ �ʵ尡 �����Ѵٸ�
				if( pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
						continue;
				}
			}

			// 3. � ����������?
			std::vector<int>::const_iterator it;
			for( it = pSubQuestTemplet->m_ClearCondition.m_vecUseItemID.begin(); 
				it != pSubQuestTemplet->m_ClearCondition.m_vecUseItemID.end(); ++it )
			{
				if( (*it) == iItemID )
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iUseItemNum > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
						kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
				}
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iUseItemNum <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_SUB_QUEST_USE_ITEM

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
void KUserQuestManager::GetCompleteChangeClassQuest( OUT std::vector< KCompleteQuestInfo >& vecQuest )
{
	std::map<int, KCompleteQuestInfo>::iterator	mit;
	for( mit = m_mapCompleteQuest.begin(); mit != m_mapCompleteQuest.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->second.m_iQuestID );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( mit->second );
		}
		else
		{
			START_LOG( cerr, L"����Ʈ ���ø� ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::GetInProgressClassChangeQuest( OUT std::vector< KQuestInstance >& vecQuest )
{
	std::map<int, KQuestInstance>::iterator	mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( mit->second );
		}
		else
		{
			START_LOG( cerr, L"����Ʈ ���ø� ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::GetCompleteChangeClassQuest( OUT std::vector< int >& vecQuest )
{
	std::map<int, KCompleteQuestInfo>::iterator	mit;
	for( mit = m_mapCompleteQuest.begin(); mit != m_mapCompleteQuest.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->second.m_iQuestID );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( pQuestTemplet->m_iID );
		}
		else
		{
			START_LOG( cerr, L"����Ʈ ���ø� ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::GetInProgressClassChangeQuest( OUT std::vector< int >& vecQuest )
{
	std::map<int, KQuestInstance>::iterator	mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( pQuestTemplet->m_iID );
		}
		else
		{
			START_LOG( cerr, L"����Ʈ ���ø� ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::SetClassChangeQuest( IN UidType iUnitUID, IN std::map< int, int >& mapComplete, IN std::map< int, int >& mapInProgress )
{
	std::map< int, KCompleteQuestInfo >::iterator mitMyComplete;
	std::map< int, KQuestInstance >::iterator mitMyInProgress;
	CTime tCurr = CTime::GetCurrentTime();


	// �Ϸ�� ����Ʈ ��ȯ
	std::map< int, int >::iterator mitComplete = mapComplete.begin();
	for(  ; mitComplete != mapComplete.end() ; ++mitComplete )
	{
		mitMyComplete = m_mapCompleteQuest.find( mitComplete->first );
		if( mitMyComplete != m_mapCompleteQuest.end() )
		{
			KCompleteQuestInfo kTempQuestInfo;
			kTempQuestInfo = mitMyComplete->second;
			kTempQuestInfo.m_iQuestID = mitComplete->second;

			m_mapCompleteQuest.insert( std::make_pair( kTempQuestInfo.m_iQuestID, kTempQuestInfo ) );
			m_mapCompleteQuest.erase( mitMyComplete );
		}
		else
		{
			START_LOG( cerr, L"�̷� �Ϸ�� ����Ʈ�� ������ ���� �ʴٰ�?! �ִٰ� �ؼ� ����Ѱ��ݾ�!!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( mitComplete->first )
				<< BUILD_LOG( mitComplete->second )
				<< END_LOG;
		}
	}

	// ������ ����Ʈ ��ȯ
	std::map< int, int >::iterator mitInProgress = mapInProgress.begin();
	for(  ; mitInProgress != mapInProgress.end() ; ++mitInProgress )
	{
		mitMyInProgress = m_mapQuesting.find( mitInProgress->first );
		if( mitMyInProgress != m_mapQuesting.end() )
		{
	
			const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitInProgress->second );
			if( pQuestTemplet == NULL )
			{
				START_LOG( cerr, L"QUEST TEMPLET�� ����." )
					<< BUILD_LOG( mitInProgress->first )
					<< BUILD_LOG( mitInProgress->second )
					<< END_LOG;
				continue;
			}

			KQuestInstance kTempQuestInstance;
			kTempQuestInstance.m_iID			= mitInProgress->second;
			kTempQuestInstance.m_OwnorUnitUID	= iUnitUID;

			//sub quest seting(�Ϸ��ߴ°��� �ƴѰ����� �ľ�)
			for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
			{
				KSubQuestInstance kSub;

				const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
				if( pSubTemplet == NULL )
				{
					START_LOG( cerr, L"SUB QUEST TEMPLET�� ����." )
						<< BUILD_LOG( kTempQuestInstance.m_iID )
						<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[uiIndexSub] )
						<< BUILD_LOG( kTempQuestInstance.m_OwnorUnitUID )
						<< END_LOG;

					continue;
				}

				kSub.m_iID			= pSubTemplet->m_iID;
				kSub.m_ucClearData	= 0;
				kSub.m_bIsSuccess	= false;

				kTempQuestInstance.m_vecSubQuestInstance.push_back( kSub );
			}
			
			m_mapQuesting.insert( std::make_pair( mitInProgress->second, kTempQuestInstance ) );
			m_mapQuesting.erase( mitMyInProgress );
		}
		else
		{
			START_LOG( cerr, L"�̷� �Ϸ�� ����Ʈ�� ������ ���� �ʴٰ�?! �ִٰ� �ؼ� ����Ѱ��ݾ�!!" )
				<< BUILD_LOG( mitInProgress->first )
				<< BUILD_LOG( mitInProgress->second )
				<< END_LOG;
		}
	}
}

void KUserQuestManager::SetClassChangeDeleteQuest( IN UidType iUnitUID, IN std::vector< int >& vecDeleteCompleteQuest, IN std::vector< int >& vecDeleteInProgressQuest )
{
	// ���� ���� ����Ʈ ����
	for( u_short index = 0; index < vecDeleteInProgressQuest.size() ; ++index )
	{
		std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.find( vecDeleteInProgressQuest[index] );
		if( mit != m_mapQuesting.end() )
		{
			m_mapQuesting.erase( mit );
		}
		else
		{
			START_LOG( cerr, L"�̷� ������ ����Ʈ�� ������ ���� �ʴٰ�?! �ִٰ� �ؼ� ����ٰ��Ѱ��ݾ�!!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( vecDeleteInProgressQuest[index] )
				<< END_LOG;
		}
	}

	// �Ϸ�� ����Ʈ ����
	for( u_short index = 0; index < vecDeleteCompleteQuest.size() ; ++index )
	{
		std::map< int, KCompleteQuestInfo >::iterator mit = m_mapCompleteQuest.find( vecDeleteCompleteQuest[index] );
		if( mit != m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest.erase( mit );
		}
		else
		{
			START_LOG( cerr, L"�̷� �Ϸ�� ����Ʈ�� ������ ���� �ʴٰ�?! �ִٰ� �ؼ� ����ٰ��Ѱ��ݾ�!!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( vecDeleteCompleteQuest[index] )
				<< END_LOG;
		}
	}
}

void KUserQuestManager::GetClassChangeDeleteQuest( IN char cDownUnitClass, OUT std::vector< int >& vecCompleteDelete, OUT std::vector< int >& vecInProgressDelete )
{
	// �Ϸ�� ����Ʈ���� ���� ��(���� ����Ʈ) ã��
	std::map< int, KCompleteQuestInfo >::iterator mitComplete = m_mapCompleteQuest.begin();
	for(  ; mitComplete != m_mapCompleteQuest.end() ; ++mitComplete )
	{
		const CXSLQuestManager::QuestTemplet* pQeustTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitComplete->first );
		if( pQeustTemplet != NULL )
		{
			std::set< CXSLUnit::UNIT_CLASS >::const_iterator sit = pQeustTemplet->m_Condition.m_setUnitClass.find( static_cast<CXSLUnit::UNIT_CLASS>(cDownUnitClass) );
			if( pQeustTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
			{
				if( sit != pQeustTemplet->m_Condition.m_setUnitClass.end() )
				{
					vecCompleteDelete.push_back( mitComplete->first );
				}
			}
		}
	}
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
void KUserQuestManager::ClearAutoCompleteQuest()
{
	m_mapAutoQuestCompleteReq.clear();
	m_vecAutoQuestCompleteAck.clear();
}

void KUserQuestManager::SetAutoCompleteQuest( IN OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ& kPacketReq )
{
	ClearAutoCompleteQuest();

	BOOST_TEST_FOREACH( KEGS_QUEST_COMPLETE_REQ&, kInfo, kPacketReq.m_vecQuestCompleteReq )
	{
		kInfo.m_bAutoComplete = true; // �ڵ����� ����Ʈ �Ϸ��Ϸ��� ��� true�� �־������.
		m_mapAutoQuestCompleteReq.insert( std::make_pair( kInfo.m_iQuestID, kInfo ) );
	}
}

bool KUserQuestManager::GetAndDeleteAutoCompleteQuest( OUT KEGS_QUEST_COMPLETE_REQ& kReq )
{
	if( m_mapAutoQuestCompleteReq.empty() )
		return false;

	std::map< int, KEGS_QUEST_COMPLETE_REQ >::iterator mit;
	for( mit = m_mapAutoQuestCompleteReq.begin(); mit != m_mapAutoQuestCompleteReq.end(); ++mit )
	{
		kReq = mit->second;
        break;
	}

	if( mit != m_mapAutoQuestCompleteReq.end() )
	{
        m_mapAutoQuestCompleteReq.erase( mit );
	}

	return true;
}

void KUserQuestManager::AddAutoCompleteQuestResult( IN const KEGS_QUEST_COMPLETE_ACK& kAck )
{
    m_vecAutoQuestCompleteAck.push_back( kAck );
}

void KUserQuestManager::GetAutoCompleteQuestResult( OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK& kPacketAck )
{
	kPacketAck.m_vecQuestCompleteAck = m_vecAutoQuestCompleteAck;

	ClearAutoCompleteQuest();
}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::CheckEventQuest( IN KGSUserPtr spUser )
{
	// �̺�Ʈ ����Ʈ�� ������ ��� ���� �����ϵ��� �Ѵ�.
	// ������ �������� ��ũ��Ʈ�� ������ ������ �����ϵ��� �Ѵ�.
	std::vector<int> vecDeleteQuest;
	std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.begin();
	for( ; mit != m_mapQuesting.end() ; ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( clog, L"�������� �ʴ� ����Ʈ ID�Դϴ�! ��ũ��Ʈ �󿡼� ������ ����Ʈ �Դϴ�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			vecDeleteQuest.push_back( mit->first );
			continue;
		}
	}

	// ��ũ��Ʈ�� ���� Quest �� ������
	BOOST_TEST_FOREACH( int, iQuest, vecDeleteQuest )
	{
		std::map< int, KQuestInstance >::iterator mitDelete = m_mapQuesting.find( iQuest );
		if( mitDelete != m_mapQuesting.end() )
		{
			m_mapQuesting.erase( mitDelete );
		}
	}

	// ���� ���°� �ƴ����� ��ũ��Ʈ�� ������ �����ϵ��� �Ѵ�.
	std::vector<int> vecNewEventQuest;
	std::map< int, CXSLQuestManager::QuestTemplet > mapTemplet;
	SiCXSLQuestManager()->GetQuestListByType( CXSLQuestManager::QT_EVENT, mapTemplet );

	if( mapTemplet.empty() == false )
	{
		std::map< int, CXSLQuestManager::QuestTemplet >::iterator mitTemplet = mapTemplet.begin();
		for( ; mitTemplet != mapTemplet.end() ; ++mitTemplet )
		{
			std::map< int, KQuestInstance >::iterator mitQuest = m_mapQuesting.find( mitTemplet->first );
			// ������ ���°� �ƴ϶��
			if( mitQuest == m_mapQuesting.end() )
			{
				KEGS_NEW_QUEST_REQ kTempReq;
				kTempReq.m_iQuestID = mitTemplet->first;
				kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
				if ( SiKGameEventManager()->IsEnableEventQuest(mitTemplet->first) == false )
				{
					continue; 
				}
#endif //SERV_ALLOW_EVENT_ERASE

				if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, true ) == false )
				{
					START_LOG( cwarn, L"�ű� �̺�Ʈ ����Ʈ ���� ����!" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( kTempReq.m_iQuestID )
						<< END_LOG;
				}
			}
		}
	}
}

void KUserQuestManager::CheckResetDayEventQuest_AutoAccept( IN KGSUserPtr spUser )
{
	// �̺�Ʈ Ÿ���� ����Ʈ�� ��� �����´�.
	std::map< int, CXSLQuestManager::QuestTemplet > mapTemplet;
	SiCXSLQuestManager()->GetQuestListByType( CXSLQuestManager::QT_EVENT, mapTemplet );

	// ������ ������ ����Ʈ ���� Ȯ������
	std::vector<int> vecCompleteDayQuest;
	std::map< int, CXSLQuestManager::QuestTemplet >::iterator mit = mapTemplet.begin();
	for( ; mit != mapTemplet.end() ; ++mit )
	{
		// �̺�Ʈ ���� ����Ʈ�� �ƴϸ� ����!
		if( mit->second.m_eRepeatType != CXSLQuestManager::QRT_DAY )
		{
			continue;
		}

#ifdef SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
		if ( mit->second.m_bIsTimeEvent == true )
		{
			continue;
		}
#endif // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
		if ( SiKGameEventManager()->IsEnableEventQuest(mit->first) == false )
			continue; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
#endif //SERV_ALLOW_EVENT_ERASE

		// ���� �� ���ΰ�?
		bool bAccept = false;

		// �̹� �Ϸ��� ����Ʈ �ΰ�?
		KCompleteQuestInfo* pCompleteQuestInfo = GetCompleteQuestInfo( mit->first );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;

				continue;
			}

			// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
			{
				// 6�ú��� �Ѵ� �ð��̶�� �������� ����
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// ���� ���� �����Ѱ���?
			if( tCheckTime <= tCurTime )
			{
				bAccept = true;
			}
		}
		else
		{
			bAccept = true;
		}

		if( bAccept == true )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = mit->first;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, false ) == false )
			{
				START_LOG( cwarn, L"[����] New Quest Req Fail.!(After)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
			}
		}
	}
}

void KUserQuestManager::CheckLimitLevelQuest( IN KGSUserPtr spUser )
{
	// ���ӽ�/������ �� ������ ������ üũ�Ͽ� LimitLv �� ������ �������� ���� ������ ����Ʈ�� �ڵ����� ���� ��Ű�� ���� ����Ʈ �������� ���� �Ѵ�.
	// ���� ������ ����Ʈ�� �˻��Ͽ� LimitLv �� üũ����
	if( m_mapQuesting.empty() == true )
		return;

	std::vector<int> vecDelQuest;

	std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.begin();
	for( ; mit != m_mapQuesting.end() ; ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( clog, L"�������� �ʴ� ����Ʈ ID�Դϴ�! ��ũ��Ʈ �󿡼� ������ ����Ʈ �Դϴ�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// LimitLv �� ���� �Ǿ� �ְ�
		// ������ �������� ������
		if( pQuestTemplet->m_Condition.m_iLimitLevel != -1 && pQuestTemplet->m_Condition.m_iLimitLevel < spUser->GetLevel() )
		{
			vecDelQuest.push_back( mit->first );
		}
	}

	BOOST_TEST_FOREACH( int, iQuestID, vecDelQuest )
	{
		KEGS_GIVE_UP_QUEST_REQ kQuest;
		kQuest.m_iQuestID = iQuestID;
		Handler_EGS_GIVE_UP_QUEST_REQ( kQuest, spUser );
	}
}

void KUserQuestManager::Handler_OnEnterTheDungeon( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ���� ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_VISIT_DUNGEON )
				continue;

			// 3. ���� ���� ���� ���� Ȯ��!
			if( spUser->GetRoomUID() <= 0 )
				continue;

			// 4. �ش� ������ �´��� Ȯ��!
			if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetDungeonIDAndDungeonDifficulty(), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
				continue;

			// 5. �̹� �Ϸ��� ����Ʈ ���� Ȯ��!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"����Ʈ SQT_VISIT_DUNGEON ����" )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

void KUserQuestManager::Handler_OnFindNPC( IN KGSUserPtr spUser, std::vector< int >& vecNPCData, bool bDungeon )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST �� ����ֽ��ϴ�!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ���� ���� ����Ʈ
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_FIND_NPC )
				continue;

			// 3. ���� ���� ���� ���� Ȯ��!
			if( spUser->GetRoomUID() <= 0 && spUser->GetMapID() <= 0 )
				continue;

			// 4. �ش� ������ �´��� Ȯ��!
			if( bDungeon == true )
			{
				// ������ �����Ѵٸ�
				if( pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetDungeonIDAndDungeonDifficulty(), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
						continue;
				}
			}
			// 4. �ش� ��Ʋ �ʵ尡 �´��� Ȯ��!
			else
			{
				// ��Ʋ �ʵ尡 �����Ѵٸ�
				if( pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
						continue;
				}
			}

			// 5. ã�� npc �� �´ٸ�
			{
				bool bExistNpc = false;
				BOOST_TEST_FOREACH( int, kNPCData, vecNPCData )
				{
					if( kNPCData == pSubQuestTemplet->m_ClearCondition.m_iFindNPCID )
					{
						bExistNpc = true;
						break;
					}
				}

				if( bExistNpc == false )
					continue;
			}			
						
			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
			
			kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
			kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"����Ʈ SQT_VISIT_DUNGEON ����" )
				<< BUILD_LOG( spUser->GetMapID() )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

bool KUserQuestManager::CheckCompleteSubQuest_BeforGroup( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser )
{
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
	int iGroupID = 0;		// �׷��ȣ�� 1~ �����Ѵ�.

	std::map< int,std::vector<int> >::const_iterator mitGroup = pQuestTemplet->m_mapSubQuestGroup.begin();
	for( ; mitGroup != pQuestTemplet->m_mapSubQuestGroup.end() ; ++mitGroup )
	{
		BOOST_TEST_FOREACH( const int, iSub, mitGroup->second )
		{
			if( iSub == iSubQuestID )
			{
				iGroupID = mitGroup->first;
				break;
			}
		}

		if( iGroupID != 0 )
			break;
	}
	
	// �׷��� ã�� ���ߴ�
	if( iGroupID == 0 )
	{
		START_LOG( cwarn, L"���� ����Ʈ�� �׷� ������ ã�� ���ߴ�. " )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( iSubQuestID )
			<< END_LOG;

		return false;
	}

	// ã�Ҵٸ�...�� �׷���  subquest ���� ��� �Ϸ� �Ǿ����� Ȯ������
	std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.find( iQuestID );
	if( mit == m_mapQuesting.end() )
	{
		return false;
	}

	KQuestInstance& kQuestInstance = mit->second;

	mitGroup = pQuestTemplet->m_mapSubQuestGroup.begin();
	for( ; mitGroup != pQuestTemplet->m_mapSubQuestGroup.end() ; ++mitGroup )
	{
		if( mitGroup->first < iGroupID )
		{
			BOOST_TEST_FOREACH( const int, iSub, mitGroup->second )
			{
				BOOST_TEST_FOREACH( const KSubQuestInstance, SubQuest, kQuestInstance.m_vecSubQuestInstance )
				{
					if( SubQuest.m_iID == iSub )
					{
						if( SubQuest.m_bIsSuccess == false )
						{
							const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( SubQuest.m_iID );
							if( pSubTemplet != NULL )
							{
								// ���ܰ� �Ǵ� ���� ����Ʈ Ÿ���� �ִ�.
								if( pSubTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION 
								 || pSubTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
								{
									//�����ؾ��� ������(������ŭ) �ִ��� Ȯ���Ͽ� ��������Ʈ �Ϸ�üũ�� �Ѵ�.
									std::map<int,int> mapItemInfo;
									mapItemInfo.insert( std::make_pair( pSubTemplet->m_ClearCondition.m_iItemID, pSubTemplet->m_ClearCondition.m_iCollectionItemNum ) );

									// �κ��丮�� �����ؾ� �� �������� �ִٸ� �Ϸ� �� ���̳� ���������Ƿ� �����ߴٰ� ��������
									if( spUser->IsEnoughItemExistOnlyInventory( mapItemInfo ) == true )
									{
										break;
									}
								}
								else
								{
									return false;
								}
									
							}

							return false;
						}
						break;
					}
				}
			}
		}
	}

	return true;
}

void KUserQuestManager::CheckEpicQuest_EnterDungeon( IN int iDungeonID, IN KGSUserPtr spUser )
{
	//# iMapID �� ���� ��ȿ�� �˻�� 
	//# EGS_STATE_CHANGE_FIELD_REQ ���� �ϰ� ���Ա� ������ ���� �ʴ´�.
	std::vector<CXSLQuestManager::QuestTemplet*>::const_iterator cvit;
	std::vector<CXSLQuestManager::QuestTemplet*> vecEpicList;

	SiCXSLQuestManager()->GetEpicQuestTempletList(vecEpicList);

	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pTemplet, vecEpicList )
	{
		if( NULL == pTemplet )
			continue;

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pTemplet->m_eQuestType &&
			pTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pTemplet->m_vecSubQuest.empty() == true )
			continue;

		// ������ ����Ʈ X
		if( IsQuest( pTemplet->m_iID ) == true )
			continue;

		// �Ϸ�� ����Ʈ X
		if( IsCompleteQuest( pTemplet->m_iID ) == true )
			continue;

		// ���� ���̵� ������.
		if( iDungeonID <= 0 )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
		if ( SiKGameEventManager()->IsEnableEventQuest(pTemplet->m_iID) == false )
			continue; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
#endif //SERV_ALLOW_EVENT_ERASE

		if( pTemplet->m_Condition.m_setEnableDungeon.size() > 0 )
		{
			std::set<int>::const_iterator sit = pTemplet->m_Condition.m_setEnableDungeon.find( iDungeonID );
			if( sit == pTemplet->m_Condition.m_setEnableDungeon.end() )
				continue;
		}
		else
			continue;

		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pTemplet->m_iID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

		if( false == Handler_EGS_NEW_QUEST_REQ(kTempReq, spUser) )
		{
			START_LOG( cwarn, L"[����] New Quest Req Fail.!(State Change)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pTemplet->m_iID )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
		}		
	}
}

void KUserQuestManager::CheckEpicQuest_EnterBattleField( IN int iMapID, IN KGSUserPtr spUser )
{
	//# iMapID �� ���� ��ȿ�� �˻�� 
	//# EGS_STATE_CHANGE_FIELD_REQ ���� �ϰ� ���Ա� ������ ���� �ʴ´�.
	std::vector<CXSLQuestManager::QuestTemplet*>::const_iterator cvit;
	std::vector<CXSLQuestManager::QuestTemplet*> vecEpicList;

	SiCXSLQuestManager()->GetEpicQuestTempletList(vecEpicList);

	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pTemplet, vecEpicList )
	{
		if( NULL == pTemplet )
			continue;

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pTemplet->m_eQuestType &&
			pTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pTemplet->m_vecSubQuest.empty() == true )
			continue;

		// ������ ����Ʈ X
		if( IsQuest( pTemplet->m_iID ) == true )
			continue;

		// �Ϸ�� ����Ʈ X
		if( IsCompleteQuest( pTemplet->m_iID ) == true )
			continue;

		// ��Ʋ�ʵ� ���̵� ������.
		if( iMapID <= 0 )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_������ // DB���� �� on/off �κ� �߰� �۾� // ���� �ʵ� ���� ����Ʈ �ڵ� ���� ���鼭 �ش� ��� �߰�
		if ( SiKGameEventManager()->IsEnableEventQuest(pTemplet->m_iID) == false )
			continue; // 2013.06.10 lygan_������ // ������ Ȱ��ȭ ���� ���� ����Ʈ�� AfterQuest �� ã�� �ʿ䰡 ����
#endif //SERV_ALLOW_EVENT_ERASE

		if( pTemplet->m_Condition.m_setEnableBattleField.size() > 0 )
		{
			std::set<int>::const_iterator sit = pTemplet->m_Condition.m_setEnableBattleField.find( iMapID );
			if( sit == pTemplet->m_Condition.m_setEnableBattleField.end() )
				continue;
		}
		else
			continue;

		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pTemplet->m_iID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

		if( false == Handler_EGS_NEW_QUEST_REQ(kTempReq, spUser) )
		{
			START_LOG( cwarn, L"[����] New Quest Req Fail.!(State Change)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pTemplet->m_iID )
				<< BUILD_LOG( iMapID )
				<< END_LOG;
		}		
	}
}
#endif SERV_REFORM_QUEST
//}}

//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
bool KUserQuestManager::IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo )
{
	std::map< UidType, bool >::const_iterator mit = mapSuitableLevelInfo.find( CharUID );
	if( mit == mapSuitableLevelInfo.end() )
	{
		START_LOG( cwarn, L"���� ���� ���� ������ ����?" )
			<< BUILD_LOG( CharUID )
			<< BUILD_LOG( mapSuitableLevelInfo.size() )
			<< END_LOG;
		return false;
	}

	return mit->second;
}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
//}}

#ifdef SERV_POINT_COUNT_SYSTEM
void KUserQuestManager::SetUpdateQuestInstance( IN KGSUserPtr spUser )
{
	if ( spUser != NULL)
	{
		std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.begin();
		for ( ; mit != m_mapQuesting.end() ; ++mit)
		{
			for( int iIndexInstance = 0; iIndexInstance < (int)mit->second.m_vecSubQuestInstance.size(); ++iIndexInstance )
			{
				const CXSLQuestManager::SubQuestTemplet* pSubQuestInstanceTemplet = SiCXSLQuestManager()->GetSubQuestTemplet(mit->second.m_vecSubQuestInstance[iIndexInstance].m_iID );
				if( pSubQuestInstanceTemplet == NULL )
				{
					START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
						<< BUILD_LOG( mit->second.m_vecSubQuestInstance[iIndexInstance].m_iID )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_08 );
					continue;
				}
				else
				{
					if ( pSubQuestInstanceTemplet->m_eClearType == CXSLQuestManager::SQT_POINT_COUNT )
					{

					}
				}
			}
		}

		if ( m_mapQuesting.empty() == false )
		{

		}
	}
}
#endif //SERV_POINT_COUNT_SYSTEM

#ifdef SERV_SKILL_USE_SUBQUEST
void KUserQuestManager::Handler_EGS_SKILL_USE_REQ( IN int iSkillID, IN KGSUserPtr spUser )
{
	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map<int, KQuestInstance>::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); mit++ )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );

		if( pQuestTemplet != NULL )
		{

#ifdef SERV_EPIC_QUEST
			// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
			if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
				pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
				)
				continue;
#endif SERV_EPIC_QUEST

			if( pQuestTemplet->m_vecSubQuest.empty() == false )
			{
				for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
				{
					const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );

					if( pSubQuestTemplet != NULL )
					{
						if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_SKILL_USE )
							continue;

						if( pSubQuestTemplet->m_ClearCondition.m_setSkillID.empty() == true ||
							pSubQuestTemplet->m_ClearCondition.m_setSkillID.find( iSkillID ) != pSubQuestTemplet->m_ClearCondition.m_setSkillID.end() )
						{
							if( pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount > kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
							{
								++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;

								if( pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount <= kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
									kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

								kNot.m_vecQuestInst.push_back( kQuestInstance );

								START_LOG( clog, L"����Ʈ SQT_SKILL_USE ����" )
									<< BUILD_LOG( iSkillID )
									<< BUILD_LOG( spUser->GetCharName() )
									<< BUILD_LOG( spUser->GetUserName() )
									<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
									<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_setSkillID.size() )
									<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount )
									<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
									<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess );
							}
						}

					}
					else
					{
						START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
							<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
							<< BUILD_LOG( spUser->GetCharUID() )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;

						SET_ERROR( ERR_QUEST_08 );
					}
				}
			}
		}
		else
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_SKILL_USE_SUBQUEST

#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
void KUserQuestManager::Handler_OnLearnNewSkill( IN KGSUserPtr spUser, IN std::vector<int> vecNowLearnSkill )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	//����Ʈ�� ���鼭
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance* pkQuestInstance = &mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );
		if( pQuestTemplet == NULL )		
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

#ifdef SERV_EPIC_QUEST
		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		//��������Ʈ�� ������ �˻��Ѵ�.
		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SUB_QUEST_TYPE::SQT_LEARN_NEW_SKILL )	//Ŭ���� ������ ������..
				continue;

			if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
				continue;

			if( vecNowLearnSkill.size() <= 0 )
				continue;

			if( pSubQuestTemplet->m_ClearCondition.m_setSkillID.empty() == true )
				continue;

			for( int i = 0; i < vecNowLearnSkill.size(); i++)
			{
				if( pSubQuestTemplet->m_ClearCondition.m_setSkillID.find(vecNowLearnSkill[i]) != pSubQuestTemplet->m_ClearCondition.m_setSkillID.end() )
				{
					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;

					if( pSubQuestTemplet->m_ClearCondition.m_iLearnNewSkillCount <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					{
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
						break;
					}
				}
			}

			kNot.m_vecQuestInst.push_back( *pkQuestInstance );

			START_LOG( clog, L"���ο� ��ų ���� ����Ʈ ����!" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iLearnNewSkillCount )
				<< BUILD_LOG( vecNowLearnSkill.size() )
				<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

#ifdef SERV_EVENT_LEVEL_UP_QUEST_CLEAR
bool KUserQuestManager::Handler_OnCheckLevelUpQuest(IN const KGSUserPtr spUser)
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return false;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() )
			continue;

		// ���� ����Ʈ �� ������ �ȵǸ� ������ �ȵ�.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType 
			&&	pQuestTemplet->m_iPlayLevel > spUser->GetLevel()	)
			continue;

		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET ������ ����.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_iID == 121340 || pSubQuestTemplet->m_iID == 121350 )
			{
				++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;
				kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
				kNot.m_vecQuestInst.push_back( kQuestInstance );
			}

			START_LOG( clog, L"����Ʈ SQT_NPC_HUNT ����" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
					<< END_LOG;
		}
	}
	
	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
	return false;
}
#endif SERV_EVENT_LEVEL_UP_QUEST_CLEAR