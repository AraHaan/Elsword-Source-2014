#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"

#ifdef SERV_CUBE_IN_ITEM_MAPPING
struct RANDOMITEM_MAPPING_DATA
{
	int		 iCubeID;
	int		 iKeyItemID;
	int		 iBeforeGroupID;
	int		 iAfterGroupID;
	bool	 bCheckMapping;

	RANDOMITEM_MAPPING_DATA()
	{
		iCubeID				= 0;
		iKeyItemID			= 0;
		iBeforeGroupID		= 0;
		iAfterGroupID		= 0;
		bCheckMapping		= false;
	}
};

struct RANDOMITEM_MAPPING_TIME
{
	CTime	 tStartTime;
	CTime	 tEndTime;
};
#endif SERV_CUBE_IN_ITEM_MAPPING

class CXSLRandomItemManager
{
	DeclareRefreshSingleton( CXSLRandomItemManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
public:

	enum RANDOM_ITEM_ID
	{
#ifdef SERV_COUNTRY_JP	
        RID_ATTRACTION_ITEM					= 65000031,	// �ӱԼ� �Ϻ� ���� 108900
		RID_ATTRACTION_ITEM_EVENT			= 60000702, // �ӱԼ� �Ϻ� ���� 108901
#else //SERV_COUNTRY_JP
        RID_ATTRACTION_ITEM					= 108900,
		RID_ATTRACTION_ITEM_EVENT			= 108901,
#endif //SERV_COUNTRY_JP
		RID_ATTRACTION_ITEM_ARCHANGEL_CHUNG	= 108902,   // û ��ũ���� - ���ε� ���� ������
		RID_ATTRACTION_ITEM_SALVATOR_CHUNG	= 160259,   // û ����丣 - ���ε� ���� ������
		RID_ATTRACTION_ITEM_OFFICER_CHUNG	= 160286,   // û �屳 - ���ε� ���� ������(�ַ�����)
		RID_ATTRACTION_ITEM_OFFICER			= 160287,   // �屳 - ���ε� ���� ������(���̾ƿ�)
		RID_ATTRACTION_ITEM_NASOD_BATTLE_CHUNG	= 160324,   // û ���ҵ� ��Ʋ - ���ε� ���� ������(�ַ�����)
		RID_ATTRACTION_ITEM_NASOD_BATTLE	= 160325,   // ���ҵ� ��Ʋ - ���ε� ���� ������(���̾ƿ�)
		RID_ATTRACTION_ITEM_ROYAL_SERVENT_CHUNG		= 160377,		// ���ε� ���� ������(û �ο� ����Ʈ)
		//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		RID_ATTRACTION_ITEM_GOLD			= 135115,   // ���ε� ���� ������(GOLD)
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
		//{{ 2012. 09. 04	�ڼ���	���ε� ���� ������(û ��ũ����)
#ifdef SERV_ATTRACTION_ITEM_ARCHDEVIL
		RID_ATTRACTION_ITEM_ARCHDEVIL_CHUNG	= 160559,   // û ��ũ���� - ���ε� ���� ������
#endif SERV_ATTRACTION_ITEM_ARCHDEVIL
		//}}
		//{{ 2012. 11. 14	�ڼ���	���ε� ���� ������(û ���� ���丮�� ����)
#ifdef SERV_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
		RID_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD	= 160597,   // ���ε� ���� ������(û ���� ���丮�� ����)
#endif SERV_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
		//}}
		// �� ť���� ��쿡�� ������ ũ����Ż�� ���ʽ��� �������� �ʴ´�. ( ��Ÿ ť���� ��쿡�� �����ϰ� ������ �� ���� ����� �� )
		RID_ATTRACTION_ITEM_9_10_ENCHANT_ITEM	= 160576,   // ��ȭ�� ���� Lv.9~10ť��
		RID_2012_WINTER_VACATION_LOTTO_CUBE		= 160630,	// �ܿ� ���� ��� ť��
		RID_ATTRACTION_ITEM_ARCHANGEL_ARA		= 160643,   // ���ε� ���� ������(�ƶ� ��ũ����)	- �ڼ���
		RID_ATTRACTION_ITEM_ELSWORD_5YEAR_CUBE	= 160644,   // ���ҵ� 5�ֳ� ��� ť��	- �ڼ���
		RID_ATTRACTION_ITEM_2013_D4_CUBE		= 160645,   // 2013�� D-4 ���� ��ǰ ���� ť��	- �ڼ���
		RID_ATTRACTION_ITEM_2013_D2_CUBE		= 160647,   // 2013�� D-2 ĳ�� �׼����� ���� ť��	- �ڼ���
		RID_ATTRACTION_ITEM_2012_GOODBYE_CUBE	= 160648,   // �¹��� 2012�� ť��	- �ڼ���
		
		//{{ 2012. 12. 28  �̴� ���� ������ ������ - ��μ�
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		RID_MINI_ATTRACTION_ITEM				= 160656,	// �̴� ���ε� ���� ������
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
		//}}
		//{{ 2012. 10. 08  �ҷ��� ������
#ifdef SERV_HALLOWEEN_ICE_HEATER
		RID_HALLOWEEN_ICE_HEATER_ITEM			= 85003498,	// �ҷ��� ������ ��ȥ
		RID_SHINE_HALLOWEEN_ICE_HEATER_ITEM		= 85003500,	// ������ �ҷ��� ������ ��ȥ
#endif SERV_HALLOWEEN_ICE_HEATER
		//}}
		RID_ATTRACTION_ITEM_EVELUN_ARA			= 160666,   // ���ε� ���� ������(�ƶ� ����丣 �̺���)	- �ڼ���

		//{{ 2013. 02. 04  Ư���ð� ������ Ȯ�� ���� - ��μ�
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
		RID_ATTRACTION_ITEM_GLACIAL_EVENT			= 160683,   // ���ε� ���� ������(�۷��̼� 2���)
		RID_ATTRACTION_ITEM_EVELUN_ARA_EVENT		= 160684,   // ���ε� ���� ������(�ƶ� ����丣 �̺��� 2���)
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
		//}
		//{{ 2013. 2. 21	�ڼ���	2013�� 02�� 21�� ��ġ
#ifdef SERV_ATTRACTION_ITEM_GOLD_CONDOR
		RID_ATTRACTION_ITEM_GOLD_CONDOR		= 160710,   // ���ε� ���� ������( Ȳ�� �ܵ� �ƹ�Ÿ )
#endif SERV_ATTRACTION_ITEM_GOLD_CONDOR
		//}}

		RID_ATTRANTION_BONUS					= 91610,
		
#ifdef SERV_ATTRACTION_ITEM_ARC_DEVIL
		RID_ATTRACTION_ITEM_ARC_DEVIL			= 70005100,	// ��ũ���� ���� ������ ( ������ �ƴ�! )
#endif
#ifdef SERV_ATTRACTION_ITEM_EU
		RID_ATTRACTION_ITEM_OFFICER_EU			= 85001530, // ���ε� ���� ������(���ҵ�, ����, ���̻�, ���̺�, �̺�, û �� ������ �屳)
		RID_ATTRACTION_ITEM_OFFICER_DE			= 85001531, // ���ε� ���� ������(���̺�, �̺�, û �� ������ �屳)
		RID_ATTRACTION_ITEM_NASOD_EU			= 85002450, //���� ������ ������(���ҵ� ��Ʋ ��Ʈ)
		RID_ATTRACTION_ITEM_NASOD_DE			= 85002440, //���� ������ ������(���̺�, �̺�, û ���ҵ� ��Ʋ ��Ʈ)
		RID_ATTRACTION_ITEM_ARCANGEL_EU			= 85002870, // ���ε� ���� ������(��ũ����)
		RID_ATTRACTION_ITEM_ARCANGEL_DE			= 85002871, // ���ε� ���� ������(�̺�, û ��ũ����)
		RID_ATTRACTION_ITEM_VELDER_EU			= 85002872, // ���ε� ���� ������(���� ���丮�� ����)
		RID_ATTRACTION_ITEM_CHUNG_VELDER_EU		= 85002873, // ���ε� ���� ������(û ���� ���丮�� ����)
		RID_ATTRACTION_ITEM_SOLES_EU			= 85002874, // ���ε� ���� ������(����丣 �ַ���)
		RID_ATTRACTION_ITEM_CHUNG_SOLES_EU		= 85002875, // ���ε� ���� ������(û ����丣 �ַ���)
		RID_ATTRACTION_ITEM_CHUNG_ARCDEVIL		= 85002876, // ���ε� ���� ������(û ��ũ����)
		RID_ATTRACTION_ITEM_ARCDEVIL			= 85002877, // ���ε� ���� ������(��ũ����)
#endif //SERV_ATTRACTION_ITEM_EU	
#ifdef SERV_MINI_ATTRACTION_ITEM_JP
		RID_MINI_ATTRACTION_ITEM_JP1			= 65001062, // �̴� ���� ������ ������
#endif SERV_MINI_ATTRACTION_ITEM_JP
#ifdef SERV_BATTLEFIELD_COOKIE_PIECE
		RID_BATTLEFIELD_COOKIE					= 85002460,	// �ʵ� ��Ű
		RID_BATTLEFIELD_COOKIE_PIECE			= 85002461,	// �ʵ� ��Ű ����
		RID_BATTLEFIELD_GOLD_COOKIE				= 85002462, // �ʵ� Ȳ�� ��Ű
		RID_BATTLEFIELD_GOLD_COOKIE_PIECE		= 85002463, // �ʵ� Ȳ�� ��Ű ����
#endif SERV_BATTLEFIELD_COOKIE_PIECE
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		RID_ONE_THIRD_MINI_ATTRACTION_ITEM		= 140000110,		// 2013.04.23 lygan_������ // 1/3 Ȯ���� �̴� ������ ������ // �ش� ������� ������ ũ����Ż�� �߰� ���� ���� �ʴ´�.
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM		
		
#ifdef	SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD// ���볯¥: 2013-04-25
		RID_ALL_IN_ONE_ATTRACTION_ITEM					= 160772,	// ���ε� ���ο� ���� ������ (����1)
#endif	// SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

#ifdef SERV_2013_CHINA_EVENT
		RID_2013_CHINA_EVENT_CUBE				= 84001860,
#endif SERV_2013_CHINA_EVENT
	};

	struct RandomUnitData
	{
		char		m_cUnitClass;
		int			m_iItemGroupID;
	};

	//{{ 2009. 12. 5  ������	����ť��ý��۰���
	typedef std::pair< int, int >	RandomItemKey;
	//}}

	struct RandomItemData
	{
		int			m_ItemID;
		bool		m_bGiveAll;
		//{{ 2012. 10. 16	�ڼ���	RandomBoxTable ���� ����
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		byte		m_byteUseCondition;
#else
		bool		m_bUnitSelect;
		bool		m_bGiveCharacter;
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		//}}
		int			m_KeyItemID;
		int			m_RequiredKeyCount;
		int			m_iRessurectionCount;
		int			m_iRestoreSpirit;

		//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		int			m_iRequiredED;
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

		std::vector< RandomUnitData > m_vecUnitData;

		//{{ 2012. 10. 16	�ڼ���	RandomBoxTable ���� ����
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		RandomItemData::RandomItemData()
			: m_byteUseCondition( 0 )
		{
		}
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		//}}
	};

	struct ItemGroupData
	{
		int			m_iItemID;
		float		m_fRate;
		int			m_iPeriod;
		int			m_iQuantity;

		ItemGroupData()
		{
			m_iItemID	= 0;
			m_fRate		= 0.0f;
			m_iPeriod	= 0;
			m_iQuantity	= 0;
		}
	};

	struct ItemGroupDataList
	{
		int			m_iGroupID;
		KLottery	m_kLottery;
		std::vector< ItemGroupData > m_vecItemData;

		ItemGroupDataList(){};
		ItemGroupDataList( const ItemGroupDataList& t ) { *this = t; }    // copy constructor
		ItemGroupDataList& operator=( const ItemGroupDataList& t )        // assign operator
		{
			m_iGroupID = t.m_iGroupID;
			m_kLottery = t.m_kLottery;
			m_vecItemData = t.m_vecItemData;
			return *this;
		}
	};

	//{{ 2009. 6. 22  ������	�ŷ¾����۸���Ʈ
	struct KAttractionItemInfo
	{
		bool	m_bNotifyMsg;
		int		m_iMsgNum;

		KAttractionItemInfo()
		{
			m_bNotifyMsg = false;
			m_iMsgNum = 0;
		}
	};
	//}}

	//{{ 2013. 3. 16	�ڼ���	�ŷ� ������ ��ũ��Ʈ ó��
#ifdef SERV_CHARM_ITEM_SCRIPT
	struct CharmItemInfo
	{
		int m_iBonusItemID;

		CharmItemInfo()
			: m_iBonusItemID( 0 )
		{
		}
	};
#endif SERV_CHARM_ITEM_SCRIPT
	//}}

public:
	CXSLRandomItemManager(void);
	~CXSLRandomItemManager(void);

	// for lua
	bool	AddRandomItemTemplet_LUA();
	bool	AddRandomItemGroup_LUA( int iGroupID, int iItemID, float fRate, int iPeriod, int iQuantity );

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	bool	AddRandomItemMappingData_LUA( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID );
#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	bool    AddRandomItemMappingTime_LUA( int nIndex, std::wstring wstrScriptStartDate, std::wstring wstrScriptEndDate );
#else SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	bool	AddRandomItemMappingTime_LUA( int nIndex, const char* szStartDate, const char* szEndDate );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	bool	ModifyMapItemGroup(RANDOMITEM_MAPPING_DATA randItemMappingData);
	bool	RestoreMapItemGroup(RANDOMITEM_MAPPING_DATA randItemMappingData);

	std::map<int, RANDOMITEM_MAPPING_DATA>		m_mapRandomItemMappingData;
	std::map<int, RANDOMITEM_MAPPING_TIME>		m_mapRandomItemMappingTime;
#endif SERV_CUBE_IN_ITEM_MAPPING

	//{{ 2009. 6. 22  ������	�ŷ¾����� ����Ʈ
	void	SetNotifyMsg_LUA( bool bVal ) { m_bIsNotifyMsg = bVal; }
#ifdef SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	bool	AddAttractionItemInfo_LUA( int iItemID, bool bNotify, bool bSeal = true );
	// �ٸ� �Լ��� ���� ���� _Global ����
	bool	IsSealedItem_Global( IN const int iItemID ) const { return ( m_setSealedItemList_Global.find( iItemID ) != m_setSealedItemList_Global.end() ); }
#else //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	bool	AddAttractionItemInfo_LUA( int iItemID, bool bNotify );
#endif //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	//}}
	//{{ 2010. 7. 26  ������	���� ���� ť��
#ifdef SERV_SEALED_RANDOM_ITEM
	bool	AddSealRandomItemInfo_LUA( int iItemID );
	bool	AddNotifyResultItemInfo_LUA( int iItemID );
#endif SERV_SEALED_RANDOM_ITEM
	//}}
	//{{ 2013. 3. 16	�ڼ���	�ŷ� ������ ��ũ��Ʈ ó��
#ifdef SERV_CHARM_ITEM_SCRIPT
	void	AddCharmItem_LUA( IN int iItemID, IN int iBonusItemID );
	bool	IsCharmItem( IN const int iItemID ) const;
	int		GetBonusCharmItem( IN const int iItemID ) const;
#endif SERV_CHARM_ITEM_SCRIPT
	//}}

	// function
	//{{ 2009. 12. 5  ������	����ť��ý��۰���
	const std::map< int, int >* GetRandomItemKeyList( IN int iRandomItemID ) const;
	//}}
	const RandomItemData* GetRandomItemData( IN int iRandomItemID, IN int iKeyItemID ) const;
	//{{ 2009. 6. 22  ������	�ŷ¾����� ����Ʈ
	bool	IsAttrationItem( IN const int iItemID ) const { return ( m_mapAttractionItem.find( iItemID ) != m_mapAttractionItem.end() ); }
	bool	MakeNotifyMsgAttrationItem( IN const int iRewardItemID, IN const std::wstring& wstrNickName, OUT std::wstring& wstrNotifyMsg );	
	//}}
	//{{ 2010. 7. 26  ������	���� ���� ť��
#ifdef SERV_SEALED_RANDOM_ITEM
	bool	MakeNotifyMsgRandomResultItem( IN const int iRewardItemID, IN const std::wstring& wstrNickName, OUT std::wstring& wstrNotifyMsg );
	bool	IsSealRandomItemCube( IN int iItemID )	{ return ( m_setSealRandomItem.find( iItemID ) != m_setSealRandomItem.end() ); }
	bool	IsNotifyResultItem( IN int iItemID )	{ return ( m_setNotifyResultItem.find( iItemID ) != m_setNotifyResultItem.end() ); }
#endif SERV_SEALED_RANDOM_ITEM
	//}}

	//{{ 2011. 05. 12  ��μ�	���� ť�� ���½� �κ��丮 �˻� ����
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
	bool	GetResultItem( IN char cUnitClass, IN const RandomItemData* pRandomItemData, OUT std::map< int, KItemInfo >& mapResultItem, OUT std::map< int, int >& mapCheckEmpty, OUT bool& bIsGiveAll ) const;
#else
	bool	GetResultItem( IN char cUnitClass, IN const RandomItemData* pRandomItemData, OUT std::map< int, KItemInfo >& mapResultItem, OUT std::map< int, int >& mapCheckEmpty ) const;
#endif SERV_RANDOM_ITEM_CHECK_INVEN
	//}}

private:
	//{{ 2009. 12. 5  ������	����ť��ý��۰���
	std::map< int, std::map< int, int > >		m_mapRandomItemKeyTable;
	//}}
	std::map< RandomItemKey, RandomItemData >	m_mapRandomItem;
	std::map< int, ItemGroupDataList >			m_mapItemGroup;
	//{{ 2009. 6. 22  ������	�ŷ¾����۸���Ʈ
	std::map< int, KAttractionItemInfo >		m_mapAttractionItem;
	bool										m_bIsNotifyMsg;
	//}}
	//{{ 2010. 7. 26  ������	���� ���� ť��
#ifdef SERV_SEALED_RANDOM_ITEM
	std::set< int >								m_setSealRandomItem;
	std::set< int >								m_setNotifyResultItem;
#endif SERV_SEALED_RANDOM_ITEM
	//}}

#ifdef SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	std::set< int >								m_setSealedItemList_Global;
#endif //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL

	//{{ 2013. 3. 16	�ڼ���	�ŷ� ������ ��ũ��Ʈ ó��
#ifdef SERV_CHARM_ITEM_SCRIPT
	std::map<int, CharmItemInfo>				m_mapCharItemInfo;
#endif SERV_CHARM_ITEM_SCRIPT
	//}}
};

DefRefreshSingletonInline( CXSLRandomItemManager );



