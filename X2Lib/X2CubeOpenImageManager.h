//////////////////////////////////////////////////////////////////////////
// 1. ������ �Ľ� �� ����
// 2. Get ���
// ... �̷��� �ΰ��� ������ �ǰڳ�
// 3. UI ���� ��ɵ� �־�� �ϰڳ�.

#ifdef CUBE_OPEN_IMAGE_MANAGER

#pragma once

class CX2CubeOpenImageManager{
	

	// ť�� ���� �׸� ����
	struct CubeOpenImageInfo{
		std::set<int>				m_setCubeID;	// ť�� ID ��
		std::vector<std::wstring>	m_vecImageName;	// ť�� ���� �̹����� ex> GACHA_PPORU_05.tga
		std::wstring				m_wstrSoundName;	// ��� ���� �̸� 
		float						m_fChangeTime;	// �̹��� ��ü �ð�
		bool						m_bStringendo;	// ���� ������
		bool						m_bLoop;		// ���� �� ���ΰ�
		int							m_iStrIndexOpen;// ť�� ���� ���� ��Ŭ���� ���� ��Ʈ��. �⺻�� 809
		int							m_iStrIndexNoKey;// ť�� ���� ���� ��Ŭ���� ���� ��Ʈ��. �⺻�� 
		bool						m_bShowResult;		// ��� â ������ ���ΰ�.
		std::map<int, std::wstring> m_mapResultImageInfo;	// ��� ������ ID, �׿� ���� �̹��� �̸�
		bool						m_bHideBar;		// �� ���沨��?
		bool						m_bShowResultCustom;	// ��� Ŀ���� ���� �����ٰų�
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
		bool						m_bImageFadeInOut;
#endif //#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
		CubeOpenImageInfo()
		{
			m_fChangeTime = 0.f;
			m_bStringendo = false;
			m_bLoop			= true;
			m_iStrIndexOpen = STR_ID_EMPTY;
			m_iStrIndexNoKey = STR_ID_EMPTY;
			m_bShowResult = false;
			m_bHideBar	= false;
			m_bShowResultCustom = false;
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
			m_bImageFadeInOut = false;
#endif //#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
		}
	};


public:
	CX2CubeOpenImageManager();
	virtual ~CX2CubeOpenImageManager();

	bool		OpenScriptFile( const WCHAR* pFileName );

	bool		AddImageInfo_LUA();

	// ť�� UI ��� ��Ȳ����
	const bool		IsPlaying() const { return m_bPlaying; }
	void			SetPlaying( bool bVal ) { m_bPlaying = bVal; }

	const bool		IsStart() const { return m_bStart; }
	void			SetStart( bool bVal ) { m_bStart = bVal; }

	const bool		IsSoundPlaying() const { return m_bSoundPlaying;}
	void			SetSoundPlaying( bool bVal ) { m_bSoundPlaying = bVal; }

	bool			IsEventCube( const int iCubeID );

	bool			IsNowResultEventCubePlaying(){ return m_bNowResultEventCubePlaying; }
	void			SetNowResultEventCubePlaying(bool bVal){ m_bNowResultEventCubePlaying = bVal; }

	bool			GetCubeImageName(IN float fProgressOpen, IN const int iCubeID, OUT std::wstring& wstrName);
	bool			GetSoundName(IN const int iCubeID, OUT std::wstring& wstrName);
	bool			GetResultImageName(IN const int iCubeID, IN const int iResultItemID, OUT std::wstring& wstrName);

	const wchar_t*	GetEventCubeOpenString(IN const int iCubeID);
	const wchar_t*	GetEventCubeNoKeyString(IN const int iCubeID);
	bool			IsShowResult(IN const int iCubeID);

	bool			IsHideBarCube(IN const int iCubeID);

	void			AddResultItemID(IN const int iResultItemID ) { m_setResultItemID.insert(iResultItemID); }
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
	bool	        IsFadeInOutImage(IN const int iCubeID);
#endif //RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
	const bool		IsResultItemID(IN const int iItemID);

	bool			IsShowResultCustom( IN const int iCubeID );

private:

	bool			 GetCubeImageInfo( IN const int iCubeID, OUT CubeOpenImageInfo* &pCubeImageInfo_ );
	
	map<int, CubeOpenImageInfo*>		m_mapCubeOpenImageInfo;
	typedef map<int, CubeOpenImageInfo*>::iterator MapCubeOpenImageInfoItor;

	// ť�� UI ��� ��Ȳ����
	int			m_iNowCubeID;
	bool		m_bPlaying;
	bool		m_bStart;
	bool		m_bSoundPlaying;
	bool		m_bNowResultEventCubePlaying;
	bool		m_bShowLastImage;

	std::set<int>	m_setResultItemID;

};


#endif CUBE_OPEN_IMAGE_MANAGER