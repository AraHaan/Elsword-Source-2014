#include "GSUser.h"
#include "NetError.h"

#include "GSUser.h"
#include "NetError.h"

#include ".\useraccountquestmanager.h"
#include "x2data/XSLQuestManager.h"

#include "x2data/XSLItemManager.h"
#include "x2data/XSLSocketItem.h"
#include "ExpTable.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM

KUserAccountQuestManager::KUserAccountQuestManager(void) : 
m_iRewardItemCount( 0 )
{
}

KUserAccountQuestManager::~KUserAccountQuestManager(void)
{
}

int	KUserAccountQuestManager::GetQuestRewardItemCount()
{
	int iQuestRewardItemCount = m_iRewardItemCount;
	m_iRewardItemCount = 0;
	return iQuestRewardItemCount;
}

KQuestInstance* KUserAccountQuestManager::GetAccountQuestInstance( const IN int iQuestID )
{
	std::map<int, KQuestInstance>::iterator	mit;
	mit = m_mapAccountQuesting.find( iQuestID );

	if( mit == m_mapAccountQuesting.end() )
		return NULL;

	return &(mit->second);
}

void KUserAccountQuestManager::ClearAccountQuest()
{
	m_mapAccountQuesting.clear();
	m_mapCompletedAccountQuest.clear();
}

bool KUserAccountQuestManager::AddAccountQuest( IN KQuestInstance& kInfo )
{
	SET_ERROR( NET_OK );

	if( IsAccountQuesting( kInfo.m_iID ) == true )
	{
		START_LOG( cerr, L"������ QUEST ������.. �� �ǿԳ�..?" )
			<< BUILD_LOG( kInfo.m_iID )
			<< BUILD_LOG( kInfo.m_OwnorUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_01 );

		return false;
	}

	m_mapAccountQuesting[kInfo.m_iID] = kInfo;
	return true;
}

bool KUserAccountQuestManager::RemoveAccountQuest( IN int iAccountQuestID )
{
	SET_ERROR( NET_OK );

	std::map< int, KQuestInstance >::iterator mit;
	mit = m_mapAccountQuesting.find( iAccountQuestID );
	if( mit == m_mapAccountQuesting.end() )
	{
		START_LOG( cerr, L"���°� ����� .��~!" )
			<< BUILD_LOG( iAccountQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );

		return false;
	}

	m_mapAccountQuesting.erase( mit );
	return true;
}

void KUserAccountQuestManager::SetAccountQuest( IN std::vector< KQuestInstance >& vecAccountQuest, IN std::vector< KCompleteQuestInfo >& vecAccountCompleteQuest, IN OUT std::vector< KQuestInstance >& vecQuest, IN OUT std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser )
{
	// ���� ����Ʈ �ʱ�ȭ
	ClearAccountQuest();

	// ���� ���� ���� ����Ʈ �Է�
	BOOST_TEST_FOREACH( KQuestInstance&, kQuestInstance, vecAccountQuest )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kQuestInstance.m_iID );
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

			START_LOG( cerr, L"���� QUEST TEMPLET�� ����." )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
				<< END_LOG;
			continue;
		}

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
			case CXSLQuestManager::SQT_CHAR_LEVEL_UP:
				{
					if( static_cast<int>(spUser->GetLevel()) >= pSubTemplet->m_ClearCondition.m_iCharLevel )
						kSub.m_bIsSuccess = true;
				}
				break;
				//{{ 2013. 03. 21	 ���� ����Ʈ - ���� ���� ���� Ŭ���� ���� �߰� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iDungeonClearCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
				//}
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

		m_mapAccountQuesting.insert( std::make_pair( kQuest.m_iID, kQuest ) );
	}

	// ������ ���� ���� ���� ����Ʈ �����͸� ���� Quest ��Ͽ� �߰�(Ŭ�󿡰� ���޿�)
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
	{
		vecQuest.push_back( mit->second );
	}

	// �Ϸ��� ���� ����Ʈ �Է�
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecAccountCompleteQuest )
	{
		std::map< int, KCompleteQuestInfo >::iterator mitQC = m_mapCompletedAccountQuest.find( kInfo.m_iQuestID );
		if( mitQC == m_mapCompletedAccountQuest.end() )
		{
			m_mapCompletedAccountQuest.insert( std::make_pair( kInfo.m_iQuestID, kInfo ) );

			vecCompleteQuest.push_back( kInfo );
		}
		else
		{
			++(mitQC->second.m_iCompleteCount);
		}
	}

	// DB�κ��� ������ �Ϸ�� ���� ����Ʈ ����Ʈ�� ������ After Quest�� open������ üũ�Ѵ�!
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecAccountCompleteQuest )
	{
		CheckAccountQuest_NewQuestByAfterQuest( kInfo.m_iQuestID, spUser, false );
	}

	// ���� ����Ʈ�� Before Quest�� open������ üũ�Ѵ�!
	CheckAccountQuest_NewQuestByBeforeQuest( spUser );
}

void KUserAccountQuestManager::CheckAccountQuest_NewQuestByAfterQuest( IN const int iQuestID, IN KGSUserPtr spUser, IN const bool bForce )
{
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cwarn, L"�������� �ʴ� ���� ����Ʈ ID�Դϴ�!" ) // cwarn ���� �����ϱ�
			<< BUILD_LOG( iQuestID )
			<< END_LOG;
		return;
	}

	// ��������Ʈ�� �������� �ʴ´ٸ�
	if( pQuestTemplet->m_vecAfterQuestID.size() <= 0 )
		return;

	// ���� ����Ʈ�� ��������Ʈ���� �˻�
	BOOST_TEST_FOREACH( const int, iAfterQuestID, pQuestTemplet->m_vecAfterQuestID )
	{
		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iAfterQuestID );
		if( IS_NULL( pAfterQuestTemplet ) )
		{
			START_LOG( cerr, L"���� ���� �ʴ� ���� ����Ʈ �����Դϴ�!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( iAfterQuestID )
				<< END_LOG;
			continue;
		}

		// after����Ʈ�� �ݺ��̳� �ƴϳĸ� üũ!
		if( pAfterQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_REPEAT )
		{
			// after �� ���� �Ǿ� ������ ���� Ÿ�Կ� ������� ��� ���� �޵��� ����
			if( bForce == false )	// ���� ������ �ƴ϶��
			{
				// ���Ȱ� �̺�Ʈ�� �����ϰ� �Ѵ�.
				if( ( pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EPIC || pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT ) == false )
				{
					START_LOG( clog, L"���� ������ �ƴѵ�(ĳ���ͼ��ý�)...After Quest �� ���õ� ����Ʈ�� ����, �̺�Ʈ�� �ƴϴ�." ) // clog �� �����ϱ�
						<< BUILD_LOG( iAfterQuestID )
						<< END_LOG;
					continue;
				}
			}

			// �Ϸ��� �̸� �˻翡�� ����
			if( IsAccountCompleteQuest( iAfterQuestID ) == true )
				continue;
		}

		// ������ �̸� �˻翡�� ����
		if( IsAccountQuesting( iAfterQuestID ) == true )
			continue;

		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = iAfterQuestID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
		if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
		{
			START_LOG( cwarn, L"New Account Quest Req Fail.!(After)" ) // cwarn ���� �����ϱ� 
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( kTempReq.m_iQuestID )
				<< END_LOG;
		}
	}
}

void KUserAccountQuestManager::CheckAccountQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser )
{
	//{{Begin} ����Ʈ Ŭ����� ��������Ʈ�� BeforeQuestID�� �˻��Ͽ� �ڵ����� ���ִ� ����ۼ�.
	std::vector< CXSLQuestManager::QuestTemplet* > vecAccountQuestTemplet;
	SiCXSLQuestManager()->GetAccountQuestTempletList( vecAccountQuestTemplet );

	// ���� ����Ʈ ����Ʈ�� ��ȸ�ϸ鼭 BeforeQuestID �� �˻���.
	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pAccountQuest, vecAccountQuestTemplet )
	{
		if( pAccountQuest == NULL )
		{
			START_LOG( cerr, L"���� ����Ʈ �����Ͱ��� �̻��մϴ�!" )
				<< BUILD_LOG( vecAccountQuestTemplet.size() )
				<< END_LOG;
			continue;
		}

		// �Ϸ��� �̸� �˻翡�� ����
		if( IsAccountCompleteQuest( pAccountQuest->m_iID ) == true )
			continue;

		// ������ �̸� �˻翡�� ����
		if( IsAccountQuesting( pAccountQuest->m_iID ) == true )
			continue;

		// ���� �������� ���� ����Ʈ �� ������ �ִ� ����Ʈ�� üũ
		bool bIsSuccessAccount = true;

		// m_vecBeforeQuestID�� ������ ������� �˻翡�� ����
		if( pAccountQuest->m_Condition.m_vecBeforeQuestID.size() == 0 )
		{
			bIsSuccessAccount = false;
		}

		BOOST_TEST_FOREACH( const int, iBeforeQuestID, pAccountQuest->m_Condition.m_vecBeforeQuestID )
		{
			if( iBeforeQuestID != 0  &&  IsAccountCompleteQuest( iBeforeQuestID ) == false )
			{
				bIsSuccessAccount = false;
				break;
			}
		}

		if( bIsSuccessAccount )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pAccountQuest->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
			{
				START_LOG( cwarn, L"New Account Quest Req Fail.!(Before)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( kTempReq.m_iQuestID )
					<< END_LOG;
			}
		}		
	}
}

bool KUserAccountQuestManager::IsAccountQuesting( IN int iAccountQuestID )
{
	std::map< int, KQuestInstance >::iterator mit = m_mapAccountQuesting.find( iAccountQuestID );
	if( mit != m_mapAccountQuesting.end() )
	{
		// ������
		// ���� �������� ���� ���� ����Ʈ�Դϴ�.
		return true;
	}

	return false;
}

bool KUserAccountQuestManager::IsAccountCompleteQuest( IN int iAccountQuestID )
{
	std::map< int, KCompleteQuestInfo >::iterator mit = m_mapCompletedAccountQuest.find( iAccountQuestID );
	if( mit != m_mapCompletedAccountQuest.end() )
	{
		// ������
		// �Ϸ��� ���� ���� ����Ʈ�Դϴ�.
		return true;
	}

	return false;
}

void KUserAccountQuestManager::CheckAccountQuest( IN KGSUserPtr spUser )
{
	// ���� ���°� �ƴ����� ��ũ��Ʈ�� ������ �����ϵ��� �Ѵ�.
	std::vector<int> vecNewEventQuest;
	std::vector< CXSLQuestManager::QuestTemplet* > vecAccountQuestTemplet;
	SiCXSLQuestManager()->GetAccountQuestTempletList( vecAccountQuestTemplet );

	if( vecAccountQuestTemplet.empty() == false )
	{
		BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pAccountQuest, vecAccountQuestTemplet )
		{
			if( pAccountQuest == NULL )
			{
				START_LOG( cerr, L"���� ����Ʈ �����Ͱ��� �̻��մϴ�!" )
					<< BUILD_LOG( vecAccountQuestTemplet.size() )
					<< END_LOG;
				continue;
			}

			std::map< int, KQuestInstance >::iterator mitQuest = m_mapAccountQuesting.find( pAccountQuest->m_iID );
			// ������ ���°� �ƴ϶��, ��������
			if( mitQuest == m_mapAccountQuesting.end() )
			{
				if( IsAccountCompleteQuest( pAccountQuest->m_iID ) == true )
					continue;

				KEGS_NEW_QUEST_REQ kTempReq;
				kTempReq.m_iQuestID = pAccountQuest->m_iID;
				kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

				if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
				{
					START_LOG( cwarn, L"�ű� ���� ����Ʈ ���� ����!" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( kTempReq.m_iQuestID )
						<< END_LOG;
				}
			}
		}
	}
}

int KUserAccountQuestManager::GetAccountQuestCompleteCount( IN const int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::const_iterator mit;
	mit = m_mapCompletedAccountQuest.find( iQuestID );
	if( mit == m_mapCompletedAccountQuest.end() )
	{
		return 0;
	}

	return mit->second.m_iCompleteCount;
}

bool KUserAccountQuestManager::Handler_EGS_NEW_QUEST_REQ_Account( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser )
{
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ���� ����Ʈ.!" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		return false;
	}
#ifdef SERV_NAVER_CHANNELING
    // ä�θ� ����Ʈ�� ������ ��쿡 �ۺ��Ű� �ٸ��� �ش� ����Ʈ�� ����� �� ���� �Ѵ�. 2014-01-22 �����
    if ( pQuestTemplet->m_ePublisher != spUser->GetChannelCode() ) 
    { // AccountQuest.lua �� m_eQuestType �׸��� ���� ���� default �� 0 �̴�.
        return false;
    }
#endif SERV_NAVER_CHANNELING

	// PC�� ���� ����Ʈ���� üũ
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			return false;
		}
	}

	//# ���� ����Ʈ �̸� �ִ���� ������ ������ ���� �ʴ´�.

	//�̹� �ϰ� �ִ� ����Ʈ����..
	if( IsAccountQuesting( kReq.m_iQuestID ) == true )
	{
		START_LOG( cwarn, L"�̹� ������ �ִ� ���� ����Ʈ�� ����Ϸ���" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		return false;
	}

	// �Ϸ� ���� ����Ʈ
	if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
	{
		// �̹� �Ϸ��� ����Ʈ �ΰ�?
		KCompleteQuestInfo* pCompleteQuestInfo = GetAccountCompleteQuest( kReq.m_iQuestID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_04 );
				return false;
			}

			// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR )
			{
				// ���ؽ�(���ӿ��� ���µǴ� �ð�) �� �Ѵ� �ð��̶�� �������� ����
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// ���� ���� �����Ѱ���?
			if( tCheckTime > tCurTime )
			{
				SET_ERROR( ERR_QUEST_04 );
				return false;
			}
		}
	}

	// To do...
	//[ing]QUEST ������ ���� ��ƾ��� ������ üũ
	if( pQuestTemplet->m_Condition.m_iConditionItemID != 0 )
	{
		if(spUser->IsExistInventoryItem( pQuestTemplet->m_Condition.m_iConditionItemID ) == false )
		{
			return false;
		}
	}

	//QUEST ������ ���� ������ �Ϸ��ؾ��� ����Ʈ �˻�.
	BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
	{
		if( GetAccountQuestCompleteCount( iBeforeQuestID ) == 0 )
		{
			START_LOG( clog, L"����� �ϳ� ���ؿ���.. !!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iBeforeQuestID )
				<< END_LOG;

			return false;
		}
	}

	// ���� �˻�
	// ����Ʈ�� �ޱ� ���� �����̶��
	if( pQuestTemplet->m_Condition.m_iLevel	> spUser->GetLevel() )
	{
		START_LOG( clog, L"���� ����Ʈ�ε� �̷� ������ ���������� ������ ����" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		return false;
	}
	// ����Ʈ�� �ޱ� ���� �����̶��
	//	m_iLimitLevel �� -1 �̸� �������� �ʴ´ٴ� �ǹ�
	if( pQuestTemplet->m_Condition.m_iLimitLevel != -1 && pQuestTemplet->m_Condition.m_iLimitLevel < spUser->GetLevel() )
	{
		START_LOG( clog, L"���� ����Ʈ�ε� �̷� ������ ���������� ������ ����" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLimitLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		return false;
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

					return false;
				}
			}

			// ����Ʈ ���� ������ ������ �󽽷��� �ִ��� �˻�
			if( spUser->IsEnoughSpaceExist( pQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"����Ʈ ������ �޴� ������ �� �κ��丮 ������� �����մϴ�." )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				return false;
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

		kDBReq.m_UserUID	= spUser->GetUID();

		spUser->SendToAccountDB( DBE_NEW_ACCOUNT_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  ������  DB ��� ����Ʈ
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Accept, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_AcceptLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  ������  ���� ���
		int iColumnIndex = KUserStatistics::US_Etc_NQuestAccept;

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return true;
}

void KUserAccountQuestManager::Handler_EGS_QUEST_COMPLETE_REQ_Account( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin /* = false */ )
{
	SET_ERROR( NET_OK );

	bool bCheckAfterQuest = true;
	std::map<int,int> mapAcceptRewardItem;
	std::vector<int>::const_iterator vitAfterQuest;

	// ���������� Ȯ���Ѵ�.	
	if( IsAccountQuesting( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ����Ʈ �ν��Ͻ� ���
	KQuestInstance* pKQuestInst = GetAccountQuestInstance( kReq.m_iQuestID );
	if( pKQuestInst == NULL )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// ����Ʈ ���ø� ���
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"���� ����Ʈ �������ε� ���ø� �����Ͱ� ����." )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_08 );
		goto error_proc;
	}

	vitAfterQuest = pQuestTemplet->m_vecAfterQuestID.begin();
	for( ; vitAfterQuest != pQuestTemplet->m_vecAfterQuestID.end() ; ++vitAfterQuest )
	{
		if( bCheckAfterQuest == false )
			break;

		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( *vitAfterQuest );
		if( pAfterQuestTemplet == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ���� ����Ʈ�� after����Ʈ�� ��ϵǾ��ֽ��ϴ�!" )
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
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		//��� ����̸� �������� �ʴ´�.
		if( bIsAdmin == true )
			continue;

		// ���� ����Ʈ�� ���� �ش� ������ ����.
/*
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
*/
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
		kDBReq.m_bAutoComplete = kReq.m_bAutoComplete;
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_bIsRepeat	= ( pQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_NORMAL );	
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

		if( mapItem.size() > 0 )
		{
			std::map<int,int> mapQuestConditionItem;
			kDBReq.m_mapInsertedItem = mapItem;	//������� ������ ����Ʈ.

			// ��������Ʈ ���� ������ ���� ����Ʈ ������ ����
/*
			//��ڸ� ������ ��������Ʈ ��쵵 �ش� �������� �������� �ʱ�����.
			if( bIsAdmin == false )
			{
				//SiCXSLQuestManager()->GetQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
				SiCXSLQuestManager()->GetAccountQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
			}
*/
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
				//�κ��� �����ϸ� �׳� �������� �˷��ְ� �ϷḦ ���� �ʴ´�.
				SET_ERROR( ERR_QUEST_13 );
				goto error_proc;
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
			int iColumnIndex = KUserStatistics::US_Etc_NQuestComplete;

			spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
			//}}
		}
		//}}

		spUser->SendToAccountDB( DBE_ACCOUNT_QUEST_COMPLETE_REQ, kDBReq );

		return;
	}

error_proc:

	KEGS_QUEST_COMPLETE_ACK kAck;
	//�Ϸ�üũ�� ���� ����Ʈ ID�� �ٽ� �����ش�.
	kAck.m_kCompleteQuestInfo.m_iQuestID = kReq.m_iQuestID;
	kAck.m_iOK	= NetError::GetLastError();
	spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
}

void KUserAccountQuestManager::Handler_DBE_QUEST_COMPLETE_ACK_Account( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser )
{
	const int iQuestID = kAck.m_kCompleteQuestInfo.m_iQuestID;

	//DBó�� ������ ���
	SET_ERROR( NET_OK );

	KEGS_QUEST_COMPLETE_ACK	kPacket;
	kPacket.m_kCompleteQuestInfo.m_iQuestID = iQuestID;

	if( IsAccountQuesting( iQuestID ) == false )
	{
		START_LOG( cerr, L"+��+;; ���� ����Ʈ�� ������ ����Ʈ�� ����." )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );
		goto end_proc;
	}

	START_LOG( clog, L"���� ����Ʈ �Ϸ� ::::" )
		<< BUILD_LOG( spUser->GetCharUID() )
		<< BUILD_LOG( spUser->GetCharName() )
		<< BUILD_LOG( spUser->GetUserName() )
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( iQuestID );

	// Quest Instance ����
	if( RemoveAccountQuest( iQuestID ) == true )
	{
		// �Ϸ� ó��
		std::map< int, KCompleteQuestInfo >::iterator mitCQ;
		mitCQ = m_mapCompletedAccountQuest.find( iQuestID );
		if( mitCQ == m_mapCompletedAccountQuest.end() )
		{
			m_mapCompletedAccountQuest.insert( std::make_pair( iQuestID, kAck.m_kCompleteQuestInfo ) );
		}
		else
		{
			++(mitCQ->second.m_iCompleteCount); // �Ϸ� ī��Ʈ�� �ø���..
			mitCQ->second.m_tCompleteDate = kAck.m_kCompleteQuestInfo.m_tCompleteDate; // �Ϸ� ��¥�� �����Ѵ�.
		}

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"ACCOUNT QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto end_proc;
		}		

		kPacket.m_iOK		= NetError::GetLastError();

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

		//{{ 2009. 1. 20  ������	��������ġ ����ó��
		if( spUser->GetLevel() < SiKGameSysVal()->GetLimitsLevel() )
		{
			spUser->m_kEXP.AddExp( pQuestTemplet->m_Reward.m_iEXP );
		}
		//}}

#ifdef SERV_PVP_NEW_SYSTEM	// �߸��� ������ �ؿ��� ����
		spUser->AddAPoint( pQuestTemplet->m_Reward.m_iSP );
#else
		spUser->m_iSPoint += pQuestTemplet->m_Reward.m_iSP;
#endif SERV_PVP_NEW_SYSTEM

		spUser->CheckCharLevelUp();

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

		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );


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
		CheckAccountQuest_NewQuestByAfterQuest( pQuestTemplet->m_iID, spUser, true );

		// BeforeQuestID�� ��ϵ� ������ ���� �Ϸ� ����Ʈ�� �ִ��� Ȯ���Ͽ� ����Ʈ�� �����Ѵ�.
		CheckAccountQuest_NewQuestByBeforeQuest( spUser );	

		// �̺�Ʈ �ݺ� ����Ʈ��� ����� �޵��� ���� 
		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT
			&& pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_REPEAT )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pQuestTemplet->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
			{
				START_LOG( cwarn, L"[����] New Account Quest Req Fail.!(After)" )
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

	kPacket.m_iOK = NetError::GetLastError();
	spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );

}

void KUserAccountQuestManager::Handler_CharaterLevelUp_Account( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapAccountQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	CTime tCurr = CTime::GetCurrentTime();
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"Account QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}
#ifdef SERV_NAVER_CHANNELING
        if( pQuestTemplet->m_ePublisher != spUser->GetChannelCode() ) // ä�θ� ���� ����Ʈ
        {
            continue;
        }
#endif SERV_NAVER_CHANNELING

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		// ��¥, ���� ���ǿ� ���� ������ �Ϸᰡ ���� �ʰ� �Ѵ�.
		// Ȱ��ȭ �ð��� �ƴϸ�
		if( SiCXSLQuestManager()->IsValidAccountQuestDate( kQuestInstance.m_iID, tCurr ) == false )
		{
			continue;
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

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

			// 2. ������ ����Ʈ�� �´���
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_CHAR_LEVEL_UP )
				continue;

			// 3. �ش� ������ �´���
			if( pSubQuestTemplet->m_ClearCondition.m_iCharLevel > static_cast<int>( spUser->GetLevel() ) )
				continue;

			// 4. �̹� �Ϸ��� ����Ʈ ���� Ȯ��!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			// ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup_AccountQuest( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"Account ����Ʈ SQT_CHAR_LEVEL_UP ����" )
					<< BUILD_LOG( static_cast<int>( spUser->GetLevel() ) )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iCharLevel )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					<< END_LOG;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}

bool KUserAccountQuestManager::CheckCompleteSubQuest_BeforGroup_AccountQuest( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser )
{
	// �׷��� �� ��������Ʈ �� �� �׷��� ��� Ŭ���� �Ǿ����� Ȯ���Ѵ�.

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iQuestID );
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
	std::map< int, KQuestInstance >::iterator mit = m_mapAccountQuesting.find( iQuestID );
	if( mit == m_mapAccountQuesting.end() )
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

void KUserAccountQuestManager::GetAccountQuesting( OUT std::vector< KQuestUpdate >& vecQuestData )
{
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
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

//{{ 2013. 03. 21	 ���� ����Ʈ - ���� ���� ���� Ŭ���� ���� �߰� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
void KUserAccountQuestManager::Handler_OnDungeonClear( IN const KGSUserPtr spUser, 
													   IN const int iDungeonID, 
													   IN const char cDifficulty, 
													   IN const char cDungeonMode, 
													   IN const std::set< UidType >& setPartyMembers,
													   IN const std::map< UidType, bool >& mapHaveExpInDungeon,
													   IN const std::map< UidType, bool >& mapSuitableLevelInfo )
{
	SET_ERROR( NET_OK );

	if( m_mapAccountQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	CTime tCurr = CTime::GetCurrentTime();
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"ACCOUNT QUEST TEMPLET ������ ����.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		// ��¥, ���� ���ǿ� ���� ������ �Ϸᰡ ���� �ʰ� �Ѵ�.
		// Ȱ��ȭ �ð��� �ƴϸ�
		if( SiCXSLQuestManager()->IsValidAccountQuestDate( kQuestInstance.m_iID, tCurr ) == false )
		{
			continue;
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

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

			bool bRVal = false;
			bool bValidDungeonID = false;

			// 1. �Ϸ�Ȱ��� üũ
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. ��ȿ�� �����̶��
			if( SiCXSLQuestManager()->CheckValidDungeonID( pSubQuestTemplet->m_iID ) == true ) 
			{
				bValidDungeonID = true;

				// SubQuest �� �ִ� ������ �÷��� �� ���� �ƴ϶��
				if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pSubQuestTemplet->m_iID, iDungeonID + static_cast<int>(cDifficulty), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					continue;
			}

			// 3. ���� �׷��� sub Quest �� ��� �Ϸ� �� ���°� �ƴ϶��
			if( CheckCompleteSubQuest_BeforGroup_AccountQuest( pQuestTemplet->m_iID, pSubQuestTemplet->m_iID, spUser ) == false )
				continue;

			// 4. Ÿ�Կ� ���� ���� üũ
			switch( pSubQuestTemplet->m_eClearType )
			{

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
							bRVal = true;
						}
					}
				}
				break;
			}

			if( bRVal == true )
			{
				switch( pSubQuestTemplet->m_eClearType )
				{
				case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
					{
						//{{ 2010. 05. 03  ������	���� Ŭ���� ī��Ʈ ���� ����
						// Ŭ���� ���� ī��Ʈ�� ���� �ʰ� �����Ѵ�.
						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
						{
							kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
						}
						//}}

						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
							kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

						kNot.m_vecQuestInst.push_back( kQuestInstance );
					}
					break;

				default:
					{
						START_LOG( cerr, L"�������� �ʴ� Accout Quest �� Ŭ���� Ÿ���Դϴ�." )
							<< BUILD_LOG( pQuestTemplet->m_iID )
							<< BUILD_LOG( pSubQuestTemplet->m_iID )
							<< BUILD_LOG( pSubQuestTemplet->m_eClearType )
							<< END_LOG;
						continue;
					}
					break;
				}

				START_LOG( clog, L"Account Quest ���� ���� ����� ����Ʈ ���� �Ϸ�" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					<< END_LOG;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

bool KUserAccountQuestManager::IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo )
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
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
//}

//{{ 2013. 03. 21	 ���� ����Ʈ - ���Ϻ� Ȱ��ȭ ��� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
void KUserAccountQuestManager::CheckResetDayAccountQuest_AutoAccept( IN KGSUserPtr spUser )
{
	// �̺�Ʈ Ÿ���� ����Ʈ�� ��� �����´�.
	std::vector< CXSLQuestManager::QuestTemplet* > vecAccountQuestTemplet;
	SiCXSLQuestManager()->GetAccountQuestTempletList( vecAccountQuestTemplet );

	// ������ ������ ����Ʈ ���� Ȯ������
	std::vector<int> vecCompleteDayQuest;
	BOOST_TEST_FOREACH( CXSLQuestManager::QuestTemplet*, pAccountQuestTemplet, vecAccountQuestTemplet )
	{
		if( pAccountQuestTemplet == NULL )
		{
			continue;
		}

		// �̺�Ʈ ���� ����Ʈ�� �ƴϸ� ����!
		if( pAccountQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_DAY )
		{
			continue;
		}

		// ���� �� ���ΰ�?
		bool bAccept = false;

		// �̹� �Ϸ��� ����Ʈ �ΰ�?
		KCompleteQuestInfo* pCompleteQuestInfo = GetAccountCompleteQuest( pAccountQuestTemplet->m_iID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"����Ʈ �Ϸ�� �ߴµ� �Ϸᳯ¥ ������ �̻��մϴ�." )
					<< BUILD_LOG( pAccountQuestTemplet->m_iID )
					<< END_LOG;

				continue;
			}

			// �̹� �Ϸ��� ����Ʈ��� Ŭ������ ��¥�� Ȯ���Ͽ� ���� ���� �������� üũ�Ѵ�.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR )
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
			kTempReq.m_iQuestID = pAccountQuestTemplet->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
			{
				START_LOG( cwarn, L"New Account Quest Req Fail.!(After)" ) // cwarn ���� �����ϱ� 
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( kTempReq.m_iQuestID )
					<< END_LOG;
			}
		}
	}
}

KCompleteQuestInfo*	KUserAccountQuestManager::GetAccountCompleteQuest( IN int iAccountQuestID )
{
	std::map< int, KCompleteQuestInfo >::iterator mit;
	mit = m_mapCompletedAccountQuest.find( iAccountQuestID );
	if( mit == m_mapCompletedAccountQuest.end() )
		return NULL;

	return &(mit->second);
}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
//}

#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}