#include ".\gameeventitemdrop.h"

KGameEventItemDrop::KGameEventItemDrop(void) :
//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
m_fDropRate( 0.0f ),
#else // SERV_DROP_EVENT_RENEWAL
m_iDropCount( 1 ),
#endif // SERV_DROP_EVENT_RENEWAL
m_bWithPlayPcBang( false )
//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
, m_iDungeongID( 0 )
, m_cUnitType(0)
, m_cUnitClass(0)
, m_iStartLevel(0)
, m_iEndLevel(0)
#endif //DROPEVENT_RENEWAL
//}}
{
}

KGameEventItemDrop::~KGameEventItemDrop(void)
{
}
