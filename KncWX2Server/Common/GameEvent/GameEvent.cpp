#include "GameEvent.h"

KGameEvent::KGameEvent()
:
m_iUID( 0 ),
//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
m_iScriptID( 0 ),
//}}
m_eType( GET_NONE ),
m_bEnable( false )
#ifdef SERV_ALLOW_EVENT_ERASE
, m_bOn( false )
#endif SERV_ALLOW_EVENT_ERASE
{

}

KGameEvent::~KGameEvent()
{

}