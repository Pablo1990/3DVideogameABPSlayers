#pragma once
#include "Goal_Think.h"

class Goal_Think;
class Npc;

class Goal_MarcarHuida : public Goal
{
public:
	Goal_MarcarHuida(Npc*);
	~Goal_MarcarHuida(void);
	void Activate();
	void AddSubGoal(Goal* g){};
	int Process();
	void Terminate(){estado=completed;}
	Npc* d;
};

