/// @file   ProtectedType.h
/// @author pch413@kogstudios.com
/// @since  2005-09-09 ���� 12:03:51

// Ver1.0
// < Note by Park Chang-hyeon >
// �Ʒ��� KProtectedType/KProtectedArray�� ���ø� ���ڿ� ���ؼ�
// Built-in types(Primitive types), �� int/char/float... ����
// �⺻ Ÿ�Կ� ���ؼ��� ��ȿ���� �����Ѵ�.
// T ���ڿ� ����� ���� �ڷ����� ���� ��� ���ο� pointer ���� ���� �ִٸ�
// ��ü ���簡 ����� �̷������ �ʰų� üũ���� ��ȿ���� ������.
// �׷��Ƿ� �⺻ Ÿ�Կ� ���ؼ��� ����� ���� �ǰ��Ѵ�.

// Ver1.1
// @modify 2007/3/7. iridology.
// ���� ������� �ʴ� ProtectedTypeArray, ProtectedTypeVector�� �����ϰ�,
// ���ɻ��� ������ �ִ� �κ��� ������ ���̵��� ��ü����.
// ProtectedRefTable( new�� ����, size�� �������� �ʰ�, ���뵵 �������� ���� ���̺� )
// �� ����� table index, ����� ���� �������ν� �������� ��ȿ���� üũ�Ѵ�.
// Primitive types�� ����ϱ⸦ �ǰ��ϴ� ���� ������� �ʾҴ�.

// @comment 2007/3/8. iridology. 
// Ư�� Ÿ���� ProtectedType�� �������
// Ŭ���� ���ø� Ư��ȭ(?)�� �̿��ؼ� ���弼��.
// ( class KProtectedType<bool> ���� )

// Ver1.2
// @comment 2007/4/6 iridology
// m_data�� verifyData�� ���� ���� ���ؼ� ������ �ϸ� �ո��°� ������.
// m_data�� >> 1 �����ؼ� ���
/*************************************************************************/
/*
                            New( >> )	    Old	        Primitive
1,000,000 	    A	            58 	        59 	        29 
                B	            13 	        13 	        9 
100,000 	    A	            6 	        6 	        3 
                B	            1 	        1 	        0 
                                                                ���� : ms

                                                                A : P3-600, 128Ram, Win98
                                                                B : P4-3.2 1G WinXp-sp2
                                                                * ��� ������ Release ��带 �������� �Ͽ���.

/************************************************************************/

// Ver1.3
// @comment 2007/4/6 iridology
// KProtectedType<int> a = 50 �̷����� �������� �߻��ϴ�
// m_data�� �����ּҸ� ã�Ƴ��� Freeze ��Ű�� �ո��°� ���� ����
// m_data�� ����ϴ� ���� �ƴϰ� operator�� �Ķ���ͷ� �Ѿ����
// rhs�� �̿��ؼ� ���
/************************************************************************/
/*
                            New( rhs )	    Old( >> )
1,000,000 	    A	            58 	        59 
                B	            13 	        13 
100,000 	    A	            6 	        6 
                B	            1 	        1 
                                                                ���� : ms

                                                                A : P3-600, 128Ram, Win98
                                                                B : P4-3.2 1G WinXp-sp2
                                                                * ��� ������ Release ��带 �������� �Ͽ���.

/************************************************************************/

// Ver1.4
// @comment 2007/5/10. daeuk = iridology ^^;
// g_bDetectedHack -> g_bHackSafe�� �ٲ���.. ���� ���ظ� ����
// |= �� �����ϴ��Ÿ� &= �� ����.
// ���� : �ѹ� �����ϸ� �� ������ �����ϴ� ���� �����ϵ��� �ϱ� ���ؼ���.

#ifndef _PROTECTEDTYPE_H_
#define _PROTECTEDTYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push)
#pragma warning(disable:4786)

#include <sstream>
#include <vector>
#include <assert.h>
#include "KProtectRefTable.h"

using namespace ProtectTable;

// KProtectedType : ������Ƽ�� Ÿ���� ���ڷ� �޴´�.
// usage 1 : KProtectedType< int > obj;
// usage 2 : KProtectedType< float > obj;
template< class T >
class KProtectedType
{
public:
    // constructors
    KProtectedType( void )
    {
        m_data = T();
        ConstructorUpdate( m_data );
    }
    KProtectedType( const T& src )
    {
        m_data = src;
        ConstructorUpdate( src );
    }
    KProtectedType( const KProtectedType< T >& src )
    {
        m_data = src.m_data;
        m_verifyData = src.m_verifyData;
        m_ucTableIndex = src.m_ucTableIndex;
    }

    // destructor
    ~KProtectedType( void )
    {
        // empty
    }

    // type-casting operator
    operator T( void ) const
    {
        return m_data;
    }

    // assignment operators
    const KProtectedType< T >& operator = ( const T& rhs )
    {
        Update( rhs );
        return *this;
    }
    const KProtectedType< T >& operator = ( const KProtectedType< T >& rhs )
    {
        m_data = rhs.m_data;
        m_verifyData = rhs.m_verifyData;
        m_ucTableIndex = rhs.m_ucTableIndex;
        return *this;
    }
    const KProtectedType< T >& operator += ( const T& rhs )
    {
        Update( m_data + rhs );
        return *this;
    }
    const KProtectedType< T >& operator -= ( const T& rhs )
    {
        Update( m_data - rhs );
        return *this;
    }
    const KProtectedType< T >& operator *= ( const T& rhs )
    {
        Update( m_data * rhs );
        return *this;
    }
    const KProtectedType< T >& operator /= ( const T& rhs )
    {
        Update( m_data / rhs );
        return *this;
    }
    const KProtectedType< T >& operator %= ( const T& rhs )
    {
        Update( m_data % rhs );
        return *this;
    }
    const KProtectedType< T >& operator <<= ( const T& rhs )
    {
        Update( m_data << rhs );
        return *this;
    }
    const KProtectedType< T >& operator >>= ( const T& rhs )
    {
        Update( m_data >> rhs );
        return *this;
    }

    // prefix increment operator
    const KProtectedType< T >& operator ++ ( void )
    {
        Update( m_data + 1 );
        return *this;
    }
    // postfix increment operator
    const KProtectedType< T >& operator ++ ( int )
    {
        Update( m_data + 1 );
        return *this;
    }
    // prefix decrement operator
    const KProtectedType< T >& operator -- ( void )
    {
        Update( m_data - 1 );
        return *this;
    }
    // postfix decrement operator
    const KProtectedType< T >& operator -- ( int )
    {
        Update( m_data - 1 );
        return *this;
    }

public:
    // verify data
    inline bool Verify( void ) const
    {
#ifdef _DEBUG
        ProtectDebugHelper::g_dwVerifyCall++;
#endif
        bool bRet = ( m_verifyData == static_cast<T>( (m_data << 1) - g_kProtectTable[ m_ucTableIndex ] ) );
        ProtectTableHelper::g_bHackSafe &= bRet;

        assert( bRet != false );

        return bRet;
    }

    // convert informations to a string
    std::wstring ToString( void ) const
    {
        std::wostringstream strmStr;

        strmStr << "value : " << m_data
            << ", verify value : " << m_verifyData
            << ", ref value : " << g_kProtectTable[ m_ucTableIndex ] << "\n";

            return strmStr.str();
    }

private:
    // update
    inline void Update( T rhs )
    {
        ProtectTableHelper::g_bHackSafe &= Verify();

        m_ucTableIndex = rand() % g_iTableSize;
        m_verifyData = static_cast<T>( (rhs << 1) - g_kProtectTable[ m_ucTableIndex ] );

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

    // for constructor
    inline void ConstructorUpdate( T rhs )
    {
        m_ucTableIndex = rand() % g_iTableSize;
        m_verifyData = static_cast<T>( (rhs << 1) - g_kProtectTable[ m_ucTableIndex ] );

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

private:
    T       m_data;             // data field
    T       m_verifyData;       // for Verify
    UCHAR   m_ucTableIndex;     // index of ref table
};

template<>
class KProtectedType<bool>
{
public:
    // constructors
    KProtectedType( void )
    {
        m_data = bool();
        ConstructorUpdate( m_data );
    }
    KProtectedType( const bool& src )
    {
        m_data = src;
        ConstructorUpdate( m_data );
    }
    KProtectedType( const KProtectedType< bool >& src )
    {
        m_data = src.m_data;
        m_verifyData = src.m_verifyData;
    }

    // destructor
    ~KProtectedType( void )
    {
        // empty
    }

    // type-casting operator
    operator bool( void ) const
    {
        return m_data;
    }

    // assignment operators
    const KProtectedType< bool >& operator = ( const bool& rhs )
    {
        Update( rhs );
        return *this;
    }
    const KProtectedType< bool >& operator = ( const KProtectedType< bool >& rhs )
    {
        m_data = rhs.m_data;
        m_verifyData = rhs.m_verifyData;
        return *this;
    }

public:
    // verify data
    inline bool Verify( void ) const
    {
#ifdef _DEBUG
        ProtectDebugHelper::g_dwVerifyCall++;
#endif

        bool bRet = ( m_verifyData == (m_data ^ 0) );
        ProtectTableHelper::g_bHackSafe &= bRet;

        assert( bRet != false );

        return bRet;
    }

    // convert informations to a string
    std::wstring ToString( void ) const
    {
        std::wostringstream strmStr;

        strmStr << "value : " << m_data
            << ", verify value : " << m_verifyData << "\n";

        return strmStr.str();
    }

private:
    // update
    inline void Update( bool rhs )
    {
        ProtectTableHelper::g_bHackSafe &= Verify();

        m_verifyData = rhs ^ 0;
#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;
    }
    // for constructor
    inline void ConstructorUpdate( bool rhs )
    {
        m_verifyData = rhs ^ 0;

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;
    }

private:
    bool    m_data;             // data field
    bool    m_verifyData;       // for Verify
};

template<>
class KProtectedType< float >
{
public:
    // constructors
    KProtectedType( void )
    {
        m_data = float();
        ConstructorUpdate( m_data );
    }
    KProtectedType( const float& src )
    {
        m_data = src;
        ConstructorUpdate( src );
    }
    KProtectedType( const KProtectedType< float >& src )
    {
        m_data = src.m_data;
        m_verifyData = src.m_verifyData;
        m_ucTableIndex = src.m_ucTableIndex;
    }

    // destructor
    ~KProtectedType( void )
    {
        // empty
    }

    // type-casting operator
    operator float( void ) const
    {
        return m_data;
    }

    // assignment operators
    const KProtectedType< float >& operator = ( const float& rhs )
    {
        Update( rhs );
        return *this;
    }
    const KProtectedType< float >& operator = ( const KProtectedType< float >& rhs )
    {
        m_data = rhs.m_data;
        m_verifyData = rhs.m_verifyData;
        m_ucTableIndex = rhs.m_ucTableIndex;
        return *this;
    }
    const KProtectedType< float >& operator += ( const float& rhs )
    {
        Update( m_data + rhs );
        return *this;
    }
    const KProtectedType< float >& operator -= ( const float& rhs )
    {
        Update( m_data - rhs );
        return *this;
    }
    const KProtectedType< float >& operator *= ( const float& rhs )
    {
        Update( m_data * rhs );
        return *this;
    }
    const KProtectedType< float >& operator /= ( const float& rhs )
    {
        Update( m_data / rhs );
        return *this;
    }

    // prefix increment operator
    const KProtectedType< float >& operator ++ ( void )
    {
        Update( m_data + 1 );
        return *this;
    }
    // postfix increment operator
    const KProtectedType< float >& operator ++ ( int )
    {
        Update( m_data + 1 );
        return *this;
    }
    // prefix decrement operator
    const KProtectedType< float >& operator -- ( void )
    {
        Update( m_data - 1 );
        return *this;
    }
    // postfix decrement operator
    const KProtectedType< float >& operator -- ( int )
    {
        Update( m_data - 1 );
        return *this;
    }

public:
    // verify data
    inline bool Verify( void ) const
    {
#ifdef _DEBUG
        ProtectDebugHelper::g_dwVerifyCall++;
#endif

        float fTempData = m_data;
        int* pInt;
        pInt = (int*)( &fTempData );
        (*pInt) = ((*pInt) << 1) - g_kProtectTable[ m_ucTableIndex ];
        bool bRet = ( m_verifyData == (*pInt) );
        ProtectTableHelper::g_bHackSafe &= bRet;

        assert( bRet != false );

        return bRet;
    }

    // convert informations to a string
    std::wstring ToString( void ) const
    {
        std::wostringstream strmStr;

        strmStr << "value : " << m_data
            << ", verify value : " << m_verifyData
            << ", ref value : " << g_kProtectTable[ m_ucTableIndex ] << "\n";

            return strmStr.str();
    }

private:
    // update
    inline void Update( float rhs )
    {
        ProtectTableHelper::g_bHackSafe &= Verify();

        m_ucTableIndex = rand() % g_iTableSize;

        float fTempData = rhs;
        int* pInt;
        pInt = (int*)( &fTempData );
        (*pInt) = ((*pInt) << 1) - g_kProtectTable[ m_ucTableIndex ];
        m_verifyData = (*pInt);

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

    // for constructor
    inline void ConstructorUpdate( float rhs )
    {

        m_ucTableIndex = rand() % g_iTableSize;

        float fTempData = rhs;
        int* pInt;
        pInt = (int*)( &fTempData );
        (*pInt) = ((*pInt) << 1) - g_kProtectTable[ m_ucTableIndex ];
        m_verifyData = (*pInt);

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

private:
    float   m_data;             // data field
    int     m_verifyData;       // for Verify
    UCHAR   m_ucTableIndex;     // index of ref table
};


struct PROTECT_VECTOR2
{
	KProtectedType<float>		m_fX;
	KProtectedType<float>		m_fY;

	PROTECT_VECTOR2()
	{
		m_fX = 0.0f;
		m_fY = 0.0f;
	}
	bool Verify()
	{
		if( m_fX.Verify() == false
			|| m_fY.Verify() == false )
		{
			return false;
		}
		return true;
	}
};

struct PROTECT_VECTOR3
{
	KProtectedType<float>		x;
	KProtectedType<float>		y;
	KProtectedType<float>		z;

	PROTECT_VECTOR3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

#ifdef VERIFY_STAT_BY_BUFF
	PROTECT_VECTOR3( const float fX_, const float fY_, const float fZ_ )
		: x( fX_ ), y( fY_ ), z( fZ_ )
	{}

	PROTECT_VECTOR3( const PROTECT_VECTOR3& protectVec3_ )
		: x( protectVec3_.x ), y( protectVec3_.y ), z( protectVec3_.z )
	{}

	PROTECT_VECTOR3( const D3DXVECTOR3& vec3_ )
		: x( vec3_.x ), y( vec3_.y ), z( vec3_.z )
	{}

	bool operator!=( const D3DXVECTOR3& vec3_ ) const
	{
		if ( vec3_.x != x && vec3_.y != y && vec3_.z != z )
			return true;	/// ���� ����
		else
			return false;	/// ����
	}

	void operator+=( const PROTECT_VECTOR3& rhs_ )
	{
		x += rhs_.x;
		y += rhs_.y;
		z += rhs_.z;
	}

	friend static PROTECT_VECTOR3 operator-( const PROTECT_VECTOR3& lhs_, const D3DXVECTOR3& rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x - rhs_.x, lhs_.y - rhs_.y, lhs_.z - rhs_.z );
	}

	friend static PROTECT_VECTOR3 operator-( const PROTECT_VECTOR3& lhs_, const PROTECT_VECTOR3& rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x - rhs_.x, lhs_.y - rhs_.y, lhs_.z - rhs_.z );
	}

	friend static PROTECT_VECTOR3 operator*( const PROTECT_VECTOR3& lhs_, const float rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x * rhs_, lhs_.y * rhs_, lhs_.z * rhs_ );
	}

	friend static PROTECT_VECTOR3 operator/( const PROTECT_VECTOR3& lhs_, const float rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x / rhs_, lhs_.y / rhs_, lhs_.z / rhs_ );
	}

	D3DXVECTOR3 GetVector3() const { return D3DXVECTOR3( x, y, z ); }
#endif // VERIFY_STAT_BY_BUFF

	
	bool Verify()
	{
		if( x.Verify() == false
			|| y.Verify() == false
			|| z.Verify() == false )
		{
			return false;
		}
		return true;
	}
};


#pragma warning(pop)
#endif //end of _PROTECTEDTYPE_H_

// Performance

// Ver1.2
/*************************************************************************/
/*
New( >> )	    Old	        Primitive
1,000,000 	    A	            58 	        59 	        29 
B	            13 	        13 	        9 
100,000 	    A	            6 	        6 	        3 
B	            1 	        1 	        0 
���� : ms

A : P3-600, 128Ram, Win98
B : P4-3.2 1G WinXp-sp2
* ��� ������ Release ��带 �������� �Ͽ���.

/************************************************************************/

// Ver1.3
/************************************************************************/
/*
New( rhs )	    Old( >> )
1,000,000 	    A	            58 	        59 
B	            13 	        13 
100,000 	    A	            6 	        6 
B	            1 	        1 
���� : ms

A : P3-600, 128Ram, Win98
B : P4-3.2 1G WinXp-sp2
* ��� ������ Release ��带 �������� �Ͽ���.

/************************************************************************/
