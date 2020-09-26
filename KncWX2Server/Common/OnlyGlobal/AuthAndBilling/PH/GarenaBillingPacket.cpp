#include "GarenaBillingPacket.h"

#ifdef SERV_COUNTRY_PH

#include "json.h"

#ifdef _DEBUG
#pragma comment( lib, "json_vc71_libmtd.lib" )
#else	
#pragma comment( lib, "json_vc71_libmt.lib" )
#endif


KGarenaBillingPacket::KGarenaBillingPacket()
{
	m_ulPacketLength = 0;
	m_iPacketType = 0;
}

KGarenaBillingPacket::~KGarenaBillingPacket()
{
}

bool KGarenaBillingPacket::ReadFromBuffer( IN const char* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	// ��Ŷ ũ��
	::memcpy( &m_ulPacketLength, pbyteBuffer, sizeof( unsigned long ) );
	const unsigned long ulTotalPacketLength = m_ulPacketLength + HEADER_SIZE_NGMT; // ���ũ�⸦ ������!
	//m_ulPacketLength = ::ntohl( m_ulPacketLength ); -- �ؽ��� ��Ŷ�� little endian���� �ٷ� ������ �־ �ϴ� �ּ�ó��!
	_JIF( ulTotalPacketLength >= HEADER_SIZE_NGMT, return false );
	_JIF( ulTotalPacketLength - HEADER_SIZE_NGMT <= MAX_PACKET_CONTENT_SIZE_NGMT, return false );

	if( ulTotalPacketLength > HEADER_SIZE_NGMT )
	{
		//::memcpy( m_abytePacketContent, pBuffer + 4, m_ulPacketLength - HEADER_SIZE_NGMT );

		// json�Ľ��� ���� ���۷� �ű���!
		m_strPacketContent.assign( pbyteBuffer + 4, ulTotalPacketLength - HEADER_SIZE_NGMT );
	}

	Json::Value root;
	Json::Reader reader;

	const bool bIsParsed = reader.parse( m_strPacketContent, root );
	if( !bIsParsed )
	{
		START_LOG( cerr, L"failed to parse" )
			<< BUILD_LOG( reader.getFormatedErrorMessages() )
			<< BUILD_LOG( m_ulPacketLength )
			<< BUILD_LOG( ulTotalPacketLength )
			<< BUILD_LOG( m_strPacketContent.size() )
			<< BUILD_LOG( m_strPacketContent )
			<< END_LOG;
		return false;
	}

	// ��Ŷ Ÿ�� ���
	m_iPacketType = static_cast<int>(root.get( "ID", 0 ).asUInt());

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"��Ŷ �Ľ� ����! ��Ŷ id���� ����!" )
		<< BUILD_LOGc( m_iPacketType )
		<< BUILD_LOG( m_ulPacketLength )
		<< BUILD_LOG( ulTotalPacketLength )
		<< BUILD_LOG( m_strPacketContent.size() )
		<< BUILD_LOG( m_strPacketContent );
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool KGarenaBillingPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	// ��Ŷ ũ��
	_JIF( m_ulPacketLength <= MAX_PACKET_CONTENT_SIZE_NGMT, return false );
	unsigned long ulPacketLength = m_ulPacketLength; //::htonl( m_ulPacketLength ); -- �ؽ��� ��Ŷ�� little endian���� �ٷ� ������ �־ �ϴ� �ּ�ó��!
	::memcpy( pbyteBuffer, &ulPacketLength, sizeof( unsigned long ) );
	if( m_ulPacketLength > HEADER_SIZE_NGMT )
	{
		::memcpy( pbyteBuffer + 4, &m_strPacketContent[0], m_ulPacketLength );
	}

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"��Ŷ ���� ����! ��Ŷ ������ ����!" )
		<< BUILD_LOGc( m_iPacketType )
		<< BUILD_LOG( m_ulPacketLength )
		<< BUILD_LOG( m_strPacketContent.size() )
		<< BUILD_LOG( m_strPacketContent );
	//////////////////////////////////////////////////////////////////////////
	return true;
}


bool KGarenaBillingPacket::Read( KEJSON_GN_CHECK_ACCOUNT_REQ& kPacket )
{
	Json::Value root;
	Json::Reader reader;

	// ����Ǿ��ִ� ���ڿ��� JSON�������� �о������!
	if( reader.parse( m_strPacketContent, root ) == false )
	{
		std::cout << "failed to parse" << reader.getFormatedErrorMessages() << std::endl;
		return false;
	}

	kPacket.m_uiGarenaUID		= root["data"].get( "garena_uid", "-1" ).asUInt();
	
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"���� Ȯ�� ��Ŷ �Ľ�!" )
		<< BUILD_LOG( kPacket.m_uiGarenaUID )
		;
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool KGarenaBillingPacket::Write( const KEJSON_GN_CHECK_ACCOUNT_ACK& kPacket )
{
	Json::Value root;

	root["ID"] = ENUM_PACKET_TYPE::CHECK_ACCOUNT;

	Json::Value data;	


	// resultCode
	data["result_code"] = kPacket.m_iResult;

	// resultMessage
	data["garena_uid"] =  kPacket.m_uiGarenaUID ;
	
	data["currency_balance"] = kPacket.m_uiCurrency_Balance;


	// resultvalue
	Json::Value characterList;
	Json::Value character;
	Json::Value subcharacterList;


	character.append(KncUtil::toNarrowString( kPacket.m_wstrCharName ));

	subcharacterList["server_id"] = 0;
	subcharacterList["characters"] = character;

	characterList.append(subcharacterList);

	data["character_list"] = characterList;

	root["data"] = data;


	Json::StyledWriter writer;
	//std::string strResult = writer.write( root ); -- UTF8������ ���� ��� �ּ�ó��
	m_strPacketContent = writer.write( root );

	//////////////////////////////////////////////////////////////////////////	
	// UTF8�� ��ȯ
	//std::wstring wstrTemp = KncUtil::toWideString_UTF8( strResult );
	//m_strPacketContent.clear();
	//m_strPacketContent.assign( (const char*)&wstrTemp[0], wstrTemp.size() * sizeof(wchar_t) );
	//////////////////////////////////////////////////////////////////////////	

	// ��Ŷ ����
	m_ulPacketLength = /*HEADER_SIZE_NGMT + */m_strPacketContent.size();

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"��Ŷ ���� ����! ��Ŷ ������ ����!" )
		<< BUILD_LOGc( m_iPacketType )
		<< BUILD_LOG( m_ulPacketLength )
		//<< BUILD_LOG( strResult.size() )
		//<< BUILD_LOG( strResult )
		//<< BUILD_LOG( wstrTemp.size() )
		//<< BUILD_LOG( wstrTemp )
		<< BUILD_LOG( m_strPacketContent.size() )
	<< BUILD_LOG( m_strPacketContent );
	//////////////////////////////////////////////////////////////////////////
	return true;
}


bool KGarenaBillingPacket::Read( KEJSON_GN_CHANGE_GAME_CURRENCY_REQ& kPacket )
{

	Json::Value root;
	Json::Reader reader;

	// ����Ǿ��ִ� ���ڿ��� JSON�������� �о������!
	if( reader.parse( m_strPacketContent, root ) == false )
	{
		std::cout << "failed to parse" << reader.getFormatedErrorMessages() << std::endl;
		return false;
	}

	kPacket.m_uiGameCurrency_Amount			= root["data"].get( "currency_amount", "0" ).asUInt();
	kPacket.m_uiShell_Amount				= root["data"].get( "shell_amount", "0" ).asUInt();
	kPacket.m_uiGarenaUID					= root["data"].get( "garena_uid", "-1" ).asUInt();
	kPacket.m_strGarenaTransactionID		= root["data"].get( "garena_txnid", "" ).asString();


	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"ĳ�� ��ȯ �Ľ�!" )
		<< BUILD_LOG( kPacket.m_uiGameCurrency_Amount )
		<< BUILD_LOG( kPacket.m_uiShell_Amount )
		<< BUILD_LOG( kPacket.m_uiGarenaUID )
		<< BUILD_LOG( kPacket.m_strGarenaTransactionID )
		;
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool KGarenaBillingPacket::Write( const KEJSON_GN_CHANGE_GAME_CURRENCY_ACK& kPacket )
{
	Json::Value root;

	root["ID"] = ENUM_PACKET_TYPE::CHANGE_GAME_CURRENCY;

	Json::Value data;

	// resultCode
	data["result_code"] = kPacket.m_iResult;

	// resultMessage
	data["garena_uid"] = kPacket.m_uiGarenaUID;

	data["garena_txnid"] = kPacket.m_strGarenaTransactionID;
	data["game_txnid"] = static_cast<double>(kPacket.m_int64GameTransactionID);

	data["currency_amount"] = kPacket.m_uiGameCurrency_Amount;
	data["currency_balance"] = kPacket.m_uiGameCurrency_Balance;

	root["data"] = data;


	Json::StyledWriter writer;
	//std::string strResult = writer.write( root ); -- UTF8������ ���� ��� �ּ�ó��
	m_strPacketContent = writer.write( root );

	//////////////////////////////////////////////////////////////////////////	
	// UTF8�� ��ȯ
	//std::wstring wstrTemp = KncUtil::toWideString_UTF8( strResult );
	//m_strPacketContent.clear();
	//m_strPacketContent.assign( (const char*)&wstrTemp[0], wstrTemp.size() * sizeof(wchar_t) );
	//////////////////////////////////////////////////////////////////////////	

	// ��Ŷ ����
	m_ulPacketLength = /*HEADER_SIZE_NGMT + */m_strPacketContent.size();

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"��Ŷ ���� ����! ��Ŷ ������ ����!" )
		<< BUILD_LOGc( m_iPacketType )
		<< BUILD_LOG( m_ulPacketLength )
		//<< BUILD_LOG( strResult.size() )
		//<< BUILD_LOG( strResult )
		//<< BUILD_LOG( wstrTemp.size() )
		//<< BUILD_LOG( wstrTemp )
		<< BUILD_LOG( m_strPacketContent.size() )
		<< BUILD_LOG( m_strPacketContent );
	//////////////////////////////////////////////////////////////////////////
	
	return true;
}

bool KGarenaBillingPacket::Read( KEJSON_GN_TRANSACTION_REQ& kPacket )
{

	Json::Value root;
	Json::Reader reader;

	// ����Ǿ��ִ� ���ڿ��� JSON�������� �о������!
	if( reader.parse( m_strPacketContent, root ) == false )
	{
		std::cout << "failed to parse" << reader.getFormatedErrorMessages() << std::endl;
		return false;
	}

	kPacket.m_uiGarenaUID					= root["data"].get( "garena_uid", "-1" ).asUInt();
	kPacket.m_strGarenaTransactionID		= root["data"].get( "garena_txnid", "" ).asString();

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"��ȯ ���� �α� ����!" )
		<< BUILD_LOG( kPacket.m_uiGarenaUID )
		<< BUILD_LOG( kPacket.m_strGarenaTransactionID )
		;
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool KGarenaBillingPacket::Write( const KEJSON_GN_TRANSACTION_ACK& kPacket )
{
	Json::Value root;

	root["ID"] = ENUM_PACKET_TYPE::TRANSACTION;

	Json::Value data;

	// resultCode
	data["result_code"] = kPacket.m_iResult;

	// resultMessage
	data["garena_uid"] = kPacket.m_uiGarenaUID ;

	data["garena_txnid"] = kPacket.m_strGarenaTransactionID;
	data["game_txnid"] = static_cast<double>(kPacket.m_int64GameTransactionID);
	data["currency_amount"] = kPacket.m_uiGameCurrency_Amount;
	data["shel_amount"] = kPacket.m_uiShell_Amount;
	data["datetime"] = kPacket.m_strDateTime;

	root["data"] = data;


	Json::StyledWriter writer;
	//std::string strResult = writer.write( root ); -- UTF8������ ���� ��� �ּ�ó��
	m_strPacketContent = writer.write( root );

	//////////////////////////////////////////////////////////////////////////	
	// UTF8�� ��ȯ
	//std::wstring wstrTemp = KncUtil::toWideString_UTF8( strResult );
	//m_strPacketContent.clear();
	//m_strPacketContent.assign( (const char*)&wstrTemp[0], wstrTemp.size() * sizeof(wchar_t) );
	//////////////////////////////////////////////////////////////////////////	

	// ��Ŷ ����
	m_ulPacketLength = /*HEADER_SIZE_NGMT + */m_strPacketContent.size();

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cwarn, L"��Ŷ ���� ����! ��Ŷ ������ ����!" )
		<< BUILD_LOGc( m_iPacketType )
		<< BUILD_LOG( m_ulPacketLength )
		//<< BUILD_LOG( strResult.size() )
		//<< BUILD_LOG( strResult )
		//<< BUILD_LOG( wstrTemp.size() )
		//<< BUILD_LOG( wstrTemp )
		<< BUILD_LOG( m_strPacketContent.size() )
		<< BUILD_LOG( m_strPacketContent );
	//////////////////////////////////////////////////////////////////////////

	return true;
}

#endif SERV_COUNTRY_PH