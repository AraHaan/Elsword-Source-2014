#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"
#include "GiantCommonPacket.h"

#ifdef SERV_COUNTRY_CN

SmartPointer( KGiantAuthPacket );

class KGiantAuthPacket : public KGiantCommonPacket
{
public:
	enum ENUM_AUTH_COMMAND_TYPE { 
		GAP_ACT_LOGIN					= 2,
		GAP_ACT_SESSION					= 3,// SERV_DIRECT_CHARGE_ELSWORD_CASH
	};

	enum ENUM_AUTH_PARACOMMAND_TYPE { 
		GA_PCT_LOGIN_REQ				= 1,
		GA_PCT_LOGIN_FAIL				= 2,
		GA_PCT_LOGIN_SUCCESS			= 3,
		GA_PCT_LOGIN_MTCARD				= 4,
	};
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	enum ENUM_AUTH_SESSION_PARACOMMAND_TYPE {
		GAS_PCT_SESSION_GETTOKEN		= 3,
	};
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

	enum ENUM_RETURN_CODE_TYPE {
		GAP_RCT_SYSTEM_ERROR			= -4,	// �ý��� ����
		GAP_RCT_ACCOUNT_NOT_EXIST		= -5,	// ���� ����
		GAP_RCT_ACCOUNT_PASSWORD_ERROR	= -6,	// ���� �н����� ����
		GAP_RCT_OTP_PASSWORD_ERROR		= -7,	// OTP ��ȣ ����
		GAP_RCT_BINDING_MATRIX_CARD		= -9,	// ��Ʈ���� ī�� ���ε�
		GAP_RCT_ACCOUNT_BLOCK			= -10,	// ���� ����
		GAP_RCT_MATRIX_NUMBER_ERROR		= -11,	// ���� ī�� ��ȣ ����
		GAP_RCT_ACCOUNT_NOT_ACTIVATE	= -13,	// ���� �� Ȱ��ȭ
	};

	enum ENUM_ACCOUNT_STATE_TYPE {

	};

	enum ENUM_ACCOUNT_TYPE {

	};

public:
	KGiantAuthPacket();
	virtual ~KGiantAuthPacket();

	// ��ӿ� ���� �����ε� ������
	using KGiantCommonPacket::Read;
	using KGiantCommonPacket::Write;

	virtual bool Read( KEPUBLISHER_AUTHENTICATION_REQ& kPacket );
	virtual bool Write( const KEPUBLISHER_AUTHENTICATION_REQ& kPacket );

	virtual bool Read( KEGIANT_AUTH_LOGIN_SUCCESS& kPacket );
	virtual bool Write( const KEGIANT_AUTH_LOGIN_SUCCESS& kPacket );

	virtual bool Read( KEGIANT_AUTH_LOGIN_FAIL& kPacket );
	virtual bool Write( const KEGIANT_AUTH_LOGIN_FAIL& kPacket );

	virtual bool Read( KEPUBLISHER_SECURITY_AUTH_REQ& kPacket );
	virtual bool Write( const KEPUBLISHER_SECURITY_AUTH_REQ& kPacket );

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	virtual bool Read( KEGIANT_AUTH_DIRECT_CHARGE_REQ& kPacket );
	virtual bool Write( const KEGIANT_AUTH_DIRECT_CHARGE_REQ& kPacket );

	virtual bool Read( KEGIANT_AUTH_DIRECT_CHARGE_ACK& kPacket );
	virtual bool Write( const KEGIANT_AUTH_DIRECT_CHARGE_ACK& kPacket );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
};

#endif // SERV_COUNTRY_CN