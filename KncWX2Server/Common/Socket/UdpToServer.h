#pragma once

#include "KncUidType.h"

#pragma pack( push, 1 )

struct KUdpToServer
{
    UidType             m_nUserUID;     ///< ���� ���̵�(Login)
    unsigned short      m_usPort;       ///< Ŭ���̾�Ʈ�� ������ ��Ʈ
};

#pragma pack( pop )
