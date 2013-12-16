#pragma once
#include "Goal_Evaluator.h"

class AttackTargetGoal_Evaluator : public Goal_Evaluator
{
public:
	AttackTargetGoal_Evaluator(void);
	~AttackTargetGoal_Evaluator(void);
	double CalculateDesirability(Bot* bot);
	void SetGoal(Bot* bot);
};

