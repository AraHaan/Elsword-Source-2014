#	include "stdafx.h"
#include ".\x2gagemanager.h"


CX2GageManager* CX2GageManager::m_pGageManager = NULL;

const CX2GageData*	CX2GageManager::CX2GageSet::GetGageData() const
{
	ASSERT( NULL != m_pGageData );
	if ( NULL != m_pGageData )
		return m_pGageData;
	else
		return NULL;
}

void CX2GageManager::CX2GageSet::UpdateGageDataFromGameUnit()
{
	if( NULL != m_pGageData )
	{
		if ( null != m_optrGameUnit )
		{
			m_pGageData->UpdateDataFromGameUnit( m_optrGameUnit->GetGageData() );

			if ( NULL != g_pX2Game && CX2Game::GT_PVP != g_pX2Game->GetGameType() )
			{
				UpdateNowHpPercent();
				UpdateNowMpPercent();
			}
		}
		else
		{
			m_pGageData->UpdateDataFromGameUnit( m_pGageData );
		}
	}
}

void CX2GageManager::CX2GageSet::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus )
{
	if ( NULL != m_pGageData )
		m_pGageData->SetMyPlayStatusFromPacket( kGamePlayStatus );
}

void CX2GageManager::CX2GageSet::UpNowHpAndMpInVillage()
{
	if ( NULL != m_pGageData )
	{
		m_pGageData->UpNowHp( m_pGageData->GetMaxHp() * 0.008f );
		UpdateNowHpPercent();

		m_pGageData->UpNowMp( 1.f );
		UpdateNowMpPercent();
	}
}



CX2GageManager::~CX2GageManager()
{
	m_vecGageSetPartyMember.clear();
	m_vecGageSetPvpMyTeam.clear();
	m_vecGageSetPvpOtherTeam.clear();
	SAFE_DELETE( m_pGageDataForRestorationToPvpGame );
}

/*static*/ CX2GageManager* CX2GageManager::GetInstance()
{
	if ( NULL == m_pGageManager )
		m_pGageManager = new CX2GageManager();

	return m_pGageManager;
}

/*static*/ void CX2GageManager::DestroyInstance()
{
	SAFE_DELETE( m_pGageManager );
}


/*static*/ void CX2GageManager::StateChange( const int iX2State_ )
{
	switch ( iX2State_ )
	{
	case CX2Main::XS_SERVER_SELECT:
		CX2GageManager::DestroyInstance();
		break;

	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_DUNGEON_RESULT:
	//case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_TRAINING_RESULT:
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();
			pGageManager->SetShow( false );
		} break;
		

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();
			pGageManager->SetShow( true );
			
		} break;

	default:
		break;
	}
}


void CX2GageManager::SetShow( const bool bShow_ )
{
	if ( NULL != m_ptrMyGageSet )
		m_ptrMyGageSet->SetShow( bShow_ );
	
	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPartyMember )
	{
		ptrGageSet->SetShow( bShow_ );
	}
}

void CX2GageManager::CreateMyGageUI( const CX2Unit::UNIT_CLASS eGameUnitClass_, const UidType uid_ )
{
	CX2GageUI*		pGageUI = NULL;
	CX2GageData*	pGageData = NULL;

	switch ( eGameUnitClass_ )
	{
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
	case CX2Unit::UC_CHUNG_IRON_CANNON:
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			pGageData = new CX2ChungGageData();
			pGageUI = new CX2ChungMyGageUI( pGageData, eGameUnitClass_ );
		} break;

	case CX2Unit::UC_ELSWORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
	case CX2Unit::UC_ELSWORD_SWORDMAN:
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			pGageData = new CX2ElswordGageData();
			pGageUI = new CX2ElswordMyGageUI( pGageData, eGameUnitClass_ );
		} break;

	case CX2Unit::UC_RAVEN_BLADE_MASTER:
	case CX2Unit::UC_RAVEN_FIGHTER:
	case CX2Unit::UC_RAVEN_OVER_TAKER:
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			pGageData = new CX2GageData();
			pGageUI = new CX2RavenMyGageUI( pGageData, eGameUnitClass_ );
		} break;

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN:
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
	case CX2Unit::UC_ARA_YAMA_RAJA:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		{
			pGageData = new CX2AraGageData();
			pGageUI = new CX2AraMyGageUI( pGageData, eGameUnitClass_ );
		} break;
#endif
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UC_ELESIS_KNIGHT:
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ


		{
			pGageData = new CX2ElesisGageData();
			pGageUI = new CX2ElesisMyGageUI( pGageData, eGameUnitClass_ );
		} break;
#endif

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CX2Unit::UC_ADD_NASOD_RULER:
	case CX2Unit::UC_ADD_PSYCHIC_TRACER:
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
#endif //SERV_ADD_LUNATIC_PSYKER
		{
			pGageData	= new CX2AddGageData();
			pGageUI		= new CX2AddMyGageUI( pGageData, eGameUnitClass_ );
		} break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //��â��
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
	case CX2Unit::UC_LIRE_COMBAT_RANGER:
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			pGageData = new CX2RenaGageData();
			pGageUI = new CX2RenaMyGageUI( pGageData, eGameUnitClass_ );
		} break;
#endif //ADD_RENA_SYSTEM

#ifdef ADD_EVE_SYSTEM_2014		// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�
	// �̺� 3���� ���� ����
	case CX2Unit::UC_EVE_NASOD:
	case CX2Unit::UC_EVE_ELECTRA:
	case CX2Unit::UC_EVE_BATTLE_SERAPH:
	case CX2Unit::UC_EVE_EXOTIC_GEAR:
	case CX2Unit::UC_EVE_CODE_NEMESIS:
	case CX2Unit::UC_EVE_ARCHITECTURE:
	case CX2Unit::UC_EVE_CODE_EMPRESS:
		{
			pGageData = new CX2GageData();
			pGageUI = new CX2EveMyGageUI( pGageData, eGameUnitClass_ );
		} break;
#endif // ADD_EVE_SYSTEM_2014	// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�
	default:
		{
			pGageData = new CX2GageData();
			pGageUI = new CX2MyGageUI( pGageData, eGameUnitClass_ );
		} break;
	}
	
	if ( NULL != pGageUI )
	{
		m_ptrMyGageSet.reset( new CX2GageSet( pGageUI, pGageData, uid_ ) );
		pGageUI->InitUI();
	}
}

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

// GageManager �� ��� UI �޽����� �̰��� ��Ĩ�ϴ�.
// �ش� �޽����� X2State �� UICustomEventProc ���� GageManager Instance �� �ҷ��� ��
// Instance ���� Call !
/*virtual*/bool CX2GageManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )	
{
	switch(wParam)
	{	
		case GUCM_BAN_FAULTY_PLAYER_BUTTON_CLICKED :		// ���� �ϱ� ��ư�� ������!
			{	
				if ( m_vecGageSetPartyMember.size() > 0 )		// m_vecGageSetPartyMember �� ���� ��
				{
					BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )		// vector �� �� �����̳ʿ� ���� 
					{
						CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );	// PartyMember �� UI �� ���´�.
					
						if ( NULL != pPartyMemberGageUI )
						{
							if ( pPartyMemberGageUI->GetVoteButtonType() == CX2PartyMemberGageUI::BVBT_NEED_VOTE )
							{
								CKTDGUIButton * pGUIButton = reinterpret_cast <CKTDGUIButton *> (lParam);

								if ( NULL != pGUIButton )
								{
									if( pPartyMemberGageUI->GetPositionIndex() == pGUIButton->GetDummyInt(0) )
									{
										if ( true == Handler_EGS_FORCED_EXIT_VOTE_REQ ( true, pPartyMemberGageUI->GetUidTypeThisPartyMember() ) )
											return true;
									}
								}
							}
						}
					}
				}
			} break;
#ifdef ERASE_BUFF_CHEAT
		case GUCM_ERASE_BUFF_CHEAT:
			{
				if( NULL != g_pX2Game && 
					NULL != g_pX2Game->GetMyUnit() )
				{
					CKTDGUIButton* pButton = reinterpret_cast<CKTDGUIButton*>(lParam);
					if( NULL != pButton )
					{
						g_pX2Game->GetMyUnit()->EraseBuffTempletFromGameUnit( static_cast<BUFF_TEMPLET_ID>(pButton->GetDummyInt(0)) );
					}
				}
			} break;
#endif // ERASE_BUFF_CHEAT
		default:
			return false;
			break;		
	}
	
	


	return false;
}


void CX2GageManager::SetAllPartyMemberVoteButtonType ( const CX2PartyMemberGageUI::BAN_VOTE_BUTTON_TYPE ButtonType_ )	// ���� �� �Լ��� ��ǥ Ÿ���� �ʱ�ȭ�ϴµ��� ����ϰ� �ֽ��ϴ�.
{
	if ( m_vecGageSetPartyMember.size() > 0 )			// �ش� ������ ����� 0 �� �Ѵ´ٸ�
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )	// �� ������ �����̳ʿ��� 
		{
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() ); // PartyMemberGageUI �� ���´�.

			if ( NULL != pPartyMemberGageUI )
			{
				pPartyMemberGageUI->SetVoteButtonType( ButtonType_ );		// ��ư Ÿ���� �������ڷ� �����Ѵ�.
			}
		}
	}
}

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*static*/ void CX2GageManager::OnFrameMoveInSpecificX2State( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
/*static*/ void CX2GageManager::OnFrameMoveInSpecificX2State()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_VILLAGE_MAP:
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();
			if ( NULL != pGageManager )
            {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                pGageManager->OnFrameMove( fElapsedTime_ );			
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				pGageManager->OnFrameMove();			
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            }
		} break;

	default:
		break;
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GageManager::OnFrameMove( float fElapsedTime_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GageManager::OnFrameMove()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
	{
		// ���� �ֱ�� HP�� MP�� ȸ��
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_ElapsedTimeCheckVillageBuff.OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_ElapsedTimeCheckVillageBuff.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		if ( m_ElapsedTimeCheckVillageBuff.CheckAndResetElapsedTime() )
			m_ptrMyGageSet->UpNowHpAndMpInVillage();
	}
	
	UpdateGageDataFromGameUnit();

	if ( NULL != m_ptrMyGageSet )
    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_ptrMyGageSet->OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_ptrMyGageSet->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    }

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPartyMember )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        ptrGageSet->OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		ptrGageSet->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	BOOST_FOREACH( CX2BossGageUIPtr ptrBossGageUI, m_vecBossGageUIList )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        ptrBossGageUI->OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		ptrBossGageUI->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpMyTeam )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        ptrGageSet->OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		ptrGageSet->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpOtherTeam )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        ptrGageSet->OnFrameMove( fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		ptrGageSet->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
}

void CX2GageManager::OnFrameRender()
{
	if ( NULL != m_ptrMyGageSet )
		m_ptrMyGageSet->OnFrameRender();

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPartyMember )
	{
		ptrGageSet->OnFrameRender();
	}

	BOOST_FOREACH( CX2BossGageUIPtr ptrBossGageUI, m_vecBossGageUIList )
	{
		ptrBossGageUI->OnFrameRender();
	}

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpMyTeam )
	{
		ptrGageSet->OnFrameRender();
	}

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpOtherTeam )
	{
		ptrGageSet->OnFrameRender();
	}
}


void CX2GageManager::SetMyGameUnit( CX2GameUnit* pMyGameUnit_ )
{
	if ( NULL != m_ptrMyGageSet )
		m_ptrMyGageSet->SetOwnerGameUnit( pMyGameUnit_ );
}

void CX2GageManager::SetPartyMemberGameUnit( const UINT uiIndex_, CX2GameUnit* pParyMemberGameUnit_ )
{
	if ( uiIndex_ >= 0 && uiIndex_ < m_vecGageSetPartyMember.size() )
	{
		CX2GageSetPtr ptrGageSet = m_vecGageSetPartyMember[uiIndex_];
		ptrGageSet->SetOwnerGameUnit( pParyMemberGameUnit_ );
	}
}
void CX2GageManager::SetPartyMemberGameUnit( CX2GameUnit* pParyMemberGameUnit_ )
{
	if ( NULL != pParyMemberGameUnit_ )
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPartyMember )
		{
			if ( ptrGageSet->GetUid() == pParyMemberGameUnit_->GetUnitUID() )
				ptrGageSet->SetOwnerGameUnit( pParyMemberGameUnit_ );
		}
	}
}


void CX2GageManager::InsertPartyMemberUI( const KPartyUserInfo& kPartyUserInfo_, CX2GameUnit* pGameUnitPartyMember_ )
{
	CX2GageData* pGageData = new CX2GageData();

	CX2GageUI* pGageUI = 
		( NULL != pGageData ? new CX2PartyMemberGageUI( kPartyUserInfo_, pGageData, m_vecGageSetPartyMember.size() ) : NULL );
	
	if ( NULL != pGageUI )
	{
		m_vecGageSetPartyMember.push_back( CX2GageSetPtr( new CX2GageSet( pGageUI, pGageData, kPartyUserInfo_.m_iUnitUID ) ) );
		
		if ( NULL != pGameUnitPartyMember_ )
			SetPartyMemberGameUnit( m_vecGageSetPartyMember.size() - 1, pGameUnitPartyMember_ );

		pGageUI->InitUI();
	}

	UpdatePartyMemberUIPosition();
}

void CX2GageManager::RemovePartyMemberUIByUserUid( const UidType uid_ )
{
	if ( !m_vecGageSetPartyMember.empty() )
	{
		auto FindUidFunc = [uid_]( CX2GageSetPtr ptrGageSet_ ) {
			return ( uid_ == ptrGageSet_->GetUid() );
		};

		vector<CX2GageSetPtr>::iterator vItr = std::find_if( m_vecGageSetPartyMember.begin(), m_vecGageSetPartyMember.end(), FindUidFunc );

		if ( m_vecGageSetPartyMember.end() != vItr )
			m_vecGageSetPartyMember.erase( vItr );
	}

	UpdatePartyMemberUIPosition();
}

void CX2GageManager::ClearPartyMemberUI()
{
	m_vecGageSetPartyMember.clear();
}


void CX2GageManager::UpdatePartyMemberUIPosition(bool bForce /*= false*/)
{
	if ( !m_vecGageSetPartyMember.empty() )
	{
		UINT uiPosindex = 0;
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )
		{
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );
						
			if ( true == bForce ||
				(NULL != pPartyMemberGageUI && pPartyMemberGageUI->GetPositionIndex() != uiPosindex) )// ������ ������ �ε����� ������ �ε����� �ٸ���
				pPartyMemberGageUI->SetPosition( uiPosindex );	// �������� ���� ��

			++uiPosindex;
		}
	}
}


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// ������ / 13-01-15 / ���� �÷��� ��, �ҷ� ���� ���� ���� �ý���


bool CX2GageManager::Handler_EGS_FORCED_EXIT_VOTE_REQ ( const bool IsWantBan_, const UidType UserUID_ )	// �����ϱ� ��ư�� ������ �� ó��
{
	if( false == g_pMain->IsWaitingServerPacket( EGS_FORCED_EXIT_VOTE_ACK ) )		// ACK �� ���� �ִٸ�
	{

		KEGS_FORCED_EXIT_VOTE_REQ kPacket;
		kPacket.m_bWantForceExit	= IsWantBan_;			// ���� ��ų ���ΰ�?
		kPacket.m_iUserUID			= UserUID_;				// ���� ��ų User �� UID

		g_pData->GetServerProtocol()->SendPacket( EGS_FORCED_EXIT_VOTE_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_FORCED_EXIT_VOTE_ACK, 60.0f );
		return true;
	}

	return false;
}

bool CX2GageManager::Handler_EGS_FORCED_EXIT_VOTE_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FORCED_EXIT_VOTE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	if ( g_pMain->DeleteServerPacket( EGS_FORCED_EXIT_VOTE_ACK ) == true )
	{
		CX2PartyMemberGageUI * pPartyMemberGageUI = GetPartyMemberDataByUID ( kEvent.m_iUserUID );	// �ش� User ID �� �޾� Party UI �� �޾ƿ´�.
		if ( NULL != pPartyMemberGageUI )
		{
			wstring wstrMessage = GET_STRING ( STR_ID_22528 );
			g_pChatBox->AddChatLog( wstrMessage.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" ); // ### ������ ��Ƽ� ƨ����ϴ�.

			pPartyMemberGageUI->SetVoteButtonType( CX2PartyMemberGageUI::BVBT_COMPLETE_VOTE );		// �ش� ��ư�� �Ϸ� ���·� �����.
		}	
		return true;
	}
	return false;
}

void CX2GageManager::ChangeButtonType ( const UidType UserUID_ )	// �����ϱ� ��ư�� ������ �� ó��
{
	CX2PartyMemberGageUI * pPartyMember = GetPartyMemberDataByUID (UserUID_);
	if ( NULL != pPartyMember )
	{
		int VoteType = pPartyMember->GetVoteButtonType();						//  ���� ����� ��ǥ ��ư Ÿ���� ����

		if ( VoteType == CX2PartyMemberGageUI::BVBT_NONE )						// ��ǥ ��ư Ÿ���� �ƹ��͵� �ƴ� ��
		{
			pPartyMember->SetVoteButtonType ( CX2PartyMemberGageUI::BVBT_NEED_VOTE );	// ��ǥ�� �ʿ��մϴ�. �� ����
		}
	}

	else	// UID �� ã�� ������ ���, ���� �ҷ����� �� ���� �ִ�.
	{
		CX2User* pMyUser = g_pData->GetMyUser();		
		if ( NULL != pMyUser )
		{
			CX2Unit* pMyUnit = pMyUser->GetSelectUnit();
			if ( NULL != pMyUnit )
			{			
				UidType MyUID = pMyUnit->GetUID();
				if ( UserUID_ == MyUID )	// �� UID �� �ҷ� ������ UID �� ���ٸ�
				{
					SetMyUserIsFaultyPlayer(true);							// ���� �ҷ� ������ �����Ѵ�.
				}
			}
		}
	}
}



void CX2GageManager::VoteAllFaultyPlayer ()							// �ҷ� ������ �ҷ� �������� �ڵ� ��ǥ
{
	if ( m_vecGageSetPartyMember.size() > 0 )			// ���Ϳ� �����Ͱ� ���� ��,
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )	// �� ������ �����̳ʿ���
		{
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );	// PartyMemberGageUI �� ���´�.
			if ( pPartyMemberGageUI->GetVoteButtonType() == CX2PartyMemberGageUI::BVBT_NEED_VOTE )		// �ش� ������ ��ǥ ��� ���̶��
			{
				Handler_EGS_FORCED_EXIT_VOTE_REQ ( true, pPartyMemberGageUI->GetUidTypeThisPartyMember() ); // �ش� ������ ��ǥ �϶�� �޽����� ������.
			}
		}
	}
}

CX2PartyMemberGageUI * CX2GageManager::GetPartyMemberDataByNickName ( const wstring & wstrName_ ) const
{
	if ( m_vecGageSetPartyMember.size() > 0 )			// ���Ϳ� �����Ͱ� ���� ��,
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )	// �� ������ �����̳ʿ���
		{
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );	// PartyMemberGageUI �� ���´�.

			if ( pPartyMemberGageUI->GetNicknameThisPartyMemeber() == wstrName_ )		// ���� PartyMember �� ID �� �������ڿ� ���ٸ�
			{
				return pPartyMemberGageUI;				// �ش� Ŭ������ ��ȯ�Ѵ�.
			}
		}
	}
	return NULL;
}

CX2PartyMemberGageUI * CX2GageManager::GetPartyMemberDataByUID ( const UidType UserID_ ) const
{
	if ( m_vecGageSetPartyMember.size() > 0 )	// ���Ϳ� �����Ͱ� ���� ��,
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )		// �� ������ �����̳ʿ���
		{
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );	// PartyMemberGageUI �� ���´�.

			if ( pPartyMemberGageUI->GetUidTypeThisPartyMember() == UserID_ )	// ���� PartyMember �� UID �� �������ڿ� ���ٸ�
			{
				return pPartyMemberGageUI;			// �ش� Ŭ������ ��ȯ�Ѵ�.
			}
		}
	}
	return NULL;
}

CX2PartyMemberGageUI * CX2GageManager::GetPartyMemberDataByIndex ( const int index_ ) const
{
	if ( m_vecGageSetPartyMember.size() > 0 )		// ���Ϳ� �����Ͱ� ���� ��,
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )		// �� ������ �����̳ʿ���
		{
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );		// PartyMemberGageUI �� ���´�.

			if ( pPartyMemberGageUI->GetPositionIndex() == index_ )	// ���� PartyMember �� UID �� �������ڿ� ���ٸ�
			{
				return pPartyMemberGageUI;		// �ش� Ŭ������ ��ȯ�Ѵ�.
			}
		}
	}
	return NULL;
}


#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM


#ifdef SKILL_SLOT_UI_TYPE_B
void CX2GageManager::ResetOtherPlayerUIPosition()
{
#ifdef FIX_SKILL_SLOT_UI_TYPE_CHANGE
	if( true == GetShowMyGageUI() )
#endif //FIX_SKILL_SLOT_UI_TYPE_CHANGE
	{
		UpdatePartyMemberUIPosition(true);
		UpdatePvpMemberUIPosition(true);	
	}
}
#endif //SKILL_SLOT_UI_TYPE_B


bool CX2GageManager::CanThisMemberInsert( const UidType uid_ )
{
	// ����� �������� ������ �ڱ� �ڽ��̸� ����Ʈ�� ���� �� ����
	if ( NULL != m_ptrMyGageSet && m_ptrMyGageSet->GetUid() == uid_ )
		return false;
	else
	{
		if ( m_vecGageSetPartyMember.empty() )
			return true;
		else
		{
			auto FindUidFunc = [uid_]( CX2GageSetPtr ptrGageSet_ ) {
				return ( uid_ == ptrGageSet_->GetUid() );
			};

			if ( m_vecGageSetPartyMember.end() == std::find_if( m_vecGageSetPartyMember.begin(), m_vecGageSetPartyMember.end(), FindUidFunc ) )
				return true;
			else
				return false;
		}
	}
	
}

void CX2GageManager::UpdateGageDataFromGameUnit()
{
	if ( NULL != m_ptrMyGageSet )
		m_ptrMyGageSet->UpdateGageDataFromGameUnit();

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPartyMember )
	{
		ptrGageSet->UpdateGageDataFromGameUnit();
	}
}

void CX2GageManager::SetMaxHp( const UidType uid_, const float fMaxHp_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetMaxHp( fMaxHp_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetMaxHp( fMaxHp_ );
				break;
			}
		}
	}
}

void CX2GageManager::SetNowHp( const UidType uid_, const float fNowHp_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetNowHp( fNowHp_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetNowHp( fNowHp_ );
				break;
			}
		}
	}
}

void CX2GageManager::SetMaxMp( const UidType uid_, const float fMaxMp_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetMaxMp( fMaxMp_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetMaxMp( fMaxMp_ );
				break;
			}
		}
	}
}

void CX2GageManager::SetNowMp( const UidType uid_, const float fNowMp_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetNowMp( fNowMp_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetNowMp( fNowMp_ );
				break;
			}
		}
	}
}

void CX2GageManager::SetMaxSoul( const UidType uid_, const float fMaxSoul_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetMaxSoul( fMaxSoul_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetMaxSoul( fMaxSoul_ );
				break;
			}
		}
	}
}

void CX2GageManager::SetNowSoul( const UidType uid_, const float fNowSoul_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetNowSoul( fNowSoul_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetNowSoul( fNowSoul_ );
				break;
			}
		}
	}
}

void CX2GageManager::SetHyperModeCount( const UidType uid_, const BYTE byHyperCount_ )
{
	if ( NULL != m_ptrMyGageSet && uid_ == m_ptrMyGageSet->GetUid() )
		m_ptrMyGageSet->SetHyperModeCount( byHyperCount_ );	
	else
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
		{
			if ( uid_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->SetHyperModeCount( byHyperCount_ );
				break;
			}
		}
	}
}

void CX2GageManager::InitMyGageWhenGameIsOver()
{
	m_ptrMyGageSet->InitMyGageWhenMovingToVillage();
}

void CX2GageManager::CreateAndInsertBossGageUI(  CX2GameUnit* pBossGameUnit_, const WCHAR* wszFaceTexName_, const WCHAR* wszFaceTexPieceName_  )
{
	if ( NULL != pBossGameUnit_ )
	{
		CX2BossGageUIPtr ptrBossGageUI( new CX2BossGageUI( pBossGameUnit_ ) );
		ptrBossGageUI->InitUI();
		ptrBossGageUI->SetBossGageTexture( wszFaceTexName_, wszFaceTexPieceName_ );

		m_vecBossGageUIList.push_back( ptrBossGageUI );
	}
}

void CX2GageManager::ShowBossGageUIByUIDAndNotShowOtherBossGageUI( const UidType uidBoss_ , bool bShow_ /*= true*/)
{
	// �ó׸�ƽ UI���¿����� �������� �ʱ�
	if( true == bShow_ )
	{
		if( g_pMain->IsInheritStateMenu() )
		{
			CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>(g_pMain->GetNowState());
			if( NULL != pStateMenu && false == pStateMenu->GetShowUI() )
			{
				return;
			}
		}
	}

	BOOST_FOREACH( CX2BossGageUIPtr val, m_vecBossGageUIList )
	{
		if ( val->IsOwnerGameUnit( uidBoss_ ) )
			val->SetShow( bShow_ );
		else
			val->SetShow( false );
	}	
}
void CX2GageManager::ShowBossGageUI( bool bShow_ )
{
	BOOST_FOREACH( CX2BossGageUIPtr val, m_vecBossGageUIList )
	{
		val->SetShow( bShow_ );
	}	
}
void CX2GageManager::ClearBossGageUIList()
{
	m_vecBossGageUIList.clear();
}

void CX2GageManager::DeleteBossGageUIByUID( const UidType uidBoss_ )
{
	if ( !m_vecBossGageUIList.empty() )
	{
		auto FindToDeleteElement = [uidBoss_]( CX2BossGageUIPtr ptrBossGageUI ) {
			return ptrBossGageUI->IsOwnerGameUnit( uidBoss_ );
		};

		std::vector<CX2BossGageUIPtr>::const_iterator cvItr = std::find_if( m_vecBossGageUIList.begin(), m_vecBossGageUIList.end(), FindToDeleteElement );
		m_vecBossGageUIList.erase( cvItr );
	}
}

const CX2GageData* CX2GageManager::GetMyGageData() const
{
	ASSERT( NULL != m_ptrMyGageSet );
	if ( NULL != m_ptrMyGageSet )
	{
		return m_ptrMyGageSet->GetGageData();
	}
	else
		return NULL;
}

CX2GageData* CX2GageManager::GetMyGageData()
{
	return const_cast<CX2GageData*>( static_cast<const CX2GageManager*>( this )->GetMyGageData() );
}

const CX2MyGageUI* CX2GageManager::GetMyGageUI() const
{
	ASSERT( NULL != m_ptrMyGageSet );
	if ( NULL != m_ptrMyGageSet )
	{
		return static_cast<CX2MyGageUI*>(m_ptrMyGageSet->GetGageUI());
	}
	else
		return NULL;
}

bool CX2GageManager::GetMyPlayStatusToPacket( OUT KGamePlayStatus& kGamePlayStatus_ ) const
{
	if ( NULL != GetMyGageData()
		 && GetMyGageData()->GetMyPlayStatusToPacket( OUT kGamePlayStatus_ ) )
	{
		GetMySkillCoolTimeListToPacket( OUT kGamePlayStatus_.m_mapSkillCoolTime );
		GetMyQuickCoolTimeListToPacket( OUT kGamePlayStatus_.m_mapQuickSlotCoolTime );
#ifdef RIDING_SYSTEM
		GetMyRidingSkillCoolTimeListToPacket( OUT kGamePlayStatus_.m_mapRidingPetCoolTime );
#endif //RIDING_SYSTEM

		return true;
	}
	else
		return false;
}

void CX2GageManager::SetMyPlayStatusFromPacket( IN const KGamePlayStatus& kGamePlayStatus_ )
{
	if ( NULL != m_ptrMyGageSet )
		m_ptrMyGageSet->SetMyPlayStatusFromPacket( kGamePlayStatus_ );

	SetMySkillCoolTimeList( kGamePlayStatus_.m_mapSkillCoolTime );
	SetMyQuickSlotCoolTimeList( kGamePlayStatus_.m_mapQuickSlotCoolTime );
#ifdef RIDING_SYSTEM
	SetMyRidingSkillCoolTimeList( kGamePlayStatus_.m_mapRidingPetCoolTime );
#endif //RIDING_SYSTEM
}

void CX2GageManager::InitMySkillCoolTimeList( const CX2UserSkillTree& userSkillTree_ )
{
	m_vecSkillCoolTime.clear();

	// B ������ ������
	if ( false == userSkillTree_.GetEnabledSkillSlotB() )
		m_vecSkillCoolTime.resize( EQUIPPED_SKILL_SLOT_COUNT );
	else // B ������ ������
		m_vecSkillCoolTime.resize( EQUIPPED_SKILL_SLOT_COUNT * 2 );
	
	// 0.0f �� �ʱ�ȭ
	fill( m_vecSkillCoolTime.begin(), m_vecSkillCoolTime.end(), 0.0f );
}

void CX2GageManager::SetMySkillCoolTimeList( const map<int, int>& mapSkillCoolTime_ )
{
	if ( mapSkillCoolTime_.empty() )
		return;

	map<int, int>::const_iterator cmItr = mapSkillCoolTime_.begin();

	while ( mapSkillCoolTime_.end() != cmItr )
	{
		const UINT iSlotIndex = cmItr->first;

		// ���� ���� ���� �ִ��� �˻�
		if ( m_vecSkillCoolTime.size() > iSlotIndex && 0 <= iSlotIndex )
			m_vecSkillCoolTime[iSlotIndex] = static_cast<float>( cmItr->second );	// ��ų ��Ÿ�� ����

		++cmItr;
	}	
}

void CX2GageManager::SetMySkillCoolTimeList( const UINT uiSlotIndex_, const float fRemainCoolTime_ )
{
	// ���� ���� ���� �ִ��� �˻�
	if ( m_vecSkillCoolTime.size() > uiSlotIndex_ && 0 <= uiSlotIndex_ )
		m_vecSkillCoolTime[uiSlotIndex_] = fRemainCoolTime_;
}

void CX2GageManager::GetMySkillCoolTimeListToPacket( OUT map<int, int>& mapSkillCoolTime_ ) const
{
	for ( UINT i = 0; i < m_vecSkillCoolTime.size(); ++i )
	{
		pair< map<int, int>::iterator, bool > pairReturn 
			= mapSkillCoolTime_.insert( pair<int, int>( i, static_cast<int>( m_vecSkillCoolTime[i] ) ) );

		// ������ �־��ٸ�
		if ( false == pairReturn.second )
			pairReturn.first->second = static_cast<int>( m_vecSkillCoolTime[i] );
	}
}

// void CX2GageManager::InitMyQuickSlotCoolTimeList( const UINT uiSizeofQuickSlot_ )
// {
// 	m_mapQuickSlotCoolTime.resize( uiSizeofQuickSlot_ );
// 
// 	// 0.0f �� �ʱ�ȭ
// 	fill( m_mapQuickSlotCoolTime.begin(), m_mapQuickSlotCoolTime.end(), 0.0f );
// }

void CX2GageManager::SetMyQuickSlotCoolTimeList( const map<int, int>& mapQuickSlotCoolTime_ )
{
	if ( mapQuickSlotCoolTime_.empty() )
		return;

	m_mapQuickSlotCoolTime = mapQuickSlotCoolTime_;	
}

void CX2GageManager::SetMyQuickSlotCoolTimeList( const map<int, CKTDXTimer>& mapQuickSlotCoolTime_ )
{
	if ( mapQuickSlotCoolTime_.empty() )
		return;

	m_mapQuickSlotCoolTime.clear();	// �������� �̷������� �ص� ����..
	map<int, CKTDXTimer>::const_iterator cmItr = mapQuickSlotCoolTime_.begin();

	while ( mapQuickSlotCoolTime_.end() != cmItr )
	{
		m_mapQuickSlotCoolTime.insert( pair<int, int>( cmItr->first, static_cast<int>( cmItr->second.elapsed() ) ) );

		++cmItr;
	}	
}

void CX2GageManager::GetMyQuickCoolTimeListToPacket( OUT map<int, int>& mapQuickCoolTime_ ) const
{
	if ( m_mapQuickSlotCoolTime.empty() )
		return;

	mapQuickCoolTime_ = m_mapQuickSlotCoolTime;
}

void CX2GageManager::SaveGageData()
{
	//ASSERT( NULL != m_pGageDataForRestorationToPvpGame );
	if ( NULL == m_pGageDataForRestorationToPvpGame )
	{
		m_pGageDataForRestorationToPvpGame = GetMyGageData()->GetCloneGageData();
	}
	m_vecTempSkillCoolTime = m_vecSkillCoolTime;
	CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;
	refUserSkillTree.ResetLeftSkillCoolTimeAll();
}

void CX2GageManager::RestoreGageData()
{
	if ( NULL != m_pGageDataForRestorationToPvpGame )
	{
		CX2GageData* pMyGageData = GetMyGageData();
		pMyGageData->CopyGageData( m_pGageDataForRestorationToPvpGame );
		
		SAFE_DELETE( m_pGageDataForRestorationToPvpGame );
	}
	if( !m_vecTempSkillCoolTime.empty() )
	{
		CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;
		refUserSkillTree.InitSkillCoolTimeFromGageManager(m_vecTempSkillCoolTime);
		
		// ������ // 2013-08-21 // m_vecSkillCoolTime�� ���� �� ������ ������ ��� ��Ÿ�� �ʱ�ȭ ���� �ʴ� ���� ������ ���� �߰�
		m_vecSkillCoolTime.assign( m_vecTempSkillCoolTime.begin(), m_vecTempSkillCoolTime.end() );

		m_vecTempSkillCoolTime.clear();
	}
}

void CX2GageManager::UpdatePartyMemberGageData( const UidType uidPartyMember_, const KPartyMemberStatus& kPartyMemberStatus_ )
{
	BOOST_FOREACH( CX2GageSetPtr ptrGageSet,m_vecGageSetPartyMember )
	{
		if ( uidPartyMember_ == ptrGageSet->GetUid() )
		{
			ptrGageSet->UpdateNowHpFromPercent( kPartyMemberStatus_.m_fHpPercent * 0.01f );
			ptrGageSet->UpdateNowMpFromPercent( kPartyMemberStatus_.m_fMpPercent * 0.01f );
		}
	}
}

#ifdef RIDING_SYSTEM
void CX2GageManager::SetMyRidingSkillCoolTimeList( const map<int, int>& mapSkillCoolTime_ )
{
	if ( mapSkillCoolTime_.empty() )
		return;

	map<int, int>::const_iterator cmItr = mapSkillCoolTime_.begin();

	while ( mapSkillCoolTime_.end() != cmItr )
	{
		const UINT iSlotIndex = cmItr->first;

		// ���� ���� ���� �ִ��� �˻�
		if ( m_vecRidingPetSkillCoolTime.size() > iSlotIndex && 0 <= iSlotIndex )
			m_vecRidingPetSkillCoolTime[iSlotIndex] = static_cast<float>( cmItr->second );	// ��ų ��Ÿ�� ����

		++cmItr;
	}
}

void CX2GageManager::SetMyRidingSkillCoolTimeList(  const UINT uiSlotIndex_, const float fRemainCoolTime_ )
{
	// ���� ���� ���� �ִ��� �˻�
	if ( m_vecRidingPetSkillCoolTime.size() > uiSlotIndex_ && 0 <= uiSlotIndex_ )
		m_vecRidingPetSkillCoolTime[uiSlotIndex_] = fRemainCoolTime_;
}

void CX2GageManager::GetMyRidingSkillCoolTimeListToPacket( OUT map<int, int>& mapSkillCoolTime_ ) const
{
	for ( UINT i = 0; i < m_vecRidingPetSkillCoolTime.size(); ++i )
	{
		pair< map<int, int>::iterator, bool > pairReturn 
			= mapSkillCoolTime_.insert( pair<int, int>( i, static_cast<int>( m_vecRidingPetSkillCoolTime[i] ) ) );

		// ������ �־��ٸ�
		if ( false == pairReturn.second )
			pairReturn.first->second = static_cast<int>( m_vecRidingPetSkillCoolTime[i] );
	}
}
#endif //RIDING_SYSTEM


//--------------------------------------------------------------------------------------------------------------------------------






















int CX2OldGageManager::m_MyTeamIndex = 0;
int CX2OldGageManager::m_OtherTeamIndex = 3;

bool CX2OldGageManager::m_sbSendReportHackUserPacket = false;


CX2OldGageManager::CX2OldGageManager(void)  
	: m_pGageUI( NULL )
{

//	m_pDLGMyUnit		= NULL;
	m_pDLGOtherUnit 	= NULL;
	m_pDLGBossGage		= NULL;

	m_bEnable		= false;
	if ( g_pX2Game != NULL )
	{
		m_bShow			= g_pX2Game->GetShowAllUnitGageBar();
	}
	else
		m_bShow			= true;

	m_fElapsedTime	= 0.0f;

	m_bCanChargeMP		= false;
	
	m_pGameUnit = NULL;

	m_Index = -2;
	
	m_bInit			= false;

	m_bHasBossGage				= false;	
	m_fRemainBossGageTime		= 0.f;		

	m_bMPLevel1		= false;
	m_bMPLevel2		= false;
	m_bMPLevel3		= false;
	
	m_OneSkillPlayCount		= 0;
	m_TwoSkillPlayCount		= 0;
	m_ThreeSkillPlayCount	= 0;

#ifdef DUNGEON_ITEM
	m_fChangeRateByItem = 0.f;
	m_fChangeRateTimeByItem = 0.f;
#endif

	m_pDlgAirGage = NULL;

#ifdef HP_RELATIVE_CHANGE_RATE
	m_bHPRelativeChangeRate = false;
#endif HP_RELATIVE_CHANGE_RATE
	//SetPingLevel( 0 );

	//{{ JHKang / ������ / 2011/01/26 / ���� HP Bar ���� ���� ����
#ifdef SERV_BOSS_GAUGE_HP_LINES
	m_usGaugeNumber = 1;
	
	m_wstrHPBar[0]	= L"_V";	// �����
	m_wstrHPBar[1]	= L"_PB";	// �Ķ���
	m_wstrHPBar[2]	= L"_B";	// �ϴû�
	m_wstrHPBar[3]	= L"_G";	// �ʷϻ�
	m_wstrHPBar[4]	= L"_YG";	// ���λ�
	m_wstrHPBar[5]	= L"_O";	// ��Ȳ��	
	m_wstrHPBar[6]	= L"";		// �����
#endif SERV_BOSS_GAUGE_HP_LINES
	//}} JHKang / ������ / 2011/01/26 / ���� HP Bar ���� ���� ����

	m_pHPGage = new Gage();
	m_pMPGage = new Gage();

	m_fSwapGageTime = 0.f;
}

CX2OldGageManager::~CX2OldGageManager(void)
{
	
	//SAFE_DELETE_DIALOG( m_pDLGMyUnit );
	SAFE_DELETE_DIALOG( m_pDLGOtherUnit );
	SAFE_DELETE_DIALOG( m_pDLGBossGage );
		
	SAFE_DELETE_DIALOG( m_pDlgAirGage );

	SAFE_DELETE(m_pHPGage);
	SAFE_DELETE(m_pMPGage);

	SAFE_DELETE( m_pGageUI );
}


void CX2OldGageManager::Init( CX2GameUnit* pGameUnit, const GAGE_UI_TYPE eGageUiType_ )
{
	if ( m_bInit == true )
		return;

	m_pGameUnit = pGameUnit;
	m_bInit = true;

	const CX2Room::SlotData* pMySlot = NULL;

	switch ( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_PVP:
		pMySlot = g_pData->GetPVPRoom()->GetMySlot();
		break;

	case CX2Game::GT_BATTLE_FIELD:
		pMySlot = g_pData->GetBattleFieldRoom()->GetMySlot();
		break;

	default:
	case CX2Game::GT_DUNGEON:
		pMySlot = g_pData->GetDungeonRoom()->GetMySlot();
		break;
	}

	CreateGageUI( eGageUiType_ );

	if ( pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER && NULL != pMySlot )
	{
		if( (int)pGameUnit->GetUnitIndex() == pMySlot->m_Index && false == pMySlot->m_bObserver )
		{
			/*
			if( pGameUnit->IsMyUnit() == true )
			{
				m_pDlgAirGage = new CKTDGUIDialog( NULL, L"DLG_UI_AIR_GAGE.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgAirGage );
				if( m_pDlgAirGage != NULL )
					m_pDlgAirGage->SetShowEnable(false, false);
			}			
			*/
		}
		else
		{

			m_pDLGOtherUnit = new CKTDGUIDialog( NULL, L"DLG_PVP_Game_Other_State.lua" );

			CKTDGUIStatic* pStaticUnitInfoGrade = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl( L"StaticOtherPlayer_Grade" );
			if ( pStaticUnitInfoGrade != NULL && pStaticUnitInfoGrade->GetPicture(0) != NULL )
			{
				CX2Unit* pUnit = NULL;

				if( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
				{
					CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pGameUnit->GetUnitUID() );
					if ( pSlotData != NULL )
					{
						pUnit = pSlotData->m_pUnit;
					}
				}

				if ( pUnit != NULL && NULL != g_pMain->GetPVPEmblem() )
				{
					CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnit->GetPvpRank() );
					if ( pPVPEmblemData != NULL )
					{
						pStaticUnitInfoGrade->SetShow( true );
						pStaticUnitInfoGrade->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
					}
				}
			}

			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOtherUnit );
		}
	}
#ifdef SERV_PVP_NEW_SYSTEM
	else
	{
		if( g_pData != NULL && g_pData->GetPVPRoom() != NULL &&
			g_pData->GetPVPRoom()->IsNpcSlot( pGameUnit->GetUnitUID() ) == true )
		{
			m_pDLGOtherUnit = new CKTDGUIDialog( NULL, L"DLG_PVP_Game_Other_State.lua" );

			CKTDGUIStatic* pStaticUnitInfoGrade = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl( L"StaticOtherPlayer_Grade" );
			if ( pStaticUnitInfoGrade != NULL && pStaticUnitInfoGrade->GetPicture(0) != NULL )
			{
				CX2Room::RoomNpcSlot *npcSlot = g_pData->GetPVPRoom()->GetNpcSlotData( pGameUnit->GetUnitUID() );
				if( npcSlot != NULL && NULL != g_pMain->GetPVPEmblem() )
				{
					CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( npcSlot->m_cRank ) );
					if ( pPVPEmblemData != NULL )
					{
						pStaticUnitInfoGrade->SetShow( true );
						pStaticUnitInfoGrade->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
					}
				}
			}

			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOtherUnit );			
		}		
	}
#endif
}

void CX2OldGageManager::SetHPTexture( const WCHAR* pTexName_ )
{
	m_pGageUI->SetHpTexture( pTexName_ );
}

void CX2OldGageManager::SetMPTexture( const WCHAR* pTexName_ )
{
	m_pGageUI->SetMpTexture( pTexName_ );
}

void CX2OldGageManager::SetShow( bool bShow )
{
	m_bShow = bShow;

	if ( m_pDLGOtherUnit != NULL )
		m_pDLGOtherUnit->SetShow( m_bShow );
}

bool CX2OldGageManager::IsMyUnit()
{
	if( NULL == g_pX2Game->GetMyUnit() )
		return false;


	if( g_pX2Game->GetMyUnit() == m_pGameUnit )
		return true;
	else
		return false;
}

void CX2OldGageManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

	if( m_pDlgAirGage != NULL &&
		m_pGameUnit != NULL && m_pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = (CX2GUUser*)m_pGameUnit;
		if ( pUser->IsMyUnit() == true && pUser->GetUnderWater() == true && pUser->GetNowHp() > 0.f )
		{	
			D3DXVECTOR3 vUserPos = pUser->GetPos();			
			vUserPos.y += 400.f;
			D3DXVECTOR2 vAirGagePos = g_pKTDXApp->GetProj3DPos( vUserPos );
			vAirGagePos.x /= g_pKTDXApp->GetResolutionScaleX();
			vAirGagePos.y /= g_pKTDXApp->GetResolutionScaleY();

			vAirGagePos.x -= 25.f;

			m_pDlgAirGage->SetPos(vAirGagePos);
			m_pDlgAirGage->SetShowEnable(true, true);

			int iGage = (int)ceil(pUser->GetOxygenGage() * 0.06f);
			if( iGage > 12 )
				iGage = 12;
			if( iGage < 0 )
				iGage = 0;
			CKTDGUIStatic* pStaticAir = (CKTDGUIStatic*)m_pDlgAirGage->GetControl( L"g_pStatic_AIR_GAGE_BAR" );
			if( pStaticAir != NULL )
			{
				WCHAR keyName[50];
				wsprintf(keyName, L"AIR_%02d", iGage);
				pStaticAir->GetPicture(0)->SetTex( L"DLG_AIR_GAGE.tga", keyName );
			}
		}
		else
		{
			m_pDlgAirGage->SetShowEnable(false, false);
		}
	}


#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif		
	if( m_pGameUnit != NULL && m_pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = (CX2GUUser*)m_pGameUnit;
		if( pUser != NULL && pUser->IsMyUnit() == true )
		{
			if( GetActiveWindow() != g_pKTDXApp->GetHWND() )
			{
				m_fSwapGageTime += fElapsedTime;
			}		
			if( m_fSwapGageTime >= 3.f )
			{			
				SwapGage();
			}
		}
	}
#if defined( _SERVICE_ )
		ELSWORD_VIRTUALIZER_END
#endif


	if( g_pKTDXApp->GetIsNowVeryfy() == true )
	{
#ifdef FIX_GAGEMANAGER01
		bool bIsVerify = true;
		bool bFindHacking = false;
		string strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Verify;

		if( m_pHPGage->fNow.Verify()						== false ||
			m_pHPGage->fMax.Verify()						== false )
		{
			bIsVerify = false;
			bFindHacking = true;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Hp_Verify;
		}
		else if( m_pMPGage->fNow.Verify()					== false ||
			m_pMPGage->fMax.Verify()					== false )
		{
			bIsVerify = false;
			bFindHacking = true;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Mp_Verify;
		}
		else if( m_ForceDownGage.fNow.Verify()			== false ||
			m_ForceDownGage.fMax.Verify()				== false )
		{
			bIsVerify = false;


			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_DownGage_Verify;
		}
#ifdef ATTACK_DELAY_GAGE
		else if( m_AttackDelayGage.fNow.Verify()		== false ||
				 m_AttackDelayGage.fMax.Verify()		== false )
		{
			bIsVerify = false;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_AttackDelay_Verify;
		}
#endif
		else if( m_pHPGage->fChangeRate.Verify()			== false )
		{
			bIsVerify = false;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Hp_ChangeRate_Verify;
		}		
		else if( m_pMPGage->fChangeRate.Verify()			== false )
		{
			bIsVerify = false;			
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_Mp_ChangeRate_Verify;
		}
		else if( m_MPChargeGage.fNow.Verify()			== false ||
				 m_MPChargeGage.fMax.Verify()			== false )
		{
			bIsVerify = false;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_MpChargeGage_Verify;
		}
		else if( m_MPChargeGage.fChangeRate.Verify()	== false )
		{
			bIsVerify = false;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_MpChangeGage_ChangeRate_Verify;
		}
		else if( m_SoulGage.fNow.Verify()				== false ||
				 m_SoulGage.fMax.Verify()				== false )
		{
			bIsVerify = false;
			bFindHacking = true;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_SoulGage_Verify;
		}
		else if( m_SoulGage.fChangeRate.Verify()		== false)
		{
			bIsVerify = false;
			strAntiHackingDesc = ANTI_HACK_STRING_AntiHacking_GageManager_SoulGage_ChangeRate_Verify;
		}

		if( bIsVerify == false )
		{			
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
				g_pKTDXApp->GetFindHacking() == false )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			}			

			if( g_pInstanceData != NULL &&
				g_pInstanceData->GetVerifyGageManagerTimer() <= 0.f )
			{				
				g_pMain->SendHackMail_DamageHistory(strAntiHackingDesc.c_str());

				g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );

				if( bFindHacking == true )
				{
					g_pKTDXApp->SetFindHacking( true );
				}
			}			

			//g_pKTDXApp->SetFindHacking( true );
			return;
		}
#endif

		if ( m_OneSkillPlayCount.Verify() == false )
		{
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			//g_pKTDXApp->SetFindHacking( true );
			return;
		}

		if ( m_TwoSkillPlayCount.Verify() == false )
		{
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			//g_pKTDXApp->SetFindHacking( true );
			return;
		}

		if ( m_ThreeSkillPlayCount.Verify() == false )
		{
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			//g_pKTDXApp->SetFindHacking( true );
			return;
		}

#ifdef DUNGEON_ITEM
		if( m_fChangeRateByItem.Verify() == false )
		{
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			//g_pKTDXApp->SetFindHacking( true );
			return;
		}
		if( m_fChangeRateTimeByItem.Verify() == false )
		{
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER )
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			//g_pKTDXApp->SetFindHacking( true );
			return;
		}
#endif
	}

	if( m_bEnable == false )
		return;

#ifdef _SERVICE_

	if ( m_sbSendReportHackUserPacket == false )
	{
		if ( m_pGameUnit != NULL )
		{
			if ( m_pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
			{
				CX2GUUser* pGUUSer = (CX2GUUser*)m_pGameUnit;
				if ( pGUUSer->IsMyUnit() == true )
				{
					bool bTraningChannel = false;

					if ( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_PVP )
					{
						std::vector<KPVPChannelInfo> vecPVPChannel = g_pMain->GetChannelList();
						for ( int i = 0; i < (int)vecPVPChannel.size(); i++ )
						{
							KPVPChannelInfo& kKPVPChannelInfo = vecPVPChannel[i];
							if ( kKPVPChannelInfo.m_iChannelID == g_pMain->GetConnectedChannelID() && 
								kKPVPChannelInfo.m_cPVPChannelClass == KPVPChannelInfo::PCC_PLAY )
							{
								bTraningChannel = true;
							}
						}
					}


					if ( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER
						&& bTraningChannel == false )
					{
						if ( m_TimerCheckSkillPlayCount.elapsed() >= 60.0f )
						{
							m_TimerCheckSkillPlayCount.restart();

							if ( m_OneSkillPlayCount >= 10 )
							{
								m_sbSendReportHackUserPacket = true;
								g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );								
								//g_pKTDXApp->SetFindHacking( true );
								//return;
								//���ƿ��� ���� 100%�ΰ� ���� �ƴѰ� �Ǹ��� �Ǹ� �̺κ��� ����.
								//MessageBox( NULL, L"asdf", L"asdfasdf", MB_OK );

								g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
							}
							else if ( m_TwoSkillPlayCount >= 7 )
							{
								m_sbSendReportHackUserPacket = true;
								g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );								
								//g_pKTDXApp->SetFindHacking( true );
								//return;

								g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
							}
							else if ( m_ThreeSkillPlayCount >= 5 )
							{
								m_sbSendReportHackUserPacket = true;
								g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
								//g_pKTDXApp->SetFindHacking( true );
								//return;

								g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
							}

							m_OneSkillPlayCount = 0;
							m_TwoSkillPlayCount = 0;
							m_ThreeSkillPlayCount = 0;
						}
					}

				}			
			}
		}
	}

#endif //_SERVICE_
	
	
	

	//{{ oasis : ����� // 2009-10-6 //
	// ���� HP ����
	if( m_pHPGage->fNow > 0)
	{
#ifdef HP_RELATIVE_CHANGE_RATE	
		if(m_bHPRelativeChangeRate)
		{
			//m_HPGage.fNow		+= m_HPGage.fChangeRate	* m_HPGage.fMax * fElapsedTime;
			// oasis907 : ����� [2011.1.24] // UpHP ��� (��ŷ ����)
			UpHP(m_pHPGage->fChangeRate * m_pHPGage->fMax * fElapsedTime);
		}
#endif HP_RELATIVE_CHANGE_RATE
		else
		{
			//m_HPGage.fNow		+= m_HPGage.fChangeRate			* fElapsedTime;
			// oasis907 : ����� [2011.1.24] // UpHP ��� (��ŷ ����)
			UpHP(m_pHPGage->fChangeRate * fElapsedTime);
		}
	}
	// ����: m_HPGage.fNow		+= m_HPGage.fChangeRate			* fElapsedTime;
	//}} oasis : ����� // 2009-10-6 //
	
#ifdef DUNGEON_ITEM
	if( m_fChangeRateTimeByItem > 0.f )
	{
		m_fChangeRateTimeByItem -= fElapsedTime;
		if( m_fChangeRateTimeByItem <= 0.f )
		{
			m_fChangeRateByItem = 0.f;
			m_fChangeRateTimeByItem = 0.f;
		}
	}

	m_pMPGage->fNow		+= ((m_pMPGage->fChangeRate + m_fChangeRateByItem) * fElapsedTime);
#else
	m_pMPGage->fNow		+= m_pMPGage->fChangeRate			* fElapsedTime;
#endif

	if( m_pHPGage->fNow <= 0)
	{
		m_pMPGage->ClearFactor();
	}	

	for(UINT i=0; i<m_pMPGage->vecFactor.size(); ++i)
	{
		GageFactor &gageFactor = m_pMPGage->vecFactor[i];
		if( gageFactor.fTime > 0.f )
		{
			m_pMPGage->fNow += gageFactor.fFactor * fElapsedTime;
			gageFactor.fTime -= fElapsedTime;

			if( gageFactor.fTime <= 0.f )
			{
				m_pMPGage->vecFactor.erase( m_pMPGage->vecFactor.begin() + i );
				--i;
			}
		}
	}

	float fTempHPNow = m_pHPGage->fNow;
	float fTempMPNow = m_pMPGage->fNow;

	CheckMaxHP();
	CheckMaxMP();

#ifdef ATTACK_DELAY_GAGE
//{{AFX
	if( m_AttackDelayGage.fNow > m_AttackDelayGage.fMax )
		m_AttackDelayGage.fNow = m_AttackDelayGage.fMax;
//}}AFX
#endif ATTACK_DELAY_GAGE	
	
	if( m_ForceDownGage.fNow > m_ForceDownGage.fMax )
		m_ForceDownGage.fNow = m_ForceDownGage.fMax;

	float fTempHPAfter = m_pHPGage->fNow;
	float fTempMPAfter = m_pMPGage->fNow;

	if( fTempHPNow < fTempHPAfter && fTempHPNow > 1.0f )
	{
		g_pKTDXApp->SetFindHacking( true );
	}
	if( fTempMPNow < fTempMPAfter && fTempMPNow > 1.0f )
	{
		g_pKTDXApp->SetFindHacking( true );
	}
	


	if( m_bCanChargeMP == true )
	{
		m_MPChargeGage.fMax = m_pMPGage->fNow;
		m_MPChargeGage.fNow	+= m_MPChargeGage.fChangeRate * fElapsedTime;

		if( m_MPChargeGage.fNow > THREE_CHARGE )
			m_MPChargeGage.fNow = THREE_CHARGE;
		else if( m_MPChargeGage.fNow > m_MPChargeGage.fMax )
			m_MPChargeGage.fNow = m_MPChargeGage.fMax;
		else if( m_MPChargeGage.fNow < 0.0f )
			m_MPChargeGage.fNow = 0.0f;

		m_bCanChargeMP = false;
	}
	else
	{
		m_MPChargeGage.fNow = 0.0f;

		m_bCanChargeMP = false;
	}

	if ( NULL != m_pGageUI )
    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_pGageUI->OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_pGageUI->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    }
}

void CX2OldGageManager::CheckMaxHP()
{
	if( m_pHPGage->fNow > m_pHPGage->fMax )
		m_pHPGage->fNow = m_pHPGage->fMax;
	else if( m_pHPGage->fNow < 0.0f )
		m_pHPGage->fNow = 0.0f;
}

void CX2OldGageManager::CheckMaxMP()
{
	if( m_pMPGage->fNow > m_pMPGage->fMax )
		m_pMPGage->fNow = m_pMPGage->fMax;
	else if( m_pMPGage->fNow < 0.0f )
		m_pMPGage->fNow = 0.0f;
}


void CX2OldGageManager::CreateGageUI( const GAGE_UI_TYPE eGageUiType_ )
{
	switch ( eGageUiType_ )
	{
	case GUT_SMALL_GAGE:
		{
			SAFE_DELETE( m_pGageUI );
			m_pGageUI = new CX2SmallGageUI( m_pGameUnit );
		} break;

	case GUT_MY_STATUS_GAGE:
		{
			ASSERT( m_pGameUnit->IsMyUnit() );
			if ( m_pGameUnit->IsMyUnit() )
			{
				SAFE_DELETE( m_pGageUI );
// 				m_pGageUI = new CX2MyGageUI( m_pGameUnit, g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
// 				if ( NULL != m_pGageUI )
// 					m_pGageUI->InitUI();
			}			
		} break;
	default:
		{
			m_pGageUI = NULL;
			// default�� ���� �������� ��� �ۿ� ����
			ASSERT( true == m_pGameUnit->IsMyUnit() );
		}
		break;
	}	
}

void CX2OldGageManager::OnFrameRender()
{

	KTDXPROFILE();

	if ( m_bShow == false )
		return;

	if( IsMyUnit() == true )
	{
		// MP / Charge
		{
			//-- MP
			ASSERT( GetMPGage()->fMax > 0.f );
			float fNow = 0.f;
			if( 0.f != GetMPGage()->fMax )
			{
				fNow = (400.0f * GetMPGage()->fNow) / GetMPGage()->fMax;
			}

			float	fChargeNum = 0.0f;
			float	fGap = 0.0f;
			
			if( GetMPChargeGage()->fMax == 0.0f )
				GetMPChargeGage()->fMax = GetMPGage()->fNow;

			float fCharge = ( (fNow - fGap) * GetMPChargeGage()->fNow ) / GetMPChargeGage()->fMax;

			//-- Charge
		}

		
	}
	
	if ( NULL != m_pGageUI )
		m_pGageUI->OnFrameRender();
	
}


/*
float CX2OldGageManager::FlushMP()
{
#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif


	if( m_fMPCharge >= THREE_CHARGE )
	{
		m_pMPGage->fNow -= m_fMPCharge;

		m_fMPCharge = THREE_CHARGE;		
		m_MPChargeGage.fNow = 0.0f;

		m_ThreeSkillPlayCount++;
	}
	else if( m_fMPCharge >= TWO_CHARGE )
	{
		m_pMPGage->fNow -= m_fMPCharge;
		m_fMPCharge = TWO_CHARGE;		
		m_MPChargeGage.fNow = 0.0f;

		m_TwoSkillPlayCount++;
	}
	else if( m_fMPCharge >= ONE_CHARGE )
	{
		m_pMPGage->fNow -= m_fMPCharge;
		m_fMPCharge = ONE_CHARGE;		
		m_MPChargeGage.fNow = 0.0f;

		m_OneSkillPlayCount++;		
	}

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return m_fMPCharge;
}
*/

bool CX2OldGageManager::FlushMP( float fMP, bool bForce )
{
	if( m_pMPGage->fNow < fMP )
		return false;

	m_pMPGage->fNow -= fMP;
	return true;

}

void CX2OldGageManager::UpSoul( float fSoul )
{
	m_SoulGage.fNow += fSoul;
	if( m_SoulGage.fNow > m_SoulGage.fMax )
		m_SoulGage.fNow = m_SoulGage.fMax;
}

bool CX2OldGageManager::FlushSoul( float fSoul )
{
	if( m_SoulGage.fNow >= fSoul )
	{
		m_SoulGage.fNow -= fSoul;
		return true;
	}
	return false;
}

bool CX2OldGageManager::DamageFlush()
{

// ���� : ���� ��ŭ ������ ����
	if( m_MPChargeGage.fNow >= ONE_CHARGE )	
	{
		m_pMPGage->fNow -= m_MPChargeGage.fNow;
		m_MPChargeGage.fNow = 0.0f;
		return true;
	}
	else
		return false;
}

void CX2OldGageManager::UpMP( float fMP )
{
	m_pMPGage->Increase( fMP, 0.f );
}


void CX2OldGageManager::UpHP( float fHP, const float hpMinimum /*= 0.f*/ )
{
	m_pHPGage->Increase( fHP, hpMinimum );
}


void CX2OldGageManager::DamagedFaceDraw( int unitClass )
{
	// kimhc // 2011-12-08 ���� PVP������ ��� ��
	KTDXPROFILE();

	if ( m_pDLGOtherUnit == NULL )
		return;
	
	CKTDGUIStatic* pStaticFace = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl( L"StaticPVPStateFace" );
	if( NULL == pStaticFace )
		return;
	
	pStaticFace->Move( D3DXVECTOR2(-RandomFloat(1.f, 4.f), RandomFloat(1.f, 4.f) ), D3DXCOLOR( 1, 0, 0, 1 ), 0.05f, true );
	pStaticFace->Move( D3DXVECTOR2( RandomFloat(1.f, 4.f),-RandomFloat(1.f, 4.f) ), D3DXCOLOR( 1, 0, 0, 1 ), 0.05f, false );
	pStaticFace->Move( D3DXVECTOR2( RandomFloat(1.f, 4.f), RandomFloat(1.f, 4.f) ), D3DXCOLOR( 1, 0, 0, 1 ), 0.05f, false );
	pStaticFace->Move( D3DXVECTOR2( -RandomFloat(1.f, 4.f),-RandomFloat(1.f, 4.f) ), D3DXCOLOR( 1, 0, 0, 1 ), 0.05f, false );
	pStaticFace->Move( D3DXVECTOR2( 0, 0 ), D3DXCOLOR( 1, 1, 1, 1 ), 0.05f, false );
}

bool CX2OldGageManager::OpenScript( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIDialog", m_pDLGOtherUnit->Get() );


    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
        return false;


	return true;

}


void CX2OldGageManager::UIFrameMove( double fTime, float fElapsedTime )
{
	if ( m_pGameUnit == NULL )
		return;

#ifdef SERV_PVP_NEW_SYSTEM
	if( m_pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER || m_pGameUnit->IsPvpBot() == true )
#else
	if( m_pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
#endif
	{
		int iMySlotIndex = -1;
		bool bObserver = false;
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_TRAINING_GAME:
			{
				if( NULL != g_pData->GetDungeonRoom() &&
					NULL != g_pData->GetDungeonRoom()->GetMySlot() )
				{
					iMySlotIndex = g_pData->GetDungeonRoom()->GetMySlot()->m_Index;
					bObserver = g_pData->GetDungeonRoom()->GetMySlot()->m_bObserver;
				}
				else
				{
					return;
				}
			} break;


		case CX2Main::XS_PVP_GAME:
			{
				if( NULL != g_pData->GetPVPRoom() &&
					NULL != g_pData->GetPVPRoom()->GetMySlot() )
				{
					iMySlotIndex = g_pData->GetPVPRoom()->GetMySlot()->m_Index;
					bObserver = g_pData->GetPVPRoom()->GetMySlot()->m_bObserver;
				}
				else
				{
					return;
				}
			} break;

		case CX2Main::XS_BATTLE_FIELD:
			{
				if( NULL != g_pData->GetBattleFieldRoom() &&
					NULL != g_pData->GetBattleFieldRoom()->GetMySlot() )
				{
					iMySlotIndex = g_pData->GetBattleFieldRoom()->GetMySlot()->m_Index;
					bObserver = g_pData->GetBattleFieldRoom()->GetMySlot()->m_bObserver;
				}
				else
				{
					ASSERT( !"Invalid" );
					return;
				}

			} break;

		default:
			{
				ASSERT( !"invalid" );
				return; 
			} break;
		}

#ifdef SERV_PVP_NEW_SYSTEM
		if( m_pGameUnit->IsPvpBot() == false && iMySlotIndex == (int)m_pGameUnit->GetUnitIndex() && bObserver == false )
#else
		if ( iMySlotIndex == (int)m_pGameUnit->GetUnitIndex() && bObserver == false )
#endif
		{
		
			if ( g_pX2Game->GetMyUnit() == NULL )
				return;

			m_Index = -1;

			// EDT�� ���� ���� �� �׸� ���� ����
			CX2GUUser* pGUUser = (CX2GUUser*)m_pGameUnit;
			if ( pGUUser->GetUnit() == NULL )
				return;


			//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
// #ifdef	NEW_CHARACTER_CHUNG
// 			// û�� ĳ�� UI�� ǥ���� ��� �ϸ�
// 			if ( 0 < m_pCannonBallUIPtr.use_count() && NULL != m_pCannonBallUIPtr->GetDLGMyUnit() )
// 			{
// 				if ( true == m_pCannonBallUIPtr->GetBerserkModeChanged() )
// 				{
// 					CKTDGUIStatic* pStaticCharPictures = static_cast<CKTDGUIStatic*>( m_pCannonBallUIPtr->GetDLGMyUnit()->GetControl( L"StaticPVPGameMyChar" ) );
// 
// 					if( NULL != pStaticCharPictures->GetPicture( 0 ) )
// 					{
// 						wstring wstrFileName;
// 						wstring wstrPieceName;
// 
// 						CX2Unit::UNIT_CLASS eClassType = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass;
// 						if ( false == m_pCannonBallUIPtr->IsBerserkMode() )
// 							CX2Data::GetCharacterImageName( wstrFileName, wstrPieceName, eClassType, CX2Data::CIT_MyGage );
// 						// ����ȭ ����̸�
// 						else
// 						{
// 							GetBerserkModeMyStateImage( wstrFileName, wstrPieceName, eClassType );
// 						}
// 
// 						pStaticCharPictures->GetPicture( 0 )->SetTex( wstrFileName.c_str(), wstrPieceName.c_str() );
// 						pStaticCharPictures->GetPicture( 0 )->SetSizeAsTextureSize();
// 
// 						m_pCannonBallUIPtr->SetBerserkModeChanged( false );
// 					}
// 				}
// 				m_pCannonBallUIPtr->CannonBallUIFrameMove();
// 			}
// #endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
		}
		else			// �ٸ� ������ ������ ���..
		{
			if ( m_pGameUnit != NULL )
			{
				int index = -1;
				CX2Room* pRoom = NULL;

				if ( NULL != g_pData->GetBattleFieldRoom() )
					pRoom = g_pData->GetBattleFieldRoom();
				else if( NULL != g_pData->GetPVPRoom() )
					pRoom = (CX2Room*) g_pData->GetPVPRoom();
				else if( NULL != g_pData->GetDungeonRoom() )
				{
					pRoom = (CX2Room*) g_pData->GetDungeonRoom();

					CKTDGUIStatic* pStaticResurrect = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl( L"StaticResurrectOtherPlayer" );
					if ( pStaticResurrect != NULL && pStaticResurrect->GetString(0) != NULL )
					{
						if ( NULL != g_pX2Game->GetMyUnit() &&
							m_pGameUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
						{
							//{{ ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� �˻� �߰�
#ifdef AUTO_PAYMENT
							if ( g_pX2Game->GetMyUnit()->GetUnit()->GetResurrectionStoneNum() <= 0 && g_pX2Game->GetMyUnit()->GetUnit()->GetAutoResStoneNum() <= 0 )
#else
							if ( g_pX2Game->GetMyUnit()->GetUnit()->GetResurrectionStoneNum() <= 0 )
#endif	//	AUTO_PAYMENT
							//}} ����� : [2009/10/20] //	�ڵ����� ��Ȱ�� �˻� �߰�
							{
								if ( m_MyTeamIndex == 3 )
								{
									pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_181 );
								}
								else if ( m_MyTeamIndex == 4 )
								{
									pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_182 );
								}
								else if ( m_MyTeamIndex == 5 )
								{
									pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_183 );
								}
							}
							else
							{
								if ( m_MyTeamIndex == 3 )
								{
									pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_184 );
								}
								else if ( m_MyTeamIndex == 4 )
								{
									pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_185 );
								}
								else if ( m_MyTeamIndex == 5 )
								{
									pStaticResurrect->GetString(0)->msg = GET_STRING( STR_ID_186 );
								}
							}

							
						}
						else
						{
							pStaticResurrect->GetString(0)->msg = L"";
						}
//{{ 09.04.27 �¿� : ��Ȱ�޽��� �� ���̰� ��. 
// ���� ���� ��Ȱ �� �� ������ �� ���� �Ŷ�� ���� ���� ���ʿ� 
// if ( m_pGameUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE ) �� �κп�
// true == pDungeonGame->GetResurrectionOperationCond() �� ������ �Բ� �߰����ִ� ������ ó���ϴ� �� �� ���� �� ����.
// �ϴ��� �ӽ� �ڵ�� ���� �ڵ带 �ǵ帮�� �ʴ� ������ ��. GetResurrectionOperationCond�� private�̱⵵ �ϰ�..
#ifdef CHILDRENS_DAY_EVENT_DUNGEON
						if(g_pData->GetDungeonRoom() != NULL )
						{
							switch(g_pData->GetDungeonRoom()->GetDungeonID())
							{
							case SEnum::DI_EVENT_KIDDAY_RUBEN:
							case SEnum::DI_EVENT_KIDDAY_ELDER:
							case SEnum::DI_EVENT_KIDDAY_BESMA:
							case SEnum::DI_EVENT_KIDDAY_ALTERA:
								{
									pStaticResurrect->SetShow(false);
								} break;

							case SEnum::DI_ELDER_HENIR_SPACE:
// 							case SEnum::DI_BESMA_HENIR_SPACE:
// 							case SEnum::DI_ALTERA_HENIR_SPACE:
// 							case SEnum::DI_FEITA_HENIR_SPACE:
// 								// kimhc // ���� ��ϸ� // 2009-10-27
// 							case SEnum::DI_VELDER_HENIR_SPACE:
// 							case SEnum::DI_HAMEL_HENIR_SPACE:
								{					
									if( (CX2Dungeon::DUNGEON_MODE) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE )
										pStaticResurrect->SetShow(false);
									else
										pStaticResurrect->SetShow(true);
								} break;
							default:
								{
									pStaticResurrect->SetShow(true);
								} break;
							}
						}
#endif CHILDRENS_DAY_EVENT_DUNGEON

					}		
				}

				CKTDGUIStatic* pStaticDie = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl( L"g_pStaticOtherPlayer_Die" );
				if ( pStaticDie != NULL )
				{
					if ( m_pGameUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
					{
						pStaticDie->SetShow( true );
					}
					else
					{
						pStaticDie->SetShow( false );
					}
				}

#ifdef SERV_PVP_NEW_SYSTEM
				UidType iUnitUid = m_pGameUnit->GetUnitUID();
				int iTeamNum = -1;
				int iLevel = 0;
				wstring wstrNickName = L"";
				bool bFindSlot = false;

				if( m_pGameUnit->IsPvpBot() == true )
				{					
					CX2Room::RoomNpcSlot *pNpcSlotData = g_pX2Room->GetNpcSlotData( iUnitUid );
					if( pNpcSlotData != NULL )
					{
						iTeamNum = m_pGameUnit->GetTeam();
						iLevel = m_pGameUnit->GetUnitLevel();
						wstrNickName = pNpcSlotData->m_wstrNpcName;
						bFindSlot = true;
					}
				}
				else
				{
					CX2Room::SlotData* pSlotData = pRoom->GetSlotData(m_pGameUnit->GetUnitIndex());
					if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
					{
						iTeamNum = pSlotData->m_TeamNum;
						iLevel = (int)pSlotData->m_pUnit->GetUnitData().m_Level;
						wstrNickName = pSlotData->m_pUnit->GetNickName();
						bFindSlot = true;				
					}						
				}
#else
				UidType iUnitUid = m_pGameUnit->GetUnitUID();
				int iTeamNum = -1;
				int iLevel = 0;
				wstring wstrNickName = L"";
				bool bFindSlot = false;
				CX2Room::SlotData* pSlotData = pRoom->GetSlotData(m_pGameUnit->GetUnitIndex());
				if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
				{
					iTeamNum = pSlotData->m_TeamNum;
					iLevel = (int)pSlotData->m_pUnit->GetUnitData().m_Level;
					wstrNickName = pSlotData->m_pUnit->GetNickName();
					bFindSlot = true;				
				}	
#endif
								
				if ( bFindSlot == true )
				{
					if ( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_PVP )
					{	
						CX2PVPRoom* pPVPRoom = (CX2PVPRoom*)pRoom;
						if ( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
						{
							index = m_OtherTeamIndex;
						}
						else
						{
							if ( pRoom->GetMySlot()->m_TeamNum == iTeamNum )
								index = m_MyTeamIndex;
							else
								index = m_OtherTeamIndex;
						}
		
					}
					else
					{
						if ( pRoom->GetMySlot()->m_TeamNum == iTeamNum )
							index = m_MyTeamIndex;
						else
							index = m_OtherTeamIndex;
					}

					

					m_Index = index;

					D3DXVECTOR2 dialogPos;

					const int widthForArcadeMode = 63;

					if ( bObserver == true )
					{
						if( g_pX2Room != NULL && g_pX2Room->GetRoomType() == CX2Room::RT_DUNGEON )
						{
							// ������ �޴��߰��� ���� ��Ƽ���� ��ġ ���� �ø�
							switch(index)
							{
							case 0:
								{
									dialogPos = D3DXVECTOR2( 460, 37 );
								} break;
							case 1:
								{
									dialogPos = D3DXVECTOR2( 648, 37 );
								} break;
							case 2:
								{
									dialogPos = D3DXVECTOR2( 836, 37 );
								} break;
							case 3:
								{
									dialogPos = D3DXVECTOR2( 836, 700-50 );
								} break;
							case 4:
								{
									dialogPos = D3DXVECTOR2( 648, 700-50 );
								} break;
							case 5:
								{
									dialogPos = D3DXVECTOR2( 460, 700-50 );
								} break;
							case 6:
								{
									dialogPos = D3DXVECTOR2( 272, 700-50 );
								} break;
							default:
								break;
							}
						}
						else
						{
							//{{ 2009.01.19 ���¿� : �ڵ����� elseif -> switch
							switch(index)
							{
							case 0:
								{
									dialogPos = D3DXVECTOR2( 460 - ( 648 - 460 ) , 37 );
								} break;
							case 1:
								{
									dialogPos = D3DXVECTOR2( 460, 37 );
								} break;
							case 2:
								{
									dialogPos = D3DXVECTOR2( 648, 37 );
								} break;
							case 3:
								{
									dialogPos = D3DXVECTOR2( 836, 37 );
								} break;
							case 4:
								{
									dialogPos = D3DXVECTOR2( 836, 700);
								} break;
							case 5:
								{
									dialogPos = D3DXVECTOR2( 648, 700 );
								} break;
							case 6:
								{
									dialogPos = D3DXVECTOR2( 460, 700 );
								} break;
							case 7:
								{
									dialogPos = D3DXVECTOR2( 272, 700 );							
								} break;
							default:
								break;
							}
						}//}} elseif -> switch

						
					}
					else
					{	
						if( g_pX2Room != NULL && g_pX2Room->GetRoomType() == CX2Room::RT_DUNGEON )
						{
							// ������ �޴��߰��� ���� ��Ƽ���� ��ġ ���� �ø�
							switch(index)
							{
							case 0:
								{
									dialogPos = D3DXVECTOR2( 460, 37 );
								} break;
							case 1:
								{
									dialogPos = D3DXVECTOR2( 648, 37 );
								} break;
							case 2:
								{
									dialogPos = D3DXVECTOR2( 836, 37 );
								} break;
							case 3:
								{
									dialogPos = D3DXVECTOR2( 836, 700-50 );
								} break;
							case 4:
								{
									dialogPos = D3DXVECTOR2( 648, 700-50 );
								} break;
							case 5:
								{
									dialogPos = D3DXVECTOR2( 460, 700-50 );
								} break;
							case 6:
								{
									dialogPos = D3DXVECTOR2( 272, 700-50 );
								} break;
							default:
								break;
							}
						}
						else
						{
							//{{ 2009.01.19 ���¿� : �ڵ����� elseif -> switch
							switch(index)
							{
							case 0:
								{
									dialogPos = D3DXVECTOR2( 460, 37 );
								} break;
							case 1:
								{
									dialogPos = D3DXVECTOR2( 648, 37 );
								} break;
							case 2:
								{
									dialogPos = D3DXVECTOR2( 836, 37 );
								} break;
							case 3:
								{
									dialogPos = D3DXVECTOR2( 836, 700);
								} break;
							case 4:
								{
									dialogPos = D3DXVECTOR2( 648, 700 );
								} break;
							case 5:
								{
									dialogPos = D3DXVECTOR2( 460, 700 );
								} break;
							case 6:
								{
									dialogPos = D3DXVECTOR2( 272, 700 );
								} break;
							default:
								break;
							}//}} elseif -> switch
						}
						
					}

					

					if ( m_pDLGOtherUnit != NULL )
						m_pDLGOtherUnit->SetPos( dialogPos );

	

					CKTDGUIStatic* pTempStatic = NULL;
					if ( m_pDLGOtherUnit != NULL )
						pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"StaticPVPStateFace");

#ifdef SERV_PVP_NEW_SYSTEM
					if( m_pGameUnit->IsPvpBot() == true )
					{
						CX2GUNPC *pNpc = (CX2GUNPC*)m_pGameUnit;
						CKTDGUIControl::CPictureData* pPicture = pTempStatic->GetPicture(0);

						wstring fileName;
						wstring pieceName;
						if( CX2Data::GetPvpNpcImageName( fileName, pieceName, pNpc->GetNPCTemplet().m_nNPCUnitID ) == true )
						{
							pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
						}	
					}
					else
					{
						CX2GUUser* pGUUser = (CX2GUUser*)m_pGameUnit;
						if ( pGUUser->GetUnit() == NULL )
							return;


						CKTDGUIControl::CPictureData* pPicture = pTempStatic->GetPicture(0);

						wstring fileName;
						wstring pieceName;
						if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pGUUser->GetUnit()->GetClass(), CX2Data::CIT_50by50 ) )
						{
							pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
						}
						else
						{
							ASSERT( !"NO" );
						}
					}
#else
					CX2GUUser* pGUUser = (CX2GUUser*)m_pGameUnit;
					if ( pGUUser->GetUnit() == NULL )
						return;


					CKTDGUIControl::CPictureData* pPicture = pTempStatic->GetPicture(0);

					wstring fileName;
					wstring pieceName;
					if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pGUUser->GetUnit()->GetClass(), CX2Data::CIT_50by50 ) )
					{
						pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
					}
					else
					{
						ASSERT( !"NO" );
					}
#endif



					if ( m_pDLGOtherUnit != NULL )
						pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"OtherPlayerHP");

					//pTempStatic->GetPicture(0)->SetShow(true);
					if ( pTempStatic != NULL )
					{
						pTempStatic->GetPicture(0)->SetSizeX( pTempStatic->GetPicture(0)->GetOriginalSize().x *
							m_pHPGage->fNow / m_pHPGage->fMax );
					}

					if ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
					{
						WCHAR arLV[8] = {0};
						if ( m_pDLGOtherUnit != NULL )
							pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerLV");

						StringCchPrintfW(arLV, ARRAY_SIZE(arLV), L"%d", iLevel );

						if ( pTempStatic != NULL )
							pTempStatic->GetString(0)->msg = arLV;

						if ( m_pDLGOtherUnit != NULL )
							pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerID");

						if ( pTempStatic != NULL )
							pTempStatic->GetString(0)->msg = wstrNickName;
					}
					else
					{
						WCHAR arLV[8] = {0};
						if ( m_pDLGOtherUnit != NULL )
							pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerLV");

						StringCchPrintfW(arLV, ARRAY_SIZE(arLV), L"", iLevel );

						if ( pTempStatic != NULL )
							pTempStatic->GetString(0)->msg = arLV;

						if ( m_pDLGOtherUnit != NULL )
							pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"StaticOtherPlayerID");

						if ( pTempStatic != NULL )
							pTempStatic->GetString(0)->msg = wstrNickName;
					}

					
#ifdef FIX_OBSERVER_MODE
					if( pRoom->GetMySlot()->m_bObserver == true ||
						pRoom->GetMySlot()->m_TeamNum ==iTeamNum ||
						( NULL != g_pX2Game->GetMyUnit() && 
						true == g_pX2Game->GetMyUnit()->GetCashItemAbility().m_bShowOppnentMP ) ||
						( NULL != g_pX2Room->GetMySlot() && g_pX2Room->GetMySlot()->m_bObserver == true ) )
#else  FIX_OBSERVER_MODE
						if( pRoom->GetMySlot()->m_bObserver == true ||
							pRoom->GetMySlot()->m_TeamNum ==iTeamNum ||
							( NULL != g_pX2Game->GetMyUnit() && 
							true == g_pX2Game->GetMyUnit()->GetCashItemAbility().m_bShowOppnentMP ) )
#endif FIX_OBSERVER_MODE
					{

						if ( m_pDLGOtherUnit != NULL )
							pTempStatic = (CKTDGUIStatic*)m_pDLGOtherUnit->GetControl(L"OtherPlayerMP");

						if ( pTempStatic != NULL )
						{
							pTempStatic->GetPicture(0)->SetShow(true);
							pTempStatic->GetPicture(0)->SetSizeX( pTempStatic->GetPicture(0)->GetOriginalSize().x *
								m_pGameUnit->GetNowMp() / m_pGameUnit->GetMaxMp() );
						}
					}

					if ( pRoom->GetMySlot()->m_bObserver == true )
					{
						if ( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_PVP )
						{
							CX2PVPRoom* pPVPRoom = (CX2PVPRoom*)pRoom;
							if ( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
							{
								m_OtherTeamIndex++;
							}
							else
							{
								if ( iTeamNum == CX2Room::TN_RED )
									m_MyTeamIndex++;
								else
									m_OtherTeamIndex++;
							}

							
						}
						else
						{
							if ( iTeamNum == CX2Room::TN_RED )
								m_MyTeamIndex++;
							else
								m_OtherTeamIndex++;
						}
					}
					else
					{
						if ( pRoom->GetMySlot()->m_TeamNum == iTeamNum )
							m_MyTeamIndex++;
						else
							m_OtherTeamIndex++;
					}
				}
			} 
		}
	}
	else if( m_pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC ) 
 	{
		CX2GUNPC* pNPC = (CX2GUNPC*) m_pGameUnit;


#ifndef RIDING_MONSTER
		// oasis907 : ����� [2010.7.19] // 
		// ���̴��� ��� HP�������� ���̵� ���� HP�������� �ٷ� ���� ����
		if(pNPC != NULL &&
			pNPC->GetRideState() == CX2GUNPC::RS_ON_RIDING &&
			pNPC->GetRideType() == CX2GUNPC::RT_RIDER &&
			pNPC->GetRideUnit() != NULL)
		{
			m_vUserUnitPos = pNPC->GetRideUnit()->GetPos();
			m_vUserUnitPos += m_vAddPos;
			m_vUserUnitPos.y += 16.f;
		}
#endif RIDING_MONSTER

		if( true == m_bHasBossGage && NULL != m_pDLGBossGage )
		{
			if( m_fRemainBossGageTime > 0.f )
			{
				//m_fRemainBossGageTime -= fElapsedTime;

				if ( m_bShow == false )
					m_pDLGBossGage->SetShow( false );
				else
					m_pDLGBossGage->SetShow( true );

				//m_pDLGBossGage->SetColor( D3DXCOLOR(1,1,1,1) );
				//m_bIsFadingBossGage = false;

				// Gauge Image
				CKTDGUIStatic* pStatic_BossState = (CKTDGUIStatic*) m_pDLGBossGage->GetControl( L"Boss_State" );
#ifndef SERV_BOSS_GAUGE_HP_LINES
				CKTDGUIControl::CPictureData* pHPBar_Left = pStatic_BossState->GetPicture(3);
				CKTDGUIControl::CPictureData* pHPBar_Middle = pStatic_BossState->GetPicture(4);
				CKTDGUIControl::CPictureData* pHPBar_Right = pStatic_BossState->GetPicture(5);

				pHPBar_Left->SetShow( true );
				pHPBar_Middle->SetShow( true );
				pHPBar_Right->SetShow( true );
#endif SERV_BOSS_GAUGE_HP_LINES

				// �⺻ ����
				const float MAGIC_LEFT_HP_BAR_LENGTH = 11.f;
				const float MAGIC_RIGHT_HP_BAR_LENGTH = 16.f;
				const float MAGIC_HP_BAR_LENGTH = 626.f + MAGIC_LEFT_HP_BAR_LENGTH + MAGIC_RIGHT_HP_BAR_LENGTH;

				//{{ JHKang / ������ / 2011/01/26 / HP ���
#ifdef SERV_BOSS_GAUGE_HP_LINES
				float fPartialHP = m_pGameUnit->GetMaxHp() / m_usGaugeNumber;
				float fHPPercent = 1.f;
				float fHPBarLength = 1.f;
				int iLineNumber = static_cast<int>(m_pGameUnit->GetNowHp() / fPartialHP);

				//{{ ��ȭ �Ǵ� HP Bar
				CKTDGUIControl::CPictureData* pHPBar_Left = pStatic_BossState->GetPicture(6);
				CKTDGUIControl::CPictureData* pHPBar_Middle = pStatic_BossState->GetPicture(7);
				CKTDGUIControl::CPictureData* pHPBar_Right = pStatic_BossState->GetPicture(8);
				//}} ��ȭ �Ǵ� HP Bar

				//{{ ���� ������ ǥ�õǴ� HP Bar
				CKTDGUIControl::CPictureData* pHPBar_Left2 = pStatic_BossState->GetPicture(3);
				CKTDGUIControl::CPictureData* pHPBar_Middle2 = pStatic_BossState->GetPicture(4);
				CKTDGUIControl::CPictureData* pHPBar_Right2 = pStatic_BossState->GetPicture(5);
				//}} ���� ������ ǥ�õǴ� HP Bar

				//{{ ���� ī����
				short sColor = 5 - ( iLineNumber - 1 ) % 6;

				if ( iLineNumber >= 1 )
				{
					fHPPercent = m_pGameUnit->GetNowHp() / fPartialHP - static_cast<float>(iLineNumber);
					fHPBarLength = MAGIC_HP_BAR_LENGTH * fHPPercent;
				}
				if ( iLineNumber == 0 )
				{
					sColor = 6;
					fHPBarLength = MAGIC_HP_BAR_LENGTH * m_pGameUnit->GetNowHp() / fPartialHP;
				}
				if ( m_usGaugeNumber == iLineNumber )	// �𰡵� �� ��ȯ�Ǵ� ���� ���
				{
					iLineNumber--;
					sColor = 5 - ( iLineNumber - 1 ) % 6;
					fHPBarLength = MAGIC_HP_BAR_LENGTH;
				}
				//}} ���� ī����
				
				wstring tempString;

				tempString += L"BOSS_HP1" + m_wstrHPBar[sColor];
				pHPBar_Left->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
				tempString.clear();
				tempString += L"BOSS_HP2" + m_wstrHPBar[sColor];
				pHPBar_Middle->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
				tempString.clear();
				tempString += L"BOSS_HP3" + m_wstrHPBar[sColor];
				pHPBar_Right->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
				tempString.clear();
				pHPBar_Left->SetShow( true );
				pHPBar_Middle->SetShow( true );
				pHPBar_Right->SetShow( true );

				if ( iLineNumber >= 1 && m_pGameUnit->GetMaxHp() != m_pGameUnit->GetNowHp() )
				{
					short sBottomBarColor = sColor + 1;
					if ( sBottomBarColor > 5 )
						sBottomBarColor = 0;
					if ( iLineNumber == 1 )
						sBottomBarColor = 6;

					tempString += L"BOSS_HP1" + m_wstrHPBar[sBottomBarColor];
					pHPBar_Left2->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
					tempString.clear();
					tempString += L"BOSS_HP2" + m_wstrHPBar[sBottomBarColor];
					pHPBar_Middle2->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
					tempString.clear();
					tempString += L"BOSS_HP3" + m_wstrHPBar[sBottomBarColor];
					pHPBar_Right2->SetTex( L"DLG_BossState.TGA", tempString.c_str() );
					tempString.clear();

					pHPBar_Left2->SetShow( true );
					pHPBar_Middle2->SetShow( true );
					pHPBar_Right2->SetShow( true );

					pHPBar_Left2->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					pHPBar_Middle2->SetSizeX( MAGIC_HP_BAR_LENGTH - MAGIC_LEFT_HP_BAR_LENGTH - MAGIC_RIGHT_HP_BAR_LENGTH );
					pHPBar_Right2->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					D3DXVECTOR2 vMiddlePos = pHPBar_Middle2->GetPos();
					D3DXVECTOR2 vPos = pHPBar_Right2->GetPos();
					vPos.x = vMiddlePos.x + MAGIC_HP_BAR_LENGTH - MAGIC_LEFT_HP_BAR_LENGTH - MAGIC_RIGHT_HP_BAR_LENGTH;
					pHPBar_Right2->SetPos( vPos );
				}
				else
				{
					pHPBar_Left2->SetShow( false );
					pHPBar_Middle2->SetShow( false );
					pHPBar_Right2->SetShow( false );
				}

				u_short usBarNumber = iLineNumber + 1;

				if ( usBarNumber > m_usGaugeNumber )
					usBarNumber = m_usGaugeNumber;

				// Bar ���� �� ���� ǥ��
				if ( m_pGameUnit->GetNowHp() <= 0 )
				{
					usBarNumber = 0;
				}
				
				// �ؽ�ó ����(������ �ؽ�ó �̿�)
				CKTDGUIControl::CPictureData* pLineNumber1		= pStatic_BossState->GetPicture(9);
				CKTDGUIControl::CPictureData* pLineNumber10		= pStatic_BossState->GetPicture(10);
				CKTDGUIControl::CPictureData* pLineNumber100	= pStatic_BossState->GetPicture(11);
				CKTDGUIControl::CPictureData* pLineX			= pStatic_BossState->GetPicture(12);

				int iNumHundred = 0;
				int iNumTen = 0;
				int iNumOne = 0;

				if ( usBarNumber >= 100 )
				{
					iNumHundred = usBarNumber / 100;
					usBarNumber = usBarNumber % 100;
					SetNumberIcon( pLineNumber100, iNumHundred );
					pLineNumber100->SetShow( true );
				}
				else
				{
					pLineNumber100->SetShow( false );
					pLineX->pos.x = pLineNumber100->pos.x;
				}
				if ( usBarNumber >= 10 )
				{
					iNumTen = usBarNumber / 10;
					SetNumberIcon( pLineNumber10, iNumTen );
					pLineNumber10->SetShow( true );
				}
				else
				{
					pLineNumber10->SetShow( false );
					pLineX->pos.x =  pLineNumber10->pos.x;
				}
				if ( usBarNumber >= 0 )
				{
					iNumOne = usBarNumber % 10;
                    SetNumberIcon( pLineNumber1, iNumOne );
					pLineNumber1->SetShow( true );
				}

				pLineX->SetShow( true );
#else
				const float fHPPercent = m_pGameUnit->GetNowHp() / m_pGameUnit->GetMaxHp();
				const float fHPBarLength = MAGIC_HP_BAR_LENGTH * fHPPercent;
#endif SERV_BOSS_GAUGE_HP_LINES
				//{{ JHKang / ������ / 2011/01/26 / HP ���

				if( fHPBarLength < MAGIC_LEFT_HP_BAR_LENGTH )
				{
					pHPBar_Left->SetSizeX( fHPBarLength );
					pHPBar_Middle->SetSizeX( 0.f );
					pHPBar_Right->SetSizeX( 0.f );

				}
				else if( fHPBarLength < MAGIC_LEFT_HP_BAR_LENGTH + MAGIC_RIGHT_HP_BAR_LENGTH )
				{
					pHPBar_Left->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					pHPBar_Middle->SetSizeX( 0 );
					pHPBar_Right->SetSizeX( fHPBarLength - MAGIC_LEFT_HP_BAR_LENGTH );
					D3DXVECTOR2 vPos = pHPBar_Left->GetPos();
					vPos.x += MAGIC_LEFT_HP_BAR_LENGTH;
					pHPBar_Right->SetPos( vPos );
				}
				else
				{
					pHPBar_Left->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					pHPBar_Middle->SetSizeX( fHPBarLength - MAGIC_LEFT_HP_BAR_LENGTH - MAGIC_RIGHT_HP_BAR_LENGTH );
					pHPBar_Right->SetSizeX( MAGIC_LEFT_HP_BAR_LENGTH );
					D3DXVECTOR2 vMiddlePos = pHPBar_Middle->GetPos();
					D3DXVECTOR2 vPos = pHPBar_Right->GetPos();
					vPos.x = vMiddlePos.x + fHPBarLength - MAGIC_LEFT_HP_BAR_LENGTH - MAGIC_RIGHT_HP_BAR_LENGTH;
					pHPBar_Right->SetPos( vPos );
				}
			}
			else
			{
				m_pDLGBossGage->SetShow( false );
			}
		}
	}
}


void CX2OldGageManager::CreateBossGage( const WCHAR* wszFaceTexName, const WCHAR* wszFaceTexPieceName )
{
	if( NULL != m_pDLGBossGage )
	{
		SAFE_DELETE_DIALOG( m_pDLGBossGage );
		m_pDLGBossGage = NULL;
	}
	m_pDLGBossGage = new CKTDGUIDialog( NULL, L"DLG_BossState_NEW.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBossGage );
	m_pDLGBossGage->SetShow( false );

	CKTDGUIStatic* pStatic_BossFace = (CKTDGUIStatic*) m_pDLGBossGage->GetControl( L"Boss_State_Face" );
	string strFaceTexName, strFaceTexPieceName;
	ConvertWCHARToChar( strFaceTexName, wszFaceTexName );
	ConvertWCHARToChar( strFaceTexPieceName, wszFaceTexPieceName );
	if ( pStatic_BossFace != NULL && pStatic_BossFace->GetPicture( 0 ) != NULL )
		pStatic_BossFace->GetPicture(0)->SetTex_LUA( strFaceTexName.c_str(), strFaceTexPieceName.c_str() );
}


void CX2OldGageManager::CrashMyStateGageUI( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor )
{
	if ( NULL != m_pGageUI )
		m_pGageUI->CrashMyStateGageUI( _min, _max, changeColor );	
}


void CX2OldGageManager::SetPingLevel( int iLevel )
{
// 	CKTDGUIStatic* pStatic_Ping = NULL;
// 
// 	if( NULL != m_pDLGMyUnit )
// 	{
// 		pStatic_Ping = (CKTDGUIStatic*) m_pDLGMyUnit->GetControl( L"UserInfo_Ping" );
// 	}
// 	else if( NULL != m_pDLGOtherUnit )
// 	{
// 		pStatic_Ping = (CKTDGUIStatic*) m_pDLGOtherUnit->GetControl( L"UserInfo_Ping" );
// 	}
// 
// 
// 	if( NULL == pStatic_Ping )
// 		return ;
// 
// 
// 
// 	pStatic_Ping->GetPicture(0)->SetShow( false );
// 	pStatic_Ping->GetPicture(1)->SetShow( false );
// 	pStatic_Ping->GetPicture(2)->SetShow( false );
// 	pStatic_Ping->GetPicture(3)->SetShow( false );
// 
// 
// 	switch( iLevel )
// 	{
// 	case 0:
// 		pStatic_Ping->GetPicture(0)->SetShow( true );
// 		break;
// 
// 	case 1:
// 		pStatic_Ping->GetPicture(1)->SetShow( true );
// 		break;
// 
// 	case 2:
// 		pStatic_Ping->GetPicture(2)->SetShow( true );
// 		break;
// 
// 	case 3:
// 		pStatic_Ping->GetPicture(3)->SetShow( true );
// 		break;
// 	}

	ASSERT( !L"�̰� ���� ����?" );

}


void CX2OldGageManager::TwinkleMPGage( float fTime )
{
	ASSERT( !L"Deprecated" );
	return;
	
}

//{{ �¿� : ������ 300�� ���� �� �����Ƿ� ���� 100(���� ���ؼ�) ���ĸ� 100 ��ġ�� �Ű��ش�
#ifdef NEW_SKILL_TREE
void CX2OldGageManager::UpdateMyManaGuageUI()
{
	ASSERT( L"Deprecated" );	
}
#endif NEW_SKILL_TREE

void CX2OldGageManager::GetBerserkModeMyStateImage( OUT wstring& wstrFileName, OUT wstring& wstrPieceName, IN const CX2Unit::UNIT_CLASS eClassType )
{
	switch ( eClassType )
	{
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		wstrFileName	= L"DLG_UI_Character04.tga";
		wstrPieceName	= L"Chung_State_Helmet";
		break;

		//{{ kimhc // 2011.1.26 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
		wstrFileName	= L"DLG_UI_Common_Texture44.tga";
		wstrPieceName	= L"F_GUARDIAN_HELMET";
		break;

	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
		wstrFileName	= L"DLG_UI_Common_Texture44.tga";
		wstrPieceName	= L"S_GUARDIAN_HELMET";
		break;
#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.26 // û 1�� ����

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		wstrFileName	= L"DLG_UI_Common_Texture61_NEW.tga";
		wstrPieceName	= L"SH_GUARDIAN_RAGE";
		break;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
		wstrFileName	= L"DLG_UI_Common_Texture56.tga";
		wstrPieceName	= L"I_PALADIN_HELMET";
		break;

	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		wstrFileName	= L"DLG_UI_Common_Texture56.tga";
		wstrPieceName	= L"D_CHASER_HELMET";
		break;
#endif CHUNG_SECOND_CLASS_CHANGE


	default:
		break;
	} // switch

}

// //{{ kimhc // 2010.12.3 // 2010-12-23 New Character CHUNG
// #ifdef	NEW_CHARACTER_CHUNG
// 
// /** @function : CreateCannonBallUI
// 	@brief : ĵ��UI Ptr �ν��Ͻ� ���� �� û�� �ڽ��� ĳ���̸� UI�� ������
// */
// void CX2OldGageManager::CreateCannonBallUI()
// {
// 	m_pCannonBallUIPtr = CCannonBallUI::CreateCannonBallUIPtr();
// 	
// 	// m_pDLGMyUnit�� ������, �� �ڽ��� ĳ���Ͱ� û�̸�
// 	//if ( NULL != m_pDLGMyUnit )
// // 	{
// // 		m_pCannonBallUIPtr->SetDLGMyUnit( m_pDLGMyUnit );
// // 	}
// 
// }
// 
// /** @function : GetUnitType
// 	@brief : � ĳ���� ���� ����
// */
// 
// CX2Unit::UNIT_TYPE CX2OldGageManager::CCannonBallUI::GetUnitType()
// { 
// 	return g_pData->GetMyUser()->GetSelectUnit()->GetType();
// }
// 
// /** @function : UpdateNumOfVerticalCannonBallUI
// 	@brief : ĳ���� �ִ� ���� ������ ���� �󸶸�ŭ�� Vertical Cannonball UI�� ����ؾ��ϴ��� �����
// */
// void CX2OldGageManager::CCannonBallUI::UpdateNumOfVerticalCannonBallUI()
// {
// 	int iNumOfCannonBallUI = ( GetMaxCannonBallCount() / NUM_OF_CANNON_BALL_PER_VERTICAL );
// 
// 	if ( 0 != ( GetMaxCannonBallCount() % NUM_OF_CANNON_BALL_PER_VERTICAL ) )
// 		iNumOfCannonBallUI += 1;
// 
// 	SetNumOfVerticalCannonBallUI( iNumOfCannonBallUI );
// }
// 
// void CX2OldGageManager::CCannonBallUI::InitializeCannonBallUI()
// {
// 	for ( int iStaticIndex = 0; iStaticIndex < GetNumOfVerticalCannonBallUI(); ++iStaticIndex )
// 	{
// 		wstringstream wstrStream;
// 		wstrStream << L"Static_Chung_Cannonball_" << iStaticIndex;
// 
// 		CKTDGUIStatic* pStaticVerticalCannonBall = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( wstrStream.str().c_str() ) );
// 
// 		ASSERT( NULL != pStaticVerticalCannonBall );
// 		if ( NULL != pStaticVerticalCannonBall )
// 		{
// 			pStaticVerticalCannonBall->SetShow( true );
// 			CannonBallUIPerVerticalUIFrameMove( pStaticVerticalCannonBall, iStaticIndex );
// 		}
// 	}
// }
// 
// /** @function : CannonBallUIFrameMove
// 	@brief : ĳ���� UI������ ���� UI�� ����
// 	@param : double fTime_, float fElapsedTime_ 
// */
// 
// void CX2OldGageManager::CCannonBallUI::CannonBallUIFrameMove()
// {
// 	// ��ȭ���� �ʾ�����
// 	if ( false == GetCannonBallChanged() )
// 		return;
// 
// 	ASSERT( NULL != m_pDLGMyUnit );
// 	if ( NULL != m_pDLGMyUnit )
// 	{
// 		SetCannonBallChanged( false );
// 
// 		for ( int iStaticIndex = 0; iStaticIndex < GetNumOfVerticalCannonBallUI(); ++iStaticIndex )
// 		{
// 			wstringstream wstrStream;
// 			wstrStream << L"Static_Chung_Cannonball_" << iStaticIndex;
// 
// 			CKTDGUIStatic* pStaticVerticalCannonBall = static_cast<CKTDGUIStatic*>( m_pDLGMyUnit->GetControl( wstrStream.str().c_str() ) );
// 
// 			ASSERT( NULL != pStaticVerticalCannonBall );
// 			if ( NULL != pStaticVerticalCannonBall )
// 			{
// 				CannonBallUIPerVerticalUIFrameMove( pStaticVerticalCannonBall, iStaticIndex );
// 			}
// 		}
// 	}
// }
// 
// /** @function :CannonBallUIPerVerticalUIFrameMove 
// 	@brief : ĳ���� ���� ���� �Ѱ� �� ����
// 	@param : CKTDGUIStatic* pStaticVertical_, (ĳ�� �Ѱ��� ���ζ��� static) int iStaticIndex_ (���° ��������)
// */
// void CX2OldGageManager::CCannonBallUI::CannonBallUIPerVerticalUIFrameMove( CKTDGUIStatic* pStaticVertical_, int iStaticIndex_ )
// {
// 	int iNumOfPicture = pStaticVertical_->GetPictureNum();
// 	// ���δ� �ּ� ĳ���� ���� (0��° ������ 0��, 1��° ������ 6�� ��)
// 	const int LOWER_CANNON_BALL_COUNT_PER_NUM_OF_LINE =	iStaticIndex_ * NUM_OF_CANNON_BALL_PER_VERTICAL;
// 
// 	// 1���� �� ������ 0��°�� ĳ���� BackGround �̹����� �����ϰ� �ֱ� ������ 0��°�� �׻� ������� �Ѵ�.
// 	for ( int iPictureIndex = 1; iPictureIndex < iNumOfPicture; ++iPictureIndex )
// 	{
// 		bool bShowPictureDataCannonBall = false;
// 
// 		// ����ȭ ���� ������ Full ������ ������ ��
// 		if ( IsBerserkMode() )
// 			bShowPictureDataCannonBall = true;
// 		// ĳ���� ������ ���δ� �ּ� ĳ�� ���� + �� ������ ĳ�� ����(������ �ε���) ���� ���ų� ������
// 		else if ( GetCannonBallCount() >= LOWER_CANNON_BALL_COUNT_PER_NUM_OF_LINE + iPictureIndex )
// 			bShowPictureDataCannonBall = true;
// 
// 		CKTDGUIControl::CPictureData* pPictureDataCannonBall = pStaticVertical_->GetPicture( iPictureIndex );
// 		ASSERT( NULL != pPictureDataCannonBall );
// 		pPictureDataCannonBall->SetShow( bShowPictureDataCannonBall );
// 	}
// }
// #endif	NEW_CHARACTER_CHUNG
// //}} kimhc // 2010.12.3 //  2010-12-23 New Character CHUNG


void CX2OldGageManager::Gage::Increase( const float increment, const float minimum /*= 0.f */ )
{
	THEMIDA_ENCODE_START

	fNow += increment;
#ifdef NO_ALL_KILL_HACKING_TEST
	m_fChanged += increment;
#endif NO_ALL_KILL_HACKING_TEST

	if( fNow > fMax )
		fNow = fMax;

	if( fNow < minimum )
		fNow = minimum;

	THEMIDA_ENCODE_END
}

/** @function	: SetNumberIcon
	@brief		: Texture ���� ���
	@date 2011/02/21
*/
#ifdef SERV_BOSS_GAUGE_HP_LINES
void CX2OldGageManager::SetNumberIcon( CKTDGUIControl::CPictureData* const pPictureData, int num )
{
	switch ( num )
	{
	case 0:
		pPictureData->SetTex( L"PC_0.dds" );
		break;

	case 1:
		pPictureData->SetTex( L"PC_1.dds" );
		break;

	case 2:
		pPictureData->SetTex( L"PC_2.dds" );
		break;

	case 3:
		pPictureData->SetTex( L"PC_3.dds" );
		break;

	case 4:
		pPictureData->SetTex( L"PC_4.dds" );
		break;

	case 5:
		pPictureData->SetTex( L"PC_5.dds" );
		break;

	case 6:
		pPictureData->SetTex( L"PC_6.dds" );
		break;

	case 7:
		pPictureData->SetTex( L"PC_7.dds" );
		break;

	case 8:
		pPictureData->SetTex( L"PC_8.dds" );
		break;

	case 9:
		pPictureData->SetTex( L"PC_9.dds" );
		break;

	default:
		ASSERT( false );
		break;
	}
}
#endif

#ifdef BUFF_ICON_UI
/** @function : PushBuff
	@brief : GameUnit���� ���ο� ������ ��ϵǾ��� �� UI��¿� ��� �Ǵ� ���� ����Ʈ�� �����ϴ� �Լ�
	@param : ������ ���ŵ� ����UID(uiUnitUID), �ش� ���� ID,�ؽ��ĸ�(BuffIconInfo_), ����,����� ����(bIsDeBuff)
*/
void CX2GageManager::PushBuff( UidType uiUnitUID, const CX2GageUI::BuffIcon& BuffIconInfo_, bool bIsDeBuff)
{	
	if( uiUnitUID == m_ptrMyGageSet->GetUid() )
	{
		m_ptrMyGageSet->GetGageUI()->PushBuff(BuffIconInfo_, bIsDeBuff);
		return;
	}
	vector<CX2GageSetPtr>::iterator it = m_vecGageSetPartyMember.begin();
	for( ; it != m_vecGageSetPartyMember.end(); ++it)
	{
		if( NULL != (*it) && uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->PushBuff(BuffIconInfo_, bIsDeBuff);
			return;
		}
	}
	it = m_vecGageSetPvpMyTeam.begin();
	for( ; it != m_vecGageSetPvpMyTeam.end(); ++it)
	{
		if( NULL != (*it) && uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->PushBuff(BuffIconInfo_, bIsDeBuff);
			return;
		}
	}
}

/** @function : EraseBuff
	@brief : GameUnit���� ������ ���ŵ� �� UI��¿� ��� �Ǵ� ���� ����Ʈ�� �����ϴ� �Լ�
	@param : ������ ���ŵ� ����UID(uiUnitUID), �ش� ���� ID(eBuffID_), ����,����� ����(bIsDeBuff)
*/
void CX2GageManager::EraseBuff( UidType uiUnitUID, BUFF_TEMPLET_ID eBuffID_, bool bIsDeBuff )
{	
	if( uiUnitUID == m_ptrMyGageSet->GetUid() )
	{
		m_ptrMyGageSet->GetGageUI()->EraseBuff(eBuffID_, bIsDeBuff);
		return;
	}

	vector<CX2GageSetPtr>::iterator it = m_vecGageSetPartyMember.begin();
	for( ; it != m_vecGageSetPartyMember.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->EraseBuff(eBuffID_, bIsDeBuff);
			return;
		}
	}
	it = m_vecGageSetPvpMyTeam.begin();
	for( ; it != m_vecGageSetPvpMyTeam.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->EraseBuff(eBuffID_, bIsDeBuff);
			return;
		}
	}
}

/** @function : ClearBuffList
	@brief : UI��¿� ���Ǵ� ���� ����Ʈ �ʱ�ȭ
	@param : ������ ���ŵ� ����UID(uiUnitUID)
*/
void CX2GageManager::ClearBuffList( UidType uiUnitUID )
{
//#ifdef X2TOOL
	if ( NULL == m_ptrMyGageSet )
		return;
//#endif

	if( uiUnitUID == m_ptrMyGageSet->GetUid() )
	{
		m_ptrMyGageSet->GetGageUI()->ClearBuffList();
		return;
	}

	vector<CX2GageSetPtr>::iterator it = m_vecGageSetPartyMember.begin();
	for( ; it != m_vecGageSetPartyMember.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->ClearBuffList();
			return;
		}
	}
	it = m_vecGageSetPvpMyTeam.begin();
	for( ; it != m_vecGageSetPvpMyTeam.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->ClearBuffList();
			return;
		}
	}
}
bool CX2GageManager::IsNotUseNotifyTimeBuff( BUFF_TEMPLET_ID eBuffID_ )
{
	// ������ // 2013-07-17
	// 5������ ������ �����Ű�� �ʴ� ���� ���ø� ����Ʈ
	switch ( eBuffID_ )
	{
	case BTI_BUFF_CHIVALRY_ATTACK:
	case BTI_BUFF_CHIVALRY_DEFENCE:
	case BTI_HEALTH_POINT_IMMEDIATELY_CHANGE_ONCE:
	case BTI_BUFF_SI_SA_CTT_TACTICAL_FIELD:
	case BTI_BUFF_SI_SA_CTT_TACTICAL_FIELD_RAID:
	case BTI_SI_SA_EEG_ATOMIC_SHIELD:
	case BTI_DEBUFF_MOVE_JUMP_SLOWDOWN:
		return true;

	default:
		return false;
	}

	return false;
}

/** @fucntion : NotifyDurationTime5sec
	@brief : ���ӽð� 5�� ���� �˸�
	@param : ������ ���ŵ� ����UID(uiUnitUID)
*/
void CX2GageManager::NotifyDurationTime5sec( UidType uiUnitUID, BUFF_TEMPLET_ID eBuffID_)
{
	// 5������ ������ �����Ű�� �ʴ� ���� ���ø� ����Ʈ
	if( true == IsNotUseNotifyTimeBuff(eBuffID_) )
		return;

	if( uiUnitUID == m_ptrMyGageSet->GetUid() )
	{
		m_ptrMyGageSet->GetGageUI()->NotifyDurationTime5sec(eBuffID_);
		return;
	}

	vector<CX2GageSetPtr>::iterator it = m_vecGageSetPartyMember.begin();
	for( ; it != m_vecGageSetPartyMember.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->NotifyDurationTime5sec(eBuffID_);
			return;
		}
	}

	it = m_vecGageSetPvpOtherTeam.begin();
	for( ; it != m_vecGageSetPvpOtherTeam.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->NotifyDurationTime5sec(eBuffID_);
			return;
		}
	}
}

/** @fucntion : SetShowBuffIcon
	@brief : ���� ������ ǥ�� ����
	@param : ǥ�� ����
*/
void CX2GageManager::SetShowBuffIcon( bool bIsShow_ )
{
	if( NULL == m_ptrMyGageSet )		
		return;

	if( NULL == m_ptrMyGageSet->GetGageUI() )
		return;
#ifdef FIX_SKILL_SLOT_UI_TYPE_CHANGE
	if( false == GetShowMyGageUI() )
		return;
#endif //FIX_SKILL_SLOT_UI_TYPE_CHANGE

	CX2MyGageUI* pMyGageUI = static_cast<CX2MyGageUI*>( m_ptrMyGageSet->GetGageUI() );

	if( NULL != pMyGageUI )
	{
		pMyGageUI->SetShow( bIsShow_ );
		pMyGageUI->UpdateBuffIcon();
	}


	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPartyMember )
	{
		CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet->GetGageUI() );
		if( NULL != pPartyMemberGageUI )
		{
			pPartyMemberGageUI->SetShow( bIsShow_);
			pPartyMemberGageUI->UpdateBuffIcon();
		}
	}

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpMyTeam )
	{
		CX2PVPPlayerGageUI* pPVPPlayerGageUI = static_cast<CX2PVPPlayerGageUI*>( ptrGageSet->GetGageUI() );
		if( NULL != pPVPPlayerGageUI )
		{
			pPVPPlayerGageUI->SetShow( bIsShow_);
			pPVPPlayerGageUI->UpdateBuffIcon();
		}
	}

	BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpOtherTeam )
	{
		CX2PVPPlayerGageUI* pPVPPlayerGageUI = static_cast<CX2PVPPlayerGageUI*>( ptrGageSet->GetGageUI() );
		if( NULL != pPVPPlayerGageUI )
		{
			pPVPPlayerGageUI->SetShow( bIsShow_);
			pPVPPlayerGageUI->UpdateBuffIcon();
		}
	}
}

/** @function 	: InitBuffIconFlicker
	@brief 		: ������ ��¦�̴� �� ���� �ʱ�ȭ
*/
void CX2GageManager::InitBuffIconFlicker( UidType uiUnitUID, BUFF_TEMPLET_ID eBuffID_)
{
	if( BTI_NONE == eBuffID_ )
		return;

	if( uiUnitUID == m_ptrMyGageSet->GetUid() )
	{
		m_ptrMyGageSet->GetGageUI()->InitBuffIconFlicker(eBuffID_);
		return;
	}

	vector<CX2GageSetPtr>::iterator it = m_vecGageSetPartyMember.begin();
	for( ; it != m_vecGageSetPartyMember.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->InitBuffIconFlicker(eBuffID_);
			return;
		}
	}

	it = m_vecGageSetPvpOtherTeam.begin();
	for( ; it != m_vecGageSetPvpOtherTeam.end(); ++it)
	{
		if( uiUnitUID == (*it)->GetUid() )
		{
			(*it)->GetGageUI()->InitBuffIconFlicker(eBuffID_);
			return;
		}
	}
}
#endif //BUFF_ICON_UI

#ifdef DISPLAY_BUFF_DURATION_TIME
void CX2GageManager::SetDurationTime( UidType uiUnitUID, BUFF_TEMPLET_ID eBuffID_, int iTime_)
{
	if( uiUnitUID == m_ptrMyGageSet->GetUid() )
	{
		if( true == IsNotUseNotifyTimeBuff(eBuffID_) )
			iTime_ = 0;

		m_ptrMyGageSet->GetGageUI()->SetDurationTime(eBuffID_, iTime_);
	}
}
#endif // DISPLAY_BUFF_DURATION_TIME
/** @function : GetBuffFactorFromGameUnit
	@brief : �μ��� ���޵� GameUnit���κ��� BuffFactor�� ���� m_veTempBuffFactor�� �����ϴ� �Լ�
	@param : �Ϲ������� �ڽ��� ��������(pGameUnit_)
*/
void CX2GageManager::GetBuffFactorFromGameUnit( const CX2GameUnit* pGameUnit_ )
{
	if ( NULL != g_pX2Game && CX2Game::GT_PVP != g_pX2Game->GetGameType() && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
		pGameUnit_->GetBuffFactor( m_vecTempBuffFactor );
}

/** @function : UpdatePartyMemberLevel
	@brief : ��Ƽ�� ���� ���� ����
	@param : ������ ��Ƽ�� UID( uiUnitUID_ ) , ������ ���� ( uiLevel_ ) 
*/
void CX2GageManager::UpdatePartyMemberLevel( const UidType uiUnitUID_, const UINT uiLevel_ )
{	
	vector<CX2GageSetPtr>::iterator it = m_vecGageSetPartyMember.begin();
	for( ; it != m_vecGageSetPartyMember.end(); ++it)
	{
		if( uiUnitUID_ == (*it)->GetUid() )
		{
			static_cast<CX2PartyMemberGageUI*>((*it)->GetGageUI())->SetLevelString(uiLevel_);
			return;
		}
	}
}

void CX2GageManager::InsertPvpMemberUI( const CX2Room::SlotData& slotData_, CX2GUUser* pGameUnitPvpMember_ )
{
	if( NULL != pGameUnitPvpMember_ )
	{
		CX2GageData* pGageData = new CX2GageData();
		CX2GageUI* pGageUI = NULL;
		if ( NULL != pGageData )
		{
			
			char cRank = pGameUnitPvpMember_->GetUnit()->GetUnitData().m_cRank;
			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() &&
				g_pX2Game->GetMyUnit()->GetTeam() == pGameUnitPvpMember_->GetTeam() )
			{
				pGageUI = new CX2PVPPlayerGageUI( slotData_, pGageData, m_vecGageSetPvpMyTeam.size(), cRank );

				if ( NULL != pGageUI )
				{
					m_vecGageSetPvpMyTeam.push_back( CX2GageSetPtr( new CX2GageSet( pGageUI, pGageData, slotData_.m_UnitUID ) ) );


					SetPartyMemberGameUnit( m_vecGageSetPvpMyTeam.size() - 1, pGameUnitPvpMember_ );
					pGageUI->InitUI();
				}
			}
			else
			{
				pGageUI = new CX2PVPPlayerGageUI( slotData_, pGageData, m_vecGageSetPvpOtherTeam.size(), cRank );

				if ( NULL != pGageUI )
				{
					m_vecGageSetPvpOtherTeam.push_back( CX2GageSetPtr( new CX2GageSet( pGageUI, pGageData, slotData_.m_UnitUID ) ) );

					SetPartyMemberGameUnit( m_vecGageSetPvpOtherTeam.size() - 1, pGameUnitPvpMember_ );
					pGageUI->InitUI();
				}
			}
		}

		UpdatePvpMemberUIPosition();
	}
}

void CX2GageManager::InsertPvpMemberUI( const CX2Room::RoomNpcSlot& npcSlotData_, CX2GUNPC* pGameUnitPvpMember_ )
{
	if( NULL != pGameUnitPvpMember_ )
	{
		CX2GageData* pGageData = new CX2GageData();
		CX2GageUI* pGageUI = NULL;
		if ( NULL != pGageData )
		{
			char cRank = -1;
//			NPC�� �Ʊ��� ����.
// 			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() &&
// 				g_pX2Game->GetMyUnit()->GetTeam() == pGameUnitPvpMember_->GetTeam() )
// 			{
// 				pGageUI = new CX2PVPPlayerGageUI( npcSlotData_, pGageData, m_vecGageSetPvpMyTeam.size(), cRank );
// 
// 				if ( NULL != pGageUI )
// 				{
// 					m_vecGageSetPvpMyTeam.push_back( CX2GageSetPtr( new CX2GageSet( pGageUI, pGageData, npcSlotData_.m_iNpcUid ) ) );
// 
// 
// 					SetPartyMemberGameUnit( m_vecGageSetPvpMyTeam.size() - 1, pGameUnitPvpMember_ );
// 					pGageUI->InitUI();
// 				}
// 			}
// 			else
			{
				pGageUI = new CX2PVPPlayerGageUI( npcSlotData_, pGageData, m_vecGageSetPvpOtherTeam.size(), cRank );
				if ( NULL != pGageUI )
				{
					m_vecGageSetPvpOtherTeam.push_back( CX2GageSetPtr( new CX2GageSet( pGageUI, pGageData, npcSlotData_.m_iNpcUid  ) ) );

					SetPartyMemberGameUnit( m_vecGageSetPvpOtherTeam.size() - 1, pGameUnitPvpMember_ );
					pGageUI->InitUI();
				}
			}
		}

		UpdatePvpMemberUIPosition();
	}
}

void CX2GageManager::RemovePvpMemberUIByUserUid( const UidType uid_ )
{
	if ( !m_vecGageSetPvpMyTeam.empty() )
	{
		auto FindUidFunc = [uid_]( CX2GageSetPtr ptrGageSet_ ) {
			return ( uid_ == ptrGageSet_->GetUid() );
		};

		vector<CX2GageSetPtr>::iterator vItr = std::find_if( m_vecGageSetPvpMyTeam.begin(), m_vecGageSetPvpMyTeam.end(), FindUidFunc );

		if ( m_vecGageSetPvpMyTeam.end() != vItr )
			m_vecGageSetPvpMyTeam.erase( vItr );
	}

	if ( !m_vecGageSetPvpOtherTeam.empty() )
	{
		auto FindUidFunc = [uid_]( CX2GageSetPtr ptrGageSet_ ) {
			return ( uid_ == ptrGageSet_->GetUid() );
		};

		vector<CX2GageSetPtr>::iterator vItr = std::find_if( m_vecGageSetPvpOtherTeam.begin(), m_vecGageSetPvpOtherTeam.end(), FindUidFunc );

		if ( m_vecGageSetPvpOtherTeam.end() != vItr )
			m_vecGageSetPvpOtherTeam.erase( vItr );
	}

	UpdatePvpMemberUIPosition();
}

void CX2GageManager::ClearPvpMemberUI()
{
	m_vecGageSetPvpMyTeam.clear();
	m_vecGageSetPvpOtherTeam.clear();
}

void CX2GageManager::UpdatePvpMemberUIPosition( bool bForce_/* = false*/ )
{
	if ( !m_vecGageSetPvpMyTeam.empty() )
	{
		UINT uiPosindex = 0;

		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPvpMyTeam )
		{
			CX2PVPPlayerGageUI* pPvpMemberGageUI = static_cast<CX2PVPPlayerGageUI*>( ptrGageSet_->GetGageUI() );
						
			if ( true == bForce_ ||
				(NULL != pPvpMemberGageUI && pPvpMemberGageUI->GetPositionIndex() != uiPosindex )) //// ������ ������ �ε����� ������ �ε����� �ٸ���
				pPvpMemberGageUI->SetPosition( uiPosindex );	// �������� ���� ��

			++uiPosindex;
		}
	}

	if ( !m_vecGageSetPvpOtherTeam.empty() )
	{
#ifdef FIX_OBSERVER_MODE
		if ( NULL != g_pX2Game && NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver )
		{
			g_pX2Game->InitPositionIndexPVPUserGageForObserver();		/// ������ �� ��, PVP ���� ������ ǥ�� ���� �ε��� �ʱ�ȭ
		}
#endif FIX_OBSERVER_MODE

		UINT uiPosindex = 0;

		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPvpOtherTeam )
		{
			CX2PVPPlayerGageUI* pPvpMemberGageUI = static_cast<CX2PVPPlayerGageUI*>( ptrGageSet_->GetGageUI() );

#ifdef FIX_OBSERVER_MODE
			// ������ �̰ų�, ������ ������ �ε����� ������ �ε����� �ٸ���
			if ( ( NULL != g_pX2Game && NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver ) ||
				 ( NULL != pPvpMemberGageUI && pPvpMemberGageUI->GetPositionIndex() != uiPosindex ) )
#else  FIX_OBSERVER_MODE
			// ������ ������ �ε����� ������ �ε����� �ٸ���
			if ( NULL != pPvpMemberGageUI && pPvpMemberGageUI->GetPositionIndex() != uiPosindex )
#endif FIX_OBSERVER_MODE
				pPvpMemberGageUI->SetPosition( uiPosindex );	// �������� ���� ��

			++uiPosindex;
		}
	}
}

void CX2GageManager::UpdatePvpMemberGageData( const UidType uidPartyMember_, CX2GameUnit* pGameUnitPartyMember_ )
{
	if ( NULL != pGameUnitPartyMember_ )
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpMyTeam )
		{
			if ( uidPartyMember_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->UpdateNowHpFromPercent( pGameUnitPartyMember_->GetNowHp() / pGameUnitPartyMember_->GetMaxHp() );
				ptrGageSet->UpdateNowMpFromPercent( pGameUnitPartyMember_->GetNowMp() / pGameUnitPartyMember_->GetMaxMp() );
			}
		}

		BOOST_FOREACH( CX2GageSetPtr ptrGageSet, m_vecGageSetPvpOtherTeam )
		{
			if ( uidPartyMember_ == ptrGageSet->GetUid() )
			{
				ptrGageSet->UpdateNowHpFromPercent( pGameUnitPartyMember_->GetNowHp() / pGameUnitPartyMember_->GetMaxHp() );

#ifdef FIX_OBSERVER_MODE		/// ������ ��带 �˻��� �� �ֵ��� ���� ����
				if ( ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
					 true == g_pX2Game->GetMyUnit()->GetCashItemAbility().m_bShowOppnentMP ) ||					/// ���� ����� �������� ��
					 ( NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver ) )			/// �������� ��
				{
					ptrGageSet->UpdateNowMpFromPercent( pGameUnitPartyMember_->GetNowMp() / pGameUnitPartyMember_->GetMaxMp() );
				}
				else
				{
					ptrGageSet->UpdateNowMpFromPercent( 0 );
				}
#else  FIX_OBSERVER_MODE
				if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
				{
					if ( ( NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver ) ||	/// �������� ��
						true == g_pX2Game->GetMyUnit()->GetCashItemAbility().m_bShowOppnentMP )				/// ���� ����� �������� ��
						ptrGageSet->UpdateNowMpFromPercent( pGameUnitPartyMember_->GetNowMp() / pGameUnitPartyMember_->GetMaxMp() );
					else
						ptrGageSet->UpdateNowMpFromPercent( 0 );
				}
#endif FIX_OBSERVER_MODE
			}
		}
	}
}


#ifdef FIX_RESURRECTION_INDEX

/** @function	: GetPartyGageUIPositionIndexByUserList
	@brief		: �˻��� ���� �ε����� ��ġ�� ��ġ�ϴ� PartyGageUI�� UID �˻�
	@param		: �˻��� ���� �ε���
	@return		: PartyGageUI�� �ε���
*/
const UidType CX2GageManager::GetPartyGageUIPositionIndexByUserList( int iIndex ) const
{
	if ( m_vecGageSetPartyMember.size() > 0 )		// ���Ϳ� �����Ͱ� ���� ��,
	{
		BOOST_FOREACH( CX2GageSetPtr ptrGageSet_, m_vecGageSetPartyMember )		// �� ������ �����̳ʿ���
		{
			if ( NULL == ptrGageSet_ )
				continue;

			// PartyMemberGageUI �� ���´�.
			CX2PartyMemberGageUI* pPartyMemberGageUI = static_cast<CX2PartyMemberGageUI*>( ptrGageSet_->GetGageUI() );

			if ( NULL != pPartyMemberGageUI )
			{
				if ( pPartyMemberGageUI->GetPositionIndex() == iIndex )	// ���� PartyMember �� UID �� �������ڿ� ���ٸ�
				{
					return ptrGageSet_->GetUid();
				}
			}
		}
	}

	return -1;
}

#endif // FIX_RESURRECTION_INDEX

#ifdef NEW_CHARACTER_EL
/** @function : GetIsExsitancePartyMemberHPLessThanPer
	@brief : ü�� @1 % ������ ��Ƽ���� ���� ���� üũ
*/
bool CX2GageManager::GetIsExsitancePartyMemberHPLessThanPer( float fHPRate_ )
{
	BOOST_FOREACH( const CX2GageSetPtr pGageSetPtr, m_vecGageSetPartyMember )
	{
		if( pGageSetPtr->GetNowHpPercent() > 0.f &&
			pGageSetPtr->GetNowHpPercent() <= fHPRate_ )
			return true;
	}

	/* ������ // 2013-07-17
	 void CX2GageManager::CX2GageSet::UpdateGageDataFromGameUnit()
	��� �Լ��� ���� ���������� �ۼ�Ʈ�� �������� �ʵ��� ����ó�� �Ǿ� �־���.
	����ó�� �� ��Ȯ�� ������ �� �� ��� �ϴ� ����HP�� �ִ�HP�� ���ϴ� ������� ���� ��
	*/
	BOOST_FOREACH( const CX2GageSetPtr pGageSetPtr, m_vecGageSetPvpMyTeam )
	{
		if( NULL != pGageSetPtr->GetGageData() )
		{
			const float fHpPercent = 
				pGageSetPtr->GetGageData()->GetNowHp() / pGageSetPtr->GetGageData()->GetMaxHp();

			if( fHpPercent > 0.f &&
				fHpPercent <= fHPRate_ )
				return true;
		}
	}

	return false;
}
#endif // NEW_CHARACTER_EL