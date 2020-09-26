#pragma once

#include "GameServer.h"

class KGSUser;

//���� ������ korea nProtect ����� Ŭ����

class KGGAuth
{
	enum ENUM_SET_TIME
	{
		ST_FIRST_AUTH_TIME		= 30,
		ST_LOOP_AUTH_TIME_MIN	= 240,	//min random time(auth check)
		ST_LOOP_AUTH_TIME_MAX	= 300,	//max random time(auth check)
	};

	enum ENUM_TIMER
	{
		TM_FIRST_AUTH_TIME = 0,
		TM_LOOP_AUTH_TIME,
		TM_END,
	};

public:
	KGGAuth();
	virtual ~KGGAuth();

	void	SetEnable( bool bEnable ){ m_bEnable = bEnable; }	//Ŭ���� ��뿩�� ����
	bool	GetEnable(){ return m_bEnable; }

	void	Init();
	bool	Tick( IN KGSUserPtr spUser );

	bool	FirstAuthQuery( IN KGSUserPtr spUser );
	bool	CheckAuthAnswer( IN KGSUserPtr spUser, IN KEGS_KNPROTECT_AUTH_CHECK_NOT& kInfo );

private:
	bool	AuthQuery( IN KGSUserPtr spUser );
	void	RandTickTime();	//�������� ����üũ�� �ϱ����� �ð��� ���� �����Ѵ�.

protected:
	bool				m_bEnable;
	CCSAuth2			m_cCSA;		//��ī �ѱ� ���

	boost::timer		m_kTimer[TM_END];
	bool				m_bIsFirstAuth;
	bool				m_bIsLoopAuth;

	double				m_AuthTickTime;
};
