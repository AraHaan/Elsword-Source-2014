#pragma once


#include "KTDSMP3Play.h"

#include "fmod/inc/fmod.h"
#include "fmod/inc/fmod_errors.h"



class CKTDSManager : public CKTDXStage
{
	public:
		CKTDSManager( HWND hWnd, LPDIRECT3DDEVICE9 pd3dDevice );
		~CKTDSManager(void);
		
#ifdef	ADD_CHANGE_BACKGROUND_MUSIC		/// ������ �ν� �Ұ�
		CKTDSMP3Play*	GetMP3Play()		/// ���� ������� ������� ��θ� ��ȯ��Ű��
		{
			if ( true == m_bUseSecondBGM )
				return m_pSecondMP3Play; 
			else
				return m_pMP3Play; 
		}
#else // ADD_CHANGE_BACKGROUND_MUSIC
		CKTDSMP3Play*	GetMP3Play(){ return m_pMP3Play; }
#endif // ADD_CHANGE_BACKGROUND_MUSIC
				
		bool			GetInit(){ return m_bInit; }

		void			SetSoundVolume( float fVolume ){ m_fSoundVolume = fVolume; }
		float			GetSoundVolume(){ return m_fSoundVolume; }

		void			SetSoundMute( bool bMute ){ m_bSoundMute = bMute; }
		bool			GetSoundMute(){ return m_bSoundMute; }


		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		
		//void ErrorCheck( FMOD_RESULT fr )
		//{
		//	if (fr != FMOD_OK)
		//	{
		//		//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		//		//exit(-1);
		//	}
		//}

		FMOD_SYSTEM* GetSystem() const { return m_pSystem; }

		bool GetEnable3DSound() const { return m_bEnable3DSound; }
		void SetEnable3DSound(bool val) { m_bEnable3DSound = val; }

		bool GetCapable3DSound() const { return m_bCapable3DSound; }
		void SetCapable3DSound(bool val) { m_bCapable3DSound = val; }


		float GetDistanceUnit() const { return m_fDistanceUnit; }

		void SetListenerData( D3DXVECTOR3 pos, D3DXVECTOR3 frontVec = D3DXVECTOR3(0, 0, 1), D3DXVECTOR3 upVec = D3DXVECTOR3( 0, 1, 0) );

#ifdef ADD_CHANGE_BACKGROUND_MUSIC		/// ������ �ν� �Ұ�
		CKTDSMP3Play* GetFirstBGM() const { return m_pMP3Play; }
		void SetFirstBGM(CKTDSMP3Play* val) { m_pMP3Play = val; }

		CKTDSMP3Play* GetSecondBGM() const { return m_pSecondMP3Play; }
		void SetSecondBGM(CKTDSMP3Play* val) { m_pSecondMP3Play = val; }

		bool GetUseSecondBGM() const { return m_bUseSecondBGM; }
		void SetUseSecondBGM(bool val) { m_bUseSecondBGM = val; }

		int GetNowBGMVolume() const { return m_iNowBGMVolume; }
		void SetNowBGMVolume(int val) { m_iNowBGMVolume = val; }
#endif // ADD_CHANGE_BACKGROUND_MUSIC


	private:
		bool					m_bInit;
		bool					m_bSoundMute;
		float					m_fSoundVolume;

		CKTDSMP3Play*			m_pMP3Play;

#ifdef ADD_CHANGE_BACKGROUND_MUSIC		/// ������ �ν� �Ұ�
		CKTDSMP3Play*			m_pSecondMP3Play;		/// �ι�° �������
		bool					m_bUseSecondBGM;		/// ���� ������� ������� ���
		int						m_iNowBGMVolume;		/// ���� �����Ǿ� �ִ� ���� �� ( �ɼ�â )
#endif // ADD_CHANGE_BACKGROUND_MUSIC

		FMOD_SYSTEM*			m_pSystem;
		bool					m_bEnable3DSound;		// �ɼ� ������� ������ 3D sound�� �������
		bool					m_bCapable3DSound;		// �ý��ۿ��� 3D sound�� �����ϴ���
		

		float					m_fDistanceUnit;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

};
