#include "StdAfx.h"


#include ".\x2statestartup.h"
	  
CX2StateStartUp::CX2StateStartUp(void)
#ifdef FIRST_SCREEN_TEST
:
// 2009.01.09 ���¿� : �ʱ�ȭ �ڵ�����
m_pTexutureBG(NULL)
#endif
{
#ifdef DXUT_KTDXLIB_SAFE_ON_RESET
	HRESULT hr = S_OK;
	g_pKTDXApp->SkipFrame();
#endif

#ifdef FIRST_SCREEN_TEST
	m_pTexutureBG = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Unit_BackGround.tga" );

#ifdef DXUT_KTDXLIB_SAFE_ON_RESET
	if ( DXUTGetDeviceLost() == false && DXUTGetDeviceObjectsReset() == true )
	{
		g_pKTDXApp->GetDevice()->BeginScene();
		OnFrameRender();
		g_pKTDXApp->GetDevice()->EndScene();
		hr = g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
		if( FAILED(hr) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				DXUTSetDeviceLost( true );
			}
			else if( D3DERR_DRIVERINTERNALERROR == hr )
			{
				// When D3DERR_DRIVERINTERNALERROR is returned from Present(),
				// the application can do one of the following:
				// 
				// - End, with the pop-up window saying that the application cannot continue 
				//   because of problems in the display adapter and that the user should 
				//   contact the adapter manufacturer.
				//
				// - Attempt to restart by calling IDirect3DDevice9::Reset, which is essentially the same 
				//   path as recovering from a lost device. If IDirect3DDevice9::Reset fails with 
				//   D3DERR_DRIVERINTERNALERROR, the application should end immediately with the message 
				//   that the user should contact the adapter manufacturer.
				// 
				// The framework attempts the path of resetting the device
				// 
				DXUTSetDeviceLost( true );
			}
		}
	}
#else
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
#endif
#endif //FIRST_SCREEN_TEST

	//���α׷� ���۽� ���� �����ϴ� ������Ʈ
	//�ʿ��� ���� �����͵��� ����� ���� ������Ʈ�� ����
	LoadData();

	// ���� �޸𸮰� 40MB ����
	DWORD totalTextureMem		= 0;
	DWORD availableTextureMem	= 0;
	g_pKTDXApp->GetVideoRam( totalTextureMem, availableTextureMem );
	totalTextureMem = totalTextureMem/1024/1024;
	if( totalTextureMem <= 40 )	
	{
		// ���� �޸𸮰� ���� ũ�� �����̸� �ػ󵵸� ���Ϸ�, color format�� 16bit��,
		g_pMain->GetGameOption().SetResolution( 640, 480, 16 );
	}

	g_pMain->GetGameOption().ApplyAllOption( true );

	// ���� ������ �� ������ Ǯ��ũ������ �����ϰ�
	//if( true == DXUTIsWindowed() )
	//{
	//	DXUTToggleFullScreen();
	//}
	//::SetFocus( g_pKTDXApp->GetHWND() );

#ifdef GET_ELSWORD_FOCUS
	SetForegroundWindow( g_pKTDXApp->GetHWND() );
	SetFocus( g_pKTDXApp->GetHWND() );
#endif GET_ELSWORD_FOCUS

#ifndef SERV_SERVER_TIME_GET
#ifndef X2TOOL
	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOGIN, NULL, false );
#endif
#endif SERV_SERVER_TIME_GET


//
//#ifndef _SERVICE_
//
//		AllocConsole();
//		g_fpStream = freopen("CONOUT$", "a", stdout);		// redirect printf to console
//		g_fpStreamError = freopen("CONOUT$", "a", stderr);	// redirect printf to console
//		dbg::logfile::SetLogLevel(2);
//
//#endif _SERVICE_

	
	g_pKTDXApp->SkipFrame();

}

CX2StateStartUp::~CX2StateStartUp(void)
{
}

void CX2StateStartUp::LoadData()
{
	//UDP �ʱ�ȭ
	g_pData->ResetGameUDP( g_pMain->GetGameP2PPort() );

	g_pData->ReadyCharacterMotion();

	//UI ��ƼŬ �ý��� ����
	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );

	//���� ��ƼŬ �ý��� ����
	g_pData->ResetGameMajorParticle( L"GameMajorParticle.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle2.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle3.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle4.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle5.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle6.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle7.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle8.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle9.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle10.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle11.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle12.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle13.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle14.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle15.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle16.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle17.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle18.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle19.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle20.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle21.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle22.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle23.txt" );
	
	g_pData->ResetGameMinorParticle( L"GameMinorParticle.txt" );

	//UI �޽��÷��̾�
	g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
	g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

	//���� �޽��÷��̾�
	g_pData->ResetGameMajorXMeshPlayer( L"GameMajorXMeshPlayer.txt" );
	g_pData->ResetGameMinorXMeshPlayer( L"GameMinorXMeshPlayer.txt" );


	g_pData->ResetUIEffectSet( L"UIEffectSet.lua" );
	g_pData->ResetGameEffectSet( L"EffectSet.lua" );

#ifdef INT_RESOURCE_DEVIDE
	g_pData->GetUIMajorParticle()->OpenScriptFile( L"UIMajorParticle_INT.txt" );
	//g_pData->GetUIMinorParticle()->OpenScriptFile( L"UIMinorParticle_INT.txt" ); ���� ���� ����
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle_INT.txt" );
	g_pData->GetGameMinorParticle()->OpenScriptFile( L"GameMinorParticle_INT.txt" );
	g_pData->GetUIMajorXMeshPlayer()->OpenScriptFile( L"UIMajorXMeshPlayer_INT.txt" );
	//g_pData->GetUIMinorXMeshPlayer()->OpenScriptFile( L"UIMinorXMeshPlayer_INT.txt" ); ���� ���� ����
	g_pData->GetGameMajorXMeshPlayer()->OpenScriptFile( L"GameMajorXMeshPlayer_INT.txt" );
	//g_pData->GetGameMinorXMeshPlayer()->OpenScriptFile( L"GameMinorXMeshPlayer_INT.txt" ); ���� ���� ����
	g_pData->GetUIEffectSet()->OpenScriptFile( L"UIEffectSet_INT.lua" );
	g_pData->GetGameEffectSet()->OpenScriptFile( L"EffectSet_INT.lua" );
#endif INT_RESOURCE_DEVIDE

#ifndef X2TOOL
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForTemplet();
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForFactor();
#endif

	//���� ����Ÿ �ε�
	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua", L"NPCStat.lua" );
	g_pData->ResetUIUnitManager();

	//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
	g_pData->ResetSocketItem( L"OptionItemData.lua", L"SocketOptionForm.lua" );
	//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
	
	g_pData->ResetEnchantItem();
	
	// ��Ƽ �Ŵ��� �ε�
	g_pData->ResetPartyManager();

	g_pMain->GetGameOption().OpenScriptFile();

#ifdef OPTIMIZATION_DEV_MODE
	g_pMain->GetGameOption().SetTexDetail( CX2GameOption::OL_LOW );
	g_pMain->GetGameOption().SetUnitDetail( CX2GameOption::OL_LOW );
	g_pMain->GetGameOption().SetMapDetail( CX2GameOption::OL_LOW );
	g_pMain->GetGameOption().SetFieldDetail( CX2GameOption::FL_LOW );
#endif OPTIMIZATION_DEV_MODE

	g_pData->ResetLocationManager();

	// Field_Data.lua �Ľ�
	g_pData->ResetBattleFieldManager( L"BattleFieldData.lua" );
}

#ifdef FIRST_SCREEN_TEST

HRESULT CX2StateStartUp::OnFrameRender()
{
#ifndef X2TOOL
	m_pTexutureBG->Draw( 0, 0, 1024, 768 );
#endif
	return S_OK;
}
#endif // FIRST_SCREEN_TEST
