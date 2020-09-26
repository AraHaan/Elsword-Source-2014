#pragma once

#include "PersonalShopUser.h"

class KPersonalShopUserManager
{
public:
	KPersonalShopUserManager(void);
	~KPersonalShopUserManager(void);

	void Init( IN int nMaxUser );
	void Reset();
	KPersonalShopUserPtr GetUser( IN UidType nCID );
	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	bool IsExistUnitUID( IN const UidType nCID ) { return ( m_mapPersonalShopUser.find( nCID ) != m_mapPersonalShopUser.end() ); }
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}	

	bool AddUser( IN KPersonalShopUserPtr spRoomUser );
	bool DeleteUser( IN UidType nCID );
	int  DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector< UidType >& vecDelUserUID );   // �ش� ���Ӽ����� ���ӵ� ������ ��� �����. (���� �������� ����)

	bool EnterPersonalShop( IN const KPersonalShopUserInfo& kInfo );
	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	bool EnterPShopAgencyOwner( IN const KPersonalShopUserInfo& kInfo );
	bool EnterPShopAgencyCustomer( IN const KPersonalShopUserInfo& kInfo );
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}
	bool LeavePersonalShop( IN UidType nCID );

	//{{ 2011. 04. 14	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	bool CreatePersonalShopAgency( IN const KPersonalShopUserInfo& kInfo );
	bool DeletePersonalShopAgency( IN UidType nCID );
#endif SERV_PSHOP_AGENCY
	//}}

	int  GetNumMember(){ return static_cast<int>( m_mapPersonalShopUser.size() ); }
	int  GetMaxMember(){ return static_cast<int>( m_dwMaxUser ); }

	bool GetPersonalShopUserGS( UidType nCID, UidType& nGSUID );
	void GetUserList( OUT std::map< UidType, std::set< UidType > >& mapUidList ); // Server UID ���� ���� User UID.
	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	void GetUserList( OUT std::vector< UidType >& vecUidList );
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

	bool IsHost( UidType nCID );
	bool GetHostUID( UidType& nUID );
	bool GetHostNickName( std::wstring& wstrNickName );
	//{{ 2011. 04. 14	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	UidType GetPShopAgencyHostUnitUID() const;
	UidType GetPShopAgencyHostGSUID() const;
	bool GetPShopAgencyHostNickName( OUT std::wstring& wstrNickName ) const;
	bool IsPShopAgencyHost( IN const UidType iUnitUID ) const;
	bool IsExistPShopAgency() const	{ return ( m_spPShopAgencyUser != NULL ); }
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2009. 2. 10  ������	���ΰŷ� ���� ����
	bool SetUpdateTemp( UidType nCID, const KItemQuantityUpdate& kUpdateTemp, const std::vector< KItemInfo >& vecItemInfoTemp, int iEDTemp );
	bool GetUpdateTemp( UidType nCID, KItemQuantityUpdate& kUpdateTemp, std::vector< KItemInfo >& vecItemInfoTemp, int& iEDTemp );
	//}}

	//{{ 2010. 05. 22  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	bool IsHostUserSameServerGroup( IN UidType iUnitUID );
#endif SERV_INTEGRATION
	//}}
	//{{ 2011. 04. 14	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	bool IsPShopAgencyHostUserSameServerGroup( IN UidType iUnitUID );
#endif SERV_PSHOP_AGENCY
	//}}

#ifdef DEF_TRADE_BOARD
	bool IsSquare( UidType nCID );
#endif DEF_TRADE_BOARD

protected:
	KPersonalShopUserPtr	MakeUser( IN const KPersonalShopUserInfo& kInfo, IN const bool bForce = false );

protected:
	std::map< UidType, KPersonalShopUserPtr >	m_mapPersonalShopUser;
	//{{ 2011. 04. 14	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	KPersonalShopUserPtr						m_spPShopAgencyUser;
#endif SERV_PSHOP_AGENCY
	//}}

	DWORD										m_dwMaxUser;
};


