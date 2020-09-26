//////////////////////////////////////////////////////////////////////////
//	NaunBilllingPacket.h
//
//	NaunBilling ��Ŷ Ŭ����
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_IN
#define MAX_PACKET_SIZE_NUBT 1024
#define HEADER_IN_PACKET_LENGTH_NUBT 2
#define MAX_PACKET_CONTENT_SIZE_NUBT ( MAX_PACKET_SIZE_NUBT - HEADER_IN_PACKET_LENGTH_NUBT )

#define KNaunBillingPacketPtr KNaunBillingPacket*
#undef KNaunBillingPacketPtr
SmartPointer( KNaunBillingPacket );

class KNaunBillingPacket
{
public:
	KNaunBillingPacket();
	virtual ~KNaunBillingPacket();

	int  GetTotalLength()				{ return ( int )m_usReqLen; }
//	int  GetPacketType()				{ return ( int )m_usReqType; }

	bool ReadFromBuffer( IN BYTE* pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );

	bool ReadByte( OUT BYTE& byteData, IN OUT int& iCP );
	bool Read2Byte( OUT unsigned short& usData, IN OUT int& iCP );
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	bool Read2ByteSigned( OUT short& sData, IN OUT int& iCP );
	//}}
	bool Read4Byte( OUT unsigned long& ulData, IN OUT int& iCP );
	bool Read8Byte( OUT __int64& iData, IN OUT int& iCP );
	bool ReadString( OUT std::wstring& wstrData, IN OUT int& iCP );
	bool ReadString( OUT std::wstring& wstrData, IN const int iFixedLength, IN OUT int& iCP );

	bool WriteByte( IN BYTE byteData, IN OUT int& iCP );
	bool Write2Byte( IN unsigned short usData, IN OUT int& iCP );
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	bool Write2ByteSigned( IN short sData, IN OUT int& iCP );
	//}}
	bool Write4Byte( IN unsigned long ulData, IN OUT int& iCP );
	bool Write8Byte( IN const __int64& iData, IN OUT int& iCP );
	bool WriteString( IN const std::wstring& wstrData, IN OUT int& iCP );
	bool WriteString( IN const std::wstring& wstrData, IN const int iFixedLength, IN OUT int& iCP );

	// ���ڸ� ��Ʈ������~
	std::string toString(int iSource);

	bool Write( IN const KENAUN_BILLING_BALANCE_REQ& kPacket );
	bool Read( OUT KENAUN_BILLING_BALANCE_ACK& kPacket );

	bool Write( IN const KNaunBuyProductRequestData& kPacket ); // �ε��� ��� �ۼ����ϴ� ������ ���� �� �����ϱ� ��Ŷ�� �����ϱ⿡ �ǵ������� �̷��� ��.
	bool Read( OUT KNaunBuyProductRequestData& kPacket );

protected:
	unsigned short  m_usReqLen;
	BYTE m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_NUBT];
};
#endif SERV_COUNTRY_IN