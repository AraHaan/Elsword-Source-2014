#pragma once
#include <map>

// [����] ��ٱ��Ͽ��� SlotData�� ��޵Ǵ� ItemID���� �ߺ����� �ʴ´�.

// ��ٱ��ϴ� ���������� ������. ���� ĳ���� ���ýÿ� �ʱ�ȭ ��Ű�� �ʴ´�.


class KUserWishList
{
private:
    enum WishListSlot
	{
		SLOT_MAX = 21,
	};

	class KWishSlotData
	{
	public:
		KWishSlotData() { m_iItemID = 0; m_iBeforeItemID = 0; }
		~KWishSlotData() {}

		int GetSlotData() const { return m_iItemID; }
		bool IsChanged() { return ( m_iItemID != m_iBeforeItemID ); }
		void SetValue( int iItemID ) { m_iItemID = iItemID; m_iBeforeItemID = iItemID; }

		void operator=( int iItemID )
		{
			m_iBeforeItemID = m_iItemID;
			m_iItemID = iItemID;
		}
		
	private:
		int m_iItemID;
		int m_iBeforeItemID;
	};

public:
	KUserWishList(void);
	~KUserWishList(void);

	void InitWishList( const std::map< int, int >& mapWishList );
	void GetWishList( std::map< int, int >& mapWishList );
	void FlushWishListChange( std::map< int, int >& mapWishList );

	bool AddWish( int iItemID );
	bool DelWish( int iItemID );

	bool GetBuyCashItemInWishList() { return m_bBuyCashItemInWishList; }
	void SetBuyCashItemInWishList( bool bValue ) { m_bBuyCashItemInWishList = bValue; }
	
private:
	KWishSlotData	m_arrWishList[SLOT_MAX];
	bool			m_bBuyCashItemInWishList;
};



