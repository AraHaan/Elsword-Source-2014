#include "InventoryItem.h"

#include "X2Data/XSLItem.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLAttribEnchantItem.h"


KInventoryItem::KInventoryItem()
	: m_iItemUID( 0 )
	, m_iItemID( 0 )
	, m_iUsageType( 0 )
	, m_iEnchantLevel( 0 )
	, m_ucSealData( 0 )
	, m_sPeriod( 0 )
//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	, m_cItemState( 0 )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}
	, m_bIsDBUpdate( true )
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	, m_byteExpandedSocketNum( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
{
}

KInventoryItem::~KInventoryItem()
{
}

// ĳ���� ������ �� DB���� �޾ƿ� ���� �������� bNewItem�� false�� �Ѵ�.
// ���ų� ������� ������ ȹ��� DB�� �߰��� �������� �κ��丮�� �߰��� ���� bNewItem�� true�� �Ѵ�.
// ���ų� ������� DB�� �������� �߰��� �� �κ��丮�� ��� ���Կ� �� ���� �������� ���� �����̹Ƿ�
// ���� ī�װ��� ���� ID�� ��� 0���� ��ϵȴ�. ���� �� �������� �߰��� ���� m_iInventoryCategory �� m_iSlotID ��
// �ʱⰪ�� 0���� ���ִ� ���̴�.
void KInventoryItem::Init( IN const KInventoryItemInfo& kInfo, IN const bool bNewItem )
{
    m_iItemUID = kInfo.m_iItemUID;
    m_iItemID = kInfo.m_kItemInfo.m_iItemID;
    m_iUsageType = static_cast<int>(kInfo.m_kItemInfo.m_cUsageType);
    if( m_iUsageType == CXSLItem::PT_QUANTITY )
    {
        m_iQuantity.SetValue( kInfo.m_kItemInfo.m_iQuantity );
    }
    else
    {
        m_iQuantity.SetValue( 1 );
    }
    if( m_iUsageType == CXSLItem::PT_ENDURANCE )
    {
	    m_iEndurance.SetValue( kInfo.m_kItemInfo.m_sEndurance );
    }
    else
    {
        m_iEndurance.SetValue( 0 );
    }
	m_iEnchantLevel		 = static_cast<int>( kInfo.m_kItemInfo.m_cEnchantLevel );
	m_kAttribEnchantInfo = kInfo.m_kItemInfo.m_kAttribEnchantInfo;
	m_vecItemSocket		 = kInfo.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-20	// �ڼ���
	m_byteExpandedSocketNum	= kInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	m_vecRandomSocket	 = kInfo.m_kItemInfo.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	m_ucSealData		 = kInfo.m_kItemInfo.m_ucSealData;
    m_sPeriod			 = kInfo.m_kItemInfo.m_sPeriod;
    m_wstrExpirationDate = kInfo.m_kItemInfo.m_wstrExpirationDate;

    if( bNewItem )
    {
        m_iInventoryCategory.SetValue( 0 );
        m_iInventoryCategory += static_cast<int>(kInfo.m_cSlotCategory);
        m_iSlotID.SetValue( 0 );
		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_iSlotID += static_cast<int>(kInfo.m_sSlotID);
#else
		m_iSlotID += static_cast<int>(kInfo.m_cSlotID);
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
    }
    else
    {
        m_iInventoryCategory.SetValue( kInfo.m_cSlotCategory );
		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_iSlotID.SetValue( kInfo.m_sSlotID );
#else
		m_iSlotID.SetValue( kInfo.m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
    }
	//{{ 2010. 8. 4	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	m_iPetUID.SetValue( 0 );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	CheckAndUpdateItemState();
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	m_byteExpandedSocketNum = kInfo.m_kItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
}

int KInventoryItem::GetQuantity() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        if( m_iQuantity != 1 )
        {
            START_LOG( cerr, L"���� �������� �ƴѵ� ���� ���� 1�� �ƴ�." )
                << BUILD_LOG( m_iItemUID )
                << BUILD_LOG( m_iItemID )
                << BUILD_LOG( m_iUsageType )
                << BUILD_LOG( m_iQuantity )
                << END_LOG;

			// �����۰��� �����ع�����.. �����غ��� ���ܵ��� ��������.
            //m_iQuantity.SetValue( 1 );
        }
    }

    return m_iQuantity;
}

int KInventoryItem::GetEndurance() const
{
    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        if( m_iEndurance != 0 )
        {
            START_LOG( cerr, L"������ �������� �ƴѵ� ������ ���� 0�� �ƴ�." )
                << BUILD_LOG( m_iItemUID )
                << BUILD_LOG( m_iItemID )
                << BUILD_LOG( m_iUsageType )
                << BUILD_LOG( m_iEndurance )
                << END_LOG;

			// �����۰��� �����ع�����.. �����غ��� ���ܵ��� ��������.
            //m_iEndurance.SetValue( 0 );
        }
    }

    return m_iEndurance;
}

int KInventoryItem::GetEnchantLevel() const
{
    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"������ ���ø��� ���� ����." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return 0;
    }

    if( !pItemTemplet->m_bCanEnchant )
    {
        return 0;
    }

	return m_iEnchantLevel;
}

void KInventoryItem::GetAttribEnchant( OUT KItemAttributeEnchantInfo& kInfo ) const
{
	kInfo = m_kAttribEnchantInfo;
}

void KInventoryItem::GetAttribEnchant( IN int iAttribEnchantSlotID, OUT int& iAttribEnchantID ) const
{
	iAttribEnchantID = 0;

	switch( iAttribEnchantSlotID )
	{
	case CXSLAttribEnchantItem::ESI_SLOT_1: iAttribEnchantID = m_kAttribEnchantInfo.m_cAttribEnchant0; break;
	case CXSLAttribEnchantItem::ESI_SLOT_2: iAttribEnchantID = m_kAttribEnchantInfo.m_cAttribEnchant1; break;
	case CXSLAttribEnchantItem::ESI_SLOT_3: iAttribEnchantID = m_kAttribEnchantInfo.m_cAttribEnchant2; break;
	}
}

//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
const std::vector< int >& KInventoryItem::GetItemSocketList() const
#else
const std::vector< short >& KInventoryItem::GetItemSocketList() const
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;
	}

	return m_vecItemSocket;
}

//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventoryItem::GetItemSocketOption( IN const int iSocketIndex, OUT int& iSocketOption )
#else
bool KInventoryItem::GetItemSocketOption( IN const int iSocketIndex, OUT short& sSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;
		return false;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	int iMaxSocketCount = GetMaxSocketCount();
#else // SERV_BATTLE_FIELD_BOSS
	int iMaxSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType );
#endif // SERV_BATTLE_FIELD_BOSS

	// ���� �ε��� �˻�
	if( iSocketIndex < 0 || iSocketIndex >= iMaxSocketCount )
	{
		START_LOG( cerr, L"�������� ���� �ε����� �̻���." )
			<< BUILD_LOG( m_iItemUID )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemGrade )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iMaxSocketCount )
			<< BUILD_LOG( iSocketIndex )
			<< END_LOG;
		return false;
	}

	// ���� �ε����� ���� ���� ���Լ��� ������!
	if( static_cast<int>(m_vecItemSocket.size()) <= iSocketIndex )
	{
		int iEmptyCount = iSocketIndex - m_vecItemSocket.size() + 1;
		if( iEmptyCount > 0 )
		{
			for( int idx=0; idx < iEmptyCount; idx++ )
			{
				m_vecItemSocket.push_back( 0 );
			}
		}
	}

	_JIF( 0 <= iSocketIndex && static_cast<int>(m_vecItemSocket.size()) > iSocketIndex, return false; );
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	iSocketOption = m_vecItemSocket[iSocketIndex];
#else
	sSocketOption = m_vecItemSocket[iSocketIndex];
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	return true;
}

int KInventoryItem::GetCountAssignedItemSocket() const
{
	int iSocketCount = 0;
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit;
#else
	std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( vit = m_vecItemSocket.begin(); vit != m_vecItemSocket.end(); ++vit )
	{
		if( *vit > 0 )
			++iSocketCount;
	}
	
	return iSocketCount;
}

//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
const std::vector< int >& KInventoryItem::GetRandomSocketList() const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID() );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( GetItemID() )
			<< END_LOG;
	}

	return m_vecRandomSocket;
}

bool KInventoryItem::GetRandomSocketOption( IN const int iSocketIndex, OUT int& iSocketOptionID )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID() );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( GetItemID() )
			<< END_LOG;
		return false;
	}

	// ���� �ε��� �˻�
	if( iSocketIndex < 0 || iSocketIndex >= CXSLSocketItem::RSC_MAX )
	{
		START_LOG( cerr, L"�������� ���� �ε����� �̻���." )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( GetItemID() )
			<< BUILD_LOG( pItemTemplet->m_ItemGrade )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iSocketIndex )
			<< END_LOG;
		return false;
	}

	// ���� �ε����� ���� ���� ���Լ��� ������!
	if( static_cast<int>(m_vecRandomSocket.size()) <= iSocketIndex )
	{
		int iEmptyCount = iSocketIndex - m_vecRandomSocket.size() + 1;
		if( iEmptyCount > 0 )
		{
			for( int idx=0; idx < iEmptyCount; ++idx )
			{
				m_vecRandomSocket.push_back( 0 );
			}
		}
	}

	_JIF( 0 <= iSocketIndex && static_cast<int>(m_vecRandomSocket.size()) > iSocketIndex, return false; );
	
	iSocketOptionID = m_vecRandomSocket[iSocketIndex];
	return true;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
int KInventoryItem::GetCountAssignedRandomSocket() const
{
	int iSocketCount = 0;

	BOOST_TEST_FOREACH( const int, iSocketOptionID, m_vecRandomSocket )
	{
		if( iSocketOptionID > 0 )
			++iSocketCount;
	}

	return iSocketCount;
}

void KInventoryItem::SetRandomSocketOption( IN const std::vector< int > vecRandomSocket )
{
    m_vecRandomSocket = vecRandomSocket;

	CheckAndUpdateItemState();
}

void KInventoryItem::CheckAndUpdateItemState()
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( GetItemID() );
	if( IS_NULL( pItemTemplet ) )
	{
		START_LOG( cerr, L"��ũ��Ʈ�� ���� itemid�Դϴ�." )
			<< BUILD_LOG( GetItemID() )
			<< END_LOG;

		m_cItemState = KItemInfo::IS_NORMAL;
		return;
	}

	// ���� ���� �������̸鼭 ���� ���������� �ϳ��� �ȹ����ִٸ� �������� ���´� "�̰���" �����̴�!
	if( pItemTemplet->m_vecRandomSocketGroupID.empty() == false  &&  m_vecRandomSocket.empty() == true )
	{
		m_cItemState = KItemInfo::IS_NOT_EVALUATED;
		return;
	}

	m_cItemState = KItemInfo::IS_NORMAL;
}

#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

//{{ 2009. 8. 27  ������	�к�
u_char KInventoryItem::GetSealCount() const
{
    if( m_ucSealData >= 100 )
	{
		return ( m_ucSealData - 100 );
	}
	else
	{
		return m_ucSealData;
	}
}
//}}

void KInventoryItem::GetItemInfo( KItemInfo& kInfo ) const
{
    kInfo.m_iItemID				= GetItemID();
    kInfo.m_cUsageType			= static_cast<char>( GetUsageType() );
    kInfo.m_iQuantity			= GetQuantity();
	kInfo.m_sEndurance			= static_cast<short>( GetEndurance() );
    kInfo.m_sPeriod				= m_sPeriod;
    kInfo.m_wstrExpirationDate	= m_wstrExpirationDate;
	kInfo.m_cEnchantLevel		= static_cast<char>( GetEnchantLevel() );
	kInfo.m_kAttribEnchantInfo	= m_kAttribEnchantInfo;
	kInfo.m_vecItemSocket		= GetItemSocketList();
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	kInfo.m_byteExpandedSocketNum	= m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	kInfo.m_vecRandomSocket		= GetRandomSocketList();
	kInfo.m_cItemState			= GetItemState();
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	kInfo.m_ucSealData			= GetSealData();

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	kInfo.m_byteExpandedSocketNum	= m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
}

void KInventoryItem::GetInventoryItemInfo( OUT KInventoryItemInfo& kInfo ) const
{
    kInfo.m_iItemUID	  = GetItemUID();
    kInfo.m_cSlotCategory = static_cast<char>( GetInventoryCategory() );
	//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	kInfo.m_sSlotID		  = static_cast<short>( GetSlotID() );
#else
	kInfo.m_cSlotID		  = static_cast<char>( GetSlotID() );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
    GetItemInfo( kInfo.m_kItemInfo );
}

//{{ 2009. 10. 12  ������	�ڵ�����
void KInventoryItem::GetInventoryItemSimpleInfo( OUT KInventoryItemSimpleInfo& kInfo ) const
{
	kInfo.m_iItemUID		= GetItemUID();
	kInfo.m_iItemID			= GetItemID();
	kInfo.m_cSlotCategory	= static_cast<char>( GetInventoryCategory() );
	//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	kInfo.m_sSlotID			= static_cast<short>( GetSlotID() );
#else
	kInfo.m_cSlotID			= static_cast<char>( GetSlotID() );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
	kInfo.m_EnchantLevel	= static_cast<char>( GetEnchantLevel() );
	kInfo.m_kAttribEnchantInfo = m_kAttribEnchantInfo;
}
//}}

bool KInventoryItem::IsQuantityFull() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( cerr, L"������������ �ƴѵ� ���� á���� Ȯ����." )
            << BUILD_LOG( m_iItemUID )
            << BUILD_LOG( m_iItemID )
            << BUILD_LOG( m_iUsageType )
            << BUILD_LOG( m_iQuantity )
			<< BUILD_LOG( m_iEndurance )
            << BUILD_LOG( m_iInventoryCategory )
            << BUILD_LOG( m_iSlotID )
            << END_LOG;

        return true;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"������ ���ø��� ���� ����." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return m_iQuantity > 1;     // ��¼��.. 1���� ũ�� full ����.
    }

    return m_iQuantity >= pItemTemplet->m_Quantity;
}

//{{ 2008. 12. 21  ������	��ȭ ����
bool KInventoryItem::IsBroken() const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;

		// �����ɷ� ó��
		return true;
	}

	// ��ȭ �������� �ƴ϶�� ������ �ȱ��������� ó��
	if( !pItemTemplet->m_bCanEnchant )
		return false;

    return ( m_iEnchantLevel < 0 );
}
//}}

//{{ 2009. 8. 27  ������	�к�
bool KInventoryItem::IsSealed() const
{
	return ( m_ucSealData >= 100 );
}
//}}

bool KInventoryItem::IsQuantityExhausted() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( cerr, L"������������ �ƴѵ� �ٽ���� Ȯ����." )
            << BUILD_LOG( m_iItemUID )
            << BUILD_LOG( m_iItemID )
            << BUILD_LOG( m_iUsageType )
            << BUILD_LOG( m_iQuantity )
			<< BUILD_LOG( m_iEndurance )
            << BUILD_LOG( m_iInventoryCategory )
            << BUILD_LOG( m_iSlotID )
            << END_LOG;

        return false;
    }

    return m_iQuantity <= 0;
}

bool KInventoryItem::IsPeriodExpired() const
{
	if( m_iUsageType != CXSLItem::PT_INFINITY )
	{
		return false;
	}

	// ������ ���������� �˻��Ѵ�.
	if( m_sPeriod <= 0 )
	{
		return false;
	}

	if( m_wstrExpirationDate.empty() )
	{
		START_LOG( cerr, L"�Ⱓ�� �������ε� �������� ����." )
			<< BUILD_LOG( m_iItemUID )
			<< END_LOG;

		return false;
	}

	CTime tExpirationTime;
	if( !KncUtil::ConvertStringToCTime( m_wstrExpirationDate, tExpirationTime ) )
	{
		START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
			<< BUILD_LOG( m_wstrExpirationDate )
			<< END_LOG;

		return false;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	if( tCurrentTime > tExpirationTime )
	{
		return true;
	}

	return false;
}

int KInventoryItem::GetRemainedQuantityCapacity() const
{
    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        return 0;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"������ ���ø��� ���� ����." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return 0;
    }

    _JIF( pItemTemplet->m_Quantity - m_iQuantity >= 0, return 0 );

    return pItemTemplet->m_Quantity - m_iQuantity;
}

int KInventoryItem::GetEnduranceToRestore() const
{
    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        return 0;
    }

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"������ ���ø��� ���� ����." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return 0;
    }

    _JIF( pItemTemplet->m_Endurance - m_iEndurance >= 0, return 0 );

    return pItemTemplet->m_Endurance - m_iEndurance;
}

void KInventoryItem::IncreaseQuantity( int iQuantity, int& iIncreased )
{
    iIncreased = 0;

    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( clog, L"������ �������� �ƴ�." )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iQuantity <=  0 )
    {
        START_LOG( cerr, L"������Ű���� ���� ���� �̻���." )
            << BUILD_LOG( iQuantity )
            << END_LOG;

        return;
    }

    if( m_iQuantity <= 0 )
    {
        START_LOG( cerr, L"���� ���� �̻���." )
            << BUILD_LOG( m_iQuantity )
            << END_LOG;

        return;
    }

    int iRemainedCapacity = GetRemainedQuantityCapacity();
    iIncreased = std::min< int >( iRemainedCapacity, iQuantity );

    m_iQuantity += iIncreased;

    return;
}

void KInventoryItem::DecreaseQuantity( int iQuantity, int& iDecreased )
{
    iDecreased = 0;

    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        START_LOG( clog, L"������ �������� �ƴ�." )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iQuantity <=  0 )
    {
        START_LOG( cerr, L"���ҽ�Ű���� ���� ���� �̻���." )
            << BUILD_LOG( iQuantity )
            << END_LOG;

        return;
    }

    if( m_iQuantity <= 0 )
    {
        START_LOG( cerr, L"���� ���� �̻���." )
            << BUILD_LOG( m_iQuantity )
            << END_LOG;

        return;
    }

    iDecreased = std::min< int >( m_iQuantity, iQuantity );

    m_iQuantity -= iDecreased;

    return;
}

void KInventoryItem::IncreaseEndurance( int iEndurance, int& iIncreased )
{
    iIncreased = 0;

    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        START_LOG( clog, L"������ �������� �ƴ�." )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iEndurance <=  0 )
    {
        START_LOG( cerr, L"������Ű���� ������ ���� �̻���." )
            << BUILD_LOG( iEndurance )
            << END_LOG;

        return;
    }

    if( m_iEndurance < 0 )
    {
        START_LOG( cerr, L"������ ���� �̻���." )
            << BUILD_LOG( m_iEndurance )
            << END_LOG;

        return;
    }

    int iEnduranceToRestore = GetEnduranceToRestore();
    iIncreased = std::min< int >( iEnduranceToRestore, iEndurance );

    m_iEndurance += iIncreased;

    return;
}

void KInventoryItem::DecreaseEndurance( int iEndurance, int& iDecreased )
{
    iDecreased = 0;

    if( m_iUsageType != CXSLItem::PT_ENDURANCE )
    {
        START_LOG( clog, L"������ �������� �ƴ�." )
			<< BUILD_LOG( m_iItemID )
            << BUILD_LOG( m_iUsageType )
            << END_LOG;

        return;
    }

    if( iEndurance <=  0 )
    {
        START_LOG( cerr, L"���ҽ�Ű���� ������ ���� �̻���." )
			<< BUILD_LOG( m_iItemID )
            << BUILD_LOG( iEndurance )
            << END_LOG;

        return;
    }

    if( m_iEndurance < 0 )
    {
        START_LOG( cerr, L"������ ���� �̻���." )
            << BUILD_LOG( m_iEndurance )
            << END_LOG;

        return;
    }

    iDecreased = std::min< int >( m_iEndurance, iEndurance );

    m_iEndurance -= iDecreased;

    return;
}

void KInventoryItem::IncreaseEnchantLevel( int iEnchantLevel, int& iIncreased )
{
	iIncreased = 0;

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"������ ���ø��� ���� ����." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }

    if( !pItemTemplet->m_bCanEnchant )
    {
        START_LOG( clog, L"��ȭ�� �� �ִ� �������� �ƴ�." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }
	
	// ������ ����� ����!
	if( iEnchantLevel <=  0 )
	{
		START_LOG( cerr, L"������Ű���� ��ȭ���� �̻���." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		return;
	}

	if( m_iEnchantLevel < -(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL)  ||  m_iEnchantLevel > CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
	{
		START_LOG( cerr, L"��ȭ �������� ������ �����." )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( m_iEnchantLevel )
			<< BUILD_LOG( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
			<< END_LOG;

		return;
	}
	
	// �ִ밪 �˻�
	iIncreased = std::min< int >( ( int )( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL - m_iEnchantLevel ), iEnchantLevel );

	m_iEnchantLevel += iIncreased;
	return;
}

void KInventoryItem::DecreaseEnchantLevel( int iEnchantLevel, int& iDecreased )
{
	iDecreased = 0;

    const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
    if( pItemTemplet == NULL )
    {
        START_LOG( cerr, L"������ ���ø��� ���� ����." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }

    if( !pItemTemplet->m_bCanEnchant )
    {
        START_LOG( clog, L"��ȭ�� �� �ִ� �������� �ƴ�." )
            << BUILD_LOG( m_iItemID )
            << END_LOG;

        return;
    }

	if( iEnchantLevel <=  0 )
	{
		START_LOG( cerr, L"���ҽ�Ű���� ��ȭ �������� �̻���." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;

		return;
	}

	if( m_iEnchantLevel < -( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )  ||  m_iEnchantLevel > CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
	{
		START_LOG( cerr, L"��ȭ �������� ������ �����." )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( m_iEnchantLevel )
			<< BUILD_LOG( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL )
			<< END_LOG;

		return;
	}

	iDecreased = std::min< int >( ( CXSLEnchantItemManager::MAX_ENCHANT_LEVEL + m_iEnchantLevel ), iEnchantLevel );

	m_iEnchantLevel -= iDecreased;

	return;
}

void KInventoryItem::SetPosition( int iCategory, int iSlotID )
{
    if( iCategory <= CXSLInventory::ST_NONE || iCategory >= CXSLInventory::ST_END )
    {
        START_LOG( cerr, L"�̻��� ī�װ�." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;
    }

    if( iSlotID < 0 )
    {
        START_LOG( cerr, L"�̻��� ���� ID." )
            << BUILD_LOG( iCategory )
            << BUILD_LOG( iSlotID )
            << END_LOG;
    }

    m_iInventoryCategory += ( iCategory - m_iInventoryCategory );
    m_iSlotID += ( iSlotID - m_iSlotID );
    return;
}

//{{ 2012. 05. 08	��μ�       ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
void KInventoryItem::PrepareIncreaseQuantity( IN int iQuantity, OUT int& iIncreased )
{
	// ������ �������� ������ �ִ� ������ ������ ���� �� �� üũ �ؾ� �� ���� ó��

	iIncreased = 0;

	if( m_iUsageType != CXSLItem::PT_QUANTITY )
	{
		START_LOG( clog, L"������ �������� �ƴ�." )
			<< BUILD_LOG( m_iUsageType )
			<< END_LOG;

		return;
	}

	if( iQuantity <=  0 )
	{
		START_LOG( cerr, L"������Ű���� ���� ���� �̻���." )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;

		return;
	}

	if( m_iQuantity <= 0 )
	{
		START_LOG( cerr, L"���� ���� �̻���." )
			<< BUILD_LOG( m_iQuantity )
			<< END_LOG;

		return;
	}

	// �ƽ��� �������� �� ���� �� �ִ� ���� ���
	int iRemainedCapacity = GetRemainedQuantityCapacity();
	iIncreased = std::min< int >( iRemainedCapacity, iQuantity );

	return;
}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

void KInventoryItem::SetPosition( IN int iCategory, IN int iSlotID, IN UidType iPetUID )
{
	if( iCategory <= CXSLInventory::ST_NONE || iCategory >= CXSLInventory::ST_END )
	{
		START_LOG( cerr, L"�̻��� ī�װ�." )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;
	}

	if( iSlotID < 0 )
	{
		START_LOG( cerr, L"�̻��� ���� ID." )
			<< BUILD_LOG( iCategory )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;
	}

	// ���� �ִ� ������ ���̶��?
	if( m_iInventoryCategory == CXSLInventory::ST_PET )
	{
		// �̵��Ϸ��� ������ ���� �ƴ϶��?
		if( iCategory != CXSLInventory::ST_PET )
		{
			m_iPetUID += ( 0 - m_iPetUID );
		}
	}
	// ���� �ִ� ������ ���� �ƴ϶��?
	{
		// �̵��Ϸ��� ������ ���̶��?
		if( iCategory == CXSLInventory::ST_PET )
		{
			m_iPetUID += ( iPetUID - m_iPetUID ); // �翬�� �� ������ ������Ʈ �Ѵ�!
		}
	}

	m_iInventoryCategory += ( iCategory - m_iInventoryCategory );
	m_iSlotID += ( iSlotID - m_iSlotID );
}

void KInventoryItem::UpdatePetUID( IN UidType iPetUID )
{
    m_iPetUID.SetValue( iPetUID );
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool KInventoryItem::SetItemSocketOption( IN const int iSocketIndex, IN const int iSocketOption )
#else
bool KInventoryItem::SetItemSocketOption( IN const int iSocketIndex, IN const short sSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;

		return false;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	int iMaxSocketCount = GetMaxSocketCount();
#else // SERV_BATTLE_FIELD_BOSS
	int iMaxSocketCount = SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType );
#endif // SERV_BATTLE_FIELD_BOSS

	// ���� �ε��� �˻�
	if( iSocketIndex < 0 || iSocketIndex >= iMaxSocketCount )
	{
		START_LOG( cerr, L"�������� ���� �ε����� �̻���." )
			<< BUILD_LOG( m_iItemUID )
			<< BUILD_LOG( m_iItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemGrade )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< BUILD_LOG( iMaxSocketCount )
			<< BUILD_LOG( iSocketIndex )
			//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			<< BUILD_LOG( iSocketOption )
#else
			<< BUILD_LOG( sSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			<< END_LOG;
		return false;
	}

	// ���� �ε����� ���� ���� ���Լ��� ������!
	if( static_cast<int>(m_vecItemSocket.size()) <= iSocketIndex )
	{
		int iEmptyCount = iSocketIndex - m_vecItemSocket.size() + 1;
		if( iEmptyCount > 0 )
		{
			for( int idx=0; idx < iEmptyCount; idx++ )
			{
				m_vecItemSocket.push_back( 0 );
			}
		}
	}

	// [����] ���Ͽ��� �ɼ��� �����ϴ� ����� ���Ͽ� ���Ͽ� �̹� �ٸ��ɼ��� �����ִ����� ���⼭ �˻����� �ʽ��ϴ�.

	_JIF( 0 <= iSocketIndex && static_cast<int>(m_vecItemSocket.size()) > iSocketIndex, return false; );
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	m_vecItemSocket[iSocketIndex] = iSocketOption;
#else
	m_vecItemSocket[iSocketIndex] = sSocketOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	return true;
}

//{{ 2008. 9. 3  ������		�Ӽ���ȭ
bool KInventoryItem::SetAttribEnchant( char cAttribEnchantSlotID, char cAttribEnchantType )
{
	if( SiCXSLAttribEnchantItem()->VerifyAttribEnchantType( cAttribEnchantType ) )
	{
		switch( cAttribEnchantSlotID )
		{
		case CXSLAttribEnchantItem::ESI_SLOT_1:
			m_kAttribEnchantInfo.m_cAttribEnchant0 = cAttribEnchantType;
			break;

		case CXSLAttribEnchantItem::ESI_SLOT_2:
			m_kAttribEnchantInfo.m_cAttribEnchant1 = cAttribEnchantType;
			break;

			//{{ 2009. 11. 9  ������	Ʈ���üӼ�
		case CXSLAttribEnchantItem::ESI_SLOT_3:
			m_kAttribEnchantInfo.m_cAttribEnchant2 = cAttribEnchantType;
			break;
			//}}

		default:
			START_LOG( cerr, L"�Ӽ���ȭ ����ID�� �̻��մϴ�." )
				<< BUILD_LOGc( cAttribEnchantSlotID )
				<< BUILD_LOGc( cAttribEnchantType )
				<< END_LOG;
			return false;
		}
	}
	else
	{
		START_LOG( cerr, L"���ǵ� �Ӽ���ȭ���� �ƴմϴ�." )
			<< BUILD_LOGc( cAttribEnchantSlotID )
			<< BUILD_LOGc( cAttribEnchantType )
			<< END_LOG;
		return false;
	}

	return true;
}
//}}

//{{ 2009. 8. 27  ������	�к�
bool KInventoryItem::Seal()
{
	if( IsSealed() )
	{
		START_LOG( cerr, L"�̹� ���εǾ� �ֽ��ϴ�." )
			<< END_LOG;

		return false;
	}

#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;

		return 0;
	}

	if( pItemTemplet->m_ucMaxSealCount == CXSLInventory::SEAL_COUNT_MAX )
		m_ucSealData += 100; // ����ó���� ��
	else
		m_ucSealData += 101; // ����ó�� �� ����Ƚ������
#else // SERV_REMOVE_SEAL_COUNT_DECREASE
	m_ucSealData += 101; // ����ó�� �� ����Ƚ������
#endif // SERV_REMOVE_SEAL_COUNT_DECREASE

	return true;
}

bool KInventoryItem::Unseal()
{
	if( !IsSealed() )
	{
		START_LOG( cerr, L"�̹� ���������Ǿ� �ֽ��ϴ�." )
			<< END_LOG;

		return false;
	}

	m_ucSealData -= 100; // ���� ����
	return true;
}
//}}

#ifdef	SERV_SHARING_BANK_TEST
bool KInventoryItem::IncreaseSealCount()
{
	++m_ucSealData; // ����Ƚ�� ����
	return true;
}
#endif	SERV_SHARING_BANK_TEST

//{{ 2010. 8. 16	������	�Ⱓ ���� ������ �̺�Ʈ
#ifdef SERV_RESET_PERIOD_EVENT

void KInventoryItem::UpdatePeriod( IN short sPeriod, IN const std::wstring& wstrExpirationDate )
{
	if( m_sPeriod == 0  &&  sPeriod != 0 )
	{
		START_LOG( cerr, L"�Ⱓ���� �ƴѵ� �Ⱓ���� �ٲܷ��� �ϳ�? ��ȹ�� �̷� ����� ����!" )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( GetItemID() )
			<< BUILD_LOG( m_sPeriod )
			<< BUILD_LOG( sPeriod )
			<< END_LOG;
		return;
	}

	m_sPeriod = sPeriod;
	m_wstrExpirationDate = wstrExpirationDate;
}

#endif SERV_RESET_PERIOD_EVENT
//}}

bool KInventoryItem::FlushQuantityChange( int& iChange )
{
    iChange = 0;

    if( m_iUsageType != CXSLItem::PT_QUANTITY )
    {
        return false;
    }

    iChange = m_iQuantity.GetChangeValue();
    m_iQuantity.SetValue( m_iQuantity );

    return iChange != 0;
}

void KInventoryItem::RollBackInitQuantity( int iChange )
{
    m_iQuantity.AdjustInitValue( m_iQuantity.GetInitValue() - iChange );
}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

bool KInventoryItem::FlushPositionChange( OUT int& iCategoryChange, OUT int& iSlotIDChange, OUT UidType& iPetUID )
{
	iCategoryChange = 0;
	iSlotIDChange = 0;
	iPetUID = 0;

	bool bIsChanged = false;

	if( m_iInventoryCategory.IsChanged()  ||  m_iSlotID.IsChanged()  ||  m_iPetUID.IsChanged() )
	{
		iCategoryChange = GetInventoryCategory();
		iSlotIDChange   = GetSlotID();

		// PetUID���� �ʱⰪ�� �޶����ٸ�?
		if( m_iPetUID.IsChanged() == true )
		{
			// PetUID�� DB�� ������Ʈ�ϱ� ���� ����!
			iPetUID	= m_iPetUID;
		}
		else
		{
			// PetUID�ʹ� ���þ����Ƿ� INVALID�� ����!
			iPetUID = KItemPosition::IPE_INVALID_PET_UID;
		}

#ifdef SERV_SHARING_BANK_TEST
		//	������� �������̶�� Ÿ���� �������ش�.
		if( iCategoryChange == CXSLInventory::ST_SHARE_BANK )
		{
			iCategoryChange = CXSLInventory::ST_BANK;
		}
#endif SERV_SHARING_BANK_TEST

		bIsChanged = true;
	}

	return bIsChanged;
}

//{{ 2008. 8. 22  ������	������ ��ġ �ǰ� ������Ʈ
void KInventoryItem::PositionUpdateComplete( int iCategoryChange, int iSlotIDChange, UidType iPetUID )
{
	m_iInventoryCategory.AdjustInitValue( iCategoryChange );
	m_iSlotID.AdjustInitValue( iSlotIDChange );
	if( iPetUID != -1 )
	{
		m_iPetUID.AdjustInitValue( iPetUID );
	}
}
//}}

#else

bool KInventoryItem::FlushPositionChange( int& iCategoryChange, int& iSlotIDChange )
{
	iCategoryChange = 0;
	iSlotIDChange = 0;

	if( m_iInventoryCategory.IsChanged()  ||  m_iSlotID.IsChanged() )
	{
		iCategoryChange = GetInventoryCategory();
		iSlotIDChange   = GetSlotID();

#ifdef SERV_SHARING_BANK_TEST
		//	������� �������̶�� Ÿ���� �������ش�.
		if( iCategoryChange == CXSLInventory::ST_SHARE_BANK )
		{
			iCategoryChange = CXSLInventory::ST_BANK;
		}
#endif SERV_SHARING_BANK_TEST
	}

	return ( iCategoryChange != 0 ) || ( iSlotIDChange != 0 );
}

//{{ 2008. 8. 22  ������	������ ��ġ �ǰ� ������Ʈ
void KInventoryItem::PositionUpdateComplete( int iCategoryChange, int iSlotIDChange )
{
	m_iInventoryCategory.AdjustInitValue( iCategoryChange );
	m_iSlotID.AdjustInitValue( iSlotIDChange );
}
//}}

#endif SERV_PET_SYSTEM
//}}

bool KInventoryItem::FlushEnduranceChange( int& iChange )
{
	iChange = 0;

	if( m_iUsageType != CXSLItem::PT_ENDURANCE )
	{
		return false;
	}

	iChange = m_iEndurance.GetChangeValue();
	m_iEndurance.SetValue( m_iEndurance );

	return iChange != 0;
}

void KInventoryItem::RollBackInitEndurance( int iChange )
{
	m_iEndurance.AdjustInitValue( m_iEndurance.GetInitValue() - iChange );
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
int KInventoryItem::GetMaxSocketCount( void ) const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( m_iItemID )
			<< END_LOG;
		return 0;
	}

	return SiCXSLSocketItem()->GetSocketCount( pItemTemplet->m_ItemGrade, pItemTemplet->m_ItemType ) + m_byteExpandedSocketNum;
}
#endif // SERV_BATTLE_FIELD_BOSS