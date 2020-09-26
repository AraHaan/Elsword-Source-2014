#pragma once
#include <list>
#include <vector>
#include <utility>


template < class T, unsigned int uiMaxNum = 5 >
class KCompareLimitList
{
public:
	KCompareLimitList()
	{
		m_uiMaxNum = uiMaxNum;
	}

	KCompareLimitList( const unsigned int uiMaxNum )
	{
		m_uiMaxNum = uiMaxNum;
	}

	~KCompareLimitList()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �� ������ ����
	void IfUpperValuePushBack( int iCompareValue, const T& data )
	{
		// �� ī��Ʈ�� �������̸� ���� insert�Ѵ�.
		std::list< std::pair< int, T > >::iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
            if( iCompareValue < lit->first )
			{
				m_listData.insert( lit, std::pair< int, T >(iCompareValue, data) );
				break;
			}
		}

		if( lit == m_listData.end() )
		{
			m_listData.push_back( std::pair< int, T >(iCompareValue, data) );
		}

		// �ִ� �����ϴ� ������ �ɶ����� ����Ʈ�� �޺κ��� ����.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ū �� ������ ����
	void IfLowerValuePushBack( int iCompareValue, const T& data )
	{
		// �� ī��Ʈ�� ū���̸� ���� insert�Ѵ�.
		std::list< std::pair< int, T > >::iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
			if( iCompareValue > lit->first )
			{
				m_listData.insert( lit, std::pair< int, T >(iCompareValue, data) );
				break;
			}
		}

		if( lit == m_listData.end() )
		{
			m_listData.push_back( std::pair< int, T >(iCompareValue, data) );
		}

		// �ִ� �����ϴ� ������ �ɶ����� ����Ʈ�� �޺κ��� ����.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ �� �տ� �ֱ�!
	void InsertFront( const T& data )
	{
		// �� ī��Ʈ�� �������̸� ���� insert�Ѵ�.
		m_listData.push_front( std::pair< int, T >(0, data) );

		// �ִ� �����ϴ� ������ �ɶ����� ����Ʈ�� �޺κ��� ����.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ �� �ڿ� �ֱ�!
	void PushBack( const T& data )
	{
		// �� ī��Ʈ�� �������̸� ���� insert�Ѵ�.
		m_listData.push_back( std::pair< int, T >(0, data) );

		// �ִ� �����ϴ� ������ �ɶ����� ����Ʈ�� �޺κ��� ����.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	const std::list< std::pair< int, T > >& GetList()	{ return m_listData; }
	unsigned int Size() const							{ return m_listData.size(); }
	unsigned int MaxSize() const						{ return m_uiMaxNum; }

	void GetValueList( std::list< T >& listData )
	{
		listData.clear();

		std::list< std::pair< int, T > >::const_iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
			listData.push_back( lit->second );
		}
	}

	void GetValueList( std::vector< T >& vecData )
	{
		vecData.clear();

		std::list< std::pair< int, T > >::const_iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
			vecData.push_back( lit->second );
		}
	}

private:
	unsigned int						m_uiMaxNum;
	std::list< std::pair< int, T > >	m_listData;
};



