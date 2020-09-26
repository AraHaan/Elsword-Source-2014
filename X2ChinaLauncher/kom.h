#pragma once

#include "define.h"

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <map>
#include <sstream>
#include "HttpSession.h"
#include "adler32.h"

#define HEADEROFFSET 72

extern bool DeleteFileForce( std::string filename );

class KSubfile
{
public:
	enum EAlgorithm
	{
		infilate = 0,
		lzma = 1
	};	

	KSubfile(std::string filename,std::ifstream & stream,int headersize); // �� ���� �޿��� �д� �Լ�	
	KSubfile(std::string filename,xmlNode * node,int * offset); // �Ź��� 	
	KSubfile(KHttpSession * con,std::string filename,xmlNode * node,int * offset); // URL �Ź���
	

	KSubfile(const KSubfile &src)
		:filename(src.filename),filetime(src.filetime),adler32(src.adler32),size(src.size),compressedsize(src.compressedsize),algorithm(src.algorithm),offset(src.offset),connection(src.connection),iscalcadler(src.iscalcadler)
	{}
	~KSubfile(){};

	bool operator == (const KSubfile &r)
	{
		//if(r.filetime==0)		
		// ����Ÿ���� üũ���� �ʴ°� �´�. ���� ��ġ�� �� �޴� ������ �����. by ������
			return (adler32 == r.adler32 && algorithm == r.algorithm &&	compressedsize == r.compressedsize);
		
		//return (adler32 == r.adler32 && algorithm == r.algorithm &&	compressedsize == r.compressedsize && filetime == r.filetime);
	}

	KSubfile & operator= (const KSubfile &r)
	{
		adler32 = r.adler32;
		algorithm = r.algorithm;
		compressedsize = r.compressedsize;		
		connection = r.connection;
		filename = r.filename;
		size = r.size;
		offset = r.offset;
		iscalcadler = r.iscalcadler;
		if(r.filetime != 0)
			filetime = r.filetime;
		return *this;
	}

	bool WriteCompressed(std::ostream &stream);
	bool Verify();


	unsigned int GetFileTime(){return filetime;}
	void SetFileTime(unsigned int ft){filetime = ft;} // �����ؼ� �� ����Ѵ�.

	unsigned int GetAdler32();

	unsigned int GetSize(){return size;}
	unsigned int GetCompressedSize(){return compressedsize;}
	int GetAlgorithm(){return algorithm;}

private:

	std::string filename;
	unsigned int filetime;
	unsigned int adler32;
	unsigned int size;
	unsigned int compressedsize;
	int algorithm;

	unsigned int offset;
	KHttpSession * connection;

	bool iscalcadler;
};


class Komfile
{
public:	
	enum EKomfileError
	{
		eKomfileError_Type_FileAccess = 0,
		eKomfileError_Type_InvalidFile,
		eKomfileError_Type_Success
	};

	enum KOM_TYPE
	{
		NOT_KOM = -1,
		OLD_KOM = 0,
		CUR_KOM = 1
	};

	static KOM_TYPE CheckKom(std::string filename);
	static bool Verify(std::string filename);
	static bool Verify(KHttpSession * session , std::string filename);
	
	
	Komfile(){Close();};
	Komfile(const Komfile& r)
		:subfiles(r.subfiles),filetime(r.filetime),adler32(r.adler32),headersize(r.headersize)
	{};

	~Komfile(){Close();};



	Komfile & operator = (const Komfile & r)
	{
		subfiles = r.subfiles;
		filetime = r.filetime;
		adler32 = r.adler32;
		headersize = r.headersize;

		return *this;
	}

	// ���� �Լ��� 
	// ���� �켱���� �����Ѵ�
	// ���� ���ʿ� URL �� �����ʿ� ������ �ֱ� �ٶ�

	bool LeftOuterJoin(Komfile &left, Komfile &right);	
	
	bool Open(KHttpSession * con,std::string url);
	bool Open(std::string filename);	
	EKomfileError Save(std::string filename,FILE_PROGRESS_CALLBACK progress = NULL);	
	void Close();

	unsigned int GetFileTime();
	unsigned int GetAdler32(){return adler32;}
	unsigned int GetHeaderSize(){return headersize;}

	bool Verify();
private:
	
	std::map<std::string,KSubfile> subfiles;
	unsigned int filetime;
	unsigned int adler32;
	unsigned int headersize;	
};