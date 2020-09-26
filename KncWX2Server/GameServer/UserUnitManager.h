#pragma once
#include "Event.h"
#include "CommonPacket.h"


//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KUserUnitManager
{
public:
	KUserUnitManager(void);
	~KUserUnitManager(void);

	void Clear();
	void Init( IN OUT KGamePlayStatusContainer& kNewGamePlayStatus, IN const KRoomUserInfo& kRoomUserInfo );

	void GetGamePlayStatus( OUT KGamePlayStatusContainer& kContainer )		{ kContainer = m_kGamePlayStatusContainer; }
	void GetPartyMemberStatus( OUT KPartyMemberStatus& kPartyMemberStatus )	{ kPartyMemberStatus = m_kPartyMemberStatus; }
	void SetGamePlayStatusAndCheckChangedHpMp( IN const KGamePlayStatus& kNewGamePlayStatus, OUT bool& bChangedHpMp, OUT KPartyMemberStatus& kHpMpInfo );

	// �ʵ�� �����ϱ� ���� ����!
	void ReservedReturnToFieldInfo( IN const KReturnToFieldInfo& kInfo );
	bool GetAndDeleteResevedReturnToFieldInfo( OUT KReturnToFieldInfo& kInfo );

	//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
	void SetEnterCashShop( bool bEnter )		{ m_bEnterCashShop = bEnter; }
	bool GetEnterCashShop()						{ return m_bEnterCashShop; }
#endif SERV_VISIT_CASH_SHOP
	//}}

private:
	KGamePlayStatusContainer			m_kGamePlayStatusContainer;
	KPartyMemberStatus					m_kPartyMemberStatus;
	std::vector< KReturnToFieldInfo >	m_vecReservedReturnToFieldInfo;

	//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
	bool								m_bEnterCashShop;
#endif SERV_VISIT_CASH_SHOP
	//}}

};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}
