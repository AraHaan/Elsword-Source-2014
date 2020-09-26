#pragma once

#include "FSM/support_FSM.h"
#include <boost/timer.hpp>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "X2Data/XSLSquareUnit.h"

#define KSquareUserPtr KSquareUser*
#undef KSquareUserPtr
SmartPointer( KSquareUser );

class KSquareUser
{
public:
	KSquareUser(void);
	virtual ~KSquareUser(void);

	void		GetSquareUserInfo( KSquareUserInfo& kInfo )			{ kInfo = m_kSquareUserInfo; }
	void		SetSquareUserInfo( const KSquareUserInfo& kInfo )	{ m_kSquareUserInfo = kInfo; }

	UidType		GetGSUID()											{ return m_kSquareUserInfo.m_iGSUID; }
	UidType		GetCID()											{ return m_kSquareUserInfo.m_iUnitUID; }

	void		SetSyncPos( VECTOR3& vSyncPos )						{ m_vSyncPos = vSyncPos; }
	VECTOR3&	GetSyncPos()										{ return m_vSyncPos; }

	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	void		SetUserEquippedTitle( int iEquippedTitleID )		{ m_kSquareUserInfo.m_iTitleID = iEquippedTitleID; }
#else
	void		SetUserEquippedTitle( short sEquippedTitleID )		{ m_kSquareUserInfo.m_sTitleID = sEquippedTitleID; }
#endif SERV_TITLE_DATA_SIZE
	//}}	
	void	    SetUserEquipItem( std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	void		SetUnitPartyInfo( bool bIsParty )					{ m_kSquareUserInfo.m_bIsParty = bIsParty; }

	//���λ��� ����.
	void		SetPersonalShopData( const KSquarePersonalShopInfo& kInfo, OUT KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kNot );
	char		GetPersonalShopState(){ return m_kSquareUserInfo.m_cPersonalShopState; }
	UidType		GetPersonalShopUID(){ return m_iPersonalShopUID; }

	//{{ 2010. 8. 2	������	���� ��ũ ����ȭ
#ifdef SERV_SQUARE_SYNC
	bool		CompareSquareSyncPacket( IN const KEGS_SQUARE_UNIT_SYNC_DATA_REQ& kInfo );
#endif SERV_SQUARE_SYNC
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	int			GetED()												{ return m_kSquareUserInfo.m_iED; }
	void		SetED( IN int& iED )								{ m_kSquareUserInfo.m_iED = iED; }
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	//zombie unit funtion
	bool ZU_Tick();
	void ZU_Refresh();


protected:
	KSquareUserInfo					m_kSquareUserInfo;
	VECTOR3							m_vSyncPos;			//�ڽ��� ��ġ ������

	//KSquarePersonalShopInfo		m_kSquarePersonalShopData;
	UidType							m_iPersonalShopUID;

	//{{ 2010. 8. 2	������	���� ��ũ ����ȭ
#ifdef SERV_SQUARE_SYNC
	KEGS_SQUARE_UNIT_SYNC_DATA_REQ	m_kSquareSyncPacket;
#endif SERV_SQUARE_SYNC
	//}}

	//Zombie user üũ�� ���� ����
	boost::timer					m_TimerZU;
};



