#ifndef ENUMFILE_H
#define ENUMFILE_H
#pragma warning(disable: 4786)

#include <iostream>
#include <map>

#include "KAssert.h"
#include "tinyxml.h"

class KStrLess
{
public:
    bool operator()(const char* pa, const char* pb) const
    {
        return (strcmp(pa, pb) < 0);
    }
};

class KDirInfo
{
public:
    char        m_szName[_MAX_PATH];
};
typedef std::map<const char*, KDirInfo*, KStrLess>  DirMap;


class KFileInfo
{
public:
    char        m_szName[_MAX_PATH];
    DWORD       m_dwSize;
    USHORT      m_usVersion;
    std::string m_strCheckSum;
//{{ Modified by ct 2003-04-29 ���� 04:53:14
    //FILETIME    m_ftLastWriteTime;
//}} Modified by ct 2003-04-29 ���� 04:53:14
};
typedef std::map<const char*, KFileInfo*, KStrLess> FileMap;


class KEnumFile
{
public:
    KEnumFile();
    ~KEnumFile();
    
    bool CreateFileList(const char* szCurDir, const char* szFileName);
    bool CreateFileList(const char* szCurDir, TiXmlDocument& xmlDoc);
    
    // �� ���� serverfilelist.xml�� �ֱٿ� ������� ������ �� ���ο� serverfilelist.xml�� ����
    bool CompareFileVersion(const TiXmlDocument& xmlOldDoc,
                            const TiXmlDocument& xmlDoc,
                            TiXmlDocument& xmlNewDoc);
    bool CompareFileVersion(const char* szOldPath, const TiXmlDocument& xmlOldDoc,
                            const char* szNewPath, const TiXmlDocument& xmlDoc,
                            TiXmlDocument& xmlNewDoc);
    
    // local�� server�� file list �� download �޾ƾ� �� file list ����
    bool CreateUpdateFileList(const char* szLocalFileName,
                              const char* szServerFileName,
                              TiXmlDocument& xmlUpdateDoc);

    bool IsDiffFiles(const TCHAR* szSrcName, 
                     const TCHAR* szDstName);

    static bool Signing(TiXmlDocument& xmlDoc);
    static bool Verify(const TiXmlDocument& xmlDoc);

    CString GetOldMainCRC() const { return m_strOldMainCRC; }
    CString GetNewMainCRC() const { return m_strNewMainCRC; }

private:
    void EnumFile();
    static TiXmlElement* GetSignElement(const TiXmlDocument& xmlDoc);
    static const std::string GetFileListStr(const TiXmlDocument& xmlDoc);

private:
    int             m_nDepth;
    char            m_szFileName[_MAX_FNAME]; // �ʿ���� ����
    char            m_szCurPath[_MAX_PATH];
    char            m_szCurWorkingDir[_MAX_PATH];
    
    // map
    DirMap*         m_pDirMap;
    FileMap*        m_pFileMap;
    
    // xml
    TiXmlElement*   m_pxmlSignElement;
    TiXmlElement*   m_pxmlDirElement;
    TiXmlElement*   m_pxmlFileElement;
    TiXmlDocument*  m_pxmlDoc;

    CString         m_strOldMainCRC;
    CString         m_strNewMainCRC;
};

#endif
