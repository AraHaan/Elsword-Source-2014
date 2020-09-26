#pragma once
#include "Event.h"
#include "CommonPacket.h"

class KUserCashInventory
{
public:
	KUserCashInventory(void);
	~KUserCashInventory(void);

	void Clear();

	// �ǽð� ��Ȱ��
	bool IsCashResurrectionStone() { return m_bCashResurrectionStone; }
	void SetCashResurrectionStone( bool bValue ) { m_bCashResurrectionStone = bValue; }
	UidType GetCashRStoneUnitUID() { return m_iCashRStoneUnitUID; }
	void SetCashRStoneUnitUID( UidType iUnitUID ) { m_iCashRStoneUnitUID = iUnitUID; }

	// ��Ű�� �Ⱦ�
	void ClearPickUpPackageInfo() { m_mapPickUpPackageInfo.clear(); m_vecUpdatedInventorySlot.clear(); }
	bool IsEmptyPickUpPackageInfo() { return m_mapPickUpPackageInfo.empty(); }
	void AddPickUpPackageInfo( unsigned long ulProductNo, const KNXBTPickUpPackageInfo& kPickUpPackageInfo );
	void DelPickUpPackageInfo( unsigned long ulProductNo );
	bool GetPickUpPackageInfo( unsigned long ulProductNo, KNXBTPickUpPackageInfo& kPickUpPackageInfo );

	void AddUpdatedInventorySlot( const std::vector< KInventoryItemInfo >& vecUpdatedSlotInfo );
	void GetUpdatedInventorySlot( std::vector< KInventoryItemInfo >& vecUpdatedSlotInfo ) { vecUpdatedSlotInfo = m_vecUpdatedInventorySlot; }

	// �����ϱ�
	const std::wstring& GetReceiverUserID() { return m_wstrReceiverUserID; }
	void SetReceiverUserID( const std::wstring& wstrUserID ) { m_wstrReceiverUserID = wstrUserID; }
	const std::wstring& GetMessage() { return m_wstrMessage; }
	void SetMessage( const std::wstring& wstrMessage ) { m_wstrMessage = wstrMessage; }

	// ĳ�� ������ ����
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetSocketOptionForCash( int iItemID, int& iSocketOption );
#else
	bool GetSocketOptionForCash( int iItemID, short& sSocketOption );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	bool SetSocketForCash( const std::map< int, int >& mapSocketForCash, const std::map< int, int >& mapSocketGroupIDForCash );
#else
	void SetSocketForCash( const std::map< int, int >& mapSocketForCash ) { m_mapSocketForCash = mapSocketForCash; }
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}


private:
	// �ǽð� ��Ȱ��
	bool							m_bCashResurrectionStone;
	UidType							m_iCashRStoneUnitUID;

	// ��Ű�� �Ⱦ�
	std::map< unsigned long, KNXBTPickUpPackageInfo >	m_mapPickUpPackageInfo;
	std::vector< KInventoryItemInfo >					m_vecUpdatedInventorySlot;

	// �����ϱ�
	std::wstring					m_wstrReceiverUserID;
	std::wstring					m_wstrMessage;

	// ĳ�� ������ ����
	std::map< int, int >			m_mapSocketForCash;
};



