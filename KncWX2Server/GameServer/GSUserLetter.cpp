#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"
//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	#include "GameSysVal/GameSysVal.h"
#endif SERV_BLOCK_TRADE
//}}

//{{ 2012. 09. 26	������		���� ���� ���� ����
#ifdef SERV_POST_COPY_BUG_FIX
	#include "SMSPhoneNumberManager.h"
#endif SERV_POST_COPY_BUG_FIX
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventRewardDataRefreshManager.h"
#else
	#include "RewardTable.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� ������
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// �������� ������� �ǽð����� ���� ����!
		KELG_FIND_USER_SEND_LETTER_NOT kPacketNot;
		kPacketNot.m_kLetter = kPacket_.m_iRewardLetter;
		SendToLoginServer( ELG_FIND_USER_SEND_LETTER_NOT, kPacketNot );

		//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		if( kPacket_.m_iDBIndex > 0 )
		{
			KDBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT kDBReq;

			m_kUserUnitSelectRewardManager.DeleteAndGetFirstSelectUnitRewardInfo( kPacket_.m_iDBIndex, kDBReq );

			SendToAccountDB( DBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT, kDBReq );
		}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		//���� ���� �ߴٸ� Ŭ�󿡰� ���� ��Ŷ�� ������.
		CTime TempTime = CTime::GetCurrentTime();
		KEGS_EVENT_CHUNG_GIVE_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrGetItemTime = TempTime.Format(L"%Y-%m-%d %H:%M:%S"); 
		SendPacket( EGS_EVENT_CHUNG_GIVE_ITEM_ACK , kPacketAck );
		SetChungGiveItem(true);
		SetChungGiveItemTime(TempTime);
#endif SERV_EVENT_CHUNG_GIVE_ITEM
	}
}

//{{ 2008. 9. 23  ������	��ü�� Ȯ��
IMPL_ON_FUNC( EGS_GET_POST_LETTER_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_POST_LETTER_LIST_ACK kPacketAck;

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( !m_kUserLetterBox.GetLetterList( kPacket_.m_cLetterListType, kPacket_.m_iPostPageNo, kPacketAck.m_vecPostItemTitle ) )
#else
	if( !m_kUserLetterBox.GetLetterList( kPacket_.m_bSystemLetter, kPacket_.m_iPostPageNo, kPacketAck.m_vecPostItemTitle ) )
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_00;
		SendPacket( EGS_GET_POST_LETTER_LIST_ACK, kPacketAck );
		return;
	}

	kPacketAck.m_iOK = NetError::NET_OK;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacketAck.m_iTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( kPacket_.m_cLetterListType );
#else
	kPacketAck.m_iTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( kPacket_.m_bSystemLetter );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	SendPacket( EGS_GET_POST_LETTER_LIST_ACK, kPacketAck );
}

IMPL_ON_FUNC( DBE_GET_POST_LETTER_LIST_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KEGS_NEW_POST_LETTER_INFO_NOT kPacket;

		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		m_kUserLetterBox.Init( GetThisPtr<KGSUser>(), kPacket_.m_vecPostItem, kPacket.m_iNewUserLetterCount, kPacket.m_iNewSystemLetterCount, kPacket.m_iNewRelationshipLetterCount, kPacket.m_vecNewLetterTitle );
#else
		m_kUserLetterBox.Init( GetThisPtr<KGSUser>(), kPacket_.m_vecPostItem, kPacket.m_iNewUserLetterCount, kPacket.m_iNewSystemLetterCount, kPacket.m_vecNewLetterTitle );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

		m_kUserLetterBox.InitBlackList( kPacket_.m_vecBlackList );

		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		kPacket.m_iTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( KEGS_GET_POST_LETTER_LIST_REQ::LLT_NORMAL );
#else
		kPacket.m_iTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( false );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

		// Ŭ���̾�Ʈ�� �������� ���� ���� �����ش�.
		SendPacket( EGS_NEW_POST_LETTER_INFO_NOT, kPacket );
	}

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	std::vector< int >	vecAccessRewardIDList;
	m_kUserUnitSelectRewardManager.GetRewardAndClear( vecAccessRewardIDList );
	if( !vecAccessRewardIDList.empty() )  // ���ӽ� �����ؾ��� �������� �ִٸ� �������� ����
	{
		BOOST_TEST_FOREACH( const int, iRewardID, vecAccessRewardIDList )
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = iRewardID;			
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif SERV_SECOND_SECURITY
	//}}
}
//}}

//{{ 2012. 10. 08	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
IMPL_ON_FUNC( EGS_SEND_LETTER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEND_LETTER_REQ, EGS_SEND_LETTER_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidPostWordString( CXSLStringFilter::FT_POST, kPacket_.m_wstrMessage ) == false )
	{
		KEGS_SEND_LETTER_ACK kAck;
		kAck.m_iOK = NetError::ERR_STRING_FILTER_06;
		SendPacket( EGS_SEND_LETTER_ACK, kAck );
		return;
	}

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		kPacket_.m_wstrTitle = SiKStringFilterManager()->FilteringNoteString( kPacket_.m_wstrTitle, L'��' );
		kPacket_.m_wstrMessage = SiKStringFilterManager()->FilteringNoteString( kPacket_.m_wstrMessage, L'��' );
	}
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_SEND_LETTER_ACK kAck;

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
			SendPacket( EGS_SEND_LETTER_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_PREPARE_INSERT_LETTER_TO_POST_REQ kPacketToDB;
	KEGS_SEND_LETTER_ACK kPacketAck;

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockSendLetter() == true )
	{
		KEGS_SEND_LETTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		KEGS_SEND_LETTER_ACK kPacket;
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_01;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_POST_LETTER_03;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_SEND_LETTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_01;
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	// 1. ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_SEND_LETTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}

	// 2. ���� ĳ���Ϳ��� ������ ���� �� ����.
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrToNickName,GetCharName()) )
#else
	if( kPacket_.m_wstrToNickName == GetCharName() )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_11;
		goto end_proc;
	}

	//{{ 2009. 3. 24  ������	GMüũ
	if( kPacket_.m_wstrToNickName == NetError::GetErrStr( NetError::STR_GM_NICKNAME_1 )  ||
		kPacket_.m_wstrToNickName == NetError::GetErrStr( NetError::STR_GM_NICKNAME_2 ) )
	{
		kPacketAck.m_iOK = NetError::ERR_GM_CHAR_DENY_00;
		goto end_proc;
	}
	//}}

	// 3. ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrToNickName ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrTitle ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	//{{ 2008. 10. 27  ������	GM�޽��� �߰�	
	if( kPacket_.m_bIsGmMessage )
	{
		if( GetAuthLevel() < SEnum::UAL_GM )
		{
			START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOGc( GetAuthLevel() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_VERIFY_12;
			goto end_proc;
		}
	}
	//}}

	// 4. ���� ���� �˻�
	{
		std::string strTitle = KncUtil::toNarrowString( kPacket_.m_wstrTitle );
		if( strTitle.size() > 36 )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_18;
			goto end_proc;
		}
	}

	{
		std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );
		if( strMessage.size() > 400 )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_19;
			goto end_proc;
		}
	}

	//{{ 2012. 08. 23	�ڼ���	���� ���� ����
//#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	// ����ȭ ������ ���� ������
	kPacketToDB.m_iIncrementED	= m_iED.GetChangeValue();
	m_iED.SetValue( m_iED );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
//#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}

	// ����!!
	// ���� �۾� ���� ������ ���� ������ m_iED�� ��ȭ���� ���� �ѹ� ó���� ������ �ʰ� �ֽ��ϴ�.
	// �۾� ���� ������ �߰��ȴٸ� end_proc: �� �ش��ϴ� �ѹ� ó���� ���־�� �մϴ�.

	// 5. ÷�ι��� ���� ���� �۾�
	if( kPacket_.m_iED > 0  ||  kPacket_.m_iItemUID > 0 ) 
	{
		if( kPacket_.m_iED <= 0 )
		{
			// 5-1. �ش� �������� �κ����� ���
			KInventoryItemInfo kInvenItemInfo;
			if( !m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kInvenItemInfo ) )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
				goto end_proc;
			}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInvenItemInfo.m_kItemInfo.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"�ش� �������� ���ø� ������ ���� ���Ͽ����ϴ�." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< BUILD_LOG( kInvenItemInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
				goto end_proc;
			}

			if( kInvenItemInfo.m_kItemInfo.m_sEndurance < pItemTemplet->m_Endurance )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_08;
				goto end_proc;
			}

			//{{ 2008. 10. 16  ������	���� �˻�
			if( kPacket_.m_iQuantity <= 0  ||  kInvenItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iQuantity )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_01;
				goto end_proc;
			}
			//}}

			//{{ 2010. 01. 04  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_PRE

			if( m_kInventory.IsDBUpdateItem( kPacket_.m_iItemUID ) == false )
			{
				START_LOG( cerr, L"PC�� �����̾� �������� ���� ÷���Ϸ��� �߽��ϴ�." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_PC_BANG_00;
				goto end_proc;
			}

#endif SERV_PC_BANG_PRE
			//}}

			if( pItemTemplet->m_bVested )
			{
				//{{ 2009. 8. 27  ������	�ͼ������� �к��̸� �������۰���
				if( kInvenItemInfo.m_kItemInfo.IsSealedItem() == false )
				{
					kPacketAck.m_iOK = NetError::ERR_POST_LETTER_16;
					goto end_proc;
				}
				//}}
			}

			//{{ 2009. 1. 22  ������	�Ⱓ�� �������� ������ ÷���Ҽ� ������ ����ó��
			if( kInvenItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_INFINITY )
			{
				if( kInvenItemInfo.m_kItemInfo.m_sPeriod > 0 )
				{
					kPacketAck.m_iOK = NetError::ERR_POST_LETTER_16;
					goto end_proc;
				}
			}
			//}}

			//{{ 2008. 12. 21  ������	���Ұ� �˻�
			if( kInvenItemInfo.m_kItemInfo.m_cEnchantLevel < 0 )
			{
				kPacketAck.m_iOK = NetError::ERR_RESTORE_ITEM_06;
				goto end_proc;
			}
			//}}

			// 5-2. ������ ���
			kPacketToDB.m_iSendLetterCost = static_cast<int>( ( ( ( pItemTemplet->m_Price * kPacket_.m_iQuantity ) * 0.2f ) * 0.05f ) + 200 );

			if( m_iED < kPacketToDB.m_iSendLetterCost )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_12;
				goto end_proc;
			}

			// 5-3. �ش� ���������� ����

			//{{ 2012. 08. 21	�ڼ���	���� ���� ����
//#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
			kPacketToDB.m_kSendLetter.m_iItemUID		= kInvenItemInfo.m_iItemUID;
			kPacketToDB.m_kSendLetter.m_iUsageType		= kInvenItemInfo.m_kItemInfo.m_cUsageType;
//#endif SERV_TRADE_LOGIC_CHANGE_LETTER
			//}}
			kPacketToDB.m_kSendLetter.m_iScriptIndex	= kInvenItemInfo.m_kItemInfo.m_iItemID;
			kPacketToDB.m_kSendLetter.m_iQuantity		= kPacket_.m_iQuantity;
			kPacketToDB.m_kSendLetter.m_cEnchantLevel	= kInvenItemInfo.m_kItemInfo.m_cEnchantLevel;
			kPacketToDB.m_kSendLetter.m_kAttribEnchantInfo = kInvenItemInfo.m_kItemInfo.m_kAttribEnchantInfo;
			kPacketToDB.m_kSendLetter.m_vecItemSocket	= kInvenItemInfo.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-20	// �ڼ���
			kPacketToDB.m_kSendLetter.m_byteExpandedSocketNum	= kInvenItemInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kPacketToDB.m_kSendLetter.m_vecRandomSocket = kInvenItemInfo.m_kItemInfo.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			//{{ 2009. 8. 27  ������	�к�
			kPacketToDB.m_kSendLetter.m_ucSealData		= kInvenItemInfo.m_kItemInfo.m_ucSealData;
			if( kInvenItemInfo.m_kItemInfo.IsSealedItem() )
			{
				kPacketToDB.m_kSendLetter.m_ucSealData -= 100; // �к�����
			}
			//}}

			const int iBeforeED = m_iED;

			m_iED -= kPacketToDB.m_iSendLetterCost;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
			//}}
		}
		else
		{
			// 5-1. ���� ������ ����
			kPacketToDB.m_iSendLetterCost = static_cast<int>( ( kPacket_.m_iED * 0.05f ) + 200 );

			if( m_iED < kPacketToDB.m_iSendLetterCost )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_12;
				goto end_proc;
			}

			// 5-2. ÷�ΰ����� �׼����� ����
			if( kPacket_.m_iED > SEnum::UI_MAX_LETTER_ED )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_02;
				goto end_proc;
			}

			// 5-3. ������ + ÷��ED����
			if( m_iED < ( kPacket_.m_iED + kPacketToDB.m_iSendLetterCost ) )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_02;
				goto end_proc;
			}

			kPacketToDB.m_kSendLetter.m_iScriptIndex = CXSLItem::EDI_BRONZE_ED; // ����� ED
			kPacketToDB.m_kSendLetter.m_iQuantity	 = kPacket_.m_iED;

			//{{ 2012. 08. 23	�ڼ���	���� ���� ����
//#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
			int iBeforeED = m_iED;

			m_iED -= kPacket_.m_iED;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_SEND_ED, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
			//}}

			iBeforeED = m_iED;

			m_iED -= kPacketToDB.m_iSendLetterCost;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
			//}}
//#endif SERV_TRADE_LOGIC_CHANGE_LETTER
			//}}
		}
	}
	else
	{
		// ���� ������
		kPacketToDB.m_iSendLetterCost = 200;

		if( m_iED < kPacketToDB.m_iSendLetterCost )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_12;
			goto end_proc;
		}

		const int iBeforeED = m_iED;

		m_iED -= kPacketToDB.m_iSendLetterCost;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}
	}

	//{{ 2012. 08. 23	�ڼ���	���� ���� ����
//#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	// kPacketToDB.m_kSendLetter.m_iQuantity �� kPacketToDB.m_iSendLetterCost ���� m_iED�� ChangeValue�� �����ϰ� �ִ�.
	m_iED.SetValue( m_iED );
//#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}

	// 6. DB�� ������Ʈ üũ
	kPacketToDB.m_iFromUnitUID				= GetCharUID();
	kPacketToDB.m_iItemUID					= kPacket_.m_iItemUID;
	kPacketToDB.m_wstrToNickName			= kPacket_.m_wstrToNickName;
	kPacketToDB.m_kSendLetter.m_cScriptType = ( kPacket_.m_bIsGmMessage ? KPostItemInfo::LT_MESSAGE : KPostItemInfo::LT_POST_OFFICE );
	kPacketToDB.m_kSendLetter.m_wstrTitle	= kPacket_.m_wstrTitle;
	kPacketToDB.m_kSendLetter.m_wstrMessage = kPacket_.m_wstrMessage;
	kPacketToDB.m_kSendLetter.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_kSendLetter.m_wstrFromNickName = GetCharName();

	SendToGameDB( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ, kPacketToDB );
	return;

end_proc: 
	//{{ 2012. 08. 23	�ڼ���	���� ���� ����
//#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	// �����ߴٸ� ����ȭ �����͵��� �ѹ� ������Ѵ�.
	if( ( kPacketToDB.m_iIncrementED != 0 ) ||
		( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange.empty() == false ) ||
		( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted.empty() == false ) )
	{
		if( kPacketToDB.m_iIncrementED != 0 )
		{
			// ED ��Ȳ �ѹ�
			m_iED += kPacketToDB.m_iIncrementED;
		}

		if( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange.empty() == false )
		{
			m_kInventory.RollBackInitQuantity( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		}

		if( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted.empty() == false )
		{
			m_kInventory.RollBackDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
		}
	}
//#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}
	SendPacket( EGS_SEND_LETTER_ACK, kPacketAck );
}

IMPL_ON_FUNC( DBE_PREPARE_INSERT_LETTER_TO_POST_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
	//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEND_LETTER_ACK );
	//#else
	//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KDBE_INSERT_LETTER_TO_POST_REQ kPacketToDB;
	KEGS_SEND_LETTER_ACK kPacketAck;

	if( kPacket_.m_iOK != NetError::NET_OK )
		goto end_proc;

	if( kPacket_.m_kSendLetter.m_iScriptIndex > 0  &&  kPacket_.m_kSendLetter.m_iScriptIndex != CXSLItem::EDI_BRONZE_ED )
	{
		// 1. �ش� �������� �κ����� ���
		KInventoryItemInfo kInvenItemInfo;
		if( !m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kInvenItemInfo ) )
		{
			START_LOG( cerr, L"send letter���� �˻縦 �ߴµ�.. �������� ���� ���� �ʴ´�? �ð����ΰ�.." )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
			goto end_proc;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInvenItemInfo.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"�ش� �������� ���ø� ������ ���� ���Ͽ����ϴ�." )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kInvenItemInfo.m_kItemInfo.m_iItemID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
			goto end_proc;
		}

		// 2. �ش� �������� ���� ����
		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased = 0;
			m_kInventory.DecreaseQuantity( kPacket_.m_iItemUID, kPacket_.m_kSendLetter.m_iQuantity, iDecreased, KDeletedItemInfo::DR_LETTER );
			if( kPacket_.m_kSendLetter.m_iQuantity != iDecreased )
			{
				START_LOG( cerr, L"���� ��ȭ �̻�." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< BUILD_LOG( kPacket_.m_kSendLetter.m_iQuantity )
					<< BUILD_LOG( iDecreased )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_01;
				goto end_proc;
			}
		}
		else
		{
			KInventoryItemInfo kDelInvenItem;			
			if( !m_kInventory.DeleteItem( kPacket_.m_iItemUID, kDelInvenItem, KDeletedItemInfo::DR_LETTER ) )
			{
				START_LOG( cerr, L"������ ���� ����." )
					<< BUILD_LOG( kPacket_.m_iItemUID )					
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_01;
				goto end_proc;
			}
		}

		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kInvenItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_POST, kInvenItemInfo.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_POST, kInvenItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

		// 3. DB������Ʈ �غ�
		KInventoryItemInfo kResultItemInfo;
		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
		//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_kInventory.GetInventorySlotInfo( kInvenItemInfo.m_cSlotCategory, kInvenItemInfo.m_sSlotID, kResultItemInfo );
		//#else
		//		m_kInventory.GetInventorySlotInfo( kInvenItemInfo.m_cSlotCategory, kInvenItemInfo.m_cSlotID, kResultItemInfo );
		//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		kPacketToDB.m_vecUpdatedInventorySlot.push_back( kResultItemInfo );

		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	}

	// 4. DB ������Ʈ
	kPacketToDB.m_iFromUnitUID				= GetCharUID();
	kPacketToDB.m_wstrToNickName			= kPacket_.m_wstrToNickName;
	kPacketToDB.m_kSendLetter				= kPacket_.m_kSendLetter;
	kPacketToDB.m_iSendLetterCost			= kPacket_.m_iSendLetterCost;
	SendToGameDB( DBE_INSERT_LETTER_TO_POST_REQ, kPacketToDB );
	return;

end_proc:

	// ED �ѹ�
	if( kPacket_.m_kSendLetter.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
	{
		const int iBeforeED = m_iED;

		m_iED += kPacket_.m_kSendLetter.m_iQuantity;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_ED_ROLLBACK, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}
	}

	const int iBeforeED2 = m_iED;

	m_iED += kPacket_.m_iSendLetterCost;

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ED �����
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST_ROLLBACK, iBeforeED2 );
#endif SERV_USER_ABUSER_MANAGER
	//}}

	//{{ 2012. 08. 23	�ڼ���	���� ���� ����
//#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	if( ( kPacket_.m_iIncrementED != 0 ) ||
		( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.empty() != true ) ||
		( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.empty() != true ) )
	{
		if( kPacket_.m_iIncrementED != 0 )
		{
			// ED ��Ȳ �ѹ�
			m_iED.SetValue( m_iED - kPacket_.m_iIncrementED );
			m_iED += kPacket_.m_iIncrementED;
		}

		if( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.empty() != true )
		{
			m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		}

		if( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.empty() != true )
		{
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		}

		START_LOG( cerr, L"ED �� ������ ����ȭ�� �����Ͽ����ϴ�.")
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iIncrementED )
			<< BUILD_LOG( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.empty() )
			<< BUILD_LOG( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.empty() )
			<< END_LOG;
	}
//#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}

	kPacketAck.m_iOK = kPacket_.m_iOK;
	kPacketAck.m_iED = m_iED;
	SendPacket( EGS_SEND_LETTER_ACK, kPacketAck );

	//{{ 2012. 09. 26	������		���� ���� ���� ����
#ifdef SERV_POST_COPY_BUG_FIX
	if( kPacket_.m_iOK == NetError::ERR_POST_LETTER_20 )
	{
		START_LOG( cout, L"�̹� ���Ӽ������� ���ϴг��� üũ �ߴµ� DBüũ���� ���ٿ� �õ��� �ִ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetUserID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrToNickName );

		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"���ϴг��� ĳ���Ϳ��� ���������� �õ��� ���� �߰�! : UserUID(%d), NickName(%s)" ) % GetUID() % GetCharName() );
		//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_SEND_A_LETTER_TO_SAME_NICKNAME, kPacketNot.m_vecPhoneNum );
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
}

IMPL_ON_FUNC( EGS_GET_ITEM_FROM_LETTER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_ITEM_FROM_LETTER_REQ, EGS_GET_ITEM_FROM_LETTER_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_GET_ITEM_FROM_LETTER_ACK kAck;

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
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_GET_ITEM_FROM_LETTER_ACK kPacketAck;
	KDBE_GET_ITEM_FROM_LETTER_REQ kPacketToDB;
	std::map< int, KItemInfo > mapInsertItem;
	kPacketToDB.m_iED = 0;

	KPostItemInfo kPostItemInfo;
	if( m_kUserLetterBox.GetLetter( kPacket_.m_iPostNo, kPostItemInfo ) == false )
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_04;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
		return;
	}

	if( kPostItemInfo.m_iScriptIndex <= 0 )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ÷�ι��� ����ִ°� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )			
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
		return;
	}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacketToDB.m_cLetterType = m_kUserLetterBox.GetLetterType( kPacket_.m_iPostNo );
#else
	kPacketToDB.m_bSystemLetter = m_kUserLetterBox.IsSystemLetter( kPacket_.m_iPostNo );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( m_kUserLetterBox.IsItemIDLetter( kPacket_.m_iPostNo ) == false )
#else
	if( kPacketToDB.m_bSystemLetter  &&  
		kPostItemInfo.m_cScriptType != KPostItemInfo::LT_MESSAGE  &&  
		kPostItemInfo.m_cScriptType != KPostItemInfo::LT_BROKEN_ITEM  &&
		kPostItemInfo.m_cScriptType != KPostItemInfo::LT_WEB_POINT_EVENT )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
	{
		// ���� �������� ItemInfo ���

		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		SiKEventRewardDataRefreshManager()->GetRewardItem( kPostItemInfo, mapInsertItem );
#else
		SiKRewardTable()->GetRewardItem( kPostItemInfo, mapInsertItem );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
	}
	else
	{
		// ���� �˻�
		if( kPostItemInfo.m_iQuantity <= 0 )
		{
			START_LOG( cerr, L"Ŭ���̾�Ʈ���� ÷�ι��� ����ִ°� �˻������ٵ�.." )
				<< BUILD_LOG( kPacket_.m_iPostNo )
				<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )
				<< BUILD_LOG( kPostItemInfo.m_iQuantity )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
			return;
		}

		// ÷�ε� �������� ED�� ���..
		if( kPostItemInfo.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
		{
			//{{ 2009. 4. 28  ������	�ִ� EDüũ
			if( CheckMaxED( kPostItemInfo.m_iQuantity ) == false )
			{
				START_LOG( cwarn, L"�ִ�ED�� �ʰ���! Ŭ�󿡼� üũ�����ٵ�.." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetED() )
					<< BUILD_LOG( kPostItemInfo.m_iQuantity )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
				SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
				return;
			}
			//}}
			kPacketToDB.m_iED = kPostItemInfo.m_iQuantity;

			//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
			if( SiKGameSysVal()->GetLetterAndTradeMonitoringIsOn() == true )
			{
				if( m_kUserHackingManager.AddLetterMonitoringList( kPostItemInfo.m_iFromUnitUID ) == true )
				{					
					KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ PacketReq;
					CTime tCurTime = CTime::GetCurrentTime();

					PacketReq.m_iRecvUnitUID = GetCharUID();
					PacketReq.m_wstrRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
					PacketReq.m_cType = 0;

					SendToLogDB( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, PacketReq );
				}
			}
#endif SERV_MONITORING_LETTER_AND_TRADE
			//}}
		}
		else
		{
			KItemInfo kInsertItemInfo;
			if( !m_kUserLetterBox.GetItemInfo( kPacket_.m_iPostNo, kInsertItemInfo ) )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_04;
				SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
				return;
			}

			mapInsertItem.insert( std::make_pair( kInsertItemInfo.m_iItemID, kInsertItemInfo ) );
		}
	}

	if( 1 < mapInsertItem.size() )
	{
		START_LOG( cerr, L"���� ÷�� ��ǰ�� ������ �����Ѵ�??")
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< BUILD_LOG( mapInsertItem.size() )
			<< END_LOG;
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
		return;
	}

	if( kPacketToDB.m_bSystemLetter != true )
	{
		if( kPacketToDB.m_iED <= 0 )
		{
			// �κ��丮 �˻�
			KItemQuantityUpdate kUpdateTemp;
			if( m_kInventory.TrialPrepareInsert( mapInsertItem, kUpdateTemp, kPacketToDB.m_vecItemInfo ) == false )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_05;
				SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
				return;
			}
			kPacketToDB.m_vecItemInfo.clear();
			if( mapInsertItem.empty() == true )
			{
				START_LOG( cerr, L"÷�ε� �������� ������ �����ϴ�.")
					<< BUILD_LOG( mapInsertItem.size() )
					<< END_LOG;
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
				SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
				return;
			}
			std::map< int, KItemInfo >::iterator it = mapInsertItem.begin();
			kPacketToDB.m_vecItemInfo.push_back( it->second );
		}
	}
	else
	{
		if( kPacketToDB.m_iED != 0 )
		{
			START_LOG( cerr, L"�ý��� ���� ED���� �����Ǿ� �ֽ��ϴ�." )
				<< BUILD_LOG( kPacketToDB.m_iED )
				<< END_LOG;
		}

		// �κ��丮 �˻�
		if( m_kInventory.PrepareInsert( mapInsertItem, kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo, true ) == false )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_05;
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
			return;
		}
	}

	// ����ȭ ������ ���� ������
	kPacketToDB.m_iIncrementED	= m_iED.GetChangeValue();
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	m_iED.SetValue( m_iED );

	// [����] ����������� ������ �����ϴ��� �ϴ� ������ DB�� ������.
	// ��ü�� ���̺��� �����۵��� �����ϱ� ���ؼ�.

	kPacketToDB.m_iUnitUID	    = GetCharUID();
	kPacketToDB.m_iPostNo	    = kPacket_.m_iPostNo;	
	SendToGameDB( DBE_GET_ITEM_FROM_LETTER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_ITEM_FROM_LETTER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_GET_ITEM_FROM_LETTER_ACK kPacket;
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_iPostNo		= kPacket_.m_iPostNo;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacket.m_cLetterType = kPacket_.m_cLetterType;
#else
	kPacket.m_bSystemLetter = kPacket_.m_bSystemLetter;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	kPacket.m_mapResultItem = kPacket_.m_mapInsertedItem;

	if( ( kPacket_.m_iIncrementED != 0 ) ||
		( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.empty() != true ) ||
		( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.empty() != true ) )
	{
		if( kPacket_.m_iIncrementED != 0 )
		{
			// ED ��Ȳ �ѹ�
			m_iED.SetValue( m_iED - kPacket_.m_iIncrementED );
			m_iED += kPacket_.m_iIncrementED;
		}

		if( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.empty() != true )
		{
			m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		}

		if( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.empty() != true )
		{
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		}

		START_LOG( cerr, L"ED �� ������ ����ȭ�� �����Ͽ����ϴ�.")
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iIncrementED )
			<< BUILD_LOG( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.empty() )
			<< BUILD_LOG( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.empty() )
			<< END_LOG;
		kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket );
		return;
	}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ED÷�ι� �ޱ�
		if( kPacket_.m_iED > 0 )
		{
			const int iBeforeED = m_iED;

			m_iED += kPacket_.m_iED;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_RECV_ED, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
			//}}

			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( 0 );
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PPostLetter, kPacket_.m_iED );

#ifdef SERV_USER_STATISTICS_RENEWAL
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PPostLetter, kPacket_.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
		}

		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		switch( kPacket_.m_cLetterType )
		{
		case KPostItemInfo::LT_POST_OFFICE:
			{
				// ���� ������� ÷�ι� �ʱ�ȭ �Ѵ�..
				m_kUserLetterBox.SetReceivedFlag( kPacket_.m_iPostNo );
			}
			break;
		default:
			{
				if( kPacket_.m_mapItemInfo.empty() == false )
				{
					// ������ ÷�ι� �ޱ�
					m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
				}

				// �ý��� ������� �����..
				m_kUserLetterBox.DeleteLetter( kPacket_.m_iPostNo );
				kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
			}
		}
#else
		if( kPacket_.m_bSystemLetter == true )
		{
			if( kPacket_.m_mapItemInfo.empty() == false )
			{
				// ������ ÷�ι� �ޱ�
				m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
			}

			// �ý��� ������� �����..
			m_kUserLetterBox.DeleteLetter( kPacket_.m_iPostNo );
			kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
		}
		else
		{
			// ���� ������� ÷�ι� �ʱ�ȭ �Ѵ�..
			m_kUserLetterBox.SetReceivedFlag( kPacket_.m_iPostNo );
		}
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	}
	else
	{
		// �����ߴٸ� �� �̻� ������ �ʿ䰡 ����. �ٷ� ����
		kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket );
		return;
	}

	kPacket.m_iED = m_iED;

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( ( kPacket_.m_cLetterType == KPostItemInfo::LT_POST_OFFICE ) && ( kPacket_.m_iED <= 0 ) )
#else
	if( ( kPacket_.m_bSystemLetter != true ) && ( kPacket_.m_iED <= 0 ) )
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	{
		// �ߺ� ����,�߰� ����
		KItemQuantityUpdate kQuantityUpdate;
		m_kInventory.FlushQuantityChange( kQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kQuantityUpdate.m_vecDeleted );

		if( ( kQuantityUpdate.m_mapQuantityChange.empty() != true ) || ( kQuantityUpdate.m_vecDeleted.empty() != true ) )
		{
			START_LOG( cerr, L"����ȭ ��ƾ�� ���� �Դµ� �̷� ���� �߻��ؼ��� �ȵȴ�.")
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kPacket_.m_iPostNo)
				//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
				<< BUILD_LOGc( kPacket_.m_cLetterType )
#else
				<< BUILD_LOG( kPacket_.m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
				//}
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket );
			return;
		}

		kPacket.m_iUnitUID = GetCharUID();
		if( m_kInventory.FixUpInventoryItemForLetter( kPacket_.m_vecUpdatedInventorySlot, kPacket.m_kItemQuantityUpdate, kPacket.m_vecMoveItemSlotInfo, kPacket.m_vecKInventorySlotInfo ) == false )
		{
			START_LOG( cerr, L"������ ���Կ� �����Ͽ����ϴ�!! ����� �־ �ȵǰ� ���� ���� ���� ����!! ��ƾ�� �����ϴ� ó�� �ܰ迡�� �˻� �߾��� ���̴�.") // �α� �ǹ� ���ϰ� �����
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kPacket_.m_iPostNo)
				//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
				<< BUILD_LOGc( kPacket_.m_cLetterType )
#else
				<< BUILD_LOG( kPacket_.m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
				//}
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket );
			return;
		}

		// �ߺ� ����,�߰� ����
		m_kInventory.FlushQuantityChange( kQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kQuantityUpdate.m_vecDeleted );
		// �̰��� ���´µ� ������ �����ϴµ� ���� �ߴٸ�....�ѹ��ؼ� �������� ������Ʈ ������Ѵ�.
		SendToGameDB( DBE_GET_ITEM_FROM_LETTER_ARRANGE_REQ, kPacket );
	}
	else
	{
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket );
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �����ۿ� ���� ���
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( kPacket_.m_cLetterType != KPostItemInfo::LT_POST_OFFICE )
#else
	if( kPacket_.m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	{
		KStatisticsKey kKey;
		std::map< int, int >::const_iterator mitStat;
		for( mitStat = kPacket_.m_mapInsertedItem.begin(); mitStat != kPacket_.m_mapInsertedItem.end(); ++mitStat )
		{
			kKey.m_vecIntKey.clear();
			kKey.m_vecIntKey.push_back( mitStat->first );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, mitStat->second );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, mitStat->second );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_LETTER, kPacket_.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#else
//{{ 2008. 9. 8  ������		���� ������ ����
IMPL_ON_FUNC( EGS_SEND_LETTER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEND_LETTER_REQ, EGS_SEND_LETTER_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_SEND_LETTER_ACK kAck;

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
			SendPacket( EGS_SEND_LETTER_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_PREPARE_INSERT_LETTER_TO_POST_REQ kPacketToDB;
	KEGS_SEND_LETTER_ACK kPacketAck;

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockSendLetter() == true )
	{
		KEGS_SEND_LETTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		KEGS_SEND_LETTER_ACK kPacket;
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_01;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_POST_LETTER_03;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_SEND_LETTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_01;
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	// 1. ü��ID ��� ����
	if( IsGuestUser() )
	{
		KEGS_SEND_LETTER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_SEND_LETTER_ACK, kPacket );
		return;
	}

	// 2. ���� ĳ���Ϳ��� ������ ���� �� ����.
	if( kPacket_.m_wstrToNickName == GetCharName() )
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_11;
		goto end_proc;
	}

	//{{ 2009. 3. 24  ������	GMüũ
	if( kPacket_.m_wstrToNickName == NetError::GetErrStr( NetError::STR_GM_NICKNAME_1 )  ||
		kPacket_.m_wstrToNickName == NetError::GetErrStr( NetError::STR_GM_NICKNAME_2 ) )
	{
		kPacketAck.m_iOK = NetError::ERR_GM_CHAR_DENY_00;
		goto end_proc;
	}
	//}}

	// 3. ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrToNickName ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrTitle ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	//{{ 2008. 10. 27  ������	GM�޽��� �߰�	
	if( kPacket_.m_bIsGmMessage )
	{
		if( GetAuthLevel() < SEnum::UAL_GM )
		{
			START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOGc( GetAuthLevel() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_VERIFY_12;
			goto end_proc;
		}
	}
	//}}

	// 4. ���� ���� �˻�
	{
		std::string strTitle = KncUtil::toNarrowString( kPacket_.m_wstrTitle );
		if( strTitle.size() > 36 )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_18;
			goto end_proc;
		}
	}

	{
		std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );
		if( strMessage.size() > 400 )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_19;
			goto end_proc;
		}
	}

#ifdef SERV_FREE_MAIL_TICKET
	bool bFreeMail = false;
	for( int iFreeMailTicketIDIndex = 0;
		iFreeMailTicketIDIndex < sizeof( _CONST_FREE_MAIL_TICKET_::arriFreeMailTicketID ) / sizeof( _CONST_FREE_MAIL_TICKET_::arriFreeMailTicketID[0] );
		++iFreeMailTicketIDIndex )
	{
		if( m_kInventory.IsExist( _CONST_FREE_MAIL_TICKET_::arriFreeMailTicketID[iFreeMailTicketIDIndex], true ) == true )
		{
			bFreeMail = true;
			break;
		}
	}
#endif //SERV_FREE_MAIL_TICKET

	// 5. ÷�ι��� ���� ���� �۾�
	if( kPacket_.m_iED > 0  ||  kPacket_.m_iItemUID > 0 ) 
	{
		if( kPacket_.m_iED <= 0 )
		{
			// 5-1. �ش� �������� �κ����� ���
			KInventoryItemInfo kInvenItemInfo;
			if( !m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kInvenItemInfo ) )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
				goto end_proc;
			}

			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInvenItemInfo.m_kItemInfo.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"�ش� �������� ���ø� ������ ���� ���Ͽ����ϴ�." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< BUILD_LOG( kInvenItemInfo.m_kItemInfo.m_iItemID )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
				goto end_proc;
			}

			if( kInvenItemInfo.m_kItemInfo.m_sEndurance < pItemTemplet->m_Endurance )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_08;
				goto end_proc;
			}

			//{{ 2008. 10. 16  ������	���� �˻�
			if( kPacket_.m_iQuantity <= 0  ||  kInvenItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iQuantity )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_01;
				goto end_proc;
			}
			//}}

			//{{ 2010. 01. 04  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_PRE

			if( m_kInventory.IsDBUpdateItem( kPacket_.m_iItemUID ) == false )
			{
				START_LOG( cerr, L"PC�� �����̾� �������� ���� ÷���Ϸ��� �߽��ϴ�." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_PC_BANG_00;
				goto end_proc;
			}

#endif SERV_PC_BANG_PRE
			//}}

			if( pItemTemplet->m_bVested )
			{
				//{{ 2009. 8. 27  ������	�ͼ������� �к��̸� �������۰���
				if( kInvenItemInfo.m_kItemInfo.IsSealedItem() == false )
				{
					kPacketAck.m_iOK = NetError::ERR_POST_LETTER_16;
					goto end_proc;
				}
				//}}
			}

			//{{ 2009. 1. 22  ������	�Ⱓ�� �������� ������ ÷���Ҽ� ������ ����ó��
			if( kInvenItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_INFINITY )
			{
				if( kInvenItemInfo.m_kItemInfo.m_sPeriod > 0 )
				{
					kPacketAck.m_iOK = NetError::ERR_POST_LETTER_16;
					goto end_proc;
				}
			}
			//}}

			//{{ 2008. 12. 21  ������	���Ұ� �˻�
			if( kInvenItemInfo.m_kItemInfo.m_cEnchantLevel < 0 )
			{
				kPacketAck.m_iOK = NetError::ERR_RESTORE_ITEM_06;
				goto end_proc;
			}
			//}}

#ifdef	SERV_SHARING_BANK_TEST
			if( m_kInventory.IsShareItem(kInvenItemInfo.m_iItemUID) == true )
			{
				START_LOG(cerr, L"���� ���� : ���� ���� �������� ���� �Ϸ� ����.")
					<< BUILD_LOG(kInvenItemInfo.m_iItemUID)
					<< BUILD_LOG(GetName())
					<< END_LOG;

				//	��� ���� �߰�
				CTime tCurTime = CTime::GetCurrentTime();

				KELOG_ITEM_TRADE_ERROR_NOT kErrorNot;
				kErrorNot.m_iUnitUID = GetCharUID();
				kErrorNot.m_iType = SEnum::TET_POST;
				kErrorNot.m_wsrtRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

				SendToLogDB( ELOG_ITEM_TRADE_ERROR_NOT, kErrorNot );

				kPacketAck.m_iOK = NetError::ERR_SHARE_BANK_01;
				goto end_proc;
			}
#endif	SERV_SHARING_BANK_TEST

			// 5-2. ������ ���
#ifdef SERV_FREE_MAIL_TICKET
			if( bFreeMail == true )
				kPacketToDB.m_iSendLetterCost = 0;
			else
#endif //SERV_FREE_MAIL_TICKET
			kPacketToDB.m_iSendLetterCost = static_cast<int>( ( ( ( pItemTemplet->m_Price * kPacket_.m_iQuantity ) * 0.2f ) * 0.05f ) + 200 );

			if( GetED() < kPacketToDB.m_iSendLetterCost )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_12;
				goto end_proc;
			}

			// 5-3. �ش� ���������� ����
			kPacketToDB.m_kSendLetter.m_iScriptIndex	= kInvenItemInfo.m_kItemInfo.m_iItemID;
			kPacketToDB.m_kSendLetter.m_iQuantity		= kPacket_.m_iQuantity;
			kPacketToDB.m_kSendLetter.m_cEnchantLevel	= kInvenItemInfo.m_kItemInfo.m_cEnchantLevel;
			kPacketToDB.m_kSendLetter.m_kAttribEnchantInfo = kInvenItemInfo.m_kItemInfo.m_kAttribEnchantInfo;
			kPacketToDB.m_kSendLetter.m_vecItemSocket	= kInvenItemInfo.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-20	// �ڼ���
			kPacketToDB.m_kSendLetter.m_byteExpandedSocketNum	= kInvenItemInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
			//{{ 2013. 06. 04	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			kPacketToDB.m_kSendLetter.m_vecRandomSocket	= kInvenItemInfo.m_kItemInfo.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
			//{{ 2009. 8. 27  ������	�к�
			kPacketToDB.m_kSendLetter.m_ucSealData		= kInvenItemInfo.m_kItemInfo.m_ucSealData;
			if( kInvenItemInfo.m_kItemInfo.IsSealedItem() )
			{
				kPacketToDB.m_kSendLetter.m_ucSealData -= 100; // �к�����
			}
			//}}

			//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			DecreaseED( kPacketToDB.m_iSendLetterCost, KUserEDManager::ER_OUT_ED_SEND_LETTER_COMMISSION );
#else
			const int iBeforeED = m_iED;

			m_iED -= kPacketToDB.m_iSendLetterCost;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
			//#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
			//#endif SERV_USER_ABUSER_MANAGER
			//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			
		}
		else
		{
			// 5-1. ���� ������ ����
#ifdef SERV_FREE_MAIL_TICKET
			if( bFreeMail == true )
				kPacketToDB.m_iSendLetterCost = 0;
			else
#endif //SERV_FREE_MAIL_TICKET
			kPacketToDB.m_iSendLetterCost = static_cast<int>( ( kPacket_.m_iED * 0.05f ) + 200 );

			if( GetED() < kPacketToDB.m_iSendLetterCost )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_12;
				goto end_proc;
			}

			// 5-2. ÷�ΰ����� �׼����� ����
			if( kPacket_.m_iED > SEnum::UI_MAX_LETTER_ED )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_02;
				goto end_proc;
			}

			// 5-3. ������ + ÷��ED����
			if( GetED() < ( kPacket_.m_iED + kPacketToDB.m_iSendLetterCost ) )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_02;
				goto end_proc;
			}

			kPacketToDB.m_kSendLetter.m_iScriptIndex = CXSLItem::EDI_BRONZE_ED; // ����� ED
			kPacketToDB.m_kSendLetter.m_iQuantity	 = kPacket_.m_iED;

			//{{ ���� ED �ŷ��� ED ����ȭ - ��μ�
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC

			//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//DecreaseED( kPacket_.m_iED, KUserEDManager::ER_OUT_ED_SEND_LETTER_ATTACH );
			//DecreaseED( kPacketToDB.m_iSendLetterCost, KUserEDManager::ER_OUT_ED_SEND_LETTER_COMMISSION );
			int iBeforeED = GetED();

			iBeforeED -= kPacket_.m_iED;

			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KUserEDManager::ER_OUT_ED_SEND_LETTER_ATTACH, iBeforeED );

			iBeforeED -= kPacketToDB.m_iSendLetterCost;

			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KUserEDManager::ER_OUT_ED_SEND_LETTER_COMMISSION, iBeforeED );
#else
			int iBeforeED = m_iED;

			iBeforeED -= kPacket_.m_iED;

			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_SEND_ED, iBeforeED );

			iBeforeED -= kPacketToDB.m_iSendLetterCost;

			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}

			KDBE_SYNC_ED_REQ kDBReq;
			kDBReq.m_bIsGmMessage = kPacket_.m_bIsGmMessage;
			kDBReq.m_wstrToNickName = kPacket_.m_wstrToNickName;
			kDBReq.m_wstrTitle = kPacket_.m_wstrTitle;
			kDBReq.m_wstrMessage = kPacket_.m_wstrMessage;
			kDBReq.m_iED = kPacket_.m_iED;
			kDBReq.m_iItemUID = kPacket_.m_iItemUID;
			kDBReq.m_iQuantity = kPacket_.m_iQuantity;
			kDBReq.m_bIsGmMessage = kPacket_.m_bIsGmMessage;
			
			kDBReq.m_iFromUnitUID = GetCharUID();
			//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			kDBReq.m_iChangeED = m_kEDManager.GetChangeValue();
#else
			kDBReq.m_iChangeED = m_iED.GetChangeValue();
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}
			kDBReq.m_iSendLetterCost = kPacketToDB.m_iSendLetterCost;

			//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			m_kEDManager.Init( GetED() - kPacket_.m_iED - kPacketToDB.m_iSendLetterCost, KUserEDManager::ER_SET_ED_SYNC_FOR_SEND_LETTER );
#else
			m_iED.SetValue( GetED() - kPacket_.m_iED - kPacketToDB.m_iSendLetterCost );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}

			SendToGameDB( DBE_SYNC_ED_REQ, kDBReq );
			return;
#else
			int iBeforeED = m_iED;

			m_iED -= kPacket_.m_iED;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_SEND_ED, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
			//}}

			iBeforeED = m_iED;

			m_iED -= kPacketToDB.m_iSendLetterCost;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
			//}}
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
			//}}
		}
	}
	else
	{
		// ���� ������
#ifdef SERV_FREE_MAIL_TICKET
		if( bFreeMail == true )
			kPacketToDB.m_iSendLetterCost = 0;
		else
#endif //SERV_FREE_MAIL_TICKET
		kPacketToDB.m_iSendLetterCost = 200;

		if( GetED() < kPacketToDB.m_iSendLetterCost )
		{
			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_12;
			goto end_proc;
		}

		//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		DecreaseED( kPacketToDB.m_iSendLetterCost, KUserEDManager::ER_OUT_ED_SEND_LETTER_COMMISSION );
#else
		const int iBeforeED = m_iED;

		m_iED -= kPacketToDB.m_iSendLetterCost;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		
	}

	// 6. DB�� ������Ʈ üũ
	kPacketToDB.m_iFromUnitUID				= GetCharUID();
	kPacketToDB.m_iItemUID					= kPacket_.m_iItemUID;
	kPacketToDB.m_wstrToNickName			= kPacket_.m_wstrToNickName;
	kPacketToDB.m_kSendLetter.m_cScriptType = ( kPacket_.m_bIsGmMessage ? KPostItemInfo::LT_MESSAGE : KPostItemInfo::LT_POST_OFFICE );
	kPacketToDB.m_kSendLetter.m_wstrTitle	= kPacket_.m_wstrTitle;
	kPacketToDB.m_kSendLetter.m_wstrMessage = kPacket_.m_wstrMessage;
	kPacketToDB.m_kSendLetter.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_kSendLetter.m_wstrFromNickName = GetCharName();

	SendToGameDB( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ, kPacketToDB );
	return;

end_proc:
	SendPacket( EGS_SEND_LETTER_ACK, kPacketAck );
}

IMPL_ON_FUNC( DBE_PREPARE_INSERT_LETTER_TO_POST_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEND_LETTER_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KDBE_INSERT_LETTER_TO_POST_REQ kPacketToDB;
	KEGS_SEND_LETTER_ACK kPacketAck;

	if( kPacket_.m_iOK != NetError::NET_OK )
		goto end_proc;

	if( kPacket_.m_kSendLetter.m_iScriptIndex > 0  &&  kPacket_.m_kSendLetter.m_iScriptIndex != CXSLItem::EDI_BRONZE_ED )
	{
		// 1. �ش� �������� �κ����� ���
		KInventoryItemInfo kInvenItemInfo;
		if( !m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kInvenItemInfo ) )
		{
			START_LOG( cerr, L"send letter���� �˻縦 �ߴµ�.. �������� ���� ���� �ʴ´�? �ð����ΰ�.." )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
			goto end_proc;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInvenItemInfo.m_kItemInfo.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"�ش� �������� ���ø� ������ ���� ���Ͽ����ϴ�." )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kInvenItemInfo.m_kItemInfo.m_iItemID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_07;
			goto end_proc;
		}

		// 2. �ش� �������� ���� ����
		if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased = 0;
			m_kInventory.DecreaseQuantity( kPacket_.m_iItemUID, kPacket_.m_kSendLetter.m_iQuantity, iDecreased, KDeletedItemInfo::DR_LETTER );
			if( kPacket_.m_kSendLetter.m_iQuantity != iDecreased )
			{
				START_LOG( cerr, L"���� ��ȭ �̻�." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< BUILD_LOG( kPacket_.m_kSendLetter.m_iQuantity )
					<< BUILD_LOG( iDecreased )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_01;
				goto end_proc;
			}
		}
		else
		{
			KInventoryItemInfo kDelInvenItem;			
			if( !m_kInventory.DeleteItem( kPacket_.m_iItemUID, kDelInvenItem, KDeletedItemInfo::DR_LETTER ) )
			{
				START_LOG( cerr, L"������ ���� ����." )
					<< BUILD_LOG( kPacket_.m_iItemUID )					
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_01;
				goto end_proc;
			}
		}

		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kInvenItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_POST, kInvenItemInfo.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_POST, kInvenItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

		// 3. DB������Ʈ �غ�
		KInventoryItemInfo kResultItemInfo;
		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_kInventory.GetInventorySlotInfo( kInvenItemInfo.m_cSlotCategory, kInvenItemInfo.m_sSlotID, kResultItemInfo );
//#else
//		m_kInventory.GetInventorySlotInfo( kInvenItemInfo.m_cSlotCategory, kInvenItemInfo.m_cSlotID, kResultItemInfo );
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		kPacketToDB.m_vecUpdatedInventorySlot.push_back( kResultItemInfo );

		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	}

	// 4. DB ������Ʈ
	kPacketToDB.m_iFromUnitUID				= GetCharUID();
	kPacketToDB.m_wstrToNickName			= kPacket_.m_wstrToNickName;
	kPacketToDB.m_kSendLetter				= kPacket_.m_kSendLetter;
	kPacketToDB.m_iSendLetterCost			= kPacket_.m_iSendLetterCost;
	SendToGameDB( DBE_INSERT_LETTER_TO_POST_REQ, kPacketToDB );
	return;

end_proc:

	// ED �ѹ�
	if( kPacket_.m_kSendLetter.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
	{
		//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		IncreaseED( kPacket_.m_kSendLetter.m_iQuantity, KUserEDManager::ER_IN_ED_SEND_LETTER_ATTACH_ROLLBACK );
#else
		const int iBeforeED = m_iED;

		m_iED += kPacket_.m_kSendLetter.m_iQuantity;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_ED_ROLLBACK, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		
	}

	//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	IncreaseED( kPacket_.m_iSendLetterCost, KUserEDManager::ER_IN_ED_SEND_LETTER_COMMISSION_ROLLBACK );
#else
	const int iBeforeED2 = m_iED;

	m_iED += kPacket_.m_iSendLetterCost;

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED �����
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST_ROLLBACK, iBeforeED2 );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	kPacketAck.m_iOK = kPacket_.m_iOK;
	kPacketAck.m_iED = GetED();
	SendPacket( EGS_SEND_LETTER_ACK, kPacketAck );

	//{{ 2012. 09. 26	������		���� ���� ���� ����
#ifdef SERV_POST_COPY_BUG_FIX
	if( kPacket_.m_iOK == NetError::ERR_POST_LETTER_20 )
	{
		START_LOG( cout, L"�̹� ���Ӽ������� ���ϴг��� üũ �ߴµ� DBüũ���� ���ٿ� �õ��� �ִ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetUserID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrToNickName );

		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"���ϴг��� ĳ���Ϳ��� ���������� �õ��� ���� �߰�! : UserUID(%d), NickName(%s), ToNickName(%s)" ) % GetUID() % GetCharName() % kPacket_.m_wstrToNickName );
		//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_SEND_A_LETTER_TO_SAME_NICKNAME, kPacketNot.m_vecPhoneNum );
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
}

IMPL_ON_FUNC( EGS_GET_ITEM_FROM_LETTER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_ITEM_FROM_LETTER_REQ, EGS_GET_ITEM_FROM_LETTER_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_GET_ITEM_FROM_LETTER_ACK kAck;

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
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_GET_ITEM_FROM_LETTER_ACK kPacketAck;
	KDBE_GET_ITEM_FROM_LETTER_REQ kPacketToDB;
	std::map< int, KItemInfo > mapInsertItem;
	kPacketToDB.m_iED = 0;

	KPostItemInfo kPostItemInfo;
	if( m_kUserLetterBox.GetLetter( kPacket_.m_iPostNo, kPostItemInfo ) == false )
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_04;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
		return;
	}

	if( kPostItemInfo.m_iScriptIndex <= 0 )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ÷�ι��� ����ִ°� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )			
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
		return;
	}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacketToDB.m_cLetterType = m_kUserLetterBox.GetLetterType( kPacket_.m_iPostNo );
#else
	kPacketToDB.m_bSystemLetter = m_kUserLetterBox.IsSystemLetter( kPacket_.m_iPostNo );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( m_kUserLetterBox.IsItemIDLetter( kPacket_.m_iPostNo ) == false )
#else
	if( kPacketToDB.m_bSystemLetter  &&  
		kPostItemInfo.m_cScriptType != KPostItemInfo::LT_MESSAGE  &&  
		kPostItemInfo.m_cScriptType != KPostItemInfo::LT_BROKEN_ITEM  &&
		kPostItemInfo.m_cScriptType != KPostItemInfo::LT_WEB_POINT_EVENT )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	{
		// ���� �������� ItemInfo ���
		
		//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		SiKEventRewardDataRefreshManager()->GetRewardItem( kPostItemInfo, mapInsertItem );
#else
		SiKRewardTable()->GetRewardItem( kPostItemInfo, mapInsertItem );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
	}
	else
	{
		// ���� �˻�
		if( kPostItemInfo.m_iQuantity <= 0 )
		{
			START_LOG( cerr, L"Ŭ���̾�Ʈ���� ÷�ι��� ����ִ°� �˻������ٵ�.." )
				<< BUILD_LOG( kPacket_.m_iPostNo )
				<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )
				<< BUILD_LOG( kPostItemInfo.m_iQuantity )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
			SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
			return;
		}

		// ÷�ε� �������� ED�� ���..
		if( kPostItemInfo.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
		{
			//{{ 2009. 4. 28  ������	�ִ� EDüũ
			if( CheckMaxED( kPostItemInfo.m_iQuantity ) == false )
			{
				START_LOG( cwarn, L"�ִ�ED�� �ʰ���! Ŭ�󿡼� üũ�����ٵ�.." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetED() )
					<< BUILD_LOG( kPostItemInfo.m_iQuantity )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_06;
				SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
				return;
			}
			//}}
			kPacketToDB.m_iED = kPostItemInfo.m_iQuantity;

			//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
			if( SiKGameSysVal()->GetLetterAndTradeMonitoringIsOn() == true )
			{
				if( m_kUserHackingManager.AddLetterMonitoringList( kPostItemInfo.m_iFromUnitUID ) == true )
				{					
					KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ PacketReq;
					CTime tCurTime = CTime::GetCurrentTime();
					
					PacketReq.m_iRecvUnitUID = GetCharUID();
					PacketReq.m_wstrRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
					PacketReq.m_cType = 0;
					
					SendToLogDB( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, PacketReq );
				}
			}
#endif SERV_MONITORING_LETTER_AND_TRADE
			//}}
		}
		else
		{
			KItemInfo kInsertItemInfo;
			if( !m_kUserLetterBox.GetItemInfo( kPacket_.m_iPostNo, kInsertItemInfo ) )
			{
				kPacketAck.m_iOK = NetError::ERR_POST_LETTER_04;
				SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
				return;
			}

			mapInsertItem.insert( std::make_pair( kInsertItemInfo.m_iItemID, kInsertItemInfo ) );
		}
	}

	// �κ��丮 �˻�
	if( m_kInventory.PrepareInsert( mapInsertItem, kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo, true ) == false )
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_05;
		SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacketAck );
		return;
	}

	// [����] ����������� ������ �����ϴ��� �ϴ� ������ DB�� ������.
	// ��ü�� ���̺��� �����۵��� �����ϱ� ���ؼ�.

	kPacketToDB.m_iUnitUID	    = GetCharUID();
	kPacketToDB.m_iPostNo	    = kPacket_.m_iPostNo;	
	SendToGameDB( DBE_GET_ITEM_FROM_LETTER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_ITEM_FROM_LETTER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_GET_ITEM_FROM_LETTER_ACK kPacket;
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_iPostNo		= kPacket_.m_iPostNo;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacket.m_cLetterType = kPacket_.m_cLetterType;
#else // SERV_RELATIONSHIP_SYSTEM
	kPacket.m_bSystemLetter = kPacket_.m_bSystemLetter;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	kPacket.m_mapResultItem = kPacket_.m_mapInsertedItem;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ED÷�ι� �ޱ�
		if( kPacket_.m_iED > 0 )
		{
			//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			IncreaseED( kPacket_.m_iED, KUserEDManager::ER_IN_ED_GET_ED_FROM_LETTER );
#else
			const int iBeforeED = m_iED;

			m_iED += kPacket_.m_iED;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
			//#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_RECV_ED, iBeforeED );
			//#endif SERV_USER_ABUSER_MANAGER
			//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			

			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( 0 );
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PPostLetter, kPacket_.m_iED );
		}

		// ������ ÷�ι� �ޱ�
		m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );

		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		switch( kPacket_.m_cLetterType )
		{
		case KPostItemInfo::LT_POST_OFFICE:
			{
				// ���� ������� ÷�ι� �ʱ�ȭ �Ѵ�..
				m_kUserLetterBox.SetReceivedFlag( kPacket_.m_iPostNo );
			}
			break;
		default:
			{
				// �ý��� ������� �����..
				m_kUserLetterBox.DeleteLetter( kPacket_.m_iPostNo );
			}
		}
#else // SERV_RELATIONSHIP_SYSTEM
		if( kPacket_.m_bSystemLetter )
		{
			// �ý��� ������� �����..
			m_kUserLetterBox.DeleteLetter( kPacket_.m_iPostNo );
		}
		else
		{
			// ���� ������� ÷�ι� �ʱ�ȭ �Ѵ�..
			m_kUserLetterBox.SetReceivedFlag( kPacket_.m_iPostNo );
		}
#endif SERV_RELATIONSHIP_SYSTEM
		//}

		//{{ 2013. 10. 18	�ڼ���	��ų����ü���� üũ(�κ��丮-�Ⱓ��) ��� ����
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
		for( std::map<int, int>::const_iterator it = kPacket_.m_mapInsertedItem.begin(); it != kPacket_.m_mapInsertedItem.end(); ++it )
		{
			const int iItemID = it->first;
			switch( iItemID )
			{
			case CXSLItem::CI_EXPAND_SKILL_SLOT_IN_PACKAGE:
				{
#ifdef	SERV_SKILL_SLOT_ITEM_BUG_FIX	// ���볯¥: 2013-07-04				
					// ������ ���� (2049-12-31 23:59) ���� �Ⱓ ���� �Ǿ� ����
					CTime tLimitEndDate = CTime( 2049, 12, 31, 23, 59, 0 );
					CTime tSkillSlotBEndDate;
					m_kSkillTree.GetSkillSolotBEndDate( tSkillSlotBEndDate );		
					if( tLimitEndDate == tSkillSlotBEndDate )
					{
						// ��ų ���� ������ �����߾��ٸ� ���� �� �ʿ� ����.
						break;
					}
#endif	// SERV_SKILL_SLOT_ITEM_BUG_FIX

					std::wstring wstrSkillSlotBEndDate;
					if( m_kInventory.GetSlotChangeBEndDate( iItemID, wstrSkillSlotBEndDate ) )
					{
						m_kSkillTree.SetSkillSolotBEndDate( wstrSkillSlotBEndDate );

						KEGS_SKILL_SLOT_CHANGE_ITEM_NOT kpacket;

						kpacket.m_cSkillSlotBExpirationState = KUserSkillTree::SSBES_NOT_EXPIRED;
						kpacket.m_wstrSkillSlotBEndDate = wstrSkillSlotBEndDate;

						SendPacket( EGS_SKILL_SLOT_CHANGE_ITEM_NOT, kpacket );
						break;
					}
				}
				break;
			}
		}
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
		//}}

		kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	}

	kPacket.m_iED = GetED();

	SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ���� �����ۿ� ���� ���
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( kPacket_.m_cLetterType != KPostItemInfo::LT_POST_OFFICE )
#else
	if( kPacket_.m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	{
		KStatisticsKey kKey;
		std::map< int, int >::const_iterator mitStat;
		for( mitStat = kPacket_.m_mapInsertedItem.begin(); mitStat != kPacket_.m_mapInsertedItem.end(); ++mitStat )
		{
			kKey.m_vecIntKey.clear();
			kKey.m_vecIntKey.push_back( mitStat->first );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, mitStat->second );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, mitStat->second );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_LETTER, kPacket_.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

IMPL_ON_FUNC( DBE_INSERT_LETTER_TO_POST_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEND_LETTER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_SEND_LETTER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ED �ѹ�
		if( kPacket_.m_kSendLetter.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
		{
			//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			IncreaseED( kPacket_.m_kSendLetter.m_iQuantity, KUserEDManager::ER_IN_ED_SEND_LETTER_ATTACH_ROLLBACK );
#else
			const int iBeforeED = m_iED;

			m_iED += kPacket_.m_kSendLetter.m_iQuantity;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
			//#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_ED_ROLLBACK, iBeforeED );
			//#endif SERV_USER_ABUSER_MANAGER
			//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			
		}

		//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		IncreaseED( kPacket_.m_iSendLetterCost, KUserEDManager::ER_IN_ED_SEND_LETTER_COMMISSION_ROLLBACK );
#else
		const int iBeforeED2 = m_iED;

		m_iED += kPacket_.m_iSendLetterCost;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST_ROLLBACK, iBeforeED2 );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		
	}

	// ���� ���� sp�� ���� ���� ��쿡�� �ϴ� �������� ���� ó���ϵ��� �Ѵ�. ( ���� �̽� �߻� )
	// RollBack �Լ��� ȣ���Ѵٰ� �ص� �ٷ� ���������� ������,
	// ���� �ϳ� �������� ���� ���¿��� ���� �ٿ��� �߻��ϴ� ���� �Ұ��׷����� �����.

	kPacket.m_iED = GetED();
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	SendPacket( EGS_SEND_LETTER_ACK, kPacket );

	// �޴� ������ ������ �ִٸ� �ǽð����� ���� �����ֱ�
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KELG_FIND_USER_SEND_LETTER_NOT kPacketNot;		
		kPacketNot.m_kLetter = kPacket_.m_kSendLetter;
		SendToLoginServer( ELG_FIND_USER_SEND_LETTER_NOT, kPacketNot );

		//////////////////////////////////////////////////////////////////////////
		// ED���
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );

		// ÷�� ED
		if( kPacket_.m_kSendLetter.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
		{
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MPostLetter, kPacket_.m_kSendLetter.m_iQuantity );
#ifdef SERV_USER_STATISTICS_RENEWAL
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MPostLetter, kPacket_.m_kSendLetter.m_iQuantity );
#endif //SERV_USER_STATISTICS_RENEWAL
		}

		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MSendLetterBasic, 200 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MSendLetterItem, ( kPacket_.m_iSendLetterCost - 200 ) );
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MSendLetterBasic, 200 );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MSendLetterItem, ( kPacket_.m_iSendLetterCost - 200 ) );
#endif //SERV_USER_STATISTICS_RENEWAL
		//////////////////////////////////////////////////////////////////////////
	}
}

IMPL_ON_FUNC( ELG_FIND_USER_SEND_LETTER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �����Կ� ���� �ֱ�
	m_kUserLetterBox.AddLetter( kPacket_.m_kLetter );

	KEGS_RECEIVE_LETTER_NOT kPacket;
	kPacket.m_wstrNickName	  = kPacket_.m_kLetter.m_wstrFromNickName;
	kPacket.m_wstrLetterTitle = kPacket_.m_kLetter.m_wstrTitle;
	kPacket.m_cScriptType	  = kPacket_.m_kLetter.m_cScriptType;
	kPacket.m_iScriptIndex	  = kPacket_.m_kLetter.m_iScriptIndex;
	//{{ 2011. 09. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	kPacket.m_iFromUnitUID	  = kPacket_.m_kLetter.m_iFromUnitUID;
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	SendPacket( EGS_RECEIVE_LETTER_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_READ_LETTER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_READ_LETTER_ACK kPacketAck;
	bool bReadUpdated = false;

	if( !m_kUserLetterBox.ReadLetter( kPacket_.m_iPostNo, kPacketAck.m_kLetter, bReadUpdated ) )
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_04;
		SendPacket( EGS_READ_LETTER_ACK, kPacketAck );
		return;
	}

	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( EGS_READ_LETTER_ACK, kPacketAck );

	// DB Update
	if( !bReadUpdated )
	{
		KDBE_READ_LETTER_NOT kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_iPostNo  = kPacket_.m_iPostNo;
		SendToGameDB( DBE_READ_LETTER_NOT, kPacketToDB );
	}
}

IMPL_ON_FUNC( EGS_DELETE_LETTER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_DELETE_LETTER_ACK kPacketAck;

	if( !m_kUserLetterBox.IsExist( kPacket_.m_vecPostNo ) )
	{
		kPacketAck.m_iOK = NetError::ERR_POST_LETTER_14;
		SendPacket( EGS_DELETE_LETTER_ACK, kPacketAck );
		return;
	}

	// ���� ����
	m_kUserLetterBox.DeleteLetter( kPacket_.m_vecPostNo );

	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_vecPostNo = kPacket_.m_vecPostNo;
	SendPacket( EGS_DELETE_LETTER_ACK, kPacketAck );

	// DB Update
	KDBE_DELETE_LETTER_NOT kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_vecPostNo = kPacket_.m_vecPostNo;
	SendToGameDB( DBE_DELETE_LETTER_NOT, kPacketToDB );
}
//}}

//{{ 2008. 9. 26  ������	��ü�� ������Ʈ
IMPL_ON_FUNC_NOPARAM( EGS_GET_POST_BLACK_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_POST_BLACK_LIST_ACK kPacket;
	m_kUserLetterBox.GetBlackList( kPacket.m_vecBlackList );
	SendPacket( EGS_GET_POST_BLACK_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_NEW_POST_BLACK_LIST_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_NEW_POST_BLACK_LIST_REQ, EGS_NEW_POST_BLACK_LIST_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// SQL Injection ����
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
	{
		KEGS_NEW_POST_BLACK_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_NEW_POST_BLACK_LIST_ACK, kPacket );
		return;
	}

	if( m_kUserLetterBox.IsBlackList( kPacket_.m_wstrNickName ) )
	{
		KEGS_NEW_POST_BLACK_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_01;
		SendPacket( EGS_NEW_POST_BLACK_LIST_ACK, kPacket );
		return;
	}

	KDBE_NEW_POST_BLACK_LIST_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_wstrNickName = kPacket_.m_wstrNickName;
	SendToGameDB( DBE_NEW_POST_BLACK_LIST_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_NEW_POST_BLACK_LIST_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_NEW_POST_BLACK_LIST_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kUserLetterBox.AddBlackList( kPacket_.m_wstrNickName );
	}

	KEGS_NEW_POST_BLACK_LIST_ACK kPacket;
	kPacket.m_iOK		   = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendPacket( EGS_NEW_POST_BLACK_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_DEL_POST_BLACK_LIST_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DEL_POST_BLACK_LIST_REQ, EGS_DEL_POST_BLACK_LIST_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// SQL Injection ����
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
	{
		KEGS_DEL_POST_BLACK_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_DEL_POST_BLACK_LIST_ACK, kPacket );
		return;
	}

	if( !m_kUserLetterBox.IsBlackList( kPacket_.m_wstrNickName ) )
	{
		KEGS_NEW_POST_BLACK_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_03;
		SendPacket( EGS_NEW_POST_BLACK_LIST_ACK, kPacket );
		return;
	}

	KDBE_DEL_POST_BLACK_LIST_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_wstrNickName = kPacket_.m_wstrNickName;
	SendToGameDB( DBE_DEL_POST_BLACK_LIST_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_DEL_POST_BLACK_LIST_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DEL_POST_BLACK_LIST_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kUserLetterBox.DelBlackList( kPacket_.m_wstrNickName );
	}

	KEGS_DEL_POST_BLACK_LIST_ACK kPacket;
	kPacket.m_iOK		   = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendPacket( EGS_DEL_POST_BLACK_LIST_ACK, kPacket );
}
//}}

IMPL_ON_FUNC_NOPARAM( EGS_RENEWAL_LETTER_REQ )
{
	KEGS_RENEWAL_LETTER_ACK kPacket;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	m_kUserLetterBox.GetRenewalLetter( GetThisPtr<KGSUser>(), kPacket.m_iNewUserLetterCount, kPacket.m_iNewSystemLetterCount, kPacket.m_iNewRelationshipLetterCount, kPacket.m_vecNewLetterTitle );
#else
	m_kUserLetterBox.GetRenewalLetter( GetThisPtr<KGSUser>(), kPacket.m_iNewUserLetterCount, kPacket.m_iNewSystemLetterCount, kPacket.m_vecNewLetterTitle );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacket.m_iTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( KEGS_GET_POST_LETTER_LIST_REQ::LLT_NORMAL );
#else
	kPacket.m_iTotalLetterCount = m_kUserLetterBox.GetTotalLetterCount( false );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	SendPacket(EGS_RENEWAL_LETTER_ACK, kPacket);
}

//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
IMPL_ON_FUNC( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_ACK )
{
	switch( kPacket_.m_iOK )
	{
	case NetError::ERR_EXIST_LETTER_MONITORING:
		{
			m_kUserHackingManager.ClearLetterMonitoringList();

			START_LOG( cerr, L"���� ���� �αװ� �ߺ� �Ǵ� ���� �ִ�. :")
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}break;
	case NetError::ERR_EXIST_TRADE_MONITORING:
		{
			m_kUserHackingManager.ClearTradeMonitoringList();

			START_LOG( cerr, L"�ŷ� ���� �αװ� �ߺ� �Ǵ� ���� �ִ�. :")
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}break;
	case NetError::ERR_INSERT_FAILD_LETTER_MONITORING:
		{
			m_kUserHackingManager.ClearLetterMonitoringList();

			START_LOG( cerr, L"���� ���� �αװ� �߰� ����! :")
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}break;
	case NetError::ERR_INSERT_FAILD_TRADE_MONITORING:
		{
			m_kUserHackingManager.ClearTradeMonitoringList();

			START_LOG( cerr, L"�ŷ� ���� �αװ� �߰� ����! :")
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}break;
	case 0:
		{
			m_kUserHackingManager.ClearLetterMonitoringList();
		}break;
	case 1:
		{
			m_kUserHackingManager.ClearTradeMonitoringList();
		}break;
	default:
		{
			START_LOG( cerr, L"���� ���� �̻��մϴ�. ����ó��")
				<< BUILD_LOG( kPacket_.m_iOK )
				<< END_LOG;
			return;
		}
	}

	//{{ 2011. 04. 13  ���� �� �ŷ� ���� �ǽð� SMS ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE_SMS
	if( GetKGameServer()->GetMonitoringLetterAndTradeSMS() == true )
	{
		KDBE_SEND_PHONE_MSG_NOT kPacketNot;

		//{{ 2012. 10. 9	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_SURVEILLANCE_THE_LETTER_AND_TRADE, kPacketNot.m_vecPhoneNum );
#else
		//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
		SiKGameSysVal()->GetRecipientSMS( KGameSysVal::SMS_SURVEILLANCE_THE_LETTER_AND_TRADE, kPacketNot.m_vecPhoneNum );
#else
		kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4118-3867" ) ); // ��âȣ �����
		kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-9317-0790" ) ); // Ȳ���� ��
		
		kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4072-1031" ) ); // ���
		kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-3898-7886" ) ); // �谭�� 
		kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-5671-0502" ) ); // �̿���
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
		//}}
#endif SERV_SMS_TOTAL_MANAGER
		//}}


		//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
		CTime tCurr = CTime::GetCurrentTime();
		CTimeSpan tSpan = CTimeSpan( 0, tCurr.GetHour(), tCurr.GetMinute(), tCurr.GetSecond() );

		if( SiKGameSysVal()->IsMonitoringLetterAndTradeBlockTime( tSpan ) == true )
		{
			kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"[��]����&�ŷ� ����! : UserUID(%d), UnitUID(%d), NickName(%s)" ) % GetUID() % GetCharUID() % GetCharName() );
			SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

			HackUserRegRejectedUser( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_MONITORING_LETTER_AND_TRADE );
			SetRandomTimeKick( KStatistics::eSIColDR_Client_Hacking, 5, 5 );

			return;
		}
#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
		//}}
		kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"����&�ŷ� ����! : UserUID(%d), UnitUID(%d), NickName(%s)" ) % GetUID() % GetCharUID() % GetCharName() );
		SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
	}
	else
	{
		START_LOG( cout, L"�系 ���������� ������ �ʴ´�!! :")
			<< END_LOG;
	}
#endif SERV_MONITORING_LETTER_AND_TRADE_SMS
	//}} 2011. 04. 13 ���� �� �ŷ� ���� �ǽð� SMS ����


	START_LOG( cout, L"�ŷ� ���� �αװ� �߰� ����! :")
		<< BUILD_LOG( GetCharUID() )
		<< END_LOG;
}
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ ���� ED �ŷ��� ED ����ȭ - ��μ�
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC
IMPL_ON_FUNC( DBE_SYNC_ED_ACK )
{
	KEGS_SEND_LETTER_ACK kAck;
	KDBE_PREPARE_INSERT_LETTER_TO_POST_REQ kPacketToDB;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 10. 11	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		m_kEDManager.Init( GetED() - kPacket_.m_kInfo.m_iChangeED + kPacket_.m_kInfo.m_iED + kPacket_.m_kInfo.m_iSendLetterCost, KUserEDManager::ER_SET_ED_SYNC_FOR_SEND_LEETER_ROLLBACK );

		IncreaseED( kPacket_.m_kInfo.m_iChangeED, KUserEDManager::ER_IN_ED_SYNC_FOR_SEND_LEETER_ROLLBACK );
#else
		m_iED.SetValue( m_iED - kPacket_.m_kInfo.m_iChangeED + kPacket_.m_kInfo.m_iED + kPacket_.m_kInfo.m_iSendLetterCost );

		m_iED += kPacket_.m_kInfo.m_iChangeED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		

		kAck.m_iOK = NetError::ERR_POST_LETTER_02;
		goto end_proc;
	}

	// 6. DB�� ������Ʈ üũ
	kPacketToDB.m_iFromUnitUID				= GetCharUID();
	kPacketToDB.m_iItemUID					= kPacket_.m_kInfo.m_iItemUID;
	kPacketToDB.m_wstrToNickName			= kPacket_.m_kInfo.m_wstrToNickName;
	kPacketToDB.m_iSendLetterCost = kPacket_.m_kInfo.m_iSendLetterCost;
	kPacketToDB.m_kSendLetter.m_cScriptType = ( kPacket_.m_kInfo.m_bIsGmMessage ? KPostItemInfo::LT_MESSAGE : KPostItemInfo::LT_POST_OFFICE );
	kPacketToDB.m_kSendLetter.m_wstrTitle	= kPacket_.m_kInfo.m_wstrTitle;
	kPacketToDB.m_kSendLetter.m_wstrMessage = kPacket_.m_kInfo.m_wstrMessage;
	kPacketToDB.m_kSendLetter.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_kSendLetter.m_wstrFromNickName = GetCharName();
	kPacketToDB.m_kSendLetter.m_iScriptIndex = CXSLItem::EDI_BRONZE_ED; // ����� ED
	kPacketToDB.m_kSendLetter.m_iQuantity	 = kPacket_.m_kInfo.m_iED;
	SendToGameDB( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ, kPacketToDB );

	return;

end_proc:

	SendPacket( EGS_SEND_LETTER_ACK, kAck );
}
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
//}}

//{{ 2012. 08. 21	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
IMPL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_ARRANGE_ACK )
{
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	// Ŭ���̾�Ʈ���� �� �ʿ� ���� ������ �����. ( ��Ŷ�� ���� ���� ó���ص� ������ )
	kPacket_.m_iUnitUID = 0;
	kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.clear();
	kPacket_.m_kItemQuantityUpdate.m_vecDeleted.clear();
	kPacket_.m_vecMoveItemSlotInfo.clear();
	SendPacket( EGS_GET_ITEM_FROM_LETTER_ACK, kPacket_ );
}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// �۾���¥: 2013-05-12	// �ڼ���
IMPL_ON_FUNC( EGS_SEND_LOVE_LETTER_EVENT_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_SEND_LOVE_LETTER_EVENT_REQ, EGS_SEND_LOVE_LETTER_EVENT_ACK );

	std::map<int, int> mapRequireItem;
	mapRequireItem.insert( std::map<int, int>::value_type( CXSLItem::CI_REQUEST_OF_LOVE, 1 ) );

	// ����� ������Ʈ �������� �����ϰ� �ִ°�?
	if( IsEnoughItemExistOnlyInventory( mapRequireItem ) == false )
	{
		KEGS_SEND_LOVE_LETTER_EVENT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_REQUEST_OF_LOVE_02;
		SendPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
		return;
	}

	// 1�� ED�� �����ϰ� �ִ°�?
	const int iBeforeED = GetED();

	if( iBeforeED < 10000 )
	{
		KEGS_SEND_LOVE_LETTER_EVENT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_REQUEST_OF_LOVE_03;
		SendPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
		return;
	}

	// ���� ���Ѽ��� ���� �ʴ°�?
	if( 700 < kPacket_.m_wstrMessage.size() )
	{
		KEGS_SEND_LOVE_LETTER_EVENT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_POST_LETTER_19;
		SendPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
		return;
	}

	// DB�� ������ ���뿡 �̻��� ���°�?
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		KEGS_SEND_LOVE_LETTER_EVENT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
		return;
	}

	// ED ����
	DecreaseED( 10000 );
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST, iBeforeED );


	KDBE_SEND_LOVE_LETTER_EVENT_REQ kPacketToDB;

	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	if( m_kInventory.DeleteAndInsert( mapRequireItem, mapDummy, kPacketToDB.m_vecUpdatedInventorySlot, vecNewItem, true, KDeletedItemInfo::DR_USE_ITEM_IN_INVENTORY ) == false )
	{
		KEGS_SEND_LOVE_LETTER_EVENT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_REQUEST_OF_LOVE_04;
		SendPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
		return;
	}

	// DB�� �����Ϸ� ���ô�.
	kPacketToDB.m_iUnitUID		= GetCharUID();
	kPacketToDB.m_wstrMessage	= kPacket_.m_wstrMessage;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_SEND_LOVE_LETTER_EVENT_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_SEND_LOVE_LETTER_EVENT_ACK )
{
	KEGS_SEND_LOVE_LETTER_EVENT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	switch( kPacket_.m_iOK )
	{
	case NetError::ERR_REQUEST_OF_LOVE_01:
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 10535;	// ���޴��� ����Ʈ �̴Ϲ�
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}

	case NetError::NET_OK:
		{
			kPacket.m_iED						= GetED();
			kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

			// ������ ���
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( CXSLItem::CI_REQUEST_OF_LOVE );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
		break;

	default:
		{
			// ������ �ѹ� üũ
			m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

			// ED �ѹ� üũ
			const int iBeforeED = GetED();
			m_iED += 10000;

			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
			//#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_LETTER_COST_ROLLBACK, iBeforeED );
			//#endif SERV_USER_ABUSER_MANAGER
			//}}
		}
		break;
	}

	SendPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
}
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////
