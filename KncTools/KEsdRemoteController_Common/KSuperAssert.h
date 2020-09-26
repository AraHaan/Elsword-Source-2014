#ifndef KSUPERASSERT_H
#define KSUPERASSERT_H

#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>

#define KGLOBALLOGBUFSIZE	2

class KSuperAssert
{

public:
    KSuperAssert();
	~KSuperAssert();

	KSuperAssert& SetAssertInfo(const char* expr, unsigned int line, 
		const char* func, const char* file);

	KSuperAssert& _initBuf();
	KSuperAssert& _trace(bool bFlush = true, int iLogIdx = 0);
	KSuperAssert& _fileout();
	KSuperAssert& _remotelog();
	bool _fatal(int sIgnoreCount, int sFailCount, DWORD dwStack, DWORD dwStackFrame);

public:
	KSuperAssert& operator << (float val)
	{
		std::ostringstream os;
		os << std::setprecision(8) << val;
		m_strInfo += os.str().c_str();

		return *this;
	}

	template <typename T>
	KSuperAssert& operator << (T val)
	{
		std::ostringstream os;
		os << val;
		m_strInfo += os.str().c_str();

		return *this;
	}

	// manipication
	template <>
		KSuperAssert& operator << (KSuperAssert& (*_manip) (KSuperAssert&))
	{
		return _manip( *this );
	}

	friend KSuperAssert& flush(KSuperAssert& kAssert);
	friend KSuperAssert& trace(KSuperAssert& kAssert);
	friend KSuperAssert& fileout(KSuperAssert& kAssert);
	friend KSuperAssert& remotelog(KSuperAssert& kAssert);

	// Names
public:
	void StartGlobalLog();
	void SetLogFilename(std::string strFilename);
	void SetRemoteLoggerPagename(std::string strPagename);
	
protected:
	std::fstream m_kGlobalFileStream[KGLOBALLOGBUFSIZE];

	std::string m_strLogFilename;
	std::string m_strRemoteLoggerPagename;

protected:
	std::string m_strExpression;
	std::string m_strFilename;
	std::string m_strFuncname;
	unsigned int m_uiLine;

	std::string m_strInfo;
	std::string m_strBuf;
	std::string m_strGlobalBuf;

	// Lock
public:
	void LockGlobalFileStream(int iIdx, bool bEnable);
	void LockLogFileStream(bool bEnable);
	void LockLockConsole(bool bEnable);
	void LockOutputDebugString(bool bEnable);

	bool IsLockGlobalFileStream(int iIdx);
	bool IsLockLogFileStream();
	bool IsLockConsole();
	bool IsLockOutputDebugString();

protected:
	bool m_bLockGlobalFileStream[KGLOBALLOGBUFSIZE];
	bool m_bLockLogFileStream;
	bool m_bLockConsole;
	bool m_bLockOutputDebugString;

};

extern KSuperAssert g_kSuperAssert;

// Macros
//#ifdef _DEBUG

#define KDbgStm g_kSuperAssert
#define KAssert(mExpr)																		\
{																							\
	/* The local instance of the ignore count and the total hits. */						\
	static int sIgnoreCount = 0;															\
	static int sFailCount   = 0;															\
	/* The local stack and frame at the assertion's location. */							\
	DWORD dwStack;																			\
	DWORD dwStackFrame;																		\
	/* Check the expression. */																\
	if (!(mExpr))																			\
	{																						\
		/* Houston, we have a problem. */													\
		_asm { MOV dwStack , ESP }															\
		_asm { MOV dwStackFrame , EBP }														\
		if (g_kSuperAssert.SetAssertInfo(#mExpr, __LINE__, __FUNCTION__, __FILE__)._fatal(	\
			sIgnoreCount, sFailCount, dwStack, dwStackFrame) == true)						\
		{																					\
			__asm INT 3																		\
		}																					\
	}																						\
	else																					\
	{																						\
		g_kSuperAssert._initBuf();															\
	}																						\
}

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

#endif // KSUPERASSERT_H