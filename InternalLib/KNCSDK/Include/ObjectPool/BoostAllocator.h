#pragma once
#include <windows.h>
#include <boost/pool/object_pool.hpp>

/************************************************************************/
/* 2010-01-15 : ������
/* boost ���̺귯���� object_pool�� ��ӹ޾Ƽ� ����� �� �ֵ��� wrapping�� Ŭ����
/* [����] BoostAllocator�� ����ϴ� class�� �ݵ�� �ڽ�Ŭ������ ���� ���� Ŭ���������Ѵ�.
/************************************************************************/


//////////////////////////////////////////////////////////////////////////
// BoostAllocator�� ����ϸ鼭 ��ӹ��� ���ϴ� ������ Ŭ������ ����� ��ũ��
#define BOOST_ALOC_CLASS(className) \
	class className ## Sealer \
	{ \
	private: className ## Sealer(){}; \
		friend class className; \
	}; \
	class className : virtual private className ## Sealer, public BoostUtil::BoostAllocator<className> \

// ex :
// BOOST_ALOC_CLASS( Test )
// {
//     ...
// }

//////////////////////////////////////////////////////////////////////////
// BoostAllocator�� ����ϸ鼭 ��ӹ��� ���ϴ� ������ Ŭ������ ����� ��ũ�� ( ���� Ŭ���� �Ѱ� ��� ���� )
#define BOOST_ALOC_CLASS_INHERITANCE(className, inheritance) \
	class className ## Sealer \
	{ \
	private: className ## Sealer(){}; \
		friend class className; \
	}; \
	class className : virtual private className ## Sealer, public BoostUtil::BoostAllocator<className>, inheritance \

// ex :
// BOOST_ALOC_CLASS_INHERITANCE( Test, public BaseTest )
// {
//     ...
// }

//////////////////////////////////////////////////////////////////////////
// BoostAllocator�� ����ϸ鼭 ��ӹ��� ���ϴ� ������ Ŭ������ ����� ��ũ��
#define BOOST_ALOC_SYNC_CLASS(className) \
	class className ## Sealer \
	{ \
	private: className ## Sealer(){}; \
		friend class className; \
	}; \
	class className : virtual private className ## Sealer, public BoostUtil::BoostAllocatorSync<className> \

// ex :
// BOOST_ALOC_SYNC_CLASS( Test )
// {
//     ...
// }

//////////////////////////////////////////////////////////////////////////
// BoostAllocator�� ����ϸ鼭 ��ӹ��� ���ϴ� ������ Ŭ������ ����� ��ũ�� ( ���� Ŭ���� �Ѱ� ��� ���� )
#define BOOST_ALOC_SYNC_CLASS_INHERITANCE(className, inheritance) \
	class className ## Sealer \
	{ \
	private: className ## Sealer(){}; \
		friend class className; \
	}; \
	class className : virtual private className ## Sealer, public BoostUtil::BoostAllocatorSync<className>, inheritance \

// ex :
// BOOST_ALOC_SYNC_CLASS_INHERITANCE( Test, public BaseTest )
// {
//     ...
// }


namespace BoostUtil {


// synchronization object
struct AllocatorCriticalSection : public CRITICAL_SECTION
{
	AllocatorCriticalSection()  { InitializeCriticalSection( this ); }
	~AllocatorCriticalSection() { DeleteCriticalSection( this ); }
};


// synchronization interface
class AllocatorLocker
{
public:
	AllocatorLocker( CRITICAL_SECTION& cs )
	{
		m_pCS = &cs;
		EnterCriticalSection( m_pCS );
	}
	~AllocatorLocker()
	{
		LeaveCriticalSection( m_pCS );
		m_pCS = NULL;
	}

private:
	CRITICAL_SECTION* m_pCS;
};


//////////////////////////////////////////////////////////////////////////
// boost::object_pool wrapping class
template< class AllocType >
class BoostAllocator
{
public:
	void* operator new( size_t size )
	{
		return s_Pool.malloc();
	}

	void operator delete( void* pPointer )
	{
		s_Pool.free( (AllocType*)pPointer );
	}

	unsigned int GetNextAllocSize()
	{
		return static_cast<unsigned int>(s_Pool.get_next_size());
	}

private:
	typedef boost::object_pool<AllocType> BoostObjectPool;
	static BoostObjectPool s_Pool;
};

template<class AllocType>
typename BoostAllocator<AllocType>::BoostObjectPool BoostAllocator<AllocType>::s_Pool(1); // �������� �������ڴ� �̸� �����س��� �޸� �� ����


//////////////////////////////////////////////////////////////////////////
// boost::object_pool wrapping class - synchronization
template< class AllocType >
class BoostAllocatorSync
{
public:
	void* operator new( size_t size )
	{
		AllocatorLocker lock( s_cs );

		return s_Pool.malloc();
	}

	void operator delete( void* pPointer )
	{
		AllocatorLocker lock( s_cs );

		s_Pool.free( (AllocType*)pPointer );
	}

	unsigned int GetNextAllocSize()
	{
		AllocatorLocker lock( s_cs );

		return static_cast<unsigned int>(s_Pool.get_next_size());
	}

private:
	typedef boost::object_pool<AllocType> BoostObjectPool;
	static BoostObjectPool s_Pool;
	static AllocatorCriticalSection s_cs;
};

template<class AllocType>
typename BoostAllocatorSync<AllocType>::BoostObjectPool BoostAllocatorSync<AllocType>::s_Pool(1); // �������� �������ڴ� �̸� �����س��� �޸� �� ����

template<class AllocType>
typename AllocatorCriticalSection BoostAllocatorSync<AllocType>::s_cs;


} // namespace BoostUtil

