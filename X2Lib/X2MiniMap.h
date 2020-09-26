#pragma once

// todo :	minimap�� ũ�⿡ ���� ������ ũ�⵵ �����ؼ� �׸���?
//			
// 09.03.11 ���¿�
// �̴ϸ� �ּ�ȭ ��� ����. UI �κ��� LUA�� ����.
// UI �ؽ��� ���� ���θʰ� ��ü�� �������ϴ� ��� -> �� �ؽ��� ���� ���θʰ� ��ü ������ + UI�� ��� �� ����.
class CX2MiniMapUI;

class CX2MiniMap : public CKTDGObject
{
public:
	enum MINIMAP_ICON
	{
		MI_MYSELF,
		MI_PARTY,
		MI_PARTY_ARROW,

		MI_MARKET_PLACE,
		MI_PVP_ARENA,
		MI_BANK,
		MI_POSTBOX,
		MI_GATE,		

		MI_NPC_COMMON,
		MI_NPC_WEAPON_SHOP,
		MI_NPC_ACCESSARY_SHOP,
		MI_NPC_ALCHEMIST,

		// ����Ʈ ����
		MI_NPC_COMPLETE_QUEST,
		MI_NPC_COMPLETE_REPEAT_QUEST,
		MI_NPC_INCOMPLETE_QUEST,
		MI_NPC_AVAIL_QUEST,
		MI_NPC_AVAIL_REPEAT_QUEST,
		MI_NPC_AVAIL_EVENT_QUEST,

		MI_BOARD,

		MI_END,
	};

	struct XYZ_DIFFUSE_TEX_VERTEX
	{
		float x, y, z;
		DWORD color;
		float u, v;
	};

private:
	CX2MiniMap( CX2MiniMapUI* pMiniMapUI );
	virtual ~CX2MiniMap(void);


public:
	static CX2MiniMap* CreateMiniMap( CX2MiniMapUI* pMiniMapUI ) { return new CX2MiniMap( pMiniMapUI ); }

	virtual void    NotifyShowObjectChanged();
	

//{{ robobeg : 2008-10-28
	//virtual HRESULT OnFrameRender();
    virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28

	float GetEyeDistance() { return m_fEyeDistance; }
	void SetEyeDistance( float fEyeDistance ) { m_fEyeDistance = fEyeDistance; }

	
	void SetStage(CKTDXStage* val) 
	{ 
		m_pStage = val; 


	}

	//{{ ����� : [2011/3/14/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	const D3DXVECTOR2& GetMiniMapWindowPos() { return m_vMiniMapWindowPos; }
	const D3DXVECTOR2& GetMiniMapWindowSize() { return m_vMiniMapWindowSize; }
#endif SERV_INSERT_GLOBAL_SERVER
	//}} ����� : [2011/3/14/] //	���� �̼�


protected:
	HRESULT RenderX2Game();
	void RenderMiniMapTexture();
	void NomalDirectCamera( CX2GameUnit* pFocusUnit, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );

	void RenderGameUnitTexture();

	HRESULT RenderSquare();
	void RenderMiniMapTextureSquare();
	void NomalDirectCameraSquare( CX2SquareUnit* pFocusUnit, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );

	HRESULT RenderVillage();
	void RenderMiniMapTextureVillage();
	void NomalDirectCameraVillage( CX2SquareUnit* pFocusUnit, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );
	bool IsInWindowTexture( const D3DXVECTOR3& vProjectedPos );

	//void ZoomDLGSetting();

private:
	CKTDXDeviceRenderTargetTexture* 	m_pRenderTargetTexture;
	CKTDXDeviceTexture*					m_pTextureMiniMap;
//	CKTDXDeviceTexture*					m_pTextureMonsterCount;
	CKTDXDeviceTexture*	m_pTextureIcon[ MI_END ];


	//float								m_fX, m_fY, m_fWidth, m_fHeight;
	float								m_fRenderTargetTextureScale;

	float								m_fEyeDistance;	

	CKTDGFontManager::CUKFont*			m_pFont;
	//{{ 09.03.11 ���¿� : �̴ϸ� �ּ�ȭ ��� ����
	//bool								m_bMinimized;
	//}}

	CKTDXStage*							m_pStage;
	CX2MiniMapUI*						m_pMiniMapUI;

	D3DXVECTOR2			m_vMiniMapWindowPos;
	D3DXVECTOR2			m_vMiniMapWindowSize;

};



