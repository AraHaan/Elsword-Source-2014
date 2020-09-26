#pragma once
#include "Event.h"
#include "CommonPacket.h"


//{{ 2011. 04. 19	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY


class KUserPersonalShopManager
{
public:

	KUserPersonalShopManager(void);
	~KUserPersonalShopManager(void);

	void Clear();
	void Init( IN OUT KUserPShopAgencyInfo& kUserPShopAgency );

	void SetPShopAgencyExpirationDate( IN const std::wstring& wstrAgencyExpirationDate );

	void SetPShopAgencyUID( IN const UidType iPShopAgencyUID )	{ m_iPShopAgencyUID = iPShopAgencyUID; }
	UidType GetPShopAgencyUID() const							{ return m_iPShopAgencyUID; }

	const std::wstring& GetAgencyExpirationDate()				{ return m_wstrAgencyExpirationDate; }

	bool IsOpenPossiblePShopAgency() const;

	//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	void SetPersonalShopBuyItemUID( IN UidType& iItemUID )				{ m_iPersonalShopBuyItemUID = iItemUID; }
	void SetPersonalShopBuyItemID( IN int& iItemID )					{ m_iPersonalShopBuyItemID = iItemID; }
	void SetPersonalShopBuyQuantity( IN int& iQuantity )				{ m_iPersonalShopBuyQuantity = iQuantity; }
	void SetPersonalShopBuyUpdate( IN KItemQuantityUpdate& kQuantityUpdate )		{ m_kQuantityUpdate = kQuantityUpdate; }
	void SetPersonalShopBuyItemInfo( IN std::vector< KItemInfo >& vecItemInfo )	{ m_vecItemInfo = vecItemInfo; }
	
	UidType GetPersonalShopBuyItemUID()			{ return m_iPersonalShopBuyItemUID; }
	int		GetPersonalShopBuyItemID()			{ return m_iPersonalShopBuyItemID; }
	int		GetPersonalShopBuyQuantity()		{ return m_iPersonalShopBuyQuantity; }
	void	GetPersonalShopBuyUpdate( OUT KItemQuantityUpdate& kQuantityUpdate )			{ kQuantityUpdate = m_kQuantityUpdate; m_kQuantityUpdate.m_mapQuantityChange.clear(); m_kQuantityUpdate.m_vecDeleted.clear(); }
	void	GetPersonalShopBuyQuantity( OUT std::vector< KItemInfo >& vecItemInfo )			{ vecItemInfo = m_vecItemInfo; m_vecItemInfo.clear();  }
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	void SetAgencyShopPickUpItemInfo( IN std::vector< UidType >& vecPickUpItemList )				{ m_vecPickUpItemList = vecPickUpItemList; }
	void GetAgencyShopPickUpItemInfo( OUT std::vector< UidType >& vecPickUpItemList )				{ vecPickUpItemList = m_vecPickUpItemList; }
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    void SetPShopType( IN const char cShopType_ );
    char GetPShopType() { return m_cShopType; };
#endif SERV_UPGRADE_TRADE_SYSTEM

private:
	UidType				m_iPShopAgencyUID;
	CTime				m_tAgencyExpirationDate;
	std::wstring		m_wstrAgencyExpirationDate;

	//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	UidType				m_iPersonalShopBuyItemUID;
	int					m_iPersonalShopBuyItemID;
	int					m_iPersonalShopBuyQuantity;
	KItemQuantityUpdate			m_kQuantityUpdate;
	std::vector< KItemInfo >	m_vecItemInfo;
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	std::vector< UidType >	m_vecPickUpItemList;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 2012.10.10 ����� �븮 ���� ����. 
    char m_cShopType; // ED�⺻ Ȥ�� ED �����̾� ���������� �����ߴ���, ĳ�þ��������� �����ߴ���. ����� 3���� Ÿ��
#endif SERV_UPGRADE_TRADE_SYSTEM


};


#endif SERV_PSHOP_AGENCY
//}}