#pragma once

class CX2InformerInven : public CX2Informer
{
	public:

		CX2InformerInven(void);
		virtual ~CX2InformerInven(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		void Reset();
		bool CheckNewItem();
#ifdef NEW_ITEM_NOTICE
		void SetNewQuickSlotItem(bool bVal, UINT uiSlotID = 0){ m_bNewQuickSlotItem = bVal; m_uiSlotID = uiSlotID; }
#endif //NEW_ITEM_NOTICE

	protected:

		void SetNowInfo();

		bool IsNowInfoChange();
		bool CheckEnoughInven();		
			

		void SetShow( bool bShow );

		bool IsEnoughAllInvenSlot();

		map < CX2Inventory::SORT_TYPE, int > m_mapNowInvenSizeForEnough;
#ifdef MODIFY_INFORMER_INVEN
		set < int > m_setNowInvenForNewItem;			//ItemID
#else
		map < int, int > m_mapNowInvenForNewItem;		//ItemID, ����
#endif //MODIFY_INFORMER_INVEN

		bool	m_bEnoughInven;
		bool	m_bNewItem;

#ifdef NEW_ITEM_NOTICE
		bool	m_bNewQuickSlotItem;		//�����Կ� �ڵ����� ���� �����ۿ� ���� �Ǵ�
		UINT	m_uiSlotID;
#endif //NEW_ITEM_NOTICE

		int		m_EnoughRemainSlotNum;		//üũ�� ����� ī�װ��� ���� ���� ���� ( ��, �� ���� ���Ϸ� ������ �˸��� ) 
		float	m_fEnoughAllInvenSlotCheckTime;
};
