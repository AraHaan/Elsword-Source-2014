#pragma once

#include "CommonPacket.h"
#include "ClientPacket.h"


#pragma pack( push, 1 )


#define MAX_PACKET_SIZE_OF_GASH_AUTH 1024

#ifdef SERV_LOGIN_RESULT_INFO
DECL_DATA( KLoginResultInfo )
{
	enum LOGIN_RESULT_TYPE_ENUM		// �α��� ��� ����
	{
		// �߱� �� ���� ���� ���׷��̵� ��
		LRTE_SUCCESS = 0,						// �α��� ����
		LRTE_FAIL_UNKNOWN,						// �� �� ���� Ÿ���� �α��� ����
		LRTE_FAIL_REJECT,						// Ư�� �ź� (�������� �α����϶� ��)
		LRTE_FAIL_SYSTEM_ERROR,					// �ý��� ����
		LRTE_FAIL_ACCOUNT_NOT_EXIST,			// ���� ����
		LRTE_FAIL_PASSWORD_ERROR,				// ���� �н����� ����
		LRTE_FAIL_OTP_PASSWORD_ERROR,			// ���̾�Ʈ OTP ��ȣ ����
		LRTE_FAIL_ACCOUNT_BLOCK,				// ���� ����
		LRTE_FAIL_MATRIX_NUMBER_ERROR,			// ���� ī�� ��ȣ ����
		LRTE_FAIL_ACCOUNT_NOT_ACTIVATE,			// ���� �� Ȱ��ȭ
		
		LRTE_SENTINEL,							// ��. ���� �߰��� ���� �ϼ���.
	};

	int											m_iServerGroupID;		// ������ ID
	UidType										m_uiServerUID;			// ���� UID
	std::vector<int>							m_vecResultCount;		// 0 : ����, 1 : ����(�� �� ����), 2 : ����(���̵� �Ǵ� �н����� ���Է�), 3 : ����(�ۺ��� ���� ���� ���� ����)

	KLoginResultInfo()
	{
		m_iServerGroupID = 0;
		m_uiServerUID = 0;
		m_vecResultCount.clear();
	}
};

typedef KLoginResultInfo						KDBE_INSERT_LOGIN_RESULT_INFO_REQ;

#endif //SERV_LOGIN_RESULT_INFO

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
DECL_PACKET( ELG_PCBANG_LOGOUT_NOT )
{
	UidType									m_iUserUID;
	std::wstring							m_wstrUserID;
	std::wstring							m_wstrIP;
#ifdef SERV_COUNTRY_TH
	std::wstring							m_wstrSockID;
	UidType									m_iGSUID;
#endif //SERV_COUNTRY_TH

	KELG_PCBANG_LOGOUT_NOT()
	{
		m_iUserUID						= 0;
		m_wstrUserID					= L"";
		m_wstrIP						= L"";
#ifdef SERV_COUNTRY_TH
		m_wstrSockID					= L"";
		m_iGSUID						= 0;
#endif //SERV_COUNTRY_TH
	};
};

#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
DECL_PACKET( ELG_ADMIN_DELETE_GUILD_ADD_NOT )
{
	int						m_iGuildUID;

};
#endif

//{{ 2011.03.04  �ӱԼ� ��ϸ� ��ŷ ���� ġƮ ( ���,������ ���� )
#ifdef SERV_DELETE_HENIR_RANKING
DECL_PACKET( ELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT )
{
	int						m_iRankingType;
	int						m_iRank;

};

DECL_PACKET( ELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT )
{
	int						m_iRankingType;
	std::wstring			m_wstrNickName;

};

DECL_PACKET( ELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT )
{
	int						m_iRankingType;
	UidType 				m_iUnitUID;

};
#endif SERV_DELETE_HENIR_RANKING
//}}

#ifdef SERV_NEW_PUNISHMENT

DECL_DATA( KPunishmentInfo )
{
	int									m_iType;
	int									m_iStartTime;
	int									m_iDuration;
};

DECL_PACKET( DBE_GET_PUNISHMENT_ACK )
{
	int									m_iOK;
	std::vector<KPunishmentInfo>		m_vecPunishment;
};

#endif SERV_NEW_PUNISHMENT

//====================================================================//
//====================================================================//
//====================================================================//
//{{ 2013.03.19 ��ȿ�� �ؿ� ���� ���� �ý��� ����
#ifdef SERV_GLOBAL_AUTH



DECL_PACKET( EPUBLISHER_AUTHENTICATION_REQ )
{
	UidType						m_iActorUID;
	std::wstring				m_wstrIP;
	std::wstring				m_wstrServiceAccountID;
	std::wstring				m_wstrServicePassword;
	int							m_iChannelingCode;

#ifdef SERV_KOG_OTP_VERIFY
	bool						m_bServerUseKogOTP;		// KogOTP ��뿩��. �������� �����ϴ� ��
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_LOGIN_MAC_ADDRESS
	std::wstring				m_wstrMACAddress;
#endif // SERV_LOGIN_MAC_ADDRESS
#ifdef SERV_PUBLISHER_OTP
	std::wstring				m_wstrPublisherOTP;
#endif // SERV_PUBLISHER_OTP

#ifdef SERV_USE_DOMAIN_ACCOUNT
	std::wstring				m_wstrDomain;	// �±��� ��� ������ Domain�� ���� �ٸ� ������
#endif // SERV_USE_DOMAIN_ACCOUNT

#ifdef SERV_COUNTRY_CN
	unsigned int				m_uiRequestID;	// �߱� �����ϰ�� �� ��
	unsigned short				m_usZone;		// �߱� �����ϰ�� �� ��
	unsigned short				m_usGame;		// �߱� �����ϰ�� �� ��
#endif // SERV_COUNTRY_CN

#ifdef SERV_COUNTRY_TH
	std::wstring				m_wstrSocketID;
#endif // SERV_COUNTRY_TH

	KEPUBLISHER_AUTHENTICATION_REQ()
	{
		m_iActorUID				= -1;
		m_wstrIP				= L"";
		m_wstrServiceAccountID	= L"";
		m_wstrServicePassword	= L"";
		m_iChannelingCode		= 0;
#ifdef SERV_KOG_OTP_VERIFY
		m_bServerUseKogOTP = false;
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_LOGIN_MAC_ADDRESS
		m_wstrMACAddress		= L"";
#endif // SERV_LOGIN_MAC_ADDRESS
#ifdef SERV_PUBLISHER_OTP
		m_wstrPublisherOTP		= L"";
#endif // SERV_PUBLISHER_OTP

#ifdef SERV_USE_DOMAIN_ACCOUNT
		m_wstrDomain			= L"";
#endif // SERV_USE_DOMAIN_ACCOUNT

#ifdef SERV_COUNTRY_CN
		m_uiRequestID			= 0;
		m_usZone				= 0;
		m_usGame				= 0;
#endif // SERV_COUNTRY_CN

#ifdef SERV_COUNTRY_TH
		m_wstrSocketID			= L"";
#endif // SERV_COUNTRY_TH
	}
};


DECL_PACKET( EPUBLISHER_AUTHENTICATION_ACK )
{
	int							m_iOK;
	int							m_iResult;

	std::wstring				m_wstrID;
#ifdef SERV_COUNTRY_TH
	UidType						m_uiPublisherUID;	// �±��� UidType ���
#else // SERV_COUNTRY_TH
	unsigned int				m_uiPublisherUID;
#endif // SERV_COUNTRY_TH
	std::wstring				m_wstrIP;
	int							m_iChannelingCode;
#ifdef SERV_PURCHASE_TOKEN
	std::wstring				wstrPurchTok;
#endif SERV_PURCHASE_TOKEN

	unsigned int				m_uiAccountType;
	unsigned int				m_uiAccountState;

#ifdef SERV_LOGIN_TOU
	std::string					m_strAgreementURL;
#endif //SERV_LOGIN_TOU

#ifdef SERV_COUNTRY_TH
	std::wstring				m_wstrMasterID;
	std::wstring				m_wstrSocketID;
	bool						m_bIsAKeyUser;
	std::wstring				m_wstrAkeyUserID;
#endif // SERV_COUNTRY_TH

	KEPUBLISHER_AUTHENTICATION_ACK()
	{
		m_iOK					= -1;
		m_iResult				= 0;

		m_wstrID				= L"";

		m_uiPublisherUID		= 0;
		m_wstrIP				= L"";
		m_iChannelingCode		= 0;

		m_uiAccountType			= 0;
		m_uiAccountState		= 0;

#ifdef SERV_LOGIN_TOU
		m_strAgreementURL		= "";
#endif //SERV_LOGIN_TOU

#ifdef SERV_COUNTRY_TH
		m_wstrMasterID			= L"";
		m_wstrSocketID			= L"";
		m_bIsAKeyUser			= false;
		m_wstrAkeyUserID		= L"";
#endif // SERV_COUNTRY_TH
	}
};


//{{ 2011.1.24 ��ȿ�� KOG_OTP_MODIFY

DECL_PACKET( DBE_CH_USER_GENERAL_LOGIN_REQ )
{
	std::wstring    m_wstrServiceAccountID;
	std::wstring	m_wstrIP;
	unsigned int	m_uiPublisherUID;

	int				m_iChannelingCode;

	//{{ 2010.12.02 ��ȿ��	�ɽ��� ���� ��. �α��� ���� �Ǵ��� ���� 
#ifdef SERV_PURCHASE_TOKEN
	std::wstring    m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
	//}}

#ifdef SERV_KOG_OTP_VERIFY
	bool			m_bServerUseKogOTP;		// KogOTP ��뿩��. �������� �����ϴ� ��
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
	std::wstring	m_wstrPassword;
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD

#ifdef SERV_COUNTRY_TH
	std::wstring				m_wstrMasterID;
	std::wstring				m_wstrSocketID;
#endif // SERV_COUNTRY_TH

#ifdef SERV_ANTI_ADDICTION_SYSTEM
	unsigned int	m_uiAccountType;
#endif SERV_ANTI_ADDICTION_SYSTEM
#ifdef SERV_COUNTRY_PH
	unsigned short m_usGarenaCyberCafe;
#endif //SERV_COUNTRY_PH

	KDBE_CH_USER_GENERAL_LOGIN_REQ()
	{
		m_wstrIP = L"";
		m_uiPublisherUID = 0;
		m_iChannelingCode = 0;
#ifdef SERV_PURCHASE_TOKEN
		m_wstrPurchaseTok = L"";
#endif //SERV_PURCHASE_TOKEN

#ifdef SERV_KOG_OTP_VERIFY
		m_bServerUseKogOTP = false;
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
		m_wstrPassword = L"";
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD

#ifdef SERV_COUNTRY_TH
		m_wstrMasterID = L"";
		m_wstrSocketID = L"";
#endif // SERV_COUNTRY_TH

#ifdef SERV_ANTI_ADDICTION_SYSTEM
		m_uiAccountType = 0;
#endif SERV_ANTI_ADDICTION_SYSTEM
#ifdef SERV_COUNTRY_PH
		m_usGarenaCyberCafe = 0;
#endif //SERV_COUNTRY_PH
	};
};

DECL_PACKET( DBE_CH_USER_GENERAL_LOGIN_ACK )
{
	int							m_iOK;
	std::wstring				m_wstrUserID;
	UidType						m_iUserUID;
	std::wstring				m_wstrOTP;
	unsigned int				m_uiPublisherUID;
	int							m_iChannelingCode;

	//{{ 2010.12.02 ��ȿ��	�ɽ��� ���� ��. �α��� ���� �Ǵ��� ���� (������)
#ifdef SERV_PURCHASE_TOKEN
	std::wstring				m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
	//}}

	KDBE_CH_USER_GENERAL_LOGIN_ACK()
	{
		m_uiPublisherUID = 0;
		m_iChannelingCode = 0;
#ifdef SERV_PURCHASE_TOKEN
		m_wstrPurchaseTok = L"";
#endif //SERV_PURCHASE_TOKEN
	};
};

DECL_PACKET( DBE_CH_USER_KOGOTP_LOGIN_REQ )
{
	std::wstring    m_wstrServiceAccountID;
	unsigned int	m_uiPublisherUID;
	std::wstring    m_wstrOTP;
	std::wstring	m_wstrIP;

	//{{ 2010.12.02 ��ȿ��	�ɽ��� ���� ��. �α��� ���� �Ǵ��� ���� 
#ifdef SERV_PURCHASE_TOKEN
	std::wstring    m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
	//}}

#if defined( SERV_STEAM ) || defined( SERV_ALL_RENEWAL_SP )
	int				m_iChannelingCode;
#endif //( SERV_STEAM ) || ( SERV_ALL_RENEWAL_SP )

	KDBE_CH_USER_KOGOTP_LOGIN_REQ()
	{
		m_uiPublisherUID = 0;
		m_wstrIP = L"";
#ifdef SERV_PURCHASE_TOKEN
		m_wstrPurchaseTok = L"";
#endif //SERV_PURCHASE_TOKEN
#if defined( SERV_STEAM ) || defined( SERV_ALL_RENEWAL_SP )
		m_iChannelingCode = 0;
#endif //( SERV_STEAM ) || ( SERV_ALL_RENEWAL_SP )
	};
};

DECL_PACKET( DBE_CH_USER_KOGOTP_LOGIN_ACK )
{
	int							m_iOK;
	std::wstring				m_wstrUserID;
	UidType						m_iUserUID;
	std::wstring				m_wstrOTP;
	unsigned int				m_uiPublisherUID;

	//{{ 2010.12.02 ��ȿ��	�ɽ��� ���� ��. �α��� ���� �Ǵ��� ���� (������)
#ifdef SERV_PURCHASE_TOKEN
	std::wstring				m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
	//}}

	KDBE_CH_USER_KOGOTP_LOGIN_ACK()
	{
		m_uiPublisherUID = 0;
#ifdef SERV_PURCHASE_TOKEN
		m_wstrPurchaseTok = L"";
#endif // SERV_PURCHASE_TOKEN
	};
};

DECL_PACKET( ELG_GAME_USER_AUTHENTICATE_REQ )
{
	KUserAuthenticateReq        m_kAuthenticateReq;
	KNexonAccountInfo           m_kNexonAccountInfo;
#ifdef SERV_EPAY_SYSTEM
	unsigned short						m_usZone;
	unsigned short						m_usGame;
#endif //SERV_EPAY_SYSTEM
};

DECL_PACKET( ELG_GAME_USER_AUTHENTICATE_ACK )
{
	int                         m_iOK;
	KAccountInfo                m_kAccountInfo;
	KNexonAccountInfo           m_kNexonAccountInfo;

	char						m_cHackingUserType;
	unsigned int				m_uiGiantAccountType;
	//{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
	bool						m_bJoinAccount;
#endif SERV_IP_ACCOUNT_LOG
	//}}
	//{{ 2011. 02. 23	������	���� ���� ī��Ʈ
#ifdef SERV_ACCOUNT_COUNT
	std::map< int, int >		m_mapAccCountInfo;
#endif SERV_ACCOUNT_COUNT
	//}}
#ifdef SERV_EPAY_SYSTEM
	unsigned short						m_usZone;
	unsigned short						m_usGame;
#endif //SERV_EPAY_SYSTEM
};

DECL_PACKET( EPUBLISHER_REPORT_HACK_USER_NOT )
{
	UidType								m_iUserUID;
	std::wstring						m_wstrUserID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	UidType								m_iServerUID;
	std::string							m_strIP;
};

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_COUNTRY_TH

//{{ ����� : [2012/6/19] //	AsiaSoft ����
DECL_PACKET( EAS_AUTH_LOGIN_REQ )
{
	UidType								m_iUserUID;
	std::wstring						m_wstrUserID;
	std::wstring						m_wstrPassword;
	std::wstring						m_wstrDomain;
	std::wstring						m_wstrIP;
	std::wstring						m_wstrSockID;
	std::wstring						m_wstrAkey;
};

DECL_PACKET( EAS_AUTH_LOGIN_ACK )
{
	int									m_iOK;

	int									m_iRecvCode;
	std::wstring						m_wstrRecvMsg;
	std::wstring						m_wstrGameID;
	int									m_iAkeyFlag;
	std::wstring						m_wstrAkeyUserID;
	std::wstring						m_wstrSockID;

	std::wstring						m_wstrAkey;
};

DECL_PACKET( EAS_GET_IPBONUS_REQ )
{
	UidType								m_iUserUID;
	std::wstring						m_wstrMasterID;
	std::wstring						m_wstrIP;
	bool								m_bIsLogin;
	std::wstring						m_wstrSockID;

	UidType								m_iGSUID;
};

DECL_PACKET( EAS_GET_IPBONUS_ACK )
{
	int									m_iOK;

	int									m_iRecvCode;
	std::wstring						m_wstrRecvMsg;
	int									m_iPCBangLevel;
	std::wstring						m_wstrSockID;
};

DECL_PACKET( EAS_AUTH_AKEY_REQ )
{
	std::wstring						m_wstrDomain;
	std::wstring						m_wstrAkeyUserID;
	std::wstring						m_wstrAkey;
	std::wstring						m_wstrIP;

	UidType								m_iServerUID;

	KEPUBLISHER_AUTHENTICATION_ACK		m_kEPUBLISHER_AUTHENTICATION_ACK;
};

DECL_PACKET( EAS_AUTH_AKEY_ACK )
{
	int									m_iOk;
	std::wstring						m_wstrRecvMessage;
	UidType								m_iServerUID;

	KEPUBLISHER_AUTHENTICATION_ACK		m_kEPUBLISHER_AUTHENTICATION_ACK;
};

DECL_PACKET( DBE_AS_USER_LOGIN_REQ )
{
	KUserAuthenticateReq        m_kAuthenticateReq;
	KNexonAccountInfo           m_kNexonAccountInfo;

	std::wstring				m_wstrMasterID;
	std::wstring				m_wstrSocketID;
	std::wstring				m_wstrOTP;
	bool						m_bLogin;
};

DECL_PACKET( DBE_AS_USER_LOGIN_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrUserID;
	std::wstring						m_wstrSocketID;
	UidType								m_iUserUID;
	std::wstring						m_wstrOTP;
};

typedef	KELG_GAME_USER_AUTHENTICATE_REQ KELG_AS_USER_AUTHENTICATE_REQ;
typedef KELG_GAME_USER_AUTHENTICATE_ACK KELG_AS_USER_AUTHENTICATE_ACK;
//}} ����� : [2012/6/19] //	AsiaSoft ����

#endif // SERV_COUNTRY_TH


//#ifdef SERV_COUNTRY_CN

DECL_PACKET( EGIANT_COMMON_INITIALIZE_REQ )
{
	std::wstring						m_wstrIP;
	unsigned short						m_usPort;
};

DECL_PACKET( EGIANT_COMMON_INITIALIZE_ACK )
{
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrZoneName;					// [32]
	BYTE								m_byteNetType;
};

DECL_PACKET( EGIANT_COMMON_NULL_SERVER )
{
};

DECL_PACKET( EGIANT_COMMON_NULL_CLIENT )
{
};

DECL_PACKET( EGIANT_AUTH_LOGIN_SUCCESS )
{
	unsigned int						m_uiRequestID;
	unsigned int						m_uiUID;
	std::wstring						m_wstrName;						// [48]
	unsigned int						m_uiType;
	std::wstring						m_wstrEMail;					// [48]
	unsigned int						m_uiState;
	unsigned int						m_uiBindType;
	std::wstring						m_wstrSuperpasswd;				// [16]
	std::wstring						m_wstrMobileAccount;			// [48]
};

DECL_PACKET( EGIANT_AUTH_LOGIN_FAIL )
{
	unsigned int						m_uiRequestID;
	int									m_iReturnCode;
	std::wstring						m_wstrName;						// [48]
	unsigned int						m_uiKey;
	std::wstring						m_wstrCoordinate;				// [7]
};


DECL_PACKET( DBE_GIANT_USER_LOGIN_REQ )
{
	bool								m_bLogin;
	std::wstring						m_wstrServiceAccountID;
	unsigned int						m_uiGiantUID;
	std::wstring						m_wstrOTP;
	unsigned int						m_uiAccountType;
	unsigned int						m_uiAccountState;

	KDBE_GIANT_USER_LOGIN_REQ()
	{
		m_uiGiantUID = 0;
		m_uiAccountType = 0;
		m_uiAccountState = 0;
	}
};

DECL_PACKET( DBE_GIANT_USER_LOGIN_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrUserID;
	UidType								m_iUserUID;
	std::wstring						m_wstrOTP;
	unsigned int						m_uiGiantUID;
	unsigned int						m_uiAccountType;
};


//////////////////////////////////////////////////////////////////////////
// 2010. 7. 19	�ڱ���		���̾�Ʈ �÷��� ����


//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
DECL_PACKET( DBE_GET_ANTI_ADDICTION_INFO_ACK )
{
	int									m_iOK;
	int			                        m_iPlayTime;
	int			                        m_iRestTime;
	std::wstring                        m_wstrLastLogoutTime;
};

DECL_PACKET( DBE_UPDATE_ANTI_ADDICTION_INFO )
{
	UidType                             m_iUserUID;
	int			                        m_iPlayTime;
	int			                        m_iRestTime;
	std::wstring						m_wstrLastLogoutTime;
};
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

DECL_PACKET( EPUBLISHER_BILLING_BALANCE_REQ )
{
	unsigned int						m_uiPublisherUID;
	UidType								m_uiActorUID;
	std::wstring						m_wstrAccount;					// [65]
	std::wstring						m_wstrIP;
#ifdef SERV_COUNTRY_CN
	std::wstring						m_wstrTID;						// [33]
	int									m_iAT;
#endif // SERV_COUNTRY_CN
#ifdef SERV_COUNTRY_IN
	std::wstring						m_wstrNickName;
#endif SERV_COUNTRY_IN

	KEPUBLISHER_BILLING_BALANCE_REQ()
	{
		m_uiPublisherUID		= 0;
		m_uiActorUID			= 0;
		m_wstrAccount			= L"";
		m_wstrIP				= L"";

#ifdef SERV_COUNTRY_CN
		m_wstrTID				= L"";
		m_iAT					= 0;
#endif // SERV_COUNTRY_CN
#ifdef SERV_COUNTRY_IN
		m_wstrNickName			= L"";
#endif SERV_COUNTRY_IN
	}
};

DECL_PACKET( EPUBLISHER_BILLING_BALANCE_ACK )
{
	int									m_iOK;
	unsigned long						m_ulBalance;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	KEPUBLISHER_BILLING_BALANCE_ACK()
	{
		m_ulBalance = 0;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		m_iCashType = -1;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	}
};



DECL_PACKET( ELG_VERIFY_GIANT_MATRIX_CARD_REQ )
{
	std::wstring						m_wstrID;
	std::wstring						m_wstrMatrix;
	unsigned int						m_uiGiantMatrixKey;
};

typedef KPacketOK		KELG_VERIFY_GIANT_MATRIX_CARD_ACK;
typedef KPacketOK		KELG_VERIFY_GIANT_MATRIX_CARD_NOT;


DECL_PACKET( EGIANT_BILLING_RETURN )
{
	std::wstring						m_wstrTID;						// [33]
	int									m_iBalance;
	int									m_iBonus;
	int									m_iHadFilled;
	int									m_iRet;
	int									m_iSubat;
	int									m_iAT;
	int									m_iPoint;
	unsigned int						m_uiGiantUID;
};



DECL_PACKET( EGIANT_BILLING_USE_CARD_CMD )
{
	short								m_sSource;
	std::wstring						m_wstrAccount;
	std::wstring						m_wstrTID;
	std::wstring						m_wstrCardID;
	std::wstring						m_wstrPasswd;
	std::wstring						m_wstrIP;
	unsigned int						m_uiUID;
	int									m_iAT;
	unsigned int						m_uiConTempID;
	unsigned int						m_uiOperateUID;
};

DECL_PACKET( EGIANT_BILLING_USE_CARD_RET_CMD )
{
	short								m_sSource;
	std::wstring						m_wstrAccount;
	std::wstring						m_wstrTID;
	std::wstring						m_wstrCardID;
	std::wstring						m_wstrPasswd;
	std::wstring						m_wstrIP;
	int									m_iPoint;
	int									m_iRet;
	unsigned int						m_uiUID;
	int									m_iAT;
	unsigned int						m_uiConTempID;
	unsigned int						m_uiOperateUID;
};

DECL_PACKET( EGIANT_BILLING_CONSUME_REQ )
{
	unsigned int						m_uiGiantUID;
	int									m_iPoint;
	int									m_iSource;
	std::wstring						m_wstrTID;						// [33]
	std::wstring						m_wstrRemark;					// [201] ������� ����
	std::wstring						m_wstrIP;						// [17]
	int									m_iAT;
};

DECL_PACKET( EGIANT_BILLING_CONSUME_ACK )
{
	int									m_iOK;
	int									m_iBalance;
	int									m_iBonus;
	bool								m_bHasFilled;
	int									m_iPoint;
	unsigned int						m_uiGiantUID;
};

DECL_PACKET( EGIANT_BILLING_PCARD_CMD )
{
	unsigned int						m_uiGiantUID;
	int									m_iSource;
	std::wstring						m_wstrTID;						// [33]
	std::wstring						m_wstrPCardID;					// [33]
	std::wstring						m_wstrIP;						// [17]
	int									m_iAT;
};

DECL_PACKET( EGIANT_BILLING_SCARD_CMD )
{
	unsigned int						m_uiGiantUID;
	int									m_iSource;
	std::wstring						m_wstrTID;						// [33]
	std::wstring						m_wstrSCardID;					// [33]
	std::wstring						m_wstrIP;						// [17]
	int									m_iAT;
};

//////////////////////////////////////////////////////////////////////////
// 2010.07.07 �ڱ��� / ���̾�Ʈ RoleRegServer ����
DECL_PACKET( EGIANT_ROLEREG )
{
	unsigned short						m_usRegType;
	unsigned short						m_usServerID;
	unsigned int						m_uiGiantUID;
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrName;						// [32]
	unsigned int						m_uiRoleUID;
	unsigned short						m_usState;
};


DECL_PACKET( EGIANT_ROLEREG_CREATE_UNIT_REQ )
{
	UidType								m_iActorUID;
	unsigned int						m_uiGiantUID;
	KEGS_CREATE_UNIT_REQ				m_CreateUnitReqPacket;
};

DECL_PACKET( EGIANT_ROLEREG_CREATE_UNIT_ACK )
{
	int									m_iOK;
	KEGS_CREATE_UNIT_REQ				m_CreateUnitReqPacket;
};

DECL_PACKET( EGIANT_ROLEREG_DELETE_UNIT_REQ )
{
	std::wstring						m_wstrNickName;
	unsigned int						m_uiGiantUID;
};

DECL_PACKET( EGIANT_ROLEREG_DELETE_UNIT_ACK )
{
	int									m_iOK;
};

DECL_DATA( KGiantDeletedUnitInfo )
{
	std::wstring						m_wstrNickName;
	UidType								m_iUnitUID;
	unsigned int						m_uiGiantUID;
};

// ���� ����ϴ� �� ����. �ּ�ó��.
// DECL_PACKET( DBE_INSERT_GIANT_DELETE_UNIT_REQ )
// {
// 	KGiantDeletedUnitInfo				m_DeletedUnitInfo;
// };

// ���� ����ϴ� �� ����. �ּ�ó��.
// DECL_PACKET( DBE_UPDATE_GIANT_DELETE_UNIT_NOT )
// {
// 	std::wstring						m_wstrNickName;
// 	unsigned int						m_uiGiantUID;
// };

DECL_PACKET( EGIANT_ROLEREG_CREATE_GUILD_REQ )
{
	UidType								m_iActorUID;
	unsigned int						m_uiGiantUID;
	KEGS_CREATE_GUILD_REQ				m_CreateGuildReqPacket;
};

DECL_PACKET( EGIANT_ROLEREG_CREATE_GUILD_ACK )
{
	int									m_iOK;
	KEGS_CREATE_GUILD_REQ				m_CreateGuildReqPacket;
};

DECL_PACKET( EGIANT_ROLEREG_DELETE_GUILD_REQ )
{
	std::wstring						m_wstrGuildName;
	unsigned int						m_uiGiantUID;
};

DECL_PACKET( EGIANT_ROLEREG_DELETE_GUILD_ACK )
{
	int									m_iOK;
};

// DECL_PACKET( DBE_GET_GIANT_DELETED_UNIT_LIST_ACK )
// {
// 	std::vector<KGiantDeletedUnitInfo>	m_vecDeletedUnitInfo;
// };

//////////////////////////////////////////////////////////////////////////
// 2010.08.09 �ڱ��� / ���̾�Ʈ InfoServer ����
DECL_PACKET( EGIANT_INFO_GET_CCU_REQ )
{
	__int64								m_TimeStamp;
};

DECL_PACKET( EGIANT_INFO_GET_TOTAL_CCU_ACK )
{
	__int64								m_TimeStamp;
	unsigned short						m_usServerID;
	unsigned short						m_usServerType;
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_strZoneName;				// [32]
	unsigned int						m_uiCCU;
};

DECL_PACKET( EGIANT_INFO_GET_CHANNEL_CCU_ACK )
{
	__int64								m_TimeStamp;
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	//unsigned int						m_uiCountryNum;
	//unsigned int						m_uiCountryID;
	//unsigned int						m_uiCountryCCU;
	std::map<UidType, UINT>				m_mapChannelCCU;
};

DECL_PACKET( EGIANT_INFO_CHAT_NOT )
{
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrUserName;				// [32]
	std::wstring						m_wstrGMName;				// [32]
	unsigned int						m_uiCountryID;
	unsigned int						m_uiSceneID;
	unsigned int						m_uiType;
	std::wstring						m_wstrContent;				// [256]
	std::wstring						m_wstrServer;				// [32]
};

DECL_PACKET( EGIANT_INFO_PUNISH_REQ )
{
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrUserName;				// [32]
	std::wstring						m_wstrCountryName;			// [32]
	std::wstring						m_wstrServerName;			// [32]
	std::wstring						m_wstrGMName;				// [32]
	std::wstring						m_wstrReason;				// [128]
	unsigned short						m_usOperation;
	int									m_iDelay;
	short								m_sLevel;
	unsigned int						m_uiGiantUID;
	unsigned int						m_uiStartTime;
};



#ifdef SERV_INFOSERVER_ADD_WORK
DECL_PACKET( EGIANT_INFO_USERONLINE_REQ )
{
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrUserName;				// [32]
	std::wstring						m_wstrGMName;				// [32]
	BYTE								m_bUserOnline;
};
DECL_PACKET( EGIANT_INFO_USERONLINE_ACK )
{
	std::wstring						m_wstrUserName;				// [32]
	std::wstring						m_wstrGMName;				// [32]
	BYTE								m_bUserOnline;
};

DECL_PACKET( EGIANT_INFO_REQUESTID_REQ )
{
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrUserName;				// [32]
	std::wstring						m_wstrGMName;				// [32]
	unsigned int						m_uiGiantUID;
	std::wstring						m_wstrAccount;				// [32]
	BYTE								m_bWrite;
};


DECL_PACKET( EGIANT_INFO_REQUESTID_ACK )
{
	std::wstring						m_wstrUserName;				// [32]
	std::wstring						m_wstrGMName;				// [32]
	unsigned int						m_uiGiantUID;
	std::wstring						m_wstrAccount;				// [32]
	BYTE								m_bWrite;
};
#endif //SERV_INFOSERVER_ADD_WORK

DECL_PACKET( ELG_CHAT_OBSERVATION_NOT )
{
	UidType								m_iServerUID;
	char								m_cChatPacketType;
	std::wstring						m_wstrSenderUnitNickName;
	std::wstring						m_wstrMsg;
};


//#endif // SERV_COUNTRY_CN 
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( EPUBLISHER_SECURITY_AUTH_REQ )
{
	// �߰� ���� ���� ������ �� �ش� ��Ŷ�� �̿��Ѵ�.
	enum SECURITY_AUTH_TYPE
	{
		SAT_NONE,						// ����(�ʱⰪ)
		SAT_CHK_SECOND_SECURITY,		// K3R 2�� �����е� ���� üũ
		SAT_MATRIX_CARD,				// Giant MatrixKey �ý��� ���
	};

	int						m_iSecurityAuthType;
	UidType					m_iUserUID;
	std::wstring			m_wstrUserID;
	unsigned int			m_uiPublisherUID;
	std::wstring			m_wstrIP;

#ifdef SERV_PUBLISHER_MATRIXKEY
	unsigned int			m_uiMatrixKey;
	std::wstring			m_wstrMatrix;
#endif // SERV_PUBLISHER_MATRIXKEY

#ifdef SERV_COUNTRY_CN
	unsigned int			m_uiRequestID;	// �߱� �����ϰ�� �� ��
	unsigned short			m_usZone;		// �߱� �����ϰ�� �� ��
	unsigned short			m_usGame;		// �߱� �����ϰ�� �� ��
#endif // SERV_COUNTRY_CN

	KEPUBLISHER_SECURITY_AUTH_REQ()
	{
		m_iSecurityAuthType = SAT_NONE;
		m_iUserUID			= -1;
		m_wstrUserID		= L"";
		m_uiPublisherUID	= 0;
#ifdef SERV_PUBLISHER_MATRIXKEY
		m_uiMatrixKey		= 0;
		m_wstrMatrix		= L"";
#endif // SERV_PUBLISHER_MATRIXKEY
#ifdef SERV_COUNTRY_CN
		m_uiRequestID		= 0;	// �߱� �����ϰ�� �� ��
		m_usZone			= 0;		// �߱� �����ϰ�� �� ��
		m_usGame			= 0;		// �߱� �����ϰ�� �� ��
#endif // SERV_COUNTRY_CN
	}

};

DECL_PACKET( EPUBLISHER_SECURITY_AUTH_ACK )
{
	int						m_iOK;
	std::string				m_strSecondSecurityURL;	// K3R 2�� ���� URL

	KEPUBLISHER_SECURITY_AUTH_ACK()
	{
		m_iOK					= -1;
		m_strSecondSecurityURL	= "";
	}
};


DECL_PACKET( EPUBLISHER_SECURITY_INFO_NOT )
{
	enum SECURITY_INFO_TYPE
	{
		SIT_NONE,						// ����(�ʱⰪ)
		SIT_MATRIX_CARD,				// Giant MatrixKey �ý��� ���
	};

	int									m_iSecurityInfoType;
	unsigned int						m_uiMatrixKey;
	std::wstring						m_wstrCoordinate;

	
	KEPUBLISHER_SECURITY_INFO_NOT()
	{
		m_iSecurityInfoType		= SIT_NONE;
		m_uiMatrixKey			= 0;
		m_wstrCoordinate		= L"";
	}
};


//EPUBLISHER_PCBANG_USER_LOGIN_REQ ���� ������
DECL_PACKET( EPUBLISHER_REG_USER_REQ )
{
	UidType							m_iActorUID;
	std::wstring					m_wstrServiceAccountID;
	std::wstring					m_wstrServicePassword;
	std::string						m_strIP;

#ifdef SERV_COUNTRY_TH
	std::wstring					m_wstrSockID;
	UidType							m_iGSUID;
#endif //SERV_COUNTRY_TH

	KEPUBLISHER_REG_USER_REQ()
	{
		m_iActorUID				= 0;
		m_wstrServiceAccountID	= L"";
		m_wstrServicePassword	= L"";
		m_strIP					= "";

#ifdef SERV_COUNTRY_TH
		m_wstrSockID			= L"";
		m_iGSUID				= 0;
#endif //SERV_COUNTRY_TH
	}
};

DECL_PACKET( EPUBLISHER_REG_USER_ACK )
{
	int						m_iOK;	// ���� ������ ���ζ� ������ ����. NET_OK �ƴϸ� ���� ����
	std::wstring			m_wstrServiceAccountID;

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
	// PCBangType�� ��� 0���� �����ؾ� ��, Ÿ�Ժ� ������ Vector �迭���� �޾ƿ��� ���·� �����Ǿ��̱� ������
	bool					m_bIsPcBang;
	int						m_iPCBangType;
	int						m_iCheckState;
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_COUNTRY_TH
	std::wstring			m_wstrSockID;
#endif //SERV_COUNTRY_TH

	KEPUBLISHER_REG_USER_ACK()
	{
		m_iOK = -1;

		m_wstrServiceAccountID	= L"";

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		m_bIsPcBang				= false;
		m_iPCBangType			= -1;	// 0�� �����ִ� PC����
		m_iCheckState			= -99;	//HAN_NETCAFE_E_UNKNOWN (JP)
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_COUNTRY_TH
		m_wstrSockID			= L"";
#endif //SERV_COUNTRY_TH
	}
};

//EPUBLISHER_PCBANG_USER_LOGOUT_REQ
DECL_PACKET( EPUBLISHER_UNREG_USER_REQ )
{
	UidType							m_iActorUID;
	std::wstring					m_wstrServiceAccountID;
	std::wstring					m_wstrServicePassword;
	std::string						m_strIP;

#ifdef SERV_COUNTRY_TH
	std::wstring					m_wstrSockID;
	UidType							m_iGSUID;
#endif //SERV_COUNTRY_TH

	KEPUBLISHER_UNREG_USER_REQ()
	{
		m_iActorUID = 0;
		m_wstrServiceAccountID = L"";
		m_wstrServicePassword = L"";
		m_strIP = "";

#ifdef SERV_COUNTRY_TH
		m_wstrSockID			= L"";
		m_iGSUID				= 0;
#endif //SERV_COUNTRY_TH
	}
};

DECL_PACKET( EPUBLISHER_UNREG_USER_ACK )
{
	int						m_iOK;
	std::wstring			m_wstrServiceAccountID;

	KEPUBLISHER_UNREG_USER_ACK()
	{
		m_iOK = -1;
		m_wstrServiceAccountID = L"";
	}
};

DECL_PACKET( ELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK )
{
	int						m_iOK;
	std::wstring			m_wstrServiceAccountID;

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
	bool					m_bIsPcBang;
	int						m_iPCBangType;
	int						m_iCheckState;
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_COUNTRY_TH
	std::wstring			m_wstrSockID;
#endif //SERV_COUNTRY_TH

	KELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK()
	{
		m_iOK = -1;
		m_wstrServiceAccountID = L"";

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		m_bIsPcBang				= false;
		m_iPCBangType			= -1;	// 0�� �����ִ� PC����
		m_iCheckState			= -99;	//HAN_NETCAFE_E_UNKNOWN (JP)
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_COUNTRY_TH
		m_wstrSockID			= L"";
#endif //SERV_COUNTRY_TH
	}
};

//{{ 2011. 03. 03	������	�߱� ���� ����
DECL_PACKET( EBILL_CHECK_PRODUCT_INFO_REQ )
{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	std::map<int, int>					m_mapReleaseTick;
#else
	int                                 m_iReleaseTick;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

};

DECL_PACKET( EBILL_CHECK_PRODUCT_INFO_ACK )
{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	std::map<int, int>					m_mapReleaseTick;
#else
	int                                 m_iReleaseTick;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	std::map< int, KBillProductInfo >       m_mapBillProductInfo;
	std::map< int, KBillProductAttribute >  m_mapBillProductAttribute;
	std::map< int, KBillPackageInfo >       m_mapBillPackageInfo;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	std::vector<std::wstring>				m_vecStartEvent;
	std::vector<std::wstring>				m_vecEndEvent;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

};


//{{ ����� : [2010/7/28/] //	ĳ�� �α�(������ �ֱ⶧���� define ����)
DECL_DATA( KBuyCharInfo )
{
	char                        m_cLevel;
	int                         m_iUnitClass;
};
//}} ����� : [2010/7/28/] //	ĳ�� �α�(������ �ֱ⶧���� define ����)

// ���� ��Ŷ�� ���� ĳ�ü����� ���ſ� �������� ��Ȱ ���� ���� �� ����
DECL_PACKET( EBILL_BUY_PRODUCT_REQ )
{
	int                                 m_iServerGroupID;
	int									m_iChannelingCode;

	std::wstring                        m_wstrUserID;
	unsigned int						m_uiPublisherUID;
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	std::wstring                        m_wstrNickName;
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	//{{ ����� : [2010/7/28/] //	ĳ�� �α�
	KBuyCharInfo						m_BuyCharInfo;
	//}} ����� : [2010/7/28/] //	ĳ�� �α�

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iUseCashType;
	int									m_iUserIP;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	// ���� ������ �� �� �ۺ��ſ��� m_wstrTotalOrderID�� �Ѱ���. 
	// ���� ���� �ϴ� ����� ����Ȯ�� �� m_wstrTotalOrderID �������� Ȯ�� �ؾ���
	std::wstring                        m_wstrTotalOrderID;


	KEBILL_BUY_PRODUCT_REQ()
	{
		m_iServerGroupID	= 0;
		m_iChannelingCode	= 0;
		m_wstrUserID		= L"";
		m_uiPublisherUID	= 0;
        m_iUserUID			= 0;
		m_iUnitUID			= 0;
		m_wstrNickName		= L"";
		m_strUserIP			= "";
		m_vecBillBuyInfo.clear();
		m_iUseCashType		= 0;
		m_iUserIP			= 0;
		m_wstrTotalOrderID	= L"";
	}
};

DECL_PACKET( EBILL_BUY_PRODUCT_ACK )
{
	int                                 m_iOK;
	std::vector< KBillOrderInfo >       m_vecBillOrderInfo;
	//{{ 100713 ������ ��Ű���������� ��� ���ϱ� ��Ͽ��� ���ֱ� ���� ���
	std::vector< int >				    m_vecPackageID;
	//}} 100713 ������ ��Ű���������� ��� ���ϱ� ��Ͽ��� ���ֱ� ���� ���
	int									m_iTotalCash;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iCashType;
	KGlobalCashInfo						m_RemainCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
};

DECL_PACKET( EBILL_PREPARE_BUY_PRODUCT_ACK )
{
	int									m_iOK;
	KEBILL_BUY_PRODUCT_REQ				m_kEBILL_BUY_PRODUCT_REQ;
};


DECL_PACKET( EBILL_GIFT_ITEM_REQ )
{
	int                                 m_iServerGroupID;
	int									m_iChannelingCode;

	std::wstring                        m_wstrSenderUserID;
	unsigned int						m_uiSenderPublisherUID;

	std::wstring                        m_wstrSenderNickName;
	std::wstring                        m_wstrReceiverUserID;
	std::wstring                        m_wstrReceiverNickName;

	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	UidType								m_iSenderUserUID;
	UidType								m_iSenderUnitUID;

	UidType								m_iReceiverUserUID;
	UidType								m_iReceiverUnitUID;
	
	std::wstring						m_wstrPresentMessage;
		
	//{{ ����� : [2010/7/28/] //	ĳ�� �α�
	KBuyCharInfo						m_BuyCharInfo;
	KBuyCharInfo						m_RecvCharInfo;
	//}} ����� : [2010/7/28/] //	ĳ�� �α�
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	std::wstring						m_wstrUserIP;

	// ���� ������ �� �� �ۺ��ſ��� m_wstrTotalOrderID�� �Ѱ���. 
	// ���� ���� �ϴ� ����� ����Ȯ�� �� m_wstrTotalOrderID �������� Ȯ�� �ؾ���
	std::wstring                        m_wstrTotalOrderID;

	KEBILL_GIFT_ITEM_REQ()
	{
		m_iServerGroupID		=	0;
		m_iChannelingCode		=	0;

		m_wstrSenderUserID		=	L"";
		m_uiSenderPublisherUID	=	0;
		
		m_wstrSenderNickName	=	L"";
		m_wstrReceiverUserID	=	L"";
		m_wstrReceiverNickName	=	L"";

		m_iSenderUserUID		=	0;
		m_iSenderUnitUID		=	0;

		m_iReceiverUserUID		=	0;
		m_iReceiverUnitUID		=	0;

		m_wstrPresentMessage	=	L"";
		
		m_wstrTotalOrderID		=	L"";
	}
};

DECL_PACKET( EBILL_GIFT_ITEM_ACK )
{
	int                                 m_iOK;
	std::vector< KBillOrderInfo >       m_vecBillOrderInfo;
	int									m_iTotalCash;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iCashType;
	KGlobalCashInfo						m_RemainCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
};

DECL_PACKET( EBILL_PREPARE_GIFT_ITEM_ACK )
{
	int									m_iOK;
	KEBILL_GIFT_ITEM_REQ				m_kEBILL_GIFT_ITEM_REQ;
};

DECL_PACKET( DBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT )
{
	UidType							m_iUserUID;
	int								m_iResultCode;
	std::string						m_strOutString;
};

//
DECL_DATA( KPurchaserInfo )
{
	int									m_iServerGroupID;
	UidType								m_iUserUID;
	std::wstring						m_wstrUserName;
	UidType								m_iUnitUID;
	std::wstring						m_wstrUnitNickName;
	UCHAR								m_ucLevel;
	std::wstring						m_wstrIP;
	unsigned int						m_uiPublisherUID;
	int									m_iChannelingCode;

	CON_COPYCON_ASSIGNOP( KPurchaserInfo, right )
	{
		m_iServerGroupID		= right.m_iServerGroupID;
		m_iUserUID				= right.m_iUserUID;
		m_wstrUserName			= right.m_wstrUserName;
		m_iUnitUID				= right.m_iUnitUID;
		m_wstrUnitNickName		= right.m_wstrUnitNickName;
		m_ucLevel				= right.m_ucLevel;
		m_wstrIP				= right.m_wstrIP;
		m_uiPublisherUID		= right.m_uiPublisherUID;
		m_iChannelingCode		= right.m_iChannelingCode;

		return *this;
	}
};

DECL_PACKET( EBILL_USE_COUPON_REQ )
{
	// ���� ��� ���� Return Code
	enum ENUM_RETURN_CODE_TYPE 
	{
		GB_RCT_SUCCESS						= 0,	// ����
		GB_RCT_FAIL							= -1,	// ����
		GB_RCT_ACCOUNT_NOT_EXIST			= -2,	// ���� ����
		GB_RCT_CASH_NOT_EXIST				= -3,	// �ܾ� ����		
		GB_RCT_ITEM_CARD_NOT_EXIST			= -4,	// ������ ī�� ����
		GB_RCT_ITEM_CARD_NOT_AREA			= -5,	// �ش� ������ ī��� �ش� �������� ��� �Ұ�
		GB_RCT_ITEM_CARD_DUPLICATE			= -6,	// ��Ʈ���� ī�� ���ε�
		GB_RCT_ZONE_CARD_ID_ERROR			= -7,	// ���� ī�� ID����
		GB_RCT_ZONE_CARD_PASSWORD_ERROR		= -8,	// ���� ī�� �н����� ����
		GB_RCT_DB_ERROR						= -9,	// DB����
	};



	KPurchaserInfo						m_PurchaserInfo;
	int									m_iCouponType;
	std::wstring						m_wstrSerialCode;
	std::wstring						m_wstrTransaction;	// ���� ����- �ۺ� ���� - ������ �ֱ� �������� ���

	KEBILL_USE_COUPON_REQ()
	{
		m_iCouponType		=	0;		// ���� �߱� �����ϰ�� �ǹ� ���� ����
		m_wstrSerialCode	=	L"";
		m_wstrTransaction	=	L"";
	}
};

DECL_DATA( KBillCouponItemInfo )
{
	int										m_iProductNO;
	int										m_iCount;
	UidType									m_iTranNo;
};

DECL_PACKET( EBILL_USE_COUPON_ACK )
{
	int										m_iOK;
	std::vector< KBillCouponItemInfo >		m_vecBillCouponItemInfo;
};


DECL_PACKET( EBILL_USE_COUPON_RESULT_REQ )
{
	UidType								m_iGSUID;
	UidType								m_iUserUID;
	std::wstring						m_wstrTransaction;
	int									m_iRet;
	int									m_iChargeItem;
	int									m_iChargeItemCnt;
	int									m_iChargeCash;
	int									m_iChargeBonus;

	KEBILL_USE_COUPON_RESULT_REQ()
	{
		m_iGSUID				=	0;
		m_iUserUID				=	0;
		m_wstrTransaction		=	L"";
		m_iRet					=	0;
		m_iChargeItem			=	0;
		m_iChargeItemCnt		=	1;	// ������ Default 1
		m_iChargeCash			=	0;
		m_iChargeBonus			=	0;
	}
};

DECL_PACKET( EBILL_USE_COUPON_RESULT_ACK )
{
	int									m_iOK;
	int									m_iTotalCash;
	std::vector< KBillCouponItemInfo >		m_vecBillCouponItemInfo;
};

DECL_PACKET( EBILL_USE_COUPON_RESERVE_ACK )
{
	int									m_iOK;
	KEBILL_USE_COUPON_REQ				m_kPacketReq;
};

// ĳ�� �κ��丮�� �ִ� �������� �� ĳ���� �κ��丮�� �������� ��Ŷ
DECL_PACKET( EBILL_PICK_UP_REQ )
{
	UidType                             m_iTransNo;
	int                                 m_iProductNo;
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapSocketForCash;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	BYTE								m_byteProductKind;
	std::vector< KNXBTPickUpPackageInfo > m_vecPickUpPackageInfo;
	int                                 m_iSubProductNo;
	int                                 m_iSubProductID;
#endif //SERV_GLOBAL_CASH_PACKAGE
};

DECL_PACKET( EBILL_PICK_UP_ACK )
{
	// ���� ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK �̰ſ� ���ε�

	int                                 m_iOK;
	int                                 m_iProductNo;
	std::map< int, int >                m_mapSocketForCash;
};

DECL_PACKET( ELG_EXCHANGE_CASH_REQ )
{
	KPurchaserInfo						m_PurchaserInfo;
	int									m_iPoint;
};

DECL_PACKET( ELG_EXCHANGE_CASH_ACK )
{
	int									m_iOK;
	int									m_iPoint;
};

DECL_PACKET( EBILL_CHECK_BALANCE_REQ )
{
	int									m_iPublisherCashBalance;

	KEBILL_CHECK_BALANCE_REQ()
	{
		m_iPublisherCashBalance = 0;
	}
};


DECL_PACKET( EBILL_EXCHANGE_CASH_RESERVE_REQ )
{
	KPurchaserInfo						m_PurchaserInfo;
	int									m_iPoint;
};

DECL_PACKET( EBILL_EXCHANGE_CASH_RESERVE_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrTransaction;
	KEBILL_EXCHANGE_CASH_RESERVE_REQ	m_kPacketReq;
};

DECL_PACKET( EBILL_EXCHANGE_CASH_REQ )
{
	UidType								m_iGSUID;
	UidType								m_iUserUID;
	std::wstring						m_wstrTransaction;
	int									m_iRet;
	int									m_iPoint;
};

DECL_PACKET( EBILL_EXCHANGE_CASH_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iTotalCash;
	int									m_iChargeCash;
	int									m_iChargeBonus;
	int									m_iBonusItem;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	KGlobalCashInfo						m_RemainCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	KEBILL_EXCHANGE_CASH_ACK()
	{
		m_iUnitUID = 0;
		m_iTotalCash = 0;
		m_iChargeCash = 0;
		m_iChargeBonus = 0;
		m_iBonusItem = 0;
	}
};

DECL_PACKET( EBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ )
{
	KEPUBLISHER_BILLING_BALANCE_REQ			m_kCheckGiantBalanceReq;
};

DECL_PACKET( EBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrTransaction;	
	KEPUBLISHER_BILLING_BALANCE_REQ			m_kCheckGiantBalanceReq;
};

//{{ ����� : [2010/8/19/] //	���� ��� ���� . SERV_GLOBAL_BILLING. ������ �߰� ��ȿ��
DECL_PACKET( DBE_GET_NICKNAME_BY_UNITUID_REQ )
{
	std::vector< UidType >	m_vecUID;
	std::vector< KBillOrderInfo >       m_vecBillOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	std::vector< KBillPackageInfo >     m_vecPackageInfo;
	std::vector< int >					m_vecSubPackageTrans;
	std::vector< int >					m_vecSubPackageInfo;
#endif //SERV_GLOBAL_CASH_PACKAGE
};

DECL_PACKET( DBE_GET_NICKNAME_BY_UNITUID_ACK )
{
	std::map< UidType, std::wstring >	m_mapNickName;
	std::vector< KBillOrderInfo >       m_vecBillOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	std::vector< KBillPackageInfo >     m_vecPackageInfo;
	std::vector< int >					m_vecSubPackageTrans;
	std::vector< int >					m_vecSubPackageInfo;
#endif //SERV_GLOBAL_CASH_PACKAGE
};
//}} ����� : [2010/8/19/] //	���� ��� ����

#ifdef GIANT_RESURRECTION_CASHSTONE

DECL_PACKET( DBE_REALTIME_RESURRECTION_CASHSTONE_NOT )
{
	UidType                             m_iUnitUID;
	int									m_iNumResurrection_CashStone;
};

DECL_PACKET( DBE_RESURRECTION_CASHSTONE_NOT ) //2011.12.19 lygan_������ //ĳ�������� �ٷ� DB �� ���� ������ ��Ŷ
{
	UidType                             m_iUnitUID;
	int									m_iNumResurrection_CashStone;
};

#endif //GIANT_RESURRECTION_CASHSTONE
//}}


//////////////////////////////////////////////////////////////////////////

#ifdef SERV_COUNTRY_US 

DECL_PACKET( EPL_BT_HEALTH_CHECK_REQ )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4
};

DECL_PACKET( EPL_BT_HEALTH_CHECK_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4
};

DECL_PACKET( EPL_BT_BALANCE_CHECK_REQ )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	unsigned long	m_ulUserNo;
	std::wstring	m_wstrUserID;

	unsigned long	m_ulRealCash;
	unsigned long	m_ulBonusCash;
};

DECL_PACKET( EPL_BT_BALANCE_CHECK_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	unsigned long	m_ulUserNo;
	std::wstring	m_wstrUserID;

	unsigned long	m_ulRealCash;
	unsigned long	m_ulBonusCash;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};


DECL_PACKET( EPL_BT_PURCHASEITEM_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	std::wstring                        m_wstrOrderID;
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrUserID;	
	UidType								m_iUserUID;
	std::wstring                        m_wstrNickName;
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;
	
	bool                                 m_bResurrectionStone;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};

DECL_PACKET( EPL_BT_GIFTITEM_REQ )
{
	unsigned long	m_ulReqKey;		// ��Ŷ ����� ���� ��Ŷ ��ȣ

	std::wstring                        m_wstrOrderID;
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrSenderUserID;
	std::wstring                        m_wstrSenderNickName;
	std::wstring                        m_wstrReceiverUserID;
	//{{ ����� : [2010/2/9/] //	UID �߰�
	UidType								m_iSenderUnitUID;
	UidType								m_iReceiverUID;
	std::wstring						m_wstrPresentMessage;
	//}} ����� : [2010/2/9/] //	UID �߰�
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;
};

DECL_PACKET( EPL_BT_GIFTITEM_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	std::wstring                        m_wstrOrderID;
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrUserID;	
	UidType								m_iUserUID;
	std::wstring                        m_wstrNickName;
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};

DECL_PACKET( EPL_BT_REGCOUPON_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	UidType								m_iUserUID;
	//int									m_iItemID;
	//int									m_iItemCount;
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};


#endif // SERV_COUNTRY_US
//////////////////////////////////////////////////////////////////////////


#ifdef SERV_COUNTRY_BR

DECL_PACKET( EPL_BR_HEALTH_CHECK_REQ )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4
};

DECL_PACKET( EPL_BR_HEALTH_CHECK_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4
};

DECL_PACKET( EPL_BR_BALANCE_CHECK_REQ )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	unsigned long	m_ulUserNo;
	std::wstring	m_wstrUserID;

	unsigned long	m_ulRealCash;
	unsigned long	m_ulBonusCash;
};

DECL_PACKET( EPL_BR_BALANCE_CHECK_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	unsigned long	m_ulUserNo;
	std::wstring	m_wstrUserID;

	unsigned long	m_ulRealCash;
	unsigned long	m_ulBonusCash;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};


DECL_PACKET( EPL_BR_PURCHASEITEM_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	std::wstring                        m_wstrOrderID;
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrUserID;	
	UidType								m_iUserUID;
	std::wstring                        m_wstrNickName;
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	bool                                 m_bResurrectionStone;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};

DECL_PACKET( EPL_BR_GIFTITEM_REQ )
{
	unsigned long	m_ulReqKey;		// ��Ŷ ����� ���� ��Ŷ ��ȣ

	std::wstring                        m_wstrOrderID;
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrSenderUserID;
	std::wstring                        m_wstrSenderNickName;
	std::wstring                        m_wstrReceiverUserID;
	//{{ ����� : [2010/2/9/] //	UID �߰�
	UidType								m_iSenderUnitUID;
	UidType								m_iReceiverUID;
	std::wstring						m_wstrPresentMessage;
	//}} ����� : [2010/2/9/] //	UID �߰�
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;
};

DECL_PACKET( EPL_BR_GIFTITEM_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	std::wstring                        m_wstrOrderID;
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrUserID;	
	UidType								m_iUserUID;
	std::wstring                        m_wstrNickName;
	//{{100310 ������ ������ �߰�
	std::string							m_strUserIP;
	//}}100310 ������ ������ �߰�
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};

DECL_PACKET( EPL_BR_REGCOUPON_ACK )
{
	unsigned short	m_usReqLen;		// IN	1
	unsigned short	m_usReqType;	// IN	2
	unsigned long	m_ulReqKey;		// IN	3
	unsigned short	m_usRetCode;	// OUT	4

	UidType								m_iUserUID;
	//int									m_iItemID;
	//int									m_iItemCount;
	std::vector< KBillBuyInfo >         m_vecBillBuyInfo;

	//{{ ���ϸ޼����� Ȯ���ϱ� ���� �κ� �߰��մϴ�. 2012.03.07 ������
	std::wstring						m_wstrRetMsg;
	//}}
};


#endif // SERV_COUNTRY_BR


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_COUNTRY_TWHK

//{{ 2011.7.12 ���� : Gash ����Ʈ ����
#ifdef SERV_BILLING_ADD_GASH_POINT

// Gash �� ����Ʈ ��û�ϴ� ��Ŷ
DECL_PACKET( EGASH_GET_GASHPOINT_REQ )
{
	UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;
	std::string m_strServicePassword;// 0
	std::string m_strAuthenticationFlag;	// 0
	std::string m_strType;					// P
	std::string m_strDuplicateCheck;		// 0
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_ACCOUNTING
	{
		GRPAT_SERVICE_CODE,
		GRPAT_SERVICE_REGION,
		GRPAT_SERVICE_ACCOUNTID,
		GRPAT_TYPE,		
		GRPAT_LOGINTIME,		
		GRPAT_LOGOUTTIME,
		GRPAT_IPADDRESS,
		GRPAT_PACKETID,

		GRPAT_NUM,
	};

	void GetGashPacket( std::string& strPacket )
	{
		strPacket.clear();

		strPacket = m_strServiceCode;
		strPacket += "\'";
		strPacket += m_strServiceRegion;
		strPacket += "\'";
		strPacket += m_strServiceAccountID;
		strPacket += "\'";
		strPacket += m_strServicePassword;
		strPacket += "\'";
		strPacket += m_strAuthenticationFlag;
		strPacket += "\'";
		strPacket += m_strType;
		strPacket += "\'";
		strPacket += m_strDuplicateCheck;
		strPacket += "\'";
		strPacket += m_strIPAddress;
		strPacket += "\'";
		strPacket += m_strPacketID;
		strPacket += "\'\r\n";

		return;
	}
};

// Gash �� ����Ʈ ��û�ϴ� ��Ŷ
DECL_PACKET( EGASH_DECREASE_GASHPOINT_REQ )
{
	UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;
	std::string m_strType;					// P
	std::string m_strShopTime;
	std::string m_strPoints;
	std::string m_strIPAddress;
	std::string m_strMemo;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_ACCOUNTING
	{
		GRPAT_SERVICE_CODE,
		GRPAT_SERVICE_REGION,
		GRPAT_SERVICE_ACCOUNTID,
		GRPAT_TYPE,		
		GRPAT_LOGINTIME,		
		GRPAT_LOGOUTTIME,
		GRPAT_IPADDRESS,
		GRPAT_PACKETID,

		GRPAT_NUM,
	};

	void GetGashPacket( std::string& strPacket )
	{
		strPacket.clear();

		strPacket = m_strServiceCode;
		strPacket += "\'";
		strPacket += m_strServiceRegion;
		strPacket += "\'";
		strPacket += m_strServiceAccountID;
		strPacket += "\'";
		strPacket += m_strType;
		strPacket += "\'";
		strPacket += m_strShopTime;
		strPacket += "\'";
		strPacket += m_strPoints;
		strPacket += "\'";
		strPacket += m_strIPAddress;
		strPacket += "\'";
		strPacket += m_strMemo;
		strPacket += "\'";
		strPacket += m_strPacketID;
		strPacket += "\'\r\n";

		return;
	}
};

struct KGashResponseData;

//{{ 2011.7.12 ���� : Gash ����Ʈ ����
typedef	KGashResponseData	KEGASH_GET_GASHPOINT_ACK;
typedef	KGashResponseData	KEGASH_DECREASE_GASHPOINT_ACK;
//}}


// Gash ����ߴٰ� �븸DB�� �α� ����µ� ����ϴ� ��Ŷ
DECL_PACKET( EGASH_USE_GASH_LOG_NOT )
{
	std::wstring                        m_wstrTotalOrderID;		// �ŷ���ȣ
	int                                 m_iServerGroupID;	// ���� 1:����  2:���� 
	std::wstring                        m_wstrUserID;		// ���� ID
	UidType								m_iUserUID;			// ���� UID
	std::wstring                        m_wstrNickName;		// ĳ���� �г���
	int									m_iBeforeCash;		// �Һ��� ���� ���� Gash ����Ʈ
	int									m_iAfterGash;		// �Һ��� ���� ���� Gash ����Ʈ
	int									m_iGashPoint;		// �̹� �Һ� Gash ����Ʈ
	int									m_iItemID;			// ������ ID
	int									m_iProductID;		// ���δ�Ʈ �ѹ�
	int									m_iFlag;			// �ŷ� ��� ǥ�� (1:���ż��� 2:����Ʈ ���� 3 : ���� ��Ŷ ���� 9:���Ž���)
	std::wstring						m_wstrMemo;			// ���(�ʼ��� ���� �ʿ� ����)   
	std::wstring						m_wstrUserIP;			// ����IP(�ʼ��� ���� �ʿ� ����))
};

#endif // SERV_BILLING_ADD_GASH_POINT
//}}

//{{ 2011.10.24 ������ : ��� Ƽ�� �۾� ( ����ϸ� ĳ�� �ִ� ������ )
#ifdef SERV_GOLD_TICKET
DECL_PACKET( EPUBLISHER_CHARGE_POINT_REQ )
{
	int                                 m_iServerGroupID;
	std::wstring                        m_wstrUserID;
	UidType			                    m_iCharUID;
	UidType								m_iItemUID;
	int									m_iItemID;
	std::wstring						m_wstrUserIP;
};

DECL_PACKET( EPUBLISHER_CHARGE_POINT_ACK )
{
	int                                 m_iOK;
	int                                 m_iChargedCash;
};

DECL_PACKET( ELOG_CHARGE_POINT_NOT )
{
	int                                 m_iOK;
	int                                 m_iChargedCash;	// ������ ĳ��
	int									m_iNowCash;		// ���� �� �� ĳ��
	UidType			                    m_iCharUID;
	int									m_iItemID;
	UidType								m_iItemUID;
};
#endif //SERV_GOLD_TICKET

//09. 08. 06 ������ Gash������ ���Ͽ� ��Ŷ ����ü ����

DECL_PACKET( EGASH_AUTHENTICATION_REQ )
{
	UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;
	std::string m_strServicePassword;
	std::string m_strLockFlag;
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_AUTHENTICATION
	{
		GRPAC_SERVICE_CODE,
		GRPAC_SERVICE_REGION,
		GRPAC_SERVICE_ACCOUNTID,
		GRPAC_SERVICE_ACCOUNTPASSWORD,
		GRPAC_LOCKFLAG,
		GRPAC_IPADDRESS,
		GRPAC_PACKETID,

		GRPAC_NUM,
	};

	void GetGashPacket( std::string& strPacket )
	{
		strPacket.clear();

		strPacket = m_strServiceCode;
		strPacket += "\'";
		strPacket += m_strServiceRegion;
		strPacket += "\'";
		strPacket += m_strServiceAccountID;
		strPacket += "\'";
		strPacket += m_strServicePassword;
		strPacket += "\'";
		strPacket += m_strLockFlag;
		strPacket += "\'";
		strPacket += m_strIPAddress;
		strPacket += "\'";
		strPacket += m_strPacketID;
		strPacket += "\'\r\n";

		return;
	}

};


DECL_PACKET( EGASH_AUTHORIZATION_REQ )
{
	UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;
	std::string m_strServicePassword;
	std::string m_strAuthenticationFlag;
	std::string m_strType;
	std::string m_strDuplicateCheck;
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_AUTHORIZATION
	{
		GRPAZ_SERVICE_CODE,
		GRPAZ_SERVICE_REGION,
		GRPAZ_SERVICE_ACCOUNTID,
		GRPAZ_SERVICE_ACCOUNTPASSWORD,
		GRPAZ_AUTHENTICATIONFLAG,
		GRPAZ_TYPE,
		GRPAZ_DUPLICATECHECK,
		GRPAZ_IPADDRESS,
		GRPAZ_PACKETID,

		GRPAZ_NUM,
	};

	void GetGashPacket( std::string& strPacket )
	{
		strPacket.clear();

		strPacket = m_strServiceCode;
		strPacket += "\'";
		strPacket += m_strServiceRegion;
		strPacket += "\'";
		strPacket += m_strServiceAccountID;
		strPacket += "\'";
		strPacket += m_strServicePassword;
		strPacket += "\'";
		strPacket += m_strAuthenticationFlag;
		strPacket += "\'";
		strPacket += m_strType;
		strPacket += "\'";
		strPacket += m_strDuplicateCheck;
		strPacket += "\'";
		strPacket += m_strIPAddress;
		strPacket += "\'";
		strPacket += m_strPacketID;
		strPacket += "\'\r\n";

		return;
	}
};


DECL_PACKET( EGASH_ACCOUNTING_REQ )
{
	UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;	
	std::string m_strType;
	std::string m_strLoginTime;
	std::string m_strLogoutTime;
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_ACCOUNTING
	{
		GRPAT_SERVICE_CODE,
		GRPAT_SERVICE_REGION,
		GRPAT_SERVICE_ACCOUNTID,
		GRPAT_TYPE,		
		GRPAT_LOGINTIME,		
		GRPAT_LOGOUTTIME,
		GRPAT_IPADDRESS,
		GRPAT_PACKETID,

		GRPAT_NUM,
	};

	void GetGashPacket( std::string& strPacket )
	{
		strPacket.clear();

		strPacket = m_strServiceCode;
		strPacket += "\'";
		strPacket += m_strServiceRegion;
		strPacket += "\'";
		strPacket += m_strServiceAccountID;
		strPacket += "\'";
		strPacket += m_strType;
		strPacket += "\'";
		strPacket += m_strLoginTime;
		strPacket += "\'";
		strPacket += m_strLogoutTime;
		strPacket += "\'";
		strPacket += m_strIPAddress;
		strPacket += "\'";
		strPacket += m_strPacketID;
		strPacket += "\'\r\n";

		return;
	}
};

DECL_DATA( KGashResponseData )
{
	std::string m_strResultCode;
	std::string m_strOutString;
	std::string m_strAccountID;
	std::string m_strPacketID;
	enum GASH_RESPONSE
	{
		GR_RESULT_CODE,
		GR_OUTSTRING,
		GR_PACKETID,

		GR_NUM,
	};

	bool ExtractFromGASHPacket( std::string& strPacket )
	{
		char cPacket[MAX_PACKET_SIZE_OF_GASH_AUTH];

		if( strPacket.size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
		{
			return false;
		}

		strPacket.copy(cPacket, strPacket.size());
		int iEnd = 0;
		for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
		{
			if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
			{
				break;
			}

			if( cPacket[iEnd] == ';' )
			{
				iEnd++;
				break;
			}

			m_strResultCode.push_back( cPacket[iEnd] );
		}
		//m_strResultCode.push_back( '\0' );

		for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
		{
			if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
			{
				break;
			}

			if( cPacket[iEnd] == ';' )
			{
				iEnd++;
				break;
			}

			m_strOutString.push_back( cPacket[iEnd] );
		}
		//m_strOutString.push_back( '\0' );
		for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
		{
			if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
			{
				break;
			}

			if( cPacket[iEnd] == ';' )
			{
				iEnd++;
				break;
			}

			m_strAccountID.push_back( cPacket[iEnd] );
		}
		//m_strAccountID.push_back( '\0' );

		for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
		{
			if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
			{
				break;
			}

			if( cPacket[iEnd] == ';' )
			{
				iEnd++;
				break;
			}

			m_strPacketID.push_back( cPacket[iEnd] );
		}
		//m_strPacketID.push_back( '\0' );

		return true;
	}
};

typedef KGashResponseData	KEGASH_AUTHENTICATION_ACK;
typedef	KGashResponseData	KEGASH_AUTHORIZATION_ACK;
typedef	KGashResponseData	KEGASH_ACCOUNTING_ACK;
typedef	KGashResponseData	KEGASH_GET_GASHPOINT_ACK;
typedef	KGashResponseData	KEGASH_DECREASE_GASHPOINT_ACK;

#endif // SERV_COUNTRY_TWHK
//////////////////////////////////////////////////////////////////////////

#ifdef SERV_COUNTRY_IN
DECL_PACKET( DBE_PUBLISHER_AUTHENTICATION_REQ )
{
	std::wstring					m_wstrServiceAccountID;
	std::wstring					m_wstrServicePassword;
};

DECL_PACKET( DBE_PUBLISHER_AUTHENTICATION_ACK )
{
	unsigned int					m_uiPublisherUID;
	int								m_iOK;
};
#endif SERV_COUNTRY_IN

#endif SERV_GLOBAL_AUTH
//}}

//{{ ����� : [2010/8/31/] //	��Ȱ�� ��� �и� �۾�
#ifdef SERV_SELECT_UNIT_NEW
DECL_PACKET( ELOG_UPDATE_STONE_NOT )
{
	int			m_iSupplyCnt;
	int			m_iQuantity;
};
#endif	//	SERV_SELECT_UNIT_NEW
//}} ����� : [2010/8/31/] //	��Ȱ�� ��� �и� �۾�

//{{ 2010. 11. 12  ��ȿ��	GameForge ä�� �α� ���� ó��
#ifdef SERV_RECORD_CHAT
DECL_PACKET( DCHATLOG_RECORD_CHAT_NOT )
{
	UidType								m_iAccountUID;
	UidType								m_iUnitUID;
	int									m_iServerGroupID;
	std::wstring						m_wstrChatText;
	int									m_iChatType;
};
#endif SERV_RECORD_CHAT
//}}

//{{ 2011.05.04   �ӱԼ� �ƹ�Ÿ �ռ� �ý���
#ifdef SERV_SYNTHESIS_AVATAR
DECL_PACKET( DBE_OPEN_SYNTHESIS_ITEM_REQ )
{
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
	//{{ 2009. 9. 2  ������		�к�
	std::set< int >						m_setSealCashItem;
	std::map<int, UidType>				m_mapSynthesisData;
	UidType								m_iKeyData;
	int									m_iLevel;
	//}}
};

DECL_PACKET( DBE_OPEN_SYNTHESIS_ITEM_ACK )
{
	int                                 m_iOK;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	std::map<int, UidType>				m_mapSynthesisData;
	UidType								m_iKeyData;
};

DECL_PACKET( DBE_SYNTHESIS_LOG_NOT )
{
	UidType								m_iUnitUID;
	int									m_iLevel;
	UidType								m_iKeyUID;
	std::map<int, UidType>				m_mapSynthesisData;
	std::vector< KItemInfo >			m_vecItemInfo;
	std::map< UidType, KItemInfo >		m_mapInsertedItemInfo;
};

#endif SERV_SYNTHESIS_AVATAR
//}}

//{{ 2011. 12. 09	�ڱ���		������ ����Ʈ
#ifdef SERVER_GROUP_UI_ADVANCED
DECL_PACKET( DBE_SERVERGROUP_LIST_ACK )
{
	std::map< int, KServerGroupInfo >		m_mapServerGroupList;
};
#endif SERVER_GROUP_UI_ADVANCED
//}}

#ifdef SERV_INVISIBLE_GM
DECL_PACKET( ERM_TOGGLE_INVISIBLE_NOT )
{
	UidType								m_iUnitUID;
	KFieldUserInfo						m_kFieldUserInfo;
};

DECL_PACKET( ETR_TOGGLE_INVISIBLE_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;
	KFieldUserInfo						m_kFieldUserInfo;
};
#endif SERV_INVISIBLE_GM

#ifdef	SERV_SHARING_BANK_TEST
DECL_DATA( KTradeShareItemInfo )
{
	UidType								m_iItemUID;

	UidType								m_iFromUnitUID;
	UidType								m_iToUnitUID;

	char								m_cInventoryCategory;
	short								m_sSlotID;

	u_char								m_ucSealCnt;
	u_char								m_ucDeleteReason;

	int									m_iItemID;			//	Log
	UidType								m_iNewItemUID;		//	Log

	std::wstring						m_wstrRegDate;		//	Log

	KTradeShareItemInfo()
	{
		m_iItemUID				= 0;

		m_iFromUnitUID			= 0;
		m_iToUnitUID			= 0;

		m_cInventoryCategory	= 0;
		m_sSlotID				= 0;

		m_ucSealCnt				= 0;
		m_ucDeleteReason		= 0;

		m_iItemID				= 0;
		m_iNewItemUID			= 0;

		m_wstrRegDate			= L"";
	}
};

DECL_DATA( KUpdateShareItemInfo )
{
	UidType								m_iItemUID;
	UidType								m_iUnitUID;

	int									m_iUsageType;
	int									m_iValue;

	KUpdateShareItemInfo()
	{
		m_iItemUID				= 0;
		m_iUnitUID				= 0;

		m_iUsageType			= 0;
		m_iValue				= 0;
	}
};

DECL_PACKET( DBE_UPDATE_SHARE_ITEM_REQ )
{
	UidType										m_iUnitUID;

	KItemPositionUpdate							m_mapUpdatePosition;
	std::map< UidType, KTradeShareItemInfo >	m_mapInItem;
	std::map< UidType, KTradeShareItemInfo >	m_mapOutItem;
	std::map< UidType, KUpdateShareItemInfo >	m_mapUpdateEndurance;
	std::map< UidType, KUpdateShareItemInfo >	m_mapUpdateQuantity;

	std::vector< UidType >						m_vecDeletedItem;

	bool										m_bReload;
	std::wstring								m_wstrReloadNickname;

	std::vector< KInventoryItemInfo >			m_vecInventorySlotItem;

	KDBE_UPDATE_SHARE_ITEM_REQ()
	{
		m_iUnitUID = 0;
		m_bReload = false;
	}
};

DECL_PACKET( DBE_UPDATE_SHARE_ITEM_ACK )
{
	int											m_iOK;

	bool										m_bReload;
	std::wstring								m_wstrReloadNickname;

	std::vector< KTradeShareItemInfo >			m_vecTradeShareItemResult;	//	Log

	std::vector< KInventoryItemInfo >			m_vecInventorySlotItem;

	KDBE_UPDATE_SHARE_ITEM_ACK()
	{
		m_iOK = 0;
		m_bReload = false;
	}
};

DECL_PACKET( ELOG_ITEM_TRADE_ERROR_NOT )
{
	UidType									m_iUnitUID;
	int										m_iType;

	std::wstring							m_wsrtRegDate;
};

DECL_PACKET( DBE_CHANGE_INVENTORY_SLOT_ITEM_REQ )
{
	char								m_cFromSlotType;
	int									m_iFromSlotID;
	char								m_cToSlotType;
	int									m_iToSlotID;

	UidType                             m_iUnitUID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	KItemEnduranceUpdate				m_kItemEnduranceUpdate;
	KItemPositionUpdate					m_kItemPositionUpdate;

	KDBE_CHANGE_INVENTORY_SLOT_ITEM_REQ()
	{
		m_cFromSlotType = 0;
		m_iFromSlotID = 0;
		m_cToSlotType = 0;
		m_iToSlotID = 0;
		m_iUnitUID = 0;
	}
};

DECL_PACKET( DBE_CHANGE_INVENTORY_SLOT_ITEM_ACK )
{
	char								m_cFromSlotType;
	int									m_iFromSlotID;
	char								m_cToSlotType;
	int									m_iToSlotID;

	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	KItemEnduranceUpdate				m_kItemEnduranceUpdate;
	KItemPositionUpdate					m_kItemPositionUpdate;

	KDBE_CHANGE_INVENTORY_SLOT_ITEM_ACK()
	{
		m_cFromSlotType = 0;
		m_iFromSlotID = 0;
		m_cToSlotType = 0;
		m_iToSlotID = 0;
	}
};
#endif	SERV_SHARING_BANK_TEST

//{{ ���� - ���� ���� ����Ʈ, ĳ�� �۾� 
#ifdef SERV_SHARING_BANK_QUEST_CASH
DECL_PACKET( DBE_SHARING_BACK_OPEN_REQ )
{
	enum SHARING_BANK_OPEN_TYPE{
		SBOT_QUEST = 1,
		SBOT_CASH  = 2,
	};

	UidType							m_iUserUID;
	UidType							m_iUnitUID;	
	int								m_iOpenType;
};

DECL_PACKET( DBE_SHARING_BACK_OPEN_ACK )
{
	int								m_iOK;
	int								m_iOpenType;
};
#endif SERV_SHARING_BANK_QUEST_CASH
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
DECL_PACKET( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT )
{
	UidType								m_iUserUID;
	std::vector< int >					m_vecCompletedEvent;
};
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
DECL_PACKET( DBE_CHECK_EVENT_UPDATE_ACK )
{
	std::map< int, int >	m_mapReleaseTick;
};
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

//{{ 2013. 05. 14	������	������ ����
#ifdef SERV_BLOCK_LIST
DECL_PACKET( DBE_CHECK_BLOCK_LIST_REQ )
{
	int								m_iReleaseTick;

	KDBE_CHECK_BLOCK_LIST_REQ()
	{
		m_iReleaseTick = 0;
	}
};

DECL_PACKET( DBE_CHECK_BLOCK_LIST_ACK )
{
	int											m_iReleaseTick;
	std::map< int, std::vector< KBlockInfo > >	m_mapBlockInfo;

	KDBE_CHECK_BLOCK_LIST_ACK()
	{
		m_iReleaseTick = 0;
	}
};

DECL_PACKET( EGB_CHECK_BLOCK_USER_NOT )
{
	std::vector< int >				m_vecBlockType;
	UidType							m_iUserUID;
	UidType							m_iUnitUID;
	std::wstring					m_wstrIP;

	KEGB_CHECK_BLOCK_USER_NOT()
	{
		m_iUserUID = 0;
		m_iUnitUID = 0;
	}
};

DECL_PACKET( EGB_UPDATE_BLOCK_RESULT_NOT )
{
	std::map< int, bool >			m_mapBlockResult;
};
#endif SERV_BLOCK_LIST
//}}

#ifdef SERV_ADD_WARP_BUTTON
DECL_PACKET( DBE_INSERT_WARP_VIP_REQ )
{
	UidType			m_iUnitUID;
	int				m_iPeriod;
};


DECL_PACKET( DBE_INSERT_WARP_VIP_ACK )
{
	int				m_iOK;
	std::wstring	m_wstrEndDate;
};
#endif // SERV_ADD_WARP_BUTTON


#ifdef SERV_EVENT_BUY_FAKE_ITEM
DECL_PACKET( EBILL_CHECK_BUY_FAKE_ITEM_REQ )
{
	int						m_iServerGroupID;
	UidType					m_iUserUID;
	unsigned long			m_ulProductNo;
};
DECL_PACKET( EBILL_CHECK_BUY_FAKE_ITEM_ACK )
{
	int						m_iOK;
};
#endif //SERV_EVENT_BUY_FAKE_ITEM

#ifdef SERV_READY_TO_SOSUN_EVENT
DECL_PACKET( DBE_READY_TO_SOSUN_EVENT_REQ )
{
	UidType								m_iUnitUID;
	std::map< int, int >				m_mapInsertedItem;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >			m_vecItemInfo;
	int									m_iFirstUnitClass;
};

DECL_PACKET( DBE_READY_TO_SOSUN_EVENT_ACK )
{
	int									m_iOK;
	std::map< int, int >				m_mapInsertedItem;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >		m_mapItemInfo;
	int									m_iFirstUnitClass;
};
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_MOMOTI_EVENT
DECL_PACKET( DBE_MOMOTI_QUIZ_EVENT_REQ )
{
	int									m_iOK;
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	int									m_istrReply;
};

DECL_PACKET( DBE_MOMOTI_QUIZ_EVENT_ACK )
{
	int									m_iOK;
	int									m_iCheckReward;
};
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
DECL_PACKET( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ )
{
	UidType					m_iUnitUID;
	UidType					m_iUserUID;
	int						m_iUnitLevel;
	int						m_iGetConnectExperience;
	int						m_iRewardBonusItem;

	KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ()
	{
		m_iUnitUID = 0;
		m_iUserUID = 0;
		m_iUnitLevel = 0;
		m_iGetConnectExperience = 0;
		m_iRewardBonusItem = 0;
	}
};

DECL_PACKET( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK )
{
	int						m_iOK;
	int						m_iRewardBonusItem;

	KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK()
	{
		m_iOK = 0;
		m_iRewardBonusItem = 0;
	}
};
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
DECL_PACKET( DBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT )
{
	int							m_iUDPPortSuccessType;
	int							m_iUseUdpPort;
	UidType						m_iUnitUID;
};
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG

#ifdef SERV_SKILL_POINT_CORRECTION
DECL_PACKET( DBE_SKILL_POINT_CORRECTION_INFO_NOT )
{
	int							m_iServerGroupID;
	UidType						m_iUserUID;
	UidType						m_iUnitUID;
	int                         m_iUnitClass;
	short						m_sLevel;
	int							m_iSPoint;
	int							m_iUsedSPoint;
	int							m_iCSPoint;
	int							m_iUsedCSPoint;
	int							m_iMaxCSPoint;
	int							m_iCorrectionSP;
};
#endif //SERV_SKILL_POINT_CORRECTION

#ifdef SERV_ID_NETMARBLE_PCBANG
DECL_PACKET( DBE_ID_PCBANG_CEHCK_AUTH_REQ )
{
	bool			m_bCheckClinetDirectPcBang;
	int				m_iIsGameBangType;

	std::wstring	m_wstrPublicIP;
	std::wstring	m_wstrMacAdress;

};

DECL_PACKET( DBE_ID_PCBANG_CEHCK_AUTH_ACK )
{
	bool			m_bCheckClinetDirectPcBang;
	int				m_iIsGameBangType;
	int				m_iIsNetmarbleGameBangType;

};
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_RELATIONSHIP_EVENT_INT
DECL_PACKET( DBE_EVENT_PROPOSE_USER_FIND_ACK )
{
	int						m_iOK;
	std::wstring			m_wstrOtherNickName;
	UidType					m_iOtherUserUID;
	UidType					m_iOtherUnitUID;
	char					m_cUnitClass;
	bool					m_bCouple;
};

typedef KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ KELG_EVENT_PROPOSE_CHECK_CONNECT_USER_REQ;
typedef KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK KELG_EVENT_PROPOSE_CHECK_CONNECT_USER_ACK;
typedef KELG_COUPLE_PROPOSE_NOT	KELG_EVENT_PROPOSE_NOT;
typedef KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT KERM_EVENT_PROPOSE_RESULT_FAIL_NOT;
typedef KDBE_COUPLE_MAKING_SUCCESS_REQ KDBE_EVENT_MAKING_SUCCESS_REQ;
typedef KDBE_COUPLE_MAKING_SUCCESS_ACK KDBE_EVENT_MAKING_SUCCESS_ACK;
typedef KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT KERM_EVENT_PROPOSE_RESULT_SUCCESS_NOT;

DECL_PACKET( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ )
{
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KDeletedItemInfo >		m_vecDeleted;
};

DECL_PACKET( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK )
{
	int									m_iOK;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( DBE_EVENT_DIVORCE_REQ )
{
	UidType								m_iUnitUID;
	UidType								m_iRelationTargetUserUid;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KDeletedItemInfo >		m_vecDeleted;
};

DECL_PACKET( DBE_EVENT_DIVORCE_ACK )
{
	int									m_iOK;
	UidType								m_iRelationTargetUserUid;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecUpdatedInventorySlot;

	KDBE_EVENT_DIVORCE_ACK()
	{
		m_iOK = 0;
	}
};

typedef KELG_BREAK_UP_NOT KELG_DIVORCE_NOT;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_COUNTRY_PH
DECL_PACKET( EJSON_GN_CHECK_ACCOUNT_REQ )
{
	unsigned int	m_uiGarenaUID;
	UidType			m_iSessionUID;

	KEJSON_GN_CHECK_ACCOUNT_REQ()
	{
		m_iSessionUID = 0;
	}
};

DECL_PACKET( EJSON_GN_CHECK_ACCOUNT_ACK )
{
	int				m_iResult;
	unsigned int	m_uiGarenaUID;
	unsigned int	m_uiCurrency_Balance;
	std::wstring	m_wstrCharName;
	UidType			m_iSessionUID;

	KEJSON_GN_CHECK_ACCOUNT_ACK()
	{
		m_iSessionUID = 0;
	}


};

DECL_PACKET( EJSON_GN_CHANGE_GAME_CURRENCY_REQ )
{
	unsigned int	m_uiGameCurrency_Amount;
	unsigned int	m_uiShell_Amount;
	unsigned int	m_uiGarenaUID;
	std::string		m_strGarenaTransactionID;
	UidType			m_iSessionUID;

	KEJSON_GN_CHANGE_GAME_CURRENCY_REQ()
	{
		m_iSessionUID = 0;
	}
};

DECL_PACKET( EJSON_GN_CHANGE_GAME_CURRENCY_ACK )
{
	int				m_iResult;
	unsigned int	m_uiGarenaUID;
	std::string		m_strGarenaTransactionID;
	__int64			m_int64GameTransactionID;
	unsigned int	m_uiGameCurrency_Amount;
	unsigned int	m_uiGameCurrency_Balance;
	std::string		m_strDateTime;
	unsigned int	m_uiShell_Amount;
	unsigned int	m_uiGameCurrency_BeforeAmount;  // 2013.08.06 lygan_������ // �����ƽþ� ĳ�� ��ȯ �����߿� �����ϸ� �ۺ��ſ� �ش� ���� ���� �ٽ� �˷� ��� �ϴµ� �󸶸� ��ȯ �Ҳ��� ���� �����Ѵ�. m_uiGameCurrency_Amount ���� ��� DB ������ �߻��ϸ� ��� �� �ɼ� �ִ�.
	UidType			m_iSessionUID;
	
	KEJSON_GN_CHANGE_GAME_CURRENCY_ACK()
	{
		m_iSessionUID = 0;
	}

};

DECL_PACKET( EJSON_GN_TRANSACTION_REQ )
{
	unsigned int	m_uiGarenaUID;
	std::string		m_strGarenaTransactionID;
	UidType			m_iSessionUID;
	
	KEJSON_GN_TRANSACTION_REQ()
	{
		m_iSessionUID = 0;
	}

};
DECL_PACKET( EJSON_GN_TRANSACTION_ACK )
{
	int				m_iResult;
	unsigned int	m_uiGarenaUID;
	std::string		m_strGarenaTransactionID;
	__int64			m_int64GameTransactionID;
	unsigned int	m_uiGameCurrency_Amount;
	unsigned int	m_uiShell_Amount;
	std::string		m_strDateTime;
	UidType			m_iSessionUID;

	KEJSON_GN_TRANSACTION_ACK()
	{
		m_iSessionUID = 0;
	}

};

DECL_PACKET( EBILL_GN_CHANGE_GAME_CURRENCY_REQ )
{
	KEJSON_GN_CHANGE_GAME_CURRENCY_REQ	m_kGarenaREQ;
	KPurchaserInfo						m_PurchaserInfo;
	int									m_iResult;
};

DECL_PACKET( EBILL_GN_CHANGE_GAME_CURRENCY_ACK )
{
	KEJSON_GN_CHANGE_GAME_CURRENCY_ACK	m_kGarenaACK;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	KGlobalCashInfo						m_AddCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	bool								m_bIsOffLine;
};


DECL_PACKET( EBILL_LOGIN_CHECK_BALANCE_REQ )
{
	unsigned int	m_uiPublisherUID;
	std::wstring	m_wstrCharName;
	UidType			m_iSessionUID;
	
	KEBILL_LOGIN_CHECK_BALANCE_REQ()
	{
		m_iSessionUID = 0;
	}

};


DECL_PACKET( EBILL_LOGIN_CHECK_BALANCE_ACK )
{
	unsigned int		m_uiPublisherUID;

	int                 m_iOK;
	unsigned long       m_ulBalance;	// �����ڵ� ȣȯ �� �ϳ��� ĳ�ø� ���� �������� ���� �״�ε�

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	bool				m_bOnlyType;		// �ܾ� ��ȸ�� �� �Ѱ��� Ÿ������ ���� (�Ѱ��� Ÿ���� ��� �ٸ� Type���� �ǵ��̸� �ȵ�)
	int					m_iCashType;		// �ܾ� ��ȸ�� �� �Ѱ��� Ÿ���� �� � ĳ��Ÿ������ ���θ� ���� ���
	KGlobalCashInfo     m_GlobalCashInfo;
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES
	std::wstring		m_wstrCharName;

	UidType				m_iSessionUID;

	KEBILL_LOGIN_CHECK_BALANCE_ACK()
	{
		m_ulBalance = 0;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		m_bOnlyType = false;
		m_iCashType = -1;
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES
		m_wstrCharName = L"";
		m_iSessionUID = 0;
	}
};

DECL_PACKET( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ )
{
	unsigned int	m_uiGarenaUID;
	UidType			m_iSessionUID;

	KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ()
	{
		m_iSessionUID = 0;
	}

};

DECL_PACKET( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK )
{
	int					m_iResult;
	UidType				m_iUserUID;
	std::wstring		m_wstrUserID;
	std::wstring		m_wstrUserName;
	unsigned int		m_uiPublisherUID;
	UidType				m_iSessionUID;
	
	KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK()
	{
		m_iSessionUID = 0;
	}

};


DECL_PACKET( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ )
{
	KEJSON_GN_CHANGE_GAME_CURRENCY_REQ m_kGNGameCurrencyREQ;

};

DECL_PACKET( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK )
{
	KEJSON_GN_CHANGE_GAME_CURRENCY_REQ		m_kGNGameCurrencyREQ;
	int										m_iResult;
	UidType									m_iUserUID;
	std::wstring							m_wstrUserID;
	std::wstring							m_wstrUserName;
	unsigned int							m_uiPublisherUID;


};


DECL_PACKET( EGS_EXCHANGE_CASH_NOT ) // 2013.08.06 lygan_������ // �����ƽþ��� ��� ��ȯ�Ҷ� EGS_EXCHANGE_CASH_REQ ��Ŷ�� ��������. �׷��� ACK�� ���� �� ���� ������ NOT�� ����Ѵ�.
{
	int									m_iOK;
	int									m_iPoint;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	KGlobalCashInfo						m_AddBonusCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
};

DECL_PACKET( EBILL_GARENA_PREPARE_PRESENT_CHECK_REQ ) // 2013.09.11 lygan_������ // �����ƽþ� �����ϱ� Ƚ�� ���� üũ��
{
	UidType									m_iUserUID;
	KEGS_PRESENT_CASH_ITEM_REQ				m_kEGSPresentCashItemREQ;
};

DECL_PACKET( EBILL_GARENA_PREPARE_PRESENT_CHECK_ACK ) // 2013.09.11 lygan_������ // �����ƽþ� �����ϱ� Ƚ�� ���� üũ�� �߰��� �������� ��� ���� ��� �ü� ������ ���� �̷��� ó���Ѵ�. ������ OK ���� �޾ƿ�
{
	UidType									m_iOK;
	KEGS_PRESENT_CASH_ITEM_REQ				m_kEGSPresentCashItemREQ;
};


#endif //SERV_COUNTRY_PH


//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
//#ifdef SERV_RELAY_DB_CONNECTION
DECL_PACKET( DBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT )
{
	enum UPDATE_TYPE
	{
		UT_NONE = 0,
		UT_LEVEL_UP,
		UT_UNITCLASS_CHANGE,
	};

	std::wstring						m_wstrUserID;
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	char								m_cLevel;
	char								m_cUnitClass;
	bool								m_bDeletedUnit;
	std::wstring						m_wstrUserRegDate;
	std::wstring						m_wstrUnitRegDate;
	int									m_iUpdateType;

	KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT()
	{
		m_iUserUID		= 0;
		m_iUnitUID		= 0;
		m_cLevel		= 0;
		m_cUnitClass	= 0;
		m_bDeletedUnit	= false;
		m_iUpdateType	= 0;
	}
};

DECL_PACKET( DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;

	KDBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ()
	{
		m_iUserUID = 0;
		m_iUnitUID = 0;
	}
};

DECL_PACKET( DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_ACK )
{
	std::vector< KJPEventRewardInfo >	m_vecRewardInfo;
};

DECL_PACKET( EBILL_REWARD_COUPON_JP_EVENT_REQ )
{
	KJPEventRewardInfo					m_kRewardInfo;
	KEBILL_USE_COUPON_REQ				m_kUseCouponPacket;
};

DECL_PACKET( EBILL_REWARD_COUPON_JP_EVENT_ACK )
{
	KJPEventRewardInfo					m_kRewardInfo;
	KEBILL_USE_COUPON_ACK				m_kUseCouponPacket;
};

DECL_PACKET( DBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT )
{
    __int64								m_iRID;

	KDBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT()
	{
		m_iRID = 0;
	}
};
//#endif SERV_RELAY_DB_CONNECTION
//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
DECL_PACKET( DBE_CHANGE_PET_ID_REQ )
{
	UidType							m_iPetUID;
	int								m_iBeforePetID;
	int								m_iAfterPetID;

	KDBE_CHANGE_PET_ID_REQ()
		: m_iPetUID( 0 ), m_iBeforePetID( 0 ), m_iAfterPetID( 0 )
	{
	}
};

DECL_PACKET( DBE_CHANGE_PET_ID_ACK )
{
	int								m_iOK;
	UidType							m_iPetUID;
	int								m_iBeforePetID;
	int								m_iAfterPetID;

	KDBE_CHANGE_PET_ID_ACK()
		: m_iOK( 0 ), m_iPetUID( 0 ), m_iBeforePetID( 0 ), m_iAfterPetID( 0 )
	{
	}
};

DECL_PACKET( DBE_CHANGE_PET_ID_LOG_NOT )
{
	UidType                         m_iUnitUID;
	UidType							m_iPetUID;
	int								m_iBeforePetID;
	int								m_iAfterPetID;

	KDBE_CHANGE_PET_ID_LOG_NOT()
		: m_iUnitUID( 0 ), m_iPetUID( 0 ), m_iBeforePetID( 0 ), m_iAfterPetID( 0 )
	{
	}
};
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_COUPON_EVENT
DECL_PACKET( DBE_COUPON_ENTRY_REQ )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	int									m_iCouponType;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};
	
DECL_PACKET( DBE_COUPON_ENTRY_ACK )
{
	int				m_iOK;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_COUPON_ENTRY_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( ELOG_COUPON_EVENT_NOT )
{
	UidType				m_iUserUID;
	UidType				m_iUnitUID;
	int					m_iCouponType;
};
#endif SERV_COUPON_EVENT

#ifdef SERV_GUILD_FIND_AD
DECL_PACKET( ELG_GET_GUILD_FIND_AD_LIST_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
    char								m_cSortType;
    u_int								m_uiViewPage;
};

DECL_PACKET( ELG_REGISTRATION_GUILD_INVITE_MSG_REQ )
{
	int									m_iGuildUID;
	UidType								m_iGuildMasterUnitUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrMessage;
	short								m_sPeriod;
	int									m_iCost;
};

DECL_PACKET( ELG_REGISTRATION_GUILD_INVITE_MSG_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrNickName;
	std::wstring						m_wstrMessage;
	short								m_sPeriod;
	int									m_iCost;
};

DECL_PACKET( DBE_REGISTRATION_GUILD_INVITE_MSG_REQ )
{
	int									m_iGuildUID;
	UidType								m_iGuildMasterUnitUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrMessage;
	short								m_sPeriod;
	int									m_iCost;
	bool								m_bExistExpiredGuildInviteMsg;
};

DECL_PACKET( DBE_REGISTRATION_GUILD_INVITE_MSG_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iGuildMasterUnitUID;
	UidType								m_iUnitUID;
	char								m_cUnitClass;
	int									m_iUnitLevel;
	std::wstring						m_wstrNickName;
	std::wstring						m_wstrMessage;
	std::wstring						m_wstrMsgRegDate;
	std::wstring						m_wstrMsgEndDate;
	short								m_sPeriod;
    int									m_iCost;
	bool								m_bExistExpiredGuildInviteMsg;
};

DECL_PACKET( DBE_MODIFY_REG_GUILD_INVITE_MSG_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	char								m_cUnitClass;
	int									m_iUnitLevel;
	std::wstring						m_wstrNickName;
	std::wstring						m_wstrMessage;
	short								m_sPeriod;
	int									m_iCost;
};

DECL_PACKET( ELG_REGISTRATION_GUILD_FIND_AD_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrFindAdMessage;
	short								m_sPeriod;
	int									m_iCost;
};

DECL_PACKET( ELG_REGISTRATION_GUILD_FIND_AD_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrFindAdMessage;
	short								m_sPeriod;
	int									m_iCost;
};

DECL_PACKET( DBE_REGISTRATION_GUILD_FIND_AD_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrFindAdMessage;
	short								m_sPeriod;
	int									m_iCost;
	bool								m_bExistExpiredFindAd;
};

DECL_PACKET( DBE_REGISTRATION_GUILD_FIND_AD_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	char								m_cUnitClass;
	int									m_iUnitLevel;
	std::wstring						m_wstrNickName;
	std::wstring						m_wstrFindAdMessage;
	std::wstring						m_wstrFindAdRegDate;
	std::wstring						m_wstrFindAdEndDate;
	short								m_sPeriod;
	int									m_iCost;
	bool								m_bExistExpiredFindAd;
};

DECL_PACKET( DBE_MODIFY_REG_GUILD_FIND_AD_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrFindAdMessage;
	short								m_sPeriod;
	int									m_iCost;
};

typedef KELG_REGISTRATION_GUILD_INVITE_MSG_REQ KELG_MODIFY_REG_GUILD_INVITE_MSG_REQ;
typedef KELG_REGISTRATION_GUILD_INVITE_MSG_ACK KELG_MODIFY_REG_GUILD_INVITE_MSG_ACK;
typedef KDBE_REGISTRATION_GUILD_INVITE_MSG_ACK KDBE_MODIFY_REG_GUILD_INVITE_MSG_ACK;

typedef KELG_REGISTRATION_GUILD_FIND_AD_REQ KELG_MODIFY_REG_GUILD_FIND_AD_REQ;
typedef KELG_REGISTRATION_GUILD_FIND_AD_ACK KELG_MODIFY_REG_GUILD_FIND_AD_ACK;
typedef KDBE_REGISTRATION_GUILD_FIND_AD_ACK KDBE_MODIFY_REG_GUILD_FIND_AD_ACK;

DECL_PACKET( ELG_GET_GUILD_INVITE_USER_LIST_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UINT								m_uiViewPage;
};

DECL_PACKET( ELG_GET_GUILD_INVITE_USER_LIST_ACK )
{
	int										m_iOK;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KGuildInviteMsgInfo >		m_vecGuildInviteUserList;
};

DECL_PACKET( ELG_GET_GUILD_INVITE_GUILD_LIST_REQ )
{
	UidType								m_iUnitUID;
	UINT								m_uiViewPage;
};

DECL_PACKET( ELG_GET_GUILD_INVITE_GUILD_LIST_ACK )
{
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KGuildInviteMsgInfo >		m_vecGuildInviteGuildList;
};

DECL_PACKET( ELG_ACCEPT_INVITE_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	KGuildMemberInfo					m_kLoginGuildMember;
	bool								m_bDeleteApplyJoin;
	bool								m_bDeleteFindAd;
	bool								m_bDeleteInviteMsg;
};

DECL_PACKET( ELG_ACCEPT_INVITE_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iGuildMasterUnitUID;
};

DECL_PACKET( DBE_ACCEPT_INVITE_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	KGuildMemberInfo					m_kLoginGuildMember;
};

typedef KDBE_GET_GUILD_INFO_ACK KDBE_ACCEPT_INVITE_ACK;

DECL_PACKET( ELG_CANCEL_INVITE_MSG_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iDeletedUnitUID;
	bool								m_bDeleteChar;
};

DECL_PACKET( DBE_CANCEL_INVITE_MSG_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iDeletedUnitUID;
	bool								m_bDeleteChar;
};

typedef KELG_CANCEL_INVITE_MSG_REQ KDBE_CANCEL_INVITE_MSG_REQ;

DECL_PACKET( ELG_DELETE_GUILD_FIND_AD_REQ )
{
	UidType								m_iUnitUID;
	bool								m_bDeleteChar;
};

DECL_PACKET( DBE_DELETE_GUILD_FIND_AD_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	bool								m_bDeleteChar;
};

typedef KELG_DELETE_GUILD_FIND_AD_REQ KDBE_DELETE_GUILD_FIND_AD_REQ;
typedef KELG_DELETE_GUILD_FIND_AD_REQ KELG_DELETE_INVITE_GUILD_LIST_REQ;
typedef KELG_DELETE_GUILD_FIND_AD_REQ KDBE_DELETE_INVITE_GUILD_LIST_REQ;
typedef KDBE_DELETE_GUILD_FIND_AD_ACK KDBE_DELETE_INVITE_GUILD_LIST_ACK;
#endif SERV_GUILD_FIND_AD

#ifdef SERV_DUNGEON_ROOM_USER_CHECK
DECL_PACKET( ERM_CHECK_DUNGEON_ROOM_USER_REQ )
{
	UidType				m_iRoomUID;
};
#endif SERV_DUNGEON_ROOM_USER_CHECK

#ifdef SERV_TOUR_TICKET_EVENT
DECL_PACKET( DBE_REGIST_TOUR_TICKET_NOT )
{
	UidType					m_iUnitUID;

	KDBE_REGIST_TOUR_TICKET_NOT()
	{
		m_iUnitUID = 0;
	}
};
#endif SERV_TOUR_TICKET_EVENT

#ifdef SERV_PERIOD_PET
DECL_PACKET( DBE_RELEASE_PET_REQ )
{
	UidType		m_iPetUID;
	UidType		m_iUnitUID;

	KDBE_RELEASE_PET_REQ()
		: m_iPetUID( 0 )
		, m_iUnitUID( 0 )
	{
	}
};
#endif SERV_PERIOD_PET

#ifdef SERV_RECRUIT_EVENT_BASE
DECL_PACKET( DBE_USE_RECRUIT_TICKET_REQ )
{
	UidType		m_iUnitUID;
};
typedef KPacketOK	KDBE_USE_RECRUIT_TICKET_ACK;
DECL_PACKET( DBE_REGISTER_RECRUITER_REQ )
{
	UidType			m_iUnitUID;
	std::wstring	m_wstrNickname;
};
DECL_PACKET( DBE_REGISTER_RECRUITER_ACK )
{
	int					m_iOK;
	KRecommendUserInfo	m_kRecruiterUnitInfo;
};

DECL_PACKET( ELG_RECRUIT_RECRUITER_INFO_NOT )
{
	UidType		m_iSenderUnitUID;
	UidType		m_iReceiverUnitUID;
	char		m_cType;
	int			m_iData;
};
DECL_PACKET( DBE_GET_RECRUIT_RECRUITER_LIST_REQ )
{
	UidType		m_iUnitUID;
};
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
DECL_PACKET( DBE_SET_EVENT_INFO_NOT )
{
	int			m_iQuestID;
	int			m_iUnitType;

	KDBE_SET_EVENT_INFO_NOT()
	{
		m_iQuestID = 0;
		m_iUnitType = 0;
	}
};
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_CONTENT_MANAGER_INT
DECL_PACKET( DBE_GET_CASHSHOP_ON_OFF_INFO_REQ )
{
	bool		m_bFirstCashshopOnOffCheck;
	int			m_iReleaseTick;
	bool		m_bEnableCashshop;

	KDBE_GET_CASHSHOP_ON_OFF_INFO_REQ()
	{
		m_bFirstCashshopOnOffCheck = false;
		m_iReleaseTick = -1;
		m_bEnableCashshop = true;
	}
};

DECL_PACKET( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK )
{
	int			m_iReleaseTick;
	bool		m_bEnableCashshop;
	int			m_iOK;

	KDBE_GET_CASHSHOP_ON_OFF_INFO_ACK()
	{
		m_iReleaseTick = -1;
		m_bEnableCashshop = true;
		m_iOK = -1;
	}
};
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
DECL_PACKET( DBE_EVENT_DB_SCRIPT_ACK ) // 2013.09.11 lygan_������ // �����ƽþ� �����ϱ� Ƚ�� ���� üũ�� �߰��� �������� ��� ���� ��� �ü� ������ ���� �̷��� ó���Ѵ�. ������ OK ���� �޾ƿ�
{
	std::map< int, std::string >						m_mapEventDBData;
	std::map< int, std::vector< KRewardData > >			m_mapDBRewardData;
};
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_ADD_EVENT_DB
//DECL_PACKET( DBE_GAME_SELECT_UNIT_EVENT_DATA_NOT )
//{
//	KDBE_SELECT_UNIT_ACK					m_kDBE_Select_Unit_Ack;
//	UidType									m_iUserUID;
//	
//};

DECL_PACKET( DBE_GAME_SELECT_UNIT_EVENT_DATA_NOT )
{
	UidType													m_iUserUID;
	KDBE_SELECT_UNIT_ACK									m_kSelectUnitAck;
#ifdef SERV_GLOBAL_EVENT_TABLE
	std::map< int, KGlobalEventTableData >					m_mapGlobalEventData;
#endif //SERV_GLOBAL_EVENT_TABLE

};
DECL_PACKET( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT )
{
	UidType													m_iUserUID;
	KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK				m_kSelectUnitAck;
};
#endif //SERV_ADD_EVENT_DB

#ifdef SERV_NEW_YEAR_EVENT_2014
DECL_PACKET( DBE_2013_EVENT_MISSION_COMPLETE_REQ )
{
	UidType		m_iUnitUID;
	u_char		m_ucLevel;
};
DECL_PACKET( DBE_2013_EVENT_MISSION_COMPLETE_ACK )
{
	int			m_iOK;
	u_char		m_ucLevel;
};
DECL_PACKET( DBE_2014_EVENT_MISSION_COMPLETE_REQ )
{
	UidType		m_iUserUID;
	UidType		m_iUnitUID;
	int			m_iNewYearMissionStepID;
};
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_USE_GM_TOOL_INFO
DECL_PACKET( DBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT )
{
	std::map< int, KItemName >			m_CurrentItemTempletNameMap;
};
#endif //SERV_USE_GM_TOOL_INFO

//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
DECL_PACKET( EGB_GET_GLOBAL_MISSION_INFO_REQ )
{
	UidType										m_iServerUID;
};

DECL_PACKET( EGB_GET_GLOBAL_MISSION_INFO_ACK )
{
	enum FLAG_TYPE
	{
		FT_NONE = 0,
		FT_INIT,
		FT_UPDATE,
		FT_BEGIN,

		FT_SUCCESS,
		FT_FAIL,
	};

	int												m_iFlag;
	std::wstring									m_wstrGlobalMissionDay;
	std::map< std::wstring, KGlobalMissionInfo >	m_mapGlobalMissionInfo;
};

typedef KEGB_GET_GLOBAL_MISSION_INFO_ACK		KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT;

DECL_PACKET( DBE_GET_GLOBAL_MISSION_INFO_REQ )
{
	std::map< std::wstring, KGlobalMissionInfo >	m_mapGlobalMissionInfo;
};

typedef KDBE_GET_GLOBAL_MISSION_INFO_REQ		KDBE_GET_GLOBAL_MISSION_INFO_ACK;

DECL_PACKET( DBE_UPDATE_GLOBAL_MISSION_INFO_REQ )
{
	std::wstring								m_wstrGlobalMissionDay;
	int											m_iNowCount;
};

typedef KDBE_UPDATE_GLOBAL_MISSION_INFO_REQ		KEGB_GLOBAL_MISSION_UPDATE_NOT;


DECL_PACKET( EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT )
{
	std::map< int, std::wstring >				m_mapGlobalMissionStartTime;
};

DECL_PACKET( DBE_INSERT_CONNECT_EVENT_REQ )
{
	std::wstring								m_wstrEventName;
	int											m_iEventType;
	std::wstring								m_wstrNextDayStartTime;
	std::wstring								m_wstrNextDayEndTime;
	int											m_iScriptID;
};

DECL_PACKET( DBE_REGIST_GLOBAL_MISSION_CLEAR_NOT )
{
	std::wstring								m_wstrGlobalMissionDay;
	UidType										m_iUnitUID;
};
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
DECL_PACKET( DBE_UNLIMITED_SECOND_CHANGE_JOB_NOT )
{
	int											m_iUnitUID;
	int											m_iOldUnitClass;
	int											m_iNewUnitClass;
};
#endif	//	SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_EVENT_CHECK_POWER
DECL_PACKET( DBE_START_CHECK_POWER_REQ )
{
	UidType								m_iUnitUID;
	unsigned char						m_ucCheckPowerCount;
	__int64								m_iCheckPowerTime;
	unsigned char						m_ucCheckPowerScore;
};
typedef KPacketOK KDBE_START_CHECK_POWER_ACK;
typedef KPacketOK KDBE_UPDATE_CHECK_POWER_ACK;
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_SHARING_BANK_EVENT
DECL_PACKET( DBE_SHARING_BANK_EVENT_REQ )
{
	UidType							m_iUserUID;
	UidType							m_iUnitUID;	
	int								m_iItemID;
};
#endif

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
DECL_PACKET( DBE_GET_ITEM_ONOFF_NPCSHOP_REQ )
{

	std::map<int , int> m_mapTimeControlItem_StaticDBReleaseTick;
};

DECL_PACKET( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK )
{

	std::map<int , int> m_mapTimeControlItem_StaticDBReleaseTick;
	std::map<int , std::vector<KPacketGetItemOnOff> > m_mapGetItemOnOff;
};

#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
DECL_PACKET( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ )
{
	std::map<int , int> m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick;
};

DECL_PACKET( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK )
{
	std::map<int , int> m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick;
	std::map<int , std::vector<KRandomItemMappingToInfoServer> > m_mapGetCubeInItemMappingOnOff;
};

#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

#ifdef SERV_EVENT_PET_INVENTORY
DECL_PACKET( DBE_EVENT_PET_EVENT_FOOD_EAT_REQ )
{
	int							m_iUnitUID;
	std::wstring				m_wstrPetName;
	UidType						m_iPetUID;
	int							m_iPetID;
	bool						m_bEventFoodEat;      // �̺�Ʈ ������ ��� ���� 
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};
DECL_PACKET( DBE_EVENT_PET_EVENT_FOOD_EAT_ACK )
{
	int							m_iUnitUID;
	std::wstring				m_wstrPetName;
	UidType						m_iPetUID;
	int							m_iPetID;
	bool						m_bEventFoodEat;      // �̺�Ʈ ������ ��� ���� 
	int							m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
DECL_PACKET( DBE_EVENT_CHUNG_GIVE_ITEM_REQ )
{
	std::wstring				m_wstrGiveItemTime_One;
	std::wstring				m_wstrGiveItemTime_Two;
	std::wstring				m_wstrGiveItemTime_Tree;
	UidType						m_iUnitUID;
	bool						m_bTwoGiveItem;
	int							m_iChoice;
	KDBE_EVENT_CHUNG_GIVE_ITEM_REQ()
	{
		m_wstrGiveItemTime_One = L"1900-01-01 00:00:00";
		m_wstrGiveItemTime_Two = L"1900-01-01 00:00:00";
		m_wstrGiveItemTime_Tree = L"1900-01-01 00:00:00";
		m_bTwoGiveItem = false;
		m_iChoice	= 0;
	}
};
DECL_PACKET( DBE_EVENT_CHUNG_GIVE_ITEM_ACK )
{
	int							m_iOK;
	bool						m_bTwoGiveItem;
	int							m_iChoice;
	KDBE_EVENT_CHUNG_GIVE_ITEM_ACK()
	{
		m_iOK	= 0;
		m_bTwoGiveItem = false;
		m_iChoice = 0;
	}
};
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
DECL_PACKET( DBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ )
{
	UidType                         m_iUnitUID;
	std::wstring					m_wstrButtonClickTime_One; //1���� ��ư Ŭ�� �ð� ���� Ÿ��
	bool							m_bItemGive; //2���� ��ư Ŭ�� �ð� ���� Ÿ��
	int								m_iDungeonClearCount;
	int								m_iFieldMonsterKillCount;
	bool							m_WeekEndItem;
	int								m_Nowday;
	bool							m_bStartButton;
	KDBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ()
	{
		m_iUnitUID = 0;
		m_wstrButtonClickTime_One = L"1900-01-01 00:00:00";
		m_bItemGive = false;
		m_iDungeonClearCount = 0;
		m_iFieldMonsterKillCount = 0;
		m_WeekEndItem = false;
		m_Nowday = 0;
		m_bStartButton = false;
	}
};
DECL_PACKET( DBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK )
{
	int								m_iOk;
	bool							m_bWeekEndItem;
	int								m_iDungeonClearCount;
	int								m_iFieldMonsterKillCount;
	int								m_NowDay;
	std::wstring					m_wstrButtonClickTime_One; //��ư Ŭ�� �ð�
	KDBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK()
	{
		m_iOk = 0;
		m_bWeekEndItem = false;
		m_iDungeonClearCount = 0;
		m_iFieldMonsterKillCount = 0;
		m_NowDay	= 0;
		m_wstrButtonClickTime_One = L"1900-01-01 00:00:00";
	}
};
DECL_PACKET( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT )
{
	UidType                         m_iUnitUID;
	std::wstring					m_wstrButtonClickTime_One; 
	bool							m_bItemGive; 
	int								m_iDungeonClearCount;
	int								m_iFieldMonsterKillCount;
	KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT()
	{
		m_iUnitUID = 0;
		m_wstrButtonClickTime_One = L"1900-01-01 00:00:00";
		m_bItemGive = false;
		m_iDungeonClearCount = 0;
		m_iFieldMonsterKillCount = 0;
	}
};
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
DECL_PACKET( DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ )
{
	UidType                 m_iUnitUID;
	int						m_iValenTineItemCount;	
	KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ()
	{
		m_iUnitUID			  = 0;
		m_iValenTineItemCount = -1;
	}
};
DECL_PACKET( DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK )
{
	int						m_iOk;
	int						m_iValenTineItemCount;	
	KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK()
	{
		m_iOk = 0;
		m_iValenTineItemCount = -1;
	}
};
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_GLOBAL_EVENT_TABLE
DECL_PACKET( DBE_GLOBAL_EVENT_TABLE_INSERT_NOT )
{
	std::map< int, KGlobalEventTableData >		m_mapGlobalEventData;
};
#endif //SERV_GLOBAL_EVENT_TABLE

#ifdef SERV_COUNTRY_IN
DECL_DATA( KNaunGetCashRequestData )
{
	std::wstring m_wstrUserUID;			// UserUID		[40]
	std::wstring m_wstrUserID;			// ������		[40]
	std::wstring m_wstrCompanyCode;		// ȸ�� �ڵ�	[4]
	std::wstring m_wstrNickName;		// ĳ���͸�		[30]
	unsigned long m_ulShopBalance;		// ���θ� ĳ�� �ܾ�
	unsigned long m_ulContentsBalance;	// ������ ĳ�� �ܾ�(�����������)
	unsigned long m_ulBonusBalance;		// ���ʽ� ĳ�� �ܾ�(�����������)
	unsigned long m_ulEtcBalance;		// ��Ÿ ĳ�� �ܾ�(�����������)
	unsigned long m_ulShopMileage;		// ���θ� ���ϸ��� �ܾ�(�����������)
	unsigned long m_ulContentsMileage;	// ������ ���ϸ��� �ܾ�(�����������)
	std::wstring m_wstrReturnCode;		// ��ȯ��		[4]

	KNaunGetCashRequestData()
	{
		m_wstrUserUID = L"";
		m_wstrUserID = L"";
		m_wstrCompanyCode = L"";
		m_wstrNickName = L"";
		m_ulShopBalance = 0;
		m_ulContentsBalance = 0;
		m_ulBonusBalance = 0;
		m_ulEtcBalance = 0;
		m_ulShopMileage = 0;
		m_ulContentsMileage = 0;
		m_wstrReturnCode = L"";
	}
};

typedef KNaunGetCashRequestData KENAUN_BILLING_BALANCE_REQ;
typedef KNaunGetCashRequestData KENAUN_BILLING_BALANCE_ACK;

DECL_DATA( KNaunBuyProductRequestData )
{
	std::wstring m_wstrUserUID;			// UserUID		[40]
	std::wstring m_wstrUserID;			// ������		[40]
	std::wstring m_wstrCompanyCode;		// ȸ�� �ڵ�	[4]
	std::wstring m_wstrNickName;		// ĳ���͸�		[30]
	std::wstring m_wstrIP;				// IP			[15]
	unsigned long m_ulBuyType;			// ���� ���(��ǰ ����, �ϰ� ����)
	std::wstring m_wstrProductName;		// ��ǰ��		[50]
	std::wstring m_wstrProductNo;		// ��ǰ ���̵�	[20]
	std::wstring m_wstrCategory;		// ��ǰ ī�װ�[20]
	unsigned long m_ulProductCnt;		// ���� ����
	std::wstring m_wstrProductEtc;		// ���� ��ǰ ���� �߰� ��� ����(�Ⱓ ��)	[20]
	std::wstring m_wstrToUserUID;		// �����޴� UserUID	[40]
	std::wstring m_wstrToUserID;		// �����޴� ������	[40]
	unsigned long m_ulPrice;			// ��ǰ �ݾ�
	std::wstring m_wstrAgencyNo;		// ������ �ֹ���ȣ(������� ����)	[20]
	std::wstring m_wstrEtc2;			// ��Ÿ2		[100]
	std::wstring m_wstrEtc3;			// ��Ÿ3		[100]
	std::wstring m_wstrEtc4;			// ��Ÿ4		[100]
	std::wstring m_wstrOrderNo;			// �ֹ���ȣ		[20]
	std::wstring m_wstrReturnCode;		// ��ȯ��		[4]

	KNaunBuyProductRequestData()
	{
        m_wstrUserUID = L"";
		m_wstrUserID = L"";
		m_wstrCompanyCode = L"";
		m_wstrNickName = L"";
		m_wstrIP = L"";
		m_ulBuyType = 0;
		m_wstrProductName = L"";
		m_wstrProductNo = L"";
		m_wstrCategory = L"";
		m_ulProductCnt = 0;
		m_wstrProductEtc = L"";
		m_wstrToUserUID = L"";
		m_wstrToUserID = L"";
		m_ulPrice = 0;
		m_wstrAgencyNo = L"";
		m_wstrEtc2 = L"";
		m_wstrEtc3 = L"";
		m_wstrEtc4 = L"";
		m_wstrOrderNo = L"";
		m_wstrReturnCode = L"";
	}
};

typedef KNaunBuyProductRequestData KENAUN_BUY_PRODUCT_REQ;
typedef KNaunBuyProductRequestData KENAUN_BUY_PRODUCT_ACK;
typedef KNaunBuyProductRequestData KENAUN_GIFT_ITEM_REQ;
#endif SERV_COUNTRY_IN

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
DECL_PACKET( ELG_CASH_DIRECT_CHARGE_CN_REQ )
{
	UidType				m_iUserUID;
	std::wstring		m_wstrServiceAccountID;
	UidType				m_iUnitUID;
	std::wstring		m_wstrUnitNickName;
	std::wstring		m_wstrIP;
};
/*EGS_CASH_DIRECT_CHARGE_CN_ACK �� ���� ��Ŷ�̶� ������
DECL_PACKET( ELG_CASH_DIRECT_CHARGE_CN_ACK )
{
	int					m_iOK;
	std::wstring		m_wstrToken;
};
*/

DECL_PACKET( EGIANT_AUTH_DIRECT_CHARGE_REQ )
{
	std::wstring				m_wstrServiceAccountID;	// ���� ����
	unsigned int				m_uiUserUID;				// ���� ����
	
	unsigned long				m_ulGameZone;			// ��ü ���Ӽ��� ���� �ڵ�

	unsigned int				m_uiUnitUID;				// ĳ���� �ڵ�
	std::wstring				m_wstrUnitNickName;		// ĳ���� ��

	std::wstring				m_wstrIP;				// Ŭ�� IP

	//unsigned short				m_usTokenLen;			// token ����
	//char						m_cToken;				// token
	

	KEGIANT_AUTH_DIRECT_CHARGE_REQ()
	{
		m_wstrServiceAccountID	= L"";
		m_uiUserUID				= 0;
		m_ulGameZone			= 0;
		m_uiUnitUID				= 0;
		m_wstrUnitNickName		= L"";
		m_wstrIP				= L"";
		//m_usTokenLen			= 0;
		//m_cToken				= 0;

	}
};

DECL_PACKET( EGIANT_AUTH_DIRECT_CHARGE_ACK )
{
	int							m_iOK;
	std::wstring				m_wstrServiceAccountID;	// ���� ����
	unsigned int				m_uiUserUID;				// ���� ����

	unsigned long				m_ulGameZone;			// ��ü ���Ӽ��� ���� �ڵ�

	unsigned int				m_uiUnitUID;				// ĳ���� �ڵ�
	std::wstring				m_wstrUnitNickName;		// ĳ���� ��

	std::wstring				m_wstrIP;				// Ŭ�� IP

	unsigned short				m_usTokenLen;			// token ����
	std::wstring				m_wstrToken;			// token
};
#endif //SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_STRING_FILTER_USING_DB
typedef KDBE_CHECK_EVENT_UPDATE_ACK KDBE_CHECK_STRING_FILTER_UPDATE_ACK;
DECL_PACKET( DBE_STRING_FILTER_UPDATE_ACK )
{
	std::vector< KStringFilterInfo >	m_vecStringFilterList;
};
#endif //SERV_STRING_FILTER_USING_DB

#pragma pack( pop )
