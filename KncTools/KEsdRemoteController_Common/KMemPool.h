#ifndef KMEMPOOL_H
#define KMEMPOOL_H

#include <vector>
#include <deque>

//using namespace std;

//---------------------------------------------------------------------------
class KPoolCreator
{
public:
	virtual void* Create() = 0;
	virtual void Destroy(void* pkChunk) = 0;

	void CleanUp() { delete this; }
};
//---------------------------------------------------------------------------
template< class tObjType >
class KDefaultCreator : public KPoolCreator
{
public:
    virtual ~KDefaultCreator() {}

	virtual void* Create()
	{   
		return (void*) new tObjType;
	}

	virtual void Destroy(void* pkChunk)
	{
		delete (tObjType*) pkChunk;
	}
};
//---------------------------------------------------------------------------
template< class tObjType >
class KMemPool
{

public:
	KMemPool();
	virtual ~KMemPool();

	void CreatePool(unsigned int uiSize, KPoolCreator* pkCreator);
	void ExtendPool(unsigned int uiSize);
	void DestroyPool();
	void SetCreator(KPoolCreator* pkCreator);

	tObjType* Alloc();
	void Free(void* pvChunk);

protected:
	struct KPoolData
	{
		typedef std::deque<void*> KObjectDeque;    
		KObjectDeque m_deqFree;
		KObjectDeque m_deqAlloc;

		KPoolCreator* m_pkCreator;

		KPoolData()
		{
			m_deqFree.clear();
			m_deqAlloc.clear();

			m_pkCreator = NULL;
		}

		~KPoolData()
		{
			if (m_pkCreator != NULL)
			{
				m_pkCreator->CleanUp();
				m_pkCreator = NULL;
			}
		}
	};

	KPoolData m_kPoolData;

};
//---------------------------------------------------------------------------
#define KDeclareMemPool(mClassname) \
public: \
	static KMemPool<mClassname> m_kMemPool; \
	static void CreatePool(unsigned int uiSize, KPoolCreator* pkCreator = NULL) \
	{ \
		m_kMemPool.CreatePool(uiSize, pkCreator); \
	} \
	static void DestroyPool() \
	{ \
		m_kMemPool.DestroyPool(); \
	} \
	static mClassname* Alloc() \
	{ \
		return m_kMemPool.Alloc(); \
	} \
	void Free() \
	{ \
		m_kMemPool.Free(this); \
	}

#define KDeclarePoolMethod \
public: \
	void InitPool() {} \
	void ReleasePool() {}

#define KImplementMemPool(mClassname) \
KMemPool<mClassname> mClassname::m_kMemPool; \
//---------------------------------------------------------------------------
#include "KMemPool.inl"

/*

	* ��� ���
	
	1. �ش� Ŭ������ KDeclareMemPool ��ũ�θ� �̿��Ͽ� Pool ��ü ����
	2. KDeclarePoolMethod ��ũ�γ� InitPool() / ReleasePool() �޽��带 ����
	  �Ͽ� Ǯ�� �̺�Ʈ ����

	* ����
	- Ŭ���� ���� Ǯ���� �ƴ�, ��ü ���� Ǯ���� ���� ����Ǿ� ���� �ʱ� ������,
	  ���� �Ŵ�¡ Ŭ������ ����� ����ϼž� �մϴ�.

*/

#endif // KMEMPOOL_H