
// X2ToolView.cpp : CX2ToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "X2Tool.h"
#endif

#include "X2ToolDoc.h"
#include "X2ToolView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPDIRECT3DDEVICE9		g_pd3dDevice;
LPDIRECT3D9				g_pD3D;
CX2SMTPMail*			g_pX2SMTPMail = NULL;
CKTDGEffectStateManager* g_pEffectStateManager = NULL;

static bool g_bRenderEtc = false;	// toggle fullscreen, ������ ��ư�� ������ ���� ����

static float	g_fLastUpdateTime_MemInfo = 10000.f;
DWORD			g_iAvailableTextureMem = 0;
DWORD			g_iTotalTextureMem = 0;
DWORD			g_iAvailableTextureMem2 = 0;
MEMORYSTATUS	g_MemStatus;

double			g_fTotalFrame = 0.0f;
int				g_RenderCount = 0;

HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void	CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
void    RenderText(float fElapsedTime);

void InitApp()
{
	// Initialize dialogs
#ifndef  REMOVE_DXUT_SAMPLE_CODE
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	g_SampleUI.Init( &g_DialogResourceManager );

	g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22 );

	g_SampleUI.SetCallback( OnGUIEvent ); iY = 10; 
#endif  REMOVE_DXUT_SAMPLE_CODE
}

HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	HRESULT hr;
#endif
	// Initialize the font

	g_pd3dDevice = pd3dDevice;
	g_pD3D = DXUTGetD3DObject();

	//{{ robobeg : 2008-10-10
	g_pEffectStateManager = new CKTDGEffectStateManager;
	ASSERT( g_pEffectStateManager != NULL );
	//}} robobeg : 2008-10-10


	g_pKTDXApp	= new CKTDXApp( DXUTGetHWND(), DXUTGetHINSTANCE(), pd3dDevice );
	//g_pMain		= new CX2Main();
	CX2ToolMain* pStage = new CX2ToolMain;
	g_pMain = (CX2Main*)pStage;
	g_pKTDXApp->InsertStage( pStage );	


#ifndef REMOVE_DXUT_SAMPLE_CODE
	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
#endif

	return S_OK;
}

HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	HRESULT hr;
#endif

#ifndef REMOVE_DXUT_SAMPLE_CODE
	V_RETURN( g_DialogResourceManager.OnResetDevice() );
	V_RETURN( g_SettingsDlg.OnResetDevice() );
#endif  REMOVE_DXUT_SAMPLE_CODE

	g_pKTDXApp->OnResetDevice( pBackBufferSurfaceDesc );

	return S_OK;
}

void CALLBACK OnLostDevice( void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	g_DialogResourceManager.OnLostDevice();
	g_SettingsDlg.OnLostDevice();
#endif  REMOVE_DXUT_SAMPLE_CODE

	g_pKTDXApp->OnLostDevice();
}

void CALLBACK OnDestroyDevice( void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	g_DialogResourceManager.OnDestroyDevice();
	g_SettingsDlg.OnDestroyDevice();
#endif  REMOVE_DXUT_SAMPLE_CODE

	SAFE_DELETE( g_pKTDXApp );

	KNP_Close();

	SAFE_DELETE( g_pX2SMTPMail );

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnDestroyDevice();

	KProfileMan::GetSingleton().SetProfileIterator( NULL );
#endif


	//{{ robobeg : 2008-10-10
	SAFE_RELEASE( g_pEffectStateManager );
	g_pd3dDevice = NULL;
	//}} robobeg : 2008-10-10

#ifdef _SERVICE_
	HWND hWeb = ::FindWindow(NULL, L"Elsword - Windows Internet Explorer");
	if( hWeb != NULL )
	{
		::ShowWindow(hWeb, SW_RESTORE);
	}
#endif _SERVICE_
}

LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	if( g_pKTDXApp != NULL )
		*pbNoFurtherProcessing = g_pKTDXApp->MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	return 0;
}

void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( bKeyDown )
	{
		switch( nChar )
		{
			//case VK_F1: g_bShowHelp = !g_bShowHelp; break;
		case VK_HOME: 
			{
				if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
				{
					g_bRenderEtc = !g_bRenderEtc; 
#ifdef NEW_VILLAGE
					g_pData->ToggleRenderEtc();
#endif
				}
			}
			break;

#ifdef KHELP_SYSTEM_TEST
		case VK_PRIOR:
			if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				// Shift+End Ű�� ���� ��� KHelpSystem�� ����Ѵ�.
				if ( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
				{
					// toggle help system
					_KHelpSystem.EnableDisplayHelp( !_KHelpSystem.IsDisplayHelp() );
				}
				else
				{
					// KHelpSystem�� Ȱ��ȭ�� ���¿��� EndŰ�� ������ ���� help�� Ȱ��ȭ��Ų��.
					if ( _KHelpSystem.IsDisplayHelp() )
					{
						_KHelpSystem.PrevHelp();
					}//if
				}//if.. else..
			}//if
			break;

		case VK_NEXT:
			if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				// Shift+End Ű�� ���� ��� KHelpSystem�� ����Ѵ�.
				if ( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
				{
					// toggle help system
					_KHelpSystem.EnableDisplayHelp( !_KHelpSystem.IsDisplayHelp() );
				}
				else
				{
					// KHelpSystem�� Ȱ��ȭ�� ���¿��� EndŰ�� ������ ���� help�� Ȱ��ȭ��Ų��.
					if ( _KHelpSystem.IsDisplayHelp() )
					{
						_KHelpSystem.NextHelp();
					}//if
				}//if.. else..
			}//if
			break;
#endif // KHELP_SYSTEM_TEST
		}
	}
}

void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	KTDXPROFILE();

	g_pKTDXApp->OnFrameMove( fTime, fElapsedTime );
}

void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//KTDXPROFILE();

	HRESULT hr;
	
	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{

		KTDXPROFILE_BEGIN("KTDXRender");
		g_pKTDXApp->OnFrameRender();
		KTDXPROFILE_END();

		if( true == g_bRenderEtc )
		{
			KTDXPROFILE_BEGIN("DXUTRender");
			DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing
			KTDXPROFILE_BEGIN("RenderText");
			RenderText(fElapsedTime);
			KTDXPROFILE_END();

			DXUT_EndPerfEvent();
			KTDXPROFILE_END();
		}

#ifdef KHELP_SYSTEM_TEST // 2008-10-23
        _KHelpSystem.DisplayHelp();
#endif // KHELP_SYSTEM_TEST // 2008-10-23
		
#ifdef BANDICAM_RECORDING
		if( g_pMain != NULL && g_pMain->IsCapturing() == true )
		{
			switch( g_pMain->m_BandiCaptureLibrary.Work((LONG_PTR)pd3dDevice) )
			{
			case BC_OK_IS_NOT_CAPTURING:	//ĸ������ �ƴմϴ�.
			case BC_OK_COMPLETED_BY_TIME:	//��ȭ �ڵ� �Ϸ� ����(�ð�)�� ���� ��ȭ �Ϸ�Ǿ����ϴ�.
			case BC_OK_COMPLETED_BY_SIZE:	//��ȭ �ڵ� �Ϸ� ����(����ũ��)�� ���� ��ȭ �Ϸ� �Ǿ����ϴ�.
			case BCERR_VIDEO_CAPTURE:		//ĸ���� ������ �߻��Ͽ����ϴ�.
			case BCERR_FILE_CREATE:			//���� ������ �����Ͽ����ϴ�.
				{
					g_pMain->Bandi_EndVidioCapture();
				}break;
			case BCERR_FILE_WRITE:			// ���� ���⸦ �����Ͽ����ϴ�.
			case BCERR_NOT_ENOUGH_DISKSPACE:// �ϵ��ũ ���� ���� ������ �����մϴ�.
				{
					g_pMain->Bandi_EndVidioCapture();
					AfxMessageBox(L"�ϵ� ��ũ �뷮�� ���� �մϴ�.", MB_OK);
				}break;
			default:
				break;
			}
		}
#endif BANDICAM_RECORDING

		V( pd3dDevice->EndScene() );
	}
}

//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText(float fElapsedTime)
{
	g_fTotalFrame += DXUTGetFPS();
	g_RenderCount += 1;
	
	// Output statistics
	WCHAR appData[512] = {0,};
	WCHAR appData3[512] = {0,};
	WCHAR wszText[512] = L"";
	swprintf( appData, DXUTGetFrameStats() );
	wcscat( appData, L"\n" );
	wcscat( appData, DXUTGetDeviceStats() );
	wcscat( appData, L", ��� ������ : ");
	wcscat( appData, _itow( (int)g_fTotalFrame / g_RenderCount, appData3, 10 ) );
	if ( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", ������Ʈ LOD : %d %%", g_pX2Game->GetWorld()->GetLOD() );
		wcscat( appData, wszText );
	}

	if ( g_pData != NULL )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", ĳ���� LOD : %d %%", g_pData->GetModelDetailPercent() );
		wcscat( appData, wszText );
	}

#ifdef FRAME_MOVE_CULL
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", FrameUpdate : %d %%", (int)CKTDGXSkinAnim::GetTestUpdate() );
	wcscat( appData, wszText );
#endif //FRAME_MOVE_CULL	

#ifdef  DYNAMIC_VERTEX_BUFFER_OPT

    if ( g_pKTDXApp != NULL && g_pKTDXApp->GetDVBManager() != NULL )
    {
        const WCHAR*  pDynamicVBMode = L"";
        switch( g_pKTDXApp->GetDVBManager()->GetDynamicVBMode() )
        {
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_MANAGED:
            pDynamicVBMode = L"MANAGED";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_PRIMITIVEUP:
            pDynamicVBMode = L"PrimitiveUP";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_DYNAMICVB:
            pDynamicVBMode = L"DynamicVB";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH:
            pDynamicVBMode = L"DynamicVBWithTC";
            break;
        }//switch

	    swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", DynamicVBMode : %s", pDynamicVBMode );
	    wcscat( appData, wszText );
    }//if
#endif  DYNAMIC_VERTEX_BUFFER_OPT
	
	wcscat( appData, L"\n" );
	
	WCHAR appData2[256] = {0,};

	int iSimultaneousUserCount = 0;
	int iPVPRoomTeamCount = 0;
	int iPVPRoomTeamDeathCount = 0;
	int iPVPRoomSurvivalCount = 0;
	int iCurPartyCount = 0;
	int iPlayGamePartyCount = 0;
	if( NULL != g_pMain )
	{	
		iSimultaneousUserCount	= g_pMain->GetServerUserCount();
		std::map<int, int>& mapPVPRoomCount = g_pMain->GetMapServerPVPRoomCount();
		if( mapPVPRoomCount.size() > 0 )
		{
			iPVPRoomTeamCount		= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_TEAM ];
			iPVPRoomTeamDeathCount	= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_TEAM_DEATH ];
			iPVPRoomSurvivalCount	= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_SURVIVAL ];
		}

		iCurPartyCount				= g_pMain->GetCurPartyCount();
		iPlayGamePartyCount			= g_pMain->GetPlayGamePartyCount();
	}

#ifndef _SERVICE_
	swprintf( appData2, L"�����ӹ��� FPS: %f �����ð�: %f  �����ӹ���: %dȸ  ����: %dȸ \n", 
		g_pKTDXApp->GetFrameMoveFPS(), g_pKTDXApp->GetElapsedTime(), g_pKTDXApp->GetFrameMoveCount(), g_pKTDXApp->GetRenderCount() );
	wcscat( appData, appData2 );
#endif

	swprintf( appData2, L"����: %d��, ������: <��:%d, ������:%d, ����:%d>, ��Ƽ: %d, ����������Ƽ: %d", 
		iSimultaneousUserCount, iPVPRoomTeamCount, iPVPRoomTeamDeathCount, iPVPRoomSurvivalCount, iCurPartyCount, iPlayGamePartyCount );
	wcscat( appData, appData2 );
	
	/*
	txtHelper.Begin();
	txtHelper.SetInsertionPos( 5, 5 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( appData );
	*/

	WCHAR timeBuf[200] = {0,};
	g_pMain->GetFontForDebuf()->OutTextMultiline( 5, 5, appData, D3DXCOLOR( 1, 1, 0, 1), 
		CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );

#ifndef _SERVICE_
	if( g_pKTDXApp != NULL && g_pKTDXApp->GetDevice() != NULL )
	{
		g_fLastUpdateTime_MemInfo += g_pKTDXApp->GetElapsedTime();
		if( g_fLastUpdateTime_MemInfo > 10.f ) 
		{
			g_fLastUpdateTime_MemInfo = 0.f;

			g_MemStatus.dwLength = sizeof( g_MemStatus );
			GlobalMemoryStatus( &g_MemStatus );

			g_pKTDXApp->GetVideoRam( g_iTotalTextureMem, g_iAvailableTextureMem );

			g_iAvailableTextureMem2 = g_pKTDXApp->GetDevice()->GetAvailableTextureMem();
		}
#ifdef CHECK_PROCESS_MEMORY_USAGE   
		PROCESS_MEMORY_COUNTERS_EX pmcex;
		GetProcessMemoryInfo( GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmcex, sizeof(pmcex)); 

		//�ְ� �޸�
		swprintf( timeBuf, L"Peak WorkingSet : %d (MB)", (pmcex.PeakWorkingSetSize)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 520, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 

		//���� �޸�
		swprintf( timeBuf, L"Currnet WorkingSet : %d (MB)", (pmcex.WorkingSetSize)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 540, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );  

		//����޸�
		swprintf( timeBuf, L"Virtual Memory : %d (MB)", (pmcex.PrivateUsage)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 560, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );  
#else
		swprintf( timeBuf, L"PM : %d/%d (MB)", (g_MemStatus.dwTotalPhys-g_MemStatus.dwAvailPhys)/1024/1024, 
			g_MemStatus.dwTotalPhys/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 540, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		swprintf( timeBuf, L"VM : %d/%d (MB)", (g_MemStatus.dwTotalVirtual-g_MemStatus.dwAvailVirtual)/1024/1024,
			g_MemStatus.dwTotalVirtual/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 560, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
#endif CHECK_PROCESS_MEMORY_USAGE 
		swprintf( timeBuf, L"Video M : %d/%d (MB)", (g_iTotalTextureMem-g_iAvailableTextureMem)/1024/1024, g_iTotalTextureMem/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 580, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		swprintf( timeBuf, L"TM : %d (MB)", g_iAvailableTextureMem2/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 600, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pData->GetDungeonRoom() != NULL && g_pX2Game != NULL )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( pDungeonGame->GetDungeon() != NULL )
		{
			int iStageIndex = pDungeonGame->GetDungeon()->GetNowStageIndex();

			if( pDungeonGame->GetDungeon()->GetNowStage() != NULL )
			{
				int iSubStageIndex = pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStageIndex();
				swprintf( timeBuf, L"stage : %d-%d", iStageIndex, iSubStageIndex );
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 40, 620, timeBuf, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
			}
		}
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		int iLineIndex = (int)g_pX2Game->GetMyUnit()->GetSyncData().lastTouchLineIndex;

		swprintf( timeBuf, L"line : %d", iLineIndex );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 640, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		int posx = (int)g_pX2Game->GetMyUnit()->GetXPos();

		swprintf( timeBuf, L"Pos X: %d", posx );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 480, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		int posy = (int)g_pX2Game->GetMyUnit()->GetYPos();

		swprintf( timeBuf, L"Pos Y: %d", posy );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 500, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		int posz = (int)g_pX2Game->GetMyUnit()->GetZPos();

		swprintf( timeBuf, L"Pos Z: %d", posz );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 520, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pKTDXApp->GetDGManager() != NULL )
	{
		int nNum = g_pKTDXApp->GetDGManager()->GetRenderObjectChainSize();
		int nAlpha = g_pKTDXApp->GetDGManager()->GetAlphaObjectChainSize();
		int nNonAlpha = g_pKTDXApp->GetDGManager()->GetNonAlphaObjectChainSize();        
//{{ robobeg : 2008-10-18
        int nCull = g_pKTDXApp->GetDGManager()->GetCullCount();
		swprintf( timeBuf, L"obj chain(a/n): %d(%d,%d), Cull: %d", nNum, nAlpha, nNonAlpha, nCull );
//}} robobeg : 2008-10-18

        
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 660, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMajorParticle() != NULL )
	{
		int nMajorSeqNum =  static_cast<int>( g_pX2Game->GetMajorParticle()->EstimateParticleSequenceNum() );

		swprintf( timeBuf, L"major p: %d", nMajorSeqNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 680, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}


	if( g_pX2Game != NULL && g_pX2Game->GetMinorParticle() != NULL )
	{		
		int nMinorSeqNum =  static_cast<int>( g_pX2Game->GetMajorParticle()->EstimateParticleSequenceNum() );

		swprintf( timeBuf, L"minor p: %d", nMinorSeqNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 700, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}


	if( g_pX2Game != NULL && g_pX2Game->GetMajorXMeshPlayer() != NULL )
	{		
		int nMajorInstanceNum = static_cast<int>( g_pX2Game->GetMajorXMeshPlayer()->EstimateInstanceNum() );

		swprintf( timeBuf, L"major m: %d", nMajorInstanceNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 720, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMinorXMeshPlayer() != NULL )
	{		
		int nMinorInstanceNum =  static_cast<int>( g_pX2Game->GetMinorXMeshPlayer()->EstimateInstanceNum() );

		swprintf( timeBuf, L"minor m: %d", nMinorInstanceNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 740, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

#endif	
}
// CMainFrame //////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CX2ToolView
IMPLEMENT_DYNCREATE(CX2ToolView, CView)

#pragma region Message Map
BEGIN_MESSAGE_MAP(CX2ToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CX2ToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND( ID_VIEW_RENDER, OnRender )
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATEAPP()
	
	ON_COMMAND(ID_NEW_LINE, &CX2ToolView::OnNewLine)	
	ON_COMMAND(ID_OPEN_LINE, &CX2ToolView::OnLoadLineMap)
	ON_COMMAND(ID_LINE_SAVE, &CX2ToolView::OnLineSave)
	ON_COMMAND(ID_LINE_SAVEAS, &CX2ToolView::OnLineSaveas)
	ON_COMMAND(ID_OPEN_WORLD, &CX2ToolView::OnOpenWorld)
	ON_COMMAND(ID_LINE_EXPORT, &CX2ToolView::OnLineExport)
	ON_COMMAND(ID_WORLD_SAVE, &CX2ToolView::OnWorldSave)
	ON_COMMAND(ID_WORLD_SAVEAS, &CX2ToolView::OnWorldSaveas)
	ON_COMMAND(ID_NEW_WORLD, &CX2ToolView::OnNewWorld)
	ON_COMMAND(ID_LOAD_HEIGHTMAP, &CX2ToolView::OnLoadHeightmap)
	ON_COMMAND(ID_SHOW_OBJECT, &CX2ToolView::OnShowObject)
	ON_COMMAND(ID_FREEZING, &CX2ToolView::OnFreezing)
	ON_COMMAND(ID_VIEW_SELECTED, &CX2ToolView::OnViewSelected)
	ON_COMMAND(ID_VIEW_TOPFRONT, &CX2ToolView::OnViewTopfront)
	ON_COMMAND(ID_SECTOR, &CX2ToolView::OnSector)
	ON_COMMAND(ID_NEW_DUNGEON, &CX2ToolView::OnNewDungeon)
	ON_COMMAND(ID_LOAD_DUNGEON, &CX2ToolView::OnLoadDungeon)
	ON_COMMAND(ID_SAVEAS_DUNGEON, &CX2ToolView::OnSaveasDungeon)
END_MESSAGE_MAP()
#pragma endregion �޽��� ��

#pragma region CX2ToolView
CX2ToolView::CX2ToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}
#pragma endregion ������

#pragma region ~CX2ToolView
CX2ToolView::~CX2ToolView()
{
	if( g_pKTDXApp != NULL )
	{
		CKTDXApp::Exit();
		DXUTGetExitCode();
	}
}
#pragma endregion �Ҹ���

BOOL CX2ToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.style = WS_CHILDWINDOW;

	return CView::PreCreateWindow(cs);
}
// CX2ToolView �׸���

void CX2ToolView::OnDraw(CDC* /*pDC*/)
{
	CX2ToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}
// CX2ToolView �μ�

void CX2ToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CX2ToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CX2ToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CX2ToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CX2ToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CX2ToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CX2ToolView ����
#ifdef _DEBUG
void CX2ToolView::AssertValid() const
{
	CView::AssertValid();
}

void CX2ToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CX2ToolDoc* CX2ToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CX2ToolDoc)));
	return (CX2ToolDoc*)m_pDocument;
}
#endif //_DEBUG

void CX2ToolView::OnRender()
{
	if( g_pKTDXApp == NULL )
		return;

	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
		if( pToolMain->m_bRenderTopView == true )
		{
			// �Ǻ� �׸���
			pToolMain->RenderAxisView();
		}
	}

	if( DXUTGetD3DDevice() != NULL )
	{
		DXUTRender3DEnvironment();
	}

	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
		pToolMain->CreateThumbnail();
	}	
}

void CX2ToolView::OnInitialUpdate( )
{
	// Begin X2 Setting ////////////////////////////////////////////////////////////////////////////////
	if( g_pKTDXApp != NULL )
		CKTDXApp::Exit();

	g_pX2SMTPMail = new CX2SMTPMail;
	CKTDXApp::StartUp();

	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCursorSettings( true, true );

	InitApp();

	DXUTInit( false, false, true, false );
#ifdef ENABLE_MULTI_MONITOR
	DXUTSetMultimonSettings(true);
#endif ENABLE_MULTI_MONITOR

	HWND hFocus = GetSafeHwnd(); //GetTopLevelParent()->GetSafeHwnd();
	HWND hDevice = GetSafeHwnd();
	DXUTSetWindow( hFocus, hDevice, hDevice, false );

	CRect rectClient;
	GetClientRect(&rectClient);
	HRESULT hr = DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 1024, 768 ); //rectClient.Width(), rectClient.Height() );
	if( FAILED(hr) )
	{
		MessageBox(L"DXUTCreateDevice", L"Error!", MB_OK);
	}

	// End X2 Setting ////////////////////////////////////////////////////////////////////////////////	

}

void CX2ToolView::OnSize(UINT nType, int cx, int cy)
{
	DXUTStaticWndProc(GetSafeHwnd(), WM_SIZE, nType, MAKELPARAM( cx, cy ));
	DXUTStaticWndProc(GetSafeHwnd(), WM_EXITSIZEMOVE, 0, 0);
}

void CX2ToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	//DXUTStaticWndProc(GetSafeHwnd(), WM_MOUSEMOVE, 
	CView::OnMouseMove(nFlags, point);
}

int CX2ToolView::PreTranslateMessage(MSG* pMsg)
{
	DXUTStaticWndProc(GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);	
}

void CX2ToolView::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	DXUTStaticWndProc(GetSafeHwnd(), WM_ACTIVATEAPP, (WPARAM)TRUE, (LPARAM)NULL);
	CView::OnActivateApp(bActive, dwThreadID);
}

#pragma region User Define Message

#pragma region OnNewLine
void CX2ToolView::OnNewLine()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( AfxMessageBox(L"���ο� ���θ��� ����ðڽ��ϱ�?", MB_YESNO) == IDYES )
	{	
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->NewLine();
		}
	}
}
#pragma endregion ���ο� ���� �׸���

#pragma region OnLoadLineMap
void CX2ToolView::OnLoadLineMap()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path���� ������ ���� �̸�

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->LoadLineMap( ofn.lpstrFile );
			
		}		
	}	
}
#pragma endregion ���� �ҷ�����

#pragma region OnLineSave
void CX2ToolView::OnLineSave()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pMain->m_wstrLineMapFullName.c_str() );
		wstring wstrPath = wszFilePathOnly;
		if( pMain->m_wstrLineMapFullName.empty() == true || wstrPath.empty() == true )
		{
			OnLineSaveas();
			return;
		}

		pMain->SaveLineMapToFile(NULL, false);		
	}
}
#pragma endregion ���� �����ϱ�

#pragma region OnLineSaveas
void CX2ToolView::OnLineSaveas()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		CString strFilter = _T("lua script (*.lua)|*.lua|");
		CFileDialog dlg(FALSE,L"lua",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;
		wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
		pMain->SaveLineMapToFile(wstrSaveFileName.c_str(), true);
		
	}
}
#pragma endregion �� �̸����� ���� �����ϱ�

#pragma region OnOpenWorld
void CX2ToolView::OnOpenWorld()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path���� ������ ���� �̸�

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->LoadWorld( ofn.lpstrFile );
		}		
	}
}
#pragma endregion ���� ����

#pragma region OnLineExport
void CX2ToolView::OnLineExport()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		CString strFilter = _T("lua script (*.ms)|*.ms|");
		CFileDialog dlg(FALSE,L"ms",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;
		wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
		pMain->ExportLineMap( wstrSaveFileName.c_str() );
		//pMain->SaveWorldToFile(wstrSaveFileName.c_str(), true);

	}
		
// 	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
// 	if( pMain != NULL )
// 	{
// 		pMain->ExportLineMap();
// 	}		
}
#pragma endregion Line Export

#pragma region OnWorldSave
void CX2ToolView::OnWorldSave()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pMain->m_wstrWorldFullName.c_str() );
		wstring wstrPath = wszFilePathOnly;
		if( pMain->m_wstrWorldFullName.empty() == true || wstrPath.empty() == true )
		{
			OnWorldSaveas();
			return;
		}

		pMain->SaveWorldToFile(NULL, false);
	}	
}
#pragma endregion ���� ����

#pragma region OnWorldSaveas
void CX2ToolView::OnWorldSaveas()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		CString strFilter = _T("lua script (*.lua)|*.lua|");
		CFileDialog dlg(FALSE,L"lua",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;
		wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
		pMain->SaveWorldToFile(wstrSaveFileName.c_str(), true);

	}
}
#pragma endregion �� �̸����� ���� ����

#pragma region OnNewWorld
void CX2ToolView::OnNewWorld()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( AfxMessageBox(L"���ο� ���带 ����ðڽ��ϱ�?", MB_YESNO) == IDYES )
	{
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		if( pMain != NULL )
		{
			pMain->LoadWorld( L"new_create_World.lua" );
		}
	}	
}
#pragma endregion �� ���� �����

#pragma region OnLoadHeightmap
void CX2ToolView::OnLoadHeightmap()
{	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bHeightMap == false )
	{
		AfxMessageBox(L"�ɽ�Ǯ�̷� ��������� ����Ҹ��Ѱ� ��� ��� ����", MB_OK);
		return;
	}	

	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path���� ������ ���� �̸�

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"bmp\0*.bmp\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		CxImage cimage;
		string strFileName;
		ConvertWCHARToChar(strFileName, ofn.lpstrFile);
		cimage.Load( strFileName.c_str(), CXIMAGE_FORMAT_BMP );
		
		long iWidth = cimage.GetWidth();
		long iHeight = cimage.GetHeight();
		
		if( iWidth > 128 || iHeight > 128 )
		{
			AfxMessageBox(L"�����ŷ��� �ش� �ػ� �������� �ʽ��ϴ�.", MB_OK);
			return;
		}

		for(long y=0; y<iHeight; ++y)
		{
			for(long x=0; x<iWidth; ++x)
			{
				BYTE h = cimage.GetPixelGray(x, y);
				pMain->CreateHeightMap(x, y, h, iWidth, iHeight);	
			}
		}

		AfxMessageBox(L"HeightMap �ε��� �Ϸ�Ǿ����ϴ�.", MB_OK);
	}		
}
#pragma endregion ���̸� �θ���

#pragma region OnShowObject
void CX2ToolView::OnShowObject()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->SetShowWorldObject();
}
#pragma endregion ������Ʈ ���̱�

#pragma region OnFreezing
void CX2ToolView::OnFreezing()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->SetFreezingWorldObject();
}
#pragma endregion ���߱�

#pragma region OnViewSelected
void CX2ToolView::OnViewSelected()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->m_bViewSelectObject = !pMain->m_bViewSelectObject;
}
#pragma endregion View ����

#pragma region OnViewTopfront
void CX2ToolView::OnViewTopfront()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
		pMain->m_bRenderTopView = !pMain->m_bRenderTopView;
}
#pragma endregion TopFront�� ����

#pragma region OnSector
void CX2ToolView::OnSector()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	m_DlgLineSector.Create(IDD_DIALOG_SECTOR, this);
	m_DlgLineSector.ShowWindow(SW_SHOW);
}
#pragma endregion ���� ����

#pragma region OnNewDungeon
void CX2ToolView::OnNewDungeon()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		pMain->ResetGame(SEnum::DI_TEMP0);
	}
}
#pragma endregion �� ���� �����

#pragma region OnLoadDungeon
void CX2ToolView::OnLoadDungeon()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	g_pData->GetDungeonManager()->OpenScriptFile(L"ToolDungeonData.lua");

	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_wndClassView.FillClassView();
}
#pragma endregion ���� �ҷ�����

#pragma region OnSaveasDungeon
void CX2ToolView::OnSaveasDungeon()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CString strFilter = _T("lua script (*.lua)|*.lua|");
	CFileDialog dlg(FALSE,L"lua",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

	if( dlg.DoModal() != IDOK )
		return;
	wstring wstrSaveFileName = (LPCTSTR)dlg.GetPathName();
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_DlgDungeonEditor.SaveAs(wstrSaveFileName);
}
#pragma endregion ���� ����

#pragma endregion �޽��� ó����

#pragma region ResetDungeon
void CX2ToolView::ResetDungeon()
{
	// ���� ���� �Լ��� �ɼ���
}
#pragma endregion ���� �ʱ�ȭ