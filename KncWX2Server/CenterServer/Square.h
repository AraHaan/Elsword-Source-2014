#pragma once

#include "CnBaseRoom.h"

#include <boost/timer.hpp>

#include "SquareUserManager.h"

#include "X2Data/XSLSquareGame.h"
#define ST(s)	CXSLSquareGame::ST_##s

SmartPointer( KSquare );

class KSquare : public KCnBaseRoom
{
	NiDeclareRTTI;
	DeclToStringW;

	enum SQUARE_INFO
	{
#ifdef SERV_SQUARE_PLAYER_CONTROL
        SQUARE_MAX_SLOT = 60, // ���� 70
#else //SERV_SQUARE_PLAYER_CONTROL
        SQUARE_MAX_SLOT = 70,
#endif //SERV_SQUARE_PLAYER_CONTROL
	};

public:
	KSquare(void);
	virtual ~KSquare(void);

	virtual void	Tick();

	void	SendSquareListInfo( int iCode );	//�������� ������Ʈ

	void	SetDefault( bool bIsDefault ){ m_bIsDefault = bIsDefault; }
	void	SetType( const char cSquareType ){ m_cSquareType = cSquareType; }
	//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
	void	SetSortIndex( char cSortIndex ){ m_cSortIndex = cSortIndex; }
#endif SERV_SQUARE_LIST_SERV_UID
	//}}
	void	GetSquareInfo( KSquareInfo& kSquareInfo );
	int		GetNumMember(){ return m_kSquareUserManager.GetNumMember(); }

	bool	IsEmpty(){ return (m_kSquareUserManager.GetNumMember() == 0); }

protected:
	virtual void	ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer

	//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	template < class T > void BroadCastSameGroupGS( unsigned short usEventID, const T& data );
#endif SERV_INTEGRATION
	//}}

	template < class T >
	void	BroadCast( u_short usEventID, const T& data );
	void	BroadCastID( u_short usEventID );
    void   _BroadCast( const KEvent& kEvent );

	void	DeleteZombie( std::vector<UidType>& vecZombieUnit );

	//////////////////////////////////////////////////////////////////////////
	// event handler
	DECL_ON_FUNC( ERM_OPEN_SQUARE_NOT );
	DECL_ON_FUNC( ERM_JOIN_SQUARE_REQ );
   _DECL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ );
   _DECL_ON_FUNC( ERM_LEAVE_SQUARE_REQ, KEGS_LEAVE_SQUARE_REQ );
   _DECL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ );
   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );
    DECL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT );

    DECL_ON_FUNC( ERM_REQUEST_TRADE_NOT );
    DECL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT );

   _DECL_ON_FUNC( ERM_SQUARE_LIST_INFO_NOT, UidType );

    DECL_ON_FUNC_NOPARAM( ERM_CHECK_OPEN_PERSONAL_SHOP_REQ );
   _DECL_ON_FUNC( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, KSquarePersonalShopInfo );
   _DECL_ON_FUNC( ERM_CHECK_JOIN_PERSONAL_SHOP_REQ, KEGS_JOIN_PERSONAL_SHOP_REQ );

    DECL_ON_FUNC_NOPARAM( ERM_TRADE_START_NOT );
    DECL_ON_FUNC_NOPARAM( ERM_TRADE_FINISH_NOT );

	//{{ 2009. 9. 23  ������	1������ĳ����
   _DECL_ON_FUNC( ERM_CHANGE_SQUARE_USER_INFO_NOT, KSquareUserInfo );
	//}}

   //{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
   DECL_ON_FUNC( ERM_SQUARE_UNIT_REFRESH_ED_NOT );
#endif SERV_ED_MONITORING_IN_GAME
   //}}

protected:
	char				m_cSquareType;
	//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
	char				m_cSortIndex;
#endif SERV_SQUARE_LIST_SERV_UID
	//}}
	bool				m_bIsDefault;		//�⺻���� ���ΰ�..?(�⺻�������̸� ������ �� ������ close ���� �ʴ´�.)

	KSquareUserManager	m_kSquareUserManager;
};

//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
template < class T >
void KSquare::BroadCastSameGroupGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_ROOM, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0, anTrace, usEventID, data );
}
#endif SERV_INTEGRATION
//}}

template < class T >
void KSquare::BroadCast( u_short usEventID, const T& data )
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
