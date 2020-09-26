#include "DBConnectionManager.h"
#include "BaseServer.h"

#include "cryptopp/des.h"
#include "cryptopp/modes.h"
#include "cryptopp/hmac.h"
#include "cryptopp/md5.h"
#include "KncSecurity/ByteStream.h"

#undef max
#include <boost/random.hpp>

// c++���Ͽ��� ����!
#define MASK_FIRST( key )		( key & 0x000000ff )
#define MASK_SECOND( key )		( ( key >> 8 ) & 0x000000ff )
#define MASK_THIRD( key )		( ( key >> 16 ) & 0x000000ff )
#define MASK_CRYPTKEY( key )	( ( key >> 24 ) & 0x000000ff )

#define CRYPTO_KEY_SIZE 8
#define CRYPTO_IV_SIZE	8
#define BYTE_TO_STRING_SIZE 3

//{{ 2012. 1. 9	������		DB���� ��ȣ ����ȭ
#ifdef SERV_DB_CONNECTION_SECURITY	// ���� ������ �ؿ��� �߰�

// �ܺο��� ���������� ���ϵ��� anonymous namespace �ȿ� ����. <---- �̰� ���߿� ���� �ڵ忡�� ����! ( lua��ũ��Ʈ�� ���? )
namespace
{
	class KCursor
	{
	public:
		KCursor()
		{
			m_uiPos = 0;
		}

		unsigned int Move( const unsigned int uiMoveCount )
		{
			m_uiPos += uiMoveCount;
			return GetCurrentPos();
		}

		int GetCurrentPos() const { return m_uiPos; }

	private:
		unsigned int m_uiPos;
	};

	std::string ConvertToString( const unsigned char ucData )
	{
		const std::string strTemp = boost::str( boost::format( "%03d" ) % static_cast<int>(ucData) );
		return strTemp;
	}

	unsigned char ConvertToChar( const std::string& strData )
	{
		unsigned char ucTemp = static_cast<unsigned char>( atoi( strData.c_str() ) );
		return ucTemp;
	}

	//////////////////////////////////////////////////////////////////////////
	// �������� ��ȣȭ
	void EncryptString( const std::string& strSource, std::string& strResult )
	{
		const char cFirst  = rand() % CHAR_MAX;
		const char cSecond = rand() % CHAR_MAX;
		const char cThird  = rand() % CHAR_MAX;
		const char cKeyCrypt = rand() % CHAR_MAX;

		const char cFirstRaelKey  = cFirst  ^ cKeyCrypt;
		const char cSecondRealKey = cSecond ^ cKeyCrypt;
		const char cThirdRealKey  = cThird  ^ cKeyCrypt;

		// ���� ù��° ����Ʈ�� ��ȣȭ�Ȱ� ����!
		strResult += ConvertToString( cFirst );
		strResult += ConvertToString( cSecond );
		strResult += ConvertToString( cThird );
		strResult += ConvertToString( cKeyCrypt );

		for( unsigned int i = 0; i < strSource.size(); ++i )
		{
			// ��ȣȭ
			char cEncryptData = 0;
			cEncryptData = cFirstRaelKey  ^ strSource[i];
			cEncryptData = cSecondRealKey ^ cEncryptData;
			cEncryptData = cThirdRealKey  ^ cEncryptData;

			strResult += ConvertToString( cEncryptData );
		}
	}

	bool DecryptString( const std::string& strSource, std::string& strResult )
	{
		KCursor kCursor;
		std::string strTemp;

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cFirst  = ConvertToChar( strTemp );

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cSecond = ConvertToChar( strTemp );

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cThird  = ConvertToChar( strTemp );

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cKeyCrypt = ConvertToChar( strTemp );

		const char cFirstRaelKey  = cFirst  ^ cKeyCrypt;
		const char cSecondRealKey = cSecond ^ cKeyCrypt;
		const char cThirdRealKey  = cThird  ^ cKeyCrypt;

		for( unsigned int i = kCursor.GetCurrentPos(); i < strSource.size(); i += BYTE_TO_STRING_SIZE )
		{
			char cData = ConvertToChar( strSource.substr( i, BYTE_TO_STRING_SIZE ) );

			// ��ȣȭ
			char cEncryptData = 0;
			cEncryptData = cFirstRaelKey  ^ cData;
			cEncryptData = cSecondRealKey ^ cEncryptData;
			cEncryptData = cThirdRealKey  ^ cEncryptData;

			strResult.push_back( cEncryptData );
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// des�˰��� ��ȣȭ
	KncSecurity::KByteStream MakeCryptKey()
	{
		//static boost::mt19937 rng;
		//static boost::uniform_int<DWORD> uint32( 1, std::numeric_limits<byte>::max() );   // 1 �̻�. SPI 0�� �����ڿ��� �̹� �����ߴ�.
		//static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

		KncSecurity::KByteStream bsKey;
		bsKey.clear();
		bsKey.resize( CRYPTO_KEY_SIZE );

		KncSecurity::KByteStream::iterator sit;
		for( sit = bsKey.begin(); sit != bsKey.end(); ++sit )
		{
			const byte randomValue = rand() % CHAR_MAX;//static_cast<byte>( die() );
			*sit = randomValue;
		}

		return bsKey;
	}

	KncSecurity::KByteStream GenerateIV()
	{
		KncSecurity::KByteStream bsIV;
		bsIV.clear();
		bsIV.resize( CRYPTO_IV_SIZE, 'A' + rand() % 40 );
		return bsIV;

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

	void EncryptStirngUseDes( const std::string& strSource, std::string& strResult )
	{
		// ��ȣȭŰ�� �����ϰ� ����!
		KncSecurity::KByteStream bsKey = MakeCryptKey();
		KncSecurity::KByteStream bsIV = GenerateIV();

		// ��ȣȭ�� ���ڿ��� ���ۿ� ����!
		KncSecurity::KByteStream payLoad;
		for( unsigned int i = 0; i < strSource.size(); ++i )
		{
			payLoad += strSource[i];
		}

		// ������ �ڸ��� �е����� ä����!
		while( ( payLoad.length() % CRYPTO_KEY_SIZE ) != 0 )
		{
			payLoad += '@';
		}

		// DES�˰��� ��ȣȭ!
		CryptoPP::CBC_Mode<CryptoPP::DES>::Encryption encryptor( &bsKey[0], bsKey.length(), &bsIV[0] );
		KncSecurity::KByteStream crypt;
		crypt.resize( payLoad.length() );
		encryptor.ProcessData( &crypt[0], payLoad.c_str(), payLoad.length() );

		// cryptŰ�� �� �տ� ������!
		for( unsigned int ui = 0; ui < bsKey.length(); ++ui )
		{
			strResult += ConvertToString( bsKey[ui] );
		}

		// IV���� �� ������ ������!
		for( unsigned int ui = 0; ui < bsIV.length(); ++ui )
		{
			strResult += ConvertToString( bsIV[ui] ^ bsKey[ui] ); // IV���� �ʹ� ������ �����̹Ƿ� Key�� xor������ �ѹ� �Ѵ�.
		}

		// �� �ڿ� �ٷ� ��ȣȭ�� payload�� ������!
		KncSecurity::KByteStream::iterator mitBS;
		for( mitBS = crypt.begin(); mitBS != crypt.end(); ++mitBS )
		{
			strResult += ConvertToString( *mitBS );
		}
	}

	bool DecryptStringUseDes( const std::string& strSource, std::string& strResult )
	{
		if( strSource.length() < CRYPTO_KEY_SIZE * BYTE_TO_STRING_SIZE )
			return false;

		KncSecurity::KByteStream bsKey;
		KncSecurity::KByteStream bsIV;

		// ��ȣȭ�� ���ڿ����� ��ȣȭŰ���� �̾Ƴ���!
		KCursor kCursor;
		for( unsigned int i = 0; i < CRYPTO_KEY_SIZE; ++i )
		{
			std::string strTemp;
			strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
			kCursor.Move( BYTE_TO_STRING_SIZE );
			const char cTemp = ConvertToChar( strTemp );
			bsKey += cTemp;
		}

		// ��ȣȭ�� ���ڿ����� IV���� �̾Ƴ���!
		for( unsigned int i = 0; i < CRYPTO_IV_SIZE; ++i )
		{
			std::string strTemp;
			strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
			kCursor.Move( BYTE_TO_STRING_SIZE );
			const char cTemp = ConvertToChar( strTemp );
			bsIV += cTemp ^ bsKey[i]; // Key���� xor�� �ѹ� �ؾ� ��¥ IV���� ���´�.
		}

		// ��ȣȭ�� ���� ���ڿ��� ���� �̾Ƴ���!
		KncSecurity::KByteStream crypt;
		for( unsigned int i = kCursor.GetCurrentPos(); i < strSource.size(); i += BYTE_TO_STRING_SIZE )
		{
			crypt += ConvertToChar( strSource.substr( i, BYTE_TO_STRING_SIZE ) );
		}

		// ��ȣȭ �۾�!
		CryptoPP::CBC_Mode<CryptoPP::DES>::Decryption decryptor( &bsKey[0], bsKey.length(), &bsIV[0] );
		KncSecurity::KByteStream payload;
		payload.resize( crypt.length() );
		decryptor.ProcessData( &payload[0], crypt.c_str(), crypt.length() );

		// ��ȣȭ�� �Ϸ�� ���ڿ��� ���� ��������!
		KncSecurity::KByteStream::iterator mitBS;
		for( mitBS = payload.begin(); mitBS != payload.end(); ++mitBS )
		{
			std::string strData;
			strData += static_cast<char>(*mitBS);
			if( strData == "@" )
				continue;

			strResult += strData;
		}

		return true;
	}
}


KDBConnectionManager::KDBConnectionManager()
{
}


KDBConnectionManager::~KDBConnectionManager(void)
{
}

bool KDBConnectionManager::GetDBConnectionInfoFromManagerDB( IN const int iServerType, 
															 IN const wchar_t* szConnStr, 
															 IN const bool bDBConnStr, 
															 OUT std::vector< KDBConnectionInfo >& vecDBConnectionInfo )
{
	vecDBConnectionInfo.clear();

	if( m_kODBC.DriverConnect( szConnStr, bDBConnStr ) == false )
	{
		START_LOG( cerr, L"DB���� ����!" )
			<< BUILD_LOG( bDBConnStr )
			<< END_LOG;
		return false;
	}
	
	DO_QUERY_NO_PROFILE( L"exec dbo.temp_get_info", L"%d", % iServerType );

	while( m_kODBC.Fetch() )
	{
		std::wstring wstrDBString;
		int iServerGroupID = 0;
		KDBConnectionInfo kInfo;
		FETCH_DATA( iServerGroupID
			>> kInfo.m_iDBType
			>> wstrDBString
			>> kInfo.m_iThreadCount
			);

		// ������ ���� -1�̸� ������ ����ó��
		if( iServerGroupID != -1 )
		{
			if( iServerGroupID != KBaseServer::GetKObj()->GetServerGroupID() )
				continue;
		}

		// ��ȣȭ
		if( DecryptDBString( wstrDBString, kInfo.m_wstrConnectionString ) == false )
		{
			START_LOG( cerr, L"���ڿ� �ؼ� ����!" )
				<< BUILD_LOG( wstrDBString )
				<< END_LOG;
			continue;
		}

		vecDBConnectionInfo.push_back( kInfo );
	}

	if( vecDBConnectionInfo.empty() )
	{
		START_LOG( cerr, L"�Ŵ��� DB�κ��� ������ �������� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( iServerType )
			<< END_LOG;
		return false;
	}
	
	return true;

end_proc:
	START_LOG( cerr, L"���� ȣ�� ����!" )
		<< END_LOG;
	return false;
}

bool KDBConnectionManager::DecryptDBString( IN const std::wstring& wstrDBString, OUT std::wstring& wstrDecryptString )
{
	const std::string strStringBuffer = KncUtil::toNarrowString( wstrDBString );
	std::string strDecryptString;
	if( DecryptStringUseDes( strStringBuffer, strDecryptString ) == false )
	{
		START_LOG( cerr, L"��ȣȭ ����!" )
			<< BUILD_LOG( wstrDBString )
			<< END_LOG;
		return false;
	}
	
	wstrDecryptString = KncUtil::toWideString( strDecryptString );
	return true;
}


#endif SERV_DB_CONNECTION_SECURITY	// ���� ������ �ؿ��� �߰�
//}}