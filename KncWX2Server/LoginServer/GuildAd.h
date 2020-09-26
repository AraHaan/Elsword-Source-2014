#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
//#include "SystemPacket.h"
#include "ClientPacket.h"
#include "BaseServer.h"

#include <KncSmartPtr.h>


#define KApplyJoinGuildInfoPtr KApplyJoinGuildInfo*
#undef KApplyJoinGuildInfoPtr
SmartPointers( KApplyJoinGuildInfo );

#define KGuildAdPtr KGuildAd*
#undef KGuildAdPtr
SmartPointer( KGuildAd );


#ifdef SERV_GUILD_AD


class KGuildAd
{
public:
	enum GUILD_AD_INFO
	{
		AD_PAGE_PER_COUNT = 6,
		APPLY_JOIN_PAGE_PER_COUNT = 6,
		MAX_APPLY_LIST_COUNT = 100,
	};

	class GuildSkillInfoManager
	{
	public:
		GuildSkillInfoManager();
		~GuildSkillInfoManager();

		bool GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo );
		void SetGuildSkillInfo( IN const KGuildSkillInfo& kInfo );

		void DeleteGuildSkillInfo() { m_bInitGuildSkillInfo = false; }

	private:
		bool				m_bInitGuildSkillInfo;
		KGuildSkillInfo		m_kGuildSkillInfo;
	};

public:
	KGuildAd( const KGuildAdInfo& kGuildAdInfo );
	virtual ~KGuildAd(void);

	// ��� ���� ����
	void SetGuildAdInfo( IN const KGuildAdInfo& kGuildAdInfo );
	const KGuildAdInfo& GetGuildAdInfo()		{ return m_kGuildAdInfo; }
	int GetGuildUID()							{ return m_kGuildAdInfo.m_iGuildUID; }
	int GetGuildNumMember()						{ return m_kGuildAdInfo.m_usCurNumMember; }
	u_char GetGuildLevel()						{ return m_kGuildAdInfo.m_ucGuildLevel; }
	int GetGuildEXP()							{ return m_kGuildAdInfo.m_iGuildEXP; }
	const std::wstring& GetGuildAdEndDate()		{ return m_kGuildAdInfo.m_wstrAdEndDate; }

	//{{ 2010. 02. 04  ������	��� ���� ���� ����
	const CTime& GetGuildFoundingDay()			{ return m_tFoundingDay; }
	//}}
	const CTime& GetCTimeRegDate()				{ return m_tAdRegDate; }
	const CTime& GetCTimeEndDate()				{ return m_tAdEndDate; }

	u_char GetGuildEXPUpCount()					{ return ( GetGuildEXP() - m_iInitGuildExp ); }

	// ��� ���� ��û��
	u_int GetApplyJoinNumber()					{ return m_mapApplyJoinGuild.size(); }
	KApplyJoinGuildInfoPtr GetApplyJoinGuildInfo( IN UidType iUnitUID );
	bool GetApplyJoinGuildInfo( IN UidType iUnitUID, OUT KApplyJoinGuildInfo& kInfo );
	bool IsExistMyApplyJoinGuild( IN UidType iUnitUID ) { return ( m_mapApplyJoinGuild.find( iUnitUID ) != m_mapApplyJoinGuild.end() ); }	

	bool IsExpired();
	bool IsDeleteTime();

	// ��� ���� ��û
	void NewApplyJoin( IN const KApplyJoinGuildInfo& kInfo );
	void DeleteApplyJoin( IN UidType iUnitUID );
	void UpdateApplyJoinInfo( IN const KApplyJoinGuildInfo& kInfo );

	// ��� ������ ����Ʈ ���
	void GetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq, OUT KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK& kAck );
	void GetApplyJoinGuildList( OUT std::vector< KApplyDeleteInfo >& vecApplyDelete );

	// ��� ��ų
	bool GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo )		{ return m_kGuildSkill.GetGuildSkillInfo( kInfo ); }
	void SetGuildSkillInfo( IN const KGuildSkillInfo& kInfo )	{ m_kGuildSkill.SetGuildSkillInfo( kInfo ); }
	void DeleteGuildSkillInfo()									{ m_kGuildSkill.DeleteGuildSkillInfo(); }
	
	// ��� ���� ������Ʈ
	void UpdateGuildEXP( IN int iGuildExp )						{ m_kGuildAdInfo.m_iGuildEXP = iGuildExp; }
	void UpdateGuildLevelResult( IN u_char ucGuildLevel )		{ m_kGuildAdInfo.m_ucGuildLevel = ucGuildLevel; }
	void UpdateGuildMasterNickName( IN const std::wstring& wstrNickName ) { m_kGuildAdInfo.m_wstrMasterNickName = wstrNickName; }
	void UpdateGuildCurMemberCount( IN u_short usMemberCount )	{ m_kGuildAdInfo.m_usCurNumMember = usMemberCount; }

	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	void UpdateGuildName( IN const std::wstring& wstrGuildName );
#endif SERV_GUILD_CHANGE_NAME
	//}}

private:
	void InsertApplyJoin( IN const KApplyJoinGuildInfo& kInfo );

	// ����
    void SortApplyJoinList();
	static bool QuickOrder( const KApplyJoinGuildInfoPtr spFirst, const KApplyJoinGuildInfoPtr spSecond );

private:
	KGuildAdInfo								m_kGuildAdInfo;
	//{{ 2010. 02. 04  ������	��� ���� ���� ����
	CTime										m_tFoundingDay;
	//}}
	CTime										m_tAdRegDate;
	CTime										m_tAdEndDate;
	int											m_iInitGuildExp;
	
	GuildSkillInfoManager						m_kGuildSkill;		// ��� ��ų ����

	std::map< UidType, KApplyJoinGuildInfoPtr >	m_mapApplyJoinGuild;
	std::vector< KApplyJoinGuildInfoPtr >		m_vecApplyJoinGuild;
};


#endif SERV_GUILD_AD