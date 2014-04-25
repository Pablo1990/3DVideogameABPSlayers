#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Bot;

class Goal_GetSalud : public Goal
{
public:
	Goal_GetSalud(Bot *);
	~Goal_GetSalud(void);

	void Activate();
	void AddSubGoal(Goal* g){};
	int Process();
	void Terminate(){estado=completed;}
	Bot* d;

};

