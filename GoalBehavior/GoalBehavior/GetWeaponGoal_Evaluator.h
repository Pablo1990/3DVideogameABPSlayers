#pragma once
#include "Goal_Evaluator.h"
class GetWeaponGoal_Evaluator:  public Goal_Evaluator
{
public:
	GetWeaponGoal_Evaluator(void);
	~GetWeaponGoal_Evaluator(void);
	double CalculateDesirability(Bot* bot);
	void SetGoal(Bot* bot);
};

