#pragma once
#include "Bot.h"
class Bot;

class Goal_Evaluator
{
public:
	Goal_Evaluator(void);
	~Goal_Evaluator(void);
	virtual double CalculateDesirability(Bot* bot)=0;
	virtual void SetGoal(Bot* bot)=0;
};

