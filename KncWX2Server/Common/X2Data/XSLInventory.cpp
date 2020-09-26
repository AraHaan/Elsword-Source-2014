#include ".\xslinventory.h"


bool CXSLInventory::IsTradePossibleCategory( char cSlotCategory )
{
	if( cSlotCategory < CXSLInventory::ST_EQUIP  ||  cSlotCategory > CXSLInventory::ST_AVARTA )
		return false;

	return true;
}

bool CXSLInventory::IsValidBankMemberShip( MEMBERSHIP_PRIVATE_BANK eGrade )
{
	switch( eGrade )
	{
	case MPB_NORMAL:
	case MPB_SILVER:
	case MPB_GOLD:
	case MPB_EMERALD:
	case MPB_DIAMOND:
	case MPB_PLATINUM:
		return true;
	}

	return false;
}

bool CXSLInventory::IsUserInventoryCategory( char cSlotCategory )
{
    switch( cSlotCategory )
	{
	case ST_EQUIP:
	case ST_ACCESSORY:
	case ST_MATERIAL:
	case ST_SPECIAL:
	case ST_QUEST:
	case ST_QUICK_SLOT:
	case ST_AVARTA:		
	//case ST_PET: -- [����comment] �� �κ��丮�� ���Խ�Ű�� �ʽ��ϴ�! ���Խ�ų��� ���� ���ܻ�Ȳ�� ����Ƿ� ���� �ٶ��ϴ�! ��ȹ��Ʈ�� �ݵ�� �ǳ�!
		return true;
	}

	return false;
}

CXSLInventory::MEMBERSHIP_PRIVATE_BANK CXSLInventory::GetNextBankMemberShip( MEMBERSHIP_PRIVATE_BANK eGrade )
{
	switch( eGrade )
	{
	case MPB_NORMAL:	return MPB_SILVER;
	case MPB_SILVER:	return MPB_GOLD;
	case MPB_GOLD:		return MPB_EMERALD;
	case MPB_EMERALD:	return MPB_DIAMOND;
	case MPB_DIAMOND:	return MPB_PLATINUM;
	case MPB_PLATINUM:	return MPB_NONE;		
	}

	return MPB_NONE;
}

