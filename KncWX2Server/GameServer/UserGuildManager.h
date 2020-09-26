#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include "CacheData.h"

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

SmartPointer(KGSUser);

class KUserGuildManager
{
	enum GUILD_ENUM
	{
		HONOR_POINT_MAX = 1600,
		GUILD_QUEST_ID  = 7630, // ��� �������� �ڰ�
	};

public:
	KUserGuildManager(void);
	~KUserGuildManager(void);

	void Clear();
	void SetGuildInfo( IN const KUserGuildInfo& kInfo );
	void SetGuildInfo( IN int iGuildUID, IN const std::wstring& wstrGuildName, IN u_char ucMemberShipGrade, IN int iHonorPoint = 0 );	

	int GetGuildUID()									{ return m_iGuildUID; }
	const std::wstring& GetGuildName()					{ return m_wstrGuildName; }
	u_char GetMemberShipGrade()							{ return m_ucMemberShipGrade; }
	int GetHonorPoint()									{ return m_iHonorPoint; }
	void SetMemberShipGrade( IN u_char ucMemberShipGrade );

	// �ټ��� �Ҹ� ���� �� ����Ʈ
	void OnDecreaseSpirit( IN KGSUserPtr spUser, IN int iDecreaseSpirit );

	//{{ 2009. 11. 16  ������	�����������Ʈ
	void OnUpdatePvpUnitInfo( IN KGSUserPtr spUser, IN int iPvpMemberCount );
	//}}

	// ��� ���� ������ ���� �˻�
	bool CheckBuyGuildItem( IN KGSUserPtr spUser, IN const std::map< int, int >& mapBuyItemList );

	//{{ 2009. 11. 24  ������	��彺ų
	void SetGuildSkill( IN const KGuildSkillInfo& kInfo );
	bool IsExistGuildSkill( IN int iGuildSkillID )		{ return ( m_mapSkillTree.find( iGuildSkillID ) != m_mapSkillTree.end() ); }
	bool IsCashSkillPointExpired()						{ return ( m_iMaxGuildCSPoint == 0 ); }
	void SetMaxGuildCSPoint( IN int iMaxGuildCSPoint )	{ m_iMaxGuildCSPoint = iMaxGuildCSPoint; }
	void UpdateGuildSkill( IN const KEGS_GET_GUILD_SKILL_NOT& kInfo );
	bool ResetGuildSkill( IN const KEGS_RESET_GUILD_SKILL_NOT& kInfo );
	void ExpireGuildSkill( IN const KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT& kInfo );
	void InitGuildSkill();

	void GetGuildPassiveSkillData( OUT std::vector< KSkillData >& vecSkillData );
	void GetGuildSkillStat( OUT KStat& kStat );
	void GetGuildSkillBonusRate( OUT float& fAddGuildBonusExpRate, IN OUT float& fAddSocketOptEDRate );

	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
	void SetGuildName( const std::wstring& wstrGuildName ) { m_wstrGuildName = wstrGuildName; }
	bool IsGuildMaster( void );
	void SetGuildNameChanging( void );
	void ResetGuildNameChanging( void );
	bool CheckGuildNameChanging( void );

private:
	void CheckHonorPoint( IN KGSUserPtr spUser );

private:
	int								m_iGuildUID;
	std::wstring					m_wstrGuildName;
	u_char							m_ucMemberShipGrade;

	//{{ 2009. 10. 27  ������	��巹��
	int								m_iHonorPoint;

	//{{ 2009. 11. 24  ������	��彺ų
	typedef std::map< int, KGuildSkillData >	GuildSkillDataMap;
	GuildSkillDataMap				m_mapSkillTree;
	int								m_iMaxGuildCSPoint;

	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
	bool							bIsGuildNameChanging;
};

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
#ifdef GUILD_TEST

SmartPointer(KGSUser);

class KUserGuildManager
{
enum GUILD_ENUM
{
HONOR_POINT_MAX = 1600,
GUILD_QUEST_ID  = 7630, // ��� �������� �ڰ�
};

public:
KUserGuildManager(void);
~KUserGuildManager(void);

void Clear();
void SetGuildInfo( IN const KUserGuildInfo& kInfo );
void SetGuildInfo( IN int iGuildUID, IN const std::wstring& wstrGuildName, IN u_char ucMemberShipGrade, IN int iHonorPoint = 0 );	

int GetGuildUID()									{ return m_iGuildUID; }
const std::wstring& GetGuildName()					{ return m_wstrGuildName; }
u_char GetMemberShipGrade()							{ return m_ucMemberShipGrade; }
int GetHonorPoint()									{ return m_iHonorPoint; }
void SetMemberShipGrade( IN u_char ucMemberShipGrade );

// �ټ��� �Ҹ� ���� �� ����Ʈ
void OnDecreaseSpirit( IN KGSUserPtr spUser, IN int iDecreaseSpirit );

//{{ 2009. 11. 16  ������	�����������Ʈ
void OnUpdatePvpUnitInfo( IN KGSUserPtr spUser, IN int iPvpMemberCount );
//}}

// ��� ���� ������ ���� �˻�
bool CheckBuyGuildItem( IN KGSUserPtr spUser, IN const std::map< int, int >& mapBuyItemList );

//{{ 2009. 11. 24  ������	��彺ų
#ifdef GUILD_SKILL_TEST
void SetGuildSkill( IN const KGuildSkillInfo& kInfo );
bool IsExistGuildSkill( IN int iGuildSkillID )		{ return ( m_mapSkillTree.find( iGuildSkillID ) != m_mapSkillTree.end() ); }
bool IsCashSkillPointExpired()						{ return ( m_iMaxGuildCSPoint == 0 ); }
void SetMaxGuildCSPoint( IN int iMaxGuildCSPoint )	{ m_iMaxGuildCSPoint = iMaxGuildCSPoint; }
void UpdateGuildSkill( IN const KEGS_GET_GUILD_SKILL_NOT& kInfo );
bool ResetGuildSkill( IN const KEGS_RESET_GUILD_SKILL_NOT& kInfo );
void ExpireGuildSkill( IN const KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT& kInfo );
void InitGuildSkill();

void GetGuildPassiveSkillData( OUT std::vector< KSkillData >& vecSkillData );
void GetGuildSkillStat( OUT KStat& kStat );
void GetGuildSkillBonusRate( OUT float& fAddGuildBonusExpRate, IN OUT float& fAddSocketOptEDRate );
#endif GUILD_SKILL_TEST
//}}

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
void SetGuildName( const std::wstring& wstrGuildName ) { m_wstrGuildName = wstrGuildName; }
bool IsGuildMaster( void );
void SetGuildNameChanging( void );
void ResetGuildNameChanging( void );
bool CheckGuildNameChanging( void );
#endif SERV_GUILD_CHANGE_NAME
//}}

private:
void CheckHonorPoint( IN KGSUserPtr spUser );

private:
int								m_iGuildUID;
std::wstring					m_wstrGuildName;
u_char							m_ucMemberShipGrade;

//{{ 2009. 10. 27  ������	��巹��
int								m_iHonorPoint;
//}}

//{{ 2009. 11. 24  ������	��彺ų
#ifdef GUILD_SKILL_TEST
typedef std::map< int, KGuildSkillData >	GuildSkillDataMap;
GuildSkillDataMap				m_mapSkillTree;
int								m_iMaxGuildCSPoint;
#endif GUILD_SKILL_TEST
//}}

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
bool							bIsGuildNameChanging;
#endif SERV_GUILD_CHANGE_NAME
//}}
};

#endif GUILD_TEST

*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


