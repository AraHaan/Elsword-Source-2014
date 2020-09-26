#pragma once


#include "./X2BaseUnitViewerUI.h"

#ifdef MONSTER_TROPHY
	class CX2UnitViewerUI : public CX2BaseUnitViewerUI
#else 
	class CX2UnitViewerUI : public CKTDGObject
#endif MONSTER_TROPHY
{
	public:

		enum UNIT_VIEWER_UI_MOTION_TYPE
		{
			UVUMT_WAIT = 0,
			UVUMT_WAIT1,
			UVVMT_WAIT2,
			UVVMT_DOWN_LANDING,
			UVUMT_READY,
			UVUMT_LEVEL_UP,
//#ifdef SERV_UNIT_WAIT_DELETE
			UVUMT_WAIT_DELETE,
//#endif //SERV_UNIT_WAIT_DELETE
		};

		enum UNIT_VIEWER_STATE
		{
			UVS_NONE		= 0,
			UVS_CASHSHOP,
			UVS_ROOM,
			UVS_FIELD,
			UVS_CLASSCHANGE,
			UVS_CREATEUNIT,
			UVS_SELECTUNIT,
			UVS_CHARINFO,
			UVS_WATCH,
		};

	private:
		CX2UnitViewerUI(void);
		virtual ~CX2UnitViewerUI(void);

	public: 
		static CX2UnitViewerUI* CreateUnitViewerUI() { return new CX2UnitViewerUI; }

		virtual void	NotifyShowObjectChanged();
		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-28
        virtual RENDER_HINT   OnFrameRender_Prepare();
		//virtual HRESULT OnFrameRender();
        virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28

//{{ kimhc // 2010.9.9 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
#ifdef	AVATAR_IMAGE_TEST
		void Render_DrawForAvartaScreenCapture();
		RENDER_HINT OnRender_PrepareForAvartaScreenCapture();
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.9.9 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
		
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType = CKTDGXSkinAnim::XAP_LOOP, bool bTransition = false );

		void SetUnit( CX2Unit* pUnit, CX2UnitViewerUI::UNIT_VIEWER_STATE eState = UVS_NONE );

		//{{ robobeg : 2011-03-18
		static void         AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pUnit, CX2UnitViewerUI::UNIT_VIEWER_STATE eState = UVS_NONE );
		//}} robobeg : 2011-03-18

		CX2Unit* GetUnit() { return m_pUnit; }

		void SetPickUnit(bool val);
		// �ɸ����� �κ��丮 �ȿ��� ó��
		void UpdateEqip( bool bEconomical = false );
		void RemoveEqip( UidType itemUID, bool bInNoReplaceEmptyToBasic = false );
		void AddEqip( UidType itemUID );

		// ��� ������ ���ø��� ���� ó��
//{{ robobeg : 2008-10-17
		//void AddEquipByTID( int itemTID );
		//void RemoveEquipByTID( int itemTID );

        void AddEquipByTID( int itemTID, bool bBasicItem = false );
//}} robobeg : 2008-10-17
		void RemoveAllEquips();

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		void InitEquipVisibility();
		void UpdateEquipSlotVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS

		CKTDGXRenderer::RenderParam* GetRenderParam();
		CKTDGXSkinAnim* GetXSkinAnim(){ return m_pXSkinAnim.get(); }
		CKTDGXSkinAnimPtr GetXSkinAnimPtr() { return m_pXSkinAnim; }
		void SetLightPos( float x, float y, float z )
		{ 
			m_LightPos.x = x;
			m_LightPos.y = y;
			m_LightPos.z = z;
		}
		void SetLightPos( D3DXVECTOR3 lightPos )
		{ 
			m_LightPos = lightPos;
		}
		D3DXVECTOR3 GetLightPos() { return m_LightPos; }

		void SetBillBoardType( CKTDGMatrix::BILLBOARD_TYPE billBoardType ){ m_BillBoardType = billBoardType; }

		void ReplaceEmptyToBasic();
		
		static void SetShowUnitViewerUI( bool bShow ) { m_sbShow = bShow; }

		void PlayByMotionType( CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE motionType );

		static bool GetUnitClicked() { return m_sbUnitClicked; }
		static void SetUnitClicked( bool bCheck ) { m_sbUnitClicked = bCheck; }

		int GetEnchantExtraDamageType();

//#ifdef TITLE_SYSTEM
        void UpdateTitle();
//#endif
	
		void SetShowAttribEnchant( bool bShow );

//{{ robobeg : 2008-10-17



// X2UnitVIewerUI �� KTDGManager �� ���, ������ �� ���� ��ǰ�鵵 ���� ���, �����ϱ� ���� ����
        virtual void    NotifyDGManagerChanged( CKTDGManager& refInManager_ );
//}} robobeg : 2008-10-17

#ifdef NEW_VILLAGE_UI
		void SetPositionOnScr(float x, float y, float z = 0.8f, float scale = 1.f );
		void SetFixed(bool val);
		bool GetFixed() { return m_bFixed; }
#endif


		bool    IsFashionItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos )
		{
			ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
			return m_ViewEquipFashion[ eqipPos ] != 0;
		}

#ifdef UNIT_EMOTION
		void EmotionFrameMove();
		bool PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionId, wstring &wstrEmotionName);
		bool GetPlayerEmotion();
		CX2Unit::EMOTION_TYPE GetNowEmotionId() { return m_ePlayedEmotion; }
		void SetNowEmotionId(CX2Unit::EMOTION_TYPE eId) { m_ePlayedEmotion = eId; }
#endif

		float GetScale() { return m_fScale; }

		void SetHideSelectUnitWeapon();		/// �ɸ��� ���� ȭ���� ������ ���� ����� �Լ�
		void SetShowObjectMeshPlayer(CKTDGXMeshPlayer::CXMeshInstanceHandle MeshInstHandle_, bool bShow_);
#ifdef SERV_RENA_NIGHT_WATCHER
		void SetClassChange( IN bool bValue_ ) { m_bChangeClass = bValue_; }
#endif SERV_RENA_NIGHT_WATCHER

#ifdef ADD_UPGRADE_WEAPON_PARTICLE	/// ��ȭ ���⿡ ���� ����Ʈ ��� ó��
		void InitUpgradeWeaponParticle();	/// ��ȭ ���� ����Ʈ �ʱ�ȭ
		void CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );	/// ��ȭ ���� ����Ʈ ����
		void ProcessUpgradeWeaponParticle();/// ��ȭ ���� ����Ʈ ����
		void DeleteUpgradeWeaponParticle();	/// ��ȭ ���� ����Ʈ ����
#endif ADD_UPGRADE_WEAPON_PARTICLE

#ifdef TAKE_OFF_ALL_ITEM
		void SetShowInvenEquip( bool bVal_ ) { m_bShowInvenEquip = bVal_; }
#endif // TAKE_OFF_ALL_ITEM

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
		bool IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId );
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
	private:
		
		void ReplaceEmptyToBasic( CX2Unit::EQIP_POSITION eqipPos, int itemID );
//{{ robobeg : 2008-10-17
		//void CheckEmptyEqipPosition();
//}} robobeg : 2008-10-17



		void CreateEnchantedWeaponParticleAtWeapon( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL );
		void CreateEnchantedWeaponParticleAtHand( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );
		void ProcessEnchantedWeaponEffectAtHand( CX2Unit::UNIT_TYPE unitType );
		void InitEnchantedWeaponEffectAtHand();


		

		void InitEnchantWeaponEffectForSword();
		void InitEnchantWeaponEffectForBow();

		void DeleteEnchantWeaponEffect();

		void ProcessEnchantWeaponEffectForSword();
		void ProcessEnchantWeaponEffectForBow();
		void ProcessEnchantWeaponEffectForEve();
#ifdef ARA_CHARACTER_BASE
		void ProcessEnchantWeaponEffectForAra();
#endif
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		void ProcessEnchantWeaponEffectForAdd();
#endif //SERV_9TH_NEW_CHARACTER

		void InitWeaponTrace();

//{{ robobeg : 2008-10-17
// ������ EQUIP_POSITION �� ��ǰ ���� ���θ� �˷��ִ� �Լ�
        bool    IsItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos )
        {
            ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
            return m_ViewEquipNormal[ eqipPos ] != 0 || m_ViewEquipFashion[ eqipPos ] != 0;
        }//IsItemEquippedAt()
//}} robobeg : 2008-10-17

#ifdef ELSWORD_SECOND_CLASS_CHANGE
		void SetUnitReForm();
#endif

		//{{ kimhc // 2010-01-05 // PC�� �����̾� ����
#ifdef	PC_BANG_WORK
		void RemovePCBangEquips();
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-05 // PC�� �����̾� ����

//{{ kimhc // 2010.12.21 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void GetWeaponTraceStartAndEndName( OUT wstring* pwstrTraceStartName_, OUT wstring* pwstrTraceEndName_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.21 //  2010-12-23 New Character CHUNG

#ifdef RIDING_SYSTEM
		bool CanNotShowEnchantedWeaponEffectToRiding();
#endif //RIDING_SYSTEM

		CX2Unit*				m_pUnit;
		CKTDXDeviceXSkinMesh*	m_pMotion;
        /// ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
        /// - jintaeks on 2009-01-12, 16:12		
		// dmlee, 2009-03-12 ��׶��� �ε��ÿ� dangling pointer ���� ���� ����� ���� ������ smart poiner�� �ٲ���ϴ�.
		CKTDGXSkinAnimPtr		m_pXSkinAnim;
		D3DXVECTOR3				m_LightPos;

//{{ robobeg : 2008-10-17
		//vector<CX2Eqip*>		m_ViewEqipList;
		//bool					m_ViewEqipPos[CX2Unit::EP_END];
		//bool					m_ViewEquipedFashion[CX2Unit::EP_END];

        typedef std::map<UidType,CX2EqipPtr>    ViewEquipMap;

        ViewEquipMap            m_ViewEquipMap;								// ED��, �м��� ��� �������
        UidType                 m_ViewEquipNormal[CX2Unit::EP_END];
        UidType                 m_ViewEquipFashion[CX2Unit::EP_END];
#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		bool					m_bViewEquipVisible[ CX2Unit::EP_END ];
#endif	NOT_RENDERING_OTHER_EQUIP_POS
//}} robobeg : 2008-10-17

		CKTDGMatrix::BILLBOARD_TYPE m_BillBoardType;

		static bool										m_sbShow;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFire;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Fire;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Fire2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Fire3;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqWeapon_Wind;		// �Ӽ� ��æƮ�� ���⸦ ����� �� �տ� �ٴ� ����Ʈ 
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeapon_Wind2;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeapon_Wind3;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWeapon_Wind4;

#ifdef NEW_CHARACTER_EL
		// ���̷� ����Ʈ ��
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSPK1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSPK2;
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEnergyBullet1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEnergyBullet2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqEnergyBullet3;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		static bool										m_sbUnitClicked;

		int												m_ExtraDamageType;
		
		// �Ӽ� ��æƮ ����Ʈ�� ǥ���ϱ� ���ؼ� ���. trace���� ��� ����
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_TRACE_START[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_TRACE_END[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

#ifdef ARA_CHARACTER_BASE
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_WEAPON_BONE[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#endif

//#ifdef TITLE_SYSTEM
        CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEmblem;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticleHandle				m_hPart_Emblem_200;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle*						m_pPart_Emblem_200;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        int                                             m_iTitleId;
//#endif
#ifdef NEW_VILLAGE_UI
		D3DXVECTOR3 m_vScrPos;
		bool		m_bFixed;
#endif

		bool		m_bPickUnit;

		CKTDXDeviceSound*		m_pSound_Eve_LevelUp;

#ifdef UNIT_EMOTION
		CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE		m_eOldMotionType;
		CX2Unit::EMOTION_TYPE							m_ePlayedEmotion;
#endif


#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
		bool m_bEnableEquipSlotVisibilityUpdate;
#endif	NOT_RENDERING_OTHER_EQUIP_POS

		bool m_bMixedEmotion[AVATAR_EMOTION_NUM];
		CKTDXDeviceSound*				m_pAvatarEmotionSound;

		CKTDXTimer m_TimerResetWeaponTraceFrame;
		float		m_fScale;

#ifdef CHUNG_SECOND_CLASS_CHANGE
		bool		m_bShowGun;
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hChungMiniGunL;	/// û �̴ϰ� ����
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hChungMiniGunR;	/// û �̴ϰ� ������
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
		bool		m_bShowSword;
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hElswordSword;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef	SERV_TRAPPING_RANGER_TEST
		bool												m_bShowLTrapSword;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hInstLTrapSwordReady;
#endif	SERV_TRAPPING_RANGER_TEST
		
#ifdef SERV_RENA_NIGHT_WATCHER
		bool												m_bShowRNWTrapSword;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hInstRNWTrapSwordReady;
		bool												m_bChangeClass;
#endif SERV_RENA_NIGHT_WATCHER

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ��ȭ ���� ����Ʈ ��ü
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_ATTACK_LINE_END0;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_ATTACK_LINE_START0_FRONT;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon3;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqUpgradeWeapon4;
#endif ADD_UPGRADE_WEAPON_PARTICLE
		
#ifdef TAKE_OFF_ALL_ITEM
		bool												m_bShowInvenEquip;
#endif // TAKE_OFF_ALL_ITEM


#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSBH1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSBH2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFireSBH3;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
		UNIT_VIEWER_STATE									m_eUnitViewerState;
		
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
		bool												m_bIsPlayAvatarEmotionSoundWithoutEmotion;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandElectricALP1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandElectricALP2;
#endif //SERV_ADD_LUNATIC_PSYKER

};
