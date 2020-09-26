#pragma once 

#include "GarenaBillingUser.h"
#include <map>
#include <list>
#include "Thread/Locker.h"
#include "FSM/FSMclass.h"
#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "ClientPacket.h"



#ifdef SERV_COUNTRY_PH


class KGarenaBillingUserManager
{
	KGarenaBillingUserManager( const KGarenaBillingUserManager& right );
	KGarenaBillingUserManager& operator= (const KGarenaBillingUserManager& right );

public:
public:
	KGarenaBillingUserManager(void);
	~KGarenaBillingUserManager(void);

	KGarenaBillingUserPtr CreateGarenaBillingUser();
	KGarenaBillingUserPtr GetGarenaBillingUser( const UidType& iUID );

	void	Tick();	// derive to KTRServer

	void	SetCreateEnable( bool bEnable ){ m_bCreateSessionEnable = bEnable; }

	bool	Insert( KGarenaBillingUserPtr pkGarenaBillingUser );  // ���ݸ� UID ���ó���� ������ map�� ���.

	size_t	GetCountTot()const { KLocker lock( m_cslist ); return m_listSessionTable.size(); }
	size_t	GetCountReg()const { KLocker lock( m_csmap ); return m_mapSessionTable.size(); }

protected:
	bool		m_bCreateSessionEnable;
	FSMclassPtr	m_spFSM;	// FSM of TRUser

	mutable KncCriticalSection				m_cslist;
	std::list<KGarenaBillingUserPtr>			m_listSessionTable; // ��� �̵�� ��� ���� ����.

	mutable KncCriticalSection				m_csmap;
	std::map<UidType,KGarenaBillingUserPtr>	m_mapSessionTable;  // ��ϵ� TRUser�� ���� ����.
};


#endif SERV_COUNTRY_PH
//}}