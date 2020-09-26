#pragma once

#include "SquareUser.h"

inline float GetDistance( VECTOR3& vPos1, VECTOR3& vPos2 )
{
	return sqrtf( powf( vPos2.x - vPos1.x, 2.0f ) + powf( vPos2.y - vPos1.y, 2.0f ) + powf( vPos2.z - vPos1.z, 2.0f ) );
}

class KSquareUserManager
{
public:
	KSquareUserManager(void);
	~KSquareUserManager(void);

	void			Init( int nMaxUser );
	void			Reset();
	KSquareUserPtr	GetUser( UidType nCID );
	DWORD			GetNumMaxUser(){ return m_dwMaxUser; }

	bool			AddUser( KSquareUserPtr spRoomUser );
	bool			DeleteUser( UidType nCID );
	int				DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector<UidType>& vecDelUserUID );   // �ش� ���Ӽ����� ���ӵ� ������ ��� �����. (���� �������� ����)

	bool			EnterSquare( const KSquareUserInfo& kInfo );
	bool			LeaveSquare( UidType nCID );

	bool			SetSyncPos( UidType nCID, VECTOR3& vSyncPos );

	void			GetUserList( OUT std::map< UidType, std::set< UidType > >& mapUidList );
	void			GetUserInfoList( std::vector<KSquareUserInfo>& vecUserInfo );
	int				GetNumMember(){ return static_cast<int>(m_mapSquareUser.size()); }
	bool			GetSortSyncPos( UidType nCID, std::vector<KSquareUserPtr>& vecUserList );
	bool			GetSquareUserGS( UidType nCID, UidType& nGSUID );

	//{{ 2009. 9. 23  ������	1������ĳ����
	bool			SetSquareUserInfo( IN const KSquareUserInfo& kInfo );
	//}}
	void			SetUserEquippedTitle( IN UidType nCID, IN short sEquippedTitleID );
	void			SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	bool			SetUnitPartyInfo( UidType nCID, bool bIsParty );

	bool			IsCreatePersonalShop();	//���λ����� �����Ҽ��ִ��� ����.
	char			GetPersonalShopState( IN UidType nCID );
	UidType			GetPersonalShopUID( IN UidType nCID );
	bool			SetPersonalShopData( IN UidType nCID, IN const KSquarePersonalShopInfo& kInfo, OUT KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kNot = KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT() );

	//{{ 2010. 8. 2	������	���� ��ũ ����ȭ
#ifdef SERV_SQUARE_SYNC
	bool			CompareSquareSyncPacket( IN UidType nCID, IN const KEGS_SQUARE_UNIT_SYNC_DATA_REQ& kInfo );
#endif SERV_SQUARE_SYNC
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	int				GetUserED( IN UidType nCID );
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	//Zombie unit funtion
	void ZU_CheckStart();
	void ZU_CheckEnd();
	bool ZU_Tick( std::vector<UidType>& vecZombieUnit );	//Tick()
	void ZU_Refresh( UidType nCID, u_short usEventID );		//ProcessEvent()

protected:
	KSquareUserPtr	MakeUser( const KSquareUserInfo& kInfo );

protected:
	std::map< UidType, KSquareUserPtr >   m_mapSquareUser;

	DWORD				m_dwMaxUser;

	bool				m_bIsCheckZU;	//zombie user check on/off
};
