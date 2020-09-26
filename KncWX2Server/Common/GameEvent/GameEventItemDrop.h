#pragma once

#include "GameEvent.h"

class KGameEventItemDrop : public KGameEvent
{
public:
	KGameEventItemDrop(void);
	virtual ~KGameEventItemDrop(void);

	//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��	
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	void	SetDropRate( IN const float fDropRate )	{ m_fDropRate = fDropRate; }
	float	GetDropRate( void ) const	{ return m_fDropRate; }
#else // SERV_DROP_EVENT_RENEWAL
	void	SetDropCount( int iDropCount )			{ m_iDropCount = iDropCount; }
	int		GetDropCount()							{ return m_iDropCount; }
#endif // SERV_DROP_EVENT_RENEWAL

	void	SetWithPlayPcBang( bool bVal )			{ m_bWithPlayPcBang = bVal; }
	bool	IsWithPlayPcBang()						{ return m_bWithPlayPcBang; }

	//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
	void	SetDungeonID( int iDungeonID)			{ m_iDungeongID = iDungeonID; }
	int		GetDungeonID()							{ return m_iDungeongID; }

	void	SetUnitType( char cUnitType )			{ m_cUnitType = cUnitType; }
	int		GetUnitType()							{ return m_cUnitType; }
	void	SetUnitClass( char cUnitClass )			{ m_cUnitClass = cUnitClass; }
	int		GetUnitClass()							{ return m_cUnitClass; }
	void	SetStartLevel(int iStartLevel)			{m_iStartLevel =  iStartLevel; }
	int		GetStartLevel()							{return m_iStartLevel; }
	void	SetEndLevel(int iEndLevel)				{m_iEndLevel =  iEndLevel; }
	int		GetEndLevel()							{return m_iEndLevel; }


#endif //DROPEVENT_RENEWAL

private:
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	float	m_fDropRate;
#else // SERV_DROP_EVENT_RENEWAL
	int		m_iDropCount;
#endif // SERV_DROP_EVENT_RENEWAL
	bool	m_bWithPlayPcBang;

	//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
	int		m_iDungeongID;
	char	m_cUnitType;
	char	m_cUnitClass;
	int		m_iStartLevel;
	int		m_iEndLevel;
#endif //DROPEVENT_RENEWAL
	//}}
};
