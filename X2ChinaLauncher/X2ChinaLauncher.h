// X2ChinaLauncher.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ

class TiXmlNode;

// CX2ChinaLauncherApp:
// �� Ŭ������ ������ ���ؼ��� X2ChinaLauncher.cpp�� �����Ͻʽÿ�.
//

class CX2ChinaLauncherApp : public CWinApp
{
public:
	CX2ChinaLauncherApp();

// ������
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����

	DECLARE_MESSAGE_MAP()

private:
	struct Addr
	{
		std::string strValue;
		std::string strPath;
	};

	void InitLauncher();
	BOOL DeleteFileForce(const std::string filename);

#if 0
	BOOL LoadLauncherPatchServer();
	BOOL LoadAddrTag(TiXmlNode* pkNode);
	BOOL GetVersionInServer(const std::string& strAddr, const std::string& strPath, unsigned int& uiChecksum);
	BOOL IsNewVersion(unsigned int uiServerChecksum);
	BOOL PatchLauncher(const std::string& strAddr, const std::string& strPath, const std::string strLauncherName);
	BOOL ReRunLauncher();
#endif

	std::vector<Addr> m_vecAddr;

	std::string m_strLocalPath;
	std::string m_strLauncherName;

};

extern CX2ChinaLauncherApp theApp;
