#pragma once

class CX2StateCreateUnit : public CX2State
{
	public:
		friend class CX2StateAutoChanger;

		enum STATE_CREATE_UNIT_UI_CUSTOM_MSG
		{
			SCUUCM_ELSWORD_BUTTON_OVER = 0,
			SCUUCM_ELSWORD_BUTTON_UP,

			SCUUCM_LIRE_BUTTON_OVER,
			SCUUCM_LIRE_BUTTON_UP,

			SCUUCM_ARME_BUTTON_OVER,
			SCUUCM_ARME_BUTTON_UP,

			SCUUCM_RAVEN_BUTTON_OVER,
			SCUUCM_RAVEN_BUTTON_UP,

			SCUUCM_CREATE_UNIT,
			SCUUCM_BACK_TO_UNIT_SELECT,

			SCUUCM_UNIT_ANIMATION_DOWN,
			SCUUCM_UNIT_ANIMATION_UP,
			SCUUCM_UNIT_ANIMATION_OVER,

			SCUUCM_ENTER_TUTORIAL_OK,
			SCUUCM_ENTER_TUTORIAL_CANCEL,

			SCUUCM_EVE_BUTTON_OVER,
			SCUUCM_EVE_BUTTON_UP,

			SCUUCM_CHUNG_BUTTON_OVER,
			SCUUCM_CHUNG_BUTTON_UP,

			SCUUCM_ARA_BUTTON_OVER, // ��� �� ��
			SCUUCM_ARA_BUTTON_UP,

			SCUUCM_SHUT_DOWN_OK,

			SCUUCM_ELESIS_BUTTON_UP,

// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			// ĳ���� ���� �̸����� ��ư, ù��° ���� 1�� ���� ����
			SCUUCM_FIRST_LINE_FIRST_CLASS_CHANGE_BUTTON_UP,
			// ĳ���� ���� �̸����� ��ư, �ι�° ���� 1�� ���� ����
			SCUUCM_SECOND_LINE_FIRST_CLASS_CHANGE_BUTTON_UP,
			// ĳ���� ���� �̸����� ��ư, ����° ���� 1�� ���� ����
			SCUUCM_THIRD_LINE_FIRST_CLASS_CHANGE_BUTTON_UP,
			// ĳ���� ���� �̸����� ��ư, ù��° ���� 2�� ���� ����
			SCUUCM_FIRST_LINE_SECOND_CLASS_CHANGE_BUTTON_UP,
			// ĳ���� ���� �̸����� ��ư, �ι�° ���� 2�� ���� ����
			SCUUCM_SECOND_LINE_SECOND_CLASS_CHANGE_BUTTON_UP,
			// ĳ���� ���� �̸����� ��ư, ����° ���� 2�� ���� ����
			SCUUCM_THIRD_LINE_SECOND_CLASS_CHANGE_BUTTON_UP,
			
			// ĳ���� ���� ����â �ܰ�� �Ѿ
			SCUUCM_CREATE_UNIT_SERVER_SELECT_STEP_BUTTON_UP,
			
			// ���̾�, �ַ��� ���� ����
			SCUUCM_SELECT_GAIA_SERVER,
			SCUUCM_SELECT_SOLES_SERVER,
			
			// 
			SCUUCM_PASS_NICKNAME_CHECK_OK,
			SCUUCM_PASS_NICKNAME_CHECK_CANCEL,

			// �г��� üũ
			SCUUCM_NICKNAME_CHECK,

			SCUUCM_ADD_BUTTON_UP,
// #endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
		};
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		enum UNIT_STATE_INFOMATION_CHART_LEVEL
		{
			USICL_NONE = 0,
			USICL_LOW,
			USICL_MIDDLE,
			USICL_HIGH,

		};
		struct UNIT_STAT_INFOMATION_CHART
		{
		
			UNIT_STATE_INFOMATION_CHART_LEVEL m_eSpeed;
			UNIT_STATE_INFOMATION_CHART_LEVEL m_eAttackDistance;
			UNIT_STATE_INFOMATION_CHART_LEVEL m_eDifficultLevel;

			UNIT_STAT_INFOMATION_CHART () : m_eSpeed ( USICL_NONE ), 
				m_eAttackDistance ( USICL_NONE ), m_eDifficultLevel ( USICL_NONE ) {}
			
			UNIT_STAT_INFOMATION_CHART ( UNIT_STATE_INFOMATION_CHART_LEVEL eSpeed, UNIT_STATE_INFOMATION_CHART_LEVEL eAttackDistance, 
				UNIT_STATE_INFOMATION_CHART_LEVEL eDifficultLevel ) :
						m_eSpeed ( eSpeed ), m_eAttackDistance ( eAttackDistance ), 
						m_eDifficultLevel ( eDifficultLevel ) {}
		};
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

	public:
		CX2StateCreateUnit(void);
		virtual ~CX2StateCreateUnit(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CREATE_UNIT_REQ();
		bool Handler_EGS_CREATE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



		void UnSelect();
		void SelectElSword();
		void SelectLire();
		void SelectArme();
		void SelectRaven();
		void SelectEve();

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void SelectChung();
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		void SelectAra();
#endif
#ifdef NEW_CHARACTER_EL
		void SelectElesis();
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		void SelectAdd();
#endif //SERV_9TH_NEW_CHARACTER


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		// ���� Ŭ���� ���� �޽���
		void SelectUpperClass ( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eClassLine_  );		
		// UnitUpperClassType �� �޾ƿ� Unit Class Type �� ��ȯ��
		CX2Unit * GetViewerUnitByUnitType ( CX2Unit::UNIT_TYPE eUnitType );		
		// ĳ���� ���� ����â DLG �� �����ְų� ����
		void ShowCreateUnitClassChangeInfoDlg ( bool bIsShow_ );	
		// ���� ���ø��� ���� ���θ�� �������� ������ �޾ƿͼ� ���� ��Ŵ
		void EquipPromotionItemByUnitTemplet ( const CX2Unit::UnitTemplet * pUnitTemplet_ );
		// ĳ���� ���� ����â�� Ŭ���� ������ �����Ѵ�.
		void ChangeCreateUnitTypeChangeInfoDlg ( CX2Unit::UNIT_TYPE eUnitType_ );
		// ������ ����� ���� �����Ѵ�.
		void SetRightSideFlagColor ( D3DXCOLOR colorFlag );
		// ĳ���� ���� ��, �� ���ĵ带 ��ü�Ѵ�. 
		void ChangeCreateUnitStandMeshInstance ( CX2Unit::UNIT_TYPE eUnitType_ );
		// ĳ���� ���� ����â�� ���� Ŭ���� ������ �����Ѵ�.
		void ChangeCreateUnitClassChangeInfoDlg ( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS_TYPE eUpperClassType_, CX2Unit::UNIT_CLASS_LINE eClassLine_ );

		wstring GetUnitClassCreateInfoTextureNameByUnitType ( CX2Unit::UNIT_TYPE eUnitType_ );
		wstring GetUnitClassCreateInfoKeyNameByPictureIndex ( int iIndex_, CX2Unit::UNIT_CLASS_TYPE eUpperClassType_, CX2Unit::UNIT_CLASS_LINE eClassLine_ );

		wstring GetUnitClassChangeInfoKeyNameByPictureIndex ( int iIndex_, CX2Unit::UNIT_CLASS eUnitClass_ );
		UNIT_STAT_INFOMATION_CHART GetUnitStatInfomationChart ( CX2Unit::UNIT_CLASS eUnitClass_ );


		bool Handler_EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ();
		bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh


		bool CreateErrorCheck();

		void InitDummyUnits();
		bool ReadyUnitResources( CX2Unit* pUnit );

		void ReadyPromotionUnitResources();
		void EquipPromotionItem( CX2Unit::UNIT_CLASS eUnitClass );

		
		void StaticControlAnimate();

		void OpenNickNameBox();
		void SetAlarmAboutName( const WCHAR* pContent );
		
		virtual bool LastMsgByESC();
		virtual void LastMsgPopOk();

		//{{ kimhc // 2010.11.30 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		UCHAR GetPageNowUnitButton() const { return m_ucPageNowUnitButton; }
		void SetPageNowUnitButton(UCHAR ucPageNowUnitButton_) { m_ucPageNowUnitButton = ucPageNowUnitButton_; }

		UCHAR GetPageMaxUnitButton() const { return m_ucPageMaxUnitButton; }
		void SetPageMaxUnitButton(UCHAR ucPageMaxUnitButton_) { m_ucPageMaxUnitButton = ucPageMaxUnitButton_; }
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.30 //  2010-12-23 New Character CHUNG

		void InitializeUnitButton();
		void SetUnitButton( int iPageNow_ );

	protected:
		CKTDGUIDialogType			m_pDLGCreateUnitBack;
		CKTDGUIDialogType			m_pDLGCreateUnitFront;

		CX2Unit::UNIT_CLASS		m_SelectUnitClass;
		wstring					m_NickName;
		UidType					m_UnitUID;

		CX2Unit*				m_pDummyElsword;
		CX2Unit*				m_pDummyArme;
		CX2Unit*				m_pDummyLire;
		CX2Unit*				m_pDummyRaven;
		CX2Unit*				m_pDummyEve;

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		CX2Unit*				m_pDummyChung;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		CX2Unit*				m_pDummyAra;		/// "�ƶ�" ����
#endif

#ifdef NEW_CHARACTER_EL
		CX2Unit*				m_pDummyEL;
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		CX2Unit*				m_pDummyAdd;
#endif //SERV_9TH_NEW_CHARACTER


		CX2UnitViewerUI*		m_pUnitViewerUI;

		bool					m_bSelectUnit;		//2dĳ���� Ŭ���Ѱ�.. 

		float					m_fMouseSensitivity;

		float					m_fRemainRotY;
		bool					m_bRotateReturn;
		float					m_fRotSpeed;

		CKTDGUIDialogType			m_pDLGMSGOkBoxEnterTutorial;

		//{{ kimhc // 2010.11.16 // ��ĳ���� û
#ifdef	NEW_CHARACTER_CHUNG

		UCHAR					m_ucPageNowUnitButton;		/// ĳ���� ���� ��ư�� ���� ������
		UCHAR					m_ucPageMaxUnitButton;		/// ĳ���� ���� ��ư�� �ִ� ������
		
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.16 // ��ĳ���� û
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		// ������ ĳ������ ���� Ÿ��, ���� �� ���� ���� â�� ����
		CX2Unit::UNIT_TYPE		m_SelectUnitType;
		
		// ĳ���� ���� ����â
		CKTDGUIDialogType		m_pDLGCreateUnitChangeClassInfo;	

		// ĳ���� ���� ����â DLG
		CKTDGUIDialogType		m_pDLGCreateUnitServerSelect;	

		// ĳ���� ���� â ���� Ȯ�� DLG
		CKTDGUIDialogType		m_pDLGCreateUnitCheckInfo;	

		// ĳ���� �� ���ĵ�
		std::vector<CKTDGXMeshPlayer::CXMeshInstanceHandle>		m_vecStandMeshInst;

		// ĳ���� �� ���ĵ� ����Ʈ
		CX2EffectSet::Handle						m_hStandEffect;

		// ĳ���� ���� ��, ��� ���� �ִ� �������� ���۵� ����
		KUnitInfo				m_kUnitInfo;

		// ������ ���� ��, ĳ���� ���� �� ��Ŷ�� �߰���
		SEnum::SERVER_GROUP_ID	m_eSelectServerGroupID;

		int						m_iGaiaServerCreateAbleUnitMany;
		int						m_iSolesServerCreateAbleUnitMany;
		float					m_fFadeTitleControlValue;

		// ���� ������ ���Ϸ� �ߺ� ����ϴ� ���� �����ϱ� ���� ������ ���� �̸�
		wstring					m_wstrIntroMovieName;

		SHORT		m_sSumDelta;	/// ���콺 �ٿ� ��
		SHORT		m_sZoomState;	/// �� ���� ( 1 ~ 3 / 3�ܰ� )
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef ADD_PLAY_SOUND //��â��
		// ���� �������� ĳ���� Ŭ���ÿ� ������ ����.
		CKTDXDeviceSound*		m_pCreateUnitSelectSound;
#endif //ADD_PLAY_SOUND

private:
#ifdef ADD_PLAY_SOUND //��â��
	void PlaySoundCreateUnit( CX2Unit::UNIT_TYPE eUnitType );
#endif //ADD_PLAY_SOUND
};
