#pragma once

//////////////////////////////////////////////////////////////////////////
// 2009.12 �¿�
// Particle Editing ����� ���� ����ϴ� Class.
// 
// Particle �Ӽ�(�̺�Ʈ �Ǵ� Emitter �Ӽ�)���� �߰��Ǿ��� �� �߰��ϴ� ��� :
// Emitter�� ��� enum EMITTER_PROPERTIES�� �߰�, Event�� ��� enum CKTDGParticle::EVENT_TYPE�� �߰�
// CX2ViewerParticleEditor�� �����ڿ��� Emitter�� m_aEmitterProperties��, Event�� m_aEventProperties�� ����
// �ش� value type�� ���� Get__Value, Set__Value �Լ� ���� �ش簪�� ���� ó�� �ڵ带 �߰�
//

class CX2ViewerParticleEditor
{
public:

	// ��ƼŬ Property�� ���� value�� Ÿ��.
	enum VALUE_TYPE
	{
		VT_INT,					// int			�Է� Box �ϳ�
		VT_FLOAT,				// float		�Է� Box �ϳ�
		VT_STRING,				// wstring		�Է� Box �ϳ�
		VT_XYZ,					// D3DVECTOR3	�Է� Box ����
		VT_XY,					// D3DVECTOR2	�Է� Box �ΰ�
		VT_RGBA,				// D3DCOLOR		�Է� Box �װ�
		VT_BOOL,				// bool			True/False �޺��ڽ�
		VT_COMBO_CUSTOM,		// ��Ÿ Ư�� enum�� ���� ����ϴ� ��� �Է�â�� ComboBox�� ����ϰ� ���� ��. 
								// ������� X2ViewerParticleEditor.cpp�� Get/SetCustomComboValue �Լ� ����.
	};

	// ParticleEventSequence ��ü�� ���� �Ӽ����� ���� enum.
	enum EMITTER_PROPERTIES
	{
		EP_PARTICLETYPE,
		EP_MESHRENDERTYPE,
		EP_LAYER,
		EP_SRCBLENDMODE,
		EP_DSTBLENDMODE,
		EP_CULLMODE,
		EP_NUMPARTICLES,
		EP_GRAVITY,
		EP_LIFETIME,
		EP_TRIGGERTIME,
		EP_TRIGGERCOUNT,
		EP_EMITRATE,
		EP_EMITRADIUS,
		EP_EMITRADIUSMIN,
		EP_ADDROTATEREL,
		EP_TRACEPARTICLELIST,
		EP_FINALPARTICLELIST,
		EP_BILLBOARDTYPE,
		EP_MESHNAME,
		EP_CHANGETEXNAME,
		EP_MULTITEXXET,
		EP_ANIXET,
		EP_ANINAME,
		EP_LANDPOS,
		EP_USELAND,
		EP_BLACKHOLEPOS,
		EP_LIGHTPOS,
		EP_RESOLUTIONCONVERT,
		EP_TRACE,
		EP_ZENABLE,
		EP_ZWRITEENABLE,
		EP_FORCELAYER,
		EP_DRAWCOUNT,
		EP_SCREW_VALUE,
		EP_SCREW_ROTATESPEED,
		EP_SPHERICAL_EMITRADIUS,
		EP_SPHERICAL_EMITAZIMUTH,
		EP_SPHERICAL_EMITPOLAR,
		EP_SPHERICAL_EMITROTATION,

		EP_END,
	};

	struct PropertyData
	{	
		int m_Type;					// � ���ΰ�? ( EMITTER_PROPERTIES �Ǵ� CKTDGParticle::EVENT_TYPE )
		wstring m_name;				// �ش� �Ӽ����� �̸�
		VALUE_TYPE m_valuetype;		// �ش� �Ӽ����� Type
		bool m_bAllowMinMax;		// �ش� �Ӽ��� random �� ����� ���Ǵ��� ( CMinMax���� )

		void Init( int PropertyType, wstring name, VALUE_TYPE ValueType, bool bAllowMinMax )
		{
			m_Type = PropertyType;
			m_name = name;
			m_valuetype = ValueType;
			m_bAllowMinMax = bAllowMinMax;
		}
	};

	CX2ViewerParticleEditor( /* UI �޾Ƶ��� */ );
	virtual ~CX2ViewerParticleEditor(void);

	PropertyData*		GetEmitterProperties( EMITTER_PROPERTIES prop ){ return &(m_aEmitterProperties[prop]); }
	PropertyData*		GetEventProperties( CKTDGParticleSystem::EVENT_TYPE prop ){ return &(m_aEventProperties[prop]); }

	// ��ũ��Ʈ ����� ���� function
	bool				ExportParticleScript( CKTDGParticleSystem* pSystem, const WCHAR* pwszFileFullPath );
	wstring				MakeParticleEventSequenceString( CKTDGParticleSystem::CParticleEventSequence* pSeq );
	wstring				MakeParticleEventString( CKTDGParticleSystem::CParticleEvent* pEvent, const float FinalTime );

	// UI ������� ���� Utility function
	template <class T>
		wstring GetMinMaxString( T valmin, T valmax );
	wstring GetMinMaxString( CMinMax<float> valfloat );
	wstring GetMinMaxString( CMinMax<D3DXVECTOR3> valV3 );
	wstring GetMinMaxString( CMinMax<D3DXVECTOR2> valV2 );

	// Particle Event�� �ð� ������ ��ũ��Ʈ�� ����Ҷ��� ����
	wstring GetEventTimeString( CKTDGParticleSystem::CParticleEvent* pEvent, float Finaltime );

	CKTDGParticleSystem::CParticleEvent* EventFactory( CKTDGParticleSystem::EVENT_TYPE eventtype );

	CMinMax<float> ParseCMinMaxFloat( wstring str );
	CMinMax<D3DXVECTOR2> ParseCMinMaxD3DVECTOR2( wstring strX, wstring strY );
	CMinMax<D3DXVECTOR3> ParseCMinMaxD3DVECTOR3( wstring strX, wstring strY, wstring strZ );
	
	//////////////////////////////////////////////////////////////////////////
	// �� Value Type�� ���� Get/Set Method ( Emitter �� )
	
	CMinMax<D3DXVECTOR3> GetXYZValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	CMinMax<D3DXVECTOR2> GetXYValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	CMinMax<float> GetFloatValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	int	GetIntValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	wstring GetStringValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	bool GetBoolValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	void GetCustomComboBoxValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CDXUTComboBox* pCombo );

	void SetXYZValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<D3DXVECTOR3> val );
	void SetXYValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<D3DXVECTOR2> val );
	void SetFloatValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<float> val );
	void SetIntValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, int val );
	void SetStringValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, wstring val );
	void SetBoolValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, bool val );
	void SetCustomComboBoxValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, DXUTComboBoxItem* pItem );

	//////////////////////////////////////////////////////////////////////////
	// �� Value Type�� ���� Get/Set Method ( Event �� )
	CMinMax<D3DXVECTOR3> GetXYZValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	CMinMax<D3DXVECTOR2> GetXYValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	CMinMax<float> GetFloatValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	wstring GetStringValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	CMinMax<D3DXCOLOR> GetRGBAValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );

	void SetXYZValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXVECTOR3> val );
	void SetXYValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXVECTOR2> val );
	void SetFloatValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<float> val );
	void SetStringValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, wstring val );
	void SetRGBAValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXCOLOR> val );

private:

	PropertyData	m_aEmitterProperties[EP_END];
	PropertyData	m_aEventProperties[CKTDGParticleSystem::ET_END];


};
