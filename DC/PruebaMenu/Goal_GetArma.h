#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Npc;

class Goal_GetArma : public Goal
{
public:
	Goal_GetArma(Npc*);
	~Goal_GetArma(void);
	void Activate();
	void AddSubGoal(Goal* g){};
	int Process();
	void Terminate(){estado=completed;}
	Npc* d;
};

