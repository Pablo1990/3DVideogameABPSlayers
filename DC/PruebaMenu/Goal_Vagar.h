#pragma once

#include "Goal_Think.h"

class Goal_Think;
class Npc;

class Goal_Vagar : public Goal
{
public:
	Goal_Vagar(Npc*);
	~Goal_Vagar(void);
	void Activate();
	void AddSubGoal(Goal* g){};
	int Process();
	void Terminate(){estado=completed;}
	Npc* d;
};

