#pragma once
#include <thread/Thread.h>
#include <atltime.h>
#include "ThreadManager.h"

#include "Event.h"

// ���� �Ϲ����� ����� loop, dispatch�� �����Ǿ� �ִ�. 
// �̺�Ʈ ���̵� ���� �ڵ鷯 �Լ��� �����ϰ��� �Ѵٸ� 
// KSubThread���� ��ӹް� �ڵ鷯�� �߰��Ѵ�.
class KTRSubThread : public KThread
{
protected:  // ���� ��ü�� ���������� �ʴ´�.
	KTRSubThread() {}
public:
	virtual ~KTRSubThread(void) {}

protected:
	inline void Loop()
	{
		if( !m_pkThreadMgr->GetKEventAll( m_EventQueue ) )
		{
			std::cout << "Sub-Thread Terminated." << std::endl;
			SetEvent( m_hKillEvent );
			return;
		}

		//DWORD dwElapTime = ::GetTickCount();  // 040428. ó�� �ҿ�ð��� ���Ѵ�.

		KEventPtr spEvent;

		while( !m_EventQueue.empty() )
		{
			spEvent = m_EventQueue.front();
			m_EventQueue.pop();

			ProcessEvent( spEvent );
		}

		//dwElapTime = ::GetTickCount() - dwElapTime;

		//m_pkThreadMgr->CheckMaxProcessingInfo( dwElapTime, spEvent );
	}

	virtual void Run()     // derived from KThread
	{
		std::cout << "KSubThread::Run(), thread start." << std::endl;

		DWORD ret;

		while( true )
		{
			ret = ::WaitForSingleObject( m_hKillEvent, 1 );

			if( ret == WAIT_OBJECT_0 ) break;

			if( ret == WAIT_TIMEOUT ) Loop();

			else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

		}
	}

	virtual void ProcessEvent( const KEventPtr& spEvent )   = 0;

private:
	std::queue< KEventPtr >                     m_EventQueue;
};



