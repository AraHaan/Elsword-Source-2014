#include "TempInventory.h"

#include "X2Data/XSLItemManager.h"
#include "NetError.h"

KTempInventory::KTempInventory() : 
m_iTempItemUID( 0 )
{
	Clear();
}

KTempInventory::~KTempInventory()
{
}

void KTempInventory::Clear()
{
	m_iTempItemUID = 0;
	m_mapTempInventory.clear();	
}

//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
bool KTempInventory::AddTempInventoryItem( IN const int iItemID
										 , IN const u_char ucSealData
										 //{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
										 , IN const KItemInfo& kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
										 //}}
										 , OUT KTempInventoryItemInfo& kTempItemInfo
										 )
#else
bool KTempInventory::AddTempInventoryItem( int iItemID, u_char ucSealData, int iDropItemUID, KTempInventoryItemInfo& kTempItemInfo )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	if( pItemTemplet->m_PeriodType == CXSLItem::PT_QUANTITY )
	{
		std::map< int, KTempInventoryItemInfo >::iterator mit;
		for( mit = m_mapTempInventory.begin(); mit != m_mapTempInventory.end(); ++mit )
		{
			if( mit->second.m_iItemID != iItemID )
				continue;

			if( mit->second.m_iQuantity >= pItemTemplet->m_Quantity )
				continue;

			// ���� ����
			++mit->second.m_iQuantity;

			// �ӽ� �κ� ������ ����
			kTempItemInfo = mit->second;
			break;
		}

		if( mit == m_mapTempInventory.end() )
		{
			KTempInventoryItemInfo kInfo;
			//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			kInfo.m_iTempItemUID = GetNextTempItemUID();
#else
			kInfo.m_iTempItemUID = iDropItemUID;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}
			kInfo.m_iItemID = iItemID;
			kInfo.m_iQuantity = 1;
			//{{ 2009. 11. 18  ������	Ư���ð�����̺�Ʈ
			kInfo.m_ucSealData = 0; // �������� �к��ɼ�����
			//}}
			//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			// ��� �������� �ƴ�����... �ϴ��� �۾� �ص���
			kInfo.m_cEnchantLevel	= kItemInfo.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
			m_mapTempInventory.insert( std::make_pair( kInfo.m_iTempItemUID, kInfo ) );

			// �ӽ� �κ� ������ ����
			kTempItemInfo = kInfo;
		}
	}
	else
	{
		KTempInventoryItemInfo kInfo;
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		kInfo.m_iTempItemUID = GetNextTempItemUID();
#else
		kInfo.m_iTempItemUID = iDropItemUID;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		kInfo.m_iItemID = iItemID;
		kInfo.m_iQuantity = 1;
		//{{ 2009. 11. 18  ������	Ư���ð�����̺�Ʈ
		kInfo.m_ucSealData = ucSealData;
		//}}
		//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		kInfo.m_cEnchantLevel	= kItemInfo.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
		m_mapTempInventory.insert( std::make_pair( kInfo.m_iTempItemUID, kInfo ) );

		// �ӽ� �κ� ������ ����
		kTempItemInfo = kInfo;
	}

	return true;
}

bool KTempInventory::PrepareTempItemToInventory( IN int iTempItemUID, OUT int& iGetItemID, OUT std::map< int, KItemInfo >& mapGetItem )
{
	mapGetItem.clear();

	std::map< int, KTempInventoryItemInfo >::iterator mit;
	mit = m_mapTempInventory.find( iTempItemUID );
	if( mit == m_mapTempInventory.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �ӽ� ���� �������� �������� �߽��ϴ�." )
			<< BUILD_LOG( iTempItemUID )
			<< BUILD_LOG( static_cast<int>( m_mapTempInventory.size() ) )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->second.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( mit->second.m_iItemID )
			<< END_LOG;

		return false;
	}

	// ȹ���Ϸ��� �������� ItemID�� ����ش�.
	iGetItemID = mit->second.m_iItemID;

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID		  = mit->second.m_iItemID;
	kItemInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
	kItemInfo.m_iQuantity	  = mit->second.m_iQuantity;
	kItemInfo.m_sEndurance	  = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
	//{{ 2009. 11. 18  ������	Ư���ð�����̺�Ʈ
	kItemInfo.m_ucSealData	  = ( pItemTemplet->m_PeriodType != CXSLItem::PT_QUANTITY ) ? mit->second.m_ucSealData : 0;
	//}}
	//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kItemInfo.m_cEnchantLevel = mit->second.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}
	mapGetItem.insert( std::make_pair( mit->second.m_iItemID, kItemInfo ) );

	return true;
}

bool KTempInventory::DeleteTempItem( IN int iTempItemUID )
{
	std::map< int, KTempInventoryItemInfo >::iterator mit;
	mit = m_mapTempInventory.find( iTempItemUID );
	if( mit == m_mapTempInventory.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �ӽ� ���� �������� �����Ϸ��� �߽��ϴ�." )
			<< BUILD_LOG( iTempItemUID )
			<< END_LOG;

		return false;
	}

	m_mapTempInventory.erase( mit );
	return true;
}


			