#pragma once

#include <KncLua.h>
#include <boost/scoped_ptr.hpp>
#include "Thread/Thread.h"
#include "StringTable.h"
#include "Performer.h"
#include <RTTI.h>
#include <ToString.h>
#include <KNCSingleton.h>
#include "CommonPacket.h"
//{{ 2010. 06. 20  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	#include "ThreadStatisticsManager.h"
#else
	#include "StatisticsManager.h"
#endif SERV_STATISTICS_THREAD
//}}
//{{ 2010. 12. 7	������	���� ���� �ý���
#ifdef SERV_MAIL_SYSTEM
	#include "Mail/MailManager.h"
#endif SERV_MAIL_SYSTEM
//}}
#include "SecurityString.h"

//{{ 2010. 07. 13  ������	using boost foreach
// include boost foreach
#include <boost/test/utils/foreach.hpp>

// boost foreach macro for map container
#define BOOST_MAP_FOREACH( key, value, variable, container ) \
	typedef std::map<const key,value> key##value##maptype; \
	BOOST_TEST_FOREACH( key##value##maptype::value_type, variable, container ) \

#define BOOST_MAP_CONST_FOREACH( key, value, variable, container ) \
	typedef std::map<const key,value> key##value##maptype; \
	BOOST_TEST_FOREACH( const key##value##maptype::value_type&, variable, container ) \
//}}


//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}


class KODBC;
class KSimLayer;
class KBaseServer : public KPerformer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclInstanceKeeping( KBaseServer );
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

public:
	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	enum VERSION_ENUM
	{
        VE_INTERNAL = 0,
		VE_OPEN_TEST,
		VE_SERVICE,
#ifdef SERV_KR_OPEN_TEST2_VERSION// �۾���¥: 2013-05-24	// �ڼ���
		VE_OPEN_TEST2,
#endif // SERV_KR_OPEN_TEST2_VERSION

		VE_MAX,
	};
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}

	enum EServerClass
	{
		ESC_CENTER		= 1,
		ESC_LOGIN		= 2,
		ESC_GAME		= 3,
		ESC_CHANNEL		= 4,
		ESC_SIMULATION	= 5,
		ESC_GLOBAL		= 6,
	};
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	enum TimeControl_Item_Release_Tick_Type
	{
		TCIRTT_TIME_CONTROL_ITEM_CHECK		= 20,
	};
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
	enum TimeControl_CubeInItemMapping_Release_Tick_Type
	{
		TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING = 50,
	};
#endif //SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

    KBaseServer(void);
    virtual ~KBaseServer(void);
	
	virtual bool Init();
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );       // �ܼ��϶� ��ü interpreter ����, �������϶� ����.
    virtual void ShutDown();
    virtual bool DestroyAllSession() = 0;           // �������� ������ ����.
    //virtual void WriteServerInfoToDB();
    virtual void Tick();
	virtual void OnServerReadyComplete();

	//{{ 2009. 6. 15  ������	cmd args
	void SetCmdArgs( const std::vector< KClearString< std::wstring > >& vecCmdArgs )	{ m_vecCmdArgs = vecCmdArgs; }
	void GetCmdArgs( std::vector< KClearString< std::wstring > >& vecCmdArgs )			{ vecCmdArgs = m_vecCmdArgs; }
	//}}

    void SetHwnd( HWND hWnd )                       { m_hWnd = hWnd; }
    HWND GetHwnd()                                  { return m_hWnd; }

	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	void SetUseVersion( int iVersionEnum )			{ m_eUseVersion = static_cast<VERSION_ENUM>(iVersionEnum); ToString( std::wcout ); }
	VERSION_ENUM	GetVersion( void ) const		{ return m_eUseVersion;	}
#else	
	void SetSecondVersion( bool bUseSecond )        { m_bUseSecondVer = bUseSecond; ToString( std::wcout ); }
	void SetMainVersion( int nIndex, const char* szVersion );
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
    void SetShutDownWait( DWORD dwWait )            { m_dwShutDownWait = dwWait; }

    int  GetServerGroupID()                         { return m_iServerGroupID; }
    void SetServerGroupID( int iServerGroupID )     { m_iServerGroupID = iServerGroupID; }

    void GetServerList( KServerList& mapServerList );
    void SetServerList( const KServerList& mapServerList );

    int  GetServerClass()                           { return m_iServerClass; }
    void SetServerClass( int iServerClass )         { m_iServerClass = iServerClass; }

	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
	int  GetDefaultMaxUser() const					{ return m_nDefaultMaxUser; }
	void SetDefaultMaxUser( int nMaxUser )			{ m_nDefaultMaxUser = nMaxUser; }
#else
	int  GetMaxUser()                               { return m_nMaxUser; }
	void SetMaxUser( int nMaxUser )                 { m_nMaxUser = nMaxUser; }
#endif SERV_REALTIME_MAX_USER
	//}}

	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	void SetPublicIP( IN const std::wstring& wstrIP )	{ m_wstrPublicIP = wstrIP; }
	const std::wstring& GetPublicIP() const			{ return m_wstrPublicIP; }
	void SetPrivateIP( IN const std::wstring& wstrIP )	{ m_wstrPrivateIP = wstrIP; }
	const std::wstring& GetPrivateIP() const			{ return m_wstrPrivateIP; }
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}

    //void GetServerInfo( KServerInfo& kServerInfo );
    void SetServerInfo( const KServerInfo& kServerInfo );

    void SetMailingList( const char* strList )      { m_strMailingList = KncUtil::toWideString( strList ); }

	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	const std::wstring& GetMainVersion() const;
#else
	std::wstring& GetMainVersion()					{ return m_strMainVer[m_bUseSecondVer ? 1 : 0]; }
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}    
    template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
#ifdef SERV_ADD_EVENT_DB
    template < class T > void SendToEventDB( unsigned short usEventID, const T& data );
#endif //SERV_ADD_EVENT_DB
	//{{ 2010. 12. 7	������	���� ���� �ý���
#ifdef SERV_MAIL_SYSTEM
	template < class T > void QueueingEventToMailManager( unsigned short usEventID, const T& data );
#endif SERV_MAIL_SYSTEM
	//}}
    void SendToAccountDB( unsigned short usEventID );
    void SendToLogDB( unsigned short usEventID );

#ifdef SERV_ADD_EVENT_DB
	void SendToEventDB( unsigned short usEventID );
#endif //SERV_ADD_EVENT_DB

	// ���� �ε� ����
	bool GetServerReady()							{ return m_bServerReady; }
	void SetServerReady( bool bReady )				{ m_bServerReady = bReady; }

	//{{ 2008. 10. 10  ������	
	UidType GetFirstActorKey();
	//}}	

	//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	virtual void DumpToLogFile();
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}

	//{{ 2010. 11. 12	������	������ ���� ����͸�
#ifdef SERV_MORNITORING
	void SetConnectionMornitoringEnable_LUA( bool bVal );
	bool IsConnectionMornitoringEnable() const { return m_bConnMornitoringEnable; }
#endif SERV_MORNITORING
	//}}

	//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
	void SetScriptParsingErrReport_LUA( bool bVal );
	void AddScriptParsingErrReportMail_LUA( const char* pszMail );
	void AddFailScriptFileName( const wchar_t* pszScriptFileName )	{ m_vecFailScriptFileList.push_back( pszScriptFileName ); }
	bool IsScriptParsingSuccess() const				{ return m_vecFailScriptFileList.empty(); }
#endif SERV_SCRIPT_PARSING_ERR_REPORT
	//}}
	//{{ 2011. 02. 23	������	������ ���� Ÿ��
#ifdef SERV_SERVER_ROLL_TYPE
	char GetServerRollType() const	{ return m_cServerRollType; }
#endif SERV_SERVER_ROLL_TYPE
	//}}
	//{{ 2011.2.11  ��ȿ��  ����͸��� ���� ��Ʈ üũ �� �����α� ���°� ����
#ifdef SERV_PERMIT_PORT_CHECK
	void AddPortCheckMoritoringServer_LUA( const char* pStrIP );
	bool IsMonitoringServerForPortCheck( std::string strIP );
#endif SERV_PERMIT_PORT_CHECK
	//}}

	//{{ 2011. 04. 13  ���� �� �ŷ� ���� �ǽð� SMS ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE_SMS
	void SetMonitoringLetterAndTradeSMS_LUA( bool bVal )			{ m_bMonitoringLetterAndTradeSMS = bVal; }
	bool GetMonitoringLetterAndTradeSMS()							{ return m_bMonitoringLetterAndTradeSMS; }
#endif SERV_MONITORING_LETTER_AND_TRADE_SMS
	//}} 2011. 04. 13 ���� �� �ŷ� ���� �ǽð� SMS ����



protected:
    inline void SetTagVersion();
    inline void LoadINIFile( const wchar_t* szFileName );
    bool Authenticate( DWORD dwTimeOut );
	
    std::wstring                                m_strTagVersion;            ///< 050716. florist. �±׺��� ���� ���ڿ�
    std::wstring                                m_strMailingList;           ///< 061011. florist. crash report mailing list.
    std::wstring                                m_strIniFilePath;
	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	std::wstring								m_strMainVer[VE_MAX];
	VERSION_ENUM								m_eUseVersion;
#else
	std::wstring								m_strMainVer[2];    // 070516. �ΰ��� ���� ���� ��û.
	bool                                        m_bUseSecondVer;    // 070516. ��� ������ ����� ������ ����.
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
    HWND                                        m_hWnd;
    int                                         m_iServerGroupID;
    DWORD							            m_dwServerInfoUpdateTick;

    boost::shared_ptr< void >                   m_spEventAck;

    // ���� ����Ʈ
    KServerList                                 m_mapServerList;
    mutable KncCriticalSection                  m_csServerList;

    int                                         m_iServerClass;
	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
	int                                         m_nDefaultMaxUser;
#else	
	int                                         m_nMaxUser;
#endif SERV_REALTIME_MAX_USER
	//}}    

	//{{ 2009. 6. 15  ������	cmd args
	std::vector< KClearString< std::wstring > >	m_vecCmdArgs;
	//}}

public:
    KStringTable                                m_stringTable;
    DWORD                                       m_dwShutDownWait;
    static DWORD                                ms_dwServerInfoUpdateGap;

	// ���� �ε� ����
	bool										m_bServerReady;

	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	std::wstring								m_wstrPublicIP;
	std::wstring								m_wstrPrivateIP;
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
	    
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	KThreadStatisticsManager					m_kTStatisticsManager;
#else
	KStatisticsManager                          m_kStatisticsManager;
#endif SERV_STATISTICS_THREAD
	//}}

	//{{ 2010. 12. 7	������	���� ���� �ý���
#ifdef SERV_MAIL_SYSTEM
	KMailManager								m_kMailManager;
#endif SERV_MAIL_SYSTEM
	//}}

	//{{ 2010. 11. 12	������	������ ���� ����͸�
#ifdef SERV_MORNITORING
	bool										m_bConnMornitoringEnable;
#endif SERV_MORNITORING
	//}}
	//{{ 2011. 02. 23	������	������ ���� Ÿ��
#ifdef SERV_SERVER_ROLL_TYPE
	char										m_cServerRollType;
#endif SERV_SERVER_ROLL_TYPE
	//}}
	//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
	bool										m_bScriptParsingErrReport;
	std::vector< std::wstring >					m_vecFailScriptFileList;
	std::vector< std::wstring >					m_vecScriptParsingErrMailList;
#endif SERV_SCRIPT_PARSING_ERR_REPORT
	//}}

	//{{ 2011.2.11  ��ȿ��  ����͸��� ���� ��Ʈ üũ �� �����α� ���°� ����
#ifdef SERV_PERMIT_PORT_CHECK
	std::set<std::string>					m_PortCheckMoritoringServerIPList;	// ���� ���� ��Ʈ�� üũ�ϴ� ����͸� ������ ������ ������
#endif SERV_PERMIT_PORT_CHECK
	//}}
	//{{ 2011. 04. 13  ���� �� �ŷ� ���� �ǽð� SMS ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE_SMS
	bool										m_bMonitoringLetterAndTradeSMS;
#endif SERV_MONITORING_LETTER_AND_TRADE_SMS
	//}} 2011. 04. 13 ���� �� �ŷ� ���� �ǽð� SMS ����
};

extern lua_State* g_pLua;

//{{ 2010. 12. 7	������	���� ���� �ý���
#ifdef SERV_MAIL_SYSTEM
template < class T >
void KBaseServer::QueueingEventToMailManager( unsigned short usEventID, const T& data )
{
	m_kMailManager.QueueingEvent( usEventID, data );
}
#endif SERV_MAIL_SYSTEM
//}}

template < class T >
void KBaseServer::SendToAccountDB( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), GetUID(), PI_ACCOUNT_DB, 0, NULL, usEventID, data );
}

template < class T >
void KBaseServer::SendToLogDB( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#ifdef SERV_ADD_EVENT_DB
template < class T >
void KBaseServer::SendToEventDB( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), GetUID(), PI_EVENT_DB, 0, NULL, usEventID, data );
}
#endif //SERV_ADD_EVENT_DB