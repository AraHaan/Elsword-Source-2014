#pragma once

#ifdef CONVERSION_VS
//���̻� ������ �ʴ� Ŭ����
#else CONVERSION_VS

#include "KTDNPacket.h"
#include "KTDNTransfer.h"
#include "KTDNDP.h"
#include "KTDNUDP.h"

class CKTDNManager : public CKTDXStage
{
	public:
		CKTDNManager(void);
		~CKTDNManager(void);

		CKTDNDP*	GetDP(){ return m_pKTDNDP; }

	private:
		CKTDNDP*	m_pKTDNDP;

};
#endif CONVERSION_VS