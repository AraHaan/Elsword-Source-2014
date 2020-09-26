#pragma once

class CX2PVPGameTeamDeath : public CX2PVPGame
{
	public:
		CX2PVPGameTeamDeath(void);
		virtual ~CX2PVPGameTeamDeath(void);

		virtual	void		Release();
		virtual	HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT		OnFrameRender();

		virtual	void		AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual bool		DetermineLastKill();

		virtual void		UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );

	protected:
		void				UpdateScore();

#ifdef MODIFY_PVP_SCORE_UI
		void				SetScoreTexture( int iMyTeamKill_, int iOtherTeamKill_, bool bIsRedTeam_ = true );
#endif //MODIFY_PVP_SCORE_UI

		float									m_fReBirthTime;		/// ������, ǥ�ÿ����δ� GUUser�� ReBirthTime�� �����
		CKTDGFontManager::CUKFont*				m_pFontForReBirth;	/// ��Ȱ �ð� ��¿�
		CKTDGFontManager::CUKFont*				m_pFontForWinKill;	/// ��ǥų ��¿�


		CKTDGPicChar*							m_pPicCharForRedAndBlueKillNum;	/// �������� ��� ���� ų���� ��Ÿ���� ���� ��

		int										m_MaxKillNum;		/// ��ǥ ų��
		float									m_fRemainRebirthTime;	/// ����ϴ� �� ����
		int										m_BeforeRedTeamKill;	/// ������ ų���� ���Ͽ� �޶��� ��� m_pPicCharForRedAndBlueKillNum�� ������Ʈ �����ֱ� ���� �뵵
		int										m_BeforeBlueTeamKill;	/// ������ ų���� ���Ͽ� �޶��� ��� m_pPicCharForRedAndBlueKillNum�� ������Ʈ �����ֱ� ���� �뵵
};
