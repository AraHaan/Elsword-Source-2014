#include "StdAfx.h"
#include ".\x2noticemanager.h"



CX2NoticeManager::CX2NoticeManager( int noticeGapWidth )
{
	m_NoticeGapWidth = noticeGapWidth;
	for(UINT i = 0; i < MAX_NOTICE_NUM; i++)
	{
		m_pDlgTakeItemBackBg[i] = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Notice_Background.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTakeItemBackBg[i] );
		m_pDlgTakeItemBackBg[i]->SetShow(false);
		m_pDlgTakeItemBackBg[i]->SetPos(D3DXVECTOR2(0.f, 748.f-i*20.f));
		m_pDlgTakeItemBackBg[i]->AddDummyInt(i);

		m_fTakeItemNoticeTime[i] = 0.0f;	/// �ʱ�ȭ
	}
#ifdef SERV_SHUTDOWN_SYSTEM	// ���� ������ �ؿ��� �߰�
	bShutDownNotice = false;
#endif // SERV_SHUTDOWN_SYSTEM

#ifdef ADD_HERO_MATCH_NOTICE
	m_bHeroMatchWisperNotice = false;		/// �ӼӸ� ������ �����ؾ� �� �� ���ÿ� ����
#endif ADD_HERO_MATCH_NOTICE
}

CX2NoticeManager::~CX2NoticeManager(void)
{
	for(UINT i = 0; i < MAX_NOTICE_NUM; i++)
	{
		SAFE_DELETE_DIALOG(m_pDlgTakeItemBackBg[i])
	}
	//SAFE_CLOSE( m_pTextureTakeItemBackBg );

	while( m_queNoticeData.empty() == false )
	{
		NoticeData* pNoticeData = m_queNoticeData.front();
		SAFE_DELETE( pNoticeData );

		m_queNoticeData.pop();
	}

	while( m_queTakeItemNoticeData.empty() == false )
	{
		NoticeData* pNoticeData = m_queTakeItemNoticeData.front();
		SAFE_DELETE( pNoticeData );

		m_queTakeItemNoticeData.pop();
	}	
}


bool CX2NoticeManager::AddNotice( KEGS_NOTIFY_MSG_NOT::NOTIFY_TYPE eNotifyType, const WCHAR* pMsg, int repeatNum /* = 1 */ )
{
	if ( pMsg == NULL )
		return false;

	NoticeData* pNoticeData = new NoticeData();	
	pNoticeData->m_Msg			= L"* ";
	pNoticeData->m_Msg			+= pMsg;
	pNoticeData->m_RepeatCount	= repeatNum;	



	// �ӽ÷� �ߺ��� ������ �����ؼ� ���ƿ��� �����ϵ��� �Ѵ�
	//{
	//	std::queue< NoticeData* > queTemp = m_queNoticeData;
	//	while( false == queTemp.empty() )
	//	{
	//		NoticeData* pTempNoticeData = queTemp.front();
	//		queTemp.pop();

	//		if( 0 == pTempNoticeData->m_Msg.compare( pNoticeData->m_Msg ) )
	//		{
	//			return false;
	//		}
	//	}


	//	for( UINT i=0; i< m_TextListToDelete.size(); i++ )
	//	{
	//		CX2TextManager::CX2Text* pX2Text = m_TextListToDelete[i];
	//		if( pX2Text != NULL )
	//		{
	//			if( g_pMain->GetTextManager()->CheckText( pX2Text ) == true )
	//			{
	//				if( 0 == pNoticeData->m_Msg.compare( pX2Text->GetTextMsg() ) )
	//				{
	//					return false;
	//				}
	//			}
	//		}
	//	}

	//	for( UINT i=0; i<m_vecSpamNotice.size(); i++ )
	//	{
	//		SpamNotice& spamNotice = m_vecSpamNotice[i];
	//		if( spamNotice.m_Timer.elapsed() > 300.f )
	//		{
	//			m_vecSpamNotice.erase( m_vecSpamNotice.begin() + i );
	//			i--;
	//			continue;
	//		}

	//		if( 0 == spamNotice.m_Msg.compare( pNoticeData->m_Msg ) )
	//		{
	//			return false;
	//		}
	//	}


	//	SpamNotice spamNotice;
	//	spamNotice.m_Timer.restart();
	//	spamNotice.m_Msg = pNoticeData->m_Msg;
	//	m_vecSpamNotice.push_back( spamNotice );
	//}

	//{{ 2010. 05. 10  ������	�������� ������ ����
	if(eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS ||
		eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_SERVER_GROUP
#ifdef SERV_SHUTDOWN_SYSTEM
		|| eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY
#endif
		)
	//}}
	{
		m_queNoticeData.push( pNoticeData );
	}
	else if( eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_MAX_LEVEL_UP ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_ENCHANT_SUCCESS ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_HENIR_CLEAR ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GUILD_CREATION ||
			 eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GUILD_LEVEL_UP 
#ifdef SERV_GUILD_CHANGE_NAME
			 || eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GUILD_NAME_CHANGED
#endif //CHANGE_GUILD_NAME
#ifdef SERV_RELATIONSHIP_SYSTEM
			 || eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_WEDDING_SUCCESS
#endif //SERV_RELATIONSHIP_SYSTEM
			 )
	{
		m_queTakeItemNoticeData.push( pNoticeData );
	}
	else
	{
		SAFE_DELETE(pNoticeData);
	}

#ifdef SERV_SHUTDOWN_SYSTEM
	if ( eNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
		bShutDownNotice = true;
	else
		bShutDownNotice = false;
#endif

	return true;
}

HRESULT	CX2NoticeManager::OnFrameMove( double fTime, float fElapsedTime )
{
	// �Ϲݰ���
	for ( int i = 0; i < (int)m_TextListToDelete.size(); i++ )
	{
		CX2TextManager::CX2Text* pX2Text = m_TextListToDelete[i];
		if ( pX2Text != NULL )
		{
			if ( g_pMain->GetTextManager()->CheckText( pX2Text ) == false )
			{
				m_TextListToDelete.erase( m_TextListToDelete.begin() + i );
				i--;
			}
		}
	}

	if ( m_TextListToDelete.empty() == true )
	{
		if ( m_queNoticeData.empty() == false )
		{
			AddNoticeData();
		} 
	}
	else
	{
		if ( m_queNoticeData.empty() == false )
		{
			CX2TextManager::CX2Text* pCXText = m_TextListToDelete[ m_TextListToDelete.size() - 1 ];
			if ( pCXText != NULL )
			{
				int textWidth = pCXText->GetNowTextWidth();
				D3DXVECTOR2 textPos = pCXText->GetPos();

				if ( textPos.x + textWidth < 1024 - m_NoticeGapWidth )
				{
					AddNoticeData();
				}
			}
		}
	}
	// ������ ȹ�� ����
	//�ð� �ٵ� ���� ����
	for( UINT i = 0; i < MAX_NOTICE_NUM ; i++)
	{
		m_fTakeItemNoticeTime[i] -= fElapsedTime;

		if( m_fTakeItemNoticeTime[i] < 0.f)
		{
			m_pDlgTakeItemBackBg[i]->SetShow(false);
		}
		else
		{
			m_pDlgTakeItemBackBg[i]->SetShow(true);
		}
	}

	//���� ���
	AddTakeItemNoticeData();

	return S_OK;
}

void CX2NoticeManager::AddNoticeData()
{
	NoticeData* tempNotice = m_queNoticeData.front();

	if ( tempNotice != NULL )
	{
		tempNotice->m_RepeatCount -= 1;

		CX2TextManager::CX2Text* pCXText;
#ifdef SERV_SHUTDOWN_SYSTEM	//#CFF00FF
		if ( true == bShutDownNotice )
			pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024., 260 ), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,0.35f,0,1), D3DXCOLOR(0,0,0,1) );
#ifdef ADD_HERO_MATCH_NOTICE		/// �ӼӸ� ������ ���ֻ� �� �Ʒ� ��ġ�� ǥ��
		else if ( true == GetHeroMatchWisperNotice() )
			pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024.f, 400.f ), tempNotice->m_Msg.c_str(), D3DXCOLOR(1.f,0.f,1.f,1.f), D3DXCOLOR(0,0,0,1) );
#endif ADD_HERO_MATCH_NOTICE
		else
			pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024, 260 ), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1) );
#else
		pCXText = g_pMain->GetTextManager()->AddText( XUF_DODUM_20_BOLD, D3DXVECTOR2( 1024, 260 ), tempNotice->m_Msg.c_str(), 
			D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1) );
#endif

		//20���ڸ� �������� 10�ʶ�� �����Ѵ�?
		float fChangeTime;
		fChangeTime = 23.0f * ( (float)pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) + 1024.0f ) / 1024.0f;

#ifdef SERV_SHUTDOWN_SYSTEM
		if ( true == bShutDownNotice )
			pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 260), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,0.35f,0,1), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#ifdef ADD_HERO_MATCH_NOTICE		/// �ӼӸ� ������ ���ֻ� �� �Ʒ� ��ġ�� ǥ��
		else if ( true == GetHeroMatchWisperNotice() )
			pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 400.f), tempNotice->m_Msg.c_str(), D3DXCOLOR(1.f,0.f,1.f,1.f), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#endif ADD_HERO_MATCH_NOTICE
		else
			pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 260), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#else
		pCXText->Move( D3DXVECTOR2( (float)(0 - pCXText->GetTextWidth( tempNotice->m_Msg.c_str() ) ) , 260), tempNotice->m_Msg.c_str(), D3DXCOLOR(1,1,0,1), D3DXCOLOR(0,0,0,1), fChangeTime, false, true );
#endif

		m_TextListToDelete.push_back( pCXText );
		if ( tempNotice->m_RepeatCount <= 0 )
		{
			SAFE_DELETE( tempNotice );
			m_queNoticeData.pop();
		}
	}

#ifdef ADD_HERO_MATCH_NOTICE
	if( true == GetHeroMatchWisperNotice() )
		SetHeroMatchWisperNotice( false );
#endif ADD_HERO_MATCH_NOTICE
}

void CX2NoticeManager::AddTakeItemNoticeData()
{
	if( m_queTakeItemNoticeData.empty() == true )
	{
		return;
	}

	NoticeData* tempNotice = m_queTakeItemNoticeData.front();
	if ( tempNotice != NULL )
	{
		//�� �˸� �� �޼����� ��Ʈ�� ���
		CKTDGUIStatic *pStatic[MAX_NOTICE_NUM];
		for( UINT i = 0; i < MAX_NOTICE_NUM; ++i)
		{
			if( NULL != m_pDlgTakeItemBackBg[i] )
			{
				pStatic[i] = (CKTDGUIStatic*)m_pDlgTakeItemBackBg[i]->GetControl(L"Notice_Message");
			}
		}
		
		//������ �߰��Ǹ� ���� ������ �ϳ��� ���� �ö󰡵��� ����.
		for( UINT i = MAX_NOTICE_NUM; i > 1 ; --i)
		{
			if( NULL != pStatic[i-1] && NULL != pStatic[i-2])
			{
				pStatic[i-1]->GetString(0)->msg = pStatic[i-2]->GetString(0)->msg;
				m_fTakeItemNoticeTime[i-1] = m_fTakeItemNoticeTime[i-2];
			}
		}

		//���Ӱ� ���� ���� �޼��� ����
		if(NULL != pStatic[0] && NULL != m_pDlgTakeItemBackBg[0])
		{
			pStatic[0]->GetString(0)->msg = tempNotice->m_Msg;
			m_fTakeItemNoticeTime[0] = 6.f;
			m_pDlgTakeItemBackBg[0]->SetShow(true);
		}

		tempNotice->m_RepeatCount = 0;

		//ä�� �α� �߰�
		D3DXCOLOR _color = D3DXCOLOR( 1,1,0,1 );
		if ( g_pChatBox != NULL )
			g_pChatBox->AddChatLog( tempNotice->m_Msg.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, _color, L"#CFFFF00" );

		//�߰��� ���� ����
		if ( tempNotice->m_RepeatCount <= 0 )
		{
			SAFE_DELETE( tempNotice );
			m_queTakeItemNoticeData.pop();
		}
	}
}
void CX2NoticeManager::CloseItemNotice(int iNum)
{

	//�� �˸� �� �޼����� ��Ʈ�� ���
	CKTDGUIStatic *pStatic[MAX_NOTICE_NUM];
	for( UINT i = 0; i < MAX_NOTICE_NUM; ++i)
	{
		if( NULL != m_pDlgTakeItemBackBg[i])
		{
			pStatic[i] = (CKTDGUIStatic*)m_pDlgTakeItemBackBg[i]->GetControl(L"Notice_Message");
		}
	}

	//���� ���ŵǸ� �ϳ��� �Ʒ��� �е��� ����
	for( UINT i = iNum; i< MAX_NOTICE_NUM-1; i++)
	{
		if( NULL != pStatic[i] && NULL != pStatic[i+1])
		{
			pStatic[i]->GetString(0)->msg = pStatic[i+1]->GetString(0)->msg;
			m_fTakeItemNoticeTime[i] = m_fTakeItemNoticeTime[i+1];
		}
	}

	//���° ���� ���� �ϳ��� ������ �������� �ϱ� ������ �� ������ ����
	if( NULL != pStatic[MAX_NOTICE_NUM-1] )
	{
		pStatic[MAX_NOTICE_NUM-1]->GetString(0)->msg = L"";
		m_fTakeItemNoticeTime[MAX_NOTICE_NUM-1] = 0.f;
	}
}