#pragma once
#include "Event.h"
#include "CommonPacket.h"
//{{ 2011. 11. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleField.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//{{ 2011. 01. 20	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW


class KRoomMonsterManager
{
public:

	//{{ dmlee 2008.11.13 PVP monster, ���� dungeonroom.h�� �ִ� �� ����� �ű�
	//////////////////////////////////////////////////////////////////////////
	//������ npc data
	//
	//�������::
	//1: �Ϲݾ����� / exp :	m_bNoDrop == false
	//2: �̺�Ʈ������ :		m_bNoDrop == false && m_bActive == true
	struct NPC_DATA
	{
		UidType	m_uiAttUnit;	//������ user unit(������ ���� �޴´�.)
		int		m_iNPCID;
		char	m_cLevel;
		bool	m_bNoDrop;
		bool	m_bActive;
		bool	m_bIsBoss;
		//{{ 2009. 12. 18  ������	��������ġ����
		char	m_cMonsterGrade;
		//}}
		//{{ 2011. 11. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		int		m_iGroupID;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		int		m_iBossGroupID;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		NPC_DATA()
		{
			m_uiAttUnit		= 0;
			m_iNPCID		= 0;
			m_cLevel		= 0;
			m_bNoDrop		= true;
			m_bActive		= false;
			m_bIsBoss		= false;
			//{{ 2009. 12. 18  ������	��������ġ����
			m_cMonsterGrade = 0;
			//}}
			//{{ 2011. 11. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			m_iGroupID		= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			m_iBossGroupID  = 0;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
		}
	};
	//}} dmlee 2008.11.13 PVP monster, ���� dungeonroom.h�� �ִ� �� ����� �ű�

public:
    KRoomMonsterManager();
    virtual ~KRoomMonsterManager();

	void	StartGame();
	void	EndGame();
	void	OnCloseRoom();

	// üũ �Լ�
	bool	GetNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
	bool	IsExistMonster( IN const int iNpcUID ) const;
	bool	IsExistMonster( IN const int iNpcUID, OUT bool& bIsDie ) const;
	bool	IsMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveNpcList.find( iNpcUID ) != m_mapAliveNpcList.end() ); }
	//bool	IsMonsterDie( IN const int iNpcUID ) const		{ return ( m_mapDieNpcList.find( iNpcUID ) != m_mapDieNpcList.end() ); }
	//{{ 2013. 04. 16	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool	IsAllBossMonstersDead() const;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	int		GetAliveMonsterCount() const					{ return static_cast<int>(m_mapAliveNpcList.size()); }
	int		GetDieMonsterCount() const						{ return static_cast<int>(m_mapDieNpcList.size()); }
	void	GetAliveMonsterCountInfo( OUT CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterInfo ) const;
	bool	GetMonsterGroupID( IN const int iNpcUID, OUT int& iGroupID ) const;

	// ����
	bool	CreateMonster( IN const int iNpcUID, IN const KNPCUnitReq& kNpcInfo );
	bool	CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID, IN const int iNpcUID = 0 );

	// ���� ó��
	virtual bool SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID, IN const bool bInsertDieList = true );
	bool	SetMonsterRebirth( IN const int iNpcUID );
	
	//////////////////////////////////////////////////////////////////////////
	void PrintLuaLog_LUA( int iType, double fValue );	
	//////////////////////////////////////////////////////////////////////////
protected:
	int							m_nNPCUID;				// NPC Unit UID �߱�
	std::map< int, NPC_DATA >	m_mapAliveNpcList;		// ���� ������ ����	[key:NpcUID,	 value:NPC_DATA]
	std::map< int, NPC_DATA >	m_mapDieNpcList;		// ���� ���� ����Ʈ	[key:NpcUID,	 value:NPC_DATA]

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
protected:
	void InsertReservedNpcUID( int iNpcUID ) { m_setReservedNpcUID.insert( iNpcUID ); }
	void DeleteReservedNpcUID( int iNpcUID ) { m_setReservedNpcUID.erase( iNpcUID ); }
	int GetNewNpcUID();

private:
	std::set< int > m_setReservedNpcUID;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
};


#endif SERV_DUNGEON_RANK_NEW
//}}


