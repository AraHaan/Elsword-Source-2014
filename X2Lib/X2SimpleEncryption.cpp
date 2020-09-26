/*------------------------------------
 *
 * X2SimpleEncryption.cpp
 * 
 * 2013. 11. Im Cheolsu.
 *
 * Desc : �ܺο��� �����Ͱ��� �˻��� �� ���� �����͸� ����(��ȣȭ)
 *------------------------------------
 */

#include "StdAfx.h"
#include "X2SimpleEncryption.h"

using namespace std;

#define ENCRYPTION_KEY  const char   addValue1		= 7;  \
						const char   addValue2		= 15; \
						const char   addValue3		= 21; \
						const size_t changeIndex1	= 5; \
						const size_t changeIndex2	= 10;


CX2SimpleEncryption::CX2SimpleEncryption( )
{

}

CX2SimpleEncryption::~CX2SimpleEncryption()
{	

}

vector<char> CX2SimpleEncryption::EncrypteData( const char * data, size_t length )
{
	vector<char> ret;
	ret.reserve(length);

	// ��ȣȭ Ű ���� ���������� ó���Ѵ�.
	// ����� �� ��� ������ ������ �ִ�.
	ENCRYPTION_KEY
	///////////////////////////////////////
	for( size_t i = 0 ; i < length ; i++ )
	{
		if( i == changeIndex1 )
			ret.push_back( data[i] + addValue1 );
		else if( i == changeIndex2 )
			ret.push_back( data[i] + addValue2 );
		else
			ret.push_back( data[i] + addValue3 );
	}

#ifdef _DEBUG
	vector<char> testDencrypte = DencrypteData(&ret[0], length);
	assert(_IsEqualData(data, &testDencrypte[0], length));
#endif


	return ret;
}

vector<char> CX2SimpleEncryption::DencrypteData( const char * data, size_t length )
{
	vector<char> ret;
	ret.reserve(length);

	// ��ȣȭ Ű ���� ���������� ó���Ѵ�.
	// ����� �� ��� ������ ������ �ִ�.
	ENCRYPTION_KEY
	/////////////////////////////////////

	for( size_t i = 0 ; i < length ; i++ )
	{
		if( i == changeIndex1 )
			ret.push_back( data[i] - addValue1 );
		else if( i == changeIndex2 )
			ret.push_back( data[i] - addValue2 );
		else
			ret.push_back( data[i] - addValue3 );
	}

	return ret;
}

string CX2SimpleEncryption::GetString( const vector<char> & data )
{	
	string retData;
	retData.assign(data.begin(), data.end());

	return retData;
}

bool CX2SimpleEncryption::_IsEqualData( const char * data1, const char * data2, size_t length )
{
	for( size_t i = 0 ; i < length  ; i++ )
	{
		if( data1[i] != data2[i] )
			return false;
	}

	return true;
}