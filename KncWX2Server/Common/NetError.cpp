#include "NetError.h"

#include <map>
#include <Windows.h>    // DWORD, GetCurrentThreadID
#include "Thread/Locker.h"

namespace NetError  // ���� ������ namespace scope�� ���� ���־�� �Ѵ�.
{

#undef NET_ERROR
#define NET_ERROR( id, comment ) L#comment,
//     wchar_t* szErrorStr[] = {
 //#include "NetError_def.h"
//     };
	
    // key : thread id. element : last error
    std::map<DWORD, NetErrorID> mapLastError;

    std::map<DWORD, NetErrorID>::iterator mit;     // ���� �۾��� ����..

    KncCriticalSection  csNetError;
}

//09. 08. 17 ������ Ŭ���̾�Ʈ���� �ܱ����� �״�� ����
wchar_t* NetError::GetErrStrF( NetErrorID errorID )
{
    KLocker lock( csNetError );
#ifdef SERV_NETERROR_EU
	if( errorID >= ERR_SENTINEL || errorID < 0 ) return szErrorStrDE[ ERR_SENTINEL ];
	return szErrorStrDE[ errorID ];
#else //SERV_NETERROR_EU
	if( errorID >= ERR_SENTINEL || errorID < 0 ) return szErrorStrF[ ERR_SENTINEL ];
	return szErrorStrF[ errorID ];
#endif //SERV_NETERROR_EU
}

wchar_t* NetError::GetErrStr( NetErrorID errorID )
{
    KLocker lock( csNetError );

    if( errorID >= ERR_SENTINEL || errorID < 0 ) return szErrorStr[ ERR_SENTINEL ];
    return szErrorStr[ errorID ];
}

void NetError::SetLastError( NetErrorID errorID_ )
{
    KLocker lock( csNetError );

    DWORD dwThreadID = ::GetCurrentThreadId();

    mit = mapLastError.find( dwThreadID );
    if( mit == mapLastError.end() ) // ���ο� �������� ����
    {
        mapLastError.insert( std::make_pair( dwThreadID, errorID_ ) );
        return;
    }

    mit->second = errorID_;  // ���� �������� �� ����.
}

int NetError::GetLastError()
{
    KLocker lock( csNetError );

    mit = mapLastError.find( ::GetCurrentThreadId() );

    if( mit == mapLastError.end() ) // ���ο� �������� ����
        return 0;

    return mit->second;
}

wchar_t* NetError::GetLastErrMsg()
{
    KLocker lock( csNetError );

#ifdef SERV_NETERROR_EU
	// 2010/11/09	��ȿ�� 	���� �޽��� �ѱ۷� ������ �� ����.
    return GetErrStrF( GetLastError() );
#else //SERV_NETERROR_EU
	return GetErrStr( GetLastError() ); 
#endif //SERV_NETERROR_EU
}
