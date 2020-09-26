#pragma once
#include <windows.h>
#include <fstream>
#include <memory>
#include <io.h>
#include <sys/stat.h>

#define KEY_SIZE	sizeof(T)

namespace FileCrypt
{
	// n���� crypt key�� n����Ʈ�� ����� ��ȣȭ
	template< typename T >
	bool FileEncrypt( IN std::vector<T>& vecEncryptionKey, IN std::string& strFileName );

	template< typename T >
	char* BufferEncrypt( IN std::vector<T>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize );

	// ��ȣȭ
	template< typename T >
	char* FileDecrypt( IN std::vector<T>& vecDecryptionKey, IN const char* pEncryptedBuffer, IN int iBufferSize );
}

template< typename T >
bool FileCrypt::FileEncrypt( IN std::vector<T>& vecEncryptionKey, IN std::string& strFileName )
{
	char pBuf[KEY_SIZE + 1];
	std::string strEncryptedFileName;
	strEncryptedFileName = strFileName;
	strEncryptedFileName += ".encrypt.lua";

	std::ofstream ofs;
	std::ifstream ifs;
	std::string strCryptBuffer;
	ifs.open( strFileName.c_str(), std::ios::binary );
	ofs.open( strEncryptedFileName.c_str(), std::ios::binary );

	if( ifs.is_open() == false || ofs.is_open() == false )	
		return false;

	// ���� ũ�� ���ϱ�
	struct stat fileSize;
	stat( strFileName.c_str(), &fileSize );

	int iKeyIndex = 0;
	int iKeySize = vecEncryptionKey.size();	
	int iRemainSize = fileSize.st_size;

	while( !ifs.eof() )
	{
		memset( pBuf, 0, KEY_SIZE + 1 );

		// ���Ϲ��� �б�
		ifs.read( pBuf, KEY_SIZE );

		T tBufTemp;
		memcpy( &tBufTemp, pBuf, KEY_SIZE );

		// ��ȣȭ
		T tEncryptData = tBufTemp ^ vecEncryptionKey[iKeyIndex];

		memcpy( pBuf, &tEncryptData, KEY_SIZE );

		if( iRemainSize < KEY_SIZE )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, KEY_SIZE );
		}

		// ���� ���� ũ�� ����
		iRemainSize -= KEY_SIZE;

		// n����Ʈ�� ������ ���鼭 ��ȣȭŰ ����
		++iKeyIndex;

		if( iKeyIndex >= iKeySize )
			iKeyIndex = 0;
	}

	ofs.write( strCryptBuffer.c_str(), strCryptBuffer.size() );

	ifs.close();
	ofs.close();

	_chmod( strFileName.c_str(), _S_IREAD | _S_IWRITE ); // ���� ������ ���ؼ�

	if( CopyFileA( strEncryptedFileName.c_str(), strFileName.c_str(), false ) == 0 )
		return false;

	if( DeleteFileA( strEncryptedFileName.c_str() ) == 0 )
		return false;

	return true;
}

template< typename T >
char* FileCrypt::BufferEncrypt( IN std::vector<T>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize )
{
	return FileCrypt::FileDecrypt( vecEncryptionKey, pDecryptedBuffer, iBufferSize );
}

// ��ȣȭ
template< typename T >
char* FileCrypt::FileDecrypt( IN std::vector<T>& vecDecryptionKey, IN const char* pEncryptedBuffer, IN int iBufferSize )
{
	char pBuf[KEY_SIZE + 1];

	std::string strCryptBuffer;

	if( pEncryptedBuffer == NULL )
		return NULL;

	int iKeyIndex = 0;
	int iKeySize = vecDecryptionKey.size();
	int iRemainSize = iBufferSize;
	int iCryptedSize = 0;

	while(1)
	{
		memset( pBuf, 0, KEY_SIZE + 1 );

		T tBufTemp;
		memcpy( &tBufTemp, pEncryptedBuffer + iCryptedSize, KEY_SIZE );

		// ��ȣȭ
		T tEncryptData = tBufTemp ^ vecDecryptionKey[iKeyIndex];

		memcpy( pBuf, &tEncryptData, KEY_SIZE );

		if( iRemainSize < KEY_SIZE )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, KEY_SIZE );
		}

		// ���� ���� ���� ����
		iRemainSize -= KEY_SIZE;

		// ��ȣȭ�� ���� ����
		iCryptedSize += KEY_SIZE;

		// 4����Ʈ�� ������ ���鼭 ��ȣȭŰ ����
		++iKeyIndex;

		if( iKeyIndex >= iKeySize )
			iKeyIndex = 0;
	}

	// ���� ���� ���� �� ��ȣȭ�� ������ ���
	char* pDecryptedBuffer = new char[strCryptBuffer.size()];
	memset( pDecryptedBuffer, 0, strCryptBuffer.size() );
	memcpy( pDecryptedBuffer, strCryptBuffer.c_str(), strCryptBuffer.size() );	

	return pDecryptedBuffer;
}