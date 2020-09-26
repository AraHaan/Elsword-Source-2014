#pragma once

//{{ robobeg : 2008-01-05
// ��𼱰� #define DEVICE_TYPE DWORD �� �����ϴ� �ٶ��� �Ʒ��� enum�� �浹�� �Ͼ.
// DEVICETYPE���� �����Ͽ���.
//}} robobeg : 2008-01-05


class CKTDXDevice;
class CKTDXDeviceManager;
typedef std::list<CKTDXDevice*> CKTDXDevicePtrList;

class CKTDXDevice
{
	public:

        friend class    CKTDXDeviceManager;

		enum DEVICETYPE
		{
			DT_TEXTURE,
			DT_SOUND,
			DT_XMESH,
			DT_XSKIN_MESH,
			DT_RENDER_TARGET_TEXTURE,
			DT_XET,
        	DT_NUM,

		//{{ seojt // 2008-10-17, 17:30
		//DT_UNKNOWN, ///< �� �� ���� ������ device type�� �����ϱ� ���� �߰� - jintaeks on 2008-10-17, 17:31
		//}} seojt // 2008-10-17, 17:30
	};

		enum    EDeviceState
		{
			DEVICE_STATE_INIT,
			//DEVICE_STATE_PENDING,
			DEVICE_STATE_LOADING,
			DEVICE_STATE_LOADED,
			DEVICE_STATE_FAILED,
			DEVICE_STATE_MAX
		};//enum    EDeviceState

	public:
		CKTDXDevice( wstring deviceID, DEVICETYPE eDeviceType )
			: m_DeviceType( eDeviceType )
			, m_DeviceID( deviceID )
			, m_Size( 0 )
			, m_bWithoutFile( false )
        	, m_csDeviceState()
			, m_eDeviceState( DEVICE_STATE_INIT )
			, m_RefCount( 0 )
		{}



		virtual HRESULT OnResetDevice(){ return S_OK; }
		virtual HRESULT OnLostDevice(){ return S_OK; }

    bool    LoadDevice(
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		);

#ifdef CHECK_SOUND_LOADING_TIME
	bool    CheckLoadDevice(
		bool& bCreateSound,
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		);
#endif // CHECK_SOUND_LOADING_TIME

	void    RefDevice();

	int GetRefCount() const     { return m_RefCount; }
		int GetSize() { return m_Size; }
		const wstring& GetDeviceID(){ return m_DeviceID; }
		DEVICETYPE GetDeviceType(){ return m_DeviceType; }

	//bool IsLoading(){ return m_bLoading; }

    EDeviceState    GetDeviceState() const { return m_eDeviceState; }
    bool    IsLoaded() const            { return m_eDeviceState == DEVICE_STATE_LOADED; }
    bool    IsLoadedOrFailed() const    { EDeviceState eState = m_eDeviceState; return eState == DEVICE_STATE_LOADED || eState == DEVICE_STATE_FAILED; }
    bool    IsFailed() const            { return m_eDeviceState == DEVICE_STATE_FAILED; }

	bool GetWithoutFile() const { return m_bWithoutFile; }


//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17
    //public:
        // qff
        /** .kom �����̳� ��ũ���� �޸𸮷� ��� ���� �Ŀ� �� �Լ��� ȣ��ȴ�.
            thread ���ο���, local memory�� ���ҽ��� �����ϴ� ���� �۾���
            �� �Լ����� �ϵ��� �����ؾ� �Ѵ�.
            - jintaeks on 2008-10-17, 16:48 */
        //virtual HRESULT     Thread_Load(DWORD /*dwParam_*/){ return S_OK; }

	//virtual HRESULT Load_LuaData(DWORD /*dwParam_*/){ return S_OK; }
	//bool Load_LuaDependentData(DWORD dwParam_)
	//{ 
//          /*virtual*/Load_LuaData( dwParam_ );
//          return true;
	//}//Load_LuaDependentData()
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

//#ifdef  _HACKPROOF_CLIENT_
    //void    Unmark_Request_LuaData()    { m_bMarked_Request_LuaData = false; }
    //void    Mark_Request_LuaData()      { m_bMarked_Request_LuaData = true; }
    //bool    IsMarked_Request_LuaData()  { return m_bMarked_Request_LuaData; }
//#endif  _HACKPROOF_CLIENT_

protected:

	virtual ~CKTDXDevice(void){ _UnLoad(); }
    EDeviceState    PendDevice();
    bool    UnrefDevice();
    bool    UnrefDeviceIfReferedOnce();
	void SetWithoutFile(bool val) { m_bWithoutFile = val; }

	virtual HRESULT     _Load( bool bSkipStateCheck = false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		)         { return S_OK; }

#ifdef CHECK_SOUND_LOADING_TIME
	virtual HRESULT _CheckLoad( bool& bCreateSound,
		bool bSkipStateCheck = false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		, bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD		
		)		{ return S_OK; }
#endif // CHECK_SOUND_LOADING_TIME

	virtual HRESULT     _UnLoad()       { return S_OK; }


protected:


	DEVICETYPE	        m_DeviceType;
                        
	wstring		        m_DeviceID;

		int			m_Size;
		bool		m_bWithoutFile;

    MemberCriticalSection   
                        m_csDeviceState;
    EDeviceState        m_eDeviceState;
	int			        m_RefCount;
};//class CKTDXDevice
