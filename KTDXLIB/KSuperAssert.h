#pragma once

#include <iostream>
#include <sstream>
#include <string>

class KSuperAssert
{

public:
    KSuperAssert();

	KSuperAssert& SetAssertInfo(const char* expr, unsigned int line, 
		const char* func, const char* file);

	void _initBuf();
	void _fatal();
	void _trace();
	void _fileout(bool bFlush);
	void _remotelog();

	template<typename T>
	KSuperAssert& operator << (T val)
	{
		std::ostringstream os;
		os << val;
		m_strInfo += os.str().c_str();

		return *this;
	}

	template<>
		KSuperAssert& operator << (const WCHAR* val)
	{
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_ACP, 0, val, -1, charBuf, 255, NULL, NULL );


		std::ostringstream os;
		os << charBuf;
		m_strInfo += os.str().c_str();

		std::cout << charBuf << std::endl;

		return *this;
	}

	template<>
		KSuperAssert& operator << (WCHAR* val)
	{
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_ACP, 0, val, -1, charBuf, 255, NULL, NULL );


		std::ostringstream os;
		os << charBuf;
		m_strInfo += os.str().c_str();

		std::cout << charBuf << std::endl;

		return *this;
	}

	// manipication
	template<>
	KSuperAssert& operator << (KSuperAssert& (*_manip) (KSuperAssert&))
	{
		return _manip( *this );
	}

	friend KSuperAssert& flush(KSuperAssert& kAssert);

	// Flush
	friend KSuperAssert& fatal(KSuperAssert& kAssert);
	friend KSuperAssert& trace(KSuperAssert& kAssert);
	friend KSuperAssert& fileout(KSuperAssert& kAssert);
	friend KSuperAssert& remotelog(KSuperAssert& kAssert);

	// Not Flush
	friend KSuperAssert& fatal_(KSuperAssert& kAssert);
	friend KSuperAssert& trace_(KSuperAssert& kAssert);
	friend KSuperAssert& fileout_(KSuperAssert& kAssert);
	friend KSuperAssert& remotelog_(KSuperAssert& kAssert);

	// Names
public:
	void SetLogFilename(std::string strFilename);
	void SetRemoteLoggerPagename(std::string strPagename);
	
protected:
	std::string m_strLogFilename;
	std::string m_strRemoteLoggerPagename;

protected:
	std::string m_strExpression;
	std::string m_strFilename;
	unsigned int m_uiLine;

	std::string m_strInfo;
	std::string m_strBuf;

};

extern KSuperAssert g_kSuperAssert;


// additional macros

#define KLOG(filename)			KDbgStm.SetLogFilename(filename); KDbgStm

#ifndef _SERVICE_

#ifdef BUG_TRAP
	#define InHouseLog( msg )
	
	#define StateLog( msg ) \
    {	\
        g_CKTDXLog.PushStateLog( msg );	\
    }

    #define ErrorLogMsg( errorEnum, errorMsg )		\
    {	\
        g_CKTDXLog.PushErrorLog( errorEnum, errorMsg );	\
    }

    #define ErrorLog( errorEnum )					\
    {	\
        g_CKTDXLog.PushErrorLog( errorEnum );	\
    }

    #define DialogLog( msg )

    #define LastErrorLog( errorMsg )					\
    {	\
        g_CKTDXLog.SetLastErrorLog( errorMsg );	\
    }

    #define ChecksumLog( msg ) 

#else BUG_TRAP

	#define InHouseLog( msg ) KLOG("InHouse.log")  << msg << fileout;
	#define StateLog( msg ) KLOG("State.log")  << msg << fileout;
	

	#define ErrorLogMsg( errorEnum, errorMsg )		\
	{	\
				KLOG("ErrorLog.txt") << "ErrorEnum(" << errorEnum << "),\t" << "ErrorMsg(\"" << errorMsg << "\")," << "\t( " <<__FILE__ << ", " << __FUNCTION__  << ", " << __LINE__ << " )" << fileout;  \
				std::cout << "ErrorEnum(" << errorEnum << "),\t" << "ErrorMsg(\"" << errorMsg << "\")," << "\t( " <<__FILE__ << ", " << __FUNCTION__  << ", " << __LINE__ << " )" << std::endl; \
				fprintf( stderr, "\n�� ErrorEnum( %d ), %s, %s, %d\n", errorEnum, __FILE__, __FUNCTION__, __LINE__ );	\
	}

	#define ErrorLog( errorEnum )					\
	{	\
				KLOG("ErrorLog.txt") << "ErrorEnum(" << errorEnum << "),\t( " <<__FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << " )" << fileout; \
				std::cout << "ErrorEnum(" << errorEnum << "),\t( " <<__FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << " )" << std::endl; \
				fprintf( stderr, "\n�� ErrorEnum( %d ), %s, %s, %d\n", errorEnum, __FILE__, __FUNCTION__, __LINE__ );	\
	}
	#define DialogLog( msg )		\
	{	\
		KLOG("DialogLog.txt")  << msg << ",\t\t" << __FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << fileout; \
	}

	#define LastErrorLog( errorMsg )


	#define ChecksumLog( msg ) KLOG("Checksum.log")  << msg << fileout;

	//{{ kimhc // 2010.10.5 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
#ifdef	AVATAR_IMAGE_TEST
	#define CheckUploadFileLog( msg )		\
	{	\
		KLOG("UploadFile.txt")  << msg << fileout; \
	}
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.10.5 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ

#endif BUG_TRAP

#else _SERVICE_

	#define InHouseLog( msg )

	#define StateLog( msg ) \
	{	\
		g_CKTDXLog.PushStateLog( msg );	\
	}

	#define ErrorLogMsg( errorEnum, errorMsg )		\
	{	\
		g_CKTDXLog.PushErrorLog( errorEnum, errorMsg );	\
	}

	#define ErrorLog( errorEnum )					\
	{	\
		g_CKTDXLog.PushErrorLog( errorEnum );	\
	}

	#define DialogLog( msg )

	#define LastErrorLog( errorMsg )					\
	{	\
		g_CKTDXLog.SetLastErrorLog( errorMsg );	\
	}

	#define ChecksumLog( msg ) 

//{{ kimhc // 2010.10.5 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ
#ifdef	AVATAR_IMAGE_TEST
	#define CheckUploadFileLog( msg )		\
	{	\
		KLOG("UploadFile.txt")  << msg << fileout; \
	}
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.10.5 // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ

#endif _SERVICE_


/*
#define P2PLogW( msg )			\
	{ \
		string strP2PLogW; \
		ConvertWCHARToChar( strP2PLogW, msg ); \
		KLOG("P2PLog.txt")  << strP2PLogW.c_str() << fileout; std::cout << strP2PLogW.c_str() << std::endl; \
	}
*/
// Macros
//#ifdef _DEBUG

	#define KDbgStm g_kSuperAssert
	#define KAssert(mExpr) \
		if (!(mExpr)) \
			g_kSuperAssert.SetAssertInfo(#mExpr, __LINE__, __FUNCTION__, __FILE__)._fatal(); \
		else \
			g_kSuperAssert._initBuf();

//#else // _DEBUG
//
//	#define KDbgStm /##/
//	#define KAssert(mExpr) /##/
//
//#endif // _DEBUG

/*
	* ��� ���
	- Trace :
		KDbgStm << Debug Info << trace;
	- Assert : 
		KDbgStm << Debug Info;
		KAssert( exp );

	* ����
	- KDbgStm ��� �� ���� ������ ��� �� ���θ��� KDbgStm�� �����־�� ��
		KDbgStm << "Value1";
		KDbgStm << "Value2";
		.
		.
		.
	  
	  ( ������ ���� ��� ���� )
		KDbgStm
				<< "Value1"
				<< "Value2"
				.
				.
				.
	  ������ ������ ��忡���� ���ϸ� ��� ���ֱ� ���� ( 2��° ����� ��� �Լ� ȣ���� ����� ���� )
*/
