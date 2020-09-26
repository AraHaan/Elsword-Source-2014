#if !defined(_KJohnStdVector_Defined_)
#define _KJohnStdVector_Defined_

#include <vector>
//#include "KJohnGen.h"


template<class _Ty, int INITIAL_SIZE = 64, class _A = std::allocator<_Ty> >
class KJohnStdVector : public std::vector<_Ty, _A>
{
public:
    /// constructor.
    KJohnStdVector() { reserve(INITIAL_SIZE); }
    //~KJohnStdVector() {}

    bool Find(const _Ty& t)
    {
        iterator itor = begin();
        while ( itor != end() )
        {
            // @note    thers must be exist 't.operator==()'
            if ( t == (*itor) )
                return true;

            ++itor;
        }//while

        return false;
    }//Find()

    bool InsertIfNotExist(const _Ty& t)
    {
        if ( false == Find(t) )
        {
            push_back( t );
            return true;
        }//if

        return false;
    }//InsertIfNotExist()
};//class KJohnStdVector<>

#endif // !defined(_KJohnStdVector_Defined_)


/** @example    class KJohnStdVector<>

  1) test case for std::vector<>

    #include <windows.h>
    #include <stdio.h>
    #include <cassert>
    #include <string.h>
    #include <iostream>
    #include <vector>
    #include "KJohnStdVector.h"
    #include "Win32Alloc.h"

    typedef std::vector<int, Win32Alloc<int> >              TVECTOR;
    typedef std::vector<int, Win32Alloc<int> >::iterator    TVECTOR_ITOR;

    void TestVector(TVECTOR& vecTest)
    {
        std::cout << "size=" << vecTest.size() << std::endl;
        for (TVECTOR_ITOR itor=vecTest.begin(); itor != vecTest.end(); ++itor)
        {
            std::cout << (*itor) << std::endl;
        }//for
    }//TestVector()

    void main()
    {
        TVECTOR         vecTest;

        //vecTest.reserve( 8 );
        for (int i=0; i < 9; ++i)
        {
            vecTest.push_back( i );
        }//for

        TestVector( vecTest );

        vecTest.clear();
    }//main()

//    9���� push_back()�� ��� std::vector<>�� ���� �޸� �Ҵ�
//    i=0, 1�� �Ҵ�
//    i=1, 2�� �Ҵ�
//    i=2, 4�� �Ҵ�
//    i=4, 8�� �Ҵ�
//    i=8, 16�� �Ҵ�
//    �� 5���� �޸� �Ҵ�


  2) test case for KJohnStdVector<>

    #include <windows.h>
    #include <stdio.h>
    #include <cassert>
    #include <string.h>
    #include <iostream>
    #include <vector>
    #include "KJohnStdVector.h"
    #include "Win32Alloc.h"

    typedef KJohnStdVector<int, 8, Win32Alloc<int> >            TVECTOR;
    typedef KJohnStdVector<int, 8, Win32Alloc<int> >::iterator  TVECTOR_ITOR;

    void TestVector(TVECTOR& vecTest)
    {
        std::cout << "size=" << vecTest.size() << std::endl;
        for (TVECTOR_ITOR itor=vecTest.begin(); itor != vecTest.end(); ++itor)
        {
            std::cout << (*itor) << std::endl;
        }//for
    }//TestVector()

    void main()
    {
        TVECTOR         vecTest;

        for (int i=0; i < 9; ++i)
        {
            vecTest.push_back( i );
        }//for

        TestVector( vecTest );

        vecTest.clear();
    }//main()

//    for loop�� �����ϱ��� 8�� �Ҵ�
//    i=8, 16�� �Ҵ�
//    �� 2���� �޸� �Ҵ�


  3) real use when data size expected to 9

    #include <windows.h>
    #include <stdio.h>
    #include <cassert>
    #include <string.h>
    #include <iostream>
    #include <vector>
    #include "KJohnStdVector.h"

    typedef KJohnStdVector<int, 9>              TVECTOR;
    typedef KJohnStdVector<int, 9>::iterator    TVECTOR_ITOR;

    void TestVector(TVECTOR& vecTest)
    {
        std::cout << "size=" << vecTest.size() << std::endl;
        for (TVECTOR_ITOR itor=vecTest.begin(); itor != vecTest.end(); ++itor)
        {
            std::cout << (*itor) << std::endl;
        }//for
    }//TestVector()

    void main()
    {
        TVECTOR         vecTest;

        for (int i=0; i < 9; ++i)
        {
            vecTest.push_back( i );
        }//for

        TestVector( vecTest );

        vecTest.clear();
    }//main()

//    for loop�� �����ϱ��� 9�� �Ҵ�
//    �� 1���� �޸� �Ҵ�
//    ����: ���� push_back()�� ���� 17�̶��, 9��° push_back() ȣ�⶧ 18�� �Ҵ�
//    �ϹǷ� �� 2���� �޸� �Ҵ��� �Ͼ�ϴ�.

*/
