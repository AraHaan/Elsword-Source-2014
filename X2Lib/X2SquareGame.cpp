#include "StdAfx.h"
#include ".\x2squaregame.h"

CX2SquareGame::CX2SquareGame( SquareData* pSquareData, CX2World::WORLD_ID worldID )
{
	g_pSquareGame			= this;

	m_SquareData.CopySquareData( pSquareData );
	m_pWorld	= NULL;
	m_pCamera	= NULL;

	m_pMyUnit.reset();

	m_pMajorParticle		= NULL;
	m_pMinorParticle		= NULL;
	m_pMajorXMeshPlayer		= NULL;
	m_pMinorXMeshPlayer		= NULL;
	m_pFontForUnitName		= NULL;

	m_MaxCount				= m_SquareData.m_MaxUser;
	m_fMaxCountCheckTime	= 1.0f;
	m_fMaxCountRenderTime	= 0;
	m_fReserveTime			= 0.0f;
	m_fSortTime				= 0.0f;
	m_fP2PSendGap			= 0.0f;

	m_SendSizePerSec		= 0;
	m_fSendGap				= 0.0f;
	m_fMarketTime			= 0.0f;

	m_bFreeCamera			= false;


	m_pTalkBoxMgrForPersonalShop	= CX2TalkBoxManager::CreateTalkBoxManger( CX2TalkBoxManager::TBMT_PERSONAL_SHOP );
	m_pTalkBoxManager				= CX2TalkBoxManager::CreateTalkBoxManger( CX2TalkBoxManager::TBMT_DEFAULT );


	
	/** @NOTE:

        ��׶��� �ε��� ����Ϸ��� ���带 ������ ��, CreateWorld()�� �ƴ�,
        ThreadRequest_CreateWorld()�� �����ؾ� �Ѵ�.
        ������, ������ ���ؼ��� urgent data�� ó���ϴ� �κ��� �������� �ʾұ�
        ������, ���� �����Ŀ� �ε��Ǵ� ���ҽ����� ���� ���̰� �� ���̴�.

        urgent data:
            ù ȭ�鿡�� �������� ��� �����͸� �ǹ��Ѵ�. ������ ��� ������ ó�� �������� ��
            �������� ��� �����͸� �ǹ��Ѵ�.
            
        - jintaeks on 2008-10-24, 10:31 */

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-18

    /** ���忡 ���� thread �ε� �׽�Ʈ�� �ϰ� �ִ�. - jintaeks on 2008-10-18, 13:54
        ���忡 ���� thread �׽�Ʈ�� ���������� �Ϸ�Ǿ����Ƿ�, BACKGROUND_LOADING_TEST����
        ����� �����ص� ����. - jintaeks on 2008-12-14, 18:30 */
		//background!!!
	m_pWorld = g_pData->GetWorldManager()->CreateWorld( worldID, NULL, true );

//#else // BACKGROUND_LOADING_TEST // 2008-10-18
//
//    /** EnableWritingPreprocessingData()�� �ǹ�:
//        ��׶��� �ε��� �ϱ� ���ؼ��� ���� ��ü�� ��ó���� ������ �ʿ��ϴ�.
//        ���� - jintaeks on 2008-10-24, 10:34 - �� ��� �� ������, bounding sphere�� ������
//        �� �޽��� local �߽� ��ǥ�̴�. �� ������ "��ũ��Ʈ�����̸�.ppd"�� �����Ϸ���
//
//            EnableWritingPreprocessingData( true );
//
//        �� �����ϸ� �ȴ�.
//        �� �κ��� ���ӿ� �������� ����, ������ ���� �ۼ��Ǿ�� �� ���̴�.
//
//        - jintaeks on 2008-10-24, 10:34 */
//
//	//g_pData->GetWorldManager()->EnableWritingPreprocessingData( true );
//	m_pWorld = g_pData->GetWorldManager()->CreateWorld( worldID );
//    //g_pData->GetWorldManager()->EnableWritingPreprocessingData( false );
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-18

	m_pCamera		= new CX2Camera();
	if( m_pWorld != NULL )
	{
		m_pWorld->PlayBGM();
		m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	}
	//m_pCamera->NomalDirectCamera( m_optrFocusUnit );


	srand((unsigned int)time(NULL));

	m_pMajorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 2
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK 
        );
	m_pMinorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 3
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
	m_pMajorXMeshPlayer	= new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        2
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
    ASSERT( m_pMajorXMeshPlayer != NULL );
	m_pMinorXMeshPlayer	= new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        3
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
    ASSERT( m_pMinorXMeshPlayer != NULL );

	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle2.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle3.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle4.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle5.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle6.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle7.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle8.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle9.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle10.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle11.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle12.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle13.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle14.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle15.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle16.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle17.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle18.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle19.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle20.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle21.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle22.txt" );
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle23.txt" );

	m_pMinorParticle->OpenScriptFile( L"GameMinorParticle.txt" );

	m_pMajorXMeshPlayer->OpenScriptFile( L"GameMajorXMeshPlayer.txt" );
	m_pMinorXMeshPlayer->OpenScriptFile( L"GameMinorXMeshPlayer.txt" );

#ifdef INT_RESOURCE_DEVIDE
	m_pMajorParticle->OpenScriptFile( L"GameMajorParticle_INT.txt" );
	m_pMinorParticle->OpenScriptFile( L"GameMinorParticle_INT.txt" );
	m_pMajorXMeshPlayer->OpenScriptFile( L"GameMajorXMeshPlayer_INT.txt" );
	//m_pMinorXMeshPlayer->OpenScriptFile( L"GameMinorXMeshPlayer_INT.txt" ); ���� ���� ����
#endif INT_RESOURCE_DEVIDE

	m_pFontForUnitName = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );

	m_bEnableKeyProcess		= true;
	m_bEnableCameraProcess	= true;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	g_pData->GetGameUDP()->ClearPeer();
//
//	g_pData->GetGameUDP()->SetMyUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
//	g_pData->GetGameUDP()->SetRelayIP( m_SquareData.m_RelayIP.c_str() );
//	g_pData->GetGameUDP()->SetRelayPort( m_SquareData.m_Port );
//	g_pData->GetGameUDP()->ConnectTestToRelay();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	g_pKTDXApp->GetDIManager()->SetEnable( true );




	if( NULL == g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap() )
	{
		g_pInstanceData->GetMiniMapUI()->CreateDungeonMiniMap();
	}

	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );
		g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->SetEyeDistance( 4000.f );
		g_pInstanceData->GetMiniMapUI()->SetTitle( m_SquareData.m_SquareName.c_str() );
	}
	

	m_bLeaveSquareGame = false;

	m_fTimeCheckShopTalkBox = 0.0f;

#if 0 // ���ǽ������ΰ�� �ŷ����忡 �����Ұ�� ������������ ������ �����Ƿ� ������
	if ( g_pData->GetMyUser()->GetUserData().hackingUserType == CX2User::HUT_AGREE_HACK_USER )
	{
#ifndef PROCESSLIST		
		g_pMain->UpdateProcessList();		
#endif		
		g_pMain->SendHackMail();
	}
#endif

	//m_pOutAreaParticle = NULL;
	m_hMarketOutArea = GetMajorParticle()->CreateSequenceHandle( NULL,  L"MarketOutArea", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMarket	= GetMajorParticle()->GetInstanceSequence( m_hMarketOutArea );
	if( pSeqMarket != NULL )
	{		
		//if( m_pOutAreaParticle == NULL )
		//{
			//m_pOutAreaParticle = 
                pSeqMarket->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		//}
		pSeqMarket->SetShowObject(false);
	}

	g_pKTDXApp->SkipFrame();

//	//{{ kimhc // 2010.3.10 //	���� ���⵵
//#ifdef	IGA_TEST
//	CX2IGA::GetInstance()->CreateIgaInfoInMarket();
//	CX2IGA::GetInstance()->Start();
//#endif	IGA_TEST
//	//}} kimhc // 2010.3.10 //	���� ���⵵

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pMain->GetPartyUI() != NULL )
	{
		g_pMain->GetPartyUI()->OpenPartyMenu(false);
		g_pMain->GetPartyUI()->OpenPartyDLG(false);

		g_pData->GetPartyManager()->SetProcessPvpMatch(false);
#ifdef REFORM_QUEST
		g_pData->GetPartyManager()->SetProcessDungeonMatch(false);
#endif //REFORM_QUEST
	}
#endif
}

CX2SquareGame::~CX2SquareGame(void)
{
//	//{{ kimhc // 2010.3.17 // ���� ���⵵ 
//#ifdef	IGA_TEST
//	CX2IGA::GetInstance()->End();
//#endif	IGA_TEST
//	//}} kimhc // 2010.3.17 // ���� ���⵵

	m_pFontForUnitName = NULL;

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );
	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxMgrForPersonalShop );



	SAFE_DELETE( m_pWorld );
	SAFE_DELETE( m_pCamera );

	m_pMyUnit.reset();
	m_UserUnitList.clear();


	GetMajorParticle()->DestroyInstanceHandle( m_hMarketOutArea );

	SAFE_DELETE( m_pMajorParticle );
	SAFE_DELETE( m_pMinorParticle );
	SAFE_DELETE( m_pMajorXMeshPlayer );
	SAFE_DELETE( m_pMinorXMeshPlayer );

	
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	g_pData->GetGameUDP()->DisconnectToRelay();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	g_pKTDXApp->GetDIManager()->SetEnable( true );

	g_pSquareGame = NULL;
}

void CX2SquareGame::JoinSquareUnit( KSquareUserInfo* pKSquareUserInfo )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( pKSquareUserInfo->m_iUnitUID );
	if( pCX2SquareUnit != NULL )
		return;

	if( pKSquareUserInfo->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
//#ifdef EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
		//background!!!
		pCX2SquareUnit = AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
//#else // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
//		pCX2SquareUnit = AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
//#endif // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
	}
	else
	{			
		CX2Unit* pCX2Unit = new CX2Unit( *pKSquareUserInfo );
		//���� ���� ������Ʈ
		pCX2Unit->ResetEqip();
//#ifdef EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
		//background!!!
        pCX2SquareUnit = AddUnit( pCX2Unit, false, false );
//#else // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
//		pCX2SquareUnit = AddUnit( pCX2Unit, false, false );
//#endif // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-14
	}

	if ( pCX2SquareUnit != NULL )
	{
		pCX2SquareUnit->SetPersonalShopName( pKSquareUserInfo->m_wstrPersonalShopName.c_str() );
		pCX2SquareUnit->SetPersonalShopState( (CX2SquareUnit::PERSONAL_SHOP_STATE)pKSquareUserInfo->m_cPersonalShopState );

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		pCX2SquareUnit->SetShopType( static_cast<SEnum::AGENCY_SHOP_TYPE>( pKSquareUserInfo->m_cPersonalShopType ) );
#else // SERV_UPGRADE_TRADE_SYSTEM
		pCX2SquareUnit->SetShopType( (CX2SquareUnit::PERSONAL_SHOP_TYPE) pKSquareUserInfo->m_cPersonalShopType );
#endif // SERV_UPGRADE_TRADE_SYSTEM
	}

#ifdef DISABLE_CHANNEL_CHANGE_IN_SQUARE
	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		g_pInstanceData->GetMiniMapUI()->SetEnableChannelComboBox(false);
	}
#endif //DISABLE_CHANNEL_CHANGE_IN_SQUARE
}

CX2SquareUnit* CX2SquareGame::AddUnit( CX2Unit* pUnit, bool bMyUnit, bool bInit )
{
	// note!! ���� �߰��Ǹ� ����, Ȥ�� unit_type����
#ifdef RIDING_SYSTEM
	CX2SquareUnitPtr pCX2SquareUnit = CX2SquareUnit::CreateSquareUnit( bMyUnit, pUnit,
		CX2SquareUnit::GetSquareUnitScriptFileName( pUnit->GetClass() ) );
#else //RIDING_SYSTEM
	CX2SquareUnitPtr pCX2SquareUnit = CX2SquareUnit::CreateSquareUnit( bMyUnit, pUnit, CX2SquareUnit::GetSquareUnitScriptFileName( pUnit->GetClass() ).c_str() );
#endif //RIDING_SYSTEM
	
	if( bInit == true )
		pCX2SquareUnit->Init();

//#ifdef TITLE_SYSTEM
    if( bInit == false )
        pCX2SquareUnit->UpdateEquippedEmblem();
//#endif

	pCX2SquareUnit->StateChange( pCX2SquareUnit->GetStateID().m_Wait  );

	m_UserUnitList.push_back( pCX2SquareUnit );
	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2SquareUnit.get() );

	if( bMyUnit == true )
	{
		m_pMyUnit = pCX2SquareUnit;

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-23
        //{{ seojt // 2008-10-22, 16:48
        /** �� unit�� ��ġ�� �����ǰ� ����, �� unit�� ����Ű��
            view matrix�� �����ϰ�, viewing frustum�� �����Ѵ�.
            �� unit �Ӹ� �ƴ϶�, �ٸ� ��� ���� ��ü�� ������ ������ ���Ŀ�
            urgent data�� ���� culling�� �����ؾ� ���� ��Ȯ�ϰ� �ϱ� ���Ͽ�
            �� �Լ��� ���ο��� urgent data�� �ε����� �ʴ´�.
            - jintaeks on 2008-10-22, 16:49 */

        m_pMyUnit->OnFrameMove( g_NowTime, 0.f );
        // ī�޶� ��Ʈ������ �����Ѵ�. m_pMyUnit�� �Կ��ϴ� ī�޶� frustum�� ����
        // culling�ϱ� ���� ���̴�.
        m_pCamera->SquareCameraUpdate( m_pMyUnit.get(), 1200.0f, 200.0f, 0.0f, 0.0f, 0.0f );

        /** @NOTE   �̰����� �Ʒ�ó�� LoadUrgentData()�� ȣ���ؼ��� �ȵȴ�.
                    ���� ��� ���� ������Ʈ�� Parsing�� ���� ���� Ȯ���� ���� �ʾҴ�.
                    LoadUrgentData()�� CX2StateSquareGame�� �����ڿ��� ȣ���Ѵ�.

                    g_pKTDXApp->GetDeviceManager()->LoadUrgentData( fTimeStamp );

                    - jintaeks on 2008-10-23, 15:57 */

        //}} seojt // 2008-10-22, 16:48
//#endif // BACKGROUND_LOADING_TEST // 2008-10-23
	}//if

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	CX2Unit::UnitData* pUnitData = pUnit->GetUnitData();
//	if( pUnitData != NULL )
//	{
//		if( g_pData->GetMyUser()->GetUID() == pUnit->GetOwnerUserUID() )
//		{
//			g_pData->GetGameUDP()->SetMyUID( pUnitData->m_UnitUID );
//			g_pData->GetGameUDP()->AddPeer( pUnitData->m_UnitUID, g_pData->GetGameUDP()->GetMyIP(), g_pData->GetGameUDP()->GetMyExtPort() );
//		}
//		else
//		{
//			g_pData->GetGameUDP()->AddPeer( pUnitData->m_UnitUID, pUnitData->m_IP.c_str(), pUnitData->m_Port );
//		}
//	}
//	g_pData->GetGameUDP()->ConnectTestToPeer();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	return pCX2SquareUnit.get();
}

void CX2SquareGame::RemoveUnit( UidType unitUID )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetUID() == unitUID )
		{
			//CX2SquareUnit::DeleteKTDGObject( pCX2SquareUnit );
				 
			m_UserUnitList.erase( m_UserUnitList.begin() + i );
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			g_pData->GetGameUDP()->RemovePeer( unitUID );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			break;
		}
	}
}
bool CX2SquareGame::InExitArea()
{
	float fDistance = 0.f;
	fDistance = GetDistance(m_pMyUnit->GetPos(), D3DXVECTOR3(1995.88f, 0.f, 0.f));

	if(fDistance <= 228.f)
		return true;
	
	return false;
}

bool CX2SquareGame::ExitMarket()
{
	if(InExitArea() == true)
		return true;

	return false;
}

HRESULT	CX2SquareGame::OnFrameMove( double fTime, float fElapsedTime )
{

	// �����ⱸ	

//	//{{ kimhc // 2010.3.17 // ���� ���⵵ 
//#ifdef	IGA_TEST
//	CX2IGA::GetInstance()->ProcessTracking( m_pCamera, fElapsedTime );
//#endif	IGA_TEST
//	//}} kimhc // 2010.3.17 // ���� ���⵵

	m_fMarketTime += fElapsedTime;
	if(InExitArea() == true)
	{				
		// �ⱸǥ��	
		D3DXVECTOR3 vLinkPos = D3DXVECTOR3(1995.88f, 300.f, 0.f);		

		D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( vLinkPos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq	= GetMajorParticle()->GetInstanceSequence( m_hMarketOutArea );
		if( NULL != pSeq )
		{
			if(m_fMarketTime <= 0.5f)
			{
				pSeq->ChangeTexForce(L"Get_Out_Up.dds");
			}
			else if(m_fMarketTime <= 1.f)
			{
				pSeq->ChangeTexForce(L"Get_Out_Down.dds");
			}
			else
			{
				m_fMarketTime = 0.f;
				pSeq->ChangeTexForce(L"Get_Out_Up.dds");
			}

			pSeq->SetPosition( D3DXVECTOR3( finalPos.x, finalPos.y, 0 ) );						
			pSeq->SetShowObject(true);
		}
	}
	else
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq	= GetMajorParticle()->GetInstanceSequence( m_hMarketOutArea );										
		if( NULL != pSeq )
		{
			pSeq->SetShowObject(false);
		}
	}

	m_fReserveTime -= fElapsedTime;
	if( m_fReserveTime < 0.0f )
	{
		m_fReserveTime = 0.0f;
		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];	
			if( pCX2SquareUnit->GetInit() == false )
			{	
				pCX2SquareUnit->Init();

				//if( m_pMyUnit != NULL )
				//{
				//	KXPT_SQUARE_UNIT_SYNC_REQ kXPT_SQUARE_UNIT_SYNC_REQ;
				//	kXPT_SQUARE_UNIT_SYNC_REQ.m_UnitUID = m_pMyUnit->GetUID();
				//	KSerBuffer buff;
				//	Serialize( &buff, &kXPT_SQUARE_UNIT_SYNC_REQ );
				//	g_pData->GetGameUDP()->Send( pCX2SquareUnit->GetUID(), (const char)XPT_SQUARE_UNIT_SYNC_REQ, (const char*)buff.GetData(), (int)buff.GetLength() );
				//}				
				m_fReserveTime = 0.5f;
				break;
			}
		}
	}
	m_fSendGap -= fElapsedTime;
	if( m_fSendGap < 0.0f )
	{
		m_fSendGap = 1.0f;
		m_SendSizePerSec = 0;
	}

	m_pMajorParticle->OnFrameMove( fTime, fElapsedTime );
	m_pMinorParticle->OnFrameMove( fTime, fElapsedTime );
	m_pMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
	m_pMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	//����
	m_fSortTime -= fElapsedTime;
	if( m_fSortTime < 0.0f )
	{
		m_fSortTime = 1.0f;
		CX2SquareUnitPtr pCX2SquareUnit;
		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			bool bSwapped = false;

			for( int j = 0; j < (int)m_UserUnitList.size() - i - 1; j++ )
			{		
				if( GetDistance( m_pMyUnit->GetPos(), m_UserUnitList[j]->GetPos() ) > GetDistance( m_pMyUnit->GetPos(), m_UserUnitList[j+1]->GetPos() ) )
				{
					pCX2SquareUnit = m_UserUnitList[j];
					m_UserUnitList[j] = m_UserUnitList[j+1];
					m_UserUnitList[j+1] = pCX2SquareUnit;
					bSwapped = true;
				}
			}

			if( bSwapped == false )
			{
				break;
			}
		}
	}
	
	if( GetEnableKey() == true )
	{
		if( false == m_bFreeCamera )
		{
				KeyProcess();
		}
	}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	P2PPacketHandler();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	m_pWorld->OnFrameMove( fTime, fElapsedTime );

	int count = 0;
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit->GetInit() == true )
		{
			if( GetDistance( m_pMyUnit->GetPos(), pCX2SquareUnit->GetPos() ) > 1000 )
			{
				//pCX2SquareUnit->SetShowObject( false );
				pCX2SquareUnit->SetPlanRender( true );
				pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );
			}
			else
			{
				if( count > m_MaxCount )
				{
					//pCX2SquareUnit->SetShowObject( false );
					pCX2SquareUnit->SetPlanRender( true );
					pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );
				}
				else
				{
					pCX2SquareUnit->SetShowObject( true );
					if(g_pMain->GetGameOption().GetFieldSD())
					{
						// SD ���
						pCX2SquareUnit->SetPlanRender( true );
					}
					else
					{
						// �Ϲ� ���						
						pCX2SquareUnit->SetPlanRender( false );
					}			

					pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );
					//pCX2SquareUnit->SetShowObject( true );
					//pCX2SquareUnit->SetPlanRender( false );
					count++;
				}			
			}	
		}
	}

	m_fMaxCountCheckTime -= fElapsedTime;
	if( m_fMaxCountCheckTime < 0.0f )
	{
		m_fMaxCountCheckTime = 1.0f;
		if( m_fMaxCountRenderTime < g_pKTDXApp->GetRenderCount() )
		{
			m_fMaxCountRenderTime = g_pKTDXApp->GetRenderCount();
			if( DXUTGetFPS() < 15.0f )
			{
				m_MaxCount -= 2;
				if( m_MaxCount < 10 )
					m_MaxCount = 10;
			}
			else
			{
				m_MaxCount += 1;
				if( m_MaxCount > m_SquareData.m_MaxUser )
					m_MaxCount = m_SquareData.m_MaxUser;
			}
		}
	}

	m_fP2PSendGap -= fElapsedTime;
	if( m_fP2PSendGap <= 0.0f )
	{
		m_fP2PSendGap = 5.0f;
		
		if( m_pMyUnit != NULL && m_bLeaveSquareGame == false )
		{
			m_pMyUnit->SendPacket( 100 );
		}
		
		/*
		int p2pCount = 0;
		KXPT_SQUARE_UNIT_SYNC kXPT_SQUARE_UNIT_SYNC;
		m_pMyUnit->GetPacketP2P( kXPT_SQUARE_UNIT_SYNC );
		KSerBuffer buff;
		Serialize( &buff, &kXPT_SQUARE_UNIT_SYNC );

		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			CX2SquareUnit* pCX2SquareUnit = m_UserUnitList[i];
			if( pCX2SquareUnit == m_pMyUnit || pCX2SquareUnit->GetInit() == false )
				continue;

			if( pCX2SquareUnit != NULL )
			{
				pCX2SquareUnit->SendPacketP2P( 0, 100 );
				//CX2Unit* pUnit = pCX2SquareUnit->GetUnit();
				//g_pData->GetGameUDP()->Send( pUnit->GetUnitData().m_IP.c_str(), pUnit->GetUnitData().m_Port, XPT_SQUARE_UNIT_SYNC, (char*)buff.GetData(), buff.GetLength() );

				//p2pCount++;
				//if( p2pCount > 20 )
				//	break;
			}			
		}
		*/
	}



	if( NULL != m_pTalkBoxManager )
	{
		m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
	}

	m_fTimeCheckShopTalkBox += fElapsedTime;
	if ( m_fTimeCheckShopTalkBox >= 0.5f )
	{
		m_fTimeCheckShopTalkBox = 0.0f;
		for ( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			CX2SquareUnitPtr pSquareUnit = m_UserUnitList[i];
			if ( pSquareUnit != NULL && pSquareUnit->GetUnit() != NULL )
			{
				wstring wstrstm;
				wstrstm = pSquareUnit->GetPersonalShopName().c_str();		
				wstring wstrShopName = g_pMain->GetStringFilter()->FilteringChatString( wstrstm.c_str(), L'��' );
				PopPersoanlShopTalkBox( pSquareUnit->GetUID(), wstrShopName, D3DXCOLOR(0,0.4f,0,1) );
			}
		}
	}

	if ( m_pTalkBoxMgrForPersonalShop != NULL )
	{
		m_pTalkBoxMgrForPersonalShop->OnFrameMove( fTime, fElapsedTime );
	}

	if( m_bEnableCameraProcess == true )
	{
		if( false == m_bFreeCamera )
		{
#ifdef NEW_VILLAGE_RENDERING_TEST
			switch( m_pWorld->GetSquareCameraData().m_eSquareCameraType )
			{
			default:
			case CX2World::SCT_DEFAULT:
				{

#ifdef HEAD_ATTACHED_CAMERA_TEST

					if( NULL != GetMyUnit() &&
						true == GetMyUnit()->GetOnRollerCoaster() )
					{
						GetMyUnit()->BoneAttachedCamera( L"Bip01_Head", D3DXVECTOR3( 0, 20, -700 ) );


					}
					else 
					{
						m_pCamera->SquareCamera( m_pMyUnit.get() );
					}

#else HEAD_ATTACHED_CAMERA_TEST
					m_pCamera->SquareCamera( m_pMyUnit.get() );

#endif HEAD_ATTACHED_CAMERA_TEST


				} break;

			case CX2World::SCT_PRESET:
				{
					m_pCamera->SquareCamera( m_pMyUnit.get(), m_pWorld->GetSquareCameraData().m_fCameraDist,
						m_pWorld->GetSquareCameraData().m_fEyeHeight, 
						m_pWorld->GetSquareCameraData().m_fLookAtHeight );
				} break;

			case CX2World::SCT_LINE_DEPENDENT:
				{
					if( NULL != m_pWorld->GetLineMap() )
					{
						const CKTDGLineMap::CameraData* pCameraData = m_pWorld->GetLineMap()->GetCameraData( m_pMyUnit->GetLastTouchLineIndex() );
						if( NULL != pCameraData )
						{
							m_pCamera->SquareLineCamera( m_pMyUnit.get(), pCameraData->m_vEye, pCameraData->m_vLookAt, pCameraData->m_bFocusUnit, 3.f );
						}
						else 
						{
							if( 0.f != m_pWorld->GetSquareCameraData().m_fCameraDist &&
								0.f != m_pWorld->GetSquareCameraData().m_fEyeHeight &&
								0.f != m_pWorld->GetSquareCameraData().m_fLookAtHeight )	// preset ������ ������
							{
								m_pCamera->SquareCamera( m_pMyUnit.get(), m_pWorld->GetSquareCameraData().m_fCameraDist,
									m_pWorld->GetSquareCameraData().m_fEyeHeight, 
									m_pWorld->GetSquareCameraData().m_fLookAtHeight );
							}
							else
							{
								m_pCamera->SquareCamera( m_pMyUnit.get(), 1000.f, 300.f );
							}
						}
					}
					else
					{
						m_pCamera->SquareCamera( m_pMyUnit.get(), 1000.f, 300.f );
					}
				} break;
			}
#else
			m_pCamera->SquareCamera( m_pMyUnit.get() );
#endif NEW_VILLAGE_RENDERING_TEST
			
			m_pCamera->OnFrameMove( fTime, fElapsedTime );
		}
		else
		{
			m_FPSCamera.SetEnablePositionMovement( true );

			m_FPSCamera.FrameMove( fElapsedTime * 300.f );			
			m_pCamera->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * 500.f + *m_FPSCamera.GetEyePt();
			m_pCamera->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
			m_pCamera->GetCamera().UpdateCamera( fElapsedTime );
		}

	}

	return S_OK;
}

HRESULT	CX2SquareGame::OnFrameRender()
{
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
	g_pKTDXApp->GetDGManager()->ObjectChainSort();

	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
    g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
	
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			pCX2SquareUnit->RenderName();
		}
	}

	if ( m_pTalkBoxMgrForPersonalShop != NULL )
		m_pTalkBoxMgrForPersonalShop->OnFrameRender();

	if( NULL != m_pTalkBoxManager )
	{
		m_pTalkBoxManager->OnFrameRender();
	}

	return S_OK;
}

bool CX2SquareGame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( true == m_bFreeCamera )
	{
		m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);		
	}

	if ( m_pTalkBoxMgrForPersonalShop->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	return false;
}

HRESULT	CX2SquareGame::OnResetDevice()
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if ( pCX2SquareUnit != NULL )
			pCX2SquareUnit->OnResetDevice();
	}

	if( m_pWorld != NULL )
		m_pWorld->OnResetDevice();

	if( m_pMajorParticle != NULL )
		m_pMajorParticle->OnResetDevice();
	if( m_pMinorParticle != NULL )
		m_pMinorParticle->OnResetDevice();

	return S_OK;
}

HRESULT	CX2SquareGame::OnLostDevice()
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if ( pCX2SquareUnit != NULL )
			pCX2SquareUnit->OnLostDevice();
	}

	if( m_pWorld != NULL )
		m_pWorld->OnLostDevice();

	if( m_pMajorParticle != NULL )
		m_pMajorParticle->OnLostDevice();
	if( m_pMinorParticle != NULL )
		m_pMinorParticle->OnLostDevice();

	return S_OK;
}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void CX2SquareGame::P2PPacketHandler()
//{
//	KTDXPROFILE();
//
//	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
//	while( pRecvData != NULL )
//	{
//		//switch( pRecvData->m_ID )
//		//{
//		//	case XPT_SQUARE_UNIT_SYNC:
//		//		{
//		//			KSerBuffer ksBuff;
//		//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//		//			KXPT_SQUARE_UNIT_SYNC kXPT_SQUARE_UNIT_SYNC;
//		//			DeSerialize( &ksBuff, &kXPT_SQUARE_UNIT_SYNC );
//		//			CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( kXPT_SQUARE_UNIT_SYNC.m_UnitUID );
//		//			if( pCX2SquareUnit != NULL )
//		//				pCX2SquareUnit->RecvPacketP2P( kXPT_SQUARE_UNIT_SYNC );
//		//		}
//		//		break;
//
//		//	case XPT_SQUARE_UNIT_SYNC_RIGHT:
//		//		{
//		//			KSerBuffer ksBuff;
//		//			ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//		//			KXPT_SQUARE_UNIT_SYNC_RIGHT kXPT_SQUARE_UNIT_SYNC_RIGHT;
//		//			DeSerialize( &ksBuff, &kXPT_SQUARE_UNIT_SYNC_RIGHT );
//		//			CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( kXPT_SQUARE_UNIT_SYNC_RIGHT.m_UnitUID );
//		//			if( pCX2SquareUnit != NULL )
//		//				pCX2SquareUnit->RecvPacketRightP2P( kXPT_SQUARE_UNIT_SYNC_RIGHT );
//		//		}
//		//		break;
//
//		//	case XPT_SQUARE_UNIT_SYNC_REQ:
//		//		{
//		//			if( m_pMyUnit != NULL )
//		//			{
//		//				KSerBuffer ksBuff;
//		//				ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//		//				KXPT_SQUARE_UNIT_SYNC_REQ kXPT_SQUARE_UNIT_SYNC_REQ;
//		//				DeSerialize( &ksBuff, &kXPT_SQUARE_UNIT_SYNC_REQ );
//
//		//				m_pMyUnit->SendPacketP2P( kXPT_SQUARE_UNIT_SYNC_REQ.m_UnitUID );
//		//			}					
//		//		}
//		//		break;
//		//}
//		SAFE_DELETE( pRecvData );
//		pRecvData = g_pData->GetGameUDP()->PopRecvData();
//	}
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


void CX2SquareGame::KeyProcess()
{
	KTDXPROFILE();

	m_InputData.Init();

	if ( GET_DOUBLEKEYPURE_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.pureDoubleLeft = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.oneDoubleLeft = true;
	}
	if ( GET_PUREKEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.pureLeft = true;
	}
	if ( GET_KEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.oneLeft = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.pureDoubleRight = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.oneDoubleRight = true;
	}
	if ( GET_PUREKEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.pureRight = true;
	}
	if ( GET_KEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.oneRight = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_UP ) == TRUE )
	{
		m_InputData.pureDoubleUp = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_UP ) == TRUE )
	{
		m_InputData.oneDoubleUp = true;
	}
	if ( GET_PUREKEY_STATE( GA_UP ) == TRUE )
	{
		m_InputData.pureUp = true;
	}
	if ( GET_KEY_STATE( GA_UP ) == TRUE )
	{
		m_InputData.oneUp = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.pureDoubleDown = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.oneDoubleDown = true;
	}
	if ( GET_PUREKEY_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.pureDown = true;
	}
	if ( GET_KEY_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.oneDown = true;
	}
	if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE )
	{
		if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
			return;

#ifdef RIDING_SYSTEM
		if ( m_pMyUnit->GetNowState() != m_pMyUnit->GetStateID().m_Wait )
			return;

		if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITREADY )
			return;

		g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2501 ) );
#else //RIDING_SYSTEM
		m_pMyUnit->PlayEmotion( CX2Unit::ET_SITREADY );
#endif //RIDING_SYSTEM
	}

	m_pMyUnit->SetInputData( &m_InputData );
}

void CX2SquareGame::Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( KEGS_SQUARE_UNIT_SYNC_DATA_NOT& kEGS_SQUARE_UNIT_SYNC_DATA_NOT )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( m_pMyUnit->GetUID() == kEGS_SQUARE_UNIT_SYNC_DATA_NOT.m_UnitUID )
			break;
		if( pCX2SquareUnit != NULL && kEGS_SQUARE_UNIT_SYNC_DATA_NOT.m_UnitUID == pCX2SquareUnit->GetUID() )
		{
			pCX2SquareUnit->RecvPacket( kEGS_SQUARE_UNIT_SYNC_DATA_NOT );
			break;
		}		
	}	
}

bool CX2SquareGame::Handler_EGS_JOIN_SQUARE_NOT( KEGS_JOIN_SQUARE_NOT& kEGS_JOIN_SQUARE_NOT )
{
	JoinSquareUnit( &kEGS_JOIN_SQUARE_NOT.m_kUserInfo );
	return true;
}

bool CX2SquareGame::Handler_EGS_LEAVE_SQUARE_NOT( KEGS_LEAVE_SQUARE_NOT& kEGS_LEAVE_SQUARE_NOT )
{
	RemoveUnit(	kEGS_LEAVE_SQUARE_NOT.m_iUnitUID );
	if ( m_pTalkBoxMgrForPersonalShop != NULL )
		m_pTalkBoxMgrForPersonalShop->Delete( kEGS_LEAVE_SQUARE_NOT.m_iUnitUID, true );

	return true;
}

bool CX2SquareGame::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT( KEGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT& kEGS_CHANGE_EQUIPPED_ITEM_NOT )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID );


	if( pCX2SquareUnit == NULL )
		return false;
	
	if( pCX2SquareUnit->GetUnit() == NULL )
		return false;



//#ifdef TITLE_SYSTEM
	if( true == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.empty() )
	{
#ifdef SERV_TITLE_DATA_SIZE
		pCX2SquareUnit->GetUnit()->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iEquippedTitleID);
#else
		pCX2SquareUnit->GetUnit()->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_sEquippedTitleID);
#endif

		pCX2SquareUnit->UpdateEquippedEmblem();
		return true;
	}
//#endif TITLE_SYSTEM


	CX2UnitViewerUI* pCX2UnitViewerUI = pCX2SquareUnit->GetUnitViewer();

	// �� ������ ��쿡
	if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID )
	{
#ifdef FIX_WRONG_EQUIP
		// EGS_CHANGE_INVENTORY_SLOT_ACK ��Ŷ�� �����ϱ� ���� �� ��Ŷ�� �����ϸ�
		// ���ŵ��� ���� �� �κ��丮�� �̿��Ͽ� �����ϱ� ������ ������Ʈ�� �� ��
		// �׷��Ƿ� ���⼭ ó������ �ʰ� EGS_CHANGE_INVENTORY_SLOT_ACK ���� ó���ϵ��� ����
#else FIX_WRONG_EQUIP
		if ( pCX2UnitViewerUI != NULL )
			pCX2UnitViewerUI->UpdateEqip( true );
#endif FIX_WRONG_EQUIP

		return true;
	}





	// �ٸ� ����� ������ ��쿡
	CX2UnitViewerUI* pUnitViewerUIWatch = NULL; 
	if( true ==  g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_WATCH) &&
		g_pData->GetUIManager()->GetUIWatch()->GetTargetUnitUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID )
	{
		pUnitViewerUIWatch = g_pData->GetUIManager()->GetUIWatch()->GetUnitViewerUI();
	}

	CX2Unit* pCX2Unit = pCX2SquareUnit->GetUnit();

	for( int i = 0; i < (int)kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo[i];

		if( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP || 
			kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_EQUIP ||
			kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_ACCESSORY ||
			kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_AVARTA )
		{
			CX2Item* pItem = pCX2Unit->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if( pItem != NULL )
			{
				pCX2Unit->RemoveEqip( pItem->GetUID() );

				if( pCX2UnitViewerUI != NULL )
					pCX2UnitViewerUI->RemoveEqip( pItem->GetUID() );

				if( pUnitViewerUIWatch != NULL )
					pUnitViewerUIWatch->RemoveEqip( pItem->GetUID() );

			}
		}
	}

	for( int i = 0; i < (int)kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo[i];
		pCX2Unit->AccessInventory().RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
	}

	for( int i = 0; i < (int)kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo[i];
		if( kInventorySlotInfo.m_iItemUID <= 0 )
			continue;

        {
		    CX2Item::ItemData kItemData( kInventorySlotInfo );
		    pCX2Unit->AccessInventory().AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, kItemData );
        }

		if( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
		{
			CX2Item* pItem = pCX2Unit->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if( pItem != NULL )
			{
				if( true == pCX2Unit->AddEqip( pItem->GetUID() ) )
				{
					if( pCX2UnitViewerUI != NULL )
						pCX2UnitViewerUI->AddEqip( pItem->GetUID() );

					if( pUnitViewerUIWatch != NULL )
					{
						pUnitViewerUIWatch->AddEqip( pItem->GetUID() );
					}
				}
			}
		}
	}



	if( pUnitViewerUIWatch != NULL )
	{
		g_pData->GetUIManager()->GetUIWatch()->ResetEquipUI();

		// note!! ���ĳ����������â���� Īȣ�� �ٲ���� �� �������ִ� �κ� �۾��ؾ���
	}


	pCX2SquareUnit->UpdateEquippedEmblem();

	return true;
}

bool CX2SquareGame::Handler_EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT( KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT )
{
	if ( g_pSquareGame != NULL )
	{
		CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT.m_iUnitUID );
		if ( pSquareUnit != NULL )
		{
			if ( pSquareUnit->GetUnit() != NULL )
			{
				pSquareUnit->SetPersonalShopState( (CX2SquareUnit::PERSONAL_SHOP_STATE)kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT.m_cPersonalShopState );
				pSquareUnit->SetPersonalShopName( kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT.m_wstrPersonalShopName.c_str() );

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
				pSquareUnit->SetShopType( static_cast<SEnum::AGENCY_SHOP_TYPE>( kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT.m_cPersonalShopType ) );
#else // SERV_UPGRADE_TRADE_SYSTEM
				pSquareUnit->SetShopType( (CX2SquareUnit::PERSONAL_SHOP_TYPE) kEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT.m_cPersonalShopType );
#endif // SERV_UPGRADE_TRADE_SYSTEM
			}
		}
	}

	return true;
}

void CX2SquareGame::GetUnitPageInfo(int &iMaxPage, int iUnitNumPerPage)
{
	int nNumUnit = (int)m_UserUnitList.size();
	iMaxPage = (nNumUnit + iUnitNumPerPage - 1) / iUnitNumPerPage; 
}

vector<CX2SquareUnit*> CX2SquareGame::GetUnitList( int& iPage, int iUnitNumPerPage /*= 10 */ )
{
	if( iUnitNumPerPage <= 0 )
		iUnitNumPerPage = 1;

	vector<CX2SquareUnit*> vecUnitList;

	int nNumUnit = (int)m_UserUnitList.size();
	int iMaxPage = (nNumUnit + iUnitNumPerPage - 1) / iUnitNumPerPage; 
	if( iMaxPage < 1 )
		return vecUnitList;

	if( iPage > iMaxPage )
		iPage = iMaxPage;

	if( iPage < 1 )
		iPage = 1;

	int iStartIndex = (iPage-1) * iUnitNumPerPage; 
	int iEndIndex;

	if( iStartIndex < (int)m_UserUnitList.size() )
	{
		iEndIndex = min( (int)m_UserUnitList.size(), iStartIndex + iUnitNumPerPage );
		for( int i=iStartIndex; i<iEndIndex; i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
			vecUnitList.push_back( pCX2SquareUnit.get() );
		}
	}

	return vecUnitList;
}




void CX2SquareGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( iUnitUID_ );
	if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetInit() == true && pCX2SquareUnit->GetShowObject() == true &&
		pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_NONE )
	{
#ifdef UNIT_EMOTION
		bool bCommandEmotion = g_pChatBox->IsEmotionID(pWstrMsg_);
		if( g_pChatBox != NULL && bCommandEmotion == false )
#endif
		{
            if( pCX2SquareUnit->GetBoundingRadius() > 0 )
            {
			    D3DXVECTOR3 center;
			    pCX2SquareUnit->GetTransformCenter( &center );

			    //�ø�
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                float fScaledBoundingRadius = pCX2SquareUnit->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
			    float fScale;
			    if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetYScale() )
			    {
				    if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
				    {
					    //X�� ���� ŭ
					    fScale = pCX2SquareUnit->GetMatrix().GetXScale();
				    }
				    else
				    {
					    //Z�� ���� ŭ
					    fScale = pCX2SquareUnit->GetMatrix().GetZScale();
				    }
			    }
			    else
			    {
				    if( pCX2SquareUnit->GetMatrix().GetYScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
				    {
					    //Y�� ���� ŭ
					    fScale = pCX2SquareUnit->GetMatrix().GetYScale();
				    }
				    else
				    {
					    //Z�� ���� ŭ
					    fScale = pCX2SquareUnit->GetMatrix().GetZScale();
				    }
			    }
                float fScaledBoundingRadius = pCX2SquareUnit->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
			    if( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
				    return;
            }

			CX2TalkBoxManagerImp::TalkBox talkBox;
			talkBox.m_OwnerUnitUID		= iUnitUID_;
			talkBox.m_wstrTalkContent	= pWstrMsg_;
			talkBox.m_bTraceUnit		= true;
			talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
			talkBox.m_coTextColor		= coTextColor_;
			talkBox.m_BackTexColor		= coBackColor_;

			talkBox.m_bUseOutLine		 = true;
			if( coOutColor_ == 0x00000000 )	//	�Է� ���� ���� �۾��� �״�� ����
			{
				talkBox.m_bUseOutLine = false;
			}

			m_pTalkBoxManager->Push( talkBox );
		}

		
#ifdef UNIT_EMOTION
		// �̸�� ��������ش�.
		if(g_pChatBox != NULL)
		{
			// ���� ���¿����� ä�����͸� ������ �ʵ��� �Ѵ�.(��ɾ�θ� �̸�ǵ�������)
			if( bCommandEmotion == false && pCX2SquareUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
				return;

			pCX2SquareUnit->PlayEmotion( g_pChatBox->GetEmotionID(pWstrMsg_) );			
		}
#endif
	}
}


CX2SquareUnit* CX2SquareGame::GetSquareUnit( int index )
{
	if( index >= (int)m_UserUnitList.size() )
		return NULL;
	return m_UserUnitList[index].get();
}


CX2SquareUnit* CX2SquareGame::GetSquareUnitByUID( UidType UID )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( pCX2SquareUnit->GetUID() == UID )
			{
				return pCX2SquareUnit.get();
			}
		}
	}

	return NULL;
}

CX2SquareUnit* CX2SquareGame::GetSquareUnitByNickName( const WCHAR* wszNickName )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( 0 == pCX2SquareUnit->GetNickName().compare( wszNickName ) )
			{
				return pCX2SquareUnit.get();
			}
		}
	}

	return NULL;
}

void CX2SquareGame::SetShowUserUnit( bool bShow )
{
	for( UINT i=0; i<m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			pCX2SquareUnit->SetShowObject( bShow );
		}
	}
}

void CX2SquareGame::SetFreeCamera( bool bFreeCamera )
{
	m_bFreeCamera = bFreeCamera;

	if( true == m_bFreeCamera )
	{
		D3DXVECTOR3 vEye	= m_pCamera->GetCamera().GetEye();
		D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();

		m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	}
}

bool CX2SquareGame::GetEnableKey()
{
#ifdef ATTRACTION_ITEM_TEST
	if( g_pData != NULL && g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayAttractionItem() == true )
		return false;
#endif
#ifdef SERV_TREASURE_BOX_ITEM
	if( g_pData != NULL && g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetUIInventory() != NULL && g_pData->GetUIManager()->GetUIInventory()->GetPlayTreasureBox() == true )
		return false;
#endif SERV_TREASURE_BOX_ITEM

	//{{ ����� : [2009/10/13] //	��� ���� UI ��½� Ű �Է� �ȸ������� ����
#ifdef GUILD_MANAGEMENT
	if( g_pData->GetGuildManager()->GetUIGuild()->GetShowGuildInfo() == true )
	{
		return false;
	}
#endif	//	GUILD_MANAGEMENT
	//}} ����� : [2009/10/13] //	��� ���� UI ��½� Ű �Է� �ȸ������� ����

#ifdef FREE_WARP
	if( g_pData != NULL && g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIInventory() != NULL && 
		g_pData->GetUIManager()->GetUIInventory()->GetShowWarpDest() == true )
	{
		return false;
	}
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	if( g_pData != NULL && g_pData->GetUIManager() != NULL &&
		g_pData->GetUIManager()->GetUIInventory() != NULL && 
		g_pData->GetUIManager()->GetUIInventory()->GetShowSecondJobDest() == true )
	{
		return false;
	}
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_ADD_WARP_BUTTON
	CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
	if( pStateMenu != NULL && pStateMenu->GetShowWarpDest() == true )
	{
		return false;
	}
#endif // SERV_ADD_WARP_BUTTON

	return m_bEnableKeyProcess;
}

void CX2SquareGame::PopPersoanlShopTalkBox( UidType iUnitUID, wstring& wstrMsg, D3DXCOLOR color /* = 0xff000000 */ )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( iUnitUID );
	if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetInit() == true )
	{

		if( pCX2SquareUnit->GetPersonalShopState() != CX2SquareUnit::PSS_SHOP  )
		{
			m_pTalkBoxMgrForPersonalShop->Delete( iUnitUID, true );
			return;
		}

        if( pCX2SquareUnit->GetBoundingRadius() > 0 )
        {
		    D3DXVECTOR3 center;
		    pCX2SquareUnit->GetTransformCenter( &center );
		//�ø�

#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
            float   fScaledBoundingRadius = pCX2SquareUnit->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
		    float fScale;
		    if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetYScale() )
		    {
			    if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
			    {
				    //X�� ���� ŭ
				    fScale = pCX2SquareUnit->GetMatrix().GetXScale();
			    }
			    else
			    {
				    //Z�� ���� ŭ
				    fScale = pCX2SquareUnit->GetMatrix().GetZScale();
			    }
		    }
		    else
		    {
			    if( pCX2SquareUnit->GetMatrix().GetYScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
			    {
				    //Y�� ���� ŭ
				    fScale = pCX2SquareUnit->GetMatrix().GetYScale();
			    }
			    else
			    {
				    //Z�� ���� ŭ
				    fScale = pCX2SquareUnit->GetMatrix().GetZScale();
			    }
		    }
            float   fScaledBoundingRadius = pCX2SquareUnit->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

		    if( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
		    {
			    m_pTalkBoxMgrForPersonalShop->Delete( iUnitUID, true );
			    return;
		    }
        }

		if ( m_pTalkBoxMgrForPersonalShop->CheckTalkBox( iUnitUID ) == true )
			return;

		if ( pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_SHOP )
		{
			CX2TalkBoxManagerImp::TalkBox talkBox;
			talkBox.m_OwnerUnitUID		= iUnitUID;
			talkBox.m_wstrTalkContent	= wstrMsg;
			talkBox.m_bTraceUnit		= true;
			talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
			talkBox.m_coTextColor		= color;
			talkBox.m_fRemainTime		= 999999.0f;

			m_pTalkBoxMgrForPersonalShop->Push( talkBox );
		}
	}
}
