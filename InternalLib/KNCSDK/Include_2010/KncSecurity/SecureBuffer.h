#pragma once

#include "ByteStream.h"
#include "ToString.h"
#include "SADatabase.h"

NAMESPACE_BEGIN( KncSecurity )

class KSecurityAssociation;

class KSecureBuffer
{
    DeclToStringA;

public:
    KSecureBuffer( SpiType nSPI );                                  // for sender
    KSecureBuffer( const KSecureBuffer& buf );                      // for... for test -_-;
    KSecureBuffer( SpiType nSPI, const void* pRecvData, unsigned long nBytes );   // for receiver
    KSecureBuffer& operator=( const KSecureBuffer& );
    ~KSecureBuffer();

    bool Create( const KByteStream& );
    bool Create( const void*, unsigned long nBytes );
    //void Set( const KByteStream& );
    bool IsAuthentic() const;   // GePayload()���� �ѹ� ȣ���. �ܺο��� �Ҹ� �� ����
    bool GetPayload( KByteStream& payload ) const;
    bool GetPayload( char*, int* );
    void SetAccepted();

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 10. 15  ������	Sequence Number ( not have replay window mask )
	bool Create_notRWM( const KByteStream& );
	bool IsAuthentic_notRWM( bool bCheckSequenceNum = true ); // GePayload()���� �ѹ� ȣ���. �ܺο��� �Ҹ� �� ����
	bool GetPayload_notRWM( KByteStream& payload, bool bAuthentic = true, bool bCheckSequenceNum = true );
	//}}
	//////////////////////////////////////////////////////////////////////////

    const char* GetDataPtr() const;     // for SOCKET I/O function.
    unsigned long GetSize() const;

private:
    KSecurityAssociation& GetSA();
    const KSecurityAssociation& GetSA() const;

    void GenerateIV( OUT KByteStream& bsIV ) const;
    KByteStream GenerateCrypt( const KByteStream& payload, const KByteStream& iv ) const;
    KByteStream GeneratePadding( const KByteStream& payload ) const;
    KByteStream GenerateICV( const KByteStream& auth ) const;

    unsigned long GetMaxSecureSize( const KByteStream& payload ) const;
    unsigned long GetMaxPadSize( const KByteStream& payload ) const;

	// �⺻ SPIndex���� �˻�
	inline bool IsDefaultSPIndex() const { return ( m_nSPIndex == 0 ); }
    // �����Ͱ� �ּҿ䱸 ������ �̻����� Ȯ���Ѵ�.
    inline bool IsValidSize() const;
    // �����Ϳ��� SPI�� �����鼭, ���� SADB�� �ִ� SPI���� Ȯ���Ѵ�.
    inline bool IsValidSPIndex( SpiType& nSPIndex ) const;
    // �����Ϳ��� SeqNumber�� �����鼭, ���� �ùٸ��� Ȯ���Ѵ�.
    inline bool IsValidSequenceNum( SeqType& nSequenceNum ) const;
	//{{ 2009. 10. 15  ������	LastSequenceNum����
	inline bool IsValidSequenceNum_notRWM( SeqType& nSequenceNum );
	//}}
    // �����Ϳ��� ���� ICV�� ���� ����� ICV�� ���Ѵ�.
    inline bool IsValidICV() const;
    bool IsValidPadding( const KByteStream& payload, unsigned long& nPadBytes ) const;

private:
    KByteStream             m_bsBuf;
    mutable SpiType         m_nSPIndex;  // Security Parameters Index
};

DeclOstmOperatorA( KSecureBuffer );
DeclOstmOperatorW( KByteStream );

NAMESPACE_END
