#pragma once

class CX2InformerQuest : public CX2Informer
{
	public:

		struct HouseQuestInfo
		{
			int m_HouseID;

			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeqAccecptableQuestInfoBG;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeqAccecptableQuestInfo;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeqCompleteQuestInfoBG;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeqCompleteQuestInfo;

			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeqAccecptableEventQuestInfoBG;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeqAccecptableEventQuestInfo;

			HouseQuestInfo();
			~HouseQuestInfo();
		};

	public:

		CX2InformerQuest(void);
		virtual ~CX2InformerQuest(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		void Reset();

		void LoadHouseQuestInfo( int villageID );
		void UnLoadHouseQuestInfo();
		void CheckHouseQuestInfo(); //�ǽð����� ���� ����Ʈ�� �ְų� �Ϸ��� ����Ʈ�� �ִ��� Ȯ���ؼ� ǥ�����ش�.
		void UnCheckHouseQuestInfo();

		//{{ kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#ifdef	SERV_DAY_QUEST
		void SetTimedEventQuest( bool bValue ) { m_bTimedEventQuest = bValue; }
		bool IsTimedEventQuest() const { return m_bTimedEventQuest; }
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����

	protected:

		void SetNowInfo();
		bool IsNowInfoChange();
		void SetShow( bool bShow );

#ifdef REFORM_QUEST
		int	 m_iNowInfoChangeQuestID;
		bool m_bIsEverySubQuestInQuestComplete;		/// ����Ʈ���� ��� ��������Ʈ�� �Ϸ� �ߴ���
		std::map< int, std::set<int> > m_mapNowClearSubQuestGroup; //<����Ʈ���̵�, ��������Ʈ �׷� �ε���>
#endif //REFORM_QUEST
		set< int >	m_NowClearQuestID;
		vector< HouseQuestInfo* > m_vecHouseQuestInfo;

		//{{ kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����
#ifdef	SERV_DAY_QUEST
		bool m_bTimedEventQuest;
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // �����ð����� ����Ʈ�� ������Ʈ �Ǵ� �ý��� ����		
};
