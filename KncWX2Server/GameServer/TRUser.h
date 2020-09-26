//////////////////////////////////////////////////////////////////////////
//des : IOCP �� �̿��� ��� ������ �Ͽ� ������ �ϸ�
//.1. : PROXY �� ������� ����
//.2. : ��Ŷ�� ��ȣȭ ���� �ʱ� ���� SOCKET OBJECT�� Ȱ��.
//////////////////////////////////////////////////////////////////////////
#pragma once 

#include "Socket/SocketObject.h"
#include "RTTI.h"
#include "KncSmartPtr.h"
#include "KncUidType.h"
#include "Serializer/SerBuffer.h"
#include <boost/enable_shared_from_this.hpp>
#include "FSM/support_FSM.h"
#include <dbg/dbg.hpp>
#include <KncSecurity/KncSecurity.h>

#include "Profiler/Profiler.h"

//{{ 2010. 04. 04  ��μ�	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
#include <ObjectPool/BoostAllocator.h>
#endif SERV_BOOST_ALLOCATOR
//}}

#define KTRUserPtr KTRUser*
#undef KTRUserPtr
#define KSkTRUserPtr KSkTRUser*
#undef KSkTRUserPtr

SmartPointer(KTRUser);
SmartPointer(KSkTRUser);


//{{ 2010. 04. 04  ��μ�	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
BOOST_ALOC_SYNC_CLASS_INHERITANCE( KSkTRUser, public KSocketObject )
#else
class KSkTRUser : public KSocketObject
#endif SERV_BOOST_ALLOCATOR
//}}
{
public:
    enum
    {
        MINIMUM_PACKET_SIZE = 3,
        MAXIMUM_PACKET_SIZE = 512,
    };

public:
    void OnRecvCompleted( DWORD dwTransfered );
    void OnSocketError();
    void OnAcceptConnection();
    KTRUserPtr GetTRUserPtr();

	//{{ 2009. 3. 12  ������	�������Ϸ� üũ�� �Լ�
	bool SendDataWithProfile( const char* szData, int iSize );
	//}}

    boost::weak_ptr< KTRUser > m_pkTRUser;
};

class KTRUser : public boost::enable_shared_from_this< KTRUser >
{
	DeclareFSM_MT;
	NiDeclareRootRTTI(KTRUser);

public:
	enum ENUM_TICKS
	{
		CONNECT_TICK = 0,
		HB_TICK,
		TICKS_NUM,
	};

	KTRUser(void);
	virtual ~KTRUser(void);

	UidType	GetUID(){ return m_iUnitUID; }

	bool	Init();
	void	Tick();
	
	bool	SendPacket( const KncSecurity::KByteStream& kByteStream );
	template< typename T >
	static bool MakeByteStream( KncSecurity::KByteStream& kByteStream, unsigned short usEventID, T& data, bool bCompress = true );

	void	ReserveDestroy(){ m_bDestroyReserved = true; }
	void	SetSendQueueSize( int iQueueSize );

	//derive to KSkTRUser
	void	OnSocketError();
	void	OnAcceptConnection();
	//void	OnRecvCompleted( IN const char cRecvCnt, IN KSerBuffer& buff );
	void	OnRecvCompleted( IN KSerBuffer& buff );

	KSkTRUserPtr	GetSkTRUserPtr(){ return m_spSockObj; }

protected:

	void	SetTick( IN ENUM_TICKS eIndex )	{ m_adwTickCount[ eIndex ] = ::GetTickCount(); }
	DWORD	GetTick( IN ENUM_TICKS eIndex)	{ return m_adwTickCount[ eIndex ]; }

	void	OnDestroy();

protected:
	KSkTRUserPtr	m_spSockObj;
    UidType			m_iUnitUID;
	bool			m_bDestroyReserved;
	DWORD			m_adwTickCount[TICKS_NUM];
};

template< typename T >
bool KTRUser::MakeByteStream( KncSecurity::KByteStream& kByteStream, unsigned short usEventID, T& data_, bool bCompress_ /*= true*/ )
{
	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"KEvent���� setdata�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	KEvent kEvent;
	kEvent.SetData( 0, NULL, usEventID, data_ );

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"KEvent���� setdata�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	//if( bCompress_ ) kEvent.m_kbuff.Compress(); ---> ���� ���� ����

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"KSerializer���� Put�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	// serialize - full event object
	KSerializer     ks;
	KSerBuffer      kbuff;
	ks.BeginWriting( &kbuff );
	ks.Put( kEvent );
	ks.EndWriting();

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"KSerializer���� Put�ϱ�" );
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"KByteStream���� append�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	kByteStream.reserve( kbuff.GetLength() + sizeof(USHORT) );
	kByteStream.Assign( kbuff.GetLength() + sizeof(USHORT), sizeof(USHORT) );
	kByteStream.append( (byte*)kbuff.GetData(), kbuff.GetLength() );   // ���� ������ ������ �ֱ�.

	///////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"KByteStream���� append�ϱ�" );
	//////////////////////////////////////////////////////////////////////////

	return true;
}



