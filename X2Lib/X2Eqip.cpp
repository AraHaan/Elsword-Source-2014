#include "stdafx.h"
#include ".\x2eqip.h"
#include <boost/static_assert.hpp>
#include "boost_kog_extension.h"


// constructor
CX2Eqip::CX2Eqip( CX2Item* pItem, CKTDGXSkinAnimPtr pUnitXSkinAnim, int enchantLevel, bool bDummyBasic, bool bLoad,
				 CX2Unit::UNIT_TYPE eUnitType /*= CX2Unit::UT_NONE */)
//{{ robobeg : 2008-10-17
    : CKTDGObject( NULL )
//}} robobeg : 2008-10-17
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );
//{{ robobeg : 2008-10-17
	//m_bShow = true;
//}} robobeg : 2008-10-17

//{{ seojt // 2008-12-15, 10:56
    /** @NOTE:
        ���ҽ� ������ �ε��� ������ ��, �ε�� ���ҽ� ��ü�� �����Ͱ� 32��Ʈ�� ǥ���ȴٰ� �����Ѵ�.
        �׷��� ������, �����Ͱ� ��ȿ������ �˻��ϱ� ���� �����͸� ���� �˻��ϴ� ���� ��ȿinvalid
        �ϴ�.
        �Ŀ�, �������� read/write�� atomic�ϴٴ� ������ ���� ������, �ε�� ���ҽ� ��ü�� ��ȿvalid
        ������ �Ǵ��ϱ� ���� ������ bool������ ����ؾ� �� ���̴�.
        - jintaeks on 2008-12-15, 10:59 */
	BOOST_STATIC_ASSERT( sizeof(CKTDGXSkinAnim*) == sizeof(int) );
	BOOST_STATIC_ASSERT( sizeof(CKTDXDeviceXSkinMesh*) == sizeof(int) );
	BOOST_STATIC_ASSERT( sizeof(CKTDXDeviceXMesh*) == sizeof(int) );
//}} seojt // 2008-12-15, 10:56


	KTDXPROFILE();


	m_bLoadedAttached = false;

	m_eObjectType = OT_CX2Eqip;

	m_bDummyBasic				= bDummyBasic;
	if ( m_bDummyBasic == true )
		m_pDummyItem = pItem;
	else
		m_pDummyItem = NULL;

	m_EnchantLevel				= enchantLevel;

	m_pItemTemplet				= pItem->GetItemTemplet();
    m_ItemID					= pItem->GetItemTemplet()->GetItemID();
	m_ItemUID					= pItem->GetUID();
	m_EqipPos					= pItem->GetItemTemplet()->GetEqipPosition();
	m_pOwnerUnit				= pItem->GetOwnerUnit();
	if( NULL != m_pOwnerUnit )
	{
		m_OwnerUnitUID				= m_pOwnerUnit->GetUID();
	}
	else
	{
		m_OwnerUnitUID				= -1;
	}


	//{{ seojt // 2009-1-12, 16:19
	m_pUnitXSkinAnim			= pUnitXSkinAnim; 
    //m_pUnitXSkinAnim = pUnitXSkinAnim->get_shared_ptr<CKTDGXSkinAnim>();
	//}} seojt // 2009-1-12, 16:19

	m_bHyperMode				= false; 
	m_bCanHyperMode				= pItem->GetItemTemplet()->GetCanHyperMode();
	m_EqipType					= pItem->GetItemTemplet()->GetUseType();

	m_Offset					= D3DXVECTOR3( 0,0,0 );
	m_fTexOffset				= 0.0f;
	if( m_pOwnerUnit != NULL && 
        m_pItemTemplet != NULL 
        )
	{
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        m_Offset = m_pItemTemplet->GetFrameOffset( m_pOwnerUnit->GetType() );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		switch( m_pOwnerUnit->GetType() )
		{
		default:
		case CX2Unit::UT_ELSWORD:
			m_Offset = m_pItemTemplet->m_FrameOffsetElsword;
			break;

		case CX2Unit::UT_ARME:
			m_Offset = m_pItemTemplet->m_FrameOffsetAisha;
			break;

		case CX2Unit::UT_LIRE:
			m_Offset = m_pItemTemplet->m_FrameOffsetLena;
			break;

		case CX2Unit::UT_RAVEN:
			m_Offset = m_pItemTemplet->m_FrameOffsetRaven;
			break;

		case CX2Unit::UT_EVE:
			m_Offset = m_pItemTemplet->m_FrameOffsetEve;
			break;

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		case CX2Unit::UT_CHUNG:
			m_Offset = m_pItemTemplet->m_FrameOffsetChung;
			break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA:
			m_Offset = m_pItemTemplet->m_FrameOffsetAra;
			break;
#endif
		}
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	}	

	m_fAttatchNormalNowAniTime	= 0.0f;


//{{ robobeg : 2008-10-21
    m_iXSkinModelAttachment = 0;
//}} robobeg : 2008-10-21

	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	{
		m_pUnitMatrix[i]	= NULL;
		D3DXMatrixIdentity( &m_mAttachNormalMatrix[i] );

		m_pNormalXSkinAnim[i] = NULL;
		m_pHyperXSkinAnim[i] = NULL;

		m_pNormalXSkinModel[i] = NULL;
		m_pHyperXSkinModel[i] = NULL;

		m_pNormalXMesh[i] = NULL;
		m_pHyperXMesh[i] = NULL;

		m_ModelName[i] = L"";
		m_ModelNameHyper[i] = L"";
	}

	m_pNormalTexChangeXET		= NULL;
	m_pNormalMultiTexXET		= NULL;
	m_pNormalAniXET				= NULL;
	m_pNormalAniData			= NULL;

	m_pHyperTexChangeXET		= NULL;
	m_pHyperMultiTexXET			= NULL;
	m_pHyperAniXET				= NULL;
	m_pHyperAniData				= NULL;

#ifdef NEW_VILLAGE_UI
	m_bFixed					= false;
#endif

	bool bCommonItem = false;

#ifdef COMMON_ITEM_TEMPLET_TEST
	if( CX2Unit::UT_NONE != eUnitType 
//#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
//        &&
//		false == m_pItemTemplet->m_mapCommonItemModelName.empty() 
//#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        ) // ���� ������
	{
        const wchar_t* pCIMName = m_pItemTemplet->GetCommonItemModelName( eUnitType );
        if ( pCIMName[0] != NULL )
        {
			bCommonItem = true;
			m_ModelName[0] = pCIMName;

			// ������ // ���� ������ XET ���� �ǵ��� �߰�
			const wchar_t* pCIMXetName = m_pItemTemplet->GetCommonItemXETName( eUnitType );
			if( pCIMXetName[0] != NULL )
				m_ChangeTexName = pCIMXetName;
			else
				m_ChangeTexName = m_pItemTemplet->GetTextureChangeXETName();

        }
		else
		{
			bCommonItem = false;
		}
		//map<CX2Unit::UNIT_TYPE, wstring>::const_iterator it = m_pItemTemplet->m_mapCommonItemModelName.find( eUnitType );
		//if( it != m_pItemTemplet->m_mapCommonItemModelName.end() )
		//{
		//	bCommonItem = true;
		//	m_ModelName[0] = it->second;
		//}
		//else
		//{
		//	bCommonItem = false;
		//}
		// fix!!! remove ��������� XET�� ���ʿ�, ��ſ� attach frame name�� �ʿ�. 
		//if( true == bDone )
		//{
		//	it = m_pItemTemplet->m_mapCommonItemXETName.find( eUnitType );
		//	if( it != m_pItemTemplet->m_mapCommonItemXETName.end() )
		//	{
		//		m_ChangeTexName = it->second;
		//	}
		//	else
		//	{
		//		m_ChangeTexName = L"";
		//	}
		//}
	}
#endif COMMON_ITEM_TEMPLET_TEST

	if( false == bCommonItem )
	{
		//ASSERT( false == m_pItemTemplet->m_ModelName.empty() );  
		// note!! '�г��� ����'�� ���� ������ �Ǵ� �޽ð� ���� ������� �ִ�.

		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
		{
            m_ModelName[i] = m_pItemTemplet->GetModelName(i);
		}

		m_ChangeTexName = m_pItemTemplet->GetTextureChangeXETName();
	}
	
	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	{
		if( false == m_ModelName[i].empty() )
		{
			m_ModelNameHyper[i]			= L"Hyper_"; 
			m_ModelNameHyper[i]			+= m_ModelName[i];
		}
	}

	if( false == m_ChangeTexName.empty() )
	{
		m_ChangeTexNameHyper		= L"Hyper_";
		m_ChangeTexNameHyper		+= m_ChangeTexName;
	}
	
    const wchar_t* pAniXETName = m_pItemTemplet->GetAniXETName();
    if ( wcslen( pAniXETName ) >= 3 )
	{
        m_AniXETName				= pAniXETName; 
		m_AniXETNameHyper			= L"Hyper_";
		m_AniXETNameHyper			+= m_AniXETName;
	}
	
    const wchar_t* pAniName = m_pItemTemplet->GetAniName();
    if ( wcslen( pAniName ) >= 3 )
	{
        m_AniName					= pAniName; 
		m_AniNameHyper				= L"Hyper_";
		m_AniNameHyper				+= m_AniName;
	}
	

	m_bResourceLoadReserved = bLoad;
	//EquipResourceLoad(bLoad);

}//CX2Eqip::CX2Eqip()


// destructor.
CX2Eqip::~CX2Eqip(void)
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );

//{{ robobeg : 2008-10-21

    if( m_pUnitXSkinAnim != NULL )
    {
	    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	    {
		    switch( m_iXSkinModelAttachment )
		    {
		    case 1:
			    m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel[i] );
			    break;
		    case 2:
			    m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel[i] );
			    break;
		    }//switch
	    }
    }//if
    m_iXSkinModelAttachment = 0;

	//m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel );
	//m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel );

//}} robobeg : 2008-10-21



	SAFE_CLOSE( m_pNormalTexChangeXET );
	SAFE_CLOSE( m_pNormalMultiTexXET );
	SAFE_CLOSE( m_pNormalAniXET );

	SAFE_CLOSE( m_pHyperTexChangeXET );
	SAFE_CLOSE( m_pHyperMultiTexXET );
	SAFE_CLOSE( m_pHyperAniXET );


	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	{
		SAFE_DELETE_KTDGOBJECT( m_pNormalXSkinAnim[i] );
		SAFE_DELETE_KTDGOBJECT( m_pHyperXSkinAnim[i] );

		SAFE_CLOSE( m_pNormalXSkinModel[i] );
		SAFE_CLOSE( m_pHyperXSkinModel[i] );

		SAFE_CLOSE( m_pNormalXMesh[i] );
		SAFE_CLOSE( m_pHyperXMesh[i] );
	}


	if( m_bDummyBasic == true )
	{
		SAFE_DELETE( m_pDummyItem );
	}


	BOOST_TEST_FOREACH( CX2SubEquip*, pSubEquip, m_vecpSubEquip )
	{
		SAFE_DELETE_KTDGOBJECT( pSubEquip );
	}
	m_vecpSubEquip.clear();

}//CX2Eqip::~CX2Eqip()


//{{ seojt // 2008-10-15, 15:22
void CX2Eqip::PreLoad()
{
	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	{
		if( NULL != m_pNormalXSkinAnim[i] )
			m_pNormalXSkinAnim[i]->PreLoad();

		if( NULL != m_pHyperXSkinAnim[i] )
			m_pHyperXSkinAnim[i]->PreLoad();

		if( NULL != m_pNormalXSkinModel[i] )
			m_pNormalXSkinModel[i]->PreLoad();

		if( NULL != m_pHyperXSkinModel[i] )
			m_pHyperXSkinModel[i]->PreLoad();

		if( NULL != m_pNormalXMesh[i] )
			m_pNormalXMesh[i]->PreLoad();

		if( NULL != m_pHyperXMesh[i] )
			m_pHyperXMesh[i]->PreLoad();
	}

}//CX2Eqip::PreLoad()
//}} seojt // 2008-10-15, 15:22





//{{ robobeg : 2008-10-17
/*virtual*/
void    CX2Eqip::NotifyShowObjectChanged()
{
    _UpdateSkin();

	for( UINT i=0; i<m_vecpSubEquip.size(); i++ )
	{
		m_vecpSubEquip[i]->SetShowObject( GetShowObject() );
	}


#ifdef EQUIP_ATTACHED_PARTICLE_TEST
	m_AttachedParticleGroup.SetShowObject( GetShowObject() );
#endif EQUIP_ATTACHED_PARTICLE_TEST

}//CX2Eqip::SetShowObject()






// OnFrameMove ���� RenderParam ������ �и��Ͽ� OnFrameMove ������ �����ϵ��� ����
HRESULT CX2Eqip::SetRenderParam( double fTime, float fElapsedTime, CKTDGXRenderer::RenderParam* pRenderParam )
{
	if( m_pItemTemplet == NULL )
		return E_FAIL;
    DWORD   dwItemID = m_pItemTemplet->GetItemID();

    if ( pRenderParam == NULL )
        return S_OK;

	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	{
		switch( m_EqipType )
		{
		case CX2Item::UT_ATTACH_ANIM:
			{
				if( m_pNormalXSkinAnim[i] != NULL )
				{
					CKTDGXRenderer::RenderParam* pEqipRenderParam = m_pNormalXSkinAnim[i]->GetRenderParam();
					*pEqipRenderParam = *pRenderParam;                        

					int iEnchantType = GetEnchantType();
					if( iEnchantType != 0 )
					{
#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 11�� �̻���ʹ� UV�ִϸ��̼� �ӵ��� �ٸ��� ����
						m_fTexOffset							+= fElapsedTime * GetUpgradeWeaponUVAnimatonSpeed();
#else  ADD_UPGRADE_WEAPON_PARTICLE
						m_fTexOffset							+= fElapsedTime * 1.0f;
#endif  ADD_UPGRADE_WEAPON_PARTICLE
						pEqipRenderParam->texOffsetStage2.y		= m_fTexOffset;
						float fImpact							= m_EnchantRenderParam.fLightFlowImpact;						
						m_EnchantRenderParam					= *pEqipRenderParam;
						m_EnchantRenderParam.renderType			= CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Z;
						m_EnchantRenderParam.fLightFlowWide		= 5.0f;
						//{{ robobeg : 2008-10-17
						if( iEnchantType == 2 )
							//}} robobeg : 2008-10-17
						{
							m_EnchantRenderParam.color				= 0xffff2222;
						}
						else if( iEnchantType == 3 )
						{
							bool bDone = false;

							for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_GOLD ); i++ )
							{
                                if( ITEM_ID_ENCHANT_TYPE3_GOLD[i] == dwItemID )
								{
									m_EnchantRenderParam.color				= 0xffffea00;
									bDone = true;
									break;
								}
							}

							if( false == bDone )
							{
								for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_BLUE ); i++ )
								{
                                    if( ITEM_ID_ENCHANT_TYPE3_BLUE[i] == dwItemID )
									{
										m_EnchantRenderParam.color				= D3DXCOLOR( 25.f/255.f, 100.f/255.f, 1.f, 1.f );
										bDone = true;
										break;
									}
								}
							}


							if( false == bDone )
							{
								for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_GREEN ); i++ )
								{
                                    if( ITEM_ID_ENCHANT_TYPE3_GREEN[i] == dwItemID )
									{
										m_EnchantRenderParam.color				= D3DXCOLOR( 0.f, 1.f, 0.f, 1.f );
										bDone = true;
										break;
									}

								}		
							}


							if( false == bDone )
							{
								for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_YELLOW ); i++ )
								{
                                    if( ITEM_ID_ENCHANT_TYPE3_YELLOW[i] == dwItemID )
									{
										m_EnchantRenderParam.color				= D3DXCOLOR( 1.f, 1.f, 0.f, 1.f );
										bDone = true;
										break;
									}

								}
							}

							
							if( false == bDone )
							{
								for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_RED ); i++ )
								{
									if( ITEM_ID_ENCHANT_TYPE3_RED[i] == dwItemID )
									{
										m_EnchantRenderParam.color				= D3DXCOLOR( 1.f, 0.f, 0.f, 1.f );
										bDone = true;
										break;
									}
								}
							}

							if( false == bDone )
							{
								for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_PINK ); i++ )
								{
									if( ITEM_ID_ENCHANT_TYPE3_PINK[i] == dwItemID )
									{
										m_EnchantRenderParam.color				= D3DXCOLOR( 0.996f, 0.619f, 1.f, 1.f );
										bDone = true;
										break;
									}
								}
							}							
						}
						else
						{
							m_EnchantRenderParam.color				= 0xffffcccc;
						}

						m_EnchantRenderParam.fLightFlowImpact	= fImpact - fElapsedTime * 200.0f;
						m_EnchantRenderParam.bAlphaBlend		= true;
						m_EnchantRenderParam.srcBlend			= D3DBLEND_SRCALPHA;
						m_EnchantRenderParam.destBlend			= D3DBLEND_DESTALPHA;
						if( m_EnchantRenderParam.fLightFlowImpact < -300.0f )
							m_EnchantRenderParam.fLightFlowImpact = 300.0f;
					}

#ifdef JIGGLEBONE_TEST
					for( int i=0; i<ARRAY_SIZE( ITEM_ID_JIGGLE_TYPE1 ); i++ )
					{
						if( ITEM_ID_JIGGLE_TYPE1[i] == dwItemID )
						{						
							pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
#ifdef UNIT_SCALE_COMBINE_ONE		// �ؿ��� ���� ����
							pEqipRenderParam->fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
							pEqipRenderParam->fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
						}
					}
#endif

					//{{ kimhc // 2010-01-12 // �ܰ��� ���� UT_ATTACH_ANIM �Ǽ��縮
					for ( int i = 0; i < ARRAY_SIZE( REAL_COLOR_ATTACH_ANIM_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( REAL_COLOR_ATTACH_ANIM_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->fOutLineWide	= 0.0f;
						}
					}
					//}} kimhc // 2010-01-12 // �ܰ��� ���� UT_ATTACH_ANIM �Ǽ��縮

					for ( int i = 0; i < ARRAY_SIZE( CARTOON_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( CARTOON_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->renderType	= CKTDGXRenderer::RT_CARTOON;
							pEqipRenderParam->fOutLineWide	= 0.0f;							
						}
					}

					// JHKang / 2012.8.29 / ���� �� ó���� UT_ATTACH_ANIM �Ǽ��縮
					for ( int i = 0; i < ARRAY_SIZE( ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->bAlphaBlend = true;
							pEqipRenderParam->bZEnable = true;
							pEqipRenderParam->renderType = CKTDGXRenderer::RT_CARTOON;
							pEqipRenderParam->fOutLineWide	= 0.0f;
							pEqipRenderParam->bZWriteEnable = false;
						}
					}

#ifdef ARA_CHARACTER_BASE
					for ( int i = 0; i < ARRAY_SIZE( REAL_COLOR_ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( REAL_COLOR_ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->bAlphaBlend = true;
							pEqipRenderParam->bZEnable = true;
							pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->fOutLineWide	= 0.0f;
							pEqipRenderParam->bZWriteEnable = true;
							pEqipRenderParam->srcBlend = D3DBLEND_SRCALPHA;
							pEqipRenderParam->destBlend = D3DBLEND_DESTALPHA;
						}
					}
#endif
					

					{					
						bool bAllAlphaBlend = false;
						for( int i=0; i<ARRAY_SIZE( ALL_ALPHA_ACCESSARY_ITEM_ID ); i++ )
						{
							if( ALL_ALPHA_ACCESSARY_ITEM_ID[i] == dwItemID )
							{
								bAllAlphaBlend = true;
								break;
							}
						}

						if( true == bAllAlphaBlend )
						{
							pEqipRenderParam->bAlphaBlend = true;
							SetAlphaObject( true );
							pEqipRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->fOutLineWide	= 0.0f;		
							pEqipRenderParam->srcBlend		= D3DBLEND_SRCALPHA;
							pEqipRenderParam->destBlend		= D3DBLEND_DESTALPHA;
							pEqipRenderParam->cullMode		= D3DCULL_NONE;
							pEqipRenderParam->bZWriteEnable = false;
						}
					}
#ifdef DARK_DRAGON_ORA_RENDERING
					// �븸 ȫ�� ��� ��� // �Ϻ� ��� ���
					if( 60003794 == dwItemID || 60003795 == dwItemID || 70004450 == dwItemID )
					{
						pEqipRenderParam->bAlphaBlend = true;
						SetAlphaObject( true );
						pEqipRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
						pEqipRenderParam->srcBlend		= D3DBLEND_SRCALPHA;
						pEqipRenderParam->destBlend		= D3DBLEND_INVSRCALPHA;
						pEqipRenderParam->cullMode		= D3DCULL_NONE;
						pEqipRenderParam->bZWriteEnable = false;
					}
#endif DARK_DRAGON_ORA_RENDERING

#ifdef ICE_POCKET_RENDERING
					// �븸 ȫ�� ���� �ָӴ�
					if ( 70007222 == dwItemID || 70007223 == dwItemID || 70007224 == dwItemID )
					{
						pEqipRenderParam->bAlphaBlend = true;
						SetAlphaObject( true );
						pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;						
					}
#endif ICE_POCKET_RENDERING

					SetRenderParamSpecial( pEqipRenderParam );


				}

				if( m_pHyperXSkinAnim[i] != NULL && m_bHyperMode == true && m_bCanHyperMode == true )
				{
					CKTDGXRenderer::RenderParam* pEqipRenderParam = m_pHyperXSkinAnim[i]->GetRenderParam();
					*pEqipRenderParam = *pRenderParam;
					//{{ robobeg : 2008-10-17
					int iEnchantType = GetEnchantType();
					if( iEnchantType != 0 )
						//}} robobeg : 2008-10-17
					{
						m_fTexOffset							+= fElapsedTime * 2.0f;
						pEqipRenderParam->texOffsetStage2.y		= m_fTexOffset;
						float fImpact							= m_EnchantRenderParam.fLightFlowImpact;						
						m_EnchantRenderParam					= *pEqipRenderParam;
						m_EnchantRenderParam.renderType			= CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Z;
						m_EnchantRenderParam.fLightFlowWide		= 5.0f;
						//{{ robobeg : 2008-10-17
						if( iEnchantType == 2 )
							//}} robobeg : 2008-10-17
						{
							m_EnchantRenderParam.color				= 0xffff2222;
						}
						else if( iEnchantType == 3 )
						{
							m_EnchantRenderParam.color				= 0xffffea00;
						}
						else
						{
							m_EnchantRenderParam.color				= 0xffffcccc;
						}
						m_EnchantRenderParam.fLightFlowImpact	= fImpact - fElapsedTime * 200.0f;
						m_EnchantRenderParam.bAlphaBlend		= true;
						m_EnchantRenderParam.srcBlend			= D3DBLEND_SRCALPHA;
						m_EnchantRenderParam.destBlend			= D3DBLEND_DESTALPHA;
						if( m_EnchantRenderParam.fLightFlowImpact < -300.0f )
							m_EnchantRenderParam.fLightFlowImpact = 300.0f;
					}
#ifdef JIGGLEBONE_TEST
					for( int i=0; i<ARRAY_SIZE( ITEM_ID_JIGGLE_TYPE1 ); i++ )
					{
						if( ITEM_ID_JIGGLE_TYPE1[i] == dwItemID )
						{
							pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
#ifdef UNIT_SCALE_COMBINE_ONE		// �ؿ��� ���� ����
							pEqipRenderParam->fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
							pEqipRenderParam->fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
						}
					}
#endif

					//{{ kimhc // 2010-01-12 // �ܰ��� ���� UT_ATTACH_ANIM �Ǽ��縮
					for ( int i = 0; i < ARRAY_SIZE( REAL_COLOR_ATTACH_ANIM_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( REAL_COLOR_ATTACH_ANIM_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->fOutLineWide	= 0.0f;
						}
					}
					//}} kimhc // 2010-01-12 // �ܰ��� ���� UT_ATTACH_ANIM �Ǽ��縮

					for ( int i = 0; i < ARRAY_SIZE( CARTOON_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( CARTOON_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->renderType	= CKTDGXRenderer::RT_CARTOON;
							pEqipRenderParam->fOutLineWide	= 0.0f;
						}
					}

					// JHKang / 2012.8.29 / ���� �� ó���� UT_ATTACH_ANIM �Ǽ��縮
					for ( int i = 0; i < ARRAY_SIZE( ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->bAlphaBlend = true;
							pEqipRenderParam->bZEnable = true;
							pEqipRenderParam->renderType = CKTDGXRenderer::RT_CARTOON;
							pEqipRenderParam->fOutLineWide	= 0.0f;
							pEqipRenderParam->bZWriteEnable = false;
						}
					}

#ifdef ARA_CHARACTER_BASE
					for ( int i = 0; i < ARRAY_SIZE( REAL_COLOR_ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( REAL_COLOR_ALPHA_BLEND_ATTACH_ANIM_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							pEqipRenderParam->bAlphaBlend = true;
							pEqipRenderParam->bZEnable = true;
							pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->fOutLineWide	= 0.0f;
							pEqipRenderParam->bZWriteEnable = true;
							pEqipRenderParam->srcBlend = D3DBLEND_SRCALPHA;
							pEqipRenderParam->destBlend = D3DBLEND_DESTALPHA;
						}
					}
#endif

					{					
						bool bAllAlphaBlend = false;
						for( int i=0; i<ARRAY_SIZE( ALL_ALPHA_ACCESSARY_ITEM_ID ); i++ )
						{
							if( ALL_ALPHA_ACCESSARY_ITEM_ID[i] == dwItemID )
							{
								bAllAlphaBlend = true;
								break;
							}
						}

						if( true == bAllAlphaBlend )
						{
							pEqipRenderParam->bAlphaBlend = true;
							SetAlphaObject( true );
							pEqipRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
							pEqipRenderParam->fOutLineWide	= 0.0f;		
							pEqipRenderParam->srcBlend		= D3DBLEND_SRCALPHA;
							pEqipRenderParam->destBlend		= D3DBLEND_DESTALPHA;
							pEqipRenderParam->cullMode		= D3DCULL_NONE;
							pEqipRenderParam->bZWriteEnable = false;
						}
					}
#ifdef DARK_DRAGON_ORA_RENDERING
					// �븸 ȫ�� ��� ��� // �Ϻ� �����
					if( 60003794 == dwItemID || 60003795 == dwItemID || 70004450 == dwItemID )
					{
						pEqipRenderParam->bAlphaBlend = true;
						SetAlphaObject( true );
						pEqipRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
						pEqipRenderParam->srcBlend		= D3DBLEND_SRCALPHA;
						pEqipRenderParam->destBlend	= D3DBLEND_INVSRCALPHA;
						pEqipRenderParam->cullMode		= D3DCULL_NONE;
						pEqipRenderParam->bZWriteEnable = false;
					}
#endif DARK_DRAGON_ORA_RENDERING

#ifdef ICE_POCKET_RENDERING
					// �븸 ȫ�� ���� �ָӴ�
					if ( 70007222 == dwItemID || 70007223 == dwItemID || 70007224 == dwItemID )
					{
						pEqipRenderParam->bAlphaBlend = true;
						SetAlphaObject( true );
						pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
					}
#endif ICE_POCKET_RENDERING

					SetRenderParamSpecial( pEqipRenderParam );

				}
			}
			break;

		case CX2Item::UT_ATTACH_NORMAL:
			{
				m_AttatchNormalRenderParam = *pRenderParam;

				// �ܰ��� ���� ī��ó��
				{
					for ( int i = 0; i < ARRAY_SIZE( CARTOON_ACCESSARY_ITEM_ID ); ++i )
					{
						if ( CARTOON_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							m_AttatchNormalRenderParam.renderType = CKTDGXRenderer::RT_CARTOON;
							m_AttatchNormalRenderParam.fOutLineWide= 0.0f;							
						}
					}
				}				

				{
					bool bAlphaBlend = false;
					for( int i=0; i<ARRAY_SIZE( ALPHA_BLEND_ACCESSARY_ITEM_ID ); i++ )
					{
						if( ALPHA_BLEND_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							bAlphaBlend = true;
							break;
						}
					}

					if( true == bAlphaBlend )
					{
						// �켱�� static mesh �Ǽ����� ���Ĵ� ���⼭ �ӽ÷� ������... ���� ��۶�, etc.
						m_AttatchNormalRenderParam.bAlphaBlend = true;
						//{{ robobeg : 2008-10-21
						SetAlphaObject( true );
						//}} robobeg : 2008-10-21
					}
				}




				{
					// �ܰ��� ���� ���� ������Ʈ �ӽ÷� �ϵ��ڵ�
					bool bAlphaBlend = false;
					for( int i=0; i<ARRAY_SIZE( ALPHA_BLEND_REAL_COLOR_ACCESSARY_ITEM_ID ); i++ )
					{
						if( ALPHA_BLEND_REAL_COLOR_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							bAlphaBlend = true;
							break;
						}
					}

					if( true == bAlphaBlend )
					{
						m_AttatchNormalRenderParam.bAlphaBlend = true;
						SetAlphaObject( true );


						m_AttatchNormalRenderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;
					}
				}




				
				{
					// dmlee, 2009-02-25, note!! �Ʒ��� �ڵ�� ��ũ���� ��Ŭ�� �������� ���� �ӽ÷� �߰��� �ڵ�, ���߿� ��ũ��Ʈ�� �ű�ų� �ؾߵ�
					bool bDestAlphaBlend = false;
					for( int i=0; i<ARRAY_SIZE( DEST_ALPHA_BLEND_ACCESSARY_ITEM_ID ); i++ )
					{
						if( DEST_ALPHA_BLEND_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							bDestAlphaBlend = true;
							break;
						}
					}

					if( true == bDestAlphaBlend )
					{
						m_AttatchNormalRenderParam.bAlphaBlend = true;
						SetAlphaObject( true );
						m_AttatchNormalRenderParam.srcBlend		= D3DBLEND_SRCALPHA;
						m_AttatchNormalRenderParam.destBlend	= D3DBLEND_DESTALPHA;
						m_AttatchNormalRenderParam.cullMode		= D3DCULL_NONE;
					}
				}

				{					
					bool bAllAlphaBlend = false;
					for( int i=0; i<ARRAY_SIZE( ALL_ALPHA_ACCESSARY_ITEM_ID ); i++ )
					{
						if( ALL_ALPHA_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							bAllAlphaBlend = true;
							break;
						}
					}

					if( true == bAllAlphaBlend )
					{
						m_AttatchNormalRenderParam.bAlphaBlend = true;
						SetAlphaObject( true );
						m_AttatchNormalRenderParam.renderType	= CKTDGXRenderer::RT_REAL_COLOR;
						m_AttatchNormalRenderParam.srcBlend		= D3DBLEND_SRCALPHA;
						m_AttatchNormalRenderParam.destBlend	= D3DBLEND_DESTALPHA;
						m_AttatchNormalRenderParam.cullMode		= D3DCULL_NONE;
						m_AttatchNormalRenderParam.bZWriteEnable = false;
						
					}
				}

				{					
					bool bAlphaCullNone = false;
					for( int i=0; i<ARRAY_SIZE( ALPHA_REAL_COLOR_CULL_NONE_ACCESSARY_ITEM_ID ); i++ )
					{
						if( ALPHA_REAL_COLOR_CULL_NONE_ACCESSARY_ITEM_ID[i] == dwItemID )
						{
							bAlphaCullNone = true;
							break;
						}
					}

					if( true == bAlphaCullNone )
					{
						m_AttatchNormalRenderParam.bAlphaBlend	= true;
						SetAlphaObject( true );
						m_AttatchNormalRenderParam.cullMode		= D3DCULL_NONE;
						m_AttatchNormalRenderParam.renderType	= CKTDGXRenderer::RT_REAL_COLOR;

					}
				}
				
				CKTDGXRenderer::RenderParam* pEqipRenderParam = pRenderParam;

				if ( NULL != pEqipRenderParam )
					SetRenderParamSpecial( pEqipRenderParam );

			} break;

		} // switch
	} // for


	return S_OK;
}//CX2Eqip::SetRenderParam()





void CX2Eqip::SetRenderParamSpecial( CKTDGXRenderer::RenderParam* pEqipRenderParam )
{
	// SF���� ���ܷ� �ܰ��� ���� ī������ �׸���
    switch( m_pItemTemplet->GetItemID() )
	{
	case 130892:
	case 130895:
		{
			pEqipRenderParam->renderType = CKTDGXRenderer::RT_CARTOON;
		} break;	

	case 215670:
	case 215700:
	case 215760:
	case 238130:
#ifdef CARTOON_SPECIAL_RENDER_ITEM_WITH_ALPHA	
	case 70000510:	// �ӱԼ� �Ϻ� �߰� ���ϵ� ����ҵ�
	case 70000520:	// �ӱԼ� �Ϻ� �߰� ���ϵ� ����ҵ�
#endif //CARTOON_SPECIAL_RENDER_ITEM_WITH_ALPHA	
		{
			pEqipRenderParam->bAlphaBlend = true;
			SetAlphaObject( true );

			pEqipRenderParam->renderType = CKTDGXRenderer::RT_CARTOON;
		} break;
	case 232170:
	case 232180:
	case 81086:
	case 81087:
	case 81088:
	case 81089:
	case 81090:
	case 81091:
	case 81161:
	case 81162:
	case 81163:
	case 81164:
	case 81165:
	case 81166:
	case 180143:
	case 81226:
	case 81236:
	case 81246:
	case 81256:
	case 81266:
	case 81276:
#ifdef REAL_COLOR_SPECIAL_RENDER_ITEM_WITH_ALPHA
	case 60005390:
	case 60005393:
	case 84001760:
	case 90002340:	// ������ ����
#endif //REAL_COLOR_SPECIAL_RENDER_ITEM_WITH_ALPHA
	case 88396: /// ������ ��ť���� ���� ( ���ҵ� )
	case 88397: /// �׷��̽� �� ���� ( ���̻� )
	case 88398: /// �׷��̽� �� ���� ( ���� )
	case 88399: /// ������ ��ť���� ���� ( ���̺� )
	case 88400: /// �׷��̽� �� ���� ( �̺� )
	case 88401: /// ������ ��ť���� ���� ( û )
	case 81986: /// �׷��̽� �� ����Ʈ ��(�ƶ�)
	case 81987:	/// �׷��̽� �� ���� (�ƶ�)
	case 81955: /// �ݰ����� ȭ����ȫ ���� (�ƶ�)
	case 81962: /// �ݰ� õ���� ����ȯ ���� (�ƶ�)
	case 82003: /// �ݰ����� ȭ����ȫ ���� (�����ý�)
	case 82010: /// �ݰ� õ���� ����ȯ ���� (�����ý�)
	case 82075:	/// �ƶ� �̺� Ʈ���̼�
		{
		pEqipRenderParam->bAlphaBlend = true;
		SetAlphaObject( true );

		pEqipRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
		} break;
	}
}


HRESULT CX2Eqip::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

//{{ robobeg : 2008-10-21
	if( m_pItemTemplet == NULL )
		return E_FAIL;

    if ( !GetShowObject() )
        return E_FAIL;

//}} robobeg : 2008-10-21



	switch( m_EqipType )
	{
	case CX2Item::UT_SKIN:
		{
			for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
			{
				if( NULL != m_pNormalXSkinModel[i] )
				{
                    switch( m_pItemTemplet->GetItemID() )
					{
					case 202560:
					case 202610:
						{
							m_pNormalXSkinModel[i]->SetAlphaRender(true);
						} break;
#ifdef FACE_OFF_MONSTER_HEAD_TEST
						// UnitTemplet.lua�� ��ϵ� ĳ���� BasicFace ��� 
					case 2:
					case 9:
					case 16:
					case 29:
					case 36:
					case 44:
					case 45:
					case 50:
					case 51:
					case 57:
					case 41400:
						{
							m_pNormalXSkinModel[i]->SetNoRenderable(true);
						} break;
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef ARA_CHARACTER_BASE
					case 41510:
					case 41640:
					case 41700:
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
					case 41736:
					case 41743:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
						{
							m_pNormalXSkinModel[i]->SetUseRenderParam( true );
						} break;
#endif
					default:
						{
							m_pNormalXSkinModel[i]->SetAlphaRender(false);
						} break;
					}
				}

				if( NULL != m_pHyperXSkinModel[i] && m_bHyperMode == true && m_bCanHyperMode == true )
				{
                    switch( m_pItemTemplet->GetItemID() )
					{
#ifdef ARA_CHARACTER_BASE
					case 41510:
					case 41640:
					case 41700:
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
					case 41736:
					case 41743:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
						{
							m_pHyperXSkinModel[i]->SetUseRenderParam( true );
						} break;
#endif

					case 202560:
					case 202610:
						{
							m_pHyperXSkinModel[i]->SetAlphaRender(true);
						} break;

					default:
						{
							m_pHyperXSkinModel[i]->SetAlphaRender(false);
						} break;
					}
				}
			}

#ifdef ATTACH_SKINMESH
			if( false == m_bLoadedAttached )
			{
				LoadAttached();
			}
#endif
		} break;

	case CX2Item::UT_ATTACH_ANIM:
		{
			for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
			{
				if( NULL != m_pUnitMatrix[i] )
				{
					if( NULL != m_pNormalXSkinAnim[i] )
					{
						CKTDGXRenderer::RenderParam* pEqipRenderParam = m_pNormalXSkinAnim[i]->GetRenderParam();
						D3DXMATRIX localMove, matTrans, matRot, matScale;

						CX2Unit::UNIT_TYPE unitType = m_pOwnerUnit->GetType();
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                        const D3DXVECTOR3& vScale = m_pItemTemplet->GetRenderScale(unitType);
                        const D3DXVECTOR3& vRotate = m_pItemTemplet->GetRenderRotate(unitType);
						D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z );
						D3DXMatrixRotationYawPitchRoll( &matRot, vRotate.y, vRotate.x, vRotate.z );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
						D3DXMatrixScaling(&matScale, m_pItemTemplet->m_RanderScale[unitType].x, m_pItemTemplet->m_RanderScale[unitType].y, m_pItemTemplet->m_RanderScale[unitType].z );
						D3DXMatrixRotationYawPitchRoll( &matRot, m_pItemTemplet->m_RanderRotate[unitType].y, m_pItemTemplet->m_RanderRotate[unitType].x, m_pItemTemplet->m_RanderRotate[unitType].z );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

						D3DXMatrixTranslation( &matTrans, m_Offset.x, m_Offset.y, m_Offset.z );

						D3DXMatrixMultiply( &localMove, &matScale, &matRot );
						D3DXMatrixMultiply( &localMove, &localMove, &matTrans );

						//{{ robobeg : 2008-10-24
						D3DXMATRIX  mWorld;
						D3DXMatrixMultiply( &mWorld, &localMove, m_pUnitMatrix[i] );
						m_pNormalXSkinAnim[i]->SetDXMatrix( mWorld );
						//}} robobeg : 2008-10-24

						m_pNormalXSkinAnim[i]->OnFrameMove( fTime, fElapsedTime );

					}


					if( NULL != m_pHyperXSkinAnim[i] && m_bHyperMode == true && m_bCanHyperMode == true )
					{
						CKTDGXRenderer::RenderParam* pEqipRenderParam = m_pHyperXSkinAnim[i]->GetRenderParam();
						D3DXMATRIX localMove, matTrans, matRot, matScale;

						CX2Unit::UNIT_TYPE unitType = m_pOwnerUnit->GetType();
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                        const D3DXVECTOR3& vScale = m_pItemTemplet->GetRenderScale(unitType);
                        const D3DXVECTOR3& vRotate = m_pItemTemplet->GetRenderRotate(unitType);
						D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z );
						D3DXMatrixRotationYawPitchRoll( &matRot, vRotate.y, vRotate.x, vRotate.z );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                        D3DXMatrixScaling(&matScale, m_pItemTemplet->m_RanderScale[unitType].x, m_pItemTemplet->m_RanderScale[unitType].y, m_pItemTemplet->m_RanderScale[unitType].z );
                        D3DXMatrixRotationYawPitchRoll( &matRot, m_pItemTemplet->m_RanderRotate[unitType].y, m_pItemTemplet->m_RanderRotate[unitType].x, m_pItemTemplet->m_RanderRotate[unitType].z );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
						D3DXMatrixTranslation( &matTrans, m_Offset.x, m_Offset.y, m_Offset.z );

						D3DXMatrixMultiply( &localMove, &matScale, &matRot );
						D3DXMatrixMultiply( &localMove, &localMove, &matTrans );

						//{{ robobeg : 2008-10-24
						D3DXMATRIX  mWorld;
						D3DXMatrixMultiply( &mWorld, &localMove, m_pUnitMatrix[i] );
						m_pHyperXSkinAnim[i]->SetDXMatrix( mWorld );
						//}} robobeg : 2008-10-24
						m_pHyperXSkinAnim[i]->OnFrameMove( fTime, fElapsedTime );
					}
				}
			}



			if( false == m_bLoadedAttached )
			{
				LoadAttached();
			}
			

		} break;

	case CX2Item::UT_ATTACH_NORMAL:
		{
			for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
			{
				if( NULL != m_pUnitMatrix[i] )
				{
					D3DXMATRIX localMove, matTrans, matRot, matScale;

					CX2Unit::UNIT_TYPE unitType = m_pOwnerUnit->GetType();
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    const D3DXVECTOR3& vScale = m_pItemTemplet->GetRenderScale(unitType);
                    const D3DXVECTOR3& vRotate = m_pItemTemplet->GetRenderRotate(unitType);
					D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z );
					D3DXMatrixRotationYawPitchRoll( &matRot, vRotate.y, vRotate.x, vRotate.z );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					D3DXMatrixScaling(&matScale, m_pItemTemplet->m_RanderScale[unitType].x, m_pItemTemplet->m_RanderScale[unitType].y, m_pItemTemplet->m_RanderScale[unitType].z );
					D3DXMatrixRotationYawPitchRoll( &matRot, m_pItemTemplet->m_RanderRotate[unitType].y, m_pItemTemplet->m_RanderRotate[unitType].x, m_pItemTemplet->m_RanderRotate[unitType].z );
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					D3DXMatrixTranslation( &matTrans, m_Offset.x, m_Offset.y, m_Offset.z );

					D3DXMatrixMultiply( &localMove, &matScale, &matRot );
					D3DXMatrixMultiply( &localMove, &localMove, &matTrans );

					//{{ robobeg : 2008-10-24
					D3DXMatrixMultiply( &m_mAttachNormalMatrix[i], &localMove, m_pUnitMatrix[i] );
					//}} robobeg : 2008-10-24
				}
			}
		} break;
	}



#ifdef ATTACH_SKINMESH
	for(UINT i=0; i<m_vecpSubEquip.size(); ++i)
	{
		CX2SubEquip *pSubEquip = m_vecpSubEquip[i];
		if( pSubEquip != NULL )
		{
			pSubEquip->OnFrameMove(fTime, fElapsedTime);
		}
	}
#endif

#ifdef EQUIP_ATTACHED_PARTICLE_TEST
	m_AttachedParticleGroup.OnFrameMove( fTime, fElapsedTime );
#endif EQUIP_ATTACHED_PARTICLE_TEST


	return S_OK;
}

//}} robobeg : 2008-10-17


//{{ robobeg : 2008-10-17
/*virtual*/
RENDER_HINT   CX2Eqip::OnFrameRender_Prepare()
{
    if ( m_pItemTemplet == NULL )
        return RENDER_HINT_NORENDER;

#ifdef WORLD_TOOL
	if( !GetShowObject() ) 
		return RENDER_HINT_NORENDER;
#endif

	// dmlee 2008.11.29 note!! eqip�ϳ��� �������� �޽ð� �� �־ render_hint�� ù��° �޽ø� �������� �ϴ� ������ ����.

    switch( m_EqipType )
    {
    case CX2Item::UT_ATTACH_ANIM:
        {
            RENDER_HINT renderHintResult = RENDER_HINT_NORENDER;
            RENDER_HINT renderHint;
	        for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	        {
                if ( m_bHyperMode == false || m_bCanHyperMode == false )
                {
                    if ( m_pNormalXSkinAnim[i] != NULL )
                    {
                        m_pNormalXSkinAnim[i]->SetInstantShow( false );
                        if ( !m_pNormalXSkinAnim[i]->GetShowObject() )
                            continue;
                        renderHint = m_pNormalXSkinAnim[i]->OnFrameRender_Prepare();
                        if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
                        {
                            m_pNormalXSkinAnim[i]->SetInstantShow( true );
                            if ( GetEnchantType() == 0 )
                                renderHintResult.MergeRenderHint( renderHint );
                            else
                                renderHintResult = RENDER_HINT_DEFAULT;
                        }//if
                    }//if
                }
                else
                {
                    if ( m_pHyperXSkinAnim[i] != NULL )
                    {
                        m_pHyperXSkinAnim[i]->SetInstantShow( false );
                        if ( !m_pHyperXSkinAnim[i]->GetShowObject() )
                            continue;
                        renderHint = m_pHyperXSkinAnim[i]->OnFrameRender_Prepare();
                        if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
                        {
                            m_pHyperXSkinAnim[i]->SetInstantShow( true );
                            if ( GetEnchantType() == 0 )
                                renderHintResult.MergeRenderHint( renderHint );
                            else
                                renderHintResult = RENDER_HINT_DEFAULT;
                        }//if
                    }//if
                }//if.. else..
            }//for
            return renderHintResult;
        }
    case CX2Item::UT_ATTACH_NORMAL:
        for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	    {
            if( m_bHyperMode == false || m_bCanHyperMode == false )
		    {
                if ( m_pNormalXMesh[i] != NULL )
                    return RENDER_HINT_XMESH( m_AttatchNormalRenderParam.renderType
                    , m_AttatchNormalRenderParam.bFog && m_AttatchNormalRenderParam.bFogShow );
            }
            else
            {
			    if ( m_pHyperXMesh[i] != NULL )
                    return RENDER_HINT_XMESH( m_AttatchNormalRenderParam.renderType
                    , m_AttatchNormalRenderParam.bFog && m_AttatchNormalRenderParam.bFogShow );
            }//if.. else..
        }//for
        break;
#ifdef ATTACH_SKINMESH
	case CX2Item::UT_SKIN:
		{			
			for(UINT i=0; i<m_vecpSubEquip.size(); ++i)
			{
				CX2SubEquip *pSubEquip = m_vecpSubEquip[i];
				if( pSubEquip != NULL && pSubEquip->GetIsSkinMesh() == true )
				{
					return pSubEquip->OnFrameRender_Prepare();
				}
			}			
		}
		break;
#endif
    }//switch

    return RENDER_HINT_NORENDER;
}//CX2Eqip::OnFrameRender_Prepare()



/*virtual*/
void    CX2Eqip::OnFrameRender_Draw()
{
	KTDXPROFILE();

//{{ robobeg : 2008-10-17

	// ���������� �ʴ� ���� OnFrameRender_Prepare() ���� �����ش�.

//}} robobeg : 2008-10-17

    DWORD   dwItemID = m_pItemTemplet->GetItemID();

	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	{
		// oasis907 : ����� [2011.8.30] ������ ��縮 �ָ� �Ǽ� - ���̺� ���ȿ��� ���� �ʰ� �ϵ� �ڵ�
		if( m_pOwnerUnit != NULL &&  
			m_pOwnerUnit->GetType() == CX2Unit::UT_RAVEN &&
			i == 1 &&
			IsNoRenderNasodHandPos( dwItemID ) )
		{
			continue;
		}
		if( m_pOwnerUnit != NULL &&  
			m_pOwnerUnit->GetType() == CX2Unit::UT_RAVEN &&
			i == 0 &&
			IsNoRenderNasodArmPos( dwItemID ) )
		{
			continue;
		}

		switch( m_EqipType )
		{
		case CX2Item::UT_ATTACH_ANIM:
			{
				if( m_bHyperMode == false || m_bCanHyperMode == false )
				{
					if ( m_pNormalXSkinAnim[i] != NULL && m_pNormalXSkinAnim[i]->GetShowObject()
                        && m_pNormalXSkinAnim[i]->IsInstantShow() )
					{
						m_pNormalXSkinAnim[i]->OnFrameRender_Draw();
						//{{ robobeg : 2008-10-17
						if( GetEnchantType() != 0 )
							//}} robobeg : 2008-10-17
						{
							CKTDGXRenderer::RenderParam* pRenderParam = m_pNormalXSkinAnim[i]->GetRenderParam();
							CKTDGXRenderer::RenderParam tempRenderParam = *pRenderParam;

							*pRenderParam = m_EnchantRenderParam;
							m_pNormalXSkinAnim[i]->OnFrameRender_Draw();
							*pRenderParam = tempRenderParam;
						}					
					}
				}
				else
				{
					if ( m_pHyperXSkinAnim[i] != NULL && m_pHyperXSkinAnim[i]->GetShowObject()
                        && m_pHyperXSkinAnim[i]->IsInstantShow() )
					{
						m_pHyperXSkinAnim[i]->OnFrameRender_Draw();
						//{{ robobeg : 2008-10-17
						if( GetEnchantType() != 0 )
							//}} robobeg : 2008-10-17
						{
							CKTDGXRenderer::RenderParam* pRenderParam = m_pHyperXSkinAnim[i]->GetRenderParam();
							CKTDGXRenderer::RenderParam tempRenderParam = *pRenderParam;

							*pRenderParam = m_EnchantRenderParam;
							m_pHyperXSkinAnim[i]->OnFrameRender_Draw();
							*pRenderParam = tempRenderParam;
						}
					}
				}			

			} break;

		case CX2Item::UT_ATTACH_NORMAL:
			{
				//{{ robobeg : 2008-10-15
				//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
				//*pRenderParam = m_AttatchNormalRenderParam;
				//}} robobeg : 2008-10-15

				if( m_bHyperMode == false || m_bCanHyperMode == false )
				{
					if ( m_pNormalXMesh[i] != NULL )
					{
						//{{ robobeg : 2008-10-08
						//if ( SUCCEEDED( g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender() ) )
						//                  {
						g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_AttatchNormalRenderParam, m_mAttachNormalMatrix[i], 
							*m_pNormalXMesh[i], m_pNormalTexChangeXET, NULL, m_pNormalAniData );
						//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
						//}//if
						//}} robobeg : 2008-10-08
					}
				}
				else
				{
					if ( m_pHyperXMesh[i] != NULL )
					{
						//{{ robobeg : 2008-10-08
						//if ( SUCCEEDED( g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender() ) )
						//{
						g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_AttatchNormalRenderParam, m_mAttachNormalMatrix[i], 
							*m_pHyperXMesh[i], m_pNormalTexChangeXET, NULL, m_pNormalAniData );
						//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
						//              }//if
						//}} robobeg : 2008-10-08
					}
				}

			} break;

#ifdef ATTACH_SKINMESH
		case CX2Item::UT_SKIN:
			{
				for(vector<CX2SubEquip*>::iterator it = m_vecpSubEquip.begin(); it<m_vecpSubEquip.end(); ++it)
				{
					CX2SubEquip* pSubEquip = *it;

					/// ���� ���� ��ο� ���� �ʴ� ���� ������ ����, ���� ���� 
					/// ( Item�� ATTATCHED_MESH ��� �� y ���� ����� ���� ���� ó��, �ش� �Լ� �ȿ��� ��Ų �Ž� ���θ� �˻��Ͽ� ������ �ְ� �ִ�. )
					//if( pSubEquip->GetIsSkinMesh() == true )
						pSubEquip->OnFrameRender_Draw();
				}
			}			
			break;
#endif
		} // switch
	} // for

	if(m_bFixed)
	{
#ifdef SUB_EQUIP_TEST
		for(vector<CX2SubEquip*>::iterator it = m_vecpSubEquip.begin(); it<m_vecpSubEquip.end(); ++it)
		{
			CX2SubEquip* pSubEquip = *it;
			pSubEquip->OnFrameRender_Draw();
		}
#endif SUB_EQUIP_TEST


#ifdef EQUIP_ATTACHED_PARTICLE_TEST
		for(vector<AttachedParticle*>::iterator itParticle = m_AttachedParticleGroup.m_vecpAttachedParticle.begin();
			itParticle < m_AttachedParticleGroup.m_vecpAttachedParticle.end();
			++itParticle)
		{
			AttachedParticle* pAttachedParticle = *itParticle;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
			
			if( true == pAttachedParticle->m_bIsMajorParticle )
			{
				pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
			}
			else
			{
				pSeq = g_pData->GetUIMinorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
			}

			if( NULL != pSeq )
			{
				CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
				pSeq->OnFrameRender_Draw();
			}
			
		}
#endif
	}
}

//}} robobeg : 2008-10-17


//{{ seojt // 2008-12-13, 19:01
// /*virtual*/ bool CX2Eqip::OnThreadLoad(DWORD dwParam_, DWORD dwParam2_)
// {
//     //CKTDGXSkinAnimPtr   m_pUnitXSkinAnim = m_ptrUnitXSkinAnim.lock();
//     //ASSERT( m_pUnitXSkinAnim != NULL );
//     //if( m_pUnitXSkinAnim == NULL )
//     //    return false;
// 
//     if( dwParam_ == CKTDXDeviceManager::THREAD_REQUEST_EQUIP_NORMAL )
//     {
//         const int iArrayIndex = (int)dwParam2_;
//         ASSERT( iArrayIndex >= 0 && iArrayIndex < ARRAY_SIZE(m_ModelName) );
//         if( !(iArrayIndex >= 0 && iArrayIndex < ARRAY_SIZE(m_ModelName)) )
//             return true; // ������ ��� index�� ��忡 ���ؼ� retry���� �ʵ��� true�� �����Ѵ�.
// 
//         const int& i = iArrayIndex;
// 
//         ASSERT( sizeof(m_pNormalXMesh[i]) == sizeof(int) );
//         m_pNormalXMesh[i] = g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelName[i] );
// 
// 	    if( m_bCanHyperMode == true )
// 	    {
//             m_pHyperXMesh[i] = g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelNameHyper[i] );
// 	    }//if
//     	
// 
// 		if( false == m_pItemTemplet->m_AttachFrameName[i].empty() )
// 		{
//             CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
// 			pFrame = m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->m_AttachFrameName[i].c_str() );
// 			if( NULL != pFrame )
// 			{
// 				m_pUnitMatrix[i] = &pFrame->combineMatrix;
// 			}//if
// 		}//if
//         return true;
//     }
//     else if( dwParam_ == CKTDXDeviceManager::THREAD_REQUEST_EQUIP_SKIN_NORMAL )
//     {
//         const int iArrayIndex = (int)dwParam2_;
//         ASSERT( iArrayIndex >= 0 && iArrayIndex < ARRAY_SIZE(m_ModelName) );
//         if( !(iArrayIndex >= 0 && iArrayIndex < ARRAY_SIZE(m_ModelName)) )
//             return true; // ������ ��� index�� ��忡 ���ؼ� retry���� �ʵ��� true�� �����Ѵ�.
// 
//         ASSERT( sizeof(m_pNormalXSkinModel[iArrayIndex]) == sizeof(int) );
//         m_pNormalXSkinModel[iArrayIndex] = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[iArrayIndex] );
// 	    if( m_bCanHyperMode == true )
// 	    {
//             m_pHyperXSkinModel[iArrayIndex] = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelNameHyper[iArrayIndex] );
// 	    }//if
// 
// 	    if ( iArrayIndex == 0 && m_pNormalXSkinModel[iArrayIndex] == NULL )
// 	    {
// 		    wstring errorMSG;
// 		    errorMSG = L"- �� �̸� : ";
// 		    errorMSG += m_ModelName[iArrayIndex];
// 
// 		    ErrorLogMsg( XEM_ERROR93, errorMSG.c_str() );
// 		    ASSERT( !"CX2Eqip::InitSkin, null normal skin model" );
// 
// 		    if ( m_bCanHyperMode == true && m_pHyperXSkinModel[iArrayIndex] == NULL )
// 		    {
// 			    wstring errorMSG2;
// 			    errorMSG2 = L"- �� �̸� : ";
// 			    errorMSG2 += m_ModelNameHyper[iArrayIndex];
// 
// 			    ErrorLogMsg( XEM_ERROR93, errorMSG2.c_str() );
// 		    }//if
// 	    }//if
// 
// 		//{{ dmlee 2009.3.19 �޽ð� �ִ� ��쿡�� update �ϵ���
// 		if( NULL != m_pNormalXSkinModel[iArrayIndex] || 
// 			NULL != m_pHyperXSkinModel[iArrayIndex] )
// 		//}} dmlee 2009.3.19 �޽ð� �ִ� ��쿡�� update �ϵ���		
// 		{
// 			//{{ dmlee 2009.3.20 ĳ���۰� ED���� ���ÿ� �����ϰ� �ִ� ��쿡 ED���� _updateSkin�� ȣ������ �ʵ���
// 			_UpdateSkin( iArrayIndex );
// 			//if( -1 != m_OwnerUnitUID &&
// 			//	NULL != m_pItemTemplet )
// 			//{
// 			//	if( true == m_pItemTemplet->GetFashion() )
// 			//	{
// 			//		_UpdateSkin( iArrayIndex );
// 			//	}
// 			//	else
// 			//	{
// 			//		if( NULL != g_pTFieldGame )
// 			//		{
// 			//			CX2SquareUnit* pFieldUnit = g_pTFieldGame->GetSquareUnitByUID( m_OwnerUnitUID );
// 			//			if( NULL != pFieldUnit &&
// 			//				pFieldUnit->GetUnitViewer() )
// 			//			{
// 			//				if( false == pFieldUnit->GetUnitViewer()->IsFashionItemEquippedAt( m_pItemTemplet->GetEqipPosition() ) )
// 			//				{
// 			//					_UpdateSkin( iArrayIndex );
// 			//				}
// 			//			}
// 			//		}
// 			//		else if( NULL != g_pSquareGame )
// 			//		{
// 			//			CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( m_OwnerUnitUID );
// 			//			if( NULL != pSquareUnit &&
// 			//				pSquareUnit->GetUnitViewer() )
// 			//			{
// 			//				if( false == pSquareUnit->GetUnitViewer()->IsFashionItemEquippedAt( m_pItemTemplet->GetEqipPosition() ) )
// 			//				{
// 			//					_UpdateSkin( iArrayIndex );
// 			//				}
// 			//			}
// 			//		}
// 			//	}
// 			//}
// 			//}} dmlee 2009.3.20 ĳ���۰� ED���� ���ÿ� �����ϰ� �ִ� ��쿡 ED���� _updateSkin�� ȣ������ �ʵ���
// 		}
//         return true;
//     }
//     else if( dwParam_ == CKTDXDeviceManager::THREAD_REQUEST_EQUIP_ATTACH_ANIM_NORMAL )
//     {
//         const int iArrayIndex = (int)dwParam2_;
//         ASSERT( iArrayIndex >= 0 && iArrayIndex < ARRAY_SIZE(m_ModelName) );
//         if( !(iArrayIndex >= 0 && iArrayIndex < ARRAY_SIZE(m_ModelName)) )
//             return true; // ������ ��� index�� ��忡 ���ؼ� retry���� �ʵ��� true�� �����Ѵ�.
// 
//         const int& i = iArrayIndex;
// 
// #ifdef _DEBUG // 2008-12-14
//         //std::string strFileName;
//         //ConvertWCHARToChar( strFileName, m_ModelName[i].c_str() );
//         //BREAK( 0 == stricmp("Mesh_Raven_Base_Weapon_Sorted.x",strFileName.c_str()) );
// #endif // _DEBUG // 2008-12-14
// 
//         ASSERT( sizeof(m_pNormalXSkinModel[i]) == sizeof(int) );
//         m_pNormalXSkinModel[i] = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[i] );
// 		ASSERT( NULL != m_pNormalXSkinModel[i] );
// 		if( NULL == m_pNormalXSkinModel[i] )
// 		{
// 			return true;
// 		}//if
// 		
// 		CKTDGXSkinAnim* pSkinAnim = NULL;
// 
//         ASSERT( sizeof(CKTDGXSkinAnim*) == sizeof(int) );
// 		pSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
// 		pSkinAnim->SetAnimXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET );
// 		pSkinAnim->AddModelXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET, m_pNormalMultiTexXET, m_pNormalTexChangeXET );
// 		pSkinAnim->ChangeAnim( m_AniName.c_str(), false );
// 		pSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
// 		pSkinAnim->UseDXMatrix( true );
// 		m_pNormalXSkinAnim[i] = pSkinAnim;
// 
// 	    if( m_bCanHyperMode == true )
// 	    {
// 			m_pHyperXSkinModel[i]		= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelNameHyper[i] );
// 
// 			ASSERT( NULL != m_pHyperXSkinModel[i] );
// 			if( NULL == m_pHyperXSkinModel[i] )
// 			{
// 				return true;
// 			}//if
// 
// 			pSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
// 			pSkinAnim->SetAnimXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET );
// 			pSkinAnim->AddModelXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET, m_pHyperMultiTexXET, m_pHyperTexChangeXET );
// 			pSkinAnim->ChangeAnim( m_AniNameHyper.c_str(), false );
// 			pSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
// 			pSkinAnim->UseDXMatrix( true );
// 			m_pHyperXSkinAnim[i] = pSkinAnim;
// 	    }//if
// 
// 		if( false == m_pItemTemplet->m_AttachFrameName[i].empty() )
// 		{
// 	        CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
// 			pFrame = m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->m_AttachFrameName[i].c_str() );
// 			if( NULL != pFrame )
// 			{
// 				m_pUnitMatrix[i] = &pFrame->combineMatrix;
// 			}//if
// 		}//if
// 
//         return true;
//     }//if.. else..
// 
//     ASSERT( !"�� ���� �߻����� �ʾƾ� �Ѵ�. - jintaeks on 2008-12-13, 18:26" );
//     return false;
// }//CX2Eqip::OnThreadLoad()
// //}} seojt // 2008-12-13, 19:01


void CX2Eqip::ChangeHyperMode()
{
    m_bHyperMode = true;

//{{ robobeg : 2008-10-21
	//switch( m_EqipType )
	//{
	//	case CX2Item::ET_SKIN:
	//		m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel );

	//		if( m_pUnitXSkinAnim->IsInModelXSkinMesh( m_pHyperXSkinModel ) == false )
	//			m_pUnitXSkinAnim->AddModelXSkinMesh( m_pHyperXSkinModel, m_pHyperAniXET, NULL, m_pHyperTexChangeXET );
	//		break;

	//	case CX2Item::ET_ATTACH_ANIM:
	//		break;

	//	case CX2Item::ET_ATTACH_NORMAL:
	//		break;
	//}
//}} robobeg : 2008-10-21
    _UpdateSkin();
}

void CX2Eqip::ChangeNormalMode()
{
	m_bHyperMode = false;

//{{ robobeg : 2008-10-21
	//switch( m_EqipType )
	//{
	//	case CX2Item::ET_SKIN:
	//		m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel );

	//		if( m_pUnitXSkinAnim->IsInModelXSkinMesh( m_pNormalXSkinModel ) == false )
	//			m_pUnitXSkinAnim->AddModelXSkinMesh( m_pNormalXSkinModel, m_pNormalAniXET, NULL, m_pNormalTexChangeXET );
	//		break;

	//	case CX2Item::ET_ATTACH_ANIM:
	//		break;

	//	case CX2Item::ET_ATTACH_NORMAL:
	//		break;
	//}
//}} robobeg : 2008-10-21
    _UpdateSkin();
}




// dmlee 2009-03-19 ��׶��� �ε� �����忡���� ȣ��Ǿ�� �Ѵ�.
// fix!!! GetShowObject()�Լ��� ȣ���Ҷ� ���������� m_pManagerObject�� �����ϴµ� dangling pointer�� �� �� �ִ�. 
// �̸� �ӽ÷� �ذ��ϱ� ���ؼ� m_pManagerObject�� �������� �ʵ��� �Ͽ���. 
// m_pManagerObject�� shared_pointer�� ����ų�, �ٸ� ���� ����� �ʿ��ϴ�. 
//{{ robobeg : 2008-10-21
void CX2Eqip::_UpdateSkin(int iIndex_)
{
#ifdef HEAVY_LOG_TEST
	
	debugWPrintf( L"[START] _UpdateSkin( %d )", iIndex_ );
	debugWPrintf( L"unit : %s", m_pOwnerUnit->GetNickName() );
	
	CX2Item* pItem = GetItem();
	debugWPrintf( L"item : %s, %d", pItem->GetFullName().c_str(), (int) GetEqipPos() );
		
#endif HEAVY_LOG_TEST

	LastErrorLog( m_ModelName[0].c_str() );
	
    if( iIndex_ < 0 || iIndex_ >= CX2Item::MAX_MODEL_COUNT_A_ITEM )
        return;

    const int& i = iIndex_;

    //CKTDGXSkinAnimPtr   m_pUnitXSkinAnim = m_ptrUnitXSkinAnim.lock();
    //ASSERT( m_pUnitXSkinAnim != NULL );
    //if( m_pUnitXSkinAnim == NULL )
    //    return;

	//{{ dmlee 2009.3.19 note!! GetShowObject()�Լ��� ȣ���Ҷ� ���������� m_pManagerObject�� �����ϴµ� dangling pointer�� �� �� �ִ�. �̸� �ӽ÷� �ذ��ϱ� ���ؼ� m_pManagerObject�� �������� �ʵ��� �Ͽ���. 
	//if ( m_EqipType != CX2Item::UT_SKIN || !GetShowObject() )
	if( m_EqipType != CX2Item::UT_SKIN || !GetShowObjectPrivate() )
	//}} dmlee 2009.3.19 note!! GetShowObject()�Լ��� ȣ���Ҷ� ���������� m_pManagerObject�� �����ϴµ� dangling pointer�� �� �� �ִ�. �̸� �ӽ÷� �ذ��ϱ� ���ؼ� m_pManagerObject�� �������� �ʵ��� �Ͽ���. 
	{
		switch( m_iXSkinModelAttachment )
		{
		case 1:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
            if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
			m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel[i] );
			break;
		case 2:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
            if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
			m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel[i] );
			break;
		}
		m_iXSkinModelAttachment = 0;
	}
	else
	{
		if ( m_bHyperMode && m_bCanHyperMode )
		{
			switch( m_iXSkinModelAttachment )
			{
			case 2:
				break;
			case 1:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
				m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel[i] );

#ifdef HEAVY_LOG_TEST
				debugWPrintf( L"remove normal skin : %s", m_pNormalXSkinModel[i]->GetDeviceID().c_str() );
#endif HEAVY_LOG_TEST

			default:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
					m_pUnitXSkinAnim->AddModelXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET, NULL, m_pHyperTexChangeXET, false, false );

				m_iXSkinModelAttachment = 2;

#ifdef HEAVY_LOG_TEST
				debugWPrintf( L"add hyper skin : %s", m_pHyperXSkinModel[i]->GetDeviceID().c_str() );
#endif HEAVY_LOG_TEST

			}
		}
		else
		{
			switch( m_iXSkinModelAttachment )
			{
			case 1:
				break;
			case 2:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
				m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel[i] );
#ifdef HEAVY_LOG_TEST
				debugWPrintf( L"remove hyper skin : %s", m_pHyperXSkinModel[i]->GetDeviceID().c_str() );
#endif HEAVY_LOG_TEST

			default:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
					m_pUnitXSkinAnim->AddModelXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET, NULL, m_pNormalTexChangeXET, false, false );

				if( NULL != m_pNormalXSkinModel[i] )
					m_iXSkinModelAttachment = 1;
				else
					m_iXSkinModelAttachment = 0;


#ifdef HEAVY_LOG_TEST
				if( m_pNormalXSkinModel[i] != NULL )
				{
					debugWPrintf( L"add normal skin : %s", m_pNormalXSkinModel[i]->GetDeviceID().c_str() );
				}
				else
				{
					debugWPrintf( L"add normal skin : NULL" );
				}
#endif HEAVY_LOG_TEST
			}
		}
	}//if.. else..



#ifdef HEAVY_LOG_TEST
	debugWPrintf( L"[END] _UpdateSkin( %d )", iIndex_ );
#endif HEAVY_LOG_TEST

}//CX2Eqip::_UpdateSkin()


void CX2Eqip::_UpdateSkin()
{

	KTDXPROFILE();


    //CKTDGXSkinAnimPtr   m_pUnitXSkinAnim = m_ptrUnitXSkinAnim.lock();
    //ASSERT( m_pUnitXSkinAnim != NULL );
    //if( m_pUnitXSkinAnim == NULL )
    //    return;



#ifdef HEAVY_LOG_TEST

	debugWPrintf( L"[START] _UpdateSkin()" );
	debugWPrintf( L"unit : %s", m_pOwnerUnit->GetNickName() );

	CX2Item* pItem = GetItem();
	if( pItem != NULL )
	{
		debugWPrintf( L"item : %s, %d", pItem->GetFullName().c_str(), (int) GetEqipPos() );
	}

#endif HEAVY_LOG_TEST

	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	{
		if ( m_EqipType != CX2Item::UT_SKIN || !GetShowObject() )
		{
			switch( m_iXSkinModelAttachment )
			{
			case 1:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
				m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel[i] );
				break;
			case 2:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
				m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel[i] );
				break;
			}
			m_iXSkinModelAttachment = 0;
		}
		else
		{
			if ( m_bHyperMode && m_bCanHyperMode )
			{
				switch( m_iXSkinModelAttachment )
				{
				case 2:
					break;
				case 1:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                    if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
					m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pNormalXSkinModel[i] );

#ifdef HEAVY_LOG_TEST
					debugWPrintf( L"REMOVE NORMAL SKIN : %s", m_pNormalXSkinModel[i]->GetDeviceID().c_str() );
#endif HEAVY_LOG_TEST
				default:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                    if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
					m_pUnitXSkinAnim->AddModelXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET, NULL, m_pHyperTexChangeXET );
					m_iXSkinModelAttachment = 2;
#ifdef HEAVY_LOG_TEST
					debugWPrintf( L"ADD HYPER SKIN : %s", m_pHyperXSkinModel[i]->GetDeviceID().c_str() );
#endif HEAVY_LOG_TEST
				}
			}
			else
			{
				switch( m_iXSkinModelAttachment )
				{
				case 1:
					break;
				case 2:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                    if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
					m_pUnitXSkinAnim->RemoveModelXSkinMesh( m_pHyperXSkinModel[i] );
#ifdef HEAVY_LOG_TEST
					debugWPrintf( L"REMOVE HYPER SKIN : %s", m_pHyperXSkinModel[i]->GetDeviceID().c_str() );
#endif HEAVY_LOG_TEST

				default:
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                    if ( m_pUnitXSkinAnim != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
					m_pUnitXSkinAnim->AddModelXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET, NULL, m_pNormalTexChangeXET );
					if( NULL != m_pNormalXSkinModel[i] )
						m_iXSkinModelAttachment = 1;
					else
						m_iXSkinModelAttachment = 0;

#ifdef HEAVY_LOG_TEST
					if( m_pNormalXSkinModel[i] != NULL )
					{
						debugWPrintf( L"ADD NORMAL SKIN : %s", m_pNormalXSkinModel[i]->GetDeviceID().c_str() );
					}
					else
					{
						debugWPrintf( L"ADD NORMAL SKIN : NULL" );
					}
#endif HEAVY_LOG_TEST
				}
			}
		}//if.. else..
	}// for



#ifdef HEAVY_LOG_TEST
	debugWPrintf( L"[END] _UpdateSkin()" );
#endif HEAVY_LOG_TEST

}//CX2Eqip::_UpdateSkin()


CX2Item* CX2Eqip::GetItem()
{
	if ( m_bDummyBasic == true)
	{
		return m_pDummyItem;
	}
	
	CX2Item* pItem = NULL; 
	if( m_pOwnerUnit != NULL )
	{
		pItem = m_pOwnerUnit->GetInventory().GetItem( m_ItemUID, true );
	}	
	
	return pItem;
}//CX2Eqip::GetItem()


//#ifdef EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13
void CX2Eqip::InitSkin()
{
	KTDXPROFILE();

	//background!!!
//     if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
//     {
// 	    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
// 	    {
//             if( m_ModelName[i].empty() )
//             {
//                 m_pNormalXSkinModel[i] = NULL;
//             }
//             else
//             {
//                 HRESULT hr = g_pKTDXApp->GetDeviceManager()->ThreadReq_OpenDevice(
//                     this, m_ModelName[i], CKTDXDeviceManager::THREAD_REQUEST_EQUIP_SKIN_NORMAL, i );
//                 ASSERT( SUCCEEDED(hr) );
//             }//if.. else..
// 	    }//for
// 
// 	    m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
// 	    m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );	
// 
// 	    if( m_bCanHyperMode == true )
// 	    {
// 		    m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
// 		    m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
//         }//if
// 
//     }
//     else
    {
	    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	    {
		    g_pMain->GetMemoryHolder()->LoadEquip( m_ModelName[i].c_str() );		// fix!! �� �κ� �ʿ��Ѱ�?

		    m_pNormalXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[i] );
	    }//for

	    m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
	    m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );	

	    if( m_bCanHyperMode == true )
	    {
		    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		    {
			    g_pMain->GetMemoryHolder()->LoadEquip( m_ModelNameHyper[i].c_str() );		// fix!! �� �κ� �ʿ��Ѱ�?

			    m_pHyperXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelNameHyper[i] );
		    }
    		
		    m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
		    m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
	    }//if

	    if ( m_pNormalXSkinModel[0] == NULL )
	    {
		    wstring errorMSG;
		    errorMSG = L"- �� �̸� : ";
		    errorMSG += m_ModelName[0];

		    ErrorLogMsg( XEM_ERROR93, errorMSG.c_str() );
		    ASSERT( !"CX2Eqip::InitSkin, null normal skin model" );

		    if ( m_bCanHyperMode == true && m_pHyperXSkinModel[0] == NULL )
		    {
			    wstring errorMSG2;
			    errorMSG2 = L"- �� �̸� : ";
			    errorMSG2 += m_ModelNameHyper[0];

			    ErrorLogMsg( XEM_ERROR93, errorMSG2.c_str() );
		    }//if

		    return;
	    }//if

        _UpdateSkin();
    }//if.. else..
}//CX2Eqip::InitSkin()
//#else // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13
////{{AFX
//void CX2Eqip::InitSkin()
//{
//	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
//	{
//		g_pMain->GetMemoryHolder()->LoadEquip( m_ModelName[i].c_str() );		// fix!! �� �κ� �ʿ��Ѱ�?
//
//		m_pNormalXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[i] );
//	}
//	m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
//	m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );	
//
//	if( m_bCanHyperMode == true )
//	{
//
//		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
//		{
//			g_pMain->GetMemoryHolder()->LoadEquip( m_ModelNameHyper[i].c_str() );		// fix!! �� �κ� �ʿ��Ѱ�?
//
//			m_pHyperXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelNameHyper[i] );
//		}
//		
//		m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
//		m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
//	}
//
//	if ( m_pNormalXSkinModel[0] == NULL )
//	{
//		wstring errorMSG;
//		errorMSG = L"- �� �̸� : ";
//		errorMSG += m_ModelName[0];
//
//		ErrorLogMsg( XEM_ERROR93, errorMSG.c_str() );
//		ASSERT( !"CX2Eqip::InitSkin, null normal skin model" );
//
//		if ( m_bCanHyperMode == true && m_pHyperXSkinModel[0] == NULL )
//		{
//			wstring errorMSG2;
//			errorMSG2 = L"- �� �̸� : ";
//			errorMSG2 += m_ModelNameHyper[0];
//
//			ErrorLogMsg( XEM_ERROR93, errorMSG2.c_str() );
//		}
//
//		return;
//	}
//
////{{ robobeg : 2008-10-21
//	//m_pUnitXSkinAnim->AddModelXSkinMesh( m_pNormalXSkinModel, m_pNormalAniXET, NULL, m_pNormalTexChangeXET );
//    _UpdateSkin();
////}} robobeg : 2008-10-21
//}//CX2Eqip::InitSkin()
////}}AFX
//#endif // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13


//#ifdef EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13
void CX2Eqip::InitAttatchAnim()
{
	KTDXPROFILE();

	m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
	m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );

	WCHAR wszMultiTexName[256] = L"";
	StringCchPrintfW( wszMultiTexName, ARRAY_SIZE(wszMultiTexName), L"MULTI_TEX_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );

	m_pNormalMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszMultiTexName, true );
	if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
	{		
		if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 11�� ���� ����Ʈ
			m_pNormalMultiTexXET->SetMultiTexStage2( "*.*", "11Gang_Protect.tga", D3DTOP_ADD );
#else ADD_UPGRADE_WEAPON_PARTICLE
			m_pNormalMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
#endif ADD_UPGRADE_WEAPON_PARTICLE
		else
			m_pNormalMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );	
	}//if
	// kimhc // 2009-12-30 // ��� ������ ��ȭ ��� �ƴϸ�
	else
	{
		if( m_pNormalMultiTexXET != NULL )
			m_pNormalMultiTexXET->DeleteMultiTexStage2();
	}

		//background!!!
//     if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
//     {
// 	    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
// 	    {
//             if( m_ModelName[i].empty() )
//             {
//                 m_pNormalXSkinModel[i] = NULL;
//             }
//             else
//             {
// 		        HRESULT hr = g_pKTDXApp->GetDeviceManager()->ThreadReq_OpenDevice(
//                     this, m_ModelName[i], CKTDXDeviceManager::THREAD_REQUEST_EQUIP_ATTACH_ANIM_NORMAL, i );
//                 ASSERT( SUCCEEDED(hr) );
//             }//if.. else..
// 	    }//for
// 
// 	    if( m_bCanHyperMode == true )
// 	    {
// 		    m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
// 		    m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
// 
// 		    WCHAR wszMultiTexName[256] = L"";
// 		    StringCchPrintfW( wszMultiTexName, ARRAY_SIZE(wszMultiTexName), L"MULTI_TEX_HYPER_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );
// 
// 		    m_pHyperMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszMultiTexName, true );
// 		    if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
// 		    {			
// 			    if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
// 				    m_pHyperMultiTexXET->SetMultiTexStage2_LUA( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
// 			    else
// 				    m_pHyperMultiTexXET->SetMultiTexStage2_LUA( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );			
// 		    }//if
// 	    }//if
//     }
//     else
    {
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		/// ������ ���� ��ġ
		CX2Unit::EQIP_POSITION	eEquipPosition	= m_pItemTemplet->GetEqipPosition();
#endif //SERV_9TH_NEW_CHARACTER

		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		{
			//g_pMain->GetMemoryHolder()->LoadEquip( m_ModelName[i].c_str() );		// fix!! �� �κ� �ʿ��Ѱ�?


#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			int iTargetIndex	= i;		/// ������ ����� �ε���

			/// �ֵ��� ������ ����, 0�� �ε����� ���⸦ �ٸ� �ε����� ��� ��������
			if( CX2Unit::UT_ADD == m_pItemTemplet->GetUnitType() &&  
				( CX2Unit::EP_WEAPON_HAND == eEquipPosition || CX2Unit::EP_AC_WEAPON == eEquipPosition ) )
				iTargetIndex = 0;

			m_pNormalXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[iTargetIndex] );
#else //SERV_9TH_NEW_CHARACTER
			m_pNormalXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[i] );
#endif //SERV_9TH_NEW_CHARACTER

			if( NULL == m_pNormalXSkinModel[i] )
			{
				//ASSERT( i != 0 );
				continue;
			}//if

			ASSERT( m_pNormalXSkinAnim[i] == NULL );
			SAFE_DELETE_KTDGOBJECT( m_pNormalXSkinAnim[i] );
			m_pNormalXSkinAnim[i] = CKTDGXSkinAnim::CreateSkinAnim();
			m_pNormalXSkinAnim[i]->SetAnimXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET );
			m_pNormalXSkinAnim[i]->AddModelXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET, m_pNormalMultiTexXET, m_pNormalTexChangeXET );
			m_pNormalXSkinAnim[i]->ChangeAnim( m_AniName.c_str(), false );
			m_pNormalXSkinAnim[i]->Play( CKTDGXSkinAnim::XAP_LOOP );
			m_pNormalXSkinAnim[i]->UseDXMatrix( true );

#ifdef JIGGLEBONE_TEST
			float fTension = GetJiggleBone();
			if( fTension >= 0.f)
			{
				m_pNormalXSkinAnim[i]->SetUnitXSkinAnim(m_pUnitXSkinAnim);
				m_pNormalXSkinAnim[i]->SetJiggleMesh(true, fTension);
			}
#endif
		}//for

	    if( m_bCanHyperMode == true )
	    {

		    m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
		    m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );

		    WCHAR wszMultiTexName[256] = L"";
		    StringCchPrintfW( wszMultiTexName, ARRAY_SIZE(wszMultiTexName), L"MULTI_TEX_HYPER_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );

		    m_pHyperMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszMultiTexName, true );
		    if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
		    {			
			    if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 11�� ���� ����Ʈ
				m_pHyperMultiTexXET->SetMultiTexStage2( "*.*", "11Gang_Protect.tga", D3DTOP_ADD );
#else ADD_UPGRADE_WEAPON_PARTICLE
				m_pHyperMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
#endif ADD_UPGRADE_WEAPON_PARTICLE
			    else
				    m_pHyperMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );			
		    }//if



		    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		    {
			    //g_pMain->GetMemoryHolder()->LoadEquip( m_ModelNameHyper[i].c_str() );

			    m_pHyperXSkinModel[i]		= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelNameHyper[i] );

			    if( NULL == m_pHyperXSkinModel[i] )
			    {
				    ASSERT( i != 0 );
				    continue;
			    }//if

				ASSERT( m_pHyperXSkinAnim[i] == NULL );
				SAFE_DELETE_KTDGOBJECT( m_pHyperXSkinAnim[i] );
			    m_pHyperXSkinAnim[i] = CKTDGXSkinAnim::CreateSkinAnim();
			    m_pHyperXSkinAnim[i]->SetAnimXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET );
			    m_pHyperXSkinAnim[i]->AddModelXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET, m_pHyperMultiTexXET, m_pHyperTexChangeXET );
			    m_pHyperXSkinAnim[i]->ChangeAnim( m_AniNameHyper.c_str(), false );
			    m_pHyperXSkinAnim[i]->Play( CKTDGXSkinAnim::XAP_LOOP );
			    m_pHyperXSkinAnim[i]->UseDXMatrix( true );

#ifdef JIGGLEBONE_TEST
				float fTension = GetJiggleBone();
				if( fTension >= 0.f)
				{
					m_pHyperXSkinAnim[i]->SetUnitXSkinAnim(m_pUnitXSkinAnim);
					m_pHyperXSkinAnim[i]->SetJiggleMesh(true, fTension, 4);

				}
#endif
		    }//for
	    }//if


        //CKTDGXSkinAnimPtr   m_pUnitXSkinAnim = m_ptrUnitXSkinAnim.lock();
        //if( m_pUnitXSkinAnim != NULL )
        {
	        CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
	        for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	        {
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
				/// �ֵ��� ���� �� ���� �Ǽ� �� ����, ������ ���� ���⸦ �ٿ�����.
				if( CX2Unit::UT_ADD == m_pItemTemplet->GetUnitType() &&  
					( CX2Unit::EP_WEAPON_HAND == eEquipPosition || CX2Unit::EP_AC_WEAPON == eEquipPosition ) )
				{
					WCHAR buf[256] = {0,};

					StringCchPrintf( buf, 255, L"Weapon0%d", i + 1 );	/// Weapon01 ~ Weapon06

					pFrame = m_pUnitXSkinAnim->GetCloneFrame( buf );

					if( NULL != pFrame )
						m_pUnitMatrix[i] = &pFrame->combineMatrix;
				}
				else
#endif //SERV_9TH_NEW_CHARACTER
				{
					if( m_pItemTemplet->GetAttachFrameName(i)[0] != NULL )
					{
			        	pFrame = 
	#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX          
                        ( m_pUnitXSkinAnim != NULL ) ? m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) ) : NULL;
	#else   X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                        m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) );
	#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
			        	if( NULL != pFrame )
			        	{
				        	m_pUnitMatrix[i] = &pFrame->combineMatrix;
						}//if
					}//if
		        }//if
	        }//for
        }//if
    }//if.. else..
}//CX2Eqip::InitAttatchAnim()

//#else // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13
//
////{{AFX
//void CX2Eqip::InitAttatchAnim()
//{
//	m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
//	m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );
//
//	WCHAR wszMultiTexName[256] = L"";
//	StringCchPrintfW( wszMultiTexName, ARRAY_SIZE(wszMultiTexName), L"MULTI_TEX_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );
//
//	m_pNormalMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszMultiTexName, true );
//	if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
//	{		
//		if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
//			m_pNormalMultiTexXET->SetMultiTexStage2_LUA( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
//		else
//			m_pNormalMultiTexXET->SetMultiTexStage2_LUA( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );	
//	}
//
//
//	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
//	{
//		g_pMain->GetMemoryHolder()->LoadEquip( m_ModelName[i].c_str() );		// fix!! �� �κ� �ʿ��Ѱ�?
//
//		m_pNormalXSkinModel[i]	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelName[i] );
//
//		if( NULL == m_pNormalXSkinModel[i] )
//		{
//			ASSERT( i != 0 );
//			continue;
//		}
//
//		m_pNormalXSkinAnim[i] = CKTDGXSkinAnim::CreateSkinAnim();
//		m_pNormalXSkinAnim[i]->SetAnimXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET );
//		m_pNormalXSkinAnim[i]->AddModelXSkinMesh( m_pNormalXSkinModel[i], m_pNormalAniXET, m_pNormalMultiTexXET, m_pNormalTexChangeXET );
//		m_pNormalXSkinAnim[i]->ChangeAnim( m_AniName.c_str(), false );
//		m_pNormalXSkinAnim[i]->Play( CKTDGXSkinAnim::XAP_LOOP );
//		m_pNormalXSkinAnim[i]->UseDXMatrix( true );
//	}
//
//
//
//	if( m_bCanHyperMode == true )
//	{
//
//		m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
//		m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
//
//		WCHAR wszMultiTexName[256] = L"";
//		StringCchPrintfW( wszMultiTexName, ARRAY_SIZE(wszMultiTexName), L"MULTI_TEX_HYPER_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );
//
//		m_pHyperMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszMultiTexName, true );
//		if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
//		{			
//			if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
//				m_pHyperMultiTexXET->SetMultiTexStage2_LUA( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
//			else
//				m_pHyperMultiTexXET->SetMultiTexStage2_LUA( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );			
//		}
//
//
//
//		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
//		{
//			g_pMain->GetMemoryHolder()->LoadEquip( m_ModelNameHyper[i].c_str() );
//
//			m_pHyperXSkinModel[i]		= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_ModelNameHyper[i] );
//
//			if( NULL == m_pHyperXSkinModel[i] )
//			{
//				ASSERT( i != 0 );
//				continue;
//			}
//
//			m_pHyperXSkinAnim[i] = CKTDGXSkinAnim::CreateSkinAnim();
//			m_pHyperXSkinAnim[i]->SetAnimXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET );
//			m_pHyperXSkinAnim[i]->AddModelXSkinMesh( m_pHyperXSkinModel[i], m_pHyperAniXET, m_pHyperMultiTexXET, m_pHyperTexChangeXET );
//			m_pHyperXSkinAnim[i]->ChangeAnim( m_AniNameHyper.c_str(), false );
//			m_pHyperXSkinAnim[i]->Play( CKTDGXSkinAnim::XAP_LOOP );
//			m_pHyperXSkinAnim[i]->UseDXMatrix( true );
//		}
//	}
//
//
//
//	CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
//	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
//	{
//        if( m_pItemTemplet->GetAttachFrameName(i)[0] != NULL )
//		{
//#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX  
//            pFrame = ( m_pUnitXSkinAnim != NULL ) ? m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) ) : NULL;
//#else   X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX  
//			pFrame = m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) );
//#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX  
//			if( NULL != pFrame )
//			{
//				m_pUnitMatrix[i] = &pFrame->combineMatrix;
//			}
//		}
//	}
//}//CX2Eqip::InitAttatchAnim()
////}}AFX
//#endif // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13


//#ifdef EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
void CX2Eqip::InitAttatchNormal()
{
	KTDXPROFILE();

    //if ( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
    {
	    m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
	    m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );
	    if( m_pNormalAniXET != NULL )
		    m_pNormalAniData	= m_pHyperAniXET->GetAniData( m_AniName.c_str() );

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		/// ������ ���� ��ġ
		CX2Unit::EQIP_POSITION	eEquipPosition	 = m_pItemTemplet->GetEqipPosition();
#endif //SERV_9TH_NEW_CHARACTER

		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		{
	#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			int iTargetIndex	= i;		/// ������ ����� �ε���

			/// �ֵ��� ������ ����, 0�� �ε����� ���⸦ �ٸ� �ε����� ��� ��������
			if( CX2Unit::UT_ADD == m_pItemTemplet->GetUnitType() && 
				( CX2Unit::EP_WEAPON_HAND ==  eEquipPosition || CX2Unit::EP_AC_WEAPON == eEquipPosition )  )
				iTargetIndex = 0;

			m_pNormalXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelName[iTargetIndex].c_str() );
	#else // SERV_9TH_NEW_CHARACTER
			m_pNormalXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelName[i].c_str() );
	#endif // SERV_9TH_NEW_CHARACTER
		}//for


	    if( m_bCanHyperMode == true )
	    {
		    m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
		    m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
		    if( m_pHyperAniXET != NULL )
			    m_pHyperAniData		= m_pHyperAniXET->GetAniData( m_AniNameHyper.c_str() );

			for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
			{
				m_pHyperXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelNameHyper[i] );
			}//for
	    }//if

		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
			for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
			{
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ

				/// �ֵ��� ���� �� ���� �Ǽ� �� ����, ������ ���� ���⸦ �ٿ�����.
				if( CX2Unit::UT_ADD == m_pItemTemplet->GetUnitType() &&  
					( CX2Unit::EP_WEAPON_HAND == eEquipPosition || CX2Unit::EP_AC_WEAPON == eEquipPosition ) )
				{
					WCHAR buf[256] = {0,};

					StringCchPrintf( buf, 255, L"Weapon0%d", i + 1 );	/// Weapon01 ~ Weapon06

					pFrame = ( m_pUnitXSkinAnim != NULL ) ? m_pUnitXSkinAnim->GetCloneFrame( buf ) : NULL;

					if( NULL != pFrame )
						m_pUnitMatrix[i] = &pFrame->combineMatrix;
				}
				else
	#endif // SERV_9TH_NEW_CHARACTER
				{
					if( m_pItemTemplet->GetAttachFrameName(i)[0] != NULL )
					{
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX  
						pFrame = ( m_pUnitXSkinAnim != NULL ) ? m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) ) : NULL;
#else   X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX  
						pFrame = m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) );
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX  
						if( NULL != pFrame )
						{
							m_pUnitMatrix[i] = &pFrame->combineMatrix;
						}//if
					}//if
				}//if
			}//for
		}//if
    }
	/*
	else
	{
		m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
		m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );
		if( m_pNormalAniXET != NULL )
			m_pNormalAniData	= m_pHyperAniXET->GetAniData( m_AniName.c_str() );


		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		{
		    m_pNormalXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelName[i] );
	    }//for


	    if( m_bCanHyperMode == true )
	    {
		    m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
		    m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
		    if( m_pHyperAniXET != NULL )
			    m_pHyperAniData		= m_pHyperAniXET->GetAniData( m_AniNameHyper.c_str() );


		    for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		    {
			    m_pHyperXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelNameHyper[i] );
		    }//for
	    }//if
    	

        //CKTDGXSkinAnimPtr   m_pUnitXSkinAnim = m_ptrUnitXSkinAnim.lock();
        //if( m_pUnitXSkinAnim != NULL )
        {
	        CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
	        for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
	        {
		        if( false == m_pItemTemplet->m_AttachFrameName[i].empty() )
		        {
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX 
                    pFrame = ( m_pUnitXSkinAnim != NULL ) ? m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->m_AttachFrameName[i].c_str() ) : NULL;
#else   X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX 
			        pFrame = m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->m_AttachFrameName[i].c_str() );
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX 
			        if( NULL != pFrame )
			        {
				        m_pUnitMatrix[i] = &pFrame->combineMatrix;
			        }//if
		        }//if
	        }//for
        }//if
    }//if.. else..
	*/
}//CX2Eqip::InitAttatchNormal()

//#else // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
//
////{{AFX
//void CX2Eqip::InitAttatchNormal()
//{
//	m_pNormalTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexName );
//	m_pNormalAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETName );
//	if( m_pNormalAniXET != NULL )
//		m_pNormalAniData	= m_pHyperAniXET->GetAniData( m_AniName.c_str() );
//
//
//	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
//	{
//		m_pNormalXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelName[i] );
//	}//for
//
//
//	if( m_bCanHyperMode == true )
//	{
//		m_pHyperTexChangeXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( m_ChangeTexNameHyper );
//		m_pHyperAniXET			= g_pKTDXApp->GetDeviceManager()->OpenXET( m_AniXETNameHyper );
//		if( m_pHyperAniXET != NULL )
//			m_pHyperAniData		= m_pHyperAniXET->GetAniData( m_AniNameHyper.c_str() );
//
//
//		for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
//		{
//			m_pHyperXMesh[i]			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( m_ModelNameHyper[i] );
//		}//for
//	}//if
//	
//
//	CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
//	for( int i=0; i< CX2Item::MAX_MODEL_COUNT_A_ITEM ; i++ )
//	{
//        if( m_pItemTemplet->GetAttachFrameName(i)[0] != NULL )
//		{
//#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX 
//            pFrame = ( m_pUnitXSkinAnim != NULL ) ? m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) ) : NULL;
//#else   X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX 
//			pFrame = m_pUnitXSkinAnim->GetCloneFrame( m_pItemTemplet->GetAttachFrameName(i) );
//#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX 
//			if( NULL != pFrame )
//			{
//				m_pUnitMatrix[i] = &pFrame->combineMatrix;
//			}//if
//		}//if
//	}//for
//
//}//CX2Eqip::InitAttatchNormal()
////}}AFX
//#endif // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14


// ĳ�� ���� �������� �� ED ���� ��ȭ ����Ʈ ���̰� ����� �ϱ� ���� �Լ�
void CX2Eqip::ChangeEnchantLevel( int level )
{
	m_EnchantLevel = level;

	if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
	{
		for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		{
			// �ϴ��� ���⸸ ó��~
			if( NULL != m_pNormalXSkinAnim[i] )
			{
				if( NULL == m_pNormalMultiTexXET )
				{
					WCHAR wszText[256];
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MULTI_TEX_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );
					m_pNormalMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszText, true );
					CKTDXDeviceXET* pOldXET = m_pNormalXSkinAnim[i]->ReplaceMultiTexXET( 0, m_pNormalMultiTexXET );
					SAFE_CLOSE( pOldXET );
				}
			}
		}

		if( NULL != m_pNormalMultiTexXET )
		{
			if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 11�� ���� ����Ʈ
			m_pNormalMultiTexXET->SetMultiTexStage2( "*.*", "11Gang_Protect.tga", D3DTOP_ADD );
#else ADD_UPGRADE_WEAPON_PARTICLE
			m_pNormalMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
#endif ADD_UPGRADE_WEAPON_PARTICLE
			else
				m_pNormalMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );	
		}	
	}
	else
	{
		if( m_pNormalMultiTexXET != NULL )
			m_pNormalMultiTexXET->DeleteMultiTexStage2();
	}

	if( m_bCanHyperMode == true )
	{
		if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_1 )
		{
			for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
			{
				// �ϴ��� ���⸸ ó��~
				if( NULL != m_pHyperXSkinAnim[i] )
				{
					if( NULL == m_pHyperMultiTexXET )
					{
						WCHAR wszText[256];
						StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MULTI_TEX_HYPER_%s_%dl", m_ModelName[0].c_str(), m_ItemUID );
						m_pHyperMultiTexXET = g_pKTDXApp->GetDeviceManager()->OpenXET( wszText, true );
						CKTDXDeviceXET* pOldXET = m_pHyperXSkinAnim[i]->ReplaceMultiTexXET( 0, m_pHyperMultiTexXET );
						SAFE_CLOSE( pOldXET );
					}
				}
			}

			if( NULL != m_pHyperMultiTexXET )
			{
				if( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )
#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 11�� ���� ����Ʈ
				m_pHyperMultiTexXET->SetMultiTexStage2( "*.*", "11Gang_Protect.tga", D3DTOP_ADD );
#else ADD_UPGRADE_WEAPON_PARTICLE
				m_pHyperMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect.tga", D3DTOP_ADD );
#endif ADD_UPGRADE_WEAPON_PARTICLE
				else
					m_pHyperMultiTexXET->SetMultiTexStage2( "*.*", "Gate_Protect_G01.tga", D3DTOP_ADD );	
			}	
		}
		else
		{
			if( m_pHyperMultiTexXET != NULL )
				m_pHyperMultiTexXET->DeleteMultiTexStage2();
		}
	}
}

void CX2Eqip::SetOwnerUnit( CX2Unit* pUnit ) 
{ 
	m_pOwnerUnit = pUnit; 
}





// �̺� ����ȭ �� �� ��� ���� ��ƼŬ �Ⱥ��̰� �ϱ� ����
void CX2Eqip::SetShowAttachedParticle( bool bShow )
{
	m_AttachedParticleGroup.SetShowObject( bShow );
}

void    CX2Eqip::AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const
{
	const WCHAR* name = NULL;

	name = GetChangeTexName();
	if( name != NULL && wcslen( name ) > 3 )
	{
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, name );
	}

	for( int j=0; j<CX2Item::MAX_MODEL_COUNT_A_ITEM; j++ )
	{
		name = GetModelName(j);
		if( name != NULL && wcslen( name ) > 3 )
		{
			switch( GetEqipType() )
			{

			case CX2Item::UT_SKIN:
				g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, name );
				break;

			case CX2Item::UT_ATTACH_ANIM:
				g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, name );
				break;

			case CX2Item::UT_ATTACH_NORMAL:
				g_pKTDXApp->GetDeviceManager()->AppendXMesh( listInOut_, name );
				break;
			}
		}
	} // for(j)
	name = GetAniXETName();
	if( name != NULL && wcslen( name ) > 3 )
	{
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, name );
	}

	if( CanHyperMode() == true )
	{
		name = GetChangeTexNameHyper();
		if( name != NULL && wcslen( name ) > 3 )
		{
			g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, name );
		}

		for( int j=0; j<CX2Item::MAX_MODEL_COUNT_A_ITEM; j++ )
		{
			name = GetModelNameHyper(j);
			if( name != NULL && wcslen( name ) > 3 )
			{
				switch( GetEqipType() )
				{
				case CX2Item::UT_SKIN:
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, name );
					break;

				case CX2Item::UT_ATTACH_ANIM:
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, name );
					break;

				case CX2Item::UT_ATTACH_NORMAL:
					g_pKTDXApp->GetDeviceManager()->AppendXMesh( listInOut_, name );
					break;
				}
			}
		} // for(j) 

		name = GetAniXETNameHyper();
		if( name != NULL && wcslen( name ) > 3 )
		{
			g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, name );
		}
	}

	switch( GetEqipType() )
	{
	case CX2Item::UT_SKIN:
#ifdef	ATTACH_SKINMESH
		AppendAttachedToDeviceList( listInOut_ );
#endif	ATTACH_SKINMESH
		break;
	case CX2Item::UT_ATTACH_ANIM:
		AppendAttachedToDeviceList( listInOut_ );
		break;
	case CX2Item::UT_ATTACH_NORMAL:
		break;
	}//switch
}//CX2Eqip::AppendToDeviceList()

void CX2Eqip::EquipResourceLoad() 
{
	switch( m_EqipType )
	{
	case CX2Item::UT_SKIN:
		InitSkin();
#ifdef ATTACH_SKINMESH
		if( false == m_bLoadedAttached )
		{
			LoadAttached();
		}
#endif
		break;

	case CX2Item::UT_ATTACH_ANIM:
		{
			InitAttatchAnim();
			if( false == m_bLoadedAttached )
			{
				LoadAttached();
			}

		} break;

	case CX2Item::UT_ATTACH_NORMAL:
		InitAttatchNormal();
		break;
	}

}

void	CX2Eqip::AppendAttachedToDeviceList( CKTDXDeviceDataList& listInOut_ ) const
{
	if ( m_pItemTemplet == NULL )
		return;


#ifdef SUB_EQUIP_TEST
    DWORD   dwNumMD = m_pItemTemplet->GetNumAttachedMeshData();
    bool    bSkinMesh = m_pItemTemplet->GetUseType() == CX2Item::UT_SKIN;
	for( UINT i=0; i<dwNumMD; i++ )
	{

        const wchar_t*  pwszMeshName = L"";
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        m_pItemTemplet->GetAttachedMeshData(i, &pwszMeshName);
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        const CX2Item::AttachedMeshData& data = m_pItemTemplet->GetAttachedMeshData(i);
        pwszMeshName = data.wstrMeshName.c_str();
        bSkinMesh = data.bSkinMesh;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		const wstring	wstrMeshName		= pwszMeshName;	/// ��Ʈ������ ���� ��ȯ
		bool			bSubEqipSkinMesh	= true;			/// �� �κ��� �Žð� ��Ų �Ž����� ����

		/// ��Ų �Žð� �ƴϸ�, �Ϲ� �Ž÷� ó���� �ش�. ( Item�� ATTATCHED_MESH ��� �� y ���� ����� ���� ���� ó�� )
		if( 'y' == wstrMeshName.back() || 'Y' == wstrMeshName.back() )
			bSubEqipSkinMesh = false;

		if ( true == bSubEqipSkinMesh && ( true == bSkinMesh || true == IsAttachedSkinMeshForAttachAnim( m_pItemTemplet->GetItemID() ) ) )
			g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pwszMeshName );
		else
			g_pKTDXApp->GetDeviceManager()->AppendXMesh( listInOut_, pwszMeshName );
	}//for
#endif SUB_EQUIP_TEST


#ifdef EQUIP_ATTACHED_PARTICLE_TEST

	// fix!! guuser�ʿ� weapon particle �ڵ�� ���ļ� �����ؾ���
	switch( m_EqipPos )
	{
	case CX2Unit::EP_AC_TITLE:			
	case CX2Unit::EP_AC_HAIR:			
	case CX2Unit::EP_AC_FACE1:			
	case CX2Unit::EP_AC_FACE2:			
	case CX2Unit::EP_AC_FACE3:			
	case CX2Unit::EP_AC_BODY:			
	case CX2Unit::EP_AC_LEG:			
	case CX2Unit::EP_AC_ARM:			
	case CX2Unit::EP_AC_RING:			
	case CX2Unit::EP_AC_NECKLESS:		
		{
			if( 
                NULL != m_pItemTemplet 
                && g_pData->GetUIMajorParticle() != NULL )
			{
                DWORD   dwNumParticles = m_pItemTemplet->GetNumAttachedParticleData();
				for( UINT j=0; j<dwNumParticles; j++ )
				{
                    const wchar_t*  pwszParticleName = L"";
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    m_pItemTemplet->GetAttachedParticleData( j, &pwszParticleName );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					const CX2Item::AttachedParticleData& particleData = m_pItemTemplet->m_vecAttachedParticleData[j];
                    pwszParticleName = particleData.wstrParticleName.c_str();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					g_pData->GetUIMajorParticle()->AppendToDeviceList( listInOut_,  pwszParticleName );
				}
			}

		} break;
	}

#endif EQUIP_ATTACHED_PARTICLE_TEST

}//CX2Eqip::AppendAttachedToDeviceList()

// note!! �ϴ� ���⿡�� UT_ATTACH_ANIM�� ��񿡸� ���� �� �ִ� ������ �����ϰ� ��������ϴ�. �׿� �ٸ� ��񿡴� �۵� ����
void CX2Eqip::LoadAttached()
{
	if( true == m_bLoadedAttached )
		return; 

#ifndef ATTACH_SKINMESH
	if( NULL == m_pNormalXSkinAnim[0] ) // fix!! �ϴ� 0��° �޽ÿ��� ���̴� ������
		return; 
#endif

	m_bLoadedAttached = true;



#ifdef SUB_EQUIP_TEST

    DWORD   dwNumMD = m_pItemTemplet->GetNumAttachedMeshData();
    bool bSkinMesh = m_pItemTemplet->GetUseType() == CX2Item::UT_SKIN;
    bool bJiggle = false;
	for( UINT i=0; i<dwNumMD; i++ )
	{
        const wchar_t* pwszMeshName = L"";
        const wchar_t* pwszBoneName = L"";

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        m_pItemTemplet->GetAttachedMeshData(i,&pwszMeshName,&pwszBoneName,&bJiggle);
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		const CX2Item::AttachedMeshData& data = m_pItemTemplet->m_vecAttachedMeshData[i];
        pwszMeshName = data.wstrMeshName.c_str();
        pwszBoneName = data.wstrBoneName.c_str();
        bSkinMesh = data.bSkinMesh;
        bJiggle = data.bJiggle;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef ATTACH_SKINMESH
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
		if( bSkinMesh == true )
		{
			if( m_pUnitXSkinAnim == NULL )
				continue;
            pFrame = m_pUnitXSkinAnim->GetCloneFrame( pwszBoneName );
		}
		else
		{
			if( m_pNormalXSkinAnim[0] == NULL )
				continue;

            pFrame = m_pNormalXSkinAnim[0]->GetCloneFrame( pwszBoneName );
			if( CX2Unit::UT_EVE == m_pItemTemplet->GetUnitType() )
			{
				if( i == 1 && NULL != m_pNormalXSkinAnim[1] )
				{
                    pFrame = m_pNormalXSkinAnim[1]->GetCloneFrame( pwszBoneName );
				}
			}
		}
#else
		// fix!! �ϴ��� �䷸�� ������ ù��° �޽ÿ��� �ٴ� ������ �ϵ��ڵ�
        CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pNormalXSkinAnim[0]->GetCloneFrame( pwszBoneName );
		if( CX2Unit::UT_EVE == m_pItemTemplet->GetUnitType() )
		{
			if( i == 1 && NULL != m_pNormalXSkinAnim[1] )
			{
                pFrame = m_pNormalXSkinAnim[1]->GetCloneFrame( pwszBoneName );
			}
		}
#endif


		if( NULL != pFrame )
		{
#ifdef ATTACH_SKINMESH
			CX2SubEquip* pSubEquip = NULL;

			if( true == IsAttachedSkinMeshForAttachAnim( m_pItemTemplet->GetItemID()))
				pSubEquip = CX2SubEquip::CreateSubEquip( &pFrame->combineMatrix, pwszMeshName, true );
			else
				pSubEquip = CX2SubEquip::CreateSubEquip( &pFrame->combineMatrix, pwszMeshName, bSkinMesh );

			if( pSubEquip != NULL && bJiggle == true )
			{				
				pSubEquip->SetUnitXSkinAnim(m_pUnitXSkinAnim);
				pSubEquip->SetJiggle(true, 3);
			}
#else
			CX2SubEquip* pSubEquip = CX2SubEquip::CreateSubEquip( &pFrame->combineMatrix, pwszMeshName);
#endif			
			m_vecpSubEquip.push_back( pSubEquip );

			pSubEquip->SetManagerObject( this );
			g_pKTDXApp->GetDGManager()->AddObjectChain( pSubEquip );			

			pSubEquip->SetLayer( GetLayer() );


			// fix!! �ϴ� �ϵ��ڵ� ���߿� item_templet���� �о�� �� �ְ� �߰��ؾ���
            switch( m_pItemTemplet->GetItemID() )
			{
			default:	
				{					
					if( true == IsAttachedSkinMeshForAttachAnim( m_pItemTemplet->GetItemID()))
					{
						CKTDGXRenderer::RenderParam* pRenderParam = pSubEquip->GetRenderParam();

						if( NULL == pRenderParam )
							return; 

						switch( m_pItemTemplet->GetItemID() )
						{
						default:
							{
								pSubEquip->SetAlphaObject( true );
								pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
								pRenderParam->bAlphaBlend = true;
								pRenderParam->srcBlend	= D3DBLEND_SRCALPHA;
								pRenderParam->destBlend = D3DBLEND_DESTALPHA;
								pRenderParam->cullMode = D3DCULL_NONE;
								pRenderParam->bZWriteEnable = false;
							} break;
						case 88622:	/// ����丣 ������ �Ƿ� �� ( ���ҵ� )
						case 88630:	/// ����丣 ������ �Ƿ� �� ( ���̻� )
						case 88638:	/// ����丣 ������ �Ƿ� �� ( ���� )
						case 88646:	/// ����丣 ������ �Ƿ� �� ( ���̺� )
						case 88654:	/// ����丣 ������ �Ƿ� �� ( �̺� )
						case 88662:	/// ����丣 ������ �Ƿ� �� ( û )
						case 88670:	/// ����丣 ������ �Ƿ� �� ( �ƶ� )
						case 88678: /// ����丣 ������ �Ƿ� �� ( �����ý� )
							{
								pSubEquip->SetAlphaObject( true );
								pRenderParam->renderType		= CKTDGXRenderer::RT_REAL_COLOR;
								pRenderParam->bAlphaBlend		= true;
								pRenderParam->bAlphaTestEnable	= true;
								pRenderParam->cullMode			= D3DCULL_NONE;
							} break;
						case 82035:
						case 82051:
							{
								pSubEquip->SetAlphaObject( true );
								pRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
								pRenderParam->bAlphaBlend	= true;
							} break;
						}
					}
					else
					{
						pSubEquip->SetAlphaObject( true );
						pSubEquip->GetNormalRenderParam().renderType = CKTDGXRenderer::RT_REAL_COLOR;
						pSubEquip->GetNormalRenderParam().bAlphaBlend = true;
						pSubEquip->GetNormalRenderParam().srcBlend	= D3DBLEND_SRCALPHA;
						pSubEquip->GetNormalRenderParam().destBlend = D3DBLEND_DESTALPHA;
					}
				} break;

			case 130892: // SF ����, ���ҵ�
			case 130893: // SF ����
			case 130894: // SF ����
			case 130895: // SF ����
#ifdef REAL_COLOR_SPECIAL_RENDER_ITEM_WITH_ALPHA			
			case 81290:	 // �ӱԼ� �Ϻ� �߰� ���ҵ� ��Ʋ ��Ʈ ĳ�� (û)
			case 88414: // �輮�� �Ϻ� �߰� ���ҵ� ��Ʋ â(�ƶ�)
#endif //REAL_COLOR_SPECIAL_RENDER_ITEM_WITH_ALPHA			
			// ����丣 �ַ���
			case 131646: 
			case 131647:
			case 131648:
			case 131649:
			case 131650:
			case 180052:
			case 88120:
			case 132157:
			case 132163:
			case 132169:
			case 132175:
			case 132181:
				{					
					pSubEquip->SetAlphaObject( true );
					pSubEquip->GetNormalRenderParam().renderType = CKTDGXRenderer::RT_REAL_COLOR;
					pSubEquip->GetNormalRenderParam().bAlphaBlend = true;
					pSubEquip->GetNormalRenderParam().srcBlend	= D3DBLEND_SRCALPHA;
					pSubEquip->GetNormalRenderParam().destBlend = D3DBLEND_INVSRCALPHA;
					pSubEquip->GetNormalRenderParam().bAlphaTestEnable = true;
				} break;

			case 130896: // SF ����, �̺�
				{					
					pSubEquip->GetNormalRenderParam().renderType = CKTDGXRenderer::RT_REAL_COLOR;
				} break;

			// UT_SKIN�� �ٴ� attach mesh
			case 132851: // �̺� Ʈ���̼�-������ �� �尩(���ҵ�)
			case 88335: // ���ҵ� ��Ʋ ��Ʈ MK 2 ����
			case 88343:
			case 88351:
			case 88359:
			case 88367:
			case 88375:
			case 88383:
			case 88448: // ���ҵ� ��Ʋ ��Ʈ MK 2 ���� [PC��]
			case 88456:
			case 88464:
			case 88472:
			case 88480:
			case 88488:
			case 88496:
			case 88504: // ���ҵ� ��Ʋ ��Ʈ MK-B ����
			case 88512:
			case 88520:
			case 88528:
			case 88536:
			case 88544:
			case 88552:
			case 88560:	// ���ҵ� ��Ʋ ��Ʈ MK 2 ���� (�Ⱓ��)
			case 88568:
			case 88576:
			case 88584:
			case 88592:
			case 88600:
			case 88608:
				{
					if( NULL != pSubEquip )
					{
						pSubEquip->SetAlphaObject( true );
						CKTDGXRenderer::RenderParam* pRenderParam = pSubEquip->GetRenderParam();
						if( NULL != pRenderParam )
						{
							pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
							pRenderParam->bAlphaBlend = true;
							pRenderParam->srcBlend	= D3DBLEND_SRCALPHA;
							pRenderParam->destBlend = D3DBLEND_DESTALPHA;
							pRenderParam->bZWriteEnable = false;
						}
					}
				} break;
			case 181495:
			case 181496:
				{
					pSubEquip->SetAlphaObject( true );
					pSubEquip->GetNormalRenderParam().renderType = CKTDGXRenderer::RT_REAL_COLOR;
					pSubEquip->GetNormalRenderParam().bAlphaBlend = true;
					pSubEquip->GetNormalRenderParam().srcBlend	= D3DBLEND_SRCALPHA;
					pSubEquip->GetNormalRenderParam().destBlend = D3DBLEND_DESTALPHA;
					pSubEquip->GetNormalRenderParam().cullMode = D3DCULL_NONE;
					pSubEquip->GetNormalRenderParam().bZWriteEnable = false;
				} break;
//#ifdef SERV_EVE_BATTLE_SERAPH
			case 41290: // ��Ʋ ������ ���θ�� �Ӹ�
				{
					CKTDGXRenderer::RenderParam* pRenderParam = pSubEquip->GetRenderParam();
					pSubEquip->SetAlphaObject( true );
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                    if ( pRenderParam != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                    {
					    pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
					    pRenderParam->bAlphaBlend = true;
					    pRenderParam->srcBlend	= D3DBLEND_SRCALPHA;
					    pRenderParam->destBlend = D3DBLEND_DESTALPHA;
					    pRenderParam->bZWriteEnable = false;
					    pRenderParam->cullMode = D3DCULL_NONE;
                    }
				} break;
			case 41150: // ��Ʋ ������ ���θ�� ����
			case 81836: /// ����丣 �̺��� �� ������
			case 81869: /// �۷��̼� �������� ũ����Ż ����(���ҵ�)
			case 81875: /// �۷��̼� �������� ũ����Ż ����(���̻�)
			case 81881: /// �۷��̼� �������� ũ����Ż ����(����)
			case 81887:	/// �۷��̼� �������� ũ����Ż ����(���̺�)
			case 81893: /// �۷��̼� �������� ũ����Ż ����(�̺�)
			case 81899:	/// �۷��̼� �������� ũ����Ż ����(û)
			case 81905:	/// �۷��̼� �������� ũ����Ż ����(�ƶ�)
			case 88222: /// �۷��̼� ������ ũ����Ż �ҵ�(���ҵ�) //PC��
			case 88228: /// �۷��̼� �������� ũ����Ż ������(���̻�)//PC��
			case 88234: /// �۷��̼� �������� ũ����Ż ����(����)//PC��
			case 88240: /// �۷��̼� ������ ũ����Ż ���̵�(���̺�)//PC��
			case 88246: /// �۷��̼� �������� ũ����Ż ���(�̺�)//PC��
			case 88252: /// �۷��̼� ������ ũ����Ż ĳ��(û)//PC��
			case 88258: /// �۷��̼� �������� ũ����Ż ���Ǿ�(�ƶ�)//PC��
				{					
					pSubEquip->SetAlphaObject( true );
					pSubEquip->GetNormalRenderParam().renderType = CKTDGXRenderer::RT_REAL_COLOR;
					pSubEquip->GetNormalRenderParam().bAlphaBlend = true;
					pSubEquip->GetNormalRenderParam().srcBlend	= D3DBLEND_SRCALPHA;
					pSubEquip->GetNormalRenderParam().destBlend = D3DBLEND_DESTALPHA;
					pSubEquip->GetNormalRenderParam().cullMode = D3DCULL_NONE;
					pSubEquip->GetNormalRenderParam().bZWriteEnable = true;
					pSubEquip->GetNormalRenderParam().bAlphaTestEnable = true;
				} break;
//#endif SERV_EVE_BATTLE_SERAPH

#ifdef ADDED_RELATIONSHIP_SYSTEM
			case 183207: /// ��ũ �巹�� ( ���̻� )
			case 183208: /// ��ũ �巹�� ( ���� )
			case 183210: /// ��ũ �巹�� ( �̺� )
			case 183212: /// ��ũ �巹�� ( �ƶ� )
			case 183213: /// ��ũ �巹�� ( �����ý� )
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			case 67006491: /// ��ũ �巹�� ( ���̻� )
			case 67006492: /// ��ũ �巹�� ( ���� )
			case 67006494: /// ��ũ �巹�� ( �̺� )
			case 67006496: /// ��ũ �巹�� ( �ƶ� )
			case 67006497: /// ��ũ �巹�� ( �����ý� )
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				{	
					pSubEquip->SetAlphaObject( true );
					if( true == pSubEquip->GetIsSkinMesh() )
					{
                        CKTDGXRenderer::RenderParam* pRenderParam = pSubEquip->GetRenderParam();
#ifdef  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                        if ( pRenderParam != NULL )
#endif  X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
                        {
						    pRenderParam->bAlphaBlend	= true;
						    pRenderParam->renderType		= CKTDGXRenderer::RT_REAL_COLOR;
						    pRenderParam->cullMode		= D3DCULL_NONE;
						    pRenderParam->bZWriteEnable	= false;
                        }
					}
					else
					{
						pSubEquip->GetNormalRenderParam().bAlphaBlend	= true;
						pSubEquip->GetNormalRenderParam().renderType		= CKTDGXRenderer::RT_REAL_COLOR;
						pSubEquip->GetNormalRenderParam().cullMode		= D3DCULL_NONE;
						pSubEquip->GetNormalRenderParam().bZWriteEnable	= false;
					}
				} break;
#endif ADDED_RELATIONSHIP_SYSTEM

			case 88615:	/// ����丣 ������ �ҵ� ( ���ҵ� )
			case 88623: /// ����丣 ������ �ϵ� ( ���̻� )
			case 88631: /// ����丣 ������ ���� ( ���� )
			case 88639: /// ����丣 ������ ���̵� ( ���̺� )
			case 88647: /// ����丣 ������ ��� ( �̺� )
			case 88655: /// ����丣 ������ ĳ�� ( û )
			case 88663: /// ����丣 ������ ���Ǿ� ( �ƶ� )
			case 88671: /// ����丣 ������ Ŭ���̸�� ( �����ý� )
				{					
					pSubEquip->SetAlphaObject( true );
					pSubEquip->GetNormalRenderParam().renderType	= CKTDGXRenderer::RT_REAL_COLOR;
					pSubEquip->GetNormalRenderParam().bAlphaBlend	= true;
					pSubEquip->GetNormalRenderParam().cullMode		= D3DCULL_NONE;
				} break;
			}
		}
	}

#endif SUB_EQUIP_TEST






#ifdef EQUIP_ATTACHED_PARTICLE_TEST

	// fix!! guuser�ʿ� weapon particle �ڵ�� ���ļ� �����ؾ���
	switch( m_EqipPos )
	{
	case CX2Unit::EP_AC_TITLE:			
	case CX2Unit::EP_AC_HAIR:			
	case CX2Unit::EP_AC_FACE1:			
	case CX2Unit::EP_AC_FACE2:			
	case CX2Unit::EP_AC_FACE3:			
	case CX2Unit::EP_AC_BODY:			
	case CX2Unit::EP_AC_LEG:			
	case CX2Unit::EP_AC_ARM:			
	case CX2Unit::EP_AC_RING:			
	case CX2Unit::EP_AC_NECKLESS:		
		{
			if( NULL != m_pItemTemplet )
			{
                DWORD   dwNumParticles = m_pItemTemplet->GetNumAttachedParticleData();
				for( UINT j=0; j<dwNumParticles; j++ )
				{
                    const wchar_t*  pwszParticleName = L"";
                    const wchar_t*  pwszBoneName = L"";
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    m_pItemTemplet->GetAttachedParticleData( j, &pwszParticleName, &pwszBoneName );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					const CX2Item::AttachedParticleData& particleData = m_pItemTemplet->GetAttachedParticleData(j);
                    pwszParticleName = particleData.wstrParticleName.c_str();
                    pwszBoneName = particleData.wstrBoneName.c_str();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					AttachParticle( true, pwszParticleName, pwszBoneName, D3DXVECTOR3(0, 0, 0 ) );
				}
			}

		} break;
	}
#endif EQUIP_ATTACHED_PARTICLE_TEST




	
}





#ifdef EQUIP_ATTACHED_PARTICLE_TEST


	void CX2Eqip::AttachParticle( bool bIsMajorParticle, const wstring& seqName, const wstring& boneName, D3DXVECTOR3 vOffsetPos )
	{
		if( true == seqName.empty() )
			return;

		if( true == boneName.empty() )
			return;

		if( NULL == g_pData->GetUIMajorParticle() ||
			NULL == g_pData->GetUIMinorParticle() )
			return;

		if( NULL == m_pNormalXSkinModel[0] )
			return;



		AttachedParticle* pAttachedParticle = new AttachedParticle;
		pAttachedParticle->m_bIsMajorParticle = bIsMajorParticle;
		pAttachedParticle->m_vOffsetPos = vOffsetPos;
		pAttachedParticle->m_wstrBoneName = boneName;


		if( true == bIsMajorParticle )
		{
			pAttachedParticle->m_hSeq = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  seqName.c_str(), D3DXVECTOR3( 0, 0, 0 ) );
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
            CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
            if ( pSeq != NULL )
                pSeq->SetRenderCullCheck( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		}
		else
		{
			pAttachedParticle->m_hSeq = g_pData->GetUIMinorParticle()->CreateSequenceHandle( NULL,  seqName.c_str(), D3DXVECTOR3( 0, 0, 0 ) );
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
            CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMinorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
            if ( pSeq != NULL )
                pSeq->SetRenderCullCheck( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		}


		if( INVALID_PARTICLE_SEQUENCE_HANDLE == pAttachedParticle->m_hSeq )
		{
			SAFE_DELETE( pAttachedParticle );
			return;
		}

		//CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
		//if( NULL != pSeq )
		//{
		//	pSeq->SetShowObject( GetShowObject() );
		//}

		string strBoneName = "";
		ConvertWCHARToChar( strBoneName, boneName );
		

		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pNormalXSkinModel[0]->FindFrame( strBoneName.c_str() );
		if( NULL == pFrame )
		{
			if( NULL != m_pNormalXSkinModel[1] )
			{
				pFrame = m_pNormalXSkinModel[1]->FindFrame( strBoneName.c_str() );
				if( NULL == pFrame )
				{
					SAFE_DELETE( pAttachedParticle );
					return;
				}
			}
			else
			{
				SAFE_DELETE( pAttachedParticle );
				return;
			}
		}
		pAttachedParticle->m_pAttachFrame = pFrame;


		m_AttachedParticleGroup.m_vecpAttachedParticle.push_back( pAttachedParticle );
	}


	
	//////////////////////////////////////////////////////////////////////////
	CX2Eqip::AttachedParticle::AttachedParticle()
	{
		m_pAttachFrame			= NULL;
		m_wstrBoneName			= L"";
		m_bIsMajorParticle		= true;
		m_vOffsetPos			= D3DXVECTOR3(0, 0, 0);
		m_hSeq					= INVALID_PARTICLE_SEQUENCE_HANDLE;
	}


	CX2Eqip::AttachedParticle::~AttachedParticle()
	{
		if( true == m_bIsMajorParticle )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeq );
		}
		else
		{
			g_pData->GetUIMinorParticle()->DestroyInstanceHandle( m_hSeq );
		}
	}



	//////////////////////////////////////////////////////////////////////////
	CX2Eqip::AttachedParticleGroup::AttachedParticleGroup()
	{

	}

	CX2Eqip::AttachedParticleGroup::~AttachedParticleGroup()
	{
		for( UINT i=0; i<m_vecpAttachedParticle.size(); i++ )
		{
			AttachedParticle* pAttachedParticle = m_vecpAttachedParticle[i];
			SAFE_DELETE( pAttachedParticle );
		}
		m_vecpAttachedParticle.clear();
	}

	void CX2Eqip::AttachedParticleGroup::OnFrameMove( double fTime, float fElapsedTime )
	{
		for( UINT i=0; i< m_vecpAttachedParticle.size(); i++ )
		{
			AttachedParticle* pAttachedParticle = m_vecpAttachedParticle[i];
			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

			if( true == pAttachedParticle->m_bIsMajorParticle )
			{
				pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
			}
			else
			{
				pSeq = g_pData->GetUIMinorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
			}

			if( NULL == pSeq )
			{
				m_vecpAttachedParticle.erase( m_vecpAttachedParticle.begin() + i );
				--i;
				continue;
			}

			// fix!! �ϴ��� ù��° �𵨿��� ���� �� �ִ� ������ ����
			// �ϴ� UT_ATTACH_ANIM Ÿ�Ը� ���� 
			D3DXVECTOR3 vAttachPos(0,0,0);

			//switch( m_EqipType )
			//{
			//case CX2Item::UT_SKIN:
			//	{
			//		// not yet!!
			//	} break;

			//case CX2Item::UT_ATTACH_ANIM:
			//	{
					if( NULL != pAttachedParticle->m_pAttachFrame )
					{
						GetFramePos( &vAttachPos, pAttachedParticle->m_pAttachFrame );
					}
			//	} break;

			//case CX2Item::UT_ATTACH_NORMAL:
			//	{
			//		// fix!! mesh�� frame�� ��ġ�� ��� ��������?
			//		// not yet!!

			//	} break;
			//}

			vAttachPos += pAttachedParticle->m_vOffsetPos;



#ifdef EQUIP_ATTACHED_PARTICLE_TRACE_TEST
			if ( true == pSeq->GetTrace() )
				pSeq->SetPosition( vAttachPos, true, true );
			else
				pSeq->SetPosition( vAttachPos, false, false );
#else EQUIP_ATTACHED_PARTICLE_TRACE_TEST
//{{AFX
			pSeq->SetPosition( vAttachPos );
//}}AFX
#endif EQUIP_ATTACHED_PARTICLE_TRACE_TEST
		}

	}

	void CX2Eqip::AttachedParticleGroup::SetShowObject( bool bShow )
	{
		for( UINT i=0; i< m_vecpAttachedParticle.size(); i++ )
		{
			AttachedParticle* pAttachedParticle = m_vecpAttachedParticle[i];
			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

			if( true == pAttachedParticle->m_bIsMajorParticle )
			{
				pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
			}
			else
			{
				pSeq = g_pData->GetUIMinorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
			}

			if( NULL != pSeq )
			{
				pSeq->SetShowObject( bShow );
			}
		}

	}
#endif EQUIP_ATTACHED_PARTICLE_TEST

void CX2Eqip::SetFixed()
{
	m_bFixed = true;

#ifdef SUB_EQUIP_TEST
	for(vector<CX2SubEquip*>::iterator it = m_vecpSubEquip.begin(); it<m_vecpSubEquip.end(); ++it)
	{
		CX2SubEquip* pSubEquip = *it;
		// DGManager Chain���� �� �׷����� �ϱ� ����. ���� Render �� �ش�.
		pSubEquip->SetShowObject(false);
	}
#endif


#ifdef EQUIP_ATTACHED_PARTICLE_TEST

	for(vector<AttachedParticle*>::iterator itParticle = m_AttachedParticleGroup.m_vecpAttachedParticle.begin();
		itParticle < m_AttachedParticleGroup.m_vecpAttachedParticle.end();
		++itParticle)
	{
		AttachedParticle* pAttachedParticle = *itParticle;
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

		if( true == pAttachedParticle->m_bIsMajorParticle )
		{
			pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
		}
		else
		{
			pSeq = g_pData->GetUIMinorParticle()->GetInstanceSequence( pAttachedParticle->m_hSeq );
		}
		// DGManager Chain���� �� �׷����� �ϱ� ����. ���� Render �� �ش�.
		if( pSeq != NULL )
		{
			pSeq->SetShowObject(false);
		}		
	}

#endif EQUIP_ATTACHED_PARTICLE_TEST
}

/** @function : ChangeAnim
	@brief : ����� �ִϸ��̼��� �����ϴ� �Լ�\
	@param : ������ �ִϸ��̼� ��(pwszAnimName_)
*/
void CX2Eqip::ChangeAnim( const WCHAR* pwszAnimName_, const bool bTransition_ )
{
	if ( CX2Item::UT_ATTACH_ANIM == m_EqipType )
	{
		for( int i = 0; i < CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
		{
			if ( NULL != m_pNormalXSkinAnim[i] )
				m_pNormalXSkinAnim[i]->ChangeAnim( pwszAnimName_, bTransition_ );

			if ( m_bCanHyperMode && NULL != m_pHyperXSkinAnim[i] )
				m_pHyperXSkinAnim[i]->ChangeAnim( pwszAnimName_, bTransition_ );
		}
	}
}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE
float CX2Eqip::GetUpgradeWeaponUVAnimatonSpeed()		/// ��ȭ ������ UV �ִϸ��̼� �ӵ�
{
	if( ENCHANT_WEAPON_EFFECT_LEVEL_2 <= m_EnchantLevel )	/// 11�� �̻�
		return 0.75f;
	else
		return 1.f;
}
#endif ADD_UPGRADE_WEAPON_PARTICLE