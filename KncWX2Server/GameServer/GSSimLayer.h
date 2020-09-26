#pragma once
#include "SimLayer.h"
#include "X2Data/XSLMain.h"

class KGSSimLayer : public KSimLayer
{
    NiDeclareRTTI;
    DeclDump;
    DeclToStringW;

	//{{ 2011. 04. 04  ��μ�   LFH
#ifdef SERV_LFH
	#define HEAP_SIZE 1024
#endif SERV_LFH
	//}}

public:

    enum SERVER_GROUP_NUM
    {
        SINGLE_SERVER_GROUP = 1,
        DOUBLE_SERVER_GROUP = 2,
    };

    KGSSimLayer(void);
    virtual ~KGSSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua();

	//{{ ��ȫ�� [2012.05.16] ���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
		std::string GetStrItemLuaName();
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
		std::string GetStrItemTransLuaName();
#endif // SERV_ITEM_LUA_TRANS_DEVIDE
		std::string GetStrFilterLuaName();
		std::string GetStrPvpNpcDataLua();
#endif SERV_UNITED_SERVER_EU
	//}}
			void SendNotMSG( IN char cNotifyType, IN const std::wstring& wstrNotMSG, IN int iCount );
			void SendNotMSG_GRP( const char* szNotMSG, int iCount = 1 );
			//{{ 2010. 05. 10  ������	�������� ������ ����
			void SendNotMSG_ALL( const char* szNotMSG, int iCount = 1  );
			//}}
			void AddIP( const char* szAddIP );
			bool CheckIP( const char* szIP );

			void AddRepeatEvent( u_short usEventID, int iCnt );

			//{{ 2009. 9. 1  ������		IOCP�������
#ifdef ABNORMAL_REASON
			void AddAbnormalDisconnectReason( DWORD dwReason );
			void SendToDBAbnormalReason( bool bFinal );
#endif ABNORMAL_REASON
			//}}
			//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
			bool GetUDPKickOff() { return m_bUDPKickUserOff; }
			void SetUDPKickOff( bool _bUDPKickOff) { m_bUDPKickUserOff = _bUDPKickOff; }
#endif UDP_CAN_NOT_SEND_USER_KICK
			//}}
			//{{ 2009. 12. 15  ������	��������
	virtual void DumpToLogFile();
			//}}
	//{{ 2011. 04. 04  ��μ�   LFH
#ifdef SERV_LFH
	void		 Enable_LFH();
	void		 Dump_Blocks_In_All_Heaps();
#endif SERV_LFH
	//}}
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	void		 TickDump_LUA();
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

#ifdef SERV_USE_XTRAP
	void ReloadXTRAP_MAP();
#ifdef SERV_XTRAP_MAP_LOADING_AUTOMATION
	int GetMapQuantity() { return m_setLoadedMaps.size(); }
	char* GetAllMapPointer() { return &m_vecUsingMap[0]; }
#endif //SERV_XTRAP_MAP_LOADING_AUTOMATION
#endif //SERV_USE_XTRAP

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	void DeleteCommonFlag_AllGS_LUA( DWORD dwFlag );
	void AddCommonFlag_AllGS_LUA( DWORD dwFlag );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

    void SetServerGroupNum( int iGroupNum );
    int GetServerGroupNum();

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
			void SetTimeControlItem_Info(std::map< int, std::vector<KPacketGetItemOnOff> > _maptimeControl_Item) { m_map_TimeControl_Item = _maptimeControl_Item; }
			std::map< int, std::vector<KPacketGetItemOnOff> > GetTimeControlItem_Info() { return m_map_TimeControl_Item; }
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	void SetTimeControlCubeInItemMapping_Info(std::map< int, std::vector<KRandomItemMappingToInfoServer> > _maptimeControl_CubeInItemMapping) { m_map_TimeControl_CubeInItemMapping = _maptimeControl_CubeInItemMapping; }
	std::map< int, std::vector<KRandomItemMappingToInfoServer> > GetTimeControlCubeInItemMapping_Info() { return m_map_TimeControl_CubeInItemMapping; }
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

protected:
    std::vector< std::string >		m_vecIP;

	//Ŭ���̾�Ʈ���� ������ ����� ��Ŷ�� �ߺ����� ���ƿ��� ��Ŷüũ�� ����
	//1 arg : event id
	//2 arg : repeat count
	std::map< u_short, int >		m_mapRepeatEvent;

	//{{ 2009. 9. 1  ������		IOCP�������
#ifdef ABNORMAL_REASON
	std::map< DWORD, int >			m_mapAbnormalDisconnectReason;
	boost::timer					m_tAbnormalReasonTimer;
#endif ABNORMAL_REASON
	//}}	
	//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	bool							m_bUDPKickUserOff;
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

#ifdef SERV_USE_XTRAP
#ifdef SERV_XTRAP_MAP_LOADING_AUTOMATION
	mutable KncCriticalSection      m_csXTrapReload;
	boost::timer					m_tXTrapReloadTimer;
	std::set<std::vector<char> >	m_setLoadedMaps;	// ���� �ö� �ִ� ������ Ȯ���� ���� ����
	std::vector<char>				m_vecUsingMap;		// ���� �޸𸮿� �ö󰡴� ����
#endif //SERV_XTRAP_MAP_LOADING_AUTOMATION
#endif //SERV_USE_XTRAP
    int m_iServerGroupNum;
	
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	std::map< int, std::vector<KPacketGetItemOnOff> > m_map_TimeControl_Item;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	std::map< int, std::vector<KRandomItemMappingToInfoServer> > m_map_TimeControl_CubeInItemMapping;
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
};

DefKObjectInline( KGSSimLayer, KSimLayer );
