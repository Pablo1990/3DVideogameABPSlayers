#pragma once
#include "Goal_Evaluator.h"
class GetWeaponGoal_Evaluator:  public Goal_Evaluator
{
public:
	GetWeaponGoal_Evaluator(int lvl);
	~GetWeaponGoal_Evaluator(void);
	double CalculateDesirability(Npc* bot);
	void SetGoal(Npc* bot);
};

