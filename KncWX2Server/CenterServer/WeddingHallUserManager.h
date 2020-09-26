#pragma once

#include "RoomUserManager.h"

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM

#define KWeddingHallUserManagerPtr KWeddingHallUserManager*
#undef KWeddingHallUserManagerPtr
SmartPointer( KWeddingHallUserManager );


class KWeddingHallUserManager : public KRoomUserManager
{
public:
    KWeddingHallUserManager();
    virtual ~KWeddingHallUserManager();

	//{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
	virtual void Reset( USERLIST_TYPE eType = UT_GAME );
	//}}

	// ���� ���
	void	SetMaxSlot( IN const int iMaxSlot ) { m_MaxSlot = iMaxSlot; }
	int		GetMaxSlot() const				{ return m_MaxSlot; }

	// Ȯ��
	bool	IsExistEmptySlot()				{ return ( GetMaxSlot() > GetNumMember() ); }

	// ���� ���� ó��
	bool	EnterRoom( IN const KRoomUserInfo& kInfo, IN const bool bConsiderTeam );
	virtual bool LeaveRoom( IN const UidType iUnitUID );

	// ����ȭ ó��
	bool	RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList );
	bool	RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iRegUnitUID );
	bool	UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList );
	bool	UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iUnregUnitUID );
	void	CheckAndGetIntrudeUnitLoadCompleteUserList( OUT std::vector< UidType >& vecNpcLoadUserList );

	bool	GetRoomUserList( std::map< UidType, KRoomUserPtr >& mapRoomUserList, USERLIST_TYPE eType = UT_GAME );
	void	GetRoomUserList( OUT std::vector< UidType >& vecUidList, USERLIST_TYPE eType = UT_GAME );

	void	GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, USERLIST_TYPE eType = UT_GAME );

	bool	IsExistGroomAndBride( USERLIST_TYPE eType = UT_GAME );

protected:
	int										m_MaxSlot;
};


#endif SERV_RELATIONSHIP_SYSTEM
//}

