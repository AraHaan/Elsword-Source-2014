#pragma once



#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

// thread safe �� ���������� ���� ���� CKTDXLog ��ü�� 1���� �����ؾ� �Ѵ�.
#define KTDXLOG_MAX_THREAD_NUM 10

#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

class CKTDXLog
{
	public:
		CKTDXLog(void);
		~CKTDXLog(void);

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
        // Ư�� thread ���� �α׸� ������� ���� �� �Լ��� ȣ���ؾ� �Ѵ�, robobeg, 2014-01-14
        bool    RegisterCurrentThread();
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

        // �� �Լ����� thread safe �ϰ� �����Ǿ���, robobeg, 2014-01-14
		void PushStateLog( const WCHAR* statLog );
		void PushErrorLog( int errNum, const WCHAR* errorLog );

		void PushStateLog( const char* statLog );
		void PushErrorLog( int errNum );
		void PushErrorLog( int errNum, const char* errorLog );

		void SetLastErrorLog( const char* statLog );
		void SetLastErrorLog( const WCHAR* statLog );
        const char *GetLastErrorLog();


        // �� �� �Լ��� thread safe ���� �ʴ�. ���α׷� ����� ��� thread �� �۵��� ���߾��ٰ� �����Ѵ�.
		void StateLogToFile( const char* fileName );
		void ErrorLogToFile( const char* fileName, const char* systemInfo = NULL );
        
        //int GetStateLogSize() { return m_StateLog.size(); }
        //int GetErrorLogSize() { return m_ErrorLog.size(); }
        //const char *GetStateLogStr(int inx) { return m_StateLog[inx].c_str(); }
        //const char *GetErrorLogStr(int inx) { return m_ErrorLog[inx].c_str(); }


#if 0
		// ������� �����Ƿ� �����Ѵ�.
		void EnQueLastErrorLog( const WCHAR* errorLog );
		const char *DeQueLastErrorLog();
#endif

	private:

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

        struct  KTLSData
        {
            volatile DWORD      m_dwThreadId;
		    std::deque<std::pair<DWORD,string> >	m_StateLog;
		    std::deque<std::pair<DWORD,string> >	m_ErrorLog;
		    string				m_LastErrorLog;

            KTLSData() : m_dwThreadId(0) {}
        };
   
        KTLSData            m_aTLSData[KTDXLOG_MAX_THREAD_NUM];
		__declspec(thread) static KTLSData*
                            ms_pTLSData;
        volatile DWORD      m_dwTimestamp;

#else   X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

		vector<string>		m_StateLog;
		vector<string>		m_ErrorLog;
		string				m_LastErrorLog;

		//std::queue<wstring>	m_queLastErrorLog;

#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
};

extern CKTDXLog g_CKTDXLog;
