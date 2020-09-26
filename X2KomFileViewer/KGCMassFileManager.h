#pragma once

#pragma warning(disable:4786)

class KGCBufferManager;

class KGCMassFileManager  
{
private:
	struct SMassFileIndex		// file�̸����� MassFile�� �����Ϳ� Index�� ������ �ϱ�����
	{
		CMassFile* pMassFile;
		unsigned int Index;
	};

public:
	KGCMassFileManager();
	virtual ~KGCMassFileManager();

	void		SetDataDirectory(const std::string& strDirectory);
	CMassFile*	AddMassFile(const std::string& strKomFileName);
	bool		LoadDataFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut);
	int			GetTotalFileCount();

private:
	//KOM������ �ƴ� Data�������� ���� ������ �о� ���̴°��..
	bool		LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO *pOut);

	std::map< std::string, SMassFileIndex >	m_mapMassFile;
	std::vector< CMassFile* >				m_vecMassFile;
	std::string								m_strDataDirectory;

	KGCBufferManager						m_BufferManager;
};
