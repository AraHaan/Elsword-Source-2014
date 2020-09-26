//#pragma once  // ������ include ����. ��� #include "ImportKncSDK.h"�� ����� ����� �ѹ��� include�ǵ��� ��������.

#define LIBNAME "KOGGamePerformanceCheck"

#if (_MSC_VER == 1300 )   // Visual Studio .NET 2003
#   define VSTYPE   "_2003"
#elif (_MSC_VER == 1400 )   // Visual Studio 2005
#   define VSTYPE   "_2005"
#elif (_MSC_VER == 1500 )   // Visual Studio 2008
#   define VSTYPE   "_2008"
#elif (_MSC_VER == 1600 )   // Visual Studio 2010
#   define VSTYPE   "_2010"
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
#endif

#pragma message( LIBNAME VSTYPE CRTTYPE ".lib will be imported" )
#pragma comment( lib, LIBNAME VSTYPE CRTTYPE )
#pragma message( LIBNAME VSTYPE CRTTYPE ".lib is imported" )

#undef LIBNAME
#undef VSTYPE
#undef CRTTYPE
