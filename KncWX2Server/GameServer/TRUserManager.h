#pragma once 

#include "TRUser.h"
#include <map>
#include <list>
#include "Thread/Locker.h"
#include "FSM/FSMclass.h"
#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "ClientPacket.h"

class KTRUserManager
{
	KTRUserManager( const KTRUserManager& right );
	KTRUserManager& operator= (const KTRUserManager& right );

public:
public:
	KTRUserManager(void);
	~KTRUserManager(void);

	KTRUserPtr CreateTRUser();
	KTRUserPtr GetTRUser( const UidType& iUID );

	void	Tick();	// derive to KTRServer

	void	SetCreateEnable( bool bEnable ){ m_bCreateSessionEnable = bEnable; }

	bool	Insert( KTRUserPtr pkTRUser );  // ���ݸ� UID ���ó���� ������ map�� ���.

	size_t	GetCountTot()const { KLocker lock( m_cslist ); return m_listSessionTable.size(); }
	size_t	GetCountReg()const { KLocker lock( m_csmap ); return m_mapSessionTable.size(); }

protected:
	bool		m_bCreateSessionEnable;
	FSMclassPtr	m_spFSM;	// FSM of TRUser

	mutable KncCriticalSection		m_cslist;
	std::list<KTRUserPtr>			m_listSessionTable; // ��� �̵�� ��� ���� ����.

	mutable KncCriticalSection      m_csmap;
	std::map<UidType,KTRUserPtr>	m_mapSessionTable;  // ��ϵ� TRUser�� ���� ����.
};


