#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"

class CXSLAttribEnchantItem
{
	DeclareRefreshSingleton( CXSLAttribEnchantItem );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum ENCHANT_TYPE
	{
		ET_NONE = 0,
		ET_BLAZE,
		ET_WATER,
		ET_NATURE,
		ET_WIND,
		ET_LIGHT,
		ET_DARK,

		ET_RANDOM,
	};

	enum ENCHANT_SLOT_ID
	{
        ESI_SLOT_1 = 0,
		ESI_SLOT_2,
		ESI_SLOT_3,

		ESI_SLOT_MAX,
	};

	// �ϵ��ڵ��ӵ�..
	enum ATTRIB_ITEM_ID
	{
		ATI_NONE			= 0,

		ATI_UNKNOWN			= 130047, // ���Ӽ�
		ATI_RED				= 130048, // ��
		ATI_BLUE			= 130049, // ��
		ATI_GREEN			= 130051, // �ڿ�
		ATI_WIND			= 130050, // �ٶ�
		ATI_LIGHT			= 130053, // ��
		ATI_DARK			= 130052, // ����

		ATI_IDENTIFY_STONE	= 130054, // ������
		ATI_UNKNOWN_STONE	= 130055, // ������

//#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
		ATI_ESSENCE			= 130208, // ���� ����
//#endif // SERV_FINALITY_SKILL_SYSTEM
	};

	enum ATTRIB_ENCHANT_TYPE
	{
		AET_NONE = 0,

		AET_SINGLE_WEAPON,
		AET_DUAL_WEAPON,
		AET_ARMOR,

		//{{ 2009. 11. 9  ������	Ʈ���üӼ�
		AET_TRIPLE_WEAPON,
		AET_DUAL_ARMOR,
		//}}
	};

	//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	enum ATTRIB_COUNT_TYPE
	{
		ACT_NONE = 0,
		ACT_SINGLE,
		ACT_DUAL,
		ACT_TRIPLE,
	};
	//}}



	struct KIdentifyInfo
	{
		int			m_iRequireItemID;
		KLottery	m_kResult;
	};

public:
	CXSLAttribEnchantItem(void);
	virtual ~CXSLAttribEnchantItem(void);

	// For Lua	
	bool	AddEnchantRequire_LUA( int iAttribEnchantType, int iEquipLv );
	bool	AddRandomAttribSingle_LUA( char cEncahntType, float fRate );
	bool	AddRandomAttribDual_LUA( char cSingleEncahntType, char cDualEncahntType, float fRate );
	bool	AddRandomAttribTriple_LUA( char cSingleEncahntType, char cDualEncahntType, char cTripleEnchantType, float fRate );
	bool	AddIdentifyInfo_LUA( int iSourceItemID, int iRequireItemID, int iResultItemID, float fRate );
	
	//{{ 2011. 01. 04  ��μ�	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM
	bool	AddAttribAttachInfo_LUA( int iItemID, char cSlot0, char cSlot1, char cSlot2 );
#endif SERV_ATTRIBUTE_CHARM
	//}}

	// �Ӽ��� ���� ����
	const KIdentifyInfo* GetIdentifyInfo( int iItemID );

	// �Ӽ���ȭ �䱸���� �Լ�
	static ATTRIB_COUNT_TYPE GetAttribCountType( IN const KItemAttributeEnchantInfo& kInfo );
	static char GetAttribEnchantType( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon );
	bool	GetRequireItemCount_Insert( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireCount ) const;
	bool	GetRequireED_Insert( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireED ) const;
	bool	GetRequireED_Remove( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireED ) const;
	static int GetRequireEDFactor( int iItemGrade );

	// �Ӽ���ȭ ���� �Լ�
	bool	IsAttribItemID( int iItemID );
	bool	VerifyAttribEnchantType( IN char cAttribEnchantType );
	char	GetRandomAttribResult( IN ATTRIB_COUNT_TYPE eCurrentCountType, IN char cFirstAttribEnchant = ET_NONE, IN char cSecondAttribEnchant = ET_NONE ) const;
	bool	IsPossibleAttribEnchant( IN OUT char& cAttribEnchantType, IN char cAttribEnchantSlotNo, IN const KItemAttributeEnchantInfo& kInfo ) const;


	static int GetRequireItemID( IN char cAttribEnchantType );
	bool	IsPossibleToPush( IN char cExistingType, IN char cTypeToAdd ) const;

	//void	SetWeaponEnchantData( CX2GUUser* pGUUser );
	//void	SetDefenseEnchantData( CX2GUUser* pGUUser );

	//void	ProcessEnchant( CX2GameUnit* pDefender, CX2DamageManager::DamageData* pDamageData, float fRandValue );

	//{{ 2011. 01. 04	������	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM
	bool	GetAttribAttachInfo( IN int iAttachItemID, OUT KItemAttributeEnchantInfo& kInfo );
#endif SERV_ATTRIBUTE_CHARM
	//}}

#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
	static	bool	IsExtractItem( const int iItemID );
#endif // SERV_FINALITY_SKILL_SYSTEM

private:
	typedef std::map< std::pair< int, int >, std::map< int, int > > MapEnchantRequire;
	typedef std::pair< char, char >	KTripleEnchantKey;
		
	KLottery								m_kLotRandomAttribSingle;	// �̱� �Ӽ� ��æƮ
	std::map< char, KLottery >				m_mapRandomAttribDual;		// ��� �Ӽ� ��æƮ
	std::map< KTripleEnchantKey, KLottery > m_mapRandomAttribTriple;	// Ʈ���� �Ӽ� ��æƮ

	MapEnchantRequire						m_mapEnchantRequireInfo;	// �Ӽ� ��ȭ �ʿ� ������ ����
	std::map< int, KIdentifyInfo >			m_mapIdentifyInfo;			// ������ ����

	//{{ 2011. 01. 04  ��μ�	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM
	std::map< int, KItemAttributeEnchantInfo >	m_mapAttribAttachInfo;	// �Ӽ� ������ ���� ����
#endif SERV_ATTRIBUTE_CHARM
	//}}


};

DefRefreshSingletonInline( CXSLAttribEnchantItem );



