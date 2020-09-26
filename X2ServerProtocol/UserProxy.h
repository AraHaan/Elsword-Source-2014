#pragma once

SmartPointer( KUserProxy );

class KUserProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;


public:
	enum USER_PROXY_TYPE
	{
		UPT_INVALID,
		UPT_GAME_SERVER,
		UPT_CHANNEL_SERVER,
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
		UPT_COLLECT_SERVER,
#endif
	};

public:
    KUserProxy( USER_PROXY_TYPE eType );
    virtual ~KUserProxy(void);

    //template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    //bool SendID( unsigned short usEvenID );

    // derived from KPerformer
    virtual void ProcessEvent( IN KEvent* pkEvent );
        
    //////////////////////////////////////////////////////////////////////////
    // Interfaces for Client Programmer.
	bool GetIntendedDisconnect() const { return m_bIntendedDisconnect; }
	void SetIntendedDisconnect(bool val) { m_bIntendedDisconnect = val; }


protected:  // Recv Msg Handler & Util Function
    virtual void OnSocketError();
	virtual void OnAuthenticFailed(); // ��Ŷ ������ ���� ������ ȣ��Ǵ� �����Լ��� ������ �Ѱ�!

private:
	USER_PROXY_TYPE m_eProxyType;
	bool m_bIntendedDisconnect;		// dmlee, 2009.3.11 - �ǵ������� ������ �����ߴ���, ������ ���� ������ ���� �Ǿ����� �����ϱ� ����
};