#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>
#include <MMSystem.h>

class KAvgFpsCheckManager
{
public:
	class KAvgFps
	{
	public:
		KAvgFps() { Init( -1, INT_MAX ); }

		void Init( int iGameSpace, int iMinSampleNum )
		{
			m_fValue = -1.0f;
			m_fValue_StdDev = -1.0f;

			m_fLastCheckTime = -1.0f;
			m_dSumOfValue = 0.0f;
			m_uiNumOfFps = 0;

			//
			m_iLastGameSpace = iGameSpace;
			m_iMinSampleNum = iMinSampleNum;
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		void SetValue( float fValue ) { m_fValue = fValue; }
		float GetValue() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fValue; }

		void SetValue_StdDev( float fValue_StdDev ) { m_fValue_StdDev = fValue_StdDev; }
		float GetValue_StdDev() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fValue_StdDev; }

		void Update();
		
	private:
		float m_fValue;		//��� �����ӷ�
		float m_fValue_StdDev;		//ǥ�� ����

		float m_fLastCheckTime;//������ üũ �ð�
		double m_dSumOfValue;
		unsigned int m_uiNumOfFps;//

		//
		int m_iLastGameSpace;
		int m_iMinSampleNum;
	};

	//
	KAvgFpsCheckManager();
	~KAvgFpsCheckManager();

	//
	void Init( float fUpdateTime, int iMinSampleNum );

	//
	void UpdateAvgFps();

	void Start();
	void Pause() { m_bPause = true; m_iSkipFrame = 2; }//���� FPS�� ������ �Ŀ� ���ŵǴ� ���������� 2�������� skip ����� �ȴ�.
	void Resume() { m_bPause = false; }
	void End();

	const KAvgFps& GetAvgFps() { return m_kAvgFps; }


private:
	//
	float m_fUpdateTime;
	int m_iMinSampleNum;

	KAvgFps m_kAvgFps;//��� �����ӷ�

	//checking ����
	bool m_bChecking;
	//pause ����
	bool m_bPause;
	//skip�� ������ ��
	int m_iSkipFrame;
};
