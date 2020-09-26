#pragma once
#ifdef SERV_EVENT_BINGO
class CX2UIBingo : public CX2ItemSlotManager
{
public:

	enum UI_BINGO_CUSTOM_MSG
	{
		UBCM_BUTTON_EXPAND	= 52000,
		UBCM_BUTTON_MINIMIZE,
		UBCM_OPEN,
		UBCM_CLOSE,
		UBCM_INITIALIZE,
		UBCM_VALUE_MIX,
		UBCM_LOTTERY,
		UBCM_REWARD_POPUP_OK,
	};
public:
	CX2UIBingo( CKTDXStage* pNowStage );
	~CX2UIBingo(void);

	// ��� �Լ�
	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual bool		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		GetCheckOperationCondition(){return true;}//modal ���¿� ������� ������ �����ֱ� ���� ����

	bool Handler_EGS_BINGO_EVENT_INFO_REQ();
	bool Handler_EGS_BINGO_EVENT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_LOTTERY_REQ();//����̱�
	bool Handler_EGS_BINGO_EVENT_LOTTERY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_MIX_REQ(); //�ڼ���
	bool Handler_EGS_BINGO_EVENT_MIX_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_RESET_REQ();//������ �̿��� �ʱ�ȭ
	bool Handler_EGS_BINGO_EVENT_RESET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_INIT_REQ();//��� ���� �ϼ� �� �ʱ�ȭ
	bool Handler_EGS_BINGO_EVENT_INIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//��� ���� ����
	void SetOpen(bool bVal);										//���� UI Show
	void SetShowEventButton(bool bVal, bool bShowEffect = false);	//���� ���� UI SHOW
	void SetExpandEventButton(bool bVal);							//���� ���� UI Ȯ����� ����

private:
	//��� ��ư
	//UI ����/����
	bool CreateBingoUI();											//���� UI ����
	void UpdateBingoNumber(int iNumberIndex = -1);					//���� ������ ����
	void UpdatePresent( int iPresentIndex = -1);						//���� ��ǰ ����
	void UpdateInfoData(bool bUpdateFunctionButton_ = false);		//������� ���� ���� ����
	void UpdateButtonUI(bool bUpdateFunctionButton_ = false);		//���� UI ��ư Ȱ��ȭ ���� ����

	void NoticePresentGain(bool bIsAllComplete_ = false);			//��ǰ ȹ�� �˸� �˾�â
	void NoticeBingoResult(bool bShow_, bool bIsSucces_ = false);	//���� ����,���� ���� �˸�
	
	void UpdateCompleteLine();										//���� ȹ�� ���� �� üũ

	bool SetBingoNum( byte BingoNum_ );								//1�� ���� ����(ȹ�濩������)
	void SetBingoNumList(const vector<byte>& vecBingoNum_);			//��ü ������ ����(ȹ�濩������)

	void SetAcquiredPresentList(const vector<byte>& vecAcuiredPresentIndex_);		//ȹ�� ��ǰ ����Ʈ
	void SetPresentList(const std::vector< std::pair<int, bool> >& vecPresentInfo_);//��ǰ ���� ����(ȹ�� ���� ����)
	void SetPresentList(const std::vector< int >& vecPresentInfo_);					//��ǰ ���� ����(������ ��ȹ��)
	
	//�ؽ��� ����
	void SetPresentTex(int iIndex);									//��ǰ �ؽ��� ����
	void SetNumberTex(int iIndex, bool bShowEffect = false);		//������ ���� �ؽ��� ����
	void GetNumberTextrueAndKeyName( OUT wstring& NumberTextureName_, OUT wstring& NumberKeyName_, 
		IN int iBingoValue_, IN bool bIsGain_) const;				//������ ���� ȹ�ξ��¿� ���� �ؽ��ĸ� ���

	//���� ���� ȹ�濩�� ����
	inline byte	GetBingoNum(int iIndex) const;						//ȹ�� ���θ� ǥ���ϴ� +100 ���� ����
	inline bool	GetIsGainNum(int iIndex) const;						//�ε����� ȹ����·� ����
	inline int GetCountOfCheckedNumber() const;						/// üũ�� ������ ���� ������

	inline byte GetNumExceptGainState(byte ByteNum_) const;			//ȹ�� ���� ������ ���� ���� �� ���

	const int GetBingoIndexByNum(byte byteNum_) const;				//���ڸ� ���� ���������� �ε��� ���

	//UI����
	CKTDGUIDialogType	m_pDLGEventOpenButton;						//�����ϴ� ���� ��ư ���̾�α�
	CKTDGUIDialogType	m_pDLGBingo;								//���� ���̾�α�
	CKTDGUIDialogType	m_pDLGRewardPopup;							//���� �˸� �˾�

	CKTDGParticleSystem::CParticleEventSequenceHandle m_hBingoOpenButton; //���� ���� ��ư ��ƼŬ
	//����Ÿ ����
	static const int	m_iMaxNumPresent = 12;						//��ǰ �ִ� ����
	static const int	m_iMaxNumBingoValue = 25;					//������� �ִ� ����
	
	int					m_iLotteryChange;							//���� ��ȸ 
	int					m_iLotteryRecord;							//����� ���� ��ȸ

	int					m_iCompleteLine;							//�ϼ� ���� ���� ��
	int					m_iUncompleteLine;							//���� ���� ���� ��

	int					m_iCheckedNumCnt;							//üũ�� ���� ����
	int					m_iResetChance;								//Ư�����(�ʱ�ȭ) ����
	int					m_iMixChance;								//Ư�����(�ڼ���) ����

	int					m_iPreAcuireNumber;							//�ֱٿ� ����̱�� ȹ���� ����


	std::pair<int, bool> m_arrayPresentData[m_iMaxNumPresent];		//<������ID, ��ǰȹ�濩��>��ǰ ����
	byte				 m_arrayBingoValue[m_iMaxNumBingoValue];	//���� ����						

	bool				m_bShowBingoUI;								//������ UI Show����
	bool				m_bIsExpandButton;							//�̺�Ʈ ��ư�� Ȯ�� ����

};
#endif //SERV_EVENT_BINGO