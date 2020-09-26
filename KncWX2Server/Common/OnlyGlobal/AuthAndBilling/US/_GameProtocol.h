#ifndef __GAME_PROTOCOL_H__
#define __GAME_PROTOCOL_H__

/**--------------------------------------------------------------
FileName        : _GameProtocol.h
Description     : 
	�������� ���Ӽ����� ����� ���Ӿ����� ���� ���������� �����Ѵ�.
	�Ʒ� ���ǵ� Packet Ÿ�԰� Structure�� �ش� ���ӻ�� ����� ����
	�Ͽ� �� ���Ӻ��� �����Ͽ��� �Ѵ�.
Caution         : 
	������������ ���ǽÿ� Structure Header�� �ش��ϴ� �κп� ��Ŷ����
	��ŶŸ��, �����ڵ带 ������� �����ϰ� short Ÿ������ �����Ѵ�.
	��Ŷ���� : short   ReqLen;		
	��ŶŸ�� : short	ReqType;	
	�����ڵ� : short	RetCode;

Copyright 2001-2008 by PayLetter Inc. All rights reserved.
Author          : soohyunk@payletter.com, 2008-06-25
Modify History  : 
---------------------------------------------------------------*/

// ���Ӽ���/����GTX���� ��ȣ ��ɾ� ����
// Packet Type ����
#define GTX_PK_TYPE_GETBALANCE					10	// �ܾ���ȸ ��û/����
#define GTX_PK_TYPE_INSPOLICYEVENT				12	// ��å�̺�Ʈĳ�� ���� ��û/����
#define GTX_PK_TYPE_PURCHASEITEM				20	// �����۱��� 
#define GTX_PK_TYPE_GIFTITEM					21	// �����ۼ��� 
#define GTX_PK_TYPE_CNLPURCHASE					22	// �����۱��� ���
#define GTX_PK_TYPE_REGCOUPON					30	// ���� ���
#define GTX_PK_TYPE_CNLCOUPON					31	// ���� ��� ���
#define GTX_PK_TYPE_HEALTH_CHECK				40	// GTX Health Check

// GTX ��������
#pragma pack(1)

// Health Check ����ü
typedef struct _GTX_PK_HEALTH_CHECK
{
	WORD	ReqLen;		// IN	1
	WORD	ReqType;	// IN	2
	DWORD	ReqKey;		// IN	3

	WORD	RetCode;	// OUT	4
} GTX_PK_HEALTH_CHECK, *PGTX_PK_HEALTH_CHECK;

// �ܾ���ȸ ����ü
typedef struct _GTX_PK_GETBALANCE
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char    UserID[50+1];		// IN   6
	
	DWORD	RealCash;			// OUT	7
	DWORD	BonusCash;			// OUT	8
	char	RetMsg[256+1];		// OUT  9
} GTX_PK_GETBALANCE, *PGTX_PK_GETBALANCE;

// ��å�̺�Ʈĳ�� ���� ����ü
typedef struct _GTX_PK_INSPOLICYEVENT
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char	UserID[50+1];		// IN   6
	DWORD	ClientIP;			// IN	7
    DWORD	CashAmt;			// IN	8
	DWORD	EventID;			// IN   9
	char	ExpireYMD[8+1];		// IN	10
	
	DWORD	RealCash;			// OUT	11
	DWORD	BonusCash;			// OUT	12
	DWORD	RetCashAmt;			// OUT	13
	char	RetExpireYMD[8+1];	// OUT	14
	char	RetMsg[256+1];		// OUT  15
} GTX_PK_INSPOLICYEVENT, *PGTX_PK_INSPOLICYEVENT;

// ������ ���� ����ü
typedef struct _GTX_PK_PURCHASEITEM
{
	WORD	ReqLen;					// IN	1
	WORD	ReqType;				// IN	2
	DWORD	ReqKey;					// IN	3
	WORD	RetCode;				// OUT	4

    DWORD	UserNo;					// IN	5
	DWORD	ClientIP;				// IN	6
	char	ItemID[256+1];			// IN	7
	char	ItemCnt[128+1];			// IN	8
	char	ItemUnitPrice[256+1];	// IN	9
	WORD	GameServerNo;			// IN	10
	DWORD	WorldNo;				// IN	11
	char	UserID[50+1];			// IN	12
    char    CharacterID[50+1];		// IN	13
    char    StatProperty1[50+1];	// IN	14
    char    StatProperty2[50+1];	// IN	15
    char    StatProperty3[50+1];	// IN	16

	DWORD	RealCash;				// OUT	17
	DWORD	BonusCash;				// OUT	18
	DWORD	ChargedCashAmt;			// OUT	19
	char	ChargeNo[512+1];		// OUT	20
	char	EventItemID[256+1];		// OUT  21
	char	EventChargeNo[512+1];	// OUT  22
	char	RetMsg[256+1];			// OUT  23
} GTX_PK_PURCHASEITEM, *PGTX_PK_PURCHASEITEM;

// ������ ���� ����ü
typedef struct _GTX_PK_GIFTITEM
{
	WORD	ReqLen;					// IN	1
	WORD	ReqType;				// IN	2
	DWORD	ReqKey;					// IN	3
	WORD	RetCode;				// OUT	4

    DWORD	UserNo;					// IN	5
	DWORD	RUserNo;				// IN	6
	DWORD	ClientIP;				// IN	7
	char	ItemID[256+1];			// IN	8
	char	ItemCnt[128+1];			// IN	9
	char	ItemUnitPrice[256+1];	// IN	10
	WORD	GameServerNo;			// IN	11
	DWORD	WorldNo;				// IN	12
	char	UserID[50+1];			// IN	13
    char    CharacterID[50+1];		// IN	14
	char    RUserID[50+1];			// IN	15
	char    RCharacterID[50+1];		// IN	16
    char    StatProperty1[50+1];	// IN	17
    char    StatProperty2[50+1];	// IN	18
    char    StatProperty3[50+1];	// IN	19

	DWORD	RealCash;				// OUT	20
	DWORD	BonusCash;				// OUT	21
	DWORD	ChargedCashAmt;			// OUT	22
	char	ChargeNo[512+1];		// OUT	23
	char	EventItemID[256+1];		// OUT  24
	char	EventChargeNo[512+1];	// OUT  25
	char    RetMsg[256+1];			// OUT  26
} GTX_PK_GIFTITEM, *PGTX_PK_GIFTITEM;

// ������� ����ü
typedef struct _GTX_PK_CNLPURCHASE
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char	UserID[50+1];		// IN	6
	char	ChargeNo[512+1];	// IN	7

	DWORD	RealCash;			// OUT	8
	DWORD	BonusCash;			// OUT	9
	DWORD	CanceledCashAmt;	// OUT	10
	char	RetMsg[256+1];		// OUT  11
} GTX_PK_CNLPURCHASE, *PGTX_PK_CNLPURCHASE;

// ������� ����ü
typedef struct _GTX_PK_REGCOUPON
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	DWORD	ClientIP;			// IN	6
	WORD	GameServerNo;		// IN	7
	DWORD	WorldNo;			// IN	8
	char	UserID[50+1];		// IN	9
    char    CharacterID[50+1];	// IN	10
	char	CouponNo[19+1];		// IN	11
	char    StatProperty1[50+1];// IN	12
    char    StatProperty2[50+1];// IN	13
    char    StatProperty3[50+1];// IN	14

	DWORD	RealCash;			// OUT	15
	DWORD	BonusCash;			// OUT	16
	char	ItemID[256+1];		// OUT	17
	char	ItemCnt[128+1];			// OUT	18
	char	ChargeNo[512+1];		// OUT	19
	char	RetMsg[256+1];		// OUT	20
} GTX_PK_REGCOUPON, *PGTX_PK_REGCOUPON;

// ���� ��� ��� ����ü
typedef struct _GTX_PK_CNLCOUPON
{
	WORD	ReqLen;				// IN	1
	WORD	ReqType;			// IN	2
	DWORD	ReqKey;				// IN	3
	WORD	RetCode;			// OUT	4

    DWORD	UserNo;				// IN	5
	char	UserID[50+1];		// IN   6
	char	CouponNo[19+1];		// IN	7

	char	RetMsg[256+1];		// OUT	8
} GTX_PK_CNLCOUPON, *PGTX_PK_CNLCOUPON;

#define GTX_PK_HEALTH_CHECK_SZ				sizeof(GTX_PK_HEALTH_CHECK)
#define GTX_PK_GETBALANCE_SZ				sizeof(GTX_PK_GETBALANCE)
#define GTX_PK_INSPOLICYEVENT_SZ			sizeof(GTX_PK_INSPOLICYEVENT)
#define GTX_PK_PURCHASEITEM_SZ				sizeof(GTX_PK_PURCHASEITEM)
#define GTX_PK_GIFTITEM_SZ					sizeof(GTX_PK_GIFTITEM)
#define GTX_PK_CNLPURCHASE_SZ				sizeof(GTX_PK_CNLPURCHASE)
#define GTX_PK_REGCOUPON_SZ					sizeof(GTX_PK_REGCOUPON)
#define GTX_PK_CNLCOUPON_SZ					sizeof(GTX_PK_CNLCOUPON)

#endif
