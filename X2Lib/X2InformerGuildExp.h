//{{ kimhc // 2009-11-02 // ���� �ڽſ� ���� ��� ����ġ�� �ö��� �� ���� �Ǵ� informer
#ifdef	GUILD_MANAGEMENT
#pragma once

class CX2InformerGuildExp : public CX2Informer
{
public:

	CX2InformerGuildExp(void);
	virtual ~CX2InformerGuildExp(void);

	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
	void Reset();
	inline void SetIncreaseGuildExpByMe( bool bChange ) { m_bIncreaseGuildExpByMe = bChange; }
	inline bool GetIncreaseGuildExpByMe() const { return m_bIncreaseGuildExpByMe; }

protected:

	void SetNowInfo();
	bool IsNowInfoChange();
	void SetShow( bool bShow );

private:
	bool m_bIncreaseGuildExpByMe;				// ���ο� ���� ��� ����ġ�� �ö� ���°�?
};
#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-11-02 // ���� �ڽſ� ���� ��� ����ġ�� �ö��� �� ���� �Ǵ� informer