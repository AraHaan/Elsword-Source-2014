#pragma once

class CX2GUUser;
class CX2ComboManager
{
	public:

		CX2ComboManager( float fComboIntervalTime );
		~CX2ComboManager(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
//{{ robobeg : 2008-10-28
		//HRESULT OnFrameRender();
//}} robobeg : 2008-10-28

		void	SetMyUnit( bool bMyUnit, CX2GUUser*	pUnit );
#ifdef ELSWORD_SHEATH_KNIGHT
		int		AddCombo();
#else
		void	AddCombo();
#endif ELSWORD_SHEATH_KNIGHT
		int		SetCombo(int comboNum) { m_ComboNum = comboNum; }
		int		GetCombo() { return m_ComboNum; }

		vector<int>&	GetComboCount()		{ return m_vecComboCount; }
		void			ResetComboCount()	{ m_vecComboCount.clear(); }
		int				GetComboScoreFull()	{ return m_ComboScoreFull; }


		void ResetMyComboDamage() { m_iMyComboDamage = 0; }
		void AddMyComboDamage(float fDamage) 
		{ 
			m_iMyComboDamage += (int)fDamage;
			if( m_iMyMaxComboDamage < m_iMyComboDamage )
			{
				m_iMyMaxComboDamage = m_iMyComboDamage;
			}
		}
		unsigned int GetMyComboDamage() { return m_iMyComboDamage; }


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		int				GetSubStageComboScore() const;		// ���� ���� ���������� �޺� ���ھ ��ȯ��
		void			ResetSubStageComboScore (); 

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

	protected:
		void ClearParticleSeqGoodSeries();
		FORCEINLINE int	MultiplyWeights( int iScore_ );


	private:
		void			Verify();
		bool					m_bMyUnit;
		CX2GUUser*				m_pUnit;
		KProtectedType<int>		m_ComboNum;

		float					m_fComboIntervalTime;
		float					m_fTime;
		float					m_fEffTime;

		WCHAR					m_ComboContent[256];

		vector<int>				m_vecComboCount;	//Dungeon result ����� ����

		CKTDGPicChar*										m_pPicChar;
		CKTDGPicChar*										m_pPicCharBig;
		/*
			[0] : Good(5)		[8] : Mavelous(85)
			[1] : Nice(10)		[9] : Gorgeous(100)
			[2] : Great(15)		[10] : Honorable(125)
			[3] : Delux(20)		[11] : Monstrous(150)
			[4] : Superb(30)	[12] : Evil(200)
			[5] : Superior(40)
			[6] : Awesome(55)
			[7] : Excellent(70)
		*/
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hComboDamage;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hInterjection[13];

		KProtectedType<float>	m_fComboDamage;		/// ��� ���� ���� �޺� ������ ��
		float					m_fComboCount;		/// �ߺ� ��� ���Ѱ� ���� ������ �� ��ƼŬ ����� ���� ����

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hScoreBonus;
		CKTDGPicChar*										m_pScoreBonusPicChar;
		KProtectedType<int>									m_ComboScore;
		KProtectedType<int>									m_ComboScoreFull;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hGoodBonus;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNiceBonus;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCoolBonus;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hGreatBonus;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPerfectBonus;

		unsigned int			m_iMyMaxComboDamage;
		unsigned int			m_iMyComboDamage;
		CKTDGPicChar*			m_pPicCharComboDamage;
		CKTDGPicChar*			m_pPicCharComboMaxDamage;

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		KProtectedType<int>									m_iUsingSubStageComboScore;		//SubStage �� ComboScore �� ����ϱ� ���� ����ϴ� ���ڰ�
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM



};
