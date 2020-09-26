#include "SecureBuffer.h"
#include "SecurityAssociation.h"
#include "SADatabase.h"
#include "dbg/dbg.hpp"
#include "KncSecurity.h"

NAMESPACE_BEGIN( KncSecurity )

KSecureBuffer::KSecureBuffer( SpiType nSPI ) : m_nSPIndex( nSPI )
{
}

KSecureBuffer::KSecureBuffer( const KSecureBuffer& buf )
:
    m_bsBuf     ( buf.m_bsBuf ),
    m_nSPIndex  ( buf.m_nSPIndex )
{
}

KSecureBuffer::KSecureBuffer( SpiType nSPI, const void* pRecvData, unsigned long nBytes )
{
    m_bsBuf.assign( reinterpret_cast< const unsigned char* >( pRecvData ), nBytes );
    m_nSPIndex = nSPI;
}

KSecureBuffer::~KSecureBuffer()
{
}

// ���߿� ������ �� ����. KEvent �����͸� �޾Ƽ� �����ǵ��� ����.
bool KSecureBuffer::Create( const void* pData, unsigned long nBytes )
{
    return Create( KByteStream( reinterpret_cast<const unsigned char*>(pData), nBytes ) );
}

bool KSecureBuffer::Create( const KByteStream& payload )
{
	// ������ �����ʹ� ��� ����
	m_bsBuf.erase();

	// ���� ������ �̸� Ȯ���Ѵ�.
	m_bsBuf.reserve( GetMaxSecureSize( payload ) );

	// ������ �߰� : SPI
	m_bsBuf.Append( m_nSPIndex );

	// ������ �߰� : sequence number
	m_bsBuf.Append( GetSA().GetSequenceNum() );

	// IV�� �Ź� �����ϰ� �����Ѵ�.
	KByteStream iv;
	GenerateIV( iv );
	m_bsBuf.append( iv );

	// ������ IV�� ��ȣȭ�Ѵ�. (padding�� padding ��������� ��ȣȭ��)
	KByteStream crypt = GenerateCrypt( payload, iv );
	if( crypt.empty() )
		return false;

	// ������ �߰� : payload, padding, padding size
	m_bsBuf.append( crypt );

	// ���ݱ��� �߰��� �����͸� ��ǲ���� ICV ����
	KByteStream icv = GenerateICV( m_bsBuf );
	if( icv.empty() )
		return false;

	// ������ �߰� : ICV
	m_bsBuf.append( icv );

	// SA�� ������ ��ȣ�� ���������ش�. (������ ���� ��Ŷ�� sequence number��.)
	GetSA().IncrSequenceNum();    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Copy a secure KByteStream

//void KSecureBuffer::Set( const KByteStream& secureBuffer )
//{
//    m_bsBuf = secureBuffer;
//    m_nSPIndex = 0;
//}

///////////////////////////////////////////////////////////////////////////////
//
// Validate a secure KByteStream by examining the authentication data

bool KSecureBuffer::IsAuthentic() const
{
	// �����Ϳ��� SPI�� �����鼭, ���� SADB�� �ִ� SPI���� Ȯ���Ѵ�.
	SpiType nSPIndex = 0;
	if( !IsValidSPIndex( nSPIndex ) )
		return false;

	// ���� SPI�� ��� ������ ����Ѵ�.
	//m_nSPIndex = nSPIndex;    // 060302. florist. Ŭ���̾�Ʈ�� ������ SPindex�� �޶� ó�������ϵ��� ����.

	// Check packet size
	if( !IsValidSize() )
		return false;

	// Validate the Integrity Check Value (ICV)
	if( !IsValidICV() )
		return false;

	// Check sequence number
	SeqType nSequenceNum;
	if( !IsValidSequenceNum( nSequenceNum ) )
		return false;
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Extract the payload from a secure KByteStream. Returns false if the payload
// is invalid or cannot be retrieved. Assumes the KByteStream has already been
// authenticated.

bool KSecureBuffer::GetPayload( KByteStream& payload ) const
{
	_JIF( IsAuthentic(), false );

    // �����Ϳ��� IV�� ����. IV�� SPI�� SeqNum �ڿ� ��ġ�Ѵ�.
    unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
    unsigned long nSize = GetSA().GetIVSize();
    if( nPos + nSize > m_bsBuf.length() )
        return false;
    KByteStream iv = m_bsBuf.substr( nPos, nSize );

    // Fill in the remaining portion of the IV with zero bytes
    // �� IV �ڿ� ���� �ڸ����� 0���� �ʱ�ȭ�Ѵ�.
    // �� IV�� �̹� GetSA().GetIVSize()�� ���̴�.
    iv.resize( GetSA().GetIVSize() );

    // �����Ϳ��� ��ȣȭ �� �κ��� �����Ѵ�. IV �ڿ� ������, ���� ���� ICV�� �����Ѵ�.
    nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
    if( nPos + GetSA().GetICVSize() > m_bsBuf.length() )
        return false;
    nSize = m_bsBuf.length() - nPos - GetSA().GetICVSize();
    KByteStream crypt = m_bsBuf.substr( nPos, nSize );

    // ��ȣȭ�Ѵ�.
    payload.clear();
    payload = GetSA().Decryption( crypt, iv );
    if( payload.empty() ) 
        return false;

    // padding ���̸� ����, padding�� ���� �ùٸ��� Ȯ���Ѵ�.
    unsigned long nPadBytes;
    if( !IsValidPadding( payload, nPadBytes ) )
        return false;

    // padding�� padding size ����Ʈ�� �����Ѵ�.
    _JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
    payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Extract the payload from a secure KByteStream. This function is designed to
// work with socket recv.

bool KSecureBuffer::GetPayload( char* pPayload, int* pnBytes )
{
    _JIF( pPayload != NULL, false );
    _JIF( pnBytes != NULL, false );
    KByteStream buf;
    if( !GetPayload( buf ) )
        return false;

    *pnBytes = buf.length();
    buf.copy( reinterpret_cast< unsigned char* >( pPayload ), *pnBytes );
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Marks a given KByteStream as authenticated and accepted. Automatically adjusts
// the replay window, so that IsAuthentic() will no longer validate the
// KByteStream correctly. Call this function only after calling IsAuthentic() and
// GetPayload(), but prior to calling IsAuthentic() on the next packet.

void KSecureBuffer::SetAccepted()
{
    SeqType nSequenceNum;
    bool bSuccess = IsValidSequenceNum( nSequenceNum );
    JIF( bSuccess );

    // Update the replay window
    GetSA().UpdateReplayWindow( nSequenceNum );
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 15  ������	Sequence Number ( not have replay window mask )

bool KSecureBuffer::Create_notRWM( const KByteStream& payload )
{
	// ������ �����ʹ� ��� ����
	m_bsBuf.erase();

	// ���� ������ �̸� Ȯ���Ѵ�.
	m_bsBuf.reserve( GetMaxSecureSize( payload ) );

	// ������ �߰� : SPI
	m_bsBuf.Append( m_nSPIndex );

	// ������ �߰� : sequence number
	m_bsBuf.Append( GetSA().GetSequenceNum() );

	// IV�� �Ź� �����ϰ� �����Ѵ�.
	KByteStream iv;
	GenerateIV( iv );
	m_bsBuf.append( iv );

	// ������ IV�� ��ȣȭ�Ѵ�. (padding�� padding ��������� ��ȣȭ��)
	KByteStream crypt = GenerateCrypt( payload, iv );
	if( crypt.empty() )
		return false;

	// ������ �߰� : payload, padding, padding size
	m_bsBuf.append( crypt );

	// ���ݱ��� �߰��� �����͸� ��ǲ���� ICV ����
	KByteStream icv = GenerateICV( m_bsBuf );
	if( icv.empty() )
		return false;

	// ������ �߰� : ICV
	m_bsBuf.append( icv );

	// SPIndex�� 0�� �ƴѰ�쿡�� sequence number�� ����!
	// SA�� ������ ��ȣ�� ���������ش�. (������ ���� ��Ŷ�� sequence number��.)
	if( !IsDefaultSPIndex() )
	{
		GetSA().IncrSequenceNum();
	}

	return true;
}

bool KSecureBuffer::IsAuthentic_notRWM( bool bCheckSequenceNum /*= true*/ )
{
	// �����Ϳ��� SPI�� �����鼭, ���� SADB�� �ִ� SPI���� Ȯ���Ѵ�.
	SpiType nSPIndex = 0;
	if( !IsValidSPIndex( nSPIndex ) )
		return false;

	// ���� SPI�� ��� ������ ����Ѵ�.
	//m_nSPIndex = nSPIndex;    // 060302. florist. Ŭ���̾�Ʈ�� ������ SPindex�� �޶� ó�������ϵ��� ����.

	// Check packet size
	if( !IsValidSize() )
		return false;

	// Validate the Integrity Check Value (ICV)
	if( !IsValidICV() )
		return false;

	// SPIndex�� 0�� �ƴѰ�쿡�� sequence number�� üũ! - üũ���� �������� �ܺο��� �������ڷ� ��������
	if( !IsDefaultSPIndex()  &&  bCheckSequenceNum )
	{
		// Check sequence number
		SeqType nSequenceNum;
		if( !IsValidSequenceNum_notRWM( nSequenceNum ) )
			return false;
	}

	return true;
}

bool KSecureBuffer::GetPayload_notRWM( KByteStream& payload, bool bAuthentic /*= true*/, bool bCheckSequenceNum /*= true*/ )
{
	//{{ 2009. 10. 15  ������	���������� �ǳʶٴ� ��� �߰�
	if( bAuthentic )
	{
		_JIF( IsAuthentic_notRWM( bCheckSequenceNum ), false );
	}
	//}}

	// �����Ϳ��� IV�� ����. IV�� SPI�� SeqNum �ڿ� ��ġ�Ѵ�.
	unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
	unsigned long nSize = GetSA().GetIVSize();
	if( nPos + nSize > m_bsBuf.length() )
		return false;
	KByteStream iv = m_bsBuf.substr( nPos, nSize );

	// Fill in the remaining portion of the IV with zero bytes
	// �� IV �ڿ� ���� �ڸ����� 0���� �ʱ�ȭ�Ѵ�.
	// �� IV�� �̹� GetSA().GetIVSize()�� ���̴�.
	iv.resize( GetSA().GetIVSize() );

	// �����Ϳ��� ��ȣȭ �� �κ��� �����Ѵ�. IV �ڿ� ������, ���� ���� ICV�� �����Ѵ�.
	nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
	if( nPos + GetSA().GetICVSize() > m_bsBuf.length() )
		return false;
	nSize = m_bsBuf.length() - nPos - GetSA().GetICVSize();
	KByteStream crypt = m_bsBuf.substr( nPos, nSize );

	// ��ȣȭ�Ѵ�.
	payload.clear();
	payload = GetSA().Decryption( crypt, iv );
	if( payload.empty() ) 
		return false;

	// padding ���̸� ����, padding�� ���� �ùٸ��� Ȯ���Ѵ�.
	unsigned long nPadBytes;
	if( !IsValidPadding( payload, nPadBytes ) )
		return false;

	// padding�� padding size ����Ʈ�� �����Ѵ�.
	_JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
	payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

	return true;
}

//}}
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// Returns a pointer to the beginning of the secure data. This function is
// designed to work with socket send().

const char* KSecureBuffer::GetDataPtr() const
{
    return( reinterpret_cast< const char* >( m_bsBuf.data() ) );
}

///////////////////////////////////////////////////////////////////////////////
//
// Returns the size of the secure KByteStream in bytes

unsigned long KSecureBuffer::GetSize() const
{
    return m_bsBuf.length();
}

const KSecurityAssociation& KSecureBuffer::GetSA() const
{
    return GetSADB().GetSA( m_nSPIndex );
}

KSecurityAssociation& KSecureBuffer::GetSA()
{
    return GetSADB().GetSA( m_nSPIndex );
}

void KSecureBuffer::GenerateIV( KByteStream& bsIV_ ) const
{
    bsIV_.clear();
    bsIV_.resize( GetSA().GetIVSize(), 'A' + rand() % 40 );

    //switch( rand() % 5 ) {
    //case 0: bsIV_.Assign( "�ӽ÷� ���� ���̺���", 8 ); break;
    //case 1: bsIV_.Assign( "crash�� �� �߻��� �� ����", 8 ); break;
    //case 2: bsIV_.Assign( "���� �� �߻����� �ʴ´ٸ�", 8 ); break;
    //case 3: bsIV_.Assign( "GetIVSize�� ũ�⸦ �־��ִ� �κ���", 8 ); break;
    //case 4: bsIV_.Assign( "�ǽ��غ� �� ����", 8 ); break;
    //default: bsIV_.Assign( "���� �߻����� �ʾƾ� �� �ڵ�", 8 ); break;
    //}

    //GetRandomGenerator().GenerateBlock( &bsIV_[0], GetSA().GetIVSize() );
}

KByteStream KSecureBuffer::GenerateCrypt( const KByteStream& payload, const KByteStream& iv ) const
{
    KByteStream crypt( payload );

    // Append padding, if any
    KByteStream pad = GeneratePadding( payload );
    crypt.append( pad );

    // Append pad length
    crypt.Append( pad.length(), 1 );

    return GetSA().Encryption( crypt, iv );
}

///////////////////////////////////////////////////////////////////////////////
//
// Generate the padding bytes based on the given payload

KByteStream KSecureBuffer::GeneratePadding( const KByteStream& payload ) const
{
    // The size of the payload, the padding and the pad length (1 byte)
    // must be evenly divisible by nBlockBytes
    // 1����� ������ �е��Ǿ�� �Ѵ�. ������ ���̰� ��ϴ����� �� ������ ������ ��� �ϳ���,
    // �� ���� ������ ������ ��ŭ�� ä������� �ҿ��� ��� �ϳ��� ä���.
    unsigned long nBlockBytes = GetSA().GetBlockSize();
    unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

    // Add some random padding to hide the true size of the payload
    // �� ����, ������ ����� ����� ���ؼ� �߰������� ���� �е� ��� ���� ��´�.
    unsigned char nRand = 0; // GetRandomGenerator().GenerateByte();
    unsigned long nRandBlocks = 0;
    // �ִ� extra block ������ 5��� ġ��, 5 + 1�� 6���� mod�ؾ� [0,5]�� ���� ���´�.
    // ���� �е� ����� 0�̸�, rand % 1�� �ǹǷ� ���� �׻� 0�̴�.
    nRandBlocks = nRand % (GetSA().GetMaxExtraPadBlocks() + 1);
    nPadBytes += nBlockBytes * nRandBlocks;

    // Create the padding KByteStream
    KByteStream pad;
    pad.resize( nPadBytes );

    // RFC 2406 says padding bytes are initialized with a series of 
    // one-byte integer values
    unsigned char* pData = &pad[0];
    for( unsigned long i = 1; i <= nPadBytes; ++i, ++pData )
        *pData = unsigned char( i );

    return pad;
}

///////////////////////////////////////////////////////////////////////////////
//
// Generate the Integrity Check Value of the given KByteStream

KByteStream KSecureBuffer::GenerateICV( const KByteStream& auth ) const
{
    return GetSA().GenerateICV( auth );
}

///////////////////////////////////////////////////////////////////////////////
//
// Determine the max size of the secure KByteStream based on the given payload

unsigned long KSecureBuffer::GetMaxSecureSize( const KByteStream& payload ) const
{
    unsigned long nSize = 0;
    nSize += sizeof( SpiType );         // SPI
    nSize += sizeof( SeqType );         // sequence number
    nSize += GetSA().GetIVSize();       // IV
    nSize += payload.size();            // payload
    nSize += GetMaxPadSize( payload );  // padding
    nSize += sizeof( unsigned char );   // pad length
    nSize += GetSA().GetICVSize();      // ICV
    return nSize;
}

unsigned long KSecureBuffer::GetMaxPadSize( const KByteStream& payload ) const
{
    // ��� ũ�⸦ ���Ѵ�.
    unsigned long nBlockBytes = GetSA().GetBlockSize();

    // �ϴ� �⺻������ �е��Ǿ�� �� ����Ʈ ���� ���Ѵ�.
    unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

    // ���� �е� ��� ���� �߰��� ���ؼ� �����ش�.
    nPadBytes += GetSA().GetMaxExtraPadBlocks() * nBlockBytes;

    LIF( ( payload.length() + 1 + nPadBytes ) % nBlockBytes  == 0 );
    LIF( nPadBytes < 255 );
    return nPadBytes;
}

bool KSecureBuffer::IsValidSize() const
{
    // Assume no padding for quick check
    if( m_bsBuf.length() < sizeof( SpiType ) +      // SPI
                        sizeof( SeqType ) +         // sequence number
                        GetSA().GetIVSize() +       // IV
                        sizeof( unsigned char ) +   // pad size
                        GetSA().GetICVSize() )      // ICV
        return false;

    // KByteStream meets minimum requires; other checks performed later
    return true;
}

bool KSecureBuffer::IsValidSPIndex( SpiType& nSPIndex ) const
{
    // ���ۿ� ����ִ� �������� SPI�� ��ȿ���� �˻��Ѵ�.

    // �������� ũ�Ⱑ SPI ������ٵ� ���� ���.
    if( m_bsBuf.length() < sizeof( SpiType ) ) return false;

    // �����Ϳ��� SPI�� �̾Ƴ���. ���� ó���� ����ִ�.
    nSPIndex = *reinterpret_cast< const SpiType* >( m_bsBuf.data() );

    //return GetSADB().Find( nSPIndex );    // 060302. florist. ������ Ŭ���̾�Ʈ���� ��ϵ� SPIndex�� �޶� �����ϵ��� ����.
    return GetSADB().Find( m_nSPIndex );
}

bool KSecureBuffer::IsValidSequenceNum( SeqType& nSequenceNum ) const
{
    // �����Ϳ��� Sequence Number�� �д´�. SPI �ٷ� �ڿ� �ִ�.
    nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

    return GetSA().IsValidSequenceNum( nSequenceNum );
}

//{{ 2009. 10. 15  ������	LastSequenceNum����
bool KSecureBuffer::IsValidSequenceNum_notRWM( SeqType& nSequenceNum )
{
	// �����Ϳ��� Sequence Number�� �д´�. SPI �ٷ� �ڿ� �ִ�.
	nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

	return GetSA().IsValidSequenceNum_notRWM( nSequenceNum );
}
//}}

///////////////////////////////////////////////////////////////////////////////
//
// Determines if the ICV is valid


bool KSecureBuffer::IsValidICV() const
{
    // ������ ���� �������� �ִ� ICV�� �о�´�.
    unsigned long nSize = GetSA().GetICVSize();
    unsigned long nPos = m_bsBuf.length() - nSize;
    KByteStream icv = m_bsBuf.substr( nPos, nSize );

    // ICV ���� ������ ������ ������ (ICV ����� ��ǲ�� �� ������)�� �о�´�.
    nSize = nPos;
    KByteStream auth = m_bsBuf.substr( 0, nSize );

    // ICV�� ����Ѵ�.
    KByteStream icvCompare = GetSA().GenerateICV( auth );

    // ����� ICV�� ���Ź��� ICV���� ���Ѵ�.
    //return( icv == icvCompare );
    if( icv != icvCompare )
    {
        START_LOG( cerr, "SPI : " << m_nSPIndex << dbg::endl
            << "icv : " << icv << dbg::endl
            << "icvCompare : " << icvCompare << dbg::endl );

        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Determines if the given padding is valid

bool KSecureBuffer::IsValidPadding( const KByteStream& payload, unsigned long& nPadBytes ) const
{
    // ��ȣȭ�� ��ģ payload�� �Է����� �޴´� (���� ������ + padding + padding size)
    
    // �ּ��� padding size�� �����ϰ� �־�� �Ѵ�.
    if( payload.length() == 0 )
        return false;

    // padding size�� ����. ���� ������ ����Ʈ�� ��ġ�Ѵ�.
    nPadBytes = *( payload.data() + payload.length() - 1 );

    // �����ʹ� �ּ��� padding size��ŭ�� ũ��� size ǥ�� ����Ʈ ũ�⺸�ٴ� Ŀ�� �Ѵ�.
    if( nPadBytes + sizeof(unsigned char) > payload.length() )
        return false;

    // �����Ϳ��� padding �� �κ��� ����.
    unsigned long nPos = payload.length() - nPadBytes - sizeof(unsigned char);
    KByteStream pad = payload.substr( nPos, nPadBytes );

    // padding bytes�� �ùٸ����� Ȯ���Ѵ�. 1���� �����ϴ� ���ӵ� one-byte integer���� �Ѵ�.
    const unsigned char* pData = pad.data();
    for( unsigned long i = 1; i <= nPadBytes; ++i, ++pData )
    {
        if( *pData != unsigned char(i) )
            return false;
    }

    // Padding is good
    return true;
}

//{{
//////////////////////////////////////////////////////////////////////////
// SADatabase �� Session ��ü�� �ɹ��� ��� �ϱ� ���Ͽ� �߰��� ����
//////////////////////////////////////////////////////////////////////////

bool KSecureBuffer::Create_Local( const void* pData, unsigned long nBytes, KSADatabase* pSADatabase )
{
	return Create_Local( KByteStream( reinterpret_cast<const unsigned char*>(pData), nBytes ), pSADatabase );
}

bool KSecureBuffer::Create_Local( const KByteStream& payload, KSADatabase* pSADatabase )
{
	// ���� Create �� ���� �Ķ���͸� �߰��� ����
	if( pSADatabase == NULL )
	{
		return false;
	}

	m_bsBuf.erase();
	m_bsBuf.reserve( GetMaxSecureSize_Local( payload, pSADatabase ) );
	m_bsBuf.Append( m_nSPIndex );
	m_bsBuf.Append( pSADatabase->GetSA( m_nSPIndex ).GetSequenceNum() );

	KByteStream iv;
	GenerateIV_Local( iv, pSADatabase );
	m_bsBuf.append( iv );

	KByteStream crypt = GenerateCrypt_Local( payload, iv, pSADatabase );
	if( crypt.empty() )
		return false;

	m_bsBuf.append( crypt );

	KByteStream icv = GenerateICV_Local( m_bsBuf, pSADatabase );
	if( icv.empty() )
		return false;

	m_bsBuf.append( icv );

	pSADatabase->GetSA( m_nSPIndex ).IncrSequenceNum();    
	return true;
}

bool KSecureBuffer::GetPayload_Local( KByteStream& payload, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}
	_JIF( IsAuthentic_Local( pSADatabase ), false );

	unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
	unsigned long nSize = pSADatabase->GetSA( m_nSPIndex ).GetIVSize();
	if( nPos + nSize > m_bsBuf.length() )
		return false;
	KByteStream iv = m_bsBuf.substr( nPos, nSize );

	iv.resize( pSADatabase->GetSA( m_nSPIndex ).GetIVSize() );

	nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
	if( nPos + pSADatabase->GetSA( m_nSPIndex ).GetICVSize() > m_bsBuf.length() )
		return false;
	nSize = m_bsBuf.length() - nPos - pSADatabase->GetSA( m_nSPIndex ).GetICVSize();
	KByteStream crypt = m_bsBuf.substr( nPos, nSize );

	payload.clear();
	payload = pSADatabase->GetSA( m_nSPIndex ).Decryption( crypt, iv );
	if( payload.empty() ) 
		return false;

	unsigned long nPadBytes;
	if( !IsValidPadding( payload, nPadBytes ) )
		return false;

	_JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
	payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

	return true;
}

bool KSecureBuffer::GetPayload_Local( char* pPayload, int* pnBytes, KSADatabase* pSADatabase )
{
	_JIF( pPayload != NULL, false );
	_JIF( pnBytes != NULL, false );
	KByteStream buf;
	if( !GetPayload_Local( buf, pSADatabase ) )
		return false;

	*pnBytes = buf.length();
	buf.copy( reinterpret_cast< unsigned char* >( pPayload ), *pnBytes );
	return true;
}

void KSecureBuffer::SetAccepted_Local( KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return;
	}

	SeqType nSequenceNum;
	bool bSuccess = IsValidSequenceNum( nSequenceNum );
	JIF( bSuccess );

	pSADatabase->GetSA( m_nSPIndex ).UpdateReplayWindow( nSequenceNum );
}

bool KSecureBuffer::Create_notRWM_Local( const KByteStream& payload, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}

	m_bsBuf.erase();
	m_bsBuf.reserve( GetMaxSecureSize( payload ) );
	m_bsBuf.Append( m_nSPIndex );
	m_bsBuf.Append( pSADatabase->GetSA( m_nSPIndex ).GetSequenceNum() );

	KByteStream iv;
	GenerateIV_Local( iv, pSADatabase );
	m_bsBuf.append( iv );

	KByteStream crypt = GenerateCrypt_Local( payload, iv, pSADatabase );
	if( crypt.empty() )
		return false;

	m_bsBuf.append( crypt );

	KByteStream icv = GenerateICV_Local( m_bsBuf, pSADatabase );
	if( icv.empty() )
		return false;

	m_bsBuf.append( icv );

	if( !IsDefaultSPIndex() )
	{
		pSADatabase->GetSA( m_nSPIndex ).IncrSequenceNum();
	}

	return true;
}

bool KSecureBuffer::IsAuthentic_notRWM_Local( bool bCheckSequenceNum /*= true*/, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}

	SpiType nSPIndex = 0;
	if( !IsValidSPIndex_Local( nSPIndex, pSADatabase ) )
		return false;

	if( !IsValidSize_Local( pSADatabase ) )
		return false;

	if( !IsValidICV_Local( pSADatabase ) )
		return false;

	if( !IsDefaultSPIndex()  &&  bCheckSequenceNum )
	{
		SeqType nSequenceNum;
		if( !IsValidSequenceNum_notRWM_Local( nSequenceNum, pSADatabase ) )
			return false;
	}

	return true;
}

bool KSecureBuffer::GetPayload_notRWM_Local( KByteStream& payload, bool bAuthentic /*= true*/, bool bCheckSequenceNum /*= true*/, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}

	if( bAuthentic )
	{
		_JIF( IsAuthentic_notRWM_Local( bCheckSequenceNum, pSADatabase ), false );
	}

	unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
	unsigned long nSize = pSADatabase->GetSA( m_nSPIndex ).GetIVSize();
	if( nPos + nSize > m_bsBuf.length() )
		return false;
	KByteStream iv = m_bsBuf.substr( nPos, nSize );

	iv.resize( pSADatabase->GetSA( m_nSPIndex ).GetIVSize() );

	nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
	if( nPos + pSADatabase->GetSA( m_nSPIndex ).GetICVSize() > m_bsBuf.length() )
		return false;
	nSize = m_bsBuf.length() - nPos - GetSA().GetICVSize();
	KByteStream crypt = m_bsBuf.substr( nPos, nSize );

	payload.clear();
	payload = pSADatabase->GetSA( m_nSPIndex ).Decryption( crypt, iv );
	if( payload.empty() ) 
		return false;

	unsigned long nPadBytes;
	if( !IsValidPadding( payload, nPadBytes ) )
		return false;

	_JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
	payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

	return true;
}

void KSecureBuffer::GenerateIV_Local( KByteStream& bsIV_, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return;
	}

	bsIV_.clear();
	bsIV_.resize( pSADatabase->GetSA( m_nSPIndex ).GetIVSize(), 'A' + rand() % 40 );
}

KByteStream KSecureBuffer::GenerateCrypt_Local( const KByteStream& payload, const KByteStream& iv, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  KByteStream();
	}

	KByteStream crypt( payload );

	KByteStream pad = GeneratePadding_Local( payload, pSADatabase );
	crypt.append( pad );
	crypt.Append( pad.length(), 1 );

	return pSADatabase->GetSA( m_nSPIndex ).Encryption( crypt, iv );
}

KByteStream KSecureBuffer::GeneratePadding_Local( const KByteStream& payload, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  KByteStream();
	}

	unsigned long nBlockBytes = pSADatabase->GetSA( m_nSPIndex ).GetBlockSize();
	unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

	unsigned char nRand = 0;
	unsigned long nRandBlocks = 0;

	nRandBlocks = nRand % (pSADatabase->GetSA( m_nSPIndex ).GetMaxExtraPadBlocks() + 1);
	nPadBytes += nBlockBytes * nRandBlocks;

	KByteStream pad;
	pad.resize( nPadBytes );

	unsigned char* pData = &pad[0];
	for( unsigned long i = 1; i <= nPadBytes; ++i, ++pData )
		*pData = unsigned char( i );

	return pad;
}

KByteStream KSecureBuffer::GenerateICV_Local( const KByteStream& auth, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  KByteStream();
	}

	return pSADatabase->GetSA( m_nSPIndex ).GenerateICV( auth );
}

unsigned long KSecureBuffer::GetMaxSecureSize_Local( const KByteStream& payload, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  0;
	}

	unsigned long nSize = 0;
	nSize += sizeof( SpiType );         // SPI
	nSize += sizeof( SeqType );         // sequence number
	nSize += pSADatabase->GetSA( m_nSPIndex ).GetIVSize();       // IV
	nSize += payload.size();            // payload
	nSize += GetMaxPadSize_Local( payload, pSADatabase );  // padding
	nSize += sizeof( unsigned char );   // pad length
	nSize += pSADatabase->GetSA( m_nSPIndex ).GetICVSize();      // ICV
	return nSize;
}

unsigned long KSecureBuffer::GetMaxPadSize_Local( const KByteStream& payload, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  0;
	}

	unsigned long nBlockBytes = pSADatabase->GetSA( m_nSPIndex ).GetBlockSize();

	unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

	nPadBytes += pSADatabase->GetSA( m_nSPIndex ).GetMaxExtraPadBlocks() * nBlockBytes;

	LIF( ( payload.length() + 1 + nPadBytes ) % nBlockBytes  == 0 );
	LIF( nPadBytes < 255 );
	return nPadBytes;
}

bool KSecureBuffer::IsValidSize_Local( KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	if( m_bsBuf.length() < sizeof( SpiType ) +      // SPI
		sizeof( SeqType ) +         // sequence number
		pSADatabase->GetSA( m_nSPIndex ).GetIVSize() +       // IV
		sizeof( unsigned char ) +   // pad size
		pSADatabase->GetSA( m_nSPIndex ).GetICVSize() )      // ICV
		return false;

	return true;
}

bool KSecureBuffer::IsValidSPIndex_Local( SpiType& nSPIndex, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	if( m_bsBuf.length() < sizeof( SpiType ) ) return false;

	nSPIndex = *reinterpret_cast< const SpiType* >( m_bsBuf.data() );

	return pSADatabase->Find( m_nSPIndex );
}

bool KSecureBuffer::IsValidSequenceNum_Local( SeqType& nSequenceNum, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

	return pSADatabase->GetSA( m_nSPIndex ).IsValidSequenceNum( nSequenceNum );
}

bool KSecureBuffer::IsValidSequenceNum_notRWM_Local( SeqType& nSequenceNum, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

	return pSADatabase->GetSA( m_nSPIndex ).IsValidSequenceNum_notRWM( nSequenceNum );
}

bool KSecureBuffer::IsValidICV_Local( KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	unsigned long nSize = pSADatabase->GetSA( m_nSPIndex ).GetICVSize();
	unsigned long nPos = m_bsBuf.length() - nSize;
	KByteStream icv = m_bsBuf.substr( nPos, nSize );

	nSize = nPos;
	KByteStream auth = m_bsBuf.substr( 0, nSize );

	KByteStream icvCompare = pSADatabase->GetSA( m_nSPIndex ).GenerateICV( auth );
	if( icv != icvCompare )
	{
		START_LOG( cerr, "SPI : " << m_nSPIndex << dbg::endl
			<< "icv : " << icv << dbg::endl
			<< "icvCompare : " << icvCompare << dbg::endl );

		return false;
	}

	return true;
}

bool KSecureBuffer::IsAuthentic_Local( KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	SpiType nSPIndex = 0;
	if( !IsValidSPIndex_Local( nSPIndex, pSADatabase ) )
		return false;

	if( !IsValidSize_Local( pSADatabase ) )
		return false;

	if( !IsValidICV_Local( pSADatabase ) )
		return false;

	SeqType nSequenceNum;
	if( !IsValidSequenceNum_Local( nSequenceNum, pSADatabase ) )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// SecureBuffer �� Session ��ü�� �ɹ��� ��� �ϱ� ���Ͽ� �߰��� ����
//////////////////////////////////////////////////////////////////////////
//}}

ImplOstmOperatorA( KByteStream );
ImplOstmOperatorA2W( KByteStream );
ImplOstmOperatorA( KSecureBuffer );

ImplToStringA( KSecureBuffer )
{
    return stm_ << "KSecureBuffer - " << m_bsBuf;
}

NAMESPACE_END