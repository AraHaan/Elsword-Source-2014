#include "stdafx.h"
#include "CommonDefine.h"

const wchar_t* GetAuthString( int iAuthLevel )
{
	switch( iAuthLevel )
	{
	case NORMAL:
		return L"���";
	case SUPER:
		return L"����������";
	case ADMIN:
		return L"����������";
	default:
		return L"";
	}
}