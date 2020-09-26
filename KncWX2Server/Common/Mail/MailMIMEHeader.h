#pragma once 


//#ifndef _MAILMIMEHEADER_H
//#define _MAILMIMEHEADER_H

//Head1�� ������ ���� �޾ƿ´�.
extern "C" _declspec(dllexport) char *MIME_MakeHead1(char *to, char *cc=NULL);
//to			�޴»��
//cc			����(bcc�� ���� �������� ���� �ʴ´�.)

//Head2�� ������ ���� �޾ƿ´�.
extern "C" _declspec(dllexport) char *MIME_MakeHead2(char *fromName, char *fromEMail
													 , char *subject, int priority);
//fromName		������� �̸�
//fromEMail		������� ���ڸ���
//subject		����
//priority		�����켱����	(0- ����(1), 1-�Ϲ�(3), 2-����(5))

//������ �����
extern "C" _declspec(dllexport) char *MIME_MakeBody(char *Text, int TextType);
//Text			����
//TextType		��������		(0-text, 1-html, 2-html/text)

//÷�������� ��ȯ�Ѵ�.
extern "C" _declspec(dllexport) char *MIME_MakeAttach(char **AttachFile, int cAttachFile);
//AttachFile	÷�������� �̸���(��α��� ����)
//cAttachFile	÷������ ����

//Data�� �������κ��� �޾ƿ´�.
extern "C" _declspec(dllexport) const char *MIME_GetEnd();

//MailMIME dll���� �Ҵ��� �޸𸮴� free�Ѵ�.(Head1, Head2, Body, Attach)
extern "C" _declspec(dllexport) void MIME_Free(char *&mem);

//#endif