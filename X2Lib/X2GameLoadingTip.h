#pragma once

class CX2GameLoadingTip
{
	struct TipGroup
	{
		int	m_MinLevel;
		int m_MaxLevel;

		vector< wstring > m_vecTextureName;
	};
#ifdef ONE_LINE_TIP_BY_LEVEL
	struct TipGroupStrID
	{
		int	m_MinLevel;
		int m_MaxLevel;
		vector< int > m_vecOneLineTip;
	};
#endif //ONE_LINE_TIP_BY_LEVEL
	public:

		CX2GameLoadingTip();
		virtual ~CX2GameLoadingTip(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
		HRESULT OnFrameRender();

		void CreateLoadingTipUI();
//{{ 2009.01.30 ���¿� : �����ε��� ������ ������ �ϱ�
#ifdef ENTER_SCREEN_FOR_DUNGEON
		void CreateLoadingOneLineTip();

	#ifdef ONE_LINE_TIP_BY_LEVEL
		void SetOneLineTipString( int iStrID_);
	#endif //ONE_LINE_TIP_BY_LEVEL
#endif
//}}

#ifdef REFORM_ENTRY_POINT
		//����, �������� ������ ��ġ�� �޶����� ����
		//�⺻ lua���ϼ����� �������� �ϰ� ������ ��ġ�� ���� ����.
		void SetVillageSetting();
#endif //REFORM_ENTRY_POINT

	protected:

		bool Init();

		TipGroup* GetTipGroup( int level );
		wstring GetRandomTextureName( TipGroup* pTipGroup );

	protected:

		CKTDGUIDialogType		m_pDLGLoadingTip;
		vector< TipGroup* > m_vecTipGroup;

//{{ 2009.01.30 ���¿� : �����ε��� ������ ������ �ϱ�
#ifdef ENTER_SCREEN_FOR_DUNGEON
	#ifdef ONE_LINE_TIP_BY_LEVEL
		vector<TipGroupStrID>	m_vecOneLineTipGroup;
	#else
		vector< wstring > m_vecOneLineTip;
	#endif //ONE_LINE_TIP_BY_LEVEL
#endif
//}}
};
