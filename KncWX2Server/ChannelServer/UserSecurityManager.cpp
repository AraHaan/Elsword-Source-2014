#include ".\usersecuritymanager.h"
//{{ 2011. 09. 06	������	�ӽ� ID ��� ������Ʈ
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	#include "Base64/base64.h"
#endif SERV_MACHINE_ID_BLOCK_UPDATE
//}}


//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_BLOCK


KUserSecurityManager::KUserSecurityManager(void)
{
	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_iChannelRandomKey	= 0;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
}

KUserSecurityManager::~KUserSecurityManager(void)
{
}

const std::string KUserSecurityManager::GetStringMachineID() const
{
	std::string strResult = KncUtil::toNarrowString( m_wstrMachineID );
	return strResult;
}

void KUserSecurityManager::SetMachineID( IN const std::string& strMachineID )
{
    m_wstrMachineID = KncUtil::toWideString( strMachineID );
}

WORD KUserSecurityManager::GetHashFromMachineID( IN const std::string& strMachineID )
{
	if( strMachineID.size() < 16 )
	{
		return 0;
	}

	WORD wHash = *(WORD*)&strMachineID[14];
	return wHash;
}

WORD KUserSecurityManager::MakeHashFromMachineID( IN const std::string& strMachineID )
{
	if( strMachineID.size() < 16 )
	{
		return 0;
	}

	// �ؽ��� üũ!
	BYTE MachineId[16] = {0, };

	for( int i = 0; i < 16; ++i )
	{
		MachineId[i] = strMachineID[i];
	}

	// fills Mac Address, Serial Number.
	WORD wHash = 0;
	for (int i=0; i<6; ++i)
	{
		wHash += ((WORD*)MachineId)[i] * (i+3);
	}

	return wHash;
}

bool KUserSecurityManager::CheckValidMachineID( IN const std::string& strMachineID ) const
{
	// �ӽ�ID�� ����ִ��� �˻�!
	if( strMachineID.empty() == true )
		return false;

	// �ӽ�ID ���� �˻�
	if( strMachineID.size() < 16 )
		return false;

	//{{ 2011. 09. 06	������	�ӽ� ID ��� ������Ʈ
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	//machine id ��ȿ�� �˻� üũ �ʿ�
	//���ڵ� ó�� �����ϸ� ��.. �� base64 decode�� �˻��ؾ� ��
	std::string strDecodingMachineID = base64_decode( strMachineID );

	if( GetHashFromMachineID( strDecodingMachineID ) != MakeHashFromMachineID( strDecodingMachineID ) )
		return false;
#endif SERV_MACHINE_ID_BLOCK_UPDATE
	//}}

    return true;
}

#endif SERV_MACHINE_ID_BLOCK
//}}


