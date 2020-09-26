#pragma once


//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST


class KUserHackingManager
{
	enum HACKING_ENUM
	{
		HE_MIN_TIME = 10,
		HE_MAX_TIME = 20,
	};

public:
	KUserHackingManager(void);
	~KUserHackingManager(void);

	void SetRandomCheckTime();
	double GetHackingToolCheckTime() { return m_fHackingToolCheckTime; }

	void GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList );

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	void ClearLetterMonitoringList()										{ m_setRecvLetterList.clear(); }
	void ClearTradeMonitoringList()											{ m_setRecvTradeList.clear(); }
	bool AddLetterMonitoringList( IN const UidType& iForomUnit );
	bool AddTradeMonitoringList( IN const UidType& iForomUnit );
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	bool HackingCheckDeserializeFailCount();
	void IncreaseDeserializeFailCount( IN const u_short usEventID );
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	void SetEDMonitoringMode( bool bEDMonitoringMode )			{ m_bEDMonitoringMode = bEDMonitoringMode; }
	bool GetEDMonitoringMode()									{ return m_bEDMonitoringMode; }
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	bool CheckHackingUserRecivedPacket( OUT char& cLastSendType );
	void SetHackingUserSendType( char cType );
	bool SetHackingUserReciveType( IN char cType, OUT char& cLastSendType );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}

private:
	double				m_fHackingToolCheckTime;
	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	std::set< UidType >				m_setRecvLetterList;
	std::set< UidType >				m_setRecvTradeList;
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	bool							m_bCheckedDeserializeFail;
	int								m_iTotalDeserializeFailCount;
	std::map< u_short, int >		m_mapDeserializeFailList;
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	bool							m_bEDMonitoringMode;
#endif SERV_ED_MONITORING_IN_GAME
	//}}
	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	char							m_cLastSendType;
	boost::timer					m_tHackingUserTimer;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
};


#endif SERV_HACKING_TOOL_LIST
//}}