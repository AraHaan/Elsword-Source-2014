#pragma once

#include "GameServer.h"

class KGSUser;

//���� ������ korea HackShield ����� Ŭ����

class KHSAuth
{
	enum ENUM_SET_TIME
	{
		// 4�� ~ 5�� ( nProtect ���� )
		//ST_LOOP_AUTH_TIME_MIN	= 240,	//min random time(auth check)
		//ST_LOOP_AUTH_TIME_MAX	= 300,	//max random time(auth check)

		// 3�� ~ 4�� ( HackShield ���� )
		ST_LOOP_AUTH_TIME_MIN	= 180,	//min random time(auth check)
		ST_LOOP_AUTH_TIME_MAX	= 240,	//max random time(auth check)
	};

	enum ENUM_TIMER
	{
		TM_LOOP_AUTH_TIME = 0,
		TM_END,
	};

public:
	KHSAuth();
	virtual ~KHSAuth();

	void	SetEnable( bool bEnable ){ m_bEnable = bEnable; }	//Ŭ���� ��뿩�� ����
	bool	GetEnable(){ return m_bEnable; }
	
	bool	Tick( IN KGSUserPtr spUser, IN bool bForce = false );

	bool	FirstHackShieldAuth( IN KGSUserPtr spUser );
	bool	OnHackShieldCheckAck( IN KGSUserPtr spUser, IN KEGS_HACKSHIELD_CHECK_ACK& kInfo );

public:
	static void SetAhnHSServerHandle( AHNHS_SERVER_HANDLE hServer ) { ms_hServer = hServer; }
	static AHNHS_SERVER_HANDLE GetAhnHSServerHandle() { return ms_hServer; }
	static void SetHSBFilePath( const std::string& strFilePath ) { ms_HSBFilePath = strFilePath; }
	static const std::string GetHSBFilePath() { return ms_HSBFilePath; }

private:
	bool	SendHackShiendCheckReq( IN KGSUserPtr spUser );
	void	RandTickTime();	//�������� ����üũ�� �ϱ����� �ð��� ���� �����Ѵ�.

protected:
	bool						m_bEnable;			// �ٽ��� ���� ����
	AHNHS_CLIENT_HANDLE			m_hClient;			// check packet ó���� �ʿ��� �ڵ�

	boost::timer				m_kTimer[TM_END];
	bool						m_bIsStarted;		// �ٽ��尡 �����ߴ���
	bool						m_bIsSendReq;		// check packet�� Ŭ���̾�Ʈ�� ���´���
	double						m_AuthTickTime;		// ���� check packet �����ð� ����

	static AHNHS_SERVER_HANDLE	ms_hServer;			// Ŭ���̾�Ʈ �ڵ��� ������� ���� �ڵ�
	static std::string			ms_HSBFilePath;		// HSB���� ���
};


