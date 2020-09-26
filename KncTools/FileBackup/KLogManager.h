/** @file
@author cruise@kogsoft.com
*/

#ifndef _KOG_KLOGMANAGER_H_
#define _KOG_KLOGMANAGER_H_ "$Id: KLogManager.h,v 1.8 2003/02/11 03:13:24 cvs Exp $"
#pragma once // for _MSC_VER > 1000

#include <string>
#include <cstdio>
#include <cstdarg>
#include <windows.h>
#include <vector>
#include "KSingleton.h"

/** Log Message Manager.
���α׷� �ۼ� �ÿ� ����� �ʿ��� log message���� �ϰ������� �ٷ�� ����
manager class�̴�. �⺻������, log message�� �� �߿䵵�� ����, 0 (����
�߿�) ~ NUM_LEVEL-1 (���� �� �߿�) �� ������ ������. level 0 �� level
1�� ���� error (�ý��ۿ� critical�� error�� �ǹ�)�� warning (����ڰ� ��
�˾ƾ� �� warning�� �ǹ�)���� �����ϰ�, ������ level���� ���α׷��Ӱ�
������̳� ��Ÿ �������� ����ϰ��� �ϴ� log message��� �����Ѵ�.

log message�� ���α׷��ӿ��� �˸��� ����� ũ�� 4������ ����Ͽ���.
<ul>
<li> �ܼ� ��� (console output, F_CONSOLE) : console program������
     ����� �Ͼ��, stderr�� log message�� ����Ѵ�.
<li> ����� ��� (debugger output, F_DEBUG) : MS Visual Studio
     ȯ�濡���� ����� â�� �޽����� ����Ѵ�.
<li> ���� ��� (file output, F_FILE) : ��� ���α׷�������
     ��밡���ϰ�, ������ ���Ͽ� �޽����� ����Ѵ�. ���� �̸���
     BeginFileOutput(...) �Լ����� �����Ѵ�.
<li> �ý��� �̺�Ʈ �α� (system Event Log, F_EVENT) : Windows �ý�����
     �̺�Ʈ �����ڿ��� �޽����� ����� �д�. �� ���� �ٸ� �ý��ۿ���
     remote�� �����ؼ� �޽����� ���� �͵� �����ϴ�. ����� �޽����� ���
     ���α׷��� ����ϴ���, ���α׷� �ĺ��ڰ� �ʿ��� ��, �� �ĺ��ڴ�
     BeginEventLogOutput(...) �Լ����� �����Ѵ�.
</ul>
�� �޽����� �����Ǵ� level�� ����, � ��� ����� �� ������ ���� ��
�ְ�, log�� ������ ������ � level�� log������ ����, �ش� level����
�̸� ������ ��� ����鿡 ����, log �޽����� ����Ѵ�.

�⺻ ������ ��� level����, �ܼ�, �����, ���� ����� on �ǰ�, �ý���
�̺�Ʈ ����� off �Ǿ� �ִ�. �����̳� �ý��� �̺�Ʈ ����� flag�� on
��Ű�� �� �̿ܿ�, BeginFileOuput(...) �̳� BeginEventLogOutput(...)
�Լ��� ȣ���� �ʿ�� �Ѵ�.

�޽����� ��� ������ <BLOCKQUOTE> (Prefix) (User-provided message)
[(FileName,LineNumber)] </BLOCKQUOTEIP> ���¸� ������. <b>(Prefix)</b>��
�ش� level�� �޽������� ���������� ������ prefix�� �ǹ��ϰ�, level 0��
level 1�� ���ؼ��� "ERROR: "�� "Warning: "�� �⺻������ ������, ������
level���� NULL string�� �⺻������ ������. <b>(User-provided
message)</b>�� ����ڰ� ������ ����ϰ��� �ϴ� message��, printf-style��
�Լ����� �����Ѵ�. �̵� �Լ����� �����Ǵ� long-type�Լ��� (�Լ� �̸���
���� ���� 'L'�� ÷���� �Լ���)�� ������, �̵� long-type �Լ����� �����
������ �κп� C preprocessor���� �����ϴ� __FILE__�� __LINE__ ��ũ��
���� �߰��� ����Ѵ�.

@verbinclude testKLogManager.cpp
*/
class KLogManager
{
public:
    /** Default constructor. */
    _DLLAPI KLogManager(void);
    /** Destructor. */
    _DLLAPI ~KLogManager(void);
private:
    /** Copy constructor: DISABLED. */
    _DLLAPI KLogManager(const KLogManager&);
    /** Assignment operator: DISABLED. */
    _DLLAPI void operator=(const KLogManager&);

public:
    /** String representation. */
    _DLLAPI std::string ToString(void) const;

    // BEGIN / END FOR FILE AND SYSTEM EVENT LOG OUTPUT ****************

    /** Begin log ouptut to the file pszFileName. */
    _DLLAPI void BeginFileOutput(const char* pszFileName);
    /** End log output to the file. */
    _DLLAPI void EndFileOutput(void);
    /** Begin log output to the system Event Log with program name. */
    _DLLAPI void BeginEventLogOutput(const char* pszProgName);
    /** End log output to the system Event Log. */
    _DLLAPI void EndEventLogOutput(void);

    // OUTPUT METHOD ***************************************************

    /** Set the output method for a level. */
    _DLLAPI void SetOutputMethod(int iLevel, int iFlag);
    /** Set the output method for a range of levels [iLevelStart..iLevelEnd]. */
    _DLLAPI void SetOutputMethod(int iLevelStart, int iLevelEnd, int iFlag);
    /** Get the current output method for a given level. */
    _DLLAPI int GetOutputMethod(int iLevel) const;

    // OUTPUT PREFIX ***************************************************

    /** Set the output string prefix for a level. */
    _DLLAPI void SetOutputPrefix(int iLevel, const char* szPrefix);
    /** Set the output string prefix for a range of levels [iLevelStart..iLevelEnd]. */
    _DLLAPI void SetOutputPrefix(int iLevelStart, int iLevelEnd, const char* szPrefix);
    /** Get the current output string prefix for a given level. */
    _DLLAPI const char* GetOutputPrefix(int iLevel) const;

    // EVENT TYPE FOR SYSTEM EVENT LOG OUTPUT **************************

    /** Set the event type of system Event Log for a level. */
    _DLLAPI void SetEventType(int iLevel, WORD wEventType);
    /** Set the event type of system Event Log for a range of levels [iLevelStart..iLevelEnd]. */
    _DLLAPI void SetEventType(int iLevelStart, int iLevelEnd, WORD wEventType);
    /** Get the current event type of system Event Log for a given level. */
    _DLLAPI WORD GetEventType(int iLevel) const;

    // LOG MESSAGE OUTPUT **********************************************

    /** Log message output, printf-style. */
    _DLLAPI const char* Log(int iLevel, const char* szFormat, ...);
    /** Log message output, for a HRESULT value. */
    _DLLAPI const char* Log(int iLevel, HRESULT hr, ...);

    /** Preset __FILE__ and __LINE__ for LogL(...) functions. */
    _DLLAPI inline void Preset(const char* szFile, int iLine) { m_pszFile = szFile; m_iLine = iLine; }
    /** Log message output, printf-style, with __FILE__ and __LINE__ information. */
    _DLLAPI const char* LogL(int iLevel, const char* szFormat, ...);
    /** Log message output, for a HRESULT value, with __FILE__ and __LINE__ information. */
    _DLLAPI const char* LogL(int iLevel, HRESULT hr, ...);

    // DEPRECATED ******************************************************

    /** Set the program name and register the Event Log source. */
    _DLLAPI void SetProgramName(const char* szProgName);
    /** Set the output file for the log messages. */
    _DLLAPI void SetOutputFile(const char* szFileName);
private:
    /** Internal function: iLevel�� ���� ����� �ʿ������� return. */
    _DLLAPI bool NeedOutput(int iLevel) const;
    /** Internal function: m_aszBuffer�� ����� message�� �����. */
    _DLLAPI void MakeMessage(int iLevel, const char* szFormat, va_list vaList);
    /** Internal function: m_aszBuffer�� __FILE__, __LINE__���� ������ ������ �߰��Ѵ�. */
    _DLLAPI void AppendMessage(const char* szFile, int iLine);
    /** Internal function: m_aszBuffer�� �ִ� message�� output method�鿡 ���� ����Ѵ�. */
    _DLLAPI void OutputMessage(int iLevel);
public:
    enum
    {
        ALL_LEVEL = -1,		///< When used for level, indicating all levels.
        NUM_LEVEL = 10,		///< Number of valid levels.
        // flags for SetOutputMethod(...)
        F_NONE = 0,		///< No output method.
        F_CONSOLE = 0x01,	///< Console output method is turning on.
        F_DEBUG = 0x02,		///< Debugger window output method is turning on.
        F_FILE = 0x04,		///< File output method is turning on.
        F_EVENT = 0x08,		///< Event Log output method is turning on.
        F_ALL = F_FILE | F_DEBUG | F_EVENT | F_CONSOLE, ///< Turning on all output methods.
        // event types for SetEventType(...)
        ET_ERROR = EVENTLOG_ERROR_TYPE,
        ET_WARNING = EVENTLOG_WARNING_TYPE,
        ET_INFORMATION = EVENTLOG_INFORMATION_TYPE,
    };
private:
    std::vector<char>* m_pvecBuffer; ///< Main buffer for printf-style processing.
    const char* m_apszEventString[1]; ///< Directing m_aszBuffer for ::ReportEvent(...).
    const char* m_apszPrefix[NUM_LEVEL]; ///< Prefix strings for each level.
    int m_iPrefixLen[NUM_LEVEL]; ///< Length of prefix strigns for each level.
    bool m_bNeedOut[NUM_LEVEL]; ///< True iff a level needs any output.
    bool m_bConOut[NUM_LEVEL]; ///< True iff a level needs console output.
    bool m_bDebugOut[NUM_LEVEL]; ///< True iff a level needs debugger window output.
    bool m_bFileOut[NUM_LEVEL]; ///< True iff a level needs file output.
    bool m_bEventOut[NUM_LEVEL]; ///< True iff a level needs system event log output.
    WORD m_wEventType[NUM_LEVEL]; ///< Event type for system event log.
    FILE* m_pFile; ///< File pointer for the output.
    HANDLE m_hEventLog; ///< Handle for system event log.
    const char* m_pszFile; ///< Temporary storage for __FILE__
    int m_iLine; ///< Temporary storage for __LINE__
    char m_szFileName[_MAX_FNAME];
};

#if ! defined(KLOG_OFF)
#define pSKLogManager KSingleton<KLogManager>::GetInstance() 
/** Log message output, printf-style.
��ũ�� �Լ��� ���ǵǰ�, printf-style�� ���� ������, �θ� ���� �ݵ��
KLOG((1, "message %d", number)) �� ����, �� ���� ��ȣ�� �ѷ� �ξ� �Ѵ�.
*/
#define KLOG(x)  pSKLogManager->Log x 
/** Log message output, printf-style, with __FILE__ and __LINE__ information.
��ũ�� �Լ��� ���ǵǰ�, printf-style�� ���� ������, �θ� ���� �ݵ��
KLOGL((1, "message %d", number)) �� ����, �� ���� ��ȣ�� �ѷ� �ξ� �Ѵ�.
*/
#define KLOGL(x) pSKLogManager->Preset(__FILE__, __LINE__); pSKLogManager->LogL x
#else
#define pSKLogManager KSingleton<KLogManager>::GetInstance()
#define KLOG(x)  ((void)0)
#define KLOGL(x) ((void)0)
#endif

#endif // _KOG_KLOGMANAGER_H_
