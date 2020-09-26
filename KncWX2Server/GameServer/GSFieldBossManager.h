#pragma once

#include "ServerDefine.h"

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-14	// �ڼ���
#include <Windows.h>
#include <KNCSingleton.h>
#include <time.h>

// �ʵ� ���� �ý����� ��κ��� �ڵ带 �ܺο� �ε��� �غ��� 2013-10-31//	�ڼ���

class KGSFieldBossManager
{
	DeclareSingleton( KGSFieldBossManager );

	enum GS_FIELD_BOSS_STATE
	{
		GFBS_PORTAL_OPEN	= ( 1 << 0 ),
		GFBS_COOL_TIME		= ( 1 << 1 ),
		GFBS_CONNECTED		= ( 1 << 2 ),	// GlobalServer�� ������ �Ǿ� �ִٸ�, ���� ������ GlobalServer�κ��� �޴´�. ���� ������ �������ٸ�, ���� ���� �ð��� ���ؼ� ���� üũ�� �Ѵ�
	};

public:
	void		SetReservedTimeForInitRequest( IN const __time64_t tReservedTimeForInitRequest )	{	m_tReservedTimeForInitRequest = tReservedTimeForInitRequest;	}
	__time64_t	GetReservedTimeForInitRequest( void ) const	{	return m_tReservedTimeForInitRequest;	}
	bool		CheckTerm( IN const __time64_t tCurrentTime );
	void		SetConnected( const bool bConnected )	{	( bConnected ) ? ( m_byteState |= GFBS_CONNECTED ) : ( m_byteState &= ~GFBS_CONNECTED );	}

	void		SetInfo( IN const __time64_t tPortalAppearanceTime
					   , IN const __time64_t tRemainPortalTime
					   , IN const __time64_t tRemainCoolTime
					   , IN const int iPortalAppearanceMap
					   , IN const int iPortalDestination
					   , IN const bool bPortalOpen
					   , IN const bool bCoolTime
					   );

	void		ClosePortal( void ){	m_byteState &= ~GFBS_PORTAL_OPEN;	}
	void		SetCoolTimeState( IN const bool bValue )	{	( bValue ) ? ( m_byteState |= GFBS_COOL_TIME ) : ( m_byteState &= ~GFBS_COOL_TIME );	}

	__time64_t	GetPortalAppearanceTime( void ) const	{	return m_tPortalAppearanceTime;	}
	int			GetPortalAppearanceMap( void ) const	{	return m_iPortalAppearanceMap;	}
	int			GetPortalDestination( void ) const	{	return m_iPortalDestination;	}
	__time64_t	GetRemainPortalTime( void ) const	{	return m_tRemainPortalTime;	}
	__time64_t	GetRemainCoolTime( void ) const	{	return m_tRemainCoolTime;	}

	bool		IsPortalOpen( void ) const	{	return ( m_byteState & GFBS_PORTAL_OPEN ) == GFBS_PORTAL_OPEN;	}
	bool		IsCoolTime( void ) const	{	return ( m_byteState & GFBS_COOL_TIME ) == GFBS_COOL_TIME;	}
	bool		IsConnected( void ) const	{	return ( m_byteState & GFBS_CONNECTED ) == GFBS_CONNECTED;	}

private:
	KGSFieldBossManager( void );
	~KGSFieldBossManager( void );

private:
	__time64_t	m_tReservedTimeForInitRequest;	// Global Server�� FieldBossManager ������ ������ ���� ���
	__time64_t	m_tCheckTerm;

	__time64_t	m_tPortalAppearanceTime;		// Local Machine Time
	__time64_t	m_tRemainPortalTime;			// ��Ż�� ���� ������ ���� �ð� ( ���� �ð����� ó���ص� �ȴ� )
	__time64_t	m_tRemainCoolTime;				// ����۱��� ���� �ð� ( ���� �ð����� ó���ص� �ȴ� )
	int			m_iPortalAppearanceMap;			// ��Ż�� ������ ��Ʋ �ʵ� ID
	int			m_iPortalDestination;			// ��Ż�� ����� ���� �ʵ� ID
	byte		m_byteState;
};

DefSingletonInline( KGSFieldBossManager );
#endif // SERV_BATTLE_FIELD_BOSS
