#ifndef _MAILSMTPHEADER_H
#define _MAILSMTPHEADER_H

//������ �����µ� �������� �ϴ� �ڷ�
//data~��� ���۵Ǵ� ���� �ޱ����ؼ� MailMIME.dll�� �̿��Ѵ�.
typedef struct{
	char *from;			//������ ���
	char *to;			//�޴»��(,���� ����)
	char *cc;			//����
	char *bcc;			//��������
	char *dataHead1;	//���Ҽ� �ִ� ���- Date, Message-ID, To, Cc, (Bcc)
	char *dataHead2;	//��ġ�ʴ� ���
	char *dataBody;		//����
	char *dataAttach;	//attach file
	const char *dataEnd;		//���κ� ����
} _Mail;

//������ �ּҸ� ������ ��
#define DEFAULT_SEPARATOR ','

//�Լ������� ����
typedef void (*ENDPROC)(BOOL bSuccess);
//bSuccess :TRUE�̸� �۾�����, FALSE�̸� �۾����� - �󼼿����� SMTP_GetLastMsg
typedef void (*ALERTPROC)(int iStep, int iProgressStep);
//iStep	:	0-��ü�ܰ踦 iProgressStep�� �־��ش�.
//			1-�������α׷� �ʱ�ȭ��
//			2-����������
//			3-������ �����
//			4-���Ϻ����� ��
//			5-�۾��Ϸ�
//iProgressStep	: ��ü�ܰ迡�� ������� ������ �Ǿ��°�.(���α׷����ٿ� SetPos���̶�� �ұ�.)

//������ ������.
//return 0�̸� �������, �׿��̸� ���༺��. ������ ���� fnEnd�� ȣ��Ǿ�� �۾���
extern "C" _declspec(dllexport) long SMTP_SendMail(HWND hwndOwner, unsigned long ulSMTPAddr
								, _Mail *pMail, ENDPROC fnEnd, ALERTPROC fnAlert=NULL);
//	hwndOwner	: �� dll�� ���ԵǾ� �ִ� ������.
//	ulSMTPAddr	: SMTP������ �ּ�
//	pMail		: ������ ���Ͽ� ���� ����(�ڼ��� ������ _Mail����ü ����)
//	fnEnd		: �۾��� ������ �� ȣ���� �Լ�. PbCont�� ���� �۾��� �����ߴ��� �˼� �ִ�.
//	fnAlert		: ���������� ���� ȣ��Ǵ� �Լ�. NULL�̸� �������� �˸��� ����.


//���Ϻ����⸦ �ߴ��Ѵ�. �׷��� ����� fnEnd�� �´�.
extern "C" _declspec(dllexport) void SMTP_Cancel();

#ifndef INADDR_NONE
#define INADDR_NONE             0xffffffff
#endif

//���ڿ� ������ �ּҸ� ("210.111.23.194" or "mail.perceptcom.com") ULONG�� ������ �ٲ�.
//�߸��� �����̸� INADDR_NONE�� ����.
extern "C" _declspec(dllexport) unsigned long SMTP_TransAddr(char *strAddr);

//pbCont�� FALSE�� �ٲ���� ��� �ڼ��� ������ ������ ����.
extern "C" _declspec(dllexport) char *SMTP_GetLastMsg();

#endif
