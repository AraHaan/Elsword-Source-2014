#ifdef FIELD_NAVIGATOR
#pragma once
class CX2FieldNavigator
{
public:
	enum MAP_TYPE
	{
		MT_INVALID	= 0,
		MT_VILLAGE,
		MT_BATTLE_FIELD,
	};
	struct FieldMapLinkInfo
	{
		//UINT		uiMapID;		// �ʵ�, ��Ʋ�ʵ带 ��� ��� ID
		UINT		uiMoveMapID;	// ��Ż�� ���� �̵� ������ ��ID
		UINT		uiPortalPosID;	// ��Ż��ġID(�ʵ�, ��Ʋ�ʵ�Ŀ� ���� �ٸ� ���� ����)
		MAP_TYPE	eMapType;		// ��Ÿ��(�ʵ�, ��Ʋ�ʵ�)

		FieldMapLinkInfo(UINT uiMoveMapID_, UINT uiPortalPosID_, MAP_TYPE eMapType_ = MT_BATTLE_FIELD):
		uiMoveMapID(uiMoveMapID_), uiPortalPosID(uiPortalPosID_), eMapType(eMapType_){}
	};
public:
	CX2FieldNavigator(void);
	~CX2FieldNavigator(void);

	void	InsertFieldMapLinkInfo(FieldMapLinkInfo MapLinkInfo);
	void	InsertFieldMapLinkInfo(UINT uiMapID);
	void	ClearVecFieldMapLinkInfo(){m_vecFieldMapLinkInfo.clear();}
private:

	map<UINT, vector<FieldMapLinkInfo>>		m_mapFieldMapLinkInfo;
	vector<FieldMapLinkInfo>				m_vecFieldMapLinkInfo;
	bool									m_bAbleInsertMapLinkInfo;
	
};
#endif //FIELD_NAVIGATOR
