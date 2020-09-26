#pragma once

class CX2PVPGameSurvival : public CX2PVPGame
{
	public:
		CX2PVPGameSurvival(void);
		virtual ~CX2PVPGameSurvival(void);

		virtual	void		Release();
		virtual	HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT		OnFrameRender();

		virtual	void		AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual bool		DetermineLastKill();

		virtual void		UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );

	protected:

		float									m_fReBirthTime;		/// ������, ǥ�ÿ����δ� GUUser�� ReBirthTime�� �����
		CKTDGFontManager::CUKFont*				m_pFontForReBirth;	/// ��Ȱ �ð� ��� ��
		CKTDGFontManager::CUKFont*				m_pFontForWinKill;	/// ��ǥ ų �� ��� ��

		int										m_MaxKillNum;		/// ��ǥ ų ��
		float									m_fRemainRebirthTime;	/// ��� �ϴ� �� ����
};
