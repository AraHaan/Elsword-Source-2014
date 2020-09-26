#include "CnPartyFeverManager.h"
#include "X2Data/XSLPartyManager.h"
#include "X2Data/XSLBattleFieldManager.h"


KCnPartyFeverManager::KCnPartyFeverManager()
{
	Reset();
}

KCnPartyFeverManager::~KCnPartyFeverManager()
{
}

void KCnPartyFeverManager::Reset()
{
	m_sFeverPoint = 0;
}

short KCnPartyFeverManager::GetFeverPoint( IN const int iNumMember ) const
{
	return SiCXSLPartyManager()->GetMaxFeverPointByNumMember( GetRealFeverPoint(), iNumMember );
}

KCnPartyFeverManager::UPDATE_FEVER_POINT KCnPartyFeverManager::UpdateFeverPoint( IN const int iFeverPoint, IN const int iNumMember, OUT int& iResultFeverPoint )
{
	iResultFeverPoint = 0;

	// ��Ƽ ��� �� ���ϱ�
	if( iNumMember == 1 )
	{	
		// ��Ƽ ����� �Ѹ��̸� �ƹ��� ������ ���� �ʴ´�.
		return UFP_NONE;
	}

	SetRealFeverPoint( iFeverPoint );

	if( GetRealFeverPoint() < 0 )
		SetRealFeverPoint( 0 );

	// fever �ߵ� ������ �����ϴ��� üũ
	if( GetRealFeverPoint() >= SiCXSLPartyManager()->GetActivateFeverPoint() )
	{
		// 4800�� �����ϸ� �ٽ� 4000���� �ʱ�ȭ �ȴ�.
		SetRealFeverPoint( SiCXSLPartyManager()->GetMaxFeverPointByNumMember( GetRealFeverPoint(), iNumMember ) );

		// fever �ߵ�!
		iResultFeverPoint = GetRealFeverPoint();
		return UFP_ACTIVE_FEVER;
	}
	else
	{
		// ���� fever��ġ ������
		if( GetRealFeverPoint() < FEVER_100_PERCENT_POINT )
		{
			iResultFeverPoint = GetRealFeverPoint();
		}
		else
		{
			// 4000�̻��̴��� Ŭ���̾�Ʈ������ UI������ 4000������ ����ؾ���.
			iResultFeverPoint = FEVER_100_PERCENT_POINT;
		}
		return UFP_CHANGE_FEVER;
	}
}

int KCnPartyFeverManager::CalcIncreaseFeverPoint( IN const int iNumMember, IN const char cRoomType, IN const short sSubStageNpcCount, IN const bool bComeBackUserInParty )
{
	if( sSubStageNpcCount == 0 )
	{
		// ���Ͱ� �ϳ��� ���� �������� fever��ġ�� �������� �ʴ´�.
		return 0;
	}

	int iIncreasePartyFever = SiCXSLPartyManager()->GetIncreasePartyFever( iNumMember );

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	if( bComeBackUserInParty == true )
	{
		iIncreasePartyFever += static_cast<int>(iIncreasePartyFever * 0.5f); // �޸� ���� ����
	}
#endif SERV_COME_BACK_USER_REWARD
	//}}

	// ��Ʋ�ʵ忡�� ���� ���Ͷ�� ��Ʋ�ʵ� ������� ������!
	if( cRoomType == CXSLRoom::RT_BATTLE_FIELD )
	{
		//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
		float fMultipliedFeverByBattleFieldFactor = static_cast<float>(iIncreasePartyFever) * SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor();
#else
		float fMultipliedFeverByBattleFieldFactor = static_cast<float>(iIncreasePartyFever) * SiCXSLBattleFieldManager()->GetBattleFieldFactor();
#endif SERV_FIELD_ED_EXP_FACTOR
		//}
		if( fMultipliedFeverByBattleFieldFactor > 0  &&  fMultipliedFeverByBattleFieldFactor < 1 )
		{
			fMultipliedFeverByBattleFieldFactor = 1.f;
		}

		iIncreasePartyFever = static_cast<int>( fMultipliedFeverByBattleFieldFactor );
	}

	// ��Ƽ �ǹ� ����
	const int iIncreaseResult = iIncreasePartyFever / sSubStageNpcCount;
	return iIncreaseResult;
}


