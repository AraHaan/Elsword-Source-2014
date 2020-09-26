#pragma once

//////////////////////////////////////////////////////////////////////////
//## scenario object collection
//## Des : �ó����� ���� Ŭ���� & ����ü ����.
//////////////////////////////////////////////////////////////////////////
namespace _SCENARIO_OBJECT_
{
	//# �������� �ؽ��� ��ü�� ����.
	class CTexture
	{
	public:
		CTexture(void)
		:
		m_iLayer(0),
		m_pTexture(NULL),
		m_RenderPos(D3DXVECTOR2(0.f, 0.f)),
		m_RenderSize(D3DXVECTOR2(100.f, 100.f)),
		m_Color(D3DXCOLOR(1.f, 1.f, 1.f, 1.f))
		{}
		~CTexture(void)
		{
			SAFE_CLOSE(m_pTexture);
		}

		bool	Load(std::wstring wstrTexName)
		{
			m_pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture(wstrTexName);
			if(m_pTexture == NULL)
				return false;
			return true;
		}
		void	Render()
		{
			if(m_pTexture != NULL)
				m_pTexture->Draw((int)m_RenderPos.x, (int)m_RenderPos.y, (int)m_RenderSize.x, (int)m_RenderSize.y, m_Color);
		}

		void		SetNumLayer(int iLayer){m_iLayer = iLayer;}
		int			GetNumLayer(){return m_iLayer;}

		wstring&	GetTexName(){return m_pTexture->GetDeviceID();}

	private:
		int					m_iLayer;
		CKTDXDeviceTexture*	m_pTexture;

		D3DXVECTOR2			m_RenderPos;
		D3DXVECTOR2			m_RenderSize;
		D3DXCOLOR			m_Color;
	};
	typedef std::list<CTexture*>	CTextureList;

	//# �׼��̺�Ʈ�� �θ� Ŭ���� �μ� �ĺ� ���ڸ��� ������ �ִ´�.
	class CAction
	{
		enum ACTION_TYPE
		{
			AT_NONE = 0,
			AT_TEX_RENDER,
			AT_TEX_DEL,

			AT_END,
		};

	public:
		CAction(void)
		:
		m_TYPE(AT_NONE),
		m_iID(0),
		m_bSingle(true)
		{}
		CAction(ACTION_TYPE atType)
		:
		m_TYPE(atType),
		m_iID(0),
		m_bSingle(true)
		{}
		~CAction(void){}

		// �׼�Ÿ�� ����ó��..
		void		SetType( ACTION_TYPE atType ){m_TYPE = atType;}
		ACTION_TYPE	GetType(){return m_TYPE;}
		// �׼��� ��������..
		void		SetID(int iID){m_iID = iID;}
		int			GetID(){return m_iID;}
		// �׼� ��������..

	protected:
		ACTION_TYPE		m_TYPE;
		int				m_iID;		// �׼��� ������ ����..
		bool			m_bSingle;	// �׼� ���� ��� : �ܵ����� ����Ǵ���, ���۸� �ϰ� ���� �̺�Ʈ�� �Ѿ������ ����..
	};
}