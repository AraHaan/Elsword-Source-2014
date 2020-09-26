#include "ChannelUser.h"
#include "HackingProtectionManager.h"


//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST


KUserHackingManager::KUserHackingManager(void)
{
	// ���� ���ڸ��� �������� 0������ �ʱ�ȭ �Ѵ�.
	m_fHackingToolCheckTime = 0.0f;
}

KUserHackingManager::~KUserHackingManager(void)
{
}

void KUserHackingManager::SetRandomCheckTime()
{
	int iRandNum = rand() % static_cast<int>(HE_MAX_TIME - HE_MIN_TIME);
	iRandNum += static_cast<int>(HE_MIN_TIME);

	m_fHackingToolCheckTime = static_cast<double>(iRandNum);
}

void KUserHackingManager::GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList )
{
	// ��ŷ�� ����Ʈ�� ��´�.
	SiKHackingProtectionManager()->GetHackingToolList( iRandomKey, vecHackingToolList );

	// ���� üũ Ÿ���� �����Ѵ�.
    SetRandomCheckTime();
}

#endif SERV_HACKING_TOOL_LIST
//}}