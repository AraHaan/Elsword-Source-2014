#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "ChannelAccountDBThread.h"
#include "ChannelServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "SimLayer.h"

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "MachineBlockManager.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2010/10/23 ��ȿ��  ���� �õ� ���� ������ �ν��� Random���� ��ü�� 
#include <boost/random.hpp>
//}}
#define CLASS_TYPE KChannelAccountDBThread
ImplPfID( KChannelAccountDBThread, PI_CHANNEL_ACCOUNT_DB );

IMPL_PROFILER_DUMP( KChannelAccountDBThread )
{
    for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
    {
        unsigned int iAvg = 0;
        if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		

        DO_QUERY_NO_PROFILE( L"exec dbo.mup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
            % vecDump[ui].m_wstrQuery
            % vecDump[ui].m_iMinTime
            % iAvg
            % vecDump[ui].m_iMaxTime
            % vecDump[ui].m_iOver1Sec
            % vecDump[ui].m_iQueryCount
            % vecDump[ui].m_iQueryFail
            );

        continue;

end_proc:
        START_LOG( cerr, vecDump[ui].m_wstrQuery )
            << BUILD_LOG( vecDump[ui].m_iMinTime )
            << BUILD_LOG( vecDump[ui].m_iMaxTime )
            << BUILD_LOG( vecDump[ui].m_iTotalTime )
            << BUILD_LOG( vecDump[ui].m_iQueryCount )
            << BUILD_LOG( iAvg )
            << BUILD_LOG( vecDump[ui].m_iOver1Sec )
            << BUILD_LOG( vecDump[ui].m_iQueryFail )
            << END_LOG;
    }
}

KChannelAccountDBThread::~KChannelAccountDBThread(void)
{
}

void KChannelAccountDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {       
#ifdef SERV_KOG_OTP_VERIFY
		CASE( DBE_CH_USER_GENERAL_LOGIN_REQ );
		CASE( DBE_CH_USER_KOGOTP_LOGIN_REQ );
#endif // SERV_KOG_OTP_VERIFY

        //CASE( DBE_GASH_USER_LOGIN_REQ );
		//{{ 2011. 01. 13 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL
		CASE( DBE_GET_CONCURRENT_USER_LOGIN_REQ );
#endif SERV_CCU_MONITORING_TOOL
		//}}
		//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		CASE( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
		CASE( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ );
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
		//}}
		//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		CASE( DBE_CHECK_SERVER_SN_REQ );
		CASE( DBE_CREATE_SERVER_SN_REQ );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		//}}

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    }
}

void KChannelAccountDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KChannelAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

//{{ 2011.1.24 ��ȿ��
#ifdef SERV_KOG_OTP_VERIFY
IMPL_ON_FUNC( DBE_CH_USER_GENERAL_LOGIN_REQ )
{
	// �ش� ��û�� �ۺ��� ������ ������ ���������� ������ �� ��û�Ǵ� �̺�Ʈ��

	KDBE_CH_USER_GENERAL_LOGIN_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	
	kPacket.m_wstrOTP = L"";	// ���ο� OTP �ޱ� ������ ���� ó��
	kPacket.m_iChannelingCode = kPacket_.m_iChannelingCode;

#ifdef SERV_COUNTRY_TH
	std::wstring wstrAccountID = kPacket_.m_wstrMasterID;
	if(wstrAccountID == L"")
		wstrAccountID = kPacket_.m_wstrServiceAccountID;
	kPacket.m_wstrUserID = wstrAccountID;
#else // SERV_COUNTRY_TH
	std::wstring wstrAccountID = kPacket_.m_wstrServiceAccountID;
	kPacket.m_wstrUserID = wstrAccountID;
#endif // SERV_COUNTRY_TH

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
	DO_QUERY( L"exec dbo.P_MUser_CHK_TW", L"N\'%s\', N\'%s\'", % wstrAccountID.c_str() % kPacket_.m_wstrPassword.c_str() );
#else //SERV_USE_KOG_ACCOUNTDB_PASSWORD

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_SEL_Auth_Global", L"N\'%s\', %d", % wstrAccountID.c_str() % kPacket_.m_iChannelingCode );
#else //SERV_ALL_RENEWAL_SP

#ifdef SERV_STEAM
	DO_QUERY( L"exec dbo.mup_auth_global_user", L"N\'%s\', %d", % wstrAccountID.c_str() % kPacket_.m_iChannelingCode );
#else //SERV_STEAM
	DO_QUERY( L"exec dbo.mup_auth_global_user", L"N\'%s\'", % wstrAccountID.c_str() );
#endif //SERV_STEAM
#endif //SERV_ALL_RENEWAL_SP

#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_iUserUID
				);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		// ���� DB ���� ����
		START_LOG( clog, L"�������� �ʰų� ����/��� �ٸ� ����." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1:	kPacket.m_iOK = NetError::ERR_VERIFY_04;	break;
		// ���Ŀ� ���� ������ �� ��� ������ ������ �������� ���� ó���� �ʿ��ϴ�.
		case -2:    kPacket.m_iOK = NetError::ERR_VERIFY_18;	break;
		case -3:	kPacket.m_iOK = NetError::ERR_VERIFY_19;	break;
		// ���� �κ� �߰��� 
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		case -5:	kPacket.m_iOK = NetError::ERR_ACCOUNT_BLOCK_01;	break;
#else //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		case -5:	kPacket.m_iOK = NetError::ERR_VERIFY_11;	break;
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		case -7:	kPacket.m_iOK = NetError::ERR_VERIFY_06;	break;  // �׽�Ʈ ������
		default:
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			break;
		}

		// �ű� ���� ����. - ���� �� ���������� mup_update_user_otp���� ȣ���
		if( kPacket.m_iOK == NetError::ERR_VERIFY_04 )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUser_INS_Global", L"N\'%s\', N\'%s\', %d, %d, %d, N\'%s\', %d, %d",
				% wstrAccountID
				% kPacket_.m_wstrIP	// �����ÿ� OTPĭ �������� ���� //kPacket_.m_wstrOTP
				% kPacket_.m_uiPublisherUID
				% 0
				% 0
				% kPacket_.m_wstrServiceAccountID
				% 0
				% kPacket_.m_iChannelingCode
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_create_global_user", L"N\'%s\', N\'%s\', %d, %d, %d, N\'%s\', %d, %d",
				% wstrAccountID
				% kPacket_.m_wstrIP	// �����ÿ� OTPĭ �������� ���� //kPacket_.m_wstrOTP
				% kPacket_.m_uiPublisherUID
				% 0
				% 0
				% kPacket_.m_wstrServiceAccountID
				% 0
				% kPacket_.m_iChannelingCode
				);
#endif //SERV_ALL_RENEWAL_SP


			bool bPlayGuide;
			bool bInternalUser;
			bool bGuestUser;
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK
					>> kPacket.m_iUserUID
					>> bPlayGuide
					>> bInternalUser
					>> bGuestUser
					);
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				// �ű� ���� ���� ����.
				START_LOG( clog, L"�ű� ���� ���� ����." )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;
			}
		}
	}

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		static boost::mt19937 generator(timeGetTime());	// timeGetTime() ���� ���� �õ�
		static boost::uniform_int<> uni_dist(0,INT_MAX) ;
		static boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(generator, uni_dist);

		int iOTP  = gen();	// 21�� ���������� iOTP �� ����

		wchar_t wszOTP[128];
#ifdef _CONVERT_VS_2010
        _itow_s( iOTP, wszOTP, 10 );
#else
		::_itow( iOTP, wszOTP, 10 );
#endif _CONVERT_VS_2010
		kPacket.m_wstrOTP = wszOTP;

#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserOTP_UPD", L"%d, N\'%s\'",
			% kPacket.m_iUserUID
			% kPacket.m_wstrOTP );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_update_user_otp", L"%d, N\'%s\'",
			% kPacket.m_iUserUID
			% kPacket.m_wstrOTP );
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				);
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			// OTP ������Ʈ ����
			START_LOG( clog, L"OTP ������Ʈ ����." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;
		}

		//{{ 2010.12.02 ��ȿ��  ���������� �� ������ ��쿡�� �־��ش�.
#ifdef SERV_PURCHASE_TOKEN
		kPacket.m_wstrPurchaseTok	= kPacket_.m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
		//}}

#ifdef SERV_COUNTRY_PH
		DO_QUERY( L"exec dbo.P_MUserOption_SET", L"%d, %d",
			% kPacket.m_iUserUID
			% kPacket_.m_usGarenaCyberCafe );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				);
			m_kODBC.EndFetch();
		}
#endif //SERV_COUNTRY_PH
	}

#ifdef SERV_ANTI_ADDICTION_SYSTEM
	// ���� Ÿ�� ������Ʈ
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		START_LOG( clog, L"���� Ÿ�� ������Ʈ." )
			<< BUILD_LOG( kPacket.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_uiAccountType )
			<< END_LOG;

		DO_QUERY( L"exec dbo.mup_update_giant_account_type", L"%d, %d",
			% kPacket.m_iUserUID
			% kPacket_.m_uiAccountType );

		kPacket.m_iOK = NetError::ERR_UNKNOWN;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK	);
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			// ���� Ÿ�� ������Ʈ ����
			START_LOG( clog, L"���� Ÿ�� ������Ʈ ����." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_uiAccountType )
				<< END_LOG;
		}
	}
#endif SERV_ANTI_ADDICTION_SYSTEM

end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket.m_wstrOTP )
		<< BUILD_LOG( kPacket.m_iOK )
		<< BUILD_LOG( kPacket.m_iUserUID )
		<< BUILD_LOG( kPacket.m_wstrPurchaseTok )
		<< END_LOG;

	SendToUser( FIRST_SENDER_UID, DBE_CH_USER_GENERAL_LOGIN_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CH_USER_KOGOTP_LOGIN_REQ )
{
	KDBE_CH_USER_KOGOTP_LOGIN_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_wstrUserID = kPacket_.m_wstrServiceAccountID;
	kPacket.m_wstrOTP = kPacket_.m_wstrOTP;

	//{{ 2011. 1. 24  ��ȿ��	*�߿�* ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	// �ʱ� �α��� ��û���� bLogin ���� ������ false�� �ٲ� ��Ŷ�� �������� ���� �� ����.
	// �� ��� OTP ��Ʈ���� �̿��� ���� ������ ������ �õ� �� �� ����
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrOTP ) )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		goto end_proc;
	}
	//}}

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_SEL_Auth_Global", L"N\'%s\', %d", % kPacket_.m_wstrServiceAccountID.c_str() % kPacket_.m_iChannelingCode );
#else //SERV_ALL_RENEWAL_SP
#ifdef SERV_STEAM
	DO_QUERY( L"exec dbo.mup_auth_global_user", L"N\'%s\', %d", % kPacket_.m_wstrServiceAccountID.c_str() % kPacket_.m_iChannelingCode );
#else //SERV_STEAM
	DO_QUERY( L"exec dbo.mup_auth_global_user", L"N\'%s\'",
		% kPacket_.m_wstrServiceAccountID.c_str() 
		);
#endif //SERV_STEAM
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iUserUID
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		// ���� DB ���� ����
		START_LOG( clog, L"�������� �ʰų� ����/��� �ٸ� ����." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1:	kPacket.m_iOK = NetError::ERR_VERIFY_04;	break;
		// ���Ŀ� ���� ������ �� ��� ������ ������ �������� ���� ó���� �ʿ��ϴ�.
		case -2:    kPacket.m_iOK = NetError::ERR_VERIFY_18;	break;
		case -3:	kPacket.m_iOK = NetError::ERR_VERIFY_19;	break;
		// ���� �κ� �߰��� 
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			case -5:	kPacket.m_iOK = NetError::ERR_ACCOUNT_BLOCK_02;	break;
#else //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			case -5:	kPacket.m_iOK = NetError::ERR_VERIFY_11;	break;
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		default:
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			break;
		}

		// KOGOTP�δ� �ű� ���� ó�� ���а� �������� ������δ� ���� ���� ����
		// ���� �ű� ���� ó�� ���п� ���� ����� �ִ� �κ� ����

		goto end_proc;
	}


	// OTP ���� ���� �Ѵ�. 
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserOTP_SEL_Check", L"%d, N\'%s\'",
		% kPacket.m_iUserUID
		% kPacket.m_wstrOTP );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_verify_user_otp", L"%d, N\'%s\'",
		% kPacket.m_iUserUID
		% kPacket.m_wstrOTP );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			);
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		// OTP ���� ����
		START_LOG( clog, L"OTP ���� ����." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket.m_iUserUID )
			<< BUILD_LOG( kPacket.m_wstrOTP )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		goto end_proc;
	}


	if( kPacket.m_iOK == NetError::NET_OK )
	{
		static boost::mt19937 generator(timeGetTime());	// timeGetTime() ���� ���� �õ�
		static boost::uniform_int<> uni_dist(0,INT_MAX) ;
		static boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(generator, uni_dist);

		int iOTP  = gen();	// 21�� ���������� iOTP �� ����
		

		wchar_t wszOTP[128];
		::_itow( iOTP, wszOTP, 10 );
		kPacket.m_wstrOTP = wszOTP;

#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserOTP_UPD", L"%d, N\'%s\'",
			% kPacket.m_iUserUID
			% kPacket.m_wstrOTP );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_update_user_otp", L"%d, N\'%s\'",
			% kPacket.m_iUserUID
			% kPacket.m_wstrOTP );
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				);
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			// OTP ������Ʈ ����
			START_LOG( clog, L"OTP ������Ʈ ����." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;
		}

		//{{ 2010.12.02 ��ȿ��  ���������� �� ������ ��쿡�� �־��ش�.
#ifdef SERV_PURCHASE_TOKEN
		kPacket.m_wstrPurchaseTok	= kPacket_.m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
		//}}
	}

end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_wstrOTP )
		<< BUILD_LOG( kPacket.m_iOK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
		<< BUILD_LOG( kPacket.m_iUserUID )
		<< BUILD_LOG( kPacket.m_wstrPurchaseTok )
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, DBE_CH_USER_KOGOTP_LOGIN_ACK, kPacket );
}
#endif // SERV_KOG_OTP_VERIFY
//}} 2011.1.24 ��ȿ�� KOG_OTP_MODIFY

/*
IMPL_ON_FUNC( DBE_GASH_USER_LOGIN_REQ )
{
    KDBE_GASH_USER_LOGIN_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_wstrOTP = kPacket_.m_wstrOTP;

    DO_QUERY( L"exec dbo.mup_auth_gash_user", L"N\'%s\', N\'%s\'", % kPacket_.m_wstrServiceAccountID.c_str() % kPacket_.m_wstrOTP.c_str() );

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK
            >> kPacket.m_iUserUID
            );

        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        // ���� DB ���� ����
        START_LOG( clog, L"�������� �ʰų� ����/��� �ٸ� ����." )
            << BUILD_LOG( kPacket.m_iOK )
            << END_LOG;

        switch( kPacket.m_iOK )
        {
        case -1:
            kPacket.m_iOK = NetError::ERR_VERIFY_04;
            break;
        case -2:            // ���Ŀ� ���� ������ �� ��� ������ ������ �������� ���� ó���� �ʿ��ϴ�.
            kPacket.m_iOK = NetError::ERR_VERIFY_18;
            break;
        case -3:
            kPacket.m_iOK = NetError::ERR_VERIFY_19;
            break;
        default:
            kPacket.m_iOK = NetError::ERR_UNKNOWN;
            break;
        }

        // �ű� ���� ����.
        if( kPacket.m_iOK == NetError::ERR_VERIFY_04 )
        {
            // �� �����忡�� sim layer ������ �����ϴ� ���� �״� ���� ���� �ƴϴ�.
            DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
            if( dwAuthFlag != KSimLayer::AF_GAMANIA_TAIWAN )
            {
                START_LOG( cerr, L"���� �÷��� �̻�." )
                    << BUILD_LOG( dwAuthFlag )
                    << END_LOG;

                goto end_proc;
            }

            DO_QUERY( L"exec dbo.mup_create_gash_user", L"N\'%s\', \'%s\', %d, %d, %d, N\'%s\', %d",
                % kPacket_.m_wstrServiceAccountID
                % kPacket_.m_wstrOTP
                % 0
                % 0
                % 0
                % kPacket_.m_wstrServiceAccountID
                % 0
                );

            bool bPlayGuide;
            bool bInternalUser;
            bool bGuestUser;
            if( m_kODBC.BeginFetch() )
            {
                FETCH_DATA( kPacket.m_iOK
                    >> kPacket.m_iUserUID
                    >> bPlayGuide
                    >> bInternalUser
                    >> bGuestUser
                    );
                m_kODBC.EndFetch();
            }

            if( kPacket.m_iOK != NetError::NET_OK )
            {
                // �ű� ���� ���� ����.
                START_LOG( clog, L"�ű� ���� ���� ����." )
                    << BUILD_LOG( kPacket.m_iOK )
                    << END_LOG;
            }
        }
    }

    if( kPacket.m_iOK == NetError::NET_OK )
    {
        int iOTP = rand();
        wchar_t wszOTP[128];
        ::_itow( iOTP, wszOTP, 10 );
        kPacket.m_wstrOTP = wszOTP;

        DO_QUERY( L"exec dbo.mup_update_user_otp", L"%d, N\'%s\'",
            % kPacket.m_iUserUID
            % kPacket.m_wstrOTP );

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( kPacket.m_iOK
                );
            m_kODBC.EndFetch();
        }

        if( kPacket.m_iOK != NetError::NET_OK )
        {
            // OTP ������Ʈ ����
            START_LOG( clog, L"OTP ������Ʈ ����." )
                << BUILD_LOG( kPacket.m_iOK )
                << END_LOG;
        }
    }

end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
#ifndef SERV_PRIVACY_AGREEMENT
        << BUILD_LOG( kPacket_.m_wstrServiceAccountID )
#endif SERV_PRIVACY_AGREEMENT
        << BUILD_LOG( kPacket_.m_wstrOTP )
        << BUILD_LOG( kPacket.m_iOK )
        << BUILD_LOG( kPacket.m_iUserUID )
        << END_LOG;

    SendToUser( FIRST_SENDER_UID, DBE_GASH_USER_LOGIN_ACK, kPacket );

}
//*/

//{{ 2011. 01. 13 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL
IMPL_ON_FUNC( DBE_GET_CONCURRENT_USER_LOGIN_REQ )
{
	KDBE_GET_CONCURRENT_USER_LOGIN_ACK kPacket;
	
	kPacket.m_iOK = NetError::NET_OK;

	DO_QUERY( L"exec dbo.mup_login_mornitor_user", L"N\'%s\', N\'%s\'", % kPacket_.m_wstrID.c_str() % kPacket_.m_wstrPW.c_str() );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iAuthLevel );

		m_kODBC.EndFetch();
	}
end_proc:
    SendToUser( FIRST_SENDER_UID, DBE_GET_CONCURRENT_USER_LOGIN_ACK, kPacket );

}
#endif SERV_CCU_MONITORING_TOOL
//}}	

//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
IMPL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ )
{
	KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK kAck; 
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_iRandomKey = 0;
	
	// ID Ư�� ���ڰ� ���� �Ǿ� �ִٸ� 
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUserID ) )
	{
		START_LOG( cout, L"Ư�� ���� ����� ID, PW �Է����� ���� ����!")
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kAck.m_iRandomKey )
			<< END_LOG;

		goto end_proc;
	}

	kAck.m_iRandomKey = rand();
	kAck.m_iRandomKey = kAck.m_iRandomKey + ( rand() << 15) + 1;  // 0 �ʰ��ϴ� �� ����

	DO_QUERY( L"exec dbo.mup_insert_user_authkey", L"N\'%s\', %d", % kPacket_.m_wstrUserID % kAck.m_iRandomKey );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"User ���� RandomKey ��� ����.!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kAck.m_iRandomKey )
			<< END_LOG;
	}

	SendToUser( FIRST_SENDER_UID, DBE_UPDATE_CHANNEL_RANDOMKEY_ACK, kAck );
}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
IMPL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ )
{
	KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK kPacket;

	// Release Tick ���
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_ReleaseTick_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.mup_get_release_tick" );
#endif //SERV_ALL_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		int iType = 0;
		int iReleaseTick = 0;

		FETCH_DATA( iType
			>> iReleaseTick );

		// Machine ID �� �ʿ��ϴ�
		if( iType != KMachineBlockManager::BT_MACHINE_ID_BLOCK )
			continue;

		// �ش� Ÿ�� �ִ°�?
		std::map< int, int >::const_iterator mit = kPacket_.m_mapReleaseTick.find( iType );
		if( mit == kPacket_.m_mapReleaseTick.end() )
			continue;

		// �ش� Ÿ���� Tick������ ������?
		if( iReleaseTick <= mit->second )
			continue;

		kPacket.m_mapReleaseTick.insert( std::make_pair( iType, iReleaseTick ) );
	}

	if( kPacket.m_mapReleaseTick.empty() )
		return;

	{
		std::map< int, int >::const_iterator mit;
		for( mit = kPacket.m_mapReleaseTick.begin(); mit != kPacket.m_mapReleaseTick.end(); ++mit )
		{
			switch( mit->first )
			{
			case KMachineBlockManager::BT_MACHINE_ID_BLOCK:
				{
					// �ŷ� �� ����Ʈ ���
#ifdef SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.P_Temp_Add_SEL" );
#else //SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.mup_get_add" );
#endif //SERV_ALL_RENEWAL_SP

					while( m_kODBC.Fetch() )
					{
						std::string strMachineID;

						FETCH_DATA( strMachineID );

						kPacket.m_vecMachineIDBlockList.push_back( strMachineID );
					}
				}
				break;
			default:
				break;
			}
		}
	}

end_proc:
	SendToServer( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK, kPacket );
}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
IMPL_ON_FUNC( DBE_CHECK_SERVER_SN_REQ )
{
	KDBE_CHECK_SERVER_SN_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_00;
	std::string	strMachineID;
	__int64	iServerSN = 0;
	int	iResult = -1;
	
	DO_QUERY( L"exec dbo.mup_get_serversn_check", L"%d", % kPacket_.m_iServerSN );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kAck.m_iOK == 1 ) // ���� ���� SN �����Ѵ�
	{
		kAck.m_iOK = NetError::ERR_SERVERSN_07;
		goto end_proc;
	}

	DO_QUERY( L"exec dbo.mup_get_mid_sn", L"%d, N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( strMachineID
				 >> iServerSN
				 >> iResult
			);
		m_kODBC.EndFetch();
	}

	bool bCompareSN = false;
	bool bCompareMid = false;
	bool bExistSN	= false;
	bool bExistMid	= false;

	if( iServerSN > 0 ) // DB�� �ӽ�ID�� �����Ѵ�.
	{
		bExistMid = true;
		// MachineID�� ��Ī�� �Ǵ� SN�� Ŭ���̾�Ʈ���� ���� SN�� ������
		if( iServerSN == kPacket_.m_iServerSN )
		{
			bCompareSN = true;
		}
	}

	if( strMachineID.empty() == false ) // DB�� sn�� �����Ѵ�.
	{
		bExistSN = true;
		if( strMachineID.compare( kPacket_.m_strMachineID ) == 0 )
		{
			bCompareMid = true;
		}
	}

	kAck.m_iOK = NetError::NET_OK;

	//////////////////////////////////////////////////////////////////////////
	if( bExistSN == true )
	{
		if( bExistMid == true )
		{
			if( bCompareSN == true )
			{
				if( bCompareMid == true )
				{
					// ����
				}
				else
				{
					kAck.m_iOK = NetError::ERR_SERVERSN_04;

					// �� ���� �ϴµ� mid �� ���� �ʴ�?
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_1" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
			else
			{
				if( bCompareMid == true )
				{
					kAck.m_iOK = NetError::ERR_SERVERSN_04;

					// �� ���� �ϴµ� sn �� ���� �ʴ�?
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_2" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// a/2, b/1 
					// �����ϱ�� ������ ��� ���� �ʴ� �������̴�.  ��κ��� �̰��̴�.
					// ��߱�, ����, ����
					kAck.m_iOK = NetError::ERR_SERVERSN_01;
					kAck.m_iServerSN = iServerSN;

					START_LOG( clog, L"Server SN - ��߱�, ����, ����" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
		else
		{
			if( bCompareSN == true )
			{
				kAck.m_iOK = NetError::ERR_SERVERSN_04;

				// Ŭ�� �� mid �� ã�� sn�� ���µ� bCompareSN �� true �� �� �ֳ�?
				START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_3" )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else
			{
				if( bCompareMid == true )
				{
					kAck.m_iOK = NetError::ERR_SERVERSN_04;

					// Ŭ�� �� mid �� ã�� sn�� ���µ� bCompareMid �� true �� �� �ֳ�?
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_4" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// c/1 
					// Ŭ�� �� mid �� ã�� sn�� ���� Ŭ�� �� sn���� ã�� mid�� �ִ�.
					// ������Ʈ, ����, ����

					int iOK = NetError::ERR_ODBC_00;
					kAck.m_iOK = NetError::ERR_SERVERSN_03;

					DO_QUERY( L"exec dbo.P_Temp_MID_SN_UPD_MID", L"%d, N\'%s\', N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() % kPacket_.m_wstrID.c_str() );
					if( m_kODBC.BeginFetch() )
					{
						FETCH_DATA( iOK );
						m_kODBC.EndFetch();
					}

					START_LOG( clog, L"MachineID ���� update" )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;

					if( iOK != NetError::NET_OK )
					{
						kAck.m_iOK = NetError::ERR_ODBC_00;

						START_LOG( cerr, L"MachineID ���� update ����?!" )
							<< BUILD_LOG( kAck.m_iOK )
							<< BUILD_LOG( kPacket_.m_wstrID )
							<< BUILD_LOG( kPacket_.m_iServerSN )
							<< BUILD_LOG( kPacket_.m_strMachineID )
							<< BUILD_LOG( iServerSN )
							<< BUILD_LOG( strMachineID )
							<< BUILD_LOG( bExistSN )
							<< BUILD_LOG( bExistMid )
							<< BUILD_LOG( bCompareMid )
							<< BUILD_LOG( bCompareSN )
							<< END_LOG;
					}
				}
			}
		}
	}
	else
	{
		if( bExistMid == true )
		{
			if( bCompareSN == true )
			{
				kAck.m_iOK = NetError::ERR_SERVERSN_04;

				// Ŭ�� �� sn �� �������� �ʴµ� bCompareMid �� bCompareSN �� True �� �� �� ����.
				START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_4" )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else
			{
				if( bCompareMid == true )
				{
					kAck.m_iOK = NetError::ERR_SERVERSN_04;

					// Ŭ�� �� sn �� �������� �ʴµ� bCompareMid �� bCompareSN �� True �� �� �� ����.
					START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_5" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// a/3
					// Ŭ�� �� sn �� ���������� mid�� ����. 
					// ��߱�, ����, ����
					kAck.m_iOK = NetError::ERR_SERVERSN_01;
					kAck.m_iServerSN = iServerSN;

					START_LOG( clog, L"Server SN - ��߱�, ����, ����" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
		else
		{
			if( bCompareSN == true || bCompareMid == true )
			{
				kAck.m_iOK = NetError::ERR_SERVERSN_04;

				// Ŭ�� �� sn �� mid �� �������� �ʴµ� bCompareMid �� bCompareSN �� True �� �� �� ����.
				START_LOG( cerr, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?_6" )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else //( bCompareSN == false && bCompareMid == false )
			{
				// c/3
				// Ŭ�� �� sn, mid �� ����. ���� ����� ����
				// �߱�, ����, ����
				kAck.m_iOK = NetError::ERR_SERVERSN_02;

				START_LOG( clog, L"Server SN - �߱�, ����, ����" )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

/*
	// ������ ��Ȳ���� ó��
	if( bExistMid == true )
	{
		if( bCompareMid == false || bCompareSN == false )
		{
			kAck.m_iOK = NetError::ERR_SERVERSN_01;
			kAck.m_iServerSN = iServerSN;
		}
		else
		{
			START_LOG( clog, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?0" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_wstrID )
				<< BUILD_LOG( kPacket_.m_iServerSN )
				<< BUILD_LOG( kPacket_.m_strMachineID )
				<< BUILD_LOG( iServerSN )
				<< BUILD_LOG( strMachineID )
				<< BUILD_LOG( bExistSN )
				<< BUILD_LOG( bExistMid )
				<< BUILD_LOG( bCompareMid )
				<< BUILD_LOG( bCompareSN )
				<< END_LOG;
		}
	}
	else
	{
		if( bExistSN == true )
		{
			if( bCompareMid == false && bCompareSN == false )
			{
				kAck.m_iOK = NetError::ERR_SERVERSN_03;
				// SN�� ������Ű�� MID�� ������Ʈ ����.

				int iOK = NetError::ERR_ODBC_00;
				DO_QUERY( L"exec dbo.P_Temp_MID_SN_UPD_MID", L"%d, N\'%s\', N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() % kPacket_.m_wstrID.c_str() );
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				START_LOG( clog, L"MachineID ���� update" )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;

				if( iOK != NetError::NET_OK )
				{
					kAck.m_iOK = NetError::ERR_SERVERSN_04;

					START_LOG( cerr, L"MachineID ���� update ����?!" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kPacket_.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
			else
			{
				START_LOG( clog, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?1" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
		}
		else
		{
			if( bCompareMid == false && bCompareSN == false )
			{
				kAck.m_iOK = NetError::ERR_SERVERSN_02;
			}
			else
			{
				START_LOG( clog, L"Server SN - ����ó���Ǳ� �ϴµ�....�̷� ��찡 �ֳ�?2" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kPacket_.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
		}
	}
*/
	START_LOG( clog, L"ServerSN ���� Ȯ��" )
		<< BUILD_LOG( kPacket_.m_wstrID )
		<< BUILD_LOG( kPacket_.m_iServerSN )
		<< BUILD_LOG( kPacket_.m_strMachineID )
		<< BUILD_LOG( iServerSN )
		<< BUILD_LOG( strMachineID )
		<< BUILD_LOG( bExistSN )
		<< BUILD_LOG( bExistMid )		
		<< BUILD_LOG( bCompareMid )
		<< BUILD_LOG( bCompareSN )
		<< END_LOG;
end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHECK_SERVER_SN_ACK, kAck );
}

IMPL_ON_FUNC( DBE_CREATE_SERVER_SN_REQ )
{
	KDBE_CREATE_SERVER_SN_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_00;
	kAck.m_iServerSN = 0;
	CTime nowTime = CTime::GetCurrentTime();
	std::wstring wstrNowTime = nowTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	std::string	strMachineID;
	__int64	iServerSN = 0;
	int	iResult = -1;

	DO_QUERY( L"exec dbo.mup_get_mid_sn", L"%d, N\'%s\'", % iServerSN % kPacket_.m_strMachineID.c_str() );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( strMachineID
			>> iServerSN
			>> iResult
			);
		m_kODBC.EndFetch();
	}

	if( iResult == 1 )
	{
		// ����� SN �̴�
		kAck.m_iOK = NetError::ERR_VERIFY_17;

		goto end_proc;
	}

	if( iServerSN > 0 )  // sn �� �����ϴ� MachineID �̴�
	{
		// SN ������� �Ǵ�
		// Client�� ���� ������� �Ǵ� 
		kAck.m_iOK = NetError::ERR_SERVERSN_05;
		kAck.m_iServerSN = iServerSN;

		goto end_proc;
	}
	else if( iServerSN < 0 )
	{
		// ServerSN 
	}

	DO_QUERY( L"exec dbo.mup_insert_mid_sn", L"N\'%s\', N\'%s\'", % kPacket_.m_strMachineID.c_str() % wstrNowTime );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iServerSN
				 >> kAck.m_iOK);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CREATE_SERVER_SN_ACK, kAck );
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}



