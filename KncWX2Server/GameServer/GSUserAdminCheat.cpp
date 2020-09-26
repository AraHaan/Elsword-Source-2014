#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "GSSimLayer.h"

#include "ExpTable.h"
#include "SpiritTable.h"
#include "StatTable.h"
#include "BaseServer.h"
#include "X2Data/XSLTitleManager.h"
#include "ChannelManager.h"
#include "RoomListManager.h"

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_EVENT_CHECK_POWER
	#include "GameSysval/GameSysVal.h"
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-09	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#include "X2Data/XSLSquareUnit.h"
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#include "odbc/Odbc.h"
#include "Enum/Enum.h"

#define CLASS_TYPE      KGSUser
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_LOCLOG_REQ )
{
	if ( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		return;

	// local statistics flush�� ������� ����
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	KSIManager.QueueingEventID( EGS_SEND_STATISTICS_LOCLOG_REQ );
#else
#pragma ELSE_MESSAGE
	//KSIManager.OnFlushLocalLog( true );
#endif SERV_STATISTICS_THREAD
	//}}

	SendToCnUser( ECN_SEND_STATISTICS_LOCLOG_REQ );

	// ġƮ �α�
	WriteCheatLogToDB( L"/stloc" );
}

IMPL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_DBLOG_REQ )
{
	if ( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		return;

	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD	
	KSIManager.QueueingEventID( EGS_SEND_STATISTICS_DBLOG_REQ );
#else
#pragma ELSE_MESSAGE
	//KSIManager.FlushData();
#endif SERV_STATISTICS_THREAD
	//}}

	SendToCnUser( ECN_SEND_STATISTICS_DBLOG_REQ );

	// ġƮ �α�
	WriteCheatLogToDB( L"/stdb" );
}

_IMPL_ON_FUNC( EGS_ADMIN_QUEST_COMPLETE_REQ, KEGS_QUEST_COMPLETE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() == SEnum::UAL_DEVELOPER )
	{
		//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		if( SiCXSLQuestManager()->IsAccountQuest( kPacket_.m_iQuestID ) == true )
		{
			m_kUserAccountQuestManager.Handler_EGS_QUEST_COMPLETE_REQ_Account( kPacket_, GetThisPtr<KGSUser>(), true );
		}
		else
		{
			m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kPacket_, GetThisPtr<KGSUser>(), true );
		}
#else
		m_kUserQuestManager.Handler_EGS_QUEST_COMPLETE_REQ( kPacket_, GetThisPtr<KGSUser>(), true );
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		

		START_LOG( cout, L"��ڵ� �ڵ� ����Ʈ Ŭ����" )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iQuestID );

		// ġƮ �α�		
		CStringW cwstrCheatContents;
		cwstrCheatContents.Format( L"%d", kPacket_.m_iQuestID );
		WriteCheatLogToDB( L"/q", cwstrCheatContents );
	}
	else
	{
		START_LOG( cerr, L"��ڵ� �ƴѵ� ���� +��+/" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_SERVER_STATUS_REQ )
{
	KEGS_ADMIN_SERVER_STATUS_ACK kPacket;
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_SERVER_STATUS_ACK, kPacket );

		return;
	}

	SendToLogDB( DBE_ADMIN_SERVER_STATUS_REQ );
}

_IMPL_ON_FUNC( DBE_ADMIN_SERVER_STATUS_ACK, KEGS_ADMIN_SERVER_STATUS_ACK )
{
	// ����Ÿ�Ժ� �氹��
	SiKRoomListManager()->GetNumPvPRoom( kPacket_.m_mapNumPvPRoom );

	// ��Ƽ ���� & �������� ��Ƽ��[�������]
	SiKChannelManager()->GetTotalPartyInfo( kPacket_.m_iCurPartyCount, kPacket_.m_iPlayGamePartyCount );

	SendPacket( EGS_ADMIN_SERVER_STATUS_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_NOTIFY_MSG_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	//{{ 2010. 05. 10  ������	�������� ������ ����
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_ALL_SERVERS  ||
		kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_REQ::NT_GM_NOTIFY_SERVER_GROUP )
	{
		GetKGSSimLayer()->SendNotMSG( kPacket_.m_cNotifyType, kPacket_.m_wstrMSG, kPacket_.m_Count );
	}
	else
	{
		START_LOG( cerr, L"�߸��� GM���� Ÿ���Դϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( kPacket_.m_cNotifyType )
			<< END_LOG;
		return;
	}
	//}}

	// ġƮ �α�	
	WriteCheatLogToDB( L"/n", kPacket_.m_wstrMSG );
}

IMPL_ON_FUNC( EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

#ifdef SERV_ADMIN_CHEAT_NICKNAME_CHECK

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrUnitNickName,GetCharName()) == false )
#else
	if( kPacket_.m_wstrUnitNickName.compare(GetCharName()) != 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		START_LOG( cerr, L"ġƮ�� �ٸ� ĳ������ ������ ������ �� ����!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}
#endif SERV_ADMIN_CHEAT_NICKNAME_CHECK

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUnitNickName ) )
	{
		KEGS_ADMIN_MODIFY_UNIT_LEVEL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK, kPacketAck );
		return;
	}

	// EXP���� 0�̶�� EXP���̺��� �����´�.
	if( kPacket_.m_iEXP == 0 )
	{
		kPacket_.m_iEXP = SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_iLevel );
	}
	// EXP���� 0�� �ƴ϶�� ����ġ �ۼ�Ƽ����! ����ؼ� DB�� ������!
	else
	{		
		int iTotalExp = SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_iLevel );
		int iNextLevelNeedExp = SiKExpTable()->GetRequireNeedExpbyLevel( kPacket_.m_iLevel + 1 );

		float fIncreaseExp = static_cast<float>(iNextLevelNeedExp) * ( static_cast<float>(kPacket_.m_iEXP) / 100.f );

		kPacket_.m_iEXP = iTotalExp + static_cast<int>(fIncreaseExp);
	}

	SendToGameDB( DBE_ADMIN_MODIFY_UNIT_LEVEL_REQ, kPacket_ );

	// ġƮ �α�	
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s %d %d", kPacket_.m_wstrUnitNickName.c_str(), kPacket_.m_iLevel, kPacket_.m_iEXP );
	WriteCheatLogToDB( L"/l", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_MODIFY_UNIT_LEVEL_ACK, KEGS_ADMIN_MODIFY_UNIT_LEVEL_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetCharUID() == kPacket_.m_kUnitInfo.m_nUnitUID )
		{
			KStatTable::KUnitStatInfo sUnitStat;
			SiKStatTable()->GetUnitStat( GetUnitClass() ,kPacket_.m_kUnitInfo.m_ucLevel, sUnitStat );
			m_ucLevel		= kPacket_.m_kUnitInfo.m_ucLevel;

			// ����ġ
			if( kPacket_.m_kUnitInfo.m_iEXP == 0 )
			{
				m_kEXP.SetExp( SiKExpTable()->GetRequireTotalExpbyLevel( kPacket_.m_kUnitInfo.m_ucLevel ) );
			}
			else
			{
				m_kEXP.SetExp( kPacket_.m_kUnitInfo.m_iEXP );
			}			

			//m_iED.SetValue( 0 );
			//m_iVSPoint.SetValue( 0 );
			//m_iVSPointMax.SetValue( 0 );
			//m_iWin = 0; 
			//m_iLose = 0;
			//m_iSeceder = 0;

			m_kStat.m_iBaseHP		= sUnitStat.m_uiHP;
			m_kStat.m_iAtkPhysic	= sUnitStat.m_usAtkPhysic;
			m_kStat.m_iAtkMagic		= sUnitStat.m_usAtkMagic;
			m_kStat.m_iDefPhysic	= sUnitStat.m_usDefPhysic;
			m_kStat.m_iDefMagic		= sUnitStat.m_usDefMagic;
		}
	}

	GetUnitInfo( kPacket_.m_kUnitInfo );

	SendPacket( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_AUTH_LEVEL_REQ )
{
	
	// config_gs_internal.lua ����  AF_INTERNAL �� �Ǿ� ���� ����
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )	// �ؿ��� ����
	//if( KSimLayer::GetKObj()->GetBillingFlag() == KSimLayer::BF_NEXON_KOREA_TEST )
	{
		if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		{
			START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
				<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_12;
			SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket );
			return;
		}

		//{{ 2010. 12. 22	������	SQL�����ǹ���
		if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUserID ) )
		{
			START_LOG( cout, L"�����ڴ�! ID�� �� �� �Է��ϼ���!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_wstrUserID );

			KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket );
			return;
		}
		//}}

		SendToAccountDB( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, kPacket_ );

		// ġƮ �α�
		CStringW cwstrCheatContents;
		cwstrCheatContents.Format( L"%s %d", kPacket_.m_wstrUserID.c_str(), (int)kPacket_.m_cAuthLevel );
		WriteCheatLogToDB( L"/a", cwstrCheatContents );
	}
	else
	{
		START_LOG( cerr, L"�������� ��� �Ұ�!" )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_AUTH_LEVEL_ACK, KEGS_ADMIN_CHANGE_AUTH_LEVEL_ACK )
{
	SendPacket( EGS_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK, kPacket );

		return;
	}	

	SendToAccountDB( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, kPacket_ );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", (int)kPacket_.m_cAuthLevel );
	WriteCheatLogToDB( L"/al", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_AUTH_LEVEL_LIST_ACK, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK )
{
	SendPacket( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_ED_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

#ifdef SERV_ADMIN_CHEAT_NICKNAME_CHECK

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_wstrUnitNickName,GetCharName()) == false )
#else
	if( kPacket_.m_wstrUnitNickName.compare(GetCharName()) != 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		START_LOG( cerr, L"ġƮ�� �ٸ� ĳ������ ED�� ������ �� ����!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}
#endif SERV_ADMIN_CHEAT_NICKNAME_CHECK

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUnitNickName ) )
	{
		KEGS_ADMIN_CHANGE_ED_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_ADMIN_CHANGE_ED_ACK, kPacketAck );
		return;
	}

	SendToGameDB( DBE_ADMIN_CHANGE_ED_REQ, kPacket_ );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s %d", kPacket_.m_wstrUnitNickName.c_str(), kPacket_.m_nED );
	WriteCheatLogToDB( L"/e", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_ED_ACK, KEGS_ADMIN_CHANGE_ED_ACK )
{
	SendPacket( EGS_ADMIN_CHANGE_ED_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_KICK_USER_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	SendToLoginServer( ELG_ADMIN_KICK_USER_REQ, kPacket_ );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s", kPacket_.m_wstrUnitNickName.c_str() );
	if( kPacket_.m_bIsUserID )
	{
		WriteCheatLogToDB( L"/kid", cwstrCheatContents );
	}
	else
	{
		WriteCheatLogToDB( L"/k", cwstrCheatContents );
	}	
}

_IMPL_ON_FUNC( ELG_ADMIN_KICK_USER_ACK, KEGS_ADMIN_KICK_USER_ACK )
{
	SendPacket( EGS_ADMIN_KICK_USER_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_SKILL_POINT_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		SET_ERROR( ERR_SKILL_PAGE_01 );
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_ADMIN_CHANGE_SKILL_POINT_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	if( kPacket_.m_iSPoint >= 0 )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // ����ö // 2013-11-22 ���� ������� ��ų ���������� ����
		std::vector<KUserSkillTree::SkillPageData>& vecSkillPageData
			= m_kSkillTree.AccessSkillPageDataVector();

		for ( UINT i = 0; i < vecSkillPageData.size(); i++ )
		{
			if ( i == kPacket_.m_iActiveSkillPageNumber )
			{
				int iSPointChange = kPacket_.m_iSPoint - vecSkillPageData[i].m_iSPoint.GetInitValue();
				vecSkillPageData[i].m_iSPoint.SetValue( vecSkillPageData[i].m_iSPoint.GetInitValue() );
				vecSkillPageData[i].m_iSPoint += iSPointChange;
				break;
			}
			
		}
#else // SERV_SKILL_PAGE_SYSTEM
		int iSPointChange = kPacket_.m_iSPoint - m_iSPoint.GetInitValue();
		m_iSPoint.SetValue( m_iSPoint.GetInitValue() );
		m_iSPoint += iSPointChange;
#endif // SERV_SKILL_PAGE_SYSTEM


		KEGS_ADMIN_CHANGE_SKILL_POINT_ACK kPacket;
		kPacket.m_iOK		= NetError::NET_OK;
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		kPacket.m_iSPoint					= m_kSkillTree.GetSPoint();
		kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#else // SERV_SKILL_PAGE_SYSTEM
		kPacket.m_iSPoint	= m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
		
		SendPacket( EGS_ADMIN_CHANGE_SKILL_POINT_ACK, kPacket );

		// ġƮ �α�
		CStringW cwstrCheatContents;
		cwstrCheatContents.Format( L"%d", kPacket_.m_iSPoint );
		WriteCheatLogToDB( L"/sp", cwstrCheatContents );
	}
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_INIT_SKILL_TREE_REQ )
#else // SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_INIT_SKILL_TREE_REQ )
#endif // SERV_SKILL_PAGE_SYSTEM
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_ADMIN_INIT_SKILL_TREE_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	//{{ 2012. 03. 23	�ڼ���	�����ڿ� ġƮŰ ���� ����
	int iDefaultSkillID1 = 0;
	int iDefaultSkillID2 = 0;
	int iDefaultSkillID3 = 0;
	int iDefaultSkillID4 = 0;
	int iDefaultSkillID5 = 0;
	int iDefaultSkillID6 = 0;

	if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), iDefaultSkillID1, iDefaultSkillID2, iDefaultSkillID3, iDefaultSkillID4, iDefaultSkillID5, iDefaultSkillID6 ) )
	{
		START_LOG( cerr, L"����Ÿ���� �̻���." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}

	int iRetrievedSPoint = 0;
	int iRetrievedCSPoint = 0;
	if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == false )
	{
		START_LOG( cerr, L"������ ȯ��� sp ������ �̻��մϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( static_cast<int>(GetLevel()) )
			<< BUILD_LOG( iRetrievedSPoint )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}

	if( false == m_kSkillTree.IsCashSkillPointExpired() )
	{
		iRetrievedCSPoint = m_kSkillTree.GetMaxCSPoint();
	}
	else
	{
		if( 0 != m_kSkillTree.GetCSPoint() )
		{
			START_LOG( cerr, L"cash skill point������ �ٵǾ��µ� ��ųƮ���� csp�� �����ִ�." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iRetrievedSPoint )
				<< BUILD_LOG( iRetrievedCSPoint )
				<< BUILD_LOG( m_kSkillTree.GetCSPoint() )
				<< END_LOG;

			KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_SKILL_12;
			SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
			return;
		}
	}

	KDBE_INIT_SKILL_TREE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iSPoint = iRetrievedSPoint;
	kPacket.m_iCSPoint = iRetrievedCSPoint;
	kPacket.m_iDefaultSkillID1 = iDefaultSkillID1;
	kPacket.m_iDefaultSkillID2 = iDefaultSkillID2;
	kPacket.m_iDefaultSkillID3 = iDefaultSkillID3;
	kPacket.m_iDefaultSkillID4 = iDefaultSkillID4;
	kPacket.m_iDefaultSkillID5 = iDefaultSkillID5;
	kPacket.m_iDefaultSkillID6 = iDefaultSkillID6;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendToGameDB( DBE_ADMIN_INIT_SKILL_TREE_REQ, kPacket );

	// ġƮ �α�
	WriteCheatLogToDB( L"/isk" );
}

_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_ACK, KEGS_ADMIN_INIT_SKILL_TREE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{		
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSPoint( kPacket_.m_iSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
		m_iSPoint.SetValue( kPacket_.m_iSPoint );
		m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.ResetThisSkillPage( kPacket_.m_iActiveSkillPageNumber );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2012. 03. 23	�ڼ���	�����ڿ� ġƮŰ ���� ����
		// �⺻ ��ų �ֱ�
		int iDefaultSkillID[6] = {0,};
	
		if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), iDefaultSkillID[0], iDefaultSkillID[1], iDefaultSkillID[2], iDefaultSkillID[3], iDefaultSkillID[4], iDefaultSkillID[5] ) )
		{
			START_LOG( cerr, L"����Ÿ���� �̻���." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetUnitType() )
				<< END_LOG;
		}

		// ��ų �ʱ�ȭ
		for( int i = 0 ; i < 6 ; ++i )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			// kimhc // ����ö // ��ų �ʱ�ȭ ġƮ ��� ��, ������� ��ų �������� �ʱ�ȭ �ϵ��� ��
			m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( iDefaultSkillID[i], 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID[i], 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
	}
	else
	{
		START_LOG( cerr, L"��ų Ʈ�� ���� ġƮ ���� ����")
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket_ );
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_INIT_SKILL_TREE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}
	
	//{{ 2012. 03. 23	�ڼ���	�����ڿ� ġƮŰ ���� ����
#ifdef SERV_FIX_THE_ADMIN_CHEAT

	int iDefaultSkillID = 0;
	int iDefaultSkillID2 = 0;
	if( !SiCXSLSkillTree()->GetUnitTypeDefaultSkill( GetUnitType(), iDefaultSkillID, iDefaultSkillID2 ) )
	{
		START_LOG( cerr, L"����Ÿ���� �̻���." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
		return;
	}

	KDBE_INIT_SKILL_TREE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iSPoint = 1000;
	kPacket.m_iCSPoint = 0;
	kPacket.m_iDefaultSkillID = iDefaultSkillID;
	kPacket.m_iDefaultSkillID2 = iDefaultSkillID2;
	SendToGameDB( DBE_ADMIN_INIT_SKILL_TREE_REQ, kPacket );
#else
	SendToGameDB( DBE_ADMIN_INIT_SKILL_TREE_REQ, GetCharUID() );
#endif SERV_FIX_THE_ADMIN_CHEAT
	//}}

	// ġƮ �α�
	WriteCheatLogToDB( L"/isk" );
}

_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_ACK, KEGS_ADMIN_INIT_SKILL_TREE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kSkillTree.Reset( true, true, false, false, false );

		//{{ 2012. 03. 23	�ڼ���	�����ڿ� ġƮŰ ���� ����
#ifdef SERV_FIX_THE_ADMIN_CHEAT
		// �⺻ ��ų �ֱ�
		int iDefaultSkillID = 0;
		int iDefaultSkillID2 = 0;
		if( !SiCXSLSkillTree()->GetUnitTypeDefaultSkill( GetUnitType(), iDefaultSkillID, iDefaultSkillID2 ) )
		{
			START_LOG( cerr, L"����Ÿ���� �̻���." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetUnitType() )
				<< END_LOG;
		}
		m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID, 1, 0 );
		m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID2, 1, 0 );
#endif SERV_FIX_THE_ADMIN_CHEAT
		//}}
	}
	else
	{
		START_LOG( cerr, L"��ų Ʈ�� ���� ġƮ ���� ����")
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendPacket( EGS_ADMIN_INIT_SKILL_TREE_ACK, kPacket_ );
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

//{{ 2009. 7. 26  ������	party fever cheat
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_INCREASE_PARTY_FEVER_POINT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	if( GetPartyUID() > 0 )
	{
		//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
		KERM_INCREASE_PARTY_FEVER_NOT kNot;
		kNot.m_iPartyUID = GetPartyUID();

		if( SiKPartyListManager()->E_IncreasePartyFever( kNot ) == false )
#else
		if( SiKPartyManager()->E_IncreasePartyFever( GetPartyUID() ) == false )
#endif SERV_CHANNEL_PARTY
			//}}		
		{
			START_LOG( cerr, L"��Ƽ ��� ��ġ ���� ����!" )
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cwarn, L"��Ƽuid�� �̻��մϴ�!" )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}

	// ġƮ �α�
	WriteCheatLogToDB( L"/pf" );
}
//}}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_SPIRIT_REQ )
{
	KEGS_ADMIN_CHANGE_SPIRIT_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_SPIRIT_ACK, kPacket );
		return;
	}

	// �ټ��� �� �˻� [������ �Ѿ�ٸ� ���� ���� ������]
	if( kPacket_.m_iSpirit < 0  ||  kPacket_.m_iSpirit > SiKSpiritTable()->GetMaxSpirit() )
	{
		//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		kPacket.m_iChangedSpirit = m_kUserSpiritManager.GetSpirit();
#else
		kPacket.m_iChangedSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
	}
	else
	{
		//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_kUserSpiritManager.SetSpirit( kPacket_.m_iSpirit );
#else
		m_iSpirit				 = kPacket_.m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
		kPacket.m_iChangedSpirit = kPacket_.m_iSpirit;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ADMIN_CHANGE_SPIRIT_ACK, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iSpirit );
	WriteCheatLogToDB( L"/cs", cwstrCheatContents );
}

IMPL_ON_FUNC( EGS_ADMIN_SET_PC_BANG_REQ )
{
	KEGS_ADMIN_SET_PC_BANG_ACK kPacket;

	// �� �ۿ��� ���ӹ� ���¸� �ٲ�� �濡 ������ �� ����� ���δ�.
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_09;
		SendPacket( EGS_ADMIN_SET_PC_BANG_ACK, kPacket );
		return;
	}

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��� �̻� ����." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_SET_PC_BANG_ACK, kPacket );
		return;
	}

	//{{ 2010. 01. 29  ������	PC�� ���ȭ
	m_kUserPcBangManager.SetIsPcBang( kPacket_.m_bOn );
#ifdef SERV_PC_BANG_TYPE
	m_kUserPcBangManager.SetPcBangType( kPacket_.m_iPcBangType );
#endif //SERV_PC_BANG_TYPE
	//}}

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2010. 01. 29  ������	PC�� ���ȭ
	kPacket.m_bOn = IsPcBang();
#ifdef SERV_PC_BANG_TYPE
	kPacket.m_iPcBangType = GetPcBangType();
	kPacket.m_kPcBangReward = GetPcBangReward();
#endif //SERV_PC_BANG_TYPE
	//}}
	SendPacket( EGS_ADMIN_SET_PC_BANG_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_VSPOINT_REQ )
{
	KEGS_ADMIN_CHANGE_VSPOINT_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK, kPacket );
		return;
	}

	// VSPoint ġƮ ����
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	/* Rating������Ʈ
		- AddRating�� ��� ��ũ ��/�ٿ��� �߻��� ���,
		������ RankPoint���� �������ش�. ( AddRatingAndRankUpDownProcess ���ο���. )
		�׷��Ƿ� ���� AddRPoint�� ����Ǿ�� �ȵȴ�.
	*/
	if( m_kUserPvpManager.AddRatingAndRankUpDownProcess( kPacket_.m_iRating ) == false )
	{
		m_kUserPvpManager.AddRPoint( kPacket_.m_iRPoint );
	}
#else
	m_kUserPvpManager.AddRating( kPacket_.m_iRating );
	m_kUserPvpManager.AddRPoint( kPacket_.m_iRPoint );
#endif SERV_2012_PVP_SEASON2
	//}}
	m_kUserPvpManager.AddAPoint( kPacket_.m_iAPoint );
#else
	m_iVSPoint		+= kPacket_.m_iVSPoint;
	m_iVSPointMax	+= kPacket_.m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	kPacket.m_cRank				 = GetPvpRankForClient();
#else
	kPacket.m_iChangedRating	 = GetRating();
#endif SERV_2012_PVP_SEASON2
	//}}
	kPacket.m_iChangedRPoint	 = GetRPoint();
	kPacket.m_iChangedAPoint	 = GetAPoint();
	SendPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d %d", kPacket_.m_iRating, kPacket_.m_iRPoint, kPacket_.m_iAPoint );
	WriteCheatLogToDB( L"/vp", cwstrCheatContents );
#else
	kPacket.m_iChangedVSPoint	 = m_iVSPoint;
	kPacket.m_iChangedVSPointMax = m_iVSPointMax;
	SendPacket( EGS_ADMIN_CHANGE_VSPOINT_ACK, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iVSPoint, kPacket_.m_iVSPointMax );
	WriteCheatLogToDB( L"/vp", cwstrCheatContents );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}	

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", (int)kPacket_.m_cUnitClass );
	WriteCheatLogToDB( L"/uc", cwstrCheatContents );

#ifdef SERV_LIMIT_UNIT
	//�������� �����ܿ��� ĳ���� ���´�. 
	const CXSLUnit::UNIT_TYPE eLimitType = static_cast< CXSLUnit::UNIT_TYPE >( _CONST_LIMIT_UNIT_::iLimitType );	
	const CXSLUnit::UNIT_TYPE eUnitType = CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) );
	if( eLimitType < eUnitType )
	{
		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // ������ ĳ���� Ŭ������ �̻���.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}
#endif SERV_LIMIT_UNIT
 

#ifdef SERV_NO_ARA
	const CXSLUnit::UNIT_TYPE eUnitARAType = CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) );
	if( CXSLUnit::UT_ARA == eUnitARAType )
	{
		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // ������ ĳ���� Ŭ������ �̻���.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}
#endif // SERV_NO_ARA

	// ���� Ŭ���� �˻�
	if( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == false )
	{
		START_LOG( cout, L"�������� �ʴ� ���� Ŭ������ ġƮ�� ����Ͽ����ϴ�. ���ڴ� ����� �Է����ּ���." )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // ������ ĳ���� Ŭ������ �̻���.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	KDBE_ADMIN_CHANGE_UNIT_CLASS_REQ kPacketToDB;

	int iDefaultSkill[6] = {0,};
	if( SiCXSLSkillTree()->GetUnitClassDefaultSkill( kPacket_.m_cUnitClass, iDefaultSkill[0], iDefaultSkill[1], iDefaultSkill[2], iDefaultSkill[3], iDefaultSkill[4], iDefaultSkill[5] ) == false )
	{
		START_LOG( cerr, L"������ �����Ϸ��� �⺻ ��ų�� �̻���" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // ������ ĳ���� Ŭ������ �̻���.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == true )
	{
		kPacketToDB.m_iNewDefaultSkill1 = iDefaultSkill[0];
		kPacketToDB.m_iNewDefaultSkill2 = iDefaultSkill[1];
		kPacketToDB.m_iNewDefaultSkill3 = 0;
		kPacketToDB.m_iNewDefaultSkill4 = 0;
		kPacketToDB.m_iNewDefaultSkill5 = 0;
		kPacketToDB.m_iNewDefaultSkill6 = 0;
	}
	else if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == true )
	{
		kPacketToDB.m_iNewDefaultSkill1 = iDefaultSkill[0];
		kPacketToDB.m_iNewDefaultSkill2 = iDefaultSkill[1];
		kPacketToDB.m_iNewDefaultSkill3 = iDefaultSkill[2];
		kPacketToDB.m_iNewDefaultSkill4 = iDefaultSkill[3];
		kPacketToDB.m_iNewDefaultSkill5 = 0;
		kPacketToDB.m_iNewDefaultSkill6 = 0;
	}
	else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == true )
	{
		kPacketToDB.m_iNewDefaultSkill1 = iDefaultSkill[0];
		kPacketToDB.m_iNewDefaultSkill2 = iDefaultSkill[1];
		kPacketToDB.m_iNewDefaultSkill3 = iDefaultSkill[2];
		kPacketToDB.m_iNewDefaultSkill4 = iDefaultSkill[3];
		kPacketToDB.m_iNewDefaultSkill5 = iDefaultSkill[4];
		kPacketToDB.m_iNewDefaultSkill6 = iDefaultSkill[5];
	}

	kPacketToDB.m_iUnitUID	 = GetCharUID();
	kPacketToDB.m_cUnitClass = kPacket_.m_cUnitClass;
	SendToGameDB( DBE_ADMIN_CHANGE_UNIT_CLASS_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ADMIN_CHANGE_UNIT_CLASS_ACK )
{
	KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// Ŭ���� ����
		SetUnitClass( kPacket_.m_cUnitClass );
		m_kSkillTree.SetUnitClass( kPacket_.m_cUnitClass );

		// ��ų �ʱ�ȭ
		int iRetrievedSPoint = 0;
		if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == true )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSPointEveryPage( iRetrievedSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
			m_iSPoint.SetValue( iRetrievedSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
		
		if( m_kSkillTree.IsCashSkillPointExpired() == true )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPointEveryPage( m_kSkillTree.GetMaxCSPoint() );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPoint( m_kSkillTree.GetMaxCSPoint() );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
		else
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPointEveryPage( 0 );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetCSPoint( 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
		}

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.ResetEveryPage();
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2012. 03. 23	�ڼ���	�����ڿ� ġƮŰ ���� ����
		// �⺻ ��ų �ֱ�
		int iDefaultSkillID[6] = {0,};

		iDefaultSkillID[0] = kPacket_.m_iNewDefaultSkill1;
		iDefaultSkillID[1] = kPacket_.m_iNewDefaultSkill2;
		iDefaultSkillID[2] = kPacket_.m_iNewDefaultSkill3;
		iDefaultSkillID[3] = kPacket_.m_iNewDefaultSkill4;
		iDefaultSkillID[4] = kPacket_.m_iNewDefaultSkill5;
		iDefaultSkillID[5] = kPacket_.m_iNewDefaultSkill6;

		// ��ų �ʱ�ȭ
		for( int i = 0 ; i < 6 ; ++i )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			// kimhc // ����ö // ���� ���� ġƮ ��� ��, ��� ��ų �������� �ʱ�ȭ �ϵ��� ��
			m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnEveryPage( iDefaultSkillID[i], 1, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSkillLevelAndCSP( iDefaultSkillID[i], 1, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
		}

		kPacket.m_cUnitClass = kPacket_.m_cUnitClass;

		kPacket.m_iDefaultSkill1 = kPacket_.m_iNewDefaultSkill1;
		kPacket.m_iDefaultSkill2 = kPacket_.m_iNewDefaultSkill2;
		kPacket.m_iDefaultSkill3 = kPacket_.m_iNewDefaultSkill3;
		kPacket.m_iDefaultSkill4 = kPacket_.m_iNewDefaultSkill4;
		kPacket.m_iDefaultSkill5 = kPacket_.m_iNewDefaultSkill5;
		kPacket.m_iDefaultSkill6 = kPacket_.m_iNewDefaultSkill6;

		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	// m_iOK != NetError::NET_OK
	kPacket.m_cUnitClass = 0;
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iSPoint = m_kSkillTree.GetSPoint();
#else // SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iSPoint = m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	kPacket.m_iCSPoint = m_kSkillTree.GetCSPoint();
	kPacket.m_iDefaultSkill1 = 0;
	kPacket.m_iDefaultSkill2 = 0;
	kPacket.m_iDefaultSkill3 = 0;
	kPacket.m_iDefaultSkill4 = 0;
	kPacket.m_iDefaultSkill5 = 0;
	kPacket.m_iDefaultSkill6 = 0;
	SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
	return;
}

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ )
#else // SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ )
#endif // SERV_SKILL_PAGE_SYSTEM
{
	// ������ ���� �����ִ� ��ų �� ���
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}	

	// ġƮ �α�
	WriteCheatLogToDB( L"/as" );

	KDBE_ADMIN_AUTO_GET_ALL_SKILL_REQ kPacketToDB;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	if( !SiCXSLSkillTree()->GetUnitClassDefaultSkill( GetUnitClass(), 
		kPacketToDB.m_iNewDefaultSkill1, 
		kPacketToDB.m_iNewDefaultSkill2, 
		kPacketToDB.m_iNewDefaultSkill3, 
		kPacketToDB.m_iNewDefaultSkill4, 
		kPacketToDB.m_iNewDefaultSkill5, 
		kPacketToDB.m_iNewDefaultSkill6 ) )
	{
		START_LOG( cerr, L"����Ÿ���� �̻���." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetUnitType() )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	int iRetrievedSPoint = 0;
	int iRetrievedCSPoint = 0;
	if( SiCXSLSkillTree()->GetCalcInitSkillPoint( GetLevel(), iRetrievedSPoint ) == false )
	{
		START_LOG( cerr, L"������ ȯ��� sp ������ �̻��մϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( static_cast<int>(GetLevel()) )
			<< BUILD_LOG( iRetrievedSPoint )
			<< END_LOG;

		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_12;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	if( false == m_kSkillTree.IsCashSkillPointExpired() )
	{
		iRetrievedCSPoint = m_kSkillTree.GetMaxCSPoint();
	}
	else
	{
		if( 0 != m_kSkillTree.GetCSPoint() )
		{
			START_LOG( cerr, L"cash skill point������ �ٵǾ��µ� ��ųƮ���� csp�� �����ִ�." )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iRetrievedSPoint )
				<< BUILD_LOG( iRetrievedCSPoint )
				<< BUILD_LOG( m_kSkillTree.GetCSPoint() )
				<< END_LOG;

			KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_SKILL_12;
			SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
			return;
		}
	}

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_cUnitClass = GetUnitClass();
	kPacketToDB.m_iSPoint = iRetrievedSPoint;
	kPacketToDB.m_iCSPoint = iRetrievedCSPoint;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, CXSLSkillTree::SkillTreeTemplet > mapSkillTreeList;
	std::map< int, CXSLSkillTree::SkillTreeTemplet >::iterator mit;

	// ��� �� �ִ� ��ų ����� ã��
	mapSkillTreeList.clear();
	SiCXSLSkillTree()->GetUnitClassSkillList( GetUnitClass(), mapSkillTreeList );
	for( mit = mapSkillTreeList.begin() ; mit != mapSkillTreeList.end() ; ++mit )
	{
		// ��ȥ ��ų�� ����
		if( mit->first == CXSLSkillTree::SI_ETC_WS_COMMON_LOVE )
			continue;

		const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first );
		if( pSkillTemplet == NULL )
			continue;

		// ������ ���Ͽ� ��� �� �ִ� tier �� ã��
		int iTier = mit->second.m_iTier;
		int iGap = 0;
		if( iTier >= 5 )		// 1�� �⺻ ��ų�� ���� ��Ų��
			++iGap;
		
		if( iTier >= 10 )		// 2�� �⺻ ��ų�� ���� ��Ų��
			++iGap;

		if( (mit->second.m_iTier - iGap) * 5 > GetLevel() )
			continue;

		// ��� �� �ִ� ��ŭ�� ��ų ������ �ø���
		int iSkillLevel = mit->second.m_iMasterSkillLevel;
		for( ; 0 < iSkillLevel ; --iSkillLevel  )
		{
			if( pSkillTemplet->m_vecRequireCharactorLevel[ iSkillLevel-1 ] <= GetLevel() )
				break;
		}

		if( iSkillLevel > 0 )
			kPacketToDB.m_mapGetSkillList.insert( std::make_pair( mit->first, iSkillLevel ) );

		if( pSkillTemplet->m_bBornSealed == true )
		{
			if( m_kSkillTree.IsSkillUnsealed( mit->first ) == false )
				kPacketToDB.m_vecUnsealedSkillID.push_back( static_cast<short>(mit->first) );
		}
		
	}
	// sp �Ҹ�� ���� ������ �Ѵ�.
	
	
	SendToGameDB( DBE_ADMIN_AUTO_GET_ALL_SKILL_REQ, kPacketToDB );
	
}

IMPL_ON_FUNC( DBE_ADMIN_AUTO_GET_ALL_SKILL_ACK)
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	// ��ų �ʱ�ȭ	
#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.SetSPoint( kPacket_.m_iSPoint );
	m_kSkillTree.SetCSPoint( kPacket_.m_iActiveSkillPageNumber - 1, kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	m_iSPoint.SetValue( kPacket_.m_iSPoint );
	m_kSkillTree.SetCSPoint( kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.ResetEveryPage();
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.Reset( true, true, false, false, false );
#endif // SERV_SKILL_PAGE_SYSTEM

	// �⺻ ��ų �ֱ�
	int iDefaultSkillID[6] = {0,};

	iDefaultSkillID[0] = kPacket_.m_iNewDefaultSkill1;
	iDefaultSkillID[1] = kPacket_.m_iNewDefaultSkill2;
	iDefaultSkillID[2] = kPacket_.m_iNewDefaultSkill3;
	iDefaultSkillID[3] = kPacket_.m_iNewDefaultSkill4;
	iDefaultSkillID[4] = kPacket_.m_iNewDefaultSkill5;
	iDefaultSkillID[5] = kPacket_.m_iNewDefaultSkill6;

	// ��ų �ʱ�ȭ
	for( int i = 0 ; i < 6 ; ++i )
	{
		std::map<int, int>::iterator mit = kPacket_.m_mapGetSkillList.find( iDefaultSkillID[i] );
		if( mit == kPacket_.m_mapGetSkillList.end() )
		{
			kPacket_.m_mapGetSkillList.insert( std::make_pair( iDefaultSkillID[i], 1 ) );
		}		
	}

	std::map<int, int>::iterator mit = kPacket_.m_mapGetSkillList.begin();
	for( ; mit != kPacket_.m_mapGetSkillList.end() ; ++mit )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // ����ö // �ڽ��� ������ �´� ��� ��ų ���� ġƮ ��� ��, ���� Ȱ��ȭ ���� ��ų ���������� ���� �ǵ��� ��
		m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( mit->first, mit->second, 0 );
#else // SERV_SKILL_PAGE_SYSTEM
		m_kSkillTree.SetSkillLevelAndCSP( mit->first, mit->second, 0 );
#endif // SERV_SKILL_PAGE_SYSTEM
	}

	BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnsealedSkillID )
	{
		m_kSkillTree.SkillUnseal( (int)iSkillID );
	}

	KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
	kPacket.m_iOK						= kPacket_.m_iOK;
	kPacket.m_iSPoint					= kPacket_.m_iSPoint;
	kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
	kPacket.m_mapSkillList				= kPacket_.m_mapGetSkillList;
	kPacket.m_vecUnsealedSkillID		= kPacket_.m_vecUnsealedSkillID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendPacket( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ADMIN_GET_SKILL_REQ )
{
	// ������ ���� �����ִ� ��ų �� ���
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}	

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_ADMIN_GET_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iSkillID );
	cwstrCheatContents.Format( L"%d", kPacket_.m_iSkillLevel );
	WriteCheatLogToDB( L"/sl", cwstrCheatContents );

	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass(), kPacket_.m_iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_01;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kPacket_.m_iSkillID );
	if( pSkillTemplet == NULL )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_01;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}
	
	if( kPacket_.m_iSkillLevel <= 0 )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_20;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}

	int iSkillLevel = 0;
	int iSkillCSPoint = 0;

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( m_kSkillTree.IsActiveSkillPageNumberValid() )
	{
		// kimhc // ����ö // 2013-11-17 // �� �κ��� ���� �ڵ忡���� ���н� ���ٸ� ó���� ���⿡
		// �׳� ���� Step ���� ���� �ϵ��� �Ͽ���
		m_kSkillTree.GetSkillLevelAndCSP( iSkillLevel, iSkillCSPoint, kPacket_.m_iSkillID, 
			m_kSkillTree.AccessLearnedSkillTree() );
	}
	else
	{
		AddLogWhenSkillPagesNumberIsWrong( L"EGS_ADMIN_GET_SKILL_REQ" );		
		return;
	}
#else // SERV_SKILL_PAGE_SYSTEM
	if( m_kSkillTree.GetSkillLevelAndCSP( kPacket_.m_iSkillID, iSkillLevel, iSkillCSPoint ) == false )
	{
		iSkillCSPoint = 0;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	bool bUnsealed = false;
	if( pSkillTemplet->m_bBornSealed == true && m_kSkillTree.IsSkillUnsealed( kPacket_.m_iSkillID ) == false )
	{
		bUnsealed = true;
	}

	KDBE_ADMIN_GET_SKILL_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iSkillID = kPacket_.m_iSkillID;
	kPacketToDB.m_iSkillLevel = kPacket_.m_iSkillLevel;
	kPacketToDB.m_iCSPoint = iSkillCSPoint;
	kPacketToDB.m_bUnsealed = true;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacketToDB.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendToGameDB( DBE_ADMIN_GET_SKILL_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ADMIN_GET_SKILL_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_ADMIN_GET_SKILL_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
		return;
	}

	if( kPacket_.m_bUnsealed == true )
		m_kSkillTree.SkillUnseal( kPacket_.m_iSkillID );

#ifdef SERV_SKILL_PAGE_SYSTEM
	// kimhc // ����ö // �Ѱ��� ��ų ���� ġƮ ��� ��, ������ ��ų �������� ���� �ǵ��� ��
	m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( kPacket_.m_iSkillID, kPacket_.m_iSkillLevel, kPacket_.m_iCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
	m_kSkillTree.SetSkillLevelAndCSP( kPacket_.m_iSkillID, kPacket_.m_iSkillLevel, kPacket_.m_iCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM

	KEGS_ADMIN_GET_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iSkillID = kPacket_.m_iSkillID;
	kPacket.m_iSkillLevel = kPacket_.m_iSkillLevel;
	kPacket.m_iCSPoint = kPacket_.m_iCSPoint;
	kPacket.m_bUnsealed = kPacket_.m_bUnsealed;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	SendPacket( EGS_ADMIN_GET_SKILL_ACK, kPacket );
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( EGS_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}	

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", (int)kPacket_.m_cUnitClass );
	WriteCheatLogToDB( L"/uc", cwstrCheatContents );

	// ���� Ŭ���� �˻�
	if( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>(kPacket_.m_cUnitClass) ) == false )
	{
		START_LOG( cout, L"�������� �ʴ� ���� Ŭ������ ġƮ�� ����Ͽ����ϴ�. ���ڴ� ����� �Է����ּ���." )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_01; // ������ ĳ���� Ŭ������ �̻���.
		SendPacket( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
		return;
	}

	KDBE_CHANGE_UNIT_CLASS_REQ kPacketToDB;
	kPacketToDB.m_usEventID  = EGS_ADMIN_CHANGE_UNIT_CLASS_REQ;
	kPacketToDB.m_iUnitUID	 = GetCharUID();
	kPacketToDB.m_cUnitClass = kPacket_.m_cUnitClass;
	SendToGameDB( DBE_CHANGE_UNIT_CLASS_REQ, kPacketToDB );
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

//{{ 2008. 10. 24  ������	Ÿ��Ʋ ġƮ
IMPL_ON_FUNC( EGS_ADMIN_GET_TITLE_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_ADMIN_GET_TITLE_CHEAT_ACK kPacketAck;
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	if( !SiCXSLTitleManager()->IsExistTitleID( kPacket_.m_iTitleID ) )
	{
		kPacketAck.m_iOK = NetError::ERR_TITLE_03;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	if( m_kUserTitleManager.IsExistTitle( kPacket_.m_iTitleID ) )
	{
		kPacketAck.m_iOK = NetError::ERR_TITLE_05;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	if( kPacket_.m_iPeriod < 0  ||  kPacket_.m_iPeriod > 30 )
	{
		kPacketAck.m_iOK = NetError::ERR_TITLE_04;
		SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );
		return;
	}

	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ADMIN_GET_TITLE_CHEAT_ACK, kPacketAck );

	// DB�� Ÿ��Ʋ�� ����!
	KDBE_INSERT_TITLE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iTitleID = kPacket_.m_iTitleID;
	kPacketToDB.m_sPeriod  = static_cast<short>(kPacket_.m_iPeriod);
	SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );

	// �ߺ� ������ ���� �ʵ��� �̸� �������� �̼� ���� - TitleID�� MissionID�� ������
	m_kUserTitleManager.DeleteMissionInstance( kPacketToDB.m_iTitleID );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iPeriod );
	WriteCheatLogToDB( L"/t", cwstrCheatContents );
}
//}}

//{{ 2009. 7. 29  ������	item set cheat
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_GET_ITEM_SET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	KDBE_ADMIN_GET_ITEM_SET_NOT kPacket;
	kPacket.m_wstrNickName = GetCharName();
	SendToGameDB( DBE_ADMIN_GET_ITEM_SET_NOT, kPacket );

	// ġƮ �α�
	WriteCheatLogToDB( L"/is" );
}
//}}

//{{ 2009. 8. 5  ������		ü��IDġƮ
_IMPL_ON_FUNC( EGS_ADMIN_GUEST_USER_MODE_NOT, bool )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	// ü��ID ��� ����
	m_kNexonAccountInfo.m_byteGuestUser = ( kPacket_ ? 1 : 0 );

	START_LOG( cout, L"�����ڰ� ü��ID ��带 �����Ͽ����ϴ�!" )		
		<< BUILD_LOG( GetName() )		
		<< BUILD_LOG( IsGuestUser() );

	// ġƮ �α�
	std::wstring wstrGuestFlag;
	if( IsGuestUser() )
	{
		wstrGuestFlag = L"true";
	}
	else
	{
		wstrGuestFlag = L"false";		
	}

	WriteCheatLogToDB( L"/guest", wstrGuestFlag );
}
//}}

#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( EGS_ADMIN_GET_GUILD_SKILL_POINT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;

		return;
	}

	KEGS_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;	

	// ġƮ��� �ϴ��� �����Ͱ� ���ϼ� ������ �����˻� �� ��帶���� �˻縦 ����!
	// ��� ���� �������� �˻�
	if( GetGuildUID() <= 0 )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
		return;
	}

	// �ش� ġƮ�� ����� �� ����
	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_SKILL_00;
		SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
		return;
	}

	// ��� ��ų ġƮ
	START_LOG( cout, L"��ڰ� ��� ��ų ����Ʈ ġƮ�� ����Ͽ����ϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( GetGuildUID() )
#else
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( GetCharName() )
		<< BUILD_LOG( m_kUserGuildManager.GetGuildName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_iGuildSPoint );

	KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iGuildSPoint = kPacket_.m_iGuildSPoint;
	SendToLoginServer( ELG_ADMIN_GET_GUILD_SKILL_POINT_REQ, kPacketToLg );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iGuildSPoint );
	WriteCheatLogToDB( L"/gsp", cwstrCheatContents );
}

IMPL_ON_FUNC( ELG_ADMIN_GET_GUILD_SKILL_POINT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iCurGuildSPoint = kPacket_.m_iGuildSPoint;
	SendPacket( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
}

#endif GUILD_SKILL_TEST

#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( EGS_ADMIN_CHANGE_PET_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iCheatType, kPacket_.m_iValue );
	WriteCheatLogToDB( L"/pet", cwstrCheatContents );

	// ġƮ ó��
	KPetInfo kPetInfo;
	int iPetInventorySlotSize = 0;
	if( m_kUserPetManager.Handler_EGS_ADMIN_CHANGE_PET_INFO_NOT( kPacket_, GetThisPtr<KGSUser>(), kPetInfo, iPetInventorySlotSize ) == false )
	{
		START_LOG( cout, L"�� ������ ġƮ ����!" );
		return;
	}

	switch( kPacket_.m_iCheatType )
	{
	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EVOLUTION_STEP:
		{
			// �� ��ȭ ���� �ߴٸ� �κ��丮 ũ�� ��������!
			m_kInventory.ResetPetInventorySize( iPetInventorySlotSize );

			// �� ��ȭ ����� Ŭ���̾�Ʈ���� �˷�����!
			KEGS_PET_EVOLUTION_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_kEvolutionResultPetInfo = kPetInfo;
			SendPacket( EGS_PET_EVOLUTION_ACK, kPacketAck );

			START_LOG( cout, L"�� ��ȭ ġƮ ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_cEvolutionStep );

			//////////////////////////////////////////////////////////////////////////
			// �ʵ��̰ų� ���̸� �˸���!
			if( GetStateID() == KGSFSM::S_FIELD_MAP )
			{
				if( GetFieldUID() <= 0 )
				{
					KEGS_PET_EVOLUTION_NOT kPacketNot;
					kPacketNot.m_iUnitUID = GetCharUID();
					kPacketNot.m_kPetInfo = kPetInfo;
					SendPacket( EGS_PET_EVOLUTION_NOT, kPacketNot );
				}
				else
				{
					KERM_PET_EVOLUTION_NOT kPacketNot;
					kPacketNot.m_iUnitUID = GetCharUID();
					kPacketNot.m_kEvolutionPetInfo = kPetInfo;
					SendToGSField( ERM_PET_EVOLUTION_NOT, kPacketNot );
				}
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
					//{{ 2012. 11. 08	�ڼ���	��Ʋ�ʵ� : �ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}
					{
						KERM_PET_EVOLUTION_NOT kPacketNot;
						kPacketNot.m_iUnitUID = GetCharUID();
						kPacketNot.m_kEvolutionPetInfo = kPetInfo;
						SendToCnRoom( ERM_PET_EVOLUTION_NOT, kPacketNot );
					}
					break;

				case CXSLRoom::RT_SQUARE:
					// �����̸� �Ⱥ�����!
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
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_SATIETY:
		{
			KEGS_CHANGED_PET_SATIETY_NOT kNot;
			kNot.m_iUnitUID = GetCharUID();
			kNot.m_iPetUID = kPetInfo.m_iPetUID;
			kNot.m_sCurrentSatiety = kPetInfo.m_sSatiety;

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				SendToCnRoom( ERM_CHANGED_PET_SATIETY_NOT, kNot );
			}
			else
			{
				SendPacket( EGS_CHANGED_PET_SATIETY_NOT, kNot );
			}

			// �ڵ� ���� ��� üũ!
			CheckAutoFeed();

			START_LOG( cout, L"�� ������ ġƮ ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sSatiety );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_INTIMACY:
		{
			KEGS_CHANGED_PET_INTIMACY_NOT kNot;
			kNot.m_iUnitUID = GetCharUID();
			kNot.m_iPetUID = kPetInfo.m_iPetUID;
			kNot.m_iCurrentIntimacy = kPetInfo.m_iIntimacy;

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				SendToCnRoom( ERM_CHANGED_PET_INTIMACY_NOT, kNot );
			}
			else
			{
				SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kNot );
			}

			START_LOG( cout, L"�� ģ�е� ġƮ ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT				
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sSatiety );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EXTROVERSION:
		{
			START_LOG( cout, L"�� ���⼺ ġƮ ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sExtroversion );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EMOTION:
		{
			START_LOG( cout, L"�� ������ ġƮ ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT	
				<< BUILD_LOG( kPacket_.m_iValue )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOGc( kPetInfo.m_sEmotion );
		}
		break;
	}
}

#endif SERV_PET_SYSTEM

//{{ 2010. 8. 16	������	�Ⱓ ���� ������ �̺�Ʈ
#ifdef SERV_RESET_PERIOD_EVENT

IMPL_ON_FUNC( EGS_ADMIN_SHOW_ED_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%s", kPacket_.m_wstrNickName.c_str() );
	WriteCheatLogToDB( L"/usered", cwstrCheatContents );

	// �α��� ������ ������!
	SendToLoginServer( ELG_ADMIN_SHOW_ED_CHEAT_REQ, kPacket_ );
}

IMPL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KELG_ADMIN_SHOW_ED_CHEAT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iSenderUserUID = kPacket_.m_iSenderUserUID;
	kPacket.m_wstrNickName = GetCharName();
	kPacket.m_iED = GetED();
	SendToLoginServer( ELG_ADMIN_SHOW_ED_CHEAT_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_ACK, KEGS_ADMIN_SHOW_ED_CHEAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADMIN_SHOW_ED_CHEAT_ACK, kPacket_ );
}

#endif SERV_RESET_PERIOD_EVENT
//}}

//{{ 2011. 04. 16	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
IMPL_ON_FUNC( EGS_ADMIN_PSHOP_AGENCY_CHEAT_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_sPeriod );
	WriteCheatLogToDB( L"/shop", cwstrCheatContents );

	KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_sAgencyPeriod = kPacket_.m_sPeriod;
	kPacketToDB.m_usEventID = EGS_ADMIN_PSHOP_AGENCY_CHEAT_REQ;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    kPacketToDB.m_cShopType = SEnum::AST_PREMIUM; // CXSLSquareUnit::PST_PREMIUM;
#else //SERV_UPGRADE_TRADE_SYSTEM
	kPacketToDB.m_cShopType = CXSLSquareUnit::PST_PREMIUM; // CXSLSquareUnit::PST_PREMIUM;
#endif //SERV_UPGRADE_TRADE_SYSTEM
	SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
}
#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 08. 11	������	���� ���� ä�θ� 
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
IMPL_ON_FUNC( EGS_ADMIN_CHANNELING_CODE_CHEAT_NOT )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	// ġƮ ����
	m_kNexonAccountInfo.m_uChannelCode = static_cast<UINT8>(kPacket_.m_iChannelingCode);

	START_LOG( cout, L"�����ڰ� ä�θ� ġƮ �ڵ带 ����Ͽ����ϴ�!" )
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetChannelCode() );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iChannelingCode );
	WriteCheatLogToDB( L"/channel", cwstrCheatContents );
}
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
//}}

//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
IMPL_ON_FUNC( EGS_ADMIN_HENIR_REWARD_CHEAT_NOT )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// ġƮ ����
	m_kUserDungeonManager.SetPossibleHenirRewardNormalCount( kPacket_.m_iHenirRewardNormalCnt );

	START_LOG( cout, L"�����ڰ� ä�θ� ġƮ �ڵ带 ����Ͽ����ϴ�!" )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iHenirRewardNormalCnt );
	WriteCheatLogToDB( L"/henir", cwstrCheatContents );
}
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
IMPL_ON_FUNC( EGS_ADMIN_ED_MONITORING_CHEAT_NOT )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	// ġƮ ����
	m_kUserHackingManager.SetEDMonitoringMode( kPacket_.m_bEDMonitoringMode );

	START_LOG( cout, L"�����ڰ� ED ���� ġƮ �ڵ带 ����Ͽ����ϴ�!" )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_bEDMonitoringMode );
	WriteCheatLogToDB( L"/EDMonitoring", cwstrCheatContents );

}
#endif SERV_ED_MONITORING_IN_GAME
//}}

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
IMPL_ON_FUNC( EGS_ADMIN_NPC_UNIT_CREATE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"GM�� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_ADMIN_NPC_UNIT_CREATE_REQ, kPacket_ );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_kNPCUnitReq.m_NPCID );
	WriteCheatLogToDB( L"/cm", cwstrCheatContents );
}

_IMPL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADMIN_NPC_UNIT_CREATE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_NOT, KEGS_ADMIN_NPC_UNIT_CREATE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADMIN_NPC_UNIT_CREATE_NOT, kPacket_ );
}
#endif SERV_CREATE_ELITE_FOR_ADMIN

//{{ 2012. 05. 30	������	�׳�ý� �Ⱓ ġƮ
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
IMPL_ON_FUNC( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	// ���ڿ� ����
	std::wstring wstrModifyDate = boost::str( boost::wformat( L"%04d-%02d-%02d %02d:%02d:00" ) % kPacket_.m_iYear % kPacket_.m_iMonth % kPacket_.m_iDay % kPacket_.m_iHour % kPacket_.m_iMinute );

	// ġƮ ����
	START_LOG( cout, L"�����ڰ� ĳ�� ��ų ����Ʈ �Ⱓ�� �����ϴ� ġƮ �ڵ带 ����Ͽ����ϴ�!" )
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetCharName() )
		<< BUILD_LOG( wstrModifyDate );

	KDBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_wstrModifyDate = wstrModifyDate;
	SendToGameDB( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ, kPacketToDB );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d %d %d %d", kPacket_.m_iYear, kPacket_.m_iMonth, kPacket_.m_iDay, kPacket_.m_iHour, kPacket_.m_iMinute );
	WriteCheatLogToDB( L"/cspdate", cwstrCheatContents );
}

_IMPL_ON_FUNC( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK, KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		START_LOG( cout, L"ĳ�� ��ų ����Ʈ �Ⱓ ���� ġƮ ����!" )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrCspEndDate );

		LIF( m_kSkillTree.SetCSPointEndDate( kPacket_.m_wstrCspEndDate ) );
	}
	else
	{
		START_LOG( cerr, L"ĳ�� ��ų ����Ʈ �Ⱓ ���� ġƮ ����!" )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrCspEndDate )
			<< END_LOG;
	}

	SendPacket( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK, kPacket_ );
}
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
//}}

//{{ 2012. 07. 24	�ڼ���	�ش� ĳ������ ��� ��ų�� �� ��� ġƮ
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
#else // SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
#endif // SERV_SKILL_PAGE_SYSTEM
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	if ( kPacket_.m_iActiveSkillPageNumber != m_kSkillTree.GetActiveSkillPageNumber() )
	{
		START_LOG( cerr, L"Ȱ���� ��ų ������ ��ȣ�� �̻��մϴ� (EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ)" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
			<< BUILD_LOG( m_kSkillTree.GetActiveSkillPageNumber() )
			<< END_LOG;

		KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SKILL_PAGE_01;;
		SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
		return;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	// 1. �ش� Ŭ������ ��� ��ų ���� ����
	const CXSLSkillTree::SkillTreeTempletMap* pSkillTreeTempletMap = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass() );
	if( pSkillTreeTempletMap == NULL )
	{
		START_LOG( cerr, L"�ش� Ŭ������ ��ų ������ ���� �� �����ϴ�.")
			<< BUILD_LOG( GetUnitClass() )
			<< END_LOG;
		
		KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
		return;
	}

	KDBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();

	CXSLSkillTree::SkillTreeTempletMap::const_iterator it;
	for( it=pSkillTreeTempletMap->begin(); it != pSkillTreeTempletMap->end(); ++it )
	{
		// ���� ���� �Ǿ��ִ� ��ų ������ �����Ѵ�.
		if( m_kSkillTree.IsSkillUnsealed( it->first ) == false )
		{
			kPacket.m_vecUnSealedSkill.push_back( it->first );
		}
		
		// ���� �ش� ��ų�� ���ǰ� �ִ� ĳ�� ��ų ����Ʈ�� ���ؿ��� ���ٸ�, 0���� �����Ѵ�.
		KAdminCheatSkill kAdminCheatSkill;

#ifdef SERV_SKILL_PAGE_SYSTEM
		if ( m_kSkillTree.IsActiveSkillPageNumberValid() )
		{
			// kimhc // ����ö // 2013-11-17 // �� �κ��� ���� �ڵ忡���� ���н� ���ٸ� ó���� ���⿡
			// �׳� ���� Step ���� ���� �ϵ��� �Ͽ���
			m_kSkillTree.GetSkillLevelAndCSP( kAdminCheatSkill.m_iSkillLevel, kAdminCheatSkill.m_iSkillCSPoint, 
				it->first, m_kSkillTree.AccessLearnedSkillTree() );
		}
		else
		{
			AddLogWhenSkillPagesNumberIsWrong( L"EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ" );
			return;
		}
#else // SERV_SKILL_PAGE_SYSTEM
		if( m_kSkillTree.GetSkillLevelAndCSP( it->first, kAdminCheatSkill.m_iSkillLevel, kAdminCheatSkill.m_iSkillCSPoint ) == false )
		{
			kAdminCheatSkill.m_iSkillCSPoint = 0;
		}
#endif // SERV_SKILL_PAGE_SYSTEM
		
		// ��ų ������ �ִ�� �����Ѵ�.
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		kAdminCheatSkill.m_iSkillLevel = it->second.m_iMasterSkillLevel;
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		kAdminCheatSkill.m_iSkillLevel = it->second.m_iMaxSkillLevel;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		kPacket.m_mapSkillInfo.insert( std::map<int, KAdminCheatSkill>::value_type( it->first, kAdminCheatSkill ) );
	}
	SendToGameDB( DBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ, kPacket );

	// ġƮ �α�
	WriteCheatLogToDB( L"/fsk" );
}

IMPL_ON_FUNC( DBE_ADMIN_CHEAT_GET_ALL_SKILL_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�Ϻ� ��ų�� ���ؼ� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
	}

	// ��ų ���� ����
	if( kPacket_.m_vecUnSealedSkill.empty() == false )
	{
		std::set<int>::iterator it;
		BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnSealedSkill )
		{
			m_kSkillTree.SkillUnseal( iSkillID );
		}
	}

	if( kPacket_.m_mapSkillInfo.empty() == false )
	{
		std::map<int, KAdminCheatSkill>::iterator it;
		for( it=kPacket_.m_mapSkillInfo.begin(); it != kPacket_.m_mapSkillInfo.end(); ++it )
		{
#ifdef SERV_SKILL_PAGE_SYSTEM
			// kimhc // ����ö // ��� ��ų ���� ġƮ ��� ��, ���� ������� ��ų �������� ���� �ǵ��� ��
			m_kSkillTree.CheckAndUpdateSkillLevelAndCSPOnUsedPage( it->first, it->second.m_iSkillLevel, it->second.m_iSkillCSPoint );
#else // SERV_SKILL_PAGE_SYSTEM
			m_kSkillTree.SetSkillLevelAndCSP( it->first, it->second.m_iSkillLevel, it->second.m_iSkillCSPoint );
#endif // SERV_SKILL_PAGE_SYSTEM
		}
	}

	//
	const CXSLSkillTree::SkillTreeTempletMap* pSkillTreeTempletMap = SiCXSLSkillTree()->GetSkillTreeTemplet( GetUnitClass() );
	if( pSkillTreeTempletMap == NULL )
	{
		START_LOG( cerr, L"�ش� Ŭ������ ��ų ������ ���� �� �����ϴ�.")
			<< BUILD_LOG( GetUnitClass() )
			<< END_LOG;
	}

	kPacket_.m_vecUnSealedSkill.clear();

	CXSLSkillTree::SkillTreeTempletMap::const_iterator it;
	for( it=pSkillTreeTempletMap->begin(); it != pSkillTreeTempletMap->end(); ++it )
	{
		if( SiCXSLSkillTree()->IsSealSkill( it->first ) == true )
		{
			kPacket_.m_vecUnSealedSkill.push_back( it->first );
		}
	}

	// Ŭ���̾�Ʈ���� �˸���
	SendPacket( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket_ );
}
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
IMPL_ON_FUNC( EGS_UDP_KICK_GAMEEDIT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}


	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_SERVER, 0, NULL, ECN_UDP_KICK_GAMEEDIT_NOT, kPacket_ );

}

#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ ��Ʋ�ʵ� ���赵 Ȯ�� ġƮ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
IMPL_ON_FUNC_NOPARAM( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT kPacket;
		kPacket.m_iDangerousValue = -1;
		SendPacket( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT, kPacket );
		return;
	}

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				SendToCnRoom( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ );

				START_LOG( cout, L"��Ʋ �ʵ� ���赵 Ȯ��!." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;
			}
			break;

		default:
			START_LOG( cerr, L"��Ʋ �ʵ尡 �ƴѵ� ���赵 Ȯ���� �Ϸ� �մϴ�." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetRoomUID() )
				<< END_LOG;
		}
	}	
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK, int )
{
	KEGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT kPacket;
	kPacket.m_iDangerousValue = kPacket_;
	SendPacket( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT, kPacket );

	START_LOG( cout, L"���� ��Ʋ �ʵ��� ���赵�� Ȯ���մϴ�." )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( kPacket.m_iDangerousValue )
		<< END_LOG;
}
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
//}}

//{{ 2012. 11. 20	�ڼ���	���� ���� ������ ������ ����Ʈ
#ifdef SERV_HERO_PVP_MANAGE_LIST
IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_HERO_PVP_USER_LIST_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_HERO_PVP_USER_LIST_ACK kPacket;
		SendPacket( EGS_ADMIN_HERO_PVP_USER_LIST_ACK, kPacket );
		return;
	}
	
	SendToGlobalServer( EGB_ADMIN_HERO_PVP_USER_LIST_REQ );
	
	// ġƮ �α�
	WriteCheatLogToDB( L"/obsread");
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_LIST_ACK, KEGS_ADMIN_HERO_PVP_USER_LIST_ACK )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_HERO_PVP_USER_ADD_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_HERO_PVP_USER_ADD_ACK kPacket;
		SendPacket( EGS_ADMIN_HERO_PVP_USER_ADD_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_HERO_PVP_USER_ADD_REQ, kPacket_ );

	// ġƮ �α�
	WriteCheatLogToDB( L"/obsadd");
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_ADD_ACK, KEGS_ADMIN_HERO_PVP_USER_ADD_ACK )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_ADD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_HERO_PVP_USER_REMOVE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK kPacket;
		SendPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_HERO_PVP_USER_REMOVE_REQ, kPacket_ );

	// ġƮ �α�
	WriteCheatLogToDB( L"/obsdel");
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_REMOVE_ACK, KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KPacketOK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
		return;
	}

	// ĳ���� �г��� ���� �˻�.
	if( kPacket_.m_wstrNickName.empty() == false )
	{
		if( ( kPacket_.m_wstrNickName.size() < 2 ) || ( 13 < kPacket_.m_wstrNickName.size() ) )
		{
			START_LOG( cerr, L"ĳ���� �г��� ���̰� �߸��Ǿ����ϴ�.")
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;

			KPacketOK kPacket;
			kPacket.m_iOK = NetError::ERR_CREATE_UNIT_04;
			SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
			return;
		}
		
#ifdef EXTEND_SERVER_GROUP_MASK
		if( kPacket_.m_iServerGroupID <= -1 )
#else EXTEND_SERVER_GROUP_MASK
		if( ( kPacket_.m_iServerGroupID <= SEnum::SGI_INVALID ) || ( SEnum::SGI_END <= kPacket_.m_iServerGroupID ) )
#endif EXTEND_SERVER_GROUP_MASK
		{
			START_LOG( cerr, L"�߸��� ������ ���̵�")
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iServerGroupID )
				<< END_LOG;

			KPacketOK kPacket;
			kPacket.m_iOK = NetError::ERR_HERO_PVP_USER_LIST_01;
			SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket );
			return;
		}
	}

	SendToGlobalServer( EGB_ADMIN_NOTIFY_HERO_PVP_USER_REQ, kPacket_ );

	// ġƮ �α�
	CStringW cwstrCheatContents;

	if( kPacket_.m_wstrNickName.empty() == true )
	{
		cwstrCheatContents.Format( L"%s", kPacket_.m_wstrNickName.c_str() );
		WriteCheatLogToDB( L"/obs", cwstrCheatContents );
	}
	else
	{
		cwstrCheatContents.Format( L"%d %s", kPacket_.m_iServerGroupID, kPacket_.m_wstrNickName.c_str() );
		WriteCheatLogToDB( L"/obsw", cwstrCheatContents );
	}
}

_IMPL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_ACK, KPacketOK )
{
	SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_ADMIN_NOTIFY_HERO_PVP_USER_NOT, KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ )
{
	SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_CONNECTION_NOT, KEGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT )
{
	SendPacket( EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_CHECK_NOT, bool )
{
	m_bHeroPvpUser = kPacket_;
}
#endif SERV_HERO_PVP_MANAGE_LIST
//}}

//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
IMPL_ON_FUNC( EGS_ADMIN_OBSERVER_MODE_REQ )
{
	KEGS_ADMIN_OBSERVER_MODE_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_OBSERVER_MODE_ACK, kPacket );
		return;
	}

	// ������ ��� ����
	m_kUserCheatManager.SetObserverMode( kPacket_.m_bObserverMode );

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ADMIN_OBSERVER_MODE_ACK, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_bObserverMode );
	WriteCheatLogToDB( L"/observer", cwstrCheatContents );
}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
IMPL_ON_FUNC( EGS_ADMIN_DELETE_GUILD_ADD_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_GUILD_ADD_NOT kPacket;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;

	SendToLoginServer( ELG_ADMIN_DELETE_GUILD_ADD_NOT, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iGuildUID );

	WriteCheatLogToDB( L"/dg", cwstrCheatContents );
}
#endif // SERV_DELETE_GUILD_ADD_CHEAT

//{{ 2011.03.04  �ӱԼ� ��ϸ� ��ŷ ���� ġƮ ( ���,������ ���� )
#ifdef SERV_DELETE_HENIR_RANKING
IMPL_ON_FUNC( EGS_ADMIN_DELETE_HENIR_RANKING_RANK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	if( kPacket_.m_iRank < 1 || kPacket_.m_iRank > 100 )
	{
		START_LOG( cerr, L"1�� ���� 100�� ������ ���� ����" )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT kPacket;
	kPacket.m_iRankingType = kPacket_.m_iRankingType;
	kPacket.m_iRank = kPacket_.m_iRank;

	SendToLoginServer( ELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iRankingType, kPacket_.m_iRank );

	WriteCheatLogToDB( L"/dhr", cwstrCheatContents );
}

IMPL_ON_FUNC( EGS_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT kPacket;
	kPacket.m_iRankingType = kPacket_.m_iRankingType;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;

	SendToLoginServer( ELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d, %s", kPacket_.m_iRankingType, kPacket_.m_wstrNickName );

	WriteCheatLogToDB( L"/dhn", cwstrCheatContents );
}

IMPL_ON_FUNC( EGS_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT kPacket;
	kPacket.m_iRankingType = kPacket_.m_iRankingType;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	SendToLoginServer( ELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d %d", kPacket_.m_iRankingType, kPacket_.m_iUnitUID );

	WriteCheatLogToDB( L"/dhu", cwstrCheatContents );
}
#endif SERV_DELETE_HENIR_RANKING
//}}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
IMPL_ON_FUNC( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK );

	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK kPacket;

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..���赵 ���� �Ϸ��� ����" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
		return;
	}

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				SendToCnRoom( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, kPacket_ );

				START_LOG( cout, L"��Ʋ �ʵ� ���赵 Ȯ��!." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;
			}
			break;

		default:
			{
				START_LOG( cerr, L"��Ʋ �ʵ尡 �ƴѵ� ���赵 ���� �Ϸ� �մϴ�." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
				SendPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
				return;
			}
		}
	}		
}

_IMPL_ON_FUNC( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK )
{
	SendPacket( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket_ );

	START_LOG( cout, L"���� ��Ʋ �ʵ��� ���赵�� �����մϴ�." )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( kPacket_.m_iDanger )
		<< END_LOG;

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iDanger );
	WriteCheatLogToDB( L"/afd", cwstrCheatContents );
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}
#ifdef SERV_POINT_COUNT_SYSTEM_GAME_EDIT_COMMAND
IMPL_ON_FUNC( EGS_QUEST_POINT_CHEAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڵ� �ƴѵ� ����" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOGc( GetAuthLevel() )
			<< END_LOG;
		return;
	}

	if (kPacket_.m_iQuestPointCheat < 0)
	{
		return;
	}

	if ( kPacket_.m_iQuestPointCheat > 200 )
		kPacket_.m_iQuestPointCheat = 200;

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iQuestPointCheat );
	WriteCheatLogToDB( L"/questpoint", cwstrCheatContents );

#ifdef SERV_POINT_COUNT_SYSTEM
	m_kUserQuestManager.SetUpdateQuestInstance(GetThisPtr<KGSUser>());
#endif //SERV_POINT_COUNT_SYSTEM
}
#endif //SERV_POINT_COUNT_SYSTEM_GAME_EDIT_COMMAND

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( EGS_ADMIN_CHANGE_COUPLE_DATE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..���赵 ���� �Ϸ��� ����" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
		return;
	}

	if( m_kUserRelationshipManager.GetRelationshipType() < SEnum::RT_COUPLE )
	{
		KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_36;
		SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
		return;
	}

	START_LOG( cout, L"Ŀ�� ���� �ð��� �����մϴ�." )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( kPacket_.m_iAddHour )
		<< END_LOG;

	KDBE_ADMIN_CHANGE_COUPLE_DATE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
	kPacket.m_fAddRate = ((float)kPacket_.m_iAddHour) / 24.0f * (-1.f);

	SendToGameDB( DBE_ADMIN_CHANGE_COUPLE_DATE_REQ, kPacket );

	// ġƮ �α�
	CStringW cwstrCheatContents;
	cwstrCheatContents.Format( L"%d", kPacket_.m_iAddHour );
	WriteCheatLogToDB( L"/couple", cwstrCheatContents );

}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_COUPLE_DATE_ACK, KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Ŀ�� ��¥ ����ġƮŰ ��� ����!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}

	SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_EVENT_CHECK_POWER
IMPL_ON_FUNC( EGS_SET_MULTIPLYER )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"�������� ��Ŀ ����" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	SiKGameSysVal()->m_fMultiplayer = kPacket_.fM;
}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-09	// �ڼ���
IMPL_ON_FUNC( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"������ ���� ������ ����Ϸ� �Ͽ����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;

		KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_10;	// ����� �� ���� �����Դϴ�.
		SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket );
		return;
	}

	// ����Ʈ�� ������ ���ΰ�?
	if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( kPacket_.m_iBattleFieldID ) == false )
	{
		KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_01;	// �ش� �ʵ忡���� ���� �ʵ� ����Ʈ�� �������� �ʽ��ϴ�.
		SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ, kPacket_ );

	// ġƮ �α� ���ܾ� ��.
	// WriteCheatLogToDB üũ
}

IMPL_ON_FUNC( EGB_ADMIN_BOSS_FIELD_GATE_OPEN_ACK )
{
	SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"������ ���� ������ ����Ϸ� �Ͽ����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	SendToGlobalServer( EGB_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );

	// ġƮ �α� ���ܾ� ��.
	// WriteCheatLogToDB üũ
}

IMPL_ON_FUNC( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"������ ���� ������ ����Ϸ� �Ͽ����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		
		KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
		return;
	}

	// ����Ʈ�� ������ ���ΰ�?
	if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( kPacket_.m_iBattleFieldID ) == false )
	{
		KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_01;	// �ش� �ʵ忡���� ���� �ʵ� ����Ʈ�� �������� �ʽ��ϴ�.
		SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ, kPacket_ );

	// ġƮ �α� ���ܾ� ��.
	// WriteCheatLogToDB üũ
}

IMPL_ON_FUNC( EGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK )
{
	SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ )
{
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cwarn, L"������ ���� ������ ����Ϸ� �Ͽ����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		
		KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_12;
		SendPacket( EGS_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
		return;
	}

	// ����Ʈ�� ������ ���ΰ�?
	if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( kPacket_.m_iBattleFieldID ) == false )
	{
		KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_01;	// �ش� �ʵ忡���� ���� �ʵ� ����Ʈ�� �������� �ʽ��ϴ�.
		SendPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK, kPacket );
		return;
	}

	SendToGlobalServer( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ, kPacket_ );

	// ġƮ �α� ���ܾ� ��.
	// WriteCheatLogToDB üũ
}

IMPL_ON_FUNC( EGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK )
{
	SendPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK, kPacket_ );
}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
IMPL_ON_FUNC( EGS_EVENT_COBO_ITEM_GIVE_CHEAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}
	if(kPacket_.m_bNextDay == true)
	{
		KEGS_EVENT_COBO_DUNGEON_FIELD_NOT kPacketCoboNot;
		//���� �ð��� �޾ƿ´�.
		CTime tChangeEventTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
		CTime tCurTime_ = CTime::GetCurrentTime();
		//���� �ð��� �ٲ��� �ϴ� �̺�Ʈ �ð��� ���Ѵ�.
		std::wstring GameEventTime = tChangeEventTime.Format(L"%Y-%m-%d %H:%M:%S");

		if( tCurTime_ < tChangeEventTime)
		{
			///���� �ð��� �ٲ��� �ϴ� ���� �ð����� ������ ���� Ŭ��� ���� �־�� �Ѵ�.
			///�� �ָ��� ��쿡�� �ʵ� ī��Ʈ�� �����־�� �ϴϱ� �ָ����� üũ�� �ؾ��Ѵ�.
			CTime	tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[1];
			CTime   tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[2];
			GameEventTime = tWeekEndTimeStart.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"���� �ָ� Ÿ�� üũ����" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;

			GameEventTime = tWeekEndTimeEnd.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"���� �ָ� Ÿ�� üũ��" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;
			///�Ϸ簡 �������� ���� ��� �ʱ�ȭ �ϰ� UIȰ��ȭ �����ָ� �ȴ�.
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_DungeonCountUI = true;
			kPacketCoboNot.m_DungeonCount = 0;
			kPacketCoboNot.m_iRemaindTime = -1;
			if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //�̷��� �ָ��̴�
			{
				//�ָ��̸� �ʵ�ī��Ʈ UI�� Ȱ��ȭ 
				kPacketCoboNot.m_FieldCountUI = true;
				kPacketCoboNot.m_FieldMonsterKillCount = 0;
			}
			SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
			START_LOG( cerr, L"Ŭ������ ������ ���� ���� ���޹޾����� �Ϸ� ��������" )
				<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
				<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
				<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
				<< END_LOG;
			///DB�� �ʱ�ȭ �Ϸ� ����
			//���⼭ ���� ų���� ���� Ŭ����� �����ؾ� �ϴϱ� ���� ����
			KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_bItemGive = 0;
			kPacketToDB.m_wstrButtonClickTime_One = GetButtonClickTime().Format(L"%Y-%m-%d %H:%M:%S");
			kPacketToDB.m_iDungeonClearCount = 0;
			kPacketToDB.m_iFieldMonsterKillCount = 0;
			SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
			SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
			SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
			SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
			SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
			SetDungeonCount(kPacketCoboNot.m_DungeonCount);
			SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
			SetCoboItemGive(false);

		}			
		else
		{
			///���� �ð��� �ٲ��� �ϴ� ���� �ð����� ������ ���� Ŭ��� ���� �־�� �Ѵ�.
			///�� �ָ��� ��쿡�� �ʵ� ī��Ʈ�� �����־�� �ϴϱ� �ָ����� üũ�� �ؾ��Ѵ�.
			CTime	tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[3];
			CTime tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[4];
			CTime TempTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
			GameEventTime = tWeekEndTimeStart.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"�ʵ� �ָ� Ÿ�� üũ����" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;
			GameEventTime = tWeekEndTimeEnd.Format(L"%Y-%m-%d %H:%M:%S");
			START_LOG( cerr, L"�ʵ� �ָ� Ÿ�� üũ��" )
				<< BUILD_LOG( GameEventTime )
				<< END_LOG;
			///�Ϸ簡 �������� ���� ��� �ʱ�ȭ �ϰ� UIȰ��ȭ �����ָ� �ȴ�.
			kPacketCoboNot.m_StartButtonUI = true;
			kPacketCoboNot.m_FieldCountUI = true;
			kPacketCoboNot.m_FieldMonsterKillCount = 0;
			kPacketCoboNot.m_DungeonCount = 0;
			kPacketCoboNot.m_iRemaindTime = -1;
			if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //�̷��� �ָ��̴�
			{
				//�ָ��̸� �ʵ�ī��Ʈ UI�� Ȱ��ȭ 
				kPacketCoboNot.m_DungeonCountUI = true;
			}
			SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
			START_LOG( cerr, L"Ŭ������ ������ ���� ���� ���޹޾����� �Ϸ� ��������" )
				<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
				<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
				<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
				<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
				<< END_LOG;
			///DB�� �ʱ�ȭ �Ϸ� ����
			KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_bItemGive = 0;
			kPacketToDB.m_wstrButtonClickTime_One = GetButtonClickTime().Format(L"%Y-%m-%d %H:%M:%S");
			kPacketToDB.m_iDungeonClearCount = 0;
			kPacketToDB.m_iFieldMonsterKillCount = 0;
			SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
			SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
			SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
			SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
			SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
			SetDungeonCount(kPacketCoboNot.m_DungeonCount);
			SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
			SetCoboItemGive(false);
		}
	}
	else
	{
		//���� ��� �ʱ�ȭ 
		KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_bItemGive = kPacket_.m_CoboEventITemGet;
		kPacketToDB.m_wstrButtonClickTime_One = GetButtonClickTime().Format(L"%Y-%m-%d %H:%M:%S");
		kPacketToDB.m_iDungeonClearCount = 0;
		kPacketToDB.m_iFieldMonsterKillCount = 0;
		SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
	}
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
IMPL_ON_FUNC( EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_CHEAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"��ڰ� �ƴѵ� ����..?��?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}
	//ġƮ ���� 
	///�ʱ�ȭ�� �����ְ� DB�� ���� ����ϰ� Ŭ�� �˷�����
	SetValentineItemCount( kPacket_.m_iValentineItemCount );
	KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT kPacketValen;
	kPacketValen.m_iValentineItemCount = kPacket_.m_iValentineItemCount;
	SendPacket(EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT,kPacketValen);
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM