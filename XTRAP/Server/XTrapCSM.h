#pragma once

#ifdef SERV_USE_XTRAP
#include "Xtrap_S_Interface.h"

class KGSUser;
SmartPointer(KGSUser);

// ���� ������ XTrap Map file üũ��� ��� Ŭ����
// 1. ���� -> Ŭ�� : üũ�� ���� ��û
// 2. Ŭ�� -> ���� : ���� ���� ����
// 3. ����		   : ���� ����

class CXTrapCSM{

	enum ENUM_SET_TIME
	{		
		ST_LOOP_AUTH_TIME = 180,
	};

public:
	CXTrapCSM();
	~CXTrapCSM();

	// ��� ����
	void SetEnable( bool bEnable ) { m_bEnable = bEnable; }
	const bool GetEnable() { return m_bEnable; }

	bool InitCSM();
	bool CSMStep1(IN KGSUserPtr spUser);
	bool CSMStep3(KEGS_XTRAP_ACK* _packet);

	// ���� ��������
	char* GetBuf(){ return m_cSessionBuf; }

protected:
	bool			m_bEnable;				// XTRAP Ȱ��ȭ ����
	boost::timer	m_kTimer;
	bool			m_bTimerStart;
	char			m_cSessionBuf[320];		// X-Trap ���� ����
};
#endif SERV_USE_XTRAP