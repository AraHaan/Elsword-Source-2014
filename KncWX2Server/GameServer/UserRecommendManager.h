#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

class KUserRecommendManager
{
public:
	enum RECOMMEND_ENUM
	{
		PAGE_PER_COUNT = 8,
	};

	KUserRecommendManager(void);
	~KUserRecommendManager(void);

	void Clear();

	void SetIsRecommend( bool bVal )	{ m_bIsRecommend = bVal; }
	bool IsRecommend()					{ return m_bIsRecommend; }

	void SetRecommendedUnitUID( UidType iUnitUID ) { m_iRecommendedUnitUID = iUnitUID; }
	UidType GetRecommendedUnitUID()		{ return m_iRecommendedUnitUID; }

	bool IsInitRecommendUserList()		{ return m_bInitRecommendUserList; }
	void SetRecommendUserList( IN std::vector< KRecommendUserInfo >& vecUserList );
	void GetRecommendUserList( IN OUT u_int& uiViewPage, OUT u_int& uiTotalPage, OUT std::vector< KRecommendUserInfo >& vecUserList );
#ifdef SERV_RECRUIT_EVENT_BASE
	bool IsInitRecruitRecruiterList()	{ return m_bInitRecruitRecruiterList; }

	void SetRecruitUnitList( IN const std::vector< KRecommendUserInfo >& vecRecruitUnitList ) { m_bInitRecruitRecruiterList = true; m_vecRecruitUnitList = vecRecruitUnitList; }
	void AddRecruitUnitInfo( KRecommendUserInfo& kRecruitUnitInfo ) { m_vecRecruitUnitList.push_back( kRecruitUnitInfo ); }
	void GetRecruitUnitList( OUT std::vector< KRecommendUserInfo >& vecRecruitUnitList ) { vecRecruitUnitList = m_vecRecruitUnitList; }

	void SetRecruiterUnitList( IN const std::vector< KRecommendUserInfo >& vecRecruiterUnitList ) { m_bInitRecruitRecruiterList = true; m_vecRecruiterUnitList = vecRecruiterUnitList; }
	void AddRecruiterUnitInfo( KRecommendUserInfo& kRecruiterUnitInfo ) { m_vecRecruiterUnitList.push_back( kRecruiterUnitInfo ); }
	void GetRecruiterUnitList( OUT std::vector< KRecommendUserInfo >& vecRecruiterUnitList ) { vecRecruiterUnitList = m_vecRecruiterUnitList; }

	void ModifyRecruitRecruiterState( IN const KELG_RECRUIT_RECRUITER_INFO_NOT& kInfo );

	bool IsRecruit() { return !m_vecRecruiterUnitList.empty(); }
#endif SERV_RECRUIT_EVENT_BASE

	void AddRecommendUser( IN const KRecommendUserInfo& kInfo );
	void ModifyRecommendUser( IN const KELG_RECOMMEND_USER_INFO_NOT& kInfo );

private:
	bool								m_bIsRecommend;				// ��õ�� �Ҽ� �ִ� �������� ����
	UidType								m_iRecommendedUnitUID;		// ���� ��õ�� ���� UnitUID

	bool								m_bInitRecommendUserList;	// ��õ�� ����Ʈ �ʱ�ȭ
	std::vector< KRecommendUserInfo >	m_vecRecommendUserList;		// ��õ�� ����Ʈ

#ifdef SERV_RECRUIT_EVENT_BASE
	bool								m_bInitRecruitRecruiterList;	// ��õ�� ����Ʈ �ʱ�ȭ
	std::vector< KRecommendUserInfo >	m_vecRecruitUnitList;		// ����õ�� ����Ʈ
	std::vector< KRecommendUserInfo >	m_vecRecruiterUnitList;		// ��õ�� ����Ʈ
#endif SERV_RECRUIT_EVENT_BASE
};