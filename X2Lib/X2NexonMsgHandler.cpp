#include "stdafx.h"
#include ".\x2nexonmsghandler.h"

#ifdef _NEXON_KR_

CX2NexonMsgHandler::CX2NexonMsgHandler()
{
}

CX2NexonMsgHandler::~CX2NexonMsgHandler()
{
}



//
//	������������ ������ �������� ��쿡 �߻�
//
void CX2NexonMsgHandler::OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent * pEvent )
{
	//ClientUtil::AddLog( g_log, _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->uType, ClientUtil::AuthResultToString( static_cast<NMLoginAuthReplyCode>( pEvent->uType ) ) );
}

#ifndef NEW_MESSENGER
//{{AFX
//
//	�޽����������� ������ ����Ǿ��� ��쿡 �߻�
//
void CX2NexonMsgHandler::OnMessengerReplyEvent( CNMMessengerReplyEvent * pEvent )
{
	//ClientUtil::AddLog( g_log, _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->nResultCode, ClientUtil::MessengerResultToString( static_cast<NMMessengerReplyCode>( pEvent->nResultCode ) ) );

	if ( pEvent->nResultCode == kMessengerReplyOK || pEvent->nResultCode == kMessengerReplyNewUser )
	{

		//if ( pEvent->nResultCode == kMessengerReplyOK )
		//{
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"�޽��� ���� ����", g_pMain->GetNowState() );
		//}

		if ( g_pData != NULL && g_pData->GetMessenger() != NULL && 
			g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
		{
			g_pData->GetMessenger()->ResetUserList();
		}
	}
}
//
//	�޽����������� ������ �������� ��쿡 �߻�
//
void CX2NexonMsgHandler::OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent * pEvent )
{
	//ClientUtil::AddLog( g_log, _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->uType, ClientUtil::MessengerConnectionClosedEventToString( pEvent->uType ) );
}


void CX2NexonMsgHandler::OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent )
{
	if ( g_pData != NULL && g_pData->GetMessenger() != NULL )
	{
		g_pData->GetMessenger()->OnRequestNewFriendEvent( pEvent  );
		//g_pData->GetMessenger()->OnRequestNewFriendEvent( pEvent->rnfData.keyToVirtual.uVirtualIDCode, pEvent->rnfData.uSerialNo, 
		//	pEvent->rnfData.szFromNickName, pEvent->rnfData.szMessage  );
	}
}

void CX2NexonMsgHandler::OnRefreshEvent( CNMRefreshEvent* pEvent )
{
	//ģ������Ʈ, ���� ���� ����Ʈ, ���� ���߾� ���� ����Ʈ ���� ����Ǿ����Ƿ� 
	//���� �о�ͼ� �ݿ��� �־�� �Ѵ�.
	if ( pEvent->uType == CNMRefreshEvent::kType_UserDataList )	
	{
		if ( g_pData != NULL && g_pData->GetMessenger() != NULL && 
			g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			g_pData->GetMessenger()->SetRefresh();
			g_pData->GetMessenger()->ResetUserList();
		}
	}
}

void CX2NexonMsgHandler::OnCustomMessageEvent( CNMCustomMessageEvent* pEvent )
{
	switch( pEvent->msgData.nCode )
	{
		case kCustomMsgCode_RequestFriendOK:		// ģ�� ��� ��û ����
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_368 ), g_pMain->GetNowState() );
			}
			break;
		case kCustomMsgCode_RequestFriendFailed:	// ģ�� ��� ��û ����
			{										// TODO : ģ�� ��� ��û ���п� ���� �޽���(pEvent->msgData.szMessage) ������.
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300),pEvent->msgData.szMessage, g_pMain->GetNowState() );
			}	
			break;
		case kCustomMsgCode_NoteReply: 				// ���� ������ ���
			{
				
			}
			break;
	}

}


void CX2NexonMsgHandler::OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent )
{
	g_pData->GetMessenger()->OnFriendInfoChangedEvent( pEvent );
}

//}}AFX
#endif NEW_MESSENGER

// 09.07.02 �¿� : �� �Ʒ��� �Լ����� �׳� ������ ��� Link Error�� ���� ������ �̸� ���ϱ� ���ؼ� NEW_MESSENGER�� ���� Dummyȭ.
// �ؽ� ��� ������ ���� �����ص� �ȴ�.
void CX2NexonMsgHandler::OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionCreatedEvent( pEvent );
#endif	
}

void CX2NexonMsgHandler::OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionInfoChangedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnCSChatMessageReceivedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionEstablishedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionClosedEvent( pEvent );
#endif
}

void CX2NexonMsgHandler::OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent )
{
#ifndef NEW_MESSENGER
	g_pData->GetMessenger()->OnGSSessionFailedEvent( pEvent );
#endif
}




#endif _NEXON_KR_
