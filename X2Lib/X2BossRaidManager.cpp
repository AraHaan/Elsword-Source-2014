#include "StdAfx.h"
#include "X2BossRaidManager.h"

#ifdef FIELD_BOSS_RAID


namespace _CONST_RAID_MANAGER_
{
	const float UPDATE_CYCLE_TIME_CONTRIBUTION_RANKING = 10.f;  // �⿩�� ���� ���� �ֱ�
}

CX2BossRaidManager* CX2BossRaidManager::m_pManager = NULL;
/*static */CX2BossRaidManager* CX2BossRaidManager::GetInstance()
{
	if( NULL == m_pManager)
		m_pManager = new CX2BossRaidManager;

	return m_pManager;
}

/*static*/ void CX2BossRaidManager::ResetBossRaidManager()
{
	DestroyInstance();
}

/*static*/ void CX2BossRaidManager::DestroyInstance()
{
	SAFE_DELETE( m_pManager );
}

CX2BossRaidManager::CX2BossRaidManager():
m_bIsOngoingRaid( false ),
m_eCurrentRaidMapID( SEnum::VMI_INVALID ),
m_eBossRaidCreatorMapID( SEnum::VMI_INVALID ),
m_fRaidFieldClearLimitTime ( 0.f ),
m_bShowFieldClearTime(false),
//m_fBanLeftTimeFromRaidField(-1.f),
m_bSendIntrudeRestrictionNot(false)
#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
,m_fElaspedTimeCalRank(0.f)
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
//m_bSendNpcLoadReq(false)
{
	m_pFontForTime = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL );
}

CX2BossRaidManager::~CX2BossRaidManager()
{
}
void CX2BossRaidManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if( NULL == g_pMain )
		return;

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			OnFrameMove_BattleField( fTime , fElapsedTime );
		} break;
	default:
		break;
	}
}
void CX2BossRaidManager::OnFrameMove_BattleField( double fTime, float fElapsedTime )
{
	CX2BattleFieldManager BattleFieldManager = g_pData->GetBattleFieldManager();
	// Ȱ��ȭ ���� ��Ʋ�ʵ忡���� ����
	if( SEnum::VMI_INVALID != m_eCurrentRaidMapID &&
		m_eCurrentRaidMapID == BattleFieldManager.GetBattleFieldIdWhereIam() )
	{
		m_bShowFieldClearTime = true;

		// ���� ���̵� ü�� 10% ���� �� �� ���� �Ұ� ó��
		if( NULL != g_pX2Game &&
			true == g_pX2Game->IsHost() )
		{
			if( true == IsBossHPLessThen10() )
				Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ();
		}
#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
		// �⿩�� ���� ����
		m_fElaspedTimeCalRank += fElapsedTime;
		if( m_fElaspedTimeCalRank >= _CONST_RAID_MANAGER_::UPDATE_CYCLE_TIME_CONTRIBUTION_RANKING )
		{
			UpdateContributionRanking();
			m_fElaspedTimeCalRank = 0.f;
		}
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	}
	else
	{
		m_bShowFieldClearTime = false;
	}

	
	if( GetStandardLeftTime() > 0.f )
	{
		// #1 ���� �� ��� �ð� ��� : 
		// ���� �ð�����, ���̵� �ʵ� ���� �� ���� �ð� ����
		CTimeSpan tElaspedTime = CTime::GetCurrentTime() - GetReceivePacketTime();

		// #2 ���� ���� �ð� ���
		// ���� ���� �� ���� �����ð����� ���� �� ����ð� ����
		float fLeftTime = GetStandardLeftTime() - tElaspedTime.GetTimeSpan() ;

		// #3 ���� �����ð� ����
		SetRaidFieldClearLimitTime( fLeftTime ) ;

		if( fLeftTime < 0.f )
		{
			SetStandardLeftTime(0.f);
		}

#ifdef ADD_RAID_FIELD_LOG
		m_fTestLimitTime -= fElapsedTime;
#endif // ADD_RAID_FIELD_LOG
	}


}
void CX2BossRaidManager::OnFrameRender()
{	
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			OnFrameRender_BattleField();
		} break;
	default:
		break;
	}
}
/** @function : OnFrameRender_BattleField
	@Note : OnFrameRender���� ȣ�� �� �� ������Ʈ �˻� �Ѵٰ� �Ǵ��ϰ� ����.
*/
void CX2BossRaidManager::OnFrameRender_BattleField()
{
	if( true == m_bShowFieldClearTime && 
		false == static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetShowLoadUi() )
	{
		const float fRaidFieldClearLimitTime = GetRaidFieldClearLimitTime();
		if( fRaidFieldClearLimitTime > 0.f )
		{			
			// ���� �ð� ǥ��
			WCHAR wszText[64] = L"";
			int iMinute = static_cast<int>(fRaidFieldClearLimitTime / 60.f);
			int iSecond = static_cast<int>(fRaidFieldClearLimitTime-static_cast<float>(iMinute)*60.f);

			if( iSecond < 10 )
				StringCchPrintf(wszText, 64, L"%2d:0%d", iMinute, iSecond ); 
			else
				StringCchPrintf( wszText, 64, L"%2d:%2d", iMinute, iSecond );

			m_pFontForTime->OutTextXY( 512, 60, wszText, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
			// �⿩�� ���� ǥ��
			RenderRanking();
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
		}

#ifdef ADD_RAID_FIELD_LOG
		// �׽�Ʈ�� ����, ���� �����ð� ��� ��� ǥ��
		if( m_fTestLimitTime > 0.f)
		{
			WCHAR wszText[64] = L"";
			int iMinute = static_cast<int>(m_fTestLimitTime / 60.f);
			int iSecond = static_cast<int>(m_fTestLimitTime-static_cast<float>(iMinute)*60.f);

			if( iSecond < 10 )
				StringCchPrintf(wszText, 64, L"%2d:0%d", iMinute, iSecond ); 
			else
				StringCchPrintf( wszText, 64, L"%2d:%2d", iMinute, iSecond );

			m_pFontForTime->OutTextXY( 512, 100, wszText, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}
#endif // ADD_RAID_FIELD_LOG

#ifdef ADD_RAID_FIELD_LOG
		// �׽�Ʈ�� ���� ���� ü�� ǥ��
		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( GetBossNPCIDByMapID(m_eCurrentRaidMapID) );
		if( NULL != pBossUnit )
		{
			D3DCOLOR colorFont = 0xffffaa77;
			D3DCOLOR colorOutLine = 0xffff0000;
			WCHAR adBuf[200] = {0,};
			StringCchPrintfW( adBuf, ARRAY_SIZE( adBuf ), L"HP: %d/%d", (int)pBossUnit->GetNowHp(), (int)pBossUnit->GetMaxHp() );
			g_pX2Game->GetFontForUnitName()->OutProjectionText( D3DXVECTOR3(0,0,0), adBuf, 	colorFont, CKTDGFontManager::FS_SHELL, colorOutLine, NULL, DT_CENTER  );
		}
#endif // ADD_RAID_FIELD_LOG	
	}
}
bool CX2BossRaidManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT:	// �Ŵ� ���� ���̵�� ���� ����Ʈ ����
		{
			return Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT:  // �Ŵ� ���� ���̵�� ���� ����Ʈ ����
		{
			return Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BOSS_FIELD_CLOSE_PROCESS_NOT:		// �Ŵ� ���� ���̵� �ð��� ���� �ǰų� / �Ŵ� ������ ����
		{
			return Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK:	// ������ ġƮ�� ����Ʈ�� ����
		{
			return Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK: // ������ ġƮ ���赵 Ȯ��
		{
			return Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK: // ������ ġƮ ���赵 ����
		{
			return Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK: // ü�� 10% ������ �� ���� �Ұ� ����
		{
			return Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK( hWnd, uMsg, wParam, lParam );
		} break; 
	case EGS_BOSS_FIELD_RETURN_PROCESS_NOT:	// ���̵��ʵ� -> �Ϲ� �ʵ�� ������ ���ư����� ��
		{
			return Handler_EGS_BOSS_FIELD_RETURN_PROCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	default:
		return false;
		break;
	}
	return false;
}
bool CX2BossRaidManager::Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_eCurrentRaidMapID = static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iBossFieldID );
	m_eBossRaidCreatorMapID = static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iBattleFieldID );

	SetIsOngoingRaid(true);

	// ���̵� �ʵ�� ���� ����Ʈ ����Ʈ ����
	UpdateLinemapPortalEffect();

	// �Ա� ���� ���� ����
	CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_OPEN_FIELD_BOSS_RAID, true);
	wstring wstrFieldOpenTextureName = L"";
	GetFieldOpenTextureName(m_eBossRaidCreatorMapID, wstrFieldOpenTextureName);
	CX2EmblemManager::GetInstance()->SetTexture2ndName(CX2EmblemManager::EI_OPEN_FIELD_BOSS_RAID, wstrFieldOpenTextureName );

	// �Ա� Ŭ���� ���� ����
	CX2EmblemManager::GetInstance()->EndEmblem(CX2EmblemManager::EI_CLOSE_FIELD_BOSS_RAID);

	return true;
}bool CX2BossRaidManager::Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_eBossRaidCreatorMapID = static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iBattleFieldID );

	SetIsOngoingRaid(false);
	UpdateLinemapPortalEffect();

	// #NOTE Map ID �� �ʱ�ȭ�� EI_OPEN_FIELD_BOSS_RAID ���� �� �̷������� ó�� ��. 
	CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_CLOSE_FIELD_BOSS_RAID, true);
	CX2EmblemManager::GetInstance()->EndEmblem(CX2EmblemManager::EI_OPEN_FIELD_BOSS_RAID);

	return true;
}
/** @function : Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT
	@brief : �������Ͱ� �װų� / �ð��� ���� �Ǿ��� �� �޴� ��Ŷ
*/
bool CX2BossRaidManager::Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
 	if( NULL != g_pX2Game )
 	{
 		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( GetBossNPCIDByMapID(m_eCurrentRaidMapID) );
 		if( NULL != pBossUnit )
 		{
			// ü���� ����ѵ� ��Ŷ�� �޾Ҵٸ� �������� ������Ʈ�� ����
			if( 0.f < pBossUnit->GetNowHp() )
 				pBossUnit->StateChange( "FIELD_BOSS_IRON_ELTRION_ESCAPE", true);

			CX2GageManager::GetInstance()->ClearBossGageUIList();
			g_pChatBox->AddChatLog( GET_STRING(STR_ID_29065), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			SetRaidFieldClearLimitTime(0.f);
			SetStandardLeftTime(0.f);


#ifdef ADD_RAID_FIELD_LOG
			// Ȯ���� ���� �׽�Ʈ�� �α� �˾�
			if( 0.f < pBossUnit->GetNowHp() )
			{
				if( GetRaidFieldClearLimitTime() > 30.f )
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�ð��� 30���̻� ���Ҵµ� ���� ��Ŷ�� �޾ҽ��ϴ�.", g_pMain->GetNowState() );
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"��Ʈ������ óġ�Ͽ����ϴ�.", g_pMain->GetNowState() );
			}
#endif // ADD_RAID_FIELD_LOG
 		}
 	}
	return true;
}
bool CX2BossRaidManager::Handler_EGS_BOSS_FIELD_RETURN_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
		static_cast<CX2StateBattleField*>(g_pMain->GetNowState())->MoveToBeforePlaceAtRaidField();

	return true;
}
void CX2BossRaidManager::Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ()
{
	if( false == g_pMain->IsWaitingServerPacket(EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK) )
	{
		if( false == m_bSendIntrudeRestrictionNot )
		{
			g_pData->GetServerProtocol()->SendID( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ );
			g_pMain->AddServerPacket( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK ); 
		}
	}
}
bool CX2BossRaidManager::Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef ADD_RAID_FIELD_LOG
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"���� ü�� 10% ���Ϸ� ���� �Ұ� ���� ��", g_pMain->GetNowState() );
#endif // ADD_RAID_FIELD_LOG
			SetSendIntrudeRestrictionNot(true);
			return true;
		}
	}

	return true;
}

/** @function : InitRaidFieldMapID
	@brief : ���̵� �ʵ� ��Ż�� ������ MapID �ʱ�ȭ.
			 ���� ���� ���������� �����ֱ� ����, ���� ���� �������� �ʱ�ȭ ó�� ��
*/
void CX2BossRaidManager::InitRaidFieldMapID()
{
	m_eCurrentRaidMapID = SEnum::VMI_INVALID;
	m_eBossRaidCreatorMapID = SEnum::VMI_INVALID;
}
/** @function : IsActiveRaidPortal
	@brief : ���̵� ��Ż Ȱ��ȭ ���� �˻�.
*/
bool CX2BossRaidManager::IsActiveRaidPortal() const
{

	// 1. ���� ���� �ʵ忡 ��ġ�� �ִ°�
	if( CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
		return false;

	// 2. �ʵ� ���̵� �������ΰ�.
	if( false == GetIsOngoingRaid() )
		return false;

	// 3. ���� �ʵ尡 �������̵� ���� �ʵ尡 �´°�
	if( m_eBossRaidCreatorMapID != g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam() )
		return false;

	return true;
}

void CX2BossRaidManager::OpenCheat()
{
	Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT(0,0,0,0);
}
void CX2BossRaidManager::CloseCheat()
{
	Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT(0,0,0,0);
}
/** @function : UpdateLinemapPortalEffect
	@brief : ��Ż ����Ʈ ����.
			 �ش� �ʵ忡 ���̵� �ʵ� �Ա��� ������, ����Ʈ ���� �� �� ���
*/
void CX2BossRaidManager::UpdateLinemapPortalEffect()
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		// #1 ���� �ʵ尡 �������̵� �����ϴ� �ʵ��ΰ�?
		if( true ) 
		{
			// #2 �������̵� �Ա� ����Ʈ ����
			if( NULL != g_pX2Game )
			{
				CX2BattleFieldGame* pBattleFieldGame = static_cast<CX2BattleFieldGame*>(g_pX2Game);
				if( NULL != pBattleFieldGame )
				{
					if( true == GetIsOngoingRaid() )
						pBattleFieldGame->CreateRaidPotal();
					else
						pBattleFieldGame->DestoryRaidPotalParticle();
				}
			}
		}
	}
}
void CX2BossRaidManager::GetFieldOpenTextureName( IN SEnum::VILLAGE_MAP_ID eBossRaidCreatorMapID_, OUT wstring& wstrOpenTextureName_ ) const
{
	switch( eBossRaidCreatorMapID_ )
	{
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_0.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_1.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_2.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_3.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:	wstrOpenTextureName_ = L"RaidField_Open_Sander_0.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:	wstrOpenTextureName_ = L"RaidField_Open_Sander_1.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03:	wstrOpenTextureName_ = L"RaidField_Open_Sander_2.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04:	wstrOpenTextureName_ = L"RaidField_Open_Sander_3.dds"; break;
	default:
		break;
	}
}
/** @function : JoinRaidFieldProcess
	@brief : ���̵� �ʵ� ���忡 ���� ����ó��
*/
void CX2BossRaidManager::JoinFieldProcess( float fFieldClearLeftTime_ )
{
// �Ϲ� �ʵ�->�ʵ� ���� �� ó��
	// ����/���� ��ư ����
	if( NULL != g_pMain && 
		NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->UpdatePartyMenu();
	}

// ���̵� �ʵ�� ���� ���� ���� ó��
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		// ��Ī ���
		if( NULL != g_pData &&
			NULL != g_pData->GetPartyManager() )
		{
			g_pData->GetPartyManager()->Handler_EGS_CANCEL_MATCH_MAKING_REQ();
			g_pData->GetPartyManager()->Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ();
		}
	}
#ifdef ADD_RAID_FIELD_LOG
	// �׽�Ʈ�� ���� ���� ���� �ð� ����ϴ� ��ƾ ����.
	m_fTestLimitTime = fFieldClearLeftTime_;
#endif // ADD_RAID_FIELD_LOG

	// �������� �� ���̵� �ʵ� ���� �ð� ����
	SetStandardLeftTime(fFieldClearLeftTime_);

	// Npc Load REQ ��Ŷ ���� ����
	//SetSendNpcLoadReq(false);

	// ���� ���̵� �ʵ� ���� �ð� �ʱ�ȭ
	SetRaidFieldClearLimitTime(0.f);
	
	// ü�� 10% ���� �� �� ���� �Ұ� ��Ŷ ���� ���� �ʱ�ȭ
	SetSendIntrudeRestrictionNot(false);

	SetReceivePacketTime(CTime::GetCurrentTime());

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	// �⿩�� ���� �ʱ�ȭ
	m_vecContributionRanking.clear();
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
}
/** @function : IsBossHPLessThen10
	@brief : ����HP 10�ۼ�Ʈ ���Ͽ� ���� �˻�.
			 �ʵ忡 ���� ������ ���� ������ �� �� �ֱ� ������ �Լ��� ���� ��
*/

bool CX2BossRaidManager::IsBossHPLessThen10() const
{
	if( NULL != g_pX2Game )
	{
		CX2UnitManager::NPC_UNIT_ID const eBossNPCID = GetBossNPCIDByMapID(static_cast<SEnum::VILLAGE_MAP_ID>(g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam()));
		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( eBossNPCID );
		if( NULL != pBossUnit )
		{
			if( 0.1f >= (pBossUnit->GetNowHp()/pBossUnit->GetMaxHp()) )
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	return false;
}
void CX2BossRaidManager::Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ() // ���� �ʵ� ��Ż�� ����
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ kPacket;
		kPacket.m_iBattleFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK ); 
	}
	else
	{
		g_pChatBox->AddChatLog( L"�ʵ忡���� ��� �� �� �ֽ��ϴ�.", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
}
bool CX2BossRaidManager::Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}
	return true;
}
void CX2BossRaidManager::Send_EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT() // ���� �ʵ� ��Ż�� �ݱ�
{
	g_pData->GetServerProtocol()->SendID( EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );
}
// ��Ʋ �ʵ��� TotalDangerousValue�� �˾Ƴ���
void CX2BossRaidManager::Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ() 
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ kPacket;
		kPacket.m_iBattleFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK ); 
	}
	else
	{
		g_pChatBox->AddChatLog( L"�ʵ忡���� ��� �� �� �ֽ��ϴ�.", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
}
// ��Ʋ �ʵ��� TotalDangerousValue�� �˾Ƴ���
bool CX2BossRaidManager::Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( kEvent.m_byteTotalDangerousValue >= 0 )
			{
				g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_19208, "i", kEvent.m_byteTotalDangerousValue ) ),
					KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}	
			
			return true;
		}
	}
	return true;
}
// ��Ʋ �ʵ��� TotalDangerousValue�� �����ϱ� ���� ġƮ ��Ŷ
void CX2BossRaidManager::Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ( int byDangerousValue_ ) 
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ kPacket;
		kPacket.m_iTotalDangerousValue = byDangerousValue_;
		kPacket.m_iBattleFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

		// true : ����
		kPacket.m_bSet = true;

		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK ); 
	}
	else
	{
		g_pChatBox->AddChatLog( L"�ʵ忡���� ��� �� �� �ֽ��ϴ�.", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
}
bool CX2BossRaidManager::Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	if ( g_pMain->DeleteServerPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}
	return true;
}

/** @function : GetCameraIndex
	@brief : ����Ʈ ��ġ�� ���ߴ� ī�޶� �ε���
*/
int CX2BossRaidManager::GetCameraIndex( IN SEnum::VILLAGE_MAP_ID eFieldMapID_  ) const
{
	switch( eFieldMapID_ )
	{
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:	return 4; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:	return 2; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:	return 4; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:	return 5; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:	return 4; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:	return 5; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03:	return 3; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04:	return 6; break;
	default:
		return -1;
		break;
	}

	return -1;
}

/** @function : GetLoadingUIFileName
	@brief : ���� Ȱ��ȭ�� ���̵� �ʵ��� �ε� UI ���� �̸� ���
*/
void CX2BossRaidManager::GetLoadingUIFileName( OUT wstring& wstrImgLFileName_, OUT wstring& wstrImgRFileName_) const
{
	switch( m_eCurrentRaidMapID )
	{
	case SEnum::VMI_FIELD_BOSSRAID_KINGDOM_OF_NASOD:	
		wstrImgLFileName_ = L"HQ_LoadingField_Boss_Gate_01.tga";
		wstrImgRFileName_ = L"HQ_LoadingField_Boss_Gate_02.tga";
		break;
	case SEnum::VMI_FIELD_BOSSRAID_VALLEY_OF_ABYSS:	
		wstrImgLFileName_ = L"HQ_LoadingField_Boss_Gate_01.tga";
		wstrImgRFileName_ = L"HQ_LoadingField_Boss_Gate_02.tga";
		break;
	default:
		wstrImgLFileName_ = L"HQ_LoadingField_Boss_Gate_01.tga";
		wstrImgRFileName_ = L"HQ_LoadingField_Boss_Gate_02.tga";
		break;
	}
}
// ���尡 ������� ���� ä��, ������ �����Ƿ��� �� ���� ����ó��.
// TODO : ��Ʈ���� ���� ���� ���Ϳ� ���ؼ��� ó���� �Ǿ�� ��.
// TODO : ������ �ʵ�->���̵� �ʵ� ������ ���� ��.���̵� �ʵ�->�ʵ嵵 ó�� �ؾ� ��.
void CX2BossRaidManager::CreateNpcExceptProcess( const CX2UnitManager::NPC_UNIT_ID eNpcID_ ) const
{
	switch(eNpcID_)
	{
	case CX2UnitManager::NUI_FIELD_BOSS_IRON_ELTRION:
		{
			// ��Ʈ������ ������ WI_FIELD_BOSSRAID_KINGDOM_OF_NASOD������ ���� ��
			if( g_pX2Game->GetWorld()->GetWorldData()->worldID != CX2World::WI_FIELD_BOSSRAID_KINGDOM_OF_NASOD )
			{
#ifdef ADD_RAID_FIELD_LOG
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"���̵� �ʵ尡 �ƴ� ������, ��Ʈ������ ��ȯ�Ǿ����ϴ�.", g_pMain->GetNowState() );
#endif // ADD_RAID_FIELD_LOG

				if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
				{
					static_cast<CX2StateBattleField*>(g_pMain->GetNowState())->SwapWorld();

					if( NULL != g_pX2Game->GetMyUnit() )
						g_pX2Game->GetMyUnit()->InitPosition( false, 1 );
				}
			}
		} break;
	default:
		break;
	}
}
CX2UnitManager::NPC_UNIT_ID CX2BossRaidManager::GetBossNPCIDByMapID( SEnum::VILLAGE_MAP_ID eFieldMapID_ ) const
{
	switch ( eFieldMapID_ )
	{
	case SEnum::VMI_FIELD_BOSSRAID_KINGDOM_OF_NASOD: return CX2UnitManager::NUI_FIELD_BOSS_IRON_ELTRION; break;
	default:
		break;
	}

	return CX2UnitManager::NUI_NONE;
}
#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
void CX2BossRaidManager::RenderRanking() const
{
	if( false == g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() &&
		true == g_pData->GetUIManager()->GetShowUI() )
	{
		if( false == m_vecContributionRanking.empty() )
		{
			WCHAR wstrRanking[200] = {0,};
			int iIndex = 0;
			int iPosY = 48;

#pragma NOTE("���� ��ġ �ȴٸ� ���ڿ� ��Ʈ������ �̱�")
			g_pMain->GetFontForDebuf()->OutTextXY( 829, iPosY-20, L"�⿩�� ����", D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT);
			BOOST_FOREACH(const wstring& wstrUnitName, m_vecContributionRanking )
			{
				swprintf( wstrRanking, L"%d�� : %s", ++iIndex, wstrUnitName.c_str() );
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 829, iPosY, wstrRanking, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT);

				iPosY += 16;
			}
		}
	}
}
void CX2BossRaidManager::UpdateContributionRanking()
{
	bool bIsActive = true;

	// 1. ���� ���� �ʵ忡 ��ġ�� �ִ°�
	if( CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
		bIsActive = false;

	if( true == bIsActive )
	{
		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( GetBossNPCIDByMapID(m_eCurrentRaidMapID) );
		if( NULL != pBossUnit )
		{
			// ������ �� key �� �� ������ ���� 5���� �����ϱ� ���� �ڵ� ���� ��Ŵ
			std::map< float, UidType, std::greater<float> > mapDamage;


			// #1 ������ ������ 1~5�� ���ϱ�
			std::map<UidType,float> mapDamagedMap = pBossUnit->GetDamagedMap();
			std::map<UidType,float>::const_iterator iter = mapDamagedMap.begin();			
			for ( ; iter != mapDamagedMap.end(); ++iter )
			{
				mapDamage.insert( std::make_pair( iter->second, iter->first ) );
			} 

			m_vecContributionRanking.clear();			
			// #2 �������� �г��� �����ϱ�
			if( false == mapDamage.empty() )
			{
				m_vecContributionRanking.reserve( mapDamage.size() );
				std::map< float, UidType, std::greater<float> >::iterator mit = mapDamage.begin();
				for( ; mit != mapDamage.end(); ++mit )
				{
					CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( mit->second );
					if( NULL != pGUUser )
					{
						m_vecContributionRanking.push_back( pGUUser->GetUnitName() );
						if( m_vecContributionRanking.size() >= 5 )
							break;
					}
				}
			}
		}
	}
	else
	{
		m_vecContributionRanking.clear();
	}
}
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
// bool CX2BossRaidManager::IsValideNpcLoadReq() const
// {
// 	// #1 ���� �������̵� �ʵ��ΰ�?
// 	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
// 	{
// 		// #2 �̹� ��Ŷ�� ���´°�?
// 		if( true == GetSendNpcLoadReq() ) 	
// 			return false;
// 
// 		// #3 ���� �ɸ��� �ʾ����� true ����
// 		return true;
// 	}
// 
// 	// #4 �������̵� �ʵ尡 �ƴϸ� ������ true
// 	return true;
// }
#endif // FIELD_BOSS_RAID
