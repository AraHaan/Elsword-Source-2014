#pragma once
#include <set>
#include "X2Eqip.h"





class CX2ItemManager
{
	public:
#ifdef PET_PREVIEW
		enum PREVIEW_PET_TYPE
		{
			PPT_PET,		// �Ϲ� ��
			PPT_RIDING_PET, // Ż �� 
		};

		struct PREVIEW_PET_DATA
		{
			int iCashShopItemID;
			int iPreviewUnitID;
			PREVIEW_PET_TYPE ePreviewPetType;

			PREVIEW_PET_DATA( int iCashShopItemID_, int iUnitID_, PREVIEW_PET_TYPE ePreviewPetType_ ):
			iCashShopItemID( iCashShopItemID_ ),
			iPreviewUnitID( iUnitID_ ), 
			ePreviewPetType( ePreviewPetType_ )
			{}

			PREVIEW_PET_DATA()
			{}
		};
#endif // PET_PREVIEW

#include    "X2ItemManager_Preprocessing.inl"

#ifdef EQUIP_STRIP_TEST
		typedef std::map<CX2Unit::EQIP_POSITION, CX2EqipPtr>	EquipPositionMap;
#endif EQUIP_STRIP_TEST

		struct ItemIDList
		{
			vector<int>		m_ItemIDList;
		};

		struct NpcItemIDList
		{
			int				m_NpcId;
			vector<int>		m_ItemIDList;
		};

		struct RandomItemData
		{
			int				m_ItemID;
			int				m_KeyItemID;
			int				m_RequiredKeyCount;
#ifdef SERV_CUBE_OPEN_ED_CONDITION
			int				m_iRequiredED;
#endif //SERV_CUBE_OPEN_ED_CONDITION
		};

		struct MaterialData
		{
			int			m_MaterialItemID;
			int			m_MaterialCount;
		};

		struct ManufactureData
		{
#ifdef ADD_SERVER_GROUP
			int			m_iServerGroupID;
#endif
			int			m_ManufactureType;	// CX2ManufactureUI::MANUFACTURE_TYPE
			int			m_ResultGroupID;			
			int			m_Cost;

			std::vector< MaterialData >	m_vecMaterials;
		};		

		struct ManufactureResultGroupData
		{
			int					m_ResultGroupID;
			std::vector< int >	m_vecResultGroupItemID;
		};

		struct CashItem  
		{
#ifndef SERV_GLOBAL_BILLING
			unsigned long       m_ulRelationProductNo;
#endif // ndef SERV_GLOBAL_BILLING
			int					m_ItemID;

			bool				m_bShow;
			
			bool				m_bIsNew;
			bool				m_bIsHot;
			bool				m_bIsRecommend;
			bool				m_bIsLimited;

#ifdef SERV_GLOBAL_BILLING
			vector< KBillProductInfo > m_vecKBillProductInfo;
#else // SERV_GLOBAL_BILLING
			vector< KNXBTProductInfo > m_vecKNXBTProductInfo; //���� ������������ �Ⱓ���� ���� ������ ���� �� �ְ���?
#endif // SERV_GLOBAL_BILLING
			
#ifdef SERV_WISH_LIST_NO_ITEM
			bool				m_bIsWishListNotEnable;
#endif SERV_WISH_LIST_NO_ITEM

			CashItem()
			{
#ifndef SERV_GLOBAL_BILLING
				m_ulRelationProductNo = -1;
#endif // ndef SERV_GLOBAL_BILLING
				m_ItemID = -1;

				m_bShow = true;

				m_bIsNew = false;
				m_bIsHot = false;
				m_bIsRecommend = false;
				m_bIsLimited = false;
#ifdef SERV_WISH_LIST_NO_ITEM
				m_bIsWishListNotEnable = false;
#endif SERV_WISH_LIST_NO_ITEM
			}

			//��ǥ ��ǰ ĳ�� ��ġ
			int GetCash();
#ifdef SERV_GLOBAL_BILLING
			const KBillProductInfo* GetGateProduct();
#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
			const std::vector<int> GetGateListOfProduct();
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
			wstring GetPeriod( KBillProductInfo& kKBillProductInfo, bool bShowSalePeriod = true );
			wstring GetSalePeriod( KBillProductInfo& kBillProductInfo );
#else
			wstring GetPeriod( KBillProductInfo& kBillProductInfo );
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
#else // SERV_GLOBAL_BILLING
			const KNXBTProductInfo* GetGateProduct();
			int	GetGateProductIndex();
			wstring GetPeriod( KNXBTProductInfo& kKNXBTProductInfo );
#endif // SERV_GLOBAL_BILLING

			bool GetIsNew() { return m_bIsNew; }
			bool GetIsHot() { return m_bIsHot; }
			bool GetIsRecommend();
#ifdef SERV_WISH_LIST_NO_ITEM
			bool GetIsWishListNotEnable() { return m_bIsWishListNotEnable; }
#endif SERV_WISH_LIST_NO_ITEM
		};

		class CCashProductSort
		{
		public: 
#ifdef SERV_GLOBAL_BILLING
		public: 
			bool operator()( const KBillProductInfo& p, const KBillProductInfo& q ) const
			{ 
				if( p.m_iSalePrice < q.m_iSalePrice )
					return true;
				else
					return false;
			}
#else // SERV_GLOBAL_BILLING
			bool operator()( const KNXBTProductInfo& p, const KNXBTProductInfo& q ) const
			{ 
				if( p.m_ulSalePrice < q.m_ulSalePrice )
					return true;
				else
					return false;
			}
#endif // SERV_GLOBAL_BILLING
		};

		//{{ 2008. 4. 24  ������  ��Ű�� ������
		struct PackageItemData
		{
			int				m_iItemID;
			unsigned short  m_usProductPieces;
			bool			m_bShowItem;
		};
		//}}
		//{{ 2013.01.26 �ֹ�ö ť����Ű�������� �̸�������
#ifdef PACKAGE_IN_QUBE_PREVIEW
		struct CubePackageData
		{
			int				m_iPackageID1;
			int				m_iPackageID2;
		};
#endif PACKAGE_IN_QUBE_PREVIEW
		//}}

#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		struct SetItemData
		{
			int							m_SetID;
			//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			int							m_iMaxLevel;	// m_SetID�� ȿ���� ������ �ִ� Item �� ���� ���� ����
			//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			wstring						m_SetName;

			map< int, vector< int > >	m_mapNeedPartsNumNOptions;	// first: �ʿ��� ��� ����, second: �ɼ�ID����

			SetItemData() : m_SetID(0)
//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			, m_iMaxLevel(0)
//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			{}

            const wchar_t*              GetSetName() const { return m_SetName.c_str(); }
		};
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef ICE_HEATER_PRE_VIEW
		struct IceHeaterItemData
		{
			int					m_iItemID;
			CX2Unit::UNIT_TYPE	m_eUnitType;
	#ifdef ICE_HEATER_RANDOM_PRE_VIEW // ����ȯ
			int					m_iItemGroup;	/// ������ �ƹ�Ÿ ������ ������� �� ������ ���� ������ �׷�
	#endif //ICE_HEATER_RANDOM_PRE_VIEW
		};
#endif //ICE_HEATER_PRE_VIEW


#ifdef NEW_ITEM_EXCHANGE_TEST
		enum ITEM_EXCHANGE_TYPE
		{
			IET_INVALID = 0,
			IET_ALL,
			IET_RANDOM,
			IET_SELECT,
		};

		// 
		struct ItemExchangeData
		{
			ITEM_EXCHANGE_TYPE						m_eExchangeType;
			int										m_iSrcItemID;
			int										m_iDestItemID;
#ifdef SERV_ITEM_EXCHANGE_NEW // ������ �� �� �ѷ��� �־ �ؿ��� ����
			int										m_iSrcQuantity;
#endif SERV_ITEM_EXCHANGE_NEW
			int										m_iQuantity;
#ifdef SERV_EXCHANGE_PERIOD_ITEM
			short									m_sPeriod;
#endif //SERV_EXCHANGE_PERIOD_ITEM
		};
		
#endif NEW_ITEM_EXCHANGE_TEST

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		struct tgaDisCountItem
		{
			int					m_DisCountProNo;
			int					m_BasicProNo;
			std::vector<int>	m_DisCountKeepItem;
		};
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP


	public:
		CX2ItemManager(void);
		~CX2ItemManager(void);

		bool	OpenScriptFile( const WCHAR* pFileName );
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		bool	OpenItemScriptFile( const WCHAR* pKimFile, unsigned uNumScriptFiles, const WCHAR* apScriptFileName[] );
        bool    DoMemoryNotEncrypt( const char* pBuffer, long nSize );
#endif	//X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		bool	AddItemTemplet_LUA();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		bool	AddShopItemList_LUA();
		bool	AddShopItemGroup_LUA();		

#ifdef SERV_CUBE_OPEN_ED_CONDITION
		bool	AddRandomItemTemplet_LUA( int iItemID_, int iKeyItemID_, int iRequiredKeyCount_, int iRequiredED_ );
#else
		bool	AddRandomItemTemplet_LUA( int iItemID, int iKeyItemID, int iRequiredKeyCount );
#endif //SERV_CUBE_OPEN_ED_CONDITION

		bool	AddManufactureTemplet_LUA();

		//{{ kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE	
			bool	AddManufactureVillageData_LUA( int iManufactureID, int iHouseID );
#else	ADD_HOUSE_ID_TO_MANUFACTURE
			bool	AddManufactureVillageData_LUA( int iManufactureID, int iVillageMapID );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���

		bool	AddManufactureResultGroup( int iGroupID, int iItemID );

		//{{ 2008. 11. 18  ������
		void	AddItemExchangeGroup_LUA( int iSourceItemID, int iResultItemID, float fRate );
		bool	IsCanItemExchange( int iItemID );
		//}}


#ifdef NEW_ITEM_EXCHANGE_TEST
		bool 	AddItemExchangeData( int iHOUSEID, ITEM_EXCHANGE_TYPE eExchangeType, int iSrcItemID, int iDestItemID, int iQuantity, float fDestItemRate = 0.f );
		bool	GetItemExchangeData( int iHouseID, std::vector< ItemExchangeData >& vecItemExchangeData );	// kimhc // 2009-07-27 // ��ϸ� �ð� ��ȯ UI
		
#endif NEW_ITEM_EXCHANGE_TEST


		//{{ 2013.01.26 �ֹ�ö ť����Ű�������� �̸�������
#ifdef PACKAGE_IN_QUBE_PREVIEW
		bool	AddCube_PackageData_LUA( int iCubePackageItemID, int iPackageItemID1, int iPackageItemID2, int iUnitType);
		bool	GetPackageCubeItemData( int iPackageItemID, std::vector< PackageItemData >& vecPackageData);
#endif PACKAGE_IN_QUBE_PREVIEW
		//}}

		//{{ 2008. 4. 24  ������  ��Ű�� ������
		bool	AddPackageItemData_LUA( int iPackageItemID, int iItemID, unsigned short usProductPieces, bool bShowItem );
		bool	GetPackageData( int iPackageItemID, std::vector< PackageItemData >& vecPackageData );
		bool	IsShowPackageItem( int iItemID );
		//}}
#ifdef SERV_TOONILAND_CHANNELING
		bool	IsShowItemTooniland( int iItemId );
#endif //SERV_TOONILAND_CHANNELING

#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_WISH_LIST_NO_ITEM
		bool	AddCashItem( vector< KBillProductInfo >& vecKBillProductInfo, std::set< int >& setWishListNoItemList );
#else	//SERV_WISH_LIST_NO_ITEM
		bool	AddCashItem( vector< KBillProductInfo >& vecKBillProductInfo );
#endif //SERV_WISH_LIST_NO_ITEM
#else // SERV_GLOBAL_BILLING
		bool	AddCashItem( vector< KNXBTProductInfo >& vecKNXBTProductInfo );
#endif // SERV_GLOBAL_BILLING
		bool	AddCashItemEx();
		bool	GetIsUpdateCashItem();
		void	ClearCashItemList();
			
		void	GetShopItemList( int mapID, vector<const CX2Item::ItemTemplet*>& itemTempletList );
		bool    GetNpcShopItemList( int mapID, int npcID, vector<const CX2Item::ItemTemplet*>& itemTempletList );

		//{{ kimhc // 2009-12-22 // ���� �����ۿ� ���� Ű�� �����ֵ��� �߰�
#ifdef	MULTI_KEY_RANDOM_ITEM
		typedef	std::multimap<int,RandomItemData*>::iterator RandomItemItr;

		std::pair<RandomItemItr,RandomItemItr> GetRandomItemData( int itemID );
		bool IsRandomItem( std::pair<RandomItemItr,RandomItemItr>& pairValue );
#else	MULTI_KEY_RANDOM_ITEM
		RandomItemData* GetRandomItemData( int itemID );
#endif	MULTI_KEY_RANDOM_ITEM
		//}} kimhc // 2009-12-22 // ���� �����ۿ� ���� Ű�� �����ֵ��� �߰�

		ManufactureData* GetManufactureData( int ManufactureID );

		//{{ kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		bool	GetManufactureVillageData( int iHouseID, std::vector< int >& vecManufactureVillage );	
#else	ADD_HOUSE_ID_TO_MANUFACTURE
		bool	GetManufactureVillageData( int iVillageMapID, std::vector< int >& vecManufactureVillage );	
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-11-23 // ���� ������ NPC ���� �����ϵ���
		
		const ManufactureResultGroupData* GetManufactureResultGroupData( int iResultGroupID );

		CashItem*	GetCashItem( int itemID );
#ifdef SERV_GLOBAL_BILLING
		wstring		GetCashItemPeriod( int iProductNo );
		KBillProductInfo* GetCashItemProduct( int iProductNo );
#else // SERV_GLOBAL_BILLING
		wstring		GetCashItemPeriod( unsigned long ulProductNo );
		KNXBTProductInfo* GetCashItemProduct( unsigned long ulProductNo );
#endif // SERV_GLOBAL_BILLING
		vector<int> GetNewCashItemList();
		vector<int> GetHotCashItemList();
		vector<int> GetRecommendCashItemList();
		vector<int> GetLimitedCashItemList();
		bool		CheckLimitedCashItem( int itemID );
		bool		CheckEventCashItem( int itemID );
#ifdef CASH_ITEM_HOT_ICON
		bool		CheckHotIconCashItem( int itemID );
#endif CASH_ITEM_HOT_ICON
		vector<int> GetAllCashItemList();

#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		bool			AddSetItemData_LUA();
		const SetItemData*	GetSetItem( int setID );
		int             GetSetItemOptions( const int iSetID_, const int iNumOfEquippedItems_, IN vector< int >& vecOptions );
		void			GetSetItemList( int setID, vector< int >& vecItemID );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
		bool	AddItemTempletTrans_LUA();
#endif SERV_ITEM_LUA_TRANS_DEVIDE
#ifdef LUA_TRANS_DEVIDE
		bool	AddSetItemDataTrans_LUA();
#endif LUA_TRANS_DEVIDE
#endif //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
		bool			AddHideSetDesc_LUA();
		bool			GetHideSetDescItem(int itemID);
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC

#ifdef PACKAGEITEM_SET_NOT_EQUIP_ITEM
		std::map< int, std::vector< PackageItemData > > GetAllPackageData() { return m_mapPackageData; }
#endif PACKAGEITEM_SET_NOT_EQUIP_ITEM

#ifdef EQUIP_STRIP_TEST
		void		UpdateEqip( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, IN OUT vector<CX2EqipPtr>& viewEqipList, CX2ItemManager::EquipPositionMap& mapBasicEquip, bool bLoad = true );
		void		ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, IN OUT vector<CX2EqipPtr>& viewEqipList, CX2ItemManager::EquipPositionMap& mapBasicEquip, bool bLoad = true );
		CX2EqipPtr	ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, IN OUT vector<CX2EqipPtr>& viewEqipList, bool* pViewEqipPosition, CX2Unit::EQIP_POSITION eqipPos, int itemID, bool bLoad = true );

#else EQUIP_STRIP_TEST
//{{AFX
		void	UpdateEqip( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, IN OUT vector<CX2EqipPtr>& viewEqipList, bool bLoad = true );
		void	ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, IN OUT vector<CX2EqipPtr>& viewEqipList, bool bLoad = true );
		void	ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, IN OUT vector<CX2EqipPtr>& viewEqipList, bool* pViewEqipPosition, CX2Unit::EQIP_POSITION eqipPos, int itemID, bool bLoad = true );
//}}AFX
#endif EQUIP_STRIP_TEST

		void	CheckEmptyEqipPosition( bool* pViewEqipPosition, IN OUT vector<CX2EqipPtr>& viewEqipList );


#ifndef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		const CX2Item::ItemTemplet* GetItemTemplet( const int itemID );
		int		SerchByUnit( vector<const CX2Item::ItemTemplet*>& itemList, CX2Unit::UNIT_CLASS unitClass );
		int		GetItemNum() { return (int)m_ItemTempletIDMap.size(); }
		const map<int, const CX2Item::ItemTemplet*>& GetItemTempletIDMap() const { return m_ItemTempletIDMap; }
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		void	GetItemList( vector<int>& vecItemID, int page, int quantityPerPage );


		static bool IsEDItem( int itemID ) 
		{
			switch( itemID )
			{
			case BRONZE_ED_ITEM_ID:
			case SILVER_ED_ITEM_ID:
			case GOLD_ED_ITEM_ID:
			case POSTBOX_ED_ITEM_ID:
#ifdef FIELD_BOSS_RAID
			case GOLD_BAR_ED_ITEM_ID:
#endif // FIELD_BOSS_RAID
				{
					return true;
				}
			}

			return false;
		}

#ifdef ADD_SERVER_GROUP
		void ClearItemExchangeData();
		void ClearManufactureData();
		void ClearManufactureVillageData();
		void ClearManufactureResultGroupData();
#endif

//{{ kimhc // 2010.12.9 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		static CX2EqipPtr CreateDummyEquip( int iItemID_, CX2Unit* pUnit_, CKTDGXSkinAnimPtr pXSkinAnim_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.9 //  2010-12-23 New Character CHUNG

		//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

#ifndef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		void UpdateSetIDAndMaxLevelMap( const int iSetID_, const int iLevel_ );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		void UpdateMaxLevelToSetItemData();
		//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

#ifdef COOLTIME_SHARE_GROUP_ITEM
		void	AddCoolTimeGroupItem_LUA( int iCoolTimeGroupID, int iSourceItemID );
		int		GetCoolTimeGroupID( int iGroupID ) const;
#ifndef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
		void	GetCoolTimeGroupItemIDList( int iCoolTimeGroupID, vector< int >& vecItemID ) const;
#endif //SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
#endif COOLTIME_SHARE_GROUP_ITEM

#ifdef IMPOSSIBLE_PRESENT_CASH_ITEM
		bool	IsImpossiblePresentItem(int iItemID)const{ return (m_setImpossiblePresentItem.find(iItemID) != m_setImpossiblePresentItem.end()); }
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM

		//{{ 2011. 10. 18	������	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
		bool SetItemLevelEnchantData_LUA( int iAtkPhysic, int iAtkMagic, int iDefPhysic, int iDefMagic, int iBaseHP );
		bool GetItemLevel( IN const KItemInfo& kItemInfo, OUT int& iItemLevel );
		void GetCaculatedItemLevelByEnchantStat( IN const KStat& kEnchantItemStat, OUT int& iItemLevel );	
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
		//}}

#ifdef ICE_HEATER_PRE_VIEW
	#ifdef ICE_HEATER_RANDOM_PRE_VIEW // ����ȯ
		bool	AddIceHeaterItemData_LUA( int iIceHeaterItemID_, int iItemID_, int eUnitType_, int iItemGroup );
	#else //ICE_HEATER_RANDOM_PRE_VIEW
		bool	AddIceHeaterItemData_LUA( int iIceHeaterItemID_, int iItemID_, int eUnitType_ );
	#endif //ICE_HEATER_RANDOM_PRE_VIEW

		bool	GetIceHeaterItemData( int iIceHeaterItemID_, CX2Unit::UNIT_TYPE eUnitType_, OUT std::vector< int >& vecIceHeaterItemData_ );
#endif //ICE_HEATER_PRE_VIEW

#ifdef PET_PREVIEW
		bool	AddPetPreViewData_LUA( int iCashShopItemID_, int iPetUnitID_, PREVIEW_PET_TYPE ePetType_ = PPT_PET );
		int		GetPreviewPetIdByItemID( int iItemID_ );
		int		GetPreviewRidingPetIdByItemID( int iItemID_ );
		int		GetPreviewIDByItemID( int iItemID_, PREVIEW_PET_TYPE ePetType_);
#endif // PET_PREVIEW

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		void	AddItemConvertInfo_LUA();
		void	AddItemConvertGroup_LUA(){}
		int		GetConvertResultItemID( int iTargetItemID_ );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef MODIFY_FIND_SHOP_IMAGE_FILE_NAME
		void CreateShopImageNameList();	
#endif // MODIFY_FIND_SHOP_IMAGE_FILE_NAME

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		inline void	 SetKeepShowItemList( std::map<int,std::vector<int>> TempKeepShowList ){ m_mapKeepShowItemList = TempKeepShowList;}
		bool	CheckKeepShowItem( int TempItemID , std::vector<int>& GetItemID );
		inline void	 SetDisCountList( std::map< int,kDisCountItemInfo > TempDisCountList ){ m_DisCountInfoMap = TempDisCountList;}
		bool	CheckDisCountItemList( int TempProDuct,bool& bIdexReset );
		bool    DisCountItemID_Find(int TempID, std::vector<int>& nTempList,int& nTempSale);
		inline std::map< int , kDisCountItemInfo > GetDisCountListProDcut(void){ return m_DisCountInfoMap; }
		int		GetChangeCashPoint(int TempItemID);
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

	protected:
		//{{ 080721.hoons.
#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		bool	TokenizeByScaleRotate( std::string& strTokenInfo, CX2Item::ItemTemplet* pItemTemplet, CX2Unit::UNIT_TYPE unitType );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		//}}
	
    private:

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

        std::vector<BYTE>                       m_vecItemTempletInfo;

#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		map<int, const CX2Item::ItemTemplet*>			m_ItemTempletIDMap;		
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


		map<int, ItemIDList*>					m_ShopItemList;
		map<int, ItemIDList*>					m_ShopItemGroup;
		map< std::pair<int,int>, NpcItemIDList*>	m_ShopNpcItemList;	// Key :: arg 1 : village id, arg 2 : npc id
		
		
		map<int, ManufactureData*>				m_mapManufacture;
		map< int, std::vector<int> >			m_mapManufactureVillage;
		map<int, ManufactureResultGroupData>	m_mapManufactureResultGroupData;
		//{{ kimhc // 2009-12-22 // ���� �����ۿ� ���� Ű�� �����ֵ��� �߰�
#ifdef	MULTI_KEY_RANDOM_ITEM
		std::multimap<int, RandomItemData*>		m_multimapRandomItem;
#else	MULTI_KEY_RANDOM_ITEM
		map<int, RandomItemData*>			    m_mapRandomItem;
#endif	MULTI_KEY_RANDOM_ITEM
		//}} kimhc // 2009-12-22 // ���� �����ۿ� ���� Ű�� �����ֵ��� �߰�
		

		map<int, CashItem* >					m_mapCashItem;
		vector< int >							m_vecCashItem;
		vector< int >							m_vecNewCashItem;
		vector< int >							m_vecHotCashItem;
		vector< int >							m_vecRecommendCashItem;
		vector< int >							m_vecLimitedCashItem;
		vector< int >							m_vecEventCashItem;
#ifdef CASH_ITEM_HOT_ICON
		vector< int >							m_vecHotIconCashItem;
#endif CASH_ITEM_HOT_ICON
#ifdef IMPOSSIBLE_PRESENT_CASH_ITEM
		std::set<int>							m_setImpossiblePresentItem;
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM

//{{ 2013.01.26 �ֹ�ö ť����Ű�������� �̸�������
#ifdef PACKAGE_IN_QUBE_PREVIEW
		std::map< int, std::map< int, CubePackageData > > m_mapCube_PackageData;
#endif PACKAGE_IN_QUBE_PREVIEW
//}}
		//{{ 2008. 4. 24  ������  ��Ű�� ������
		std::map< int, std::vector< PackageItemData > > m_mapPackageData;
		std::set< int >							m_setShowPackageItem;
		//}}

#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		map< int, SetItemData* >				m_mapSetItemData;
		map< int, std::vector<int> >			m_mapSetItemIDList;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		//{{ 2008. 11. 18  ������	�����۱�ȯ
		map< int, int >							m_mapItemExchange;
		//}}


#ifdef NEW_ITEM_EXCHANGE_TEST
		std::map< int, std::vector< ItemExchangeData > > m_mapItemExchangeData;
#endif NEW_ITEM_EXCHANGE_TEST


		//int										m_nString_Index;


		//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		// first: SetID, second: MaxLevel In SetItem �� �Ͽ� 
		// ��Ʈ ȿ�� ����, ��Ʈ ���� ������ �� ���� ���� ������ ������ �ִ� �ڷᱸ��
		// �� �ڷᱸ���� SetItem.lua�� �Ľ��� ��� ���� �Ŀ� clear �Ѵ�.
		typedef std::map<int, int> SetIDAndMaxLevelMap;

		SetIDAndMaxLevelMap						m_mapSetIDAndMaxLevel;

		//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


#ifdef COOLTIME_SHARE_GROUP_ITEM
		map < int, int >						m_mapCoolTimeGroupID;
		map < int, vector<int> >				m_mapCoolTimeGroupItemIDList;
#endif COOLTIME_SHARE_GROUP_ITEM

		//{{ 2011. 10. 17	������	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
		KStat									m_kItemLevelEnchantData;
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
		//}}

#ifdef ICE_HEATER_PRE_VIEW
		std::map< int, std::vector< IceHeaterItemData > > m_mapIceHeaterItemData;
#endif //ICE_HEATER_PRE_VIEW


#ifdef PET_PREVIEW
		std::vector< PREVIEW_PET_DATA > m_vecPetPreViewData; // ������ID, (��ID, ��Ÿ��)
#endif // PET_PREVIEW

#ifdef PACKAGE_IN_QUBE_PREVIEW
		int 									m_iLastPackacgeID;
#endif PACKAGE_IN_QUBE_PREVIEW
		//}} 2013.01.26 �ֹ�ö ť����Ű�������� �̸�������

#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
		std::map< int, bool > m_mapHideSetDescItem; // ������ID, hide����
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		typedef std::map<int, int> ItemConvertList; // <���� ��� ������ID, ���� ��� ������ ID>

		// NPC ID, <���� ��� ������ID, ���� ��� ������ ID>
		std::map< int, ItemConvertList> m_mapNpcItemConvertList;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		std::map<int,std::vector<int>>						m_mapKeepShowItemList;
		std::map< int, kDisCountItemInfo >					m_DisCountInfoMap;
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
};

