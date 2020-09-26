#pragma once
#include "GSBattleFieldRoom.h"

// class
#define KBattleFieldListPtr KBattleFieldList*
#undef KBattleFieldListPtr
SmartPointer( KBattleFieldList );


//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldList
{
public:
	KBattleFieldList();
	virtual ~KBattleFieldList(void);

	KGSBattleFieldRoomPtr GetRoomInfo( IN const UidType iRoomUID, IN const bool bErrorLog = false );
	bool	IsExist( IN const UidType iRoomUID )			{ return m_mapList.find( iRoomUID ) != m_mapList.end(); }
	bool	AddRoomInfo( IN const KBattleFieldRoomInfo& kInfo );
	bool	DelRoomInfo( IN const UidType iRoomUID );
	//{{ 2012. 11. 20	������		��Ʋ�ʵ� �� ���� ��� �߰�
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
	void	DelRoomInfoByCnServerUID( IN const UidType iCnServerUID );
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
	//}}
	bool	UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo );
	
	// ���ǿ� ���� ���� ������ �ʵ�� �̵��ϱ� ���� �Լ�
	bool	GetRoomUIDForJoinBattleField( IN const UidType iUnitUID, IN const UidType iTargetRoomUID, IN const UidType iPartyUID, OUT UidType& iJoinBFRoomUID );

protected:
	UidType GetBattleFieldRoomUID_MaxPartyMemberCount( IN const UidType iUnitUID, IN const UidType iPartyUID );
	UidType GetBattleFieldRoomUID_TargetRoom( IN const UidType iTargetRoomUID );
	UidType GetBattleFieldRoomUID_AllRandom();

protected:
	std::vector< KGSBattleFieldRoomPtr >		m_vecList;
	std::map< UidType, KGSBattleFieldRoomPtr >	m_mapList;
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}