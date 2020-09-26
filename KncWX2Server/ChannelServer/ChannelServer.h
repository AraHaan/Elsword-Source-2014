#pragma once
#include "BaseServer.h"
#include "KncSend.h"

class KChannelServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;
    DeclPfID;

public:
    KChannelServer(void);
    virtual ~KChannelServer(void);	

    // derived from KBaseServer
    virtual bool Init();
    virtual void RegToLua();
    virtual void ShutDown();
    virtual bool DestroyAllSession();         // �������� ������ ����.

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void Tick();

    //////////////////////////////////////////////////////////////////////////

protected:
	void WriteServerInfoToDB();
	virtual void OnServerReadyComplete();

	//_DECL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK );
	_DECL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_CHANNEL_SERVER_ON_ACK );	// ���ҵ� �ؿ��� ����
   _DECL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
	DECL_ON_FUNC( DBE_CHANNEL_LIST_ACK );

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	DECL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_ACK );
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	DECL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK );
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}
	//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	DECL_ON_FUNC( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK );
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	//{{ 2012. 10. 15	�ڼ���	ä�� ���� �ǽð� ��ũ��Ʈ ���� ���
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	_DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	//}}

#ifdef SERVER_GROUP_UI_ADVANCED
	DECL_ON_FUNC( DBE_SERVERGROUP_LIST_ACK );
#endif SERVER_GROUP_UI_ADVANCED
public:
	void RefreshChannelList();
	void UpdateChannelList( const std::map< int, KChannelInfo >& mapChannelList );

	//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	void	UpdateChannelBonusList( const std::map< int, KChannelBonusInfo >& mapChannelBonusList );
	const std::map< int, KChannelBonusInfo >& GetChannelBonusList() { return m_mapChannelBonusList; }
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}

	const std::map< int, KChannelInfo >& GetChannelList() { return m_mapChannelList; }
	
	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	template < class T > void SendToSMSDB( unsigned short usEventID, const T& data );
#endif SERV_CHECK_DROP_CCU
	//}}
#ifdef SERVER_GROUP_UI_ADVANCED
	void RefreshServerGroupList();
	void UpdateServerGroupList( const std::map< int, KServerGroupInfo >& mapServerGroupList );
	const std::map< int, KServerGroupInfo >& GetServerGroupList() { return m_mapServerGroupList; }
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_LOGIN_RESULT_INFO
	void InitLoginResultInfo();
	void WriteLoginResultInfoToDB();
	void PlusLoginResultCount(KLoginResultInfo::LOGIN_RESULT_TYPE_ENUM eResultType);
#endif SERV_LOGIN_RESULT_INFO
protected:
	std::map< int, KChannelInfo >		m_mapChannelList;

	//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	std::map< int, KChannelBonusInfo >	m_mapChannelBonusList;
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}

	boost::timer						m_kChannelListRefreshTimer;
	bool								m_bIsFirstUpdate;
#ifdef SERVER_GROUP_UI_ADVANCED
	std::map< int, KServerGroupInfo >		m_mapServerGroupList;
	boost::timer							m_kServerGroupListRefreshTimer;
	bool									m_bIsFirstUpdateServerGroup;
#endif SERVER_GROUP_UI_ADVANCED	

#ifdef SERV_LOGIN_RESULT_INFO
	KLoginResultInfo					m_kLoginResultInfo;
	boost::timer						m_kLoginResultInfoTimer;
#endif SERV_LOGIN_RESULT_INFO

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	boost::timer					m_tTimeProcessCommunicationONOFF;
	bool							m_bServerRunningProcessCommunicationOnOff;
#endif //SERV_PROCESS_COMMUNICATION_KSMS
};

DefKObjectInline( KChannelServer, KBaseServer );

//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
template < class T >
void KChannelServer::SendToSMSDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( GetPfID(), GetUID(), PI_CHANNEL_SMS_DB, 0, anTrace, usEventID, data );
}
#endif SERV_CHECK_DROP_CCU
//}}
