#ifdef QUEST_GUIDE
#pragma once
class CX2NPCIndicator
{
public:
	struct GuideTarget
	{
		D3DXVECTOR3 vTargetPos;
		bool bIsNPC;
		bool bIsEpic;

		GuideTarget(): vTargetPos(0,0,0), bIsNPC(false), bIsEpic(false) {}
	};

	struct StartPosInfo
	{
		UINT uiStartPos;
		UINT iMoveMapIndex;
		StartPosInfo(UINT iMoveMapIndex_):
		iMoveMapIndex(iMoveMapIndex_), uiStartPos(0){};
	};

public:
	CX2NPCIndicator(void);
	~CX2NPCIndicator(void);
	void OnFrameMove();
	void ResetGuideTargetInfo();
	void SetShow(bool bVal);
private:	
	void Clear();
	void CreateIndicatorParticle();					
	void SetRotateParticle(int iParticleIndex,const D3DXVECTOR3& vMyUserPos, float fDistance, bool bMove);	//��ƼŬ ȸ�� ����
	void SetShowAllParticle(bool bVal);
	float GetDirVecToDegree( D3DXVECTOR3 dirVec );

	bool GetLinkInfo( OUT map<int, D3DXVECTOR3>& mapVillageInfo_); //���� ��ġ���� �̵� �� �� �ִ� ����, �ʵ忡 ���� ���� ���

	CKTDGParticleSystem::CParticleEventSequenceHandle m_hIndicator[MAX_QUEST_INDICATOR_NUM];			//ȭ��ǥ ��ƼŬ
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hIndicatorMark[MAX_QUEST_INDICATOR_NUM];		//Q��ũ ��ƼŬ
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hPortalQuestion;								//��Ż�� �ٴ� ����Ʈ �Ϸ� ��ƼŬ	

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticleHandle		      m_hParticleIndicator[MAX_QUEST_INDICATOR_NUM];			//ȭ��ǥ ��ƼŬ
	CKTDGParticleSystem::CParticleHandle			  m_hParticleIndicatorMark[MAX_QUEST_INDICATOR_NUM];		//Q��ũ ��ƼŬ
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticle*					  m_pIndicator[MAX_QUEST_INDICATOR_NUM];			//ȭ��ǥ ��ƼŬ
	CKTDGParticleSystem::CParticle*					  m_pIndicatorMark[MAX_QUEST_INDICATOR_NUM];		//Q��ũ ��ƼŬ
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	CKTDGUIDialogType m_pDLGParticle;					//��ƼŬ�� ���̾ ���� �ϱ� ����, ��ƼŬ�� ����� ������ ���̾�α� ����
	
	GuideTarget		m_GuideTargetInfo[MAX_QUEST_INDICATOR_NUM];		//Ÿ�� ���� ��� ����ü
	int				m_iIndicatorCount;	
	float			m_fCheckDistance;								//������ �������� �Ǻ��ϱ� ���� �Ÿ���
	bool			m_bShow; 

};
#endif //QUEST_GUIDE