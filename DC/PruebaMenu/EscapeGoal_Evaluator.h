#pragma once
#include "Goal_Evaluator.h"
class EscapeGoal_Evaluator : public Goal_Evaluator
{
public:
	EscapeGoal_Evaluator(void);
	~EscapeGoal_Evaluator(void);
	double CalculateDesirability(Npc* bot);
	void SetGoal(Npc* bot);
};

