#pragma once

#ifdef COUPON_SYSTEM

class CX2UICouponBox
{
public:
	enum COUPON_TAB_TYPE
	{
		CTT_AVAILABLE = 0,		/// �� - ��� ������ ����
		CTT_HAVE_BEEN_USED,		/// �� - ���� ��� ����
		CTT_REGISTER,			/// �� - ���� ���
	};

	enum COUPON_BOX_COUSTOM_UI_MSG
	{
		CBCUM_COUPON_BOX_CLOSE = 600,			/// �ݱ�
		
		CBCUM_SELECT_COUPON_TAB_AVAILABLE,		/// �� - ��� ������ ����
		CBCUM_SELECT_COUPON_TAB_HAVE_BEEN_USED,	/// �� - ���� ��� ����
		CBCUM_SELECT_COUPON_TAB_REGISTER,		/// �� - ���� ���
		
		CBCUM_SELECT_COUPON_LIST_TAB_ALL,
		CBCUM_SELECT_COUPON_LIST_TAB_FREE_CHARGE,
		CBCUM_SELECT_COUPON_LIST_TAB_DISCOUNT,

		CBCUM_PUSH_DOUBLE_LEFT_BUTTON,
		CBCUM_PUSH_SINGLE_LEFT_BUTTON,
		CBCUM_PUSH_DOUBLE_RIGHT_BUTTON,
		CBCUM_PUSH_SINGLE_RIGHT_BUTTON,

		CBCUM_BUTTON_REGISTER_COUPON,			/// ���� ��ȣ ��� ��ư
		CBCUM_BUTTON_USE_COUPON,				/// ���� ���
	};

	struct CouponInfo
	{
		//SEnum::NEXON_COUPON_BOX_TYPE m_eCouponBoxType;  // ����
		wstring m_wstrCouponName;	// ���� ��
		int		m_iCouponCardNo;	// ���� ��ȣ

		// ����, ��¥ �� ������ ���߱� ���� ���ڿ��� �۾� ��.
		wstring m_wstrCouponBoxType;	// ����
		wstring m_wstrDiscountValue;	// ���� ����
		wstring m_wstrObtainDataTime;	// ���� ��
		wstring m_wstrExpireDataTime;	// ���� ��
		wstring m_wstrUsedDateTime;		// ��� ��

		bool	m_bIsInstantlyUse;		// ��� ��� ���� ����

		wstring m_CouponTargetItemDesc;	// ���� ���� ������ ���� ����
		
		CouponInfo():
		m_iCouponCardNo(-1),
		m_bIsInstantlyUse(false)
		{}

		CouponInfo( const KClientCouponBox& sKClientCouponBox_, bool bUsed_, bool bIsInstantlyUse_ )
		{
			m_iCouponCardNo = sKClientCouponBox_.m_iCouponCardNo;// ���� ��ȣ
			m_wstrCouponName = sKClientCouponBox_.m_wstrContractDetailName;// ���� ��
			LimitLengthCouponName( m_wstrCouponName, bUsed_ );
			m_wstrObtainDataTime = sKClientCouponBox_.m_wstrObtainDateTime; // ������
			if( 10 < m_wstrObtainDataTime.size() )
			{
				m_wstrObtainDataTime.erase( wstring::size_type(10));
			}

			if( true == bUsed_ )// ���� ��� ����
			{
				m_wstrUsedDateTime = sKClientCouponBox_.m_wstrUsedDateTime; // ��� ��
				if( 10 < m_wstrUsedDateTime.size() )
				{
					// ��¥ ���� �ڸ���
					m_wstrUsedDateTime.erase( wstring::size_type(10));
				}
			}
			else// ��� ���� ����
			{
				m_wstrDiscountValue = sKClientCouponBox_.m_wstrDiscountValue;//���� ����
				m_wstrExpireDataTime = sKClientCouponBox_.m_wstrExpireDateTime; // ��� ���� ��
				if( 10 < m_wstrExpireDataTime.size() )
				{
					// ��¥ ���� �ڸ���
					m_wstrExpireDataTime.erase( wstring::size_type(10));
				}
			}

			// ���� / ���� ���� 
			m_bIsInstantlyUse = bIsInstantlyUse_;
			if( true == m_bIsInstantlyUse ) // ��� ��� ������ ��� ���� : ����
				m_wstrCouponBoxType = GET_STRING( STR_ID_27302 );
			else
				m_wstrCouponBoxType = GET_STRING( STR_ID_27303 );
		}


		void LimitLengthCouponName( OUT wstring& wstrCouponName_, bool bUsed_ );

	};

public:
	CX2UICouponBox();
	~CX2UICouponBox();

	void SetOpen( const bool bShow_ );
	HRESULT OnFrameMove(double fTime, float fElapsedTime) {}
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {}

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetNowState( CX2State* pState ) {}

	CX2UICouponBox::COUPON_TAB_TYPE GetCouponTabType() const { return m_eCouponTabType; }
	bool SetCouponTabType(CX2UICouponBox::COUPON_TAB_TYPE val);

	SEnum::NEXON_COUPON_BOX_TYPE GetCouponBoxType() const { return m_eCouponBoxType; }
	bool SetCouponBoxType(SEnum::NEXON_COUPON_BOX_TYPE val);

	// ���� ��� ����
	void Handler_COUPON_LIST_REQ();
	bool Handler_COUPON_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ���� ���
	void Handler_REGISTER_COUPON_REQ( const WCHAR* wszCouponNumber );
	bool Handler_REGISTER_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ���� ���
	void Handler_USE_COUPON_REQ( const UINT uiColumn );
	bool Handler_USE_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	void Initialize();
	void RegisterLuaBind();

	void UpdateUI();
	void SetShowStaticForm( const bool bShowAbleToUse_, const bool bShowHaveBeenUsed_, 
		const bool bShowRegister_, const bool bShowFrameAvalable_, const bool bShowFrame_HaveBeenUsed_, const bool bPageNum_ );

	void SetShowButtonForm( const bool bShowArrow_, const bool bShowRegister_, const bool bPageNum_, const bool bDesc_ );
	void SetShowRadioButtonForm( const bool bShowListType_ );
	void SetEnableImeEditBox( const bool bEnable_ );
	void SetShowAndCouponNum(  const bool bShow_, const UINT uiCouponNum_ = 0 ); // ���� ����

	void RegisterCoupon();// ���� ��ȣ ��� ��ư
	void SelectPageButton( UINT uiPageNum_, UINT uiMaxPageNum_ );// ������ ����
	void UpdateCouponInfo(); // ���� ��� ����	
	void SetCouponInfo(const CouponInfo& sCouponInfo_, const UINT uiColumn_ );

	void SetNowPageIndex( USHORT uiPageIndex_ );
private:
	CKTDGUIDialogType			m_pDlgCouponBox;
	COUPON_TAB_TYPE				m_eCouponTabType; // ���� ���� �� UI �� Ÿ��

	// ���� �ڽ� Ÿ�� : ��ü/����������/���� ���� �з� 
	SEnum::NEXON_COUPON_BOX_TYPE m_eCouponBoxType;	// ���� �ڽ� Ÿ��

	USHORT						m_uiNowPageIndex;	// ���� : 0
	USHORT						m_uiMaxPage;		// ���� : 0

	const static BYTE			byDoublePageDistance;
	const static UINT			uiCouponNameAbailableLimit;
	const static UINT			uiCouponNameHasBeenUsedLimit;

	vector<CouponInfo>			m_vecCouponInfoList;  // ���� ��� 
	
};
#endif // COUPON_SYSTEM