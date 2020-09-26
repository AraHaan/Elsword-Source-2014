#pragma once

class CX2TFieldNpcShop
{   
public:
	enum NPC_MESSAGE_UI_MSG
	{
		NMUM_CLOSE				= 15000,
		NMUM_QUEST,
		NMUM_SHOP,
		NMUM_PVP,
		NMUM_MAKE,
		NMUM_FREETRAIN,
		NMUM_TRAINING,
		NMUM_PASS,
		NMUM_NEXT,
		NMUM_AGREE,
		NMUM_COMPLETE,
		NMUM_ENCHANT,
		NMUM_ATTRIBUTE,
		NMUM_RANKING,
		NMUM_EXCHANGE,			// ��ϸ� �ð��� ���� ������ ��ȯ	// kimhc // 2009-07-25
		
		//{{ kimhc // 2009-08-06 // ĳ���ͺ� ����
//#ifdef PRIVATE_BANK
		NMUN_PRIVATE_BANK,		// ���� ����							// kimhc // 2009-08-03
//#endif PRIVATE_BANK
		//}} kimhc // 2009-08-06 // ĳ���ͺ� ����
		NMUM_GUILD,
		NMUM_WEB_EVENT,		// oasis907 : ����� [2010.2.24] // 
		NMUM_PERSONAL_SHOP,
//#ifdef SERV_PSHOP_AGENCY
		NMUM_AGENCY_TRADER_REGISTER,	// �븮���� - ��ǰ���
		NMUM_AGENCY_TRADER_RECEIVE,		// �븮���� - ��ǰ�ޱ�
//#endif
//#ifdef SERV_PVP_NEW_SYSTEM
		NMUM_FORMAL_PVP,
//#endif
//#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT 
		NMUM_WEDDING_EVENT			= 15022,	// ��ȥ �ý��� ��Ī �̺�Ʈ �Խ��� ��ư Ŭ��
		// <cocy33 ��â��> ���� �ڵ尡 UIManager���� �ش� UI�� Ŭ������ ������ �ְ� �޼����� �ش� UI�� UICustom���� �ް� �Ǿ��ִµ� �̺�Ʈ�� ���ؼ� Ŭ������ ����� ���� ��ȿ�����̶� ����.
		NMUM_WEDDING_EVENT_SEND_LETTER,			// ��ȥ �ý��� ��Ī �̺�Ʈ ���������� ���� �߼� ��ư Ŭ��
		NMUM_WEDDING_EVENT_CLOSE,				// ��ȥ �ý��� ��Ī �̺�Ʈ ������ �ݱ�
//#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		NMUM_NEW_ITEM_EXCHANGE,

//#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		NMUM_WEDDING_EVENT_FOCUS_LETTER,		// ��ȥ �ý��� ��Ī �̺�Ʈ ������ ��Ŀ�� ó��
//#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

//{{ 2011.05.04   �ӱԼ� �ƹ�Ÿ �ռ� �ý���
//#ifdef SERV_SYNTHESIS_AVATAR
		NMUM_SYNTHESIS,
//#endif SERV_SYNTHESIS_AVATAR
//}}
//#ifdef NPC_EVENT_BUTTON
		NMUM_EVENT1,
		NMUM_EVENT2,
		NMUM_EVENT3,
		NMUM_EVENT4,
//#endif NPC_EVENT_BUTTON
	};

	enum NPC_SHOP_BUTTON_TYPE
	{
		NSBT_NONE = 0,
		NSBT_QUEST,
		NSBT_SHOP,
		NSBT_PVP,
		NSBT_MAKE,
		NSBT_FREETRAIN,
		NSBT_TRAINING,
		NSBT_CLOSE,
		NSBT_PASS,
		NSBT_NEXT,
		NSBT_AGREE,
		NSBT_COMPLETE,
		NSBT_ENCHANT,
		NSBT_ATTRIBUTE,
		NSBT_RANKING,
		NSBT_EXCHANGE,			// ��ϸ� �ð��� ���� ������ ��ȯ	// kimhc // 2009-07-25
		//{{ kimhc // 2009-08-06 // ĳ���ͺ� ����
//#ifdef PRIVATE_BANK
		NSBT_PRIVATE_BANK,		// ��������								// kimhc // 2009-08-03
//#endif PRIVATE_BANK
		//}} kimhc // 2009-08-06 // ĳ���ͺ� ����

//#ifdef GUILD_BOARD
		NSBT_GUILD,				// oasis907 : ����� [2009.11.24] //
//#endif GUILD_BOARD
//#ifdef WEB_POINT_EVENT
		NSBT_WEB_EVENT,
//#endif WEB_POINT_EVENT
//#ifdef DEF_TRADE_BOARD
		NSBT_PERSONAL_SHOP,
//#endif DEF_TRADE_BOARD
//#ifdef SERV_PSHOP_AGENCY
		NSBT_AGENCY_TRADER_REGISTER,	// �븮���� - ��ǰ���
		NSBT_AGENCY_TRADER_RECEIVE,		// �븮���� - ��ǰ�ޱ�
//#endif
		//{{ 2011.05.04   �ӱԼ� �ƹ�Ÿ �ռ� �ý���
//#ifdef SERV_SYNTHESIS_AVATAR
		NSBT_SYNTHESIS,
//#endif SERV_SYNTHESIS_AVATAR
		//}}
//#ifdef NPC_EVENT_BUTTON
		NSBT_EVENT1,
		NSBT_EVENT2,
		NSBT_EVENT3,
		NSBT_EVENT4,
//#endif NPC_EVENT_BUTTON
//#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		NSBT_WEDDING_EVENT,				// ��ȥ �ý��� �̺�Ʈ ��ư
//#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		NSBT_EXCHANGE_NEW_ITEM,
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	};

	struct NPCSHOPTYPE
	{
		bool			m_bShowNpcShop;
		NPC_SHOP_BUTTON_TYPE	m_eShopType;
	};	

    CX2TFieldNpcShop();
    virtual ~CX2TFieldNpcShop();

    void RegisterLuaBind();
	void SetStage(CKTDXStage *pStage);

	void SetNpcId(int npcId, wstring houseName);
	void SetShopType(NPC_SHOP_BUTTON_TYPE eType, bool bShow = true);
	bool GetShopType(NPC_SHOP_BUTTON_TYPE eType);
	void SetShow(bool val);
    void SetShow(NPC_SHOP_BUTTON_TYPE eType, bool val);
	bool GetShow() { return m_bShowNpcShop; }
	bool GetShow(NPC_SHOP_BUTTON_TYPE eType);
	
	void SetHouseName(wstring houseName);	
	void SetNpcMessage(const wstring &npcMsg);
#ifdef SERV_PSHOP_AGENCY
	void SetNpcMessageWithEtc(const wstring &npcMsg, const wstring &npcMsgEtc);
#endif

	void CreateHouseNPCName(CX2LocationManager::HouseTemplet* pHouse);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	int	 UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void LeaveField();
	void SetQuestDesc(const wstring &title, const wstring &message);

	void SetPlayGuide(int val) { m_iIsPlayGuide = val; }
	void SetKeyEvent();

#ifdef DEF_TRADE_BOARD
	void SetEnablePersonalShopBoardButton(bool bEnable);
#endif DEF_TRADE_BOARD

	
private:
	void UpdateQuestDesc( const wstring& wstrDesc );
	void UpdateQuestDescPage( int iPage );
	

protected:    

    //{{ seojt // 2009-8-18, 18:03, qff
    CKTDGUIDialogType       m_pDlgMessage;
    //}} seojt // 2009-8-18, 18:03
#ifdef SERV_READY_TO_SOSUN_EVENT
	CKTDGUIDialogType		m_pDlgReadyToSosun;
#endif SERV_READY_TO_SOSUN_EVENT

	std::vector<NPCSHOPTYPE> m_vecShopType;

private:
	int						m_iNpcID;
	bool					m_bShowNpcShop;

#ifdef DEF_TRADE_BOARD
	bool					m_bEnableButtonPersonalShop;
#endif DEF_TRADE_BOARD

	CKTDGUIButton*			m_pButtonQuest;
	CKTDGUIButton*			m_pButtonPvp;
	CKTDGUIButton*			m_pButtonShop;
	CKTDGUIButton*			m_pButtonMake;
	CKTDGUIButton*			m_pButtonTraining;
	CKTDGUIButton*			m_pButtonFreeTraining;
	CKTDGUIButton*			m_pButtonGuild; // oasis907 : ����� [2009.11.24] // ��� �Խ���
	CKTDGUIButton*			m_pButtonRanking;

	//{{ kimhc // 2009-07-25 // ��ϸ��� �ð��� �߰��Ǵ� ITEM_EXCHANGE_SHOP �۾�
#ifdef	ITEM_EXCHANGE_SHOP
	CKTDGUIButton*			m_pButtonExchange;
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // ��ϸ��� �ð��� �߰��Ǵ� ITEM_EXCHANGE_SHOP �۾�
	
	//{{ kimhc // 2009-08-03 // ĳ���ͺ� ����
#ifdef PRIVATE_BANK
	CKTDGUIButton*			m_pButtonPrivateBank;
#endif PRIVATE_BANK
	//}} kimhc // 2009-08-03 // ĳ���ͺ� ����

#ifdef WEB_POINT_EVENT
	CKTDGUIButton*			m_pButtonWebEvent;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
	CKTDGUIButton*			m_pButtonPersonalShop;
#endif DEF_TRADE_BOARD
	//{{ 2011.05.04   �ӱԼ� �ƹ�Ÿ �ռ� �ý���
#ifdef SERV_SYNTHESIS_AVATAR
	CKTDGUIButton*			m_pButtonSynthesis;
#endif SERV_SYNTHESIS_AVATAR
	//}}
	CKTDGUIButton*			m_pButtonBack;
	CKTDGUIButton*			m_pButtonOut;
	CKTDGUIButton*			m_pButtonPass;
	CKTDGUIButton*			m_pButtonWindowPass;

	CKTDGUIButton*			m_pButtonNext;
	CKTDGUIButton*			m_pButtonAgree;
	CKTDGUIButton*			m_pButtonComplete;

	CKTDGUIButton*			m_pButtonEnchant;
	CKTDGUIButton*			m_pButtonAttribute;

#ifdef SERV_PSHOP_AGENCY
	CKTDGUIButton*			m_pButtonAgencyTraderRegister;
	CKTDGUIButton*			m_pButtonAgencyTraderReceive;
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	CKTDGUIButton*			m_pButtonFormalPvp;
#endif

#ifdef NPC_EVENT_BUTTON
	CKTDGUIButton*			m_pButtonEvent1;
	CKTDGUIButton*			m_pButtonEvent2;
	CKTDGUIButton*			m_pButtonEvent3;
	CKTDGUIButton*			m_pButtonEvent4;
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	CKTDGUIButton*			m_pButtonWeddingEvent;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	CKTDGUIButton*			m_pButtoenExchangeNewItem;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05


	CKTDGUIStatic*			pStaticShopName1;
	CKTDGUIStatic*			pStaticShopName2;
	CKTDGUIStatic*			pStaticShopmessage;

	//CX2TFieldShop*			m_pShop;
	NPC_SHOP_BUTTON_TYPE	m_eNowState;

	int						m_nEnabledButton;

    //{{ seojt // 2009-8-18, 18:03
    CKTDGUIDialogType			m_pDLGNpcName;
    //}} seojt // 2009-8-18, 18:03
	int						m_iHouseID;

	D3DXVECTOR2			m_vQuestDescSize;				// ����Ʈ ���� ���ڰ� ���� â�� ũ��, ���� �κи�
	wstring				m_wstrQuestTitle;				// ���õ� ����Ʈ�� ����
	vector<wstring>		m_vecQuestDesc;					// ���õ� ����Ʈ�� ����
	int					m_iNowQuestDescPage;
	int					m_iMaxQuestDescPage;

	// �ʽ��� ���̵� ���࿩�θ� �����Ѵ�.(���̵� �ܰ谪)
	int				m_iIsPlayGuide;

	vector<wstring>		m_vButtonNameList;

#ifdef LOCAL_RANKING_UPDATE
	bool				m_bShowLocalRankingInfo;
#endif //LOCAL_RANKING_UPDATE

};
