#pragma once 

#include "Event.h"
#include "CommonPacket.h"
#include "SectorID.h"
//{{ 2010. 01. 15  ������	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
	#include <ObjectPool/BoostAllocator.h>
#endif SERV_BOOST_ALLOCATOR
//}}

#define KFieldUserPtr KFieldUser*
#undef KFieldUserPtr
SmartPointer( KFieldUser );

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BOOST_ALLOCATOR
BOOST_ALOC_CLASS( KFieldUser )
#else
class KFieldUser
#endif SERV_BOOST_ALLOCATOR
//////////////////////////////////////////////////////////////////////////
{
public:
	KFieldUser(void);
	virtual ~KFieldUser(void);

	void	GetFieldUserInfo( KFieldUserInfo& kInfo )						{ kInfo = m_kFieldUserInfo; }
	void	SetFieldUserInfo( const KFieldUserInfo& kInfo );

	const KCommunityUserInfo& GetCommunityUserInfo()						{ return m_kCommunityUserInfo; }

	// ��Ƽ ����Ʈ	
	bool    IsParty( UidType iUnitUID )                                     { return ( m_setPartyUIDList.find( iUnitUID ) != m_setPartyUIDList.end() ); }	
    void    GetParty( std::set< UidType >& setPartyList )                   { setPartyList = m_setPartyUIDList; }
    void    SetParty( const std::set< UidType >& setPartyList )             { m_setPartyUIDList = setPartyList; }	
	void	SetPartyUID( UidType iPartyUID )								{ m_kFieldUserInfo.m_iPartyUID = iPartyUID; m_kCommunityUserInfo.m_iStateCode = iPartyUID; }
	
	// ģ�� ����Ʈ
	bool	IsFriend( UidType iUnitUID )									{ return ( m_setFriendUIDList.find( iUnitUID ) != m_setFriendUIDList.end() ); }
	void    GetFriend( std::set< UidType >& setFriendList )					{ setFriendList = m_setFriendUIDList; }
	void    SetFriend( const std::set< UidType >& setFriendList )			{ m_setFriendUIDList = setFriendList; }

	//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	void	SetPetInfo( IN const std::vector< KPetInfo >& vecPetInfo );
#endif SERV_PET_SYSTEM
	//}}

	// �ʵ� �ɼ�
	void	SetFieldOption( const KFieldOption& kFieldOption )				{ m_kFieldOption = kFieldOption; }	
	KFieldOption::LIMIT_OPTION GetLimitOption()								{ return static_cast< KFieldOption::LIMIT_OPTION >( m_kFieldOption.m_sLimitOption ); }

	UidType	GetCID()														{ return m_kFieldUserInfo.m_iUnitUID; }

	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	void	SetUserEquippedTitle( int iEquippedTitleID )					{ m_kFieldUserInfo.m_iTitleID = iEquippedTitleID; }
#else
	void	SetUserEquippedTitle( short sEquippedTitleID )					{ m_kFieldUserInfo.m_sTitleID = sEquippedTitleID; }
#endif SERV_TITLE_DATA_SIZE
	//}}	
	void	SetUserEquipItem( std::vector< KInventoryItemInfo >& vecInventorySlotInfo );

	// ���� ����	
	int		GetMySectorID()													{ return m_iSectorID; }
    void    SetMySectorID( SectorID iSectorID )                             { m_iSectorID = iSectorID; }
	SectorsPtr GetAdjacentSectors()                                         { return m_spAdjacentSectors; }
    void    SetAdjacentSectors( SectorsPtr spSectors )                      { m_spAdjacentSectors = spSectors; }

	//{{ 2008. 12. 9  ������	���� ��ε�ĳ����
	bool	IsEventRecvUser( UidType iUnitUID )								{ return ( m_setEventRecvUIDList.find( iUnitUID ) != m_setEventRecvUIDList.end() ); }
	bool	AddEventRecvUser( UidType iUnitUID )							{ if( IsMaxEventRecvUser() ) return false; if( iUnitUID == GetCID() ) return false; return ( m_setEventRecvUIDList.insert( iUnitUID ) ).second; }
	bool	DelEventRecvUser( UidType iUnitUID )							{ return ( m_setEventRecvUIDList.erase( iUnitUID ) >= 1 ); }
	bool	IsMaxEventRecvUser()											{ return ( static_cast< u_int >( m_setEventRecvUIDList.size() ) >= static_cast< u_int >( m_kFieldOption.m_sLimitOption) ); }
	//}}

    bool    IsEventSendUser( UidType iUnitUID )                             { return ( m_setEventSendUIDList.find( iUnitUID ) != m_setEventSendUIDList.end() ); }
    bool    AddEventSendUser( UidType iUnitUID )                            { return ( m_setEventSendUIDList.insert( iUnitUID ) ).second; }
    bool    DelEventSendUser( UidType iUnitUID )                            { return ( m_setEventSendUIDList.erase( iUnitUID ) ) >= 1; }
    void    GetEventSendUser( std::set< UidType >& setSendUser )            { setSendUser = m_setEventSendUIDList; }

	void	UpdateUserPos( const VECTOR3& UserPos )                         { m_kFieldUserInfo.m_vPos = UserPos; }
    void    GetBroadcastDestination( std::vector< UidType >& vecDestination );

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	void	UpdateUserED( int iED )											{ m_kCommunityUserInfo.m_iED = iED; }
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	void	SetRidingPetInfo( IN const UidType iRidingPetUID, IN const USHORT usRidingPetID )
	{
		m_kFieldUserInfo.m_iRidingPetUID = iRidingPetUID;
		m_kFieldUserInfo.m_usRidingPetID = usRidingPetID;
	}
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	void	SetLoverUnitUID( UidType iUnitUID )											{ m_kFieldUserInfo.m_iLoverUnitUID = iUnitUID; }
#endif SERV_RELATIONSHIP_SYSTEM
	//}

protected:
    KFieldUserInfo      m_kFieldUserInfo;
	//{{ 2009. 1. 15  ������	�ʵ� ���� ����Ʈ
	KCommunityUserInfo	m_kCommunityUserInfo;
	//}}

    SectorID            m_iSectorID;                // �ڽ��� �ҼӵǾ��ִ� ����
    SectorsPtr          m_spAdjacentSectors;        // �ҼӼ����� �ֺ�����

    //{{ 2008. 12. 9  ������ ���� ��ε�ĳ����
    std::set< UidType > m_setEventRecvUIDList;
    std::set< UidType > m_setEventSendUIDList;
    std::set< UidType > m_setPartyUIDList;
	std::set< UidType > m_setFriendUIDList;
    KFieldOption        m_kFieldOption;
    //}}
};


