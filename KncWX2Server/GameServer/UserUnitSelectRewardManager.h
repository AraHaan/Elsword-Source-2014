#pragma once
#include "Event.h"
#include "CommonPacket.h"

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY

class KUserUnitSelectRewardManager
{
public:

	KUserUnitSelectRewardManager(void);
	~KUserUnitSelectRewardManager(void);

	void Clear();
	void AddRewardItemInfo( IN const int iAddRewardID );
	void GetRewardAndClear( OUT std::vector< int >& vecRewardInfo );


	//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	void SetFirstSelectUnitRewardInfo( IN std::vector<KFirstSelectUnitReward>& vecInfo );
	bool GetFirstSelectUnitRewardInfo( IN int iServerGroupID, OUT std::vector<KFirstSelectUnitReward>& vecInfo );
	void UpdateFirstSelectUnitRewardInfo( IN int iDBIndexID, UidType iUnitUID );
	void DeleteAndGetFirstSelectUnitRewardInfo( IN int iDBIndexID, KFirstSelectUnitReward& kInfo );
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

private:
	std::vector< int >			m_vecAccessItem;			// ���ӽ� ������ ���� - ���� �е� ��� �����

	//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	std::map<int, KFirstSelectUnitReward>			m_mapFirstSelectUnitRewardInfo;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}
};


#endif SERV_SECOND_SECURITY
//}}