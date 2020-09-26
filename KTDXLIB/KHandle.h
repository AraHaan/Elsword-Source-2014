/** @file   KHandle.h
    @desc   interface of KHandle
            KHandleArray<>���� ����ϴ� ����� ���� ���θ� �Ǵ��ϱ� ���� magic number�� �����ϴ�
            �ڵ鰪�� �����ϱ� ���� ó�� �����ϴ�.
    @author jintaeks@kogstudios.com
    @since  2008-3-31, 16:13
*/

#ifndef _KHandle_h
#define _KHandle_h

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#include    <lua_tinker.h>
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

//#include "KJohnGen.h"

///-----------------------------------------------------------------------------
/// @class	KHandle
/// @brief  magic number(�Ϲ������� reference counter)�� �����ϴ� �ڵ��� �����ϱ�
///         ���� �Ϲ� ������ handle wrapper�̴�.
///         KHandleArray<>�� �����ϴ� ��忡 ����, handle�� ��ȿ������ �ٸ� ������
///         ������ �߸��� ��带 �Ǵ��ϱ� ���� ó������ ����Ѵ�.
/// @date   jintaeks on 2008-3-31, 16:13
///
struct KHandle
{
public:
    enum KHANDLE_VALUES
    {
        KHandleType_Invalid_HANDLE  = -1,
    };//enum KHANDLE_VALUES

private:
    int     m_iHandle;  ///< handle to something(node or etc.)
    UINT    m_uiMagic;  ///< magic number(reference counter and etc.)

public:
    /// constructor.
    KHandle() : m_iHandle(-1), m_uiMagic(0) {}

    KHandle(int iHandle_, UINT uiMagic_)
    {
        m_iHandle = iHandle_;
        m_uiMagic = uiMagic_;
    }//KHandle()

    /// copy constructor.
    KHandle( const KHandle& rhs_ )
    {
        m_iHandle = rhs_.m_iHandle;
        m_uiMagic = rhs_.m_uiMagic;
    }//KHandle()

    void Invalidate()
    {
        m_iHandle = -1;
        m_uiMagic = 0;
    }//Invalidate()

    bool IsValid() const
    {
        return m_iHandle >= 0 && m_uiMagic > 0;
    }//IsValid()

    /// handle number part�� ��´�.
    int GetHandlePart() const { return m_iHandle; }

    /// magic number part�� ��´�.
    UINT GetMagicPart() const { return m_uiMagic; }

    /// handle number part�� �����Ѵ�.
    void SetHandlePart(int iHandle_) { m_iHandle = iHandle_; }

    /// magic number part�� �����Ѵ�.
    void SetMagicPart(UINT uiMagic_) { m_uiMagic = uiMagic_; }

    /// handle�� magic number part�� ���ÿ� �����Ѵ�.
    void SetHandleAndMagicPart(int iHandle_, UINT uiMagic_) { m_iHandle = iHandle_; m_uiMagic = uiMagic_; }

    /** int�� ȣȭ�Ǵ� handleó�� ����ϱ� ����, operator int()�� ���ؼ�
        handle part�� �����Ѵ�.
        ������, operator int&()�� �������� �ʴµ� �̰��� �߸��� ������� magic number�� �����ϸ鼭
        handle�� �����Ű�� ���� ���� �����̴�.
        - jintaeks on 2008-04-01, 10:06 */
    //operator int() const
    //{
    //    return m_iHandle;
    //}//operator int()

    /// handle part�� magic number part�� ��� ������ �˻��Ѵ�.
    bool operator==(const KHandle& rhs_) const
    {
        return m_iHandle == rhs_.m_iHandle && m_uiMagic == rhs_.m_uiMagic;
    }//operator==()

    bool operator!=(const KHandle& rhs_) const
    {
        return m_iHandle != rhs_.m_iHandle || m_uiMagic != rhs_.m_uiMagic;
    }//operator==()

    //bool operator!=(const int& rhs_) const
    //{
    //    return m_iHandle != rhs_;
    //}//operator==()

    /// KHandle�� container�� node�� ���� ��츦 ���ؼ� ���� �Լ��̴�.
    /// @note   stl container�� equivalence�� ������Ű�� ���ؼ� �Ʒ��� ����ó��
    ///         ���� �ڵ鿡 ���ؼ� ���ϰ�, ������ magic number�� ���ؾ� ����
    ///         ��������.
    bool operator<(const KHandle& rhs_) const
    {
        /** ���(equivalence)�� ������Ű�� ���ؼ��� �ݵ�� ������ if������ �����Ǿ�� �Ѵ�.
            - jintaeks on 2008-03-31, 16:21 */
        if ( m_iHandle != rhs_.m_iHandle )
            return m_iHandle < rhs_.m_iHandle;

        return m_uiMagic < rhs_.m_uiMagic;
    }//operator<()

    //int& operator=(int rhs_)
    //{
    //    if( rhs_ == NULL )
    //    {
    //        Invalidate();
    //    }
    //    else
    //    {
    //        //m_uiMagic = 0;
    //        m_iHandle = rhs_;
    //    }//if.. else..

    //    return m_iHandle;
    //}//operator=()

    //inline friend bool operator==( int iHandle_, const KHandle& handle_ )
    //{
    //    return iHandle_ == handle_.m_iHandle;
    //}//operator==()

    //inline friend bool operator!=( int iHandle_, const KHandle& handle_ )
    //{
    //    return iHandle_ != handle_.m_iHandle;
    //}//operator!=()

};//class KHandle




template<typename TYPE, typename TAG>
class   KHandleType;

template<typename TYPE, typename TAG>
struct  KHandleType_Invalid
{
    typedef TYPE        value_type;
    static const TYPE   value = TYPE(-1);

    bool                operator == ( const KHandleType<TYPE,TAG>& rhs_ ) const;
    bool                operator != ( const KHandleType<TYPE,TAG>& rhs_ ) const;
};

template<typename TYPE, typename TAG>
class   KHandleType
{
public:

    typedef TYPE        value_type;
    typedef KHandleType_Invalid<TYPE,TAG>
                        invalid_handle;

    explicit            KHandleType(const TYPE& value )
                            : m_value( value )                      {}
    /*CONSTRUCTOR*/     KHandleType()
                            : m_value( invalid_handle::value )                   {}
    /*CONSTRUCTOR*/     KHandleType( invalid_handle )
                            : m_value( invalid_handle::value )                   {}
    /*CONSTRUCTOR*/     KHandleType( const KHandleType& rhs_ ) 
                            : m_value( rhs_.m_value )               {}

    KHandleType&        operator = ( const KHandleType& rhs_ )          { m_value = rhs_.m_value; return *this; }
    KHandleType&        operator = ( invalid_handle )              { m_value = invalid_handle::value; return *this; }
    const TYPE&         GetValue() const                            { return m_value; }
    void                SetValue( const TYPE& value )               { m_value = value; }
    bool                IsValid() const                             { return m_value != invalid_handle::value; }
    bool                operator == ( const KHandleType& rhs_ ) const   { return m_value == rhs_.m_value; }
    bool                operator != ( const KHandleType& rhs_ ) const   { return m_value != rhs_.m_value; }
    bool                operator == ( invalid_handle ) const       { return m_value == invalid_handle::value; }
    bool                operator != ( invalid_handle ) const       { return m_value != invalid_handle::value; }

    // map � ���� �� �ֵ���
    bool                operator < ( const KHandleType& rhs_ ) const   { return m_value < rhs_.m_value; }
    bool                operator <= ( const KHandleType& rhs_ ) const   { return m_value <= rhs_.m_value; }
    bool                operator > ( const KHandleType& rhs_ ) const   { return m_value > rhs_.m_value; }
    bool                operator >= ( const KHandleType& rhs_ ) const   { return m_value >= rhs_.m_value; }

    // lua �� export �ϴ� �뵵�� ���
    bool                IsEqual( const KHandleType& rhs_ ) const { return this->operator == (rhs_); }

private:

    TYPE                m_value;
};

template<typename TYPE, typename TAG>
inline bool             KHandleType_Invalid<TYPE,TAG>::operator == ( const KHandleType<TYPE,TAG>& rhs_ ) const   
{ 
    return rhs_.operator == ( *this ); 
}
template<typename TYPE, typename TAG>
inline bool            KHandleType_Invalid<TYPE,TAG>:: operator != ( const KHandleType<TYPE,TAG>& rhs_ ) const   
{ 
    return rhs_.operator != ( *this ); 
}


namespace   lua_tinker
{
    template<typename TYPE, typename TAG>
	void push(lua_State *L, KHandleType<TYPE,TAG> ret)					
    { 
        if ( ret.IsValid() == true )
            type2lua<TYPE>(L, ret.GetValue()); 
        else
            lua_pushnil(L);		
    }

    template<typename TYPE, typename TAG>
	void push(lua_State *L, KHandleType_Invalid<TYPE,TAG> ret)					
    { 
        lua_pushnil(L);		
    }
}



#endif // _KHandle_h


/** @example    KHandle

  1) KGOCar�� ��� ������ KHandle�� �����Ѵ�:

    KHandle         m_khSparkParticleSystem1;


  2) �����ڿ��� �ʱ�ȭ�Ѵ�.

    m_khSparkParticleSystem1.SetHandleAndMagicPart( -1, 0 );


  3) frame move���� �ڵ��� ��ȿ���� �����ϰ� �Ǵ��Ѵ�:

    void KGOCar::__UpdateParticleSystemHandle(KHandle& khInOutSystem_)
    {
        // handle�� valid�� range�� �ִ� ���, node�� magic number�� �ٸ���
        // �ı��� ����̹Ƿ� ���� �����ؾ� �Ѵ�.
        if ( g_kParticleSystemSuite.GetParticleSystemMan().IsValidHandleRange( khInOutSystem_ ) )
        {
            if ( khInOutSystem_.GetMagicPart() !=
                g_kParticleSystemSuite.GetParticleSystemMan().GetMagicNumber( khInOutSystem_ ) )
            {
                khInOutSystem_.SetHandlePart( -1 );
            }//if
        }
        // handle�� valid�� range�� �ƴϸ�, particle system�� ���� �����ؾ� �Ѵ�.
        else
        {
            khInOutSystem_.SetHandlePart( -1 );
        }//if.. else..
    }//KGOCar::__UpdateParticleSystemHandle()


  4) frame move���� target ��ü�� �����ϰ� �����Ѵ�.

    KParticleSystemEx* KGOCar::__CreateParticleSystem(KHandle& khInOutSystem_, const char* pszTemplate_)
    {
        // particle system�� �����ϰ�, handle�� ��´�.
        khInOutSystem_.SetHandlePart( g_kParticleSystemSuite.CreateParticleSystem( pszTemplate_ ) );
        ASSERT( khInOutSystem_ >= 0 );
        // ���� ���� ����� magic number�� ������ �д�.
        khInOutSystem_.SetMagicPart( 
            g_kParticleSystemSuite.GetParticleSystemMan().GetMagicNumber(khInOutSystem_) );
        return g_kParticleSystemSuite.GetParticleSystemEx( khInOutSystem_ );
    }//KGOCar::__CreateParticleSystem()

*/
