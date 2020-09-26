#pragma once

class CX2Informer
{

	public:

		enum X2_INFORMER_TYPE
		{
			XIT_SKILL = 0,
			XIT_QUEST,
			XIT_INVEN,
			XIT_CASH_SHOP,
			XIT_GUILD_EXP,
		};

		enum X2_UID_USED_IN_INFORMER
		{
			XUUII_ENOUGH_INVEN = 1,
			XUUII_NEW_ITEM,
			XUUII_GUILD_EXP,
			XUUII_NEW_QUICK_SLOT_ITEM,
//#ifdef CASHSHOP_CLASS_CHANGE_NOTIFICATION
			XUUII_CLASS_CHANGE,
			XUUII_PRESENT,
//#endif CASHSHOP_CLASS_CHANGE_NOTIFICATION
		};

	public:

		CX2Informer(void);
		virtual ~CX2Informer(void);

		X2_INFORMER_TYPE GetType() { return m_InformerType; }

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); //��ũ�ڽ� ����� �� ���� Ȯ�� �� ��ȿȭ

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

		virtual void Reset(); // ĳ���� �����ϰ� ó�� ������ ���� �� �� üũ�ϴ� �Լ�
								// ���� ��ų ��������� ���� �Ϸ��� ����Ʈ ���� ����
								
																		//������Ʈ ü���� �� �� ���� üũ�ص� ����Ʈ
		virtual void CheckInfo( bool bStateChange );		//�ٲ� �κ��� �ִٸ� ���� ���� ������Ʈ
																		//�� ���� ������Ʈ���� �˸�â�� Ȱ��ȭ ���־��ٸ� ��ȿȭ
									
		virtual void InvalidInfo();
		virtual void ClearAllInfo() 
		{
			if ( m_pTalkBoxManager != NULL )
				m_pTalkBoxManager->Clear();
		}
		void SetTalkBoxOffsetPos(D3DXVECTOR2 vPos){m_vTalkBoxOffsetPos=vPos;}

		bool GetNeedCheckInfo() const { return m_bNeedCheckInfo; }
		void SetNeedCheckInfo(bool val) { m_bNeedCheckInfo = val; }

	protected:	

		virtual void SetNowInfo() = 0;
		virtual bool IsNowInfoChange() = 0;
		virtual void SetShow( bool bShow );

		X2_INFORMER_TYPE	m_InformerType;

		CX2TalkBoxManager*	m_pTalkBoxManager;
		D3DXVECTOR2			m_vTalkBoxPos;

		bool		m_bCheckInfo;	//�ѹ� �˷ȴ��� �Ⱦ˷ȴ���

		bool		m_bMouseDown;

#ifdef EVENT_SCENE
		bool		m_bSetShowAfterEventSceneEnd;
#endif EVENT_SCENE

		CKTDXDeviceSound*	m_pSoundAlarm;
		//ĳ���� ���¿� ���� �޴� ��ư�� ��ġ�� �޶� ���� ������ ��ġ ���� �߰�
		D3DXVECTOR2			m_vTalkBoxOffsetPos;

		bool m_bNeedCheckInfo;		/// üũ�� �ʿ䰡 �ִ��� ����
};
