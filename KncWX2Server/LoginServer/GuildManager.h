#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include "Guild.h"
#include <boost/timer.hpp>

#ifdef GUILD_TEST


//////////////////////////////////////////////////////////////////////////
// Guild Manager : ��� ������ ����

class KGuildManager
{
	DeclareSingleton( KGuildManager );
	DeclDump;
	DeclToStringW;

public:
	KGuildManager(void);
	~KGuildManager(void);

	//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	void Tick();
#endif GUILD_SKILL_TEST
	//}}

	bool IsExistGuild( IN int iGuildUID ) { return ( m_mapGuild.find( iGuildUID ) != m_mapGuild.end() ); }
	KGuildPtr GetGuild( IN int iGuildUID );

	//////////////////////////////////////////////////////////////////////////
	// ��� ���� ��� & ������Ʈ
	bool GetGuildInfo( IN int iGuildUID, OUT KEGS_GUILD_INFO_NOT& kInfo );
	//}}
	bool GetGuildSkillInfo( IN int iGuildUID, OUT KGuildSkillInfo& kInfo );
	bool UpdateGuildInfo( IN const KGuildInfo& kGuildInfo );
	bool UpdateGuildMemberList( IN int iGuildUID, IN const std::vector< KGuildMemberInfo >& vecMemberInfo );	// ���� ����Ʈ ������Ʈ

	//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	bool UpdateGuildSkill( IN int iGuildUID, IN const KGuildSkillInfo& kInfo );
#endif GUILD_SKILL_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ��� ���� ���
	bool CheckJoinGuild( IN int iGuildUID, IN UidType iUnitUID, OUT UidType& iGuildMasterUnitUID );
	bool RemoveReserve( IN int iGuildUID, IN UidType iUnitUID );

	bool JoinGuildMember( IN int iGuildUID, IN const KGuildMemberInfo& kMemberInfo, IN bool bIsLoginUser );		// ��� ����
	bool LoginGuildMember( IN int iGuildUID, IN const KGuildMemberInfo& kMemberInfo );							// ���� �α���
	bool LogoutGuildMember( IN int iGuildUID, IN UidType iUnitUID );											// ���� �α׾ƿ�
	
	bool CheckInviteCondition( IN int iGuildUID, IN UidType iReceiverUnitUID, IN UidType iSenderUnitUID, OUT KELG_INVITE_GUILD_FIND_USER_REQ& kPacket );	// ���� �ʴ�
	bool GetGuildUserList( IN const KELG_GET_GUILD_USER_LIST_REQ& kReq, OUT KEGS_GET_GUILD_USER_LIST_ACK& kAck );									// ��� ���� ����Ʈ
	
	bool GuildChat( IN const KELG_CHAT_REQ& kInfo );															// ��� ä��

	bool CheckChangeGradeGuild( IN const KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ& kReq, OUT KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK& kAck );		// ��� ��� ��� ���� üũ
	bool UpdateChangeGuildMemberGrade( IN const KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK& kInfo );					// ��� ��� ��� ����
	bool UpdateChangeGuildMessage( IN const KDBE_CHANGE_GUILD_MESSAGE_ACK& kInfo );								// ��� �޽��� ����
	bool UpdateKickGuildMember( IN const KDBE_KICK_GUILD_MEMBER_ACK& kInfo, OUT std::wstring& wstrGuildName );	// ���� Ż��
	bool UpdateChangeGuildMemberMessage( IN const KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK& kInfo );				// ���� �λ縻 ����
	bool DisbandGuild( IN int iGuildUID );																		// ��� �ػ�
	bool UpdateGuildMemberInfo( IN int iGuildUID, IN const KGuildMemberInfo& kInfo );							// ��� ��� ���� ����

	bool CheckGuildMaxMember( IN const KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ& kInfo );							// ��� �ִ� �ο� Ȯ�� ���� ���� �˻�
	bool UpdateGuildMaxMember( IN int iGuildUID, IN u_short usMaxNumMember );									// ��� �ִ� �ο� Ȯ��

	//{{ 2009. 10. 27  ������	��巹��
	bool UpdateGuildEXP( IN int iGuildUID, IN const KDBE_UPDATE_GUILD_EXP_ACK& kInfo );
	bool UpdateGuildLevelResult( IN int iGuildUID, IN const KDBE_GUILD_LEVEL_UP_ACK& kInfo );
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST

	bool CheckLearnGuildSkill( IN const KELG_GET_GUILD_SKILL_REQ& kReq, OUT KDBE_GET_GUILD_SKILL_REQ& kPacketToDB );
	bool InsertGuildSkill( IN const KDBE_GET_GUILD_SKILL_ACK& kAck );

	bool CheckResetGuildSkill( IN const KELG_RESET_GUILD_SKILL_REQ& kReq, OUT KDBE_RESET_GUILD_SKILL_REQ& kPacketToDB );
	bool ResetGuildSkill( IN const KDBE_RESET_GUILD_SKILL_ACK& kAck );

	bool CheckInitGuildSkill( IN const KELG_INIT_GUILD_SKILL_REQ& kReq, OUT KDBE_INIT_GUILD_SKILL_REQ& kPacketToDB );
	bool InitGuildSkill( IN const KDBE_INIT_GUILD_SKILL_ACK& kAck );

	bool InsertGuildCashSkillPoint( IN const KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT& kNot );
	bool InsertGuildCashSkillPointResult( IN const KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK& kAck );

	bool ExpireGuildCashSkillPointResult( IN const KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK& kAck );

	bool AdminInsertGuildSkillPoint( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ& kReq );
	bool AdminInsertGuildSkillPointResult( IN const KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK& kAck );

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 01. 13  ������	���Խ���
#ifdef SERV_GUILD_AD	
	
	bool CheckRegistrationGuildAd( IN int iGuildUID, IN UidType iUnitUID );
	bool MakeGuildAdInfo( IN const KDBE_REGISTRATION_GUILD_AD_ACK& kAck, OUT KGuildAdInfo& kInfo );
	bool ResultRegGuildAd( IN int iGuildUID, IN const std::wstring& wstrEndDate );
	
	bool CheckGetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq );
	bool CheckDeleteApplyJoin( IN const KELG_DELETE_APPLY_JOIN_GUILD_REQ& kReq );

#endif SERV_GUILD_AD
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	bool ReserveGuildName( IN const UidType& iUnitUID, IN const std::wstring& wstrGuildName );
	bool CancelGuildName( IN const UidType& iUnitUID );
	bool GetReservedGuildName( IN const UidType& iUnitUID, OUT std::wstring& wstrReservedGuildName );
	bool CheckReservedGuildName( IN const UidType& iUnitUID );
	void TimeRefresh( IN const UidType& iUnitUID );
#endif SERV_GUILD_CHANGE_NAME
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-24
	void ResetLocalRankingInfo( void );
#endif	// SERV_LOCAL_RANKING_SYSTEM

private:
	void InsertGuild( IN const KGuildInfo& kInfo );
	void DeleteGuild( IN int iGuildUID );
	void DeleteReserve( IN int iGuildUID );

private:
	std::map< int, KGuildPtr >		m_mapGuild;
	std::vector< KGuildPtr >		m_vecGuild;

	//{{ 2009. 12. 2  ������	��屸������
	std::vector< int >				m_vecDelReserved;
	//}}

	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	struct ReservedGuildName
	{
		CTime			m_dRecordTime;
		UidType			m_iUnitUID;
		std::wstring	m_wstrGuildName;

		ReservedGuildName( IN const CTime& dRecordTime, IN const UidType& iUnitUID, IN const std::wstring& wstrGuildName )
		{
			m_dRecordTime = dRecordTime;
			m_iUnitUID = iUnitUID;
			m_wstrGuildName = wstrGuildName;
		}
	};

	std::list< ReservedGuildName >		m_listReservedGuildName;
	boost::timer						m_kTimer;
#endif SERV_GUILD_CHANGE_NAME
	//}}
};

DefSingletonInline( KGuildManager );


#endif GUILD_TEST


