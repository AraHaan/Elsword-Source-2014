#pragma once
#ifdef _NEXON_KR_

class CX2NexonMsgHandler : public CNMEventHandler
{
	public:
		CX2NexonMsgHandler();
		virtual ~CX2NexonMsgHandler();

		//	������������ ������ �������� ��쿡 �߻�
		virtual void OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent * pEvent );


#ifndef NEW_MESSENGER
		//	�޽����������� ������ ����Ǿ��� ��쿡 �߻�
		virtual void OnMessengerReplyEvent( CNMMessengerReplyEvent * pEvent );

		//	�޽����������� ������ �������� ��쿡 �߻�
		virtual void OnMsgConnectionClosedEvent( CNMMsgConnectionClosedEvent * pEvent );

		// ���ο� ģ�� ��� ��û�� ���� �� �߻�?
		virtual void OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent );

		// ���� ������ ����(�� ����, ģ�� ����Ʈ�� ����, �� ��� ����Ʈ�� ����, �������� ����)�� ���� �� 
		// �߻��ϴ� �̺�Ʈ�̴�. 
		// ���� �� ������ �����ϰų�, ģ�� �Ǵ� �׷��� �߰�/����/������ ��쿡�� �߻��Ѵ�.
		virtual void OnRefreshEvent( CNMRefreshEvent* pEvent );

		// ģ�� �߰� ����, ����, ���� ���� ���� ���� �̺�Ʈ ó��
		virtual void OnCustomMessageEvent( CNMCustomMessageEvent* pEvent );

		// ģ���� ������ ����Ǿ��� ��� �߻��ϴ� �̺�Ʈ
		virtual void OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent );

#endif // #ifndef NEW_MESSENGER

		// ������ ������ ��� �߻��ϴ� �̺�Ʈ
		virtual void OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent );

		// ���ǿ� ���� ���� ������ ����Ǿ��� ��� �߻�.
		virtual void OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );

		// �ý��� �޽����� �˸� �޽����� �޽��� ���� ����� ��ȭ �������� ���� ��ȭ ���� �˷��ִ� �޽����� ��� �ִ�.
		virtual void OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent );

		// ������ ���������� ���� �Ǿ��� ���(�ʴ��� ģ������ Join�Ͽ� Operation�� ������ ���) �߻�
		virtual void OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );

		// ������ ����Ǿ��� ��� �߻�. �ش� ������ Serial Key�� �� �̻� ��ȿ���� �ʰ� �ȴ�.
		// �޽��� �α׾ƿ� �� ������ ������ ���� ��� �߻��ϹǷ�, �� �̺�Ʈ�� �� ��쿡�� ä��â�� �ݵ��� �Ѵ�.
		virtual void OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );

		// ���� ���� ������ ������ ���������� ���õ��� �ʾ��� ��� �߻�.
		virtual void OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );

	
		

	private:
};
#endif _NEXON_KR_
