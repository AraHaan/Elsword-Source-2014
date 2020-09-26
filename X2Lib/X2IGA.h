//{{ kimhc // 2010.3.17 //	���� ���⵵
#ifdef	IGA_TEST

#pragma once

#define ELAPSED_TIME_TO_CHECK 0.5

class CX2Camera;

class CX2IGA
{
	DeclareSingleton( CX2IGA );

public:	
	struct IGA_INFO
	{
		D3DXVECTOR3 m_vPoint[4];
		D3DXVECTOR3 m_vNormal;

		void SetPoint( D3DXVECTOR3& vLB, D3DXVECTOR3& vLT, D3DXVECTOR3& vRT, D3DXVECTOR3& vRB )
		{
			m_vPoint[0] = vLB;
			m_vPoint[1] = vLT;
			m_vPoint[2] = vRT;
			m_vPoint[3] = vRB;
		}
	};

	enum IGA_STATE
	{
		IS_NONE,
		IS_INIT,
		IS_SET_ENV,
		IS_CLEAR_IMG,
		IS_SET_USER,
		IS_DOWN_LOAD_TEX,
		IS_START,
		IS_END,
	};

	CX2IGA() {}

	//# ���α׷� ������ ���ѹ��� ȣ��
	void Init();

	//# ���α׷� ����� ���ѹ��� ȣ��
	void Clear();

	//# ĳ���� ������ �������
	// - ���� ���� ��� �Լ�
	void SetUser( wstring& wstrID, int iAge, bool bMan );

	//# �ܺ��� �ֵ弭���� ���� �����̹��� ������ �޾Ƽ� �����ϴ� �Լ�
	void DownLoadTexture();

	//# ������ �������� ��ġ ������ �ϵ��ڵ� �Ǿ� �ִ� �Լ�
	void CreateIgaInfoInVillage( int iVillageID );

	//# �ŷ������� �������� ��ġ ������ �ϵ��ڵ� �Ǿ� �ִ� �Լ�
	void CreateIgaInfoInMarket();

	//# ������⵵ ������ ������ ���� �ѹ��� ȣ���ϴ� �Լ�
	//  �� ��, End() ȣ�� �Ŀ� �ٽ� ������� ������ ������ ���ؼ��� �ٽ� �ѹ� ȣ���� �ؾ���
	void Start();

	//# ������⵵ ������ �� ���� �� ȣ���ϴ� �Լ�
	void End();

	//# ȭ�� ������⵵ ������ ������
	//  �� ���� or ���忡�� ELAPSED_TIME_TO_CHECK�� ������ �ʸ���
	void ProcessTracking( CX2Camera* pCamera, float fElapsedTime );

	//# ������⵵�� ����ϴ� �Լ�
	void Track( IGA_INFO& IgaInfo, int iIndex, CX2Camera* pCamera );

	//# �������� ��ġ������ normal������ ����
	//  ��m_vecIgaInfo�� ����
	void InitAdCoord( D3DXVECTOR3& vLT, D3DXVECTOR3& vLB, D3DXVECTOR3& vRT, D3DXVECTOR3& vRB );

public:
	std::vector<IGA_INFO>	m_vecIgaInfo;
	static IGA_STATE		m_eNowState;
	IGA_TEXTURE				m_IgaTexture;
	string					m_strInvid;
	string					m_strFileName;

	static float			m_fElapsedTime;

};

DefSingletonInline( CX2IGA );
#endif	IGA_TEST
//}} kimhc // 2010.3.17 //	���� ���⵵