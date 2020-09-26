#pragma once

#include "CnBaseRoom.h"
#include "FSM/support_FSM.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"
#include <boost/timer.hpp>

#include "Room/PersonalShopUserManager.h"
#include "X2Data/XSLSquareUnit.h"

SmartPointer( KPersonalShop );

#define PS_OPENING_ED	1000	//�ӽ÷� ���� �������� ���.. ���� ������ ���� ������ �������� �����Ѵ�. 
#define TD_COMMISSION	0.03	//�ŷ� �Խ��� ������
#define TD_HOST_COMMISSION	0.1	//�Ǹ��� ������(�����̾� �ƴҶ�)

class KPersonalShop : public KCnBaseRoom
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	enum ENUM_TIMER
	{
		TM_WAIT_OTHER,
		TM_TOTAL_NUM,
	};

	KPersonalShop(void);
	virtual ~KPersonalShop(void);

	virtual void CloseRoom();
	virtual void Tick();

	bool IsEmpty();
	bool IsFull();

protected:
	void BreakPersonalShop( int iReason, UidType iHostUID );

	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer
	//{{ 2011. 04. 06	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	virtual void ProcessNativeEvent( const KEventPtr& spEvent ) {}
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	template < class T > void BroadCastSameGroupGS( unsigned short usEventID, const T& data );
#endif SERV_INTEGRATION
	//}}

	template < class T >
	void BroadCast( u_short usEventID, const T& data );
	void BroadCastID( u_short usEventID );
	void _BroadCast( const KEvent& kEvent );

#ifdef DEF_TRADE_BOARD
	//## ������ ����Ʈ ����.
	//# �߰� : ���� ������ ������ ��ϿϷ�
	//# ���� : ���� ����, �����ۺ� �ǸſϷ�
	//## �Լ�����
	//# NerError::ERR_PERSONAL_SHOP_26 : ������ ����Ʈ �߰�
	//# NerError::ERR_PERSONAL_SHOP_27 : ������ ����Ʈ ����
	//# NerError::ERR_PERSONAL_SHOP_28 : ������ ����Ʈ ����
	void SendPShopListInfo( int iCode, std::vector<KSellPersonalShopItemInfo> & veckInfo );
	void SendPShopListInfo( int iCode, KSellPersonalShopItemInfo & kInfo );
#endif DEF_TRADE_BOARD

	//////////////////////////////////////////////////////////////////////////
	// event handler
   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );
	DECL_ON_FUNC( ERM_OPEN_PERSONAL_SHOP_REQ );
	DECL_ON_FUNC( ERM_REG_PERSONAL_SHOP_ITEM_REQ );
	DECL_ON_FUNC( ERM_JOIN_PERSONAL_SHOP_REQ );
   _DECL_ON_FUNC( ERM_BREAK_PERSONAL_SHOP_REQ, KEGS_BREAK_PERSONAL_SHOP_REQ );	//�����ڰ� �����ų� ���������� ������ ���� ��Ʈ��ũ ������ ��������..
    DECL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ );	//�����ڰ� �ƴҰ�� ��������� ������ ���� ����
	DECL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
	DECL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK );
	//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	DECL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}

protected:
	KPersonalShopUserManager	m_kPersonalShopUserManager;
	boost::timer				m_kTimer[TM_TOTAL_NUM];

	std::wstring							m_wstrPersonalShopName;
	std::vector<KSellPersonalShopItemInfo>	m_vecSellItemInfo;

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	SEnum::AGENCY_SHOP_TYPE		m_PersonalShopType;
#else //SERV_UPGRADE_TRADE_SYSTEM
	CXSLSquareUnit::PERSONAL_SHOP_TYPE		m_PersonalShopType;
#endif //SERV_UPGRADE_TRADE_SYSTEM
};

//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
template < class T >
void KPersonalShop::BroadCastSameGroupGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_ROOM, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0, anTrace, usEventID, data );
}
#endif SERV_INTEGRATION
//}}

template< class T >
void KPersonalShop::BroadCast( u_short usEventID, const T& data )
{
	KEvent kEvent;  // _BroadCast���� clone�� ���̹Ƿ� ���⼱ heap �ƴϾ ��.
	kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
	kEvent.m_usEventID = usEventID;

	KSerializer ks;
	kEvent.m_kbuff.Clear();

	// serialize - only data
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( data );
	ks.EndWriting();

	_BroadCast( kEvent );
}


