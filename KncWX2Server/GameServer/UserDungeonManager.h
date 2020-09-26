#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include "CacheData.h"
#include "X2Data\XSLUnit.h"


//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST


class KUserDungeonManager
{
public:
	KUserDungeonManager(void);
	~KUserDungeonManager(void);

	enum HENIR_REWARD_LIMIT_COUNT
	{
		HRLC_NORMAL				= 3,		// Normal �Ϲ� ���
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
		HRLC_NORMAL_CHALLENGE	= 1,		// Normal ���� ���
#endif // SERV_HENIR_RENEWAL_2013
		HRLC_PREMIUM			= 2,		// pc�� ����
	};

	void	Clear();
	void	Init( IN const bool bUnlimitedEvent				// ������ �ΰ�?
				, IN const int iLimitEvent					// Event �߰� Ƚ�� MAX
				, IN const int iNormalCnt					// ���� ���� Ƚ�� �Ϲ�
				, IN const int iPremiumCnt					// ���� ���� Ƚ�� pc��
				, IN const int iEventCnt					// ���� ���� Ƚ�� �̺�Ʈ
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
				, IN const int iChallengeNormalCnt
				, IN const int iChallengePremiumCnt
				, IN const int iChallengeEventCnt
#endif // SERV_HENIR_RENEWAL_2013
				, IN const bool bIsPcBang					// PC������ ���� �ֱ�
#ifdef SERV_PC_BANG_TYPE
				, IN const int iPcBangType					// PC�� Ÿ��
#endif SERV_PC_BANG_TYPE
				);

	// ���� ���� �� �ִ� ���� Ƚ�� ���� - ġƮ��
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	void	SetPossibleHenirRewardNormalCount( IN const int iNormal )
	{
		m_iHenirRewardCount = HRLC_NORMAL - iNormal;
		m_iHenirChallengeRewardCount = HRLC_NORMAL_CHALLENGE - iNormal;
	}
#else // SERV_HENIR_RENEWAL_2013
	void	SetPossibleHenirRewardNormalCount( IN const int iNormal )		{ m_iHenirRewardCount = HRLC_NORMAL - iNormal; }
#endif // SERV_HENIR_RENEWAL_2013


	bool	IsPremiumUser() const										{ return m_bIsPcBang; }
#ifdef SERV_PC_BANG_TYPE
	int		GetPcBangType() const										{ return m_iPcBangType; }
#endif SERV_PC_BANG_TYPE

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	bool	IncreaseHenirRewardCount( IN const char cDungeonMode );		// ���� ȹ�� Ƚ�� ����
#else // SERV_HENIR_RENEWAL_2013
	bool	IncreaseHenirRewardCount();																// ���� ȹ�� Ƚ�� ����
#endif // SERV_HENIR_RENEWAL_2013

	// �̺�Ʈ�� ���� �� ����
	void	SetUnLimitedHenirRewardEvnet( IN const bool bLimit )		{ m_bUnLimitedHenirRewardEvnet = bLimit; }	// ���� ���� ȹ�� ����
	void	SetHenirRewardEvnetLimitCount( IN const int iLimit )		{ m_iHenirRewardEventLimitCount = iLimit; }	// �̺�Ʈ�� ���� ���� Ƚ�� ����

	// ���� ���� �� �ִ� ���� Ƚ�� ���
	int		GetPossibleHenirRewardNormalCount();
	int		GetPossibleHenirRewardEventCount();
	int		GetPossibleHenirRewardPremiumCount();
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	int		GetPossibleHenirChallengeRewardNormalCount( void ) const;
	int		GetPossibleHenirChallengeRewardEventCount( void ) const;
	int		GetPossibleHenirChallengeRewardPremiumCount( void ) const;
#endif // SERV_HENIR_RENEWAL_2013

	// ���� ���� ���� Ƚ�� ���
	int		GetHenirRewardNormalCount()									{ return m_iHenirRewardCount; }
	int		GetHenirRewardEventCount()									{ return m_iHenirRewardEventCount; }
	int		GetHenirRewardPremiumCount()								{ return m_iHenirRewardPremiumCount; }
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	int		GetHenirChallengeRewardNormalCount( void ) const			{ return m_iHenirChallengeRewardCount; }
	int		GetHenirChallengeRewardEventCount( void ) const				{ return m_iHenirChallengeRewardEventCount; }
	int		GetHenirChallengeRewardPremiumCount( void ) const			{ return m_iHenirChallengeRewardPremiumCount; }
#endif // SERV_HENIR_RENEWAL_2013
	
	// ������ ���� �̺�Ʈ ���� ���
	bool	GetPossibleHenirRewardEventUnLimited()								{ return m_bUnLimitedHenirRewardEvnet; }

	void	GetHenirRewardCountInfo( IN KEGS_HENIR_REWARD_COUNT_NOT& kNot );

	//{{ 2012. 02. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType GetAutoPartyWaitNumber() const								{ return m_iAutoPartyWaitNumber; }
	void	SetAutoPartyWaitNumber( IN const UidType iWaitNumber )		{ m_iAutoPartyWaitNumber = iWaitNumber; }
	UidType GetAutoPartyUID() const										{ return m_iAutoPartyUID; }
	void	SetAutoPartyUID( IN const UidType iAutoPartyUID )			{ m_iAutoPartyUID = iAutoPartyUID; }

	void	SetDungeonGameInfo( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode );
	void	SetDungeonGameInfo( IN const KDungeonGameInfo& kInfo )		{ SetDungeonGameInfo( kInfo.m_iDungeonID, kInfo.m_cDifficulty, kInfo.m_cDungeonMode ); }
	int		GetDungeonID() const										{ return m_kDungeonGameInfo.m_iDungeonID; }
	bool	IsPlayingNowHenirChallengeMode() const;

	// for autoparty
	void	GetAutoPartyBonusDungeonList( IN const int iVillageMapID, IN OUT std::map< int, KAutoPartyDungeonBonusInfo >& mapAutoPartyBonusList );
	void	GetAutoPartyBonusDungeonList( IN const std::vector< KDungeonGameSimpleInfo >& vecDungeonList, IN OUT std::map< int, KAutoPartyDungeonBonusInfo >& mapAutoPartyBonusList );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	char		GetDungeonDifficulty() const							{ return m_kDungeonGameInfo.m_cDifficulty; }
#endif SERV_REFORM_QUEST
	//}}

private:
	bool				m_bIsPcBang;
#ifdef SERV_PC_BANG_TYPE
	int					m_iPcBangType;
#endif SERV_PC_BANG_TYPE

//{{-------- ���� �ޱ� ���� Ƚ�� ���� ������ ---------
	// ���� ���� Ƚ���� ����
	int					m_iHenirRewardCount;				// ��ϸ� ���� ����� Ƚ�� (�Ϲ�)
	int					m_iHenirRewardPremiumCount;			// ��ϸ� ���� ����� Ƚ�� (PC��)
	int					m_iHenirRewardEventCount;			// ��ϸ� ���� ����� Ƚ�� (�̺�Ʈ) Ƚ��
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	// ���� ��� ���� ���� Ƚ���� ����
	int					m_iHenirChallengeRewardCount;			// ��ϸ� ���� ��� ���� ����� Ƚ�� (�Ϲ�)
	int					m_iHenirChallengeRewardPremiumCount;	// ��ϸ� ���� ��� ���� ����� Ƚ�� (PC��)
	int					m_iHenirChallengeRewardEventCount;		// ��ϸ� ���� ��� ���� ����� Ƚ�� (�̺�Ʈ) Ƚ��
#endif // SERV_HENIR_RENEWAL_2013
	
	// ���� ���� �� �ִ� Ƚ��
	int					m_iHenirRewardEventLimitCount;		// ��ϸ� ���� ���� Ƚ�� (�̺�Ʈ) ����
	// ���� ���� �̺�Ʈ ���°�
	bool				m_bUnLimitedHenirRewardEvnet;		// ��ϸ� ���� ���� Ƚ�� (�̺�Ʈ) ������
//}}----------------------------------------------------

	//{{ 2012. 02. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType				m_iAutoPartyWaitNumber;
	UidType				m_iAutoPartyUID;

	KDungeonGameInfo	m_kDungeonGameInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};


#endif SERV_NEW_HENIR_TEST
//}}