#pragma once

// KExceptionSafeUtil.cpp �� ��� '����'�� ���� 
// C/C++ -> �ڵ� ���� -> C++ ���� ó�� ���� �ɼ��� '��, SEH ���� ���� (/EHa) �� �����Ǿ�� ��
// direct3d dynamic vertex buffer ���� XP �ӽſ��� ũ���� �߻��ϴ� �ü�� ������ �����ϱ� ���� ����
// - robobeg, 2014-01-21

namespace   KExceptionSafeUtil
{
    bool    ESCopyMemory( void* destination, const void* source, size_t length ) throw();
}