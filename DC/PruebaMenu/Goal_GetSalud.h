#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Npc;

class Goal_GetSalud : public Goal
{
public:
	Goal_GetSalud(Npc *);
	~Goal_GetSalud(void);

	void Activate();
	void AddSubGoal(Goal* g){};
	int Process();
	void Terminate(){estado=completed;}
	Npc* d;

};

