#pragma once


#ifdef FIRST_GAME_LOADING_THREAD_TEST

	typedef void (*PREENDTHREAD_CALLBACK)(DWORD dwUserParam_, void* pFileVerifierObject_);

	class CX2StateLoadingThread : public KJohnThread
	{
	public:
		CX2StateLoadingThread(void);
		virtual ~CX2StateLoadingThread(void);

		/// @note   override KJohnThread::RunThread()
		virtual DWORD RunThread();

		/// end thread �ݹ��Լ��� �����ϰ�, thread�� �����Ѵ�.
		/// @param  pPreEndThreadCallback_: [in] RunThread() �� �����ϱ� ������ ȣ���ϴ� ����� �Լ�
		/// @param  dwUserParam_: [in] pPreEndThreadCallback_�� ù��° �Ķ���ͷ� ���޵Ǵ� ��
		/// @return bool: 
		bool BeginThreadWithPreEndCB(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

		/// thread�� �ʿ��� event ��ü�� �����Ѵ�.
		void _InitializeObject();

		/// thread�� ���� event ��ü�� �ı��Ѵ�.
		void _FinalizeObject();

		/// thread�� �����ϱ� ������ ȣ���ϴ� ����� ���� �ݹ� �Լ��� �����Ѵ�.
		/// @param  dwUserParam_: pPreEndThreadCallback_()�Լ��� ù��° �Ķ���ͷ� ���޵Ǵ� custom ��
		/// @note   pPreEndThreadCallback_�� �ι��� �Ķ���ʹ� �� �Լ��� ȣ���� ��ü�� ���� �ּ��̴�.
		///         �� this�� ���޵ȴ�.
		void _SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

		/// thread�� suspend ��Ų��.
		void SuspendThread();

		/// thread�� resume��Ų��.
		void ResumeThread();

		/// thread�� suspend�Ǿ� �ִ� ��� true�� �����Ѵ�.
		bool IsThreadSuspended() const { return m_bThreadSuspended; }

		bool GetLoadCompleted() const { return m_bLoadCompleted; }

	private:
		HANDLE                  m_hThreadEvent;         ///< thread�� suspend/resume�� ���� event
		bool                    m_bThreadSuspended;
		MemberCriticalSection   m_cs;                   ///< CRITICAL_SECTION�� wrapper
		DWORD                   m_dwThreadUserParam;    ///< 'm_fpPreEndThreadCallback'�� ù���� �Ķ����
		PREENDTHREAD_CALLBACK   m_fpPreEndThreadCallback;   ///< thread�� �����ϱ� ������ ȣ���ϴ� ����� ���� �ݹ� �Լ�
		bool                    m_bForceExistThread;    ///< thread�� ������ ������Ѿ� �ϴ� ��� �� ���� ������ �����.
		/// checksum �˻簡 �Ϸ����� �ʾ�����, ����ڰ� ������ ���������� ��� ����Ѵ�.

		bool					m_bLoadCompleted;
	};

#endif FIRST_GAME_LOADING_THREAD_TEST


class CX2StateLoading : public CX2State
{
	public:
		CX2StateLoading(void);
		virtual ~CX2StateLoading(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();


		void SetLoadingGageBar( int iLoadingPercent );
		void SetLoadingString( const WCHAR* msg );
		void DropItemLoading();

	protected:
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



#ifdef NO_MORE_ERROR_TEST
		void CheckOutPossibleError();
#endif NO_MORE_ERROR_TEST

#if defined( SERV_HACKING_TOOL_LIST )
//#ifdef	ADD_SERVER_GROUP
//		bool OpenScriptServerGroupFile();
//#endif
		bool ConnectToChannelServer();
		bool Handler_ECH_VERIFY_ACCOUNT_REQ();
		bool Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_ECH_DISCONNECT_REQ();
		bool Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		bool Handler_ECH_NEW_SERVER_SN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

		//bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
		//bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
		CKTDGUIDialogType								m_pDLGLoadingFront;
		float											m_fLogoTime;

		//# KOG & Nexon �պ��ΰ�
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCompanyLogo;
		//# ���� �ΰ� ���
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hLogoBack;
#ifndef DELIBERATIONLOGO_SKIP
		//# ���ǵ�� �ΰ� 
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hDeliberationLogo;
#endif DELIBERATIONLOGO_SKIP


		//{{AFX
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNexonLogo;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hKOGBack;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hKOGLogo;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hKOGLogoMini;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNexonLogoMini;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPPoruSurprise;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPPoruLogo;

		//CKTDGXMeshPlayer::CXMeshInstance*				m_pWillium;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pElSword;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pLena;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pAisha;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pBenders;

		//CKTDXDeviceTexture*								m_pTextureTestMark;
		//}}AFX


#ifdef FIRST_GAME_LOADING_THREAD_TEST
		CX2StateLoadingThread*			m_pStateLoadingThread;
		bool							m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ;
#endif FIRST_GAME_LOADING_THREAD_TEST

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		bool							m_bFirstVerifyAccount;
#endif

#ifdef ADD_NEW_YEAR_SOUND
		bool							m_bPlayLogoSound;
#endif

};
