#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include "Enum/Enum.h"

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM


class KUserRelationshipManager
{
public:
	KUserRelationshipManager(void);
	~KUserRelationshipManager(void);

	enum BREAK_UP_COST
	{
		BUC_COUPLE		= 100000,
		BUC_MERRIED		= 200000,
		BUC_MAX			= 100000000,
	};

	void Init( IN KRelationshipInfo& kInfo );

	char	GetRelationshipType()	const { return m_cRelationshipType; }
	UidType GetLoverUnitUID()		const { return m_iLoverUnitUID; }
	const std::wstring&	GetLoverNickName() const	{ return m_wstrLoverNickName; }
	void	GetLoveWord( std::wstring& wstrWord );
	void	GetAnniversaryDay( CTime& tDate );

	bool	SetRelationshipType( char cType );
	void	SetLoverUnitUID( UidType iUnitUID )				{ m_iLoverUnitUID = iUnitUID; }
	void	SetLoverNickName( std::wstring& wstrNickName )	{ m_wstrLoverNickName = wstrNickName; }
	void	SetLoveWord( std::wstring& wstrWord )			{ m_wstrLoveWord = wstrWord; }
	void	SetAnniversaryDay( CTime& tDate )				{ m_tAnniversaryDay = tDate; }
	void	SetLastRewardDate( CTime& tDate );
	void	SetRewardTitleType( char cRewardTitleType )		{ m_cRewardTitleType = cRewardTitleType; }

	bool	IsSolo()										{ return m_cRelationshipType == SEnum::RT_SOLO; }
	bool	IsSolo( IN char cRelationshipType )				{ return cRelationshipType == SEnum::RT_SOLO; }
	bool	IsCouple()										{ return m_cRelationshipType == SEnum::RT_COUPLE; }
	bool	IsMarried()										{ return m_cRelationshipType == SEnum::RT_MARRIED; }

#ifdef SERV_NEW_CHARACTER_EL	// ���볯¥: 2013-07-18
	int		CheckChar( IN char cUnitClassA, IN char cUnitClassB );
#else
	bool	CheckChar( IN char cUnitType, IN char cUnitClass );
#endif	// SERV_NEW_CHARACTER_EL
	
	bool	CheckCoupleDate();

	bool	IsValiedWeddingOfficiantNpc( IN const char cOfficiantNPC );
	bool	IsValiedWeddingHall( IN const char cWeddingHallType );

	bool	IsWeddingItem( IN const int iItemID, OUT char& cWeddingStatus );

	int		GetBreakUpCost();
	void	SetBreakUp();

	bool	IsRewardDay();
	int		IsRewardTitleItem( OUT int& iTitleRewardStep );

private:
	char				m_cRelationshipType;			// [�ַ�, Ŀ��, ��ȥ]��������
	UidType				m_iLoverUnitUID;				// ���� ĳ����
	std::wstring		m_wstrLoverNickName;			// ���� ĳ���͸�
	std::wstring		m_wstrLoveWord;					// ��Ī - Ŀ�� ����Ʈ ��Ʈ��
	CTime				m_tAnniversaryDay;				// �����(Ŀ�� ������, ��ȥ��)
	CTime				m_tLastRewardDate;				// ������ ���� ���� �ð�
	char				m_cRewardTitleType;				// ���� ���� Ÿ��Ʋ

	std::map< UidType, int >			m_mapWeddingTicketItem;	// [ûø��/����Ǿ�����UID, ��ȥ��UID]
};


#endif SERV_RELATIONSHIP_SYSTEM
//}