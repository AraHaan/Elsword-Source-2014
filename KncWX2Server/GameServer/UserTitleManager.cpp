#include "GSUser.h"
#include "NetError.h"

#include ".\usertitlemanager.h"
#include "X2Data/XSLTitleManager.h"
#include "X2Data/XSLSocketItem.h"

#include "TrainingCenterTable.h"

KUserTitleManager::KUserTitleManager(void)
{
}

KUserTitleManager::~KUserTitleManager(void)
{
}

void KUserTitleManager::Clear()
{
	m_iEquippedTitle.SetValue( 0 );
	m_mapMission.clear();
	m_mapTitle.clear();
	m_setDBupdateMission.clear();

	//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	m_bUsedResurrectionStone = false;
	m_iStartedNumMember		 = 0;
#endif SERV_ADD_TITLE_CONDITION
	//}}
	//{{ 2013. 02. 05  Īȣ ȹ�� ���� �߰�(��Ȱ�� ��� Ƚ��, ���� ����) - ��μ�
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	m_iUseResurrectionStoneCount	 = 0;
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}
}

void KUserTitleManager::Init( IN bool bIsPcBang, IN OUT std::vector< KMissionInstance >& vecMission, IN OUT std::vector< KTitleInfo >& vecTitle )
{
	// �������� �̼� ����
	std::vector< KMissionInstance >::iterator vit;
	for( vit = vecMission.begin(); vit != vecMission.end(); ++vit )
	{
		KMissionInstance& kMissionInstance = *vit;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cwarn, L"�̼� ���ø��� �������� �ʽ��ϴ�." )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< END_LOG;
			continue;
		}

		//{{ 2008. 10. 22  ������	����� �̺�Ʈ �̼�
		if( pMissionTemplet->m_bIsSecretTitle )
			continue;
		//}}

		KMissionInstance kMission;
		kMission.m_iID = kMissionInstance.m_iID;

		// ���� �̼� ����		
		for( u_int iIdx = 0; iIdx < pMissionTemplet->m_vecSubMission.size(); ++iIdx )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[iIdx] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"���� �̼� ���ø��� �������� �ʽ��ϴ�." )
					<< BUILD_LOG( kMissionInstance.m_iID )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[iIdx] )
					<< END_LOG;
				// ���ø� �б� ������ �̼��� �߰��Ǹ� �ȵ˴ϴ�.
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
				kMission.m_vecSubMissionInstance.clear();
				break;
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
				continue;
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
			}

            KSubMissionInstance kSubMission;
			kSubMission.m_iID		 = pSubMissionTemplet->m_iSubMissionID;
			kSubMission.m_sClearData = kMissionInstance.m_vecSubMissionInstance[iIdx].m_sClearData;

			kMissionInstance.m_vecSubMissionInstance[iIdx].m_iID = pSubMissionTemplet->m_iSubMissionID;

			switch( pSubMissionTemplet->m_eClearType )
			{
//			case CXSLTitleManager::TMCT_DUNGEON_TIME:
//			case CXSLTitleManager::TMCT_DUNGEON_RANK:
			case CXSLTitleManager::TMCT_NPC_TALK:
			case CXSLTitleManager::TMCT_QUEST:
			case CXSLTitleManager::TMCT_USE_ITEM:
			case CXSLTitleManager::TMCT_COLLECT_TITLE:
				{
					if( kSubMission.m_sClearData != 0 )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

// 			case CXSLTitleManager::TMCT_DUNGEON_DAMAGE:
// 				{
// 					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData <= pSubMissionTemplet->m_ClearCondition.m_iDungeonDamage )
// 					{
// 						kSubMission.m_bIsSuccess = true;
// 						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
// 					}
// 				}
// 				break;
			case CXSLTitleManager::TMCT_DUNGEON_TIME:
			case CXSLTitleManager::TMCT_DUNGEON_RANK:
			case CXSLTitleManager::TMCT_DUNGEON_DAMAGE:
			case CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT:
				//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			case CXSLTitleManager::TMCT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
				//}}
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_NPC_HUNT:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iKillNum )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_ITEM_COLLECTION:
			case CXSLTitleManager::TMCT_QUEST_ITEM_COLLECTION:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iCollectionItemNum )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_PVP_PLAY:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPVPPlay )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_PVP_WIN:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPVPWin )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_PVP_KILL:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPVPKill )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

				//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
			case CXSLTitleManager::TMCT_GIVE_PET_FEED:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPetFeedCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
			case CXSLTitleManager::TMCT_PLAYER_WITH_DUNGEON_CLEAR:
			case CXSLTitleManager::TMCT_RESURRECTION_STONE:
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
			case CXSLTitleManager::TMCT_TOGETHER_MISSION_CLEAR:
				{
// 					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
// 					{
// 						kSubMission.m_bIsSuccess = true;
// 
// 						for( int num = 0 ; num < (int)pMissionTemplet->m_vecSubMission.size(); ++num )		// ��� �̼� ����
// 						{
// 							kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
// 						}
// 					}
				}
				break;
			case CXSLTitleManager::TMCT_USER_UNIT_DIE:
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
#endif SERV_ADD_TITLE_CONDITION
				//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// ���볯¥: 2013-08-13
			case CXSLTitleManager::TMCT_ITEM_SOCKET:
			case CXSLTitleManager::TMCT_ITEM_ENCHANT_LEVEL:
			case CXSLTitleManager::TMCT_ITEM_ENCHANT_COUNT:
			case CXSLTitleManager::TMCT_ITEM_ATTRIB:
			case CXSLTitleManager::TMCT_ITEM_RESOLVE:
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
#endif // SERV_ADD_TITLE_CONDITION_2013_08

			default:
				{
					START_LOG( cerr, L"���� ���� �̼� Ÿ��." )
						<< BUILD_LOG( pSubMissionTemplet->m_eClearType )
						<< BUILD_LOG( kSubMission.m_iID )
						<< END_LOG;
				}
			}

			kMission.m_vecSubMissionInstance.push_back( kSubMission );
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMission.m_vecSubMissionInstance.size() > 0 )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
		m_mapMission[kMission.m_iID] = kMission;
	}
	
	// ������ ��������Ʈ �����͸� ���� ����
	//kInfo.m_vecQuest.clear();
	//std::map<int,KQuestInstance>::iterator mit;
	//for( mit = m_mapQuest.begin(); mit != m_mapQuest.end(); mit++ )
	//{
	//	kInfo.m_vecQuest.push_back( mit->second );
	//}

	// ���� Ÿ��Ʋ(�Ϸ� �̼�) �����ؼ� �ֱ�
	CTime kInfinityTime( 2040, 1, 1, 0, 0, 0 );
	CTime kCurTime = CTime::GetCurrentTime();

	std::vector< KTitleInfo > vecCheckingTitle = vecTitle;
	vecTitle.clear();

	std::vector< KTitleInfo >::const_iterator vitT;
	for( vitT = vecCheckingTitle.begin(); vitT != vecCheckingTitle.end(); ++vitT )
	{
		KTitleInfo kTitle = *vitT;		

		CTime tEndDate;
		if( !KncUtil::ConvertStringToCTime( kTitle.m_wstrEndDate, tEndDate ) )
		{
			START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
				<< BUILD_LOG( kTitle.m_wstrEndDate )
				<< END_LOG;
			continue;
		}

		// �Ⱓ ����������..
		kTitle.m_bInfinity = ( kInfinityTime < tEndDate );
		kTitle.m_kCTime = tEndDate.GetTime();
		m_mapTitle.insert( std::make_pair( kTitle.m_iTitleID, kTitle ) );

		vecTitle.push_back( kTitle );
	}

	//{{ 2009. 10. 26  ������	PC���̺�Ʈ
	if( bIsPcBang == true )
	{
		std::map< int, KTitleInfo >::const_iterator mitPB = m_mapTitle.find( PC_BANG_TITLE_ID );
		if( mitPB == m_mapTitle.end() )
		{
			KTitleInfo kTitle;
			kTitle.m_iTitleID = PC_BANG_TITLE_ID;
			kTitle.m_bInfinity = true;
			kTitle.m_kCTime = kInfinityTime.GetTime();
			m_mapTitle.insert( std::make_pair( kTitle.m_iTitleID, kTitle ) );

			vecTitle.push_back( kTitle );
		}
		else
		{
			START_LOG( cerr, L"�̹� pc�� Īȣ�� �ִ�? �߻��ϸ� �ȵǴ� ����!" )
				<< END_LOG;
		}
	}
	//}}
}

/************************************************************************/
/* void CheckNewMission() : ���� ���� ������ �̼��� ã�Ƽ� �������� �̼����� ������ִ� �Լ�
/* 1. ��ũ��Ʈ���� ��� �̼��� �����˻��Ͽ� ���Ǻ񱳸� �ϹǷ� ����ȭ�� �ʿ���.
/************************************************************************/

void KUserTitleManager::CheckNewMission( IN KGSUserPtr spUser, IN bool bFirst /*= false*/ )
{
	std::map< int, KMissionInstance >::const_iterator mitM;
	std::map< int, KTitleInfo >::const_iterator mitT;

	KEGS_NEW_MISSION_NOT kPacketNot;
	

	std::vector< CXSLTitleManager::MissionTemplet >::const_iterator vit;
	const std::vector< CXSLTitleManager::MissionTemplet >& vecMissionList = SiCXSLTitleManager()->GetMissionList();

	for( vit = vecMissionList.begin(); vit != vecMissionList.end(); ++vit )
	{
		const CXSLTitleManager::MissionTemplet& kMissionTemplet = *vit;

		// 1. �Ϸ�� �̼��� ����
		mitT = m_mapTitle.find( kMissionTemplet.m_iMissionID );
		if( mitT != m_mapTitle.end() )
			continue;

		// 2. �������� �̼��� ����
		mitM = m_mapMission.find( kMissionTemplet.m_iMissionID );
		if( mitM != m_mapMission.end() )
			continue;

		// 3. ����� Ÿ��Ʋ�� ����
		if( kMissionTemplet.m_bIsSecretTitle )
			continue;

		// 3. ���� ����
		if( kMissionTemplet.m_kCondition.m_iLevel > spUser->GetLevel() )
			continue;

		// 4. ���� Ÿ��
		if( kMissionTemplet.m_kCondition.m_eUnitType != CXSLUnit::UT_NONE )
		{
			if( kMissionTemplet.m_kCondition.m_eUnitType != spUser->GetUnitType() )
				continue;
		}

		// 5. ���� Ŭ����
		if( kMissionTemplet.m_kCondition.m_eUnitClass != CXSLUnit::UC_NONE )
		{
			if( kMissionTemplet.m_kCondition.m_eUnitClass != spUser->GetUnitClass() )
				continue;
		}

		// 6. ����̼� üũ
		if( kMissionTemplet.m_kCondition.m_iClearMissionID != 0 )
		{
			mitT = m_mapTitle.find( kMissionTemplet.m_kCondition.m_iClearMissionID );
			if( mitT == m_mapTitle.end() )
				continue;
		}

		// ��� ���� ������ new�̼����� ��� - Ŭ���̾�Ʈ���Ե� �˷���
		KMissionInstance kMission;
		kMission.m_iID = kMissionTemplet.m_iMissionID;

		// ���� �̼� ����
		for( u_int iIdx = 0; iIdx < kMissionTemplet.m_vecSubMission.size(); ++iIdx )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( kMissionTemplet.m_vecSubMission[iIdx] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"���� �̼� ���ø��� �������� �ʽ��ϴ�." )
					<< BUILD_LOG( kMission.m_iID )
					<< BUILD_LOG( kMissionTemplet.m_vecSubMission[iIdx] )
					<< END_LOG;
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
				kMission.m_vecSubMissionInstance.clear();
				break;
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
				continue;
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
			}

			KSubMissionInstance kSubMission;
			kSubMission.m_iID = pSubMissionTemplet->m_iSubMissionID;
			kMission.m_vecSubMissionInstance.push_back( kSubMission );
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMission.m_vecSubMissionInstance.size() > 0 )
		{
			m_mapMission[kMission.m_iID] = kMission;
			// Ŭ���̾�Ʈ �����ٰ�
			kPacketNot.m_vecNewMission.push_back( kMission );
		}
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
		m_mapMission[kMission.m_iID] = kMission;

		// Ŭ���̾�Ʈ �����ٰ�
		kPacketNot.m_vecNewMission.push_back( kMission );
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
	}	

	if( kPacketNot.m_vecNewMission.empty() )
		return;

	kPacketNot.m_bFirst = bFirst;
	spUser->SendPacket( EGS_NEW_MISSION_NOT, kPacketNot );
}

KMissionInstance* KUserTitleManager::GetMissionInstance( IN int iMissionID )
{
	std::map< int, KMissionInstance >::iterator mit;
	mit = m_mapMission.find( iMissionID );
	if( mit == m_mapMission.end() )
		return NULL;
	
	return &(mit->second);
}

void KUserTitleManager::OnNpcUnitDie( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iMonsterID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
		
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET �� INSTANCE �� SUB MISSION ������ Ʋ��! ��ũ��Ʈ Ȯ�� �ʿ�!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_NPC_HUNT )
				continue;

			//{{ 2012. 1. 11	Merge �ڼ���	2012.12.26 �ӱԼ� Ÿ��Ʋ �̼� Ŭ���� NPC ���� ����
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
			if( pSubMissionTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iMonsterID) ) == pSubMissionTemplet->m_ClearCondition.m_setKillNPCID.end() )
				continue;
#else
			if( pSubMissionTemplet->m_ClearCondition.m_eKillNPCID != iMonsterID )
				continue;
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
			//}}

			//���� ������ �ִ���
			if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID > 0 )
			{
				if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
					continue;

				//�׸��� ���̵� ������ �ִ��� Ȯ���Ѵ�.
				if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty >= 0 )
				{
					// Ư�� ���̵� �̻��� ���ǿ� ����
					if( pSubMissionTemplet->m_ClearCondition.m_bUpperDifficulty == false )
					{
						if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
							continue;
					}
					else
					{
						if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
							continue;
					}
				}

				if( ( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID ) && ( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode ) )
					continue;
			}

#ifdef DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM
			/// ���� ������ �������� �ʾҴµ�, 
			/// ���� �÷������� ������ �����ƷüҶ�� ī��Ʈ ���� �ʴ´�.
			else if ( KTrainingCenterTable::GetInstance()->IsTrainingCenter( iDungeonID ) )
				continue;
#endif // DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM
			
			if( pSubMissionTemplet->m_ClearCondition.m_iKillNum > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iKillNum <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_NPC_HUNT ����" )
					<< BUILD_LOG( iMonsterID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}
	
	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnTalkWithNpc( IN int iNPCID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET �� INSTANCE �� SUB MISSION ������ Ʋ��! ��ũ��Ʈ Ȯ�� �ʿ�!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
		
		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_NPC_TALK )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_eTalkNPCID != iNPCID )
				continue;

			if( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData == 0 )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_NPC_TALK ����" )
					<< BUILD_LOG( iNPCID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_eTalkNPCID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

//{{ 2010. 8. 23	������	���� Īȣ
#ifdef SERV_HELL_MODE_TITLE
void KUserTitleManager::OnDungeonClear( IN KGSUserPtr spUser, IN int iDungeonID, IN char cDifficulty, IN char cDungeonMode, IN char cRank, IN int iPlayTime, IN int iDamage, IN const std::set< UidType >& setPartyMembers )
#else
void KUserTitleManager::OnDungeonClear( IN KGSUserPtr spUser, IN int iDungeonID, IN char cDifficulty, IN char cRank, IN int iPlayTime, IN int iDamage, IN const std::set< UidType >& setPartyMembers )
#endif SERV_HELL_MODE_TITLE
//}}
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET �� INSTANCE �� SUB MISSION ������ Ʋ��! ��ũ��Ʈ Ȯ�� �ʿ�!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			bool bRVal = false;

			//{{ 2010. 8. 23	������	���� Īȣ
//#ifdef SERV_HELL_MODE_TITLE
			//���� ������ �ִ���
			if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID > 0 )
			{
				// ��д����̶��..
				if( CXSLDungeon::IsSecretDungeon( pSubMissionTemplet->m_ClearCondition.m_eDungeonID ) == true )
				{
					// �ش� ��д����� �´��� �˻�
					if( ( pSubMissionTemplet->m_ClearCondition.m_eDungeonID / 10 ) != ( iDungeonID / 10 ) )
						continue;

					// ���̵� �˻�
					if( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID )
					{
						if( pSubMissionTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode )
								continue;
						}
					}
				}
				// �Ϲݴ����̶��..
				else
				{
					if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
						continue;

					//�׸��� ���̵� ������ �ִ��� Ȯ���Ѵ�.
					if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty >= 0 )
					{
						// Ư�� ���̵� �̻��� ���ǿ� ����
						if( pSubMissionTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
								continue;
						}
						else
						{
							if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
								continue;
						}
					}
				}
			}
//#else
//			//���� ������ �ִ���
//			if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID > 0 )
//			{
//				if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
//					continue;
//
//				//�׸��� ���̵� ������ �ִ��� Ȯ���Ѵ�.
//				if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty >= 0 )
//				{
//					if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
//						continue;
//				}
//			}
//#endif SERV_HELL_MODE_TITLE
			//}}

			switch( pSubMissionTemplet->m_eClearType )
			{
			case CXSLTitleManager::TMCT_DUNGEON_TIME:
				{
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearTime >= iPlayTime )
						bRVal = true;
				}
				break;

			case CXSLTitleManager::TMCT_DUNGEON_RANK:
				{
					if( (char)pSubMissionTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
						bRVal = true;
				}
				break;

			case CXSLTitleManager::TMCT_DUNGEON_DAMAGE:
				{
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonDamage >= iDamage )
						bRVal = true;
				}
				break;

#ifndef DEPRECATED_SERVER_GROUP_MASK
				//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			case CXSLTitleManager::TMCT_WITH_DIF_SERV_USER:
				{
					std::set< UidType >::const_iterator sit;
					for( sit = setPartyMembers.begin(); sit != setPartyMembers.end(); ++sit )
					{
						if( *sit == spUser->GetCharUID() )
							continue;

						// �ٸ� ������ ������ �÷����Ѱ��̶��..?
						if( spUser->IsSameServerGroupUID( *sit ) == false )
						{
							bRVal = true;
						}
					}
				}
				break;
#endif SERV_INTEGRATION
				//}}
#endif DEPRECATED_SERVER_GROUP_MASK

			case CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT:
				{
					//080811.hoons. �̺�Ʈ ����Ʈ �϶��� �ƴҶ� Ŭ���� Ÿ���� Ʋ������.
					//if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT )
					//{
					//	if( CXSLDungeon::IsRUBENDungeon( iDungeonID ) == false && CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
					//		bRVal = true;
					//}
					//else
					{
						//���� Ŭ���� ����� �¸��� �ϸ� �Ǳ⶧���� �̰����� ó��
						//## �ʽ����� ���� ����ó���Ѵ�.
						if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL  &&
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
						{
							bRVal = true;
						}
					}
				}
				break;
				//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
			case CXSLTitleManager::TMCT_PLAYER_WITH_DUNGEON_CLEAR:
			case CXSLTitleManager::TMCT_RESURRECTION_STONE:
				{
					if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL  
					&&	CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
					{
						bRVal = true;
					}
				}
				break;
			case CXSLTitleManager::TMCT_TOGETHER_MISSION_CLEAR:
				{
					bRVal = true;
				}
				break;
#endif SERV_ADD_TITLE_CONDITION
				//}}
			}

			// ���� �̼� Ŭ����
			if( bRVal )
			{
				//{{ 2010. 05. 17  ������	���� ���� ������ ����
//#ifdef SERV_INTEGRATION
				if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT  ||
					//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
//#ifdef SERV_ADD_TITLE_CONDITION
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_RANK  ||
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_TIME  ||
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_DAMAGE  ||
//#endif SERV_ADD_TITLE_CONDITION
					//}}
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_WITH_DIF_SERV_USER )
//#else
//				if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT )
//#endif SERV_INTEGRATION
				//}}				
				{
					//{{ 2010. 05. 03  ������	���� Ŭ���� ī��Ʈ ���� ����
					// Ŭ���� ���� ī��Ʈ�� ���� �ʰ� �����Ѵ�.
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}
					//}}

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

					kNot.m_vecMissionInst.push_back( kMissionInstance );
				}
				//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
				else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_PLAYER_WITH_DUNGEON_CLEAR )
				{
					if( pSubMissionTemplet->m_ClearCondition.m_iPlayerCount >= GetStartedNumMember() )
					{
						if(	pSubMissionTemplet->m_ClearCondition.m_iDungeonClearTime != 0 )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearTime < iPlayTime )
								continue;
						}
						if(	pSubMissionTemplet->m_ClearCondition.m_eDungeonRank != 0 )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_eDungeonRank > cRank )
								continue;
						}

						// ���� ���� ������ Ŭ���� ī���� ����
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
					}

					kNot.m_vecMissionInst.push_back( kMissionInstance );
				}
				else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_RESURRECTION_STONE )
				{
					//{{ 2013. 02. 05  Īȣ ȹ�� ���� �߰�(��Ȱ�� ��� Ƚ��, ���� ����) - ��μ�
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
					if( pSubMissionTemplet->m_ClearCondition.m_bCheckResurrectionStone == false && spUser->IsUseResurrectionStone() == false )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}
					else if( pSubMissionTemplet->m_ClearCondition.m_bCheckResurrectionStone == true && spUser->IsUseResurrectionStone() == true )
					{
						if( pSubMissionTemplet->m_ClearCondition.m_iUseResurrectionStoneCount <= spUser->GetUseResurrectionStoneCount() )
						{
							kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
						}
					}
#else
					if( pSubMissionTemplet->m_ClearCondition.m_bCheckResurrectionStone == true && spUser->IsUseResurrectionStone() == false )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}
#endif SERV_ADD_TITLE_CONDITION_SANDER
					//}
					
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
					}

					kNot.m_vecMissionInst.push_back( kMissionInstance );
				}
				else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_TOGETHER_MISSION_CLEAR )
				{
					bool bResult = true;
					
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
					for( u_int nMission = 0; nMission < kMissionInstance.m_vecSubMissionInstance.size(); ++nMission )
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
					for( u_int nMission = 0; nMission < pMissionTemplet->m_vecSubMission.size(); ++nMission )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
					{
						// ���� ���� �̼� ��� Ŭ���� �Ǿ�� �Ѵ�. 
						if( kMissionInstance.m_vecSubMissionInstance[nMission].m_bIsSuccess == false )
						{
							if( nMission != nSub )	// �̼� Ÿ���� ���� ������ �ƴϾ�� �Ѵ�.// ���� ����̼��� ���ø��� �̼��̴�.
							{
								bResult = false;
								break;
							}
						}
					}

					if( bResult == true )		// ��� Ŭ����Ǿ��ٸ�
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1; // Ŭ���� Ƚ�� ����
						
						if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
						{
							kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
						}
						else
						{
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
							for( u_int num = 0; num < kMissionInstance.m_vecSubMissionInstance.size(); ++num )
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
							for( u_int num = 0; num < pMissionTemplet->m_vecSubMission.size(); ++num )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
							{
								if( num != nSub )
								{
									kMissionInstance.m_vecSubMissionInstance[num].m_sClearData = 0;
									kMissionInstance.m_vecSubMissionInstance[num].m_bIsSuccess = false;	// ���� ���� �̼� ���з� �ǵ�����
									// DB ������Ʈ�� �߰��� ID ����
									DeleteUpdateMission( kNot.m_vecMissionInst, kMissionInstance.m_iID );
								}
							}
						}
					}
					else					// ��� Ŭ���� ���� �ʾҴٸ� 
					{
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
						for( u_int num = 0; num < kMissionInstance.m_vecSubMissionInstance.size(); ++num )
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
						for( u_int num = 0; num < pMissionTemplet->m_vecSubMission.size(); ++num )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
						{
							if( num != nSub )
							{
								kMissionInstance.m_vecSubMissionInstance[num].m_sClearData = 0;
								kMissionInstance.m_vecSubMissionInstance[num].m_bIsSuccess = false;	// ��� �̼� ���з� �ǵ�����
								// DB ������Ʈ�� �߰��� ID ����
								DeleteUpdateMission( kNot.m_vecMissionInst, kMissionInstance.m_iID );
							}
						}

						// DB ������Ʈ ���� ���� �ʵ��� �ѹ� add ���ش�.
						AddUpdateMission( kMissionInstance.m_iID );
						continue;				// �������� ó�� ���� �ʰ� �ǳʶٰ� �Ѵ�.
					}

					kNot.m_vecMissionInst.push_back( kMissionInstance );

				}
#endif SERV_ADD_TITLE_CONDITION
				//}}
				else
				{
					if( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData == 0 )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

						kNot.m_vecMissionInst.push_back( kMissionInstance );
					}
				}

				START_LOG( clog, L"�̼� TMCT_DUNGEON_TIME or TMCT_DUNGEON_RANK ����" )
					<< BUILD_LOGc( cRank )
					<< BUILD_LOG( iPlayTime )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnQuestComplete( IN int iQuestID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET �� INSTANCE �� SUB MISSION ������ Ʋ��! ��ũ��Ʈ Ȯ�� �ʿ�!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_QUEST )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iQuestID == iQuestID )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_QUEST ����" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iQuestID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnUseItem( IN const int iItemID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET �� INSTANCE �� SUB MISSION ������ Ʋ��! ��ũ��Ʈ Ȯ�� �ʿ�!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_USE_ITEM )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iItemID == iItemID )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_USE_ITEM ����" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iItemID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::CheckCompleteMission( IN int iMissionID, IN KGSUserPtr spUser )
{
	KMissionInstance* pMissionInstance = GetMissionInstance( iMissionID );
	if( pMissionInstance == NULL )
	{
		START_LOG( cerr, L"�������� �̼��� �ƴմϴ�!" )
			<< BUILD_LOG( iMissionID )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetCharName() )
			<< END_LOG;
		return;
	}

	const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( iMissionID );
	if( pMissionTemplet == NULL )
	{
		START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
			<< BUILD_LOG( iMissionID )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetCharName() )
			<< END_LOG;
		return;
	}

	for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
	{
		const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
		if( pSubMissionTemplet == NULL )
		{
			START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;
			return;
		}

		if( !pMissionInstance->m_vecSubMissionInstance[nSub].m_bIsSuccess )
			return;
	}

	if( !SiCXSLTitleManager()->IsExistTitleID( pMissionInstance->m_iID ) )
	{
		START_LOG( cerr, L"�̼�ID�� Ÿ��ƲID�� ��ġ���� �ʴ°� �ִ�?" )
			<< BUILD_LOG( pMissionInstance->m_iID )
			<< END_LOG;
		return;
	}
	
	// �̼� ���� �Ϸ�! Īȣ ������ ����!
	KDBE_INSERT_TITLE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = spUser->GetCharUID();
	kPacketToDB.m_iTitleID = pMissionInstance->m_iID;
	kPacketToDB.m_sPeriod  = pMissionTemplet->m_sPeriod;
	spUser->SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );

	// �ߺ� ������ ���� �ʵ��� �̸� �������� �̼� ���� - TitleID�� MissionID�� ������
	DeleteMissionInstance( kPacketToDB.m_iTitleID );
}

bool KUserTitleManager::IsExistTitle( IN int iTitleID )
{
	std::map< int, KTitleInfo >::const_iterator mit;
	mit = m_mapTitle.find( iTitleID );
	if( mit == m_mapTitle.end() )
		return false;

	return true;
}

void KUserTitleManager::DeleteMissionInstance( IN int iMissionID )
{
	// �������� �̼ǰ� DB������Ʈ�� �̼��� ����
	m_mapMission.erase( iMissionID );
	m_setDBupdateMission.erase( iMissionID );
}

bool KUserTitleManager::AddTitle( IN int iTitleID, IN std::wstring&	wstrEndDate, OUT __int64& tCTime, OUT bool& bInfinity )
{
	CTime kInfinityTime( 2040, 1, 1, 0, 0, 0 );

	KTitleInfo kTitle;
	kTitle.m_iTitleID = iTitleID;
	kTitle.m_wstrEndDate = wstrEndDate;	

	CTime tEndDate;
	if( !KncUtil::ConvertStringToCTime( kTitle.m_wstrEndDate, tEndDate ) )
	{
		START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
			<< BUILD_LOG( kTitle.m_iTitleID )
			<< BUILD_LOG( kTitle.m_wstrEndDate )
			<< END_LOG;
		return false;
	}

	// �Ⱓ ����������..
	kTitle.m_bInfinity = ( kInfinityTime < tEndDate );
	bInfinity = kTitle.m_bInfinity;

	kTitle.m_kCTime = tEndDate.GetTime();
	tCTime = kTitle.m_kCTime;

	// �̹� ������ �ִ� Īȣ���� �˻�!
	std::map< int, KTitleInfo >::iterator mit = m_mapTitle.find( kTitle.m_iTitleID );
	if( mit == m_mapTitle.end() )
	{
		m_mapTitle.insert( std::make_pair( kTitle.m_iTitleID, kTitle ) );
	}
	else
	{
		START_LOG( clog, L"���� Ÿ��Ʋ ������ ������� �Ⱓ��������!" )
			<< BUILD_LOG( kTitle.m_iTitleID )
			<< BUILD_LOG( mit->second.m_iTitleID )
			<< BUILD_LOG( kTitle.m_wstrEndDate )
			<< BUILD_LOG( mit->second.m_wstrEndDate );

        mit->second = kTitle; // �������!
	}
	
	return true;
}

bool KUserTitleManager::EquipTitle( IN int iTitleID )
{
	SET_ERROR( ERR_UNKNOWN );

	// 1. �̹� �����ϰ� �ִ��� �˻� - TitleID�� 0���� ���°��� ���������� �ǹ�
	if( iTitleID != 0 )
	{
		if( !SiCXSLTitleManager()->IsExistTitleID( iTitleID ) )
		{
			START_LOG( cerr, L"�������� �ʴ� Īȣ�� �����Ϸ��� �ϳ�..?" )
				<< BUILD_LOG( iTitleID )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			return false;		
		}

		// 2. �̹� �����ϰ� �ִ��� �˻�
		if( m_iEquippedTitle == iTitleID )
		{
			SET_ERROR( ERR_TITLE_01 );
			return false;		
		}

		// 3. �������� Ÿ��Ʋ���� �˻�
		std::map< int, KTitleInfo >::const_iterator mit;
		mit = m_mapTitle.find( iTitleID );
		if( mit == m_mapTitle.end() )
		{
			START_LOG( cerr, L"������ ������ ���� Īȣ�� �����Ϸ��� �ϳ�..? ��ŷ�ΰ�?" )
				<< BUILD_LOG( iTitleID )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_00 );
			return false;
		}

		// 4. �Ⱓ Ÿ��Ʋ�̸� �Ⱓ �˻�
		CTime tCurTime = CTime::GetCurrentTime();

		if( tCurTime.GetTime() > mit->second.m_kCTime )
		{
			SET_ERROR( ERR_TITLE_02 );
			return false;
		}
	}

	// 5. ���� ������ �����Ǹ� ����
	m_iEquippedTitle += ( iTitleID - m_iEquippedTitle );

	SET_ERROR( NET_OK );
	return true;
}

//{{ 2010. 10. 18	������	�� ���� ��ų
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PET_AURA_SKILL
//////////////////////////////////////////////////////////////////////////

//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
void KUserTitleManager::GetEquippedStat( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#else
void KUserTitleManager::GetEquippedStat( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	if( GetEquippedTitleID() == 0 )
		return;

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( GetEquippedTitleID() );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� Ÿ��Ʋ�� ������?" )
			<< BUILD_LOG( GetEquippedTitleID() )
			<< END_LOG;
		return;
	}

	// 1. �⺻ ���� ����
	kAddStat.m_iBaseHP		+= pTitleInfo->m_iBaseHP;
	kAddStat.m_iAtkPhysic	+= pTitleInfo->m_iAtkPhysic;
	kAddStat.m_iAtkMagic	+= pTitleInfo->m_iAtkMagic;
	kAddStat.m_iDefPhysic	+= pTitleInfo->m_iDefPhysic;
	kAddStat.m_iDefMagic	+= pTitleInfo->m_iDefMagic;

	// 2. ���� �ɼ� ����
	KStat kSocketOptionStat;
	//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	SiCXSLSocketItem()->GetSocketOptionStat( pTitleInfo->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
#else
	SiCXSLSocketItem()->GetSocketOptionStat( pTitleInfo->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	// ���� ���ϱ�
	kAddStat.AddStat( kSocketOptionStat );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

void KUserTitleManager::GetEquippedStat( IN UINT uiBaseHP, OUT KStat& kStat, IN OUT float& fTotalIncHPRate, IN bool bIsDungeon )
{
	kStat.Init();

	if( GetEquippedTitleID() == 0 )
		return;

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( GetEquippedTitleID() );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� Ÿ��Ʋ�� ������?" )
			<< BUILD_LOG( GetEquippedTitleID() )
			<< END_LOG;
		return;
	}

	// 1. �⺻ ���� ����
	kStat.m_iBaseHP		+= pTitleInfo->m_iBaseHP;
	kStat.m_iAtkPhysic	+= pTitleInfo->m_iAtkPhysic;
	kStat.m_iAtkMagic	+= pTitleInfo->m_iAtkMagic;
	kStat.m_iDefPhysic	+= pTitleInfo->m_iDefPhysic;
	kStat.m_iDefMagic	+= pTitleInfo->m_iDefMagic;

	// 2. ���� �ɼ� ����
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit;
#else
	std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( vit = pTitleInfo->m_vecSocketOption.begin(); vit != pTitleInfo->m_vecSocketOption.end(); ++vit )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"���������ʴ� ���Ͽɼ��� Ÿ��Ʋ�� ���Ͽɼ����� ���õǾ� �ֽ��ϴ�." )
				<< BUILD_LOG( m_iEquippedTitle )
				<< BUILD_LOG( *vit )
				<< END_LOG;
			continue;
		}

		//{{ 2009. 4. 1  ������		�����¸�
		if( pSocketData->m_bDungeonOnly  &&  !bIsDungeon )
			continue;
		//}}


		if( true == pSocketData->m_bPVPOnly && true == bIsDungeon )	
			continue;


		//{{ 2008. 11. 20  ������	HP������ 50% ����
		float fIncreaseHPRate = pSocketData->m_IncreaseHPRate;

		if( ( fTotalIncHPRate + fIncreaseHPRate ) > 0.5f )
		{
			fIncreaseHPRate = 0.5f - fTotalIncHPRate;
		}

		fTotalIncHPRate += fIncreaseHPRate;
		//}}

		kStat.m_iBaseHP		+= (int)( uiBaseHP * fIncreaseHPRate );
		kStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP );
		kStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic );
		kStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic );
		kStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic );
		kStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic );
	}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PET_AURA_SKILL
//////////////////////////////////////////////////////////////////////////
//}}

void KUserTitleManager::GetEquippedTitleBonusRate( IN const bool bIsDungeon, OUT float& fAddTitleExpRate, OUT float& fAddTitleEDRate )
{
	if( GetEquippedTitleID() == 0 )
		return;

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( GetEquippedTitleID() );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� Ÿ��Ʋ�� ������?" )
			<< BUILD_LOG( GetEquippedTitleID() )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	BOOST_TEST_FOREACH( const int, iSocketOption, pTitleInfo->m_vecSocketOption )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( iSocketOption );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"���������ʴ� ���Ͽɼ��� Ÿ��Ʋ�� ���Ͽɼ����� ���õǾ� �ֽ��ϴ�." )
				<< BUILD_LOG( GetEquippedTitleID() )
				<< BUILD_LOG( iSocketOption )
				<< END_LOG;
			continue;
		}
#else
	BOOST_TEST_FOREACH( const short, sSocketOption, pTitleInfo->m_vecSocketOption )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( sSocketOption );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"���������ʴ� ���Ͽɼ��� Ÿ��Ʋ�� ���Ͽɼ����� ���õǾ� �ֽ��ϴ�." )
				<< BUILD_LOG( GetEquippedTitleID() )
				<< BUILD_LOG( sSocketOption )
				<< END_LOG;
			continue;
		}

#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		//{{ 2009. 4. 1  ������		�����¸�
		if( pSocketData->m_bDungeonOnly == true  &&  bIsDungeon == false )
			continue;
		//}}

		if( pSocketData->m_bPVPOnly == true  &&  bIsDungeon == true )
			continue;

		if( pSocketData->m_fIncreaseExpPer > 0.0f )
			fAddTitleExpRate += pSocketData->m_fIncreaseExpPer;

		if( pSocketData->m_fIncreaseEDPer > 0.0f )
			fAddTitleEDRate += pSocketData->m_fIncreaseEDPer;
	}
}

void KUserTitleManager::ExpireTitle( IN KGSUserPtr spUser )
{
	CTime tCurTime = CTime::GetCurrentTime();

	if( m_iEquippedTitle > 0 )
	{
		// �������� Ÿ��Ʋ�� ���� �Ⱓ�˻�
		std::map< int, KTitleInfo >::const_iterator mit;
		mit = m_mapTitle.find( m_iEquippedTitle );
		if( mit == m_mapTitle.end() )
		{
			START_LOG( cerr, L"�������� ���� Ÿ��Ʋ�� �������Դϴ�?" )
				<< BUILD_LOG( m_iEquippedTitle )
				<< END_LOG;

			KEGS_TITLE_EXPIRATION_NOT kNot;
			kNot.m_iTitleID = GetEquippedTitleID();
			spUser->SendPacket( EGS_TITLE_EXPIRATION_NOT, kNot );

			// ��������
			EquipTitle( 0 );
		}
		else
		{
			const KTitleInfo& kTitle = mit->second;
			
			if( tCurTime.GetTime() > kTitle.m_kCTime )
			{
				KEGS_TITLE_EXPIRATION_NOT kNot;
				kNot.m_iTitleID = GetEquippedTitleID();
				spUser->SendPacket( EGS_TITLE_EXPIRATION_NOT, kNot );

				// ��������
				EquipTitle( 0 );
			}
		}
	}

	// �������� Ÿ��Ʋ�� ���� �Ⱓ�˻�
	std::vector< int > vecDelTitle;

	std::map< int, KTitleInfo >::const_iterator mit;
	for( mit = m_mapTitle.begin(); mit != m_mapTitle.end(); ++mit )
	{
		if( tCurTime.GetTime() > mit->second.m_kCTime )
		{
			vecDelTitle.push_back( mit->first );
		}
	}

	if( vecDelTitle.empty() )
		return;

	// ���� Ÿ��Ʋ���� ����
	std::vector< int >::const_iterator vit;
	for( vit = vecDelTitle.begin(); vit != vecDelTitle.end(); ++vit )
	{
		m_mapTitle.erase( *vit );
	}

	// ������ Ÿ��Ʋ�� �̼ǿ� ���� üũ - �Ⱓ�� �̼� ���� ����
	CheckNewMission( spUser, true );

	//{{ 2009. 6. 5  ������		Ÿ��Ʋ �Ⱓ �ٵʿ� ���� ���� ���� ����
	spUser->SendUpdateChangeEquippedItem();
	//}}
}

void KUserTitleManager::AddUpdateMission( IN int iMissionID )
{
	m_setDBupdateMission.insert( iMissionID );
}

//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
void KUserTitleManager::DeleteUpdateMission( IN OUT std::vector< KMissionInstance >& vecMission , IN int iMissionID )
{
	std::set< int >::iterator sit = m_setDBupdateMission.find( iMissionID );
	if( sit != m_setDBupdateMission.end() )
	{
		m_setDBupdateMission.erase( sit );
		START_LOG( clog, L"m_setDBupdateMission �Ϸ� �̼� ���� �Ϸ�" )
			<< BUILD_LOG( iMissionID )
			<< END_LOG;
	}

	{
		std::vector< KMissionInstance >::iterator vit = vecMission.begin();
		for( ; vit != vecMission.end() ; ++vit )
		{
			const KMissionInstance& kMission = (*vit);
			if( kMission.m_iID == iMissionID )
				break;
		}

		if( vit != vecMission.end() )
		{
			vecMission.erase( vit );
			START_LOG( clog, L"vecMission �Ϸ� �̼� ���� �Ϸ�" )
				<< BUILD_LOG( iMissionID )
				<< END_LOG;
		}
	}
}
#endif SERV_ADD_TITLE_CONDITION
//}}

//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
void KUserTitleManager::GetUpdateInfo( IN bool bIsPcBang, OUT int& iInitTitleID, OUT int& iEquippedTitleID, OUT std::vector< KMissionUpdate >& vecMissionData )
#else
void KUserTitleManager::GetUpdateInfo( IN bool bIsPcBang, OUT short& sInitTitleID, OUT short& sEquippedTitleID, OUT std::vector< KMissionUpdate >& vecMissionData )
#endif SERV_TITLE_DATA_SIZE
//}}
{
	vecMissionData.clear();
	vecMissionData.reserve( m_setDBupdateMission.size() );

	// �������� �̼� ������Ʈ
	std::set< int >::const_iterator sit;
	std::map< int, KMissionInstance >::const_iterator mit;
	for( sit = m_setDBupdateMission.begin(); sit != m_setDBupdateMission.end(); ++sit )
	{		
		mit = m_mapMission.find( *sit );
		if( mit == m_mapMission.end() )
		{
			START_LOG( cerr, L"�̼� �ν��Ͻ� ������ ���µ� DB������Ʈ�� �Ϸ��ϳ�?" )
				<< BUILD_LOG( *sit )
				<< END_LOG;
			continue;
		}

		KMissionUpdate kMissionUpdate;
		kMissionUpdate.m_iTitleID = mit->second.m_iID;
		kMissionUpdate.m_vecClearData.reserve( 5 );
		
		std::vector< KSubMissionInstance >::const_iterator vitSub;
		vitSub = mit->second.m_vecSubMissionInstance.begin();

		for( int iIdx = 0; iIdx < MAX_SUB_MISSION_NUM; ++iIdx )
		{
			if( vitSub != mit->second.m_vecSubMissionInstance.end() )
			{
				kMissionUpdate.m_vecClearData.push_back( vitSub->m_sClearData );

				++vitSub;
			}
			else
			{
				kMissionUpdate.m_vecClearData.push_back( 0 );
			}
		}

		vecMissionData.push_back( kMissionUpdate );
	}

	//{{ 2009. 10. 26  ������	PC���̺�Ʈ
	if( bIsPcBang == true )
	{
        if( m_iEquippedTitle == PC_BANG_TITLE_ID )
		{
			//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
			iInitTitleID = m_iEquippedTitle.GetInitValue();
			iEquippedTitleID = m_iEquippedTitle.GetInitValue();
#else
			sInitTitleID = m_iEquippedTitle.GetInitValue();
			sEquippedTitleID = m_iEquippedTitle.GetInitValue();
#endif SERV_TITLE_DATA_SIZE
			//}}			
			return;
		}
	}
	//}}
	
	// �������� Ÿ��Ʋ ������Ʈ
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	iInitTitleID	 = m_iEquippedTitle.GetInitValue(); // ������ �����ߴ� Īȣ
	iEquippedTitleID = m_iEquippedTitle;				// ���� �������� Īȣ
	m_iEquippedTitle.SetValue( iEquippedTitleID );
#else
	sInitTitleID	 = m_iEquippedTitle.GetInitValue(); // ������ �����ߴ� Īȣ
	sEquippedTitleID = m_iEquippedTitle;				// ���� �������� Īȣ
	m_iEquippedTitle.SetValue( sEquippedTitleID );
#endif SERV_TITLE_DATA_SIZE
	//}}	
}

//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
// �� ���� �̼� �˻�
bool KUserTitleManager::GivePetFeed( IN const CXSLItem::ITEM_GRADE eGrade, IN KGSUserPtr spUser ) // �� ���� �ֱ�
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return false;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_GIVE_PET_FEED )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_eItemGrade == eGrade )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = false;

				if( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPetFeedCount )
				{
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
				}

				kNot.m_vecMissionInst.push_back( kMissionInstance );
				AddUpdateMission( kMissionInstance.m_iID );

				START_LOG( clog, L"�̼� TMCT_GIVE_PET_FEED ����" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iItemID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					<< END_LOG;
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
		return true;
	}
	return false;
}

//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
void KUserTitleManager::OnUserUnitDie( IN KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON eReason, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET �� INSTANCE �� SUB MISSION ������ Ʋ��! ��ũ��Ʈ Ȯ�� �ʿ�!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_USER_UNIT_DIE )
			{
				if( eReason != pSubMissionTemplet->m_ClearCondition.m_eDieReason )
					continue;
			}
			else			// ���� ���� ��� �������� ���� �ۿ� ����.
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_USER_UNIT_DIE ����" )
					<< BUILD_LOG( eReason )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}
//}}

#endif SERV_ADD_TITLE_CONDITION
//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// ���볯¥: 2013-08-13
void KUserTitleManager::OnSocketItem( IN KGSUserPtr spUser, IN int iItemLevel, IN int iSocketUseCount )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_SOCKET )
			{
				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;
			}
			else
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += static_cast<short>(iSocketUseCount);

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_ITEM_SOCKET ����" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnEnchantItemLevel( IN KGSUserPtr spUser, IN int iItemLevel, IN int iEnchantLevel, IN bool bEnchantResult )
{
	// ��þƮ ���� �� ��þƮ Ƚ�� ���� ó��
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_ENCHANT_LEVEL )
			{
				if( bEnchantResult == false )	// ��ȭ ���� �ÿ��� ó������
					continue;

				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;

				if( iEnchantLevel < pSubMissionTemplet->m_ClearCondition.m_iEnchantLevel )
					continue;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
				{
					++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

					kNot.m_vecMissionInst.push_back( kMissionInstance );

					START_LOG( clog, L"�̼� TMCT_ITEM_ENCHANT_LEVEL ����" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetUserName() )
						<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
						<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
						;

					// DB ������Ʈ
					AddUpdateMission( kMissionInstance.m_iID );
				}
			}
			else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_ENCHANT_COUNT )
			{
				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
				{
					++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

					kNot.m_vecMissionInst.push_back( kMissionInstance );

					START_LOG( clog, L"�̼� TMCT_ITEM_ENCHANT_COUNT ����" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetUserName() )
						<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
						<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
						;

					// DB ������Ʈ
					AddUpdateMission( kMissionInstance.m_iID );
				}
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnAttribItem( IN KGSUserPtr spUser, IN int iItemLevel )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_ATTRIB )
			{
				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;
			}
			else
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_ITEM_ATTRIB ����" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnResolveItem( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET ������ ����.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET ������ ����.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_RESOLVE )
			{
				// �˻� ������ ����. �׳� ���
			}
			else
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"�̼� TMCT_ITEM_RESOLVE ����" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB ������Ʈ
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// �̼� �߰���� Ŭ���̾�Ʈ ����
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// �̼� ���� �Ϸ� üũ �� ����
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}
#endif // SERV_ADD_TITLE_CONDITION_2013_08
