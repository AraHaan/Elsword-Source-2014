#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"

class CXSLEnchantItemManager
{
	DeclareRefreshSingleton( CXSLEnchantItemManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
public:

	enum ENCHANT_INFO
	{
		MAX_ENCHANT_LEVEL = 20,	
	};

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	enum RANDOM_ENCHANT_TYPE
	{
		RET_DROP = 0,
		RET_CUBE,

		RET_MAX,
	};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	struct KAttachInfo 
	{
		int	m_iEnchantLv;
		int m_iItemType;
#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
		int	m_iMinItemLevel;
		int m_iMaxItemLevel;
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM

		KAttachInfo()
			: m_iEnchantLv( 0 )
			, m_iItemType( 0 )
#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
			, m_iMinItemLevel( 0 )
			, m_iMaxItemLevel( 0 )
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM
		{
		}
	};

	typedef std::pair< int, int > KEStoneInfoKey;
	typedef std::pair< int, int > KRestoreInfoKey;

public:
	CXSLEnchantItemManager(void);
	~CXSLEnchantItemManager(void);
	
	bool GetEnchantRate( IN int iEnchantLevel, OUT float& fPercent );
#ifdef SERV_ENCHANT_PLUS_ITEM
	bool GetEnchantResult( IN bool bIsRare, IN bool bIsPlus, IN int iEnchantLevel, OUT int& iResult );
#else
	bool GetEnchantResult( IN bool bIsRare, IN int iEnchantLevel, OUT int& iResult );
#endif SERV_ENCHANT_PLUS_ITEM
	
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	std::vector< int > GetWeaponEnchantStoneID()          { return m_vecWeaponEnchantStone; }
	std::vector< int > GetRareWeaponEnchantStoneID()      { return m_vecRareWeaponEnchantStone; }
	std::vector< int > GetArmorEnchantStoneID()           { return m_vecArmorEnchantStone; }
	std::vector< int > GetRareArmorEnchantStoneID()       { return m_vecRareArmorEnchantStone; }
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	int  GetWeaponEnchantStoneID()          { return m_iWeaponEnchantStone; }
	int  GetRareWeaponEnchantStoneID()      { return m_iRareWeaponEnchantStone; }
	int  GetArmorEnchantStoneID()           { return m_iArmorEnchantStone; }
	int  GetRareArmorEnchantStoneID()       { return m_iRareArmorEnchantStone; }
#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE

	//{{ 2008. 11. 16  ������	��ȭ ����
	int  GetEnchantStoneItemID( int iEquipLv, int iItemType );
	//}}
	//{{ 2008. 12. 14  ������	��ȭ ������
	int  GetEnchantSupportMaterialItemID( int iEquipLv );
	//}}
	//{{ 2008. 12. 21  ������	��ȭ���� ����
	int	 GetRestoreItemID( int iEquipLv, int iItemType );
	//}}
	//{{ 2008. 12. 26  ������	��ȭ �ο�
	bool GetAttachItemEnchantLevel( int iAttachItemID, int& iEnchantLevel, int& iItemType );
	//}}

	// ��ũ��Ʈ �Ľ�
	bool SetEnchantRate_LUA( int iEnchantLevel, float fPercent );
	bool SetEnchantProbability_LUA( int iEnchantLevel );
	bool SetEnchantRareProbability_LUA( int iEnchantLevel );

#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	void AddWeaponEnchantStone_LUA( int iWeaponEnchantStone )			{ m_vecWeaponEnchantStone.push_back( iWeaponEnchantStone ); }
	void AddRareWeaponEnchantStone_LUA( int iRareWeaponEnchantStone )	{ m_vecRareWeaponEnchantStone.push_back( iRareWeaponEnchantStone ); }
	void AddArmorEnchantStone_LUA( int iArmorEnchantStone )				{ m_vecArmorEnchantStone.push_back( iArmorEnchantStone ); }
	void AddRareArmorEnchantStone_LUA( int iRareArmorEnchantStone )		{ m_vecRareArmorEnchantStone.push_back( iRareArmorEnchantStone ); }
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	//{{hoons.080815. ���� ���� ��ũ�� �ƴ� �Լ����ڷ� �ް���.
	void SetWeaponEnchantStone_LUA( int iWeaponEnchantStone )			{ m_iWeaponEnchantStone = iWeaponEnchantStone; }
	void SetRareWeaponEnchantStone_LUA( int iRareWeaponEnchantStone )	{ m_iRareWeaponEnchantStone = iRareWeaponEnchantStone; }
	void SetArmorEnchantStone_LUA( int iArmorEnchantStone )				{ m_iArmorEnchantStone = iArmorEnchantStone; }
	void SetRareArmorEnchantStone_LUA( int iRareArmorEnchantStone )		{ m_iRareArmorEnchantStone = iRareArmorEnchantStone; }
	//}}
#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE

	//{{ 2008. 11. 16  ������	��ȭ ����
	bool AddEnchantStoneInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax );
	bool AddSupportMaterialInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax );
	//}}
	//{{ 2008. 12. 21  ������	������
	bool AddRestoreItemInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax );
	//}}
	//{{ 2008. 12. 25  ������	��ȭ�ο�
#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
	bool AddAttachItemInfo_LUA( int iItemID );
#else // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM
    bool AddAttachItemInfo_LUA( int iItemID, int iItemType, int iEnchantLevel );
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM
	//}}
	//{{ 2011. 01. 24	������	�÷���� ��ȭ ���� ����
#ifdef SERV_SUPPORT_MATERIAL_LIMIT
	void SetSupportMaterialEnchantLimitLevel_LUA( IN int iEnchantLimitLevel );
	int GetSupportMaterialEnchantLimitLevel() const									{ return m_iEnchantLimitLevel; }
#endif SERV_SUPPORT_MATERIAL_LIMIT
	//}}

	//{{  2011.12.21     ��μ�    �ں� �÷���� ���� �̺�Ʈ
#ifdef SERV_COBO_SUPPORT_MATERIAL_EVENT
	bool AddEventSupportMaterialInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax );
	
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
	void GetEventEnchantSupportMaterialItemID( int iEquipLv , std::vector< int >& vecItemID );
#else
	int GetEventEnchantSupportMaterialItemID( int iEquipLv );
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#endif SERV_COBO_SUPPORT_MATERIAL_EVENT
	//}}
	//{{  2011.12.21     ��μ�    �ں� �ູ���� ������ �ֹ��� �̺�Ʈ
#ifdef SERV_COBO_RESTORE_SCROLL_EVENT
	bool AddEventRestoreItemInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax );
	
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
	void GetEventRestoreItemID( int iEquipLv, int iItemType, std::vector< int >& vecItemID );
#else
	int GetEventRestoreItemID( int iEquipLv, int iItemType );
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
#endif SERV_COBO_RESTORE_SCROLL_EVENT
	//}}

#ifdef SERV_EVENT_ENCHANT_STONE_MULTI
	bool AddEventEnchantStoneInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax );
	void  GetEventEnchantStoneItemID( int iEquipLv, int iItemType, std::vector< int >& vecItemID );
#endif SERV_EVENT_ENCHANT_STONE_MULTI

#ifdef SERV_ENCHANT_LIMIT
	void SetEnchantLimitLevel_LUA( IN int iEnchantLimitLevel );
	int GetEnchantLimitLevel() const									{ return m_iRealEnchantLimitLevel; }
#endif SERV_ENCHANT_LIMIT

	//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
	int GetEnchantPlusItemID( int iEquipLv );
	bool AddEnchantPlusInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax );
	bool SetEnchantPlusProbability_LUA( int iEnchantLevel );
#endif
	//}}
	//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
	int GetDestroyGuardItemID( int iEquipLv );
	bool AddDestroyGuardInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax );
#endif
	//}}

	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	void SetEventSupportMaterialEnchantLimitLevel_LUA( IN int iEnchantLimitLevel );
	int GetEventSupportMaterialEnchantLimitLevel() const									{ return m_iEventEnchantLimitLevel; }
	bool SetEnchantProbability_Event_LUA( int iEnchantLevel );
	bool GetEventEnchantResult( IN bool bIsRare, IN int iEnchantLevel, OUT int& iResult );
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	bool AddDropRandomEnchantInfo_LUA( int iEnchantLevel, float fRate );
	bool AddCubeRandomEnchantInfo_LUA( int iEnchantLevel, float fRate );

	bool CheckRandomEnchant( IN const int iItemID, IN const RANDOM_ENCHANT_TYPE eType ) const;
	bool RandomEnchant( IN const int iItemID
					  , IN const RANDOM_ENCHANT_TYPE eType
					  , IN OUT char& cTimeEnchantEventLevel
					  , OUT char& cEnchantLevel
					  );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
	bool	CheckAttachItemEnableLevel( IN const int iAttachItemID, IN const int iUseLevel ) const;
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM

private:
	std::map< int, float >		 			m_mapEnchantRate;
	std::map< int, KLottery >               m_mapEnchantProb;
	std::map< int, KLottery >               m_mapEnchantRareProb;
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	std::vector< int >						m_vecWeaponEnchantStone;
	std::vector< int >						m_vecRareWeaponEnchantStone;
	std::vector< int >						m_vecArmorEnchantStone;
	std::vector< int >						m_vecRareArmorEnchantStone;
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
    int                                     m_iWeaponEnchantStone;
    int                                     m_iRareWeaponEnchantStone;
    int                                     m_iArmorEnchantStone;
    int                                     m_iRareArmorEnchantStone;
#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	//{{ 2011. 01. 24	������	�÷���� ��ȭ ���� ����
#ifdef SERV_SUPPORT_MATERIAL_LIMIT
	int										m_iEnchantLimitLevel;
#endif SERV_SUPPORT_MATERIAL_LIMIT
	//}}
	
	std::map< KEStoneInfoKey, int >			m_mapEnchantStoneInfo;		// ��ȭ�� ����
	std::map< int, int >					m_mapSupportMaterialInfo;	// ������
	std::map< KRestoreInfoKey, int >		m_mapRestoreItemInfo;		// �����ֹ���
	std::map< int, KAttachInfo >			m_mapEnchantAttach;			// ��ȭ�ο�

	//{{  2011.12.21     ��μ�    �ں� �÷���� ���� �̺�Ʈ
#ifdef SERV_COBO_SUPPORT_MATERIAL_EVENT
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
	std::multimap< int, int >				m_multimapEventSupportMaterialInfo;
#else
	std::map< int, int >					m_mapEventSupportMaterialInfo;	// �̺�Ʈ ������
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#endif SERV_COBO_SUPPORT_MATERIAL_EVENT
	//}}
	//{{  2011.12.21     ��μ�    �ں� �ູ���� ������ �ֹ��� �̺�Ʈ
#ifdef SERV_COBO_RESTORE_SCROLL_EVENT
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
	std::multimap< KRestoreInfoKey, int >	m_multimapEventRestoreItemInfo;
#else
	std::map< KRestoreInfoKey, int >		m_mapEventRestoreItemInfo;		// �����ֹ���
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
#endif SERV_COBO_RESTORE_SCROLL_EVENT
	//}}
#ifdef SERV_EVENT_ENCHANT_STONE_MULTI
	std::multimap< KEStoneInfoKey, int >	m_multimapEventEnchantStoneInfo;
#endif SERV_EVENT_ENCHANT_STONE_MULTI

//#ifdef SERV_ENCHANT_LIMIT
	int										m_iRealEnchantLimitLevel;
//#endif SERV_ENCHANT_LIMIT

	//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
	std::map< int, KLottery >               m_mapEnchantPlusProb;	// ��ȭ Ȯ�� ���� ������ ��� ���� Ȯ�� 
	std::map< int, int >					m_mapEnchantPlusInfo;	// ��ȭ Ȯ�� ���� ������
#endif
	//}}

	//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
	std::map< int, int >					m_mapDestroyGuardInfo;		// �ı� ����
#endif
	//}}

	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	int										m_iEventEnchantLimitLevel;
	std::map< int, KLottery >               m_mapEventEnchantProb;
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}
	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	KLottery								m_kRandomEnchant[RET_MAX];
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
};

DefRefreshSingletonInline( CXSLEnchantItemManager );



