#pragma once
#include "Socket/Session.h"
#include <windows.h>
#include <Queue>

#define KActorProxyPtr KActorProxy*
#undef KActorProxyPtr
SmartPointer( KActorProxy );

struct KServerInfo;
class KActorProxy : public KSession
{
    NiDeclareRTTI;
    DeclToStringW;

protected:
    KActorProxy();

public:
    virtual ~KActorProxy(void) {}

    void SetHwnd( HWND hWnd );
    void SetThrowWinMsg( bool bThrowWinMsg );
    bool GetThrowWinMsg()                   { return m_bThrowWinMsg; }

    // 060720. ���� ó������ ��ٸ���. KUserProxy, KGSProxy�� �̸� ����Ѵ�.
	int ConnectNAuth( IN const WCHAR* pIP, IN const int port, IN const KEvent& kEvent, IN DWORD dwTimeOut );

    void SetClassID( int iClassID )         { m_iClassID = iClassID; }  // ������ �޼����� �����Ҷ� ����� id �Ҵ�.
    int GetClassID()                        { return m_iClassID; }

    void SetLoad( int iLoad )               { m_iLoad = iLoad; }
    int GetLoad()                           { return m_iLoad; }

	//{{ 2010. 01. 27  ������	���������
	void SetLoad_NEW( int iType, bool bIsOpen );
	u_int GetLoad_NEW( int iType );
	//}}

	int GetProxyID()						{ return m_iProxyID; }
	void SetProxyID( int iProxyID )			{ m_iProxyID = iProxyID; }

	//{{ 2010. 05. 10  ������	�������� ������ ����
	virtual int	 GetActorServerGroup()		{ return -1; }
	virtual bool IsSameServerGroup()		{ return false; }
	//}}

protected:
    //void PostEvent( const KEventPtr& spEvent );    // �̺�Ʈ�� ������ �޽����� �ϰų� ���� ť�� ť���Ѵ�.	

    HWND                        m_hWnd;
    bool                        m_bThrowWinMsg;     ///< false�̸� ���� �޽����� ť�� �־�α⸸ �Ѵ�.

    boost::shared_ptr<void>     m_spEventAck;
    int                         m_nAckOK;
    int                         m_iClassID;         // �̺�Ʈ�� ������ �޼����� ���� ��, ������ ������ �Ѵ�.

    int                         m_iLoad;
	int							m_iProxyID;

	//{{ 2010. 01. 27  ������	���������
	KncCriticalSection          m_csLoad;
	std::map< int, u_int >		m_mapLoad;
	//}}	

public:

    KncCriticalSection          m_csRecvMsg;
    std::queue<KEventPtr>       m_queRecvMsg;       ///< m_bThrowWinmsg = false�϶� ���.
};