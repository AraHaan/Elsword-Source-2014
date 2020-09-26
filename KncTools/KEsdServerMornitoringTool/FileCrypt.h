#pragma once
#define IN
#define OUT
#include <vector>

class CFileCrypt
{
public:
	CFileCrypt(void);
	~CFileCrypt(void);

	// n���� crypt key�� Ư�� ����Ʈ�� ����� 4����Ʈ ������ ��ȣȭ
	static bool FileEncrypt( IN std::vector<int>& vecEncryptionKey, IN std::string& strFileName );
	static char* BufferEncrypt( IN std::vector<int>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize );

	// ��ȣȭ
	static char* FileDecrypt( IN std::vector<int>& vecEncryptionKey, IN std::string& strFileName );
	static char* BufferDecrypt( IN std::vector<int>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize );
};
