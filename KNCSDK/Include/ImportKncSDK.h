//#pragma once  // ������ include ����. ��� #include "ImportKncSDK.h"�� ����� ����� �ѹ��� include�ǵ��� ��������.

#ifndef KNCLIBNAME
#error ImportKOGSDK.h�� include �ϱ� ���� KNCLIBNAME ��ũ�θ� ������ �־�� �մϴ�.
#endif

#if (_MSC_VER == 1200 )     // Visual Studio 6.0
#   error KOGSDK�� Microsoft Visual Studio 6.0 �����Ϸ��� �������� �ʽ��ϴ�. 
#elif (_MSC_VER == 1300 )   // Visual Studio .NET 2002
#   error KOGSDK�� Microsoft Visual Studio .NET 2002 �����Ϸ��� �������� �ʽ��ϴ�. 
#elif (_MSC_VER == 1310 )   // Visual Studio .NET 2003
#   define CLVERSION   "71"
#elif (_MSC_VER == 1400 )   // Visual Studio 2005 Express Edition
#   error KOGSDK�� ���� Visual Studio 2005 Express Edition�� �������� �ʽ��ϴ�. �غ����Դϴ�.
#endif

#ifdef _MT
#   ifdef _DLL
#       ifdef _DEBUG
#           define CRTTYPE  "MDd"   // _MDd
#       else
#           define CRTTYPE  "MD"    // _MD
#       endif
#   else
#       ifdef _DEBUG
#           define CRTTYPE  "MTd"   // _MTd
#       else
#           define CRTTYPE  "MT"    // _MT
#       endif
#   endif
#else
#   ifdef _DEBUG
#       define CRTTYPE  "MLd"       // _MLd
#   else
#       define CRTTYPE  "ML"        // _ML
#   endif
#endif

#include <iostream>     // a Standard C++ Library Header

#pragma comment( lib, KNCLIBNAME CLVERSION CRTTYPE )
#pragma message( KNCLIBNAME CLVERSION CRTTYPE ".lib is imported" )

#undef KNCLIBNAME
#undef CLVERSION
#undef CRTTYPE
