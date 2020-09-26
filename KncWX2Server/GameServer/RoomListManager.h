#pragma once

#include "RoomList.h"
#include <KNCSingleton.h>
#include <ToString.h>

class KRoomListManager
{
	DeclareSingleton( KRoomListManager );
	DeclDump;
	DeclToStringW;

	enum
	{
		PVP_PAGE_NUM	 = 12,
		SQUARE_PAGE_NUM	 = 6,
	};

public:
	KRoomListManager(void);
	~KRoomListManager(void);

	bool			OpenScriptFile( const char* pFileName );
	
	bool			OpenPVPRoomList( IN KGSUserPtr spUser, OUT KEGS_OPEN_PVP_ROOM_LIST_ACK& kInfo );
	bool			ChangePVPRoomList( IN KGSUserPtr spUser, IN UINT uiRoomListID );

	//gsuser���� ������ ���	
	KRoomList::ROOM_LIST_TYPE GetRoomListType( IN UINT uiRoomListID );

	int				GetPVPChannelClassByRoomListID( IN UINT uiRoomListID );
	int				GetPVPChannelClassByRoomUID( IN UidType iRoomUID );
	UidType			GetWaitingRoomUID( IN UINT uiRoomListID, IN KEGS_QUICK_JOIN_REQ& kReq );

	bool			GetRoomInfoPage( IN UINT uiRoomListID, IN KEGS_ROOM_LIST_REQ& kReq, OUT KEGS_ROOM_LIST_ACK& kAck );
	bool			GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq, OUT KEGS_SQUARE_LIST_ACK& kAck );	

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	bool			AddRoomInfo( const KRoomInfo& kRoomInfo );
	bool			ChangeRoomInfo( const KRoomInfo& kRoomInfo );
	KRoomInfoPtr	GetRoomInfo( const UidType nRoomUID );
	bool			DelRoomInfo( const KRoomInfo& kRoomInfo );
	int				DelRoomInfo( const UidType nCenterUID );	//���ͼ����� ������� �ش缾�Ϳ� ��������� ���� �����Ѵ�.

	//////////////////////////////////////////////////////////////////////////
	// �������� ����
	bool			AddSquareInfo( const KSquareInfo& kSquareInfo );
	bool			ChangeSquareInfo( const KSquareInfo& kSquareInfo );
	KSquareInfoPtr	GetSquareInfo( const UidType iSquareUID );
	bool			DelSquareInfo( const KSquareInfo& kSquareInfo );
	int				DelSquareInfo( const UidType iCenterUID );

	//////////////////////////////////////////////////////////////////////////
	// ����� �����ڿ��� ��������
	void            GetNumPvPRoom( OUT std::map< int, int > &mapNumPvPRoom );
	void            ChangeNumPvPRoom( int iGameMode, int iChange );

	//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	UINT			GetPvpOffcialRoomListID() const		{ return m_nPvpOffcialRoomListID; }
#endif SERV_PVP_NEW_SYSTEM
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
	UINT			GetTournamentPVPID( void ) const{	return m_uiTournamentPVPID;	}
#endif	// SERV_HERO_PVP_MANAGE_LIST

private:
	KRoomListPtr	Get( UINT uiRoomListID );
	KRoomListPtr	GetPVPFreeChannel();

	bool			AddRoomListTemplet_LUA();
	void			SetStartPVPID_LUA( UINT nID )		{ m_nStartPVPID = nID; }

	//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	void			SetPvpOffcialRoomListID_LUA( UINT nID );	
#endif SERV_PVP_NEW_SYSTEM
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
	void			SetTournamentPVPID_LUA( UINT nID )		{ m_uiTournamentPVPID = nID; }
#endif	// SERV_HERO_PVP_MANAGE_LIST
	
	bool			InsertRoomInfo( IN UINT uiChannelID, IN KRoomInfoPtr spRoomInfo );
	bool			DeleteRoomInfo( IN UINT uiChannelID, IN KRoomInfoPtr spRoomInfo );

private:
	// �� ����Ʈ�� ������ �����̳�
	std::map< UINT, KRoomListPtr >		m_mapRoomList;  // [key : RoomListID]
	KSquareListPtr						m_spSquare;

	// �� ������ ���� ������ �����̳�
	std::map< UidType, KRoomInfoPtr >	m_mapRoomInfoList;
	std::map< UidType, KSquareInfoPtr >	m_mapSquareInfoList;

	UINT								m_nStartPVPID;
#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
	UINT								m_uiTournamentPVPID;
#endif	// SERV_HERO_PVP_MANAGE_LIST

	//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	UINT								m_nPvpOffcialRoomListID;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	// ��ڿ��� �Ѱ��ִ� ���� ������ PvP Room ������ Dungeon Room ������ �Ѱ��ش�.
	std::map< int, int >                m_mapNumPvPRoom;
};

DefSingletonInline( KRoomListManager );



