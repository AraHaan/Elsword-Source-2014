#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"
#include "GiantCommonPacket.h"

#ifdef SERV_COUNTRY_CN

SmartPointer( KGiantBillingPacket );

class KGiantBillingPacket : public KGiantCommonPacket
{
public:
	enum ENUM_BILLING_COMMAND_TYPE 
	{ 
		GB_BCT_CONSUME						= 2,
		GB_BCT_BALANCE						= 4,
		GB_BCT_RETURN						= 5,
		GB_BCT_CARD							= 6,
		//GB_BCT_USE_CARD					= 9,	// ���� ī�� ��� (������)
	};

	enum ENUM_BILLING_PARACOMMAND_TYPE 
	{ 
		GB_PCT_USE_CARD_REQ					= 3,
		GB_PCT_USE_CARD_ACK					= 4,
		GB_PCT_PCARD						= 6,
		GB_PCT_SCARD						= 7,
	};

// KEBILL_USE_COUPON_REQ ����ü�� �ش� enum �� �̵� ��Ŵ
// 	enum ENUM_RETURN_CODE_TYPE 
// 	{
// 		GB_RCT_SUCCESS						= 0,	// ����
// 		GB_RCT_FAIL							= -1,	// ����
// 		GB_RCT_ACCOUNT_NOT_EXIST			= -2,	// ���� ����
// 		GB_RCT_CASH_NOT_EXIST				= -3,	// �ܾ� ����		
// 		GB_RCT_ITEM_CARD_NOT_EXIST			= -4,	// ������ ī�� ����
// 		GB_RCT_ITEM_CARD_NOT_AREA			= -5,	// �ش� ������ ī��� �ش� �������� ��� �Ұ�
// 		GB_RCT_ITEM_CARD_DUPLICATE			= -6,	// ��Ʈ���� ī�� ���ε�
// 		GB_RCT_ZONE_CARD_ID_ERROR			= -7,	// ���� ī�� ID����
// 		GB_RCT_ZONE_CARD_PASSWORD_ERROR		= -8,	// ���� ī�� �н����� ����
// 		GB_RCT_DB_ERROR						= -9,	// DB����
// 	};

	enum ENUM_REQUEST_TYPE 
	{
		GB_RT_RECHARGE						= 0,	// ���� ��û
		GB_RT_EXCHANGE_POINT				= 1,	// ����Ʈ ��ȯ ��û
		//GB_RT_USE_ZONE_CARD				= 2,	// ���� ī�� ��� ��û
		GB_RT_BALLANCE						= 3,	// �ܰ� üũ ��û
		//GB_RT_RECHARGE_MONTH_CARD			= 4,	// ���� ī�� ���� ��û
		GB_RT_USE_ITEM_CARD					= 5,	// ������ ī�� ��� ��û
		//GB_RT_EXCHANGE_POINT_FUND			= 6,	// ??
		//GB_RT_EXCHANGE_POINT_INSURANCE	= 7,	// ??
		//GB_RT_EXCHANGE_POINT_SECOND		= 8,	// ??
		//GB_RT_EXCHANGE_POINT_MONTH		= 9,	// ??
	};


public:
	KGiantBillingPacket();
	virtual ~KGiantBillingPacket();

	// ��ӿ� ���� �����ε� ������
	using KGiantCommonPacket::Read;
	using KGiantCommonPacket::Write;

	//{{ 2011. 02. 28	������	�߱� ���̾�Ʈ ����
	//virtual bool Read( KEGIANT_BILLING_USE_CARD_CMD& kPacket );
	//virtual bool Write( const KEGIANT_BILLING_USE_CARD_CMD& kPacket );

	//virtual bool Read( KEGIANT_BILLING_USE_CARD_RET_CMD& kPacket );
	//virtual bool Write( const KEGIANT_BILLING_USE_CARD_RET_CMD& kPacket );

	virtual bool Read( KEPUBLISHER_BILLING_BALANCE_REQ& kPacket );
	virtual bool Write( const KEPUBLISHER_BILLING_BALANCE_REQ& kPacket );	

	virtual bool Read( KEGIANT_BILLING_CONSUME_REQ& kPacket );
	virtual bool Write( const KEGIANT_BILLING_CONSUME_REQ& kPacket );

	virtual bool Read( KEGIANT_BILLING_PCARD_CMD& kPacket );
	virtual bool Write( const KEGIANT_BILLING_PCARD_CMD& kPacket );

	virtual bool Read( KEGIANT_BILLING_SCARD_CMD& kPacket );
	virtual bool Write( const KEGIANT_BILLING_SCARD_CMD& kPacket );

	virtual bool Read( KEGIANT_BILLING_RETURN& kPacket );
	virtual bool Write( const KEGIANT_BILLING_RETURN& kPacket );
	//}}
};


#endif // SERV_COUNTRY_CN