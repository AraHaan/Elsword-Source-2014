#include "FieldUser.h"
#include "X2Data/XSLInventory.h"
#include <boost/test/utils/foreach.hpp>


KFieldUser::KFieldUser(void)
:
m_iSectorID( INVALID_SECTOR )
{
}

KFieldUser::~KFieldUser(void)
{
}

void KFieldUser::SetFieldUserInfo( const KFieldUserInfo& kInfo )
{
	m_kFieldUserInfo = kInfo;

	// �ʵ� ���� ����Ʈ�� ���� ����
	m_kCommunityUserInfo.m_iUnitUID		= kInfo.m_iUnitUID;
	m_kCommunityUserInfo.m_wstrNickName = kInfo.m_wstrNickName;
	m_kCommunityUserInfo.m_cUnitClass	= kInfo.m_cUnitClass;
	m_kCommunityUserInfo.m_ucLevel		= kInfo.m_ucLevel;
	m_kCommunityUserInfo.m_cState		= static_cast<char>( KCommunityUserInfo::US_FIELD );
	m_kCommunityUserInfo.m_iStateCode	= kInfo.m_iPartyUID;
	m_kCommunityUserInfo.m_iRoomUID		= 0;
	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	m_kCommunityUserInfo.m_iED		= kInfo.m_iED;
#endif SERV_ED_MONITORING_IN_GAME
	//}}
}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

void KFieldUser::SetPetInfo( IN const std::vector< KPetInfo >& vecPetInfo )
{
	m_kFieldUserInfo.m_vecPet.clear();

	if( vecPetInfo.size() > 1 )
	{
		START_LOG( cerr, L"�� ������ 1������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( m_kFieldUserInfo.m_iUnitUID )
			<< END_LOG;

		// �׳� ���!
	}

	BOOST_TEST_FOREACH( const KPetInfo&, kPet, vecPetInfo )
	{
		KFieldPetInfo kInfo;
		kInfo.m_iPetUID			= kPet.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		kInfo.m_iPetID			= kPet.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
		kInfo.m_cPetID			= kPet.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
		kInfo.m_wstrPetName		= kPet.m_wstrPetName;
		kInfo.m_cEvolutionStep	= kPet.m_cEvolutionStep;
		m_kFieldUserInfo.m_vecPet.push_back( kInfo );
		break;
	}
}

#endif SERV_PET_SYSTEM
//}}

void KFieldUser::SetUserEquipItem( std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	std::vector< KInventoryItemInfo >::iterator vit;
	for( vit = vecInventorySlotInfo.begin(); vit != vecInventorySlotInfo.end(); vit++ )
	{
		if( vit->m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
		{
			// �������� ���� �ϴ� ���� update �Ѵ�
			std::vector< KInventoryItemSimpleInfo >::iterator vit2;
			for( vit2 = m_kFieldUserInfo.m_vecEquippedItem.begin(); vit2 != m_kFieldUserInfo.m_vecEquippedItem.end(); ++vit2 )
			{
				//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				if( vit->m_sSlotID == vit2->m_sSlotID )
#else
				if( vit->m_cSlotID == vit2->m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}				
                {
					vit2->m_iItemUID		= vit->m_iItemUID;
					vit2->m_cSlotCategory	= vit->m_cSlotCategory;
					//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					vit2->m_sSlotID			= vit->m_sSlotID;
#else
					vit2->m_cSlotID			= vit->m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					vit2->m_iItemID			= vit->m_kItemInfo.m_iItemID;
					vit2->m_EnchantLevel	= vit->m_kItemInfo.m_cEnchantLevel;						
					vit2->m_kAttribEnchantInfo = vit->m_kItemInfo.m_kAttribEnchantInfo;
					break;
				}
			}

			if( vit2 == m_kFieldUserInfo.m_vecEquippedItem.end() )
			{
				KInventoryItemSimpleInfo kInfo;
				kInfo.m_iItemUID		= vit->m_iItemUID;
				kInfo.m_cSlotCategory	= vit->m_cSlotCategory;
				//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
				kInfo.m_sSlotID			= vit->m_sSlotID;
#else
				kInfo.m_cSlotID			= vit->m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
				//}}
				kInfo.m_iItemID			= vit->m_kItemInfo.m_iItemID;
				kInfo.m_EnchantLevel	= vit->m_kItemInfo.m_cEnchantLevel;				
				kInfo.m_kAttribEnchantInfo	= vit->m_kItemInfo.m_kAttribEnchantInfo;

				m_kFieldUserInfo.m_vecEquippedItem.push_back( kInfo );
			}
		}
	}
}

void KFieldUser::GetBroadcastDestination( std::vector< UidType >& vecDestination )
{
    vecDestination.clear();

	std::vector< UidType > vecPartyFriend;
	
	{
		// ��Ƽ����Ʈ�� ģ������Ʈ �ߺ��� �ɷ�����
		std::set< UidType >::iterator sitParty = m_setPartyUIDList.begin();
		std::set< UidType >::iterator sitFriend = m_setFriendUIDList.begin();

		while( sitParty != m_setPartyUIDList.end() || sitFriend != m_setFriendUIDList.end() )
		{
			if( sitParty == m_setPartyUIDList.end() )
			{
				vecPartyFriend.push_back( *sitFriend );
				sitFriend++;
				continue;
			}

			if( sitFriend == m_setFriendUIDList.end() )
			{
				vecPartyFriend.push_back( *sitParty );
				sitParty++;
				continue;
			}

			if( *sitParty < *sitFriend )
			{
				vecPartyFriend.push_back( *sitParty );
				sitParty++;
				continue;
			}
			else if( *sitParty > *sitFriend )
			{
				vecPartyFriend.push_back( *sitFriend );
				sitFriend++;
				continue;
			}
			else
			{
				vecPartyFriend.push_back( *sitParty );
				sitParty++;
				sitFriend++;
				continue;
			}
		}
	}
	
	{
		std::vector< UidType >::iterator vitPF = vecPartyFriend.begin();
		std::set< UidType >::iterator sitSend = m_setEventSendUIDList.begin();

		// SendUIDList�� �ߺ��� �ɷ�����
		while( vitPF != vecPartyFriend.end() || sitSend != m_setEventSendUIDList.end() )
		{
			if( vitPF == vecPartyFriend.end() )
			{
				vecDestination.push_back( *sitSend );
				sitSend++;
				continue;
			}

			if( sitSend == m_setEventSendUIDList.end() )
			{
				vecDestination.push_back( *vitPF );
				vitPF++;
				continue;
			}

			if( *vitPF < *sitSend )
			{
				vecDestination.push_back( *vitPF );
				vitPF++;
				continue;
			}
			else if( *vitPF > *sitSend )
			{
				vecDestination.push_back( *sitSend );
				sitSend++;
				continue;
			}
			else
			{
				vecDestination.push_back( *vitPF );
				vitPF++;
				sitSend++;
				continue;
			}
		}
	}	
}


