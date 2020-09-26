#ifndef __GAME_PROTOCOL_H__
#define __GAME_PROTOCOL_H__


#define GTX_PK_TYPE_GETBALANCE					10	// �ܾ���ȸ ��û/����
#define GTX_PK_TYPE_PURCHASEITEM				20	// �����۱��� 

// GTX ��������
#pragma pack( push , 1)

// �ܾ���ȸ ����ü
typedef struct _BILL_PACK_BALANCE
{
	WORD    ReqLen;
	WORD	ReqType;
	WORD	RetCode;

	__int64 GUserNo;
	char	GUserID[50+1];
	DWORD	CashRemain;

} BILL_PACK_BALANCE, *PBILL_PACK_BALANCE;

// ������ ���� ����ü
typedef struct _BILL_PACK_BUY
{
	WORD	ReqLen;
	WORD	ReqType;
	WORD	RetCode;

	__int64 GUserNo;
	char	GUserID[50+1];

	DWORD	CashRemain;
	char	ChargeNos[512+1];

	char	ItemIDs[256+1];
	char	ItemNames[2048+1];
	char    ItemUnitPrices[256+1];
} BILL_PACK_BUY, *PBILL_PACK_BUY;

#pragma pack(pop)

#endif
