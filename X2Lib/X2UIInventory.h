#ifdef NEW_VILLAGE_UI

#pragma once

#define MAX_INVENTORY_SIZE 24

//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ��� ��ü �� �ƹ�Ÿ ��ü
#ifdef	REAL_TIME_ELSWORD
namespace	_CONST_UIINVENTORY_
{
	const	int		g_iEquippedNormalWeaponSlotID		=		9;
	const	int		g_iEquippedFashionWeaponslotID		=		10;
}
#endif	REAL_TIME_ELSWORD
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ��� ��ü �� �ƹ�Ÿ ��ü

//class CX2StateMenu;
class CX2UIInventory : public CX2UIDragable
{
public:
//	enum SORT_TYPE
//	{
//		ST_NONE = 0,
//		ST_EQUIP,
//		ST_ACCESSORY,
//		ST_MATERIAL,
//		ST_SPECIAL,
//		ST_QUEST,
//		ST_QUICK_SLOT, //�Һ�
//		ST_AVARTA,
//		ST_REPOSITORY,
//		ST_E_EQUIP,
//		ST_E_SKILL,
//		ST_E_QUICK_SLOT,
//		ST_END,
//	};

	enum UI_INVENTORY_MSG
	{
		UIM_EXIT = 11000,
		UIM_EQUIP,
		UIM_ACCESSORY,
		UIM_QUICK_SLOT,
		UIM_MATERIAL,
		UIM_QUEST,
		UIM_SPECIAL,
		UIM_AVATAR,
		UIM_PREV_PAGE,
		UIM_NEXT_PAGE,	

#ifdef SERV_REFORM_INVENTORY_TEST
		UIM_PAGE_1,
		UIM_PAGE_2,
		UIM_PAGE_3,
//#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		UIM_PAGE_4,
//#endif SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR	
		UIM_OVER_PAGE_1,
		UIM_OVER_PAGE_2,
		UIM_OVER_PAGE_3,
//#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		UIM_OVER_PAGE_4,
//#endif SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR	
#endif SERV_REFORM_INVENTORY_TEST

		UIM_OPEN_RANDOM_ITEM = 11100,
		UIM_CLOSE_RANDOM_ITEM_RESULT,
		UIM_OPEN_RANDOM_ITEM_NEW,
		UIM_EXIT_RANDOM_ITEM_NEW,

		UIM_CREATE_PERSONAL_SHOP = 11200,

		UIM_RESOLVE_ITEM_OK			= 11300,
		UIM_RESOLVE_ITEM_CANCLE,
		UIM_RESOLVE_ITEM_RESULT_OK,
		UIM_RESOLVE_ITEM_RESULT_PREV_PAGE,
		UIM_RESOLVE_ITEM_RESULT_NEXT_PAGE,
		//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
//#ifdef SERV_MULTI_RESOLVE
		UIM_RESOLVE_ITEM_OPEN,
//#endif SERV_MULTI_RESOLVE

		UIM_SOCKET_DLG_CLOSE				= 11400,
		UIM_SOCKET_NORMAL_STONE,
		UIM_SOCKET_RARE_STONE,
		UIM_SOCKET_REMOVE,
		UIM_SOCKET_FEE_OK,

		UIM_EL_CHANGER_EXIT		= 11500,
		UIM_EL_CHANGER_NUM_PLUS,
		UIM_EL_CHANGER_NUM_MINUS,
		UIM_EL_CHANGER_NUM_UPDATE,
		UIM_EL_CHANGER_OK,
		UIM_EL_CHANGER_RESULT_OK,

		UIM_ITEM_RECOVERY_OK = 11600,
		UIM_ITEM_USE_WARP,		
		UIM_SKILL_PLUS_ITEM_USE,

//#ifdef SERV_TREASURE_BOX_ITEM
		UIM_TREASURE_BOX_CANCLE			= 11650,
//#endif SERV_TREASURE_BOX_ITEM

		UIM_ATTRACTION_ITEM_OK			= 11700,
		UIM_ATTRACTION_ITEM_CANCLE,
		UIM_ATTRACTION_ITEM_RESULT_OK,

		UIM_USE_COUPON_ITEM_OK,
		UIM_USE_COUPON_ITEM_CANCLE,

		UIM_OPEN_RANDOM_ITEM_CANCLE,
		UIM_ATTRACTION_ITEM_DUMMY,					// ESC ����� : �ƹ��͵� �� �ϴ� �޽���

		UIM_SELL_CATEGORY_ALL,
		UIM_DELETE_ITEM,

//#ifdef SERV_ATTRIBUTE_CHARM
		UIM_USE_ATTRIBUTE_CHARM_COUPON_ITEM_OK,
//#endif SERV_ATTRIBUTE_CHARM

		//{{ 2012. 02. 21	��μ�	�κ��丮 ����
//#ifdef SERV_SORT_CATEGORY_ITEM 
		UIM_SORT_CATEGORY_ITEM,
//#endif SERV_SORT_CATEGORY_ITEM 
		//}}

		//{{ kimhc // �ǽð� ���ҵ� �� ������ ������ ������
//#ifdef REAL_TIME_ELSWORD

		UIM_REGISTER_QUANTITY_CONFIRM = 11750,
		UIM_REGISTER_QUANTITY_CANCEL,
		UIM_REGISTER_QUANTITY_PLUS,
		UIM_REGISTER_QUANTITY_MINUS,
		UIM_REGISTER_QUANTITY_UPDATE,
		UIM_DELETE_ITEM_CONFIRM_OK,
		UIM_DELETE_ITEM_CONFIRM_CANCEL,

//#endif	REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� ������ ������ ������

		UIM_CHANGE_NICK_NAME_OK			= 11800,
		UIM_SKILL_INIT_OK,

		UIM_ITEM_USE_TITLE, 		


		UIM_ITEM_USE_SKILL_UNSEAL,

		//{{ kimhc // 2009-08-19 // ���� ���� ���̾�α�
		UIM_SEAL_ITEM_CONFIRM_OK,
		UIM_SEAL_ITEM_CONFIRM_CANCEL,
		UIM_UNSEAL_ITEM_CONFIRM_OK,
		UIM_UNSEAL_ITEM_CONFIRM_CANCEL,
		//}} kimhc // 2009-08-19 // ���� ���� ���̾�α�

		UIM_WARP_DEST_CLOSE,
		UIM_WARP_DEST_SELECT,
		UIM_WARP_DEST,	
		UIM_GUILD_SKILL_INIT_OK,

		UIM_PC_BANG,

		UIM_SKILL_NOTE,
		UIM_UNKNOWN_TO_IDENTIFY, // �� �Ӽ� �Ǻ�

//#ifdef SERV_PET_SYSTEM
		UIM_PET_FOOD,	// �� �����ֱ�
		UIM_PET_FOOD_OK,
//#ifdef ADD_PET_INVENTORY_BUTTON
		UIM_PET_INVENTORY_OPEN,
//#endif ADD_PET_INVENTORY_BUTTON
//#endif
//#ifdef BUFF_TEMPLET_SYSTEM
		UIM_ITEM_USE_DEFENCE_DUNGEON_POTION,
//#endif BUFF_TEMPLET_SYSTEM
//#ifdef SERV_PSHOP_AGENCY
		UIM_CHECK_PSHOP_OK,
//#endif
		UIM_WARP_DEST_OK,
//#ifdef PET_DROP_ITEM_PICKUP
		UIM_PET_DROP_ITEM_PICK_UP_OK,
//#endif //PET_DROP_ITEM_PICKUP
//#ifdef SPT_NONE_ITEM_DELETE
		UIM_SPTNONE_ITEM_DELETE_OK,
		UIM_SPTNONE_ITEM_DELETE_CANCEL,
//#endif //SPT_NONE_ITEM_DELETE
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //�̶�Ŭť��
		UIM_USE_RESTORE_ITEM_EVALUATION_OK,
		UIM_USE_RESTORE_ITEM_EVALUATION_CANCEL,
		UIM_USE_EVALUATE_ITEM_OK,
		UIM_USE_EVALUATE_ITEM_CANCEL,
		UIM_EXCHANGE_NEW_ITEM_OK,
		UIM_EXCHANGE_NEW_ITEM_CANCEL,
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
//#ifdef RIDING_ITEM_POPUP_CHECK
		UIM_RIDING_ITEM_USE_OK,
//#endif //RIDING_ITEM_POPUP_CHECK

//#ifdef FINALITY_SKILL_SYSTEM //JHKang
		UIM_EL_BREAK_EXIT,
		UIM_EL_BREAK_NUM_PLUS,
		UIM_EL_BREAK_NUM_MINUS,
		UIM_EL_BREAK_NUM_UPDATE,
		UIM_EL_BREAK_OK,
		UIM_EL_BREAK_RESULT_OK,
//#endif //FINALITY_SKILL_SYSTEM

//#ifdef GOOD_ELSWORD //JHKang
		UIM_EXPAND_INVENTORY_CASH,
		UIM_EXPAND_INVENTORY_ED,
		UIM_EXPAND_INVENTORY_ED_BUY_CONFIRM,
		UIM_EXPAND_INVENTORY_ED_BUY_CANCEL,
//#endif //GOOD_ELSWORD
//#ifdef FIELD_BOSS_RAID // ���� ���� Ȯ��
		UIM_USE_ADD_SOCKET_SLOT_OK		= 11841,
		UIM_USE_ADD_SOCKET_SLOT_CANCEL	= 11842,
//#endif // FIELD_BOSS_RAID
//#ifdef REFORM_SKILL_NOTE_UI
		UIM_BUY_SKILL_NOTE_GUIDE_MOUSE_UP = 11843,
//#endif // REFORM_SKILL_NOTE_UI
//#ifdef SERV_GOLD_TICKET
		UIM_USE_GOLD_TICKET,
//#endif SERV_GOLD_TICKET
//#ifdef SERV_TOUR_TICKET_EVENT
		UIM_USE_TOUR_TICKET,
//#endif SERV_TOUR_TICKET_EVENT
//#ifdef SERV_RELATIONSHIP_EVENT_INT
		UIM_USE_PROPOSE_ITEM,
		UIM_USE_PROPOSE_ITEM_OK,
		UIM_USE_PROPOSE_ITEM_CANCLE,
		UIM_USE_DIVORCE_ITEM,
//#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
		UIM_PET_HALLOWEEN_TRANSFORM_POSION_OK,
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
#ifdef SERV_RECRUIT_EVENT_BASE
		UIM_RECOMMEND_OK,
		UIM_RECOMMEND_CANCEL,
#endif SERV_RECRUIT_EVENT_BASE
#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
		UIM_USE_EXPAND_INVENTORY_OK,
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		UIM_UNLIMITED_SECOND_CHANGE_JOB_CLOSE,
		UIM_UNLIMITED_SECOND_CHANGE_JOB_SELECT,
		UIM_UNLIMITED_SECOND_CHANGE_JOB_DEST,	
		UIM_UNLIMITED_SECOND_CHANGE_JOB_OK,
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
//#ifdef SERV_RECRUIT_EVENT_BASE
//		UIM_RECOMMEND_OK,
//		UIM_RECOMMEND_CANCEL,
//#endif SERV_RECRUIT_EVENT_BASE
//#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
//		UIM_USE_EXPAND_INVENTORY_OK,
//#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	};

	enum RANDOM_ITEM_OPEN_EVENT_TYPE
	{
		RIOET_NONE,
		RIOET_ICE_HEATER,			// ������
		RIOET_MONSTER_CARD_SET,		// ��ϸ� �ð� ���� ���� ī�� ��Ʈ
#ifdef APRIL_5TH_TREE_DAY_EVENT
		RIOET_APRIL_5TH_TREE_DAY_EVENT,		// �ĸ��� �̺�Ʈ
#endif
		RIOET_GOLD_ICE_HEATER,		// ��� ���� ������ ������
//#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		RIOET_MINI_ICE_HEATER,		// �̴� ���� ������ ������
//#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
//#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
		RIOET_ALL_IN_ONE_ICE_HEATER, // ���ο� ���� ������ ������
//#endif SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
#ifdef RIDING_SYSTEM
		RIOET_RIDING_PET_GACHA,  //Ż �� ��í
#endif RIDING_SYSTEM

#ifdef SERV_HALLOWEEN_ICE_HEATER
		RIOET_HALLOWEEN_ICE_HEATER,
#endif //SERV_HALLOWEEN_ICE_HEATER

		//{{100416 ������ Ʈ���� �ڽ� �߰�
//#ifdef 	SERV_TREASURE_BOX_ITEM
		RIOET_TREASURE_BOX_ITEM,		// Ʈ���� �ڽ�
//#endif 	SERV_TREASURE_BOX_ITEM
		//}}100416 ������ Ʈ���� �ڽ� �߰�

//#ifdef CUBE_OPEN_IMAGE_MANAGER
		RIOET_EVENT_CUBE,			// �̺�Ʈ �n
//#endif CUBE_OPEN_IMAGE_MANAGER
	};

	//kimhc // 2009-08-12 // ����ť�긦 ���� ���̾�α׸� �ٸ��� ó���ؾ��� ���� ī�� ������
	enum MONSTER_CARD_SET_ID
	{
		MONSTER_CARD_SET_RUBEN	= 69353,
		MONSTER_CARD_SET_ELDER,
		MONSTER_CARD_SET_BESMA,
		MONSTER_CARD_SET_PEITA,
		MONSTER_CARD_SET_ALTERA,
		MONSTER_CARD_SET_SECRET,
		MONSTER_CARD_SET_RUBEN_GOLD_EDITION,
		MONSTER_CARD_SET_ELDER_GOLD_EDITION,
		MONSTER_CARD_SET_BESMA_GOLD_EDITION,
		MONSTER_CARD_SET_PEITA_GOLD_EDITION,
		MONSTER_CARD_SET_ALTERA_GOLD_EDITION,
		MONSTER_CARD_SET_SECRET_GOLD_EDITION,
		MONSTER_CARD_SET_ELIOS,
		MONSTER_CARD_SET_ELIOS_GOLD_EDITION,
		MONSTER_CARD_SET_ELIOS_ALL_EDITION,

	};
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	//������ // 2012-03-07 // ȹ���� ������ ��� Ȯ�� â
	enum ITEM_OBTAIN_RESULT_TYPE
	{
		IORT_NONE,
		IORT_CUBE_OPEN,			//ť�� ����
		IORT_RANDOM_EXCHANGE,	//���� ��ȯ
		IORT_ITEM_RESOLVE,		//������ ����
	};
#endif RANDOM_EXCHANGE_RESULT_VIEW

#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// ���� ������ ������ Ÿ�̹�
	enum ATTRACTION_ITEM_TIME_INFO
	{
		AITI_NONE,
		AITI_START_PARTICLE_1,	/// ù��° ��ƼŬ ���� �ð�
		AITI_START_PARTICLE_2,	/// �ι�° ��ƼŬ ���� �ð�
		AITI_START_PARTICLE_3,	/// ����° ��ƼŬ ���� �ð�
		AITI_END_PARTICLE,		/// ���� ��ƼŬ ���� �ð�
		AITI_END_TIME,			/// ���� �ð�
		AITI_END,
	};
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	enum ATTRACTION_ITEM_PARTICLE_NAME_INFO
	{
		AIPNI_NONE,
		AIPNI_START_PARTICLE_1,	/// ù��° ��ƼŬ �̸�
		AIPNI_START_PARTICLE_2,	/// �ι�° ��ƼŬ �̸�
		AIPNI_START_PARTICLE_3,	/// ����° ��ƼŬ �̸�
		AIPNI_START_PARTICLE_4,	/// ����° ��ƼŬ �̸�
		AIPNI_START_PARTICLE_5,	/// ����° ��ƼŬ �̸�
		AIPNI_END_PARTICLE_1,	/// ���� ��ƼŬ �̸�
		AIPNI_END_PARTICLE_2,	/// ���� ��ƼŬ �̸�
		AIPNI_END,
	};
#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

#ifdef FIX_ICE_HEATER_EVENT
	
	struct IceHeaterEventData
	{
		wstring m_wstrWaitUIEffectSetName;			/// ������ ��� ���� ����Ʈ�� �̸�
		wstring m_wstrPlayUIEffectSetName;			/// ������ ��� ���� ����Ʈ �� �̸�

		wstring m_wstrWaitMeshPlayerName;			/// ������ �޽� ��ü �̸�
		wstring m_wstrPlayMeshPlayerName;			/// ������ �޽� ��ü �̸�

		wstring m_wstrMainDialogTitleTexture;		/// ������ ���� ���̾�α� Ÿ��Ʋ �ؽ�ó �̸�
		wstring m_wstrMainDialogTitleTextureKey;	/// ������ ���� ���̾�α� Ÿ��Ʋ �ؽ�ó Ű

		wstring m_wstrResultDialogTitleTexture;		/// ������ ��� ���̾�α� Ÿ��Ʋ �ؽ�ó �̸�
		wstring m_wstrResultDialogTitleTextureKey;	/// ������ ��� ���̾�α� Ÿ��Ʋ �ؽ�ó Ű

		float	m_fCautionIconOffSetPositionX;		///	������ ���̾�α׿� ǥ�õ� ����ǥ �������� ������ X ��ġ
		float	m_fCautionIconOffSetPositionY;		///	������ ���̾�α׿� ǥ�õ� ����ǥ �������� ������ Y ��ġ

		float	m_fCautionStringOffSetPositionX;	/// ������ ���̾�α��� ����ǥ ������ ���� ǥ�õ� ��Ʈ���� ������ X ��ġ
		float	m_fCautionStringOffSetPositionY;	/// ������ ���̾�α��� ����ǥ ������ ���� ǥ�õ� ��Ʈ���� ������ Y ��ġ

		wstring	m_wstrIceHeaterPlaySound;			/// ������ ���� �����

		int		m_iCautionStringNum;				/// ������ ���� �ȳ� ���� ��Ʈ�� ���̵�

		float	m_fPlayTime;						/// ��� �ð� ( ������ ���� ~ ������� �ð� )

		IceHeaterEventData()
		: m_wstrWaitUIEffectSetName( L"" )
		, m_wstrPlayUIEffectSetName( L"" )
		, m_wstrWaitMeshPlayerName( L"" )
		, m_wstrPlayMeshPlayerName( L"" )
		, m_wstrMainDialogTitleTexture( L"" )
		, m_wstrMainDialogTitleTextureKey( L"" )
		, m_wstrResultDialogTitleTexture( L"" )
		, m_wstrResultDialogTitleTextureKey( L"" )
		, m_fCautionIconOffSetPositionX( 0.f )
		, m_fCautionIconOffSetPositionY( 0.f )
		, m_fCautionStringOffSetPositionX( 0.f )
		, m_fCautionStringOffSetPositionY( 0.f )
		, m_wstrIceHeaterPlaySound( L"" )
		, m_iCautionStringNum( 0 )
		, m_fPlayTime( 0.f )
		{}

	};

#endif FIX_ICE_HEATER_EVENT

public:

	CX2UIInventory( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIInventory(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL

	bool GetShow() { return m_bShow; }
	void SetShow(bool val);

	void SetLayer(X2_DIALOG_LAYER layer);
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition(D3DXVECTOR2 vec, bool bAbsolute = true );

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	// ������ // 2012/03/02 // ȹ�� ���â�� ����ϱ� ���� ������ ȹ�� Ÿ���� �����ϴ� �Լ�(ť�����, ���������۱�ȯ, ������ ����)
	void SetItemObtainResultType(CX2UIInventory::ITEM_OBTAIN_RESULT_TYPE eType){m_eItemObtainResultType = eType;}

#if defined( SERV_MULTI_RESOLVE ) || defined( EXCHANGE_OPEN_IMAGE )
	CX2UIInventory::ITEM_OBTAIN_RESULT_TYPE GetItemObtainResultType(){ return m_eItemObtainResultType; }
#endif // SERV_MULTI_RESOLVE

#endif RANDOM_EXCHANGE_RESULT_VIEW

#ifdef SERV_SOCKET_NEW
	// oasis907 : ����� [2010.4.6] // 
	CX2UISocketItem* GetUISocketItem(){ return m_pUISocketItem; }		
#endif SERV_SOCKET_NEW

	// �κ� ����
	bool ChangeInventoryTab( CX2Inventory::SORT_TYPE sortType );
	bool SetInventorySort( CX2Inventory::SORT_TYPE sortType, int pageNum = 1 );
	bool ChangeInventoryTabByUid( UidType iItemUID );
	bool UpdateInventorySlot();

	void UpdateUIButton();

#ifndef	SERV_REFORM_INVENTORY_TEST
	void ResetInvenPage( int nowPage, int maxPage );
	void ResetInvenPageUI();
	void NextInvenPage();
	void PrevInvenPage();
#endif SERV_REFORM_INVENTORY_TEST

	CX2Inventory::SORT_TYPE GetSortType(){ return m_NowInventorySortType; }
	int GetNowInvenSortTypePageNum();
	bool UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo, bool bInventoryUIUpdate = true );

	// ������ �ʿ��� �˷��ִ� ���ο� �����ۿ��� ����
	void SetNewItem(bool val) { m_bIsNewItem = val;}
	bool GetNewItem() { return m_bIsNewItem; }

	bool GetOverMouseInven();

	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem );
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );

	//{{ 2012. 02. 21	��μ�	�κ��丮 ����
#ifdef SERV_SORT_CATEGORY_ITEM 
	bool Handler_EGS_SORT_CATEGORY_ITEM_REQ( CX2Inventory::SORT_TYPE NowInventorySortType );
	bool Handler_EGS_SORT_CATEGORY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SORT_CATEGORY_ITEM 
	//}}


	// �κ��� ��������
#ifdef BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid, UidType iTempCode = 0, int iItemID = 0);
#else  //BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid, int iTempCode = 0);
#endif //BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );       

#ifdef SERV_SHARING_BANK_TEST
	//{{ ���� : ���ణ ��ȯ ������ ���������� üũ�ϴ� �Լ�
	bool CheckCanMoveShareItem(CX2Item* pFromItem, CX2Item* pToItem, KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ &kMoveInfo);
	//}}
	bool Handler_EGS_UPDATE_ITEM_POSITION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SHARING_BANK_TEST

#ifdef SERV_TREASURE_BOX_ITEM
	bool GetPlayTreasureBox() 
	{ //���ȭ���� �׻� �����ִ�.
		if(m_pDLGGetAttractionItemBG != NULL)
			return m_pDLGGetAttractionItemBG->GetShow(); 		
		return false;
	}
#endif SERV_TREASURE_BOX_ITEM

	//{{ kimhc // �ǽð� ���ҵ� �� ������ ������
#ifdef REAL_TIME_ELSWORD
	bool GetOpenDeleteItemConfirmDLG();
	bool GetOpenRegisterQuantityDLG();
#endif
	//{{ kimhc // �ǽð� ���ҵ� �� ������ ������

#ifdef ATTRACTION_ITEM_TEST
	bool GetPlayAttractionItem() 
	{ 
		if(m_pDLGOpenAttraction != NULL)
			return m_pDLGOpenAttraction->GetShow(); 
		return false;
	}
#endif

#ifdef FREE_WARP
	void InitWarpList();
	void AddWarpList(CKTDGUIContolList *pControlList, const int row, const int iVillageId);
	void CreateWarpDest();
	void SetShowWarpDest(bool val);
	bool GetShowWarpDest();
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	void InitSecondJobList();
	bool AddSecondJobList(CKTDGUIContolList *pControlList, const int row, const CX2Unit::UNIT_CLASS eSecondJobId);
	void CreateSecondJobDest();
	void SetShowSecondJobDest(bool val);
	bool GetShowSecondJobDest();
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

	//kimhc // 2010-02-03 // PC�濡���� PC ���� ���� �����ϱ� ����, Ư���� ��찡 �ƴ϶�� �� �Լ��� sortype�� ���� �ϹǷ� SetInventorySort()�� ���� �ٶ�
	void SetSortType( CX2Inventory::SORT_TYPE eSortType ) { m_NowInventorySortType = eSortType; }

#ifdef SERV_SKILL_NOTE
	void HideSkillNote(bool bVal);
	void SetEnableSkillNote(bool bVal);
#endif
#ifdef REFORM_SKILL_NOTE_UI // ����� ��Ʈ ��ư �������� �ʰ� ��Ȱ��ȭ�� ����
	void SetEnableBuySkillNote( bool bVal );
	void RegisterMemo( const CX2Item::ItemTemplet* pItemTemplet_, UidType uiMemoItemUID_ );
#endif // REFORM_SKILL_NOTE_UI


#ifdef RANDOM_CUBE_UI_NEW
	CKTDGUIDialogType GetDLGResolveItemResult(){ return m_pDLGResolveItemResult; }
#endif RANDOM_CUBE_UI_NEW

#ifdef SERV_PET_SYSTEM
	void FeedPet();
	void SetEnableFeedPet(bool bVal);

	void SetPetFoodSlot(CX2SlotItem* pSlotItem){ m_pPetFoodSlot = pSlotItem; }

	void OpenPetInventory(); // oasis907 : ����� [2010.9.7] // �� �κ��丮
	CX2UIPetInventory* GetUIPetInventory(){ return m_pUIPetInventory; }
	void SetAutoFeed(bool bAutoFeed){ m_bAutoFeed = bAutoFeed; }
	bool GetAutoFeed(){ return m_bAutoFeed; }

#ifdef ADD_PET_INVENTORY_BUTTON
	bool IsPetInventoryExist();
	void UpdateButtonPetInventory();
#endif ADD_PET_INVENTORY_BUTTON
#endif
	//������ // 2012-03-06 // ���������� ��ȯ ��� â ����� ���� CX2UIItemExChangeShop ���� ��� �� �� �ֵ��� public���� �̵�
	void OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
		bool bJackpot );

#ifdef JUNK_AVATAR
	bool AuthDeleteItem( UidType DeleteItemUid_, int Quantity_  )
	{
		return Handler_EGS_DELETE_ITEM_REQ( DeleteItemUid_, Quantity_ );
	}
#endif
	void SetEDString(int iED_);
	void SetAPString(int iAP_);

#ifdef EXCHANGE_OPEN_IMAGE
	void OpenRandomItem( int iItemID, ITEM_OBTAIN_RESULT_TYPE eIORT );
#endif EXCHANGE_OPEN_IMAGE

#ifdef FIX_ICE_HEATER_EVENT
	void	OpenScriptFile( const WCHAR* szScriptFileName_ );
	void	AddIceHeaterEventInfo_LUA();
#endif // FIX_ICE_HEATER_EVENT

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	const CX2Inventory::SORT_TYPE GetInventorySortType() { return m_NowInventorySortType; }

	/// �ܺ� ������ ����, Public���� ����
	bool	UpdatePageUI( int iPage = 1 );

	/// /// ���� ī�װ� ������ �� ��ȯ ( �ܺ� ������ ����, Public���� ���� )
	int		GetNowInventoryPageNum( CX2Inventory::SORT_TYPE InventorySortType_ ) const { return m_NowInventoryPageNum[ InventorySortType_ - 1 ]; }
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool	GetIsMouseOver();
#endif // CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	void	UseTransformPetItem();
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

protected:
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	//������ // 2012-03-06 // ť�����, ���������� ��ȯ���� ȹ���� �����ۿ� ���� ���â �ʱ�ȭ
	void InitItemObtainResult(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem );
	//������ // 2012-03-06 // ������ ���ط� ȹ���� �����ۿ� ���� ���â �ʱ�ȭ
	void InitItemResolveResult(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem );
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	void InitItemObtainResult2(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem, int iSlotNum = RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
#endif RANDOM_EXCHANGE_RESULT_VIEW
	//{{ kimhc // 2009-08-10 // ����
#ifdef	PRIVATE_BANK
	bool Handler_EGS_EXPAND_BANK_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// ���� ���� Ȯ��
#endif	PRIVATE_BANK	
	//}} kimhc // 2009-08-10 // ����

	bool Handler_EGS_CHANGE_INVENTORY_SLOT_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// �κ��� ��밡���� ���������� �˻�
	bool CheckUseItem(CX2Item *pItem);


	//{{ 2007. 10. 8  ������  ���� ������ �ڵ鷯
	bool Handler_EGS_OPEN_RANDOM_ITEM_REQ();
	bool Handler_EGS_OPEN_RANDOM_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		
	//}}
	void UpdateOpenRandomItemAck();
	void UpdateOpenMonsterCardSet();

#ifdef RANDOM_CUBE_UI_NEW
	void UpdateOpenRandomItemAckNew();
#endif RANDOM_CUBE_UI_NEW
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
	void UpdateOpenMiniRandomItemAckNew();
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

	//�� ����, �ǵ�����
	void OpenElChangerWindow( bool bIdentify, CX2Item* pItem );
	void UpdateElChangerDLG( bool bReadIME );
	void OpenElChangerResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo );
	bool Handler_EGS_IDENTIFY_ITEM_REQ();
	bool Handler_EGS_IDENTIFY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ����
	void OpenResolveWindow( CX2SlotItem* pSlot );
	//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
	void OpenResolveWindow();
#endif SERV_MULTI_RESOLVE
	//}}
	void ChangeResolveResultPage( bool bNextPage );
	//������ // 2012-03-06 // ���������� ��ȯ ��� â ����� ���� CX2UIItemExChangeShop ���� ��� �� �� �ֵ��� public���� �̵�
// 	void OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
// 							      bool bJackpot );
	bool Handler_EGS_RESOLVE_ITEM_REQ();
	bool Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef SERV_SOCKET_NEW
// oasis907 : ����� [2010.4.19] // 
	bool OpenNewSocketWindow( CX2SlotItem* pSlot );
#else
	// ����
	void OpenSocketWindow( CX2SlotItem* pSlot );
	void ResetSocketWindow();
	void OpenSocketCheatWindow();
	void CloseSocketCheatWindow();

	bool Handler_EGS_SOCKET_ITEM_REQ( int socketIndex, bool bIsRareMagicStone, bool bIsRemoveSocketOption );
	bool Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SOCKET_NEW

#ifdef SERV_GOLD_TICKET
	bool handler_EGS_CHARGE_POINT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_GOLD_TICKET

	//���� ����
	virtual bool MouseDown( D3DXVECTOR2 mousePos );
	virtual bool MouseUp( D3DXVECTOR2 mousePos );

	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );

	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );

	void DrawSlotMouseOverImage();
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

	// ������ ���� ����
#ifdef ITEM_RECOVERY_TEST
	void GetRequiredRecoveryStoneID( IN const int iItemLevel_, OUT vector<int>& vecRecoveryItemIdList_ );
	void GetRequiredArmorOnlyRecoveryStoneID( IN const int iItemLevel_, OUT vector<int>& vecRecoveryItemIdList_ );
	void CheckRequiredRecoveryStoneInInventory( IN OUT vector<int>& vecRecoveryItemIdList_ );
	bool Handler_EGS_RESTORE_ITEM_REQ( UidType RecoveryItemUid );
	bool Handler_EGS_RESTORE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef ATTRACTION_ITEM_TEST
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// �̴� ���� ������ ������ ó���� ����, ���� �̺�Ʈ Ÿ�� �߰�
	void PlayGetAttractionItem( RANDOM_ITEM_OPEN_EVENT_TYPE eRandomItemEventType = RIOET_ICE_HEATER );
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
	void PlayGetAttractionItem();
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
	void PlayGetAttractionItemGold();
	void UpdateOpenAttractionItemAck();
	bool IsEnchantCouponUseable(int CouponItemID, int TargetEnchantLevel );
	bool IsAttachItem( int ItemID );
	bool IsDefenceOnlyCoupon( int ItemID );

	bool Handler_EGS_ENCHANT_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID );
	bool Handler_EGS_ENCHANT_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_ATTRIBUTE_CHARM
	bool IsAttributeItem( int ItemID );
	wstring GetAttributeItemExtraDamageName( int itemTID );
	bool Handler_EGS_ATTRIB_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID );
	bool Handler_EGS_ATTRIB_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ATTRIBUTE_CHARM

#endif

#ifdef APRIL_5TH_TREE_DAY_EVENT			// �ĸ��� �̺�Ʈ ���� �ڵ�
	void UpdateOpenTreeDayItemAck();
	bool IsAprilEventRandomItem( int ItemID );
#endif

		//{{100416 ������ Ʈ���� �ڽ� �߰�
#ifdef 	SERV_TREASURE_BOX_ITEM
	void UpdateOpenTreasureBoxItemAck();
	bool IsTreasureBoxRandomItem( int ItemID );
#endif 	SERV_TREASURE_BOX_ITEM
		//}}100416 ������ Ʈ���� �ڽ� �߰�
	//}}
#ifdef	SERV_SHARING_BANK_TEST
	bool IsSealItem( CX2Item* pItem, int &iCanSealCount );	//	���� �ؾ��ϴ� ���������� üũ
#endif	SERV_SHARING_BANK_TEST
		
	bool IsMonsterCardSetItem( int iItemID );

#ifdef AUTH_DELETE_ITEM

	//{{ kimhc // �ǽð� ���ҵ� �� ������ ������ ������
#ifdef REAL_TIME_ELSWORD
	bool Handler_EGS_DELETE_ITEM_REQ( UidType DeleteItemUid, int Quantity  );
#else REAL_TIME_ELSWORD
	bool Handler_EGS_DELETE_ITEM_REQ( UidType DeleteItemUid );
#endif REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� ������ ������ ������

	bool Handler_EGS_DELETE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

	//{{ kimhc // �ǽð� ���ҵ� �� ������ ������ ������
#ifdef REAL_TIME_ELSWORD
	void OpenDeleteItemConfirmDLG();
	void OpenRegisterQuantityDLG( D3DXVECTOR2 pos );
	void UpdateRegisterQuantityDLG( bool bReadIME );
#endif REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� ������ ������ ������

	//{{ kimhc // 2009-08-19 // ���ν�Ű�� �������� ������ Ŭ�� ��
#ifdef	SEAL_ITEM
	void RClickItemForSeal( UidType itemUID );
	bool CanSealUpThisItem( CX2Item* pItem ) const;
	void ResetItemIDForSealAndCursorState();

	bool Handler_EGS_SEAL_ITEM_REQ( UidType itemUID );
	bool Handler_EGS_SEAL_ITEM_ACK( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UNSEAL_ITEM_REQ( UidType itemUID );
	bool Handler_EGS_UNSEAL_ITEM_ACK( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#endif	SEAL_ITEM
	//}} kimhc // 2009-08-19 // ���ν�Ű�� �������� ������ Ŭ�� ��

	bool CheckRadioButtonBySortType( CX2Inventory::SORT_TYPE eType );


#ifdef	SERV_REFORM_INVENTORY_TEST
	void	SetShowInventoryPageUI(int iMaxPageNum = 0 );

#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// �ܺ� ������ ����, Public���� �̵�
	bool	UpdatePageUI( int iPage = 1 );
#endif //SERV_UPGRADE_TRADE_SYSTEM

	int		GetPageNumBySlotID( int iSlotID );

	int		GetMaxLineNumPerCategory( CX2Inventory::SORT_TYPE InventorySortType_ );	/// ���� ī�װ��� �ִ� ���� �� �� ��ȯ

#ifndef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	/// �ܺ� ������ ����, Public���� �̵� ( ���� ī�װ� ������ �� ��ȯ )
	int		GetNowInventoryPageNum( CX2Inventory::SORT_TYPE InventorySortType_ ) const { return m_NowInventoryPageNum[ InventorySortType_ - 1 ]; }
#endif //SERV_UPGRADE_TRADE_SYSTEM

	/// ���� ī�װ� ������ �� ����
	void	SetNowInventoryPageNum( CX2Inventory::SORT_TYPE InventorySortType_, int iPageNum_ ) { m_NowInventoryPageNum[ InventorySortType_ - 1 ] = iPageNum_; }
#endif	SERV_REFORM_INVENTORY_TEST

#ifdef SERV_TOUR_TICKET_EVENT
	void	ShowTourEventDlg();
#endif SERV_TOUR_TICKET_EVENT

	//{{ 2012. 10. 08  �ҷ��� ������
#ifdef SERV_HALLOWEEN_ICE_HEATER
	void UpdateOpenHalloWeenItemAck();
#endif SERV_HALLOWEEN_ICE_HEATER

protected:
	void UseDefenceDungeonPotion( const UidType uidItem_, const int iItemId_ );
#ifdef FREE_WARP
	void UseWarpItem( CX2SlotItem* pItemSlot_, bool bIsEdConsumption_ );
	void UseWarpPopup(bool bIsEdConsumption_);
#endif //FREE_WARP

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	void UseUSCJobItem( CX2SlotItem* pItemSlot_);
	void UseUSCJobPopup();
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef PET_DROP_ITEM_PICKUP
	void SetPetAutoLooting( UidType uiPetUID_ );
#endif //PET_DROP_ITEM_PICKUP

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //�̶�Ŭť��
	bool IsRestoreEvaluateItem( const int ItemID_ ) const;
	void UseRestoreEvaluteItem( UidType iConsumeItemUID_ );

	bool Handler_EGS_RESTORE_ITEM_EVALUATE_REQ();
	bool Handler_EGS_RESTORE_ITEM_EVALUATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_ITEM_EVALUATE_REQ();
	bool Handler_EGS_ITEM_EVALUATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_ITEM_CONVERT_REQ( UidType iItemUID, int iQuantity );
	bool Handler_EGS_ITEM_CONVERT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05


#ifdef ADDED_ENCHANT_COUPON_HAVE_LEVEL_LIMIT    // ������ / 13-07-03 / ���� ������ ���� ��ȭ�� ����
	bool CheckLevelIfLevelLimitEnchantCoupon ( const int iCouponItemID_ , const int iTargetItemLevel_ );
#endif // ADDED_ENCHANT_ITEM_HAVE_LEVEL_LIMIT // ������ / 13-07-03 / ���� ������ ���� ��ȭ�� ����

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	void OpenElBreakWindow( IN CX2Item* pItem_ );
	void UpdateElBreakDLG( IN const bool bReadIME_ );
	bool Handler_EGS_ITEM_EXTRACT_REQ();
	bool Handler_EGS_ITEM_EXTRACT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //FINALITY_SKILL_SYSTEM

#ifdef GOOD_ELSWORD //JHKang
#ifndef NO_GOOD_ELSWORD_INT
	bool	Handler_EGS_GET_NEXT_INVENTORY_ED_REQ();
	bool	Handler_EGS_GET_NEXT_INVENTORY_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_EXPAND_INVENTORY_ED_REQ();
	bool	Handler_EGS_EXPAND_INVENTORY_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif NO_GOOD_ELSWORD_INT
#endif //GOOD_ELSWORD

#ifdef ADD_SOCKET_SLOT // ���� ���� Ȯ��
	bool IsAddSocketSlotItem( const int ItemID_ ) const;
	void UseAddSocketSlotItem( UidType iConsumeItemUID_ );

	void Handler_EGS_SOCKET_EXPAND_ITEM_REQ();
	bool Handler_EGS_SOCKET_EXPAND_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // ADD_SOCKET_SLOT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	void Handler_EGS_USE_PROPOSE_ITEM_REQ( UidType iItemUID, wstring wstrNickName );
	bool Handler_EGS_USE_PROPOSE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Handler_EGS_USE_DIVORCE_ITEM_REQ();
	bool Handler_EGS_USE_DIVORCE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	bool Handler_EGS_CUBE_IN_ITEM_MAPPING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_RECRUIT_EVENT_BASE
	bool Handler_EGS_USE_RECRUIT_TICKET_REQ( UidType iUid );
	bool Handler_EGS_USE_RECRUIT_TICKET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REGISTER_RECRUITER_REQ( const WCHAR* wszNickname, bool bCancel = false );
	bool Handler_EGS_REGISTER_RECRUITER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OpenRecommendMsgBox( bool bOpen );
#endif SERV_RECRUIT_EVENT_BASE

private:
	CX2Inventory::SORT_TYPE m_NowInventorySortType;

#ifdef	SERV_REFORM_INVENTORY_TEST
	int						m_NowInventoryPageNum[7];
#else	SERV_REFORM_INVENTORY_TEST
	int						m_NowInvenSortTypePageNum;
	int						m_NowInvenSortTypePageMaxNum;
#endif	SERV_REFORM_INVENTORY_TEST

	//CX2Inventory*			m_pInventory;
	D3DXVECTOR2				m_MovedPosition;

	CKTDGUIDialogType			m_pDLGUIInventory;
	CKTDGUIDialogType			m_pDLGResolveItem;
	CKTDGUIDialogType			m_pDLGResolveItemResult;
	CKTDGUIDialogType			m_pDLGSocketItem;
	CKTDGUIDialogType			m_pDLGSocketItemPushOk;
	CKTDGUIDialogType			m_pDLGSocketItemCheat;
	CKTDGUIDialogType			m_pDLGCubeCheck;
	CKTDGUIDialogType			m_pDLGElChanger;
	CKTDGUIDialogType			m_pDLGElChangerResult;

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	CKTDGUIDialogType			m_pDLGElBreak;			/// �� ���ر�
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_PSHOP_AGENCY
	CKTDGUIDialogType			m_pDLGCheckPShop;
#endif

#ifdef SERV_SOCKET_NEW
	CX2UISocketItem*			m_pUISocketItem;
#endif SERV_SOCKET_NEW

	//{{ kimhc // �ǽð� ���ҵ� �� ������ ������ ������
#ifdef REAL_TIME_ELSWORD
	CKTDGUIDialogType			m_pDLGItemEnroll;
	CKTDGUIDialogType			m_pDLGDeleteItemConfirm;
	int							m_iItemEnrollNum;			// ���� �������� ����
	UidType						m_EnrollItemUid;			// ���� �������� UidType
#endif REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� ������ ������ ������

	//{{ kimhc // 2009-08-19 // ������ ���� ����� �� Ȯ�� â
#ifdef	SEAL_ITEM
	CKTDGUIDialogType			m_pDLGSealItemConfirm;
	CX2SlotItem*				m_pSlotItemToBeSealed;				// ���ε� slotItem
	UidType						m_itemUIDForSeal;					// ����� ����� �ߵ����� UID
	UidType						m_itemUIDToBeUnSealed;				// ������ Ǯ������ UID
	//}} kimhc // 2009-08-19 // ������ ���� ����� �� Ȯ�� â
#endif	SEAL_ITEM


	//{{ 2007. 10. 8  ������  ���� ������ ���̾�α� ������
	CKTDGUIDialogType			m_pDLGRandomItem;
	//}}

	// 09.04.01 �¿� : ���������� ���� ���� �ڵ� ����
	// Ȯ��â�� ���� ����(��Ŭ���� �� ����)�� m_eRandomItemEventType�� ���� �����ϰ�,
	// Ȯ��â�� ��ҵǴ� ���� �Ǵ� ���� �� ������ Ȯ�� ���� �� m_eRandomItemEventType�� ���� NONE���� �ٽ� �ٲ��ش�.
	RANDOM_ITEM_OPEN_EVENT_TYPE	m_eRandomItemEventType;

	UidType					m_iRandomItemUID;
	int						m_RandomItemID;

	bool					m_bShow;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	KEGS_OPEN_RANDOM_ITEM_ACK m_TempOpenRandomItemAck;

	// ���� / ȯ�� ����
	UidType					m_ElChangerItemUID;
	int						m_ElChangerItemCount;

	// ���ذ���
	UidType					m_DecompositionItemUID;
	int									m_DecompositionWindowNowPageNum;
	std::map< int, int >                m_mapResolveResultInsertedItem;

	// ���ϰ���
	UidType					m_SocketItemUID;
	int						m_SocketSlotIndex;
	bool					m_bSocketRareMagicStone;
	bool					m_bSocketRemove;


#ifdef ATTRACTION_ITEM_TEST
	float					m_fPlayTime;

	//bool					m_bOpenedAttractionItem;
	bool					m_bPlayAttractionItem;
	CKTDGUIDialogType									m_pDLGOpenAttraction;
	CKTDGUIDialogType									m_pDLGAttractionResult;
	CKTDGUIDialogType									m_pDLGGetAttractionItemBG;	

	CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstMeltMachine;
	CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstMeltMachineStart;

	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpreadLight;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight2;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight3;

	// ��ȭ�ǰ���
	UidType					m_CouponUid;
	UidType					m_CouponTargetUid;
#endif

#ifdef APRIL_5TH_TREE_DAY_EVENT			// �ĸ��� �̺�Ʈ ���� �ڵ�
	CKTDGUIDialogType			m_pDLGTreeDay1;
	CKTDGUIDialogType			m_pDLGTreeDay2;
	bool					m_bPlayingTreedayEvent;
	bool					m_bAprilSoundPlayed;

#endif

	//{{100416 ������ Ʈ���� �ڽ� �߰�
#ifdef 	SERV_TREASURE_BOX_ITEM
	CKTDGUIDialogType			m_pDLGTreasureBox1;
	CKTDGUIDialogType			m_pDLGTreasureBox2;
	CKTDGUIDialogType			m_pDLGTreasureBox3;
	CKTDGUIDialogType			m_pDLGTreasureBox4;
	CKTDGUIDialogType			m_pDLGTreasureBox5;
	CKTDGUIDialogType			m_pDLGTreasureBox6;
	CKTDGUIDialogType			m_pDLGTreasureBox7;
	CKTDGUIDialogType			m_pDLGTreasureBox8;
	CKTDGUIDialogType			m_pDLGTreasureBox9;
	CKTDGUIDialogType			m_pDLGTreasureBox_White;

	bool						m_bPlayingTreasureBox;
	bool						m_bTreasureBoxSoundPlayed;

#ifdef TREASURE_BOX_ITEM_THIN
	int							m_TreasureBoxSPItemID;
#endif TREASURE_BOX_ITEM_THIN
#endif 	SERV_TREASURE_BOX_ITEM
	//}}100416 ������ Ʈ���� �ڽ� �߰�

#ifdef ITEM_RECOVERY_TEST
	UidType					m_RecoveryItemUid;
#endif

#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
	bool					m_bUsingRandomItemAutoOpen;
	int						m_OpenedRandomItemCount;
	CKTDGUIDialogType			m_pDLGAutoOpenMsgBox;
	std::map< int, int >	m_MapAutoRandomItemGet;
#endif

#ifdef RANDOM_CUBE_UI_NEW
	CKTDGUIDialogType		m_pDLGCubeOpenEffect;
	float					m_fCubeOpenTime;
	bool					m_bCubeJustOpen;
	bool					m_bCubeOpen;
	std::map< int, int >	m_mapCubeItemGet;
#endif RANDOM_CUBE_UI_NEW
	UidType					m_WarpItem;

#ifdef BUFF_TEMPLET_SYSTEM
	UidType					m_DefencedungeonPotion;
	int						m_DefencedungeonPotionID;
#endif BUFF_TEMPLET_SYSTEM

	// ���ο� �������� �ִ����� ���� ���θ� ����
	bool					m_bIsNewItem;

	UidType					m_UseItemTempUID;
	UidType					m_UsedItemUID;
	UidType					m_TitleItemUID;

#ifdef FREE_WARP
	CKTDGUIDialogType			m_pDLGWarpDestination;
	std::vector<CKTDGUIButton*> m_vecWarpListButton;
	int							m_iSelectedWarpDest;
	SHORT						m_SumDelta;

	int							m_iSelectedWarpDestIndex;//���� ��ġ�� �Ÿ��� �˱� ���� �ε��� ����
	int							m_iCurrentVillageWarpIndex; //���� ��ġ �ε����� ����
	bool						m_bIsEdConsumptionWarp;		//���������� ED�Ҹ𿩺�
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	CKTDGUIDialogType			m_pDLGUSCJob;
	std::vector<CKTDGUIButton*> m_vecUSCJobButton;
	int							m_iSelectedUSCJob;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	UidType						m_UseTransformdItemUID;
	UidType						m_UseTransformItemPetUID;
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

	CKTDGUIDialogType		m_pDLGIdentifyStone;

#ifdef SERV_PET_SYSTEM
	CX2UIPetInventory*		m_pUIPetInventory;
	bool					m_bAutoFeed;

	CX2SlotItem*			m_pPetFoodSlot;
#endif

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	ITEM_OBTAIN_RESULT_TYPE	m_eItemObtainResultType; 
#endif RANDOM_EXCHANGE_RESULT_VIEW

#ifdef NEW_ITEM_NOTICE
	CKTDGUIDialogType		m_pDLGItemHighLight;
#endif //NEW_ITEM_NOTICE
#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
	bool					m_bProcessCubeOpen;			//ť�� ���� ���� ó���� ����
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG

#ifdef	SERV_SHARING_BANK_TEST
public:
	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ	m_kTempChangeSlotInfo;
#endif	SERV_SHARING_BANK_TEST

#ifdef CUBE_OPEN_IMAGE_MANAGER
	bool					m_bHideCubeOpneBar;			// ť�� ���� ���忡�� ���� �� ǥ�ÿ���
	CKTDGUIDialogType		m_pDLGEventCubeResultImage;					// ��� �̹��� UI
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef SPT_NONE_ITEM_DELETE
	bool					m_bIsSptNoneItem;
	CKTDGUIDialogType		m_pSptNoneItemDelete;
#endif //SPT_NONE_ITEM_DELETE

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
	int						m_iUseOpenRandomItemID;		// ���콺 ���� Ŭ������ ������ ������ ť�� ID �ӽ� ����
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	bool					m_bIsAllInOne;
#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
#ifdef RIDING_SYSTEM
	bool					m_bIsRidingGacha;
#endif //RIDING_SYSTEM

#ifdef SERV_HALLOWEEN_ICE_HEATER
	bool					m_bIsHalloweenIceHeater;
#endif //SERV_HALLOWEEN_ICE_HEATER

#ifdef FIX_ICE_HEATER_EVENT

	map<RANDOM_ITEM_OPEN_EVENT_TYPE, IceHeaterEventData*>	m_mapIceHeaterEventData;		/// ������ ��� ������ ���� ��
	IceHeaterEventData*										m_pNowIceHeaterEventData;		/// ���� ������� ������ ��� ������
	CX2EffectSet::Handle									m_hUIEffectSetWait;		/// ���� �����⿡�� ������� ��� UIEffectSet �ڵ�
	CX2EffectSet::Handle									m_hUIEffectSetPlay;		/// ���� �����⿡�� ������� ��� UIEffectSet �ڵ�
#endif // FIX_ICE_HEATER_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //�̶�Ŭť��
	UidType					m_iConsumeItemUID;
	UidType					m_iTargetItemUID;
	int						m_iConsumeItemQuantity;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef RIDING_ITEM_POPUP_CHECK
	UidType					m_iRidingItemUid;
#endif //RIDING_ITEM_POPUP_CHECK

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	UidType		m_ElBreakItemUID;
	int			m_ElBreakItemCount;
#endif //FINALITY_SKILL_SYSTEM

#ifdef GOOD_ELSWORD //JHKang
	int					m_iConfirmED;
	CKTDGUIDialogType	m_pDLGBuyConfirm;
#endif //GOOD_ELSWORD

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	float		m_fLeftMouseWhellCoolTime;
#endif // CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef SERV_TOUR_TICKET_EVENT
	CKTDGUIDialogType		m_pDLGTourEventDlg;
	bool					m_bPlayTourEvent;
	float					m_fPlayTourEventDlgTime;
#endif SERV_TOUR_TICKET_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	CKTDGUIDialogType		m_pDLGMarryEventNickName;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
	CKTDGUIDialogType		m_pDLGRecommend;
#endif SERV_RECRUIT_EVENT_BASE
};

#endif
