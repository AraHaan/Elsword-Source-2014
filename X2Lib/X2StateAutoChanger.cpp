#include "StdAfx.h"
#include ".\x2stateautochanger.h"

//#include "./X2StateUnitSelect.h"
//#include "./X2StateCreateUnit.h"
//#include "./X2StatePVPLobby.h"
//#include "./X2StatePVPRoom.h"
//#include "./X2StatePVPGame.h"
//#include "./X2StateDungeonRoom.h"
//#include "./X2StateDungeonGame.h"
//#include "./X2StateSquareGame.h"
//#include "./X2StateDungeonResult.h"
//#include "./X2StateVillageMap.h"
//#include "./X2StateLocalMap.h"
//#include "./X2StateWorldMap.h"
//#include "./X2StateShop.h"
//#include "./X2StateNPCHouse.h"
//#include "./X2StateTrainingSchool.h"
//#include "./X2StateAlchemistHouse.h"
//#include "./X2StateClassChange.h"
//


CX2StateAutoChanger::CX2StateAutoChanger()
{
	m_bIsAutoChangingState		= false;
	m_iMyUnitUID				= -1;
	m_iTargetStateID			= CX2Main::XS_INVALID;

	m_iTryEnterRoomCount		= 0;

	m_iLoadingBGIndex			= 0;
	m_pDLGLoadingBG				= NULL;
}

CX2StateAutoChanger::~CX2StateAutoChanger()
{
	SAFE_DELETE_DIALOG( m_pDLGLoadingBG );
}

void CX2StateAutoChanger::StateChangeAfterConstructor()
{
	if( true == m_bIsAutoChangingState )
	{
		int iNowStateID = (int) g_pMain->GetNowStateID();

		DoStateChange( iNowStateID );
	}
}


void CX2StateAutoChanger::EndStateChange()
{
	m_bIsAutoChangingState	= false;
	//SAFE_DELETE_DIALOG( m_pDLGLoadingBG );
}


void CX2StateAutoChanger::StartStateChange( int iNowStateID, int iTargetStateID, TARGET_DETAIL targetDetail, UidType myUnitUID /*= -1*/ )
{	
	m_bIsAutoChangingState	= true;

	m_iTargetStateID		= iTargetStateID;
	m_TargetDetail			= targetDetail;
	m_iMyUnitUID			= myUnitUID;

	DoStateChange( iNowStateID );
}

#ifndef ELSWORD_NEW_BEGINNING
void CX2StateAutoChanger::InCase_XS_CREATE_UNIT()
{
	CX2StateCreateUnit* pState = (CX2StateCreateUnit*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_VILLAGE_MAP:
		{
			if(CX2StateServerSelect::m_sbSelectedServerSet == false)
			{
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_551 ) );
			}
			else
			{
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh ���� ���� ����
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_552 ), L"DLG_UI_Selection_MessageBox_No_Button.lua" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_552 ) );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh ���� ���� ����
			}
			
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/CX2Main::XS_SERVER_SELECT, NULL, false );

		} break;
	}
}
#endif ELSWORD_NEW_BEGINNING

void CX2StateAutoChanger::InCase_XS_SERVER_SELECT()
{
#ifdef ELSWORD_NEW_BEGINNING
	CX2StateBeginning* pState = (CX2StateBeginning*) g_pMain->GetNowState();
#else
	CX2StateServerSelect* pState = (CX2StateServerSelect*) g_pMain->GetNowState();
#endif ELSWORD_NEW_BEGINNING
	switch( m_iTargetStateID )
	{
	default:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	//case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
			EndStateChange();
		} break;

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_DUNGEON_GAME:		// ĳ���� ����â���� Ʃ�丮��� �ٷ� ���� ��츦 ó���ϱ� ���ؼ�
		{
			if( NULL != g_pData->GetMyUser()->GetUnitByUID( m_iMyUnitUID ) )
			{
				const int MAGIC_UNIT_LEVEL = -1;
				pState->Handler_EGS_SELECT_UNIT_REQ( m_iMyUnitUID, MAGIC_UNIT_LEVEL );
			}
			else
			{
				EndStateChange();
			}

		} break;
	}
}

//
void CX2StateAutoChanger::InCase_XS_VILLAGE_MAP()
{
	CX2StateField* pState = (CX2StateField*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;
//#ifndef NEW_VILLAGE_UI
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//		{
//			if( NULL != g_pData->GetMyUser() &&
//				NULL != g_pData->GetMyUser()->GetSelectUnit() &&
//				g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV &&
//				g_pData->GetSelectUnitLevel() < 3 )
//			{
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"���� 3�̻���� ����忡 ������ �� �ֽ��ϴ�.", NULL );
//				EndStateChange();
//				return;
//			}
//
//			pState->Handler_EGS_ENTER_PVP_CHANNEL_REQ();
//
//		} break;
//#endif
//
//	case CX2Main::XS_DUNGEON_ROOM:
//	case CX2Main::XS_DUNGEON_GAME:
//		{
//			pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
//
//		} break;
//
//	case CX2Main::XS_ARCADE_LOBBY:
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:
//		{
//			pState->Handler_EGS_OPEN_ARCADE_ROOM_LIST_REQ();
//
//		} break;
//	
//	case CX2Main::XS_VILLAGE_MAP:
//		{
//			int iVillageID = g_pMain->GetNowDetailStateID();
//
//			if( m_TargetDetail.m_iVillageID != -1 )
//			{
//				if( iVillageID == m_TargetDetail.m_iVillageID )
//				{
//					EndStateChange();
//				}
//				else
//				{
//					pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();					
//				}
//			}
//			else
//			{
//				EndStateChange();
//			}
//		} break;
//
//	case CX2Main::XS_LOCAL_MAP:
//	case CX2Main::XS_WORLD_MAP:
//		{
//			pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
//		} break;
//
//	case CX2Main::XS_SHOP:
//		{
//			int iVillageID = g_pMain->GetNowDetailStateID();
//			CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( (SEnum::VILLAGE_MAP_ID) iVillageID );
//			if( NULL != pVillageTemplet )
//			{
//				for( UINT i=0; i< pVillageTemplet->m_HouseList.size(); i++ )
//				{
//					CX2LocationManager::HOUSE_ID eHouseID = pVillageTemplet->m_HouseList[i];
//
//					if( m_TargetDetail.m_iHouseID == (int)eHouseID )
//					{
//						pState->Handler_EGS_ENTER_EQUIP_SHOP_REQ( m_TargetDetail.m_iHouseID );
//					}
//					else
//					{
//						pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ(); // �ٸ� ���� �����ΰ� ����
//					}
//				}
//			}
//			else
//			{
//				// error
//				EndStateChange(); // ������ ��������?
//			}
//
//		} break;
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	//case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
			if( true == g_pInstanceData->GetIsDungeonLounge() )
			{
				if( true == g_pData->GetPartyManager()->AmIPartyLeader() )
				{
					ASSERT( !L"EGS_PARTY_TO_FIELD_MAP_REQ" );
				}
				else
				{
					pState->Handler_EGS_LEAVE_ROOM_REQ();
				}
			}
			else
			{
				pState->Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
			}
		} break;

	//case CX2Main::XS_TRAINING_SCHOOL:
	//	{
	//		int iVillageID = g_pMain->GetNowDetailStateID();

	//		m_TargetDetail.m_iVillageID = (int)SEnum::VMI_RUBEN; // fix!! �ϴ� �ƷüҴ� ������ �纥������

	//		if( iVillageID == m_TargetDetail.m_iVillageID )
	//		{
	//			pState->Handler_EGS_ENTER_TC_CHANNEL_REQ();
	//		}
	//		else
	//		{
	//			pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
	//		}
	//	} break;
	}
}


//void CX2StateAutoChanger::InCase_XS_LOCAL_MAP()
//{
//	CX2StateLocalMap* pState = (CX2StateLocalMap*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	default:
//		EndStateChange();
//		break;
//
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//	case CX2Main::XS_ARCADE_LOBBY:
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:
//		{
//			pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//			//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ(); 
//		} break;
//
//	case CX2Main::XS_DUNGEON_ROOM:
//	case CX2Main::XS_DUNGEON_GAME:
//		{
//			// ä�ο� �� �ִ��� 
//			if( true == pState->GetEnteringDungeonChannel() )
//			{
//				//  ���� ä�ο� �ִ��� 
//				if( g_pMain->GetConnectedChannelID() == (UINT)m_TargetDetail.m_iChannelID )
//				{
//					TryEnterDungeonRoom();
//				}
//				else 
//				{
//					pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();	// �ش� �������� �ִ� ����ä���� �ƴϸ� �ϴ� ���øʿ��� �ٽ� ����	
//					// �������� statelocalmap���� ó��
//				}
//			}
//			else
//			{
//				TryEnterDungeonChannel();
//			}
//
//		} break;
//
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_VILLAGE_MAP:
//		{
//			// ä�ο� �� ������ ���� ä���� ������
//			if( true == pState->GetEnteringDungeonChannel() )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ(); 
//				// �������� statelocalmap���� ó��
//			}
//			else
//			{
//				SEnum::VILLAGE_MAP_ID eVillageID = 
//					g_pData->GetLocationManager()->GetVillageID( (CX2LocationManager::LOCAL_MAP_ID) g_pMain->GetNowDetailStateID() );
//
//				if( m_TargetDetail.m_iVillageID != -1 )
//				{
//					if( m_TargetDetail.m_iVillageID == (int)eVillageID )
//					{
//						//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//						pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//					}
//					else
//					{
//						if( eVillageID == SEnum::VMI_INVALID )
//						{
//							// error!! ���� ���ø� ID �� �̻��ϴ� 
//							EndStateChange();
//						}
//						else
//						{
//							pState->Handler_EGS_STATE_CHANGE_WORLD_MAP_REQ();
//						}
//					}
//				}
//				else
//				{
//					// �ƹ� �����̳� ������ٸ� �׳� ������~
//					//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ(); 
//					pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//				}
//			}
//		} break;
//
//	case CX2Main::XS_LOCAL_MAP:
//		{
//			// ä�ο� �� �ִ��� 
//			if( true == pState->GetEnteringDungeonChannel() )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ(); 
//				// �������� statelocalmap���� ó��
//			}
//			else
//			{
//				int iLocalMapID = g_pMain->GetNowDetailStateID();
//				if( -1 == m_TargetDetail.m_iLocalMapID ||
//					iLocalMapID == m_TargetDetail.m_iLocalMapID )
//				{
//					EndStateChange();
//				}
//				else
//				{
//					// ���� ���ϴ� ������ ���ø����� Ȯ���ϰ� �ƴϸ� ����� ���ٰ� �ٽ� �;� �Ѵ�
//					pState->Handler_EGS_STATE_CHANGE_WORLD_MAP_REQ();
//				}
//			}
//		} break;
//
//	case CX2Main::XS_WORLD_MAP:
//		{
//			// ä�ο� �� ������ ���� ä���� ���� �Ŀ� 
//			if( true == pState->GetEnteringDungeonChannel() )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ(); 
//				// �������� statelocalmap���� ó��
//			}
//			else
//			{
//				pState->Handler_EGS_STATE_CHANGE_WORLD_MAP_REQ();
//			}
//			
//		} break;
//
//	case CX2Main::XS_SHOP:
//		{
//			
//			// �ش� ������ �ִ� ������ ������ local�� �ִٸ�
//			//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ(); // fix!! ���� ID�� �Է��ؾ�~
//			pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//
//			// �ٸ� ������ �����̶�� ���ø�����?			
//		} break;
//
//	//case CX2Main::XS_UNIT_SELECT:
//	case CX2Main::XS_SERVER_SELECT:
//		{
//			if( true == pState->GetEnteringDungeonChannel() ) // ä�ο� �� �ִٸ� ä���� ������
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ(); 
//				// ������ ������ statelocalmap���� ó��
//			}
//			else
//			{
//				//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//				pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//			}
//
//		} break;
//
//	case CX2Main::XS_TRAINING_SCHOOL:
//		{
//
//			// ä�ο� �� ������ ���� ä���� ������
//			if( true == pState->GetEnteringDungeonChannel() )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ(); 
//				// �������� statelocalmap���� ó��
//			}
//			else
//			{
//				SEnum::VILLAGE_MAP_ID eVillageID = 
//					g_pData->GetLocationManager()->GetVillageID( (CX2LocationManager::LOCAL_MAP_ID) g_pMain->GetNowDetailStateID() );
//
//				m_TargetDetail.m_iVillageID = (int)SEnum::VMI_RUBEN; // note!! �ƷüҴ� �纥�������� �ִ�
//				if( m_TargetDetail.m_iVillageID != -1 )
//				{
//					if( m_TargetDetail.m_iVillageID == (int)eVillageID )
//					{
//						//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//						pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//					}
//					else
//					{
//						if( eVillageID == SEnum::VMI_INVALID )
//						{
//							// error!! ���� ���ø� ID �� �̻��ϴ� 
//							EndStateChange();
//						}
//						else
//						{
//							pState->Handler_EGS_STATE_CHANGE_WORLD_MAP_REQ();
//						}
//					}
//				}
//				else
//				{
//					// �ƹ� �����̳� ������ٸ� �׳� ������~
//					//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ(); 
//					pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//				}
//			}
//
//		} break;
//
//	}
//}

//void CX2StateAutoChanger::InCase_XS_WORLD_MAP()
//{
//	CX2StateWorldMap* pState = (CX2StateWorldMap*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	default:
//	case CX2Main::XS_WORLD_MAP:
//		{
//			EndStateChange();
//		} break;
//
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_ARCADE_LOBBY:
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:
//		{
//			// fix!! Ư�������� ���������� ���� �Ϳ� ���� ó���� �ʿ�?
//			pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( g_pMain->GetPrevLocalStateID() );
//		} break;
//
//	case CX2Main::XS_DUNGEON_ROOM:
//	case CX2Main::XS_DUNGEON_GAME:
//		{
//			CX2LocationManager::LOCAL_MAP_ID eLocalMapID = 
//				g_pData->GetLocationManager()->GetLocalMapID( (SEnum::DUNGEON_ID) m_TargetDetail.m_iDungeonID ); // fix!! ä��manager���� ���� ���̵� ��������
//
//			if( CX2LocationManager::LMI_INVALID == eLocalMapID )
//			{
//				// error 
//				EndStateChange();
//			}
//			else
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( eLocalMapID );
//			}
//		} break;
//
//	case CX2Main::XS_VILLAGE_MAP:
//		{
//			CX2LocationManager::LOCAL_MAP_ID eLocalMapID = 
//				g_pData->GetLocationManager()->GetLocalMapID( (SEnum::VILLAGE_MAP_ID) m_TargetDetail.m_iVillageID );
//
//			if( -1 == m_TargetDetail.m_iVillageID )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( g_pMain->GetPrevLocalStateID() );
//			}
//			else
//			{
//				if( eLocalMapID != CX2LocationManager::LMI_INVALID )
//				{
//					pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( eLocalMapID );
//				}
//				else
//				{
//					// error 
//					EndStateChange();
//				}
//			}
//
//		} break;
//
//	case CX2Main::XS_SHOP:
//		{
//			// fix!! not implemented yet
//			EndStateChange();
//
//		} break;
//
//	case CX2Main::XS_LOCAL_MAP:
//		{
//			if( m_TargetDetail.m_iLocalMapID != -1 )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( (UINT) m_TargetDetail.m_iLocalMapID );
//			}
//			else
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( g_pMain->GetPrevLocalStateID() );
//			}
//		} break;
//		
//
//	//case CX2Main::XS_UNIT_SELECT:
//	case CX2Main::XS_SERVER_SELECT:
//		{
//			pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( g_pMain->GetPrevLocalStateID() );
//		} break;
//
//
//	case CX2Main::XS_TRAINING_SCHOOL:
//		{
//			m_TargetDetail.m_iLocalMapID = CX2LocationManager::LMI_VELDER_NORTH; // �ƷüҴ� �纥����( �������� ���� )�� �ִ�
//			if( m_TargetDetail.m_iLocalMapID != -1 )
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( (UINT) m_TargetDetail.m_iLocalMapID );
//			}
//			else
//			{
//				pState->Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ( g_pMain->GetPrevLocalStateID() );
//			}
//		} break;
//		
//	}
//}

void CX2StateAutoChanger::InCase_XS_PVP_LOBBY()
{
	CX2StatePvPLobby* pState = (CX2StatePvPLobby*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_LOBBY:
		{
			if( -1 == m_TargetDetail.m_iChannelID )
			{
				EndStateChange();
			}
			else
			{
				// ���� ä��
				if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )				
				{
					TryEnterPVPRoom();
				}
				else
				{
#ifndef NEW_VILLAGE_UI
					pState->Handler_EGS_CHANGE_PVP_CHANNEL_REQ( (UINT) m_TargetDetail.m_iChannelID );
#else
					pState->Handler_EGS_CHANGE_PVP_ROOM_LIST_REQ( (UINT) m_TargetDetail.m_iChannelID );
#endif
					// ������������ statepvplobby���� ó��
					// ä�� �̵� �����ϸ� ������ join
				}
			}

		} break;


	case CX2Main::XS_DUNGEON_ROOM:
		{
			pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
		}
		break;
	
	case CX2Main::XS_VILLAGE_MAP:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
#ifndef NEW_VILLAGE_UI
			pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
#else
			pState->Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
			//pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
#endif
		} break;
	}
}

void CX2StateAutoChanger::InCase_XS_PVP_ROOM()
{
	// fix!! case�� ���� state�� �߰������ �Ѵ�
	CX2StatePVPRoom* pState = (CX2StatePVPRoom*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
		{
			// ���� ä��
			if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )
			{
				// ���� room 
				if( g_pData->GetPVPRoom()->GetRoomUID() == m_TargetDetail.m_iRoomUID )
				{
					EndStateChange();
				}
				else
				{
					pState->Handler_EGS_LEAVE_ROOM_REQ();
				}
			}
			else
			{
				pState->Handler_EGS_LEAVE_ROOM_REQ();
			}
			
		} break;

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_DUNGEON_ROOM:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
			pState->Handler_EGS_LEAVE_ROOM_REQ();

		} break;
		
	}
}

void CX2StateAutoChanger::InCase_XS_DUNGEON_ROOM()
{
	CX2StateDungeonRoom* pState = (CX2StateDungeonRoom*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_TRAINING_SCHOOL:
		{
			pState->Handler_EGS_LEAVE_ROOM_REQ();
		} break;

	case CX2Main::XS_DUNGEON_ROOM:
		{
			// ���� ä��
			if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )
			{
				// fix!! ���� ID�� ���̵� üũ�ϴ� �ڵ� �ʿ�
				if( m_TargetDetail.m_iRoomUID == -1 )
				{
					EndStateChange();
				}
				else
				{
					// ���� room 
					if( g_pData->GetDungeonRoom()->GetRoomUID() == m_TargetDetail.m_iRoomUID )
					{
						EndStateChange();
					}
					else
					{
						pState->Handler_EGS_LEAVE_ROOM_REQ();
					}
				}
			}
			else
			{
				pState->Handler_EGS_LEAVE_ROOM_REQ();
			}

		} break;

	case CX2Main::XS_DUNGEON_GAME:
		{
			// ���� ä��
			if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )
			{
				// fix!! ���� ID�� ���̵� üũ�ϴ� �ڵ� �ʿ�
				if( m_TargetDetail.m_iRoomUID == -1 )
				{
					EndStateChange(); // fix!! �Ƹ� dungeon_game���� �ϴ°� �°���
					
					// fix!! �ٷ� ������ �����ϰڴٴ� �Է����ڸ� �޾Ƽ� ó���ؾ���, �ϴ��� �ٷν���
					pState->Handler_EGS_STATE_CHANGE_GAME_START_REQ();
				}
				else
				{
					// ���� room 
					if( g_pData->GetDungeonRoom()->GetRoomUID() == m_TargetDetail.m_iRoomUID )
					{
						EndStateChange();

						// fix!! �ٷ� ������ �����ϰڴٴ� �Է����ڸ� �޾Ƽ� ó���ؾ���, �ϴ��� �ٷν���
						pState->Handler_EGS_STATE_CHANGE_GAME_START_REQ();
					}
					else
					{
						pState->Handler_EGS_LEAVE_ROOM_REQ();
					}
				}
			}
			else
			{
				pState->Handler_EGS_LEAVE_ROOM_REQ();
			}
		} break;
	
	}
}

void CX2StateAutoChanger::InCase_XS_DUNGEON_GAME()
{
	CX2StateDungeonGame* pState = (CX2StateDungeonGame*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_TRAINING_SCHOOL:
	case CX2Main::XS_DUNGEON_ROOM:
	case CX2Main::XS_DUNGEON_GAME:
		{
			if( NULL != g_pX2Game )
			{
				// note!! �������ӿ����� leave_game��� leave_room�� ���
				g_pX2Game->Handler_EGS_LEAVE_ROOM_REQ(); // fix!!! �ϴ� �ӽ÷� ������ ���� ������
			}
			else
			{
				EndStateChange();
			}

		} break;
	}

}

void CX2StateAutoChanger::InCase_XS_DUNGEON_RESULT()
{
	CX2StateDungeonResult* pState = (CX2StateDungeonResult*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;
		
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_ROOM:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	//case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_TRAINING_SCHOOL:
	case CX2Main::XS_DUNGEON_GAME:
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_553 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false );
		} break;

	case CX2Main::XS_DUNGEON_RESULT:
		{
			EndStateChange();
		} break;
	}
}

//void CX2StateAutoChanger::InCase_XS_SHOP()
//{
//	CX2StateShop* pState = (CX2StateShop*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	default:
//		EndStateChange();
//		break;
//
//	case CX2Main::XS_VILLAGE_MAP:
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_LOCAL_MAP:
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//	case CX2Main::XS_DUNGEON_ROOM:
//	//case CX2Main::XS_UNIT_SELECT:
//	case CX2Main::XS_SERVER_SELECT:
//	case CX2Main::XS_ARCADE_LOBBY:
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:
//		{
//			pState->StateChangeToVillage();
//
//		} break;
//
//	case CX2Main::XS_SHOP:
//		{
//			// fix!! ��� ������ �̵��� ������ ���� ó�� �ʿ�
//			EndStateChange();
//		} break;
//	}
//}


//void CX2StateAutoChanger::InCase_XS_NPC_HOUSE()
//{
//	CX2StateNPCHouse* pState = (CX2StateNPCHouse*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	default:
//		EndStateChange();
//		break;
//
//	case CX2Main::XS_VILLAGE_MAP:
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_LOCAL_MAP:
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//	case CX2Main::XS_DUNGEON_ROOM:
//	//case CX2Main::XS_UNIT_SELECT:
//	case CX2Main::XS_SERVER_SELECT:
//	case CX2Main::XS_ARCADE_LOBBY:
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:
//		{
//			pState->LeaveHouse();
//
//		} break;
//
//	case CX2Main::XS_NPC_HOUSE:
//		{
//			// fix!! ��� NPC house
//			EndStateChange();
//		} break;
//	}
//}
//
//
//
//void CX2StateAutoChanger::InCase_XS_ALCHEMIST_HOUSE()
//{
//	CX2StateAlchemistHouse* pState = (CX2StateAlchemistHouse*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	default:
//		EndStateChange();
//		break;
//
//	case CX2Main::XS_VILLAGE_MAP:
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_LOCAL_MAP:
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//	case CX2Main::XS_DUNGEON_ROOM:
//	//case CX2Main::XS_UNIT_SELECT:
//	case CX2Main::XS_SERVER_SELECT:
//	case CX2Main::XS_ARCADE_LOBBY:
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:
//		{
//			pState->LeaveHouse();
//
//		} break;
//
//	case CX2Main::XS_ALCHEMIST_HOUSE:
//		{
//			// fix!! ��� alchemsist house??? 
//			EndStateChange();
//		} break;
//	}
//}
//
//
//
void CX2StateAutoChanger::InCase_XS_SQUARE_GAME()
{
	CX2StateSquareGame* pState = (CX2StateSquareGame*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_ROOM:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
			pState->Handler_EGS_LEAVE_SQUARE_REQ();

		} break;

	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			EndStateChange();
		} break;
	}
}


void CX2StateAutoChanger::InCase_XS_TRAINING_SCHOOL()
{
	CX2StateTrainingSchool* pState = (CX2StateTrainingSchool*) g_pMain->GetNowState();

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_DUNGEON_ROOM:
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
			pState->LeaveHouse(); // fix!!
			
		} break;

	case CX2Main::XS_TRAINING_SCHOOL:
		{
			EndStateChange();
#ifndef NEW_VILLAGE_UI
			pState->ChangeSchoolMode( (CX2StateTrainingSchool::TRAINING_SCHOOL_MODE) m_TargetDetail.m_iTrainingSchoolMode );
#endif
		} break;
	}
}

//void CX2StateAutoChanger::InCase_XS_CLASS_CHANGE()
//{
//	CX2StateClassChange* pState = (CX2StateClassChange*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	case CX2Main::XS_CLASS_CHANGE:
//		{
//			EndStateChange();
//		} break;
//
//	default:
//		{
//			//pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//			pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();
//		} break;
//	}
//}



// void CX2StateAutoChanger::InCase_XS_ARCADE_LOBBY()
// {
	// fix!!! �����̵� �ڵ� ���� ������ ��
//	EndStateChange();

	//CX2StateArcadeLobby* pState = (CX2StateArcadeLobby*) g_pMain->GetNowState();

	//switch( m_iTargetStateID )
	//{
	//default:
	//	EndStateChange();
	//	break;

	//case CX2Main::XS_ARCADE_LOBBY:
	//	{
	//		// fix!! arcade ���� ���� �ڵ� �ٽ� ��������
	//		//if( -1 == m_TargetDetail.m_iChannelID )
	//		//{
	//			EndStateChange();
	//		//}
	//		//else
	//		//{
	//		//	// ���� ä��
	//		//	//if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )
	//		//	if( g_pData->GetAracdeChannelData().m_nChannelID == (UINT) m_TargetDetail.m_iChannelID )
	//		//	{
	//		//		EndStateChange();
	//		//	}
	//		//	else
	//		//	{
	//		//		pState->Handler_EGS_CHANGE_ARCADE_ROOM_LIST_REQ( (UINT) m_TargetDetail.m_iChannelID );
	//		//		EndStateChange(); // fix!! pvp lobby �� �ٸ��� ó��???
	//		//		
	//		//		// ������������ CX2StateArcadeLobby���� ó��
	//		//		// ä�� �̵� �����ϸ� ������ join �õ�
	//		//	}
	//		//}

	//	} break;

	//case CX2Main::XS_ARCADE_ROOM:
	//case CX2Main::XS_ARCADE_GAME:
	//	{
	//		// fix!! arcade ���� ���� �ڵ� �ٽ� ��������
	//		//if( -1 == m_TargetDetail.m_iChannelID )
	//		//{
	//			EndStateChange();
	//		//}
	//		//else
	//		//{
	//		//	// ���� ä��
	//		//	//if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )
	//		//	if( g_pData->GetAracdeChannelData().m_nChannelID == (UINT) m_TargetDetail.m_iChannelID )
	//		//	{
	//		//		TryEnterArcadeRoom();
	//		//	}
	//		//	else
	//		//	{
	//		//		pState->Handler_EGS_CHANGE_ARCADE_ROOM_LIST_REQ( (UINT) m_TargetDetail.m_iChannelID );
	//		//		// ������������ CX2StateArcadeLobby���� ó��
	//		//		// ä�� �̵� �����ϸ� ������ join �õ�
	//		//	}
	//		//}

	//	} break;

	//case CX2Main::XS_VILLAGE_MAP:

	//case CX2Main::XS_PVP_ROOM:
	//case CX2Main::XS_PVP_GAME:
	//case CX2Main::XS_PVP_LOBBY:
	//case CX2Main::XS_LOCAL_MAP:
	//case CX2Main::XS_DUNGEON_ROOM:
	//case CX2Main::XS_WORLD_MAP:
	//case CX2Main::XS_SHOP:
	//	//case CX2Main::XS_UNIT_SELECT:
	//case CX2Main::XS_SERVER_SELECT:
	//	{
	//		// fix!! ������? 
	//		pState->Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
	//	} break;
	//}
//}


//
//void CX2StateAutoChanger::InCase_XS_ARCADE_ROOM()
//{
//	CX2StateArcadeRoom* pState = (CX2StateArcadeRoom*) g_pMain->GetNowState();
//
//	switch( m_iTargetStateID )
//	{
//	default:
//		EndStateChange();
//		break;
//
//	case CX2Main::XS_VILLAGE_MAP:
//	case CX2Main::XS_PVP_LOBBY:
//	case CX2Main::XS_PVP_ROOM:
//	case CX2Main::XS_PVP_GAME:
//	case CX2Main::XS_LOCAL_MAP:
//	case CX2Main::XS_DUNGEON_ROOM:
//	case CX2Main::XS_DUNGEON_GAME:
//	//case CX2Main::XS_UNIT_SELECT:
//	case CX2Main::XS_SERVER_SELECT:
//	case CX2Main::XS_TRAINING_SCHOOL:
//	case CX2Main::XS_ARCADE_LOBBY:
//		{
//			pState->Handler_EGS_LEAVE_ROOM_REQ();
//
//		} break;
//
//	case CX2Main::XS_ARCADE_ROOM:
//	case CX2Main::XS_ARCADE_GAME:	
//		{
//			// fix!! �����̵� �ڵ� ����
//			EndStateChange();
//
//			//// ���� ä��
//			////if( g_pMain->GetConnectedChannelID() == (UINT) m_TargetDetail.m_iChannelID )
//			//if( g_pData->GetAracdeChannelData().m_nChannelID == (UINT) m_TargetDetail.m_iChannelID )
//			//{
//			//	// fix!! ���� ID�� ���̵� üũ�ϴ� �ڵ� �ʿ�
//			//	if( m_TargetDetail.m_iRoomUID == -1 )
//			//	{
//			//		EndStateChange();
//			//	}
//			//	else
//			//	{
//			//		// ���� room 
//			//		if( NULL != g_pX2Room && 
//			//			g_pX2Room->GetRoomUID() == m_TargetDetail.m_iRoomUID )
//			//		{
//			//			EndStateChange();
//			//		}
//			//		else
//			//		{
//			//			pState->Handler_EGS_LEAVE_ROOM_REQ();
//			//		}
//			//	}
//			//}
//			//else
//			//{
//			//	pState->Handler_EGS_LEAVE_ROOM_REQ();
//			//}
//
//		} break;
//	}
//}





// void CX2StateAutoChanger::InCase_XS_ARCADE_GAME()
// {
// 	CX2StateArcadeGame* pState = (CX2StateArcadeGame*) g_pMain->GetNowState();
// 	
// 	switch( m_iTargetStateID )
// 	{
// 	default:
// 		EndStateChange();
// 		break;
// 
// 	case CX2Main::XS_VILLAGE_MAP:
// 	case CX2Main::XS_PVP_LOBBY:
// 	case CX2Main::XS_PVP_ROOM:
// 	case CX2Main::XS_PVP_GAME:
// #ifdef ELSWORD_NEW_BEGINNING
// 	case CX2Main::XS_BEGINNING:
// #else
// 	case CX2Main::XS_SERVER_SELECT:
// #endif ELSWORD_NEW_BEGINNING
// 	case CX2Main::XS_TRAINING_SCHOOL:
// 	case CX2Main::XS_DUNGEON_ROOM:
// 	case CX2Main::XS_DUNGEON_GAME:
// 		{
// 			if( NULL != g_pX2Game )
// 			{
// 				// note!! ��������(or �����̵� ����)������ leave_game��� leave_room�� ���
// 				g_pX2Game->Handler_EGS_LEAVE_ROOM_REQ(); // fix!!! �ϴ� �ӽ÷� ������ ���� ������
// 			}
// 			else
// 			{
// 				EndStateChange();
// 			}
// 
// 		} break;
// 	}
// }

//void CX2StateAutoChanger::TryEnterDungeonRoom( bool bIsPublic /*= true */)
//{
//	CX2StateLocalMap* pState = (CX2StateLocalMap*) g_pMain->GetNowState();
//
//	if( -1 == m_TargetDetail.m_iRoomUID )
//	{
//		// fix!! ���⿡�� �ٷ� �ʽ��ڽ����� ������ �ϵ��ڵ��� �κ� �Է����ڸ� �޾Ƽ� ó���ϵ��� ���ľ���. �ϴ� �̷��� ����~
//		pState->Handler_EGS_CREATE_ROOM_REQ( L"Rookie's", L"m3af*Aq1", false, 1, 
//			(int) SEnum::DI_EL_FOREST_GATE_NORMAL, (int) CX2Dungeon::DL_NORMAL, 19999.f );
//
//		m_iTryEnterRoomCount++; // note: �ι� create_room_req �ϴ� ��츦 ���� ���ؼ� 
//	}
//	else
//	{
//		// ��й��̸� 
//		if( false == bIsPublic )
//		{
//			pState->OpenDLGJoinPasswordRoom( m_TargetDetail.m_iRoomUID );
//		}
//		else // �������̸� �ٷ� ���νõ� 
//		{
//			pState->Handler_EGS_JOIN_ROOM_REQ( m_TargetDetail.m_iRoomUID );	
//		}
//	}
//
//	m_iTryEnterRoomCount++;
//}


void CX2StateAutoChanger::TryEnterPVPRoom( bool bIsPublic /* = true */ )
{
	CX2StatePvPLobby* pState = (CX2StatePvPLobby*) g_pMain->GetNowState();

	// ��й��̸� 
	if( false == bIsPublic )
	{
#ifdef SERV_NEW_PVPROOM_PROCESS
		if( m_TargetDetail.m_bInvitation == true )
		{
			pState->Handler_EGS_JOIN_ROOM_REQ( m_TargetDetail.m_iRoomUID );
		}
		else
		{
			pState->OpenDLGJoinPasswordRoom( m_TargetDetail.m_iRoomUID );
		}
#else
		pState->OpenDLGJoinPasswordRoom( m_TargetDetail.m_iRoomUID );
#endif SERV_NEW_PVPROOM_PROCESS

		// fix!! ���� �õ��� �����ϸ� ���� state�� �̵�
	}
	else // �������̸� �ٷ� ���νõ� 
	{
		pState->Handler_EGS_JOIN_ROOM_REQ( m_TargetDetail.m_iRoomUID );
	}

	m_iTryEnterRoomCount++;
}


void CX2StateAutoChanger::TryEnterArcadeRoom( bool bIsPublic /* = true */ )
{
	// fix!! �����̵� �ڵ� ����
	//CX2StateArcadeLobby* pState = (CX2StateArcadeLobby*) g_pMain->GetNowState();

	//// ��й��̸� 
	//if( false == bIsPublic )
	//{
	//	pState->OpenDLGJoinPasswordRoom( m_TargetDetail.m_iRoomUID );

	//	// fix!! ���� �õ��� �����ϸ� ���� state�� �̵�
	//}
	//else // �������̸� �ٷ� ���νõ� 
	//{
	//	pState->Handler_EGS_JOIN_ROOM_REQ( m_TargetDetail.m_iRoomUID );
	//}

	//m_iTryEnterRoomCount++;
}



//void CX2StateAutoChanger::TryEnterDungeonChannel()
//{
//	CX2StateLocalMap* pState = (CX2StateLocalMap*) g_pMain->GetNowState();
//
//	CX2LocationManager::LOCAL_MAP_ID eLocalMapID = 
//		g_pData->GetLocationManager()->GetLocalMapID( (SEnum::DUNGEON_ID) m_TargetDetail.m_iDungeonID );
//
//	if( CX2LocationManager::LMI_INVALID == eLocalMapID )
//	{
//		// error
//		EndStateChange();
//	}
//	else
//	{
//		if( (int)eLocalMapID == g_pMain->GetNowDetailStateID() )
//		{
//			pState->OpenDLGDetailLocal( CX2StateLocalMap::DLT_DUNGEON, m_TargetDetail.m_iDungeonID, NULL, false );
//			// ������ ������ statelocalmap���� ó�� 
//		}
//		else
//		{
//			pState->Handler_EGS_STATE_CHANGE_WORLD_MAP_REQ();
//		}
//	}
//}





void CX2StateAutoChanger::DoStateChange( int iNowStateID )
{
	if( false == m_bIsAutoChangingState )
		return;

	WCHAR buff[256] = {0};
	StringCchPrintf( buff, 256, L"\nAutoChanger::DoStateChange id = %d\n", iNowStateID );
	//wsprintf( buff, L"\nAutoChanger::DoStateChange id = %d\n", iNowStateID );
	DialogLog( buff );

	switch( iNowStateID )
	{

#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
		{
			InCase_XS_SERVER_SELECT();
		} break;

#else
	//case CX2Main::XS_UNIT_SELECT:
	case CX2Main::XS_SERVER_SELECT:
		{
			InCase_XS_SERVER_SELECT();
		} break;
#endif ELSWORD_NEW_BEGINNING

#ifndef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_CREATE_UNIT:
		{
			InCase_XS_CREATE_UNIT();

		} break;
#endif ELSWORD_NEW_BEGINNING
	case CX2Main::XS_PVP_LOBBY:
		{
			InCase_XS_PVP_LOBBY();

		} break;

	case CX2Main::XS_PVP_ROOM:	
		{
			InCase_XS_PVP_ROOM();
		} break;

	case CX2Main::XS_DUNGEON_ROOM:
		{
			InCase_XS_DUNGEON_ROOM();

		} break;

	case CX2Main::XS_DUNGEON_RESULT:
		{
			InCase_XS_DUNGEON_RESULT();
		} break;

	case CX2Main::XS_VILLAGE_MAP:
		{
			InCase_XS_VILLAGE_MAP();
		} break;

	//case CX2Main::XS_LOCAL_MAP:
	//	{
	//		InCase_XS_LOCAL_MAP();
	//	} break;

	//case CX2Main::XS_WORLD_MAP:
	//	{
	//		InCase_XS_WORLD_MAP();
	//	} break;

	//case CX2Main::XS_SHOP:
	//	{
	//		InCase_XS_SHOP();
	//	} break;

	//case CX2Main::XS_NPC_HOUSE:
	//	{
	//		InCase_XS_NPC_HOUSE();
	//	} break;

	case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			InCase_XS_SQUARE_GAME();
		} break;

	case CX2Main::XS_TRAINING_SCHOOL:
		{
			InCase_XS_TRAINING_SCHOOL();
		} break;

	//case CX2Main::XS_ALCHEMIST_HOUSE:
	//	{
	//		InCase_XS_ALCHEMIST_HOUSE();
	//	} break;

	case CX2Main::XS_DUNGEON_GAME:
		{
			InCase_XS_DUNGEON_GAME();
		} break;

	case CX2Main::XS_BATTLE_FIELD:
		{
			InCase_XS_BATTLE_FIELD();
		} break;

	//case CX2Main::XS_CLASS_CHANGE:
	//	{
	//		InCase_XS_CLASS_CHANGE();
	//	} break;

	//case CX2Main::XS_ARCADE_ROOM:
	//	{
	//		InCase_XS_ARCADE_ROOM();
	//	} break;

	
	default:
		{
			// error
			EndStateChange();
		}
	}
}

void CX2StateAutoChanger::InCase_XS_BATTLE_FIELD()
{
	CX2StateBattleField* pStateBattleField = static_cast<CX2StateBattleField*>( g_pMain->GetNowState() );

	switch( m_iTargetStateID )
	{
	default:
		EndStateChange();
		break;

	case CX2Main::XS_SERVER_SELECT:
		{
			pStateBattleField->Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
		} break;
	}
}


