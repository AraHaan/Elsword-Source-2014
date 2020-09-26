#pragma once

#define MASSFILE_VERSION		(1)
#define MASSFILE_NAMESIZE		(60)

#define MFI_REALDATA			(1)
#define MFI_NODECOMPRESS		(2)

class CMassFile
{
	struct MASSFILE_HEADER
	{
		char strVersionInfo[50];
		UINT iTotalFileNo;
		bool bCompressed;
	};
	struct MASSFILE_MEMBERFILEHEADER
	{
		char strFileName[MASSFILE_NAMESIZE];
		long RealSize;
		long CompressSize;
		long offSet;		//RealData�κп����� OffSet�Դϴ�
	};

public:
	struct MASSFILE_MEMBERFILEINFO
	{
		char strFileName[MASSFILE_NAMESIZE];
		long size;
		long CompressedSize;
		char *pRealData;
		DWORD dwFlag;
	};

public:
	CMassFile();
	virtual ~CMassFile();
	UINT GetFileCount();		//MassFile�� ��� File�� ���ԵǾ��ִ��� �˾ƺ��ϴ�.

	bool LoadMassFile(const char* strMassFile);
	bool GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
	bool GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

	static KGCBufferManager m_BufferManager1;
	static KGCBufferManager m_BufferManager2;

	//�Ʒ� �� �Լ��� MassFile�� Load���ѵ��� �ʰ� �ϳ��� �о� ���̰��� �Ҷ�..
	//������ ����ҽ� �ӵ� ������ ������ ��
	bool GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut);
	bool GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut);

	bool SaveMassFile(char* strFileName, std::vector<char*> *pVecFile, bool bZip = false);
	bool SaveMassFile();
	bool ModifyMassFile(CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile);
	bool ModifyMassFile(CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile, int iSubFileSize);

	//FILE����...Help
	long GetFileSize(const char* strFileName);
	void GetFileName(char* Buffer,char* strFilePath);

	bool IsCompressed();

private:

	std::vector<MASSFILE_MEMBERFILEHEADER> m_vecMemberFileHeader;
	MASSFILE_HEADER						   m_MassFileHeader;
	char								   m_strMassFileName[MAX_PATH];
	FILE*								   m_File;
	int									   m_iFileVersion;

};
