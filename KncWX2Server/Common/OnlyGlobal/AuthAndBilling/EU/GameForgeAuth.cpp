#include "ActorManager.h"
#include "gameforgeauth.h"

#ifdef SERV_COUNTRY_EU

#include "GameForgeAuthManager.h"
#include "NetError.h"


#include "../Common/OnlyGlobal/AuthAndBilling/eu/SoapGameForge/soapH.h" // or whatever it is called, you must already have it
#include "../Common/OnlyGlobal/AuthAndBilling/eu/SoapGameForge/soapStub.h"
#include "../Common/OnlyGlobal/AuthAndBilling/eu/SoapGameForge/ElswordBinding.nsmap" // this is what you have to add to fix the problem


#define CLASS_TYPE  KGameForgeAuth

std::map< int, KGameForgeAuth::KWebMethod >  KGameForgeAuth::ms_mapWebMethodPath;

_ImplementException( KGameForgeAuth );



KGameForgeAuth::KGameForgeAuth(std::string strWsdlPath)
{
	m_strWsdlPath = strWsdlPath;
//	m_WsmlFile = WsmlFile;

	p_gf_soap = NULL;
}

KGameForgeAuth::~KGameForgeAuth()
{
	End( 1500 );
}

// 2010.10.24 ��ȿ��. 
// ���� GameForgeAuth�� �Ŵ����� ��ϵ� ���� KChannelSimLayer�� m_vecpThreadMgr �̹Ƿ�
// ������ Lua�� AddWebMethod�� KGameForgeAuth::Run() ���� ������ ����ȴ�. 
void KGameForgeAuth::Run()     // derived from KThread
{
	std::cout << "KSubThread::Run(), thread start." << std::endl;
	START_LOG( cout2, L"[Test Log] KGameForgeAuth Start" )
		<< END_LOG;


	srand( (unsigned) time( NULL ) );

	// �߰��� �ڵ� //
	SoapInitialize();
	//


	START_LOG( cout2, L"[�׽�Ʈ�α�] KGameForgeAuth Soap Initialized" )
		<< END_LOG;
	
	testfunction();
 
	while( true )
	{
		//{{ 2010/11/11 ��ȿ�� SOAP ��� ���� ���� ���� �� �ٽ� ������ ���� //
		if( p_gf_soap == NULL ){
			SoapInitialize();
		}
		//}}

		DWORD ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

	}

	// �߰��� �ڵ� //
	SoapUninitialize();
	//

	START_LOG( cout2, L"[�׽�Ʈ�α�] KGameForgeAuth Soap End" )
		<< END_LOG;
}

 
void KGameForgeAuth::SoapInitialize()
{
	p_gf_soap = soap_new();
	if( !p_gf_soap )	//// couldn't allocate: stop
	{
		p_gf_soap = NULL;

		START_LOG( cerr, L"Soap_new Error" )
			<< END_LOG;

		return;
	}
}


void KGameForgeAuth::SoapUninitialize()
{
	if( p_gf_soap != NULL )
	{
		soap_destroy(p_gf_soap);
		soap_end(p_gf_soap);
		p_gf_soap = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
void KGameForgeAuth::AddWebMethod( int iSoapAction, std::string szMethod )
{
	if( iSoapAction < 0 || iSoapAction >= GFO_NUM )
	{
		START_LOG( cerr, L"�̻��� SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}


	if( szMethod == "" )
	{
		START_LOG( cerr, L"method �̻�." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	KWebMethod kMethod;
	kMethod.m_iSoapAction = iSoapAction;
	kMethod.m_strMethodPath = szMethod;

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit != ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"�̹� �����ϴ� soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	// ������ �ִ´�.
	ms_mapWebMethodPath[iSoapAction] = kMethod;
}

bool KGameForgeAuth::GetWebMethod( int iSoapAction, KWebMethod& kInfo )
{
	if( iSoapAction < 0 || iSoapAction >= GFO_NUM )
	{
		START_LOG( cerr, L"�̻��� SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit == ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	kInfo = mit->second;

	return true;
}

/*
DISPID KGameForgeAuth::GetIDOfName(LPOLESTR lpName)
{

	HRESULT hr;
	DISPID dispid;

	if( m_pSoapClient == NULL )
	{
		START_LOG( cerr, L"m_pSoapClient Not Initialize." )
			<< BUILD_LOG( lpName )
			<< END_LOG;

		return -1;
	}

	hr = m_pSoapClient->GetIDsOfNames(IID_NULL, &lpName, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if( FAILED(hr) ) 
	{
		START_LOG( cerr, L"Soap GetIDsOfNames failed." )
			<< BUILD_LOG( lpName )
			<< END_LOG;

		return -1;
	}

	return dispid;
}


_bstr_t KGameForgeAuth::GetEndPointURL()
{
	return m_pSoapClient->ConnectorProperty["EndPointURL"];
}


void KGameForgeAuth::PutEndPointURL(_bstr_t EndPointURL)
{
	m_pSoapClient->ConnectorProperty["EndPointURL"] = EndPointURL;
}

//=========================================================================//
//			Web Method Call				
//=========================================================================//
// Return value : (-1 = function call error, 1 = OK, 2 = Wrong Password, 3 = User not found, 4 = User not validated) 
int KGameForgeAuth::ConfirmLoginOnWebDb(_bstr_t userName, _bstr_t password, int& iAccountID )
{
	HRESULT hr;
	DISPPARAMS params;
	VARIANT varg[2];
	_variant_t resultInfo;
	EXCEPINFO excepinfo;
	unsigned int uArgErr;



	KWebMethod kMethod;
	_JIF( GetWebMethod( GFO_AUTH, kMethod ), return -1 );
	

	static DISPID dispid = -1;
	if(dispid == -1) dispid = GetIDOfName( _bstr_t( kMethod.m_strMethod.c_str()) );
	

	varg[1].vt = VT_BSTR;
	varg[1].bstrVal = userName;

	varg[0].vt = VT_BSTR;
	varg[0].bstrVal = password;

    params.cArgs = 2;
	params.rgvarg = varg;
	params.cNamedArgs = 0;
	params.rgdispidNamedArgs = NULL;


	memset(&excepinfo, 0, sizeof(excepinfo));

	uArgErr = -1;

	hr = m_pSoapClient->Invoke(
		dispid, 
		IID_NULL, 
		LOCALE_SYSTEM_DEFAULT, 
		DISPATCH_METHOD, 
		&params,
		&resultInfo, 
		&excepinfo, 
		&uArgErr);

	
	if( FAILED(hr) )
	{
		START_LOG( cerr, L"Soap Invoke failed" )
			<< BUILD_LOG( kMethod.m_strMethod )
			<< BUILD_LOG( dispid )
			<< BUILD_LOG( userName )
			<< BUILD_LOG( password )
			<< BUILD_LOG( excepinfo.bstrSource )
			<< BUILD_LOG( excepinfo.bstrDescription )
			<< END_LOG;

			return -1;
	}

	
	if( (VT_BSTR|VT_ARRAY) != resultInfo.vt )
	{
		START_LOG( cerr, L"Invoke Return Type mismatched" )
			<< BUILD_LOG( kMethod.m_strMethod )
			<< BUILD_LOG( dispid )
			<< BUILD_LOG( resultInfo.vt )
			<< END_LOG;

		return -1;
	}

	UINT uDim        = 0;    // ����� ���ڿ� �迭�� ���� ����
	long lLBound     = 0l;   // ����� ���ڿ� �迭�� ���� �ε���
	long lUBound     = 0l;   // ����� ���ڿ� �迭�� ���� �ε���
	long nBoundIndex = 0l;   // ����� ���ڿ� �迭 �ε����� ���� �о�ö� ����մϴ�.
	BSTR bstrElement = NULL; // ����� ���ڿ� �迭�� �ϳ��� ���� �о�ö� ����մϴ�.


	uDim = SafeArrayGetDim    (resultInfo.parray);                 // �� �����迭���� Ȯ���մϴ�.
	hr   = SafeArrayGetLBound (resultInfo.parray, uDim, &lLBound); // �迭�� ���� �ε����� ���մϴ�.
	hr   = SafeArrayGetUBound (resultInfo.parray, uDim, &lUBound); // �迭�� ���� �ε����� ���մϴ�.



	// 1���� �迭���� �����մϴ�.
	_JIF( (uDim == 1 && lUBound == 1), return -1 );
	

	//confirmLoginOnWebDb(userName, password)	returns an array including two integers:
	//$arr[0] = Result (1 = OK, 2 = Wrong Password, 3 = User not found, 4 = User not validated) 
	//$arr[1] = Account-ID (includes the ID of the account in our database, only if Result = 1)
	//User not validated�� ��� ��ȣ üũ���� ������

	hr = SafeArrayGetElement(resultInfo.parray, &nBoundIndex, &bstrElement);
	int iLoginResult = _ttoi ( bstrElement );
	SysFreeString (bstrElement);
	bstrElement = NULL;

	hr = SafeArrayGetElement(resultInfo.parray, &nBoundIndex, &bstrElement);
	iAccountID = _ttoi( bstrElement );
	SysFreeString (bstrElement);
	bstrElement = NULL;


	// �迭�� ���� ��ŭ �ݺ�
// 	for (nBoundIndex=lLBound; nBoundIndex<=lUBound; nBoundIndex++)
// 	{
// 		// �迭�� ���� �ε����� �ش��ϴ� ���ڿ��� �о�ɴϴ�.
// 		hr = SafeArrayGetElement(resultInfo.parray, &nBoundIndex, &bstrElement);
// 
// 		START_LOG( cout, bstrElement)
// 			<< END_LOG;
// 		
// 		_ttoi ( bstrElement );
// 
// 
// 		SysFreeString (bstrElement);
// 		bstrElement = NULL;
// 	}


	return iLoginResult;

}
*/

//=========================================================================//
//			Process Event handle			
//=========================================================================//

void KGameForgeAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	
	CASE( EPUBLISHER_AUTHENTICATION_REQ );
	default:
		START_LOG( cerr, "�ڵ鷯�� �������� ���� �̺�Ʈ." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}


//////////////////////////////////////////////////////



void KGameForgeAuth::testfunction()
{


	//struct _ns1__confirmLoginOnWebDB tmp_input; 
	//tmp_input.userName = "jin1234";
	//tmp_input.password = "wls123";

	char tmpInputUserName[MAX_PATH] = "jin1234";
	char tmpInputPassword[MAX_PATH] = "wls123";

	struct ns1__confirmLoginOnWebDBResponse tmp_output;


	KWebMethod tmp_web_path;
	if( GetWebMethod(GFO_AUTH, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod GFO_AUTH. Check Connection." )
			<< BUILD_LOG( tmpInputUserName )	//<< BUILD_LOG( tmp_input.userName )
			<< BUILD_LOG( tmpInputPassword )	//<< BUILD_LOG( tmp_input.password )
			<< END_LOG;

		return;
	}


	//{{ 2010/11/11 ��ȿ��	soap �Լ� ȣ�� ������ Ȥ�� �𸣴� �׻� Ȯ������.
	if( p_gf_soap == NULL )
	{// soap ��� ���� ���� �ȵ� ��� // ����ó��
		return;
	}


// 	int soap_call_result = soap_call_ns1__confirmLoginOnWebDB(p_gf_soap, 
// 		m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
// 		&tmp_input, &tmp_output );

	int soap_call_result = soap_call_ns1__confirmLoginOnWebDB(p_gf_soap, 
		m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
		tmpInputUserName, tmpInputPassword, &tmp_output );

	// Soap ȣ�� ���� //
	if( 0 != soap_call_result )
	{
		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDB ERROR. Check Connection " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< END_LOG;

		return;
	}

	// ���������� ��쿡 ���ϰ� �������� ������ �״°� ���� �ϱ� ���ؼ�
	if( tmp_output.confirmLoginOnWebDBReturn != NULL && 
		tmp_output.confirmLoginOnWebDBReturn->__ptr != NULL )
	{

		// ���� �� ���� //
		if( (tmp_output.confirmLoginOnWebDBReturn->__ptr[0]) == NULL )
		{
			START_LOG( cerr, L"[ErrorLog] Login Return value __ptr[0] ERROR. Check Connection " )
				<< END_LOG;

			return;
		}
		
		// ���� �� ���� //
		if( (tmp_output.confirmLoginOnWebDBReturn->__ptr[1]) == NULL )
		{
			START_LOG( cerr, L"[ErrorLog] Login Return value __ptr[1] ERROR. Check Connection " )
				<< END_LOG;

			return;
		}

		// ���� �� ���� //
		if( (tmp_output.confirmLoginOnWebDBReturn->__ptr[2]) == NULL )
		{
			START_LOG( cerr, L"[ErrorLog] Login Return value __ptr[1] ERROR. Check Connection " )
				<< END_LOG;

			return;
		}

		
	}
	// ���������� ��� 
	else
	{
		START_LOG( cerr, L"[ErrorLog] Test confirmLoginOnWebDBReturn Return NULL!!!. SOAP ERROR!!! " )
			<< END_LOG;
	}


	return;
}


IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ )
{
	//{{ 2010. 11. 25. ��ȿ��	�ƹ��͵� �Էµ��� ���� ��� ���� ó����
	if( kPacket_.m_wstrServiceAccountID == L"" )
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketReq;
		kPacketReq.m_iOK = NetError::ERR_GAMEFORGE_01;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketReq );
		KActorManager::GetKObj()->QueueingTo( kPacket_.m_iActorUID, spEvent );
		return;
	}
	else if ( kPacket_.m_wstrServicePassword == L"" )
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketReq;
		kPacketReq.m_iOK = NetError::ERR_GAMEFORGE_00;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketReq );
		KActorManager::GetKObj()->QueueingTo( kPacket_.m_iActorUID, spEvent );
		return;
	}
	//}}
	
	const int MaxNameSize = 100;
	const int MaxPasswordSize = 100;
	
	//{{ 2010. 11. 24. ��ȿ�� ������ NULL�� ������ �ֱ� ���� ����
	char tmp_input_userName[MaxNameSize+1] = {0,};
	char tmp_input_password[MaxPasswordSize+1] = {0,};
	tmp_input_userName[MaxNameSize] = NULL;
	tmp_input_password[MaxNameSize] = NULL;
	//}}

	int iResult = 0;
	int iWebAccountID = -1;
	std::wstring wstrPurchTok = L"";

	UidType iActorUID = kPacket_.m_iActorUID;

//	struct _ns1__confirmLoginOnWebDB tmp_input; 
	struct ns1__confirmLoginOnWebDBResponse tmp_output;

	// size ���� 
	strncpy(tmp_input_userName,KncUtil::toNarrowString(kPacket_.m_wstrServiceAccountID).c_str(), MaxNameSize ); 
	strncpy(tmp_input_password,KncUtil::toNarrowString(kPacket_.m_wstrServicePassword).c_str(), MaxPasswordSize ); 


//	tmp_input.userName = tmp_input_userName;
//	tmp_input.password = tmp_input_password;

	KWebMethod tmp_web_path;
	if( GetWebMethod(GFO_AUTH, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod GFO_AUTH" )
			<< BUILD_LOG( tmp_input_userName )
			<< BUILD_LOG( tmp_input_password )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketReq;
		kPacketReq.m_iOK = NetError::ERR_VERIFY_21;
					
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketReq );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}


	//{{ 2010/11/11 ��ȿ��	soap �Լ� ȣ�� ������ Ȥ�� �𸣴� �׻� Ȯ������.
	if( p_gf_soap == NULL )
	{// soap ��� ���� ���� �ȵ� ��� // ����ó��
		return;
	}



	//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
	int iTmpCase = 0;
	CTime kStartFuncTime = CTime::GetCurrentTime();
	DWORD dwStartFuncTime = ::GetTickCount();
#endif SERV_SOAP_LOGIN_FUNC_LOG
	//}}


//	int soap_call_result = soap_call___ns1__confirmLoginOnWebDB(p_gf_soap, 
//							m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
//							&tmp_input, &tmp_output );


	int soap_call_result = soap_call_ns1__confirmLoginOnWebDB(p_gf_soap, 
							m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
							tmp_input_userName, tmp_input_password, &tmp_output );


	//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
	DWORD dwTmpDiffTime = ( ::GetTickCount() - dwStartFuncTime );
#endif SERV_SOAP_LOGIN_FUNC_LOG
	//}}

	// Soap ȣ�� ���� //
	if( 0 != soap_call_result )
	{
		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDB ERROR. " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketReq;
		kPacketReq.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketReq );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
		iTmpCase = 1;
		SiKGameForgeAuthManager()->SendSoapLoginFuncLog(KncUtil::toWideString(tmp_input_userName),kStartFuncTime,dwTmpDiffTime,iTmpCase);
#endif SERV_SOAP_LOGIN_FUNC_LOG
		//}}


		return;
	}

	// ���������� ��쿡 ���ϰ� �������� ������ �״°� ���� �ϱ� ���ؼ�
	if( tmp_output.confirmLoginOnWebDBReturn != NULL && 
		tmp_output.confirmLoginOnWebDBReturn->__ptr != NULL )
	{
		// ���� �� 
		if( (tmp_output.confirmLoginOnWebDBReturn->__ptr[0]) != NULL )
			iResult = atoi(tmp_output.confirmLoginOnWebDBReturn->__ptr[0]);
		if( (tmp_output.confirmLoginOnWebDBReturn->__ptr[1]) != NULL )
			iWebAccountID = atoi(tmp_output.confirmLoginOnWebDBReturn->__ptr[1]);
		if( (tmp_output.confirmLoginOnWebDBReturn->__ptr[2]) != NULL )
			wstrPurchTok = KncUtil::toWideString( tmp_output.confirmLoginOnWebDBReturn->__ptr[2] );

		// iResult			: �������� �������� Ȯ��
		// iWebAccountID	: SN ��ȣ
		// wstrPurchTok		: ĳ�������� �������� ���� �߰��Ǵ� ��Ʈ��
		
	}
	// ���������� ��� 
	else
	{
		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDBReturn Return NULL!!!. SOAP ERROR!!! " )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketReq;
		kPacketReq.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketReq );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
		iTmpCase = 2;
		SiKGameForgeAuthManager()->SendSoapLoginFuncLog(KncUtil::toWideString(tmp_input_userName),kStartFuncTime,dwTmpDiffTime,iTmpCase);
#endif SERV_SOAP_LOGIN_FUNC_LOG
		//}}

		return;
	}

	// SOAP���� purchTok�� �߸� �޾Ƽ� �ý����� ���� ��츦 Ȯ�� �ϱ� ���ؼ�
	if( wstrPurchTok.length() >= 50 )
	{
		START_LOG( cerr, L"[ERROR] wstrPurchTok String size error!" )
			<< BUILD_LOG( tmp_input_userName )
			<< BUILD_LOG( tmp_input_password )
			<< BUILD_LOG( wstrPurchTok )
			<< END_LOG;
	}
	//}}


	// Success Case //
	if( iResult == 1 )
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iResult = 1;
		kPacketAck.m_wstrIP = kPacket_.m_wstrIP;
		kPacketAck.m_wstrID = kPacket_.m_wstrServiceAccountID;
		kPacketAck.m_uiPublisherUID = (unsigned int)iWebAccountID;
#ifdef SERV_PURCHASE_TOKEN
		kPacketAck.wstrPurchTok = wstrPurchTok;
#endif SERV_PURCHASE_TOKEN

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );


		//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
		iTmpCase = 3;
		SiKGameForgeAuthManager()->SendSoapLoginFuncLog(KncUtil::toWideString(tmp_input_userName),kStartFuncTime,dwTmpDiffTime,iTmpCase);
#endif SERV_SOAP_LOGIN_FUNC_LOG
		//}}
	}

	// Fail Case //
	else
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;

		switch( iResult )
		{
			
		case 2:	kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_00;	break;	// 2 = Wrong Password
		case 3:	kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_01;	break;	// 3 = User not found
		case 4:	kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_02;	break;	// 4 = User not validated
		case 5:	kPacketAck.m_iOK = NetError::ERR_VERIFY_05;		break;	// 5 = User account marked for deletion
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			case 6:	kPacketAck.m_iOK = NetError::ERR_ACCOUNT_BLOCK_04;		break;	// 6 = User account is blocked
#else //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			case 6:	kPacketAck.m_iOK = NetError::ERR_VERIFY_11;		break;	// 6 = User account is blocked
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		
		default: // ���� �������� �ý��� ������ �߻��Ͽ����ϴ�.
			{
				kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

				START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDB Return ERROR. ���� �������� �ý��� ������ �߻��Ͽ����ϴ�. " )
					<< BUILD_LOG( iResult )
					<< END_LOG;
			}
			break;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );


		//{{ 2011. 08. 17	��ȿ��	�α��� SOAP �Լ� �α� 
#ifdef SERV_SOAP_LOGIN_FUNC_LOG
		iTmpCase = 4;
		SiKGameForgeAuthManager()->SendSoapLoginFuncLog(KncUtil::toWideString(tmp_input_userName),kStartFuncTime,dwTmpDiffTime,iTmpCase);
#endif SERV_SOAP_LOGIN_FUNC_LOG
		//}}
	}


	START_LOG( clog, L"[�׽�Ʈ�α�] GF authentication result" )
		<< BUILD_LOG( kPacket_.m_iActorUID )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( iWebAccountID )
		<< BUILD_LOG( wstrPurchTok )	
		<< END_LOG;

}


#endif // SERV_COUNTRY_EU
