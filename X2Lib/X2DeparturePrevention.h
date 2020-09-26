/**@file	X2DeparturePrevention.h
   @breif	��Ż ���� ���� �ý��� 
*/

#pragma once

#ifdef DEPARTURE_PREVENTION_SYSTEM

/** @class : CX2PremuimBuff
	@brief : ������ ���� �������� ���� ���� �� ȭ�鿡 UI ǥ��
	@date  : 2011/05/31
*/
class CX2DeparturePrevention
{
public:
	// public variable start
	enum STATE_DEPARTURE_PREVENTION_PREV_STATE
	{
		SDPPS_NONE				= 0,
		SDPPS_EXIT				= 1,
		SDPPS_CHARACTER_SELECT	= 2,
		SDPPS_SERVER_SELECT		= 3,
	};

	enum STATE_DEPARTURE_PREVENTION_UI_CUSTOM_MSG
	{
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_EXIT	= 51000,	/// ��Ż ���� ���� ���� ��ȭ���� ������ �̺�Ʈ
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_ONE		= 51001,	/// ù ��° ���� ����
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_TWO		= 51002,	/// �� ��° ���� ����
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_THREE	= 51003,	/// �� ��° ���� ����
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_FOUR	= 51004,	/// �� ��° ���� ����
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_FIVE	= 51005,	/// �ټ� ��° ���� ����

		SDPRUCM_DEPARTURE_PREVENTION_NOREWARD_EXIT	= 51006,	/// ���� �����ʰ� ����

		SDPRUCM_DEPARTURE_PREVENTION_RESULT_OK		= 51007,	/// ���� �Ϸ�(�������� ���ư�)
	};
	// public variable start

public:
	// public function start
	CX2DeparturePrevention(void);
	~CX2DeparturePrevention(void);

	//{{ �̺�Ʈ ó��
	bool UICustomEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	//{{ ������ �� ó��
	HRESULT OnFrameMove( IN double fTime, IN float fElapsedTime );
	//}}

	//{{ ������ ���
	bool Handler_EGS_OUT_USER_RETAINING_REQ();
	bool Handler_EGS_OUT_USER_RETAINING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_RETAINING_SELECT_REWARD_REQ( IN int iVal_ );
	bool Handler_EGS_RETAINING_SELECT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UIServerEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	void SetPrevState( IN STATE_DEPARTURE_PREVENTION_PREV_STATE eVal_ );
	// public function end

private:
	// private function start
	void Clear();
	void UpdateRewardUI( IN KEGS_OUT_USER_RETAINING_ACK &kEvent_ );
	void QuitGame();
	// private function end

private:
	// private variable start
	CKTDGUIDialogType	m_pDLGLeaveUserReward;		/// ��Ż ���� ���� ���� ��ȭ����
	CKTDGUIDialogType	m_pDLGLeaveUserResult;		/// ��Ż ���� ���� ���� ��� ��ȭ����
	CKTDGUIDialogType	m_pMsgBoxReQuitGame;		/// ���� �ʰ� ���� ����

	STATE_DEPARTURE_PREVENTION_PREV_STATE m_ePrevState;		/// ��Ż ���� Ȯ�� �� ��û ����
	// private variable start
};

#endif

