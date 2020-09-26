#pragma once


class CX2NoticeManager
{
	public:

		struct NoticeData
		{			
			int				m_RepeatCount;
			std::wstring	m_Msg;
			
			NoticeData()
			{				
				m_RepeatCount	= 0;
				m_Msg			= L"";
			}

			NoticeData& NoticeData::operator=( const NoticeData& rhs )
			{
				if ( this == &rhs ) 
				{ 
					return *this; 
				}
				
				m_RepeatCount	= rhs.m_RepeatCount;
				m_Msg			= rhs.m_Msg;

				return *this;
			}

		};

	public:

		CX2NoticeManager( int noticeGapWidth = 150 );
		virtual ~CX2NoticeManager(void);

		bool	AddNotice( KEGS_NOTIFY_MSG_NOT::NOTIFY_TYPE eNotifyType, const WCHAR* pMsg, int repeatCount = 1 );
		HRESULT	OnFrameMove( double fTime, float fElapsedTime );

		void	CloseItemNotice(int iNum);

#ifdef ADD_HERO_MATCH_NOTICE		/// �ӼӸ� ������ �����ؾ� �� �� ���ÿ� ����
		void	SetHeroMatchWisperNotice( bool bHeroMatchWisperNotice_ ) { m_bHeroMatchWisperNotice = bHeroMatchWisperNotice_; }
		bool	GetHeroMatchWisperNotice() { return m_bHeroMatchWisperNotice; }
#endif ADD_HERO_MATCH_NOTICE

	protected:

		void	AddNoticeData();
		void	AddTakeItemNoticeData();

		int		m_NoticeGapWidth;

		std::queue< NoticeData* >				m_queNoticeData;
		std::queue< NoticeData* >				m_queTakeItemNoticeData;
		vector< CX2TextManager::CX2Text* >		m_TextListToDelete;

		//�ѹ��� ������ �� �ִ� �˸� ���� 3���� �ø��� ���� ����
		CKTDGUIDialogType	m_pDlgTakeItemBackBg[MAX_NOTICE_NUM];
		float				m_fTakeItemNoticeTime[MAX_NOTICE_NUM];
		//// �ӽ÷� �ߺ� ���� ���� �� �ְ� 
		//std::vector< SpamNotice >				m_vecSpamNotice;

#ifdef SERV_SHUTDOWN_SYSTEM
		bool	bShutDownNotice;
#endif

#ifdef ADD_HERO_MATCH_NOTICE
		bool	m_bHeroMatchWisperNotice;		/// �ӼӸ� ���� ����
#endif ADD_HERO_MATCH_NOTICE
};
