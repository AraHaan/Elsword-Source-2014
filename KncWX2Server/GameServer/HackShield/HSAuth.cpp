#include "GSUser.h"
#include "HSAuth.h"
#include "NetError.h"

#define VERIFY_ENABLE( reCode ) \
	if( GetEnable() == false ) \
	{ \
	reCode; \
	}


AHNHS_SERVER_HANDLE KHSAuth::ms_hServer = NULL;
std::string	KHSAuth::ms_HSBFilePath;


KHSAuth::KHSAuth()
:
m_bEnable(false),
m_hClient(ANTICPX_INVALID_HANDLE_VALUE),
m_bIsStarted(false),
m_bIsSendReq(false),
m_AuthTickTime( ST_LOOP_AUTH_TIME_MAX )
{	
}

KHSAuth::~KHSAuth()
{
	if( m_bEnable  ||  m_hClient != ANTICPX_INVALID_HANDLE_VALUE )
	{
		_AhnHS_CloseClientHandle( m_hClient );

		//{{ 2009. 11. 17  ������	�ٽ��� ����ó��
		// �ڵ��� �ݾҴٸ� �ʱ�ȭ ���ݴϴ�.
		m_hClient = ANTICPX_INVALID_HANDLE_VALUE;
		//}}
	}

	m_bEnable = false;	
}

bool KHSAuth::Tick( IN KGSUserPtr spUser, IN bool bForce /*= false*/ )
{
	VERIFY_ENABLE( return true );

	if( m_bIsStarted == true )
	{
		if( m_kTimer[TM_LOOP_AUTH_TIME].elapsed() >= m_AuthTickTime  ||  bForce == true )
		{
			//{{ 2009. 8. 28  ������	req check
			if( m_bIsSendReq == true )
			{
				if( bForce )
				{
					START_LOG( cout, L"Ŭ���̾�Ʈ�� HackShield Check��û�� �Ͽ����� �̹� REQ��Ŷ�� Ŭ���̾�Ʈ�� �������� ���¶� �ƹ��� ó�� ����." )
						<< BUILD_LOG( spUser->GetUID() )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< END_LOG;

					return true;
				}

#ifdef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
				// ĳ�ü����� Ŭ���̾�Ʈ�� �� �������� ���� ��, 
				// ���� �����尡 ���̴� ������ ����, ĳ�ü������� ������ ���������� �Ǵ� ��
				if( true == spUser->GetEnterCashShop() )
				{
					return true;
				}
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

				START_LOG( cerr, L"check req�� ���� ack������ ���� �ʾ���." )
					<< BUILD_LOG( spUser->GetUID() )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< END_LOG;

				m_bIsStarted = false;				
				return false;
			}
			//}}

			if( SendHackShiendCheckReq( spUser ) == false )
			{
				m_bIsStarted = false;
				return false;
			}

			m_kTimer[TM_LOOP_AUTH_TIME].restart();
			RandTickTime();
		}
	}

	return true;
}

bool KHSAuth::FirstHackShieldAuth( IN KGSUserPtr spUser )
{
	VERIFY_ENABLE( return true );

	// ���� �ڵ��� �̿��� ������ ������ �ڵ��� ����
	m_hClient = _AhnHS_CreateClientObject( KHSAuth::GetAhnHSServerHandle() );

	if( m_hClient == ANTICPX_INVALID_HANDLE_VALUE )
	{
		START_LOG( cerr, L"hackshield client object ���� ���� ����!" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;

		return false;
	}

	// send req
	if( SendHackShiendCheckReq( spUser ) == false )
	{
		START_LOG( cerr, L"hackshield req �޽��� ó�� ������ ���� ����!" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;

		return false;
	}

	// set first auth timer
	m_kTimer[TM_LOOP_AUTH_TIME].restart();
	RandTickTime();
	m_bIsStarted = true;

	return true;
}

bool KHSAuth::SendHackShiendCheckReq( IN KGSUserPtr spUser )
{
	AHNHS_TRANS_BUFFER stRequestBuf;

	//{{ 2009. 11. 17  ������	�ٽ��� ����ó��	
	if( m_hClient == ANTICPX_INVALID_HANDLE_VALUE )
	{
		START_LOG( cerr, L"�ٽ��� üũ ��Ŷ�� �������µ� �ڵ鰪�� ��ȿ���� �ʳ�?" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;

		return false;
	}
	//}}

#ifdef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
	// ĳ�ü����� Ŭ���̾�Ʈ�� �� �������� ���� ��, 
	// ���� �����尡 ���̴� ������ ����, ĳ�ü������� ������ ���������� �Ǵ� ��
	if( true == spUser->GetEnterCashShop() )
	{
		// �������� ����̱� ������ true ���� ��
		return true;
	}
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

	unsigned long ulRet = _AhnHS_MakeRequest( m_hClient, &stRequestBuf );

	if( ulRet != ERROR_SUCCESS  ||  stRequestBuf.nLength == 0  || stRequestBuf.nLength > ANTICPX_TRANS_BUFFER_MAX )
	{
		START_LOG( cerr, L"hack shield req �޽��� ���� ����!" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( ulRet )
			<< BUILD_LOG( stRequestBuf.nLength )
			<< END_LOG;

		return false;
	}

	// byGuidReqMsg
	KEGS_HACKSHIELD_CHECK_REQ kReq;
	kReq.insert( kReq.begin(), &stRequestBuf.byBuffer[0], &stRequestBuf.byBuffer[stRequestBuf.nLength] ); // buffer ���̰˻� �ݵ�� �ؾ���!

#ifdef SERV_HACKSHILED_SILENCEMODE_CHECK_FIRST
	if(spUser->GetCheckHackShiledSilenceMode())
	{
		// �ѹ� ���� �� Ȯ���ϸ�, �ٽǵ��� �������� �̰� ������ �ʴ´�.
	}
	else
	{
		//spUser->SetCheckHackShiledSilenceMode(true);
		spUser->SendPacket( EGS_HACKSHIELD_CHECK_REQ, kReq );
	}
#else SERV_HACKSHILED_SILENCEMODE_CHECK_FIRST
	spUser->SendPacket( EGS_HACKSHIELD_CHECK_REQ, kReq );
#endif SERV_HACKSHILED_SILENCEMODE_CHECK_FIRST
	//{{ 2009. 8. 28  ������	send check
	m_bIsSendReq = true;
	//}}
	return true;
}

bool KHSAuth::OnHackShieldCheckAck( IN KGSUserPtr spUser, IN KEGS_HACKSHIELD_CHECK_ACK& kInfo )
{
#ifdef HSB_ALWAYS_VALID_IN_THE_CASHSHOP
	// ĳ�ü����� Ŭ���̾�Ʈ�� �� �������� ���� ��, 
	// ���� �����尡 ���̴� ������ ����, ĳ�ü������� ������ ���������� �Ǵ� ��
	if( true == spUser->GetEnterCashShop() )
	{
		m_bIsSendReq = false;
		return true;
	}
#endif // HSB_ALWAYS_VALID_IN_THE_CASHSHOP

	if( kInfo.size() <= 0  ||  kInfo.size() > ANTICPX_TRANS_BUFFER_MAX )
	{
		START_LOG( cerr, L"HackShield Ack ��Ŷsize�� �̻���!" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( kInfo.size() )
			<< END_LOG;

		//���� ������ Ŭ���̾�Ʈ���� �˷��־�� �Ѵ�.
		KPacketOK kShowDown;
		kShowDown.m_iOK = NetError::ERR_KNP_00;
		spUser->SendPacket( EGS_KNPROTECT_USER_SHOWDOWN_NOT, kShowDown );
		return false;
	}

	AHNHS_TRANS_BUFFER stResponseBuf;
	stResponseBuf.nLength = kInfo.size();
	CopyMemory( stResponseBuf.byBuffer, &kInfo[0], kInfo.size() );

	unsigned long pnErrorCode = 0;

	//{{ 2009. 11. 17  ������	�ٽ��� ����ó��	
	if( m_hClient == ANTICPX_INVALID_HANDLE_VALUE )
	{
		START_LOG( cerr, L"�ٽ��� üũ ��Ŷ�� �������µ� �ڵ鰪�� ��ȿ���� �ʳ�?" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;

		return false;
	}
	//}}

	unsigned long ulRet = _AhnHS_VerifyResponseEx( m_hClient, stResponseBuf.byBuffer, stResponseBuf.nLength, &pnErrorCode );

	if( ulRet == ANTICPX_RECOMMAND_CLOSE_SESSION )
	{
		CStringW cstrErrCode;
		cstrErrCode.Format( L"0x%08x", pnErrorCode );

		//{{ 2009. 9. 3  ������		�ٽ��� �����α� �����ؼ� ǥ��
		if( pnErrorCode == 0xe9040019 )
		{
			START_LOG( cout, L"HackShield Ack ��Ŷ ���� ��� : Ŭ���̾�Ʈ���� ��ŷ�� ����" )
				<< BUILD_LOG( spUser->GetName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( pnErrorCode )
				<< BUILD_LOG( cstrErrCode.GetBuffer() );
		}
		else if( pnErrorCode == 0xe904000e )
		{
			START_LOG( cerr, L"HackShield Ack ��Ŷ ���� ��� : HSB������ Ŭ���̾�Ʈ�� ���� �ʽ��ϴ�" )
				<< BUILD_LOG( spUser->GetName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( pnErrorCode )
				<< BUILD_LOG( cstrErrCode.GetBuffer() );
		}
		else if( pnErrorCode == 0xE9040018 )
		{
			START_LOG( cout, L"HackShield Ack ��Ŷ ���� ��� : Ŭ���̾�Ʈ���� �������� �ٽ��忡�� �ʿ��� ����� ���������� ���������� �ʽ��ϴ�. ��ŷ�� �ǽ�!" )
				<< BUILD_LOG( spUser->GetName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( pnErrorCode )
				<< BUILD_LOG( cstrErrCode.GetBuffer() );
		}
		else
		{
			START_LOG( cerr, L"HackShield Ack ��Ŷ ���� ����!" )
				<< BUILD_LOG( spUser->GetName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( pnErrorCode )
				<< BUILD_LOG( cstrErrCode.GetBuffer() )
				<< END_LOG;
		}
		//}}

		//���� ������ Ŭ���̾�Ʈ���� �˷��־�� �Ѵ�.
		KPacketOK kShowDown;
		kShowDown.m_iOK = NetError::ERR_KNP_00;

#ifdef SERV_SECURITY_MODULE_AUTH_FILE_CHECK
		if( pnErrorCode == 0xe904000e && spUser->GetAuthLevel() == SEnum::UAL_DEVELOPER )
		{
			kShowDown.m_iOK = NetError::ERR_HACKSHIELD_00;
		}
#endif SERV_SECURITY_MODULE_AUTH_FILE_CHECK

		spUser->SendPacket( EGS_KNPROTECT_USER_SHOWDOWN_NOT, kShowDown );
		return false;
	}

	//{{ 2009. 8. 28  ������	send check
	m_bIsSendReq = false;
	//}}
	return true;
}

void KHSAuth::RandTickTime()
{
	int iRandNum = rand() % static_cast<int>(ST_LOOP_AUTH_TIME_MAX - ST_LOOP_AUTH_TIME_MIN);
	iRandNum += static_cast<int>(ST_LOOP_AUTH_TIME_MIN);

	m_AuthTickTime = static_cast<double>(iRandNum);
}


