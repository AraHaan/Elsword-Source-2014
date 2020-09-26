#pragma once


class CX2FieldMapBase : public CKTDGObject
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

protected:
	CX2FieldMapBase(void);
	virtual ~CX2FieldMapBase(void);

public:
	virtual void SetVillageID( int iMapID ) = NULL;

	bool IsInWindowTexture( const D3DXVECTOR2& vProjectedPos );
	void CalcWindowTexturePos( const D3DXVECTOR2& vProjectedMyPos );


	D3DXVECTOR2 ProjectToScreenShot( const D3DXVECTOR3& vPos );		// 3D �������� ��ǥ�� 2D ��ũ���� ���� ��ǥ��
	D3DXVECTOR2 CalcPosInWindowTexture( const D3DXVECTOR2& vPos );	// 2D ��ũ���� ���� ��ǥ�� 2D Minimap window texture ���� ��ǥ��
	D3DXVECTOR2 CalcPosOnScreen( const D3DXVECTOR2& vPos );			// 2D Minimap window ���� ��ǥ�� ���� �������� screen ���� ��ǥ��
	

	void ResetMapPosAndSize( int iMapID );
	void ResetCameraMatrix( int iMapID );
	void CalcCameraMatrix();


protected:
	CKTDXDeviceTexture* m_pTextureWholeMap;
	CKTDXDeviceTexture*	m_pTextureIcon[ MI_END ];

	D3DXVECTOR3 m_vEyePos;
	D3DXVECTOR3 m_vLookAt;
	float	m_fFov;		/// �þ߰� ��, ���� ����(��Ʈ���� Degree ���� �ָ� �������� ����ؼ� �־�ߵ�)
	float	m_fAspect;	/// ��Ⱦ��
	
	CKTDGUIButton*	m_pMyChar;
	CKTDGUIButton*	m_pPartyMember[4];

	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;
	D3DVIEWPORT9	m_ViewPort;

	D3DXVECTOR2 m_vScreenShotSize;			// (0,0) ~ (x, y)

	D3DXVECTOR2 m_vPosInWholeTexture;		// screen shot �󿡼� �����ǥ. left_top
	D3DXVECTOR2 m_vWholeTextureSize;		

	D3DXVECTOR2 m_vWindowTexturePos;		// screen shot �󿡼� window�� ��ġ, �����ǥ. left_top
	D3DXVECTOR2 m_vWindowTextureSize;

	D3DXVECTOR2 m_vMiniMapWindowPos;		// ȭ�鿡 ���� �̴ϸ��� ������ �� ��ġ, left_top
	D3DXVECTOR2 m_vMiniMapWindowSize;		// ȭ�鿡 ���� �̴ϸ��� ������ �� ũ��

	
};
