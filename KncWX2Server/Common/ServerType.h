#pragma once

enum ServerType
{
    ST_GUILD            = 0x00000001,   // ������ ��� ����.
    ST_BROADCASTING     = 0x00000002,   // ��ۿ� ����. ���������� �������� ������, Ŭ���̾�Ʈ�� �˷���.
    ST_OPEN_TESOP       = 0x00000004,   // Exp, GP�� ���Ѽ� ������ 3��� �ø���.
    ST_INTERNAL_TESOP   = 0x00000008,   // play time Ȯ�� �н�, üũ�� Ȯ�� �н�, db�� ���ī��Ʈ ��� ����.
    ST_NOUSE_01         = 0x00000010,   // 
    ST_INDIGO           = 0x00000020,   // ����� ��
    ST_NPGG_ON          = 0x00000040,   // NProtect Ȱ��ȭ
    ST_NPGG_KILL        = 0x00000080,   // NProtect �������� ���߽� ���� ���� ����.
    ST_DEBUG_AUTH       = 0x00000100,   // '����� ����'�̶�� �е��� ����. DB�� PASSWD���� ��ٷ� ��.
    ST_AHNHS_ON         = 0x00000200,   // HShield Ȱ��ȭ
    ST_AHNHS_KILL       = 0x00000400,   // HShield ���� ���� ���߽� ���� ���� ����.
    ST_FORCE_DWORD      = 0xFFFFFFFF,
};