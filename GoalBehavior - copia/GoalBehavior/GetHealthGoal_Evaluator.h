#pragma once
#include "Goal_Evaluator.h";


class GetHealthGoal_Evaluator: public Goal_Evaluator
{
public:
	GetHealthGoal_Evaluator(void);
	~GetHealthGoal_Evaluator(void);
	double CalculateDesirability(Bot* bot);
	void SetGoal(Bot* bot);
};

