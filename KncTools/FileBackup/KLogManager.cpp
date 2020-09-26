/** @file
@author cruise@kogsoft.com
*/

const static char* cvsid = "$Id: KLogManager.cpp,v 1.9 2003/02/14 09:04:28 cvs Exp $";

#include "KLogManager.h"
#include <sstream>

/** Default Constructor. */
KLogManager::KLogManager(void) : m_pvecBuffer(NULL), m_pFile(NULL), m_hEventLog(NULL)
{
    m_pvecBuffer = new std::vector<char>(BUFSIZ);
    m_apszEventString[0] = &(m_pvecBuffer->at(0));
    // make all defaults
    SetOutputMethod(ALL_LEVEL, F_FILE | F_DEBUG | F_CONSOLE);
    SetOutputPrefix(ALL_LEVEL, NULL);
    SetEventType(ALL_LEVEL, ET_INFORMATION);
    // special settings for level 0 and 1.
    SetOutputPrefix(0, "ERROR: ");
    SetOutputPrefix(1, "Warning: ");
    SetEventType(0, ET_ERROR);
    SetEventType(1, ET_WARNING);
    memset( m_szFileName, 0, sizeof(m_szFileName) );
}

/** Destructor. */
KLogManager::~KLogManager(void)
{
    EndFileOutput();
    EndEventLogOutput();
    if (m_pvecBuffer != NULL)
    {
        delete m_pvecBuffer;
        m_pvecBuffer = NULL;
    }
}

/** Begin log output to the file `pszFileName'.
@warning �ش� level�� F_FILE flag�� set�Ǿ� �־�� Log(...)�� ����� �ش� ���Ϸ� ��µ�.
@param pszFileName [in] file name for log output
*/
void KLogManager::BeginFileOutput(const char* pszFileName)
{
    EndFileOutput();
    strcpy( m_szFileName, pszFileName );

    m_pFile = ::fopen(m_szFileName, "w");
}

/** End log output to the file. */
void KLogManager::EndFileOutput(void)
{
    if (m_pFile != NULL)
    {
        ::fclose(m_pFile);
        m_pFile = NULL;
    }
}

/** Begin log output to the system Event Log with program name.
@warning �ش� level�� F_EVENT flag�� set�Ǿ� �־�� Log(...)�� ����� Event Log�� ��µ�.
@param pszProgName [in] system Event Log�� ����� program name
*/
void KLogManager::BeginEventLogOutput(const char* pszProgName)
{
    EndEventLogOutput();
    m_hEventLog = ::RegisterEventSource(NULL, pszProgName);
}

/** End log output to the system Event Log. */
void KLogManager::EndEventLogOutput(void)
{
    if (m_hEventLog != NULL)
    {
        ::DeregisterEventSource(m_hEventLog);
        m_hEventLog = NULL;
    }
}

/** Set the program name and register the Event Log source.
Log�� ����ϴ� ���α׷��� �̸��� ����ϰ�, ���ÿ� EventLog�� ����ϱ�
���� �ý��ۿ� ����Ѵ�.
@param pszProgName [in] string for the program name
@warning system event log���� ȣ���� ���ؼ��� �� �Լ��� �ݵ�� ȣ���ؾ� �Ѵ�.
@deprecated Use BeginEventLogOutput(...).
*/
void KLogManager::SetProgramName(const char* pszProgName)
{
    BeginEventLogOutput(pszProgName);
}

/** Set the output file for the log messages.
Log�� ����� ������ open�Ѵ�.
@param pszFileName [in] string for the file name
@deprecated Use BeginFileOutput(...).
*/
void KLogManager::SetOutputFile(const char* pszFileName)
{
    BeginFileOutput(pszFileName);
}

/** Set the output method for a level.
�ϳ��� log level�� ���ؼ� log�� ����ϴ� ����� �����Ѵ�.
<ul>
<li> F_NONE : �ƹ��� ��µ� ���� �ʴ´�.
<li> F_CONSOLE : stderr �� log�� ��� (console program������ �۵���)
<li> F_DEBUG : (Visual Studio�� ����) Debugger ȭ������ ���
<li> F_FILE : ���Ϸ� ��� (BeginFileOutput(...), EndFileOutput(...) ���̿����� �۵�)
<li> F_EVENT : system event log�� ��� (BeginEventLogOutput(...),
                                        EndEventLogOutput(...) ���̿����� �۵�)
<li> F_ALL : ��� ������ ����� ��� ����
</ul>
�ʱ⿡�� (F_CONSOLE | F_DEBUG | F_FILE) �� �����Ǿ� �ִ�.
@param iLevel [in] ALL_LEVEL �Ǵ� setting �ϰ��� �ϴ� level (0 ~ NUM_LEVEL-1)
@param iFlag [in] F_NONE, F_ALL or ORing of { F_CONSOLE, F_DEBUG, F_FILE, F_EVENT }
*/
void KLogManager::SetOutputMethod(int iLevel, int iFlag)
{
    if (iLevel == ALL_LEVEL)
    {
        SetOutputMethod(0, NUM_LEVEL - 1, iFlag);
    }
    else if (iLevel >= 0 && iLevel < NUM_LEVEL)
    {
        m_bConOut[iLevel] = ((iFlag & F_CONSOLE) != 0);
        m_bDebugOut[iLevel] = ((iFlag & F_DEBUG) != 0);
        m_bFileOut[iLevel] = ((iFlag & F_FILE) != 0);
        m_bEventOut[iLevel] = ((iFlag & F_EVENT) != 0);
        m_bNeedOut[iLevel] = (m_bConOut[iLevel] || m_bDebugOut[iLevel] ||
                              m_bFileOut[iLevel] || m_bEventOut[iLevel]);
    }
}

/** Set the output method for a range of levels [iLevelStart..iLevelEnd].
@param iLevelStart,iLevelEnd [in] iLevelStart���� iLevelEnd������ level���� ����
@param iFlag [in] F_NONE, F_ALL or ORing of F_FILE, F_DEBUG, F_EVENT
@see SetOutputMethod(int iLevel, int iFlag)
*/
void KLogManager::SetOutputMethod(int iLevelStart, int iLevelEnd, int iFlag)
{
    if (iLevelStart < 0) iLevelStart = 0;
    if (iLevelEnd >= NUM_LEVEL) iLevelEnd = NUM_LEVEL - 1;
    bool bConOut = ((iFlag & F_CONSOLE) != 0);
    bool bDebugOut = ((iFlag & F_DEBUG) != 0);
    bool bFileOut = ((iFlag & F_FILE) != 0);
    bool bEventOut = ((iFlag & F_EVENT) != 0);
    bool bNeedOut = (bConOut || bDebugOut || bFileOut || bEventOut);
    for (int i = iLevelStart; i <= iLevelEnd; i++)
    {
        m_bConOut[i] = bConOut;
        m_bDebugOut[i] = bDebugOut;
        m_bFileOut[i] = bFileOut;
        m_bEventOut[i] = bEventOut;
        m_bNeedOut[i] = bNeedOut;
    }
}

/** Get the current output method for a given level.
@param iLevel [in] ������ �ϴ� level (0 ~ NUM_LEVEL-1)
@return ORing of F_CONSOLE, F_DEBUG, F_FILE, F_EVENT or 0 for invalid levels.
*/
int KLogManager::GetOutputMethod(int iLevel) const
{
    if (iLevel < 0 || iLevel >= NUM_LEVEL) return 0;
    return (m_bConOut[iLevel] ? F_CONSOLE : 0) |
           (m_bDebugOut[iLevel] ? F_DEBUG : 0) |
           (m_bFileOut[iLevel] ? F_FILE : 0) |
           (m_bEventOut[iLevel] ? F_EVENT : 0);
}

/** Set the output string prefix for a level.
Ư�� level�� log �޽����� ����� ��, �׻� ���� �տ� � prefix��
����ϰ� �� �� ����Ѵ�. SetOutputPrefix(5, "--> ") �� ���� ������ level
5�� log message���� log message���� �׻� "--> "�� ���� �տ� �߰���
���·� ��µǵ��� �����Ѵ�.
@warning pszPrefix�� ���� ���۷� copy���� �����Ƿ�, ������ ���� string��
calling function���� ��� �����Ͽ��� �Ѵ�.
@param iLevel [in] ALL_LEVEL �Ǵ� setting �ϰ��� �ϴ� level (0 ~ NUM_LEVEL-1)
@param pszPrefix [in] log message ��� �ÿ�, ���� �տ� �� prefix (��: "ERROR: "). ���ʿ�ô� NULL
*/
void KLogManager::SetOutputPrefix(int iLevel, const char* pszPrefix)
{
    if (iLevel == ALL_LEVEL)
    {
        SetOutputPrefix(0, NUM_LEVEL - 1, pszPrefix);
    }
    else if (iLevel >= 0 && iLevel < NUM_LEVEL)
    {
        m_apszPrefix[iLevel] = pszPrefix;
        m_iPrefixLen[iLevel] = (pszPrefix == NULL) ? 0 : strlen(pszPrefix);
    }
}

/** Set the output string prefix for a range of levels [iLevelStart..iLevelEnd].
@warning pszPrefix�� ���� ���۷� copy���� �����Ƿ�, ������ ���� string��
calling function���� ��� �����Ͽ��� �Ѵ�.
@param iLevelStart,iLevelEnd [in] iLevelStart���� iLevelEnd������ level���� ����
@param pszPrefix [in] log message ��� �ÿ�, ���� �տ� �� prefix (��: "ERROR: "). ���ʿ�ô� NULL
@see SetOutputPrefix(int iLevel, const char* pszPrefix)
*/
void KLogManager::SetOutputPrefix(int iLevelStart, int iLevelEnd, const char* pszPrefix)
{
    if (iLevelStart < 0) iLevelStart = 0;
    if (iLevelEnd >= NUM_LEVEL) iLevelEnd = NUM_LEVEL - 1;
    int iLen = (pszPrefix == NULL) ? 0 : strlen(pszPrefix);
    for (int i = iLevelStart; i <= iLevelEnd; i++)
    {
        m_apszPrefix[i] = pszPrefix;
        m_iPrefixLen[i] = iLen;
    }
}

/** Get the current output string prefix for a given level.
@warning return�Ǵ� pointer�� delete�ϸ� �� ��.
@param iLevel [in] ������ �ϴ� level (0 ~ NUM_LEVEL-1)
@return current string prefix or NULL.
*/
const char* KLogManager::GetOutputPrefix(int iLevel) const
{
    return ((iLevel >= 0) && (iLevel < NUM_LEVEL)) ? m_apszPrefix[iLevel] : NULL;
}

/** Set the event type of system Event Log for a level.
�ý����� �����ϴ� Event Log�� ��Ͻÿ� ����� event type�� �����Ѵ�.
default ���� level 0 �� level 1�� ���� ET_ERROR (EVENTLOG_ERROR_TYPE)��
ET_WARNING (EVENTLOG_WARNING_TYPE)�̰�, ������ level���� ���
ET_INFORMATION (EVENTLOG_INFORMATION_TYPE) �̴�.
@param iLevel [in] ALL_LEVEL �Ǵ� setting �ϰ��� �ϴ� level (0 ~ NUM_LEVEL-1)
@param wEventType [in] �����Ϸ��� event type: ET_ERROR, ET_WARNING, ET_INFORMATION
@see ::ReportEvent(...)
*/
void KLogManager::SetEventType(int iLevel, WORD wEventType)
{
    if (iLevel == ALL_LEVEL)
    {
        SetEventType(0, NUM_LEVEL - 1, wEventType);
    }
    else if (iLevel >= 0 && iLevel < NUM_LEVEL)
    {
        m_wEventType[iLevel] = wEventType;
    }
}

/** Set the event type of system Event Log for a range of levels [iLevelStart..iLevelEnd].
@param iLevelStart,iLevelEnd [in] iLevelStart���� iLevelEnd������ level���� ����
@param wEventType [in] �����Ϸ��� event type: ET_ERROR, ET_WARNING, ET_INFORMATION
@see ::ReportEvent(...)
*/
void KLogManager::SetEventType(int iLevelStart, int iLevelEnd, WORD wEventType)
{
    if (iLevelStart < 0) iLevelStart = 0;
    if (iLevelEnd >= NUM_LEVEL) iLevelEnd = NUM_LEVEL - 1;
    for (int i = iLevelStart; i <= iLevelEnd; i++)
    {
        m_wEventType[i] = wEventType;
    }
}

/** Get the current event type of system Event Log for a given level.
@param iLevel [in] ������ �ϴ� level (0 ~ NUM_LEVEL-1)
@return current event type of system Event Log or 0.
*/
WORD KLogManager::GetEventType(int iLevel) const
{
    return ((iLevel >= 0) && (iLevel < NUM_LEVEL)) ? m_wEventType[iLevel] : 0;
}

/** Internal function: iLevel�� ���� ����� �ʿ������� return. */
bool KLogManager::NeedOutput(int iLevel) const
{
    return (iLevel >= 0) && (iLevel < NUM_LEVEL) && m_bNeedOut[iLevel];
}

/** Internal function: m_pvecBuffer�� ����� message�� �����. */
void KLogManager::MakeMessage(int iLevel, const char* pszFormat, va_list vaList)
{
    int iOffset = 0;
    if (m_apszPrefix[iLevel] != NULL)
    {
        while (strlen(m_apszPrefix[iLevel]) + 1 > m_pvecBuffer->size() - iOffset)
        {
            m_pvecBuffer->resize(m_pvecBuffer->size() + BUFSIZ);
        }
        strcpy(&(m_pvecBuffer->at(iOffset)), m_apszPrefix[iLevel]);
        iOffset = m_iPrefixLen[iLevel];
    }
    while (::_vsnprintf(&(m_pvecBuffer->at(iOffset)), m_pvecBuffer->size() - iOffset,
                        pszFormat, vaList) < 0)
    {
        m_pvecBuffer->resize(m_pvecBuffer->size() + BUFSIZ);
    }
}

/** Internal function: m_pvecBuffer�� __FILE__, __LINE__���� ������ ������ �߰��Ѵ�. */
void KLogManager::AppendMessage(const char* pszFile, int iLine)
{
    int iOffset = strlen(&(m_pvecBuffer->at(0)));
    while (::_snprintf(&(m_pvecBuffer->at(iOffset)), m_pvecBuffer->size() - iOffset,
                       " (%s,%d)", pszFile, iLine) < 0)
    {
        m_pvecBuffer->resize(m_pvecBuffer->size() + BUFSIZ);
    }
}

/** Internal function: m_pvecBuffer�� �ִ� message�� �̸� ������ output method�鿡 ���� ����Ѵ�. */
void KLogManager::OutputMessage(int iLevel)
{
    if (m_bConOut[iLevel] == true)
    {
        ::fputs(&(m_pvecBuffer->at(0)), stderr);
        ::fputs("\n", stderr);
        ::fflush(stderr);
    }
    if (m_bDebugOut[iLevel] == true)
    {
        ::OutputDebugString(&(m_pvecBuffer->at(0)));
        ::OutputDebugString("\n");
    }
    if (m_bFileOut[iLevel] == true && m_pFile != NULL)
    {
        ::fputs(&(m_pvecBuffer->at(0)), m_pFile);
        ::fputs("\n", m_pFile);
        ::fflush(m_pFile);
    }
    if (m_hEventLog != NULL && m_bEventOut[iLevel] == true)
    {
        m_apszEventString[0] = &(m_pvecBuffer->at(0));
        ::ReportEvent(m_hEventLog, m_wEventType[iLevel], 0, 0, NULL, 1, 0, m_apszEventString, NULL);
    }
}

/** Log message output, printf-style.
@param iLevel [in] ����� log message�� level (0 ~ NUM_LEVEL-1)
@param pszFormat [in] printf-style format string
@param ... [in] printf-style remaining parameters
@return NULL if no need to output, otherwise the output string
*/
const char* KLogManager::Log(int iLevel, const char* pszFormat, ...)
{
    if (! NeedOutput(iLevel)) return NULL;
    // make the string to the m_pvecBuffer.
    va_list vaList;
    va_start(vaList, pszFormat);
    MakeMessage(iLevel, pszFormat, vaList);
    va_end(vaList);
    // output the string
    OutputMessage(iLevel);
    // end
    return &(m_pvecBuffer->at(0));
}

/** Log message output, printf-style, with __FILE__ and __LINE__ information.
�� �Լ��� __FILE__, __LINE__ ������ Preset(...)���� �̹� �����ߴٰ� �����Ѵ�.
@param iLevel [in] ����� log message�� level (0 ~ NUM_LEVEL-1)
@param pszFormat [in] printf-style format string
@param ... [in] printf-style remaining parameters
@return NULL if no need to output, otherwise the output string
*/
const char* KLogManager::LogL(int iLevel, const char* pszFormat, ...)
{
    if (! NeedOutput(iLevel)) return NULL;
    // make the string to the m_pvecBuffer.
    va_list vaList;
    va_start(vaList, pszFormat);
    MakeMessage(iLevel, pszFormat, vaList);
    va_end(vaList);
    // append file and line information
    AppendMessage(m_pszFile, m_iLine);
    // output the string
    OutputMessage(iLevel);
    // end
    return &(m_pvecBuffer->at(0));
}

/** String representation. */
std::string KLogManager::ToString(void) const
{
    std::ostringstream s;
    s << "KLogManager";
    for (int i = 0; i < NUM_LEVEL; i++)
    {
        s << std::endl;
        s << "  " << i;
        s << ": out=" << m_bNeedOut[i];
        s << ", con=" << m_bConOut[i];
        s << ", debug=" << m_bDebugOut[i];
        s << ", file=" << m_bFileOut[i];
        s << ", event=" << m_bEventOut[i];
        s << ", eventType=" << m_wEventType[i];
        if (m_apszPrefix[i] != NULL) s << ", prefix=\"" << m_apszPrefix[i] << "\"";
        else s << ", prefix=NULL";
    }
    return s.str();
}
