#include "SecurityAssociation.h"
#include "cryptopp/des.h"
#include "cryptopp/modes.h"
#include "cryptopp/hmac.h"
#include "cryptopp/md5.h"
#include "KncSecurity.h"
#include "dbg/dbg.hpp"

#undef max
#include <boost/random.hpp>

SERIALIZE_DEFINE_PUT( KncSecurity::KSecurityAssociation, obj, ks )
{
    return  ks.Put( obj.m_bsAuthKey ) &&
        ks.Put( obj.m_bsCryptoKey ) &&
        ks.Put( obj.m_nSequenceNum ) &&
        ks.Put( obj.m_nLastSequenceNum ) &&
        ks.Put( obj.m_nReplayWindowMask );
}

SERIALIZE_DEFINE_GET( KncSecurity::KSecurityAssociation, obj, ks )
{
    return  ks.Get( obj.m_bsAuthKey ) &&
        ks.Get( obj.m_bsCryptoKey ) &&
        ks.Get( obj.m_nSequenceNum ) &&
        ks.Get( obj.m_nLastSequenceNum ) &&
        ks.Get( obj.m_nReplayWindowMask );
}

NAMESPACE_BEGIN( KncSecurity )

KSecurityAssociation::KSecurityAssociation()
:
    m_nSequenceNum( 1 ),
    m_nLastSequenceNum( 0 ),
    m_nReplayWindowMask( 0 )
{
    m_bsAuthKey.Assign( "�ӽ÷θ����Ű", AUTH_KEY_SIZE );
    m_bsCryptoKey.Assign( "��Ŀ�������Ű", CRYPTO_KEY_SIZE );
}

KSecurityAssociation::KSecurityAssociation( const KSecurityAssociation& sa )
:
    m_bsAuthKey             ( sa.m_bsAuthKey ),
    m_bsCryptoKey           ( sa.m_bsCryptoKey ),
    m_nSequenceNum          ( sa.m_nSequenceNum ),
    m_nLastSequenceNum      ( sa.m_nLastSequenceNum ),
    m_nReplayWindowMask     ( sa.m_nReplayWindowMask )
{
}

KSecurityAssociation& KSecurityAssociation::operator=( const KSecurityAssociation& sa )
{
    KSecurityAssociation temp( sa );
    Swap( temp );
    return *this;
}

bool KSecurityAssociation::operator==(const KSecurityAssociation& right) const
{
    return  m_bsAuthKey.compare( right.m_bsAuthKey ) == 0 &&
            m_bsCryptoKey.compare( right.m_bsCryptoKey ) == 0 &&
            m_nSequenceNum      == right.m_nSequenceNum &&
            m_nLastSequenceNum  == right.m_nLastSequenceNum &&
            m_nReplayWindowMask == right.m_nReplayWindowMask;
}


void KSecurityAssociation::Swap( KSecurityAssociation& sa )
{
    m_bsAuthKey.swap( sa.m_bsAuthKey );
    m_bsCryptoKey.swap( sa.m_bsCryptoKey );
    std::swap( m_nSequenceNum, sa.m_nSequenceNum );
    std::swap( m_nLastSequenceNum, sa.m_nLastSequenceNum );
    std::swap( m_nReplayWindowMask, sa.m_nReplayWindowMask );
}

KSecurityAssociation::~KSecurityAssociation()
{
}

void KSecurityAssociation::ResetRandomizeKey()
{
    static boost::mt19937 rng;
    static boost::uniform_int<DWORD> uint32( 1, std::numeric_limits<byte>::max() );   // 1 �̻�. SPI 0�� �����ڿ��� �̹� �����ߴ�.
    static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

    KByteStream::iterator sit;

    m_bsAuthKey.clear();
    m_bsAuthKey.resize( AUTH_KEY_SIZE );
    for( sit = m_bsAuthKey.begin(); sit != m_bsAuthKey.end(); sit++ )
    {
        *sit = static_cast<byte>( die() );
    }

    m_bsCryptoKey.clear();
    m_bsCryptoKey.resize( CRYPTO_KEY_SIZE );
    for( sit = m_bsCryptoKey.begin(); sit != m_bsCryptoKey.end(); sit++ )
    {
        *sit = static_cast<byte>( die() );
    }
}

void KSecurityAssociation::SetAuthKey( const KByteStream& bsKey )
{
    m_bsAuthKey = bsKey;
}

void KSecurityAssociation::SetCryptoKey( const KByteStream& bsKey )
{
    m_bsCryptoKey = bsKey;
}

const KByteStream& KSecurityAssociation::GetAuthKey() const
{
    return m_bsAuthKey;
}

const KByteStream& KSecurityAssociation::GetCipherKey() const
{
    return m_bsCryptoKey;
}

SeqType KSecurityAssociation::GetSequenceNum() const
{
    return m_nSequenceNum;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 15  ������	Sequence Number ( not have replay window mask )

// �Ʒ� �Լ� �ΰ��� replay window mask�� ���� �ʴ� �Լ��Դϴ�. TCP��Ŀ����� ����ϼ���.

void KSecurityAssociation::IncrSequenceNum_notRWM()
{
	++m_nSequenceNum;

	// SequenceNum�� �ݵ�� 1���� �����ؾ���.
	if( m_nSequenceNum == 0 )
		m_nSequenceNum = 1;
}

bool KSecurityAssociation::IsValidSequenceNum_notRWM( SeqType nSequenceNum_ )
{
	// sequence number�� ������ 1���� �����մϴ�.
	if( nSequenceNum_ == 0 )
		return false;

	// ������ �޾Ҵ� ��Ŷ�� sequence number���� ������Ŷ�� sequence number�� ũ�ٸ� �����Դϴ�.
	if( nSequenceNum_ > m_nLastSequenceNum )
	{
		m_nLastSequenceNum = nSequenceNum_;
		return true;
	}

	// ������ �޾Ҵ� ��Ŷ�� sequence number�� ������Ŷ�� sequence number�� ���� ���� �� �����ϴ�.
	if( nSequenceNum_ == m_nLastSequenceNum )
		return false;

	// ������ �޾Ҵ� ��Ŷ�� sequence number���� �������� sequence number��� last�� MAX_SEQUENCE_NUM������ üũ�մϴ�.
	if( m_nLastSequenceNum != MAX_SEQUENCE_NUM )
		return false;

	m_nLastSequenceNum = nSequenceNum_;
	return true;
}

//}}
//////////////////////////////////////////////////////////////////////////

void KSecurityAssociation::IncrSequenceNum()
{
    ++m_nSequenceNum;
}

bool KSecurityAssociation::IsValidSequenceNum( SeqType nSequenceNum_ ) const
{
    // The following algorithm is based on the Sequence Space Window
    // Code Example presented in RFC 2401.
    //
    // The "right" edge of the window represents the highest validated
    // Sequence Number received. Packets that contain Sequence Numbers lower
    // than the "left" edge of the window are rejected. Packets falling
    // within the window are checked against a list of received packets
    // within the window. Duplicates are rejected. If the received packet 
    // falls within the window or is new, or if the packet is to the right 
    // of the window, the Sequence Number is valid and the packet moves on
    // to the next verification stage.

    // Check for sequence number wrap
    if( nSequenceNum_ == 0 )
        return false;

    // Nominal case: the new number is larger than the last packet
    if( nSequenceNum_ > m_nLastSequenceNum )
        return true;

    const unsigned long nReplayWindowSize = sizeof( m_nReplayWindowMask ) * CHAR_BIT;
    SeqType nDiff = m_nLastSequenceNum - nSequenceNum_;

    // Packet is too old or wrapped
    if( nDiff >= nReplayWindowSize )
        return false;

    // Packet is a duplicate
    ReplayWinType nBit = 1;
    if( m_nReplayWindowMask & ( nBit << nDiff ) )
        return false;

    // Out of order, but within window
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Update the replay window based on the given (validated) sequence number

void KSecurityAssociation::UpdateReplayWindow( SeqType nSequenceNum_ )
{
    // The following algorithm is based on the Sequence Space Window
    // Code Example presented in RFC 2401.
    //
    // The "right" edge of the window represents the highest validated
    // Sequence Number received. Packets that contain Sequence Numbers lower
    // than the "left" edge of the window are rejected. Packets falling
    // within the window are checked against a list of received packets
    // within the window. Duplicates are rejected. If the received packet 
    // falls within the window or is new, or if the packet is to the right 
    // of the window, the Sequence Number is valid and the packet moves on
    // to the next verification stage.

    JIF( IsValidSequenceNum( nSequenceNum_ ) );

    const unsigned long nReplayWindowSize = sizeof( m_nReplayWindowMask ) * CHAR_BIT;

    // Nominal case: the new number is larger than the last packet
    if( nSequenceNum_ > m_nLastSequenceNum )
    {
        SeqType nDiff = nSequenceNum_ - m_nLastSequenceNum;

        // If the packet is within the window, slide the window
        if( nDiff < nReplayWindowSize )
        {
            m_nReplayWindowMask <<= nDiff;
            m_nReplayWindowMask |= 1;
        }
        else
        {
            // packet way outside the window; reset the window
            m_nReplayWindowMask = 1;
        }

        // Update the "last" sequence number
        m_nLastSequenceNum = nSequenceNum_;
    }
    else
    {
        // New number is smaller than the last packet
        SeqType nDiff = m_nLastSequenceNum - nSequenceNum_;

        // Mark the packet as seen
        ReplayWinType nBit = 1;
        m_nReplayWindowMask |= ( nBit << nDiff );
    }
}

KByteStream KSecurityAssociation::Encryption( const KByteStream& payload, const KByteStream& IV ) const
{
    CryptoPP::CBC_Mode<CryptoPP::DES>::Encryption encryptor( &m_bsCryptoKey[0], m_bsCryptoKey.length(), &IV[0] );

    KByteStream crypt;
    crypt.resize( payload.length() );

    encryptor.ProcessData( &crypt[0], payload.c_str(), payload.length() );

    return crypt;
}

KByteStream KSecurityAssociation::Decryption( const KByteStream& crypt, const KByteStream& IV ) const
{
    CryptoPP::CBC_Mode<CryptoPP::DES>::Decryption decryptor( &m_bsCryptoKey[0], m_bsCryptoKey.length(), &IV[0] );

    KByteStream payload;
    payload.resize( crypt.length() );

    decryptor.ProcessData( &payload[0], crypt.c_str(), crypt.length() );

    return payload;
}

KByteStream KSecurityAssociation::GenerateICV( const KByteStream& auth ) const
{
    CryptoPP::HMAC<CryptoPP::MD5> mac( &m_bsAuthKey[0], m_bsAuthKey.length() );

    KByteStream icv;
    icv.resize( GetICVSize() );

    mac.Update( auth.data(), auth.length() );
    mac.TruncatedFinal( &icv[0], GetICVSize() );

    return icv;
}

NAMESPACE_END