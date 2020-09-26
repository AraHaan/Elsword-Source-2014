#ifndef __KHTTPWRAPPER_H__
#define __KHTTPWRAPPER_H__

#include "Patcher.h"
#include "KAssert.h"
#include <wininet.h>
#include <string>



/// HTTP ����� ���� �ٿ�ε带 ����ϴ� Ŭ����.
class KHttpWrapper
{
public:
    /// Default constructor.
    KHttpWrapper( void );
    /// Default destructor.
    ~KHttpWrapper( void );
private:
    /// Copy constructor - disabled.
    KHttpWrapper( const KHttpWrapper& )
    { ASSERT( false && _T("Do not use copy constructor!") ); }
    /// Assignment operator - disabled.
    const KHttpWrapper& operator=( const KHttpWrapper& )
    { ASSERT( false && _T("Do not use assignment operator!") ); return *this; }
public:
    /// HTTP ������ ����.
    bool Open( void );
    /// HTTP ������ �ݴ´�.
    void Close( void );
    /// HTTP ������ �����Ѵ�.
    bool Reset( void );
    /// �־��� ������ �ٿ�ε��Ѵ�.
    bool Get( const char* szSrcPath_,
              const char* szFileName_,
              const char* szDestPath_,
			  bool bInvalidRect = true ) const;
private:
    /// �־��� ���ڿ����� '\'�� '/'�� �ٲ۴�.
    void BackSlash2Slash( const char* szSrc, char* szDst ) const;
    /// �־��� ���ڿ����� '/'�� '\'�� �ٲ۴�.
	void Slash2BackSlash( const char* szSrc, char* szDst ) const;
private:
    HINTERNET   m_hInternet; ///< Internet handle.
};

#endif // __KHTTPWRAPPER_H__