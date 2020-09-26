/** @file   KGCMassFileBufferManager.h
    @desc   interface of KGCMassFileBufferManager
    @author seojt@kogstudios.com
    @since  2008-10-16, 16:56
            - ���ҵ� ���ҽ� ��׶��� �ε��� �۾��� �Ϻη� KGCMassFileManager�� ����ϴ�
                �޸� �����ڸ� thread sensitive�ϰ� ��� ���� ó������ �ۼ��� �����ϴ�.
*/

#pragma once

#include "CSLock.h"
#include <vector>

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER


#include <boost/intrusive_ptr.hpp>
#include <boost/intrusive/list.hpp>


/** @class  KGCMassFileBuffer  
    @brief  KGCMassFileManager�� ���������� ����ϴ� ���� �Ŵ����� �ܺη� �����Դ�.
            �̸� �浹�� ���ϱ� ���� ���� �̸� KGCBufferManager�� KGCMassFileBuffer�� �ٲپ���.

    @desc   KGCMassFileBuffer�� ���ϴ� ũ���� heap �޸𸮸� ��� ���� ����Ѵ�.

            KGCMassFileBuffer�� ���������� critical section�� ������� �ʴ´�. �̰���
            KGCMassFileBufferManager�� ����� ���۸� ������ ��, critical section�� ����ϸ�,
            �̷��� ������ ���۴� �ϳ��� thread���� ���ȴٴ� ������ �Ǳ� �����̴�.

    @date   jintaeks on 2008-10-16, 16:59 */


class KGCMassFileBuffer;
namespace boost
{
    void intrusive_ptr_add_ref(KGCMassFileBuffer * p);
    void intrusive_ptr_release(KGCMassFileBuffer * p);
}


class KGCMassFileBufferManager;

class KGCMassFileBuffer : public boost::intrusive::list_base_hook<>
{
public:
    /// constructor.



    /// �޸� ����Ʈ ũ�Ⱑ iSize�� �޸� ��Ͽ� ���� ���� �ּҸ� ��´�.
    /// ���� ������ ũ�Ⱑ iSize���� �۴ٸ�, �Լ� ���ο��� ������ ũ��� �ڵ����� Ŀ����.
    /// @return int: buffer size in bytes

    char*   GetBuffer()     { return m_pData + m_iOffset; }


	int		GetMemorySize() { return m_iMemorySize; }
	int		GetOffset()		{ return m_iOffset; }
	char*	GetMemoryBuffer() { return m_pData; }
	char*	DetachMemoryBuffer();


private:

    friend class    KGCMassFileBufferManager;
	friend void ::boost::intrusive_ptr_add_ref(KGCMassFileBuffer * p);
	friend void ::boost::intrusive_ptr_release(KGCMassFileBuffer * p);

    KGCMassFileBuffer( int iSize, KGCMassFileBufferManager* pManager = NULL, int iBucketIndex = -1 );

    /// destructor.
    ~KGCMassFileBuffer();

    void    AddRef();
    void    Release();
    LONG    GetRefCount() const { return m_nRefCount; }

    KGCMassFileBufferManager*   GetManager() const { return m_pManager; }
    int     GetBucketIndex() const          { return m_iBucketIndex; }
    int     GetOffset() const               { return m_iOffset; }
    void    SetOffset( int iOffset )        { if ( iOffset >= 0 && iOffset < m_iMemorySize ) m_iOffset = iOffset; }

private:

    volatile LONG               m_nRefCount;
    char*						m_pData;
    int                         m_iMemorySize;  ///< buffer size in bytes

    KGCMassFileBufferManager*   m_pManager;
    int                         m_iBucketIndex;
    int                         m_iOffset;


};//class KGCMassFileBuffer

inline void ::boost::intrusive_ptr_add_ref(KGCMassFileBuffer * p)
{
    if ( p != NULL )
        p->AddRef();
}//
inline void ::boost::intrusive_ptr_release(KGCMassFileBuffer * p)
{
    if ( p != NULL )
        p->Release();
}//


/** boost::shared_ptr<>�� �����Ǵ� �����ʹ� reference counter�� ����Ͽ�
    smart pointer�̴�. KGCMassFileBufferManager�� KGCMassFileBufferPtr�� �����ϸ�
    KGCMassFileBufferPtr�� ���Ϲ��� ������ reference counter�� 2�� �Ǹ�(KGCMassFileBufferManager��
    �����ϹǷ� +1, ���Ϲ޴� ���� ����Ǿ����Ƿ� +1), �̷��� reference counter�� >= 2��
    ���۴� �̹� ����ϰ� �ִٰ� �Ǵ��Ѵ�. KGCMassFileBufferPtr�� block scope�� ���� �� ��
    reference counter�� �ڵ����� �����ϸ�, reference counter�� 1�� ���۴� �ٽ� �������
    �ȴ�. - jintaeks on 2008-10-18, 11:49 */
typedef boost::intrusive_ptr<KGCMassFileBuffer> KGCMassFileBufferPtr;


/** @class  KGCMassFileBufferManager
    @brief  KGCMassFileManager�� ����ϴ� ���۸� �����Ѵ�.
            ���� ��� ������ KGCMassFileBuffer�� ã�Ƽ� �����Ѵ�.

    @desc   ���� �Ҵ�� ��� ���۰� ���ǰ� ������ ���ο� KGCMassFileBuffer�� �����Ѵ�.
            ���� - jintaeks on 2008-10-18, 11:50 - ���ҵ��� single thread���� ����ϰ�
            �ִ� ������ ���� ��� 5�� ����.

    @date   jintaeks on 2008-10-16, 17:10 */
class KGCMassFileBufferManager
{
public:
    /// constructor.
    KGCMassFileBufferManager();
    /// destructor.
    ~KGCMassFileBufferManager();

    /// ��� ������ ���ο� �ϳ��� KGCMassFileBuffer��ü�� ��´�.
    /// @note   KGCMassFileBufferPtr�� ���� ���� ������ �� ���� �����ؼ� ����ؾ� �Ѵ�.
    ///         �׷��� GetBuffer()�� ȣ���� �ʿ��� reference counter ���� 2�� �ȴ�.
    KGCMassFileBufferPtr    GetBuffer( int iSize );

    //void                    OnFrameMove( float fElapsedTime );
    void                    ReturnBuffer( KGCMassFileBuffer* pBuffer );
	void					ReleaseAllBuffers();

private:

    typedef boost::intrusive::list<KGCMassFileBuffer>   BUFFER_LIST;
	static const unsigned int BUCKET_NUMS = 23;

    struct      BufferBucket
    {
        MemberCriticalSection   m_cs;
        BUFFER_LIST             m_listBuffer;
    };//struct      BufferBucket

    boost::array< BufferBucket, BUCKET_NUMS >   
                                m_aBuckets;

    //float                       m_fTimer;
    //int                         m_iFlushBucket;


};//class KGCMassFileBufferManager






//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//
//#include <boost/shared_ptr.hpp>
//
//
//
//
///** @class  KGCMassFileBuffer  
//    @brief  KGCMassFileManager�� ���������� ����ϴ� ���� �Ŵ����� �ܺη� �����Դ�.
//            �̸� �浹�� ���ϱ� ���� ���� �̸� KGCBufferManager�� KGCMassFileBuffer�� �ٲپ���.
//
//    @desc   KGCMassFileBuffer�� ���ϴ� ũ���� heap �޸𸮸� ��� ���� ����Ѵ�.
//
//            KGCMassFileBuffer�� ���������� critical section�� ������� �ʴ´�. �̰���
//            KGCMassFileBufferManager�� ����� ���۸� ������ ��, critical section�� ����ϸ�,
//            �̷��� ������ ���۴� �ϳ��� thread���� ���ȴٴ� ������ �Ǳ� �����̴�.
//
//    @date   jintaeks on 2008-10-16, 16:59 */
//
//
//
//class KGCMassFileBuffer
//{
//public:
//    /// constructor.
//    KGCMassFileBuffer();
//    /// destructor.
//    ~KGCMassFileBuffer();
//    /// �޸� ����Ʈ ũ�Ⱑ iSize�� �޸� ��Ͽ� ���� ���� �ּҸ� ��´�.
//    /// ���� ������ ũ�Ⱑ iSize���� �۴ٸ�, �Լ� ���ο��� ������ ũ��� �ڵ����� Ŀ����.
//    char* GetBuffer(int iSize);
//    /// @return int: buffer size in bytes
//    int   GetBufferSize() { return m_iMemorySize; }
//    /// ���۸� ��� �����.
//    void  Flush();
//
//private:
//    int     m_iMemorySize;  ///< buffer size in bytes
//    char*   m_pData;        ///< start address of buffer
//    // KGCMassFileBuffer�� �ϳ��� instance�� ���� ����� �ϳ��� thread�� �����ؼ� ����Ѵٰ�
//    // �����ϹǷ�, ������ critical section�� �ʿ����.
//    //MemberCriticalSection   m_cs;
//};//class KGCMassFileBuffer
//
//
///** boost::shared_ptr<>�� �����Ǵ� �����ʹ� reference counter�� ����Ͽ�
//    smart pointer�̴�. KGCMassFileBufferManager�� KGCMassFileBufferPtr�� �����ϸ�
//    KGCMassFileBufferPtr�� ���Ϲ��� ������ reference counter�� 2�� �Ǹ�(KGCMassFileBufferManager��
//    �����ϹǷ� +1, ���Ϲ޴� ���� ����Ǿ����Ƿ� +1), �̷��� reference counter�� >= 2��
//    ���۴� �̹� ����ϰ� �ִٰ� �Ǵ��Ѵ�. KGCMassFileBufferPtr�� block scope�� ���� �� ��
//    reference counter�� �ڵ����� �����ϸ�, reference counter�� 1�� ���۴� �ٽ� �������
//    �ȴ�. - jintaeks on 2008-10-18, 11:49 */
//typedef boost::shared_ptr<KGCMassFileBuffer>    KGCMassFileBufferPtr;
//
//
///** @class  KGCMassFileBufferManager
//    @brief  KGCMassFileManager�� ����ϴ� ���۸� �����Ѵ�.
//            ���� ��� ������ KGCMassFileBuffer�� ã�Ƽ� �����Ѵ�.
//
//    @desc   ���� �Ҵ�� ��� ���۰� ���ǰ� ������ ���ο� KGCMassFileBuffer�� �����Ѵ�.
//            ���� - jintaeks on 2008-10-18, 11:50 - ���ҵ��� single thread���� ����ϰ�
//            �ִ� ������ ���� ��� 5�� ����.
//
//    @date   jintaeks on 2008-10-16, 17:10 */
//class KGCMassFileBufferManager
//{
//    typedef std::vector<KGCMassFileBufferPtr>   BUFFER_VECTOR;
//
//private:
//    BUFFER_VECTOR               m_vecBuffers;
//    /// ���� �ֱٿ� ����� ������ index�̴�. ���� ��� ������ ���۸� ������ ã�� ���� ����Ѵ�.
//    size_t                      m_uiCursor;
//    // KGCMassFileBufferManager ��ü�� ���� thread�� �������� �����Ƿ�, critical section��
//    // �ʿ����. - jintaeks on 2008-10-16, 19:46
//    MemberCriticalSection       m_csVecBuffers; ///< m_mapBuffers�� thread safe�ϰ� �����ϱ� ���� ����Ѵ�.
//
//public:
//    /// constructor.
//    KGCMassFileBufferManager();
//    /// destructor.
//    ~KGCMassFileBufferManager();
//
//    /// ��� ������ ���ο� �ϳ��� KGCMassFileBuffer��ü�� ��´�.
//    /// @note   KGCMassFileBufferPtr�� ���� ���� ������ �� ���� �����ؼ� ����ؾ� �Ѵ�.
//    ///         �׷��� GetBuffer()�� ȣ���� �ʿ��� reference counter ���� 2�� �ȴ�.
//    KGCMassFileBufferPtr    GetBuffer();
//    /// ���� �Ҵ�� ������ ������ �����Ѵ�.
//    size_t                  GetNumBuffers() const { return m_vecBuffers.size(); }
//};//class KGCMassFileBufferManager
//
//
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER


/** @example    KGCMassFileBufferManager

    #include "AssertTrace.h"
    #include <string>
    #include <boost/shared_ptr.hpp>
    #include "KGCMassFileBufferManager.h"


    KGCMassFileBufferManager    g_bufMan;


    void TestInner( KGCMassFileBufferPtr p )
    {
        printf( "use_count = %i\r\n", p.use_count() );
    }//TestInner()

    void Test()
    {
        KGCMassFileBufferPtr p = g_bufMan.GetBuffer();
        printf( "use_count = %i\r\n", p.use_count() );
        KGCMassFileBufferPtr p2 = g_bufMan.GetBuffer();
        TestInner( p2 );
        printf( "use_count = %i\r\n", p.use_count() );

        printf( "NumBuffers = %i\r\n", g_bufMan.GetNumBuffers() );
    }//Test()


    void main()
    {
        Test();
        Test();

        // output:
        //use_count = 2
        //use_count = 3
        //use_count = 2
        //NumBuffers = 2
        //use_count = 2
        //use_count = 3
        //use_count = 2
        //NumBuffers = 2
        //Press any key to continue
    }//main()
*/
