/** @file   X2Eqip.h
    @date   jintaeks on 2008-10-15, 15:21
            - managed�� ���ҽ��� local memory�� preload�ϴ� ����� �߰��ϴ�.

    @date   robobeg on 2008-10-17
            - ������ ����ȭ�� ���� CX2Eqip �� CKTDGObject �κ��� �����ϰ�, CKTDGManager �� ������ �� �ֵ��� ����
*/
#pragma once


//{{ seojt // 2009-1-13, 15:47
class CX2SubEquip;
class CX2Eqip;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2Eqip>  CX2EqipPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2Eqip>  CX2EqipPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 15:47


//����ġ �ִϸ��̼� ���� 3���� �ִϸ��̼��� ������
//1. Nomal
//2. Action
//3. Die
//������ ���� �ִϸ��̼��� ������
//4. Change

//{{ robobeg : 2008-10-17
class CX2Eqip : public CKTDGObject
//}} robobeg : 2008-10-17
{
	private:
		//CX2Eqip() { ASSERT( !"invalid" ); }
		// @eUnitType: �������� ��쿡 unit_type�� ���� �ٸ� �޽ø� �ε��Ѵ�.
		CX2Eqip( CX2Item* pItem, CKTDGXSkinAnimPtr pUnitXSkinAnim, int enchantLevel, bool bDummyBasic = false, bool bLoad = true, 
			CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE );

		virtual ~CX2Eqip(void);

		void	AppendAttachedToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

	public: 
		//{{ seojt // 2009-1-14, 21:29
		static CX2EqipPtr CreateEquip( CX2Item* pItem, CKTDGXSkinAnimPtr pUnitXSkinAnim
            , int enchantLevel, bool bDummyBasic = false, bool bLoad = true
            , CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE )
		{
			CX2EqipPtr  ptrEquip( new CX2Eqip( pItem, pUnitXSkinAnim, enchantLevel, bDummyBasic, bLoad, eUnitType )
#ifndef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                                    , CKTDGObject::KTDGObjectDeleter()
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                                    );
            
			// ���ҽ� �ε��� CX2Eqip �����ڿ��� �����ʰ� ������ ȣ�� ���Ŀ� �ϴ� ������ 
			// shared_ptr�� �ʱ�ȭ�� ���Ŀ� ��׶��� �ε��� ��û �ؾ��ϱ� ������
			if( ptrEquip != NULL &&
				true == ptrEquip->GetResourceLoadReserved() )
			{
				ptrEquip->EquipResourceLoad();
			}
			
			return ptrEquip;
		}//CreateEquip
		//}} seojt // 2009-1-14, 21:29

		 void    AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

		void EquipResourceLoad();

		void LoadAttached();
		bool GetResourceLoadReserved() const { return m_bResourceLoadReserved; }




        /// managed�� ���ҽ��� local memory�� preload�Ѵ�.
        /// - jintaeks on 2008-10-15, 15:21
        void                    PreLoad();

//{{ robobeg : 2008-10-17
		//void					SetShow( bool bShow ) { m_bShow = bShow; }
        virtual void            NotifyShowObjectChanged();
//}} robobeg : 2008-10-17

//{{ robobeg : 2008-10-17
        // OnFrameMove ���� RenderParam ������ �и��Ͽ� OnFrameMove ������ �����ϵ��� ����
        HRESULT                 SetRenderParam( double fTime, float fElapsedTime, CKTDGXRenderer::RenderParam* pRenderParam = NULL );
		void SetRenderParamSpecial( CKTDGXRenderer::RenderParam* pEqipRenderParam );

        virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );

        virtual RENDER_HINT		OnFrameRender_Prepare();
		virtual void            OnFrameRender_Draw();
//}} robobeg : 2008-10-17

        //{{ seojt // 2008-12-13, 19:01
		//background!!!
        //virtual bool            OnThreadLoad(DWORD dwParam_, DWORD dwParam2_);
        //}} seojt // 2008-12-13, 19:01

		void					ChangeHyperMode();
		void					ChangeNormalMode();

		bool					CanHyperMode()	const		{ return m_bCanHyperMode; }

//{{ robobeg : 2008-10-17
		//virtual HRESULT			OnResetDevice()			{ return S_OK; }
		//virtual HRESULT			OnLostDevice()			{ return S_OK; }
//}} robobeg : 2008-10-17

		//CKTDGXSkinAnim*			GetUnitXSkinAnim()		{ return m_pUnitXSkinAnim; }
		CX2Item*				GetItem();
		CX2Item::USE_TYPE		GetEqipType() const			{ return m_EqipType; }
		
		//{{ dmlee 2008.11.29 mesh index�� ���ڷ� �Է¹޵��� �����߽��ϴ�.
		CKTDGXSkinAnim*			GetNormalAnim( int index )			{ return m_pNormalXSkinAnim[index]; }
		CKTDGXSkinAnim*			GetHyperAnim( int index )			{ return m_pHyperXSkinAnim[index]; }
		CKTDXDeviceXSkinMesh*	GetNormalModel( int index )			{ return m_pNormalXSkinModel[index]; }
		CKTDXDeviceXSkinMesh*	GetHyperModel( int index )			{ return m_pHyperXSkinModel[index]; }

		const WCHAR*			GetModelName( int index ) const			{ return m_ModelName[index].c_str(); }
		const WCHAR*			GetModelNameHyper( int index ) const	{ return m_ModelNameHyper[index].c_str(); }
		//}} dmlee 2008.11.29 mesh index�� ���ڷ� �Է¹޵��� �����߽��ϴ�.


		CKTDXDeviceXET*			GetNormalTexChangeXET() const { return m_pNormalTexChangeXET; }

		const WCHAR*			GetChangeTexName()	const	{ return m_ChangeTexName.c_str(); }
		const WCHAR*			GetChangeTexNameHyper()	const { return m_ChangeTexNameHyper.c_str(); }
		const WCHAR*			GetAniXETName()	const		{ return m_AniXETName.c_str(); }
		const WCHAR*			GetAniXETNameHyper() const	{ return m_AniXETNameHyper.c_str(); }

		int						GetItemID()	const			{ return m_ItemID; }
		UidType					GetItemUID() const			{ return m_ItemUID; }
		CX2Unit::EQIP_POSITION	GetEqipPos() const			{ return m_EqipPos; }

		int						GetEnchantLevel() const { return m_EnchantLevel; }
		void					ChangeEnchantLevel( int level );

		void					SetOwnerUnit( CX2Unit* pUnit );
		CX2Unit*				GetOwnerUnit() const { return m_pOwnerUnit; }

#ifdef NEW_VILLAGE_UI
		void					SetFixed();
#endif

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ��ȭ ������ UV �ִϸ��̼� �ӵ�
		float					GetUpgradeWeaponUVAnimatonSpeed();
#endif ADD_UPGRADE_WEAPON_PARTICLE

//{{ robobeg : 2008-10-17

		bool                    IsDummyBasic() const { return m_bDummyBasic; }
		int                     GetEnchantType() const
		{
			// fix!! ���� ���߿� script�� ������, switch ������ �ٲٴ��� ���� ���� ���� �� ��.
			if ( m_pItemTemplet != NULL )
			{
                DWORD   dwItemID = m_pItemTemplet->GetItemID();
				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_GOLD ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_GOLD[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_BLUE ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_BLUE[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_GREEN ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_GREEN[i] == dwItemID )
						return 3;
				}		

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_YELLOW ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_YELLOW[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_RED ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_RED[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_PINK ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_PINK[i] == dwItemID )
						return 3;
				}



				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE2 ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE2[i] == dwItemID )
						return 2;
				}

				if ( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_0 )
					return 1;

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE1 ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE1[i] == dwItemID )
						return 1;
				}
			}

			return 0;
        }//GetEnchantType()

		float GetJiggleBone()
		{
			if ( m_pItemTemplet != NULL )
			{
                DWORD   dwItemID = m_pItemTemplet->GetItemID();
				// ����
				for(int i=0; i<ARRAY_SIZE( ITEM_ID_JIGGLE_TYPE1 ); ++i)
				{
					if( ITEM_ID_JIGGLE_TYPE1[i] == dwItemID )
						return 1.5f;
				}
			}

			return -1.f;
		}

//}} robobeg : 2008-10-17


#ifdef EQUIP_ATTACHED_PARTICLE_TEST

		void SetShowAttachedParticle( bool bShow );
		void AttachParticle( bool bIsMajorParticle, const wstring& seqName, const wstring& boneName, D3DXVECTOR3 vOffsetPos );



		// fix!! not yet!! ������ �Ϸ�� ���°� �ƴ�
		struct AttachedParticle
		{
			wstring			m_wstrBoneName;
			bool			m_bIsMajorParticle;
			D3DXVECTOR3 	m_vOffsetPos;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pAttachFrame;

			AttachedParticle();
			~AttachedParticle();
		};

		struct AttachedParticleGroup
		{
			vector<AttachedParticle*>	m_vecpAttachedParticle;

			AttachedParticleGroup();
			~AttachedParticleGroup();

			void OnFrameMove( double fTime, float fElapsedTime );
			void SetShowObject( bool bShow );
		};
#endif EQUIP_ATTACHED_PARTICLE_TEST

		void					ChangeAnim( const WCHAR* pwszAnimName_, const bool bTransition_ );

private:

		void					InitSkin();
		void					InitAttatchAnim();
		void					InitAttatchNormal();

//{{ robobeg : 2008-10-17
        void                    _UpdateSkin(int iIndex_);
        void                    _UpdateSkin();
//}} robobeg : 2008-10-17

//{{ robobeg : 2008-10-17
		//bool						m_bShow;
//}} robobeg : 2008-10-17

		int							m_EnchantLevel;
		bool						m_bDummyBasic;
		CX2Item*					m_pDummyItem;
		const CX2Item::ItemTemplet*		m_pItemTemplet;
		int							m_ItemID;
		UidType						m_ItemUID;
		CX2Unit::EQIP_POSITION		m_EqipPos;
		
		CX2Unit*					m_pOwnerUnit;
		UidType						m_OwnerUnitUID;		// ��׶��� �ε��߿� unit�� �����ϱ� ���ؼ� m_pOwnerUnit�� ������ uid�� �����صε��� �߽��ϴ�.


		//{{ seojt // 2009-1-9, 13:52
		//CKTDGXSkinAnim*				m_pUnitXSkinAnim;
		// dmlee, 2009-03-12 ��׶��� �ε��ÿ� dangling pointer ���� ���� ����� ���� ������ smart poiner�� �ٲ���ϴ�.
        CKTDGXSkinAnimPtr           m_pUnitXSkinAnim;
		//}} seojt // 2009-1-9, 13:52
		bool						m_bHyperMode;
		bool						m_bCanHyperMode;
		CX2Item::USE_TYPE			m_EqipType;

		float						m_fAttatchNormalNowAniTime;
		CKTDGXRenderer::RenderParam m_AttatchNormalRenderParam;
		CKTDGXRenderer::RenderParam m_EnchantRenderParam;


//{{ robobeg : 2008-10-21
// ���� SkinModel attachment ���¸� �˷��ش�. 0: attach ����, 1:Normal, 2:Hyper
        int                         m_iXSkinModelAttachment;
//}} robobeg : 2008-10-21




		//{{ dmlee 2008.11.29 �̺� ���� 2�� �޽� �ٸ��� ���� �� �ְ� �۾�
		D3DXMATRIX*					m_pUnitMatrix[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];						// �θ� unit�� attach�� frame�� combine_matrix
		D3DXMATRIX                  m_mAttachNormalMatrix[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

        // ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
        // - jintaeks on 2009-01-12, 16:19
		CKTDGXSkinAnim*				m_pNormalXSkinAnim[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
        // ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
        // - jintaeks on 2009-01-12, 16:19
		CKTDGXSkinAnim*				m_pHyperXSkinAnim[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

		CKTDXDeviceXSkinMesh*		m_pNormalXSkinModel[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		CKTDXDeviceXSkinMesh*		m_pHyperXSkinModel[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

		CKTDXDeviceXMesh*			m_pNormalXMesh[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		CKTDXDeviceXMesh*			m_pHyperXMesh[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

		wstring						m_ModelName[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		wstring						m_ModelNameHyper[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];		
		//}} dmlee 2008.11.29 �̺� ���� 2�� �޽� �ٸ��� ���� �� �ְ� �۾�
		



		CKTDXDeviceXET*				m_pNormalTexChangeXET;		
		CKTDXDeviceXET*				m_pNormalMultiTexXET;
		CKTDXDeviceXET*				m_pNormalAniXET;
		const CKTDXDeviceXET::AniData*	m_pNormalAniData;

		CKTDXDeviceXET*				m_pHyperTexChangeXET;
		CKTDXDeviceXET*				m_pHyperMultiTexXET;
		CKTDXDeviceXET*				m_pHyperAniXET;
		const CKTDXDeviceXET::AniData*	m_pHyperAniData;

		wstring						m_ChangeTexName;
		wstring						m_AniXETName;
		wstring						m_AniName;
		
		wstring						m_ChangeTexNameHyper;
		wstring						m_AniXETNameHyper;		
		wstring						m_AniNameHyper;

		D3DXVECTOR3					m_Offset;
		float						m_fTexOffset;

		vector<CX2SubEquip*>		m_vecpSubEquip;

#ifdef NEW_VILLAGE_UI
		bool						m_bFixed;
#endif

#ifdef EQUIP_ATTACHED_PARTICLE_TEST
		AttachedParticleGroup		m_AttachedParticleGroup;
#endif EQUIP_ATTACHED_PARTICLE_TEST


		bool						m_bResourceLoadReserved;		// ��ü �����Ŀ� ���ҽ� �ε� ���� ����
		bool						m_bLoadedAttached;				// subequip, attached_particle�� eqip mesh�� ��� �ε��� ���Ŀ� �ε��ؾ��ؼ� flag�� �ϳ� ����
};







