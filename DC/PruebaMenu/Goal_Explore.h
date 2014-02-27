#pragma once

#include "Goal_Think.h"

class Goal_Think;
class Npc;

class Goal_Explore : public Goal_Composite
{
public:
	Goal_Explore(Npc* b);
	~Goal_Explore(void);
	void Activate();

  int Process();

  void Terminate(){estado=completed;}
	Npc* d;
};
