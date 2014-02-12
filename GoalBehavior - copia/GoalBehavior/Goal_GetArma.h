#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Bot;

class Goal_GetArma : public Goal
{
public:
	Goal_GetArma(Bot*);
	~Goal_GetArma(void);
	void Activate();
	void AddSubGoal(Goal* g){};
	int Process();
	void Terminate(){estado=completed;}
	Bot* d;
};

