#pragma once

class CX2TalkBoxManagerImp
{
	protected:

		struct DRAWFACE_RHW_VERTEX
		{
			float x, y, z, rhw;
			DWORD color;
			float u, v;
		};

	public:

		enum TALK_BOX_TYPE
		{
			TBT_FROM_UP_LEFT = 0,
			TBT_FROM_UP_RIGHT,
			TBT_FROM_DOWN_LEFT,
			TBT_FROM_DOWN_RIGHT,
// #ifdef ADDED_RELATIONSHIP_SYSTEM
			TBT_TALKBOX_EFFECT,
// #endif // ADDED_RELATIONSHIP_SYSTEM
			// fix!! �Ʒ��� �ִ� �͵鵵 �������� �����ؾ�
			//TBT_FROM_LEFT_UP,		
			//TBT_FROM_LEFT_DOWN,
			//TBT_FROM_RIGHT_UP,
			//TBT_FROM_RIGHT_DOWN,

		};

		struct TalkBox
		{
			int									m_TalkBoxID;
			int									m_AddLineNum;


			UidType								m_OwnerUnitUID;					// �� 3���� ������ talkbox�� unique�ϰ� indexing
			CX2GameUnit::GAME_UNIT_TYPE			m_GameUnitType;
			bool								m_bTraceUnit;

			wstring								m_wstrTalkContent;		
			D3DXVECTOR2							m_vPos;							// ȭ����� ��ġ, talkbox�� ������ ���κ��� ��ġ
			float								m_fRemainTime;
			TALK_BOX_TYPE						m_TalkBoxType;

			int									m_iFontArticleID;
			int									m_nFontArticleRow;				// �� ��, �� ���� 
			int									m_iFontArticleColumnSize;		// pixel ����

			D3DXVECTOR2							m_vTalkBoxSize;
			D3DXVECTOR2							m_vTalkBoxSizeScale;			// 0~1������ ��
			D3DXVECTOR2							m_vTalkBoxSizeScaleDelta;		// sizescle�� �� ������ ��ȭ��

			float								m_fTalkBoxTipLerpCoef;			// talkbox�� ��� �κп� tip�� ��ġ ����
			bool								m_bShow;
			D3DXCOLOR							m_coTextColor;
			D3DXCOLOR							m_BackTexColor;
			D3DXCOLOR							m_OutLineTextColor;				//{{ ����� : [2009/10/23] // �ܰ��� �÷� �߰�
			bool								m_bUseOutLine;					//{{ ����� : [2009/10/23] // �ƿ����� ���� ���� �߰�

			bool								m_bOverTalkBox;
			bool								m_bExtraVariable;				// ��Ƽ����(User), ����npc(Npc)

#ifdef SERV_PET_SYSTEM
			bool								m_bPetTalk;
#endif

#ifdef NUMBER_TO_LANGUAGE
			bool								m_bTrade;						// ���ϴ� ��ġ�� �̵�(Player, NPC�� ���밡 �ƴ� �ٸ� ��)
#endif NUMBER_TO_LANGUAGE

#ifdef ADD_TALK_BOX_DELAY_TIME
			float								m_fDelayTime;
#endif //ADD_TALK_BOX_DELAY_TIME

#ifdef ADDED_RELATIONSHIP_SYSTEM
			wstring								m_wstrBacklayerTexture;			// �ؽ�ó �̸� 
			float								m_fBacklayerTextureWidth;		// �ؽ�ó �ʺ�
			float								m_fBacklayerTextureHeight;		// �ؽ�ó ����
			D3DXVECTOR3							m_vTalkBoxPosition;				// 
		
#endif // ADDED_RELATIONSHIP_SYSTEM
			TalkBox()
			{
				m_TalkBoxID						= -1;
				m_OwnerUnitUID					= -1;
				m_AddLineNum					= 0;

				m_fRemainTime					= 5.0f;
				m_iFontArticleID				= -1;
				m_nFontArticleRow				= 0;
				m_iFontArticleColumnSize		= 0;

				m_bTraceUnit					= false;
				m_vTalkBoxSize					= D3DXVECTOR2( 0, 0 );
				m_vTalkBoxSizeScale				= D3DXVECTOR2( 0, 0 );
				m_vTalkBoxSizeScaleDelta		= D3DXVECTOR2( 0, 0	);
				m_GameUnitType					= CX2GameUnit::GUT_USER;

				m_TalkBoxType					= TBT_FROM_DOWN_LEFT;
				m_fTalkBoxTipLerpCoef			= 0.5f;
				m_wstrTalkContent				= L"";
				m_bShow							= true;
				m_coTextColor					= D3DXCOLOR( 0, 0, 0, 1 );
				m_BackTexColor					= D3DXCOLOR( 1, 1, 1, 1 );
				m_OutLineTextColor				= D3DXCOLOR( 0, 0, 0, 0 );		//{{ ����� : [2009/10/23] //	�� �ʱ�ȭ. ��� 0�̸� �������� ����
				m_bUseOutLine					= false;

				m_bOverTalkBox					= false;
				m_bExtraVariable				= false;

#ifdef SERV_PET_SYSTEM
				m_bPetTalk						= false;
#endif
#ifdef NUMBER_TO_LANGUAGE
				m_bTrade						= false;
#endif NUMBER_TO_LANGUAGE

#ifdef ADD_TALK_BOX_DELAY_TIME
				m_fDelayTime					= -1.f;
#endif //ADD_TALK_BOX_DELAY_TIME

#ifdef ADDED_RELATIONSHIP_SYSTEM
				m_wstrBacklayerTexture			= L"";
				m_fBacklayerTextureWidth		= 0.f;
				m_fBacklayerTextureHeight		= 0.f;
				m_vTalkBoxPosition = D3DXVECTOR3 ( 0, 0, 0 );
				
#endif // ADDED_RELATIONSHIP_SYSTEM

			}
			TalkBox( const TalkBox& t ) { *this = t; }    // copy constructor

			//{{ ����� : [2009/10/28] //	�ּ�ó��(�⺻������ ���Ե�)

			//TalkBox& operator=( const TalkBox& t )        // assign operator
			//{
			//	m_TalkBoxID = t.m_TalkBoxID;
			//	m_AddLineNum = t.m_AddLineNum;
			//	m_OwnerUnitUID = t.m_OwnerUnitUID;
			//	m_GameUnitType = t.m_GameUnitType;
			//	m_bTraceUnit = t.m_bTraceUnit;
			//	m_wstrTalkContent = t.m_wstrTalkContent;
			//	m_vPos = t.m_vPos;
			//	m_fRemainTime = t.m_fRemainTime;
			//	m_TalkBoxType = t.m_TalkBoxType;
			//	m_iFontArticleID = t.m_iFontArticleID;
			//	m_nFontArticleRow = t.m_nFontArticleRow;
			//	m_iFontArticleColumnSize = t.m_iFontArticleColumnSize;
			//	m_vTalkBoxSize = t.m_vTalkBoxSize;
			//	m_vTalkBoxSizeScale = t.m_vTalkBoxSizeScale;
			//	m_vTalkBoxSizeScaleDelta = t.m_vTalkBoxSizeScaleDelta;
			//	m_fTalkBoxTipLerpCoef = t.m_fTalkBoxTipLerpCoef;
			//	m_bShow = t.m_bShow;
			//	m_coTextColor = t.m_coTextColor;
			//	m_BackTexColor = t.m_BackTexColor;
			//	m_bOverTalkBox = t.m_bOverTalkBox;
			//	m_bExtraVariable = t.m_bExtraVariable;
			//	return *this;
			//}

			//}} ����� : [2009/10/28] //	�ּ�ó��

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
			void Update( CKTDGFontManager::CUKFont* pKTDGFont, const float fMarginLeft, const float fMarginTop, const D3DXVECTOR2 vOffset );
			void ResetFontPos(  CKTDGFontManager::CUKFont* pKTDGFont, const float fMarginLeft, const float fMarginTop );
#else
			void Update( CKTDGFontManager::CKTDGFont* pKTDGFont, const float fMarginLeft, const float fMarginTop, const D3DXVECTOR2 vOffset );
			void ResetFontPos(  CKTDGFontManager::CKTDGFont* pKTDGFont, const float fMarginLeft, const float fMarginTop );
#endif

			void GetTalkBoxPosByUnitUID( IN UidType uiOwnerUnitUID_, OUT D3DXVECTOR3& vPos_ );
#ifdef ADDED_RELATIONSHIP_SYSTEM
			void GetTalkBoxPosByCostomPos( OUT D3DXVECTOR3& vPos_ ) const;
#endif // ADDED_RELATIONSHIP_SYSTEM
		};


	public:
		CX2TalkBoxManagerImp(void);
		virtual ~CX2TalkBoxManagerImp(void);

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		//void Push(TalkBox* talkBox) { Push( *talkBox ); }
		void Push( TalkBox& talkBox );
		bool Delete( UidType unitUID, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );
		bool DeleteByIndex( int index );
		int GetTalkBoxNum() { return (int)m_TalkBoxList.size(); }
		CX2TalkBoxManagerImp::TalkBox* GetTalkBoxByUID( UidType unitUID );
		bool CheckTalkbox( UidType unitUID );
		void Clear();

		void SetPos( UidType unitUID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );
		void SetPos( int talkBoxID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );

		int GetMaxStrLen() { return m_nMaxStrLen; }

		bool CheckMousePointInTalkbox( D3DXVECTOR2 mousePos, UidType& uidType );	
		void SetOffset(D3DXVECTOR2 vPos) { m_Offset = vPos; }

		CKTDGFontManager::CUKFont* GetUKFont() { return m_pUKFont; }

		virtual void SeTOverTalkbox(bool val) { m_bOverTalkBox = val; };

	protected:

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		virtual void DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color = 0xffffffff );
//		virtual void DrawReverseFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color = 0xffffffff );
//#endif
		virtual void DrawBalloon( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
			float fWidthScale, float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		void SetFaceUV( DRAWFACE_RHW_VERTEX arvVertices[4], float nX, float nY, float nWidth, float nHeight, 
			float fU, float fV, float fTexWidth, float fTexHeight, 
			float fClockWiseRotate = 0.f, int iFlipType = 0, D3DCOLOR color = 0xffffffff );

		void DrawBalloonBodyAndTip( int iType, float _fX, float _fY, float fX, float fY, float fCenterWidth, float fCenterHeight, 
			float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor );

		void SetBalloonTip( DRAWFACE_RHW_VERTEX arvVertices[4], int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
			float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor );
//#else
//		virtual void DrawFaceUV( float nX, float nY, float nWidth, float nHeight, 
//								float fU, float fV, float fTexWidth, float fTexHeight, 
//								float fClockWiseRotate = 0.f, int iFlipType = 0, D3DCOLOR color = 0xffffffff );
//
//		void DrawBalloonBody( float fX, float fY, float fCenterWidth, float fCenterHeight, 
//							float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor );
//
//		void DrawBalloonTip( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
//							float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor );
//#endif

#ifdef NUMBER_TO_LANGUAGE
		void LineBreak( int& nRow, int& iColumnSize, wstring& wstrSpeech, bool bTrade );
#else
		void LineBreak( int& nRow, int& iColumnSize, wstring& wstrSpeech );
#endif NUMBER_TO_LANGUAGE

		void InitTalkBox( TalkBox& talkBox );
		void ResetTalkBox( TalkBox& talkBox );

		virtual void MouseMove( D3DXVECTOR2 mousePos );

#ifdef ADDED_RELATIONSHIP_SYSTEM
		void DrawTalkBoxEffectInnerFrame( int iType, float _fX, float _fY, float fX, float fY, float fCenterWidth, float fCenterHeight, 
			float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor );

		void DrawTalkBoxEffect ( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
			float fWidthScale, float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor, wstring backTexName );
#endif // ADDED_RELATIONSHIP_SYSTEM
	protected:

		CKTDXDeviceTexture* m_pTalkBoxTexture;
#ifdef ADDED_RELATIONSHIP_SYSTEM
		CKTDXDeviceTexture* m_pBackEffectTexture;
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		CKTDGFontManager::CUKFont* m_pFont;
#else
		CKTDGFontManager::CKTDGFont* m_pFont;
#endif
		CKTDGFontManager::CUKFont* m_pUKFont;

		vector< TalkBox >	m_TalkBoxList;

		int					m_FontWidth;
		int					m_FontHeight;
//{{ robobeg : 2008-10-13
	    CKTDGStateManager::KStateID m_RenderStateID;
//}} robobeg : 2008-10-13

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		LPDIRECT3DVERTEXBUFFER9	m_pVB;
//		LPDIRECT3DINDEXBUFFER9	m_pIB;
//		float               m__fX_VB;
//		float               m__fY_VB;
//		float               m_fX_VB;
//		float               m_fY_VB;
//		float               m_fCenterWidth_VB;
//		float               m_fCenterHeight_VB;
//		float               m_fWidthScale_VB;
//		float               m_fHeightScale_VB;
//		D3DXCOLOR           m_d3dxBackTexColor_VB;
//#endif


		int					m_OffsetPosX;
		int					m_nMaxStrLen;




		D3DXVECTOR2 		m_Size;					// talkbox�� ũ��, ���ڰ� ������ �κ��� ũ�� rect, ���� pixel???
#ifdef CLIENT_GLOBAL_LINEBREAK
		D3DXVECTOR2 		m_ResSize;					// �ػ󵵿� �°� ����� ������ ( �ؿ��� ���� �극��Ŀ�� �ش� �κ� ����� �ȵǼ� ��ǳ�� �ȿ� ���� ������ �� ���̴� ��찡 �־��� )
#endif //CLIENT_GLOBAL_LINEBREAK
		float				m_fMarginLeft;			// talkbox������ �� ���� ����, ���� pixel
		float				m_fMarginRight;
		float				m_fMarginTop;
		float				m_fMarginBottom;

		float				m_fTalkBoxCornerWidth;
		float				m_fTalkBoxCornerHeight;

		CKTDXDeviceSound*	m_pSoundOver;
		UidType				m_NowOverTalkBoxUnitUID;		

		D3DXVECTOR2			m_Offset;
		bool				m_bOverTalkBox;
		bool				m_bTalkBoxOverImg;

		bool				m_bExtraVariable;
};
