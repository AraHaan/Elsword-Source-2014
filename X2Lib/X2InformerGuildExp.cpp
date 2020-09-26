//{{ kimhc // 2009-11-02 // ���� �ڽſ� ���� ��� ����ġ�� �ö��� �� ���� �Ǵ� informer
#include "StdAfx.h"
#ifdef	GUILD_MANAGEMENT

#include ".\X2InformerGuildExp.h"

CX2InformerGuildExp::CX2InformerGuildExp()
: m_bIncreaseGuildExpByMe( false )
{
	m_InformerType				= CX2Informer::XIT_GUILD_EXP;

#ifdef SERV_PET_SYSTEM
	m_vTalkBoxPos				= D3DXVECTOR2( 882, 702 );
#else
	m_vTalkBoxPos				= D3DXVECTOR2( 521, 711 );
#endif
}

CX2InformerGuildExp::~CX2InformerGuildExp()
{

}

void CX2InformerGuildExp::Reset()
{
	SetIncreaseGuildExpByMe( false );
}


void CX2InformerGuildExp::SetNowInfo()
{
}

bool CX2InformerGuildExp::IsNowInfoChange()
{
	// �ٸ� üũ�� �߰� �� ��� �� �ؿ� else if�� ����
	if ( GetIncreaseGuildExpByMe() == true )	
		return true;

	return false;
}

void CX2InformerGuildExp::SetShow( bool bShow )
{
	CX2Informer::SetShow( bShow );
	
	if ( bShow == true )
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;


		//ĳ���� ���¿� ���� �޴� ��ư�� ��ġ�� �޶� ���� ������ ��ġ ����
		talkBox.m_vPos				= m_vTalkBoxPos+m_vTalkBoxOffsetPos;
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;

		if ( GetIncreaseGuildExpByMe() == true )
		{
			SetIncreaseGuildExpByMe( false );
			talkBox.m_OwnerUnitUID		= XUUII_GUILD_EXP;
			talkBox.m_wstrTalkContent	= GET_STRING( STR_ID_4723 );
			talkBox.m_fRemainTime		= 4.0f;
		}

		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
		talkBox.m_coTextColor		= D3DXCOLOR(0,0,0,1);

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"MessageAlarm.ogg", false, false );

		m_pTalkBoxManager->Push( talkBox );
	}
	else
	{
		m_pTalkBoxManager->DeleteByIndex( m_pTalkBoxManager->GetTalkBoxNum() - 1 );
	}
}

HRESULT	CX2InformerGuildExp::OnFrameMove( double fTime, float fElapsedTime )
{
	
	return CX2Informer::OnFrameMove( fTime, fElapsedTime );
}

#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-11-02 // ���� �ڽſ� ���� ��� ����ġ�� �ö��� �� ���� �Ǵ� informer