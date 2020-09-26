#pragma once

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

	void	SetHwnd( HWND hWnd );
	void	SetSendMsgCallBack( SEND_MSG_FUNC pSendGameMessage ){ m_pSendGameMessage = pSendGameMessage; }
	void	SetThrowWinMsg( bool bThrowWinMsg );
	bool	GetThrowWinMsg() { return m_bThrowWinMsg; }

	// 060720. ���� ó������ ��ٸ���. KUserProxy, KGSProxy�� �̸� ����Ѵ�.
	int		ConnectNAuth( IN const WCHAR* pIP, IN const int port, IN const KEvent& kEvent, IN DWORD dwTimeOut );

	void	SetClassID( int iClassID )	{ m_iClassID = iClassID; }  // ������ �޼����� �����Ҷ� ����� id �Ҵ�.
	int		GetClassID()				{ return m_iClassID; }
	void	SetServerMsgID( UINT serverMsgID ){ m_ServerMsgID = serverMsgID; }
    void SetLoad( int iLoad )               { m_iLoad = iLoad; }
    int GetLoad()                           { return m_iLoad; }

    void SetServerUID( UidType iServerUID ) { m_iServerUID = iServerUID; }
    UidType GetServerUID()                  { return m_iServerUID; }



protected:
	void PostEvent( KEvent* pkEvent );    // �̺�Ʈ�� ������ �޽����� �ϰų� ���� ť�� ť���Ѵ�.

	HWND                        m_hWnd;
	SEND_MSG_FUNC				m_pSendGameMessage;
	bool                        m_bThrowWinMsg;     ///< false�̸� ���� �޽����� ť�� �־�α⸸ �Ѵ�.
	UINT						m_ServerMsgID;

	boost::shared_ptr<void>     m_spEventAck;
	int                         m_nAckOK;
	int                         m_iClassID;         // �̺�Ʈ�� ������ �޼����� ���� ��, ������ ������ �Ѵ�.

    int                         m_iLoad;
    UidType                     m_iServerUID;
public:

	KncCriticalSection          m_csRecvMsg;
	std::queue<KEventPtr>       m_queRecvMsg;       ///< m_bThrowWinmsg = false�϶� ���.

};