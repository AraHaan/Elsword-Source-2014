#pragma once

#include "XSLStat.h"
#include "RefreshSingleton.h"

class CXSLMapData
{
	DeclareRefreshSingleton( CXSLMapData );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	struct sMAP_DATA
	{
		int					m_iID;

		int					m_iBeforeID;
		int					m_iAfterID;

		int					m_iRequireMinLevel;
		int					m_iRequireMaxLevel;

		std::vector< int >	m_vecRequireDungeonID;

		sMAP_DATA()
		{
			m_iID				= -1;
			m_iBeforeID			= -1;
			m_iAfterID			= -1;
			m_iRequireMinLevel	= -1;
			m_iRequireMaxLevel	= -1;
		}
	};

	//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	struct SLocalMapInfo
	{
		std::vector< KDungeonGameSimpleInfo >	m_vecDungeonInfo;
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
	CXSLMapData(void);
	virtual ~CXSLMapData(void);
	
	void	AddMapData_LUA();
	//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	AddLocalMapInfo_LUA( int iVillageMapID, int iDungeonMode, int iDungeonIDWithDif );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	int		GetDefaultMapID() { return m_kDefaultMapData.m_iID; }
	//{{ 2009. 3. 30  ������	���� mapid
	int		GetBeforeMapID( int iMapID );
	//}}
	//{{ 2012. 11. 13	������		���� ������ ���� �˻� �� ����
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int		GetPossibleEnterVillageMapID( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapClearInfo );
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}

	// �����ϴ� ��������.?
	bool	CheckMapData( const int iMapID );

	// �������� üũ
	//{{ 2012. 09. 21   ��μ�   ���� ���� ���� �޽��� ����
#ifdef SERV_ENTER_VILLAGE_ERROR
	bool	CheckRequireData( IN const int iMapID, IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapClearInfo, OUT int& iLimitLevel, OUT int& iRequireDungeonID );
#else
	bool	CheckRequireData( const int iMapID, u_char ucLevel, std::map< int, KDungeonClearInfo >& mapClearInfo );
#endif SERV_ENTER_VILLAGE_ERROR
	//}}
	
	//{{ 2008. 11. 5  ������	�ʵ� ����
	void	UpdateFieldInfo( int iMapID, UidType iFieldUID );
	bool	GetFieldUID( int iMapID, UidType& iFieldUID );
	bool	VerifyMapID( int iMapID );
	const std::map< int, UidType >& GetFieldInfo() const { return m_mapFieldInfo; }
	//}}

	//{{ 2012. 05. 16	��μ�       �ɸ��� ���ý� ������ ���� ��ID �� ��ȯ
#ifdef SERV_SELECT_UNIT_CHANGE_MAP_ID
	void	CheckMapID( IN unsigned char& ucLevel, IN OUT int& iMapID );
#endif SERV_SELECT_UNIT_CHANGE_MAP_ID
	//}}

	//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	const SLocalMapInfo* GetLocalMapList( IN const int iVillageMapID ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// lastposition packing and unpacking
	static void PackingLastPosition( IN const int iMapID, IN const u_char ucLastTouchLineIndex, IN const u_short usLastPosValue, OUT int& iLastPosition );
	static void UnpackingLastPosition( IN const int iLastPosition, OUT UINT& uiMapID, OUT u_char& ucLastTouchLineIndex, OUT u_short& usLastPosValue );

	static	int GetMapID( int iDungeonID );
	static bool IsDungeonGate( int iMapID );

	//{{ 2012. 10. 31	�ڼ���	�ں� �ͽ������� Ƽ�� �߰�
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
	int		CheckCOBOExpressTicketMapID( IN int iMapID );
	int		ComputeCOBOExpressTicketCost( IN int iSrcMapID, IN int iDesMapID, IN u_char ucLevel );
#endif SERV_ADD_COBO_EXPRESS_TICKET
	//}}

protected:
	const sMAP_DATA*	GetMapData( const int iMapID );

protected:
	std::map< int, sMAP_DATA >			m_mapMapData;
	sMAP_DATA							m_kDefaultMapData;
	
	//{{ 2008. 11. 5  ������	�ʵ� ����
	std::map< int, UidType >			m_mapFieldInfo;
	//}}
	//{{ 2012. 08. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM	
	std::map< int, SLocalMapInfo >		m_mapLocalMap;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

DefRefreshSingletonInline( CXSLMapData );


