#pragma once

#include "Room/RoomManager.h"
#include "CnBaseRoom.h"
#include "X2Data/XSLRoom.h"
//{{ 2010. 03. 08  ������	TickDumper
#ifdef SERV_ROOM_TICK
	#include "TickDumper.h"
#endif SERV_ROOM_TICK
//}}

#include "CnRMEventProcess.h"

//{{ 2008. 12. 3  ������	Ŭ���� ������� ���� �޸� ���� ���� �ذ�
#pragma pack( push, 1 )
//}}

class KCnRoomManager : public KRoomManager
{
	NiDeclareRTTI;
	DeclDump;
	DeclToStringW;

public:
	KCnRoomManager();
	virtual ~KCnRoomManager();

	virtual void Run();     // derived from KThread

	bool Init();
	void Tick();
	void RegToLua();	

	virtual void ShutDown();

	void SetNumRoom( int iRoomType, DWORD dwNumRoom );
	void SetNumPvPRoom( DWORD dwNumRoom );
	void SetNumDungeonRoom( DWORD dwNumRoom );
	void SetNumSquare( DWORD dwNumRoom );
	void SetNumTrade( DWORD dwNumRoom );
	void SetNumTrainingCenterRoom( DWORD dwNumRoom );
	void SetNumPersonalShop( DWORD dwNumRoom );	//08.02.12 hoons : �水ü��뿡 ���� ������ �������� �Ŵ����� ���� ���Ϸ��� �����غ���.	
	//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void SetNumBattleField( DWORD dwNumRoom );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	void SetNumWeddingHall( DWORD dwNumRoom );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	void PrepareSquare( std::vector< KSquareCreateInfo >& vecSquareCreateInfo );

	DWORD GetNumEmptyRoom( int iRoomType ) const;

	UidType OpenRoom( CXSLRoom::ROOM_TYPE eRoomType );
	bool CloseRoom( UidType iRoomUID );

	// derived from KRoomManager
	virtual void QueueingEventToRoom( const KEventPtr& spEvent );
	virtual void QueueingToAll( int iRoomType, const KEventPtr& spEvent );
	virtual void QueueingToAll( const KEventPtr& spEvent );

	//{{ 2010. 06. 17  ������	������ ����
	void QueueingEventToRoomManager( const KEventPtr& spEvent );
	//}}

	//����� �Է����� ���� ������ �ϱ�����
	void OpenSquare_Lua( int iSquareType, char* szSquareName );

	//Zombie user ����.
	void	SetZUTickTime( double dfTickTime );	
	double	GetZUTickTime();
	//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetBattleFieldZUTickTime( double dfTickTime );
	double	GetBattleFieldZUTickTime() const				{ return m_dfBattleFieldZUTickTime; }
	double	GetBattleFieldZUEchoTime() const				{ return m_dfBattleFIeldZUEchoTime; }

#endif SERV_BATTLE_FIELD_SYSTEM
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    void    SetBattleFieldZombieAlertTickTime( double dfTickTime );
    double  GetBattleFieldZombieAlertTickTime() const       { return m_dfBattleFieldZombieAlertTickTime; }
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	//}}

	//{{ 2011. 12. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	DumpRoomMonster_LUA( const char* pUserNickName, int iNpcUID );
	void	DumpBattleField_LUA( const char* pUserNickName );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	void	KillZombieUser_LUA( const char* pUserNickName );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}

protected:
	// packet send function
	template < class T >
	void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID );

	FSMclassPtr CreateRoomFSM( int iRoomType );
	KCnBaseRoomPtr CreateRoom( int iRoomType );

	KCnBaseRoomPtr Get( UidType iRoomUID );

protected:
	std::vector< KSquareCreateInfo >		m_vecSquareCreateInfo;

	std::map< UidType, KCnBaseRoomPtr >		m_mapRoomList[CXSLRoom::RT_TOTAL_NUM];
	//{{ 2012. 04. 18	������	�� �Ŵ��� �����ս� ���
#ifdef SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
	std::vector< KCnBaseRoomPtr >			m_vecRoomList[CXSLRoom::RT_TOTAL_NUM];
#endif SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
	//}}
	std::map< UidType, UidType >			m_mapEmptyRoomList[CXSLRoom::RT_TOTAL_NUM];
	DWORD									m_dwNumRoom[CXSLRoom::RT_TOTAL_NUM];
	FSMclassPtr                             m_spRoomFSM[CXSLRoom::RT_TOTAL_NUM];
	mutable KncCriticalSection				m_csEmptyRoomList[CXSLRoom::RT_TOTAL_NUM];	// �� �� ����Ʈ ��ȣ.

	double									m_dfZUTickTime;
	//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	double									m_dfBattleFieldZUTickTime;
	double									m_dfBattleFIeldZUEchoTime;

#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    double									m_dfBattleFieldZombieAlertTickTime;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	//{{ 2010. 06. 17  ������	��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_REALTIME_SCRIPT
	KCnRMEventProcess						m_kRMEventProcess;
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2010. 03. 08  ������	TickDumper
#ifdef SERV_ROOM_TICK
	KTickDumper								m_kRoomManagerTickDump;
#endif SERV_ROOM_TICK
	//}}
	//{{ 2011. 03. 24	������	���� ���� üũ
#ifdef SERV_ROOM_TICK_DUMP
	KMultiTickDumper						m_kRoomTickDump;
#endif SERV_ROOM_TICK_DUMP
	//}}
};

DefKObjectInline( KCnRoomManager, KRoomManager );

template < class T >
void KCnRoomManager::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_CN_ROOM, 0, PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}

//{{ 2008. 12. 3  ������	Ŭ���� ������� ���� �޸� ���� ���� �ذ�
#pragma pack( pop )
//}}

/*    64 bit ( 8 byte )

| ff ff ff ff | ff ff ff ff |

| 00 00 00 ff | ff ff ff ff | ���� uid ����. ( 5byte = 40bit. 2^40 = 1 0995 1162 7776 )
| 80 00 00 00 | 00 00 00 00 | ��ȣ��Ʈ�� ���ܵ�
| 40 00 00 00 | 00 00 00 00 | ���ÿ��� �ӽ÷� ����Ǵ� uid.
| 3f 00 00 00 | 00 00 00 00 | ������ uid.
| 00 ff 00 00 | 00 00 00 00 | �� ���� uid.
| 00 00 f0 00 | 00 00 00 00 | reserved.
| 00 00 0f 00 | 00 00 00 00 | room type.

*/