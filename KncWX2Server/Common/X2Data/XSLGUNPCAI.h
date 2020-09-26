#pragma once

class CXSLGUNPCAI
{
public:
	enum TARGET_PRIORITY
	{
		TP_RANDOM,
		TP_ONLY_ONE,
		TP_HIGH_LEVEL_FIRST,
		TP_LOW_LEVEL_FIRST,
		TP_HIGH_HP_FIRST,
		TP_LOW_HP_FIRST,
		TP_NEAR_FIRST,
		TP_FAR_FIRST,
	};

	struct TargetData
	{
		bool				targetNPC;
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;
		int					targetNearRange;	// �� ���� �ȿ� ������ ������ Ÿ���� �ȴ�
		int					targetRange;		// �� ���� �ȿ� ������ Ÿ���� �ȴ�
		int					targetLostRange;	// �� ���� ������ ����� ���� Ÿ���� �Ұ� �ȴ�.
		float				targetSuccessRate;	
		float				attackTargetRate;	// ���� ������ ������ Ÿ������ Ȯ��
		float				preserveLastTargetRate;
	};

	struct ChaseMoveData
	{
		int			moveSplitRange;
		int			destGap;			// ���������� �̰Ÿ� �ȿ� ������ �����ߴٰ� �Ǵ��Ѵ�
		int			moveGap;

		float		dirChangeInterval;

		float		walkInterval;
		float		nearWalkRate;
		float		farWalkRate;

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;
	};

	struct PatrolMoveData
	{
		float		patrolBeginRate;
		int			patrolRange;
		float		patrolCoolingTime;
		bool		bOnlyThisLineGroup;
	};

	struct EscapeMoveData
	{
		int			moveSplitRange;
		int			escapeGap;			// �� �Ÿ� ���� �ָ� ���� ����

		float		walkInterval;
		float		nearWalkRate;
		float		farWalkRate;

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;
	};

	struct EscapeCondition
	{ 
		float		escapeRate;
		int			myHP;
		float		escapeRange;

		EscapeCondition()
		{
			escapeRate		= 0;
			myHP			= 0;
			escapeRange 	= 0.f;			
		}
	};

	struct AIData
	{
		TargetData		targetData;
		ChaseMoveData	chaseMoveData;
		PatrolMoveData	patrolMoveData;
		EscapeMoveData	escapeMoveData;
		EscapeCondition	escapeCondition;
	};

public:
	CXSLGUNPCAI(void);
	~CXSLGUNPCAI(void);

};