#pragma once


class CKTDXDeviceHolder;
class CKTDXStage : public CKTDXDeviceHolder
{
	public:
		enum STAGE_STATE
		{
			SS_NULL = 0,
			SS_ACTIVE,
			SS_DEACTIVE,
			SS_READY_TO_DELETE,
		};

	public:
		CKTDXStage(){ m_eStageState = SS_ACTIVE; }
		virtual ~CKTDXStage(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime ){ return S_OK; }
		virtual HRESULT OnFrameRender(){ return S_OK; }

		virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }

		virtual HRESULT OnResetDevice(){ return S_OK; }
		virtual HRESULT OnLostDevice(){ return S_OK; }

		STAGE_STATE		GetStageState(){ return m_eStageState; }
		void			SetStageState( STAGE_STATE eStageState ){ m_eStageState = eStageState; }


		virtual void	SetLuaFrameMoveFunc_LUA( const char* pFuncName ) {}

		void			SetStageName( const WCHAR* wszStageName ) { m_StageName = wszStageName; }
		wstring			GetStageName() { return m_StageName; }

	protected:
		STAGE_STATE				m_eStageState;
		wstring					m_StageName;	// FieldFix: ������ ��忡�� X��ư ������ �κп� ���� ó�� �ܿ� ���� ����ϴ� ���� ��� bool�� �ص� �ɵ�
};
