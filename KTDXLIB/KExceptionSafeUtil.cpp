#include "StdAfx.h"

// KExceptionSafeUtil.cpp �� ��� ������ ���� 
// C/C++ -> �ڵ� ���� -> C++ ���� ó�� ���� �ɼ��� '��, SEH ���� ���� (/EHa) �� �����Ǿ�� ��
// direct3d dynamic vertex buffer ���� XP �ӽſ��� ũ���� �߻��ϴ� �ü�� ������ �����ϱ� ���� ����
// - robobeg, 2014-01-21]

#pragma NOTE( "KExceptionSafeUtil.cpp �� ��� ������ ����/EHa �ɼ��� �����Ǿ� �־�� �մϴ�. (���� �ּ� ����)" )

#ifdef KTDGDEVICEFONT_SIZE_CACHE
bool    KExceptionSafeUtil::ESCopyMemory( void* destination, const void* source, size_t length ) throw()
{
    bool    bExceptionOccurred = false;

    try
    {
        CopyMemory( destination, source, length );
    }
    catch(...)
    {
        bExceptionOccurred = true;
    }

    return  bExceptionOccurred;
}//KExceptionSafeUtil::ESCopyMemory()
#else
#if defined(CLIENT_COUNTRY_TH)
bool    KExceptionSafeUtil::ESCopyMemory( void* destination, const void* source, size_t length ) throw()
{
	bool    bExceptionOccurred = false;

	try
	{
		CopyMemory( destination, source, length );
	}
	catch(...)
	{
		bExceptionOccurred = true;
	}

	return  bExceptionOccurred;
}
#endif // defined(CLIENT_COUNTRY_TH)
#endif