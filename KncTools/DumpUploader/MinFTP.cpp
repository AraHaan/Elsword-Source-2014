#include "StdAfx.h"
#include ".\minftp.h"
#include "Logger.h"
extern CLogger* g_Logger;
CMinFTP::CMinFTP(void)
{
    m_pFTPServerInfo = NULL;
    m_pFTPConnection = NULL;
}

CMinFTP::~CMinFTP(void)
{
    CloseFTP();
    
}

bool CMinFTP::SetFTPInfo( IElement* pElement )
{
    if( NULL != pElement && pElement->IsValid() && pElement->GetType() == IElement::ELEMENTTYPE::ELEMENTTYPE_FTPINFO )
    {
        m_pFTPServerInfo = (CElementFTPInfo *)pElement;
        return true;
    }
    return false;
}

bool CMinFTP::FTPConnection()
{
    try
    {
    
    m_pFTPConnection = m_Session.GetFtpConnection( m_pFTPServerInfo->GetFTPAddress(),
                                 m_pFTPServerInfo->GetUserID(), m_pFTPServerInfo->GetUserPW() );
    }
    catch (...) {
        
        return false;
    }
    return true;
}

bool CMinFTP::UpLoadFile( char* strFileName, char* strFolderName )
{
    BOOL bRet;
    CString strCurrentDir;
    CString strDestDir;
    CString strRemoteFullFileName;
    
    CFileStatus localFileStatus;
    bRet = CFile::GetStatus( strFileName ,localFileStatus );
    if( !bRet )
        return false;
    //����(root) ������ �˾Ƴ���.
    bRet = m_pFTPConnection->GetCurrentDirectory( strCurrentDir );
    if( !bRet )
        return false;

    //Crash������ �ִ��� Ȯ���Ѵ�.
    //������ �����
    //ũ���� ������ �̵��Ѵ�.
    strCurrentDir += "CrashDump";
    bRet = m_pFTPConnection->SetCurrentDirectory( strCurrentDir );
    if( !bRet )
    {
        bRet = m_pFTPConnection->CreateDirectory( strCurrentDir );
        if( !bRet )
            return false;
        bRet = m_pFTPConnection->SetCurrentDirectory( strCurrentDir );
        if( !bRet )
            return false;
    }//end if
    
    
    // strFileName �� �ִ��� Ȯ���Ѵ�.
    // ������ �����.
    // strFileName ������ �̵��Ѵ�.
    strCurrentDir = strCurrentDir+"/"+strFolderName;
    bRet = m_pFTPConnection->SetCurrentDirectory( strCurrentDir );
    if( !bRet )
    {
        bRet = m_pFTPConnection->CreateDirectory( strCurrentDir );
        if( !bRet )
            return false;
        bRet = m_pFTPConnection->SetCurrentDirectory( strCurrentDir );
        if( !bRet )
            return false;
    }//end if

    // zip������ ���ε� �Ѵ�.
    strRemoteFullFileName = strCurrentDir +"/"+ strFileName;
    bRet = m_pFTPConnection->PutFile( localFileStatus.m_szFullName, strRemoteFullFileName );
    if( !bRet )
        return false;


    if(g_Logger)
    {
        CStringEx strLog;
        strLog.Format("���ε� ���� : %s", (LPCSTR) strRemoteFullFileName );
        g_Logger->AddLog( strLog );
    }
    return true;
}

bool CMinFTP::CloseFTP()
{
    if( m_pFTPConnection == NULL )
    {
        //Err
        //Log
        return false;
    }//end if
    m_pFTPConnection->Close();
    return true;
}