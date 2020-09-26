/** @file   KFileVerifier.h
    @desc   interface of KFileVerifier
            ��׶���� file���� checksum�� �˻��Ѵ�.
            ���ҵ� ������ �ӵ� ������ �����ֱ� ���� �������� ó������ �ۼ��ϴ�.
    @author seojt@kogstudios.com
    @since  2008-1-5, 12:12

    @date   jintaeks on 2008-01-05, 15:01
            - ��������
            - ���� �Ϸ�
                --  thread���� checksum�� �˻��ϰ�, hack ����Ʈ�� ����� �� ���� ����
    @date   jintaeks on 2008-01-06, 18:27
            -   checksum �˻簡 ������ ���� �ٸ� ��ũ IO�۾��� ���۵� �� �����Ƿ�,
                thread�� suspend/resume ��Ű�� ����� �߰��ϴ� �۾��� ����
*/

#ifndef _KFILEVERIFIER_H
#define _KFILEVERIFIER_H

//#include "AssertTrace.h"
#include <string>
#include <map>
#include <vector>
#include "KJohnThread.h"
#include "CSLock.h"

//{{ robobeg : 2008-05-02
#include "KStringUtil.h"
//}} robobeg : 2008-05-02


///-----------------------------------------------------------------------------
/// @class  KFileInfo
/// @brief  KFileVerifier�� ó���ϴ� �ϳ��� ���Ͽ� ���� ������ ��Ÿ����,
///         std::map<>�� ���� ����Ѵ�.
///
struct KFileInfo
{
public:
    std::string     m_strFilename;  ///< ���� �̸�
    DWORD           m_dwSize;       ///< ���� ũ��(����: byte)
    USHORT          m_usVersion;    ///< ���� ����
    std::string     m_strCheckSum;  ///< ���� binary�� checksum

public:
    /// constructor.
    KFileInfo()
    {
        m_strFilename.clear();
        m_dwSize        = 0L;
        m_usVersion     = 0;
        m_strCheckSum.clear();
    }//KFileInfo()

    /** KFileInfo����ü�� std::map<>�� ���� insert�ǹǷ�
        ���� �����ڿ� operator=()�� overload�� �־�� �Ѵ�. */

    /// copy constructor.
    KFileInfo(const KFileInfo& rhs_)
    {
        m_strFilename   = rhs_.m_strFilename;
        m_dwSize        = rhs_.m_dwSize;
        m_usVersion     = rhs_.m_usVersion;
        m_strCheckSum   = rhs_.m_strCheckSum;
    }//KFileInfo()

    KFileInfo& operator=(const KFileInfo& rhs_)
    {
        m_strFilename   = rhs_.m_strFilename;
        m_dwSize        = rhs_.m_dwSize;
        m_usVersion     = rhs_.m_usVersion;
        m_strCheckSum   = rhs_.m_strCheckSum;

        return *this;
    }//operator=()
};//struct KFileInfo

//{{ robobeg : 2008-05-02
// �ʿ��� ���ϸ��� ��ҹ��� ���о��� ���ϱ� ���� ����

//typedef std::map<std::string, KFileInfo>    FileInfoMap;
typedef std::map<std::string, KFileInfo, KCaseInsensitiveLessA>    FileInfoMap;
//}} robobeg : 2008-05-02
typedef std::vector<KFileInfo>              FileInfoArr;


// thread�� �����ϱ� ������ ȣ���ϴ� ����� �Լ��� protype�� ���� Ÿ�� ����
typedef void (*PREENDTHREAD_CALLBACK)(DWORD dwUserParam_, void* pFileVerifierObject_);


/** @class  KFileVerifier
    @brief  ��׶���� file���� checksum�� �˻��Ѵ�.
            ���ҵ� ������ �ӵ� ������ �����ֱ� ���� ������seojt@kogstudios.com�� ó������ �ۼ��ϴ�.
            - jintaeks on 2008-01-05, 12:21
    @see    CKTDXThread, tinyxml.h, CRC_32.h|cpp

    @desc   [��ü���� ���� ���� �� ��ȹ] - jintaeks on 2008-01-05, 12:20

            1)  ���� ���� �������� Ver.xml ������ �о checksum�� �˻��� ���ϵ��� ����Ʈ�� ��
                ���� �����.
            2)  �α��� �� ó�� �ѹ� ���ҽ��� �ε��ϴ� ���� Ȥ�� �ε尡 ���� ��,
                verifier thread�� �����Ѵ�.
                    -   thread ������ KTDSLIB�� ���Ե� CKTDXThread Ŭ����(KTDXThread.h|cpp)��
                        ����Ѵ�.
                (������ �����Ƿ� CPU bound������ ������, ���������� frame�� ������ ���̴�)
            3)  verify�� �����ϸ� ������ �޽����� ������ verify�� ���������� �˸���.
            4)  3) ������ ������ ���Ͽ� ���ؼ��� �����ϹǷ�, .kom�� ����� ���ϵ��� checksum
                �˻縦 ���� ������ ���� ����Ʈ�� �����Ѵ�.
                �� ���ϵ��� .kom���� ������ �б� ����, ó�� ������ �� checksum �˻縦 �� �־��
                �ϴµ�, ó�� �����ϴ� ������ Ư���� �ڵ带 ������ ��, ���� �Լ��� hook�ؼ�
                �ڵ����� ó�������� ������̴�.

            [���� �� ���� ����]

*/
class KFileVerifier : public KJohnThread
{
private:
    char                    m_szProgramPath[1024];  ///< ���� client�� ��ġ�� ���� ���. ���� \\�� ����
    FileInfoMap             m_fileMap;              ///< ������ <File> ���ǿ� ��õ� ������ ����Ʈ
    /** .kom ���� ��ü�� ���ؼ� checksum�� ���ϴ� ���� ȿ�������� �ʴٰ� �ǴܵǸ�,
        .kom ���Ͽ� package�� ���Ͽ� ���ؼ� checksum �񱳸� �õ��ؾ� �ϴµ�,
        �׷��� ������ Ver.xml�� ������ ���ǿ� �־�ΰ�,
        thread���� .kom ������ unpack�Ͽ� checksum �˻縦 �ϵ��� ���� �����ؾ� �Ѵ�.

        @note   ���� - jintaeks on 2008-01-06, 18:25 - ������� �ʴ´�.

        - jintaeks on 2008-01-06, 18:24 */
    FileInfoMap             m_komFileMap;           ///< ���Ӱ� <KomFile> ���ǿ� ��õ� ������ ����Ʈ

    /** thread�� ����Ǿ��� �� m_hackFileArr�� �ִ� ���ϵ��� checksum�� ��ġ���� �ʴ� hack��
        ������ ���̴�. */
    FileInfoArr             m_hackFileArr;          ///< ��ŷ��(checksum�� ��ġ���� �ʴ�) ���� �迭

    // thread ���� --------------------------------------------------
    //
    HANDLE                  m_hThreadEvent;         ///< thread�� suspend/resume�� ���� event
    bool                    m_bThreadSuspended;
    MemberCriticalSection   m_cs;                   ///< CRITICAL_SECTION�� wrapper
    DWORD                   m_dwThreadUserParam;    ///< 'm_fpPreEndThreadCallback'�� ù���� �Ķ����
    PREENDTHREAD_CALLBACK   m_fpPreEndThreadCallback;   ///< thread�� �����ϱ� ������ ȣ���ϴ� ����� ���� �ݹ� �Լ�
    bool                    m_bForceExistThread;    ///< thread�� ������ ������Ѿ� �ϴ� ��� �� ���� ������ �����.
        /// checksum �˻簡 �Ϸ����� �ʾ�����, ����ڰ� ������ ���������� ��� ����Ѵ�.

public:
    /// constructor.
    KFileVerifier();
    /// destructor.
    virtual ~KFileVerifier();

    ///-----------------------------------------------------------------------------
    // thread related functions
    //
public:
    /// @note   override KJohnThread::RunThread()
    /// ���� thread �Լ��̴�. 'm_fileMap'�� ������ ������ checksum�� �˻��ϰ�,
    /// ����� 'm_hackFileArr'�� �����Ѵ�. thread�� �����ϱ� ������,
    /// 'm_fpPreEndThreadCallback' �ݹ��Լ��� ȣ���Ѵ�.
    virtual DWORD RunThread();

    /// end thread �ݹ��Լ��� �����ϰ�, thread�� �����Ѵ�.
    /// @param  pPreEndThreadCallback_: [in] RunThread() �� �����ϱ� ������ ȣ���ϴ� ����� �Լ�
    /// @param  dwUserParam_: [in] pPreEndThreadCallback_�� ù��° �Ķ���ͷ� ���޵Ǵ� ��
    /// @return bool: 
    bool BeginThreadWithPreEndCB(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

    /// thread�� �ʿ��� event ��ü�� �����Ѵ�.
    void _InitializeObject();

    /// thread�� ���� event ��ü�� �ı��Ѵ�.
    void _FinalizeObject();

    /// thread�� �����ϱ� ������ ȣ���ϴ� ����� ���� �ݹ� �Լ��� �����Ѵ�.
    /// @param  dwUserParam_: pPreEndThreadCallback_()�Լ��� ù��° �Ķ���ͷ� ���޵Ǵ� custom ��
    /// @note   pPreEndThreadCallback_�� �ι��� �Ķ���ʹ� �� �Լ��� ȣ���� ��ü�� ���� �ּ��̴�.
    ///         �� this�� ���޵ȴ�.
    void _SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

    /// thread�� suspend ��Ų��.
    void SuspendThread();

    /// thread�� resume��Ų��.
    void ResumeThread();

    /// thread�� suspend�Ǿ� �ִ� ��� true�� �����Ѵ�.
    bool IsThreadSuspended() const { return m_bThreadSuspended; }

    ///-----------------------------------------------------------------------------
    // KFileVerifier interfaces
    //
public:
    /// Ver.xml ���Ͽ��� ���� ����Ʈ�� �о �ʿ� �����Ѵ�.
    /// @param  pszFullPathFilename_: ��θ� ������ Ver.xml ���� �̸�
    /// @return bool: �����ϸ� true�� �����Ѵ�.
    ///                 �����ϸ� false�� �����Ѵ�.
    bool LoadFileList(const char* pszFullPathFilename_);

    /// �����̳��� ������ ��� clear�Ѵ�.
    void Clear();

    /// hack�� ����(checksum�� ��ġ���� �ʴ� ����)�� ������ �����Ѵ�.
    inline int GetNumHackFiles() const { return (int)m_hackFileArr.size(); }

    ///-----------------------------------------------------------------------------
    // debug functions
    //
public:
    /// container�� ������ ����� ȭ�鿡 ����Ѵ�.
    void Debug_DumpToOutputWindow();

    /// 'm_hackFileArr'�� ������ ����� �����Ѵ�.
    void Debug_DumpHackFiles();
};//class KFileVerifier

#endif // _KFILEVERIFIER_H


/** @example    class KFileVerifier

  1) checksum�� ��� �˻��ϰ� thread�� �����ϱ� ������ callback �Լ� �ۼ�

    void PreEndThreadCallback(DWORD dwUserParam_, void* pFileVerifierObject_)
    {
        ASSERT( pFileVerifierObject_ );
        KFileVerifier*  pFileVerifier = reinterpret_cast<KFileVerifier*>( pFileVerifierObject_ );
        ASSERT( pFileVerifier );
        pFileVerifier->Debug_DumpHackFiles();
    }//PreEndThreadCallback()

  2) CKTDXApp�� �����ڿ��� ��ü�� �����, thread ����

    m_pFileVerifier     = new KFileVerifier();
    ASSERT( m_pFileVerifier );
    m_pFileVerifier->LoadFileList( "Ver.xml" );
    m_pFileVerifier->Debug_DumpToOutputWindow();
    m_pFileVerifier->_SetPreEndThreadCallback( PreEndThreadCallback );
    m_pFileVerifier->BeginThread();

  3) CKTDXApp�� �ı��ڿ��� ��ü �ı�

    ASSERT( m_pFileVerifier );
    if ( m_pFileVerifier )
    {
        m_pFileVerifier->EndThread( 1000 );
        SAFE_DELETE( m_pFileVerifier );
    }//if

*/
