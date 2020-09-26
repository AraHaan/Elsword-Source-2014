#include ".\guild.h"
#include "NetError.h"
#include "Enum/Enum.h"
//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	#include "LoginServer.h"
#endif SERV_INTEGRATION
//}}
//{{ 2009. 10. 28  ������	��巹��
#ifdef GUILD_TEST
	#include "X2Data/XSLGuildManager.h"
#endif GUILD_TEST
//}}
//{{ 2010. 01. 19  ������	���Խ���
#ifdef SERV_GUILD_AD
	#include "GuildBoardManager.h"
#endif SERV_GUILD_AD
//}}
#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-24
	#include "GameSysVal/GameSysVal.h"
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef GUILD_TEST

KGuild::KGuild( const KGuildInfo& kGuildInfo ) :
m_iGuildMasterUnitUID( 0 ),
m_iLoginedMemberCount( 0 )
{
	m_kGuildInfo = kGuildInfo;

	// ��� ������ üũ
	CheckGuildLevelUp();
}

KGuild::~KGuild(void)
{
}

//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST

void KGuild::Tick()
{
	// 1�и��� ĳ�ý�ų ����Ʈ�� ���� �Ⱓ üũ
	if( m_kTimer[TM_CHECK_GUILD_CASH_SKILL_POINT].elapsed() < 60.0 )
		return;

	m_kTimer[TM_CHECK_GUILD_CASH_SKILL_POINT].restart();

	if( m_kSkillTree.IsCashSkillPointExpired() == false )
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		if( tCurrentTime > m_kSkillTree.GetGuildCSPointEndDate() )
		{
			// ��� ��ų Ʈ���߿��� CSP�� ��� ��ų �ǵ�����, ���� CSP ����Ʈ ����ϰ� m_iMaxCSPoint�� ���� SP�� �����ְ�
			// ������ ��ų�߿� �ȹ�� ��ų�� ������ Ż���Ѵ�
			KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ kPacketToDB;
			kPacketToDB.m_iGuildUID = GetUID();
			m_kSkillTree.CalcExpireCashSkillPoint( kPacketToDB.m_iGuildSPoint, kPacketToDB.m_iRetrievedSPoint, kPacketToDB.m_vecGuildSkillData );
			SendToGameDB( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ, kPacketToDB );
		}
	}
}

#endif GUILD_SKILL_TEST
//}}

//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST

void KGuild::GetGuildInfo( OUT KGuildInfo& kGuildInfo, OUT std::wstring& wstrGuildMasterName, OUT std::vector< UidType >& vecMemberUnitUID, OUT KGuildSkillInfo& kGuildSkillInfo ) const
{
	vecMemberUnitUID.clear();

	// ��� ����
	GetGuildInfo( kGuildInfo );

	// ���� ����Ʈ
	std::vector< KGuildMemberInfoPtr >::const_iterator vit;
	for( vit = m_vecGuildMember.begin(); vit != m_vecGuildMember.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		// ��� ������ �г���
		if( m_iGuildMasterUnitUID == (*vit)->m_iUnitUID )
		{
			wstrGuildMasterName = (*vit)->m_wstrNickName;
		}

		vecMemberUnitUID.push_back( (*vit)->m_iUnitUID );
	}

	// ��� ��ų ���� ���
	m_kSkillTree.GetGuildSkillInfo( kGuildSkillInfo );
}

#else

void KGuild::GetGuildInfo( OUT KGuildInfo& kGuildInfo, OUT std::wstring& wstrGuildMasterName, OUT std::vector< UidType >& vecMemberUnitUID ) const
{
	vecMemberUnitUID.clear();

	// ��� ����
	GetGuildInfo( kGuildInfo );

	// ���� ����Ʈ
	std::vector< KGuildMemberInfoPtr >::const_iterator vit;
	for( vit = m_vecGuildMember.begin(); vit != m_vecGuildMember.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		// ��� ������ �г���
		if( m_iGuildMasterUnitUID == (*vit)->m_iUnitUID )
		{
			wstrGuildMasterName = (*vit)->m_wstrNickName;
		}

		vecMemberUnitUID.push_back( (*vit)->m_iUnitUID );
	}
}

#endif GUILD_SKILL_TEST
//}}

KGuildMemberInfoPtr KGuild::GetGuildMember( UidType iUnitUID ) const
{
	std::map< UidType, KGuildMemberInfoPtr >::const_iterator mit = m_mapGuildMember.find( iUnitUID );
	if( mit == m_mapGuildMember.end() )
	{
		START_LOG( cwarn, L"ã�� ��� ����� �����ϴ�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KGuildMemberInfoPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��� ��� ������ �̻�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KGuildMemberInfoPtr();
	}

	return mit->second;
}

bool KGuild::IsLoginedMember( IN UidType iUnitUID )
{
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"���� ���� ��� ����. ���� �α��� ����!" )			
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	// �α׾ƿ� �ð��� 0�̶�� �α��ε� �����̴�.
	return spGuildMember->IsLogined();
}

int KGuild::LoginedMemberCounting() const
{
	int iLoginedMemberCount = 0;

	// ���� ����Ʈ
	std::vector< KGuildMemberInfoPtr >::const_iterator vit;
	for( vit = m_vecGuildMember.begin(); vit != m_vecGuildMember.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->IsLogined() )
		{
			++iLoginedMemberCount;
		}
	}

    return iLoginedMemberCount;
}

void KGuild::UpdateGuildMemberList( IN const std::vector< KGuildMemberInfo >& vecMemberInfo )
{
	std::vector< KGuildMemberInfo >::const_iterator vit;
	for( vit = vecMemberInfo.begin(); vit != vecMemberInfo.end(); ++vit )
	{
		InsertGuildMember( *vit );
	}

	// ��� ��� ����Ʈ�� ������Ʈ �Ǿ����� �����ѹ� �սô�!
	GuildMemberSort();

	// ��帶���� ������ ������Ʈ �� �Ǿ����� Ȯ��
	if( m_iGuildMasterUnitUID == 0 )
	{
		START_LOG( cerr, L"���� ����Ʈ�� ������Ʈ �ߴµ� ��帶���Ͱ� ����? ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( vecMemberInfo.size() )
			<< END_LOG;
	}
}

//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST
void KGuild::UpdateGuildSkill( IN const KGuildSkillInfo& kInfo )
{
    m_kSkillTree.UpdateGuildSkillTree( kInfo );
}
#endif GUILD_SKILL_TEST
//}}

bool KGuild::InsertGuildMember( IN const KGuildMemberInfo& kMemberInfo )
{
	if( IsGuildMember( kMemberInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� ���� ������ �ִ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( kMemberInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	// ���� �߰�
	KGuildMemberInfoPtr spGuildMember( new KGuildMemberInfo( kMemberInfo ) );

	m_mapGuildMember.insert( std::make_pair( kMemberInfo.m_iUnitUID, spGuildMember ) );

	// ��� ������ ���� üũ
	if( kMemberInfo.m_ucMemberShipGrade == SEnum::GUG_MASTER )
	{
		if( m_iGuildMasterUnitUID != 0  &&  
			m_iGuildMasterUnitUID != kMemberInfo.m_iUnitUID )
		{
			START_LOG( cerr, L"���� ��帶�����ε� ��� �����Ͱ� �� �ֳ�?" )
				<< BUILD_LOG( m_iGuildMasterUnitUID )
				<< BUILD_LOG( kMemberInfo.m_iUnitUID )
				<< END_LOG;
		}

		m_iGuildMasterUnitUID = kMemberInfo.m_iUnitUID;
	}

	// ���� ����Ʈ �߰�
	m_vecGuildMember.push_back( spGuildMember );
	return true;
}

bool KGuild::DeleteGuildMember( IN UidType iUnitUID )
{
	std::map< UidType, KGuildMemberInfoPtr >::iterator mit = m_mapGuildMember.find( iUnitUID );
	if( mit == m_mapGuildMember.end() )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ� �̻��մϴ�. ���� �߻��ؼ��� �ȵǴ� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		// return���� �ʽ��ϴ�. vector �����̳ʰ͵� ������ �ϰŵ��!
	}

	// map�����̳ʿ��� ���� ���� ����	
	m_mapGuildMember.erase( mit );

	// vector�����̳ʿ��� ���� ���� ����
	std::vector< KGuildMemberInfoPtr >::iterator vit;
	for( vit = m_vecGuildMember.begin(); vit != m_vecGuildMember.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->m_iUnitUID == iUnitUID )
			break;
	}

	if( vit != m_vecGuildMember.end() )
	{
		m_vecGuildMember.erase( vit );
	}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-24
	KLocalRankingUserInfo kUserInfo;
	KLocalRankingUnitInfo kUnitInfo;
	GetUserUnitInfo( iUnitUID, kUserInfo, kUnitInfo );

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker;
	for( itRanker = m_mapRankerTab.begin(); itRanker != m_mapRankerTab.end(); ++itRanker )
	{
		Remove( itRanker->second, kUserInfo, kUnitInfo );
	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

	return true;
}

bool KGuild::CheckJoinGuild( IN UidType iUnitUID, OUT UidType& iGuildMasterUnitUID )
{
	// �̹� ��������� �˻�
	if( IsGuildMember( iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� ���� ������ �ִ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_14 );
		return false;
	}
    
	// ���� ������ �ο����� üũ
	if( GetMaxMember() <= ( GetNumMember() + GetWaitingJoinNumMember() ) )
	{
        SET_ERROR( ERR_GUILD_18 );
		return false;
	}

	// ��� ������ UnitUID�� ����!
	iGuildMasterUnitUID = m_iGuildMasterUnitUID;

	// ���� ����� ����Ʈ�� ����!
    m_setWaitingForJoinUserList.insert( iUnitUID );

	SET_ERROR( NET_OK );
	return true;
}

void KGuild::RemoveReserve( IN UidType iUnitUID )
{
    m_setWaitingForJoinUserList.erase( iUnitUID );
}

bool KGuild::JoinGuildMember( IN const KGuildMemberInfo& kMemberInfo, IN bool bIsLoginUser )
{
#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( KncUid::ExtractServerGroupID( kMemberInfo.m_iUnitUID ) != GetKLoginServer()->GetServerGroupID() )
	{
		START_LOG( cerr, L"�ٸ� �������� ������ ��尡�� ��ų�� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kMemberInfo.m_iUnitUID )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
			<< END_LOG;

		return false;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// ��� ��� ���� ����
	if( InsertGuildMember( kMemberInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kMemberInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	// �α��� Ÿ�� ����
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kMemberInfo.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"���� ���� ��� ����. ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( kMemberInfo.m_iUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		return false;
	}

	// �α����� ������� �α��� ������ ����!
	if( bIsLoginUser )
	{
		spGuildMember->m_tLogInTime	= CTime::GetCurrentTime().GetTime();
		spGuildMember->m_tLogOutTime = 0;
	}
	else
	{
		spGuildMember->m_tLogInTime	= 0;
		spGuildMember->m_tLogOutTime = CTime::GetCurrentTime().GetTime();
	}

	// ���ο� ������ ���������� ��޼����� �����ѹ� �սô�!
	GuildMemberSort();

	// �α��� ��� ī��Ʈ ����
	m_iLoginedMemberCount = LoginedMemberCounting();

	// ���� ������ ��� �ٸ� �������� ���� ����� �˸���!
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType		= KEGS_GUILD_MESSAGE_NOT::MT_JOIN_MEMBER;
		kPacket.m_iUnitUID			= kMemberInfo.m_iUnitUID;
		kPacket.m_wstrNickName		= kMemberInfo.m_wstrNickName;
		kPacket.m_iMessageData		= static_cast<int>(kMemberInfo.m_ucMemberShipGrade);
		BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	}

	// ��� �޽����� �α����� �������� ������!
	if( m_kGuildInfo.m_wstrGuildMessage.empty() == false  &&  bIsLoginUser == true )
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType		= KEGS_GUILD_MESSAGE_NOT::MT_GUILD_MESSAGE;
		kPacket.m_wstrMessage		= m_kGuildInfo.m_wstrGuildMessage;
		SendToGSCharacter( kMemberInfo.m_iGSUID, kMemberInfo.m_iUnitUID, ELG_GUILD_MESSAGE_NOT, kPacket );
	}

	return true;
}

bool KGuild::KickGuildMemeber( IN UidType iUnitUID, IN UidType iTargetUnitUID )
{
	//// �ڽ��� ��� ���� ã��.
	//KGuildMemberInfoPtr spSelfInfo = GetGuildMember( iUnitUID );
	//if( spSelfInfo == NULL )
	//{
	//	START_LOG( cerr, L"�ڽ��� ������� ã�� ����.!" )
	//		<< BUILD_LOG( GetUID() )
	//		<< BUILD_LOG( iUnitUID )
	//		<< END_LOG;

	//	return false;
	//}

	// Ż���ϴ� ���� ���� ã��
	KGuildMemberInfoPtr spTargetInfo = GetGuildMember( iTargetUnitUID );
	if( spTargetInfo == NULL )
	{
		START_LOG( cerr, L"���� ���� ��� ����." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iTargetUnitUID )
			<< END_LOG;

		return false;
	}

	//switch( spSelfInfo->m_ucMemberShipGrade )
	//{
	//case SEnum::GUG_MASTER:
	//case SEnum::GUG_SYSOP:
	//	{
	//		if( spSelfInfo->m_ucMemberShipGrade >= spTargetInfo->m_ucMemberShipGrade )
	//			return false;
	//	}
	//	break;

	//default:
	//	{
	//		if( iUnitUID != iTargetUnitUID )
	//			return false;
	//	}
	//	break;
	//}

	// Ż���ڿ��Ե� �޽����� �ֱ����� �޽������� ������!
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType	= ( iUnitUID == iTargetUnitUID ? KEGS_GUILD_MESSAGE_NOT::MT_KICK_MEMBER : KEGS_GUILD_MESSAGE_NOT::MT_KICK_BY_MASTER );
	kPacket.m_iUnitUID		= iTargetUnitUID;
	kPacket.m_wstrNickName  = spTargetInfo->m_wstrNickName;
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );

	// ��� ��� ���� ����
	if( DeleteGuildMember( iTargetUnitUID ) == false )
	{
		START_LOG( cerr, L"��� ��� ���� ���� ����! �Ͼ�� ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iTargetUnitUID )
			<< END_LOG;

		return false;
	}

	// �α��� ��� ī��Ʈ ����
	m_iLoginedMemberCount = LoginedMemberCounting();
	return true;
}

bool KGuild::LoginGuildMember( IN const KGuildMemberInfo& kMemberInfo )
{
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kMemberInfo.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"���� ���� ��� ����. ���� �α��� ����!" )
			<< BUILD_LOG( kMemberInfo.m_iUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		return false;
	}

	// ��� ������
	if( kMemberInfo.m_ucMemberShipGrade == SEnum::GUG_MASTER )
	{
		m_iGuildMasterUnitUID = kMemberInfo.m_iUnitUID;
	}

	// ���� ���� ������Ʈ
	spGuildMember->m_iGSUID				= kMemberInfo.m_iGSUID;
	//spGuildMember->m_iUnitUID;								// �������� �ʽ��ϴ�.
	spGuildMember->m_wstrNickName		= kMemberInfo.m_wstrNickName;
	spGuildMember->m_cUnitClass			= kMemberInfo.m_cUnitClass;
	spGuildMember->m_ucLevel			= kMemberInfo.m_ucLevel;
	spGuildMember->m_ucMemberShipGrade	= kMemberInfo.m_ucMemberShipGrade;
	spGuildMember->m_iChannelID			= kMemberInfo.m_iChannelID;
	spGuildMember->m_iMapID				= kMemberInfo.m_iMapID;
	spGuildMember->m_cPosition			= kMemberInfo.m_cPosition;
	spGuildMember->m_tLogInTime			= CTime::GetCurrentTime().GetTime();
	spGuildMember->m_tLogOutTime		= 0;
	//spGuildMember->m_wstrMessage;								// �������� �ʽ��ϴ�.

	// ������ �α��� ������ �����ѹ� �սô�!
	GuildMemberSort();

	// �α��� ��� ī��Ʈ ����
	m_iLoginedMemberCount = LoginedMemberCounting();

	// ���� �α��� ����� �ٸ� �����鿡�� �˸���!
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType		= KEGS_GUILD_MESSAGE_NOT::MT_CONNECT_MEMBER;
		kPacket.m_iUnitUID			= kMemberInfo.m_iUnitUID;
		kPacket.m_wstrNickName		= kMemberInfo.m_wstrNickName;
		kPacket.m_iMessageData		= static_cast<int>(kMemberInfo.m_ucMemberShipGrade);
		BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	}

	// ��� �޽����� ������!
	if( m_kGuildInfo.m_wstrGuildMessage.empty() == false )
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType		= KEGS_GUILD_MESSAGE_NOT::MT_GUILD_MESSAGE;
		kPacket.m_wstrMessage		= m_kGuildInfo.m_wstrGuildMessage;
		SendToGSCharacter( kMemberInfo.m_iGSUID, kMemberInfo.m_iUnitUID, ELG_GUILD_MESSAGE_NOT, kPacket );
	}

	// �λ縻�� ������� ������ ä������ �λ縻�� ������
	if( spGuildMember->m_wstrMessage.empty() == false )
	{
		KELG_CHAT_NOT kChatMessage;
		kChatMessage.m_cChatPacketType			= KEGS_CHAT_REQ::CPT_GUILD;
		kChatMessage.m_iSenderUnitUID			= spGuildMember->m_iUnitUID;
		kChatMessage.m_wstrSenderUnitNickName	= spGuildMember->m_wstrNickName;
		kChatMessage.m_iReceiverUnitUID			= 0;
		kChatMessage.m_wstrRecieverUnitNickName = L"";
		kChatMessage.m_wstrMsg					= spGuildMember->m_wstrMessage;
		BroadCast( ELG_CHAT_NOT, kChatMessage );
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2010. 01. 19  ������	���Խ���
#ifdef SERV_GUILD_AD

	if( kMemberInfo.m_ucMemberShipGrade == SEnum::GUG_MASTER  ||
		kMemberInfo.m_ucMemberShipGrade == SEnum::GUG_SYSOP )
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType = KEGS_GUILD_MESSAGE_NOT::MT_GUILD_AD_PERIOD_INFO;
        if( SiKGuildBoardManager()->GetGuildAdEndDate( GetUID(), kPacket.m_wstrMessage ) == true )
		{
			// �Ⱓ ���ᰡ �ȵǾ� �־����.
			if( SiKGuildBoardManager()->IsExpiredGuildAd( GetUID() ) == false )
			{
				SendToGSCharacter( kMemberInfo.m_iGSUID, kMemberInfo.m_iUnitUID, ELG_GUILD_MESSAGE_NOT, kPacket );
			}			
		}
	}

#endif SERV_GUILD_AD
	//}}
	//////////////////////////////////////////////////////////////////////////	
	
	return true;
}

bool KGuild::LogoutGuildMember( IN UidType iUnitUID )
{
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"���� ���� ��� ����. ���� �α׾ƿ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	// ���� ���� ������Ʈ
	spGuildMember->m_iGSUID		 = 0;
	spGuildMember->m_iChannelID	 = 0;
	spGuildMember->m_cPosition	 = 0;
	spGuildMember->m_tLogInTime	 = 0;
	spGuildMember->m_tLogOutTime = CTime::GetCurrentTime().GetTime();

	// ������ �α׾ƿ� ������ �����ѹ� �սô�!
	GuildMemberSort();

	// �α��� ��� ī��Ʈ ����
	m_iLoginedMemberCount = LoginedMemberCounting();

	// ���� �α׾ƿ� ����� �ٸ� �����鿡�� �˸���!
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_DISCONNECT_MEMBER;
	kPacket.m_iUnitUID		= iUnitUID;
	kPacket.m_wstrNickName  = spGuildMember->m_wstrNickName;
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	return true;
}

bool KGuild::CheckInviteCondition( IN UidType iReceiverUnitUID, IN UidType iSenderUnitUID, OUT KELG_INVITE_GUILD_FIND_USER_REQ& kPacket ) const
{
	// �ʴ����� �������� ������ �ش� ������ �´��� ã��
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( iSenderUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴѵ� ����ʴ븦 �ߴ�." )
			<< BUILD_LOG( iSenderUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_07 );
		return false;
	}

	// ��� �ʴ밡 ������ ������� �˻�
	switch( spGuildMember->m_ucMemberShipGrade )
	{
	case SEnum::GUG_MASTER: 
	case SEnum::GUG_SYSOP:
	case SEnum::GUG_OLD_USER:
	case SEnum::GUG_VIP_USER:
		break;

	default:
		{
			START_LOG( cerr, L"��� �ʴ� ������ ���µ� ����ʴ븦 �ߴ�." )
				<< BUILD_LOG( iSenderUnitUID )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_12 );
			return false;
		}
	}

	// ���̻� ��� ���� �ο��� �ø��� ���°�?
	if( GetNumMember() >= KGuildInfo::GUILD_MAX_MEMBER_LIMIT )
	{
		START_LOG( clog, L"�̹� ��� �ο� �����ε� �� Ȯ���Ϸ�����" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iSenderUnitUID )			
			<< END_LOG;

		SET_ERROR( ERR_GUILD_41 );
		return false;
	}

	// ��� �ο��� ���� á���� �˾ƺ���!
	if( GetMaxMember() <= GetNumMember() )
	{
		// �ð����� �߻� ������ ������.
		START_LOG( clog, L"������ �� �߰��Ҽ� ����." )
			<< BUILD_LOG( iSenderUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_13 );
		return false;
	}

	// �̹� ��忡 ���Ե� �������� Ȯ���غ���
	if( IsGuildMember( iReceiverUnitUID ) == true )
	{
		// �ð����� ���� �߻��� �� �ִ� ������.
		START_LOG( cwarn, L"�̹� ������ ������ ����ʴ븦 �ߴ�." )
			<< BUILD_LOG( iReceiverUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_14 );
		return false;
	}

	// �ʴ�� �������� ���� ����
	kPacket.m_kGuildInfo = m_kGuildInfo;
	kPacket.m_wstrSenderNickName = spGuildMember->m_wstrNickName;
	return true;
}

void KGuild::GetGuildUserList( IN const KELG_GET_GUILD_USER_LIST_REQ& kReq, OUT KEGS_GET_GUILD_USER_LIST_ACK& kAck ) const
{
	kAck.m_iOK = NetError::NET_OK;
	kAck.m_uiViewPage = kReq.m_uiViewPage;
	kAck.m_cGuildUserListType = kReq.m_cGuildUserListType;
	kAck.m_iLoginedMemberCount = GetLoginedMemberCount();

	switch( kReq.m_cGuildUserListType )
	{
	case KEGS_GET_GUILD_USER_LIST_REQ::ULT_POSITION:
	case KEGS_GET_GUILD_USER_LIST_REQ::ULT_GRADE:
		{
            GetGuildMemberPositionGrade( kAck.m_uiViewPage, kAck.m_uiTotalPage, kAck.m_vecMemberPositionGradeList );
		}
		break;

	case KEGS_GET_GUILD_USER_LIST_REQ::ULT_MESSAGE:
		{
            GetGuildMemberMessageInfo( kAck.m_uiViewPage, kAck.m_uiTotalPage, kAck.m_vecMemberMessageList );
		}
		break;

	default:
		{
			START_LOG( cerr, L"�˼����� �����������ƮŸ���Դϴ�." )
				<< BUILD_LOGc( kReq.m_cGuildUserListType )
				<< END_LOG;
		}
		break;
	}
}

void KGuild::GetGuildMemberPositionGrade( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KGuildMemberInfo >& vecMemberList ) const
{
	vecMemberList.clear();

	_JIF( uiViewPage > 0, return; );

	if( m_vecGuildMember.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return;
	}

	uiTotalPage = m_vecGuildMember.size() / MEMBER_PAGE_PER_COUNT;
	if( ( m_vecGuildMember.size() % MEMBER_PAGE_PER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * MEMBER_PAGE_PER_COUNT ) - MEMBER_PAGE_PER_COUNT;
	if( m_vecGuildMember.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * MEMBER_PAGE_PER_COUNT ) - MEMBER_PAGE_PER_COUNT;

		if( m_vecGuildMember.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecGuildMember.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KGuildMemberInfoPtr >::const_iterator vit = m_vecGuildMember.begin();
	vit += uiBeginIndex;

	while( vit != m_vecGuildMember.end() )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"��� ��� �����Ͱ� �̻��մϴ�." )
				<< END_LOG;
			break;
		}
		
		vecMemberList.push_back( (*(*vit)) );

		if( vecMemberList.size() >= MEMBER_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

void KGuild::GetGuildMemberMessageInfo( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KGuildMemberMessageInfo >& vecMemberList ) const
{
	vecMemberList.clear();

	_JIF( uiViewPage > 0, return; );

	if( m_vecGuildMember.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return;
	}

	uiTotalPage = m_vecGuildMember.size() / MEMBER_PAGE_PER_COUNT;
	if( ( m_vecGuildMember.size() % MEMBER_PAGE_PER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * MEMBER_PAGE_PER_COUNT ) - MEMBER_PAGE_PER_COUNT;
	if( m_vecGuildMember.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * MEMBER_PAGE_PER_COUNT ) - MEMBER_PAGE_PER_COUNT;

		if( m_vecGuildMember.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecGuildMember.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KGuildMemberInfoPtr >::const_iterator vit = m_vecGuildMember.begin();
	vit += uiBeginIndex;

	while( vit != m_vecGuildMember.end() )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"��� ��� �����Ͱ� �̻��մϴ�." )
				<< END_LOG;
			break;
		}

		KGuildMemberMessageInfo kInfo;
		kInfo.m_iUnitUID		  = (*vit)->m_iUnitUID;
		kInfo.m_wstrNickName	  = (*vit)->m_wstrNickName;
		kInfo.m_ucMemberShipGrade = (*vit)->m_ucMemberShipGrade;
		kInfo.m_cUnitClass		  = (*vit)->m_cUnitClass;
		kInfo.m_ucLevel			  = (*vit)->m_ucLevel;
		kInfo.m_wstrMessage 	  = (*vit)->m_wstrMessage;
		vecMemberList.push_back( kInfo );

		if( vecMemberList.size() >= MEMBER_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

int KGuild::GetGuildGradeMemberCount( IN u_char ucMemberShipGrade ) const
{
	int iMemberCount = 0;

	std::vector< KGuildMemberInfoPtr >::const_iterator vit;
	for( vit = m_vecGuildMember.begin(); vit != m_vecGuildMember.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		// �ش� ���� �������?
		if( (*vit)->m_ucMemberShipGrade == ucMemberShipGrade )
		{
			++iMemberCount;
		}
	}

	return iMemberCount;
}

void KGuild::GuildChat( IN const KELG_CHAT_REQ& kInfo ) const
{
    KELG_CHAT_NOT kPacket;
	kPacket.m_cChatPacketType			= kInfo.m_cChatPacketType;
	kPacket.m_iSenderUnitUID			= kInfo.m_iSenderUnitUID;
	kPacket.m_wstrSenderUnitNickName	= kInfo.m_wstrSenderUnitNickName;
	kPacket.m_wstrMsg					= kInfo.m_wstrMsg;
	BroadCast( ELG_CHAT_NOT, kPacket );
}

bool KGuild::CheckChangeGradeGuild( IN const KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ& kReq, OUT KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK& kAck ) const
{
	SET_ERROR( NET_OK );

	// �̸� ���� ����!
	kAck.m_iTargetUnitUID = kReq.m_iTargetUnitUID;
	kAck.m_ucMemberShipGrade = kReq.m_ucMemberShipGrade;
	kAck.m_bChangeGuildMaster = false;	

	// 1. ��� ���� ��û��
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"��� ����� �ƴմϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// 2. ��� ���� �����
	KGuildMemberInfoPtr spTargetMember = GetGuildMember( kReq.m_iTargetUnitUID );
	if( spTargetMember == NULL )
	{
		START_LOG( cerr, L"��� ����� �ƴմϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iTargetUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// 3. ���� ������� ��ް� �����Ϸ��� ��� ��
	if( spTargetMember->m_ucMemberShipGrade == kReq.m_ucMemberShipGrade )
	{
		START_LOG( cerr, L"�̹� ����� ����Դϴ�." )			
			<< BUILD_LOG( kReq.m_iTargetUnitUID )
			<< BUILD_LOG( spTargetMember->m_ucMemberShipGrade )
			<< BUILD_LOGc( kReq.m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_33 );
		return false;
	}

	// 4. ��� ��� ���� ���� �˻�
	if( spGuildMember->m_ucMemberShipGrade == SEnum::GUG_MASTER )
	{
		// 4-1. ��� �����Ͱ� �ٸ� �������� ��� ������ ����!
		if( kReq.m_ucMemberShipGrade == SEnum::GUG_MASTER )
		{
			if( spTargetMember->m_ucMemberShipGrade == SEnum::GUG_MASTER )
			{
				START_LOG( cerr, L"�ѱ�忡 ��帶���Ͱ� 2���ΰ�? ������ ���¿���!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( kReq.m_iTargetUnitUID )
					<< END_LOG;

				SET_ERROR( ERR_GUILD_19 );
				return false;
			}

			// ��� ������ ���� üũ!
			kAck.m_bChangeGuildMaster = true;
		}
		// 4-2. ��� �����Ͱ� ��� ������ �Ӹ�!
		else if( kReq.m_ucMemberShipGrade == SEnum::GUG_SYSOP )
		{
			// ���� ��� ������ �� ���
			const int iGuildSysopCount = GetGuildGradeMemberCount( static_cast<u_char>(SEnum::GUG_SYSOP) );
			int iGuildSysopMaxCount = 0;
			if( GetNumMember() <= 10 )
			{
				iGuildSysopMaxCount = 1;
			}
			else
			{
				iGuildSysopMaxCount = ( ( GetNumMember() - 1 ) / 10 ) + 1; // ��� �����ڴ� �������� 1~10����� 1������ �����ؼ� 10��� 1������ �Ӹ���
			}

			if( iGuildSysopMaxCount <= iGuildSysopCount )
			{
				// ��� ������� ���ڶ� �������ڸ� �Ӹ��� �� �����ϴ�.
				SET_ERROR( ERR_GUILD_34 );
				return false;
			}
		}
	}
	else if( spGuildMember->m_ucMemberShipGrade == SEnum::GUG_SYSOP )
	{
		// ��� �����ڰ� ��� ������ �Ǵ� �ٸ� ���������� ����� ������ ���� ����.
		if( spTargetMember->m_ucMemberShipGrade == SEnum::GUG_MASTER  ||
			spTargetMember->m_ucMemberShipGrade == SEnum::GUG_SYSOP )
		{
			START_LOG( cerr, L"��� �����ڰ� ��� ������ �Ǵ� �ٸ� ���������� ����� �����Ϸ��� �ϳ�?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_iTargetUnitUID )
				<< BUILD_LOGc( spTargetMember->m_ucMemberShipGrade )
				<< BUILD_LOGc( kReq.m_ucMemberShipGrade )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_20 ); // ���� ��޺��� ������ �����ϴ�.
			return false;
		}

		switch( kReq.m_ucMemberShipGrade )
		{
		case SEnum::GUG_OLD_USER:
		case SEnum::GUG_VIP_USER:
		case SEnum::GUG_NORMAL_USER:
		case SEnum::GUG_NEW_USER:
			break;

		default:
			{
				START_LOG( cerr, L"�����Ϸ��� ��� ������ �̻��ϴ�!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOGc( kReq.m_ucMemberShipGrade )
					<< END_LOG;

				SET_ERROR( ERR_GUILD_20 ); // ������ �ʴ� ��޺����Դϴ�.
				return false;
			}
		}
	}
	else
	{
		START_LOG( cerr, L"��� ������ �Ǵ� ��� �����ڰ� �ƴѵ� ������޺����� �Ϸ��� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_20 );
		return false;
	}

	// ��� ���� ������� �г��� ����
	kAck.m_wstrTargetNickName = spTargetMember->m_wstrNickName;
	return true;
}

bool KGuild::UpdateChangeGuildMemberGrade( IN const KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK& kInfo )
{
	// ��� ������ ������ ���
	if( kInfo.m_bChangeGuildMaster )
	{
		const UidType iOldGuildMasterUnitUID = m_iGuildMasterUnitUID;

		KGuildMemberInfoPtr spGuildMaster = GetGuildMember( m_iGuildMasterUnitUID );
		if( spGuildMaster == NULL )
		{
			START_LOG( cerr, L"��� ������ ���� ��� ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_iGuildMasterUnitUID )
				<< END_LOG;

			return false;
		}

		// �Ϲ� �������� ��� �϶�
		spGuildMaster->m_ucMemberShipGrade = SEnum::GUG_NORMAL_USER;

		// ���� �Ӹ�� ���� UnitUID�� ��� ������ UnitUID�� ����
		m_iGuildMasterUnitUID = kInfo.m_iTargetUnitUID;

		// ��帶������ ���� ����� �ٸ� �����鿡�� �˸���!
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_MEMBER_GRADE;
		kPacket.m_iUnitUID		= iOldGuildMasterUnitUID;
		kPacket.m_wstrNickName	= spGuildMaster->m_wstrNickName;
		kPacket.m_iMessageData	= static_cast<int>(spGuildMaster->m_ucMemberShipGrade);
		BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	}

	// ��� ����� ���� ã��
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kInfo.m_iTargetUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kInfo.m_iTargetUnitUID )
			<< END_LOG;

		return false;
	}

	// ���� ��� ���� ������Ʈ
	spGuildMember->m_ucMemberShipGrade = kInfo.m_ucMemberShipGrade;

	// ���� ����� �ٲ������ ��޼����� �����ѹ� �սô�!
	GuildMemberSort();

	// ���� ��� ���� ����� �ٸ� �����鿡�� �˸���!
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_MEMBER_GRADE;
	kPacket.m_iUnitUID		= kInfo.m_iTargetUnitUID;
	kPacket.m_wstrNickName	= spGuildMember->m_wstrNickName;
	kPacket.m_iMessageData	= static_cast<int>(kInfo.m_ucMemberShipGrade);
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );

	// ��� ������ ������ ��� ��� ������ �������ݽô�
	if( kInfo.m_bChangeGuildMaster )
	{
		KEGS_GUILD_INFO_NOT kPacketNot;
		//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
		GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID, kPacketNot.m_kGuildSkillInfo );
#else
		GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID );
#endif GUILD_SKILL_TEST
		//}}
		BroadCast( ELG_GUILD_INFO_NOT, kPacketNot );
	}

	return true;
}

void KGuild::UpdateChangeGuildMessage( IN const KDBE_CHANGE_GUILD_MESSAGE_ACK& kInfo )
{
	// �޽��� ����
    m_kGuildInfo.m_wstrGuildMessage = kInfo.m_wstrMessage;

	// ��� �������� ������� ������ �˸���!
	{
		KEGS_GUILD_INFO_NOT kPacketNot;
		//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
		GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID, kPacketNot.m_kGuildSkillInfo );
#else
		GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID );
#endif GUILD_SKILL_TEST
		//}}
		BroadCast( ELG_GUILD_INFO_NOT, kPacketNot );
	}

	// ����� ��� �޽����� ������!
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType		= KEGS_GUILD_MESSAGE_NOT::MT_GUILD_MESSAGE;
		kPacket.m_wstrMessage		= m_kGuildInfo.m_wstrGuildMessage;
		BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	}
}

bool KGuild::UpdateChangeGuildMemberMessage( IN const KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK& kInfo )
{
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kInfo.m_iTargetUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kInfo.m_iTargetUnitUID )
			<< END_LOG;

		return false;
	}

	// ����� �޽��� ������Ʈ!
	spGuildMember->m_wstrMessage = kInfo.m_wstrMessage;
    
	// ���� �λ縻 ���� ����� �ٸ� �����鿡�� �˸���!
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_MEMBER_MESSAGE;
	kPacket.m_iUnitUID		= kInfo.m_iTargetUnitUID;
	kPacket.m_wstrNickName  = spGuildMember->m_wstrNickName;
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	return true;
}

// ��� ��� ���� ����
bool KGuild::UpdateGuildMemberInfo( IN const KGuildMemberInfo& kInfo )
{
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kInfo.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		// �ð����� �߻� ������ ���� �Դϴ�.
		START_LOG( cwarn, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}
	
	const bool bIsChangeLevel = ( spGuildMember->m_ucLevel != kInfo.m_ucLevel );

	// ����� ���� ������Ʈ!	
	spGuildMember->m_cUnitClass	  = kInfo.m_cUnitClass;
	spGuildMember->m_ucLevel	  = kInfo.m_ucLevel;
	spGuildMember->m_iMapID		  = kInfo.m_iMapID;
	spGuildMember->m_cPosition	  = kInfo.m_cPosition;

	// ����� �����߿� ���������� ������ �����ѹ�����!
	if( bIsChangeLevel )
	{
		// ���� ������ �ٲ������ ��޼����� �����ѹ� �սô�!
		GuildMemberSort();
	}

	// ���� ��� ���� ���� ����� �ٸ� �����鿡�� �˸���!
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_UPDATE_MEMBER_INFO;
	kPacket.m_iUnitUID		= kInfo.m_iUnitUID;
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	return true;
}

bool KGuild::CheckGuildMaxMember( IN const KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ& kInfo )
{
	SET_ERROR( NET_OK );

	// ��� �Ҽ��ΰ�?
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kInfo.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ������ �Ǵ� ��� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER  &&
		spGuildMember->m_ucMemberShipGrade != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"��� ������ �Ǵ� �����ڰ� �ƴѵ� ��� �ο� Ȯ���� �Ϸ� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_40 );
		return false;
	}

	// ��� �ο� Ȯ�� ������ �Ǵ°�?
	if( GetMaxMember() >= KGuildInfo::GUILD_MAX_MEMBER_LIMIT )
	{
		START_LOG( cwarn, L"�̹� ��� �ο� �����ε� �� Ȯ���Ϸ�����" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( GetMaxMember() )
			<< END_LOG;
		
		SET_ERROR( ERR_GUILD_41 );
		return false;
	}
	
	return true;
}

void KGuild::UpdateGuildMaxMember( IN u_short usMaxNumMember )
{
	// ��� �ִ� �ο� ������Ʈ
	m_kGuildInfo.m_usMaxNumMember = usMaxNumMember;
	LIF( m_kGuildInfo.m_usMaxNumMember <= KGuildInfo::GUILD_MAX_MEMBER_LIMIT  );

	// ����� ��� ������ ��� �������� �˸���!
	KEGS_GUILD_INFO_NOT kPacketNot;
	//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID, kPacketNot.m_kGuildSkillInfo );
#else
	GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID );
#endif GUILD_SKILL_TEST
	//}}
	BroadCast( ELG_GUILD_INFO_NOT, kPacketNot );

	// ��� �ִ� �ο� ���� �˸���
	{
		KEGS_GUILD_MESSAGE_NOT kPacket;
		kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_EXPAND_MAX_MEMBER;
		kPacket.m_iMessageData = GetMaxMember();
		BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	}
}

void KGuild::UpdateGuildEXP( IN const KDBE_UPDATE_GUILD_EXP_ACK& kInfo )
{
	// ��� �� ����Ʈ ������Ʈ
	m_kGuildInfo.m_iGuildEXP = kInfo.m_iGuildEXP;

	// ��� �� ����Ʈ ���� �˸���
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType	= KEGS_GUILD_MESSAGE_NOT::MT_GUILD_EXP;
	kPacket.m_iUnitUID = kInfo.m_iUnitUID;
	kPacket.m_iMessageData = m_kGuildInfo.m_iGuildEXP;
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );

	// ��� ������ üũ
	CheckGuildLevelUp();
}

void KGuild::GuildMemberSort()
{
	// [����] ��ü�� �����ϱ� ���ؼ��� std::stable_sort()�Լ��� �����.

	std::stable_sort( m_vecGuildMember.begin(), m_vecGuildMember.end(), &KGuild::GuildMemberGradeSort );
}

bool KGuild::GuildMemberGradeSort( const KGuildMemberInfoPtr spFirst, const KGuildMemberInfoPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. �������� ������ ����!
	if( spFirst->IsLogined()  &&  !spSecond->IsLogined() )
	{
		return true;
	}
	// �¶��� ����
	else if( spFirst->IsLogined()  &&  spSecond->IsLogined() )
	{
		// 2. ��� ����� ���� ��!
		if( spFirst->m_ucMemberShipGrade < spSecond->m_ucMemberShipGrade )
		{
			return true;
		}
		else if( spFirst->m_ucMemberShipGrade == spSecond->m_ucMemberShipGrade )
		{
			// 3. ĳ���� ������ ���� ��!
			if( spFirst->m_ucLevel > spSecond->m_ucLevel )
			{
				return true;
			}
			else if( spFirst->m_ucLevel == spSecond->m_ucLevel )
			{
				// 4. ���� ������ ���� ��!
				if( spFirst->m_tLogInTime > spSecond->m_tLogInTime )
				{
					return true;
				}
			}
		}
	}
	// �������� ����
	else if( !spFirst->IsLogined()  &&  !spSecond->IsLogined() )
	{
		// 2. �ֱ� ���� ������ �������
		if( spFirst->m_tLogOutTime > spSecond->m_tLogOutTime )
		{
			return true;
		}
	}

	return false;
}

void KGuild::CheckGuildLevelUp()
{
	// ��� ������ ���� üũ
	u_char ucNowLevel = GetGuildLevel();

	m_kGuildInfo.m_ucGuildLevel = SiCXSLGuildManager()->CheckGuildLevelUp( ucNowLevel, m_kGuildInfo.m_iGuildEXP );

	if( ucNowLevel < m_kGuildInfo.m_ucGuildLevel )
	{
		// ��� ������ �ߴ�! GameDB�� ������ �Ϸ� ����!
		KDBE_GUILD_LEVEL_UP_REQ kPacket;
		kPacket.m_iGuildUID = GetUID();
		kPacket.m_wstrGuildName = m_kGuildInfo.m_wstrGuildName;
		kPacket.m_ucGuildLevel = GetGuildLevel();
		//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
		kPacket.m_iGuildSPoint = m_kSkillTree.GetGuildSPoint() + 1; // 1�������Ѽ� ������Ʈ�Ѵ�.
#endif GUILD_SKILL_TEST
		//}}

		if( CXSLGuildManager::IsValidSkillPointFigure( kPacket.m_iGuildSPoint ) == false )
		{
			START_LOG( cerr, L"��ų ����Ʈ ���� ������ �̻��մϴ�." )
				<< BUILD_LOG( kPacket.m_iGuildUID )
				<< BUILD_LOG( kPacket.m_wstrGuildName )
				<< BUILD_LOG( kPacket.m_iGuildSPoint )
				<< END_LOG;
		}

		SendToGameDB( DBE_GUILD_LEVEL_UP_REQ, kPacket );
	}
}

void KGuild::UpdateGuildLevelResult( IN const KDBE_GUILD_LEVEL_UP_ACK& kInfo )
{
	if( kInfo.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��� ���� �� ��ų����Ʈ DB������Ʈ ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOGc( kInfo.m_ucGuildLevel )
			//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
			<< BUILD_LOG( kInfo.m_iGuildSPoint )
#endif GUILD_SKILL_TEST
			//}}
			<< END_LOG;

		return;
	}

	//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	// ��� ��ų ���� ������Ʈ
	m_kSkillTree.SetGuildSPoint( kInfo.m_iGuildSPoint );
#endif GUILD_SKILL_TEST
	//}}

	// ��� ������ �������� �˸���!
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType = KEGS_GUILD_MESSAGE_NOT::MT_GUILD_LEVEL_UP;
	kPacket.m_iMessageData = static_cast<int>(GetGuildLevel());
	//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	kPacket.m_iMessageData2 = m_kSkillTree.GetGuildSPoint();
#endif GUILD_SKILL_TEST
	//}}
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
}

#endif GUILD_TEST

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST

bool KGuild::CheckLearnGuildSkill( IN const KELG_GET_GUILD_SKILL_REQ& kReq, OUT KDBE_GET_GUILD_SKILL_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// ��� �Ҽ��ΰ�?
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"��� ������ �Ǵ� �����ڰ� �ƴѵ� ��� ��ų�� ���� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_00 );
		return false;
	}
	
	//////////////////////////////////////////////////////////////////////////
	
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	// �̹� �ְ������� ��� ��ų���� �˻�
	if( m_kSkillTree.IsMasterSkillLevel( kReq.m_iGuildSkillID ) == true )
	{
		START_LOG( cerr, L"�̹� �ְ� �������� ��� ��� ��ų�Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_01 );
		return false;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kReq.m_iGuildSkillID );
	if( pSkillTemplet == NULL )
	{
		START_LOG( cerr, L"��ų ���ø� ������ �����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	// ���ེų�� �ְ������� �����ߴ��� üũ
	if( m_kSkillTree.IsAllPrecedingSkillMasterLevel( kReq.m_iGuildSkillID ) == false )
	{
		START_LOG( cerr, L"���ེų�� ���� �����ͷ������� �������� ���߽��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_03 );
		return false;
	}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	// �̹� �ְ������� ��� ��ų���� �˻�
	if( m_kSkillTree.IsMaxSkillLevel( kReq.m_iGuildSkillID ) == true )
	{
		START_LOG( cerr, L"�̹� �ְ� �������� ��� ��� ��ų�Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_01 );
		return false;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kReq.m_iGuildSkillID, 1 );
	if( pSkillTemplet == NULL )
	{
		START_LOG( cerr, L"��ų ���ø� ������ �����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	// ���ེų�� �ְ������� �����ߴ��� üũ
	if( m_kSkillTree.IsAllPrecedingSkillMaxLevel( kReq.m_iGuildSkillID ) == false )
	{
		START_LOG( cerr, L"���ེų�� ���� �ְ������� �������� ���߽��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_03 );
		return false;
	}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	// ���� tier�� ����� SP�� �Һ��ߴ��� üũ
	if( m_kSkillTree.IsTierOpened( kReq.m_iGuildSkillID ) == false )
	{
		START_LOG( cerr, L"���� tire���� ����� SP�� �Һ����� �ʾҽ��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_04 );
		return false;
	}

	// ����� SP Ȥ�� CSP�� ������ �ִ��� Ȯ��
	if( m_kSkillTree.IsCashSkillPointExpired() )
	{
		if( m_kSkillTree.GetGuildSPoint() <= 0 )
		{
			SET_ERROR( ERR_GUILD_SKILL_05 );
			return false;
		}
	}
	else
	{
		if( ( m_kSkillTree.GetGuildSPoint() + m_kSkillTree.GetGuildCSPoint() ) <= 0 )
		{
			SET_ERROR( ERR_GUILD_SKILL_05 );
			return false;
		}
	}

	int iSkillLevel = 0;
	int iSkillCSPoint = 0;
	if( m_kSkillTree.GetSkillLevelAndCSP( kReq.m_iGuildSkillID, iSkillLevel, iSkillCSPoint ) == false )
	{
		iSkillLevel = 0;
		iSkillCSPoint = 0;
	}

	// DB�� ������Ʈ�� ����
	kPacketToDB.m_iGuildUID			= GetUID();
	kPacketToDB.m_iUnitUID			= kReq.m_iUnitUID;
	kPacketToDB.m_iGuildSkillID		= kReq.m_iGuildSkillID;
	kPacketToDB.m_iGuildSkillLevel	= iSkillLevel + 1;
	kPacketToDB.m_iMaxGuildCSPoint	= m_kSkillTree.GetMaxGuildCSPoint();

	// �Ⱓ�� ��ų����Ʈ�� ȹ���� �������� �˻�
	if( m_kSkillTree.IsCashSkillPointExpired() == true )
	{
		kPacketToDB.m_iGuildSkillCSPoint	= iSkillCSPoint;

		kPacketToDB.m_iGuildCSPoint			= -1; // invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
		kPacketToDB.m_iGuildSPoint			= m_kSkillTree.GetGuildSPoint() - 1;
	}
	else
	{
		// �Ⱓ�� ��ų ����Ʈ�� ȹ���� ���¶�� ���ݺ��� ������ ��ų����Ʈ�� �Ⱓ�� ��ų����Ʈ�� �����Ѵ�.
		kPacketToDB.m_iGuildSkillCSPoint	= iSkillCSPoint + 1;

		if( m_kSkillTree.GetGuildCSPoint() > 0 )
		{
			kPacketToDB.m_iGuildCSPoint = m_kSkillTree.GetGuildCSPoint() - 1;
			kPacketToDB.m_iGuildSPoint	= m_kSkillTree.GetGuildSPoint();
		}
		else
		{
			kPacketToDB.m_iGuildCSPoint	= -1; // invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
			kPacketToDB.m_iGuildSPoint	= m_kSkillTree.GetGuildSPoint() - 1;
		}
	}

	//////////////////////////////////////////////////////////////////////////	

	return true;
}

bool KGuild::InsertGuildSkill( IN const KDBE_GET_GUILD_SKILL_ACK& kAck )
{
	//��ų����� �Ҹ��sp ���ش�.
	if( kAck.m_iGuildCSPoint >= 0 )
	{
		// CSPoint�� ����ؼ� ��ų�� ȹ���� ���
		m_kSkillTree.SetGuildCSPoint( kAck.m_iGuildCSPoint );
	}
	else
	{
		// SPoint �� ����ؼ� ��ų ȹ���� ���
		m_kSkillTree.SetGuildSPoint( kAck.m_iGuildSPoint );
	}

	if( m_kSkillTree.SetSkillLevelAndCSP( kAck.m_iGuildSkillID, kAck.m_iGuildSkillLevel, kAck.m_iGuildSkillCSPoint ) == false )
		return false;

	// ��ų�� ���鼭 ����� ������ �����͵� �ٽ� �Ѱ��ش�.
	KEGS_GET_GUILD_SKILL_NOT kPacketNot;
	kPacketNot.m_iGuildSkillID		= kAck.m_iGuildSkillID;
	kPacketNot.m_iGuildSkillLv		= kAck.m_iGuildSkillLevel;
	kPacketNot.m_iGuildSkillCSPoint	= kAck.m_iGuildSkillCSPoint;
	kPacketNot.m_iGuildSPoint		= m_kSkillTree.GetGuildSPoint();
	kPacketNot.m_iGuildCSPoint		= m_kSkillTree.GetGuildCSPoint();
	BroadCast( ELG_GET_GUILD_SKILL_NOT, kPacketNot );
	return true;
}

bool KGuild::CheckResetGuildSkill( IN const KELG_RESET_GUILD_SKILL_REQ& kReq, OUT KDBE_RESET_GUILD_SKILL_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// ��� �Ҽ��ΰ�?
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"��� ������ �Ǵ� �����ڰ� �ƴѵ� ��� ��ų �ǵ����⸦ �Ϸ� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_07 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	// �ʱ�ȭ ���� ���
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kReq.m_iGuildSkillID );
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( kReq.m_iGuildSkillID, 1 );
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	if( pSkillTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	// �ʱ�ȭ ��ų �� ���� ��ŭ ��ų ������ ������  �˻�
	int iSkillLevel = 0;
	int iSkillCSPoint = 0;
	if( m_kSkillTree.GetSkillLevelAndCSP( kReq.m_iGuildSkillID, iSkillLevel, iSkillCSPoint ) == false )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ��ų �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	if( iSkillLevel < 1 )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ��ų �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	// ���� ��� ���� �˻�
	if( m_kSkillTree.IsAllFollowingSkillLevelZero( kReq.m_iGuildSkillID ) == false )
	{
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ���ܳ�� �˻縦 �����ٵ�.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_09 );
		return false;
	}

	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( kReq.m_iGuildSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"Ʈ���� �������� �ʴ� ��彺ųID�� �ǵ����� �Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	// ��ų �ǵ����⸦ �ϰ� �� �Ŀ� ���� tier�� ��ų�� �� ��� �Ǵ� ��� ��ų ������ �� �� ����
	std::vector< int > vecCumulativeTierSPoint;
	m_kSkillTree.CalcCumulativeUsedSPointOnEachTier( vecCumulativeTierSPoint );
	if( pSkillTreeTemplet->m_iTier < 0 ||
		pSkillTreeTemplet->m_iTier > (int)vecCumulativeTierSPoint.size() - 1 )
	{
		START_LOG( cerr, L"��ų ���ø��� tier index�� �̻��մϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( pSkillTreeTemplet->m_iTier )
			<< BUILD_LOG( kReq.m_iGuildSkillID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_02 );
		return false;
	}

	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	for( u_int ui = pSkillTreeTemplet->m_iTier + 1; ui < vecCumulativeTierSPoint.size(); ++ui )
	{
		if( vecCumulativeTierSPoint[ui-1]-1 < (int)ui * MAGIC_SKILL_POINT_PER_TIER )
		{
			START_LOG( cerr, L"���� tier�� �ִ� ��ųID�� �ʱ�ȭ �Ϸ� �߽��ϴ�." )
				<< BUILD_LOG( GetUID() )				
				<< BUILD_LOG( pSkillTreeTemplet->m_iTier )
				<< BUILD_LOG( ui )
				<< BUILD_LOG( vecCumulativeTierSPoint[ui-1] )
				<< BUILD_LOG( kReq.m_iGuildSkillID )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_SKILL_02 );
			return false;
		}
	}

	// DB�� ����
	kPacketToDB.m_iGuildUID = GetUID();
	kPacketToDB.m_iUnitUID = kReq.m_iUnitUID;
	kPacketToDB.m_iGuildSkillID = kReq.m_iGuildSkillID;
	kPacketToDB.m_iGuildSkillLevel = iSkillLevel - 1;
	kPacketToDB.m_iMaxGuildCSPoint = m_kSkillTree.GetMaxGuildCSPoint();

	if( iSkillCSPoint > 0 )
	{
		kPacketToDB.m_iGuildSkillCSPoint = iSkillCSPoint - 1;
	}
	else
	{
		kPacketToDB.m_iGuildSkillCSPoint = iSkillCSPoint;
	}

	if( m_kSkillTree.IsCashSkillPointExpired() == false )
	{
		// Cash skill point�� ��ȿ�� �Ⱓ�̶�� CSP�� �����ְ�, �ƴ϶�� SP�� �����ش�
		kPacketToDB.m_iGuildCSPoint = m_kSkillTree.GetGuildCSPoint() + 1;
		kPacketToDB.m_iGuildSPoint	= m_kSkillTree.GetGuildSPoint();
	}
	else
	{
		kPacketToDB.m_iGuildCSPoint = -1; // invalid �� ���� ������ SP�� ����ٴ� ���� �����Ѵ�
		kPacketToDB.m_iGuildSPoint	= m_kSkillTree.GetGuildSPoint() + 1;
	}
	
	return true;
}

void KGuild::ResetGuildSkill( IN const KDBE_RESET_GUILD_SKILL_ACK& kAck )
{
	// ��ų ����Ʈ �߰�
	if( kAck.m_iGuildCSPoint >= 0 )
	{
		m_kSkillTree.SetGuildCSPoint( kAck.m_iGuildCSPoint );
	}
	else
	{
		m_kSkillTree.SetGuildSPoint( kAck.m_iGuildSPoint );
	}

	// ��� ��ų �ǵ�����
	m_kSkillTree.SetSkillLevelAndCSP( kAck.m_iGuildSkillID, kAck.m_iGuildSkillLevel, kAck.m_iGuildSkillCSPoint );

	// ��� ��ų �ǵ����� ����� �����鿡�� �˷�����!
	KEGS_RESET_GUILD_SKILL_NOT kPacketNot;
	kPacketNot.m_iDelGuildSkillID = kAck.m_iGuildSkillID;
	if( kAck.m_iGuildCSPoint >= 0 )
	{
		kPacketNot.m_iRestoreGuildSPoint	   = 0;
		kPacketNot.m_iRestoreGuildCSPoint	   = 1;
	}
	else
	{
		kPacketNot.m_iRestoreGuildSPoint	   = 1;
		kPacketNot.m_iRestoreGuildCSPoint	   = 0;
	}
	BroadCast( ELG_RESET_GUILD_SKILL_NOT, kPacketNot );
}

bool KGuild::CheckInitGuildSkill( IN const KELG_INIT_GUILD_SKILL_REQ& kReq, OUT KDBE_INIT_GUILD_SKILL_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// ��� �Ҽ��ΰ�?
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"��� ������ �Ǵ� �����ڰ� �ƴѵ� ��� ��ų �ʱ�ȭ�� �Ϸ� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_11 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	// ��� ��ų�� ���ٸ�..
	int iUsedSPoint = 0;
	int iUsedCSPoint = 0;
	m_kSkillTree.CalcUsedSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );

	if( ( iUsedSPoint <= 0 && iUsedCSPoint <= 0 )  ||  ( iUsedSPoint + iUsedCSPoint <= 0 ) )
	{
		START_LOG( cwarn, L"��ų�� �ϳ��� ����� �ʾҰų� �⺻ ��ų�� �ִ� �����ε�( ��, ���̻� �ʱ�ȭ �� �ʿ䰡 ���µ�) �ʱ�ȭ �Ϸ��� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( iUsedSPoint )
			<< BUILD_LOG( iUsedCSPoint )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_13 );
		return false;
	}
	
	int iRetrievedSPoint = 0 ;	// �������� SP
	int iRetrievedCSPoint = 0;	// �������� CSP

	if( m_kSkillTree.IsCashSkillPointExpired() == false )
	{
		iRetrievedSPoint	= iUsedSPoint + iUsedCSPoint + m_kSkillTree.GetGuildCSPoint() - m_kSkillTree.GetMaxGuildCSPoint();
		iRetrievedCSPoint	= m_kSkillTree.GetMaxGuildCSPoint();
	}
	else
	{
		if( 0 == iUsedCSPoint  &&  0 == m_kSkillTree.GetGuildCSPoint() )
		{
			iRetrievedSPoint	= iUsedSPoint;
			iRetrievedCSPoint	= 0;
		}
		else
		{
			START_LOG( cerr, L"cash skill point������ �ٵǾ��µ� ��ųƮ���� csp�� �����ִ�." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( iUsedSPoint )
				<< BUILD_LOG( iUsedCSPoint )
				<< BUILD_LOG( m_kSkillTree.GetGuildCSPoint() )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_SKILL_13 );
			return false;
		}
	}

	// DB�� ������Ʈ�� �����͸� ����!
	kPacketToDB.m_iGuildUID		= GetUID();
	kPacketToDB.m_iUnitUID		= kReq.m_iUnitUID;
	kPacketToDB.m_iItemUID		= kReq.m_iItemUID;
	kPacketToDB.m_iGuildSPoint	= m_kSkillTree.GetGuildSPoint() + iRetrievedSPoint;
	kPacketToDB.m_iGuildCSPoint	= iRetrievedCSPoint;	
	return true;    
}

void KGuild::InitGuildSkill( IN const KDBE_INIT_GUILD_SKILL_ACK& kAck )
{
	// ��ų����Ʈ ���� ����
	m_kSkillTree.SetGuildSPoint( kAck.m_iGuildSPoint );
	m_kSkillTree.SetGuildCSPoint( kAck.m_iGuildCSPoint );

	// ��ų ����
	m_kSkillTree.ResetGuildSkillTree();

    // ��� �����鿡�� ��� ��ų �ʱ�ȭ �˷��ֱ�
	KEGS_INIT_GUILD_SKILL_NOT kPacketNot;
	kPacketNot.m_iGuildSPoint = m_kSkillTree.GetGuildSPoint();
	kPacketNot.m_iGuildCSPoint = m_kSkillTree.GetGuildCSPoint();
	BroadCast( ELG_INIT_GUILD_SKILL_NOT, kPacketNot );
}

bool KGuild::InsertGuildCashSkillPoint( IN const KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT& kNot )
{
	SET_ERROR( NET_OK );

	// ��� �Ҽ��ΰ�?
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kNot.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kNot.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"��� ������ �Ǵ� �����ڰ� �ƴѵ� ��� ��ų �ǵ����⸦ �Ϸ� ��." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kNot.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_15 );
		return false;
	}

	// �̹� �׳�ý� ���ູ �������� ������̶�� �ٽ� ����� �� ����.
	if( m_kSkillTree.IsCashSkillPointExpired() == false )
	{
		START_LOG( cerr, L"�̹� �׳�ý� ���ູ�� ������̸� �ٽ� ������. üũ�����ٵ�.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kNot.m_iUnitUID )			
			<< END_LOG;
		
		SET_ERROR( ERR_GUILD_SKILL_17 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	
	KDBE_INSERT_GUILD_CASH_SKILL_POINT_REQ kPacket;
	kPacket.m_iGuildUID = GetUID();
	kPacket.m_iGuildCSPoint = kNot.m_iGuildCSPoint;
	kPacket.m_iPeriod = kNot.m_iPeriod;
	kPacket.m_bUpdateEndDateOnly = ( m_kSkillTree.IsCashSkillPointExpired() == true ? false : true );
	SendToGameDB( DBE_INSERT_GUILD_CASH_SKILL_POINT_REQ, kPacket );
	return true;
}

void KGuild::InsertGuildCashSkillPointResult( IN const KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK& kAck )
{
	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��� ĳ�� ��ų ����Ʈ insert DB������Ʈ ����!" )
			<< BUILD_LOG( kAck.m_iGuildCSPoint )
			<< BUILD_LOG( kAck.m_iPeriod )
			<< BUILD_LOG( kAck.m_bUpdateEndDateOnly )
			<< END_LOG;
		return;
	}

	// �Ⱓ ������ �ƴ϶��..
	if( kAck.m_bUpdateEndDateOnly == false )
	{
		m_kSkillTree.SetGuildCSPoint( kAck.m_iGuildCSPoint );
		m_kSkillTree.SetMaxGuildCSPoint( kAck.m_iGuildCSPoint );
	}
	m_kSkillTree.SetGuildCSPointEndDate( kAck.m_wstrEndDate );

	// �����鿡�� �˸���!
	{
		KEGS_GUILD_MESSAGE_NOT kPacketNot;
		kPacketNot.m_cMessageType = KEGS_GUILD_MESSAGE_NOT::MT_INSERT_GUILD_CSPOINT;
		kPacketNot.m_iMessageData = m_kSkillTree.GetGuildCSPoint();
		kPacketNot.m_iMessageData2 = m_kSkillTree.GetMaxGuildCSPoint();
		kPacketNot.m_wstrMessage = kAck.m_wstrEndDate;
        BroadCast( ELG_GUILD_MESSAGE_NOT, kPacketNot );
	}
}

void KGuild::ExpireGuildCashSkillPointResult( IN const KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK& kAck )
{
	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��� ĳ�� ��ų ����Ʈ �Ⱓ���� DB������Ʈ ����!" )
			<< BUILD_LOG( kAck.m_iRetrievedSPoint )
			<< END_LOG;
		return;
	}

	// ��ų ���� ����
	KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT kPacketNot;

	m_kSkillTree.SetGuildSPoint( m_kSkillTree.GetGuildSPoint() + kAck.m_iRetrievedSPoint );
	m_kSkillTree.ExpireCashSkillPoint( kPacketNot.m_vecGuildSkillData );

	// ��� ��ų ���� ���� ����
	kPacketNot.m_iGuildSPoint = m_kSkillTree.GetGuildSPoint();
	kPacketNot.m_iGuildCSPoint = m_kSkillTree.GetGuildCSPoint();
	BroadCast( ELG_EXPIRE_GUILD_CASH_SKILL_POINT_NOT, kPacketNot );
}

bool KGuild::AdminInsertGuildSkillPoint( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ& kReq )
{
	SET_ERROR( NET_OK );

	// ��� �Ҽ��ΰ�?
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cwarn, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cwarn, L"��� �����Ͱ� �ƴѵ� ��� ��ų ����Ʈ ġƮ�� ����Ϸ���." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_SKILL_00 );
		return false;
	}
    
	// DB�� ������Ʈ�Ϸ� ����!
    SendToGameDB( DBE_ADMIN_GET_GUILD_SKILL_POINT_REQ, kReq );
	return true;
}

void KGuild::AdminInsertGuildSkillPointResult( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK& kAck )
{
    // ġƮ ��� ������Ʈ
	m_kSkillTree.SetGuildSPoint( kAck.m_iGuildSPoint );

	// ġƮ ��� ��Ŷ ����
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kAck.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kAck.m_iUnitUID )
			<< END_LOG;
		
		return;
	}

	if( spGuildMember->IsLogined() == false )
	{
		START_LOG( cerr, L"�α��� ���°� �ƴմϴ�" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kAck.m_iUnitUID )
			<< END_LOG;

		return;
	}

	// ��Ŷ ����
	KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iGuildSPoint = m_kSkillTree.GetGuildSPoint();
	SendToGSCharacter( spGuildMember->m_iGSUID, spGuildMember->m_iUnitUID, ELG_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
}

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////	
//{{ 2010. 01. 13  ������	���Խ���
#ifdef SERV_GUILD_AD

bool KGuild::CheckRegistrationGuildAd( IN IN UidType iUnitUID )
{
	SET_ERROR( NET_OK );
	
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��� ���� ����Ҽ� �ִ� ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER  &&
		spGuildMember->m_ucMemberShipGrade != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"��� ���� ����Ҽ��ִ� ����� �ƴմϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_00 );
		return false;
	}

	return true;
}

bool KGuild::MakeGuildAdInfo( IN const KDBE_REGISTRATION_GUILD_AD_ACK& kAck, OUT KGuildAdInfo& kInfo )
{
	KGuildMemberInfoPtr spGuildMember = GetGuildMember( m_iGuildMasterUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"��� �������� ������ �����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_iGuildMasterUnitUID )
			<< END_LOG;
		
		return false;
	}

	// ��� ���� ���� �����
	kInfo.m_iGuildUID = GetUID();
	kInfo.m_wstrGuildName = GetGuildName();
	kInfo.m_ucGuildLevel = GetGuildLevel();
	kInfo.m_iGuildEXP = m_kGuildInfo.m_iGuildEXP;
	kInfo.m_wstrMasterNickName = spGuildMember->m_wstrNickName;
	kInfo.m_usCurNumMember = GetNumMember();
	kInfo.m_wstrAdMessage = kAck.m_wstrAdMessage;
	//{{ 2010. 02. 04  ������	��� ���� ���� ����
	kInfo.m_wstrFoundingDay	= m_kGuildInfo.m_wstrFoundingDay;
	//}}
	kInfo.m_wstrAdRegDate = kAck.m_wstrAdRegDate;
	kInfo.m_wstrAdEndDate = kAck.m_wstrAdEndDate;
	return true;
}

void KGuild::ResultRegGuildAd( IN const std::wstring& wstrEndDate )
{
	KEGS_GUILD_MESSAGE_NOT kPacket;
	kPacket.m_cMessageType = KEGS_GUILD_MESSAGE_NOT::MT_REG_GUILD_AD;
	kPacket.m_wstrMessage = wstrEndDate;
	BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
}

bool KGuild::CheckGetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq )
{
	SET_ERROR( NET_OK );

	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	return true;
}

bool KGuild::CheckDeleteApplyJoin( IN const KELG_DELETE_APPLY_JOIN_GUILD_REQ& kReq )
{
	SET_ERROR( NET_OK );

	KGuildMemberInfoPtr spGuildMember = GetGuildMember( kReq.m_iUnitUID );
	if( spGuildMember == NULL )
	{
		START_LOG( cerr, L"�ش� ������ �ƴմϴ�. ���� ���� ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_19 );
		return false;
	}

	// ��û ���� ������ ����ΰ�?
	if( spGuildMember->m_ucMemberShipGrade != SEnum::GUG_MASTER  &&
		spGuildMember->m_ucMemberShipGrade != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"��� ���� ��û ���� ������ ����� �ƴմϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( spGuildMember->m_ucMemberShipGrade )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_00 );
		return false;
	}

	return true;
}

#endif SERV_GUILD_AD
//}}
//////////////////////////////////////////////////////////////////////////

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-24
void KGuild::RankingClear( void )
{
	m_mapRankerTab.clear();

	m_mapRankerUserInfo.clear();
	m_mapRankerUnitInfo.clear();

	m_mapUserUnitList.clear();
}

void KGuild::UpdateGuildMemberRanking( IN int iGuildUID
									 , IN const std::map<UidType, KLocalRankingUnitInfo>& mapRankerUnitInfo
									 , IN const std::map<UidType, UidType>& mapRankerUIDInfo
									 , IN const std::map<UidType, KLocalRankingUserInfo>& mapRankerUserInfo
									 )
{
	RankingClear();

	// ��ŷ ���� ����
	std::map<UidType, KLocalRankingUnitInfo>::const_iterator it;
	for( it = mapRankerUnitInfo.begin(); it != mapRankerUnitInfo.end(); ++it )
	{
		std::map<UidType, UidType>::const_iterator itUIDInfo = mapRankerUIDInfo.find( it->first );
		if( itUIDInfo == mapRankerUIDInfo.end() )
		{
			continue;
		}

		std::map<UidType, KLocalRankingUserInfo>::const_iterator itUserInfo = mapRankerUserInfo.find( itUIDInfo->second );
		if( itUserInfo == mapRankerUserInfo.end() )
		{
			continue;
		}

		std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT> vecRankUpNot;
		SomethingChanged( itUserInfo->second, it->second, vecRankUpNot );
	}
}

int KGuild::CheckChangedInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const
{
	bool bDataChanged = false;

	std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		bDataChanged |= !( itUser->second.m_kInfo == kUserInfo );
	}

	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		if( itUnit->second.m_kInfo.m_iPoint[SEnum::LRMTC_SPIRIT] != kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] )
		{
			return LRM_SPIRIT_CHANGED;
		}
		else if( itUnit->second.m_kInfo.m_iPoint[SEnum::LRMTC_AP] != kUnitInfo.m_iPoint[SEnum::LRMTC_AP] )
		{
			return LRM_AP_CHANGED;
		}
		else
		{
			bDataChanged |= !( itUnit->second.m_kInfo == kUnitInfo );
		}
	}
	else
	{
		if( ( 0 < kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] ) || ( 0 < kUnitInfo.m_iPoint[SEnum::LRMTC_AP] ) )
			return LRM_INSERT_DATA;
		else
			return LRM_DATA_CHANGED;
	}

	if( bDataChanged == true )
	{
		return LRM_DATA_CHANGED;
	}

	return LRM_NONE;
}

bool KGuild::SomethingChanged( IN const KLocalRankingUserInfo& kUserInfo
							 , IN const KLocalRankingUnitInfo& kUnitInfo
							 , OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
							 )
{
	// ������ �ٲ���°�?
	const int iChangeFlag = CheckChangedInfo( kUserInfo, kUnitInfo );
	switch( iChangeFlag )
	{
	case LRM_SPIRIT_CHANGED:
	case LRM_AP_CHANGED:
	case LRM_INSERT_DATA:
		// 2. ���� ELP ������ �ٲ����.
		// RankingProcess�� �����Ѵ�.
		RankingProcess( iChangeFlag, kUserInfo, kUnitInfo, vecRankUpNot );
		break;

	case LRM_DATA_CHANGED:
		// 3. ��Ÿ ������ �ٲ����.
		// ������ ����.
		DataUpdate( kUserInfo, kUnitInfo );
		break;

	default:
		// �ƹ��͵� ���� �ʴ´�.
		return false;
	}

	return true;
}

void KGuild::RankingProcess( IN const int iChangeFlag
						   , IN const KLocalRankingUserInfo& kUserInfo
						   , IN const KLocalRankingUnitInfo& kUnitInfo
						   , OUT std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>& vecRankUpNot
						   )
{
	const int iCategory = SEnum::LRLC_NONE + 1;
	int iMin;
	int iMax;

	if( iChangeFlag == LRM_INSERT_DATA )
	{
		iMin = SEnum::LRMTC_NONE + 1;
		iMax = SEnum::LRMTC_MAX;
	}
	else if( iChangeFlag == LRM_SPIRIT_CHANGED )
	{
		iMin = SEnum::LRMTC_SPIRIT;
		iMax = iMin + 1;
	}
	else if( iChangeFlag == LRM_AP_CHANGED )
	{
		iMin = SEnum::LRMTC_AP;
		iMax = iMin + 1;
	}

	for( int i = iMin; i < iMax; ++i )
	{
		// ���� ��ũ ���
		UidType iNextUnitUID	= 0;
		int		iPrevRank		= GetRankAndNextUnitUID( i, iCategory, kUnitInfo.m_iUnitUID, iNextUnitUID );
		int		iNextRank		= 0;

		// ��ũ ó��
		Remove( i, iCategory, kUserInfo, kUnitInfo );
		if( Add( i, iCategory, kUnitInfo.m_iPoint[i], kUserInfo, kUnitInfo ) == true )
		{
			// ���� ��ũ ��� ( iterator ���� )
			iNextRank = GetRankAndNextUnitUID( i, iCategory, kUnitInfo.m_iUnitUID, iNextUnitUID );
		}

		// ��ŷ�� ����Ͽ��ٸ�, ��Ŷ�� �ۼ��Ͽ� ��ȯ�� �����̳ʿ� ��´�.
		// ����( ��ŷ 0 )�� ���� ����
		if( ( 0 < iNextRank ) && ( iNextRank < iPrevRank ) )
		{
			KELG_LOCAL_RANKING_RANK_UP_NOT kPacket;
			kPacket.m_iMyRank		= iNextRank;
			kPacket.m_cMainTabIndex	= i;
			kPacket.m_cSubTabIndex	= SEnum::LRSTC_GUILD;
			GetUnitInfo( kUnitInfo.m_iUnitUID, kPacket.m_kMyUnitInfo );
			GetUnitInfo( iNextUnitUID, kPacket.m_kNextUnitInfo );
			vecRankUpNot.push_back( kPacket );
		}
	}
}

void KGuild::DataInsert( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	// ���� ���� ó��
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser == m_mapRankerUserInfo.end() )
	{
		KLocalRankingUserInfoCounter kUserInfoCount;
		itUser = m_mapRankerUserInfo.insert( std::map<UidType, KLocalRankingUserInfoCounter>::value_type( kUserInfo.m_iUserUID, kUserInfoCount ) ).first;
	}

	itUser->second.m_kInfo = kUserInfo;
	++itUser->second.m_iCounter;

	// ���� ���� ó��
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit == m_mapRankerUnitInfo.end() )
	{
		KLocalRankingUnitInfoCounter kUnitInfoCount;
		itUnit = m_mapRankerUnitInfo.insert( std::map<UidType, KLocalRankingUnitInfoCounter>::value_type( kUnitInfo.m_iUnitUID, kUnitInfoCount ) ).first;
	}

	itUnit->second.m_iUserUID	= kUserInfo.m_iUserUID;
	itUnit->second.m_kInfo		= kUnitInfo;
	++itUnit->second.m_iCounter;

	// ������ ���� ���� �̾��ֱ�
	std::map< UidType, std::set< UidType > >::iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
	if( itUnitList == m_mapUserUnitList.end() )
	{
		std::set<UidType> setUnitList;
		itUnitList = m_mapUserUnitList.insert( std::map< UidType, std::set< UidType > >::value_type( kUserInfo.m_iUserUID, setUnitList ) ).first;
	}

	itUnitList->second.insert( kUnitInfo.m_iUnitUID );
}

void KGuild::DataRemove( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	int iRemainUserInfoCount = 0;

	// ���� ���� ó��
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		if( 1 < itUser->second.m_iCounter )
		{
			--itUser->second.m_iCounter;
			iRemainUserInfoCount = itUser->second.m_iCounter;
		}
		else
		{
			m_mapRankerUserInfo.erase( itUser );
		}
	}

	// ���� ���� ó��
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		if( 1 < itUnit->second.m_iCounter )
		{
			--itUnit->second.m_iCounter;
		}
		else
		{
			m_mapRankerUnitInfo.erase( itUnit );
		}
	}

	// ������ ���� ���� �����ϱ�
	if( 0 < iRemainUserInfoCount )
	{
		std::map< UidType, std::set< UidType > >::iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
		if( itUnitList != m_mapUserUnitList.end() )
		{
			itUnitList->second.erase( kUnitInfo.m_iUnitUID );
		}
	}
	else
	{
		m_mapUserUnitList.erase( kUserInfo.m_iUserUID );
	}
}

void KGuild::DataUpdate( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	// ���� ���� ����
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		itUser->second.m_kInfo = kUserInfo;
	}

	// ���� ���� ����
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		itUnit->second.m_kInfo = kUnitInfo;
	}
}

bool KGuild::Remove( IN const int iMainTabIndex
				   , IN int iCategory
				   , IN const KLocalRankingUserInfo& kUserInfo
				   , IN const KLocalRankingUnitInfo& kUnitInfo
				   )
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return false;
	}

	return Remove( itRanker->second, kUserInfo, kUnitInfo );
}

bool KGuild::Add( IN const int iMainTabIndex
				, IN int iCategory
				, IN const int iAccumulatedPoint
				, IN const KLocalRankingUserInfo& kUserInfo
				, IN const KLocalRankingUnitInfo& kUnitInfo
				)
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		std::pair< std::map< std::pair< int, int >, RankMMAP >::iterator, bool> pairResult = m_mapRankerTab.insert( std::map< std::pair< int, int >, RankMMAP >::value_type( std::make_pair( iMainTabIndex, iCategory ), RankMMAP() ) );
		if( pairResult.second == false )
		{
			return false;
		}

		itRanker = pairResult.first;
	}

	return Add( itRanker->second, iAccumulatedPoint, kUserInfo, kUnitInfo );
}


bool KGuild::Remove( IN OUT RankMMAP& mmapRanker
				   , IN const KLocalRankingUserInfo& kUserInfo
				   , IN const KLocalRankingUnitInfo& kUnitInfo
				   )
{
	RankMMAP::iterator it;
	for( it = mmapRanker.begin(); it != mmapRanker.end(); ++it )
	{
		if( it->second == kUnitInfo.m_iUnitUID )
		{
			break;
		}
	}

	// �ش��ϴ� ������ ���ٸ� ��ƾ ����
	if( it == mmapRanker.end() )
	{
		return false;
	}

	// ��ũ���� ����
	mmapRanker.erase( it );

	DataRemove( kUserInfo, kUnitInfo );

	return true;
}

bool KGuild::Add( IN OUT RankMMAP& mmapRanker
				, IN const int iAccumulatedPoint
				, IN const KLocalRankingUserInfo& kUserInfo
				, IN const KLocalRankingUnitInfo& kUnitInfo
				)
{
	if( iAccumulatedPoint <= 0 )
	{
		return false;
	}

	if( static_cast<size_t>( SiKGameSysVal()->GetLocalRankingNumOfRanker() ) <= mmapRanker.size() )
	{
		// �������� �ѾƳ��� ��Ŀ�� �� �� �ִ� �����̴�.
		RankMMAP::reverse_iterator rit = mmapRanker.rbegin();
		if( rit != mmapRanker.rend() )
		{
			if( iAccumulatedPoint < rit->first )
			{
				// ��ŷ�� �� �� ���� ELP�̴�.
				return false;
			}
		}
	}

	// ������ �������, ���߿� ���Ե� ����� ���� ��ũ�� ��ġ ��Ų��.
	RankMMAP::iterator it = mmapRanker.find( iAccumulatedPoint );
	mmapRanker.insert( it, RankMMAP::value_type( iAccumulatedPoint, kUnitInfo.m_iUnitUID ) );

	// ���� ���� ó��
	DataInsert( kUserInfo, kUnitInfo );

	return true;
}

int KGuild::GetRankAndNextUnitUID( IN const int iMainTabIndex, IN const int iCategory, IN const UidType iMyUnitUID, OUT UidType& iNextUnitUID ) const
{
	int iRank		= 0;
	iNextUnitUID	= 0;

	std::map< std::pair< int, int >, RankMMAP >::const_iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return iRank;
	}

	const RankMMAP& mmapRanker = itRanker->second;

	RankMMAP::const_iterator it;
	for( it = mmapRanker.begin(); it != mmapRanker.end(); ++it )
	{
		++iRank;
		if( it->second == iMyUnitUID )
		{
			break;
		}
	}

	if( it == mmapRanker.end() )
	{
		return 0;
	}

	if( ++it == mmapRanker.end() )
	{
		iNextUnitUID = 0;
		return iRank;
	}

	iNextUnitUID = it->second;
	return iRank;
}

void KGuild::GetUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUnitInfo& kInfo ) const
{
	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator it = m_mapRankerUnitInfo.find( iUnitUID );
	if( it != m_mapRankerUnitInfo.end() )
	{
		kInfo = it->second.m_kInfo;
	}
}

void KGuild::GetRankerList( IN OUT UINT& nViewPage
						  , IN const char cMainTabIndex
						  , IN const byte byteFilter
						  , IN const std::set<UidType>& setFriendList
						  , OUT UINT& nTotalPage
						  , OUT std::vector<KLocalRankingRankerInfo>& vecRankerList
						  , OUT KLocalRankingRankerInfo& kPrevRankerInfo
						  , IN OUT KLocalRankingRankerInfo& kMyRankInfo
						  )
{
	if( ( cMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= cMainTabIndex ) )
	{
		START_LOG( cerr, L"�߸��� cMainTabIndex �Դϴ�." )
			<< BUILD_LOG( cMainTabIndex )
			<< END_LOG;
		return;
	}

	const int iCategory = SEnum::LRLC_NONE + 1;
	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( static_cast<int>( cMainTabIndex ), iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return;
	}

	const int iMax = 2;
	int iIndex = 0;
	RankMMAP* mmapRanker = &itRanker->second;
	RankMMAP mmapStorage[iMax];
	RankMMAP* mmapFiltered = mmapStorage + iIndex;

	if( byteFilter & SEnum::LRF_FRIEND )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			if( setFriendList.find( it->second ) != setFriendList.end() )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}

		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		//mmapFiltered->clear();
	}

	if( byteFilter & SEnum::LRF_UNIT )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( it->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			if( itUnit->second.m_kInfo.m_cUnitClass == kMyRankInfo.m_kUnitInfo.m_cUnitClass )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}

		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		mmapFiltered->clear();
	}

	if( SEnum::CheckFlag( byteFilter, SEnum::LRF_GENDER_MALE ) == true )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( it->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
			if( itUser == m_mapRankerUserInfo.end() )
			{
				continue;
			}

			if( SEnum::CheckFlag( itUser->second.m_kInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == true )
			{
				continue;
			}

			if( itUser->second.m_kInfo.m_byteGender == 1 )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}

		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		mmapFiltered->clear();
	}
	else if( SEnum::CheckFlag( byteFilter, SEnum::LRF_GENDER_FEMALE ) == true )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( it->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
			if( itUser == m_mapRankerUserInfo.end() )
			{
				continue;
			}

			if( SEnum::CheckFlag( itUser->second.m_kInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == true )
			{
				continue;
			}

			if( itUser->second.m_kInfo.m_byteGender == 2 )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}

		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		mmapFiltered->clear();
	}

	_GetRankerList( *mmapRanker
		, nViewPage
		, nTotalPage
		, vecRankerList
		, kPrevRankerInfo
		, kMyRankInfo
		);
}

void KGuild::_GetRankerList( IN const RankMMAP& mmapRanker
						   , IN OUT UINT& nViewPage
						   , OUT UINT& nTotalPage
						   , OUT std::vector<KLocalRankingRankerInfo>& vecRankerList
						   , OUT KLocalRankingRankerInfo& kPrevRankerInfo
						   , IN OUT KLocalRankingRankerInfo& kMyRankInfo
						   ) const
{
	const int iNumOfPageUser	= 7;
	const int iNumOfRanker		= SiKGameSysVal()->GetLocalRankingNumOfRanker();

	// ��ü ������ �� ���ϱ�
	int iTotalNum = min( static_cast<int>( mmapRanker.size() ), iNumOfRanker );
	nTotalPage = iTotalNum / iNumOfPageUser;
	if( 0 < ( iTotalNum % iNumOfPageUser ) )
	{
		++nTotalPage;
	}

	// nViewPage�� ���� ����Ʈ ���ϱ�
	UINT nNowPage	= 1;
	int iRank		= 0;
	std::vector< std::pair< UidType, int > > vecUnitList;

	RankMMAP::const_iterator itRanker;
	for( itRanker = mmapRanker.begin(); itRanker != mmapRanker.end(); ++itRanker )
	{
		++iRank;

		if( iNumOfPageUser == vecUnitList.size() )
		{
			++nNowPage;
			if( nViewPage < nNowPage )
			{
				break;
			}
			vecUnitList.clear();
		}
		vecUnitList.push_back( std::make_pair( itRanker->second, iRank ) );
	}

	// ���� ������ ���� ����
	nViewPage = min( nViewPage, nNowPage );

	// ���� ���� ���� ���ϱ�
	std::vector< std::pair< UidType, int > >::const_iterator itUnit;
	for( itUnit = vecUnitList.begin(); itUnit != vecUnitList.end(); ++itUnit )
	{
		KLocalRankingRankerInfo kInfo;
		kInfo.m_iRank = itUnit->second;
		GetUserUnitInfo( itUnit->first, kInfo.m_kUserInfo, kInfo.m_kUnitInfo, true );
		vecRankerList.push_back( kInfo );
	}

	// �� ���� ���� �� �� ��ŷ ���� ���ϱ�
	iRank = mmapRanker.size();
	const int iSize	= mmapRanker.size();
	RankMMAP::const_reverse_iterator rit = mmapRanker.rbegin();

	if( iNumOfRanker < iSize )
	{
		const int iCount = iSize - iNumOfRanker;
		for( int i = 0; i < iCount; ++i )
		{
			if( rit == mmapRanker.rend() )
			{
				break;
			}
			++rit;
			--iRank;
		}
	}

	// ������ ���� ���� ����
	kPrevRankerInfo.m_iRank					= iRank;
	kPrevRankerInfo.m_kUnitInfo.m_iUnitUID	= rit->second;

	// �� ���� ã��
	for( ; rit != mmapRanker.rend(); ++rit )
	{
		if( rit->second == kMyRankInfo.m_kUnitInfo.m_iUnitUID )
		{
			break;
		}
		--iRank;
	}

	// �� ��ũ üũ
	if( rit != mmapRanker.rend() )
	{
		// ���� ��Ŀ�̴�. ���� �� ���� ������ ������?
		kMyRankInfo.m_iRank = iRank;

		// �� ���� ���� üũ
		if( ++rit != mmapRanker.rend() )
		{
			kPrevRankerInfo.m_iRank					= --iRank;
			kPrevRankerInfo.m_kUnitInfo.m_iUnitUID	= rit->second;
		}
	}

	GetUserUnitInfo( kPrevRankerInfo.m_kUnitInfo.m_iUnitUID, kPrevRankerInfo.m_kUserInfo, kPrevRankerInfo.m_kUnitInfo, true );
}

void KGuild::GetUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo , OUT KLocalRankingUnitInfo& kUnitInfo, IN const bool bUserFilter /*= false */) const
{
	kUserInfo.clear();
	kUnitInfo.clear();

	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		kUnitInfo = itUnit->second.m_kInfo;

		std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
		if( itUser != m_mapRankerUserInfo.end() )
		{
			const KLocalRankingUserInfo& kSrcUserInfo = itUser->second.m_kInfo;
			KLocalRankingUserInfo& kDstUserInfo = kUserInfo;

			if( bUserFilter == true )
			{
				kDstUserInfo.m_iUserUID		= kSrcUserInfo.m_iUserUID;
				kDstUserInfo.m_byteFilter	= kSrcUserInfo.m_byteFilter;
				if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == false )
				{
					kDstUserInfo.m_iCategory = kSrcUserInfo.m_iCategory;
				}

				if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_BIRTH ) == false )
				{
					kDstUserInfo.m_iBirth = kSrcUserInfo.m_iBirth;
				}

				if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_PROFILE ) == false )
				{
					kDstUserInfo.m_wstrProfile = kSrcUserInfo.m_wstrProfile;
				}

				if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == false )
				{
					kDstUserInfo.m_byteGender = kSrcUserInfo.m_byteGender;
				}
			}
			else
			{
				kDstUserInfo = kSrcUserInfo;
			}
		}
	}
}

void KGuild::ResetLocalRankingInfo( void )
{
	std::map<UidType, KLocalRankingUnitInfo>	mapRankerUnitInfo;
	std::map<UidType, UidType>					mapRankerUIDInfo;
	std::map<UidType, KLocalRankingUserInfo>	mapRankerUserInfo;
	UpdateGuildMemberRanking( GetUID()
		, mapRankerUnitInfo
		, mapRankerUIDInfo
		, mapRankerUserInfo
		);
}
#endif	// SERV_LOCAL_RANKING_SYSTEM