#include "GSSimLayer.h"
#include "GSUser.h"
#include "XTrapCSM.h"
#include "NetError.h"

#define VERIFY_ENABLE( reCode ) \
	if( GetEnable() == false ) \
	{ \
	reCode; \
	}

#ifdef SERV_USE_XTRAP
CXTrapCSM::CXTrapCSM() : m_bEnable(false), 
						 m_bTimerStart(false)
{

}
CXTrapCSM::~CXTrapCSM()
{

}

bool CXTrapCSM::InitCSM()
{
	VERIFY_ENABLE( return true )

	XTrap_S_SessionInit( 600, GetKGSSimLayer()->GetMapQuantity(), GetKGSSimLayer()->GetAllMapPointer(), m_cSessionBuf );
	return true;
}

// step1 : ���� ��Ŷ ���� Ŭ�� ������.
bool CXTrapCSM::CSMStep1( IN KGSUserPtr spUser )
{
	VERIFY_ENABLE( return true )

	static DWORD dwExecute = 0;
	if( ( GetTickCount() - dwExecute ) <= 20000 )
	{
		return true;
	}
		
	KEGS_XTRAP_REQ kPacket;
	kPacket.m_vecData.resize(200);

	unsigned char temp[200];
	INT nRet = XTrap_CS_Step1(m_cSessionBuf, temp);

	for( u_int ui = 0; ui < 200; ++ui )
	{
		kPacket.m_vecData[ui] = temp[ui];
	}

	spUser->SendPacket(EGS_XTRAP_REQ, kPacket);

	if( nRet == XTRAP_API_RETURN_OK )		// ��ȯ���� ������ ���
	{
		dwExecute = GetTickCount();

		if( !m_bTimerStart )
		{
			m_bTimerStart = true;
			m_kTimer.restart();
		}
		else
		{
			if(m_kTimer.elapsed() >= ST_LOOP_AUTH_TIME)
			{
				spUser->SetDisconnectReason(KStatistics::eSIColDR_nProtect_Hacking);
				spUser->ReserveDestroy();	
			}
		}

		return true;
	}
	else
	{
		START_LOG( cerr, L"XTrap_CS_Step1 ���� ���� :" << nRet );

		if( nRet == XTRAP_API_RETURN_DETECTHACK )
		{
			unsigned int DetectCode = 0;
			// ���� ���� �ּ�
			//memcpy( &DetectCode, (static_cast<unsigned char*>(m_cSessionBuf)) + 8, 4 );

			//START_LOG( cerr, L"����1, DetectCode �� : " << std::hex << DetectCode );
		}

		return false;
	}
}

// step3 : Ŭ���̾�Ʈ�κ��� ��Ŷ�� �޴´�
bool CXTrapCSM::CSMStep3(KEGS_XTRAP_ACK* _packet)
{
	VERIFY_ENABLE( return true )

	m_kTimer.restart();
	unsigned char arrResult[200];

	for( u_int ui = 0; ui < 200; ++ui )
	{
		arrResult[ui] = _packet->m_vecData[ui];
	}

	unsigned int nRet = XTrap_CS_Step3( m_cSessionBuf, arrResult );
	if( nRet == XTRAP_API_RETURN_OK )	// ��ȯ���� ������ ���
	{
		return true;
	}
	else
	{
		START_LOG( cerr, L"XTrap step3 ���� ���� : " << nRet );

		if( nRet == XTRAP_API_RETURN_DETECTHACK )
		{
			unsigned int DetectCode=0;
			memcpy( &DetectCode, ((unsigned char *)m_cSessionBuf+8), 4 );

			START_LOG( cerr, L"����3, DetectCode �� : " << std::hex << DetectCode );
		}

		return false;
	}
}
#endif SERV_USE_XTRAP