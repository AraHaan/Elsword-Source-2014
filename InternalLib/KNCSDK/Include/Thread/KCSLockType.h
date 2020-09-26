/* @file   KCSLockType.h
    @brief  syntactic safeguard for data in a critical section.
    @author robobeg@kogstudios.com
    @since  2009-1-12, 09:00
*/

#ifndef _KCSLOCKTYPE_H
#define _KCSLOCKTYPE_H

#include    <boost/noncopyable.hpp>
#include    <boost/static_assert.hpp>
#include    <boost/type_traits/is_const.hpp>
#include    <boost/type_traits/is_reference.hpp>
#include    <boost/type_traits/add_const.hpp>
#include    <boost/type_traits/add_reference.hpp>
#include    <boost/type_traits/remove_const.hpp>
#include    <boost/type_traits/remove_reference.hpp>
#include    <windows.h>


template<typename T>
class KCSLockType: private boost::noncopyable
{
public:

    BOOST_STATIC_ASSERT( !boost::is_const<T>::value );
    BOOST_STATIC_ASSERT( !boost::is_reference<T>::value );

    typedef T              TYPE;

    template<bool bIsConst>
    class   KCSLockedAccess;

    template<>
    class   KCSLockedAccess<false>
    {
    public:
        
        KCSLockedAccess( KCSLockType& kIn_ )
            : m_cs( kIn_.m_cs ), m_variable( kIn_.m_variable )
        {
            ::EnterCriticalSection( &m_cs );
        }//KCSLockedAccess()

        ~KCSLockedAccess()
        {
            ::LeaveCriticalSection( &m_cs );
        }//~KCSLockedAccess()

        T&                      AccessVariable() { return m_variable; }
    private:
        CRITICAL_SECTION&       m_cs;
        T&                      m_variable;    
    };//

    template<>
    class   KCSLockedAccess<true>
    {
    public:

        KCSLockedAccess( const KCSLockType& kIn_ )
            : m_cs( kIn_.m_cs ), m_variable( kIn_.m_variable )
        {
            ::EnterCriticalSection( &m_cs );
        }//KCSLockedAccess()

        ~KCSLockedAccess()
        {
            ::LeaveCriticalSection( &m_cs );
        }//~KCSLockedAccess()

        const T&                AccessVariable() const { return m_variable; }
    private:
        mutable CRITICAL_SECTION&       m_cs;
        const T&                m_variable;    
    };//

    KCSLockType()           { ::InitializeCriticalSection( &m_cs ); }
    KCSLockType( const T& kIn_ ) : m_variable( kIn_ ) { ::InitializeCriticalSection( &m_cs ); }
    ~KCSLockType()          { ::DeleteCriticalSection( &m_cs ); }

private:
    mutable CRITICAL_SECTION       m_cs;
    T                       m_variable;
};//KCSLockType

template<bool C, typename A, typename B> 
struct _kcslock_if_ {};
template<typename A, typename B> 
struct _kcslock_if_<true, A, B> { typedef A type; };
template<typename A, typename B> 
struct _kcslock_if_<false, A, B> { typedef B type; };

#define _KCSLOCK_TYPEDEF_(_type_)            \
    _kcslock_if_< boost::is_reference< _type_ >::value \
        , boost::add_reference< KCSLockType<boost::remove_reference< _type_ >::type> >::type \
        , KCSLockType<boost::remove_reference< _type_ >::type> >::type



/* @def   KCSLOCK_TYPE(_variable_)
    @brief  \a _variable_ �� \c KCSLockType
*/
#define KCSLOCK_TYPE(_variable_)            __KCSLockType__##_variable_


/* @def   KCSLOCK_VARIABLE_TYPE(_variable_)
    @brief \a _variable_ �� type
*/
#define KCSLOCK_VARIABLE_TYPE(_variable_)   boost::remove_reference<KCSLOCK_TYPE(_variable_)>::type::TYPE


/* @def   KCSLOCK_DECLARE(_type_,_variable_)
    @brief type \a _type_ �� \a _variable_ �� \c KCSLockType ���� �����Ѵ�.

    ��)
    @code
        KCSLOCK_DECLARE( std::set<int>, m_set )     // KCSLockType< std::set<int> > Ÿ���� ���� m_set ����
        KCSLOCK_DECLARE( int&, m_refInt )           // KCSLockType< int >& Ÿ���� ���� m_refInt ����
                                                    // ����! KCSLockType< int& > Ÿ���� ���� ������ �ƴϴ�.
        static KCSLOCK_DECLARE( bool, ms_bInitialized )   // KCSLockType< bool > Ÿ���� static ���� ms_bInitialized ����
    @endcode
*/
#define KCSLOCK_DECLARE(_type_,_variable_)  _KCSLOCK_TYPEDEF_(_type_) (_variable_);  typedef _KCSLOCK_TYPEDEF_(_type_) KCSLOCK_TYPE(_variable_);


/* @def  KCSLOCK_IMPLEMENT(_classname_,_variable_)
    @brief static KCSLOCK_DECLARE( ... ) ���·� �����ϸ� static member variable �� �ȴ�. �̿� ���� implementation�� ���� ���ȴ�.
    ��)
    @code
        KCSLOCK_IMPLEMENT( KClassName, ms_variable );
    Ȥ��
        KCSLOCK_IMPLEMENT( KClassName, ms_variable ) = ... ; // �ʱ�ȭ
    @endcode
*/
#define KCSLOCK_IMPLEMENT(_classname_,_variable_)   _classname_::KCSLOCK_TYPE(_variable_) _classname_::_variable_



/* @def KCSLOCK_BEGIN(_variable_)
    @brief non-const ��� �Լ����� ��� ������ read/write ������ �� ���
*/
#define KCSLOCK_BEGIN(_variable_)  { boost::remove_reference<KCSLOCK_TYPE(_variable_)>::type::KCSLockedAccess<false> _kcsLockedAccess_(_variable_); \
    boost::add_reference<KCSLOCK_VARIABLE_TYPE(_variable_)>::type (_variable_)( _kcsLockedAccess_.AccessVariable() );


/* @def KCSLOCK_CBEGIN(_variable_)
    @brief const ��� �Լ� �������� ��� ������ read�� �� �� ���
*/
#define KCSLOCK_CBEGIN(_variable_)  { boost::add_const<boost::remove_reference<KCSLOCK_TYPE(_variable_)>::type::KCSLockedAccess<true> >::type _kcsLockedAccess_(_variable_); \
    boost::add_reference<boost::add_const<KCSLOCK_VARIABLE_TYPE(_variable_)>::type>::type (_variable_)( _kcsLockedAccess_.AccessVariable() );


/* @def KCSLOCK_END()
    @brief \c KCSLOCK_BEGIN �Ǵ� \c KCSLOCK_CBEGIN ���� ������ �� ���ȴ�.
*/
#define KCSLOCK_END()     }


/* @define KCOMMA()
    @brief ��ũ�� �Ķ���� �ȿ��� ',' ��� ��� 
    @code std::map<int, int> @endcode �� ���� ','�� ���Ե� Ÿ���� ��ũ�� �Ķ���ͷ� �ٷ� �ѱ� �� ����. �̷� ���
    @code std::map<int KCOMMA() int> @endcode �� ���� �Ѱ��ش�. �̺��� �� ���� ����� 
    @code typedef std::map<int, int> KMapIntInt; @endcode �� Ÿ���� ������ �� \c KMapIntInt �� �ѱ�� ���̴�.
*/
#define KCOMMA()   ,


/* @def KCSLOCK_SET_VALUE(_variable_)
    @brief non-const ��� �Լ����� ��� ������ � ���� �����ϰ� �����Ϸ��Ҷ� ���
*/
#define KCSLOCK_SET_VALUE(_variable_, value)   KCSLOCK_BEGIN(_variable_) _variable_ = value; KCSLOCK_END()


/* @example KCSLockType.h

1. Ŭ���� �������� KCSLockType ���� ����

@code

#include <algorithm>
#include <vector>
#include "KCSLockType.h"


class   KMainThread
{
public:

    void    AccessKCSLock();
    void    ConstAccessKCSLock() const;
    void    PassToSubThread();

private:

    KCSLOCK_DECLARE( std::vector<int>, m_vecInt );  
    // KCSLockType< std::vector<int> > Ÿ���� ���� m_vecInt �� ����

};//class   KMainThread

@endcode


2. KCSLockType ���� ����

@code 

void    KMainThread::AccessKCSLock()
{
    KCSLOCK_BEGIN( m_vecInt )       // BEGIN, END �� �������� �����ο� ������ �����ϴ�.
        m_vecInt.push_back( 1 );
        m_vecInt.push_back( 2 );
        m_vecInt.push_back( 3 );
    KCSLOCK_END()

    // m_vecInt.push_back( 4 );     // BEGIN, END �� �ۿ��� �����Ϸ� �ϸ� compile-time error �� �߻��Ѵ�.
}//KMainThread::AccessKCSLock()


void    KMainThread::ConstAccessKCSLock() const
{
    KCSLOCK_CBEGIN( m_vecInt )   // const �Լ� �������� KCSLOCK_CBEGIN�� ����Ͽ� �����Ѵ�.

        KCSLOCK_VARIABLE_TYPE( m_vecInt )::const_iterator iter 
            = std::find( m_vecInt.begin(), m_vecInt.end(), 3 );
        if ( iter == m_vecInt.end() )
        {
            return; // BEGIN, END �� ������ �������� �� continue, break, return, goto�� ����ص� �ȴ�.
        }//if
        //...

    KCSLOCK_END()
}//KMainThread::ConstAccessKCSLock()

@endcode

3. KCSLockType ������ reference ����

@code

class   KSubThread
{
private:

    KCSLOCK_DECLARE( std::vector<int>&, m_refVecInt );    
    // std::vector<int>& Ÿ���� �ƴ϶� KCSLockType< std::vector<int> >& m_refVecInt �� �����ϴ� ����.

public:

    // KCSLockType�� reference�� �Ķ���ͷ� �޴´�. 
    // KSubThread( KCSLockType< std::vector<int> >& refIn_ ); �� �ǹ̰� ����.
    KSubThread( KCSLOCK_TYPE( m_refVecInt ) refIn_ );

    void    AccessKCSLock();    
};//class   KSubThread


KSubThread::KSubThread( KSubThread::KCSLOCK_TYPE( m_refVecInt ) refIn_ )
    : m_refVecInt( refIn_ )
{
}//KSubThread::KSubThread()


void    KMainThread::PassToSubThread()
{
    KSubThread  kSubThread( m_vecInt );  // KCSLockType ������ �ٸ� Ŭ������ reference�� ������ ���� BEGIN/END ���� ������� �ʴ´�.
    //...
}//KMainThread::PassToSubThread()


void    KSubThread::AccessKCSLock()
{
    KCSLOCK_BEGIN( m_refVecInt )
        // reference ���� ���� ����� �����ϴ�.
    KCSLOCK_END()
}//KSubThread::AccessKCSLock()

@endcode

*/


#endif // #ifndef _KCSLOCKTYPE_H

