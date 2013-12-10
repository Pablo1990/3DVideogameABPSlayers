#pragma once
#include "Goal_Evaluator.h"

class ExploreGoal_Evaluator : public Goal_Evaluator
{
public:
	ExploreGoal_Evaluator(void);
	~ExploreGoal_Evaluator(void);
		double CalculateDesirability(Bot* bot);
	void SetGoal(Bot* bot);

};

