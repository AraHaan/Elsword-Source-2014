#pragma once
#include <boost/timer.hpp>


//{{ 2013. 02. 	������	Tick�����ս� DB�α� ���
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG


class KSimLayerTickChecker
{
public:
	KSimLayerTickChecker();	
	virtual ~KSimLayerTickChecker();

	void Reset();
	DWORD GetSimLayerMaxTick() const				{ return m_dwMaxTick; }
	const std::wstring GetSimLayerMaxTickRegDate() const;
	void CheckSimLayerTick();

private:
	DWORD				m_dwSimLayerTick;
	DWORD				m_dwMaxTick;
	CTime				m_tRegTime;
	boost::timer		m_tTimer;
};


#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}