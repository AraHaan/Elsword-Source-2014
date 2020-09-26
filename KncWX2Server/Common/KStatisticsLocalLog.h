#pragma once

#include "Statistics.h"
#include "Enum/Enum.h"

class KStatisticsLocalLog
{
public:
	enum ELocalLogSpirit
	{
		Max_level = 80,
	};

	enum KLocalLog
	{
		LOG_PVP = 0,
		LOG_PVP_ROOM,
		LOG_DUNGEON_ROOM,
		LOG_TITLE,
		LOG_DUNGEON,
		//{{ 2010. 9. 14	������	�� �ý���
#ifdef SERV_PET_SYSTEM
		LOG_PET_SUMMON,
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
		LOG_SERV_DISCONNECT,
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
		//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
		LOG_BILLING_PACKET,
#endif SERV_BILLING_PACKET_LOG
		//}}
		//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
		LOG_ABUSER_MORNITORING,
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
		//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
		LOG_UNDEFINED_QUEST_TEMPLET,
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

        LOG_MAX,
	};

	struct KLocalLogInfo
	{
		int				m_iStatisticsID;
		std::string		m_strLogFileName;
		std::wfstream	m_wfsLocalLog;

		void Init( int iID, const char* pFileName )
		{
			m_iStatisticsID	 = iID;
			m_strLogFileName = pFileName;
		}
	};

public:
	KStatisticsLocalLog();
	virtual ~KStatisticsLocalLog();

	//{{ 2008. 10. 27  ������	�ڵ� ����
	void InitLocalLogInfo();
	void InitLocalLog( KLocalLog eEnum );
	std::wfstream& LocalLogStm( KLocalLog eEnum );
	//}}

	void FlushLocalLogStm();
	void ClearLocalLogData();
	void CloseLocalLogData();

	void WriteLocalLogData( std::wfstream& fout );
	void WriteLocalLogHead( std::wfstream& fout, int iStatisticsID );
	
	void ClearLocalLog();

	void LoadLocalLogFromFile();

	//////////////////////////////////////////////////////////////////////////
	// ��ϸ� ��ŷ - �α��� ���� �����̶� ���� �Ǿ�����
	void InitLocalLog_Henir_Ranking( int iRankingType );
	std::wfstream& LocalLogStm_Henir_Ranking( int iRankingType );
	void CloseHenirRankingLogData( int iRankingType );
	void DeleteHenirRankingLog();
	//////////////////////////////////////////////////////////////////////////
	
private:
	// ���� ���
	void LoadLocalLogFromFile( int iStatisticsID, const char* strFileName, unsigned int uiLastIndex );
	void FlushLocalLogStm( int iStatisticsID, const char* strFileName );

protected:
	//{{ 2008. 10. 27  ������	�ڵ� ����
	KLocalLogInfo					m_kLocalLog[LOG_MAX];
	//}}

	//{{ 2009. 7. 7  ������		��ŷ����	
	std::wfstream					m_wfsLocalLog_Henir_Ranking[SEnum::RT_MAX_NUM];
	//}}
};



