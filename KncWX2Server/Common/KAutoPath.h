#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class KAutoPath
{
public:
	//{{ 2009. 7. 13  ������	�ϵ��ڵ� ����
	static void AddPath( const std::string& strPath ); // ���� ��� �߰� �Լ�
	//}}

public:
	KAutoPath();
	~KAutoPath(void);

	bool	GetPullPath( std::wstring& wstrFileName );
	bool	GetPullPath( std::string& strFileName );

private:
	void	InitDirectory( std::string strCurDir );

private:
	std::vector< std::string >	m_vecDirectory;
};