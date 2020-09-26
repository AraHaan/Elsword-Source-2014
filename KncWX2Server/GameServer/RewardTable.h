#pragma once
#include "RefreshSingleton.h"
#include <map>
#include <vector>

#include "ClientPacket.h"


struct KItemInfo;
struct KPostItemInfo;
class KRewardTable
{
	DeclareRefreshSingleton( KRewardTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:

	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	enum EVENT_REWARD_ID
	{
#ifdef SERV_SECOND_SECURITY_REWARD_ID
		// �ؿ��� ������ ���� ������
#if defined( SERV_COUNTRY_JP )
		ERI_SECOND_SECURITY = 650,
#elif defined( SERV_COUNTRY_TWHK ) 
		ERI_SECOND_SECURITY = 716,
#elif defined( SERV_COUNTRY_US ) || defined( SERV_COUNTRY_CN )
		ERI_SECOND_SECURITY = 10135,
#elif defined( SERV_COUNTRY_EU )
		ERI_SECOND_SECURITY = 20219,
#else
		ERI_SECOND_SECURITY = 0,
#endif
#else // SERV_SECOND_SECURITY_REWARD_ID
		ERI_SECOND_SECURITY = 10135,       // 2�� ���� �ý��� ���� ����
#endif // SERV_SECOND_SECURITY_REWARD_ID

		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		ERI_COME_BACK_USER_TITLE				= 20150,	// Ÿ��ƲID  - �ٽ� ��� ��(�޸����� Īȣ)
		ERI_COME_BACK_USER_ITEM_CUBE_ONE		= 10143,	// reward
		ERI_COME_BACK_USER_ITEM_CUBE_TWO		= 10144,	// reward
		ERI_COME_BACK_USER_ITEM_CUBE_THREE		= 10145,	// reward
#endif SERV_COME_BACK_USER_REWARD
		//}}
#ifdef SERV_2013_JUNGCHU_TITLE
		ERI_2013_JUNGCHU_TITLE					= 50100,
#endif SERV_2013_JUNGCHU_TITLE

#ifdef SERV_STEAM_EVENT
		ERI_STEAM_EVENT_TITLE					= 35210, // ���ο� ����κ����� ���谡 Īȣ
#endif //SERV_STEAM_EVENT
#ifdef SERV_CREATE_CHUNG_REWARD_TITLE
		ERI_CREATE_CHUNG_REWARD_TITLE					= 70250, // �ϸ��� �Ͼ� ���� Īȣ
#endif //SERV_CREATE_CHUNG_REWARD_TITLE

#ifdef SERV_CREATE_NEW_CHARACTER_REWARD_TITLE
		ERI_CREATE_NEW_CHARACTER_REWARD_TITLE					= 35240, // ���ο� ������ ź��
#endif //SERV_CREATE_NEW_CHARACTER_REWARD_TITLE

#ifdef SERV_CHANNELING_AERIA_EVENT
		ERI_CHANNELING_AERIA_EVENT_TITLE		= 35230,
#endif SERV_CHANNELING_AERIA_EVENT
	};
#endif SERV_SECOND_SECURITY
	//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	struct KRewardData
	{	
		int				m_iRewardItemID;
		int				m_iQuantity;
		short			m_sPeriod;

		KRewardData(){};
		KRewardData( const KRewardData& t ) { *this = t; }    // copy constructor
		KRewardData& operator=( const KRewardData& t )       // assign operator
		{			
			m_iRewardItemID = t.m_iRewardItemID;
			m_iQuantity = t.m_iQuantity;
			m_sPeriod = t.m_sPeriod;

			return *this;
		}
	};
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
	

public:
	KRewardTable(void);
	~KRewardTable(void);

	// for lua
	bool AddRewardData_LUA( int iRewardID, int iItemID, int iQuantity, short sPeriod );

	void GetRewardItem( IN const KPostItemInfo& kPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const;
	void GetRewardItem( IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const;
	bool GetRewardInfo( IN int iRewardID, OUT std::map< int, int >& mapRewardInfo ) const;
	void GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, int >& mapRewardInfo ) const;
	void GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::vector< std::wstring >& vecMessage ) const;

	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	void SetOutUserRetainingRewardItemInfo_LUA( int iSlot, int iItemID, int iQuantity );
	void GetOutUserRetainingRewardItemInfo( std::map< int, KRewardItem >& mapRewardItem );
	bool GetOutUserRetainingSelectItemInfo( IN int& iSlot, OUT KRewardItem& kInfo );
	bool GetOutUserRetainingSelectItemInfo( IN int& iItemID, OUT int& iQuantity );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	bool AddLevelUpRewardData_LUA( u_char ucLevel, int iRewardID );
	bool GetLevelUpRewardItem( IN u_char& ucLevel, OUT std::vector< int >& vecRewardID );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	const std::map< int, std::vector< KRewardData > >& GetMapRewardDBData() const { return m_mapRewardDBData; }
	const std::map< int, std::vector< KRewardData > >& GetMapRewardScriptData() const { return m_mapRewardData; }

	const std::map< int, std::vector< KRewardData > >& GetMapTotalRewardData() const { return m_mapTotalRewardData; }

	void SetMapTotalRewardData(  IN const std::map< int, std::vector< KRewardData > > mapRewardScriptData, IN const std::map< int, std::vector< KRewardData > > mapRewardDBData  );
	void SetMapRewardDBData(std::map< int, std::vector< KRewardData > > mapRewardDBData) { m_mapRewardDBData = mapRewardDBData; }
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
	
private:
	std::map< int, std::vector< KRewardData > >		m_mapRewardData;

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	std::map< int, std::vector< KRewardData > >		m_mapTotalRewardData;
	std::map< int, std::vector< KRewardData > >		m_mapRewardDBData;
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	std::map< int, KRewardItem >					m_mapRewardItem;
#endif SERV_NEXON_SESSION_PROTOCOL
	//}}
	//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	std::map< u_char, std::vector< int > >			m_mapLevelUpRewardItem;
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}
};

DefRefreshSingletonInline( KRewardTable );
