// 2009.01.29 ���¿� : ������ ǥ�� UI.
// UICharInfo�� member�� ����. -> 09/03/23 ���� : MiniMapUI �Ʒ��� �ŰܿԴ�.
// SetShow(true)�� �޾��� �� Update�� ��. �������� 20% �Ʒ��� ������ ��� ������ ��Ÿ���� ����.
// ���� �ڵ忡 �ִ� �������� state �̵��� �� ( CX2UIManager::OnStateChange ) �� ��� ����(����/�����Ա� ��)�� ���� �� ( CX2UICharInfo::ResetNowEquipUI )
// -> �� ���� �� (CX2MiniMapUI::SetShowMiniMap)�� ��񺯰�( CX2UICharInfo::ResetNowEquipUI )���� ��ü

#pragma once

class CX2UIEnduranceChecker
{
public:

	CX2UIEnduranceChecker(void);
	virtual ~CX2UIEnduranceChecker(void);

	bool Update();
	
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	
	
private:
	bool					m_bShow;
	
	CKTDGUIDialogType			m_pDLGUIEnduranceChecker;   

};

