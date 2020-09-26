#pragma once
class CX2PlayGuide
{
public:
	enum TUTORIAL_QUEST_ID
	{
		TQI_VISIT_RUBEN			= 11000, //������ ������
		TQI_CHASE_THIEF			= 11005, //������ ����
		TQI_SAVE_EL				= 11010, //���� ���Ѷ�		
		TQI_VISIT_RUIN_OF_ELF	= 11030, //������ ������ �ǹ�����( ������ ���� ����)	
	};
	enum PLAY_GUIDE_TYPE
	{
		//���̵� �߰� �� �켱������ �´� ��ġ�� �߰� �Ͽ��� �մϴ�.
		PGT_NONE = -1,
		PGT_LEVEL_UP,				//ĳ������ ������ @�� �Ǿ����ϴ�.
		PGT_OBTAIN_SKILL_POINT,		//��ų ����Ʈ�� @ ȹ�� �Ͽ����ϴ�.
		PGT_DUNGEON_PLAY,			//'���� ����' ��ư�� ���� �ȳ��� ���� ������ �����ϼ���.
		PGT_QUEST_COMPLETE,			//@ ��ư���� �Ϸ�� ����Ʈ�� Ȯ���ϼ���
		PGT_NOVICE_GUIDE_01,		//����Ʈ ȭ��ǥ �������� �̵��Ͽ� ����Ʈ�� �����ϼ���
		PGT_NOVICE_GUIDE_02,		//�������� ȥ���ְ� �������� ��ǥ�� Ȯ���ϼ���.
		PGT_HYPER_MODE,				//@ ��ư���� ������ �� �ֽ��ϴ�.
		PGT_SPECIAL_ATTACK,			//@,@,@,@ ��ư���� ��ų�� ����� �� �ֽ��ϴ�.
		PGT_GO_NEXT_STAGE,			//���� �ı��Ͽ� ���� ���������� �̵��ϼ���.
		PGT_OBTAIN_EQUIP_ITEM,		//@' ��ư���� '�κ��丮'�� ���� ȹ���� ��� Ȯ���� �� �ֽ��ϴ�.
		PGT_OBTAIN_TITLE,			//@' ��ư���� 'ĳ����â'�� ���� ȹ���� Īȣ�� ������ �� �ֽ��ϴ�.
		PGT_HAS_SKILL_POINT,		//@' ��ư���� '��ųâ'�� ���� ȹ���� ��ų ����Ʈ�� ������ �� �ֽ��ϴ�.
		PGT_COMPLETE_EPIC_QUEST,	//��Ư�������̸��� ���� �̵��ϼ���.
		PGT_MAX_TYPE,				
	};

	enum GUIDE_PLACE_TYPE
	{
		GPT_NONE = -1,
		GPT_ALL,			//����, ����, �ʵ�
		GPT_DUNGEON,		//����
		GPT_DUNGEON_FIELD,	//����, �ʵ�
		GPT_VILLAGE_FIELD,	//����, �ʵ�
	};

	enum NOVICE_GUIDE_STEP
	{
		NGS_STEP_NONE	= -1,

		NGS_STEP_0,		//����Ʈ �޴�
		NGS_STEP_1,		//��������Ʈ ��
		NGS_STEP_2,		//��������Ʈ �Ϸ���
		NGS_STEP_3,		//����Ʈ �Ϸ�
		NGS_STEP_4,		//����Ʈ ���� �Ϸ�
		NGS_STEP_5,		//��������
		NGS_STEP_6,		//���� ����
		NGS_STEP_7,		//���� ��û
		NGS_STEP_8,		//������ ����
		NGS_STEP_END,
	};

	enum EPIC_QUEST_GUIDE_STEP
	{
		EQGS_VISIT_NOAS = 0,	//��ƽ��� ȣ�� ���� ���̵�
		EQGS_KILL_TOBU,			//��� ų ���̵�
		EQGS_ENTER_EL_TREE,		//���� ���� ���� ���̵�
		EQGS_ENTER_DUNGEON,		//���� ���� ���̵�(����)
		EQGS_KILL_THIEF_PPORU,	//���� �Ƿ� ų ���̵�
		EQGS_KILL_BORR,			//���� ų ���̵�
		EQGS_QMARK_GUIDE,		//Q��ũ�� ���� �̵��Ͽ� ����Ʈ�� �����ϼ���
	};

	struct PlayGuideData
	{
		PLAY_GUIDE_TYPE		m_eGuideType;
		GUIDE_PLACE_TYPE	m_ePlaceType;
		int					m_eVillageID; //�����̵� ���̵带 ���� ��.

		wstring				m_GuideString;  // �⺻���ڿ��� ���ڸ� ��ģ ���� ���̵�
		bool				m_bShow;		 
		bool				m_bRepeat;		// �ݺ� ��� ����
		int					m_iStrID;		// �⺻ ���ڿ� 
		int					m_iLimitLV;		// ��� ���� ����(�ִ� ����)	

		PlayGuideData();
		void SetPlayGuide(CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType,	CX2PlayGuide::GUIDE_PLACE_TYPE ePlaceType,
			int iStrID, int iLimitLV, bool bRepeat = true);
	};

	class GuideOrderManager
	{
	public:
		void							InsertGuide(const PLAY_GUIDE_TYPE ePlayGuide_);
		void							EraseGuide(const PLAY_GUIDE_TYPE ePlayGuide_);
		void							SetEndCurrentGuide(){ m_bEndCurrentGuide = true;}
		bool							GetGuideListEmpty()const;
		void							ClearGuideList(){m_setGuideList.clear();}
		void							SetInvalideGuide(){ m_eCurrentGuide = PGT_NONE; }
		CX2PlayGuide::PLAY_GUIDE_TYPE	GetNextGuide();

		GuideOrderManager():m_eCurrentGuide(PGT_NONE),m_bEndCurrentGuide(false),m_iShowCount(0)
		{}

	private:
		std::set<PLAY_GUIDE_TYPE>	m_setGuideList;
		PLAY_GUIDE_TYPE				m_eCurrentGuide;
		bool						m_bEndCurrentGuide;
		int							m_iShowCount;
	};
public:
	CKTDGUIDialogType	GetDlgPlayGuide() {return m_pDLGPlayGuide;}	
	PLAY_GUIDE_TYPE		GetCurrentPlayGuide(); //���� Ȱ��ȭ �� ���̵� ���
	bool				GetShowGuideByPlaceType( GUIDE_PLACE_TYPE ePlaceType_);
	void SetPlayGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow, int iVal = -1);
	void SetPlayGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow, wstring str1, wstring str2 = L"", wstring str3 = L"", wstring str4 = L"" );
	void SetVillageID(int eVillageID);
	void SetHide(bool bVal){ m_bHide = bVal; }
	int GetVillageID(){return m_PlayGuide[PGT_COMPLETE_EPIC_QUEST].m_eVillageID;}
	void OnFrameMove( double fTime, float fElapsedTime );
	void ClearPlayGuide();

//Ʃ�丮��
	void SetMouseClickIndex(int val);
	void ResetNoviceGuide();
	UINT GetGuideStep()const {return m_uiCurrentGuideStep;}
	bool GetActiveNoviceGuide()const { return (false == m_bCompleteTutorial) ? m_bActiveNoviceGuide : false;  }
	bool GetShowDungeonMenu()const { return (true == m_bCompleteTutorial) ? true : m_bShowDungeonMenu;}
	void SetCompleteTutorial(bool bVal){m_bCompleteTutorial = bVal;}
	void SetShowDungeonMenu(bool bVal){m_bShowDungeonMenu = bVal;}


	void DeleteQuestGuideParticle();
	void DeleteInputGuideParticle();

	void StartGoEffect();
	void StopGoEffect();
#ifdef GUIDE_FIRST_EPIC_QUEST
	void SetFirstEpicQuestGuide( EPIC_QUEST_GUIDE_STEP GuideStep_);
	void SetShowFirstEpicQuestGuide(bool bShow_);
#endif //GUIDE_FIRST_EPIC_QUEST
	void SetShowPlayGuide(bool bVal_);

	CX2PlayGuide(void);
	~CX2PlayGuide(void);

private:
	void SetShowGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow);
	bool SetGuideString(PLAY_GUIDE_TYPE eGuideType, int iVal);
	bool SetGuideString(PLAY_GUIDE_TYPE eGuideType, wstring str);
	bool SetGuideString(PLAY_GUIDE_TYPE eGuideType, wstring str1, wstring str2, wstring str3, wstring str4);

	bool IsShowGuide();

	void OnFrameMove_SetAlpha( float fElapsedTime );

#ifdef GUIDE_FIRST_EPIC_QUEST
	void OnFrameMove_FirstEpicGuide(float fElapsedTime);
#endif //GUIDE_FIRST_EPIC_QUEST

	//{{Ʃ�丮�� �������� ���Ǵ� ����Ʈ ���̵� 
	void OnFrameMove_Tutorial();
	void CreateQuestGuideParticle();
	void SetShowGuideParticle(bool bVal);
	void CreateInputGuide();
	void SetPostionInputGuide(const D3DXVECTOR3& vPos);
	//}}Ʃ�丮�� �������� ���Ǵ� ����Ʈ ���̵� 


	
	CKTDGUIDialogType	m_pDLGPlayGuide;			// ���̵� ���̾�α� ��ü
	PlayGuideData		m_PlayGuide[PGT_MAX_TYPE];  // ������ ���̵忡���� ����
	float				m_fOnTime;					// ������ �� ��� �ð�
	bool				m_bHide;					
	GuideOrderManager	m_GuideOrderManager;		// ���� ����ؾ��� ���̵� ������
	PLAY_GUIDE_TYPE		m_eOldPlayGuide;			// ���� ��µ� ���̵�


	UINT				m_uiGuideStep;
	UINT				m_uiCurrentGuideStep;
	bool				m_bStart;
	bool				m_bActiveNoviceGuide;		// �ʺ��� ���̵� Ȱ��ȭ ����
	bool				m_bCompleteTutorial;		// Ʃ�丮�� �Ϸ� ����
	bool				m_bShowDungeonMenu;			// �������� Ȱ��ȭ ����(11010����Ʈ�� �Ϸ��ϱ� ���� ��Ȱ��ȭ)


	//{{Ʃ�丮�� �������� ���Ǵ� ����Ʈ ���̵� 
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hIndicator;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hIndicatorMark;
	CKTDGUIDialogType									m_pDLGParticle;
	bool												m_bShowGuideParticle;


	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hInputGuide;
	//}}Ʃ�丮�� �������� ���Ǵ� ����Ʈ ���̵� 


	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleGo;//������ �̵� ���� ���̵�


#ifdef GUIDE_FIRST_EPIC_QUEST
	//������ ����, ���� ���Ѷ� ����Ʈ�� ���� �ʺ��� ���̵� �� 
	CKTDGUIDialogType									m_pDLGUIFirstEpicQuestGuide;
	bool												m_bShowFirstEpicQuestGuide;
	float												m_fEpicGuideDelayTime;
	float												m_fEpicGuideAlphaTime;
#endif //GUIDE_FIRST_EPIC_QUEST

};



